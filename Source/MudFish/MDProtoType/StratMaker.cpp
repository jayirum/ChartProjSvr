
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
CStratMaker::CStratMaker(
	char* pzSymbol, char* pzOpenPrc, CMemPool* pMemPool, unsigned dwSaveThread, unsigned dwSendThread
	, char* pzCloseTM, char* pzMaxSLCnt):CBaseThread("SratMaker")
{
	m_dwSaveThread = dwSaveThread;
	m_dwSendThread = dwSendThread;
	m_pMemPool = pMemPool;
	
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zOpenPrc, pzOpenPrc);
	strcpy(m_zCloseTM, pzCloseTM);
	strcpy(m_zMaxSLCnt, pzMaxSLCnt);

	m_nMarketStatus = MARKET_ON;

	m_stratHist = new CStratHistManager(pzMaxSLCnt, 2);	//TODO

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
	CUtil::logOutput("WORKER THREAD ID : %d", GetCurrentThreadId());
	CStratMaker* p = (CStratMaker*)lp;
	while (WaitForSingleObject(p->m_hWorkDie, 1) != WAIT_OBJECT_0)
	{
		//CHECK Market Close 
		p->CheckMarketClosing();

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_MARKET_CLOSE)
			{
				char* pData = (char*)msg.lParam;
				strcpy(p->m_zCloseTM, pData);
				p->m_pMemPool->release(pData);
			}
			if (msg.message == WM_RECV_API_MD)
			{
				char* pData = (char*)msg.lParam;
				//CUtil::logOutput("WM_RECV_API_MD(%s)", pData);
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
	strcpy(m_zLastCurrPrc, zCurrPrc);

	// Current Time
	sprintf(zNowPackTime, "%.8s", pPack->Time);
	sprintf(zPackDT, "%.*s", sizeof(pPack->Date), pPack->Date);
	
	if (m_nMarketStatus == MARKET_CLOSED || 
		m_nMarketStatus == MARKET_CLOSING )
		return;

	// 당일 2회손절
	//TODO if (m_stratHist->IsAlreadySLMaxCnt())
	//	return;

	//TODO. 이미 이익실현 했으면
	//if (m_stratHist->IsProfitRealized())
		//return;

	m_stratHist->SetMaxPLPrc(m_zLastCurrPrc);

	char cCurrOpenSide = m_stratHist->IsOpenSrategyExist();

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
	char zUpperPrc[32], zLowerPrc[32];
	sprintf(zUpperPrc, "%.2f", atof(m_zOpenPrc) * (1 + DEF_ENTRY_SPREAD));	//TODO. 2f
	sprintf(zLowerPrc, "%.2f", atof(m_zOpenPrc) * (1 - DEF_ENTRY_SPREAD));	//TODO. 2f

	int nCondition = 0;

	int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zUpperPrc, LEN_PRC, 2, LEN_PRC);	//TODO. 2
	if (nComp >= 0) nCondition = 1;

	nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zLowerPrc, LEN_PRC, 2, LEN_PRC);	//TODO. 2
	if (nComp <= 0) nCondition = 2;

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
		strcpy(zBasePrc, zUpperPrc);
		sprintf(zMsg1, "[매수진입][Curr:%s >= BasePrc:%s] (BasePrc = Open(%s)+(0.1 Percent) [API TM:%s]", 
			pzCurrPrc, zUpperPrc, m_zOpenPrc, pzApiTm);
	}
	else {
		strcpy(zStratID, STRATID_SELL_OPEN);
		stOrd.Side[0] = CD_SELL;
		strcpy(zBasePrc, zLowerPrc);

		sprintf(zMsg1, "[매도진입][Curr:%s <= BasePrc:%s] (BasePrc = Open(%s)-(0.1 Percent) [API TM:%s]",
			pzCurrPrc, zLowerPrc, m_zOpenPrc, pzApiTm);
	}
	memcpy(stOrd.StratID, zStratID, strlen(zStratID));
	stOrd.ClrTp[0] = CD_OPEN;
	
	memcpy(stOrd.OpenPrc, m_zOpenPrc, strlen(m_zOpenPrc));
	memcpy(stOrd.BasePrc, zBasePrc, strlen(zBasePrc));
	memcpy(stOrd.CurrPrc, pzCurrPrc, strlen(pzCurrPrc));
	memcpy(stOrd.OrdPrc, pzCurrPrc, strlen(pzCurrPrc));
	//memcpy(stOrd.MaxPLPrc, m_zSymbol, strlen(m_zSymbol));
	//memcpy(stOrd.MaxPL, m_zSymbol, strlen(m_zSymbol));
	//memcpy(stOrd.CurrPL, m_zSymbol, strlen(m_zSymbol));
	stOrd.OrdProtTp[0] = CD_ORD_TP_MARKET;

	char tmp[128];
	strcpy(tmp, CUtil::Get_NowTime());
	memcpy(stOrd.OrdTM, tmp, strlen(tmp));

	memcpy(stOrd.ApiDT, pzApiDT, strlen(pzApiDT));
	memcpy(stOrd.ApiTM, pzApiTm, strlen(pzApiTm));

	int nLen = strlen(zMsg1);
	memcpy(stOrd.Note, zMsg1, strlen(zMsg1));

	m_stratHist->SetStrategyExist(zStratID, pzCurrPrc, m_zOpenPrc);

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
	_Out_ char* pzBasePrc, _Out_ char* pzStratID, _Out_ char* pzClrMsg)
{
	double dOpenPrc = atof(m_zOpenPrc);	
	strcpy(pzStratID, STRATID_NONE);
	char zBasePrc[32];

	// BUY OPEN
	if (cCurrOpenSide == CD_BUY)
	{
		// SL SELL : CurrPrc <= Open Prc
		strcpy(zBasePrc, m_zOpenPrc);
		int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zBasePrc, LEN_PRC, 2/*TODO*/, LEN_PRC);
		if (nComp <= 0)
		{
			strcpy(pzStratID, STRATID_SELL_SL);
			sprintf(pzClrMsg, "LONG손절조건.(오픈가:%s, 진입가:%s, 현재가:%s)",
				m_zOpenPrc, m_stratHist->GetEntryPrc(), pzCurrPrc);
			CUtil::logOutput("%s\n", pzClrMsg);
		}

		else
		{
			if (m_stratHist->IsHitPTPrc())
			{
				if (m_stratHist->IsPTCondition(pzCurrPrc, pzClrMsg)) {
					g_log.log(LOGTP_SUCC, pzClrMsg);
					strcpy(pzStratID, STRATID_SELL_PT);
				}
			}
		}
	}

	// SELL OPEN
	if (cCurrOpenSide == CD_SELL)
	{
		// SL BUY : CurrPrc >= Open Prc
		strcpy(zBasePrc, m_zOpenPrc);
		int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zBasePrc, LEN_PRC, 2/*TODO*/, LEN_PRC);
		if (nComp >= 0) {
			strcpy(pzStratID, STRATID_BUY_SL);
			sprintf(pzClrMsg, "SHORT손절조건.(오픈가:%s, 진입가:%s, 현재가:%s)",
				m_zOpenPrc, m_stratHist->GetEntryPrc(), pzCurrPrc);
			CUtil::logOutput("%s\n", pzClrMsg);
		}
		// PT BUY : CurrPrc <= OpenPrc + (OpenPrc * 0.005)
		else
		{
			if (m_stratHist->IsHitPTPrc())
			{
				char zMsg[512] = { 0, };
				if (m_stratHist->IsPTCondition(pzCurrPrc, pzClrMsg)) {
					g_log.log(LOGTP_SUCC, pzClrMsg);
					strcpy(pzStratID, STRATID_BUY_PT);
				}
			}
		}
	}
	return pzStratID;
}

VOID CStratMaker::StratClose(char cCurrOpenSide, char* pzCurrPrc, char* pzApiDT, char* pzApiTm)
{
	char zBasePrc[32] = { 0, };
	char zMaxPrc[32] = { 0, };
	char zStratID[32] = { 0, };
	char zEntryPrc[32] = { 0, };
	char zClrMsg[512] = { 0, };

	GetCloseOrdType(cCurrOpenSide, pzCurrPrc, zBasePrc, zStratID, zClrMsg);

	if (strcmp(zStratID, STRATID_NONE) == 0)
	{
		//TODO LOGGING
		return;
	}

	ST_MF_STRAT_ORD stOrd;
	int nStructSize = sizeof(ST_MF_STRAT_ORD);
	memset(&stOrd, 0x20, nStructSize);
	memcpy(stOrd.Symbol, m_zSymbol, strlen(m_zSymbol));

	if (strcmp(zStratID, STRATID_BUY_SL) == 0)	//매수손절
	{
		stOrd.Side[0] = CD_BUY;
	}
	if (strcmp(zStratID, STRATID_BUY_PT) == 0)	//매수익절
	{
		stOrd.Side[0] = CD_BUY;
	}
	if (strcmp(zStratID, STRATID_SELL_SL) == 0)	//매도손절
	{
		stOrd.Side[0] = CD_SELL;
	}

	if (strcmp(zStratID, STRATID_SELL_PT) == 0)	//매도익절
	{
		stOrd.Side[0] = CD_SELL;
	}
	memcpy(stOrd.StratID, zStratID, strlen(zStratID));
	stOrd.ClrTp[0] = CD_CLOSE;

	memcpy(stOrd.OpenPrc, m_zOpenPrc, strlen(m_zOpenPrc));
	memcpy(stOrd.BasePrc, zBasePrc, strlen(zBasePrc));
	memcpy(stOrd.CurrPrc, pzCurrPrc, strlen(pzCurrPrc));
	memcpy(stOrd.OrdPrc, pzCurrPrc, strlen(pzCurrPrc));

	strcpy(zMaxPrc, m_stratHist->GtMaxPLPrc());
	memcpy(stOrd.MaxPLPrc, zMaxPrc, strlen(zMaxPrc));

	strcpy(zEntryPrc, m_stratHist->GetEntryPrc());
	memcpy(stOrd.EntryPrc, zEntryPrc, strlen(zEntryPrc));

	stOrd.OrdProtTp[0] = CD_ORD_TP_MARKET;

	char tmp[128];
	strcpy(tmp, CUtil::Get_NowTime());
	memcpy(stOrd.OrdTM, tmp, strlen(tmp));

	memcpy(stOrd.ApiDT, pzApiDT, strlen(pzApiDT));
	memcpy(stOrd.ApiTM, pzApiTm, strlen(pzApiTm));

	sprintf(stOrd.Note, "%.*s", min(sizeof(stOrd.Note) - 1, strlen(zClrMsg)), zClrMsg);

	m_stratHist->SetStrategyExist(zStratID, pzCurrPrc, m_zOpenPrc);


	//PostThreadMessage
	char* pData = NULL;
	if (m_pMemPool->get(&pData))
	{
		memcpy(pData, (void*)&stOrd, nStructSize);
		PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);
	}

	//TODO. 익절처리했으면 OPEN 가를 변경한다.
	if (strcmp(zStratID, STRATID_SELL_PT) == 0 ||
		strcmp(zStratID, STRATID_BUY_PT) == 0)
	{
		strcpy(m_zOpenPrc, pzCurrPrc);
		m_stratHist->SetOpenPrc(pzCurrPrc);
	}

}


void CStratMaker::CheckMarketClosing()
{
	if (m_nMarketStatus == MARKET_CLOSING || m_nMarketStatus == MARKET_CLOSED)
		return;

	char zNow[32]; SYSTEMTIME st; GetLocalTime(&st);
	sprintf(zNow, "%02d:%02d", st.wHour, st.wMinute);


	// 05:00 장마감. ==> 분이 같으면 장마감.
	if (strncmp(zNow, m_zCloseTM, 5) == 0)
	{
		g_log.log(LOGTP_SUCC, "[%s 장마감] (장마감시간:%s) (현재시간:%s)", m_zSymbol, m_zCloseTM, zNow);
		CUtil::logOutput("[%s 장마감] (장마감시간:%s) (현재시간:%s)\n", m_zSymbol, m_zCloseTM, zNow);
		m_nMarketStatus = MARKET_CLOSING;

		// 장마감 청산
		MarketCloseClr();
	}
}

// 장마감 청산
VOID CStratMaker::MarketCloseClr()
{
	m_nMarketStatus = MARKET_CLOSED;

	char cCurrOpenSide = m_stratHist->IsOpenSrategyExist();

	if (cCurrOpenSide != CD_BUY && cCurrOpenSide != CD_SELL)
		return;

	ST_MF_STRAT_ORD stOrd;
	int nStructSize = sizeof(ST_MF_STRAT_ORD);
	memset(&stOrd, 0x20, nStructSize);
	memcpy(stOrd.Symbol, m_zSymbol, strlen(m_zSymbol));	

	memcpy(stOrd.StratID, STRATID_MARKET_CLR, strlen(STRATID_MARKET_CLR));
	stOrd.ClrTp[0] = CD_CLOSE;

	memcpy(stOrd.OpenPrc, m_zOpenPrc, strlen(m_zOpenPrc));
	//memcpy(stOrd.BasePrc, zBasePrc, strlen(zBasePrc));
	memcpy(stOrd.CurrPrc, m_zLastCurrPrc, strlen(m_zLastCurrPrc));
	memcpy(stOrd.OrdPrc, m_zLastCurrPrc, strlen(m_zLastCurrPrc));

	char zMaxPLPrc[32];
	strcpy(zMaxPLPrc, m_stratHist->GtMaxPLPrc());
	memcpy(stOrd.MaxPLPrc, zMaxPLPrc, strlen(zMaxPLPrc));
	stOrd.OrdProtTp[0] = CD_ORD_TP_MARKET;

	char zTM[128], zDT[128];
	strcpy(zTM, CUtil::Get_NowTime());
	memcpy(stOrd.OrdTM, zTM, strlen(zTM));
	memcpy(stOrd.ApiTM, zTM, strlen(zTM));

	strcpy(zDT, CUtil::GetToday(zDT));
	memcpy(stOrd.ApiDT, zDT, strlen(zDT));

	char zMsg1[128] = { 0, }, zEntryPrc[32] = { 0, };
	strcpy(zEntryPrc, m_stratHist->GetEntryPrc());

	if (cCurrOpenSide == CD_SELL)
	{
		stOrd.Side[0] = CD_BUY;
		sprintf(zMsg1, "[매수장마감청산 of SHORT][Curr:%s, 진입가:%s]  TM:%s]",
			m_zLastCurrPrc, zEntryPrc, zTM);
	}
	else
	{
		stOrd.Side[0] = CD_BUY;
		sprintf(zMsg1, "[매도장마감청산 of LONG][Curr:%s, 진입가:%s]  TM:%s]",
			m_zLastCurrPrc, zEntryPrc, zTM);
	}
	memcpy(stOrd.Note, zMsg1, strlen(zMsg1));

	CUtil::logOutput("%s\n", zMsg1);


	m_stratHist->SetStrategyExist(STRATID_MARKET_CLR, m_zLastCurrPrc, m_zOpenPrc);



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