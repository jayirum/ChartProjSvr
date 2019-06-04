
#include "DealManagerTenOp.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/TimeUtils.h"
#include "../common/FBIInc.h"
#include <memory.h>


extern BOOL		g_bContinue;	// flag whether continue process or not
extern CLogMsg	g_log;
extern BOOL		g_bDebugLog;
extern char		g_zConfig[_MAX_PATH];
extern CMemPool	g_memPool;


CDealManagerTenOp::CDealManagerTenOp(char* pzStkCd, char* pzArtcCd, int nIdx, unsigned int	nToClientThread) : CBaseThread(pzStkCd)
{
	strcpy(m_zStkCd, pzStkCd);
	strcpy(m_zArtcCd, pzArtcCd);
	m_pChartShm = nullptr;
	m_nIdx		= nIdx;
	m_unToClientThread = nToClientThread;
}


CDealManagerTenOp::~CDealManagerTenOp()
{
	Finalize();
}



BOOL CDealManagerTenOp::Initialize()
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
			g_log.log( NOTIFY, "[10M](%s)Thread DB OPEN FAILED.(%s)(%s)(%s)", m_zStkCd, ip, id, pwd);
			return FALSE;
		}
	}

	if (!InitChartShm())
		return FALSE;

	ResumeThread();
	
	m_hRsltProc = (HANDLE)_beginthreadex(NULL, 0, &Thread_ResultProcByChart, this, 0, &m_unRsltProc);

	if (IsTimesaveClass()) {
		m_hTimeSave = (HANDLE)_beginthreadex(NULL, 0, &Thread_TimeSave, this, 0, &m_unTimeSave);
		m_bTimeSaveRun = TRUE;
	}
	

	return TRUE;
}

BOOL CDealManagerTenOp::InitChartShm()
{	
	char zChartNmTp[32];
	CUtil::GetConfig(g_zConfig, "CHARTNAME_TYPE", "TYPE", zChartNmTp);

	m_pChartShm = new CChartShmUtilFixed(atoi(zChartNmTp));
	if (!m_pChartShm->OpenChart(m_zArtcCd))
	{
		g_log.log(LOGTP_ERR, "[10M]%s", m_pChartShm->getmsg());
		return FALSE;
	}

	g_log.log(INFO, "[10M]%s Chart Shm is opened", m_zArtcCd);

	return TRUE;
}


VOID CDealManagerTenOp::Finalize()
{
	m_bTimeSaveRun = FALSE;

	SAFE_DELETE(m_pDBPool);

	std::map<int, _FBI::ST_DEAL_INFO_TENOP*>::iterator it;
	EnterCriticalSection(&m_csDeal);
	for (it = m_mapDeal.begin(); it != m_mapDeal.end(); ++it)
	{
		_FBI::ST_DEAL_INFO_TENOP* s = (*it).second;
		delete s;
	}
	m_mapDeal.clear();
	DeleteCriticalSection(&m_csDeal);

	SAFE_DELETE(m_pChartShm);
}

BOOL CDealManagerTenOp::LoadDealInfo()
{
	char zQ[1024];
	char zOrderStartTm[128];
	char zEndTm[32];
	char zTradeDt[32];
	char zNextDt[32];
	char zNowTmFull[32];
	char zOrdTmFull[32];
	char zBaseTm[32] = { 0, };
	char zTemp[32] = { 0, };
	int nDealDateTp;
	SYSTEMTIME st;
	GetLocalTime(&st);

	// yyyymmddhh:mm:ss
	char zDate[32], zTm[32];
	sprintf(zDate,	"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	sprintf(zTm,	"%02d%02d%02d", st.wHour, st.wMinute, st.wSecond);
	CTimeUtils::AddMins(zDate, zTm, -10, zTemp);
	sprintf(zBaseTm, "%.10s:%.2s", zTemp, zTemp + 10);

	sprintf(zNowTmFull, "%04d%02d%02d%02d:%02d:%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	CDBHandlerAdo db(m_pDBPool->Get());
	
	sprintf(zQ, "EXEC AA_GET_DEAL_INFO '%s', 1 ", m_zArtcCd);
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "[10M]Load DealInfo Error(%s)", zQ);
		return FALSE;
	}

	
	while (db->IsNextRow())
	{
		// hh:mm:ss
		if (db->GetStrWithLen("TM_ORD_START", 10, zOrderStartTm) == NULL)
		{
			g_log.log(ERR, "[10M]LoadDealInfo TM_ORD_START Error(%s)", db->GetError());
			return FALSE;
		}

		if (db->GetStrWithLen("TM_END", 10, zEndTm) == NULL)
		{
			g_log.log(ERR, "[10M]LoadDealInfo TM_END Error(%s)", db->GetError());
			return FALSE;
		}
		if (db->GetStrWithLen("TRADE_DT", 10, zTradeDt) == NULL)
		{
			g_log.log(ERR, "[10M]LoadDealInfo TRADE_DT Error(%s)", db->GetError());
			return FALSE;
		}
		if (db->GetStrWithLen("NEXT_DT", 10, zNextDt) == NULL)
		{
			g_log.log(ERR, "[10M]LoadDealInfo NEXT_DT Error(%s)", db->GetError());
			return FALSE;
		}

		nDealDateTp = db->GetLong("DATE_TP");

		
		// 주문시작시간이 이미 지난건 버린다.
		if (nDealDateTp == _FBI::DATETP_TRADE)
			sprintf(zOrdTmFull, "%s%s", zTradeDt, zOrderStartTm);
		else
			sprintf(zOrdTmFull, "%s%s", zNextDt, zOrderStartTm);

		if (strcmp(zOrdTmFull, zBaseTm)<0)
		{
			//g_log.log(INFO, "[%s]Deal의 주문시간(%s)이 현재시간(%s)보다 이전이므로 버린다.",
			//	m_zArtcCd, zOrdTmFull, zNowTmFull);

			db->Next();
			continue;
		}



		_FBI::ST_DEAL_INFO_TENOP* pInfo = new _FBI::ST_DEAL_INFO_TENOP;
		ZeroMemory(pInfo, sizeof(_FBI::ST_DEAL_INFO_TENOP));
		pInfo->DealSeq = db->GetLong("DEAL_SEQ");
		db->GetStrWithLen("TM_ORD_START", 8, pInfo->tm_order1);		// hh:mm:ss
		db->GetStrWithLen("TM_ORD2", 8, pInfo->tm_order2);		// hh:mm:ss
		db->GetStrWithLen("TM_ORD3", 8, pInfo->tm_order3);		// hh:mm:ss
		db->GetStrWithLen("TM_ORD4", 8, pInfo->tm_order4);		// hh:mm:ss
		db->GetStrWithLen("TM_ORD5", 8, pInfo->tm_order5);		// hh:mm:ss
		db->GetStrWithLen("TM_ORD6", 8, pInfo->tm_order6);		// hh:mm:ss
		db->GetStrWithLen("TM_ORD7", 8, pInfo->tm_order7);		// hh:mm:ss
		db->GetStrWithLen("TM_ORD8", 8, pInfo->tm_order8);		// hh:mm:ss
		
		db->GetStr("ARTC_CD",			pInfo->ArtcCd);
		db->GetStr("TM_WAIT_START",		pInfo->tm_wait);
		db->GetStr("TM_CHARTWAIT_START", pInfo->tm_chartwait);
		db->GetStr("TM_END",			pInfo->tm_end);
		db->GetStr("TM_CHART",			pInfo->tm_chart);

		pInfo->DealStatus	= (_FBI::EN_DEAL_SATTUS_TENOP)db->GetLong("DEAL_STATUS");
		pInfo->DurationMin	= db->GetLong("DURATION");
		pInfo->DateTp		= nDealDateTp;

		if (nDealDateTp == _FBI::DATETP_TRADE)
			strcpy(pInfo->Date, zTradeDt);
		else
			strcpy(pInfo->Date, zNextDt);

		//g_log.log(INFO, "[SEQ:%d](%s)(%s)(%s)(%s)(%s)", 
		//	pInfo->DealSeq,pInfo->Date, pInfo->tm_order1, pInfo->tm_wait, pInfo->tm_chartwait, pInfo->tm_chart);
		EnterCriticalSection(&m_csDeal);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		LeaveCriticalSection(&m_csDeal);

		db->Next();
	}
	db->Close();
	g_log.log(INFO, "[10M]Succeeded in DealInfo[%s][Count:%d]", m_zArtcCd, m_mapDeal.size());
	return TRUE;
}





VOID CDealManagerTenOp::ThreadFunc()
{
	// Terminate this proces

	if (!LoadDealInfo()) {
		g_log.log(NOTIFY, "[10M]LoadDealInfo 오류로 프로세스 Terminate!");
		exit(0);
		return;
	}

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, _FBI::TIMEOUT_CHECK_DEAL, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			g_log.log(LOGTP_ERR, "[10M][THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
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
unsigned WINAPI CDealManagerTenOp::Thread_ResultProcByChart(LPVOID lp)
{
	CDealManagerTenOp* pThis = (CDealManagerTenOp*)lp;

	// Deal 정보
	int		nDealSeq;
	int		nComp;
	char	zChartNm[32] = { 0 , };
	char	UpDown[2] = { 0 , };
	char	zOpen[32] = { 0 , };
	char	zClose[32] = { 0 , };

	_FBI::ST_DEAL_INFO_TENOP recvData;
	ST_SHM_CHART_UNIT	chartData;
	MSG msg;

	while(GetMessage(&msg, NULL, 0,0))
	{
		if (msg.message == _FBI::WM_TERMINATE)			break;
		if (msg.message != _FBI::WM_RESULT_START)	continue;

		// Have enough time to get the lastest chart data
		Sleep(pThis->m_nResultWaitSec);

		memcpy(&recvData, (_FBI::ST_DEAL_INFO_TENOP*)msg.lParam, sizeof(_FBI::ST_DEAL_INFO_TENOP));
		_FBI::ST_DEAL_INFO_TENOP * pDeal = (_FBI::ST_DEAL_INFO_TENOP *)&recvData;	// msg.lParam;
		g_memPool.release((char*)msg.lParam);

		nDealSeq = pDeal->DealSeq;
		pThis->m_pChartShm->ComposeChartName(pDeal->Date, pDeal->tm_end, TP_1MIN, zChartNm);

		// CHART 를 가져온다.
		ZeroMemory(&chartData, sizeof(chartData));		
		int nRetriedCnt = 0;		
		BOOL bRet = pThis->ReadChartShm(TP_1MIN, zChartNm, pDeal->tm_end, chartData, nRetriedCnt);
		if (!bRet)
		{
			if (strncmp(pThis->m_zArtcCd, "HSI", 3) != 0 && strncmp(pThis->m_zArtcCd, "SCN", 3) != 0)
				g_log.log(NOTIFY, "[10M][%s](END_TM:%s)(CHART_TM:%s)(%s)There is no chart.Resulting Failed!!!", pThis->m_zStkCd, pDeal->tm_end, zChartNm);

			*UpDown = 'E';
			strcpy(zOpen, "0");
			strcpy(zClose, "0");
		}
		else
		{
			// Deal 정보와 비교한다.
			nComp = strncmp(chartData.open, chartData.close, sizeof(chartData.open));
			if (nComp > 0)			*UpDown = 'D';
			else if (nComp == 0)	*UpDown = 'E';
			else if (nComp < 0)		*UpDown = 'U';
			sprintf(zOpen, "%.*s", sizeof(chartData.open), chartData.open);
			sprintf(zClose, "%.*s", sizeof(chartData.close), chartData.close);

			if (nRetriedCnt > 0)
				g_log.log(INFO, "[10M]Get Chartdata Ok[RETRY:%d][%s](END_TM:%s)(CHART_NM:%s)(Open:%s)(Close:%s)", nRetriedCnt, pThis->m_zStkCd, pDeal->tm_end, zChartNm, zOpen, zClose);
		}

		// Save the result on DB and Send to Client
		pThis->SaveAndSendResult(pDeal, UpDown[0], zOpen, zClose, zChartNm);

		// delete DealInfo
		pThis->DealErase(nDealSeq);

	} // while(GetMessage(&msg, NULL, 0,0))

	return 0;
}


BOOL CDealManagerTenOp::ReadChartShm(CHART_TP ChartTp, char* pzChartNm, char* pzTmEnd, _Out_ ST_SHM_CHART_UNIT& chart, _Out_ int& refRetryCnt)
{
	int nSleep = 100;
	int nTotRetrySec = 10;
	int nTotRetryCnt = nTotRetrySec * 1000 / nSleep;
	BOOL bRet = FALSE;
	for (refRetryCnt = 0; refRetryCnt < nTotRetryCnt; refRetryCnt++)	// 10초.
	{
		bRet = m_pChartShm->GetChartData(m_zStkCd, TP_1MIN, pzChartNm, chart);
		
		if (bRet)	break;

		if (refRetryCnt >= 10)	// 1초 이후 부터는 로깅한다.
		{
			if (strncmp(m_zArtcCd, "HSI", 3) != 0 && strncmp(m_zArtcCd, "SCN", 3) != 0)
				g_log.log(ERR, "[10M][%s](END_TM:%s)(CHART_TM:%s)Get Chartdata Error(%s)", m_zStkCd, pzTmEnd, pzChartNm, m_pChartShm->getmsg());
		}
		Sleep(nSleep);
	}

	return bRet;
}


VOID CDealManagerTenOp::SaveAndSendResult(_FBI::ST_DEAL_INFO_TENOP *pDeal, char UpDown, char* pzOpen, char* pzClose, char* pzChartNm)
{
	char z[128];
	int nDealSeq = pDeal->DealSeq;

	//DB UPDATE
	for (int i = 0; i < 3; i++)
	{
		CDBHandlerAdo db(m_pDBPool->Get());
		char zQ[1024];

		sprintf(zQ, "EXEC AA_ORD_RESULT_TENOP "
			"'%s'"
			",%d"
			",%d"
			//",'%c'"
			",'%s'"
			",'%s'"
			",'%s'"
			, m_zStkCd
			, nDealSeq
			, TP_1MIN
			//, UpDown[0]
			, pzOpen
			, pzClose
			, pzChartNm
		);

		if (FALSE == db->ExecQuery(zQ))
		{
			g_log.log(ERR/*NOTIFY*/, "[10M]Result Error(AA_ORD_RESULT_TENOP) error(%s)", zQ);
			Sleep(1000);
			db->Close();
			continue;
		}

		g_log.log(INFO, "[10M][주문처리 성공][%s][SEQ:%d][CHART_NM:%s][OPEN:%.10s][CLOSE:%.10s]",m_zArtcCd, nDealSeq, pzChartNm, pzOpen, pzClose);

		// send to Client
		char* pData = g_memPool.get();

		_FBI::PT_DEAL_STATUS *cPack = (_FBI::PT_DEAL_STATUS *)pData;
		int nCPackLen = sizeof(_FBI::PT_DEAL_STATUS);

		// send to Client
		memset(cPack, 0x20, nCPackLen);
		cPack->STX[0] = _FBI::STX;

		char len[32], temp[32];
		sprintf(len, "%d", sizeof(_FBI::PT_DEAL_STATUS));
		memcpy(cPack->Len, len, strlen(len));

		cPack->DealTp[0] = _FBI::DEAL_TP_TEN_OP_C;

		memcpy(cPack->ArtcCd,	m_zArtcCd, min(sizeof(cPack->ArtcCd), strlen(m_zArtcCd)));
		memcpy(cPack->StkCd,	m_zStkCd, min(sizeof(cPack->StkCd), strlen(m_zStkCd)));
		
		sprintf(z, "%d", nDealSeq);
		memcpy(cPack->DealSeq, z, strlen(z));
		
		sprintf(temp, "%02d", _FBI::DEAL_STATUS_DONE_TENOP);
		memcpy(cPack->DealStatus, temp, 2);
		
		cPack->OrdResult[0] = UpDown;
		
		memcpy(cPack->Time,			pDeal->tm_end, sizeof(cPack->Time));
		memcpy(cPack->CandleTime,	pDeal->tm_chart, min(sizeof(cPack->CandleTime), sizeof(pDeal->tm_chart)));
		memcpy(cPack->ClosePrc,		pzClose, min(sizeof(cPack->ClosePrc), strlen(pzClose)));

		cPack->ETX[0] = _FBI::ETX;
		*(cPack->ETX + 1) = 0x00;

		PostThreadMessage(m_unToClientThread, _FBI::WM_RESULT_START, 0, (LPARAM)cPack);

		break;

	} // for (int i = 0; i < 3; i++) for DB
}


VOID CDealManagerTenOp::DealManage()
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
BOOL CDealManagerTenOp::DealManageInner()
{
	char zNowFull[32] = { 0, }; 
	char zCompareFull[32] = { 0, };
	SYSTEMTIME st;
	int nCompareLen = 0;

	// 최대 2개의 deal 이 진행중이다.
	// 결과 처리 / deal 진행
	int nDealSeq;
	std::string sOrdTm, sRsltTm;
	_FBI::ST_DEAL_INFO_TENOP* pInfo;
	std::map<int, _FBI::ST_DEAL_INFO_TENOP*>::iterator it;



	// DEAL MAP 에서 현재 시간보다 주문시간이 작은 DEAL 을 꺼낸다. 단, 이미 결과진행중인건 놔둔다.
	// 상태가 이미 완료된건 지운다.

	BOOL bMatched = FALSE;
	for (it = m_mapDeal.begin(); it != m_mapDeal.end(); )
	{
		sOrdTm.clear();
		pInfo		= NULL;
		nDealSeq	= (*it).first;
		pInfo		= (*it).second;

		GetLocalTime(&st);
		sprintf(zNowFull, "%04d%02d%02d%02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		nCompareLen = strlen(zNowFull)-3;	// HH:MM 까지만 비교 		

		// 결과진행중인건 skip 한다.
		if (pInfo->DealStatus == _FBI::DEAL_STATUS_RESULTING_TENOP)
		{
			it++;
			continue;
		}

		// 결과시간 < 지금시간  인데, 아직 미시작인건 삭제한다.
		sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_end);
		if (strncmp(zCompareFull, zNowFull, nCompareLen) <= 0)
		{
			if (pInfo->DealStatus == _FBI::DEAL_STATUS_NONSTART_TENOP)
			{
				it = m_mapDeal.erase(it);
				delete pInfo;
				continue;
			}
		}


		// 주문시간 <= 지금시간 ==> 점검한다.
		sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_order1);
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
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_DONE_TENOP)
	{
		g_log.log(INFO, "[10M][DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]이미 완료된 DEAL 이라서 MAP 에서 제거",pInfo->ArtcCd, zNowFull, sOrdTm.c_str(), _FBI::dealstatusTenOp(pInfo->DealStatus, status));
		
		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo;
		return TRUE;
	}
	
	// yyyymmddhh:mm:ss 까지 비교한다.
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_end);
	nCompareLen = strlen(zCompareFull);

	// 결과대기시간 < 지금  => 결과 점검.(  )
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		if (DealResulting(zNowFull, pInfo))
		{
			char* pDeal = g_memPool.get();
			memcpy(pDeal, pInfo, sizeof(_FBI::ST_DEAL_INFO_TENOP));
			PostThreadMessage(m_unRsltProc, _FBI::WM_RESULT_START, 0, (LPARAM)pDeal);
		}
		return TRUE;
	} 

	// 차트대기시간 < 지금  ==> 차트 점검
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_chartwait);
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		return DealChartWait(zNowFull, pInfo);
	}

	// 대기시간 < 지금  ==> 대기 점검
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_wait);
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		return DealWait(zNowFull, pInfo);
	}

	// 주문시간8 < 지금  ==> 주문가능상태 점검
	sprintf(zCompareFull, "%s%s", pInfo->Date, pInfo->tm_order1);
	if (strncmp(zCompareFull, zNowFull, nCompareLen) < 0)
	{
		return DealOrd(zNowFull, pInfo);
	}

	return FALSE;
}


int CDealManagerTenOp::GetOrdTmRange(char* pzNowFullTm, _FBI::ST_DEAL_INFO_TENOP* pInfo)
{
	char zOrdTm1[128], zOrdTm2[128];
	int nCompareLen = strlen(pzNowFullTm);

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order1);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_order2);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 1;

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order2);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_order3);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 2;

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order3);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_order4);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 3;

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order4);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_order5);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 4;

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order5);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_order6);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 5;

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order6);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_order7);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 6;

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order7);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_order8);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 7;

	sprintf(zOrdTm1, "%s%s", pInfo->Date, pInfo->tm_order8);
	sprintf(zOrdTm2, "%s%s", pInfo->Date, pInfo->tm_wait);
	if (strncmp(zOrdTm1, pzNowFullTm, nCompareLen) <= 0 && strncmp(pzNowFullTm, zOrdTm2, nCompareLen) < 0)
		return 8;

	return 0;
}


BOOL CDealManagerTenOp::DealOrd(char* pzNowFullTm, _FBI::ST_DEAL_INFO_TENOP* pInfo)
{
	char status[128];
	int nOrdTmRange = GetOrdTmRange(pzNowFullTm, pInfo);

	if (nOrdTmRange == 0)	return FALSE;
	
	BOOL bUpdate = FALSE;
	switch (nOrdTmRange )
	{
	case 1:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_1)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_1;
			bUpdate = TRUE;
		}
		break;
	case 2:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_2)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_2;
			bUpdate = TRUE;
		}
		break;
	case 3:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_3)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_3;
			bUpdate = TRUE;
		}
		break;
	case 4:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_4)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_4;
			bUpdate = TRUE;
		}
		break;
	case 5:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_5)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_5;
			bUpdate = TRUE;
		}
		break;
	case 6:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_6)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_6;
			bUpdate = TRUE;
		}
		break;
	case 7:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_7)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_7;
			bUpdate = TRUE;
		}
		break;
	case 8:
		if (pInfo->DealStatus != _FBI::DEAL_STATUS_ORD_8)
		{
			pInfo->DealStatus = _FBI::DEAL_STATUS_ORD_8;
			bUpdate = TRUE;
		}
		break;
	
	} // switch (nOrdTmRange )

	if (bUpdate)
	{
		if (g_bDebugLog) 
		{
			if (strncmp(pInfo->ArtcCd, "CL", 2) == 0)
				g_log.log(INFO, "[10M](%s)(SEQ:%d)/주문/(현재:%s) <%s>으로 UPDATE",pInfo->ArtcCd, pInfo->DealSeq, pzNowFullTm, _FBI::dealstatusTenOp(pInfo->DealStatus, status));
		}

		//+-----------------------------------------------
		UpdateDealStatus(pInfo);
		//+-----------------------------------------------
		
		m_mapDeal[pInfo->DealSeq] = pInfo;

		if (IsTimesaveClass()) {
			PostThreadMessage(m_unTimeSave, _FBI::WM_DEAL_STATUS, 0, 0);
		}
	}
	//else
	//{
	//	//상태가 이상하다. 에러 처리.
	//	g_log.log(ERR/*NOTIFY*/, "[10M][DEAL_MANAGE(%s)(SEQ:%d)/주문상태업데이트오류(현재:%s)(현상태:%s)]",
	//		pInfo->ArtcCd, pInfo->DealSeq, pzNowFullTm, _FBI::dealstatusTenOp(pInfo->DealStatus, status));
	//	
	//	m_mapDeal.erase(pInfo->DealSeq);
	//	delete pInfo;
	//}
	return bUpdate;
}


BOOL CDealManagerTenOp::DealWait(char* pzNowFull, _FBI::ST_DEAL_INFO_TENOP* pInfo)
{
	//char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT_TENOP)
	{
		return FALSE;;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD_8)
	{
		if (g_bDebugLog) 
			g_log.log(INFO, "[10M](%s)(SEQ:%d)/(현재:%s)(DEAL대기시간:%s%s) <대기>로 UPDATE",pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_wait);
		
		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_WAIT_TENOP;
		UpdateDealStatus(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		return TRUE;
	}
	//else
	//{
	//	//상태가 이상하다. 에러 처리.
	//	g_log.log(ERR/*NOTIFY*/, "[10M][DEAL_MANAGE(%s)(SEQ:%d)/대기/(현재:%s)(DEAL대기시간:%s%s)(현상태:%s)]대기시간 지났는데 상태가 <주문8>이 아니다. ",
	//		pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_wait, _FBI::dealstatusTenOp(pInfo->DealStatus, status));

	//	m_mapDeal.erase(pInfo->DealSeq);
	//	delete pInfo;
	//	return FALSE;
	//}
	return FALSE;
}


BOOL CDealManagerTenOp::DealChartWait(char* pzNowFull, _FBI::ST_DEAL_INFO_TENOP* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_CHARTWAIT_TENOP)
	{
		return FALSE;;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT_TENOP)
	{
		// update.
		if (g_bDebugLog) 
			g_log.log(INFO, "[10M](%s)(SEQ:%d)/(현재:%s)(DEAL차트시간:%s%s)] <차트>로 UPDATE",pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_chartwait);
		
		pInfo->DealStatus = _FBI::DEAL_STATUS_CHARTWAIT_TENOP;
		UpdateDealStatus(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		
		return TRUE;
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[10M][DEAL_MANAGE(%s)(SEQ:%d)/차트/(현재:%s)(DEAL차트시간:%s%s)(현상태:%s)]차트시간 지났는데 상태가 <대기>가 아니다. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_chartwait, _FBI::dealstatusTenOp(pInfo->DealStatus, status));

		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo;
		return FALSE;
	}
}


// 
BOOL CDealManagerTenOp::DealResulting(char* pzNowFull, _FBI::ST_DEAL_INFO_TENOP* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_RESULTING_TENOP)
	{
		return FALSE;;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_CHARTWAIT_TENOP)
	{
		// update.
		if (g_bDebugLog) 
			g_log.log(INFO, "[10M](%s)(SEQ:%d)/(현재:%s)(DEAL결과시간:%s%s) <결과>로 UPDATE", pInfo->ArtcCd, pInfo->DealSeq, pzNowFull, pInfo->Date, pInfo->tm_end);
		
		pInfo->DealStatus = _FBI::DEAL_STATUS_RESULTING_TENOP;
		UpdateDealStatus(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		return TRUE;
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[10M][DEAL_MANAGE(%s)/결과/(SEQ:%d)(현재:%s)(DEAL결과시간:%s%s)(현상태:%s)]결과시간 지났는데 상태가 <차트>가 아니다. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNowFull,pInfo->Date, pInfo->tm_end, _FBI::dealstatusTenOp(pInfo->DealStatus, status));

		m_mapDeal.erase(pInfo->DealSeq);
		delete pInfo; 
		return FALSE;
	}
}

VOID CDealManagerTenOp::DealErase(int nDealSeq)
{
	EnterCriticalSection(&m_csDeal);
	m_mapDeal.erase(nDealSeq);
	LeaveCriticalSection(&m_csDeal);
}


/*
struct ST_DEAL_INFO_TENOP
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
void CDealManagerTenOp::UpdateDealStatus(_FBI::ST_DEAL_INFO_TENOP* pInfo)
{
	// send to the client
	char z[128];
	char len[32];
	char* packet = g_memPool.get();
	
	_FBI::PT_DEAL_STATUS* pDeal = (_FBI::PT_DEAL_STATUS*)packet;
	memset(pDeal, 0x20, sizeof(_FBI::PT_DEAL_STATUS));

	pDeal->STX[0] = _FBI::STX;

	sprintf(len, "%d", sizeof(_FBI::PT_DEAL_STATUS));
	memcpy(pDeal->Len, len, strlen(len));
	
	pDeal->DealTp[0] = _FBI::DEAL_TP_TEN_OP_C;
	
	memcpy(pDeal->ArtcCd,	m_zStkCd, min(strlen(m_zArtcCd),_FBI::FBILEN_SYMBOL));
	memcpy(pDeal->StkCd,	m_zStkCd, min(strlen(m_zStkCd), _FBI::FBILEN_SYMBOL));

	sprintf(z, "%0*d", _FBI::FBILEN_DEAL_SEQ, pInfo->DealSeq);
	memcpy(pDeal->DealSeq, z, _FBI::FBILEN_DEAL_SEQ);

	sprintf(z, "%02d", pInfo->DealStatus);
	memcpy(pDeal->DealStatus, z, sizeof(pDeal->DealStatus));

	pDeal->ETX[0] = _FBI::ETX;
	*(pDeal->ETX + 1) = 0x00;

	switch (pInfo->DealStatus)
	{
	case _FBI::DEAL_STATUS_ORD_1:
		memcpy(pDeal->Time, pInfo->tm_order1, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_ORD_2:
		memcpy(pDeal->Time, pInfo->tm_order2, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_ORD_3:
		memcpy(pDeal->Time, pInfo->tm_order3, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_ORD_4:
		memcpy(pDeal->Time, pInfo->tm_order4, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_ORD_5:
		memcpy(pDeal->Time, pInfo->tm_order5, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_ORD_6:
		memcpy(pDeal->Time, pInfo->tm_order6, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_ORD_7:
		memcpy(pDeal->Time, pInfo->tm_order7, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_ORD_8:
		memcpy(pDeal->Time, pInfo->tm_order8, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_WAIT_TENOP:
		memcpy(pDeal->Time, pInfo->tm_wait, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_CHARTWAIT_TENOP:
		memcpy(pDeal->Time, pInfo->tm_order1, sizeof(pDeal->Time));
		break;
	case _FBI::DEAL_STATUS_RESULTING_TENOP:
		memcpy(pDeal->Time, pInfo->tm_end, sizeof(pDeal->Time));
		break;
	default:
		g_log.log(ERR/*NOTIFY*/, "[10M][%s](Status:%s)Update Error", 
			pInfo->ArtcCd, _FBI::dealstatusTenOp(pInfo->DealStatus, z));
		return;
	}
	memcpy(pDeal->CandleTime, pInfo->tm_chart, min( sizeof(pDeal->CandleTime), sizeof(pInfo->tm_chart)));

	//DB UPDATE
	for (int i = 0; i < 3; i++)
	{
		CDBHandlerAdo db(m_pDBPool->Get());
		char zQ[1024];

		sprintf(zQ, "EXEC AA_UPDATE_DEAL_STATUS %d, %d, '%s', 1"
			, pInfo->DealStatus
			, pInfo->DealSeq
			, m_zArtcCd
		);
		if (FALSE == db->ExecQuery(zQ))
		{
			g_log.log(ERR/*NOTIFY*/, "[10M]Update DEAL_MST error(%s)", zQ);
			Sleep(1000);
			db->Close();
			continue;
		}
		else
		{
			break;
		}
	}

	// Result is sent after complete the result
	if (pInfo->DealStatus != _FBI::DEAL_STATUS_RESULTING_TENOP) {
		PostThreadMessage(m_unToClientThread, _FBI::WM_RESULT_START, 0, (LPARAM)pDeal);
		g_log.log(INFO, "[10M][UpdateDealStatus](%s)", packet);
	}
}



unsigned WINAPI CDealManagerTenOp::Thread_TimeSave(LPVOID lp)
{
	CDealManagerTenOp* p = (CDealManagerTenOp*)lp;
	int nCnt = 0;
	BOOL bStart = FALSE;
	char zQ[1024];

	while (p->m_bTimeSaveRun)
	{
		Sleep(100);

		if (++nCnt == 5 && bStart)
		{
			//	DB UPDATE
			CDBHandlerAdo db(p->m_pDBPool->Get());
			sprintf(zQ, "AA_UPDATE_ORD_TM_TENOP");
			if (FALSE == db->ExecQuery(zQ))
			{
				g_log.log(ERR/*NOTIFY*/, "Update Curr Time error(%s)", zQ);
				Sleep(1000);
				db->Close();
				continue;
			}
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

