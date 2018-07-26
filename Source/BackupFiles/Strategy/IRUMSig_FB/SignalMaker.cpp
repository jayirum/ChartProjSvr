
#include "SignalMaker.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "Main.h"

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
CSignalMaker::CSignalMaker(char* pzSymbol, char* pzArtcCode, CMemPool* pMemPool, /*CQueueShm* pShm,*/ unsigned dwSaveThread, unsigned dwSendThread)
{
	m_dwSaveThread = dwSaveThread;
	m_dwSendThread = dwSendThread;
	m_pMemPool = pMemPool;
	
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zArtc, pzArtcCode);
	GET_SHM_NM(pzArtcCode, m_zShmNm);
	GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);
	m_nChartTp = TP_5MIN;	//TODO
	GET_GROUP_KEY(m_zSymbol, m_nChartTp, m_zGroupKey);

	m_pShmQ = NULL;	//TODO pShm;
	//m_pMDSub = NULL;
	m_pDBPool = NULL;
	m_pMcastRecv = NULL;

	m_pSigOpenPrc = NULL;
	m_pSigAssist = NULL;
	m_pSigCrossMA = NULL;
	m_pSigSideWay = NULL;

	
		
	m_bInitCheck = FALSE;
	m_pSignalResult = new char[LEN_BUFF_SIZE];
	m_pClientBuff = new char[LEN_BUFF_SIZE];
	ResumeThread();
}


CSignalMaker::~CSignalMaker()
{
	EndSubScribe();
	EndShm();
	EndSignals();
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


BOOL CSignalMaker::BeginSubscribe()
{
	//MCAST
	//SAFE_DELETE(m_pMDSub);

	//m_pMDSub = new CNanoPubSub(TP_SUB);
	//return (m_pMDSub->Begin(INNER_CHANNEL_PUBSUB));

	m_pMcastRecv = new CMCastRecv;

	char zLocalIP[32], zMcastIP[32], port[32];
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP", zLocalIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP_MCAST", zMcastIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "PORT", port);

	BOOL bRet = m_pMcastRecv->Begin(zLocalIP, zMcastIP, atoi(port));
	if (bRet)
		g_log.log(LOGTP_SUCC, "[%s]시세수신 Init 성공(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", zLocalIP, zMcastIP, port);
	else
		g_log.log(LOGTP_ERR, "[%s]시세수신 Init 실패(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", zLocalIP, zMcastIP, port);
	return bRet;
}

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

VOID CSignalMaker::EndSubScribe()
{
	//MCAST
	//m_pMDSub->End();
	//SAFE_DELETE(m_pMDSub);
	SAFE_DELETE(m_pMcastRecv);
}

void CSignalMaker::EndShm()
{
	m_pShmQ->Close();

	//TODO 
	SAFE_DELETE(m_pShmQ);
}

BOOL CSignalMaker::BeginSignals()
{
	m_pSigOpenPrc = new CSignalOpenPrc(m_pShmQ, (void*)m_pDBPool);
	m_pSigOpenPrc->Begin();

	m_pSigAssist = new CSignalAssist(m_pShmQ, (void*)m_pDBPool);
	m_pSigAssist->Begin();

	m_pSigCrossMA = new CSignalCrossMA(m_pShmQ, (void*)m_pDBPool);
	m_pSigCrossMA->Begin();

	m_pSigSideWay = new CSignalSideWay(m_pShmQ, (void*)m_pDBPool);
	m_pSigSideWay->Begin();

	return TRUE;
}

VOID CSignalMaker::EndSignals()
{
	SAFE_DELETE(m_pSigOpenPrc);
	SAFE_DELETE(m_pSigAssist);
	SAFE_DELETE(m_pSigCrossMA);
	SAFE_DELETE(m_pSigSideWay);
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

	g_log.log(LOGTP_SUCC, "[THREAD ID:%d](%s)Start...", GetMyThreadID(), m_zSymbol);
	
	if (!BeginDB())
		return;

	if (!BeginShm())
		return ;

	if (!BeginSubscribe())
		return ;

	if (!LoadSymbolInfo())
	{
		return;
	}

	if (!BeginSignals())
		return;


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
			if (msg.message== WM_MANAGER_NOTIFY_SOCK)
			{
				ST_MANAGER_SOCKNOTIFY* p = (ST_MANAGER_SOCKNOTIFY*)msg.lParam;
				delete p;
				break;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))


		memset(zMarketDataBuff, 0x00, sizeof(zMarketDataBuff));
		//MCAST int nRecvLen = m_pMDSub->Subscribe((void*)zMarketDataBuff);
		int nRecvLen = m_pMcastRecv->RecvData(zMarketDataBuff, sizeof(zMarketDataBuff));

		// Do Make Signal after receiving marketdata;
		if (nRecvLen > 0)
		{
			ST_PACK2CHART* p = (ST_PACK2CHART*)zMarketDataBuff;
			sprintf(zRecvSymbol, "%.*s", sizeof(p->ShortCode), p->ShortCode);
			if (strncmp(m_zSymbol, zRecvSymbol, nSymbolLen) != 0)
				continue;

			//g_log.log(LOGTP_SUCC, "[SUBSCRIBE](%s)",zMarketDataBuff);
			SignalProcWrapper(zMarketDataBuff);

		}
		
	} // while (TRUE)
	
}

VOID CSignalMaker::SignalProcWrapper(char* pMarketData)
{
	__try
	{
		__try
		{

			SignalProc(pMarketData);
		}
		__except (ReportException(GetExceptionCode(), "CSignalMaker::SignalProcWrapper", m_szMsg))
		{
			g_log.log(LOGTP_FATAL, m_szMsg);
		}

	}
	__finally
	{ }
}


VOID CSignalMaker::SignalProc(char* pMarketData)
{
	char zNowPackTime[32] = { 0, };
	char zPackDT[32] = { 0, };
	char zGroupKey[32] = { 0, };
	char zNowChartNm[32] = { 0, };
	char zCurrPrc[LEN_PRC + 1] = { 0, };

	ST_PACK2CHART	*pPack = (ST_PACK2CHART*)pMarketData;

	sprintf(zCurrPrc, "%.*s", sizeof(pPack->Close), pPack->Close);

	// 현재가 시각
	sprintf(zNowPackTime, "%.8s", pPack->Time);
	sprintf(zPackDT, "%.*s", sizeof(pPack->Date), pPack->Date);
	
	GET_GROUP_KEY(m_zSymbol, m_nChartTp, zGroupKey);
	GET_CHART_NM_EX(zPackDT, zNowPackTime, m_nChartTp, zNowChartNm);
	//g_log.log(LOGTP_SUCC, "현재시간(%s),차트이름(%s)", zNowPackTime, zNowChartNm);


	// 최초실행이면 수행하지 않는다.
	if (IsInitCheckDone() == FALSE)
	{
		SetInitCheckDone();
		return;
	}

	ST_SHM_CHART_UNIT	nowChart, prevChart;	//	현재 구성되고 있는 차트

	// Get Last ChartShm and previous ChartShm	
	INT nRet = GetLastChartShm(zGroupKey, zNowChartNm, prevChart, nowChart);
	if (nRet<0) {
		if (nRet == -1) {
			g_log.log(LOGTP_ERR, "[SignalProc]Get Shm Info error(Packet Time:%s,Symbol:%.5s) (GroupKey:%s, ChartNM:%s)",
				zNowPackTime, pPack->ShortCode, zGroupKey, zNowChartNm);
		}
		return;
	}
	
	//////////////////////////////////////////////////////////////
	// 1. check Signal-1 : OpenPrice
	//////////////////////////////////////////////////////////////
	ZeroMemory(m_pSignalResult, LEN_BUFF_SIZE);
	m_pSigOpenPrc->MakeSignal((CHART_TP)m_nChartTp, zCurrPrc, zGroupKey, zNowChartNm,
		&nowChart, &prevChart, m_SymbolInfo.zTickSize, m_SymbolInfo.nDotCnt, m_pSignalResult,0);

	CHECKSIG_RESULT_OPENPRC* pOpenRslt = (CHECKSIG_RESULT_OPENPRC*)m_pSignalResult;
	if (pOpenRslt->sigFilled.SigRet==SIGNAL_EXIST)
	{
		ZeroMemory(m_pClientBuff, LEN_BUFF_SIZE);
		SetSigPack_OpenPrc(TRUE, /*in*/m_pSignalResult, /*out*/m_pClientBuff);
		PACK_STRAT_FB_OPENPRC* p = (PACK_STRAT_FB_OPENPRC*)m_pClientBuff;
		g_log.log(LOGTP_SUCC, "[OPEN_FILLED](COMP_ID:%.*s)(STRAT_TP:%c)(STRAT_ID:%.*s)(SYMBOL:%.*s)"
			"(CHART_TP:%.2s)(CHART_NM:%.*s)"
			"(PRC:%.*s)(TM:%.*s)(CHART_GB:%c)(ARROW:%c)",
			sizeof(p->Header.StratCompID), p->Header.StratCompID,
			p->Header.StratTp[0],
			sizeof(p->StratID), p->StratID,
			sizeof(p->Header.Symbol), p->Header.Symbol,
			p->ChartTp,
			sizeof(p->ChartNm), p->ChartNm,
			sizeof(p->StratPrc), p->StratPrc,
			sizeof(p->Header.TM), p->Header.TM,
			p->ChartGb[0],
			p->ArrowType[0]
		);
		SendSaveSignal(m_pClientBuff);
	}
	if (pOpenRslt->sigEmpty.SigRet == SIGNAL_EXIST)
	{
		ZeroMemory(m_pClientBuff, LEN_BUFF_SIZE);
		SetSigPack_OpenPrc(FALSE, m_pSignalResult, m_pClientBuff);
		PACK_STRAT_FB_OPENPRC* p = (PACK_STRAT_FB_OPENPRC*)m_pClientBuff;
		g_log.log(LOGTP_SUCC, "<OPEN_EMPTY>(COMP_ID:%.*s)(STRAT_TP:%c)(STRAT_ID:%.*s)(SYMBOL:%.*s)(CHART_TP:%.2s)"
			"(CHART_NM:%.*s)(PRC:%.*s)(TM:%.*s)(CHART_GB:%c)(ARROW:%c)",
			sizeof(p->Header.StratCompID), p->Header.StratCompID,
			p->Header.StratTp[0],
			sizeof(p->StratID), p->StratID,
			sizeof(p->Header.Symbol), p->Header.Symbol,
			p->ChartTp,
			sizeof(p->ChartNm), p->ChartNm,
			sizeof(p->StratPrc), p->StratPrc,
			sizeof(p->Header.TM), p->Header.TM,
			p->ChartGb[0],
			p->ArrowType[0]
		);
		SendSaveSignal(m_pClientBuff);
	}
	
	//////////////////////////////////////////////////////////////
	// 2. check Signal-2 : Assist Signal (End Price)
	//////////////////////////////////////////////////////////////

	// 보조지표 설정
	ZeroMemory(m_pSignalResult, LEN_BUFF_SIZE);
	m_pSigAssist->MakeSignal((CHART_TP)m_nChartTp, zCurrPrc, zGroupKey, zNowChartNm,
		&nowChart, &prevChart, m_SymbolInfo.zTickSize, m_SymbolInfo.nDotCnt, m_pSignalResult, 1);
	
	CHECKSIG_RESULT_ASSIST* pAssistRslt = (CHECKSIG_RESULT_ASSIST*)m_pSignalResult;
	
	if (pAssistRslt->SigRet == SIGNAL_EXIST)
	{
		ZeroMemory(m_pClientBuff, LEN_BUFF_SIZE);
		SetSigPack_AssistPrc(m_pSignalResult,FALSE, m_pClientBuff);
		PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)m_pClientBuff;
		g_log.log(LOGTP_SUCC, "<ASSIST SET>(COMP_ID:%.*s)(STRAT_TP:%c)(STRAT_ID:%.*s)(SYMBOL:%.*s)(CHART_TP:%.2s)"
			"(CHART_NM:%.*s)(PRC:%.*s)(TM:%.*s)(CHART_GB:%c)(MATCHED_CHART:%.*s)",
			sizeof(p->Header.StratCompID), p->Header.StratCompID,
			p->Header.StratTp[0],
			sizeof(p->StratID), p->StratID,
			sizeof(p->Header.Symbol), p->Header.Symbol,
			p->ChartTp,
			sizeof(p->ChartNm), p->ChartNm,
			sizeof(p->StratPrc), p->StratPrc,
			sizeof(p->Header.TM), p->Header.TM,
			p->ChartGb[0],
			sizeof(p->MatchedChartNm), p->MatchedChartNm
		);
		SendSaveSignal(m_pClientBuff);
	}

	// 보조지표 청산 신호
	ZeroMemory(m_pSignalResult, LEN_BUFF_SIZE);
	m_pSigAssist->MakeSignal((CHART_TP)m_nChartTp, zCurrPrc, zGroupKey, zNowChartNm,
		&nowChart, &prevChart, m_SymbolInfo.zTickSize, m_SymbolInfo.nDotCnt, m_pSignalResult, 2);

	if (pAssistRslt->SigRet == SIGNAL_EXIST)
	{
		ZeroMemory(m_pClientBuff, LEN_BUFF_SIZE);
		SetSigPack_AssistPrc(m_pSignalResult, TRUE, m_pClientBuff);
		PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)m_pClientBuff;
		g_log.log(LOGTP_SUCC, "<ASSIST CLR>(COMP_ID:%.*s)(STRAT_TP:%c)(STRAT_ID:%.*s)(SYMBOL:%.*s)(CHART_TP:%.2s)"
			"(CHART_NM:%.*s)(PRC:%.*s)(TM:%.*s)(CHART_GB:%c)(MATCHED_CHART:%.*s)",
			sizeof(p->Header.StratCompID), p->Header.StratCompID,
			p->Header.StratTp[0],
			sizeof(p->StratID), p->StratID,
			sizeof(p->Header.Symbol), p->Header.Symbol,
			p->ChartTp,
			sizeof(p->ChartNm), p->ChartNm,
			sizeof(p->StratPrc), p->StratPrc,
			sizeof(p->Header.TM), p->Header.TM,
			p->ChartGb[0],
			sizeof(p->MatchedChartNm), p->MatchedChartNm
		);
		SendSaveSignal(m_pClientBuff);
	}

	////////////////////////////////////////////////////////
	// TODO
	////////////////////////////////////////////////////////
	return;

	//////////////////////////////////////////////////////////////
	// 3. check Signal-3 : MA(Moving Average) 
	//////////////////////////////////////////////////////////////
	
	// 진입
	ZeroMemory(m_pSignalResult, LEN_BUFF_SIZE);
	m_pSigCrossMA->MakeSignal((CHART_TP)m_nChartTp, zCurrPrc, zGroupKey, zNowChartNm,
		&nowChart, &prevChart, m_SymbolInfo.zTickSize, m_SymbolInfo.nDotCnt, m_pSignalResult,0);

	CHECKSIG_RESULT_CROSSMA* pCrossMA = (CHECKSIG_RESULT_CROSSMA*)m_pSignalResult;

	if (pCrossMA->SigRet == SIGNAL_EXIST)
	{
		ZeroMemory(m_pClientBuff, LEN_BUFF_SIZE);
		SetSigPack_CrossMA(m_pSignalResult, m_pClientBuff);
		PACK_STRAT_FB_CROSSMA* p = (PACK_STRAT_FB_CROSSMA*)m_pClientBuff;
		g_log.log(LOGTP_SUCC, "<CROSS MA>(COMP_ID:%.*s)(STRAT_TP:%c)(STRAT_ID:%.*s)(SYMBOL:%.*s)(CHART_TP:%.2s)"
			"(CHART_NM:%.*s)(PRC:%.*s)(TM:%.*s)(TP:%c)(MA PRC:%.*s)(NOW PRC:%.*s)",
			sizeof(p->Header.StratCompID), p->Header.StratCompID,
			p->Header.StratTp[0],
			sizeof(p->StratID), p->StratID,
			sizeof(p->Header.Symbol), p->Header.Symbol,
			p->ChartTp,
			sizeof(p->ChartNm), p->ChartNm,
			sizeof(p->StratPrc), p->StratPrc,
			sizeof(p->Header.TM), p->Header.TM,
			p->Tp[0],
			sizeof(p->MAPrice), p->MAPrice,
			sizeof(p->NowPrice), p->NowPrice
		);
		SendSaveSignal(m_pClientBuff);
	}

	//////////////////////////////////////////////////////////////
	// 4. check Signal-4 : 횡보구간(SideWay)
	//////////////////////////////////////////////////////////////
	ZeroMemory(m_pSignalResult, LEN_BUFF_SIZE);
	m_pSigSideWay->MakeSignal((CHART_TP)m_nChartTp, zCurrPrc, zGroupKey, zNowChartNm,
		&nowChart, &prevChart, m_SymbolInfo.zTickSize, m_SymbolInfo.nDotCnt, m_pSignalResult);

	CHECKSIG_RESULT_SIDEWAY* pSideWay = (CHECKSIG_RESULT_SIDEWAY*)m_pSignalResult;

	if (pSideWay->SigRet == SIGNAL_EXIST)
	{
		ZeroMemory(m_pClientBuff, LEN_BUFF_SIZE);
		SetSigPack_SideWay(m_pSignalResult, m_pClientBuff);

		PACK_STRAT_FB_SIDEWAY* p = (PACK_STRAT_FB_SIDEWAY*)m_pClientBuff;
		g_log.log(LOGTP_SUCC, "<SIDEWAY>(COMP_ID:%.*s)(STRAT_TP:%c)(STRAT_ID:%.*s)(SYMBOL:%.*s)(CHART_TP:%.2s)"
			"(CHART_NM:%.*s)(PRC:%.*s)(TM:%.*s)(H_L PRC:%.*s)(NOW PRC:%.*s)(TP:%c)",
			sizeof(p->Header.StratCompID), p->Header.StratCompID,
			p->Header.StratTp[0],
			sizeof(p->StratID), p->StratID,
			sizeof(p->Header.Symbol), p->Header.Symbol,
			p->ChartTp,
			sizeof(p->ChartNm), p->ChartNm,
			sizeof(p->StratPrc), p->StratPrc,
			sizeof(p->Header.TM), p->Header.TM,			
			sizeof(p->StratPrc), p->StratPrc,
			sizeof(p->NowPrice), p->NowPrice,
			p->Tp[0]
		);
		SendSaveSignal(m_pClientBuff);
	}
}


/*
	Post message to main thread to send data to client
*/
VOID CSignalMaker::SendSaveSignal(_In_ const char* pSignalPacket)
{	
	char* pData = m_pMemPool->get();
	strcpy(pData, pSignalPacket);
	int nLen = strlen(pData);

	PostThreadMessage(m_dwSaveThread, WM_SEND_STRATEGY, (WPARAM)nLen, (LPARAM)pData);

	char *pData2 = m_pMemPool->get();
	strcpy(pData2, pSignalPacket);
	PostThreadMessage(m_dwSendThread, WM_SEND_STRATEGY, (WPARAM)nLen, (LPARAM)pData2);
}


// 현재 기준 현재 차트와 직전 차트 가져오기
INT CSignalMaker::GetLastChartShm(char* pzGroupKey, char* pzChartNm, ST_SHM_CHART_UNIT& shmLast, ST_SHM_CHART_UNIT& shmNow)
{
	char szChart[LEN_CHART_NM + 1];
	int nLoop = 0;

	// 현재 차트 정보 가져오기
	BOOL bExist = m_pShmQ->DataGet(pzGroupKey, pzChartNm, (char*)&shmNow);

	// 1. 현재 차트가 없는 경우 ==> 나간다. 재 시도한다.
	while(FALSE == bExist)
	{
		Sleep(1000);
		if (++nLoop > 2) {
			// 지금시간 차트가 없는데, 지금시간 체결가는 들어온 상황이라면 차트 이상이다.
			g_log.log(LOGTP_ERR, "[%s][%s] 현재가수신했는데 현재차트가 없다.", pzGroupKey, pzChartNm);
			return -1;
		}
		bExist = m_pShmQ->DataGet(pzGroupKey, pzChartNm, (char*)&shmNow);

	} // if(!bExist)


	// 2. 현재 차트가 있는 경우 직전 차트 가져오기	  
	sprintf(szChart, "%.*s", LEN_CHART_NM, shmNow.prevNm);
	if (strcmp(szChart, "NONE") == 0)
		return -3;

	if (FALSE == m_pShmQ->DataGet(pzGroupKey, szChart, (char*)&shmLast))
	{
		g_log.log(LOGTP_ERR, "(%s)(%.*s) 직전 차트가 SHM 에 없다", pzGroupKey, LEN_CHART_NM, szChart);
		return -2;
	}
	//g_log.log(LOGTP_SUCC, "직전차트(%.4s)", chart[0].Nm);


	return 1;
}





BOOL	CSignalMaker::LoadSymbolInfo()
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

/*
	client 로 전달할 패킷 구성
	(Compose Packet for client)
*/
VOID	CSignalMaker::SetSigPack_OpenPrc(_In_ BOOL bFilled, _In_ void* pSignal, _Out_ char* pRsltPack)
{
	char temp[128];

	CHECKSIG_RESULT_OPENPRC* pSig = (CHECKSIG_RESULT_OPENPRC*)pSignal;
	CHECKSIG_RESULT_OPENPRC_ONE* pOneSig;
	if (bFilled)
		pOneSig = &pSig->sigFilled;
	else
		pOneSig = &pSig->sigEmpty;

	PACK_STRAT_FB_OPENPRC* p = (PACK_STRAT_FB_OPENPRC*)pRsltPack;
	memset(pRsltPack, 0x20, sizeof(PACK_STRAT_FB_OPENPRC));
	*(pRsltPack + sizeof(PACK_STRAT_FB_OPENPRC)) = 0x00;

	memcpy(p->Header.Code, "SS0001", 6);
	memcpy(p->Header.StratCompID, COMP_FB, strlen(COMP_FB));
	p->Header.StratTp[0] = STRAT_TP_SIGNAL;
	memcpy(p->Header.Symbol, m_zSymbol, strlen(m_zSymbol));
	memcpy(p->Header.GroupKey, m_zGroupKey, strlen(m_zGroupKey));
	p->Header.API_YN[0] = 'N';

	memcpy(p->StratID, pOneSig->zStratID, min(sizeof(p->StratID), strlen(pOneSig->zStratID)));
	sprintf(temp, "%02d", m_nChartTp);
	memcpy(p->ChartTp, temp, sizeof(p->ChartTp));
	memcpy(p->ChartNm, pOneSig->zChartNm, strlen(pOneSig->zChartNm));
	memcpy(p->StratPrc, pOneSig->zSigPrc, min(sizeof(p->StratPrc), strlen(pOneSig->zSigPrc)));
	NOWTIME_NONE_DOT(temp);
	memcpy(p->Header.TM, temp, strlen(temp));

	memcpy(p->ChartGb, pOneSig->ChartGb, sizeof(pOneSig->ChartGb));
	if (bFilled)	p->ArrowType[0] = 'F';
	else			p->ArrowType[0] = 'E';

	sprintf(temp, "%d", pOneSig->nCrossTp);
	p->CrossTp[0] = temp[0];
	
	p->EOL[0] = DEF_EOL;
}



/*
client 로 전달할 패킷 구성
(Compose Packet for client)
*/
VOID	CSignalMaker::SetSigPack_AssistPrc(_In_ void* pSignal, BOOL bClr, _Out_ char* pRsltPack)
{
	char temp[128];

	CHECKSIG_RESULT_ASSIST* pSig = (CHECKSIG_RESULT_ASSIST*)pSignal;

	PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)pRsltPack;
	memset(pRsltPack, 0x20, sizeof(PACK_STRAT_FB_ASSIST));
	*(pRsltPack + sizeof(PACK_STRAT_FB_ASSIST)) = 0x00;

	memcpy(p->Header.Code, "SS0002", 6);
	memcpy(p->Header.StratCompID, COMP_FB, strlen(COMP_FB));
	p->Header.StratTp[0] = STRAT_TP_SIGNAL;
	p->Header.API_YN[0] = 'N';
	memcpy(p->Header.Symbol, m_zSymbol, strlen(m_zSymbol));	
	memcpy(p->Header.GroupKey, m_zGroupKey, strlen(m_zGroupKey));

	NOWTIME_NONE_DOT(temp);
	memcpy(p->Header.TM, temp, strlen(temp));

	memcpy(p->StratID, pSig->zStratID, min(sizeof(p->StratID), strlen(pSig->zStratID)));
	sprintf(temp, "%02d", m_nChartTp);
	memcpy(p->ChartTp, temp, sizeof(p->ChartTp));
	memcpy(p->ChartNm, pSig->zChartNm, min(sizeof(p->ChartNm),strlen(pSig->zChartNm)));
	memcpy(p->StratPrc, pSig->zAssistPrc, min(sizeof(p->StratPrc), strlen(pSig->zAssistPrc)));

	if (!bClr) {
		p->ClrYN[0] = 'N';
		memcpy(p->ChartGb, pSig->ChartGb, sizeof(pSig->ChartGb));
		memcpy(p->MatchedChartNm, pSig->zMatchedChartNm, min(sizeof(pSig->zMatchedChartNm), strlen(pSig->zMatchedChartNm)));
	}
	else
	{
		p->ClrYN[0] = 'Y';
		p->ClrCrossTp[0] = pSig->cClrCrossTp[0];

		sprintf(temp, "%.*s", LEN_PRC, pSig->zClrAssistPrc);
		memcpy(p->ClrAssistPrc, temp, min(LEN_PRC, strlen(pSig->zClrAssistPrc)));

		sprintf(temp, "%.*s", LEN_PRC, pSig->zClrNowPrc);
		memcpy(p->ClrNowPrc, temp, min(LEN_PRC, strlen(pSig->zClrNowPrc)));

	}
	p->EOL[0] = DEF_EOL;
}


/*
client 로 전달할 패킷 구성
(Compose Packet for client)
*/
VOID	CSignalMaker::SetSigPack_SideWay(_In_ void* pSignal, _Out_ char* pRsltPack)
{
	char temp[128];

	CHECKSIG_RESULT_SIDEWAY* pSig = (CHECKSIG_RESULT_SIDEWAY*)pSignal;

	PACK_STRAT_FB_SIDEWAY* p = (PACK_STRAT_FB_SIDEWAY*)pRsltPack;
	memset(pRsltPack, 0x20, sizeof(PACK_STRAT_FB_SIDEWAY));
	*(pRsltPack + sizeof(PACK_STRAT_FB_SIDEWAY)) = 0x00;

	memcpy(p->Header.Code, "SS0004", 6);
	memcpy(p->Header.StratCompID, COMP_FB, strlen(COMP_FB));
	p->Header.StratTp[0] = STRAT_TP_SIGNAL;
	memcpy(p->Header.Symbol, m_zSymbol, strlen(m_zSymbol));
	p->Header.API_YN[0] = 'N';
	memcpy(p->Header.GroupKey, m_zGroupKey, strlen(m_zGroupKey));

	memcpy(p->StratID, pSig->zStratID, min(sizeof(p->StratID), strlen(pSig->zStratID)));
	sprintf(temp, "%02d", m_nChartTp);
	memcpy(p->ChartTp, temp, sizeof(p->ChartTp));
	memcpy(p->ChartNm, pSig->zChartNm, strlen(pSig->zChartNm));
	//memcpy(p->Header.StratPrc, pSig->zAssistPrc, min(sizeof(p->Header.StratPrc), strlen(pSig->zAssistPrc)));
	NOWTIME_NONE_DOT(temp);
	memcpy(p->Header.TM, temp, strlen(temp));

	memcpy(p->StratPrc, pSig->zHihgLowPrice, min(sizeof(p->StratPrc), strlen(pSig->zHihgLowPrice)));
	memcpy(p->NowPrice, pSig->zNowPrice, min(sizeof(p->NowPrice), strlen(pSig->zNowPrice)));
	p->Tp[0] = pSig->Tp[0];

	p->EOL[0] = DEF_EOL;
}




/*
client 로 전달할 패킷 구성
(Compose Packet for client)
*/
VOID	CSignalMaker::SetSigPack_CrossMA(_In_ void* pSignal, _Out_ char* pRsltPack)
{
	char temp[128];

	CHECKSIG_RESULT_CROSSMA* pSig = (CHECKSIG_RESULT_CROSSMA*)pSignal;

	PACK_STRAT_FB_CROSSMA* p = (PACK_STRAT_FB_CROSSMA*)pRsltPack;
	memset(pRsltPack, 0x20, sizeof(PACK_STRAT_FB_CROSSMA));
	*(pRsltPack + sizeof(PACK_STRAT_FB_CROSSMA)) = 0x00;

	memcpy(p->Header.Code, "SS0003", 6);
	memcpy(p->Header.StratCompID, COMP_FB, strlen(COMP_FB));
	p->Header.StratTp[0] = STRAT_TP_SIGNAL;
	memcpy(p->Header.Symbol, m_zSymbol, strlen(m_zSymbol));
	memcpy(p->Header.GroupKey, m_zGroupKey, strlen(m_zGroupKey));
	p->Header.API_YN[0] = 'N';

	memcpy(p->StratID, pSig->zStratID, min(sizeof(p->StratID), strlen(pSig->zStratID)));
	sprintf(temp, "%02d", m_nChartTp);
	memcpy(p->ChartTp, temp, sizeof(p->ChartTp));
	memcpy(p->ChartNm, pSig->zChartNm, strlen(pSig->zChartNm));
	//memcpy(p->Header.StratPrc, pSig->zAssistPrc, min(sizeof(p->Header.StratPrc), strlen(pSig->zAssistPrc)));
	NOWTIME_NONE_DOT(temp);
	memcpy(p->Header.TM, temp, strlen(temp));

	memcpy(p->MAPrice, pSig->zMAPrice, min(sizeof(p->MAPrice), strlen(pSig->zMAPrice)));
	memcpy(p->NowPrice, pSig->zNowPrice, min(sizeof(p->NowPrice), strlen(pSig->zNowPrice)));
	p->Tp[0] = pSig->Tp[0];

	p->EOL[0] = DEF_EOL;
}