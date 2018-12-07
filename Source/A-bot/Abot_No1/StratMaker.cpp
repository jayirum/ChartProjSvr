

#include "StratMaker.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_util/IRUM_Common.h"

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
CStratMaker::CStratMaker(char* pzSymbol, char* pzArtc, CStratHistManager* h) :CBaseThread("SratMaker")
{
	m_option = NULL;
	m_chart = NULL;
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zArtc, pzArtc);
	GET_SHM_NM(m_zArtc, m_zShmNm);
	GET_SHM_LOCK_NM(m_zArtc, m_zMutexNm);

	m_h = h;
	m_nMarketStatus = MARKET_NONE;
}

BOOL CStratMaker::Initialize()
{
	//CHART SHM
	if (!InitChartShm())
		return FALSE;

	ResumeThread();	
	
	// worker thread 생성
	m_hWorkDie = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hStratThread = (HANDLE)_beginthreadex(NULL, 0, &StratThread, this, 0, &m_dwStratThreadID);

	return TRUE;
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
	CloseChartShm();

	//DeleteCriticalSection(&m_cs);
}

void CStratMaker::SetInitInfo(CMemPool* pMemPool,unsigned dwSaveThread, unsigned dwSendThread)
{
	m_dwSaveThread = dwSaveThread;
	m_dwSendThread = dwSendThread;
	m_pMemPool = pMemPool;

	TradeOption();
}

BOOL CStratMaker::TradeOption()
{
	// TRADE OPTION
	if (m_option) {
		SAFE_DELETE(m_option);
	}

	m_option = new CTradeOption(g_zConfig);
	if (!m_option->TurnOn_Cross(m_zSymbol))
	{
		g_log.log(ERR, "%s", m_option->cross()->getmsg());
		return FALSE;
	}

	//if(!m_option->TurnOn_PSecured(m_zSymbol))
	//{
	//	g_log.log(ERR, "%s", m_option->p_secured()->getmsg());
	//	return FALSE;
	//}

	return TRUE;
}



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


BOOL	CStratMaker::InitChartShm()
{
	if (!m_chart)
		m_chart = new CChartShmUtil;

	if (!m_chart->OpenChart(m_zArtc, &g_log))
	{
		g_log.log(LOGTP_ERR, "%s", m_chart->getmsg());
		return FALSE;
	}

	return TRUE;
}

VOID	CStratMaker::CloseChartShm()
{
	SAFE_DELETE(m_chart);
}


unsigned WINAPI CStratMaker::StratThread(LPVOID lp)
{
	CUtil::logOutput("WORKER THREAD ID : %d", GetCurrentThreadId());
	CStratMaker* p = (CStratMaker*)lp;

	int nLen;
	char* pData;
	while (WaitForSingleObject(p->m_hWorkDie, 1) != WAIT_OBJECT_0)
	{
		//CHECK Market Close 
		if (p->CheckMarketTime() != MARKET_ON)
		{
			Sleep(1000);
			continue;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 시세처리
			switch (msg.message)
			{
			case WM_RECV_API_MD:
				pData = (char*)msg.lParam;
				nLen = (int)msg.wParam;
				p->StratProc(pData);
				p->m_pMemPool->release(pData);
				break;
				
			case WM_CLOSE_POSITION:
				p->ClosePosition();
				break;
			}
			
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}

VOID CStratMaker::ClosePosition()
{
	MarketCloseClr();
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
	sprintf(zUpperPrc, "%.*f",m_h->dotcnt(), m_h->initialprc_d() * (1 + m_h->entryspread()));
	sprintf(zLowerPrc, "%.*f", m_h->dotcnt(), m_h->initialprc_d() * (1 - m_h->entryspread()));

	int nCondition = 0;

	int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zUpperPrc, LEN_PRC, m_h->dotcnt(), LEN_PRC);
	if (nComp >= 0) 
		nCondition = 1;

	nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zLowerPrc, LEN_PRC, m_h->dotcnt(), LEN_PRC);
	if (nComp <= 0) 
		nCondition = 2;

	if (nCondition==0)
		return;

	char* pData = NULL;
	int nStructSize = sizeof(ST_API_ORD_RQST);
	if (!m_pMemPool->get(&pData))
		return;

//	ST_MF_STRAT_ORD stOrd;
	ST_API_ORD_RQST* apiOrd = (ST_API_ORD_RQST*)pData;
	memset(apiOrd, 0x20, sizeof(ST_API_ORD_RQST));
	memcpy(apiOrd->Symbol, m_zSymbol, strlen(m_zSymbol));
	memcpy(apiOrd->Code, CDAPI_ORD_RQST, strlen(CDAPI_ORD_RQST));

	ABOTLOG_NO1 dblog = { 0, };

	//CHART INFO
	ST_SHM_CHART_UNIT chart;
	CROSS_TP cross1, cross3, cross5;
	int nFindChart = 0;
	if (m_option->IsOn_Cross())
	{
		if (m_option->cross()->Is_1MinCandle())
		{
			if (m_chart->CurrChart(m_zSymbol, TP_1MIN, pzApiDT, pzApiTm, chart))
			{
				cross1 = m_chart->GetCross(&chart, m_h->dotcnt(), dblog.zCross_1min);
				nFindChart++;
			}
		}

		if (m_option->cross()->Is_3MinCandle())
		{
			if (m_chart->CurrChart(m_zSymbol, TP_3MIN, pzApiDT, pzApiTm, chart))
			{
				cross3 = m_chart->GetCross(&chart, m_h->dotcnt(), dblog.zCross_3min);
				nFindChart++;
			}
		}

		if (m_option->cross()->Is_5MinCandle())
		{
			if (m_chart->CurrChart(m_zSymbol, TP_5MIN, pzApiDT, pzApiTm, chart))
			{
				cross5 = m_chart->GetCross(&chart, m_h->dotcnt(), dblog.zCross_5min);
				nFindChart++;
			}
		}
		if (nFindChart == 0)
		{
			g_log.log(ERR, "Chart SHM 이상으로 차트데이터를 읽지 못함");
		}
	}
	char zMsg1[512];
	char zStratID[32];
	BOOL bFire = FALSE;

	//DB LOG
	strcpy(dblog.zSymbol, m_zSymbol);
	//dblog.FireYN[0] = 'Y';
	dblog.OpenClose[0] = 'O';	//O, C
	sprintf(dblog.zOrdQty, "%d", m_h->ordqty());
	strcpy(dblog.zCurrPrc, pzCurrPrc);
	sprintf(dblog.zOpenPrc, m_h->initialprc());
	sprintf(dblog.zEntryPercent, "%.2f", m_h->entryspread()*100);
	sprintf(dblog.zEntryPrc, m_h->GetEntryPrc());
	sprintf(dblog.zApiTM, pzApiTm);

	if (nCondition == 1) 
	{
		strcpy(zStratID, STRATID_BUY_OPEN);
		//stOrd.Side[0] = CD_BUY;
		apiOrd->Side[0] = CD_BUY;
		strcpy(zBasePrc, zUpperPrc);

		sprintf(zMsg1, "[전략발동][매수진입][%s][Curr:%s >= BasePrc:%s] (BasePrc = Open(%s)+(%.2f Percent)[API TM:%s]"
			, m_zSymbol, pzCurrPrc, zUpperPrc, m_h->initialprc(),m_h->entryspread()*100, pzApiTm);
		
		//DB LOG		
		strcpy(dblog.zStratID, zStratID);
		dblog.BsTp[0] = CD_BUY;		//B, S
		sprintf(dblog.zStratPrc, zUpperPrc);
		sprintf(dblog.zMsg, zMsg1);

		bFire = TRUE;
		if (m_option->IsOn_Cross() && nFindChart>0)
		{
			if (m_option->cross()->Is_1MinCandle() && (cross1 != GOLDEN_CROSS))
				bFire = FALSE;
			if (m_option->cross()->Is_3MinCandle() && (cross3 != GOLDEN_CROSS))
				bFire = FALSE;
			if (m_option->cross()->Is_5MinCandle() && (cross5 != GOLDEN_CROSS))
				bFire = FALSE;

			if (!bFire)
			{
				dblog.FireYN[0] = 'N';
				sprintf(zMsg1, "[매수진입조건이나 골든(%d분차트)이 아니므로 오픈가를 현재가로 재조정][Curr:%s <= BasePrc:%s] (BasePrc = Open(%s)-(%.2f Percent)[API TM:%s]"
					,m_option->cross()->GetCandleMin(), pzCurrPrc, zLowerPrc, m_h->initialprc(), m_h->entryspread()*100, pzApiTm);
				sprintf(dblog.zMsg, zMsg1);
				m_h->SetInitialPrc(pzCurrPrc);
			}
		}
	}
	if (nCondition == 2) 
	{
		strcpy(zStratID, STRATID_SELL_OPEN);
		apiOrd->Side[0] = CD_SELL;
		strcpy(zBasePrc, zLowerPrc);

		sprintf(zMsg1, "[전략발동][매도진입][%s][Curr:%s <= BasePrc:%s] (BasePrc = Open(%s)-(%.2f Percent)[API TM:%s]"
			,m_zSymbol, pzCurrPrc, zLowerPrc, m_h->initialprc(), m_h->entryspread()*100, pzApiTm);

		//DB LOG
		strcpy(dblog.zStratID, zStratID);
		dblog.BsTp[0] = CD_SELL;		//B, S
		sprintf(dblog.zStratPrc, zLowerPrc);
		sprintf(dblog.zMsg, zMsg1);

		bFire = TRUE;
		if ( m_option->IsOn_Cross() && nFindChart>0)
		{
			if (m_option->cross()->Is_1MinCandle() && (cross1 != GOLDEN_CROSS))
				bFire = FALSE;
			if (m_option->cross()->Is_3MinCandle() && (cross3 != GOLDEN_CROSS))
				bFire = FALSE;
			if (m_option->cross()->Is_5MinCandle() && (cross5 != GOLDEN_CROSS))
				bFire = FALSE;
			if (!bFire)
			{
				dblog.FireYN[0] = 'N';
				sprintf(zMsg1, "[매도진입조건이나 데드(%d분차트)가 아니므로 오픈가를 현재가로 재조정][Curr:%s <= BasePrc:%s] (BasePrc = Open(%s)-(%.2f Percent)[API TM:%s]"
					, m_option->cross()->GetCandleMin(), pzCurrPrc, zLowerPrc, m_h->initialprc(), m_h->entryspread()*100, pzApiTm);
				sprintf(dblog.zMsg, zMsg1);
				m_h->SetInitialPrc(pzCurrPrc);
			}
		}
	}
	
	dblog.FireYN[0] = (bFire) ? 'Y' : 'N';
	SaveDBLog(&dblog);
	if (bFire)
	{
		g_log.log(INFO, zMsg1);

		char tmp[128];
		strcpy(tmp, CUtil::Get_NowTime());

		////
		apiOrd->OrdPrcTp[0] = CD_ORD_PROC_NEW;
		apiOrd->OrdTp[0] = CD_ORD_TP_MARKET;
		apiOrd->OrdPrc[0] = '0';					//시장가인 경우 "0      "
		sprintf(tmp, "%-*d", sizeof(apiOrd->OrdQty), m_h->ordqty());
		memcpy(apiOrd->OrdQty, tmp, sizeof(apiOrd->OrdQty));
		MakeGUID(tmp);
		memcpy(apiOrd->UUID, tmp, min(sizeof(apiOrd->UUID), strlen(tmp)));
		memcpy(apiOrd->Date, pzApiDT, strlen(pzApiDT));
		memcpy(apiOrd->TM, pzApiTm, strlen(pzApiTm));
		apiOrd->EOL[0] = DEF_EOL;
		apiOrd->EOL[1] = 0x00;

		//g_log.log(INFO, "[전략발동][%s]Open주문(%s) 전송준비", m_zSymbol, zStratID);
		PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);

		m_h->SetOrderSent((double)m_h->ordqty());
	}
}


/*
	StopLoss, ProfitTaking 

	return value:
	enum { SLPT_NONE=0, SL_BUY, SL_SELL, PT_BUY, PT_SELL};

*/

char* CStratMaker::GetCloseOrdType(char* pzCurrPrc,
	_Out_ char* pzBasePrc, _Out_ char* pzStratID, _Out_ char* pzClrMsg, _Out_ ABOTLOG_NO1 *dblog)
{
	strcpy(pzStratID, STRATID_NONE);
	char zBasePrc[32];

	///////////////////////////////////////////////////////////////////////////////////////////
	// 손절 점검
	///////////////////////////////////////////////////////////////////////////////////////////
	if (m_h->IsLong())
	{
		// 손절조건 점검.SL SELL : CurrPrc <= Open Prc
		// 20181207. 손절조건을 진입의 0.1% 손실로 변경한다.
		//strcpy(zBasePrc, m_h->initialprc());
		sprintf(zBasePrc, "%.*f", m_h->dotcnt(), m_h->entryprc() * (1 - m_h->entryspread()));

		int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zBasePrc, LEN_PRC, m_h->dotcnt(), LEN_PRC);
		if (nComp <= 0)
		{
			strcpy(pzStratID, STRATID_SELL_SL);
			sprintf(pzClrMsg, "[전략발동][%s][LONG손절][진입가(%s) 대비 %.2f Percent 손실(%s)] <= 현재가(%s)"
				, m_zSymbol
				, m_h->entryprc_s()
				, m_h->entryspread()*100
				, zBasePrc
				, pzCurrPrc
			);
			g_log.log(INFO, pzClrMsg);

			// DB LOG
			dblog->PLTp[0] = 'L';
			dblog->BsTp[0] = CD_SELL;
			dblog->FireYN[0] = 'Y';
			sprintf(dblog->zEntryPercent, "%.2f", m_h->entryspread() * 100);

			return pzStratID;
		}
	}

	if (m_h->IsShort())
	{
		// SL BUY : CurrPrc >= Open Prc
		// 20181207. 손절조건을 진입의 0.1% 손실로 변경한다.
		//strcpy(zBasePrc, m_h->initialprc());
		sprintf(zBasePrc, "%.*f", m_h->dotcnt(), m_h->entryprc() * (1 + m_h->entryspread()));

		int nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zBasePrc, LEN_PRC, m_h->dotcnt(), LEN_PRC);
		if (nComp >= 0) {
			strcpy(pzStratID, STRATID_BUY_SL);
			sprintf(pzClrMsg, "[전략발동][%s][SHORT손절][진입가(%s) 대비 %.2f Percent 손실(%s)] >= 현재가(%s)"
				, m_zSymbol
				, m_h->entryprc_s()
				, m_h->entryspread() * 100
				, zBasePrc
				, pzCurrPrc
			);

			g_log.log(INFO, pzClrMsg);

			// DB LOG
			dblog->PLTp[0] = 'L';
			dblog->BsTp[0] = CD_BUY;
			dblog->FireYN[0] = 'Y';
			sprintf(dblog->zEntryPercent, "%.2f", m_h->entryspread() * 100);
			return pzStratID;
		}
	}



	///////////////////////////////////////////////////////////////////////////////////////////
	// 익절 점검
	///////////////////////////////////////////////////////////////////////////////////////////
	if (m_h->IsProfitTakingCondition(pzCurrPrc, (void*)m_option, pzClrMsg, pzStratID))
	{
		// DB LOG
		dblog->PLTp[0] = 'P';
		dblog->BsTp[0] = (m_h->IsLong()) ? CD_SELL : CD_BUY;
		dblog->FireYN[0] = 'Y';
		sprintf(dblog->zCurrPrc, pzCurrPrc);
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
	
	ABOTLOG_NO1 dblog = { 0, };
	dblog.FireYN[0] = 'N';

	///////////////////////////////////////////////////////////////////
	GetCloseOrdType(pzCurrPrc, zBasePrc, zStratID, zClrMsg, &dblog);
	///////////////////////////////////////////////////////////////////


	if (strcmp(zStratID, STRATID_NONE) == 0)
	{
		//청산조건이 아님
		return;
	}

	sprintf(dblog.zSymbol, m_zSymbol);
	sprintf(dblog.zStratID, zStratID);
	dblog.OpenClose[0] = 'C';	//O, C
	sprintf(dblog.zOrdQty, "%d", m_h->entryqty());
	sprintf(dblog.zCurrPrc, pzCurrPrc);
	sprintf(dblog.zOpenPrc, m_h->initialprc());
	sprintf(dblog.zEntryPrc, m_h->GetEntryPrc());
	sprintf(dblog.zApiTM, pzApiTm);
	sprintf(dblog.zMsg, zClrMsg);

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
	if (strcmp(zStratID, STRATID_BUY_PSECURED) == 0)	//매수수익확보
		apiOrd->Side[0] = CD_BUY;
	if (strcmp(zStratID, STRATID_SELL_SL) == 0)	//매도손절
		apiOrd->Side[0] = CD_SELL;
	if (strcmp(zStratID, STRATID_SELL_PT) == 0)	//매도익절
		apiOrd->Side[0] = CD_SELL;
	if (strcmp(zStratID, STRATID_SELL_PSECURED) == 0)	//매도수익확보
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

	SaveDBLog(&dblog);
}


INT CStratMaker::CheckMarketTime()
{
	if (m_nMarketStatus == MARKET_CLOSING || m_nMarketStatus == MARKET_CLOSED)
		return m_nMarketStatus;

	char zNow[32]; SYSTEMTIME st; GetLocalTime(&st);
	sprintf(zNow, "%02d:%02d", st.wHour, st.wMinute);


	// 장개시?
	if (m_nMarketStatus == MARKET_NONE)
	{
		if (IsPassedTime(m_h->starttm(), TIME_HH_MM))
		{
			m_nMarketStatus = MARKET_ON;
			g_log.log(INFO, "[%s Market Open] (OpenTime:%s) (CurrTime:%s)", m_zSymbol, m_h->starttm(), zNow);
		}

		return m_nMarketStatus;
	}

	// 05:00 장마감. ==> 분이 같으면 장마감.
	if (m_nMarketStatus == MARKET_ON)
	{
		if (IsPassedTime(m_h->endtm(), TIME_HH_MM))
		{
			// If there is no open position to be closed, just set market closed
			if (!m_h->IsOpenSrategyExist())
			{
				g_log.log(INFO, "[%s MarketClosed] No Open Position (EndTime:%s) (CurrTime:%s)", m_zSymbol, m_h->endtm(), zNow);
				m_nMarketStatus = MARKET_CLOSED;
			}
			else
			{
				g_log.log(INFO, "[%s 장마감] (장마감시간:%s) (현재시간:%s)", m_zSymbol, m_h->endtm(), zNow);
				m_nMarketStatus = MARKET_CLOSING;

				// 장마감 청산
				MarketCloseClr();
			}
		}
	}
	return m_nMarketStatus;
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
		apiOrd->Side[0] = CD_SELL;
		sprintf(zMsg1, "[전략발동][LONG매도장마감청산][Curr:%s, 진입가:%s]  TM:%s]",
			m_zLastCurrPrc, zEntryPrc, zTM);
		g_log.enter();
		g_log.log(INFO, "================================================");
		g_log.log(INFO, zMsg1);
	}

	//DB LOG
	ABOTLOG_NO1 dblog = { 0, };
	strcpy(dblog.zSymbol, m_zSymbol);
	dblog.FireYN[0] = 'Y';
	dblog.OpenClose[0] = 'C';	//O, C
	sprintf(dblog.zOrdQty, "%d", m_h->entryqty());
	strcpy(dblog.zCurrPrc, m_zLastCurrPrc);
	sprintf(dblog.zOpenPrc, m_h->initialprc());
	sprintf(dblog.zEntryPercent, "%.2f", m_h->entryspread() * 100);
	sprintf(dblog.zEntryPrc, m_h->GetEntryPrc());
	sprintf(dblog.zApiTM, zTM);
	strcpy(dblog.zStratID, "MARKET_CLOSE");
	dblog.BsTp[0] = apiOrd->Side[0];		//B, S
	sprintf(dblog.zStratPrc, m_zLastCurrPrc);
	sprintf(dblog.zMsg, "MARKET CLOSE ORDER");
	SaveDBLog(&dblog);

	//PostThreadMessage
	//g_log.log(INFO, "[전략발동][%s]장마감청산(%s) 전송준비", m_zSymbol);
	PostThreadMessage(m_dwSendThread, WM_SENDORD_API, (WPARAM)nStructSize, (LPARAM)pData);

	m_h->SetOrderSent((double)m_h->ordqty());
}


/*
	Post message to main thread to send data to client
*/
VOID CStratMaker::SaveDBLog(_In_ ABOTLOG_NO1* p)
{
	char* pData = NULL;
	if (m_pMemPool->get(&pData))
	{
		memcpy(pData, p, sizeof(ABOTLOG_NO1));
		PostThreadMessage(m_dwSaveThread, WM_STRAT_LOGGING, (WPARAM)sizeof(ABOTLOG_NO1), (LPARAM)pData);
	}
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