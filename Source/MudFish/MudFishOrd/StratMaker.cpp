
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
CStratMaker::CStratMaker(char* pzSymbol, CStratHistManager* h):CBaseThread("SratMaker")
{
	//InitializeCriticalSection(&_cs);
	strcpy(m_zSymbol, pzSymbol);
	m_h= h;
	m_nMarketStatus = MARKET_ON;

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

	//DeleteCriticalSection(&m_cs);
}

void CStratMaker::SetInitInfo(CMemPool* pMemPool,unsigned dwSaveThread, unsigned dwSendThread)
{
	//lock();
	m_dwSaveThread = dwSaveThread;
	m_dwSendThread = dwSendThread;
	m_pMemPool = pMemPool;
	//unlock();
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
//	g_log.log(INFO, "SHM OPEN 성공(%s)", m_zMutexNm);
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
//	//	g_log.log(ERR, "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
//	//	g_log.log(ERR, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
//	//	return FALSE;
//	//}
//	//g_log.log(ERR, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
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
	g_log.log(INFO, "[Strat MainThread ID:%d](%s)Start...\n", GetMyThreadID(), m_zSymbol);
	
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
			g_log.log(ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
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
				g_log.log(INFO, "[THREAD ID:%d] Recv Kill Msg", GetMyThreadID());
				break;
			}
			//주문처리
			case WM_RECV_API_CNTR:
			{
				char* pData = (char*)msg.lParam;
				APIOrdProc(pData);
				m_pMemPool->release(pData);
			}
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))


		
	} // while (TRUE)
	
}


//주문처리
VOID CStratMaker::APIOrdProc(char* pData)
{
	ST_API_CNTR_REAL* p = (ST_API_CNTR_REAL*)pData;
	char zBsTp[2], zCntrPrc[32], zCntrQty[32];
	sprintf(zBsTp, "%.1s", p->Side);
	sprintf(zCntrPrc, "%.*s", sizeof(p->CntrPrc), p->CntrPrc);
	sprintf(zCntrQty, "%.*s", sizeof(p->CntrQty), p->CntrQty);

	m_h->AcptCntrProc(zBsTp, zCntrPrc, zCntrQty);
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
			char* pData = (char*)msg.lParam;

			// 시세처리
			if (msg.message == WM_RECV_API_MD)
			{
				int nLen = (int)msg.wParam;
				p->StratProc(pData);
			}
			p->m_pMemPool->release(pData);
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

	// 이미 횟수 초과한 경우 거래 중단.
	if (m_h->IsFinished())
		return;

	// 아직 주문 진행중이면 패스
	if (m_h->IsAlreadySent())
		return;

	m_h->SetMaxPLPrc(m_zLastCurrPrc);

	BOOL bOpen = m_h->IsOpenSrategyExist();
	if(bOpen)
		StratClose(zCurrPrc, zPackDT, zNowPackTime);
	else
		StratOpen(zCurrPrc, zPackDT, zNowPackTime);

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


	//API 에게 전송할 주문 패킷
	typedef struct _ST_API_ORD_RQST
	{
	char	Code[LEN_PACKET_CODE];	//CD_ORD_RQST
	char	Symbol[LEN_SYMBOL];		//space trailing
	char	OrdPrcTp[1];			// 신규,정정,취소
	char	Side[1];				//CD_BUY / CD_SELL
	char	OrdTp[1];				//CD_ORD_TP_LIMIT, ...
	char	OrdPrc[LEN_PRC];		//시장가인 경우 "0      "
	char	OrdQty[LEN_QTY];
	char	OrgOrdNo[LEN_ORD_NO];
	char	OrgPrc[LEN_PRC];
	char	UUID[32];				//내부주문번호
	char	Date[8];
	char	TM[9];	//hhmmssmmm
	}ST_API_ORD_RQST;
*/
VOID CStratMaker::StratOpen(char* pzCurrPrc, char* pzApiDT, char* pzApiTm)
{
	char zBasePrc[128];
	char zUpperPrc[32], zLowerPrc[32];
	sprintf(zUpperPrc, "%.*f",m_h->dotcnt(), m_h->openprc_d() * (1 + m_h->entryspread()));
	sprintf(zLowerPrc, "%.*f", m_h->dotcnt(), m_h->openprc_d() * (1 - m_h->entryspread()));

	int nCondition = 0;

	int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zUpperPrc, LEN_PRC, 2, LEN_PRC);	//TODO. 2
	if (nComp >= 0) nCondition = 1;

	nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zLowerPrc, LEN_PRC, 2, LEN_PRC);	//TODO. 2
	if (nComp <= 0) nCondition = 2;

	if (!nCondition)
		return;

	char* pData = NULL;
	int nStructSize = sizeof(ST_API_ORD_RQST);
	if (!m_pMemPool->get(&pData))
		return;

	g_log.enter();
	g_log.log(INFO, "===============================================");

//	ST_MF_STRAT_ORD stOrd;
	ST_API_ORD_RQST* apiOrd = (ST_API_ORD_RQST*)pData;
	memset(apiOrd, 0x20, sizeof(ST_API_ORD_RQST));
	memcpy(apiOrd->Symbol, m_zSymbol, strlen(m_zSymbol));
	memcpy(apiOrd->Code, CDAPI_ORD_RQST, strlen(CDAPI_ORD_RQST));

	//int nStructSize = sizeof(ST_MF_STRAT_ORD);
	//memset(&stOrd, 0x20, nStructSize);
	//memcpy(stOrd.Symbol, m_zSymbol, strlen(m_zSymbol));

	char zMsg1[512];
	char zStratID[32];
	if (nCondition == 1) {
		strcpy(zStratID, STRATID_BUY_OPEN);
		//stOrd.Side[0] = CD_BUY;
		apiOrd->Side[0] = CD_BUY;
		strcpy(zBasePrc, zUpperPrc);
		sprintf(zMsg1, "[전략발동][매수진입][%s][Curr:%s >= BasePrc:%s] (BasePrc = Open(%s)+(0.1 Percent)[API TM:%s]"
			, m_zSymbol, pzCurrPrc, zUpperPrc, m_h->openprc(), pzApiTm);
		g_log.log(INFO, zMsg1);
	}
	else {
		strcpy(zStratID, STRATID_SELL_OPEN);
		apiOrd->Side[0] = CD_SELL;
		strcpy(zBasePrc, zLowerPrc);

		sprintf(zMsg1, "[전략발동][매도진입][%s][Curr:%s <= BasePrc:%s] (BasePrc = Open(%s)-(0.1 Percent)[API TM:%s]"
			,m_zSymbol, pzCurrPrc, zLowerPrc, m_h->openprc(), pzApiTm);
		g_log.log(INFO, zMsg1);
	}
	

	char tmp[128];
	strcpy(tmp, CUtil::Get_NowTime());
	
	////
	apiOrd->OrdPrcTp[0] = CD_ORD_PROC_NEW;
	apiOrd->OrdTp[0] = CD_ORD_TP_MARKET;
	apiOrd->OrdPrc[0] = '0';					//시장가인 경우 "0      "
	sprintf(tmp, "%-*d", sizeof(apiOrd->OrdQty), m_h->ordqty());
	memcpy(apiOrd->OrdQty, tmp, sizeof(apiOrd->OrdQty));
	MakeGUID(tmp);
	memcpy(apiOrd->UUID, tmp, min(sizeof(apiOrd->UUID),strlen(tmp))); 
	memcpy(apiOrd->Date, pzApiDT, strlen(pzApiDT));
	memcpy(apiOrd->TM, pzApiTm, strlen(pzApiTm));
	apiOrd->EOL[0] = DEF_EOL;
	apiOrd->EOL[1] = 0x00;

	//g_log.log(INFO, "[전략발동][%s]Open주문(%s) 전송준비", m_zSymbol, zStratID);
	PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);
	

	m_h->SetOrderSent((double)m_h->ordqty());
}


/*
	StopLoss, ProfitTaking 

	return value:
	enum { SLPT_NONE=0, SL_BUY, SL_SELL, PT_BUY, PT_SELL};

*/

char* CStratMaker::GetCloseOrdType(char* pzCurrPrc, 
	_Out_ char* pzBasePrc, _Out_ char* pzStratID, _Out_ char* pzClrMsg)
{
	double dOpenPrc = atof(m_h->openprc());	
	strcpy(pzStratID, STRATID_NONE);
	char zBasePrc[32];

	// BUY OPEN
	if (m_h->IsLong())
	{
		// 손절조건 점검.SL SELL : CurrPrc <= Open Prc
		strcpy(zBasePrc, m_h->openprc());
		int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zBasePrc, LEN_PRC, m_h->dotcnt(), LEN_PRC);
		if (nComp <= 0)
		{
			g_log.enter();
			g_log.log(INFO, "===============================================");
			strcpy(pzStratID, STRATID_SELL_SL);
			sprintf(pzClrMsg, "[전략발동][LONG손절](진입가에서 다시 오픈가로 복귀) 오픈가(%s) <= 현재가(%s). 참조로 진입가(%s)"
				, m_h->openprc(), pzCurrPrc, m_h->GetEntryPrc());
			g_log.log(INFO, pzClrMsg);
		}

		else
		{
			// 0.5% 건드렸으면 익절조검 점검한다.
			if (m_h->IsHitPTPrc())
			{
				if (m_h->IsPTCondition(pzCurrPrc, pzClrMsg)) {
					g_log.enter();
					g_log.log(INFO, "===============================================");
					g_log.log(INFO, pzClrMsg);
					strcpy(pzStratID, STRATID_SELL_PT);
				}
			}
		}
	}

	// SELL OPEN
	if (m_h->IsShort())
	{
		// SL BUY : CurrPrc >= Open Prc
		strcpy(zBasePrc, m_h->openprc());
		int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zBasePrc, LEN_PRC, m_h->dotcnt(), LEN_PRC);
		if (nComp >= 0) {
			strcpy(pzStratID, STRATID_BUY_SL);
			g_log.enter();
			g_log.log(INFO, "===============================================");
			sprintf(pzClrMsg, "[전략발동][SHORT손절](진입가에서 다시 오픈가로 복귀) 오픈가(%s) >= 현재가(%s). 참조로 진입가(%s)"
				,m_h->openprc(), pzCurrPrc, m_h->GetEntryPrc());
			g_log.log(INFO, pzClrMsg);
		}
		// PT BUY : CurrPrc <= OpenPrc + (OpenPrc * 0.005)
		else
		{
			if (m_h->IsHitPTPrc())
			{
				char zMsg[512] = { 0, };
				if (m_h->IsPTCondition(pzCurrPrc, pzClrMsg)) {
					g_log.enter();
					g_log.log(INFO, "===============================================");
					g_log.log(INFO, pzClrMsg);
					strcpy(pzStratID, STRATID_BUY_PT);
				}
			}
		}
	}
	return pzStratID;
}

VOID CStratMaker::StratClose(char* pzCurrPrc, char* pzApiDT, char* pzApiTm)
{
	char zBasePrc[32] = { 0, };
	char zMaxPrc[32] = { 0, };
	char zStratID[32] = { 0, };
	char zEntryPrc[32] = { 0, };
	char zClrMsg[512] = { 0, };

	GetCloseOrdType(pzCurrPrc, zBasePrc, zStratID, zClrMsg);

	if (strcmp(zStratID, STRATID_NONE) == 0)
	{
		//청산조건이 아님
		return;
	}

	char* pData = NULL;
	int nStructSize = sizeof(ST_API_ORD_RQST);
	if (!m_pMemPool->get(&pData))
		return;

	ST_API_ORD_RQST *apiOrd = (ST_API_ORD_RQST *)pData;
	memset(apiOrd, 0x20, sizeof(ST_API_ORD_RQST));
	memcpy(apiOrd->Symbol, m_zSymbol, strlen(m_zSymbol));
	memcpy(apiOrd->Code, CDAPI_ORD_RQST, strlen(CDAPI_ORD_RQST));

	if (strcmp(zStratID, STRATID_BUY_SL) == 0)	//매수손절
		apiOrd->Side[0] = CD_BUY;
	if (strcmp(zStratID, STRATID_BUY_PT) == 0)	//매수익절
		apiOrd->Side[0] = CD_BUY;
	if (strcmp(zStratID, STRATID_SELL_SL) == 0)	//매도손절
		apiOrd->Side[0] = CD_SELL;
	if (strcmp(zStratID, STRATID_SELL_PT) == 0)	//매도익절
		apiOrd->Side[0] = CD_SELL;

	char tmp[128];
	strcpy(tmp, CUtil::Get_NowTime());
	////
	apiOrd->OrdPrcTp[0] = CD_ORD_PROC_NEW;
	apiOrd->OrdTp[0] = CD_ORD_TP_MARKET;
	apiOrd->OrdPrc[0] = '0';					//시장가인 경우 "0      "
	sprintf(tmp, "%-*d", sizeof(apiOrd->OrdQty), m_h->entryqty());	//청산이므로 포지션수량
	memcpy(apiOrd->OrdQty, tmp, sizeof(apiOrd->OrdQty));
	MakeGUID(tmp);
	memcpy(apiOrd->UUID, tmp, sizeof(apiOrd->UUID));
	memcpy(apiOrd->Date, pzApiDT, strlen(pzApiDT));
	memcpy(apiOrd->TM, pzApiTm, strlen(pzApiTm));
	apiOrd->EOL[0] = DEF_EOL;
	apiOrd->EOL[1] = 0x00;

	//g_log.log(INFO, "[전략발동][%s]청산주문(%s) 전송준비", m_zSymbol, zStratID);
	PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);

	m_h->SetOrderSent((double)m_h->ordqty());
}


void CStratMaker::CheckMarketClosing()
{
	if (m_nMarketStatus == MARKET_CLOSING || m_nMarketStatus == MARKET_CLOSED)
		return;

	char zNow[32]; SYSTEMTIME st; GetLocalTime(&st);
	sprintf(zNow, "%02d:%02d", st.wHour, st.wMinute);


	// 05:00 장마감. ==> 분이 같으면 장마감.
	if (strncmp(zNow, m_h->endtm(), 5) == 0)
	{
		if (!m_h->IsOpenSrategyExist())
			return;

		g_log.log(INFO, "[%s 장마감] (장마감시간:%s) (현재시간:%s)", m_zSymbol, m_h->endtm(), zNow);
		CUtil::logOutput("[%s 장마감] (장마감시간:%s) (현재시간:%s)\n", m_zSymbol, m_h->endtm(), zNow);
		m_nMarketStatus = MARKET_CLOSING;

		// 장마감 청산
		MarketCloseClr();
	}
}

// 장마감 청산
VOID CStratMaker::MarketCloseClr()
{
	m_nMarketStatus = MARKET_CLOSED;

	if (!m_h->IsOpenSrategyExist())
		return;

	char* pData = NULL;
	int nStructSize = sizeof(ST_API_ORD_RQST);
	if (!m_pMemPool->get(&pData))
		return;

	char zTM[128], zDT[128];
	strcpy(zTM, CUtil::Get_NowTime());
	strcpy(zDT, CUtil::GetToday(zDT));

	char zMsg1[512] = { 0, }, zEntryPrc[32] = { 0, };
	strcpy(zEntryPrc, m_h->GetEntryPrc());

	char tmp[128];
	ST_API_ORD_RQST *apiOrd = (ST_API_ORD_RQST *)pData;;
	memset(apiOrd, 0x20, sizeof(ST_API_ORD_RQST));
	memcpy(apiOrd->Symbol, m_zSymbol, strlen(m_zSymbol));
	memcpy(apiOrd->Code, CDAPI_ORD_RQST, strlen(CDAPI_ORD_RQST));
	apiOrd->OrdPrcTp[0] = CD_ORD_PROC_NEW;
	apiOrd->OrdTp[0] = CD_ORD_TP_MARKET;
	apiOrd->OrdPrc[0] = '0';					//시장가인 경우 "0      "
	sprintf(tmp, "%-*d", sizeof(apiOrd->OrdQty), m_h->entryqty());	//청산이므로 포지션수량
	memcpy(apiOrd->OrdQty, tmp, sizeof(apiOrd->OrdQty));
	MakeGUID(tmp);
	memcpy(apiOrd->UUID, tmp, min(sizeof(apiOrd->UUID), strlen(tmp)));
	memcpy(apiOrd->Date, zDT, strlen(zDT));
	memcpy(apiOrd->TM, zTM, strlen(zTM));
	apiOrd->EOL[0] = DEF_EOL;
	apiOrd->EOL[1] = 0x00;

	if (m_h->IsShort())
	{
		apiOrd->Side[0] = CD_BUY;
		sprintf(zMsg1, "[전략발동][SHORT매수장마감청산][Curr:%s, 진입가:%s]  TM:%s]",
			m_zLastCurrPrc, zEntryPrc, zTM);
		g_log.enter();
		g_log.log(INFO, "================================================");
		g_log.log(INFO, zMsg1);
	}
	if (m_h->IsLong())
	{
		apiOrd->Side[0] = CD_BUY;
		sprintf(zMsg1, "[전략발동][LONG매도장마감청산][Curr:%s, 진입가:%s]  TM:%s]",
			m_zLastCurrPrc, zEntryPrc, zTM);
		g_log.enter();
		g_log.log(INFO, "================================================");
		g_log.log(INFO, zMsg1);
	}

	CUtil::logOutput("%s\n", zMsg1);

	//PostThreadMessage
	//g_log.log(INFO, "[전략발동][%s]장마감청산(%s) 전송준비", m_zSymbol);
	PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);

	m_h->SetOrderSent((double)m_h->ordqty());
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
//	//	g_log.log(ERR, "Load Symbol Info failed(%s)(%s)", sQ, db->GetError());
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
//	//	g_log.log(ERR, "There is no symbol info (%s)", sQ);
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
//		g_log.log(INFO, "[%s]시세수신 Init 성공(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", zLocalIP, zMcastIP, port);
//	else
//		g_log.log(ERR, "[%s]시세수신 Init 실패(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", zLocalIP, zMcastIP, port);
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