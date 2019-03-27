
#include "ChartMaker.h"
#include "../../IRUM_util/IRUM_Common.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/ChartUtils.h"
//#include "C:\ta-lib\c\include\ta_libc.h"


extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];
extern CMemPool	*g_pMemPool;


CChartMaker::CChartMaker(char* pzSymbol, char* pzArtcCode, int nDotCnt, unsigned dwMainThreadId, BOOL bSaveChart)
{
	m_bSaveChart = bSaveChart;
	m_dwMainThreadId = dwMainThreadId;
	//m_pMemPool = pMemPool;
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zArtc, pzArtcCode);
	m_nDotCnt = nDotCnt;
	//GET_SHM_NM(pzArtcCode, m_zShmNm);
	//GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);

	char zChartNmTp[32] = { 0, };
	CUtil::GetConfig(g_zConfig, "CHARTNAME_TYPE", "TYPE", zChartNmTp);
	m_chartNmType = (CHARTNAME_TYPE)atoi(zChartNmTp);
	
	m_chartUtil = new CChartShmUtilFixed(m_chartNmType);

	char szDir[128];
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	m_logData.OpenLog(szDir, "ChartShm");

	CUtil::GetConfig(g_zConfig, "DEBUG", "LOG_DATA", szDir);
	g_log.log(INFO, "LOG_DATA:%c", szDir[0]);
	m_bLogData = (szDir[0] == 'Y') ? TRUE : FALSE;

	ResumeThread();

	// worker thread 생성
	m_hWorkDie = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hWorkThread = (HANDLE)_beginthreadex(NULL, 0, &WorkThread, this, 0, &m_dwWorkThreadID);
}

CChartMaker::~CChartMaker()
{
	CloseChartShm();
	SetEvent(m_hWorkDie);
	if (WaitForSingleObject(m_hWorkThread, 3000) != WAIT_OBJECT_0) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hWorkThread, dwExitCode);
	}
	CloseHandle(m_hWorkDie);
	CloseHandle(m_hWorkThread);
	m_hDie = m_hThread = NULL;
	CloseMemPool();
}

BOOL CChartMaker::IsMySymbol(char* pSise)
{
	return TRUE;
}


BOOL CChartMaker::InitChartSHM()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);

	//	OPEN SHM	
	if (!m_chartUtil->OpenChart(m_zArtc))
	{
		g_log.log(LOGTP_FATAL, "CHART SHM OPEN 에러((symbol:%s)(%s)", m_zSymbol, m_chartUtil->getmsg());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN 성공(%s)", m_chartUtil->GetShmName());

	char temp[32];
	for (int i = TP_1MIN; i < TP_DAY; i++)
	{
		ST_SHM_CHART_UNIT chart;

		char zStructKey[LEN_CHART_NM + 1] = { 0, };
		if (m_chartUtil->GetLastChartData(m_zSymbol, (CHART_TP)i, &chart))
		{
			sprintf(temp, "%.*s", sizeof(chart.Nm), chart.Nm);
			m_sLastChartNm[i] = temp;
		}
		else
		{
			//
		}
	}
	return TRUE;
}




BOOL CChartMaker::InitMemPool()
{
	m_pMemPool = new CMemPool(MEMPOOL_PRE_ALLOC, MEMPOOL_MAX_ALLOC, MEMPOOL_BLOCK_SIZE);

	if (m_pMemPool->available() == 0)
	{
		return FALSE;
	}
	return TRUE;
}
VOID CChartMaker::CloseMemPool()
{
	SAFE_DELETE(m_pMemPool);
}

VOID CChartMaker::ThreadFunc()
{
	//printf("thread:%d\n", GetCurrentThreadId());
	if (!InitChartSHM())
	{
		g_log.log(LOGTP_ERR, ">>>>>> SHM OPEN ERROR <<<<<<<<<");
		SetEvent(m_hWorkDie);
		return;
	}

	if( !InitMemPool())
	{
		g_log.log(LOGTP_ERR, ">>>>>> Mempool OPEN ERROR <<<<<<<<<");
		SetEvent(m_hWorkDie);
		return;
	}

	//char zMarketDataBuff[1024] = { 0, };
	int nSymbolLen = strlen(m_zSymbol);

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			//g_log.log(LOGTP_ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
		}
		
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_CHART_ALL_KILL:
				g_log.log(LOGTP_SUCC, "[THREAD ID:%d] Recv Kill Msg", GetMyThreadID());
				break;
			//case WM_CHART_DATA:
			//{
			//	ST_PACK2CHART_EX* p = (ST_PACK2CHART_EX*)msg.lParam;		// MEM POOL
			//	if (strncmp(m_zSymbol, p->ShortCode, nSymbolLen) != 0) {
			//		g_pMemPool->release((char*)msg.lParam); // main 에서 넘어온 메모리
			//		continue;
			//	}

			//	PostThreadMessage(m_dwWorkThreadID, WM_RECV_API_MD, msg.wParam, msg.lParam);

			//	break;
			//}
			} // switch (msg.message)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	SetEvent(m_hWorkDie);
}



unsigned WINAPI CChartMaker::WorkThread(LPVOID lp)
{
	CChartMaker* p = (CChartMaker*)lp;
	while (WaitForSingleObject(p->m_hWorkDie, 1) != WAIT_OBJECT_0)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_RECV_API_MD)
			{
				char* pData = (char*)msg.lParam;
				int nLen = (int)msg.wParam;

				int i = (int)TP_1MIN;	// 0;
				//for (i = (int)TP_1MIN; i < (int)TP_DAY; i++)	//일,주,월은 배치로
				{
					p->ChartProc((void*)pData, (int)i);
				}
				p->m_pMemPool->release(pData);
			} //if (msg.message == WM_SEND_STRATEGY)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	//p->CloseMemPool();
	return 0;
}


VOID	CChartMaker::ChartProc(VOID* pIn, int tp)
{
	ST_PACK2CHART_EX* p = (ST_PACK2CHART_EX*)pIn;
	char temp[32];
	BOOL bRet;
	//char zSymbol[32];
	//sprintf(zSymbol, "%.*s", sizeof(p->ShortCode), p->ShortCode);

	//ir_cvtcode_uro_6e(zSymbol, temp);
	//memset(p->ShortCode, 0x20, sizeof(p->ShortCode));
	//memcpy(p->ShortCode, temp, strlen(temp));
	ST_SHM_CHART_UNIT recvUnit;
	memset(&recvUnit, 0x20, sizeof(recvUnit));

	// GROUP KEY (symbol+charttp)
	char szGroupKey[LEN_SHM_GROUP_KEY + 1];
	GET_GROUP_KEY(p->ShortCode, tp, szGroupKey);

	// STRUCT KEY (chart name) 0000, 0005, 0010
	char szChartNm[LEN_SHM_STRUCT_KEY + 1];

	m_chartUtil->ComposeChartName(p->Date, p->Time, tp, szChartNm);
	//if(m_bLogData)
	//	m_logData.log(DATA, "[DATE:%.8s][TIME:%.8s][CHART:%s][TICK:%.*s]", p->Date, p->Time, szChartNm, LEN_PRC, p->Close);
	//return;

	///////////////////////////////////////////////////////////////////////////////////////
	// chart unit 구성

	// DB에도 아무것도 없는 경우 (If there is no data from DB)
	if (m_sLastChartNm[tp].empty())
	{
		m_sPrevChartNm[tp] = DEFINE_NO_CHART;
		m_sLastChartNm[tp] = szChartNm;
	}


	/*
		최종차트이름과 현재가 소속 차트이름이 다른 경우 ==> 두번째 차트를 처음으로 구성하는 경우이다.
		- 이 경우 직전차트이름에 최종차트이름을 저장하고, 최종차트이름에는 현재차트이름을 저장한다.

		즉, 최종차트이름과 현재차트이름이 동일한 경우 같은 차트가 계속 그려지고 있는 경우이므로
		직전차트이름과 최종차트이름은 변경하지 않는다.
	*/
	else if (m_sLastChartNm[tp].compare(szChartNm) != 0)
	{
		m_sPrevChartNm[tp] = m_sLastChartNm[tp];
		m_sLastChartNm[tp] = szChartNm;
	}

	// recvUnit 에 packet 정보를 저장
	memcpy(recvUnit.Nm, szChartNm, sizeof(recvUnit.Nm));	// hhmm		0000, 0005, 0010
	memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), min(sizeof(recvUnit.prevNm), m_sPrevChartNm[tp].size()));
	memcpy(recvUnit.open, p->Close, sizeof(p->Close));
	memcpy(recvUnit.high, p->Close, sizeof(p->Close));
	memcpy(recvUnit.low, p->Close, sizeof(p->Close));
	memcpy(recvUnit.close, p->Close, sizeof(p->Close));
	memcpy(recvUnit.cntr_qty, p->CntrQty, sizeof(p->CntrQty));

	sprintf(temp, "%d", m_nDotCnt);
	memcpy(recvUnit.dotcnt, temp, strlen(temp));

	// group(SYMBOL+TP) 을 찾아서 없으면 insert. 즉, 최초 저장 (Find Group)
	if (!m_chartUtil->GroupFind(szGroupKey))
	{
		if (!m_chartUtil->GroupInsert(szGroupKey))
		{
			g_log.log(ERR, "GroupInsert ERROR!!!(%s)(%s)", m_zSymbol, szGroupKey);
			return;
		}
		printf("GroupInsert(%s)(%s)\n", m_zSymbol, szGroupKey);
		if(!m_chartUtil->DataInsert(szGroupKey, (char*)&recvUnit))
		{
			g_log.log(LOGTP_ERR, "DataInsert ERROR-1!!!(%s)(%s)(%s)", m_zSymbol, szGroupKey, szChartNm);
			return;
		}
		printf("DataInsert-1(%s)(%s)(%s)\n", m_zSymbol, szGroupKey, szChartNm);
		if(m_bLogData)
			m_logData.log(DATA, "Insert Group/Data[%s][NM:%.*s][O:%.20s][H:%.20s][L:%.20s][C:%.20s](Q:%.20s)",
				szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.open, recvUnit.high, recvUnit.low, recvUnit.close, recvUnit.cntr_qty);
		
		return;
	}

	// 데이터를 찾는다. 
	ST_SHM_CHART_UNIT existUnit;
	bRet = m_chartUtil->DataGet(szGroupKey, szChartNm, (char*)&existUnit); // Get data using chart name (struct key)
	
	// 데이터가 없다. insert. 즉, 시가 입력이라는 얘기이다.
	if (!bRet)
	{
		memcpy(recvUnit.open, p->Close, sizeof(p->Close));
		memcpy(recvUnit.low, p->Close, sizeof(p->Close));
		memcpy(recvUnit.high, p->Close, sizeof(p->Close));
		//memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), LEN_CHART_NM);
		memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), min(sizeof(recvUnit.prevNm), m_sPrevChartNm[tp].size()));

		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		// 시가를 받은 경우 직전차트의 SMA 를 구해서 저장한다.
		// Chart_SMA(tp, szGroupKey, szChartNm, (char*)&recvUnit);

		// 시가를 받은 경우 직전차트 기록
		if (m_bLogData)
		{
			ST_SHM_CHART_UNIT prevChart;
			if (m_chartUtil->DataGet(szGroupKey, recvUnit.prevNm, (char*)&prevChart)) {
				m_logData.log(DATA_DT, "");
				m_logData.log(DATA, "[%s][NM:%.*s][O:%.20s][H:%.20s][L:%.20s][C:%.20s](Q:%.20s)",
					szGroupKey, LEN_CHART_NM, prevChart.Nm, prevChart.open, prevChart.high, prevChart.low, prevChart.close, prevChart.cntr_qty);
			}
		}

		if (!m_chartUtil->DataInsert(szGroupKey, (char*)&recvUnit))
		{
			g_log.log(LOGTP_ERR, "DataInsert ERROR-2!!!(%s)(%s)(%s)", m_zSymbol, szGroupKey, szChartNm);
			return;
		}
		//printf("DataInsert-2(%s)(%s)(%s)\n", m_zSymbol, szGroupKey, szChartNm);

		// DB 저장
		if (m_bSaveChart)
		{
			ST_SHM_CHART_UNIT* pChart = new ST_SHM_CHART_UNIT;
			memcpy(pChart, &recvUnit, sizeof(ST_SHM_CHART_UNIT));
			memcpy(pChart->Reserved, szGroupKey, LEN_GROUP_KEY);
			memcpy(pChart->stk_cd, m_zSymbol, strlen(m_zSymbol));
			PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)pChart);
		}
		return;
	}

	// 데이터가 있다.

	// 차트 계산
	double dOpen = S2D(existUnit.open, sizeof(existUnit.open));
	double dHigh = S2D(existUnit.high, sizeof(existUnit.high));
	double dLow = S2D(existUnit.low, sizeof(existUnit.low));
	double dNow = S2D(recvUnit.close, sizeof(recvUnit.close));

	dHigh = (dHigh > dNow) ? dHigh : dNow;
	dLow = (dLow < dNow) ? dLow : dNow;

	//FORMAT_PRC(dHigh, S2N(existUnit.dotcnt, sizeof(existUnit.dotcnt)), temp);
	FORMAT_PRC(dHigh, m_nDotCnt, temp);
	memcpy(existUnit.high, temp, min(sizeof(existUnit.high), strlen(temp)));

	//FORMAT_PRC(dLow, S2N(existUnit.dotcnt, sizeof(existUnit.dotcnt)), temp);
	FORMAT_PRC(dLow, m_nDotCnt, temp);
	memcpy(existUnit.low, temp, min(sizeof(existUnit.low), strlen(temp)));

	if (dNow > dOpen)	existUnit.gb[0] = CD_PLUS_CANDLE;	// 양봉
	if (dNow < dOpen)	existUnit.gb[0] = CD_MINUS_CANDLE;	// 음봉
	if (dNow == dOpen)	existUnit.gb[0] = '0';

	memcpy(existUnit.close, recvUnit.close, sizeof(recvUnit.close));
	//memcpy(existUnit.seq, recvUnit.seq, sizeof(recvUnit.seq));
	//memcpy(existUnit.prevNm, m_sPrevChartNm[tp].c_str(), LEN_CHART_NM);
	memcpy(existUnit.prevNm, m_sPrevChartNm[tp].c_str(), min(sizeof(existUnit.prevNm), m_sPrevChartNm[tp].size()));

	int nQty = S2N(existUnit.cntr_qty, sizeof(existUnit.cntr_qty)) + S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty));
	char zQty[LEN_QTY + 1];
	sprintf(zQty, "%*d", LEN_QTY, nQty);
	memcpy(existUnit.cntr_qty, zQty, strlen(zQty));


	//SHM 반영
	bRet = m_chartUtil->DataUpdate(szGroupKey, szChartNm, (char*)&existUnit);

	if (!bRet) {
		g_log.log(LOGTP_ERR, "DataUpdate ERROR!!!(%s)(%s)(%s)", m_zSymbol, szGroupKey, szChartNm);
		return;
	}
	else {
		//printf("DataUpdate(%s)(%s)(%s)\n", m_zSymbol, szGroupKey, szChartNm);
		// DB 저장
		if (m_bSaveChart)
		{
			ST_SHM_CHART_UNIT* pChart = new ST_SHM_CHART_UNIT;
			memcpy(pChart, &existUnit, sizeof(ST_SHM_CHART_UNIT));
			memcpy(pChart->Reserved, szGroupKey, LEN_GROUP_KEY);
			memcpy(pChart->stk_cd, m_zSymbol, strlen(m_zSymbol));
			PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)pChart);
			//g_log.log(INFO, "[%.*s](O:%.*s)(H:%.*s)(L:%.*s)(C:%.*s)",
			//	LEN_CHART_NM, existUnit.Nm,
			//	LEN_PRC, existUnit.open,
			//	LEN_PRC, existUnit.high,
			//	LEN_PRC, existUnit.low,
			//	LEN_PRC, existUnit.close
			//);
		}	//	szGroupKey, debug.Nm, debug.prevNm, atoi(debug.seq), atof(debug.open), atof(debug.close), debug.gb);
	}
}



/*
// simple moving average
- 시가를 받은 경우 그 직전 차트의 SMA 를 구한다.
*/
VOID CChartMaker::Chart_SMA(int tp, char* pzGroupKey, char* pzNowChartNm, _Out_ char* i_pNowChart)
{
	ST_SHM_CHART_UNIT* pNowChart = (ST_SHM_CHART_UNIT*)i_pNowChart;

	// 현 종목 차트 개수 가져온다.
	if (!m_chartUtil->IsSavedEnoughDataCnt(pzGroupKey, SMA_LONG_CNT + 1))
		return;

	char zChartNm[32];// , zCloseChartNm[32];
	ST_SHM_CHART_UNIT chartShm;//, stLastChartShm;
	memcpy(&chartShm, pNowChart, sizeof(chartShm));

	CMA	 ma(SMA_SHORTEST_CNT, SMA_SHORT_CNT, SMA_LONG_CNT, FALSE);

	double sum = 0;
	int nLoop = 0;
	double dShortestMA, dShortMA, dLongMA;
	for (int i = 0; i <SMA_LONG_CNT; i++, nLoop++)
	{
		sprintf(zChartNm, "%.*s", LEN_CHART_NM, chartShm.prevNm);
		if (strcmp(DEFINE_NO_CHART, zChartNm) == 0)	break;

		if (FALSE == m_chartUtil->DataGet(pzGroupKey, zChartNm, (char*)&chartShm))
		{
			g_log.log(LOGTP_ERR, "[Chart_SMA][%s][%s]차트가 SHM 에 없다(SMA_LONG_CNT:%d,없는차트Index:%d)"
				, pzGroupKey, zChartNm, SMA_LONG_CNT, i);
			continue;
		}

		sum += atof(chartShm.close);
		if (nLoop == SMA_SHORT_CNT - 1)
		{
			dShortMA = sum / (double)SMA_SHORT_CNT;
		}
		if (nLoop == SMA_SHORTEST_CNT - 1)
		{
			dShortestMA = sum / (double)SMA_SHORTEST_CNT;
		}
		ma.setClose(atof(chartShm.close));
	}
	dLongMA = sum / (double)SMA_LONG_CNT;
	//ma.getMAEx(&dShortestMA, &dShortMA, &dLongMA);

	char zPrc[32];
	FORMAT_PRC(dLongMA, m_nDotCnt, zPrc);
	memcpy(pNowChart->sma_long, zPrc, strlen(zPrc));

	FORMAT_PRC(dShortMA, m_nDotCnt, zPrc);
	memcpy(pNowChart->sma_short, zPrc, strlen(zPrc));

	FORMAT_PRC(dShortestMA, m_nDotCnt, zPrc);
	memcpy(pNowChart->sma_shortest, zPrc, strlen(zPrc));

	//g_log.log(LOGTP_SUCC, "[Chart_SMA][LMA:%.*s][SMA:%.*s]", LEN_PRC, stLastChartShm.sma_long, LEN_PRC, stLastChartShm.sma_short);

	//SHM 반영
	//m_shmQ.DataUpdate(pzGroupKey, zCloseChartNm, 0, sizeof(ST_SHM_CHART_UNIT), (char*)&stLastChartShm);
}




//
//BOOL CChartMaker::InitChartSHM()
//{
//	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
//	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);
//
//	//	LAST SHM OPEN
//	//if (!m_shmLast.Open(LASTCHART_SHM_NM, LASTCHART_LOCK_NM))
//	//{
//	//	g_log.log(LOGTP_FATAL, ">>>>>>>LAST CHART SHM OPEN 에러(%s)(symbol:%s)(%s)", LASTCHART_SHM_NM, LASTCHART_LOCK_NM, m_shmLast.GetErr());
//	//	return FALSE;
//	//}
//	
//	//	OPEN SHM
//	if (!m_shmQ.Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
//	{
//		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN 에러(%s)(symbol:%s)(%s)",m_zShmNm, m_zSymbol, m_shmQ.GetErr());
//		return FALSE;
//	}
//	g_log.log(LOGTP_SUCC, "SHM OPEN 성공(%s)", m_zMutexNm);
//
//
//	// DB 에서 마지막 차트 이름을 가져온다.
//	char ip[32], id[32], pwd[32], name[32];
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_IP", ip);
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_ID", id);
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_PWD", pwd);
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_NAME", name);
//
//	CDBPoolAdo *pDBPool;
//	pDBPool = new CDBPoolAdo(ip, id, pwd, name);
//	if (!pDBPool->Init(1))
//	{
//		SAFE_DELETE(pDBPool);
//		return 0;
//	}
//	CDBHandlerAdo db(pDBPool->Get());
//	char zQ[1024];
//
//
//	for (int i = TP_1MIN; i < TP_DAY; i++)
//	{
//		sprintf(zQ, "EXEC CHART_GET_LASTCHART_NM '%s', %d ", m_zSymbol, i);
//		if (FALSE == db->QrySelect(zQ))
//		{
//			g_log.log(LOGTP_ERR, "마지막차트 조회 오류(%s)(%s)", db->GetError(), zQ);
//			return 0;
//		}
//
//
//		while (db->IsNextRow())
//		{
//			char temp[128];
//			int nTp = db->GetLong("CHART_TP");
//			m_sLastChartNm[nTp] = db->GetStr("CHART_NM", temp);
//			db->Next();
//		}
//	}
//	db->Close();
//
//	return TRUE;
//}
//



//VOID CChartMaker::Chart_SMA(int tp, char* pzGroupKey, char* pzNowChartNm, _Out_ char* i_pNowChart)
//{
//	ST_SHM_CHART_UNIT* pNowChart = (ST_SHM_CHART_UNIT*)i_pNowChart;
//
//	// 현 종목 차트 개수 가져온다.
//	if(!m_shmQ.IsSavedEnoughDataCnt(pzGroupKey, SMA_LONG_CNT + 1))
//		return;
//
//	char zChartNm[32], zCloseChartNm[32];
//	ST_SHM_CHART_UNIT chartShm;//, stLastChartShm;
//	TA_Real stSMA[SMA_LONG_CNT] = { 0, };
//
//	int nLoop = SMA_LONG_CNT;
//	sprintf(zChartNm, "%.*s", LEN_CHART_NM, pNowChart->prevNm);
//	int nCnt = 1;
//
//	/*
//	ARRAY 에 거꾸로 담아야 한다.
//	*/
//	for (int i = nLoop - 1; i > -1; i--)
//	{
//		if (strcmp(DEFINE_NO_CHART, zChartNm) == 0)	break;
//
//		if (FALSE == m_shmQ.DataGet(pzGroupKey, zChartNm, (char*)&chartShm))
//		{
//			g_log.log(LOGTP_ERR, "[Chart_SMA][%s][%s]차트가 SHM 에 없다", pzGroupKey, zChartNm);
//			continue;
//		}
//
//		// 제일 첫번째 차트 정보.
//		if (i == nLoop - 1) {
//			strcpy(zCloseChartNm, zChartNm);
//			//memcpy(&stLastChartShm, &chartShm, sizeof(chartShm));
//		}
//
//		sprintf(zChartNm, "%.*s", LEN_CHART_NM, chartShm.prevNm);
//
//		//Log(TRUE, "[Chart_SMA][%d][NOW:%.4s](%.20s)", i, chartShm.Nm, chartShm.close);
//
//		stSMA[i] = atof(chartShm.close);
//	}
//
//
//	TA_Real		outShort[10], outLong[20];
//	TA_Integer outBeg, outNbElement;
//	TA_Integer outBeg2, outNbElement2;
//	TA_RetCode retCode;
//	retCode = TA_MA(0, 19, &stSMA[0], 20, TA_MAType_SMA, &outBeg, &outNbElement, &outLong[0]);
//	retCode = TA_MA(0, 9, &stSMA[10], 10, TA_MAType_SMA, &outBeg2, &outNbElement2, &outShort[0]);
//
//	char zPrc[32], zDotCnt[32];
//	sprintf(zDotCnt, "%.*s", sizeof(chartShm.dotcnt), chartShm.dotcnt);
//	int nDotCnt = atoi(zDotCnt);
//
//	double dLong = CUtil::roundoff(outLong[0], nDotCnt);	// 반올림
//	double dShort = CUtil::roundoff(outShort[0], nDotCnt);	// 반올림
//
//	FORMAT_PRC(dLong, nDotCnt, zPrc);
//	//memcpy(stLastChartShm.sma_long, zPrc, strlen(zPrc));
//	memcpy(pNowChart->sma_long, zPrc, strlen(zPrc));
//
//	FORMAT_PRC(dShort, nDotCnt, zPrc);
//	//memcpy(stLastChartShm.sma_short, zPrc, strlen(zPrc));
//	memcpy(pNowChart->sma_short, zPrc, strlen(zPrc));
//	
//	//g_log.log(LOGTP_SUCC, "[Chart_SMA][LMA:%.*s][SMA:%.*s]", LEN_PRC, stLastChartShm.sma_long, LEN_PRC, stLastChartShm.sma_short);
//
//	//SHM 반영
//	//m_shmQ.DataUpdate(pzGroupKey, zCloseChartNm, 0, sizeof(ST_SHM_CHART_UNIT), (char*)&stLastChartShm);
//}

void CChartMaker::CloseChartShm()
{
	m_chartUtil->ReleaseChart();

}

