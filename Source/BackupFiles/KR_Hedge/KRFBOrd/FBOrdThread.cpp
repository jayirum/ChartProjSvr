#include "stdafx.h"
#include "FBOrdThread.h"
#include "../../IRUM_UTIL/Log.h"
#include "../../IRUM_UTIL/util.h"
#include "../Common/KRcommon.h"
#include "../../IRUM_UTIL/SharedMem.h"
#include "../../IRUM_UTIL/screwdb.h"

#include "Poco/Thread.h"

using namespace Poco;

extern CLogMsg g_log;


ACNT_TRADE_INFO::ACNT_TRADE_INFO()
{
	InitializeCriticalSectionAndSpinCount(&m_csLimitOrd, 2000);
	dTickSize = 0;
	nDotCnt = 0;
	bInitCheckDone = FALSE;					//	시작하자마자는 처리하지 않도록
	ZeroMemory(&Pos, sizeof(Pos));

	//for (int i = 0; i < CHART_TP_CNT; i++)
	{
		mClrOpenBreak = FALSE;	//	진입가붕괴 청산주문 중복 실행방지를 위해
		mStratOpenDone = "";	//	진입가전략 최종수행 차트 이름
		mStratCloseDone = "";	//	종가전략(보조지표) 최종수행 차트 이름
		mClrOpenHighLow = "";	// 진입후 진입봉 고/저 +/- 2틱 청산 stop 주문
		//arrrSignalSide[i] = 0x00;		//	signal side (매수, 매도)
		//arrSignalChartNm[i] = "";		//	신호 차트이름
		//arrSignalStopOrdDone = "";
		//arrLastOpenPrcDick[i]="";		// 최종 open price tick => CODE-6  
		mLastClrChartNm = "";
		mAssistClrChecked = FALSE;
	}
	ZeroMemory(&mAssist, sizeof(mAssist));//	보조지표
}

ACNT_TRADE_INFO::~ACNT_TRADE_INFO()
{
	LimitOrd_Clear();
	DeleteCriticalSection(&m_csLimitOrd);
}

VOID ACNT_TRADE_INFO::LimitOrd_Clear()
{
	MAPIT_LIMITORD it;
	
	LimitOrd_Lock();
	for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
	{
		delete (*it).second;
	}

	mapLimitOrd.clear();
	LimitOrd_UnLock();
}
//
//VOID ACNT_TRADE_INFO::SetSellSignal(const char* pzChartNm)
//{
//	arrSignalChartNm[idxMyChart] = pzChartNm;
//	arrrSignalSide[idxMyChart] = DEF_SELL;
//}
//
//VOID ACNT_TRADE_INFO::SetBuySignal(const char* pzChartNm)
//{
//	arrSignalChartNm[idxMyChart] = pzChartNm;
//	arrrSignalSide[idxMyChart] = DEF_BUY;
//}

BOOL ACNT_TRADE_INFO::IsSameChartOfOpen(const char* psChartNm)
{
	if (Pos.zChartNm[0] == 0x00)	return FALSE;

	int nComp = strncmp(Pos.zChartNm, psChartNm, LEN_CHART_NM);
	return (nComp >= 0);
}

BOOL ACNT_TRADE_INFO::IsSameChartOfClr(const char* psChartNm)
{
	if (mLastClrChartNm.GetLength() == 0)
		return FALSE;

	char zNm[32]; sprintf(zNm, "%.*s", LEN_CHART_NM, psChartNm);
	int nComp = mLastClrChartNm.Compare(zNm);

	return (nComp >= 0);
}

int ACNT_TRADE_INFO::LimitOrd_Add(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char* pzPrc, int nTotProfitCutTick)
{
	LIMIT_ORDER* p = new LIMIT_ORDER;

	sprintf(p->zChartNm, "%.*s", LEN_CHART_NM, pzChartNm);
	p->nOrdNo = nOrdNo;
	p->nOrigNo = nOrigNo;
	p->nRemainQty = nRemainQty;
	p->cSide[0] = cSide;
	strcpy(p->zPrc, pzPrc);
	p->bProfitOrder = FALSE;
	
	// 포지션이 있는 경우 이 주문이 익절 주문인지 확인한다. 
	// 익절주문은 취소대상이 아니므로 여기에 추가하지 않는다.
	if(IsPosOpened())
	{ 
		if (Pos.cSide[0] != cSide)
		{
			double dOpenPrc = atof(Pos.zPosPrc);
			double dOrdPrc = atof(pzPrc);
			int nTickGap = 0;

			if (Pos.cSide[0] == DEF_BUY)
				nTickGap = CUtil::GetTickGap(dOrdPrc, dOpenPrc, nDotCnt, dTickSize);
			else
				nTickGap = CUtil::GetTickGap(dOpenPrc, dOrdPrc, nDotCnt, dTickSize);

			if (nTickGap == nTotProfitCutTick)
				p->bProfitOrder = TRUE;
		}
	}
	LimitOrd_Lock();
	mapLimitOrd[nOrdNo] = p;
	LimitOrd_UnLock();

	return mapLimitOrd.size();
}

INT ACNT_TRADE_INFO::LimitOrd_Modify(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char* pzPrc)
{
	LimitOrd_Lock();
	MAPIT_LIMITORD it = mapLimitOrd.find(nOrigNo);
	if (it == mapLimitOrd.end()) {
		LimitOrd_UnLock();
		return -1;
	}

	// 기존주문번호 지우고 새주문번호로 저장
	delete (*it).second;
	mapLimitOrd.erase(nOrigNo);

	LIMIT_ORDER* p = new LIMIT_ORDER;

	sprintf(p->zChartNm, pzChartNm, LEN_CHART_NM);
	p->nOrdNo = nOrdNo;
	p->nOrigNo = nOrigNo;
	p->nRemainQty = nRemainQty;
	p->cSide[0] = cSide;
	strcpy(p->zPrc, pzPrc);

	mapLimitOrd[nOrdNo] = p;

	LimitOrd_UnLock();

	return mapLimitOrd.size();
}

int ACNT_TRADE_INFO::LimitOrd_Cancel(_In_ INT nOrigNo)
{
	LimitOrd_Lock();
	MAPIT_LIMITORD it = mapLimitOrd.find(nOrigNo);
	if (it == mapLimitOrd.end()) {
		LimitOrd_UnLock();
		return -1;
	}

	// 기존주문번호 지우고 새주문번호로 저장
	delete (*it).second;
	mapLimitOrd.erase(nOrigNo);

	LimitOrd_UnLock();

	return mapLimitOrd.size();
}


BOOL ACNT_TRADE_INFO::LimitOrd_GetFirstOrd(_Out_ LIMIT_ORDER* pOrdInfo)
{
	LimitOrd_Lock();
	if (mapLimitOrd.size() == 0) {
		LimitOrd_UnLock();
		return FALSE;
	}

	MAPIT_LIMITORD it = mapLimitOrd.begin();
	CopyMemory(pOrdInfo, (*it).second, sizeof(LIMIT_ORDER));
	delete (*it).second;
	mapLimitOrd.erase(it);
	
	LimitOrd_UnLock();
	return TRUE;
}


// 현재보다 이전차트에서 된 주문 취소하기 위해 정보 반환
BOOL ACNT_TRADE_INFO::LimitOrd_GetOrdTobeCanceled(_In_ char* pzNowChartNm, _In_ int nPosQty, _In_ char cPosSide, _Out_ LIMIT_ORDER* p)
{
	LimitOrd_Lock();
	if (mapLimitOrd.size() == 0) {
		LimitOrd_UnLock();
		return FALSE;
	}

	MAPIT_LIMITORD it;
	for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
	{
		LIMIT_ORDER* pInfo = (*it).second;

		// 해당되면 즉시 반환
		if (strncmp(pInfo->zChartNm, pzNowChartNm, LEN_CHART_NM) < 0)
		{
			if (pInfo->bProfitOrder)
				continue;

			// 청산주문은 남겨놓는다.
			if (nPosQty > 0)
			{
				if (cPosSide != pInfo->cSide[0])
					continue;
			}
		
			CopyMemory(p, pInfo, sizeof(LIMIT_ORDER));
			delete pInfo;
			mapLimitOrd.erase(it);
			return TRUE;
		}
	}

	LimitOrd_UnLock();

	return FALSE;
}



// 같은방향 주문 취소
int ACNT_TRADE_INFO::LimitOrd_Cancel_SameSide(_In_ char cSide,  _Out_ int *pnOrdNo)
{
	int nCnt = 0;
	LimitOrd_Lock();
	if (mapLimitOrd.size() == 0) {
		LimitOrd_UnLock();
		return 0;
	}

	MAPIT_LIMITORD it;
	for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
	{
		LIMIT_ORDER* pInfo = (*it).second;

		// 해당되면 주문번호 저장
		if (pInfo->cSide[0] == cSide)
		{
			pnOrdNo[nCnt++] = pInfo->nOrdNo;
		}
	}

	LimitOrd_UnLock();

	return FALSE;
}

BOOL ACNT_TRADE_INFO::IsExistSameSideLimitOrd(_In_ char cSide)
{
	LimitOrd_Lock();
	MAPIT_LIMITORD it;
	for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
	{
		LIMIT_ORDER* p = (*it).second;
		if (p->cSide[0] == cSide) {
			LimitOrd_UnLock();
			return TRUE;
		}
	}
	LimitOrd_UnLock();
	return FALSE;
}


/*
	같은 방향의 더 좋은 지정가 주문이 있는지 점검
*/
BOOL ACNT_TRADE_INFO::LimitOrd_FindBetterOrd(_In_ char cSide, char* pzPrc,  _Out_ int* pnOrdNo, _Out_ char* pzOrdPrc)
{
	LimitOrd_Lock();
	MAPIT_LIMITORD it;

	__try
	{
		for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
		{
			LIMIT_ORDER* p = (*it).second;
			if (p->cSide[0] != cSide)
				continue;

			int nComp = CUtil::CompPrc(p->zPrc, strlen(p->zPrc), pzPrc, strlen(pzPrc), nDotCnt, LEN_PRC);

			// 매수, 지정가 가격 >= 비교대상
			if (cSide == DEF_BUY && nComp >= 0)
			{
				*pnOrdNo = p->nOrdNo;
				strcpy(pzOrdPrc, p->zPrc);
				return TRUE;
			}

			// 매도, 지정가 가격 <= 비교대상
			if (cSide == DEF_SELL && nComp <= 0)
			{
				*pnOrdNo = p->nOrdNo;
				strcpy(pzOrdPrc, p->zPrc);
				return TRUE;
			}
		}
	}
	__finally {
		LimitOrd_UnLock();
	}

	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
CFBOrdThread::CFBOrdThread(char* pAcnt, char* pPwd, API_OBJ* api, CStratLog* pLog)
{
	InitializeCriticalSectionAndSpinCount(&m_csMapTrade, 2000);
	InitializeCriticalSectionAndSpinCount(&m_csProtect, 2000);
	m_bDoStrategy = FALSE;
	m_sAcnt = pAcnt;
	m_sPwd = pPwd;
	m_api = api;
	m_siseSub = new CSisePubSub(TP_SUB);
	m_siseSub->InitSock();
	m_pStopOrd = new CStopOrd;
	m_pStratLog = pLog;
	ZeroMemory(m_zDate, sizeof(m_zDate));
}


CFBOrdThread::~CFBOrdThread()
{
	CloseChartSHM();
	ReleaseStrategy(FALSE/*DBSAVE*/, 'N', "");
	SAFE_DELETE(m_siseSub);
	SAFE_DELETE(m_pStopOrd);
	DeleteCriticalSection(&m_csMapTrade);
	DeleteCriticalSection(&m_csProtect);
}



BOOL CFBOrdThread::InitChartSHM()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);
	
	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
	CUtil::GetConfig(szConfig, "SHM_INFO", "SHM_NAME", szSHMName);
	CUtil::GetConfig(szConfig, "SHM_INFO", "MUTEX_NAME", szMutexName);

	//	OPEN SHM
	if (!m_shm.Open(szSHMName, szMutexName))
	{
		return FALSE;
	}


	//CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
	//CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);
	//if (!m_shm[LASTNM].Open(szSHMName, szMutexName))
	//{
	//	PrintMsg(FALSE, FALSE, "LASTNM SHM OPEN 에러(%s)", szSHMName);
	//	return FALSE;
	//}
	//PrintMsg(FALSE, FALSE, "SHM OPEN 성공(%s)", szSHMName);

	return TRUE;
}

VOID CFBOrdThread::CloseChartSHM()
{
	m_shm.Close();
}



VOID CFBOrdThread::ThreadFunc()
{
	__try {
		MainFunc();
	}
	__except (ReportException(GetExceptionCode(), "CFBOrdThread::ThreadFunc", m_szMsg))
	{
		g_log.log(LOGTP_FATAL, m_szMsg);
		exit(0);
	}

}

VOID CFBOrdThread::MainFunc()
{

	g_log.log(LOGTP_SUCC, "[THREAD ID:%d](%s)Start...", GetMyThreadID(), AcntNo());
	
	// 전략요소들 로드
	if (!Initialize())
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>>>>>>>>>>>>>계좌정보 조회 오류로 이 스레드는 종료한다[%s]", m_sAcnt.GetBuffer());
		return;
	}


	if (!InitChartSHM()) {
		g_log.log(LOGTP_FATAL, ">>>>>>>>>>>>>>>>>>>(%s)SHM OPEN 실패.KRChart실행중인지 확인 필요", m_sAcnt.GetBuffer());
		return ;
	}


	//BOOL bMySise;
	char zSiseBuff[1024];
	char zMsg[1024];
	CString sSymbol;
	CHART_IDX idxChart;
	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
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
					if (p->OrdTp[0] == NOTIFY_ORD_TP_NEW)
					{
						if (strncmp(p->AcntNo, AcntNo(), sizeof(p->AcntNo)) != 0) {
							break;
						}

						char zPrc[32], zSide[32];
						sSymbol.Format("%.*s", sizeof(p->Symbol), p->Symbol);
						sSymbol.Trim();

						if (IsMySymbol(sSymbol)==FALSE) {
							sprintf(zMsg, "%s%s신규주문요청 수신.(%s) 그러나 선택종목(%s) 아니므로(%s) SKIP",
								LOG1_ADMIN, LOG2_ADMIN_NEWORD, AcntNo(), GetMySymbol(), sSymbol.GetBuffer());
							g_log.log(LOGTP_SUCC, zMsg);
							m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_NEWORD, zMsg);
							break;
						}
						ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
						

						if (!IsDoStrategy()) {
							sprintf(zMsg, "%s%s신규주문요청 수신.그러나 전략적용대상이 아니므로 SKIP(%s)(%s)",
								LOG1_ADMIN, LOG2_ADMIN_NEWORD, AcntNo(), sSymbol.GetBuffer());
							g_log.log(LOGTP_SUCC, zMsg);
							m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_NEWORD, zMsg);
							break;
						}


						sprintf(zPrc, "%.*s", sizeof(p->Prc), p->Prc);
						double dPrc = atof(zPrc);

						if(dPrc==0){
						//if (p->PrcTp[0] == NOTIFY_PRC_TP_MARKET) {
							ZeroMemory(zPrc, sizeof(zPrc));
						}
						else { //10151500648CLQ7 11148.2500000
							FORMAT_PRC(dPrc, pTrade->nDotCnt, zPrc);
						}
						sprintf(zSide, "%.1s", p->Side);

						// 현재 포지션과 같은 방향이면 하지 않는다.
						if (IsPosOpened(pTrade) && (pTrade->Pos.cSide[0] == zSide[0]))
						{
							sprintf(zMsg, "%s%s신규주문요청 수신(%s)(%s).현재포지션방향(%c)=주문방향(%c)이라서 거부처리한다.", 
								LOG1_ADMIN, LOG2_ADMIN_NEWORD, AcntNo(), sSymbol.GetBuffer(), 
								pTrade->Pos.cSide[0], zSide[0]);
						}
						else
						{
							sprintf(zMsg, "%s%s신규주문요청 수신(%s)(%s).주문타입(%c)주문방향(%c)(가격:%10.5f).",
								LOG1_ADMIN, LOG2_ADMIN_NEWORD, AcntNo(), sSymbol.GetBuffer(),
								p->PrcTp[0], zSide[0], dPrc);
						}
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CLEARALL, zMsg);

						DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, zPrc, zSide[0], 1, sSymbol, NULL);
					}
					else if (p->OrdTp[0] == NOTIFY_ORD_TP_ALLCLR)
					{
						if (strncmp(p->AcntNo, AcntNo(), sizeof(p->AcntNo)) != 0)
							break;

						sSymbol.Format("%.*s", sizeof(p->Symbol), p->Symbol);
						sSymbol.Trim();

						sprintf(zMsg, "%s%sALL CLEAR 수신(%s)(%s)",LOG1_ADMIN, LOG2_ADMIN_CLEARALL, AcntNo(), sSymbol.GetBuffer());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CLEARALL, zMsg);
						ClearAllOrdPos(sSymbol);
					}
					else if (p->OrdTp[0] == NOTIFY_ORD_TP_ALLCNCL)
					{
						if (strncmp(p->AcntNo, AcntNo(), sizeof(p->AcntNo)) != 0)
							break;

						sSymbol.Format("%.*s", sizeof(p->Symbol), p->Symbol);
						sSymbol.Trim();
						sprintf(zMsg, "%s%sALL CANCEL 수신(%s)(%s)",LOG1_ADMIN, LOG2_ADMIN_CNCLALL, AcntNo(), sSymbol.GetBuffer());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CNCLALL, zMsg);
						CancelAllOrd_Symbol(sSymbol);
						
						
					}
					break;
				} // case WM_MANAGER_NOTIFY_SOCK:
				case WM_MANAGER_NOTIFY:
				{
					ST_MANAGER_NOTIFY* p = (ST_MANAGER_NOTIFY*)msg.lParam;
					switch (p->nWorkTp)
					{
					case MNG_FB_ORDSYMBOL_ADD:	//7)FB 자동주문종목 설정
					case MGN_FB_STRAT_SET:		//10) FB특정계좌 전략설
						sprintf(zMsg, "%s%s전략설정/종목설정(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo(), p->zWorkVal);
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);
						
						EnterCriticalSection(&m_csProtect);
						ReleaseStrategy(FALSE/*DBSAVE*/,'N', "관리자 전략설정 선작업");
						Initialize();
						LeaveCriticalSection(&m_csProtect);
						
						break;
					case MNG_FB_ORDSYMBOL_RELEASE:	// 8)FB 자동주문종목 해제
						sprintf(zMsg, "%s%s종목해제(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_SYMBOL, AcntNo(), p->zWorkVal);
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_SYMBOL, zMsg);
						
						EnterCriticalSection(&m_csProtect);
						ReleaseSymbol(CString(p->zWorkVal));
						LeaveCriticalSection(&m_csProtect);
						break;
					case MGN_FB_STRAT_RELEASE:	// 11) FB특정계좌 전략해제
						sprintf(zMsg, "%s%s 계좌전략해제(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);

						EnterCriticalSection(&m_csProtect);
						ReleaseStrategy(TRUE/*DB SAVE*/, 'N', "관리자 해제");
						LeaveCriticalSection(&m_csProtect);
						break;
					case MGN_FB_CROSS_CHECK:
						bCrossCheck = TRUE;
						sprintf(zMsg, "%s%s크로스적용 설정(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
						break;
					case MNG_FB_CROSS_UNCHECK:
						sprintf(zMsg, "%s%s크로스적용 해제(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
						bCrossCheck = FALSE;
						break;
					case MNG_FB_PROFIT_TICK:
					case MNG_FB_LOSS_TICK:
						sprintf(zMsg, "%s%s전략해제 틱수 재 설정(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, zMsg);
						LoadAcntTradeInfo();
						break;
					default:
						g_log.log(LOGTP_ERR, "[잘못된 작업TP:%d]", p->nWorkTp);
						break;
					}

					delete p;
					break;
				} // case WM_MANAGER_NOTIFY:

			case WM_NOTI_MATCH:
				{
					EnterCriticalSection(&m_csProtect);
					if (IsDoStrategy())
					{
						if (IsMySymbolData(FALSE, (char*)msg.lParam, /*out*/sSymbol, /*out*/idxChart))
							DoProc_RecvAPIReplyWrapper((At0985O_st*)msg.lParam, sSymbol);
					}
					delete ((void*)msg.lParam);
					LeaveCriticalSection(&m_csProtect);

					break;
				} // case WM_NOTI_MATCH:

			} // switch (msg.message)

			
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))


		memset(zSiseBuff, 0x00, sizeof(zSiseBuff));
		int nRecvLen = m_siseSub->Subscribe((void*)zSiseBuff);		
		
		//bMySise = FALSE;
		// 시세를 수신한 경우
		if (nRecvLen > 0)
		{
			if (FALSE == IsDoStrategy())
				continue;
			EnterCriticalSection(&m_csProtect);
			//g_log.log(LOGTP_SUCC, "[RECV](%.100s)", zSiseBuff);
			if (IsMySymbolData(TRUE, zSiseBuff, /*out*/sSymbol, /*out*/idxChart))
			{
				//g_log.log(LOGTP_SUCC, zSiseBuff);
				DoSiseWrapper(idxChart, zSiseBuff, sSymbol);
			}
			LeaveCriticalSection(&m_csProtect);
		}
		
	} // while (TRUE)
	
}

VOID CFBOrdThread::DoSiseWrapper(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol)
{
	__try
	{
		__try
		{
			// 지난 차트에 들어가있는 미체결주문 취소
			Cancel_RemainOrd(pSise, sSymbol);

			// 진입즉시 설정한 STOP 주문 CHECK
			Check_StopFire(pSise, sSymbol);

			// 시가일치, 진입가붕괴 점검
			DoProc_RecvSise(idx, pSise, sSymbol);
		}
		__except (ReportException(GetExceptionCode(), "CFBOrdThread::DoSiseWrapper", m_szMsg))
		{
			g_log.log(LOGTP_FATAL, m_szMsg);
		}
	}
	__finally
	{ }
}

//	CODE-8. 이전 차트에 들어간 미체결주문 취소
VOID CFBOrdThread::Cancel_RemainOrd(const char* pSise, _In_ CString& sSymbol)
{
	char zNowChartNm[32];
	char zNowPackTime[32];	// , zPackDT[32];
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;

	// 현재가 시각
	sprintf(zNowPackTime, "%.8s", pPack->Time);

	// 현재 구성되고 있는 차트 이름
	GET_CHART_NM(zNowPackTime, pTrade->Tp(), zNowChartNm);

	// 이전차트주문 취소
	char msg[1024];
	LIMIT_ORDER stOrd;
	while (pTrade->LimitOrd_GetOrdTobeCanceled(zNowChartNm, pTrade->Pos.nOpenQty, pTrade->Pos.cSide[0], &stOrd))
	{
		// 해당주문 취소처리
		char szOrigNo[32];
		sprintf(szOrigNo, "%-d", stOrd.nOrdNo);
		DoOrder(ORD_TYPE_CNCL, FALSE/*STOP*/, stOrd.zPrc, stOrd.cSide[0], stOrd.nRemainQty, sSymbol, szOrigNo);
		sprintf(msg, "%s%s[Cancel_RemainOrd]이전차트주문 취소처리(%s)(주문번호:%d)(주문SIDE:%c)(주문가격:%10.5f)(주문봉:%s)(현재봉:%s)",
			LOG1_STRAT_ORD, LOG2_CNCL_SEND, sSymbol.GetBuffer(), stOrd.nOrdNo,stOrd.cSide[0], atof(stOrd.zPrc), stOrd.zChartNm, zNowChartNm);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
	}
}

/*
	설정해놓은 손절주문 발동 체크
*/
VOID CFBOrdThread::Check_StopFire(const char* pSise, _In_ CString& sSymbol)
{
	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;
	double dCurrPrc = S2D(pPack->close, sizeof(pPack->close));

	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);

	int nDotCnt = pTrade->nDotCnt;
	char zCurrPrc[LEN_PRC + 1];
	FORMAT_PRC(dCurrPrc, nDotCnt, zCurrPrc);


	char zStopPrc[LEN_PRC + 1] = { 0, };
	char zSide[2] = { 0, };
	int nQty = 0;
	STOPORD_TP stopOrdTp;


	char zNowPackTime[32];	// , zPackDT[32];
	char zNowChartNm[32];
	sprintf(zNowPackTime, "%.8s", pPack->Time);

	GET_CHART_NM(zNowPackTime, pTrade->Tp(), zNowChartNm);

	BOOL bResult = m_pStopOrd->CheckFire(sSymbol, zCurrPrc, pTrade->Pos.zChartNm, zNowChartNm, zStopPrc, zSide, &nQty, &stopOrdTp);
	if (bResult == FALSE)
		return;

	char msg[1024];
	char stopTp[32];
	if (stopOrdTp == STOPORD_ASSIST_BUY)		sprintf(stopTp, "보조지표매수STOP");
	if (stopOrdTp == STOPORD_ASSIST_SELL)		sprintf(stopTp, "보조지표매도STOP");
	if (stopOrdTp == STOPORD_AFTERSIGNAL)	sprintf(stopTp, "SIGNAL STOP");
	if (stopOrdTp == STOPORD_OPENHILO)		sprintf(stopTp, "진입봉고저 STOP");
	if (stopOrdTp == STOPORD_OPEN)			sprintf(stopTp, "진입후 STOP");

	// AFTERSIGNAL 에 대해서 1분30초 점검
	if (stopOrdTp == STOPORD_AFTERSIGNAL && !IsPosOpened(pTrade))
	{
		DWORD dGap;
		if (IsPassedAssistTime(pTrade->mAfterSignal.dwStartClock, &dGap))
		{
			sprintf(msg, "%s%s[Check_StopFire][AFTERSIGNAL 시간초과](%s)(%s)(%s)(GAP:%d 초)",
				LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), (char*)sSymbol.GetBuffer(), pTrade->GroupKeyEx(), dGap);

			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
			return;
		}
	}

	// 보조지표STOP인 경우 1분30초 점검(진입의 경우만)
	if (stopOrdTp == STOPORD_ASSIST_SELL || stopOrdTp == STOPORD_ASSIST_BUY)
	{
		if (IsPosOpened(pTrade) == FALSE)
		{
			DWORD dGap;
			if (IsPassedAssistTime(pTrade->mAssist.dwAssistClock, &dGap))
			{
				sprintf(msg, "%s%s[Check_StopFire][보조지표시간초과](%s)(%s)(%s)(GAP:%d 초)",
					LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), (char*)sSymbol.GetBuffer(), pTrade->GroupKeyEx(), dGap);

				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
				ZeroMemory(&pTrade->mAssist, sizeof(pTrade->mAssist));
				return;
			}
		}
		// 보조지표 stop 인 경우 진입봉에서는 청산하지 않는다.
		else
		{
			if (stopOrdTp == STOPORD_ASSIST_SELL && pTrade->Pos.cSide[0] == DEF_BUY)
			{
				if (pTrade->IsSameChartOfOpen(zNowChartNm))
				{
					sprintf(msg, "%s%s[Check_StopFire](%s)청산STOP발동조건이나 보조지표매도청산의 경우 진입봉(%.*s)과 현재봉(%.*s)이 같으므로 하지 않는다",
							LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), LEN_CHART_NM, pTrade->Pos.zChartNm, LEN_CHART_NM, zNowChartNm);
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
					return;
				}
			}
			if (stopOrdTp == STOPORD_ASSIST_BUY && pTrade->Pos.cSide[0] == DEF_SELL)
			{
				if (pTrade->IsSameChartOfOpen(zNowChartNm))
				{
					sprintf(msg, "%s%s[Check_StopFire](%s)청산STOP발동조건이나 보조지표매수청산의 경우 진입봉(%.*s)과 현재봉(%.*s)이 같으므로 하지 않는다",
							LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), LEN_CHART_NM, pTrade->Pos.zChartNm, LEN_CHART_NM, zNowChartNm);
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
					return;
				}
			}
		}
	} // if (stopOrdTp == STOPORD_ASSIST_SELL || stopOrdTp == STOPORD_ASSIST_BUY)

	// 어떤 STOP이든 발동하면 같은 방향 다른 STOP 은 취소처리 한다.
	// 단 같은 방향의 경우만
	//if ((stopOrdTp == STOPORD_ASSIST) ||(stopOrdTp == STOPORD_AFTERSIGNAL))
	int nCnt = m_pStopOrd->RemoveOrdWhenOtherFired(stopOrdTp, zSide[0]);
	if (nCnt > 0)
	{
		sprintf(msg, "%s%s[Check_StopFire][%s](%s)Stop주문중 (%s) 발동으로 나머지(%d) 취소처리",
			LOG1_STRAT_RQMNT, LOG2_STOP_CNCL, AcntNo(), sSymbol.GetBuffer(), stopTp, nCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_CNCL, msg);
	}


	// 이미 포지션이 있는데 추가 진입이면 오류이므로 주문하지 않는다.
	if (IsPosOpened(pTrade))
	{
		if (pTrade->Pos.cSide[0] == zSide[0])
		{
			sprintf(msg, "%s%s[Check_StopFire][%s](%s)(%s 발동)이미 진입되어 있으므로 추가진입STOP발동은 거부한다.(포지션SIDE:%c)(포지션수량:%d)(포지션가격:%10.5f)",
				LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), stopTp, zSide[0], pTrade->Pos.nOpenQty, atof(pTrade->Pos.zPosPrc));
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
			return;
		}
	}


	// 더 유리한 지정가 있으면 거부
	if (IsOverlappedStopAndLimitOrd(zStopPrc, zSide[0], pTrade))
		return;

	// 이미 주문했으면 두번체크 방지
	// stop 설정할 때와 stop발동될 때 두번 update 한다.
	if (stopOrdTp == STOPORD_OPENHILO)
		pTrade->mClrOpenHighLow = zNowChartNm;
	
	// 청산주문이면 시장가 전송한다.
	char prcTp[32];
	sprintf(prcTp, "진입_지정가");
	if (IsPosOpened(pTrade))
	{
		if (pTrade->Pos.cSide[0] != zSide[0]) {
			ZeroMemory(zStopPrc, sizeof(zStopPrc));
			sprintf(prcTp, "청산_시장가");
		}
	}
	sprintf(msg, "%s%s[Check_StopFire][%s](%s)(%s)(%s)(CURR:%10.5f)(STOP PRC:%10.5f)(SIDE:%c)(진입봉:%s)(현재봉:%s)",
		LOG1_STRAT_ORD, LOG2_STOP_FIRE, AcntNo(), sSymbol.GetBuffer(), prcTp, stopTp, atof(zCurrPrc), atof(zStopPrc), zSide[0], pTrade->Pos.zChartNm, zNowChartNm);
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_FIRE, msg);
	
	DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, zStopPrc, zSide[0], nQty, sSymbol, NULL);
	

	// 보조지표청산이면 보조지표 삭제
	if (stopOrdTp == STOPORD_ASSIST_SELL || stopOrdTp == STOPORD_ASSIST_BUY)
	{
		ZeroMemory(&pTrade->mAssist, sizeof(pTrade->mAssist));
		UpdateTradeMap(sSymbol, pTrade, FALSE);
		sprintf(msg, "%s%s[Check_StopFire]보조지표STOP 발동으로 보조지표삭제[%s](%s)(%s)(기존stopPRC:%10.5f)(SIDE:%c)",
			LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), sSymbol.GetBuffer(), stopTp, atof(zStopPrc), zSide[0]);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER,msg);
	}
}

/*
	STOP 발동시킬 때 더 유리한 조건의 LIMIT 주문이 이미 있으면 주문하지 않기 위해서.
*/
BOOL CFBOrdThread::IsOverlappedStopAndLimitOrd(_In_ char* pzStopPrc, _In_ char cStopSide, _In_ ACNT_TRADE_INFO* pTrade)
{
	BOOL bOverLapped = FALSE;
	char zLimitPrc[32];
	int nLimitNo;

	if (!pTrade->LimitOrd_FindBetterOrd(cStopSide, pzStopPrc, &nLimitNo, zLimitPrc))
		return FALSE;

	char msg[1024];
	sprintf(msg, "%s%s[Check_StopFire]STOP발동됐으나, 더유리한 지정가가 있어서 거부처리.(%s)(%s)(SIDE:%c)(StopPrc:%10.5f)(지정가No.:%d)(지정가PRc:%10.5f)",
		LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), pTrade->Symbol(), cStopSide, atof(pzStopPrc), nLimitNo, atof(zLimitPrc));
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), pTrade->Symbol(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
	return TRUE;
}


/*
	시세를 수신한 경우 처리하는 전략 및 Task 함수 호출
*/
VOID CFBOrdThread::DoProc_RecvSise(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);

	char zNowPackTime[32];	// , zPackDT[32];
	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;

	// 현재가 시각
	sprintf(zNowPackTime, "%.8s", pPack->Time);
	if (strncmp(m_zDate, pPack->Date, sizeof(pPack->Date))!=0)
	{
		sprintf(m_zDate, "%.*s", sizeof(pPack->Date), pPack->Date);
	}

	char zGroupKey[32];
	char zNowChartNm[32];

	// GROUP KEY : 종목코드+차트TP
	sprintf(zGroupKey, "%.*s", LEN_SHM_GROUP_KEY, pTrade->sGroupKey[idx].GetBuffer());

	// 현재 구성되고 있는 차트 이름
	GET_CHART_NM(zNowPackTime, pTrade->Tp(), zNowChartNm);
	//g_log.log(LOGTP_SUCC, "현재시간(%s),차트이름(%s)", zNowPackTime, zChartNm);

	ST_SHM_CHART_UNIT	nowChart, prevChart;	//	현재 구성되고 있는 차트

	// 현재 차트와 최종차트 정보 가져오기
	
	INT nRet = GetLastChartSHM(zGroupKey, zNowChartNm, prevChart, nowChart);
	if (nRet<0) {
		if (nRet == -1) {
			g_log.log(LOGTP_ERR, "[DoProc_RecvSise]수신(패킷시각:%s,종목:%.5s) (GroupKey:%s, ChartNM:%s)",
				zNowPackTime, pPack->shortCode, zGroupKey, zNowChartNm);
		}
		return;
	}

	// 최초실행이면 수행하지 않는다.
	if (pTrade->IsInitDone() == FALSE)
	{
		pTrade->bInitCheckDone = TRUE;
		pTrade->mStratOpenDone = zNowChartNm;
		pTrade->mStratCloseDone = zNowChartNm;
		
		UpdateTradeMap(sSymbol, pTrade, FALSE);
		return;
	}

	// CODE-1. 시가일치전략 점검
	CString sDone = pTrade->mStratOpenDone;
	if (sDone.Compare(zNowChartNm) != 0) {
		Strat_Open(sSymbol, idx, zGroupKey, zNowChartNm, pTrade, nowChart, prevChart/*, zNowPackTime*/);
		UpdateTradeMap(sSymbol, pTrade, FALSE);
	}

	// CODE-2. 종가일치전략 (보조지표) 점검
	sDone = pTrade->mStratCloseDone;
	if (sDone.Compare(zNowChartNm) != 0) {
		Strat_AssistSet(sSymbol, idx, zGroupKey, zNowChartNm, pTrade, prevChart, nowChart);
		UpdateTradeMap(sSymbol, pTrade, FALSE);
	}

	// CODE-3. 보조지표 발동 점검Open
	/* 보조지표stop 은 미리걸어놓는다. - 2017.06.27
	Strat_AssistFire(idx, pSise, sSymbol, pTrade, nowChart, prevChart);
	UpdateTradeMap(sSymbol, pTrade,FALSE);
	*/

	/* 2017.06.07 이나영대표 요청으로 제외
	// CODE-4. 진입 후 진입가격붕괴 청산 점검
	Strat_Clr_OpenPrcBreak(pSise, sSymbol);
	UpdateTradeMap(sSymbol, pTrade);
	*/

	// CODE-5. 진입 후 다음차트부터 진입차트 고가/저가 2틱 청산점검
	/* 2017.07.27 Delete this requirement based on Lee Na Young - 2017.07.27
	Strat_Clr_OpenHiLo(idx, pSise, sSymbol, zGroupKey, pTrade, prevChart, zNowChartNm);
	UpdateTradeMap(sSymbol, pTrade,FALSE);
	*/


	//	CODE-6. 직전매도신호 후 다음봉들 매도봉 고가 1분30초 안에 돌파여부 점검 및 진입
	Strat_AfterSignal_HighLow(sSymbol, pTrade, pSise, zGroupKey, zNowChartNm, nowChart);

	//	보조지표 청산조건 점검. 보조지표>현재가 OR 보조지표<현재가
	Strat_AssistClr(sSymbol, pTrade, pSise);
}


//	CODE-6. 직전매도/매수신호 후 다음봉들 매도봉 고가/저가 1분30초 안에 돌파여부 점검 및 진입/청산
VOID CFBOrdThread::Strat_AfterSignal_HighLow(_In_ CString& sSymbol, ACNT_TRADE_INFO* pTrade, const char* pSise, 
									const char* pzGroupKey, const char* pzNowChartNm, ST_SHM_CHART_UNIT& nowChart)
{
	// 포지션이 있으면 pass
	//if (pTrade->IsPosOpened())
	//	return;

	// 이미 STOP주문을 설정한 경우
	if (pTrade->mAfterSignal.SignalStopOrdDone.GetLength() > 0) {
		if (pTrade->mAfterSignal.SignalStopOrdDone.Compare(pzNowChartNm) >= 0)
			return;
	}

	// 포지션이 없는 경우
	if (IsPosOpened(pTrade) == FALSE)
	{
		// 지금이 시가로 부터 90초가 지났으면 pass (0606)
		if (IsPassedSecs(pzNowChartNm, DEF_CHECK_STRAT_SEC)) {
			return;
		}

		//	최종청산봉과 현재봉이 같으면 진입하지 않는다.
		if (pTrade->IsSameChartOfClr(nowChart.Nm))
		{
			return;
		}
	}

	char sEntry[32];

	ST_PACK2ORD	*p = (ST_PACK2ORD*)pSise;
	double dCurrPrc = S2D(p->close, sizeof(p->close));	// 현재가
	double dCheckPrc = 0;
	double dOrdPrc;

	// 이전 10개봉 scan
	ST_SHM_CHART_UNIT chart;
	char zPrevNm[LEN_CHART_NM + 1];
	sprintf(zPrevNm, "%.*s", LEN_CHART_NM, nowChart.prevNm);
	int nComp = 0, nCompCurr = 0;
	char zPrc[LEN_PRC + 1];
	char msg[1024];
	for (int i = 0; i < DEF_CHECK_CHART_CNT; i++)
	{
		ZeroMemory(sEntry, sizeof(sEntry));
		if (strcmp(DEFINE_NO_CHART, zPrevNm) == 0)	break;

		if (!m_shm.GetData((char*)pzGroupKey, zPrevNm, (char*)&chart))
		{
			g_log.log(LOGTP_ERR, "[Strat_AfterSignal_HighLow] 차트정보조회 오류(%s)(%s)", pzGroupKey, zPrevNm);
			return;
		}

		/*
		매수신호 경우 => 매도주문
		1. 매수신호봉의 저가 가져온다.
		2. 붕괴점검 ( 저가 <= 현차트 시가 and 저가 > 현재가 )
		3. 주문가격 = 저가 - 1틱
		*/
		if (chart.signalGb[0] == DEF_BUY )	// 매수신호==>매도주문
		{
			strcpy(sEntry, "진입");
			// 기존포지션이 있는 경우 청산방향만
			if (IsPosOpened(pTrade))
			{
				if (pTrade->Pos.cSide[0] == DEF_SELL)
					continue;
				strcpy(sEntry, "청산");
			}

			if (chart.gb[0] == '+')
			{
				// 저가, 현차트 시가
				nComp = CUtil::CompPrc(chart.low, sizeof(chart.low), nowChart.open, sizeof(nowChart.open), pTrade->nDotCnt, LEN_PRC);

				// 저가, 현재가
				nCompCurr = CUtil::CompPrc(chart.low, sizeof(chart.low), p->close, sizeof(p->close), pTrade->nDotCnt, LEN_PRC);

				if (nComp <= 0 && nCompCurr > 0)
				{					
					dCheckPrc = atof(chart.low);

					// STOP 주문
					dOrdPrc = dCheckPrc - (1 * pTrade->dTickSize);
					int nCnt = 0;
					pTrade->mAfterSignal.SignalStopOrdDone = pzNowChartNm;
					pTrade->mAfterSignal.dwStartClock = GetTickCount();
					if (AddNewStopOrder(sSymbol, dOrdPrc, DEF_SELL, m_CommFactor.nOrdQTy,
						pTrade->nDotCnt, zPrc, (char*)pzNowChartNm, STOPORD_AFTERSIGNAL, pTrade, &nCnt))
					{
						sprintf(msg, "%s%s[이전매수신호 저가붕괴](저가:%10.5f > 현재가:%10.5f)(매수신호봉:%.8s)STOP매도%s설정.(%s)(%s)(주문가:%10.5f)(총갯수:%d)",
							LOG1_STRAT_ORD, LOG2_STOP_SET, dCheckPrc, dCurrPrc, chart.Nm, sEntry, AcntNo(), sSymbol.GetBuffer(), dOrdPrc, nCnt);
						g_log.log(LOGTP_SUCC, msg);
						m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
					}
					break;	// break for statement
				}
			}
			else
			{
				//g_log.log(LOGTP_SUCC, "[이전매수신호 저가붕괴]매수신호이기는 하나 양봉이 아니므로 skip(봉구분:%c)", chart.gb[0]);
			}
		} // if( chart.signalGb[0]==DEF_BUY)

		/*
		매도신호인 경우 => 매수주문
		1. 매도신호봉의 고가 가져온다.
		2. 돌파점검 ( 고가 >= 현차트 시가 and 고가 < 현재가 )
		3. 주문가 = 고가 + 1틱
		*/
		if (chart.signalGb[0] == DEF_SELL  )// 매도신호==>매수주문
		{
			strcpy(sEntry, "진입");
			// 기존포지션이 있는 경우 청산방향만
			if (IsPosOpened(pTrade))
			{
				if (pTrade->Pos.cSide[0] == DEF_BUY)
					continue;
				strcpy(sEntry, "청산");
			}

			if (chart.gb[0] == '-')
			{
				// 저가, 현차트 시가
				nComp = CUtil::CompPrc(chart.high, sizeof(chart.high), nowChart.open, sizeof(nowChart.open), pTrade->nDotCnt, LEN_PRC);

				// 저가, 현재가
				nCompCurr = CUtil::CompPrc(chart.high, sizeof(chart.high), p->close, sizeof(p->close), pTrade->nDotCnt, LEN_PRC);

				if (nComp >= 0 && nCompCurr < 0)
				{
					// STOP 주문
					dCheckPrc = atof(chart.high);
					dOrdPrc = dCheckPrc + (1 * pTrade->dTickSize);;
					pTrade->mAfterSignal.SignalStopOrdDone = pzNowChartNm;
					pTrade->mAfterSignal.dwStartClock = GetTickCount();
					int nCnt = 0;
					if (AddNewStopOrder(sSymbol, dOrdPrc, DEF_BUY, m_CommFactor.nOrdQTy, pTrade->nDotCnt, zPrc,
						(char*)pzNowChartNm, STOPORD_AFTERSIGNAL, pTrade, &nCnt))
					{
						sprintf(msg, "%s%s[이전매도신호 고가돌파](고가:%10.5f < 현재가:%10.5f)(매도신호봉:%.8s)STOP매수%s설정.(%s)(%s)(주문가:%10.5f)(총갯수:%d)",
							LOG1_STRAT_ORD, LOG2_STOP_SET, dCheckPrc, dCurrPrc, chart.Nm, sEntry, AcntNo(), sSymbol.GetBuffer(), dOrdPrc,nCnt);
						g_log.log(LOGTP_SUCC, msg);
						m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
					}
					break;	// break for statement
				}
			}
			else
			{
				//g_log.log(LOGTP_SUCC, "[이전매도신호 고가돌파]매도신호봉이기는 하나 음봉이 아니므로 skip(봉구분:%c)", chart.gb[0]);
			}
		} // if (chart.signalGb[0] == DEF_SELL)

		sprintf(zPrevNm, "%.*s", LEN_CHART_NM, chart.prevNm);
	} // for (int i = 0; i < DEF_CHECK_CHART_CNT; i++)
	

}

VOID CFBOrdThread::UpdateTradeMap(CString& sSymbol, ACNT_TRADE_INFO *p, BOOL bDelete)
{
	MAPLOCK();
	if (bDelete)
		m_mapTrade.erase(sSymbol);
	else
		m_mapTrade[sSymbol] = p;
	MAPUNLOCK();
}

/*
청산
1) 진입 후 다음 캔들 생성 후 진입캔들 저가 2틱 아래 매수 주문 (선stop)
2) 진입 후 다음 캔들 생성 후 진입캔들 고가 2틱 위 매도 주문 (선stop)

2017.07.05)
매수진입 후 다음 봉부터 계속 진입봉 종가 2틱 아래 매도청산 주문
매도진입 후 다음 봉부터 계속 진입봉 종가 2틱 위 매수청산 주문
*/
VOID	CFBOrdThread::Strat_Clr_OpenHiLo(
					const CHART_IDX idx, 
					const char* pSise, 
					_In_ CString& sSymbol,
					const char* pzGroupKey,
					ACNT_TRADE_INFO* pTrade,
					const ST_SHM_CHART_UNIT& prevChart,
					const char* pzNowChartNm
)
{
	// 기 포지션이 없으면 나간다.
	if ( IsPosOpened(pTrade)==FALSE)
		return;

	
	int nComp = strncmp(pTrade->Pos.zChartNm, pzNowChartNm, LEN_CHART_NM);
	// 5분봉에서 20분 진입, 현재 20분 이면 안한다.
	if (nComp >= 0)
		return;

	// 이미 stop 이 있으면 나간다.
	if (m_pStopOrd->IsStopOrdExists(STOPORD_OPENHILO))
		return;

	// 이미 주문했으면 나간다.(0606)
	// stop 설정할 때와 stop발동될 때 두번 update 한다.
	if (pTrade->mClrOpenHighLow.Compare(pzNowChartNm)>=0 )
		return;


	/*
	nComp = strncmp(pTrade->Pos.zChartNm, prevChart.Nm, LEN_CHART_NM);
	// 5분봉에서 20분 진입, 현재 30분(직전 25분) 이면 안한다.
	if ( nComp < 0)
		return;
	

	// 20분봉 진입. 25분봉 점검(직전봉 20분봉)
	int nComp = strncmp(pTrade->Pos.zChartNm, prevChart.Nm, LEN_CHART_NM);
	if (nComp != 0)
		return;
	*/

	
	// 진입차트를 가져온다.
	ST_SHM_CHART_UNIT chartPos;
	if (m_shm.GetData((char*)pzGroupKey, pTrade->Pos.zChartNm, (char*)&chartPos) == FALSE)
	{
		g_log.log(LOGTP_SUCC, "[Strat_Clr_OpenHiLo] 진입봉 정보 조회 오류(%s)(%s)(%s)", AcntNo(), pzGroupKey, pTrade->Pos.zChartNm);
		return;
	}

	double dOrdPrc, dChckPrc = 0;	
	char zPrc[128];
	char zMsg[128];
	char msg[1024];

	// 매도포지션의 경우 => 매수주문 : 진입캔들 종가 + 2틱 
	if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		dChckPrc = S2D((char*)chartPos.close, sizeof(chartPos.close));
		dOrdPrc = dChckPrc + (2 * pTrade->dTickSize);
		sprintf(zMsg, "매수청산주문(종가:%10.5f)(주문가:%10.5f)", dChckPrc, dOrdPrc);
	}

	// 매수포지션의 경우 => 매도주문 : 진입캔들 종가 - 2틱 
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		dChckPrc = S2D((char*)chartPos.close, sizeof(chartPos.close));
		dOrdPrc = dChckPrc - (2 * pTrade->dTickSize);
		sprintf(zMsg, "매도청산주문(종가:%10.5f)(주문가:%10.5f)", dChckPrc, dOrdPrc);
	}

	// 중복수행 방지
	pTrade->mClrOpenHighLow = pzNowChartNm;

	// 기존 stop order 취소
	//m_pStopOrd->RemoveOrder(sSymbol);	=> AddNewStopOrder 함수에 내장

	// 신규 stop 주문 add
	char cOrdSide = (pTrade->Pos.cSide[0] == DEF_SELL) ? DEF_BUY : DEF_SELL;
	int nCnt = 0;
	int nQty = 1;	//pTrade->Pos.nOpenQty
	if (AddNewStopOrder(sSymbol, dOrdPrc, cOrdSide, nQty, pTrade->nDotCnt,
		zPrc, (char*)pzNowChartNm, STOPORD_OPENHILO, pTrade, &nCnt))
	{
		sprintf(msg, "%s%s[Strat_Clr_OpenHiLo][직전진입봉종가 +/- 2틱 STOP미리설정](%s)(%s)(%s)(진입봉:%.*s)(현재봉:%s)(%s)(총갯수:%d)",
			LOG1_STRAT_ORD, LOG2_STOP_SET, AcntNo(),
			pTrade->GroupKeyEx(),
			sSymbol.GetBuffer(),
			LEN_CHART_NM,
			chartPos.Nm,
			pTrade->mClrOpenHighLow.GetBuffer(),
			zMsg,nCnt
		);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
	}
}


/* try catch 를 위해*/
void CFBOrdThread::DoProc_RecvAPIReplyWrapper( At0985O_st* p, _In_ CString& sSymbol)
{
	__try
	{
		__try
		{
			DoProc_RecvAPIReply(p, sSymbol);
		}
		__except (ReportException(GetExceptionCode(), "CFBOrdThread::DoProc_RecvAPIReplyWrapper", m_szMsg))
		{
			g_log.log(LOGTP_FATAL, m_szMsg);
		}
	}
	__finally
	{

	}
}

/*
	API 로 부터 주문/체결 결과 수신 받은 것 처리

	- ESApiDllDlgDlg::OnRecvData
*/

VOID CFBOrdThread::DoProc_RecvAPIReply( At0985O_st* p, _In_ CString& sSymbol)
{
	int nOrigNo = S2N((char*)p->Orig_ord_no, sizeof(p->Orig_ord_no));
	int nOrdNo = S2N((char*)p->Ord_no, sizeof(p->Ord_no));
	int nRemainQty = S2N((char*)p->RemainQty, sizeof(p->RemainQty));
	

	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]종목정보가 없는데 주문/체결 수신(주문번호:%d)", sSymbol.GetBuffer(), nOrdNo);
		return;
	}
	char msg[1024];
	/*
		포지션구분:현재포지션의 구분. SIDE:체결SIDE 
			-> ADD : 둘이 같다.
			-> 일부청산 : 둘이 다르다.
		잔고수량:현재포지션수량
		청산가능수량 : 잔고수량-미체결수량
		[체결수신][API_수신](GCQ7)(포지션구분:1)(SIDE:1)(청산가능수량:0000000001)(잔고수량:0000000001)(체결가격:1254.90000)(체결수량:1                   )(체결번호:00047)
		[체결수신][API_수신](GCQ7)(포지션구분:1)(SIDE:1)(청산가능수량:0000000001)(잔고수량:0000000002)(체결가격:1255.00000)(체결수량:1                   )(체결번호:00048)
		[체결수신][API_수신](GCQ7)(포지션구분:1)(SIDE:1)(청산가능수량:0000000002)(잔고수량:0000000003)(체결가격:1255.00000)(체결수량:1                   )(체결번호:00049)
		[체결수신][API_수신](GCQ7)(포지션구분:1)(SIDE:2)(청산가능수량:0000000001)(잔고수량:0000000002)(체결가격:1254.90000)(체결수량:1                   )(체결번호:00050)
		[체결수신][API_수신](GCQ7)(포지션구분:1)(SIDE:2)(청산가능수량:0000000000)(잔고수량:0000000001)(체결가격:1254.90000)(체결수량:1                   )(체결번호:00051)
		[체결수신][API_수신](GCQ7)(포지션구분:2)(SIDE:2)(청산가능수량:0000000000)(잔고수량:0000000000)(체결가격:1254.90000)(체결수량:1                   )(체결번호:00052)
	*/
	if (p->ReplyType[0] == DEF_RSLT_MATCH) 
	{
		sprintf(msg,  "%s%s(%s)(포지션구분:%c)(SIDE:%c)(청산가능수량:%.*s)(잔고수량:%.*s)(체결가격:%10.5f)(체결수량:%.*s)(체결번호:%.*s)"
			, LOG1_CNTR_RECV, LOG2_ORD_API_R
			, sSymbol.GetBuffer()
			, p->Open_tp[0]
			, p->Side[0]
			, sizeof(p->Rsrb_q), p->Rsrb_q
			, sizeof(p->Open_q), p->Open_q
			, S2D((char*)p->ExecPrice,sizeof(p->ExecPrice))
			, sizeof(p->ExecQty), p->ExecQty
			, sizeof(p->Trd_no), p->Trd_no
		);
		g_log.log(LOGTP_SUCC,  msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_CNTR_RECV, LOG2_ORD_API_R, msg);
	}
	else {
		sprintf(msg, "%s%s(%s)(응답유형:%c)구분(%c)(주문수량:%.*s)(주문잔량:%.*s)(주문가격:%10.5f)(주문번호:%d)(원주문번호:%d)"
			, LOG1_ORD_RECV, LOG2_ORD_API_R
			, sSymbol.GetBuffer()
			, p->ReplyType[0]
			, p->Side[0]
			, sizeof(p->Qty), p->Qty
			, sizeof(p->RemainQty), p->RemainQty
			, S2D((CHAR*)p->Price,sizeof(p->Price))
			, nOrdNo
			, nOrigNo
		);
		g_log.log(LOGTP_SUCC, "%s", msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_ORD_RECV, LOG2_ORD_API_R, msg);
	}


	///////////////////////////////////////////////////////////
	// 거부주문 처리, 
	if (p->ReplyType[0] == DEF_RSLT_RJCT_NEW ||
		p->ReplyType[0] == DEF_RSLT_RJCT_MDFY ||
		p->ReplyType[0] == DEF_RSLT_RJCT_CNCL
		)
	{
		g_log.log(LOGTP_ERR, "주문거부수신[%s][원주문번호:%d]", sSymbol.GetBuffer(), nOrigNo);
		return;
	}


	char zChartNm[32];
	char zTradeTm[32];
	sprintf(zTradeTm, "%.8s", p->TradeTime);
	GET_CHART_NM(zTradeTm, pTrade->Tp(), zChartNm);//0606

	////////////////////////////////////////////////////////////
	// 신규 / 정정 / 취소 주문
	int nIdx = -1;
	if (p->ReplyType[0] == DEF_RSLT_NEW || p->ReplyType[0] == DEF_RSLT_MDFY || p->ReplyType[0] == DEF_RSLT_CNCL)
	{
		char zPrc[LEN_PRC + 1];
		double dPrc = S2D((char*)p->Price, sizeof(p->Price));
		FORMAT_PRC(dPrc, pTrade->nDotCnt, zPrc);

		if (p->ReplyType[0] == DEF_RSLT_NEW)
		{
			int nCnt = pTrade->LimitOrd_Add(zChartNm, nOrdNo, nOrigNo, nRemainQty, p->Side[0], zPrc, pTrade->TotProfitCutTick );
			sprintf(msg, "%s%s[DoProc_RecvAPIReply]지정가주문추가(%s)(%s)(주문번호:%d)(SIDE:%c)(가격:%10.5f)(총갯수:%d)",
				LOG1_ORD_RECV, LOG2_ORD_API_R, AcntNo(), sSymbol.GetBuffer(), nOrdNo, p->Side[0], dPrc, nCnt);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_ORD_RECV, LOG2_ORD_API_R, msg);
		}
		else if (p->ReplyType[0] == DEF_RSLT_MDFY)
		{
			if( !pTrade->LimitOrd_Modify(zChartNm, nOrdNo, nOrigNo, nRemainQty, p->Side[0], zPrc))
			{
				g_log.log(LOGTP_ERR, "원주문정보가 없이 정정 수신(원주문번호:%d)(주문번호:%d)", nOrigNo, nOrdNo);
			}
		}
		else if (p->ReplyType[0] == DEF_RSLT_CNCL)
		{
			int nResult = pTrade->LimitOrd_Cancel(nOrigNo);
		}

		UpdateTradeMap(sSymbol, pTrade, FALSE);

	} // if (p->ReplyType[0] == DEF_RSLT_NEW || p->ReplyType[0] == DEF_RSLT_MDFY || p->ReplyType[0] == DEF_RSLT_CNCL)

	if (p->ReplyType[0] == DEF_RSLT_MATCH)
	{
		// 체결전에 들어온 주문완료에 의한 정보 삭제
		int nOrdNo = S2N((char*)p->Ord_no, sizeof(p->Ord_no));
		int nCnt = pTrade->LimitOrd_Cancel(nOrdNo);

		sprintf(msg, "%s%s[DoProc_RecvAPIReply](%s)(%s)체결수신하여 주문정보삭제(주문번호:%d)(남은주문갯수:%d)",
			LOG1_ORD_RECV, LOG2_ORD_API_R, AcntNo(), sSymbol.GetBuffer(), nOrdNo, nCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_ORD_RECV, LOG2_ORD_API_R, msg);
		UpdateTradeMap(sSymbol, pTrade, FALSE);

		int nOpenQty = S2N((char*)p->Open_q, sizeof(p->Open_q));

		char zChartNm[32];
		char zTradeTm[32];
		sprintf(zTradeTm, "%.8s",  p->TradeTime);
		GET_CHART_NM(zTradeTm, pTrade->Tp(), zChartNm);//0606

		// 진입주문이면 SL, PT 주문을 미리 건다.
		if (nOpenQty > 0)
		{
			// 이미 포지션이 있는 상태에서 추가 체결은 처리하지 않는다.
			if (IsPosOpened(pTrade))
			{
				sprintf(msg, "%s%s이미 포지션이 있는 상태에서 청산이 아닌 체결이 들어옴.처리하지 않는다.(%s)(%s)(기잔고수량:%d)(기잔고방향:%c)(신규포지션수량:%d)(체결방향:%c)(잔고방향:%c)",
					LOG1_STRAT_RQMNT, LOG2_ERROR, AcntNo(), sSymbol.GetBuffer(), pTrade->Pos.nOpenQty, pTrade->Pos.cSide[0], S2N(p->Open_q,sizeof(p->Open_q)), p->Side[0], p->Open_tp[0]);
				g_log.log(LOGTP_ERR, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
			}
			else
			{
				// 포지션정보 저장;
				pTrade->Pos.nCntrNo = S2N((char*)p->Trd_no, sizeof(p->Trd_no));
				pTrade->Pos.nOpenQty = nOpenQty;
				pTrade->Pos.cSide[0] = p->Side[0];
				sprintf(pTrade->Pos.zPosPrc, "%.*s", sizeof(p->ExecPrice), p->ExecPrice);
				sprintf(pTrade->Pos.zOpenTM, "%.*s", sizeof(p->TradeTime), p->TradeTime);
				sprintf(pTrade->Pos.zChartNm, zChartNm);
				//GET_CHART_NM(pTrade->Pos.zOpenTM, pTrade->idxMyChart, pTrade->Pos.zChartNm);//0606

				UpdateTradeMap(sSymbol, pTrade, FALSE);

				// StopLoss, Profit 주문 
				// 포지션수량이 1 이상이면 주문걸지 않는다ㅑ.
				if (nOpenQty > 1)
				{
					sprintf(msg, "%s%s포지션수량이 1 이상이다.(%d)(%s)(%s)",
						LOG1_STRAT_RQMNT, LOG2_ERROR, nOpenQty, AcntNo(), sSymbol.GetBuffer());
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
				}
				if (nOpenQty == 1)
				{
					//기존같은방향 지정가 취소
					CancelSamesideLimitOrd(pTrade, p->Side[0]);
					
					SetStop_OnEntryPos(sSymbol);
					SetProfit_OnEntryPos(sSymbol);
				}
			} // else of if (IsPosOpened(pTrade))

		} // if (nOpenQty > 0)

		// 청산주문인 경우 당일 누적손실 체크 및 전략해제 체크
		else
		{
			// 진입가붕괴시 청산주문 중복방지 flag 초기화
			pTrade->mClrOpenBreak = FALSE;

			// 보조지표 청산주문 중복방지 FLAG 초기화
			pTrade->mAssistClrChecked = FALSE;

			// Open 가 전략 중복방지 초기화
			//for (int i = 0; i < CHART_TP_CNT; i++) {
			//	pTrade->mStratOpenDone[i] = "";
			//}

			// 청산봉 저장
			pTrade->mLastClrChartNm = zChartNm;

			if (AcmlPL_CheckRelease(p, pTrade, sSymbol) == TRUE)
			{
				// 전략해제
				ReleaseStrategy(TRUE,'Y', "12틱/20틱 조건");
			}
			else
			{
				// 기존주문들 취소처리
				sprintf(msg, "%s%s청산수신하여 취소처리 시도한다.(%s)(%s)", LOG1_STRAT_ORD, LOG2_CNCL_SEND,AcntNo(), sSymbol.GetBuffer());
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
				CancelAllOrd_Symbol(sSymbol);
				ZeroMemory(&pTrade->Pos, sizeof(pTrade->Pos));
				UpdateTradeMap(sSymbol, pTrade, FALSE);
			}
		} // else if (nOpenQty > 0) 청산주문인 경우

		// DB 저장
		memset((void*)p->FullCode, 0x20, sizeof(p->FullCode));
		memcpy((void*)p->FullCode, sSymbol.GetBuffer(), sSymbol.GetLength());
		m_pStratLog->DBSaveCntr((char*)p);
	} // if (p->ReplyType[0] == DEF_RSLT_MATCH)
}


/*
	1. 손익 누적 
	2. 전략해제 점검. 
		- 누적수익이 12틱 이상인 경우 전략중단 / 누적손.실 20틱 이상이면 전략 중단
*/
BOOL CFBOrdThread::AcmlPL_CheckRelease(const At0985O_st* p, const ACNT_TRADE_INFO* pTrade, _In_ CString& sSymbol)
{
	//double dPow = pow(10., (double)pTrade->nDotCnt);
	//// 손익계산
	//double dOpenPrc = atof(pTrade->Pos.zPosPrc) * dPow;
	//double dClrPrc = S2D((char*)p->ExecPrice, sizeof(p->ExecPrice)) * dPow;
	//int nGapTick = 0;
	//if (pTrade->Pos.cSide[0] == DEF_BUY)
	//{
	//	nGapTick = (int)((dClrPrc - dOpenPrc) / pTrade->dTickSize / dPow);
	//}
	//if (pTrade->Pos.cSide[0] == DEF_SELL)
	//{
	//	nGapTick = (int)((dOpenPrc - dClrPrc) / pTrade->dTickSize / dPow);
	//}

	if (pTrade->Pos.nOpenQty == 0)
		return FALSE;

	if (atof(pTrade->Pos.zPosPrc) <= 0)
		return FALSE;

	int nGapTick;
	double dOpenPrc = atof(pTrade->Pos.zPosPrc);
	double dClrPrc = S2D((char*)p->ExecPrice, sizeof(p->ExecPrice));
	if (pTrade->Pos.cSide[0] == DEF_BUY)
		nGapTick = CUtil::GetTickGap(dClrPrc, dOpenPrc, pTrade->nDotCnt, pTrade->dTickSize);
	else
		nGapTick = CUtil::GetTickGap(dOpenPrc, dClrPrc, pTrade->nDotCnt, pTrade->dTickSize);

	// 누적손익tick sum
	UpdateAcmlPL(nGapTick);

	BOOL bRelease = FALSE;
	// 전략 해제
	if (m_nAccmPLTick >= pTrade->TotProfitCutTick)
		bRelease = TRUE;

	if (m_nAccmPLTick <= pTrade->TotLossLimitTick)
		bRelease = TRUE;

	return bRelease;
}


/*
당일 누적손익 저장
*/
VOID CFBOrdThread::UpdateAcmlPL(int nPL)
{ 
	m_nAccmPLTick += nPL; 

	// DB 저장
	CDBHandler db(GET_DBPTR->Get());

	CString sQ;
	sQ.Format("UPDATE FB_ACNT_INFO SET ACCM_PL_TICK = %d WHERE ACNT_NO = '%s'", m_nAccmPLTick, m_sAcnt.GetBuffer());
	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE == db->Exec())
	{
		g_log.log(LOGTP_ERR, "[UpdateAcmlPL]FB_ACNT_INFO ACCML_PL_TICK  UPDATE 오류(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
	}
};


/*
	특정종목 전략 해제
*/
VOID CFBOrdThread::ReleaseSymbol(CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL)	return;

	CancelAllOrd_Symbol(sSymbol);
	delete pTrade;

	UpdateTradeMap(sSymbol, pTrade, TRUE);

	char msg[1024];
	sprintf(msg, "%s%s[ReleaseSymbol][%s][%s] 종목 전략 해제", LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, AcntNo(), sSymbol.GetBuffer());
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, msg);
}


VOID CFBOrdThread::ClearAllOrdPos(CString& sSymbol)
{
	// 미체결주문취소
	MAPIT_ACNT_TRADE it;
	MAPLOCK();
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		CString symbol = (*it).first;
		if(sSymbol.Compare(symbol)==0)
			CancelAllOrd_Symbol(symbol);
	}

	// 포지션청산	
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		CString symbol = (*it).first;
		if (sSymbol.Compare(symbol) != 0)
			continue;
		ACNT_TRADE_INFO* p = (*it).second;

		if (p->IsPosOpened()) {
			char cSide = (p->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
			DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, NULL/*시장가*/, cSide, p->Pos.nOpenQty, symbol, NULL);
		}
	}
	/*
	
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		delete (*it).second;
	}
	m_mapTrade.clear();
	*/
	MAPUNLOCK();
}

// 전략해제
VOID CFBOrdThread::ReleaseStrategy(const BOOL bDBSave, const char cNotifySaveYN, const char* pzReleaseNote)
{
	// 기존주문취소처리
	MAPIT_ACNT_TRADE it; 
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		CString symbol = (*it).first;
		CancelAllOrd_Symbol(symbol);
	}

	m_bDoStrategy = FALSE;
	ZeroMemory(&m_CommFactor, sizeof(m_CommFactor));

	// tradeinfo 초기화	
	MAPLOCK();
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		delete (*it).second;
	}		
	m_mapTrade.clear();
	MAPUNLOCK();

	if (!bDBSave)
		return;

	// DB 저장
	CDBHandler db(GET_DBPTR->Get());

	CString sQ;
	sQ.Format("EXEC FB_STRAT_UPDATE '%s', 'N', '%s', '%c' ", m_sAcnt.GetBuffer(), pzReleaseNote, cNotifySaveYN);
	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE == db->Exec())
	{
		g_log.log(LOGTP_ERR, "[ReleaseStrategy]FB_STRAT_UPDATE UPDATE 오류(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
	}
	else
	{
		char msg[1024];
		sprintf(msg, "%s%s[ReleaseStrategy][%s]전략 해제", LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, AcntNo());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), "ALL", (CHART_TP)0, LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, msg);
	}
	
}


/*
	진입되자 마자 12tick 익절 주문
*/
BOOL CFBOrdThread::SetProfit_OnEntryPos(CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]종목정보가 없는데 익절주문시도(%s)", sSymbol.GetBuffer());
		return FALSE;
	}

	int nDotCnt = pTrade->nDotCnt;
	double dOrdPrc = 0.;
	double dPosPrc = atof(pTrade->Pos.zPosPrc);
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		dOrdPrc = dPosPrc + (pTrade->TotProfitCutTick * pTrade->dTickSize);
		//g_log.log(LOGTP_SUCC, "LONG->매도PROFIT.AVG(%.*f)(설정가:%.*f)", nDotCnt, dPosPrc, nDotCnt, dOrdPrc);
	}
	else if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		dOrdPrc = dPosPrc - (pTrade->TotProfitCutTick * pTrade->dTickSize);
		//g_log.log(LOGTP_SUCC, "SHORT->매수PROFIT.AVG(%.*f)(설정가:%.*f)", nDotCnt, dPosPrc, nDotCnt, dOrdPrc);
	}

	char msg[1024];
	// 청산주문
	char zPrc[32];
	char cSide = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
	FORMAT_PRC(dOrdPrc, nDotCnt, zPrc);
	sprintf(msg, "%s%s[SetProfit_OnEntryPos][진입후PROFIT주문설정 (%s)(%s)(진입가:%.*f)(PRC:%.*f)(SIDE:%c)",
		LOG1_STRAT_ORD, LOG2_LIMIT_SEND, AcntNo(), sSymbol.GetBuffer(), nDotCnt, dPosPrc, nDotCnt, dOrdPrc, cSide);
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_LIMIT_SEND, msg);
	
	DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, zPrc, cSide, pTrade->Pos.nOpenQty, sSymbol, NULL);
	
	//char zProfitTick[32];
	//sprintf(zProfitTick, "%d", m_CommFactor.nTotProfitCutTick);
	//m_pStratLog->Logging(
	//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_ENTRY,
	//	CHK_RECV_OPEN, "기존진입가격/청산주문가", pTrade->Pos.zPosPrc, zPrc, "익절틱/SIDE", zProfitTick, "",
	//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
	//	ORD_LIMIT_CLR, cSide, zPrc, "", ""
	//);
	return TRUE;
}

/*
	1) 매수 진입이 되자 마자 기존 9개 봉 중 최저가 - 2 TICK 가격에 지정가 주문 설정
	2) 매도 진입이 되자 마자 기존 9개 봉 중 최고가 + 2 TICK 가격에 지정가 주문 설정
*/
BOOL CFBOrdThread::SetStop_OnEntryPos( CString& sSymbol)
{
	char zNowPackTime[32];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(zNowPackTime, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	
	char zGroupKey[32];
	char zChartNm[32];

	// GROUP KEY : 종목코드+차트TP

	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]종목정보가 없는데 손절주문시도(%s)", sSymbol.GetBuffer());
		return FALSE;
	}
	strcpy(zGroupKey, pTrade->sGroupKey[pTrade->Idx()]);


	// 현재 구성되고 있는 차트 이름
	GET_CHART_NM(zNowPackTime, pTrade->Tp(), zChartNm);

	ST_SHM_CHART_UNIT	nowChart, lastChart;	//	현재 구성되고 있는 차트

	// 과거 차트가 9개가 안되면 pass
	int nChartCnt = m_shm.GetCurrStructCnt(zGroupKey);
	if (nChartCnt < DEF_CHECK_CHART_CNT) {
		//g_log.log(LOGTP_SUCC, "차트가 %d개가 안된다.(%.5s)(%d)", 9, zGroupKey, nChartCnt);
		return FALSE;
	}

	// 현재 차트와 최종차트 정보 가져오기
	INT nRet = GetLastChartSHM(zGroupKey, zChartNm, lastChart, nowChart);
	if (nRet<0) {
		if (nRet == -1) {
			g_log.log(LOGTP_ERR, "[SetStop_OnEntryPos]수신(시각:%s,종목:%s) (GroupKey:%s, ChartNM:%s)",
				zNowPackTime, sSymbol.GetBuffer(), zGroupKey, zChartNm);
		}
		return FALSE; 
	}


	/*
	과거 9개의 차트를 가져와서
	- 매수진입상태에서 기존 9개봉중 최저가 2틱 아래 손절
	- 매도진입상태에서 기존 9개봉중 최고가 2틱 위   손절
	*/

	char prevChartNm[LEN_CHART_NM + 1];
	double dCheckPrc = 0., dOrdPrc=0.;
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, nowChart.prevNm);
	int nLoop = DEF_CHECK_CHART_CNT;	// (nChartCnt < m_nMaxChartCnt) ? nChartCnt : m_nMaxChartCnt;

	int i = -1;
	for (i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		ZeroMemory(&lastChart, sizeof(lastChart));
		if (FALSE == m_shm.GetData(zGroupKey, prevChartNm, (char*)&lastChart))
		{
			g_log.log(LOGTP_ERR, "[SetStop_OnEntryPos][%s][%s]차트가 SHM 에 없다", prevChartNm, zGroupKey);
			continue;
		}
		
		if (pTrade->Pos.cSide[0] == DEF_BUY)
		{
			double dLow = atof(lastChart.low);
			if (i == 0)
				dCheckPrc = dLow;
			else
				dCheckPrc = (dCheckPrc < dLow) ? dCheckPrc : dLow ;	// 최저가
		}
		else if (pTrade->Pos.cSide[0] == DEF_SELL)
		{
			double dHigh = atof(lastChart.high);
			if (i == 0)
				dCheckPrc = dHigh;
			else
				dCheckPrc = (dCheckPrc > dHigh) ? dCheckPrc : dHigh ;	// 최고가
		}
		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, lastChart.prevNm);

	} // for (int i = 0; i < nLoop; i++)

	int nDotCnt = pTrade->nDotCnt;
	double dPosPrc = atof(pTrade->Pos.zPosPrc);
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		dOrdPrc = dCheckPrc - (m_CommFactor.nLosscutTick * pTrade->dTickSize);

		// 15틱을 넘을 순 없다.
		int nGapTick = CUtil::GetTickGap(dOrdPrc, dPosPrc, pTrade->nDotCnt, pTrade->dTickSize);
		if (nGapTick < (MAX_ENTRY_STOP_TICK*-1))
		{
			dOrdPrc = dPosPrc - (MAX_ENTRY_STOP_TICK* pTrade->dTickSize);
		}
	}
	else if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		dOrdPrc = dCheckPrc + (m_CommFactor.nLosscutTick * pTrade->dTickSize);

		// 15틱을 넘을 순 없다.
		int nGapTick = CUtil::GetTickGap(dPosPrc, dOrdPrc, pTrade->nDotCnt, pTrade->dTickSize);
		if (nGapTick < (MAX_ENTRY_STOP_TICK*-1))
		{
			dOrdPrc = dPosPrc + (MAX_ENTRY_STOP_TICK* pTrade->dTickSize);
		}
	}

	// 청산주문
	char zPrc[32];
	char cSide = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
	int nCnt = 0;
	int nQty = 1;	//pTrade->Pos.nOpenQty
	if (AddNewStopOrder(sSymbol, dOrdPrc, cSide, nQty, nDotCnt,
		zPrc, zChartNm, STOPORD_OPEN, pTrade, &nCnt))
	{
		char msg[1024];
		sprintf(msg, "%s%s[SetStop_OnEntryPos][진입후STOP설정.(%s)(%s)(기준차트:%.8s)(기준가:%.*f)(로스컷틱:%d)(주문가격:%10.5f)(주문SIDE:%c)(총갯수:%d)",
			LOG1_STRAT_ORD, LOG2_STOP_SET, AcntNo(), sSymbol.GetBuffer(), lastChart.Nm, nDotCnt, dCheckPrc, m_CommFactor.nLosscutTick, atof(zPrc), cSide, nCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
	}
	//char zLossTick[32]; 
	//sprintf(zLossTick, "%d", m_CommFactor.nLosscutTick);
	//m_pStratLog->Logging(
	//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_ENTRY,
	//	CHK_RECV_OPEN, "기존진입가격/STOP설정가", pTrade->Pos.zPosPrc, zPrc, "손절틱/SIDE", zLossTick, "",
	//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
	//	ORD_STOP_SET, cSide, zPrc, "", ""
	//);
	return TRUE;
}

BOOL CFBOrdThread::AddNewStopOrder(CString& sSymbol, const double dPrc, const char cOrdSide,
								const int nPosQty, const int nDotCnt, _Out_ char* pzPrc, 
								_In_ char* pzChartNm, STOPORD_TP stopOrdTp, ACNT_TRADE_INFO* pTrade, _Out_ int* pnTotCnt)
{
	// STOP 기존 주문 취소
	m_pStopOrd->RemoveOrder(stopOrdTp);

	FORMAT_PRC(dPrc, nDotCnt, pzPrc);
	int nResult = m_pStopOrd->AddNewOrd(&sSymbol, pzPrc, cOrdSide, nPosQty, pzChartNm, stopOrdTp, pnTotCnt);
	if (nResult == 0)
	{
		char msg[1024];
		sprintf(msg, "%s%s[AddNewStopOrder]이미 같은방향,같은가격 STOP있어서 설정거부(%s)(%s)%s", 
			LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), m_pStopOrd->GetMsg());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
		return FALSE;
	}
	return (nResult > 0);
}

/*
	현재가와 보조지표를 비교하여 조건이 맞는 경우 청산
	1. 붕괴 : 보조지표 > 현재가 => 매수포지션 매도주문 청산만
	2. 돌파 : 보조지표 < 현재가 => 매도포지션 매수주문 청산만
*/
VOID	CFBOrdThread::Strat_AssistClr(CString& sSymbol, _In_ ACNT_TRADE_INFO* pTrade, const char* pSise)
{
	if (NULL == pSise)	return;
	if (FALSE == IsPosOpened(sSymbol))	return;
	if (FALSE == pTrade->IsExistAssistPrc())	return;
	if (pTrade->mAssistClrChecked == TRUE)		return;

	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;
	
	// 매수포지션인경우
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		// 붕괴인가? 보조지표 > 현재가
		if (CUtil::CompPrc(pTrade->mAssist.zPrc, sizeof(pTrade->mAssist.zPrc), pPack->close, sizeof(pPack->close), pTrade->nDotCnt, LEN_PRC) > 0)
		{
			char msg[1024] = { 0, };
			sprintf(msg, "%s%s[Strat_AssistClr]보조지표붕괴 청산시장가 전송 전 취소부터 한다(%s)(%s)",
				LOG1_STRAT_ORD, LOG2_CNCL_SEND, AcntNo(), sSymbol.GetBuffer());
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);

			CancelAllOrd_Symbol(sSymbol);

			sprintf(msg, "%s%s[Strat_AssistClr][보조지표붕괴 청산시장가주문](%s)(%s)(%s)(POS SIDE:%c)(보조지표:%10.5f)>(현재가:%10.5f)",
				LOG1_STRAT_ORD, LOG2_LIMIT_SEND, AcntNo(), pTrade->sGroupKey[pTrade->Idx()].GetBuffer(), sSymbol.GetBuffer(),
				pTrade->Pos.cSide[0], atof(pTrade->mAssist.zPrc), S2D(pPack->close, sizeof(pPack->close)));
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_LIMIT_SEND, msg);

			DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, NULL, DEF_SELL, m_CommFactor.nOrdQTy, sSymbol, NULL);
			pTrade->mAssistClrChecked = TRUE;
		}
	}
	else
	{
		// 돌파인가? 보조지표 < 현재가
		if (CUtil::CompPrc(pTrade->mAssist.zPrc, sizeof(pTrade->mAssist.zPrc), pPack->close, sizeof(pPack->close), pTrade->nDotCnt, LEN_PRC) < 0)
		{

			char msg[1024] = { 0, };
			sprintf(msg, "%s%s[Strat_AssistClr]보조지표돌파 청산시장가 전송 전 취소부터 한다(%s)(%s)",
				LOG1_STRAT_ORD, LOG2_CNCL_SEND, AcntNo(), sSymbol.GetBuffer());
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);

			CancelAllOrd_Symbol(sSymbol);

			sprintf(msg, "%s%s[Strat_AssistClr][보조지표돌파 청산시장가주문](%s)(%s)(%s)(POS SIDE:%c)(보조지표:%10.5f)<(현재가:%10.5f)",
				LOG1_STRAT_ORD, LOG2_LIMIT_SEND, AcntNo(), pTrade->sGroupKey[pTrade->Idx()].GetBuffer(), sSymbol.GetBuffer(),
				pTrade->Pos.cSide[0], atof(pTrade->mAssist.zPrc), S2D(pPack->close, sizeof(pPack->close)));
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_LIMIT_SEND, msg);

			DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, NULL, DEF_BUY, m_CommFactor.nOrdQTy, sSymbol, NULL);
			pTrade->mAssistClrChecked = TRUE;
		}

	}
}


/*
1. 시세로만 체크한다.
2. 포지션이 있을때만 체크한다.
3. 진입봉에서는 이루어지지 않고, 그 다음 생성봉 부터 적용

- 진입가격 붕괴시 청산

*/
VOID CFBOrdThread::Strat_Clr_OpenPrcBreak(const char* pSise, _In_ CString& sSymbol)
{
	if (NULL == pSise)	return;
	if (FALSE == IsPosOpened(sSymbol))	return;

	////////////////////////////////////////////////////
	// GROUP KEY : 종목코드+차트TP
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]종목정보가 없는데 진입가붕괴전략시도(%s)", AcntNo(), sSymbol.GetBuffer());
		return;
	}
	
	BOOL bClear = FALSE;
	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;
	char zNowTime[32], zOpenTime[32];
	char zGroupKey[32];
	char zChartNm[32], zOpenChartNm[32];

	// 시세시각
	sprintf(zNowTime, "%.8s", pPack->Time);

	// 진입시각
	sprintf(zOpenTime, "%.8s", m_mapTrade[sSymbol]->Pos.zOpenTM);

	// 현재 구성되고 있는 차트 이름
	GET_CHART_NM(zNowTime, pTrade->Tp(), zChartNm);

	// 진입시각의 차트 이름
	GET_CHART_NM(zOpenTime, pTrade->Tp(), zOpenChartNm);

	// 진입시각과 현재구성중인 차트가 같은 시간대이면 전략을 수행하지 않는다.
	if (strcmp(zOpenChartNm, zChartNm) >= 0)
		return;

	// 이미 전송했으면 SKIP 한다.
	if (pTrade->mClrOpenBreak==TRUE) {
		//g_log.log(LOGTP_SUCC, "이미 직전차트 전략 수행(%.4s)", chart[0].Nm);
		return;
	}
		
	strcpy(zGroupKey, pTrade->sGroupKey[pTrade->Idx()]);
	
	double dPrc = S2D(pPack->close, sizeof(pPack->close));
	double dPosPrc = 0.;

	char msg[1024];

	// 매수포지션. 진입가 > 현재가
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		if (atof(pTrade->Pos.zPosPrc) > dPrc) {
			bClear = TRUE;
			dPosPrc = atof(pTrade->Pos.zPosPrc) - pTrade->dTickSize;	// 1tick 조정
			sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak]진입가붕괴MATCH.매수포지션.(평단:%10.5f)(현재가:%10.5f)(주문가:%10.5f)", 
				LOG1_STRAT_RQMNT, LOG2_STRAT,atof(pTrade->Pos.zPosPrc), dPrc, dPosPrc);
		}
	}

	// 매도포지션. 진입가 < 현재가
	if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		if (atof(pTrade->Pos.zPosPrc) < dPrc) {
			bClear = TRUE;
			dPosPrc = atof(pTrade->Pos.zPosPrc) + pTrade->dTickSize;	// 1tick 조정
			sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak]진입가돌파MATCH.매도포지션.(평단:%10.5f)(현재가:%10.5f)(주문가:%10.5f)",
				LOG1_STRAT_RQMNT, LOG2_STRAT, atof(pTrade->Pos.zPosPrc), dPrc, dPosPrc);
		}
	}
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);

	// 청산주문 전송
	if (bClear) {

		// 진입봉청산불가
		// 진입봉에서는 청산하지 않는다.
		if (pTrade->IsSameChartOfOpen(zChartNm))
		{
			sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak](%s)진입가붕괴 청산주문을 해야하나, 진입봉(%.*s)과 현재봉(%.*s)이 같으므로 하지 않는다.",
				LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), LEN_CHART_NM, pTrade->Pos.zChartNm, LEN_CHART_NM, zChartNm);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);

			return;
		}

		///////////////////////////////////////////////////////////
		// 기존주문취소부터 한다.
		sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak]진입가붕괴 청산지정가 주문 전송 전 취소부터 한다(%s)(%s)",
			LOG1_STRAT_ORD, LOG2_CNCL_SEND, AcntNo(), sSymbol.GetBuffer());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
		CancelAllOrd_Symbol(sSymbol);

		char zPrc[32];
		char cSide = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
		//sprintf(zPrc, "%f", dPosPrc);
		FORMAT_PRC(dPosPrc, pTrade->nDotCnt, zPrc);
		
		sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak][진입가붕괴 청산지정가주문](%s)(%s)(%s)[진입차트:%.*s][점검차트:%.*s](SIDE:%c)(진입가:%s)(주문가:%s)",
			LOG1_STRAT_ORD, LOG2_LIMIT_SEND,AcntNo(), zGroupKey, sSymbol.GetBuffer(),
			LEN_CHART_NM, zOpenChartNm, LEN_CHART_NM, zChartNm,
			pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, zPrc);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_LIMIT_SEND, msg);

		DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, zPrc, cSide, pTrade->Pos.nOpenQty, sSymbol, NULL);
		pTrade->mClrOpenBreak = TRUE;

		
		//m_pStratLog->Logging(
		//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_SISE,
		//	CHK_ENT,RY_DOWN, "진입차트/진입가격", zOpenChartNm, pTrade->Pos.zPosPrc, "현재차트/현재가격", zChartNm, pPack->close,
		//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
		//	ORD_STOP_SET, cSide, zPrc, "", "진입가격붕괴에 의한 청산주문 전송"
		//);
	} // if (bClear)
}


/*
	Strategy-1
	
1) 새로 생성된 봉의 시가를 이전 9개봉의 시가와 비교를 한다.
2) 일치하는 시가가 양봉의 시가이면 매수, 음봉의 시가이면 매도
3) 즉, 캔들마다 1번만 수행한다.

*/
VOID CFBOrdThread::Strat_Open(
	_In_ CString& sSymbol,
	const CHART_IDX idx,
	const char* pzGroupKey,
	const char* pzNowChartNm,
	ACNT_TRADE_INFO* pTrade,
	_In_ const ST_SHM_CHART_UNIT& nowChart,
	_In_ const ST_SHM_CHART_UNIT& prevChart
	//char* pzPackTime
)
{

	// 이미 전략을 수행했으면 나간다.  CHECK
	// sDone(1201) 보다 zNowChartNm(1201 or 1200) 이 같거나 이전.
	CString sDone = pTrade->mStratOpenDone;
	if ( sDone.Compare(pzNowChartNm)>=0 ) {
		return;
	}

	// 과거 차트가 9개가 안되면 pass
	int nChartCnt = m_shm.GetCurrStructCnt((char*)pzGroupKey);
	if (nChartCnt < DEF_CHECK_CHART_CNT) {
		//g_log.log(LOGTP_SUCC, "차트가 %d개가 안된다.(%.5s)(%d)", DEF_CHECK_CHART_CNT, pzGroupKey, nChartCnt);
		return;
	}

	/*
		과거 9개의 차트의 양,음봉 구분, 시가 를 가져와서
		1. 최종차트의 시가가 양봉시가에도 있고, 음봉시가에도 있으면 pass
		2. 과거 양봉중 시가가 일치하는게 있으면 매수주문
		3. 과거 음봉중 시가가 일치하는게 있으면 매도주문
	*/
	
	ST_SHM_CHART_UNIT	chart[DEF_CHECK_CHART_CNT];	//	완료된 차트 9개

	INT nFoundPlus = -1, nFoundMinus = -1;

	char prevChartNm[LEN_CHART_NM + 1];
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, nowChart.prevNm);

	char temp[128];
	//g_log.log(LOGTP_SUCC, "[STRAT_OPEN.%s][%s][NOW:%.*s](%c)(%10.5f)",
	//	AcntNo(), pzGroupKey, LEN_CHART_NM, nowChart.Nm, nowChart.gb[0], atof(nowChart.open));

	int nLoop = DEF_CHECK_CHART_CNT;	//(nChartCnt < m_nMaxChartCnt) ? nChartCnt : m_nMaxChartCnt;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_shm.GetData((char*)pzGroupKey, (char*)prevChartNm, (char*)&chart[i]))
		{
			g_log.log(LOGTP_ERR, "[Strat_Open][%s][%s]차트가 SHM 에 없다", prevChartNm, pzGroupKey);
			continue;
		}
		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, chart[i].prevNm);

		//g_log.log(LOGTP_SUCC, "[strat_open:%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, chart[i].Nm, chart[i].gb[0], atof(chart[i].open));

		// 음봉도 양봉도 아닌 경우
		if (chart[i].gb[0] != DEF_CANDLE_PLUS && chart[i].gb[0] != DEF_CANDLE_MINUS) {
			continue;
		}

		// 시가가 일치하는가?
		//if (atof(nowChart.open) != atof(chart[i].open))
		if(!IsSamePrc(nowChart.open, sizeof(nowChart.open), chart[i].open, sizeof(chart[i].open), pTrade->nDotCnt, temp))
			continue;

		// 양봉 또는 음봉인가?
		if (chart[i].gb[0] == DEF_CANDLE_PLUS)	nFoundPlus = i;
		if (chart[i].gb[0] == DEF_CANDLE_MINUS)	nFoundMinus = i;

	}
	//g_log.log(LOGTP_SUCC, "--------------------------------------------------------");

	// 중복실행 방지	
	pTrade->mStratOpenDone = pzNowChartNm;

	if ((nFoundPlus == -1) && (nFoundMinus == -1))
	{
		return;
	}

	// 기 포지션이 있으면 청산주문만 가능
	BOOL bMustClrOrd = FALSE; 
	

	// 둘다 일치. 일치하는 차트가 있어도 음봉, 양봉 모두 일치하면 안된다.
	// 단, 청산방향은 괜찮다.
	
	if (nFoundPlus > -1 && nFoundMinus > -1)
	{
		if (pTrade->IsPosOpened())
			bMustClrOrd = TRUE;
		else
			return;
	}

	char msg[1024];
	/////////////////////////////////////////////////////////
	// 진입주문 수행
	ORD_TP ordTp;
	char side;
	if (nFoundPlus > -1 || nFoundMinus>-1) 
	{
		int nPos = (nFoundPlus > -1) ? nFoundPlus : nFoundMinus;
		
		// 음봉,양봉 모두 일치하고 기존 포지션이 있으면 청산주문 한다.
		if(bMustClrOrd)
		{
			ordTp = ORD_LIMIT_CLR;
			side = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
			sprintf(msg, "%s%s[Strat_Open][OPEN가 전략]음봉,양봉모두 일치하나 청산주문(%s)(%s)(현재봉 시가:[%.*s]Open:%7.5f) (일치:[%.*s][%c]Open:%7.5f)",
				LOG1_STRAT_RQMNT, LOG2_STRAT,AcntNo(), pzGroupKey,
				LEN_CHART_NM, nowChart.Nm, atof(nowChart.open),
				LEN_CHART_NM, chart[nPos].Nm, chart[nPos].gb[0], atof(chart[nPos].open)
			);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		}
		else 
		{
			// 매수주문
			if (nFoundPlus > -1)
			{
				// 매수Signal 표시 (0606)
				if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
					SetSignal(DEF_BUY, pzGroupKey, pzNowChartNm);
					sprintf(msg, "%s%s[Strat_Open][SetSignal]매수신호발생(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
				}

				// 이미 매수진입을 했는지 점검
				if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_BUY))
					return;
				else if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_SELL))
					ordTp = ORD_LIMIT_CLR;
				else
					ordTp = ORD_LIMIT_ENTRY;
			}

			// 매도주문
			if (nFoundMinus > -1)
			{
				// 매도Signal 표시 (0606)
				if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
					SetSignal(DEF_SELL, pzGroupKey, pzNowChartNm);
					sprintf(msg, "%s%s[Strat_Open][SetSignal]매도신호발생(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
				}


				// 이미 매도진입을 했는지 점검
				if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_SELL))
					return;
				else if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_BUY))
					ordTp = ORD_LIMIT_CLR;
				else
					ordTp = ORD_LIMIT_ENTRY;
			}

			side = (chart[nPos].gb[0] == DEF_CANDLE_PLUS) ? DEF_BUY : DEF_SELL;

			char zOrdTp[32] = { 0, };
			if (ordTp == ORD_LIMIT_CLR)	sprintf(zOrdTp, "청산신호");
			if (ordTp == ORD_LIMIT_ENTRY)	sprintf(zOrdTp, "진입신호");
			sprintf(msg, "%s%s[Strat_Open][OPEN가 전략](%s)(%s)(%s)(현재봉 시가:[%.*s]Open:%7.5f) (일치:[%.*s][%c]Open:%7.5f)",
				LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzGroupKey, zOrdTp,
				LEN_CHART_NM, nowChart.Nm, atof(nowChart.open),
				LEN_CHART_NM, chart[nPos].Nm, chart[nPos].gb[0], atof(chart[nPos].open));

			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		} // else of if(bMustClrOrd)

		// 진입주문이면 CROSS 를 본다. 데드크로스:매수진입만, 골든크로스:매도진입만. (0606)
		if (ordTp == ORD_LIMIT_ENTRY)
		{
			//	최종청산봉과 같은 봉이면 진입하지 않는다.
			if (pTrade->IsSameChartOfClr(nowChart.Nm))
			{
				sprintf(msg, "%s%s[Strat_Open](%s)[시가일치]로 주문을 해야하나 최종청산봉(%.*s)와 현재봉(%.*s)가 같다.",
					LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), LEN_CHART_NM, pTrade->mLastClrChartNm.GetBuffer(), LEN_CHART_NM, nowChart.Nm);

				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
				return;
			}

			// 매수진입의 경우, 현재봉의 직전봉이 양봉일 때만 매수진입한다.
			if (side == DEF_BUY)
			{
				if (chart[0].gb[0] != DEF_CANDLE_PLUS) // 음봉, 십자봉 모두 skip.	
				{
					sprintf(msg, "%s%s[Strat_Open](%s)[시가일치]로 매수진입주문을 해야하나 직전봉(%.*s)이 양봉이 아니므로 진입하지 않는다.",
						LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), LEN_CHART_NM, chart[0].Nm);

					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
					return;
				}
			}


			// 매도진입의 경우, 현재봉의 직전봉이 음봉일 때만 매도진입한다.
			if (side == DEF_SELL)
			{
				if (chart[0].gb[0] != DEF_CANDLE_MINUS)	// 양봉, 십자봉 모두 SKIP
				{
					sprintf(msg, "%s%s[Strat_Open](%s)[시가일치]로 매도진입주문을 해야하나 직전봉(%.*s)이 음봉이 아니므로 진입하지 않는다.",
						LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), LEN_CHART_NM, chart[0].Nm);

					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
					return;
				}
			}

			CROSS_TP crossTp = GetCross(prevChart, pTrade);
			sprintf(msg, "%s%s[Strat_Open]LONG SMA:%.20s    SHORT SM:%.20s", LOG1_STRAT_RQMNT, LOG2_REFER, prevChart.sma_long, prevChart.sma_short);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
			if ((crossTp == DEAD_CROSS) && (side == DEF_SELL) || (crossTp == GOLDEN_CROSS) && (side == DEF_BUY))
			{
				sprintf(msg, "%s%s[Strat_Open]OPEN PRC 일치. 그러나, CROSS 요건에 의해 주문SKIP(%s)(크로스:%d)(SIDE:%c)", 
					LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), (int)crossTp, side);
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
				return;
			}
		} // if (ordTp == ORD_LIMIT_ENTRY)
		else if (ordTp == ORD_LIMIT_CLR)	// 청산주문인 경우 진입봉에서 청산은 안되다.
		{
			if (pTrade->IsSameChartOfOpen(nowChart.Nm))
			{
				sprintf(msg, "%s%s[Strat_Open]시가일치에 의한 청산조건이나 진입봉(%.*s), 현재봉(%.*s) 가 일치하여 skip 한다.",
					LOG1_STRAT_RQMNT, LOG2_REFER, LEN_CHART_NM, pTrade->Pos.zChartNm, LEN_CHART_NM, nowChart.Nm);
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
				return;
			}
		}

		//if (pTrade->IsExistSameSideLimitOrd(side))
		//{
		//	g_log.log(LOGTP_SUCC, "시가일치로 주문해야 하나 이미 동방향 미체결이 있으므로 SKIP(계좌:%s)(심볼:%s)(방향:%c)",
		//		AcntNo(), sSymbol.GetBuffer(), side);
		//	return;
		//}
		DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, chart[nPos].open, side, m_CommFactor.nOrdQTy, sSymbol, NULL);

		char zSameChart[32]; sprintf(zSameChart, "%.*s", sizeof(chart[nPos].Nm), chart[nPos].Nm);
		//m_pStratLog->Logging(
		//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_SISE,
		//	CHK_OPENPRC, "현재캔들(mmddhhmm)/시가", (char*)pzNowChartNm, (char*)nowChart.open, "일치캔들(mmddhhmm)/가격", zSameChart, chart[nPos].open,
		//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
		//	ordTp, side, (char*)nowChart.open, "", "시가일치에 따른 주문"
		//);
	}
		
}


/*
	신호가 발생한 봉에 표시
*/
BOOL CFBOrdThread::SetSignal(const char cSide, const char* pzGroupKey, const char* pzChartNm)
{
	ST_SHM_CHART_UNIT stShm;
	int nPosSignal = sizeof(stShm.Nm) + sizeof(stShm.prevNm) + sizeof(stShm.gb) + sizeof(stShm.open)
		+ sizeof(stShm.high) + sizeof(stShm.low) + sizeof(stShm.close) + sizeof(stShm.dotcnt)
		+ sizeof(stShm.seq) + sizeof(stShm.sma_short) + sizeof(stShm.sma_long);
	
	char zSide[2];
	sprintf(zSide, "%c", cSide);

	return m_shm.SetData((char*)pzGroupKey, (char*)pzChartNm, nPosSignal, sizeof(stShm.signalGb), zSide);
}

/*
	보조지표점검(종가)
	- 현재의 직전 캔들 종가와 그 이전 9개의 캔들 종가 비교
*/
VOID CFBOrdThread::Strat_AssistSet(
	_In_ CString& sSymbol,
	const CHART_IDX idx,
	const char* pzGroupKey,
	const char* pzNowChartNm,
	ACNT_TRADE_INFO* pTrade,
	const ST_SHM_CHART_UNIT& lastChart,
	const ST_SHM_CHART_UNIT& nowChart
)
{

	// 이미 전략을 수행했으면 나간다.  CHECK
	// sDone(1201) 보다 zNowChartNm(1201 or 1200) 이 같거나 이전.
	CString sDone = pTrade->mStratCloseDone;
	if (sDone.Compare(pzNowChartNm) >= 0) {
		return;
	}

	//// 과거 차트가 10개가 안되면 pass (직전 이전부터 9개 이므로)
	int nChartCnt = m_shm.GetCurrStructCnt((char*)pzGroupKey);
	if (nChartCnt < DEF_CHECK_CHART_CNT + 1) {
		//g_log.log(LOGTP_SUCC, "차트가 %d개가 안된다.(%.5s)(%d)", DEF_CHECK_CHART_CNT + 1, pzGroupKey, nChartCnt);
		return;
	}

	/*
		1. 직전 캔들의 종가를 가져온다.
		2. 그 이후 9개의 캔들의 종가와 비교한다.
		3. 양봉은 양봉, 음봉은 음봉
	*/
	ST_SHM_CHART_UNIT	checkChart;
	INT nFound = -1;
	BOOL bSame;

	if (lastChart.gb[0] != '+' && lastChart.gb[0] != '-')
	{
		//g_log.log(LOGTP_SUCC, "[STRAT_CLOSE] 양봉, 음봉 모두 아니므로 skip(%s)(%s)(%.*s)(%10.5f)", 
		//			AcntNo(), pzGroupKey, LEN_CHART_NM, lastChart.Nm, atof(lastChart.close) );
		pTrade->mStratCloseDone = pzNowChartNm;
		return;
	}

	char prevChartNm[LEN_CHART_NM + 1];
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, lastChart.prevNm);
	//g_log.log(LOGTP_SUCC, "STRAT_CLOSE[%s][비교기준차트:%.*s](%c)(%10.5f)",
	//	 pzGroupKey, LEN_CHART_NM, lastChart.Nm, lastChart.gb[0], atof(lastChart.close));

	// 직전 캔들 이전 9개
	int nLoop = DEF_CHECK_CHART_CNT;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_shm.GetData((char*)pzGroupKey, (char*)prevChartNm, (char*)&checkChart))
		{
			g_log.log(LOGTP_ERR, "[STRAT_CLOSE][%s][%s]차트가 SHM 에 없다", prevChartNm, pzGroupKey);
			continue;
		}
		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, checkChart.prevNm);

		//g_log.log(LOGTP_SUCC, "start_close[%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, checkChart.Nm, checkChart.gb[0], atof(checkChart.close));

		// 부호가 다르면 PASS
		if (lastChart.gb[0] != checkChart.gb[0])
			continue;

		// 종가 일치하는가?
		bSame = IsSamePrc(
			checkChart.close, sizeof(checkChart.close),
			lastChart.close, sizeof(lastChart.close),
			pTrade->nDotCnt, pTrade->mAssist.zPrc
		);
		if (bSame)
		{
			nFound = i;
			break;
		}
	}
	//g_log.log(LOGTP_SUCC, "--------------------------------------------------------");

	// 중복실행 방지	
	pTrade->mStratCloseDone = pzNowChartNm;

	if (nFound == -1)
		return;
	
	// 보조지표가격 저장 시각 
	double dNowOpen = S2D((char*)nowChart.open, sizeof(nowChart.open));
	pTrade->mAssist.dwAssistClock = GetTickCount();
	double dAssistPrc = atof(lastChart.close);
	char msg[1024];
	sprintf(msg, "%s%s[Strat_AssistSet][보조지표설정](%s)(%s)(최근종가:[%.*s][%c]Close:%7.5f)(일치:[%.*s][%c]Close:%7.5f)(시가:%10.5f)",
		LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzGroupKey,
		LEN_CHART_NM, lastChart.Nm, lastChart.gb[0], dAssistPrc,
		LEN_CHART_NM, checkChart.Nm, checkChart.gb[0], dAssistPrc, dNowOpen);

	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);

	// 기존 보조신호에 의한 STOP 취소
	m_pStopOrd->RemoveOrder(STOPORD_ASSIST_BUY);
	m_pStopOrd->RemoveOrder(STOPORD_ASSIST_SELL);

	//	매도매수를 미리 걸어놓는다.
	//	가격은 보조지표 +/- 1틱
	char zPrc[32];
	char zChartNm[32];
	sprintf(zChartNm, "%.*s", LEN_CHART_NM, nowChart.Nm);
	int nCnt;
	char cSide = DEF_BUY;
	double dOrdPrc = dAssistPrc + (1 * pTrade->dTickSize);

	// CROSS CHECK
	CROSS_TP crossTp = GetCross(lastChart, pTrade);
	if (crossTp == GOLDEN_CROSS) 
	{
		sprintf(msg, "%s%s[Strat_AssistSet][보조지표설정.선매수STOP설정].그러나 CROSS 요건에 의해 생략.(%s)(%s)(골든크로스)(매수주문)",
			LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);		
	}
	else
	{
		// 보조신호 > 시가 ==> 매수
		if (CUtil::CompPrc(dAssistPrc, dNowOpen, pTrade->nDotCnt, LEN_PRC) < 0)
		{
			sprintf(msg, "%s%s[Strat_AssistSet][보조지표설정.선매수STOP설정].그러나 [보조신호<시가] 로 매수주문SKIP.(%s)(%s)(지표:%10.5f)(시가:%10.5f)",
				LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), dAssistPrc, dNowOpen);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);
		}
		else
		{
			if (AddNewStopOrder(sSymbol, dOrdPrc, cSide, m_CommFactor.nOrdQTy, pTrade->nDotCnt, zPrc, zChartNm, STOPORD_ASSIST_BUY, pTrade, &nCnt))
			{
				sprintf(msg, "%s%s[Strat_AssistSet][보조지표설정.선매수STOP설정](%s)(%s)(%s)(보조지표:%10.5f)(주문가격:%10.5f)(주문side:%c)(주문가격:%10.5f)(총갯수:%d)",
					LOG1_STRAT_ORD, LOG2_STOP_SET, AcntNo(),
					pTrade->GroupKeyEx(),
					sSymbol.GetBuffer(),
					dAssistPrc,
					dOrdPrc,
					cSide,
					dOrdPrc, nCnt);
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
			}
		}
	}
	
	cSide = DEF_SELL;
	dOrdPrc = dAssistPrc - (1 * pTrade->dTickSize);
	if (crossTp == DEAD_CROSS)
	{
		sprintf(msg, "%s%s[Strat_AssistSet][보조지표설정.선매도STOP설정].그러나 CROSS 요건에 의해 생략.(%s)(%s)(데드크로스)(매도주문)",
			LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);
	}
	else
	{
		// 보조신호 < 시가 ==> 매도
		if (CUtil::CompPrc(dAssistPrc, dNowOpen, pTrade->nDotCnt, LEN_PRC) > 0)
		{
			sprintf(msg, "%s%s[Strat_AssistSet][보조지표설정.선매도STOP설정].그러나 [보조신호>시가] 로 매도주문SKIP.(%s)(%s)(지표:%10.5f)(시가:%10.5f)",
				LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), dAssistPrc, dNowOpen);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);
		}
		else
		{
			if (AddNewStopOrder(sSymbol, dOrdPrc, cSide, m_CommFactor.nOrdQTy, pTrade->nDotCnt, zPrc, zChartNm, STOPORD_ASSIST_SELL, pTrade, &nCnt))
			{
				sprintf(msg, "%s%s[Strat_AssistSet][보조지표설정.선매도STOP설정](%s)(%s)(%s)(보조지표:%10.5f)(주문가격:%10.5f)(주문side:%c)(주문가격:%10.5f)(총갯수:%d)",
					LOG1_STRAT_ORD, LOG2_STOP_SET, AcntNo(),
					pTrade->GroupKeyEx(),
					sSymbol.GetBuffer(),
					dAssistPrc,
					dOrdPrc,
					cSide,
					dOrdPrc, nCnt);
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
			}
		}
	}
}

BOOL CFBOrdThread::IsSamePrc(
	const char* pPrc1, 
	const int nLen1, 
	const char* pPrc2, 
	const int nLen2, 
	int nDotCnt, 
	_Out_ char* pzPrc)
{
	char zPrc1[32], zPrc2[32];
	sprintf(zPrc1, "%.*s", nLen1, pPrc1);
	sprintf(zPrc2, "%.*s", nLen2, pPrc2);
	FORMAT_PRC(atof(zPrc1), nDotCnt, zPrc1);
	FORMAT_PRC(atof(zPrc2), nDotCnt, zPrc2);
	
	BOOL bSame = FALSE;
	if (strcmp(zPrc1, zPrc2) == 0) {
		bSame = TRUE;
		strcpy(pzPrc, zPrc1);
	}
	else {
		*pzPrc = 0x00;
	}

	return bSame;

}




CROSS_TP CFBOrdThread::GetCross(const ST_SHM_CHART_UNIT& shmLast, ACNT_TRADE_INFO *pTrade)
{
	if (!bCrossCheck)
		return NONE_CROSS;


	int nRet = CUtil::CompPrc(shmLast.sma_short, sizeof(shmLast.sma_short), 
						shmLast.sma_long, sizeof(shmLast.sma_long), pTrade->nDotCnt, LEN_PRC);
	char msg[512];
	CROSS_TP tp;

	double dShort = S2D((char*)shmLast.sma_short, sizeof(shmLast.sma_short));
	double dLong = S2D((char*)shmLast.sma_long, sizeof(shmLast.sma_long));
	if (dShort <= 0 || dLong <= 0)
	{
		sprintf(msg, "[GetCross]<NONE_CROSS-데이터이상> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
		tp = NONE_CROSS;
	}
	
	if (nRet == 0) {
		tp = NONE_CROSS;
		sprintf(msg, "[GetCross]<NONE_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}
	else if (nRet > 0) {
		tp = GOLDEN_CROSS;
		sprintf(msg, "[GetCross]<GOLDEN_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}
	else {
		tp = DEAD_CROSS;
		sprintf(msg, "[GetCross]<DEAD_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}

	//g_log.log(LOGTP_SUCC,"%s(%s)", msg, AcntNo());
	return tp;
}


// 현재 기준 현재 차트와 직전 차트 가져오기
INT CFBOrdThread::GetLastChartSHM(char* pzGroupKey, char* pzChartNm, ST_SHM_CHART_UNIT& shmLast, ST_SHM_CHART_UNIT& shmNow)
{
	char szChart[LEN_CHART_NM + 1];

	// 현재 차트 정보 가져오기
	BOOL bExist = m_shm.GetData(pzGroupKey, pzChartNm, (char*)&shmNow);

	// 1. 현재 차트가 없는 경우 ==> 나간다.
	if (FALSE == bExist)
	{
		// 지금시간 차트가 없는데, 지금시간 체결가는 들어온 상황이라면 차트 이상이다.
		g_log.log(LOGTP_ERR, "(%s)[%s][%s] 현재가수신했는데 현재차트가 없다.", AcntNo(), pzGroupKey, pzChartNm);
		return -1;
	} // if(!bExist)


	// 2. 현재 차트가 있는 경우 직전 차트 가져오기	  
	sprintf(szChart, "%.*s", LEN_CHART_NM, shmNow.prevNm);
	if (strcmp(szChart, "NONE") == 0)
		return -3;

	if (FALSE == m_shm.GetData(pzGroupKey, szChart, (char*)&shmLast))
	{
		g_log.log(LOGTP_ERR, "(%s)(%s)(%.*s) 직전 차트가 SHM 에 없다", AcntNo(), pzGroupKey, LEN_CHART_NM, szChart);
		return -2;
	}
	//g_log.log(LOGTP_SUCC, "직전차트(%.4s)", chart[0].Nm);


	return 1;
}


ACNT_TRADE_INFO* CFBOrdThread::GetTradeInfo(const CString& sSymbol)
{
	MAPLOCK();
	MAPIT_ACNT_TRADE itFind = m_mapTrade.find(sSymbol);
	if (itFind == m_mapTrade.end())
	{
		MAPUNLOCK();
		return NULL;
	}
	MAPUNLOCK();

	return  (*itFind).second;
}


// 기존 걸려있는 주문들 취소처리 (해당종목만)
VOID	CFBOrdThread::CancelAllOrd_Symbol(_In_ CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		//g_log.log(LOGTP_ERR, "(%s)[%s]종목정보가 없는데 전체취소시도", AcntNo(), sSymbol.GetBuffer());
		return;
	}

	// 설정된 STOP 취소
	//int nRemoveCnt = m_pStopOrd->RemoveOrder(sSymbol);
	int nRemoveCnt = m_pStopOrd->RemoveAllOrder();
	if (nRemoveCnt > 0) {
		char cnt[32]; sprintf(cnt, "%d", nRemoveCnt);
		char msg[1024]; 
		sprintf(msg, "%s%s[CancelAllOrd_Symbol]STOP주문취소처리(%s)(%s)(%d 개주문)", LOG1_STRAT_ORD, LOG2_STOP_CNCL, AcntNo(), sSymbol.GetBuffer(), nRemoveCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_CNCL, msg);
	}


	LIMIT_ORDER stOrd;
	while (pTrade->LimitOrd_GetFirstOrd(&stOrd))
	{
		char szOrigNo[32];
		sprintf(szOrigNo, "%-d", stOrd.nOrdNo);

		//g_log.log(LOGTP_SUCC, "[CancelAllOrd_Symbol][LimitOrd_GetFirstOrd]기존주문취소처리(%s)(%s)(주문번호:%d)", AcntNo(), sSymbol.GetBuffer(), stOrd.nOrdNo);

		DoOrder(ORD_TYPE_CNCL, FALSE/*STOP*/, stOrd.zPrc, stOrd.cSide[0], stOrd.nRemainQty, sSymbol, szOrigNo);

		//m_pStratLog->Logging(
		//	AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_CLR,
		//	CHK_LIMIT_CNCL, "", "", "", "", "", "",
		//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
		//	ORD_LIMIT_CNCL, 0x00, "", szOrigNo, "청산수신에 따른 STOP 취소"
		//);

	}

	UpdateTradeMap(sSymbol, pTrade, FALSE);

}


// 같은 방향 지정가 주문 취소
VOID	CFBOrdThread::CancelSamesideLimitOrd(ACNT_TRADE_INFO* pTrade, char cSide)
{
	char msg[1024];
	char szOrigNo[32];
	LIMIT_ORDER stOrd;
	ZeroMemory(&stOrd, sizeof(stOrd));
	while (pTrade->LimitOrd_GetFirstOrd(&stOrd))
	{
		if (stOrd.cSide[0] == cSide)
		{
			sprintf(szOrigNo, "%-d", stOrd.nOrdNo);

			DoOrder(ORD_TYPE_CNCL, FALSE/*STOP*/, stOrd.zPrc, stOrd.cSide[0], stOrd.nRemainQty, pTrade->sSymbol, szOrigNo);

			sprintf(msg, "%s%s[체결수신후 같은방향지정가취소](ACC:%s)(종목:%s)(방향:%c)(수량:%d)(원주문:%s)",
				LOG1_ORD_SND, LOG2_CNCL_SEND, 
				AcntNo(),
				pTrade->Symbol(),
				cSide,
				stOrd.nRemainQty,
				szOrigNo
			);
			g_log.log(LOGTP_SUCC, "%s", msg);
			m_pStratLog->LoggingEx(AcntNo(), pTrade->Symbol(), (CHART_TP)(pTrade->Tp()), LOG1_ORD_SND, LOG2_CNCL_SEND, msg);
		}

	}

}


VOID	CFBOrdThread::DoOrder(const char cOrdType, const BOOL bStop, const char* zPrc, const char cSide, const int nQty, _In_ CString& sSymbol, const char* sOrigNo)
{
	char			szBuff[4096];
	lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

	memset(szBuff, 0x20, 4096);

	char temp[128];
	sprintf(temp, "%010.0f", (double)(unsigned long)m_api->hWnd);
	memcpy(lpTrIn->Head.WinId, temp, strlen(temp));

	// 계좌번호
	memcpy(lpTrIn->Account, m_sAcnt, m_sAcnt.GetLength());

	// 비밀번호
	GET_APIPTR->GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), m_sAcnt, m_sPwd);

	// 주문구분	(1.신규주문 2.정정주문 3.취소주문)
	lpTrIn->Order_Type[0] = cOrdType;

	if (cOrdType == ORD_TYPE_CNCL)
		memcpy(lpTrIn->Order_Org_No, sOrigNo, min(sizeof(lpTrIn->Order_Org_No), strlen(sOrigNo)));

	// 종목코드
	memcpy(lpTrIn->ShortCode, sSymbol.GetBuffer(), sSymbol.GetLength());

	// 매도매수구분 (1.매수 2.매도). 
	lpTrIn->BuySell_Type[0] = cSide;

	// 가격조건	(1.지정가 2.시장가). 
	// 체결조건 (시장가일경우(3) 지정가일경우(1))
	if (NULL == zPrc || atof(zPrc)==0)
	{
		if (cOrdType == ORD_TYPE_NEW) 
		{
			lpTrIn->Price_Type[0] = '2';
			lpTrIn->Trace_Type[0] = '3';
			
		}
		//if (cOrdType == ORD_TYPE_NEW) 
		//{
		//	lpTrIn->Price_Type[0] = '1';
		//	lpTrIn->Trace_Type[0] = '1';
		//}
		memset(lpTrIn->Order_Price, 0x20, sizeof(lpTrIn->Order_Price));
	}
	else 
	{
		lpTrIn->Price_Type[0] = '1';
		lpTrIn->Trace_Type[0] = '1';
		if (cOrdType == ORD_TYPE_CNCL)
			memset(lpTrIn->Order_Price, 0x20, LEN_PRC);
		else
			memcpy(lpTrIn->Order_Price, zPrc, LEN_PRC);
	}
	// 주문수량
	sprintf(temp, "%*d", sizeof(lpTrIn->Order_Volume), nQty);
	memcpy(lpTrIn->Order_Volume, temp, sizeof(lpTrIn->Order_Volume));
	
	// 통신주문구분
	GET_APIPTR->GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

	// 주문전략구분 (1.일반주문 2.STOP주문)
	if (bStop) {
		lpTrIn->Stop_Type[0] = '2';
		memcpy(lpTrIn->Stop_Price, zPrc, LEN_PRC);	// STOP주문가격 (STOP주문이면 입력 아니면 0 셋팅)
	}
	else
		lpTrIn->Stop_Type[0] = '1';


	// 주문조작구분 
	if (cOrdType == ORD_TYPE_NEW)
		lpTrIn->Oper_Type[0] = '1';
	else if (cOrdType == ORD_TYPE_CNCL)
		lpTrIn->Oper_Type[0] = '3';

	APILOCK();
	if(cOrdType==ORD_TYPE_NEW)
		GET_APIPTR->SendTrData(ESID_5601, szBuff, SZ_Es5601I);
	else if (cOrdType == ORD_TYPE_CNCL)
		GET_APIPTR->SendTrData(ESID_5603, szBuff, SZ_Es5601I);
	APIUNLOCK();

	char msg[1024];
	sprintf(msg, "%s%s[주문유형:%c](ACC:%.*s)(종목:%.*s)(방향:%.*s)(수량:%.*s)(PRC:%.*s)(원주문:%.*s)",
		LOG1_ORD_SND, LOG2_ORD_API,lpTrIn->Order_Type[0],
		sizeof(lpTrIn->Account), lpTrIn->Account,
		LEN_SYMBOL, lpTrIn->ShortCode,
		sizeof(lpTrIn->BuySell_Type), lpTrIn->BuySell_Type,
		sizeof(lpTrIn->Order_Volume), lpTrIn->Order_Volume,
		sizeof(lpTrIn->Order_Price), lpTrIn->Order_Price,
		sizeof(lpTrIn->Order_Org_No), lpTrIn->Order_Org_No
	);
	g_log.log(LOGTP_SUCC, "%s", msg);
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_ORD_SND, LOG2_ORD_API, msg);

	// DB 저장
	//CDBHandler db(GET_DBPTR->Get());
	//
	//CString sQ;
	//sQ.Format("EXEC FB_ORD_LOG_SAVE '%.*s', '%s', '%.c', '%c', '%c', '%.*s', '%.*s', '%.*s' ", 
	//	sizeof(lpTrIn->Account), lpTrIn->Account,
	//	sSymbol.GetBuffer(),
	//	lpTrIn->Order_Type[0],
	//	lpTrIn->BuySell_Type[0],
	//	lpTrIn->Price_Type[0],
	//	sizeof(lpTrIn->Order_Price), lpTrIn->Order_Price,
	//	sizeof(lpTrIn->Order_Volume), lpTrIn->Order_Volume,
	//	sizeof(lpTrIn->Order_Org_No), lpTrIn->Order_Org_No
	//	);
	//if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE == db->Exec())
	//{
	//	g_log.log(LOGTP_ERR, "주문로그저장 오류(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
	//}
}


//BOOL CFBOrdThread::IsMyChartData(_In_ CHART_IDX idx, _In_ CString& sSymbol)
//{
//	return (MAPDATA_CHART_IDX(sSymbol) == idx);
//}

char* CFBOrdThread::GetMySymbol()
{
	CString sSymbol = (*m_mapTrade.begin()).first;
	return sSymbol.GetBuffer();
}

BOOL CFBOrdThread::IsMySymbolData(_In_ BOOL bSise, char* pData, CString& sSymbol, _Out_ CHART_IDX& idx)
{
	BOOL bMyData = FALSE;
	if (bSise) {
		ST_PACK2ORD* p = (ST_PACK2ORD*)pData;
		sSymbol.Format("%.*s", sizeof(p->shortCode), p->shortCode);
	}
	else
	{
		At0985O_st* p = (At0985O_st*)pData;
		CString sFullCode;
		sFullCode.Format("%.*s", sizeof(p->FullCode), p->FullCode);
		sSymbol = GET_APIPTR->GetShortCode(sFullCode);
	}

	MAPLOCK();
	MAPIT_ACNT_TRADE it = m_mapTrade.find(sSymbol);
	bMyData = (it != m_mapTrade.end()) ? TRUE : FALSE;
	if (bMyData)
		idx = ((*it).second)->Idx();
	MAPUNLOCK();

	return bMyData;
}


BOOL CFBOrdThread::IsMySymbol(_In_ CString& sSymbol)
{
	BOOL bMyData = FALSE;
	MAPLOCK();
	MAPIT_ACNT_TRADE it = m_mapTrade.find(sSymbol);
	bMyData = (it != m_mapTrade.end()) ? TRUE : FALSE;
	return bMyData;
}


BOOL CFBOrdThread::Initialize()
{
	if (!LoadCommonFactor())
		return FALSE;

	if (!LoadAcntTradeInfo())
		return FALSE;
	return TRUE;
}

// 공통정보 조회
BOOL	CFBOrdThread::LoadCommonFactor()
{
	//char sApplyYN[2];

	CDBHandler db(GET_DBPTR->Get());

	CString sQ;
	sQ.Format("EXEC FB_GET_STRATEGY_FACTOR");
	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE==db->Exec())
	{
		m_bDoStrategy = FALSE;
		g_log.log(LOGTP_ERR, "FB_GET_STRATEGY_FACTOR 수행 오류(%s)(%s)(%s)", AcntNo(), sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}

	if (db->NextRow())
	{
		m_CommFactor.nOrdQTy = db->GetInt("ORD_QTY");
		//m_CommFactor.nTotProfitCutTick = db->GetInt("TOT_PROFITCUT_TICK");
		//m_CommFactor.nTotLossLimitTick = db->GetInt("TOT_LOSS_LIMIT_TICK") * -1;
		m_CommFactor.nLosscutTick = db->GetInt("LOSSCUT_TICK");

		sprintf(&m_CommFactor.ChartYN[IDX_1MIN], "%.1s", db->Get("CHART_1MIN_YN"));
		sprintf(&m_CommFactor.ChartYN[IDX_5MIN], "%.1s", db->Get("CHART_5MIN_YN"));
		sprintf(&m_CommFactor.ChartYN[IDX_10MIN], "%.1s", db->Get("CHART_10MIN_YN"));
		sprintf(&m_CommFactor.ChartYN[IDX_30MIN], "%.1s", db->Get("CHART_30MIN_YN"));
		sprintf(&m_CommFactor.ChartYN[IDX_60MIN], "%.1s", db->Get("CHART_60MIN_YN"));


	} // if (db->NextRow())
	else
	{
		g_log.log(LOGTP_ERR, "FB_GET_STRATEGY_FACTOR 데이터가 없음");
		//m_bDoStrategy = FALSE;
		return FALSE;
	}


	//char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH], temp[128];
	//CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);
	//
	//// 최소 차트 수량
	//CUtil::GetConfig(szConfig, "CHART_INFO", "MIN_CHART_CNT", temp);
	//m_nMinChartCnt = atoi(temp);

	//CUtil::GetConfig(szConfig, "CHART_INFO", "MAX_CHART_CNT", temp);
	//m_nMaxChartCnt = atoi(temp);

	return TRUE;
}



// 계좌 전략정보 조회
BOOL	CFBOrdThread::LoadAcntTradeInfo()
{
	char temp[128];
	char sApplyYN[2];
	char msg[1024];

	//if (pDbPool->Available() < 1)
	//	return FALSE;
	CDBHandler db(GET_DBPTR->Get());

	CString sQ;
	sQ.Format("EXEC FB_GET_ACNT_STRATEGY_INFO '%s'", m_sAcnt.GetBuffer());
	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE == db->Exec())
	{
		m_bDoStrategy = FALSE;
		g_log.log(LOGTP_ERR, "FB_GET_ACNT_STRATEGY_INFO 수행 오류(%s)(%s)(%s)", AcntNo(), sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}

	m_bDoStrategy = FALSE;
	int nLoop = 0;
	while (db->NextRow())
	{
		if (nLoop == 0) {
			m_nAccmPLTick = db->GetInt("ACCM_PL_TICK");
			sprintf(sApplyYN, "%.1s", db->Get("APPLY_STRATEGY_YN"));
			m_bDoStrategy = (sApplyYN[0] == 'Y') ? TRUE : FALSE;
			m_sPwd.Format(db->Get("ACNT_PWD"));
		}

		CString sSymbol; 
		sSymbol.Format("%.*s", LEN_SYMBOL, db->Get("SYMBOL"));
	
		
		ACNT_TRADE_INFO *pTrade = new ACNT_TRADE_INFO;

		pTrade->sSymbol = sSymbol;

		CHART_IDX idx = (CHART_IDX)(db->GetInt("CHART_TP") - 1);
		pTrade->SetIdx(idx);
		
		pTrade->dTickSize = db->GetDouble("TICK_SIZE");
		pTrade->nDotCnt = db->GetInt("DOT_CNT");
		pTrade->TotProfitCutTick = db->GetInt("TOT_PROFITCUT_TICK");
		pTrade->TotLossLimitTick = db->GetInt("TOT_LOSS_LIMIT_TICK")*-1;

		GET_GROUP_KEY(sSymbol.GetBuffer(), TP_1MIN, temp);
		pTrade->sGroupKey[IDX_1MIN] = temp;

		GET_GROUP_KEY(sSymbol.GetBuffer(), TP_5MIN, temp);
		pTrade->sGroupKey[IDX_5MIN] = temp;

		GET_GROUP_KEY(sSymbol.GetBuffer(), TP_10MIN, temp);
		pTrade->sGroupKey[IDX_10MIN] = temp;

		GET_GROUP_KEY(sSymbol.GetBuffer(), TP_30MIN, temp);
		pTrade->sGroupKey[IDX_30MIN] = temp;

		GET_GROUP_KEY(sSymbol.GetBuffer(), TP_60MIN, temp);
		pTrade->sGroupKey[IDX_60MIN] = temp;

		char yn[32];
		strcpy(yn, db->Get("APPLY_CROSS_YN"));
		if(yn[0]=='Y')	bCrossCheck = TRUE;
		else			bCrossCheck = FALSE;

		UpdateTradeMap(sSymbol, pTrade, FALSE);
		
		sprintf(msg, "%s%s[LOAD ACNT INFO](%s)(%s)(CHART_TP:%d)(전략적용:%c)(CROSS_YN:%c)(전략해제이익/손실틱수:%d/%d)(현재손익틱:%d)",
			LOG1_ETC, LOG2_ETC,
			m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), pTrade->Tp(), sApplyYN[0], yn[0],
			pTrade->TotProfitCutTick, pTrade->TotLossLimitTick, m_nAccmPLTick);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_ETC, LOG2_ETC, msg);
		nLoop++;
		
	} // while (db->NextRow())
	
	if(nLoop==0)
	{
		m_bDoStrategy = FALSE;
		//return FALSE;
	}
	return TRUE;
}



BOOL CFBOrdThread::IsChartUse(CHART_IDX tpIdx)
{
	return (m_CommFactor.ChartYN[tpIdx] == 'Y');
}



// 보조지표 점검 시간이 지났는지 여부
BOOL CFBOrdThread::IsPassedAssistTime(DWORD dStartClock, DWORD* pdGap)
{
	DWORD result = (DWORD)(GetTickCount() - dStartClock) / CLOCKS_PER_SEC;
	*pdGap = result;
	if (result > DEF_CHECK_STRAT_SEC)
		return TRUE;

	return FALSE;
}


// 특정초가 지났는지 점검.
BOOL CFBOrdThread::IsPassedSecs(const char *pzChartNm, const UINT nCheckSecs)
{
	char tmp[32];
	time_t tmNow = time(NULL);
	struct tm *stTm = localtime(&tmNow);
	sprintf(tmp, "%.2s", pzChartNm+4);		stTm->tm_hour = atoi(tmp);
	sprintf(tmp, "%.2s", pzChartNm + 6);	stTm->tm_min = atoi(tmp);
	sprintf(tmp, "01" );			stTm->tm_sec = atoi(tmp);	// chart name 은 4자리

	time_t tmChart = mktime(stTm);
	double past = difftime(tmNow, tmChart);	// sec
	//g_log.log(LOGTP_SUCC, "시간체크.(차트시간:%s)(GAP:%f 초)", pzChartNm, past);
	return ((UINT)past >= nCheckSecs);
}

