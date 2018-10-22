
// MudFishOrdDlg.cpp : implementation file
//


#include "AbotMain.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "Main.h"

extern CLogMsg		g_log;
extern char			g_zConfig[_MAX_PATH];


CAbotMain::CAbotMain():CBaseThread("Abot", TRUE)
{
	m_unApiTick = m_unApiOrd = m_unSaveData = 0;
	m_hApiTick = m_hApiOrd = m_hSaveData = NULL;
	m_pMemPool = NULL;
	m_pDBPool = NULL;
	m_bContinue = TRUE;
	m_pApiClient[0] = m_pApiClient[1] = NULL;
	m_pMonitorClient = NULL;
}	

CAbotMain::~CAbotMain()
{
	Finalize();
}


VOID CAbotMain::Finalize()
{
	PostThreadMessage(m_unMonitor, WM_DIE, 0, 0);
	SAFE_CLOSEHANDLE(m_hMonitor);

	StopStrategies();
	SAFE_DELETE(m_pMemPool);
	SAFE_DELETE(m_pDBPool);
	ClearStratMap();
}


BOOL CAbotMain::Initialize()
{
	//	GET LOG DIR
	char szDir[_MAX_PATH] = { 0, };
	CProp prop;
	if (!prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY))
	{
		showMsg(FALSE, "Registry Open Failed");
		return FALSE;
	}
	strcpy(szDir, prop.GetValue("CONFIG_DIR_ABOT"));
	if(szDir[0]==0)
	{
		showMsg(FALSE, "Registry value Failed");
		return FALSE;
	}
	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, EXENAME);

	// mem pool init
	m_pMemPool = new CMemPool(MEM_PRE_ALLOC, MEM_MAX_ALLOC, MEM_BLOCK_SIZE);

	if (m_pMemPool->available() == 0)
	{
		showMsg(FALSE, "Init MemPool failed");
		return FALSE;
	}

	if (!LoadSymbolInfo(TRUE))
		return FALSE;

	// create recv thread
	m_hMonitor = (HANDLE)_beginthreadex(NULL, 0, &Thread_Monitor, this, 0, &m_unMonitor);

	ResumeThread();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


unsigned WINAPI CAbotMain::Thread_Monitor(LPVOID lp)
{
	CAbotMain* pThis = (CAbotMain*)lp;

	pThis->InitMonitorSocket();

	if (!pThis->m_pMonitorClient->Begin((LPSTR)pThis->m_sMonitorIP.c_str(), pThis->m_nMonitorPort, 10))
	{
		pThis->showMsg(FALSE, "Monitor Socket Error[%s][%d]", 
			pThis->m_sMonitorIP.c_str(), pThis->m_nMonitorPort);
		Sleep(5000);
	}
	else
		pThis->showMsg(TRUE, "Monitor Socket OK(%s)(%d)", 
			pThis->m_sMonitorIP.c_str(), pThis->m_nMonitorPort);

	pThis->m_pMonitorClient->StartRecvData();

	while (pThis->m_bContinue)
	{
		Sleep(1);

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				pThis->CloseMonitorSocket();
				pThis->showMsg(TRUE, "CloseMonitorSocket");
				return 0;
			}
		}

		if (pThis->m_pMonitorClient->HappenedRecvError())
		{
			pThis->showMsg(FALSE, "MONITOR DATA RECV ERROR:%s", pThis->m_pMonitorClient->GetMsg());
			continue;
		}

		char* pBuf = NULL;;
		if (!pThis->m_pMemPool->get(&pBuf))
		{
			pThis->showMsg(FALSE, "memory pool get error");
			continue;
		}

		int nLen = pThis->m_pMonitorClient->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			pThis->m_pMemPool->release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			pThis->showMsg(FALSE, "TICK DATA PAKCET 이상(%s)(%s)", pBuf, pThis->m_pMonitorClient->GetMsg());
			pThis->m_pMemPool->release(pBuf);
			continue;
		}

		if (nLen > 0)
		{
			//TODO

			// TODO : Re-Init
			// LoadSymbolInfo(FALSE);

			// TODO : Finalize
			// Finalize();

			// TODO : Close Open position
			// CloseOpenPosition();

			// TODO : Start Strategy
			// StartStrategies();
		}
	}
	pThis->CloseMonitorSocket();
	pThis->showMsg(TRUE, "m_pMonitorClient Close");

	return 0;
}


unsigned WINAPI CAbotMain::Thread_ApiTick(LPVOID lp)
{

	CAbotMain* p = (CAbotMain*)lp;

	ST_PACK2CHART_EX* pSise;
	char zSymbol[128];
	char tm[9];
	char zCurrPrc[32];

	if (!p->m_pApiClient[API_TICK]->Begin((LPSTR)p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK], 10))
	{
		p->showMsg(FALSE, "API DataFeed Socket Error[%s][%d]", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);
		Sleep(5000);
	}
	else
		p->showMsg(TRUE, "API DataFeed Socket OK(%s)(%d)", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);

	p->m_pApiClient[API_TICK]->StartRecvData();

	while (p->m_bContinue)
	{
		Sleep(1);
		//connect
		//if (p->m_pApiClient[API_TICK]) {
		//	if (!p->m_pApiClient[API_TICK]->IsConnected())
		//	{
		//		sprintf(temp, "socket:%d, m_bConnect:%d\n", p->m_pApiClient[API_TICK]->m_sock, 
		//			p->m_pApiClient[API_TICK]->m_bConn);
		//		OutputDebugString(temp);

		//		if (!p->m_pApiClient[API_TICK]->Connect())
		//		{
		//			p->showMsg(FALSE, "API DataFeed Socket connect Error[%s][%d]", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);
		//			Sleep(5000);
		//			continue;
		//		}
		//		p->showMsg(TRUE, "API DataFeed Socket connect OK(%s)(%d)", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);
		//		Sleep(1000);
		//	}
		//}
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				p->CloseApiSocket(API_TICK);
				p->showMsg(TRUE, "CloseApiSocket(API_TICK)-1");
				return 0;
			}
		}

		if (p->m_pApiClient[API_TICK]->HappenedRecvError())
		{
			p->showMsg(FALSE, "TICK DATA RECV ERROR:%s", p->m_pApiClient[API_TICK]->GetMsg());
			continue;
		}
		char* pBuf = NULL;;
		if (!p->m_pMemPool->get(&pBuf))
		{
			p->showMsg(FALSE, "memory pool get error");
			continue;
		}
		int nLen = p->m_pApiClient[API_TICK]->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			p->m_pMemPool->release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			p->showMsg(FALSE,"TICK DATA PAKCET 이상(%s)(%s)", pBuf, p->m_pApiClient[API_TICK]->GetMsg());
			p->m_pMemPool->release(pBuf);
			continue;
		}

		if (nLen > 0)
		{
			pSise = (ST_PACK2CHART_EX*)pBuf;
			sprintf(tm, "%.2s:%.2s:%.2s", pSise->Time, pSise->Time + 2, pSise->Time + 4);
			memcpy(pSise->Time, tm, sizeof(pSise->Time));
			sprintf(zCurrPrc, "%.*s", sizeof(pSise->Close), pSise->Close);

			sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
			CUtil::TrimAll(zSymbol, strlen(zSymbol));
			std::string sSymbol = zSymbol;

			//char buf[128]; sprintf(buf,"tick[%s]\n", zSymbol);
			//OutputDebugString(buf);
			ITMAP_STRAT it = p->m_mapStrat.find(sSymbol);
			if (it == p->m_mapStrat.end())
			{
				p->m_pMemPool->release(pBuf);
				continue;
				//g_log.log(LOGTP_ERR, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
			}

			ST_STRAT* pStrat = (*it).second;

			// strat class 에 전달
			
			if (!pStrat->bFirstFeed) {
				pStrat->h->SetOpenPrc(zCurrPrc);
				pStrat->bFirstFeed = TRUE;
				p->m_mapStrat[sSymbol] = pStrat;
			}
			else {
				PostThreadMessage(pStrat->m->GetStratThreadId(), WM_RECV_API_MD, 0, (LPARAM)pBuf);
			}
		}
	}
	p->CloseApiSocket(API_TICK);
	p->showMsg(TRUE, "CloseApiSocket(API_TICK)-2");
	return 0;
}

unsigned WINAPI CAbotMain::Thread_ApiOrd(LPVOID lp)
{
	CAbotMain* p = (CAbotMain*)lp;

	char zSymbol[128];
	int nErrCode;

	if (!p->m_pApiClient[API_ORD]->Begin((LPSTR)p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD], 10))
	{
		p->showMsg(FALSE, "API API_ORD Socket Error[%s][%d](%s)",
			p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD], p->m_pApiClient[API_ORD]->GetMsg());
		Sleep(5000);
	}
	else
		p->showMsg(TRUE, "API API_ORD Socket OK(%s)(%d)", p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD]);

	p->m_pApiClient[API_ORD]->StartRecvData();
	

	while (p->m_bContinue)
	{
		Sleep(1);

		//connect
		//if (p->m_pApiClient[API_ORD])
		//{
		//	if (!p->m_pApiClient[API_ORD]->IsConnected())
		//	{
		//		sprintf(temp, "socket:%d, m_bConnect:%d\n", p->m_pApiClient[API_ORD]->m_sock,
		//			p->m_pApiClient[API_ORD]->m_bConn);
		//		OutputDebugString(temp);

		//		if (!p->m_pApiClient[API_ORD]->Connect())
		//		{
		//			p->showMsg(FALSE, "API API_ORD Socket Connect Error[%s][%d](%s)",
		//				p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD], p->m_pApiClient[API_ORD]->GetMsg());
		//			Sleep(5000);
		//			continue;
		//		}
		//		p->showMsg(TRUE, "API API_ORD Socket connect(%s)(%d)", p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD]);
		//		Sleep(1000);
		//	}
		//}
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				p->CloseApiSocket(API_ORD);
				p->showMsg(TRUE, "CloseApiSocket(API_ORD)-1");
				return 0;
			}
			if (msg.message == WM_SENDORD_API)
			{
				//SEND ORDER
				char* pData = (char*)msg.lParam;
				int res = p->m_pApiClient[API_ORD]->SendData(pData, (int)msg.wParam, &nErrCode);
				if (res<=0)
				{
					p->showMsg(FALSE,"API_ORD DATA Send ERROR:%.*s", (int)msg.wParam, pData);
					continue;
				}
				p->showMsg(TRUE, "API_ORD SEND(%.*s)", (int)msg.wParam, pData);
			}
		}

		if (p->m_pApiClient[API_ORD]->HappenedRecvError())
		{
			p->showMsg(FALSE,"API_ORD DATA RECV ERROR:%s", p->m_pApiClient[API_ORD]->GetMsg());
			continue;
		}
		char* pBuf = NULL;;
		if (!p->m_pMemPool->get(&pBuf))
		{
			p->showMsg(FALSE, "memory pool get error");
			continue;
		}
		int nLen = p->m_pApiClient[API_ORD]->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			p->m_pMemPool->release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			p->showMsg(FALSE, "API_ORD DATA PAKCET 이상(%s)(%s)", pBuf, p->m_pApiClient[API_ORD]->GetMsg());
			p->m_pMemPool->release(pBuf);
			continue;
		}

		//p->showMsg(TRUE, "API_ORD응답수신(%s)", pBuf);

		ITMAP_STRAT it;
		ST_API_ORD_RESPONSE *pData = (ST_API_ORD_RESPONSE*)pBuf;

		// API REJECT
		if( strncmp(pData->Code, CDAPI_ERROR, strlen(CDAPI_ERROR))==0)
		{
			p->ApiOrd_Err(pBuf);
			p->m_pMemPool->release(pBuf);
			continue;
		}
		else
		{
			sprintf(zSymbol, "%.*s", sizeof(pData->Symbol), pData->Symbol);
			CUtil::TrimAll(zSymbol, strlen(zSymbol));
			std::string sSymbol = zSymbol;

			it = p->m_mapStrat.find(sSymbol);
			if (it == p->m_mapStrat.end())
			{
				p->m_pMemPool->release(pBuf);
				p->showMsg(FALSE, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
				continue;
			}
		}

		if (strncmp(pData->Code, CDAPI_ORD_ACPT, strlen(CDAPI_ORD_ACPT)) == 0)
		{
			p->ApiOrd_Acpt(pBuf);
		}

		if (strncmp(pData->Code, CDAPI_ORD_REAL, strlen(CDAPI_ORD_REAL)) == 0)
		{
			p->ApiOrd_RealOrd(pBuf);
		}

		if (strncmp(pData->Code, CDAPI_CNTR_REAL, strlen(CDAPI_CNTR_REAL)) == 0)
		{
			//just logging
			p->ApiOrd_RealCntr(pBuf);
			// strat class 에 전달
			ST_STRAT* pStrat = (*it).second;
			UINT msg = WM_RECV_API_CNTR;
			PostThreadMessage(pStrat->m->GetMyThreadID(), msg, 0, (LPARAM)pBuf);
		}

		
	}
	p->CloseApiSocket(API_ORD);
	p->showMsg(TRUE, "CloseApiSocket(API_ORD)-2");
	return 0;
}


/*

typedef struct _ST_API_ERROR
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];	//999999
char	ApiMsgCd[10];
char	ApiMsg[128];
char	Date[8];
char	UUID[32];
char	TM[9];	//hhmmssmmm};
char	ETX[1];
}ST_API_ERROR;

*/
VOID CAbotMain::ApiOrd_Err(char* pPacket)
{
	ST_API_ERROR* p = (ST_API_ERROR*)pPacket;
	showMsg(TRUE, "[주문오류](API CODE:%.*s) MSG(%.*s) UUID(%.*s)",
		sizeof(p->ApiMsgCd), p->ApiMsgCd, sizeof(p->ApiMsg), p->ApiMsg, sizeof(p->UUID), p->UUID);

	//TODO. SAVE
	PostThreadMessage(m_unSaveData, WM_SAVE_API_ORD, 0, (LPARAM)pPacket);
}


/*

typedef struct _ST_API_ORD_ACPT
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];	//100101
char	Symbol[LEN_SYMBOL];
char	OrdNo[LEN_ORD_NO];
char	OrdPrc[LEN_PRC];
char	OrdQty[LEN_QTY];
char	OrdProcTp[1];
char	RjctTp[1];		//0:정상 1:거부
char	UUID[32];
char	ApiDT[8];
char	ApiTM[9];		// HH:MM:SS
char	ETX[1];		//
}ST_API_ORD_RESPONSE;
*/
VOID CAbotMain::ApiOrd_Acpt(char* pPacket)
{
	ST_API_ORD_RESPONSE* p = (ST_API_ORD_RESPONSE*)pPacket;

	g_log.log(LOGTP_ERR, "[주문접수](%.*s) No(%.*s) OrdProcTp(%c) ApiDT(%.8s) ApiTM(%.9S) UUID(%.32s)",
		sizeof(p->Symbol), p->Symbol,
		sizeof(p->OrdNo), p->OrdNo,
		p->OrdProcTp[0],
		p->ApiDT,
		p->ApiTM,
		p->UUID);

	//TODO. SAVE ?
	PostThreadMessage(m_unSaveData, WM_SAVE_API_ORD, 0, (LPARAM)pPacket);
}


/*
typedef struct _ST_API_ORD_REAL
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];
char	Symbol[LEN_SYMBOL];
char	OrdNo[LEN_ORD_NO];
char	Side[1];
char	OrdPrc[LEN_PRC];
char	OrdQty[LEN_QTY];
char	RemnQty[LEN_QTY];
char	OrgOrdNo[LEN_ORD_NO];
char	ApiDT[8];
char	ApiTM[9];		// HH:MM:SS
char	ETX[1];		//

}ST_API_ORD_REAL;
*/
VOID CAbotMain::ApiOrd_RealOrd(char* pPacket)
{
	ST_API_ORD_REAL* p = (ST_API_ORD_REAL*)pPacket;
	//showMsg(TRUE, "[주문REAL수신](%.*s) No(%.*s) Side(%c) Prc(%.*s) Qty(%.*s) Rmnq(%.*s) DT(%.8s) TM(%.9s)",
	//	sizeof(p->Symbol), p->Symbol,
	//	sizeof(p->OrdNo), p->OrdNo,
	//	p->Side[0],
	//	sizeof(p->OrdPrc), p->OrdPrc,
	//	sizeof(p->OrdQty), p->OrdQty,
	//	sizeof(p->RemnQty), p->RemnQty,
	//	p->ApiDT, p->ApiTM);
}

/*
typedef struct _ST_API_CNTR_REAL
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];
char	Symbol[LEN_SYMBOL];
char	OrdNo[LEN_ORD_NO];
char	Side[1];
char	OrdPrc[LEN_PRC];
char	OrdQty[LEN_QTY];
char	CntrPrc[LEN_PRC];
char	CntrQty[LEN_QTY];
char	EngagedAmt[LEN_MONEY];	//약정금액
char	Cmsn[LEN_MONEY];
char	ApiOrdDT[8];
char	ApiOrdTM[9];		// HH:MM:SS
char	ApiCntrDT[8];
char	ApiCntrTM[9];		// HH:MM:SS
char	ETX[1];		//

}ST_API_CNTR_REAL;
*/
VOID CAbotMain::ApiOrd_RealCntr(char* pPacket)
{
	ST_API_CNTR_REAL* p = (ST_API_CNTR_REAL*)pPacket;
	showMsg(TRUE, "[체결REAL수신](%.*s) No(%.*s) Side(%c) OPrc(%.*s) CPrc(%.*s)"
							"OQty(%.*s) CQty(%.*s) EnggAmt(%.*s) DT(%.8s) TM(%.9s)",
		sizeof(p->Symbol), p->Symbol,
		sizeof(p->OrdNo), p->OrdNo,
		p->Side[0],
		sizeof(p->OrdPrc), p->OrdPrc,
		sizeof(p->CntrPrc), p->CntrPrc,
		sizeof(p->OrdQty), p->OrdQty,
		sizeof(p->CntrQty), p->CntrQty,
		sizeof(p->EngagedAmt), p->EngagedAmt,
		p->ApiCntrDT, p->ApiCntrTM);
}

unsigned WINAPI CAbotMain::Thread_SaveData(LPVOID lp)
{

	//CAbotMain* p = (CAbotMain*)lp;

	//ST_PACK2CHART_EX* pSise;
	//char zSymbol[128];
	//char tm[9];
	//char zCurrPrc[32];
	//int nErrCode, res;
	//while (p->m_bContinue)
	//{
	//	Sleep(1);
	//	MSG msg;
	//	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	//	{
	//		if (msg.message == WM_DIE)
	//		{
	//			return 0;
	//		}
	//		if (msg.message == WM_SENDORD_API)
	//		{
	//			char* pData = (char*)msg.lParam;
	//			int nLen = msg.wParam;

	//			res = p->m_pApiClient[API_ORD]->SendData(pData, nLen, &nErrCode);
	//			p->m_pMemPool->release(pData);
	//			if(res<=0)
	//			{
	//				p->showMsg(FALSE, "주문전송 에러(%d)(%s)", nErrCode, p->m_pApiClient[API_ORD]->GetMsg());
	//				continue;
	//			}
	//			p->showMsg(TRUE, "주문전송 성공(%d)(%.*s)", res,nLen, pData);
	//		}
	//	}

	//	if (p->m_pApiClient[API_ORD]->HappenedRecvError())
	//	{
	//		p->showMsg(FALSE, "API_ORD DATA RECV ERROR:%s", p->m_pApiClient[API_ORD]->GetMsg());
	//		continue;
	//	}
	//	char* pBuf = NULL;;
	//	if (!p->m_pMemPool->get(&pBuf))
	//	{
	//		p->showMsg(FALSE, "memory pool get error");
	//		continue;
	//	}
	//	int nLen = p->m_pApiClient[API_ORD]->GetOneRecvedPacket(pBuf);
	//	if (nLen == 0) {
	//		p->m_pMemPool->release(pBuf);
	//		continue;
	//	}
	//	if (nLen < 0)
	//	{
	//		p->showMsg(FALSE, "API_ORD DATA PAKCET 이상(%s)(%s)", pBuf, p->m_pApiClient[API_ORD]->GetMsg());
	//		p->m_pMemPool->release(pBuf);
	//		continue;
	//	}

	//	if (nLen > 0)
	//	{
	//		pSise = (ST_PACK2CHART_EX*)pBuf;
	//		sprintf(tm, "%.2s:%.2s:%.2s", pSise->Time, pSise->Time + 2, pSise->Time + 4);
	//		memcpy(pSise->Time, tm, sizeof(pSise->Time));
	//		sprintf(zCurrPrc, "%.*s", sizeof(pSise->Close), pSise->Close);

	//		sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
	//		CUtil::TrimAll(zSymbol, strlen(zSymbol));
	//		std::string sSymbol = zSymbol;

	//		ITMAP_STRAT it = p->m_mapStrat.find(sSymbol);
	//		if (it == p->m_mapStrat.end())
	//		{
	//			p->m_pMemPool->release(pBuf);
	//			continue;
	//			//g_log.log(LOGTP_ERR, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
	//		}

	//		// 화면에 표시
	//		//p->DatafeedProc(sSymbol, pBuf);
	//		p->showMsg(TRUE, "주문수신(%d)(%.*s)", nLen, nLen, pBuf);

	//		// strat class 에 전달
	//		ST_STRAT* pStrat = (*it).second;
	//		PostThreadMessage(pStrat->m->GetStratThreadId(), WM_RECV_API_ORD, 0, (LPARAM)pBuf);
	//	}
	//}
	return 0;
}


void CAbotMain::SetOpenPrc(char* pzSymbol, char* pzOpePrc)
{
	std::string symbol = (LPCSTR)pzSymbol;
	ITMAP_STRAT it = m_mapStrat.find(symbol);
	if (it != m_mapStrat.end())
	{
		ST_STRAT* s = (*it).second;
		s->h->SetOpenPrc(pzOpePrc);
	}
}

void CAbotMain::ReSetSymbolInfo(
	char *pzSymbol
	, double dTickVal
	, double dTickSize
	, int nDotCnt
	, char* pzQty
	, char* pzEndTM
	, int nMaxSLCnt
	, int nMaxPTCnt
	, double dEntrySpread
	, double dClrSpread
	, double dPtPoint
)
{

	std::string symbol = (LPCSTR)pzSymbol;
	ITMAP_STRAT it = m_mapStrat.find(symbol);
	if (it != m_mapStrat.end())
	{
		ST_STRAT* s = (*it).second;
		s->h->SetInitInfo(
			dTickVal,
			dTickSize,
			nDotCnt,
			"",	//(LPSTR)sOpenPrc.GetString(),
			atoi(pzQty),
			pzEndTM,
			nMaxSLCnt,
			nMaxPTCnt,
			dEntrySpread,
			dClrSpread,
			dPtPoint
		);

		s->m->SetInitInfo(
			m_pMemPool,
			m_unSaveData,
			m_unApiOrd);
		m_mapStrat[symbol] = s;
	}
}


BOOL CAbotMain::LoadSymbolInfo(BOOL bCreateStrat)
{
	showMsg(TRUE, "trying to LoadSymbolInfo...");
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	if (!m_pDBPool)
	{
		m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
		if (!m_pDBPool->Init(atoi(cnt)))
		{
			showMsg(FALSE, "DB OPEN FAILED.(%s)(%s)(%s)", ip, id, pwd);
			return FALSE;
		}
	}

	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC MF_LOAD_SYBOL ");
	if (FALSE == db->ExecQuery(zQ))
	{
		showMsg(FALSE, "Load Symbol Error(%s)", zQ);
	}
	else
	{
		while (db->IsNextRow())
		{
			char zSymbol[128];	db->GetStr("SYMBOL", zSymbol);
			char zName[128];	db->GetStr("NAME", zName);
			double dTickVal		= db->GetDouble("TICK_VALUE");
			double dTickSize	= db->GetDouble("TICK_SIZE");
			int nDotCnt			= db->GetLong("DOT_CNT");
			char zQty[128];		db->GetStr("ORD_QTY", zQty);			
			char zEndTM[128];	db->GetStr("END_TM", zEndTM);		
			int nMaxCntSL		= db->GetLong("MAXCNT_SL");			
			int nMaxCntPT		= db->GetLong("MAXCNT_PT");			
			double dEntrySpread = db->GetDouble("ENTRY_SPREAD");			
			double dClrSpread	= db->GetDouble("CLR_SPREAD");
			double dPtPoint		= db->GetDouble("PT_POINT");

			if (bCreateStrat)
			{
				ST_STRAT* st = new ST_STRAT;
				st->h = new CStratHistManager(zSymbol);
				st->m = new CStratMaker(zSymbol, st->h);

				st->bFirstFeed = FALSE;
				m_mapStrat[zSymbol] = st;
			}
			ReSetSymbolInfo(zSymbol, dTickVal, dTickSize, nDotCnt,
				zQty, zEndTM, nMaxCntSL, nMaxCntPT, dEntrySpread, dClrSpread, dPtPoint);

			db->Next();
		}
	}
	db->Close();
	showMsg(TRUE, "Succeeded in LoadSymbolInfo...");
	return TRUE;
}



VOID CAbotMain::ClearStratMap()
{
	ITMAP_STRAT it;
	for (it = m_mapStrat.begin(); it != m_mapStrat.end();++it )
	{
		ST_STRAT* s = (*it).second;
		delete s->h;
		delete s->m;
		delete s;
	}
	m_mapStrat.clear();
}

void CAbotMain::showMsg(BOOL bSucc, char* pMsg, ...)
{
	char buff1[512], buff2[512];
	va_list argptr;
	SYSTEMTIME	st;

	va_start(argptr, pMsg);
	vsprintf_s(buff1, pMsg, argptr);
	va_end(argptr);

	GetLocalTime(&st);
	sprintf(buff2, "[%02d:%02d:%02d.%03d]%s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);
	
	if(bSucc)	g_log.log(INFO, buff1);
	else		g_log.log(ERR, buff1);
}



void CAbotMain::StartStrategies()
{
	showMsg(TRUE, "=======================================================");
	showMsg(TRUE, "전략을 시작합니다!!!!!");
	showMsg(TRUE, "=======================================================");
	m_bContinue = TRUE;
	if (!LoadSymbolInfo(FALSE))
	{
		return;
	}

	InitApiSocket(API_ORD);
	InitApiSocket(API_TICK);

	// create recv thread
	m_hApiTick = (HANDLE)_beginthreadex(NULL, 0, &Thread_ApiTick, this, 0, &m_unApiTick);

	// create ord thread
	m_hApiOrd = (HANDLE)_beginthreadex(NULL, 0, &Thread_ApiOrd, this, 0, &m_unApiOrd);

	// create save thread
	m_hSaveData = (HANDLE)_beginthreadex(NULL, 0, &Thread_SaveData, this, 0, &m_unSaveData);

	ITMAP_STRAT it;
	for (it = m_mapStrat.begin(); it != m_mapStrat.end(); ++it)
	{
		ST_STRAT* p = (*it).second;
		p->m->SetInitInfo(
			m_pMemPool,
			m_unSaveData,
			m_unApiOrd);
	}
}


void CAbotMain::StopStrategies()
{
	showMsg(TRUE, "전략을 중단합니다.");
	m_bContinue = FALSE;

	PostThreadMessage(m_unApiTick, WM_DIE, 0, 0);
	PostThreadMessage(m_unApiOrd, WM_DIE, 0, 0);
	PostThreadMessage(m_unSaveData, WM_DIE, 0, 0);

	SAFE_CLOSEHANDLE(m_hApiTick);
	SAFE_CLOSEHANDLE(m_hApiOrd);
	SAFE_CLOSEHANDLE(m_hSaveData);
	ClearStratMap();
}



void CAbotMain::InitApiSocket(int Idx)
{
	char ip[32], port[32];

	if (Idx == API_ORD)
	{
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "ORD_IP", ip);
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "ORD_PORT", port);
		m_sApiIP[API_ORD] = ip;
		m_nApiPort[API_ORD] = atoi(port);

		if (m_pApiClient[API_ORD] == NULL)
			m_pApiClient[API_ORD] = new CTcpClient("API_ORD");
	}

	if (Idx == API_TICK)
	{
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "SISE_IP", ip);
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "SISE_PORT", port);
		m_sApiIP[API_TICK] = ip;
		m_nApiPort[API_TICK] = atoi(port);

		if (m_pApiClient[API_TICK] == NULL)
			m_pApiClient[API_TICK] = new CTcpClient("MD");
	}
}


void CAbotMain::InitMonitorSocket()
{
	char ip[32], port[32];

	CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "MONITOR_IP", ip);
	CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "MONITOR_PORT", port);
	m_sMonitorIP = ip;
	m_nMonitorPort = atoi(port);

	if (m_pMonitorClient == NULL)
		m_pMonitorClient = new CTcpClient("MONITOR");
	
}

void CAbotMain::CloseApiSocket(int Idx)
{
	if (Idx == API_ORD) {
		//showMsg(TRUE, "CloseApiSocket ORD");
		SAFE_DELETE(m_pApiClient[API_ORD]);
	}

	if (Idx == API_TICK)
	{
		//showMsg(TRUE, "CloseApiSocket API_TICK");
		SAFE_DELETE(m_pApiClient[API_TICK]);
	}
}



void CAbotMain::CloseMonitorSocket()
{
	SAFE_DELETE(m_pMonitorClient);
}


void CAbotMain::ThreadFunc()
{
	WaitForSingleObject(CBaseThread::m_hDie, INFINITE);
}



void CAbotMain::CloseOpenPosition(char *pzSymbol)
{
	ITMAP_STRAT it = m_mapStrat.find(std::string(pzSymbol));
	if (it != m_mapStrat.end())
	{
		ST_STRAT* p = (*it).second;
		PostThreadMessage(p->m->GetStratThreadId(), WM_CLOSE_POSITION, 0, 0);
	}
}