
#include "SignalMaker.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "Main.h"

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];
extern CMemPool	*g_pMemPool;
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
CSignalMaker::CSignalMaker(char* pzSymbol, char* pzArtcCode, CMemPool* pMemPool, /*CQueueShm* pShm,*/ unsigned dwSaveThread, unsigned dwSendThread):CBaseThread("CSignalMaker")
{
	m_dwSaveThread = dwSaveThread;
	m_dwSendThread = dwSendThread;
	m_pMemPool = pMemPool;
	
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zArtc, pzArtcCode);
	GET_SHM_NM(pzArtcCode, m_zShmNm);
	GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);
	m_nChartTp = TP_10MIN;	// TP_5MIN;	//TODO
	GET_GROUP_KEY(m_zSymbol, m_nChartTp, m_zGroupKey);
	

	m_pShmQ = NULL;	
	m_pDBPool = NULL;
	
	m_bIsFirstRunning = TRUE;
	m_pSignalResult = new char[LEN_BUFF_SIZE];
	m_pClientBuff = new char[LEN_BUFF_SIZE];
	ResumeThread();

	// worker thread 생성
	m_hWorkDie = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hStratThread = (HANDLE)_beginthreadex(NULL, 0, &StratThread, this, 0, &m_dwStratThreadID);
}


CSignalMaker::~CSignalMaker()
{
	SetEvent(m_hWorkDie);
	if (WaitForSingleObject(m_hStratThread, 3000) != WAIT_OBJECT_0) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hStratThread, dwExitCode);
	}
	SAFE_CLOSEHANDLE(m_hWorkDie);
	SAFE_CLOSEHANDLE(m_hStratThread);


	//EndSubScribe();
	EndShm();
	EndDB();
	delete[] m_pSignalResult;
	delete[] m_pClientBuff;
}

BOOL CSignalMaker::BeginShm()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);

	//	OPEN SHM
	m_pShmQ = new CQueueShm();
	if (!m_pShmQ->Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN 에러(%s)(symbol:%s)(%s)", m_zShmNm, m_zSymbol, m_pShmQ->GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN 성공(%s)", m_zMutexNm);


	return TRUE;
}

//

BOOL CSignalMaker::BeginDB()
{
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
	if (!m_pDBPool->Init(1))
	{
		g_log.log(LOGTP_ERR, "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
		g_log.log(LOGTP_ERR, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
		return FALSE;
	}
	g_log.log(LOGTP_ERR, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
	return TRUE;
}

VOID CSignalMaker::EndDB()
{
	SAFE_DELETE(m_pDBPool);
}

void CSignalMaker::EndShm()
{
	m_pShmQ->Close();

	//TODO 
	SAFE_DELETE(m_pShmQ);
}



VOID CSignalMaker::ThreadFunc()
{
	__try {
		MainFunc();
	}
	__except (ReportException(GetExceptionCode(), "CSignalMaker::ThreadFunc", m_szMsg))
	{
		g_log.log(LOGTP_FATAL, m_szMsg);
		exit(0);
	}

}

VOID CSignalMaker::MainFunc()
{
	g_log.log(LOGTP_SUCC, "[THREAD ID:%d](%s)Start...\n", GetMyThreadID(), m_zSymbol);
	printf("[CSignalMaker THREAD ID:%d](%s)Start...", GetMyThreadID(), m_zSymbol);
	
	if (!BeginDB())
		return;

	if (!BeginShm())
		return ;

	if (!LoadSymbolSpec())
	{
		return;
	}

	char zMarketDataBuff[1024] = { 0, };
	char zMsg[1024] = { 0, };
	char zRecvSymbol[128] = { 0, };
	int nSymbolLen = strlen(m_zSymbol);

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) 
		{
			g_log.log(LOGTP_ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
			continue;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_MANAGER_NOTIFY_SOCK:
			{
				ST_MANAGER_SOCKNOTIFY* p = (ST_MANAGER_SOCKNOTIFY*)msg.lParam;
				delete p;
				break;
			}
			case WM_CHART_ALL_KILL:
			{
				g_log.log(LOGTP_SUCC, "[THREAD ID:%d] Recv Kill Msg", GetMyThreadID());
				break;
			}
			//case WM_CHART_DATA:
			//{
			//	PostThreadMessage(m_dwStratThreadID, WM_RECV_API_MD, msg.wParam, msg.lParam);
			//	break;
			//}
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))


		
	} // while (TRUE)
	
}


unsigned WINAPI CSignalMaker::StratThread(LPVOID lp)
{
	CSignalMaker* p = (CSignalMaker*)lp;
	while (WaitForSingleObject(p->m_hWorkDie, 1) != WAIT_OBJECT_0)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_RECV_API_MD)
			{
				char* pData = (char*)msg.lParam;
				//printf("work recv(%.40s)\n", pData);
				int nLen = (int)msg.wParam;
				p->SignalProc(pData);
				p->m_pMemPool->release(pData);
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}


VOID CSignalMaker::SignalProc(char* pMarketData)
{
	char zNowPackTime[32] = { 0, };
	char zPackDT[32] = { 0, };
	char zNowChartNm[32] = { 0, };
	char zCurrPrc[LEN_PRC + 1] = { 0, };

	ST_PACK2CHART_EX	*pPack = (ST_PACK2CHART_EX*)pMarketData;

	sprintf(zCurrPrc, "%.*s", sizeof(pPack->Close), pPack->Close);

	// Current Time
	sprintf(zNowPackTime, "%.8s", pPack->Time);
	sprintf(zPackDT, "%.*s", sizeof(pPack->Date), pPack->Date);
	
	GET_CHART_NM_EX(zPackDT, zNowPackTime, m_nChartTp, zNowChartNm);
	//g_log.log(LOGTP_SUCC, "현재시간(%s),차트이름(%s)", zNowPackTime, zNowChartNm);


	// If this is the first time, don't proceed
	if (m_bIsFirstRunning == TRUE)
	{
		m_bIsFirstRunning = FALSE;
		return;
	}

	// Buffer to read chart shm
	ST_SHM_CHART_UNIT	nowChart, prevChart;

	// Get Last ChartShm and previous ChartShm	
	INT nRet = GetCurrChartShm(m_zGroupKey, zNowChartNm, prevChart, nowChart);
	if (nRet<0) {
		if (nRet == -1) {
			g_log.log(LOGTP_ERR, "[SignalProc]Get Shm Info error(Packet Time:%s,Symbol:%.5s) (GroupKey:%s, ChartNM:%s)",
				zNowPackTime, pPack->ShortCode, m_zGroupKey, zNowChartNm);
		}
		return;
	}

	char zStratID[32] = { 0, };
	
	m_stratHist.WhichStratShouldbeChecked(zNowChartNm, zStratID);

	if( strcmp(zStratID,STRAT_OPEN)==0 )
	{
		StratOpen(m_zGroupKey, zNowChartNm, prevChart, nowChart, zCurrPrc, zPackDT, zNowPackTime);
	}

	if (strcmp(zStratID, STRATID_SELL_CLOSE) == 0 || 
		strcmp(zStratID, STRATID_BUY_CLOSE) == 0 )
	{
		StratClose(m_zGroupKey, zNowChartNm, prevChart, nowChart, zCurrPrc, zPackDT, zNowPackTime);
	}

}

/*
	STRATID_BUY_OPEN	Tick is 0.1% higher then Open price 
	STRATID_SELL_OPEN	Tick is 0.1% lower then Open price


*/
VOID CSignalMaker::StratOpen(
	char* pzGroupKey, char* pzChartNm, 
	_In_ ST_SHM_CHART_UNIT& shmPrev, _In_ ST_SHM_CHART_UNIT& shmNow, 
	char* pzCurrPrc,	char* pzApiDT, char* pzApiTM)
{
	char zOpenPrc[LEN_PRC + 1];
	sprintf(zOpenPrc, "%.*s", sizeof(shmNow.open), shmNow.open);
	double dCurrPrc = atof(pzCurrPrc);
	int nComp = CUtil::CompPrc(pzCurrPrc, strlen(pzCurrPrc), zOpenPrc, strlen(zOpenPrc), m_SymbolInfo.nDotCnt, LEN_PRC);

	if (nComp == 0)
		return;

	char tmp[128];

	// check buy
	if (nComp > 0)
	{
		//TODO double dBasePrc = atof(zOpenPrc) * (1 + 0.001);
		double dBasePrc = atof(zOpenPrc) + 0.02;
		nComp = CUtil::CompPrc(dCurrPrc, dBasePrc, m_SymbolInfo.nDotCnt, LEN_PRC);
		if (nComp > 0)
		{
			ST_STRAT_REAL_CLIENT stSave;
			memset(&stSave, 0x20, sizeof(stSave));
			
			memcpy(stSave.Symbol, m_zSymbol,strlen(m_zSymbol));
			memcpy(stSave.GroupKey, pzGroupKey, strlen(pzGroupKey));
			memcpy(stSave.ChartNm, pzChartNm, strlen(pzChartNm));
			memcpy(stSave.StratID, STRATID_BUY_OPEN, strlen(STRATID_BUY_OPEN));
			memcpy(stSave.StratPrc, pzCurrPrc, strlen(pzCurrPrc));
			memcpy(stSave.ApiDT, pzApiDT, strlen(pzApiDT));
			memcpy(stSave.ApiTM, pzApiTM, strlen(pzApiTM));
			sprintf(tmp, "OpenPrc:%.*s, BasePrc:%*f, CurrPrc:%.*s"
				, LEN_PRC, zOpenPrc, LEN_PRC, dBasePrc, LEN_PRC, pzCurrPrc);
			memcpy(stSave.Note, tmp, strlen(tmp));
			stSave.EOL[0] = DEF_EOL;

			m_stratHist.SetStrategyExist(pzChartNm, STRATID_BUY_OPEN);
			SendSaveSignal((char*)&stSave, sizeof(stSave));
			g_log.log(LOGTP_SUCC, "STRATID_BUY_OPEN(%.100s)", stSave.Note);
			printf("STRATID_BUY_OPEN(%.80s)\n", stSave.Note);
		}
	}

	// check sell
	if (nComp < 0)
	{
		//TODO double dBasePrc = atof(zOpenPrc) * (1 - 0.001);
		double dBasePrc = atof(zOpenPrc)  - 0.02;
		nComp = CUtil::CompPrc(dCurrPrc, dBasePrc, m_SymbolInfo.nDotCnt, LEN_PRC);
		if (nComp < 0)
		{
			ST_STRAT_REAL_CLIENT stSave;
			memset(&stSave, 0x20, sizeof(stSave));

			memcpy(stSave.Symbol, m_zSymbol, strlen(m_zSymbol));
			memcpy(stSave.GroupKey, pzGroupKey, strlen(pzGroupKey));
			memcpy(stSave.ChartNm, pzChartNm, strlen(pzChartNm));
			memcpy(stSave.StratID, STRATID_SELL_OPEN, strlen(STRATID_SELL_OPEN));
			memcpy(stSave.StratPrc, pzCurrPrc, strlen(pzCurrPrc));
			memcpy(stSave.ApiDT, pzApiDT, strlen(pzApiDT));
			memcpy(stSave.ApiTM, pzApiTM, strlen(pzApiTM));
			sprintf(tmp, "OpenPrc:%.*s, BasePrc:%*f, CurrPrc:%.*s"
				, LEN_PRC, zOpenPrc, LEN_PRC, dBasePrc, LEN_PRC, pzCurrPrc);
			memcpy(stSave.Note, tmp, strlen(tmp));
			stSave.EOL[0] = DEF_EOL;

			m_stratHist.SetStrategyExist(pzChartNm, STRATID_SELL_OPEN);
			SendSaveSignal((char*)&stSave, sizeof(stSave));
			g_log.log(LOGTP_SUCC, "STRATID_SELL_OPEN(%.100s)", stSave.Note);
			printf("STRATID_SELL_OPEN(%.80s)\n", stSave.Note);

		}
	}
}


VOID CSignalMaker::StratClose(
	char* pzGroupKey, char* pzChartNm, 
	_In_ ST_SHM_CHART_UNIT& shmPrev, _In_ ST_SHM_CHART_UNIT& shmNow,
	char* pzCurrPrc, char* pzApiDT, char* pzApiTM)
{
	char tmp[128];
	char zOpenPrc[32] = { 0, };
	sprintf(zOpenPrc, "%.*s", sizeof(shmNow.open), shmNow.open);

	int nComp = CUtil::CompPrc(zOpenPrc, strlen(zOpenPrc), pzCurrPrc, strlen(pzCurrPrc), m_SymbolInfo.nDotCnt, LEN_PRC);

	// open prc > curr prc ==> sell close
	if (nComp > 0)
	{
		// check buy open
		if (m_stratHist.IsSrategyExist(pzChartNm, STRATID_BUY_OPEN))
		{
			ST_STRAT_REAL_CLIENT stSave;
			memset(&stSave, 0x20, sizeof(stSave));

			memcpy(stSave.Symbol, m_zSymbol, strlen(m_zSymbol));
			memcpy(stSave.GroupKey, pzGroupKey, strlen(pzGroupKey));
			memcpy(stSave.ChartNm, pzChartNm, strlen(pzChartNm));
			memcpy(stSave.StratID, STRATID_SELL_CLOSE, strlen(STRATID_SELL_CLOSE));
			memcpy(stSave.StratPrc, pzCurrPrc, strlen(pzCurrPrc));
			memcpy(stSave.ApiDT, pzApiDT, strlen(pzApiDT));
			memcpy(stSave.ApiTM, pzApiTM, strlen(pzApiTM));
			sprintf(tmp, "OpenPrc:%.*s > CurrPrc:%.*s", LEN_PRC, zOpenPrc, LEN_PRC, pzCurrPrc);
			memcpy(stSave.Note, tmp, strlen(tmp));
			stSave.EOL[0] = DEF_EOL;

			m_stratHist.SetStrategyExist(pzChartNm, STRATID_SELL_CLOSE);
	
			//	SAVE ON DB, SAVE IN StratHistManager
			SendSaveSignal((char*)&stSave, sizeof(stSave));
			g_log.log(LOGTP_SUCC, "STRATID_SELL_CLOSE(%.100s)", stSave.Note);
			printf("STRATID_SELL_CLOSE(%.80s)\n", stSave.Note);

		}
	}
	// open prc < curr prc ==> buy close
	else if (nComp < 0)
	{
		// check buy open
		if (m_stratHist.IsSrategyExist(pzChartNm, STRATID_SELL_OPEN))
		{
			ST_STRAT_REAL_CLIENT stSave;
			memset(&stSave, 0x20, sizeof(stSave));

			memcpy(stSave.Symbol, m_zSymbol, strlen(m_zSymbol));
			memcpy(stSave.GroupKey, pzGroupKey, strlen(pzGroupKey));
			memcpy(stSave.ChartNm, pzChartNm, strlen(pzChartNm));
			memcpy(stSave.StratID, STRATID_BUY_CLOSE, strlen(STRATID_BUY_CLOSE));
			memcpy(stSave.StratPrc, pzCurrPrc, strlen(pzCurrPrc));
			memcpy(stSave.ApiDT, pzApiDT, strlen(pzApiDT));
			memcpy(stSave.ApiTM, pzApiTM, strlen(pzApiTM));
			sprintf(tmp, "OpenPrc:%.*s < CurrPrc:%.*s", LEN_PRC, zOpenPrc, LEN_PRC, pzCurrPrc);
			memcpy(stSave.Note, tmp, strlen(tmp));
			stSave.EOL[0] = DEF_EOL;

			m_stratHist.SetStrategyExist(pzChartNm, STRATID_BUY_CLOSE);

			//	SAVE ON DB, SAVE IN StratHistManager
			SendSaveSignal((char*)&stSave, sizeof(stSave));
			g_log.log(LOGTP_SUCC, "STRATID_BUY_CLOSE(%.100s)", stSave.Note);
			printf("STRATID_BUY_CLOSE(%.80s)\n", stSave.Note);

		}
	}
}


/*
	Post message to main thread to send data to client
*/
VOID CSignalMaker::SendSaveSignal(_In_ const char* pSignalPacket, int nDataLen)
{
	char* pData = NULL;
	if (m_pMemPool->get(&pData))
	{
		memcpy(pData, pSignalPacket, nDataLen);
		PostThreadMessage(m_dwSaveThread, WM_SEND_STRATEGY, (WPARAM)nDataLen, (LPARAM)pData);
	}
	//TODO
	/*char *pData2 = m_pMemPool->get();
	memcpy(pData, pSignalPacket, nDataLen);
	PostThreadMessage(m_dwSendThread, WM_SEND_STRATEGY, (WPARAM)nDataLen, (LPARAM)pData2);*/
}


// Get the current chart & previous chart
INT CSignalMaker::GetCurrChartShm(
	char* pzGroupKey, char* pzChartNm, _Out_ ST_SHM_CHART_UNIT& shmPrev, _Out_ ST_SHM_CHART_UNIT& shmNow)
{
	char szChart[LEN_CHART_NM + 1];
	int nLoop = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the current chart
	BOOL bExist = m_pShmQ->DataGet(pzGroupKey, pzChartNm, (char*)&shmNow);

	// retry 2 times
	while(FALSE == bExist)
	{
		Sleep(1000);
		if (++nLoop > 2) {
			// There is no chart of the current time. Something is wrong.
			g_log.log(LOGTP_ERR, "[%s][%s] No Chart even if receive curr price.", pzGroupKey, pzChartNm);
			return -1;
		}
		bExist = m_pShmQ->DataGet(pzGroupKey, pzChartNm, (char*)&shmNow);

	} // if(!bExist)

	 
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the prev chart
	sprintf(szChart, "%.*s", LEN_CHART_NM, shmNow.prevNm);

	// NONE means the current chart is the 1st chart
	if (strcmp(szChart, "NONE") == 0)
		return -3;

	if (FALSE == m_pShmQ->DataGet(pzGroupKey, szChart, (char*)&shmPrev))
	{
		g_log.log(LOGTP_ERR, "(%s)(%.*s) There is no prev chart in SHM 에 없다", pzGroupKey, LEN_CHART_NM, szChart);
		return -2;
	}
	//g_log.log(LOGTP_SUCC, "직전차트(%.4s)", chart[0].Nm);


	return 1;
}





BOOL	CSignalMaker::LoadSymbolSpec()
{
	//char sApplyYN[2];
	CDBHandlerAdo db(m_pDBPool->Get());

	char sQ[1024];
	sprintf(sQ, "SELECT TICK_SIZE, DOT_CNT FROM TRADE_SECURITY_ARTC WHERE ARTC_CODE='%s'", m_zArtc );
	if (FALSE == db->ExecQuery(sQ))
	{
		g_log.log(LOGTP_ERR, "Load Symbol Info failed(%s)(%s)", sQ, db->GetError());
		return FALSE;
	}

	if (db->IsNextRow())
	{
		m_SymbolInfo.nDotCnt = db->GetLong("DOT_CNT");
		sprintf(m_SymbolInfo.zTickSize, "%.*f", m_SymbolInfo.nDotCnt, db->GetDouble("TICK_SIZE"));
		
	} // if (db->NextRow())
	else
	{
		g_log.log(LOGTP_ERR, "There is no symbol info (%s)", sQ);
		//m_bDoStrategy = FALSE;
		return FALSE;
	}

	return TRUE;
}

//BOOL CSignalMaker::BeginSubscribe()
//{
//	//MCAST
//	//SAFE_DELETE(m_pMDSub);
//
//	//m_pMDSub = new CNanoPubSub(TP_SUB);
//	//return (m_pMDSub->Begin(INNER_CHANNEL_PUBSUB));
//
//	m_pMcastRecv = new CMCastRecv;
//
//	char zLocalIP[32], zMcastIP[32], port[32];
//	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP", zLocalIP);
//	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP_MCAST", zMcastIP);
//	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "PORT", port);
//
//	BOOL bRet = m_pMcastRecv->Begin(zLocalIP, zMcastIP, atoi(port));
//	if (bRet)
//		g_log.log(LOGTP_SUCC, "[%s]시세수신 Init 성공(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", zLocalIP, zMcastIP, port);
//	else
//		g_log.log(LOGTP_ERR, "[%s]시세수신 Init 실패(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", zLocalIP, zMcastIP, port);
//	return bRet;
//}


//
//VOID CSignalMaker::EndSubScribe()
//{
//	//MCAST
//	//m_pMDSub->End();
//	//SAFE_DELETE(m_pMDSub);
//	SAFE_DELETE(m_pMcastRecv);
//}