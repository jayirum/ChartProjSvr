
#include "StratMaker.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_INC/IRUM_Common.h"

extern CLogMsg g_log;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
CStratMaker::CStratMaker(char* pzSymbol, char* pzOpenPrc, CMemPool* pMemPool, unsigned dwSaveThread, unsigned dwSendThread):CBaseThread("SratMaker")
{
	m_dwSaveThread = dwSaveThread;
	m_dwSendThread = dwSendThread;
	m_pMemPool = pMemPool;
	
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zOpenPrc, pzOpenPrc);

	//m_pSignalResult = new char[LEN_BUFF_SIZE];
	//m_pClientBuff = new char[LEN_BUFF_SIZE];
	ResumeThread();

	// worker thread 생성
	m_hWorkDie = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hStratThread = (HANDLE)_beginthreadex(NULL, 0, &StratThread, this, 0, &m_dwStratThreadID);
}


CStratMaker::~CStratMaker()
{
	SetEvent(m_hWorkDie);
	if (WaitForSingleObject(m_hStratThread, 3000) != WAIT_OBJECT_0) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hStratThread, dwExitCode);
	}
	SAFE_CLOSEHANDLE(m_hWorkDie);
	SAFE_CLOSEHANDLE(m_hStratThread);


	//EndSubScribe();
	//EndShm();
	//EndDB();
	//delete[] m_pSignalResult;
	//delete[] m_pClientBuff;
}

//BOOL CStratMaker::BeginShm()
//{
//	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
//	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);
//
//	//	OPEN SHM
//	m_pShmQ = new CQueueShm();
//	if (!m_pShmQ->Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
//	{
//		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN 에러(%s)(symbol:%s)(%s)", m_zShmNm, m_zSymbol, m_pShmQ->GetErr());
//		return FALSE;
//	}
//	g_log.log(LOGTP_SUCC, "SHM OPEN 성공(%s)", m_zMutexNm);
//
//
//	return TRUE;
//}

//

//BOOL CStratMaker::BeginDB()
//{
//	char ip[32], id[32], pwd[32], cnt[32], name[32];
//	//CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
//	//CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
//	//CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
//	//CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
//	//CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);
//
//	//m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
//	//if (!m_pDBPool->Init(1))
//	//{
//	//	g_log.log(LOGTP_ERR, "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
//	//	g_log.log(LOGTP_ERR, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
//	//	return FALSE;
//	//}
//	//g_log.log(LOGTP_ERR, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
//	return TRUE;
//}

//VOID CStratMaker::EndDB()
//{
//	//SAFE_DELETE(m_pDBPool);
//}
//
//void CStratMaker::EndShm()
//{
//	m_pShmQ->Close();
//
//	//TODO 
//	SAFE_DELETE(m_pShmQ);
//}



VOID CStratMaker::ThreadFunc()
{
	__try {
		MainFunc();
	}
	__except (ReportException(GetExceptionCode(), "CStratMaker::ThreadFunc", m_szMsg))
	{
		g_log.log(LOGTP_FATAL, m_szMsg);
		exit(0);
	}

}

VOID CStratMaker::MainFunc()
{
	g_log.log(LOGTP_SUCC, "[THREAD ID:%d](%s)Start...\n", GetMyThreadID(), m_zSymbol);
	printf("[CStratMaker THREAD ID:%d](%s)Start...", GetMyThreadID(), m_zSymbol);
	
	//if (!BeginDB())
	//	return;

	//if (!BeginShm())
	//	return ;

	//if (!LoadSymbolSpec())
	//{
	//	return;
	//}

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


unsigned WINAPI CStratMaker::StratThread(LPVOID lp)
{
	CStratMaker* p = (CStratMaker*)lp;
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
				p->StratProc(pData);
				p->m_pMemPool->release(pData);
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}


VOID CStratMaker::StratProc(char* pMarketData)
{
	char zNowPackTime[32] = { 0, };
	char zPackDT[32] = { 0, };
	char zCurrPrc[LEN_PRC + 1] = { 0, };

	ST_PACK2CHART_EX	*pPack = (ST_PACK2CHART_EX*)pMarketData;

	sprintf(zCurrPrc, "%.*s", sizeof(pPack->Close), pPack->Close);

	// Current Time
	sprintf(zNowPackTime, "%.8s", pPack->Time);
	sprintf(zPackDT, "%.*s", sizeof(pPack->Date), pPack->Date);
	
	
	// If this is the first time, don't proceed
	//if (m_bIsFirstRunning == TRUE)
	//{
	//	m_bIsFirstRunning = FALSE;
	//	return;
	//}

	char cCurrOpenSide = m_stratHist.IsOpenSrategyExist();

	switch (cCurrOpenSide)
	{
	case CD_BUY:
	case CD_SELL:
		StratClose(cCurrOpenSide, zCurrPrc, zPackDT, zNowPackTime);
		break;
	default:
		StratOpen(zCurrPrc, zPackDT, zNowPackTime);
		break;
	}

}

/*
	STRATID_BUY_OPEN	Tick is 0.1% higher then Open price 
	STRATID_SELL_OPEN	Tick is 0.1% lower then Open price

	typedef struct _ST_MF_STRAT_ORD
	{
	char	Symbol[LEN_SYMBOL];
	char	StratID[32];
	char	ClrTp[1];				// CD_OPEN,CD_CLOSE
	char	Side[1];				// CD_BUY, CD_SELL
	char	OpenPrc[LEN_PRC];
	char	BasePrc[LEN_PRC];
	char	CurrPrc[LEN_PRC];
	char	BestPrc[LEN_PRC];		// 진입 후 최고좋은 가격
	char	BestPL[LEN_PL];			// 최고가격의 이익
	char	CurrPL[LEN_PL];			// 현재가격의 이익
	char	OrdProtTp[1];			// CD_ORD_TP_MARKET, CD_ORD_TP_LIMIT
	char	OrdTM[12];				// hh:mm:ss.mmm
	char	ApiDT[8];
	char	ApiTM[8];	//hh:mm:ss
	char	Note[20];
	}ST_MF_STRAT_ORD;
*/
VOID CStratMaker::StratOpen(char* pzCurrPrc, char* pzApiDT, char* pzApiTm)
{
	char zBasePrc[128];

	double dCurrPrc = atof(pzCurrPrc);
	double dOpenPrc = atof(m_zOpenPrc);
	double dUpperPrc = dOpenPrc * (1 + 0.001);
	double dLowerPrc = dOpenPrc * (1 - 0.001);

	int nCondition = 0;

	if (dCurrPrc >= dUpperPrc) nCondition = 1;
	if (dCurrPrc <= dLowerPrc) nCondition = 2;

	if (!nCondition)
		return;

	ST_MF_STRAT_ORD stOrd;
	int nStructSize = sizeof(ST_MF_STRAT_ORD);
	memset(&stOrd, 0x20, nStructSize);
	memcpy(stOrd.Symbol, m_zSymbol, strlen(m_zSymbol));

	char zMsg1[128];
	char zStratID[32];
	if (nCondition == 1) {
		strcpy(zStratID, STRATID_BUY_OPEN);
		stOrd.Side[0] = CD_BUY;
		sprintf(zBasePrc, "%.2f", dUpperPrc);
		
		sprintf(zMsg1, "[매수진입][%.2f >= %.2f] (CurrPrc >= BasePrc(0.1 Percent) [API TM:%s]", 
			dCurrPrc, dUpperPrc, pzApiTm);
	}
	else {
		strcpy(zStratID, STRATID_BUY_OPEN);
		stOrd.Side[0] = CD_SELL;
		sprintf(zBasePrc, "%.2f", dLowerPrc);

		sprintf(zMsg1, "[매도진입][%.2f <= %.2f] (CurrPrc <= BasePrc(0.1 Percent)[API TM:%s]", 
			dCurrPrc, dLowerPrc, pzApiTm);
	}
	memcpy(stOrd.StratID, zStratID, strlen(zStratID));
	stOrd.ClrTp[0] = CD_OPEN;
	
	memcpy(stOrd.OpenPrc, m_zOpenPrc, strlen(m_zOpenPrc));
	memcpy(stOrd.BasePrc, zBasePrc, strlen(zBasePrc));
	memcpy(stOrd.CurrPrc, pzCurrPrc, strlen(pzCurrPrc));
	//memcpy(stOrd.BestPrc, m_zSymbol, strlen(m_zSymbol));
	//memcpy(stOrd.BestPL, m_zSymbol, strlen(m_zSymbol));
	//memcpy(stOrd.CurrPL, m_zSymbol, strlen(m_zSymbol));
	stOrd.OrdProtTp[0] = CD_ORD_TP_MARKET;

	char tmp[128];
	strcpy(tmp, CUtil::Get_NowTime());
	memcpy(stOrd.OrdTM, tmp, strlen(tmp));

	memcpy(stOrd.ApiDT, pzApiDT, strlen(pzApiDT));
	memcpy(stOrd.ApiTM, pzApiTm, strlen(pzApiTm));

	int nLen = strlen(zMsg1);
	memcpy(stOrd.Note, zMsg1, strlen(zMsg1));

	m_stratHist.SetStrategyExist(zStratID);


	//PostThreadMessage
	char* pData = NULL;
	if (m_pMemPool->get(&pData))
	{
		memcpy(pData, (void*)&stOrd, nStructSize);
		PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);
	}
}


/*
	StopLoss, ProfitTaking 

	return value:
	enum { SLPT_NONE=0, SL_BUY, SL_SELL, PT_BUY, PT_SELL};

*/

char* CStratMaker::GetCloseOrdType(char cCurrOpenSide, char* pzCurrPrc, 
	_Out_ double* pBasePrc, _Out_ char* pzStratID)
{
	double dCurrPrc = atof(pzCurrPrc);
	double dOpenPrc = atof(m_zOpenPrc);	
	strcpy(pzStratID, STRATID_NONE);

	// BUY OPEN
	if (cCurrOpenSide == CD_BUY)
	{
		// SL SELL : CurrPrc <= Open Prc
		*pBasePrc = dOpenPrc;
		int nComp = CUtil::CompPrc(dCurrPrc, dOpenPrc, 2/*TODO*/, LEN_PRC);
		if (nComp <= 0)
			strcpy(pzStratID, STRATID_SELL_SL);

		// PT SELL : CurrPrc >= OpenPrc + (OpenPrc * 0.005)
		*pBasePrc = dOpenPrc + (dOpenPrc*0.005);
		nComp = CUtil::CompPrc(dCurrPrc, *pBasePrc, 2/*TODO*/, LEN_PRC);
		if (nComp >= 0)
			strcpy(pzStratID, STRATID_SELL_PT);
	}

	// SELL OPEN
	if (cCurrOpenSide == CD_SELL)
	{
		// SL BUY : CurrPrc >= Open Prc
		*pBasePrc = dOpenPrc;
		int nComp = CUtil::CompPrc(dCurrPrc, dOpenPrc, 2/*TODO*/, LEN_PRC);
		if (nComp >= 0)
			strcpy(pzStratID, STRATID_BUY_SL);

		// PT BUY : CurrPrc <= OpenPrc + (OpenPrc * 0.005)
		*pBasePrc = dOpenPrc - (dOpenPrc*0.005);
		nComp = CUtil::CompPrc(dCurrPrc, *pBasePrc, 2/*TODO*/, LEN_PRC);
		if (nComp <= 0)
			strcpy(pzStratID, STRATID_BUY_PT);
	}
	return pzStratID;
}

VOID CStratMaker::StratClose(char cCurrOpenSide, char* pzCurrPrc, char* pzApiDT, char* pzApiTm)
{
	double dBasePrc = 0;
	char zBasePrc[32];
	char zStratID[32] = { 0, };
	GetCloseOrdType(cCurrOpenSide, pzCurrPrc, &dBasePrc, zStratID);

	if (strcmp(zStratID, STRATID_NONE) == 0)
	{
		//TODO LOGGING
		return;
	}

	ST_MF_STRAT_ORD stOrd;
	int nStructSize = sizeof(ST_MF_STRAT_ORD);
	memset(&stOrd, 0x20, nStructSize);
	memcpy(stOrd.Symbol, m_zSymbol, strlen(m_zSymbol));


	sprintf(zBasePrc, "%.*f", 2/*TODO*/, dBasePrc);
	char zMsg1[128];
	if (strcmp(zStratID, STRATID_BUY_SL) == 0)
	{
		stOrd.Side[0] = CD_BUY;
		sprintf(zMsg1, "[매수손절 of SHORT][%s >= %s] (CurrPrc >= OpenPrc) [API TM:%s]",
			pzCurrPrc, m_zOpenPrc, pzApiTm);
	}
	if (strcmp(zStratID, STRATID_BUY_PT) == 0)
	{
		stOrd.Side[0] = CD_BUY;
		sprintf(zMsg1, "[매수익절 of SHORT][%s <= %s] (CurrPrc <= BasePrc(0.5 Percent) [API TM:%s]",
			pzCurrPrc, zBasePrc, pzApiTm);
	}
	if (strcmp(zStratID, STRATID_SELL_SL) == 0)
	{
		stOrd.Side[0] = CD_SELL;
		sprintf(zMsg1, "[매도손절 of LONG ][%s >= %s] (CurrPrc <= OpenPrc) [API TM:%s]",
			pzCurrPrc, m_zOpenPrc, pzApiTm);
	}

	if (strcmp(zStratID, STRATID_SELL_PT) == 0)
	{
		stOrd.Side[0] = CD_SELL;
		sprintf(zMsg1, "[매도익절 of LONG ][%s >= %s] (CurrPrc >= BasePrc(0.5 Percent) [API TM:%s]",
			pzCurrPrc, zBasePrc, pzApiTm);
	}
	memcpy(stOrd.StratID, zStratID, strlen(zStratID));
	stOrd.ClrTp[0] = CD_CLOSE;

	memcpy(stOrd.OpenPrc, m_zOpenPrc, strlen(m_zOpenPrc));
	memcpy(stOrd.BasePrc, zBasePrc, strlen(zBasePrc));
	memcpy(stOrd.CurrPrc, pzCurrPrc, strlen(pzCurrPrc));
	//memcpy(stOrd.BestPrc, m_zSymbol, strlen(m_zSymbol));
	//memcpy(stOrd.BestPL, m_zSymbol, strlen(m_zSymbol));
	//memcpy(stOrd.CurrPL, m_zSymbol, strlen(m_zSymbol));
	stOrd.OrdProtTp[0] = CD_ORD_TP_MARKET;

	char tmp[128];
	strcpy(tmp, CUtil::Get_NowTime());
	memcpy(stOrd.OrdTM, tmp, strlen(tmp));

	memcpy(stOrd.ApiDT, pzApiDT, strlen(pzApiDT));
	memcpy(stOrd.ApiTM, pzApiTm, strlen(pzApiTm));

	memcpy(stOrd.Note, zMsg1, strlen(zMsg1));

	m_stratHist.SetStrategyExist(zStratID);


	//PostThreadMessage
	char* pData = NULL;
	if (m_pMemPool->get(&pData))
	{
		memcpy(pData, (void*)&stOrd, nStructSize);
		PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);
	}
}


/*
	Post message to main thread to send data to client
*/
VOID CStratMaker::SendSaveSignal(_In_ const char* pSignalPacket, int nDataLen)
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




//
//BOOL	CStratMaker::LoadSymbolSpec()
//{
//	//char sApplyYN[2];
//	//CDBHandlerAdo db(m_pDBPool->Get());
//
//	//char sQ[1024];
//	//sprintf(sQ, "SELECT TICK_SIZE, DOT_CNT FROM TRADE_SECURITY_ARTC WHERE ARTC_CODE='%s'", m_zArtc );
//	//if (FALSE == db->ExecQuery(sQ))
//	//{
//	//	g_log.log(LOGTP_ERR, "Load Symbol Info failed(%s)(%s)", sQ, db->GetError());
//	//	return FALSE;
//	//}
//
//	//if (db->IsNextRow())
//	//{
//	//	m_SymbolInfo.nDotCnt = db->GetLong("DOT_CNT");
//	//	sprintf(m_SymbolInfo.zTickSize, "%.*f", m_SymbolInfo.nDotCnt, db->GetDouble("TICK_SIZE"));
//	//	
//	//} // if (db->NextRow())
//	//else
//	//{
//	//	g_log.log(LOGTP_ERR, "There is no symbol info (%s)", sQ);
//	//	//m_bDoStrategy = FALSE;
//	//	return FALSE;
//	//}
//
//	return TRUE;
//}

//BOOL CStratMaker::BeginSubscribe()
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
//VOID CStratMaker::EndSubScribe()
//{
//	//MCAST
//	//m_pMDSub->End();
//	//SAFE_DELETE(m_pMDSub);
//	SAFE_DELETE(m_pMcastRecv);
//}