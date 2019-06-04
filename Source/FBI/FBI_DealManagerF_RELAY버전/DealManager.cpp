
#include "DealManager.h"
#include "../../IRUM_UTIL/LogMsg.h"
extern BOOL		g_bDebugLog;
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../common/FBIInc.h"
#include <memory.h>


extern BOOL		g_bContinue;	// flag whether continue process or not
extern CLogMsg	g_log;
extern char		g_zConfig[_MAX_PATH];
extern CMemPool	g_memPool;


CDealManager::CDealManager(char* pzStkCd, char* pzArtcCd, int nIdx) : CBaseThread(pzStkCd)
{
	strcpy(m_zStkCd, pzStkCd);
	strcpy(m_zArtcCd, pzArtcCd);
	m_pClient = NULL;
	m_chartShm = NULL;
	m_nIdx = nIdx;
	//m_nLoggingCnt = 0;
	//ZeroMemory(m_zNextCandleTm, sizeof(m_zNextCandleTm));
}


CDealManager::~CDealManager()
{
	Finalize();
}



BOOL CDealManager::Initialize()
{
	InitializeCriticalSection(&m_csDeal);

	// 결과산출을 위한 대기 시간
	char waitSecs[32] = { 0, };
	CUtil::GetConfig(g_zConfig, "TIME", "RESULT_WAIT_SEC", waitSecs);
	m_nResultWaitSec = atoi(waitSecs) * 1000;
	if (m_nResultWaitSec <= 0)
		m_nResultWaitSec = 3000;

	char ip[32], id[32], pwd[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);

	if (!m_pDBPool)
	{
		m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
		if (!m_pDBPool->Init(3))
		{
			g_log.log( ERR/*NOTIFY*/, "(%s)Thread DB OPEN FAILED.(%s)(%s)(%s)", m_zStkCd, ip, id, pwd);
			return FALSE;
		}
	}

	//if (!InitClientConnect())	return FALSE;

	if (!InitChartShm())
		return FALSE;

//	if (!LoadDealInfo())	return FALSE;

	ResumeThread();
	
	m_hRsltProc = (HANDLE)_beginthreadex(NULL, 0, &Thread_ResultProcByChart, this, 0, &m_unRsltProc);

	if (IsTimesaveClass()) {
		m_hTimeSave = (HANDLE)_beginthreadex(NULL, 0, &Thread_TimeSave, this, 0, &m_unTimeSave);
		m_bTimeSaveRun = TRUE;
	}
	

	return TRUE;
}

BOOL CDealManager::InitChartShm()
{	
	char zChartNmTp[32];
	CUtil::GetConfig(g_zConfig, "CHARTNAME_TYPE", "TYPE", zChartNmTp);

	m_chartShm = new CChartShmUtilFixed(atoi(zChartNmTp));
	if (!m_chartShm->OpenChart(m_zArtcCd))
	{
		g_log.log(LOGTP_ERR, "%s", m_chartShm->getmsg());
		return FALSE;
	}

	g_log.log(INFO, "%s Chart Shm is opened", m_zArtcCd);

	return TRUE;
}


VOID CDealManager::Finalize()
{
	m_bTimeSaveRun = FALSE;

	SAFE_DELETE(m_pDBPool);

	std::map<int, _FBI::ST_DEAL_INFO*>::iterator it;
	EnterCriticalSection(&m_csDeal);
	for (it = m_mapDeal.begin(); it != m_mapDeal.end(); ++it)
	{
		_FBI::ST_DEAL_INFO* s = (*it).second;
		delete s;
	}
	m_mapDeal.clear();
	DeleteCriticalSection(&m_csDeal);

	SAFE_DELETE(m_chartShm);
}

BOOL CDealManager::LoadDealInfo()
{
	char zQ[1024];
	char zOrderTime[128], zEndTm[32];
	char zTradeDt[32],zNextDt[32], zNowTmFull[32], zOrdTmFull[32];
	//char temp[32];
	int nDealDateTp;
	SYSTEMTIME st;
	GetLocalTime(&st);

	CDBHandlerAdo db(m_pDBPool->Get());
	
	sprintf(zQ, "EXEC AA_GET_DEAL_INFO '%s', 0 ", m_zArtcCd);
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load DealInfo Error(%s)", zQ);
		return FALSE;
	}

	
	while (db->IsNextRow())
	{
		// hh:mm:ss
		if (db->GetStrWithLen("TM_ORD_START", 10, zOrderTime) == NULL)
		{
			g_log.log(ERR, "LoadDealInfo TM_ORD_START Error(%s)", db->GetError());
			return FALSE;
		}

		if (db->GetStrWithLen("TM_END", 10, zEndTm) == NULL)
		{
			g_log.log(ERR, "LoadDealInfo TM_END Error(%s)", db->GetError());
			return FALSE;
		}
		if (db->GetStrWithLen("TRADE_DT", 10, zTradeDt) == NULL)
		{
			g_log.log(ERR, "LoadDealInfo TRADE_DT Error(%s)", db->GetError());
			return FALSE;
		}
		if (db->GetStrWithLen("NEXT_DT", 10, zNextDt) == NULL)
		{
			g_log.log(ERR, "LoadDealInfo NEXT_DT Error(%s)", db->GetError());
			return FALSE;
		}

		nDealDateTp = db->GetLong("DATE_TP");

		// yyyymmddhh:mm:ss
		sprintf(zNowTmFull, "%04d%02d%02d%02d:%02d:%02d",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		// 주문시작시간이 이미 지난건 버린다.
		if (nDealDateTp == _FBI::DATETP_TRADE)
			sprintf(zOrdTmFull, "%s%s", zTradeDt, zOrderTime);
		else
			sprintf(zOrdTmFull, "%s%s", zNextDt, zOrderTime);

		if (strcmp(zOrdTmFull, zNowTmFull)<0)
		{
			//g_log.log(INFO, "[%s]Deal의 주문시간(%s)이 현재시간(%s)보다 이전이므로 버린다.",
			//	m_zArtcCd, zOrdTmFull, zNowTmFull);

			db->Next();
			continue;
		}



		_FBI::ST_DEAL_INFO* pInfo = new _FBI::ST_DEAL_INFO;
		ZeroMemory(pInfo, sizeof(_FBI::ST_DEAL_INFO));
		pInfo->DealSeq = db->GetLong("DEAL_SEQ");
		//db->GetStr("TM_ORD_START", pInfo->tm_order);		// hh:mm:ss
		db->GetStr("ARTC_CD", pInfo->ArtcCd);
		strcpy(pInfo->tm_order, zOrderTime);
		db->GetStr("TM_WAIT_START", pInfo->tm_wait);
		db->GetStr("TM_CHARTWAIT_START", pInfo->tm_chartwait);
		db->GetStr("TM_END", pInfo->tm_end);
		db->GetStr("TM_CHART", pInfo->tm_chart);
		pInfo->DealStatus = (_FBI::EN_DEAL_SATTUS)db->GetLong("DEAL_STATUS");
		pInfo->DurationMin = db->GetLong("DURATION");
		pInfo->DateTp = nDealDateTp;
		if (nDealDateTp == _FBI::DATETP_TRADE)
			strcpy(pInfo->Date, zTradeDt);
		else
			strcpy(pInfo->Date, zNextDt);

		//g_log.log(INFO, "[SEQ:%d](%s)(%s)(%s)(%s)(%s)", 
		//	pInfo->DealSeq,pInfo->Date, pInfo->tm_order, pInfo->tm_wait, pInfo->tm_chartwait, pInfo->tm_chart);
		EnterCriticalSection(&m_csDeal);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		LeaveCriticalSection(&m_csDeal);

		db->Next();
	}
	db->Close();
	g_log.log(INFO, "[ 3M]Succeeded in DealInfo[%s][Count:%d]", m_zArtcCd, m_mapDeal.size());
	return TRUE;
}





VOID CDealManager::ThreadFunc()
{
	// Terminate this proces
	if (!LoadDealInfo()) {
		g_log.log(NOTIFY, "LoadDealInfo 오류로 프로세스 Terminate!");
		exit(0);
		return;
	}

	while (!InitClientConnect() )
	{
		if (!m_bContinue)
			return;

		Sleep(3000);
	}

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, _FBI::TIMEOUT_CHECK_DEAL, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			g_log.log(LOGTP_ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
			continue;
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			DealManage();
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case _FBI::WM_RECV_API_CHART:
				//m_chartMap.Save((char*)msg.lParam);
				g_memPool.release((char*)msg.lParam);
				break;
			case _FBI::WM_TERMINATE:
				//TODO
				return;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
}

/*
	chart 데이터를 이용한 체결처리
	
	typedef struct _ST_SHM_CHART_UNIT
	{
		char	Nm[LEN_CHART_NM];		//8,8 KEY. chart name. hhmm		0000, 0005, 0010
		char	prevNm[LEN_CHART_NM];	//8,16 previous EY
		char	gb[1];					//4,17 +:양봉(plus candel), -:음봉(minus candle)
		char	open[LEN_PRC];			//
		char	high[LEN_PRC];			//
		char	low[LEN_PRC];			//
		char	close[LEN_PRC];			//
		char	cntr_qty[LEN_QTY];
		char	dotcnt[5];				//5,100
										//char	seq[LEN_SEQNO];			//10,110 시간저장한다.
		char	sma_short[LEN_PRC];		//20,130 단기short term SMA(Simple Moving Average). 10
		char	sma_long[LEN_PRC];		//20,150 장기long term SMA(Simple Moving Average). 20
		char	sma_shortest[LEN_PRC];	//20,170 단기short term SMA(Simple Moving Average). 5
		char	Reserved[50];
	
	}ST_SHM_CHART_UNIT;	// 
				
*/
unsigned WINAPI CDealManager::Thread_ResultProcByChart(LPVOID lp)
{
	CDealManager* pThis = (CDealManager*)lp;

	// Deal 정보
	int nDealSeq;
	char zChartNm[32];
	char zClientSendBuf[1024] = { 0, };
	char z[128];

	_FBI::PT_DEAL_STATUS *cPack = (_FBI::PT_DEAL_STATUS *)zClientSendBuf;
	int nCPackLen = sizeof(_FBI::PT_DEAL_STATUS);
	
	ST_SHM_CHART_UNIT	chartData;
	MSG msg;
	int nComp;
	char UpDown[2] = { 0 , };
	char zOpen[32], zClose[32];
	_FBI::ST_DEAL_INFO recvData;
	while(GetMessage(&msg, NULL, 0,0))
	{
		if (msg.message == _FBI::WM_TERMINATE)
			break;

		if (msg.message != _FBI::WM_RESULT_START)
			continue;

		// Have enough time to get the lastest chart data
		Sleep(pThis->m_nResultWaitSec);

		memcpy(&recvData, (_FBI::ST_DEAL_INFO*)msg.lParam, sizeof(_FBI::ST_DEAL_INFO));
		_FBI::ST_DEAL_INFO * pDeal = (_FBI::ST_DEAL_INFO *)&recvData;	// msg.lParam;
		g_memPool.release((char*)msg.lParam);

		nDealSeq = pDeal->DealSeq;
		pThis->m_chartShm->ComposeChartName(pDeal->Date, pDeal->tm_end, TP_1MIN, zChartNm);

		// CHART 를 가져온다.
		ZeroMemory(&chartData, sizeof(chartData));
		BOOL bRet=FALSE;
		int nRetriedCnt = 0;

		int nSleep = 100;
		int nTotRetrySec = 10;
		int nTotRetryCnt = nTotRetrySec * 1000 / nSleep;
		for (nRetriedCnt = 0; nRetriedCnt < nTotRetryCnt; nRetriedCnt++)	// 10초.
		{
			bRet = pThis->m_chartShm->GetChartData(pThis->m_zStkCd, TP_1MIN, zChartNm, chartData);
			if (bRet)
				break;
			if (nRetriedCnt >= 10)	// 1초 
			{
				if (strncmp(pThis->m_zArtcCd, "HSI", 3) != 0 && strncmp(pThis->m_zArtcCd, "SCN", 3) != 0)
				{
					g_log.log(ERR, "[%s](END_TM:%s)(CHART_TM:%s)Get Chartdata Error(%s)"
						, pThis->m_zStkCd, pDeal->tm_end, zChartNm, pThis->m_chartShm->getmsg());
				}
			}
			Sleep(100);
		}
		if (!bRet)
		{
			if (strncmp(pThis->m_zArtcCd, "HSI", 3) != 0 && strncmp(pThis->m_zArtcCd, "SCN", 3) != 0)
			{
				g_log.log(NOTIFY, "[%s](END_TM:%s)(CHART_TM:%s)(%s)There is no chart.Resulting Failed!!!"
					, pThis->m_zStkCd, pDeal->tm_end, zChartNm);
			}
			*UpDown = 'E';
			strcpy(zOpen, "0");
			strcpy(zClose, "0");
		}
		else
		{
			if (nRetriedCnt > 0) {
				g_log.log(INFO, "[RETRY:%d][%s](END_TM:%s)(CHART_TM:%s)Get Chartdata Ok(%s)"
					, nRetriedCnt, pThis->m_zStkCd, pDeal->tm_end, zChartNm);
			}
			// Deal 정보와 비교한다.
			nComp = strncmp(chartData.open, chartData.close, sizeof(chartData.open));
			if (nComp > 0)			*UpDown = 'D';
			else if (nComp == 0)	*UpDown = 'E';
			else if (nComp < 0)		*UpDown = 'U';
			sprintf(zOpen, "%.*s", sizeof(chartData.open), chartData.open);
			sprintf(zClose, "%.*s", sizeof(chartData.close), chartData.close);
		}

		//DB UPDATE
		for (int i = 0; i < 3; i++)
		{
			CDBHandlerAdo db(pThis->m_pDBPool->Get());
			char zQ[1024];

			sprintf(zQ, "EXEC AA_ORD_RESULT "
				"'%s'"
				",%d"
				",%d"
				",'%c'"
				",'%s'"
				",'%s'"
				",'%s'"
				, pThis->m_zStkCd
				, nDealSeq
				, TP_1MIN
				, UpDown[0]
				, zOpen
				, zClose
				, zChartNm
			);

			//if (g_bDebugLog) {
			//	g_log.log(INFO, "[Result](%s)", zQ);
			//}
			if (FALSE == db->ExecQuery(zQ))
			{
				g_log.log(ERR/*NOTIFY*/, "Result Error(AA_ORD_RESULT) error(%s)", zQ);
				Sleep(1000);
				db->Close();
				continue;
			}
			else
			{
				//if (g_bDebugLog) {
				//	g_log.log(INFO, "[주문처리 성공][%s][SEQ:%d][CHART_NM:%s][OPEN:%.10s][CLOSE:%.10s]",
				//		pThis->m_zArtcCd, nDealSeq, zChartNm, chartData.open, chartData.close);
				//}

				// send to Client
				memset(cPack, 0x20, nCPackLen);
				cPack->STX[0] = _FBI::STX;

				char len[32];
				sprintf(len, "%d", sizeof(_FBI::PT_DEAL_STATUS));
				memcpy(cPack->Len, len, strlen(len));
				
				cPack->DealTp[0] = _FBI::DEAL_TP_ONE_OP_C;

				memcpy(cPack->ArtcCd, pThis->m_zArtcCd, min(sizeof(cPack->ArtcCd), strlen(pThis->m_zArtcCd)));
				memcpy(cPack->StkCd, pThis->m_zStkCd, min(sizeof(cPack->StkCd), strlen(pThis->m_zStkCd)));
				sprintf(z, "%d", nDealSeq);
				memcpy(cPack->DealSeq, z, strlen(z));
				memcpy(cPack->DealStatus, "05", 2);
				//cPack->DealStatus[0] = '5';
				cPack->OrdResult[0] = *UpDown;
				memcpy(cPack->Time, pDeal->tm_end, sizeof(cPack->Time));

				// yyyymmddhhmm => hh:mm
				//sprintf(pThis->m_zNextCandleTm, "%.2s:%.2s", zChartNm + 8, zChartNm + 10);
				//sprintf(cPack->CandleTime, "%.2s:%.2s", zChartNm + 8, zChartNm + 10);
				memcpy(cPack->CandleTime, pDeal->tm_chart, min(sizeof(cPack->CandleTime),sizeof(pDeal->tm_chart)));
				
				cPack->ETX[0] = _FBI::ETX;
				*(cPack->ETX + 1) = 0x00;

				pThis->SendToClient(cPack, 0);

				// delete DealInfo
				pThis->DealErase(nDealSeq);
				break;
			} // else of if (FALSE == db->ExecQuery(zQ))
		} // for (int i = 0; i < 3; i++) for DB

		//g_memPool.release((char*)pDeal);

	} // while(GetMessage(&msg, NULL, 0,0))

	return 0;
}

// 1초마다 DB UPDATE
unsigned WINAPI CDealManager::Thread_TimeSave(LPVOID lp)
{
	CDealManager* p = (CDealManager*)lp;
	int nCnt = 0;
	BOOL bStart = FALSE;
	char zQ[1024];

	while (p->m_bTimeSaveRun)
	{
		Sleep(100);

		if (++nCnt == 5 && bStart)	// 500 ms 마다 호출하기 위해
		{
			//	DB UPDATE
			CDBHandlerAdo db(p->m_pDBPool->Get());
			sprintf(zQ, "AA_UPDATE_ORD_TM");
			if (FALSE == db->ExecQuery(zQ))
			{
				g_log.log(ERR/*NOTIFY*/, "Update Curr Time error(%s)", zQ);
				Sleep(1000);
				db->Close();
				continue;
			}
			//else
			//	g_log.log(INFO, "call AA_UPDATE_ORD_TM");

			nCnt = 0;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case _FBI::WM_DEAL_STATUS:
				//g_log.log(INFO, "Peek Message to time Save Thread");
				bStart = TRUE;
				nCnt = 0;
				break;
			}
		}
	}
	return 0;
}

VOID CDealManager::DealManage()
{
	EnterCriticalSection(&m_csDeal);
	DealManageInner();
	LeaveCriticalSection(&m_csDeal);
}

/*
	현재시간과 MAP 의 가장처음 DEAL STATUS 를 비교해서 
	UPDATE DB
	 CLIENT

	enum EN_DEAL_SATTUS {
		DEAL_STATUS_NONSTART = 0//	미시작
		, DEAL_STATUS_ORD		//	주문가능 1
		, DEAL_STATUS_WAIT		//	대기 2
		, DEAL_STATUS_CHARTWAIT	//	차트대기 3
		, DEAL_STATUS_RESULTING	//	결과산출 4
		, DEAL_STATUS_DONE		//	완료 5
	};

*/
BOOL CDealManager::DealManageInner()
{
	char zNowFull[32] = { 0, }; 
	char zCompareFull[32];
	SYSTEMTIME st;
	int nCompareLen = 0;

	// 최대 2개의 deal 이 진행중이다.
	// 결과 처리 / deal 진행
	int nDealSeq;
	std::string sOrdTm, sRsltTm;
	_FBI::ST_DEAL_INFO* pInfo;
	std::map<int, _FBI::ST_DEAL_INFO*>::iterator it;



	// DEAL MAP 에서 현재 시간보다 주문시간이 작은 DEAL 을 꺼낸다. 단, 이미 결과진행중인건 놔둔다.
	// 상태가 이미 완료된건 지운다.

	BOOL bMatched = FALSE;
	for (it = m_mapDeal.begin(); it != m_mapDeal.end(); )
	{
		sOrdTm.clear();
		pInfo = NULL;
		nDealSeq = (*it).first;
		pInfo = (*it).second;

		GetLocalTime(&st);
		sprintf(zNowFull, "%04d%02d%02d%02d:%02d:%02d", 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		nCompareLen = strlen(zNowFull)-3;	// HH:MM 까지만 비교 		

		// 결과시간 지났는데 아직 미시작인건 삭제한다.
		sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_end);
		if (strncmp(zCompareFull, zNowFull, nCompareLen) <= 0)
		{
			if (pInfo->DealStatus == _FBI::DEAL_STATUS_NONSTART)
			{
				it = m_mapDeal.erase(it);
				delete pInfo;
				continue;
			}
		}

		// 결과진행중인건 skip 한다.
		if (pInfo->DealStatus == _FBI::DEAL_STATUS_RESULTING)
		{
			it++;
			continue;
		}

		// 주문시간 <= 지금시간 ==> 점검한다.
		sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_order);
		if (strncmp(zCompareFull, zNowFull, nCompareLen) <= 0)
		{
			bMatched = TRUE;
			break;
		}
		// 주문시간 > 지금시간 ==> 점검 불필요
		else
		{
			break;
		}
	}

	if (!bMatched)	return FALSE;	//----------------------------------

	char status[32];

	// 이미 처리가 된 DEAL 이면 MAP 에서 제거
	// ==> 결과처리 후 거기서 삭제를 하기 때문에 이 코드는 실행 안될 것이다.
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_DONE)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]이미 완료된 DEAL 이라서 MAP 에서 제거",
			pInfo->ArtcCd, zNowFull, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		
		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo; 
		return TRUE;
	}
	
	// yyyymmddhh:mm:ss 까지 비교한다.
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_end);
	nCompareLen = strlen(zCompareFull);

	// 결과대기시간 과 비교. 지금이 결과대기시간을 지났으면 점검.( 결과대기시간 < 지금 )
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		if (DealResulting(zNowFull, pInfo))
		{
			char* pDeal = g_memPool.get();
			memcpy(pDeal, pInfo, sizeof(_FBI::ST_DEAL_INFO));
			PostThreadMessage(m_unRsltProc, _FBI::WM_RESULT_START, 0, (LPARAM)pDeal);
		}
		return TRUE;
	} 

	// 차트대기시간 과 비교. 지금이 차트대기시간을 지났으면 점검.( 차트대기시간 < 지금 )
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_chartwait);
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		return DealChartWait(zNowFull, pInfo);
	} // if (strncmp(zNow, pInfo->tm_result, nLen) >= 0)

	// 대기시간과 과 비교. 지금이 대기시간을 지났으면 점검.( 대기시간 < 지금 )
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_wait);
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		return DealWait(zNowFull, pInfo);
	}

	// 주문시간과 과 비교. 지금이 주문시간을 지났으면 점검.( 주문시간 < 지금 )
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_order);
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		return DealOrd(zNowFull, pInfo);
	}

	return FALSE;
}


BOOL CDealManager::DealOrd(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD)
	{
		return FALSE;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_NONSTART)
	{
		//if (g_bDebugLog) {
		//	g_log.log(INFO, "(%s)(SEQ:%d)/주문/(현재:%s)(DEAL주문시간:%s%s) <주문>으로 UPDATE",
		//		pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_order);
		//}
		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_ORD;
		UpdateDealStatus(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;

		if (IsTimesaveClass()) {
			PostThreadMessage(m_unTimeSave, _FBI::WM_DEAL_STATUS, 0, 0);
			//g_log.log(INFO, "Post Message to time Save Thread");
		}

		return TRUE;
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(SEQ:%d)/(현재:%s)(DEAL주문시간:%s%s)(상태:%s)]주문시간 지났는데 상태가 <미시작>이 아니다. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_order, _FBI::dealstatus(pInfo->DealStatus, status));
		
		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo;
		return FALSE;
	}
}


BOOL CDealManager::DealWait(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT)
	{
		return FALSE;;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD)
	{
		//if (g_bDebugLog) {
		//	g_log.log(INFO, "(%s)(SEQ:%d)/(현재:%s)(DEAL대기시간:%s%s) <대기>로 UPDATE",
		//		pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_wait, _FBI::dealstatus(pInfo->DealStatus, status));
		//}

		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_WAIT;
		UpdateDealStatus(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		return TRUE;
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(SEQ:%d)/대기/(현재:%s)(DEAL대기시간:%s%s)(상태:%s)]대기시간 지났는데 상태가 <주문>이 아니다. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_wait, _FBI::dealstatus(pInfo->DealStatus, status));

		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo;
		return FALSE;
	}
}


BOOL CDealManager::DealChartWait(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_CHARTWAIT)
	{
		return FALSE;;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT)
	{
		// update.
		//if (g_bDebugLog) {
		//	g_log.log(INFO, "(%s)(SEQ:%d)/(현재:%s)(DEAL차트시간:%s%s)(상태:%s)] <차트>로 UPDATE",
		//		pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_chartwait);
		//}

		pInfo->DealStatus = _FBI::DEAL_STATUS_CHARTWAIT;
		UpdateDealStatus(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		
		return TRUE;
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(SEQ:%d)/차트/(현재:%s)(DEAL차트시간:%s%s)(상태:%s)]차트시간 지났는데 상태가 <대기>가 아니다. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_chartwait, _FBI::dealstatus(pInfo->DealStatus, status));

		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo;
		return FALSE;
	}
}


// 
BOOL CDealManager::DealResulting(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_RESULTING)
	{
		return FALSE;;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_CHARTWAIT)
	{
		// update.
		//if (g_bDebugLog) {
		//	g_log.log(INFO, "(%s)(SEQ:%d)/(현재:%s)(DEAL결과시간:%s%s) <결과>로 UPDATE",
		//		pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_end);
		//}

		pInfo->DealStatus = _FBI::DEAL_STATUS_RESULTING;
		UpdateDealStatus(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		return TRUE;
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)/결과/(SEQ:%d)(현재:%s)(DEAL결과시간:%s%s)(상태:%s)]결과시간 지났는데 상태가 <차트>가 아니다. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNowFull,pInfo->Date, pInfo->tm_end, _FBI::dealstatus(pInfo->DealStatus, status));

		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo; 
		return FALSE;
	}
}

VOID CDealManager::DealErase(int nDealSeq)
{
	EnterCriticalSection(&m_csDeal);
	m_mapDeal.erase(nDealSeq);
	LeaveCriticalSection(&m_csDeal);
}


/*
struct ST_DEAL_INFO
{
char	Symbol[LEN_SYMBOL];
int		DealSeq;
char	SysDt[8];
char	tm_order[8];	//hh:mm:ss
char	tm_wait[8];		//hh:mm:ss
char	tm_result[8];	//hh:mm:ss
int		DealStatus;
int		DurationMin;
};

struct ST_DEAL_STATUS
{
char Symbol[LEN_SYMBOL];
char DealSeq[LEN_DEAL_SEQ];
char DealStatus[2];
char Time[8];				// 각 status 시작시간
};
*/
void CDealManager::UpdateDealStatus(_FBI::ST_DEAL_INFO* pInfo)
{
	// send to the client
	char z[128];
	char packet[1024] = { 0, };
	_FBI::PT_DEAL_STATUS* p = (_FBI::PT_DEAL_STATUS*)packet;
	memset(p, 0x20, sizeof(_FBI::PT_DEAL_STATUS));

	p->STX[0] = _FBI::STX;

	char len[32];
	sprintf(len, "%d", sizeof(_FBI::PT_DEAL_STATUS));
	memcpy(p->Len, len, strlen(len));
	p->DealTp[0] = _FBI::DEAL_TP_ONE_OP_C;
	memcpy(p->ArtcCd, m_zStkCd, min(strlen(m_zArtcCd),_FBI::FBILEN_SYMBOL));
	memcpy(p->StkCd, m_zStkCd, min(strlen(m_zStkCd), _FBI::FBILEN_SYMBOL));


	sprintf(z, "%0*d", _FBI::FBILEN_DEAL_SEQ, pInfo->DealSeq);
	memcpy(p->DealSeq, z, _FBI::FBILEN_DEAL_SEQ);

	sprintf(z, "%02d", pInfo->DealStatus);
	memcpy(p->DealStatus, z, sizeof(p->DealStatus));

	p->ETX[0] = _FBI::ETX;
	*(p->ETX + 1) = 0x00;

	switch (pInfo->DealStatus)
	{
	case _FBI::DEAL_STATUS_ORD:
		sprintf(p->Time, pInfo->tm_order);
		break;
	case _FBI::DEAL_STATUS_WAIT:
		sprintf(p->Time, pInfo->tm_wait);
		break;
	case _FBI::DEAL_STATUS_CHARTWAIT:
		sprintf(p->Time, pInfo->tm_chartwait);
		break;
	case _FBI::DEAL_STATUS_RESULTING:
		sprintf(p->Time, pInfo->tm_end);
		break;
	default:
		g_log.log(ERR/*NOTIFY*/, "[%s](Status:%s)Update Error", pInfo->ArtcCd, _FBI::dealstatus(pInfo->DealStatus, z));
		return;
	}
	memcpy(p->CandleTime, pInfo->tm_chart, min(sizeof(pInfo->tm_chart),sizeof(p->CandleTime)));

	//DB UPDATE
	for (int i = 0; i < 3; i++)
	{
		CDBHandlerAdo db(m_pDBPool->Get());
		char zQ[1024];

		sprintf(zQ, "EXEC AA_UPDATE_DEAL_STATUS %d, %d, '%s', 0"
			, pInfo->DealStatus
			, pInfo->DealSeq
			, m_zArtcCd
		);
		if (FALSE == db->ExecQuery(zQ))
		{
			g_log.log(ERR/*NOTIFY*/, "Update DEAL_MST error(%s)", zQ);
			Sleep(1000);
			db->Close();
			continue;
		}
		else
		{
			//if (g_bDebugLog) {
			//	g_log.log(INFO, "Update DB DEAL_MST OK[%s][SEQ:%d][%s]", m_zArtcCd, pInfo->DealSeq, _FBI::dealstatus(pInfo->DealStatus, z));
			//}
			break;
		}
	}

	// Result is sent after complete the result
	if(pInfo->DealStatus!= _FBI::DEAL_STATUS_RESULTING)
		SendToClient(p, 0);
}


BOOL CDealManager::SendToClient(_FBI::PT_DEAL_STATUS* pPacket, int nRecurCnt)
{
	if (nRecurCnt > 10)
		return FALSE;

	int nLen = sizeof(_FBI::PT_DEAL_STATUS);
	char zSendBuffer[1024] = { 0, };
	memcpy(zSendBuffer, (char*)pPacket, nLen);
	zSendBuffer[nLen++] = 0x0a;

	int nErrCode;
	int res = m_pClient->SendData(zSendBuffer, nLen, &nErrCode);
	if (res <= 0)
	{
		g_log.log(ERR, "SendToClient Error(%s)", m_pClient->GetMsg());
		Sleep(1000);
		if (InitClientConnect())
		{
			nRecurCnt++;
			return SendToClient(pPacket, nRecurCnt);
		}
		return FALSE;
	}
	nRecurCnt = 0;

	//if (g_bDebugLog) 
	{
		if (strncmp(pPacket->ArtcCd, "CL", 2) == 0) {
			g_log.log(INFO, "[3M]Send To Client(%.*s)", nLen, zSendBuffer);
		}
	}
	return TRUE;
}



BOOL CDealManager::InitClientConnect()
{
	if (m_pClient)
		delete m_pClient;

	m_pClient = new CTcpClient("DealManager");
	char zIP[32], zPort[32];
	CUtil::GetConfig(g_zConfig, "DEAL_UPDATE", "IP", zIP);
	CUtil::GetConfig(g_zConfig, "DEAL_UPDATE", "PORT", zPort);

	if (!m_pClient->Begin(zIP, atoi(zPort), 10))
	{
		g_log.log(ERR, "[Connect To ClientAgent Error]%s", m_pClient->GetMsg());
		return FALSE;
	}
	else {
		g_log.log(LOGTP_SUCC, "TCP CLIENT Initialize and connect OK(IP:%s)(PORT:%s)", zIP, zPort);
	}


	return TRUE;
}









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChartMap
//////////////////////////////////////////////////////////////////////////////////////////////
//CChartMap::CChartMap()
//{
//	InitializeCriticalSection(&m_csChart);
//}
//
//CChartMap::~CChartMap()
//{
//	EnterCriticalSection(&m_csChart);
//	std::map<std::string, _FBI::PT_API_CHART*>::iterator it2;
//	for (it2 = m_mapChart.begin(); it2 != m_mapChart.end(); ++it2)
//	{
//		_FBI::PT_API_CHART* p = (*it2).second;
//		delete p;
//	}
//	m_mapChart.clear();
//	DeleteCriticalSection(&m_csChart);
//}
//
//
//VOID CChartMap::Save(char* pChartData)
//{
//	_FBI::PT_API_CHART* p = (_FBI::PT_API_CHART*)pChartData;
//
//	_FBI::PT_API_CHART* pNew = new _FBI::PT_API_CHART;
//	memcpy(pNew, p, sizeof(_FBI::PT_API_CHART));
//
//	char zChartNm[32] = { 0, };
//	_FBI::chartName(p->Date, p->ChartTime, zChartNm);
//
//	EnterCriticalSection(&m_csChart);
//	m_mapChart[zChartNm] = pNew;
//	LeaveCriticalSection(&m_csChart);
//
//	//if (m_sFirstChartNm.compare(zChartNm) > 0)
//	//	m_sFirstChartNm = std::string(zChartNm);
//}
//
//VOID CChartMap::DeleteAfterOrerProc(_In_ std::string sChartNm)
//{
//	EnterCriticalSection(&m_csChart);
//	m_mapChart.erase(sChartNm);
//	LeaveCriticalSection(&m_csChart);
//}
//
//
//// return : chart name
//_FBI::PT_API_CHART* CChartMap::GetFirstChart(_Out_ std::string *psChartNm)
//{
//	_FBI::PT_API_CHART* p = NULL;
//
//	if (m_mapChart.empty())
//		return NULL;
//
//	EnterCriticalSection(&m_csChart);
//	*psChartNm = (*m_mapChart.begin()).first;
//	p = (*m_mapChart.begin()).second;
//	LeaveCriticalSection(&m_csChart);
//	return p;
//}
//
//_FBI::PT_API_CHART* CChartMap::GetNextChart(_In_ std::string sPrevChartNm, _Out_ std::string *psChartNm)
//{
//	_FBI::PT_API_CHART* p = NULL;
//
//	EnterCriticalSection(&m_csChart);
//	std::map<std::string, _FBI::PT_API_CHART*>::iterator it = m_mapChart.find(sPrevChartNm);
//	if (it == m_mapChart.end())
//	{
//		LeaveCriticalSection(&m_csChart);
//		return NULL;
//	}
//
//	it++;
//
//	if (it == m_mapChart.end())
//	{
//		LeaveCriticalSection(&m_csChart);
//		return NULL;
//	}
//
//	*psChartNm = (*it).first;
//	p = (*it).second;
//	LeaveCriticalSection(&m_csChart);
//	return p;
//}



#if 0	// shm 을 이용하기 때문에 이 로직 불필요
/*
struct PT_API_CHART
{
char	STX[1];
char	Len[4];
char	StkCd[FBILEN_SYMBOL];
char	Date[8];
char	ChartType[1];	// m:minute, h:hour, d:day
char	TimeFrame[3];	// 001: 1min, 005
char	ChartTime[5];	// hh:mm
char	Open[FBILEN_PRC];
char	High[FBILEN_PRC];
char	Low[FBILEN_PRC];
char	Close[FBILEN_PRC];
char	Volume[FBILEN_VOLUME];
char	ETX[1];
};

DB 의 당일 차트정보를 로딩해서 MAP 에 저장
*/
BOOL CDealManager::LoadChartInfo()
{
	char zQ[1024];
	char z[1024];
	char zDt[32], zTm[32];

	CUtil::GetConfig(g_zConfig, "SQL", "LOAD_CHART_DATA", z);
	sprintf(zQ, z, m_zArtcCd);

	CDBHandlerAdo db(m_pDBPool->Get());
	//sprintf(zQ, "SELECT TRADE_DT, CHART_TP, CHART_NM, OPEN_PRC, HIGH_PRC, LOW_PRC, CLOSE_PRC, CNTR_QTY "
	//	" FROM AA_CHART_DATA_MIN_01 WHERE SYMBOL='%s' ORDER BY CHART_NM", m_zStkCd);
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load Chart Error(%s)(%s)", db->GetError(), zQ);
		return FALSE;
	}
	while (db->IsNextRow())
	{
		_FBI::PT_API_CHART* pChart = new _FBI::PT_API_CHART;
		memset(pChart, 0x20, sizeof(_FBI::PT_API_CHART));

		db->GetStr("TRADE_DT", z);
		memcpy(pChart->StkCd, m_zStkCd, min(sizeof(pChart->StkCd), strlen(z)));

		// yyyymmddhhss
		db->GetStr("CHART_NM", z);
		sprintf(zDt, "%.8s", z);
		sprintf(zTm, "%.2s:%.2s", z + 8, z + 10);

		memcpy(pChart->Date, zDt, sizeof(pChart->Date));
		pChart->ChartType[0] = 'M';
		memcpy(pChart->TimeFrame, "001", 3);
		memcpy(pChart->ChartTime, zTm, sizeof(pChart->ChartTime));

		sprintf(z, "%.5f", db->GetDbl("OPEN_PRC"));
		memcpy(pChart->Open, z, min(sizeof(pChart->Open), strlen(z)));

		sprintf(z, "%.5f", db->GetDbl("HIGH_PRC"));
		memcpy(pChart->High, z, min(sizeof(pChart->High), strlen(z)));

		sprintf(z, "%.5f", db->GetDbl("LOW_PRC"));
		memcpy(pChart->Low, z, min(sizeof(pChart->Low), strlen(z)));

		sprintf(z, "%.5f", db->GetDbl("CLOSE_PRC"));
		memcpy(pChart->Close, z, min(sizeof(pChart->Close), strlen(z)));

		m_chartMap.Save((char*)pChart);

		db->Next();
	}
	db->Close();
	g_log.log(INFO, "Succeeded in DealInfo[%s]", m_zArtcCd);
	return TRUE;
}
#endif 
