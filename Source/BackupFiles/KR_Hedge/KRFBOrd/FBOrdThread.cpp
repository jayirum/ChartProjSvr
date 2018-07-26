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
	bInitCheckDone = FALSE;					//	�������ڸ��ڴ� ó������ �ʵ���
	ZeroMemory(&Pos, sizeof(Pos));

	//for (int i = 0; i < CHART_TP_CNT; i++)
	{
		mClrOpenBreak = FALSE;	//	���԰��ر� û���ֹ� �ߺ� ��������� ����
		mStratOpenDone = "";	//	���԰����� �������� ��Ʈ �̸�
		mStratCloseDone = "";	//	��������(������ǥ) �������� ��Ʈ �̸�
		mClrOpenHighLow = "";	// ������ ���Ժ� ��/�� +/- 2ƽ û�� stop �ֹ�
		//arrrSignalSide[i] = 0x00;		//	signal side (�ż�, �ŵ�)
		//arrSignalChartNm[i] = "";		//	��ȣ ��Ʈ�̸�
		//arrSignalStopOrdDone = "";
		//arrLastOpenPrcDick[i]="";		// ���� open price tick => CODE-6  
		mLastClrChartNm = "";
		mAssistClrChecked = FALSE;
	}
	ZeroMemory(&mAssist, sizeof(mAssist));//	������ǥ
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
	
	// �������� �ִ� ��� �� �ֹ��� ���� �ֹ����� Ȯ���Ѵ�. 
	// �����ֹ��� ��Ҵ���� �ƴϹǷ� ���⿡ �߰����� �ʴ´�.
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

	// �����ֹ���ȣ ����� ���ֹ���ȣ�� ����
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

	// �����ֹ���ȣ ����� ���ֹ���ȣ�� ����
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


// ���纸�� ������Ʈ���� �� �ֹ� ����ϱ� ���� ���� ��ȯ
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

		// �ش�Ǹ� ��� ��ȯ
		if (strncmp(pInfo->zChartNm, pzNowChartNm, LEN_CHART_NM) < 0)
		{
			if (pInfo->bProfitOrder)
				continue;

			// û���ֹ��� ���ܳ��´�.
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



// �������� �ֹ� ���
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

		// �ش�Ǹ� �ֹ���ȣ ����
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
	���� ������ �� ���� ������ �ֹ��� �ִ��� ����
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

			// �ż�, ������ ���� >= �񱳴��
			if (cSide == DEF_BUY && nComp >= 0)
			{
				*pnOrdNo = p->nOrdNo;
				strcpy(pzOrdPrc, p->zPrc);
				return TRUE;
			}

			// �ŵ�, ������ ���� <= �񱳴��
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
	//	PrintMsg(FALSE, FALSE, "LASTNM SHM OPEN ����(%s)", szSHMName);
	//	return FALSE;
	//}
	//PrintMsg(FALSE, FALSE, "SHM OPEN ����(%s)", szSHMName);

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
	
	// ������ҵ� �ε�
	if (!Initialize())
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>>>>>>>>>>>>>�������� ��ȸ ������ �� ������� �����Ѵ�[%s]", m_sAcnt.GetBuffer());
		return;
	}


	if (!InitChartSHM()) {
		g_log.log(LOGTP_FATAL, ">>>>>>>>>>>>>>>>>>>(%s)SHM OPEN ����.KRChart���������� Ȯ�� �ʿ�", m_sAcnt.GetBuffer());
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
							sprintf(zMsg, "%s%s�ű��ֹ���û ����.(%s) �׷��� ��������(%s) �ƴϹǷ�(%s) SKIP",
								LOG1_ADMIN, LOG2_ADMIN_NEWORD, AcntNo(), GetMySymbol(), sSymbol.GetBuffer());
							g_log.log(LOGTP_SUCC, zMsg);
							m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_NEWORD, zMsg);
							break;
						}
						ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
						

						if (!IsDoStrategy()) {
							sprintf(zMsg, "%s%s�ű��ֹ���û ����.�׷��� ������������ �ƴϹǷ� SKIP(%s)(%s)",
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

						// ���� �����ǰ� ���� �����̸� ���� �ʴ´�.
						if (IsPosOpened(pTrade) && (pTrade->Pos.cSide[0] == zSide[0]))
						{
							sprintf(zMsg, "%s%s�ű��ֹ���û ����(%s)(%s).���������ǹ���(%c)=�ֹ�����(%c)�̶� �ź�ó���Ѵ�.", 
								LOG1_ADMIN, LOG2_ADMIN_NEWORD, AcntNo(), sSymbol.GetBuffer(), 
								pTrade->Pos.cSide[0], zSide[0]);
						}
						else
						{
							sprintf(zMsg, "%s%s�ű��ֹ���û ����(%s)(%s).�ֹ�Ÿ��(%c)�ֹ�����(%c)(����:%10.5f).",
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

						sprintf(zMsg, "%s%sALL CLEAR ����(%s)(%s)",LOG1_ADMIN, LOG2_ADMIN_CLEARALL, AcntNo(), sSymbol.GetBuffer());
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
						sprintf(zMsg, "%s%sALL CANCEL ����(%s)(%s)",LOG1_ADMIN, LOG2_ADMIN_CNCLALL, AcntNo(), sSymbol.GetBuffer());
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
					case MNG_FB_ORDSYMBOL_ADD:	//7)FB �ڵ��ֹ����� ����
					case MGN_FB_STRAT_SET:		//10) FBƯ������ ������
						sprintf(zMsg, "%s%s��������/������(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo(), p->zWorkVal);
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);
						
						EnterCriticalSection(&m_csProtect);
						ReleaseStrategy(FALSE/*DBSAVE*/,'N', "������ �������� ���۾�");
						Initialize();
						LeaveCriticalSection(&m_csProtect);
						
						break;
					case MNG_FB_ORDSYMBOL_RELEASE:	// 8)FB �ڵ��ֹ����� ����
						sprintf(zMsg, "%s%s��������(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_SYMBOL, AcntNo(), p->zWorkVal);
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_SYMBOL, zMsg);
						
						EnterCriticalSection(&m_csProtect);
						ReleaseSymbol(CString(p->zWorkVal));
						LeaveCriticalSection(&m_csProtect);
						break;
					case MGN_FB_STRAT_RELEASE:	// 11) FBƯ������ ��������
						sprintf(zMsg, "%s%s ������������(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);

						EnterCriticalSection(&m_csProtect);
						ReleaseStrategy(TRUE/*DB SAVE*/, 'N', "������ ����");
						LeaveCriticalSection(&m_csProtect);
						break;
					case MGN_FB_CROSS_CHECK:
						bCrossCheck = TRUE;
						sprintf(zMsg, "%s%sũ�ν����� ����(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
						break;
					case MNG_FB_CROSS_UNCHECK:
						sprintf(zMsg, "%s%sũ�ν����� ����(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
						bCrossCheck = FALSE;
						break;
					case MNG_FB_PROFIT_TICK:
					case MNG_FB_LOSS_TICK:
						sprintf(zMsg, "%s%s�������� ƽ�� �� ����(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, AcntNo());
						g_log.log(LOGTP_SUCC, zMsg);
						m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, zMsg);
						LoadAcntTradeInfo();
						break;
					default:
						g_log.log(LOGTP_ERR, "[�߸��� �۾�TP:%d]", p->nWorkTp);
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
		// �ü��� ������ ���
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
			// ���� ��Ʈ�� ���ִ� ��ü���ֹ� ���
			Cancel_RemainOrd(pSise, sSymbol);

			// ������� ������ STOP �ֹ� CHECK
			Check_StopFire(pSise, sSymbol);

			// �ð���ġ, ���԰��ر� ����
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

//	CODE-8. ���� ��Ʈ�� �� ��ü���ֹ� ���
VOID CFBOrdThread::Cancel_RemainOrd(const char* pSise, _In_ CString& sSymbol)
{
	char zNowChartNm[32];
	char zNowPackTime[32];	// , zPackDT[32];
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;

	// ���簡 �ð�
	sprintf(zNowPackTime, "%.8s", pPack->Time);

	// ���� �����ǰ� �ִ� ��Ʈ �̸�
	GET_CHART_NM(zNowPackTime, pTrade->Tp(), zNowChartNm);

	// ������Ʈ�ֹ� ���
	char msg[1024];
	LIMIT_ORDER stOrd;
	while (pTrade->LimitOrd_GetOrdTobeCanceled(zNowChartNm, pTrade->Pos.nOpenQty, pTrade->Pos.cSide[0], &stOrd))
	{
		// �ش��ֹ� ���ó��
		char szOrigNo[32];
		sprintf(szOrigNo, "%-d", stOrd.nOrdNo);
		DoOrder(ORD_TYPE_CNCL, FALSE/*STOP*/, stOrd.zPrc, stOrd.cSide[0], stOrd.nRemainQty, sSymbol, szOrigNo);
		sprintf(msg, "%s%s[Cancel_RemainOrd]������Ʈ�ֹ� ���ó��(%s)(�ֹ���ȣ:%d)(�ֹ�SIDE:%c)(�ֹ�����:%10.5f)(�ֹ���:%s)(�����:%s)",
			LOG1_STRAT_ORD, LOG2_CNCL_SEND, sSymbol.GetBuffer(), stOrd.nOrdNo,stOrd.cSide[0], atof(stOrd.zPrc), stOrd.zChartNm, zNowChartNm);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
	}
}

/*
	�����س��� �����ֹ� �ߵ� üũ
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
	if (stopOrdTp == STOPORD_ASSIST_BUY)		sprintf(stopTp, "������ǥ�ż�STOP");
	if (stopOrdTp == STOPORD_ASSIST_SELL)		sprintf(stopTp, "������ǥ�ŵ�STOP");
	if (stopOrdTp == STOPORD_AFTERSIGNAL)	sprintf(stopTp, "SIGNAL STOP");
	if (stopOrdTp == STOPORD_OPENHILO)		sprintf(stopTp, "���Ժ����� STOP");
	if (stopOrdTp == STOPORD_OPEN)			sprintf(stopTp, "������ STOP");

	// AFTERSIGNAL �� ���ؼ� 1��30�� ����
	if (stopOrdTp == STOPORD_AFTERSIGNAL && !IsPosOpened(pTrade))
	{
		DWORD dGap;
		if (IsPassedAssistTime(pTrade->mAfterSignal.dwStartClock, &dGap))
		{
			sprintf(msg, "%s%s[Check_StopFire][AFTERSIGNAL �ð��ʰ�](%s)(%s)(%s)(GAP:%d ��)",
				LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), (char*)sSymbol.GetBuffer(), pTrade->GroupKeyEx(), dGap);

			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
			return;
		}
	}

	// ������ǥSTOP�� ��� 1��30�� ����(������ ��츸)
	if (stopOrdTp == STOPORD_ASSIST_SELL || stopOrdTp == STOPORD_ASSIST_BUY)
	{
		if (IsPosOpened(pTrade) == FALSE)
		{
			DWORD dGap;
			if (IsPassedAssistTime(pTrade->mAssist.dwAssistClock, &dGap))
			{
				sprintf(msg, "%s%s[Check_StopFire][������ǥ�ð��ʰ�](%s)(%s)(%s)(GAP:%d ��)",
					LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), (char*)sSymbol.GetBuffer(), pTrade->GroupKeyEx(), dGap);

				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
				ZeroMemory(&pTrade->mAssist, sizeof(pTrade->mAssist));
				return;
			}
		}
		// ������ǥ stop �� ��� ���Ժ������� û������ �ʴ´�.
		else
		{
			if (stopOrdTp == STOPORD_ASSIST_SELL && pTrade->Pos.cSide[0] == DEF_BUY)
			{
				if (pTrade->IsSameChartOfOpen(zNowChartNm))
				{
					sprintf(msg, "%s%s[Check_StopFire](%s)û��STOP�ߵ������̳� ������ǥ�ŵ�û���� ��� ���Ժ�(%.*s)�� �����(%.*s)�� �����Ƿ� ���� �ʴ´�",
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
					sprintf(msg, "%s%s[Check_StopFire](%s)û��STOP�ߵ������̳� ������ǥ�ż�û���� ��� ���Ժ�(%.*s)�� �����(%.*s)�� �����Ƿ� ���� �ʴ´�",
							LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), LEN_CHART_NM, pTrade->Pos.zChartNm, LEN_CHART_NM, zNowChartNm);
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
					return;
				}
			}
		}
	} // if (stopOrdTp == STOPORD_ASSIST_SELL || stopOrdTp == STOPORD_ASSIST_BUY)

	// � STOP�̵� �ߵ��ϸ� ���� ���� �ٸ� STOP �� ���ó�� �Ѵ�.
	// �� ���� ������ ��츸
	//if ((stopOrdTp == STOPORD_ASSIST) ||(stopOrdTp == STOPORD_AFTERSIGNAL))
	int nCnt = m_pStopOrd->RemoveOrdWhenOtherFired(stopOrdTp, zSide[0]);
	if (nCnt > 0)
	{
		sprintf(msg, "%s%s[Check_StopFire][%s](%s)Stop�ֹ��� (%s) �ߵ����� ������(%d) ���ó��",
			LOG1_STRAT_RQMNT, LOG2_STOP_CNCL, AcntNo(), sSymbol.GetBuffer(), stopTp, nCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_CNCL, msg);
	}


	// �̹� �������� �ִµ� �߰� �����̸� �����̹Ƿ� �ֹ����� �ʴ´�.
	if (IsPosOpened(pTrade))
	{
		if (pTrade->Pos.cSide[0] == zSide[0])
		{
			sprintf(msg, "%s%s[Check_StopFire][%s](%s)(%s �ߵ�)�̹� ���ԵǾ� �����Ƿ� �߰�����STOP�ߵ��� �ź��Ѵ�.(������SIDE:%c)(�����Ǽ���:%d)(�����ǰ���:%10.5f)",
				LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), stopTp, zSide[0], pTrade->Pos.nOpenQty, atof(pTrade->Pos.zPosPrc));
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
			return;
		}
	}


	// �� ������ ������ ������ �ź�
	if (IsOverlappedStopAndLimitOrd(zStopPrc, zSide[0], pTrade))
		return;

	// �̹� �ֹ������� �ι�üũ ����
	// stop ������ ���� stop�ߵ��� �� �ι� update �Ѵ�.
	if (stopOrdTp == STOPORD_OPENHILO)
		pTrade->mClrOpenHighLow = zNowChartNm;
	
	// û���ֹ��̸� ���尡 �����Ѵ�.
	char prcTp[32];
	sprintf(prcTp, "����_������");
	if (IsPosOpened(pTrade))
	{
		if (pTrade->Pos.cSide[0] != zSide[0]) {
			ZeroMemory(zStopPrc, sizeof(zStopPrc));
			sprintf(prcTp, "û��_���尡");
		}
	}
	sprintf(msg, "%s%s[Check_StopFire][%s](%s)(%s)(%s)(CURR:%10.5f)(STOP PRC:%10.5f)(SIDE:%c)(���Ժ�:%s)(�����:%s)",
		LOG1_STRAT_ORD, LOG2_STOP_FIRE, AcntNo(), sSymbol.GetBuffer(), prcTp, stopTp, atof(zCurrPrc), atof(zStopPrc), zSide[0], pTrade->Pos.zChartNm, zNowChartNm);
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_FIRE, msg);
	
	DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, zStopPrc, zSide[0], nQty, sSymbol, NULL);
	

	// ������ǥû���̸� ������ǥ ����
	if (stopOrdTp == STOPORD_ASSIST_SELL || stopOrdTp == STOPORD_ASSIST_BUY)
	{
		ZeroMemory(&pTrade->mAssist, sizeof(pTrade->mAssist));
		UpdateTradeMap(sSymbol, pTrade, FALSE);
		sprintf(msg, "%s%s[Check_StopFire]������ǥSTOP �ߵ����� ������ǥ����[%s](%s)(%s)(����stopPRC:%10.5f)(SIDE:%c)",
			LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), sSymbol.GetBuffer(), stopTp, atof(zStopPrc), zSide[0]);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER,msg);
	}
}

/*
	STOP �ߵ���ų �� �� ������ ������ LIMIT �ֹ��� �̹� ������ �ֹ����� �ʱ� ���ؼ�.
*/
BOOL CFBOrdThread::IsOverlappedStopAndLimitOrd(_In_ char* pzStopPrc, _In_ char cStopSide, _In_ ACNT_TRADE_INFO* pTrade)
{
	BOOL bOverLapped = FALSE;
	char zLimitPrc[32];
	int nLimitNo;

	if (!pTrade->LimitOrd_FindBetterOrd(cStopSide, pzStopPrc, &nLimitNo, zLimitPrc))
		return FALSE;

	char msg[1024];
	sprintf(msg, "%s%s[Check_StopFire]STOP�ߵ�������, �������� �������� �־ �ź�ó��.(%s)(%s)(SIDE:%c)(StopPrc:%10.5f)(������No.:%d)(������PRc:%10.5f)",
		LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), pTrade->Symbol(), cStopSide, atof(pzStopPrc), nLimitNo, atof(zLimitPrc));
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), pTrade->Symbol(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
	return TRUE;
}


/*
	�ü��� ������ ��� ó���ϴ� ���� �� Task �Լ� ȣ��
*/
VOID CFBOrdThread::DoProc_RecvSise(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);

	char zNowPackTime[32];	// , zPackDT[32];
	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;

	// ���簡 �ð�
	sprintf(zNowPackTime, "%.8s", pPack->Time);
	if (strncmp(m_zDate, pPack->Date, sizeof(pPack->Date))!=0)
	{
		sprintf(m_zDate, "%.*s", sizeof(pPack->Date), pPack->Date);
	}

	char zGroupKey[32];
	char zNowChartNm[32];

	// GROUP KEY : �����ڵ�+��ƮTP
	sprintf(zGroupKey, "%.*s", LEN_SHM_GROUP_KEY, pTrade->sGroupKey[idx].GetBuffer());

	// ���� �����ǰ� �ִ� ��Ʈ �̸�
	GET_CHART_NM(zNowPackTime, pTrade->Tp(), zNowChartNm);
	//g_log.log(LOGTP_SUCC, "����ð�(%s),��Ʈ�̸�(%s)", zNowPackTime, zChartNm);

	ST_SHM_CHART_UNIT	nowChart, prevChart;	//	���� �����ǰ� �ִ� ��Ʈ

	// ���� ��Ʈ�� ������Ʈ ���� ��������
	
	INT nRet = GetLastChartSHM(zGroupKey, zNowChartNm, prevChart, nowChart);
	if (nRet<0) {
		if (nRet == -1) {
			g_log.log(LOGTP_ERR, "[DoProc_RecvSise]����(��Ŷ�ð�:%s,����:%.5s) (GroupKey:%s, ChartNM:%s)",
				zNowPackTime, pPack->shortCode, zGroupKey, zNowChartNm);
		}
		return;
	}

	// ���ʽ����̸� �������� �ʴ´�.
	if (pTrade->IsInitDone() == FALSE)
	{
		pTrade->bInitCheckDone = TRUE;
		pTrade->mStratOpenDone = zNowChartNm;
		pTrade->mStratCloseDone = zNowChartNm;
		
		UpdateTradeMap(sSymbol, pTrade, FALSE);
		return;
	}

	// CODE-1. �ð���ġ���� ����
	CString sDone = pTrade->mStratOpenDone;
	if (sDone.Compare(zNowChartNm) != 0) {
		Strat_Open(sSymbol, idx, zGroupKey, zNowChartNm, pTrade, nowChart, prevChart/*, zNowPackTime*/);
		UpdateTradeMap(sSymbol, pTrade, FALSE);
	}

	// CODE-2. ������ġ���� (������ǥ) ����
	sDone = pTrade->mStratCloseDone;
	if (sDone.Compare(zNowChartNm) != 0) {
		Strat_AssistSet(sSymbol, idx, zGroupKey, zNowChartNm, pTrade, prevChart, nowChart);
		UpdateTradeMap(sSymbol, pTrade, FALSE);
	}

	// CODE-3. ������ǥ �ߵ� ����Open
	/* ������ǥstop �� �̸��ɾ���´�. - 2017.06.27
	Strat_AssistFire(idx, pSise, sSymbol, pTrade, nowChart, prevChart);
	UpdateTradeMap(sSymbol, pTrade,FALSE);
	*/

	/* 2017.06.07 �̳�����ǥ ��û���� ����
	// CODE-4. ���� �� ���԰��ݺر� û�� ����
	Strat_Clr_OpenPrcBreak(pSise, sSymbol);
	UpdateTradeMap(sSymbol, pTrade);
	*/

	// CODE-5. ���� �� ������Ʈ���� ������Ʈ ��/���� 2ƽ û������
	/* 2017.07.27 Delete this requirement based on Lee Na Young - 2017.07.27
	Strat_Clr_OpenHiLo(idx, pSise, sSymbol, zGroupKey, pTrade, prevChart, zNowChartNm);
	UpdateTradeMap(sSymbol, pTrade,FALSE);
	*/


	//	CODE-6. �����ŵ���ȣ �� �������� �ŵ��� �� 1��30�� �ȿ� ���Ŀ��� ���� �� ����
	Strat_AfterSignal_HighLow(sSymbol, pTrade, pSise, zGroupKey, zNowChartNm, nowChart);

	//	������ǥ û������ ����. ������ǥ>���簡 OR ������ǥ<���簡
	Strat_AssistClr(sSymbol, pTrade, pSise);
}


//	CODE-6. �����ŵ�/�ż���ȣ �� �������� �ŵ��� ��/���� 1��30�� �ȿ� ���Ŀ��� ���� �� ����/û��
VOID CFBOrdThread::Strat_AfterSignal_HighLow(_In_ CString& sSymbol, ACNT_TRADE_INFO* pTrade, const char* pSise, 
									const char* pzGroupKey, const char* pzNowChartNm, ST_SHM_CHART_UNIT& nowChart)
{
	// �������� ������ pass
	//if (pTrade->IsPosOpened())
	//	return;

	// �̹� STOP�ֹ��� ������ ���
	if (pTrade->mAfterSignal.SignalStopOrdDone.GetLength() > 0) {
		if (pTrade->mAfterSignal.SignalStopOrdDone.Compare(pzNowChartNm) >= 0)
			return;
	}

	// �������� ���� ���
	if (IsPosOpened(pTrade) == FALSE)
	{
		// ������ �ð��� ���� 90�ʰ� �������� pass (0606)
		if (IsPassedSecs(pzNowChartNm, DEF_CHECK_STRAT_SEC)) {
			return;
		}

		//	����û����� ������� ������ �������� �ʴ´�.
		if (pTrade->IsSameChartOfClr(nowChart.Nm))
		{
			return;
		}
	}

	char sEntry[32];

	ST_PACK2ORD	*p = (ST_PACK2ORD*)pSise;
	double dCurrPrc = S2D(p->close, sizeof(p->close));	// ���簡
	double dCheckPrc = 0;
	double dOrdPrc;

	// ���� 10���� scan
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
			g_log.log(LOGTP_ERR, "[Strat_AfterSignal_HighLow] ��Ʈ������ȸ ����(%s)(%s)", pzGroupKey, zPrevNm);
			return;
		}

		/*
		�ż���ȣ ��� => �ŵ��ֹ�
		1. �ż���ȣ���� ���� �����´�.
		2. �ر����� ( ���� <= ����Ʈ �ð� and ���� > ���簡 )
		3. �ֹ����� = ���� - 1ƽ
		*/
		if (chart.signalGb[0] == DEF_BUY )	// �ż���ȣ==>�ŵ��ֹ�
		{
			strcpy(sEntry, "����");
			// ������������ �ִ� ��� û����⸸
			if (IsPosOpened(pTrade))
			{
				if (pTrade->Pos.cSide[0] == DEF_SELL)
					continue;
				strcpy(sEntry, "û��");
			}

			if (chart.gb[0] == '+')
			{
				// ����, ����Ʈ �ð�
				nComp = CUtil::CompPrc(chart.low, sizeof(chart.low), nowChart.open, sizeof(nowChart.open), pTrade->nDotCnt, LEN_PRC);

				// ����, ���簡
				nCompCurr = CUtil::CompPrc(chart.low, sizeof(chart.low), p->close, sizeof(p->close), pTrade->nDotCnt, LEN_PRC);

				if (nComp <= 0 && nCompCurr > 0)
				{					
					dCheckPrc = atof(chart.low);

					// STOP �ֹ�
					dOrdPrc = dCheckPrc - (1 * pTrade->dTickSize);
					int nCnt = 0;
					pTrade->mAfterSignal.SignalStopOrdDone = pzNowChartNm;
					pTrade->mAfterSignal.dwStartClock = GetTickCount();
					if (AddNewStopOrder(sSymbol, dOrdPrc, DEF_SELL, m_CommFactor.nOrdQTy,
						pTrade->nDotCnt, zPrc, (char*)pzNowChartNm, STOPORD_AFTERSIGNAL, pTrade, &nCnt))
					{
						sprintf(msg, "%s%s[�����ż���ȣ �����ر�](����:%10.5f > ���簡:%10.5f)(�ż���ȣ��:%.8s)STOP�ŵ�%s����.(%s)(%s)(�ֹ���:%10.5f)(�Ѱ���:%d)",
							LOG1_STRAT_ORD, LOG2_STOP_SET, dCheckPrc, dCurrPrc, chart.Nm, sEntry, AcntNo(), sSymbol.GetBuffer(), dOrdPrc, nCnt);
						g_log.log(LOGTP_SUCC, msg);
						m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
					}
					break;	// break for statement
				}
			}
			else
			{
				//g_log.log(LOGTP_SUCC, "[�����ż���ȣ �����ر�]�ż���ȣ�̱�� �ϳ� ����� �ƴϹǷ� skip(������:%c)", chart.gb[0]);
			}
		} // if( chart.signalGb[0]==DEF_BUY)

		/*
		�ŵ���ȣ�� ��� => �ż��ֹ�
		1. �ŵ���ȣ���� �� �����´�.
		2. �������� ( �� >= ����Ʈ �ð� and �� < ���簡 )
		3. �ֹ��� = �� + 1ƽ
		*/
		if (chart.signalGb[0] == DEF_SELL  )// �ŵ���ȣ==>�ż��ֹ�
		{
			strcpy(sEntry, "����");
			// ������������ �ִ� ��� û����⸸
			if (IsPosOpened(pTrade))
			{
				if (pTrade->Pos.cSide[0] == DEF_BUY)
					continue;
				strcpy(sEntry, "û��");
			}

			if (chart.gb[0] == '-')
			{
				// ����, ����Ʈ �ð�
				nComp = CUtil::CompPrc(chart.high, sizeof(chart.high), nowChart.open, sizeof(nowChart.open), pTrade->nDotCnt, LEN_PRC);

				// ����, ���簡
				nCompCurr = CUtil::CompPrc(chart.high, sizeof(chart.high), p->close, sizeof(p->close), pTrade->nDotCnt, LEN_PRC);

				if (nComp >= 0 && nCompCurr < 0)
				{
					// STOP �ֹ�
					dCheckPrc = atof(chart.high);
					dOrdPrc = dCheckPrc + (1 * pTrade->dTickSize);;
					pTrade->mAfterSignal.SignalStopOrdDone = pzNowChartNm;
					pTrade->mAfterSignal.dwStartClock = GetTickCount();
					int nCnt = 0;
					if (AddNewStopOrder(sSymbol, dOrdPrc, DEF_BUY, m_CommFactor.nOrdQTy, pTrade->nDotCnt, zPrc,
						(char*)pzNowChartNm, STOPORD_AFTERSIGNAL, pTrade, &nCnt))
					{
						sprintf(msg, "%s%s[�����ŵ���ȣ ������](��:%10.5f < ���簡:%10.5f)(�ŵ���ȣ��:%.8s)STOP�ż�%s����.(%s)(%s)(�ֹ���:%10.5f)(�Ѱ���:%d)",
							LOG1_STRAT_ORD, LOG2_STOP_SET, dCheckPrc, dCurrPrc, chart.Nm, sEntry, AcntNo(), sSymbol.GetBuffer(), dOrdPrc,nCnt);
						g_log.log(LOGTP_SUCC, msg);
						m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
					}
					break;	// break for statement
				}
			}
			else
			{
				//g_log.log(LOGTP_SUCC, "[�����ŵ���ȣ ������]�ŵ���ȣ���̱�� �ϳ� ������ �ƴϹǷ� skip(������:%c)", chart.gb[0]);
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
û��
1) ���� �� ���� ĵ�� ���� �� ����ĵ�� ���� 2ƽ �Ʒ� �ż� �ֹ� (��stop)
2) ���� �� ���� ĵ�� ���� �� ����ĵ�� �� 2ƽ �� �ŵ� �ֹ� (��stop)

2017.07.05)
�ż����� �� ���� ������ ��� ���Ժ� ���� 2ƽ �Ʒ� �ŵ�û�� �ֹ�
�ŵ����� �� ���� ������ ��� ���Ժ� ���� 2ƽ �� �ż�û�� �ֹ�
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
	// �� �������� ������ ������.
	if ( IsPosOpened(pTrade)==FALSE)
		return;

	
	int nComp = strncmp(pTrade->Pos.zChartNm, pzNowChartNm, LEN_CHART_NM);
	// 5�к����� 20�� ����, ���� 20�� �̸� ���Ѵ�.
	if (nComp >= 0)
		return;

	// �̹� stop �� ������ ������.
	if (m_pStopOrd->IsStopOrdExists(STOPORD_OPENHILO))
		return;

	// �̹� �ֹ������� ������.(0606)
	// stop ������ ���� stop�ߵ��� �� �ι� update �Ѵ�.
	if (pTrade->mClrOpenHighLow.Compare(pzNowChartNm)>=0 )
		return;


	/*
	nComp = strncmp(pTrade->Pos.zChartNm, prevChart.Nm, LEN_CHART_NM);
	// 5�к����� 20�� ����, ���� 30��(���� 25��) �̸� ���Ѵ�.
	if ( nComp < 0)
		return;
	

	// 20�к� ����. 25�к� ����(������ 20�к�)
	int nComp = strncmp(pTrade->Pos.zChartNm, prevChart.Nm, LEN_CHART_NM);
	if (nComp != 0)
		return;
	*/

	
	// ������Ʈ�� �����´�.
	ST_SHM_CHART_UNIT chartPos;
	if (m_shm.GetData((char*)pzGroupKey, pTrade->Pos.zChartNm, (char*)&chartPos) == FALSE)
	{
		g_log.log(LOGTP_SUCC, "[Strat_Clr_OpenHiLo] ���Ժ� ���� ��ȸ ����(%s)(%s)(%s)", AcntNo(), pzGroupKey, pTrade->Pos.zChartNm);
		return;
	}

	double dOrdPrc, dChckPrc = 0;	
	char zPrc[128];
	char zMsg[128];
	char msg[1024];

	// �ŵ��������� ��� => �ż��ֹ� : ����ĵ�� ���� + 2ƽ 
	if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		dChckPrc = S2D((char*)chartPos.close, sizeof(chartPos.close));
		dOrdPrc = dChckPrc + (2 * pTrade->dTickSize);
		sprintf(zMsg, "�ż�û���ֹ�(����:%10.5f)(�ֹ���:%10.5f)", dChckPrc, dOrdPrc);
	}

	// �ż��������� ��� => �ŵ��ֹ� : ����ĵ�� ���� - 2ƽ 
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		dChckPrc = S2D((char*)chartPos.close, sizeof(chartPos.close));
		dOrdPrc = dChckPrc - (2 * pTrade->dTickSize);
		sprintf(zMsg, "�ŵ�û���ֹ�(����:%10.5f)(�ֹ���:%10.5f)", dChckPrc, dOrdPrc);
	}

	// �ߺ����� ����
	pTrade->mClrOpenHighLow = pzNowChartNm;

	// ���� stop order ���
	//m_pStopOrd->RemoveOrder(sSymbol);	=> AddNewStopOrder �Լ��� ����

	// �ű� stop �ֹ� add
	char cOrdSide = (pTrade->Pos.cSide[0] == DEF_SELL) ? DEF_BUY : DEF_SELL;
	int nCnt = 0;
	int nQty = 1;	//pTrade->Pos.nOpenQty
	if (AddNewStopOrder(sSymbol, dOrdPrc, cOrdSide, nQty, pTrade->nDotCnt,
		zPrc, (char*)pzNowChartNm, STOPORD_OPENHILO, pTrade, &nCnt))
	{
		sprintf(msg, "%s%s[Strat_Clr_OpenHiLo][�������Ժ����� +/- 2ƽ STOP�̸�����](%s)(%s)(%s)(���Ժ�:%.*s)(�����:%s)(%s)(�Ѱ���:%d)",
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


/* try catch �� ����*/
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
	API �� ���� �ֹ�/ü�� ��� ���� ���� �� ó��

	- ESApiDllDlgDlg::OnRecvData
*/

VOID CFBOrdThread::DoProc_RecvAPIReply( At0985O_st* p, _In_ CString& sSymbol)
{
	int nOrigNo = S2N((char*)p->Orig_ord_no, sizeof(p->Orig_ord_no));
	int nOrdNo = S2N((char*)p->Ord_no, sizeof(p->Ord_no));
	int nRemainQty = S2N((char*)p->RemainQty, sizeof(p->RemainQty));
	

	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]���������� ���µ� �ֹ�/ü�� ����(�ֹ���ȣ:%d)", sSymbol.GetBuffer(), nOrdNo);
		return;
	}
	char msg[1024];
	/*
		�����Ǳ���:������������ ����. SIDE:ü��SIDE 
			-> ADD : ���� ����.
			-> �Ϻ�û�� : ���� �ٸ���.
		�ܰ����:���������Ǽ���
		û�갡�ɼ��� : �ܰ����-��ü�����
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:1)(û�갡�ɼ���:0000000001)(�ܰ����:0000000001)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00047)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:1)(û�갡�ɼ���:0000000001)(�ܰ����:0000000002)(ü�ᰡ��:1255.00000)(ü�����:1                   )(ü���ȣ:00048)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:1)(û�갡�ɼ���:0000000002)(�ܰ����:0000000003)(ü�ᰡ��:1255.00000)(ü�����:1                   )(ü���ȣ:00049)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:2)(û�갡�ɼ���:0000000001)(�ܰ����:0000000002)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00050)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:2)(û�갡�ɼ���:0000000000)(�ܰ����:0000000001)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00051)
		[ü�����][API_����](GCQ7)(�����Ǳ���:2)(SIDE:2)(û�갡�ɼ���:0000000000)(�ܰ����:0000000000)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00052)
	*/
	if (p->ReplyType[0] == DEF_RSLT_MATCH) 
	{
		sprintf(msg,  "%s%s(%s)(�����Ǳ���:%c)(SIDE:%c)(û�갡�ɼ���:%.*s)(�ܰ����:%.*s)(ü�ᰡ��:%10.5f)(ü�����:%.*s)(ü���ȣ:%.*s)"
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
		sprintf(msg, "%s%s(%s)(��������:%c)����(%c)(�ֹ�����:%.*s)(�ֹ��ܷ�:%.*s)(�ֹ�����:%10.5f)(�ֹ���ȣ:%d)(���ֹ���ȣ:%d)"
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
	// �ź��ֹ� ó��, 
	if (p->ReplyType[0] == DEF_RSLT_RJCT_NEW ||
		p->ReplyType[0] == DEF_RSLT_RJCT_MDFY ||
		p->ReplyType[0] == DEF_RSLT_RJCT_CNCL
		)
	{
		g_log.log(LOGTP_ERR, "�ֹ��źμ���[%s][���ֹ���ȣ:%d]", sSymbol.GetBuffer(), nOrigNo);
		return;
	}


	char zChartNm[32];
	char zTradeTm[32];
	sprintf(zTradeTm, "%.8s", p->TradeTime);
	GET_CHART_NM(zTradeTm, pTrade->Tp(), zChartNm);//0606

	////////////////////////////////////////////////////////////
	// �ű� / ���� / ��� �ֹ�
	int nIdx = -1;
	if (p->ReplyType[0] == DEF_RSLT_NEW || p->ReplyType[0] == DEF_RSLT_MDFY || p->ReplyType[0] == DEF_RSLT_CNCL)
	{
		char zPrc[LEN_PRC + 1];
		double dPrc = S2D((char*)p->Price, sizeof(p->Price));
		FORMAT_PRC(dPrc, pTrade->nDotCnt, zPrc);

		if (p->ReplyType[0] == DEF_RSLT_NEW)
		{
			int nCnt = pTrade->LimitOrd_Add(zChartNm, nOrdNo, nOrigNo, nRemainQty, p->Side[0], zPrc, pTrade->TotProfitCutTick );
			sprintf(msg, "%s%s[DoProc_RecvAPIReply]�������ֹ��߰�(%s)(%s)(�ֹ���ȣ:%d)(SIDE:%c)(����:%10.5f)(�Ѱ���:%d)",
				LOG1_ORD_RECV, LOG2_ORD_API_R, AcntNo(), sSymbol.GetBuffer(), nOrdNo, p->Side[0], dPrc, nCnt);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_ORD_RECV, LOG2_ORD_API_R, msg);
		}
		else if (p->ReplyType[0] == DEF_RSLT_MDFY)
		{
			if( !pTrade->LimitOrd_Modify(zChartNm, nOrdNo, nOrigNo, nRemainQty, p->Side[0], zPrc))
			{
				g_log.log(LOGTP_ERR, "���ֹ������� ���� ���� ����(���ֹ���ȣ:%d)(�ֹ���ȣ:%d)", nOrigNo, nOrdNo);
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
		// ü������ ���� �ֹ��Ϸῡ ���� ���� ����
		int nOrdNo = S2N((char*)p->Ord_no, sizeof(p->Ord_no));
		int nCnt = pTrade->LimitOrd_Cancel(nOrdNo);

		sprintf(msg, "%s%s[DoProc_RecvAPIReply](%s)(%s)ü������Ͽ� �ֹ���������(�ֹ���ȣ:%d)(�����ֹ�����:%d)",
			LOG1_ORD_RECV, LOG2_ORD_API_R, AcntNo(), sSymbol.GetBuffer(), nOrdNo, nCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_ORD_RECV, LOG2_ORD_API_R, msg);
		UpdateTradeMap(sSymbol, pTrade, FALSE);

		int nOpenQty = S2N((char*)p->Open_q, sizeof(p->Open_q));

		char zChartNm[32];
		char zTradeTm[32];
		sprintf(zTradeTm, "%.8s",  p->TradeTime);
		GET_CHART_NM(zTradeTm, pTrade->Tp(), zChartNm);//0606

		// �����ֹ��̸� SL, PT �ֹ��� �̸� �Ǵ�.
		if (nOpenQty > 0)
		{
			// �̹� �������� �ִ� ���¿��� �߰� ü���� ó������ �ʴ´�.
			if (IsPosOpened(pTrade))
			{
				sprintf(msg, "%s%s�̹� �������� �ִ� ���¿��� û���� �ƴ� ü���� ����.ó������ �ʴ´�.(%s)(%s)(���ܰ����:%d)(���ܰ����:%c)(�ű������Ǽ���:%d)(ü�����:%c)(�ܰ����:%c)",
					LOG1_STRAT_RQMNT, LOG2_ERROR, AcntNo(), sSymbol.GetBuffer(), pTrade->Pos.nOpenQty, pTrade->Pos.cSide[0], S2N(p->Open_q,sizeof(p->Open_q)), p->Side[0], p->Open_tp[0]);
				g_log.log(LOGTP_ERR, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
			}
			else
			{
				// ���������� ����;
				pTrade->Pos.nCntrNo = S2N((char*)p->Trd_no, sizeof(p->Trd_no));
				pTrade->Pos.nOpenQty = nOpenQty;
				pTrade->Pos.cSide[0] = p->Side[0];
				sprintf(pTrade->Pos.zPosPrc, "%.*s", sizeof(p->ExecPrice), p->ExecPrice);
				sprintf(pTrade->Pos.zOpenTM, "%.*s", sizeof(p->TradeTime), p->TradeTime);
				sprintf(pTrade->Pos.zChartNm, zChartNm);
				//GET_CHART_NM(pTrade->Pos.zOpenTM, pTrade->idxMyChart, pTrade->Pos.zChartNm);//0606

				UpdateTradeMap(sSymbol, pTrade, FALSE);

				// StopLoss, Profit �ֹ� 
				// �����Ǽ����� 1 �̻��̸� �ֹ����� �ʴ´٤�.
				if (nOpenQty > 1)
				{
					sprintf(msg, "%s%s�����Ǽ����� 1 �̻��̴�.(%d)(%s)(%s)",
						LOG1_STRAT_RQMNT, LOG2_ERROR, nOpenQty, AcntNo(), sSymbol.GetBuffer());
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
				}
				if (nOpenQty == 1)
				{
					//������������ ������ ���
					CancelSamesideLimitOrd(pTrade, p->Side[0]);
					
					SetStop_OnEntryPos(sSymbol);
					SetProfit_OnEntryPos(sSymbol);
				}
			} // else of if (IsPosOpened(pTrade))

		} // if (nOpenQty > 0)

		// û���ֹ��� ��� ���� �����ս� üũ �� �������� üũ
		else
		{
			// ���԰��ر��� û���ֹ� �ߺ����� flag �ʱ�ȭ
			pTrade->mClrOpenBreak = FALSE;

			// ������ǥ û���ֹ� �ߺ����� FLAG �ʱ�ȭ
			pTrade->mAssistClrChecked = FALSE;

			// Open �� ���� �ߺ����� �ʱ�ȭ
			//for (int i = 0; i < CHART_TP_CNT; i++) {
			//	pTrade->mStratOpenDone[i] = "";
			//}

			// û��� ����
			pTrade->mLastClrChartNm = zChartNm;

			if (AcmlPL_CheckRelease(p, pTrade, sSymbol) == TRUE)
			{
				// ��������
				ReleaseStrategy(TRUE,'Y', "12ƽ/20ƽ ����");
			}
			else
			{
				// �����ֹ��� ���ó��
				sprintf(msg, "%s%sû������Ͽ� ���ó�� �õ��Ѵ�.(%s)(%s)", LOG1_STRAT_ORD, LOG2_CNCL_SEND,AcntNo(), sSymbol.GetBuffer());
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_CNCL_SEND, msg);
				CancelAllOrd_Symbol(sSymbol);
				ZeroMemory(&pTrade->Pos, sizeof(pTrade->Pos));
				UpdateTradeMap(sSymbol, pTrade, FALSE);
			}
		} // else if (nOpenQty > 0) û���ֹ��� ���

		// DB ����
		memset((void*)p->FullCode, 0x20, sizeof(p->FullCode));
		memcpy((void*)p->FullCode, sSymbol.GetBuffer(), sSymbol.GetLength());
		m_pStratLog->DBSaveCntr((char*)p);
	} // if (p->ReplyType[0] == DEF_RSLT_MATCH)
}


/*
	1. ���� ���� 
	2. �������� ����. 
		- ���������� 12ƽ �̻��� ��� �����ߴ� / ������.�� 20ƽ �̻��̸� ���� �ߴ�
*/
BOOL CFBOrdThread::AcmlPL_CheckRelease(const At0985O_st* p, const ACNT_TRADE_INFO* pTrade, _In_ CString& sSymbol)
{
	//double dPow = pow(10., (double)pTrade->nDotCnt);
	//// ���Ͱ��
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

	// ��������tick sum
	UpdateAcmlPL(nGapTick);

	BOOL bRelease = FALSE;
	// ���� ����
	if (m_nAccmPLTick >= pTrade->TotProfitCutTick)
		bRelease = TRUE;

	if (m_nAccmPLTick <= pTrade->TotLossLimitTick)
		bRelease = TRUE;

	return bRelease;
}


/*
���� �������� ����
*/
VOID CFBOrdThread::UpdateAcmlPL(int nPL)
{ 
	m_nAccmPLTick += nPL; 

	// DB ����
	CDBHandler db(GET_DBPTR->Get());

	CString sQ;
	sQ.Format("UPDATE FB_ACNT_INFO SET ACCM_PL_TICK = %d WHERE ACNT_NO = '%s'", m_nAccmPLTick, m_sAcnt.GetBuffer());
	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE == db->Exec())
	{
		g_log.log(LOGTP_ERR, "[UpdateAcmlPL]FB_ACNT_INFO ACCML_PL_TICK  UPDATE ����(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
	}
};


/*
	Ư������ ���� ����
*/
VOID CFBOrdThread::ReleaseSymbol(CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL)	return;

	CancelAllOrd_Symbol(sSymbol);
	delete pTrade;

	UpdateTradeMap(sSymbol, pTrade, TRUE);

	char msg[1024];
	sprintf(msg, "%s%s[ReleaseSymbol][%s][%s] ���� ���� ����", LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, AcntNo(), sSymbol.GetBuffer());
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, msg);
}


VOID CFBOrdThread::ClearAllOrdPos(CString& sSymbol)
{
	// ��ü���ֹ����
	MAPIT_ACNT_TRADE it;
	MAPLOCK();
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		CString symbol = (*it).first;
		if(sSymbol.Compare(symbol)==0)
			CancelAllOrd_Symbol(symbol);
	}

	// ������û��	
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		CString symbol = (*it).first;
		if (sSymbol.Compare(symbol) != 0)
			continue;
		ACNT_TRADE_INFO* p = (*it).second;

		if (p->IsPosOpened()) {
			char cSide = (p->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
			DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, NULL/*���尡*/, cSide, p->Pos.nOpenQty, symbol, NULL);
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

// ��������
VOID CFBOrdThread::ReleaseStrategy(const BOOL bDBSave, const char cNotifySaveYN, const char* pzReleaseNote)
{
	// �����ֹ����ó��
	MAPIT_ACNT_TRADE it; 
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		CString symbol = (*it).first;
		CancelAllOrd_Symbol(symbol);
	}

	m_bDoStrategy = FALSE;
	ZeroMemory(&m_CommFactor, sizeof(m_CommFactor));

	// tradeinfo �ʱ�ȭ	
	MAPLOCK();
	for (it = m_mapTrade.begin(); it != m_mapTrade.end(); it++)
	{
		delete (*it).second;
	}		
	m_mapTrade.clear();
	MAPUNLOCK();

	if (!bDBSave)
		return;

	// DB ����
	CDBHandler db(GET_DBPTR->Get());

	CString sQ;
	sQ.Format("EXEC FB_STRAT_UPDATE '%s', 'N', '%s', '%c' ", m_sAcnt.GetBuffer(), pzReleaseNote, cNotifySaveYN);
	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE == db->Exec())
	{
		g_log.log(LOGTP_ERR, "[ReleaseStrategy]FB_STRAT_UPDATE UPDATE ����(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
	}
	else
	{
		char msg[1024];
		sprintf(msg, "%s%s[ReleaseStrategy][%s]���� ����", LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, AcntNo());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), "ALL", (CHART_TP)0, LOG1_STRAT_RELEASE, LOG2_STRAT_RELEASE, msg);
	}
	
}


/*
	���Ե��� ���� 12tick ���� �ֹ�
*/
BOOL CFBOrdThread::SetProfit_OnEntryPos(CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]���������� ���µ� �����ֹ��õ�(%s)", sSymbol.GetBuffer());
		return FALSE;
	}

	int nDotCnt = pTrade->nDotCnt;
	double dOrdPrc = 0.;
	double dPosPrc = atof(pTrade->Pos.zPosPrc);
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		dOrdPrc = dPosPrc + (pTrade->TotProfitCutTick * pTrade->dTickSize);
		//g_log.log(LOGTP_SUCC, "LONG->�ŵ�PROFIT.AVG(%.*f)(������:%.*f)", nDotCnt, dPosPrc, nDotCnt, dOrdPrc);
	}
	else if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		dOrdPrc = dPosPrc - (pTrade->TotProfitCutTick * pTrade->dTickSize);
		//g_log.log(LOGTP_SUCC, "SHORT->�ż�PROFIT.AVG(%.*f)(������:%.*f)", nDotCnt, dPosPrc, nDotCnt, dOrdPrc);
	}

	char msg[1024];
	// û���ֹ�
	char zPrc[32];
	char cSide = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
	FORMAT_PRC(dOrdPrc, nDotCnt, zPrc);
	sprintf(msg, "%s%s[SetProfit_OnEntryPos][������PROFIT�ֹ����� (%s)(%s)(���԰�:%.*f)(PRC:%.*f)(SIDE:%c)",
		LOG1_STRAT_ORD, LOG2_LIMIT_SEND, AcntNo(), sSymbol.GetBuffer(), nDotCnt, dPosPrc, nDotCnt, dOrdPrc, cSide);
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_LIMIT_SEND, msg);
	
	DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, zPrc, cSide, pTrade->Pos.nOpenQty, sSymbol, NULL);
	
	//char zProfitTick[32];
	//sprintf(zProfitTick, "%d", m_CommFactor.nTotProfitCutTick);
	//m_pStratLog->Logging(
	//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_ENTRY,
	//	CHK_RECV_OPEN, "�������԰���/û���ֹ���", pTrade->Pos.zPosPrc, zPrc, "����ƽ/SIDE", zProfitTick, "",
	//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
	//	ORD_LIMIT_CLR, cSide, zPrc, "", ""
	//);
	return TRUE;
}

/*
	1) �ż� ������ ���� ���� ���� 9�� �� �� ������ - 2 TICK ���ݿ� ������ �ֹ� ����
	2) �ŵ� ������ ���� ���� ���� 9�� �� �� �ְ� + 2 TICK ���ݿ� ������ �ֹ� ����
*/
BOOL CFBOrdThread::SetStop_OnEntryPos( CString& sSymbol)
{
	char zNowPackTime[32];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(zNowPackTime, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	
	char zGroupKey[32];
	char zChartNm[32];

	// GROUP KEY : �����ڵ�+��ƮTP

	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]���������� ���µ� �����ֹ��õ�(%s)", sSymbol.GetBuffer());
		return FALSE;
	}
	strcpy(zGroupKey, pTrade->sGroupKey[pTrade->Idx()]);


	// ���� �����ǰ� �ִ� ��Ʈ �̸�
	GET_CHART_NM(zNowPackTime, pTrade->Tp(), zChartNm);

	ST_SHM_CHART_UNIT	nowChart, lastChart;	//	���� �����ǰ� �ִ� ��Ʈ

	// ���� ��Ʈ�� 9���� �ȵǸ� pass
	int nChartCnt = m_shm.GetCurrStructCnt(zGroupKey);
	if (nChartCnt < DEF_CHECK_CHART_CNT) {
		//g_log.log(LOGTP_SUCC, "��Ʈ�� %d���� �ȵȴ�.(%.5s)(%d)", 9, zGroupKey, nChartCnt);
		return FALSE;
	}

	// ���� ��Ʈ�� ������Ʈ ���� ��������
	INT nRet = GetLastChartSHM(zGroupKey, zChartNm, lastChart, nowChart);
	if (nRet<0) {
		if (nRet == -1) {
			g_log.log(LOGTP_ERR, "[SetStop_OnEntryPos]����(�ð�:%s,����:%s) (GroupKey:%s, ChartNM:%s)",
				zNowPackTime, sSymbol.GetBuffer(), zGroupKey, zChartNm);
		}
		return FALSE; 
	}


	/*
	���� 9���� ��Ʈ�� �����ͼ�
	- �ż����Ի��¿��� ���� 9������ ������ 2ƽ �Ʒ� ����
	- �ŵ����Ի��¿��� ���� 9������ �ְ� 2ƽ ��   ����
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
			g_log.log(LOGTP_ERR, "[SetStop_OnEntryPos][%s][%s]��Ʈ�� SHM �� ����", prevChartNm, zGroupKey);
			continue;
		}
		
		if (pTrade->Pos.cSide[0] == DEF_BUY)
		{
			double dLow = atof(lastChart.low);
			if (i == 0)
				dCheckPrc = dLow;
			else
				dCheckPrc = (dCheckPrc < dLow) ? dCheckPrc : dLow ;	// ������
		}
		else if (pTrade->Pos.cSide[0] == DEF_SELL)
		{
			double dHigh = atof(lastChart.high);
			if (i == 0)
				dCheckPrc = dHigh;
			else
				dCheckPrc = (dCheckPrc > dHigh) ? dCheckPrc : dHigh ;	// �ְ�
		}
		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, lastChart.prevNm);

	} // for (int i = 0; i < nLoop; i++)

	int nDotCnt = pTrade->nDotCnt;
	double dPosPrc = atof(pTrade->Pos.zPosPrc);
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		dOrdPrc = dCheckPrc - (m_CommFactor.nLosscutTick * pTrade->dTickSize);

		// 15ƽ�� ���� �� ����.
		int nGapTick = CUtil::GetTickGap(dOrdPrc, dPosPrc, pTrade->nDotCnt, pTrade->dTickSize);
		if (nGapTick < (MAX_ENTRY_STOP_TICK*-1))
		{
			dOrdPrc = dPosPrc - (MAX_ENTRY_STOP_TICK* pTrade->dTickSize);
		}
	}
	else if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		dOrdPrc = dCheckPrc + (m_CommFactor.nLosscutTick * pTrade->dTickSize);

		// 15ƽ�� ���� �� ����.
		int nGapTick = CUtil::GetTickGap(dPosPrc, dOrdPrc, pTrade->nDotCnt, pTrade->dTickSize);
		if (nGapTick < (MAX_ENTRY_STOP_TICK*-1))
		{
			dOrdPrc = dPosPrc + (MAX_ENTRY_STOP_TICK* pTrade->dTickSize);
		}
	}

	// û���ֹ�
	char zPrc[32];
	char cSide = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
	int nCnt = 0;
	int nQty = 1;	//pTrade->Pos.nOpenQty
	if (AddNewStopOrder(sSymbol, dOrdPrc, cSide, nQty, nDotCnt,
		zPrc, zChartNm, STOPORD_OPEN, pTrade, &nCnt))
	{
		char msg[1024];
		sprintf(msg, "%s%s[SetStop_OnEntryPos][������STOP����.(%s)(%s)(������Ʈ:%.8s)(���ذ�:%.*f)(�ν���ƽ:%d)(�ֹ�����:%10.5f)(�ֹ�SIDE:%c)(�Ѱ���:%d)",
			LOG1_STRAT_ORD, LOG2_STOP_SET, AcntNo(), sSymbol.GetBuffer(), lastChart.Nm, nDotCnt, dCheckPrc, m_CommFactor.nLosscutTick, atof(zPrc), cSide, nCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_SET, msg);
	}
	//char zLossTick[32]; 
	//sprintf(zLossTick, "%d", m_CommFactor.nLosscutTick);
	//m_pStratLog->Logging(
	//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_ENTRY,
	//	CHK_RECV_OPEN, "�������԰���/STOP������", pTrade->Pos.zPosPrc, zPrc, "����ƽ/SIDE", zLossTick, "",
	//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
	//	ORD_STOP_SET, cSide, zPrc, "", ""
	//);
	return TRUE;
}

BOOL CFBOrdThread::AddNewStopOrder(CString& sSymbol, const double dPrc, const char cOrdSide,
								const int nPosQty, const int nDotCnt, _Out_ char* pzPrc, 
								_In_ char* pzChartNm, STOPORD_TP stopOrdTp, ACNT_TRADE_INFO* pTrade, _Out_ int* pnTotCnt)
{
	// STOP ���� �ֹ� ���
	m_pStopOrd->RemoveOrder(stopOrdTp);

	FORMAT_PRC(dPrc, nDotCnt, pzPrc);
	int nResult = m_pStopOrd->AddNewOrd(&sSymbol, pzPrc, cOrdSide, nPosQty, pzChartNm, stopOrdTp, pnTotCnt);
	if (nResult == 0)
	{
		char msg[1024];
		sprintf(msg, "%s%s[AddNewStopOrder]�̹� ��������,�������� STOP�־ �����ź�(%s)(%s)%s", 
			LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), m_pStopOrd->GetMsg());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STOP_RJCT, msg);
		return FALSE;
	}
	return (nResult > 0);
}

/*
	���簡�� ������ǥ�� ���Ͽ� ������ �´� ��� û��
	1. �ر� : ������ǥ > ���簡 => �ż������� �ŵ��ֹ� û�길
	2. ���� : ������ǥ < ���簡 => �ŵ������� �ż��ֹ� û�길
*/
VOID	CFBOrdThread::Strat_AssistClr(CString& sSymbol, _In_ ACNT_TRADE_INFO* pTrade, const char* pSise)
{
	if (NULL == pSise)	return;
	if (FALSE == IsPosOpened(sSymbol))	return;
	if (FALSE == pTrade->IsExistAssistPrc())	return;
	if (pTrade->mAssistClrChecked == TRUE)		return;

	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;
	
	// �ż��������ΰ��
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		// �ر��ΰ�? ������ǥ > ���簡
		if (CUtil::CompPrc(pTrade->mAssist.zPrc, sizeof(pTrade->mAssist.zPrc), pPack->close, sizeof(pPack->close), pTrade->nDotCnt, LEN_PRC) > 0)
		{
			char msg[1024] = { 0, };
			sprintf(msg, "%s%s[Strat_AssistClr]������ǥ�ر� û����尡 ���� �� ��Һ��� �Ѵ�(%s)(%s)",
				LOG1_STRAT_ORD, LOG2_CNCL_SEND, AcntNo(), sSymbol.GetBuffer());
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);

			CancelAllOrd_Symbol(sSymbol);

			sprintf(msg, "%s%s[Strat_AssistClr][������ǥ�ر� û����尡�ֹ�](%s)(%s)(%s)(POS SIDE:%c)(������ǥ:%10.5f)>(���簡:%10.5f)",
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
		// �����ΰ�? ������ǥ < ���簡
		if (CUtil::CompPrc(pTrade->mAssist.zPrc, sizeof(pTrade->mAssist.zPrc), pPack->close, sizeof(pPack->close), pTrade->nDotCnt, LEN_PRC) < 0)
		{

			char msg[1024] = { 0, };
			sprintf(msg, "%s%s[Strat_AssistClr]������ǥ���� û����尡 ���� �� ��Һ��� �Ѵ�(%s)(%s)",
				LOG1_STRAT_ORD, LOG2_CNCL_SEND, AcntNo(), sSymbol.GetBuffer());
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);

			CancelAllOrd_Symbol(sSymbol);

			sprintf(msg, "%s%s[Strat_AssistClr][������ǥ���� û����尡�ֹ�](%s)(%s)(%s)(POS SIDE:%c)(������ǥ:%10.5f)<(���簡:%10.5f)",
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
1. �ü��θ� üũ�Ѵ�.
2. �������� �������� üũ�Ѵ�.
3. ���Ժ������� �̷������ �ʰ�, �� ���� ������ ���� ����

- ���԰��� �ر��� û��

*/
VOID CFBOrdThread::Strat_Clr_OpenPrcBreak(const char* pSise, _In_ CString& sSymbol)
{
	if (NULL == pSise)	return;
	if (FALSE == IsPosOpened(sSymbol))	return;

	////////////////////////////////////////////////////
	// GROUP KEY : �����ڵ�+��ƮTP
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		g_log.log(LOGTP_ERR, "[%s]���������� ���µ� ���԰��ر������õ�(%s)", AcntNo(), sSymbol.GetBuffer());
		return;
	}
	
	BOOL bClear = FALSE;
	ST_PACK2ORD	*pPack = (ST_PACK2ORD*)pSise;
	char zNowTime[32], zOpenTime[32];
	char zGroupKey[32];
	char zChartNm[32], zOpenChartNm[32];

	// �ü��ð�
	sprintf(zNowTime, "%.8s", pPack->Time);

	// ���Խð�
	sprintf(zOpenTime, "%.8s", m_mapTrade[sSymbol]->Pos.zOpenTM);

	// ���� �����ǰ� �ִ� ��Ʈ �̸�
	GET_CHART_NM(zNowTime, pTrade->Tp(), zChartNm);

	// ���Խð��� ��Ʈ �̸�
	GET_CHART_NM(zOpenTime, pTrade->Tp(), zOpenChartNm);

	// ���Խð��� ���籸������ ��Ʈ�� ���� �ð����̸� ������ �������� �ʴ´�.
	if (strcmp(zOpenChartNm, zChartNm) >= 0)
		return;

	// �̹� ���������� SKIP �Ѵ�.
	if (pTrade->mClrOpenBreak==TRUE) {
		//g_log.log(LOGTP_SUCC, "�̹� ������Ʈ ���� ����(%.4s)", chart[0].Nm);
		return;
	}
		
	strcpy(zGroupKey, pTrade->sGroupKey[pTrade->Idx()]);
	
	double dPrc = S2D(pPack->close, sizeof(pPack->close));
	double dPosPrc = 0.;

	char msg[1024];

	// �ż�������. ���԰� > ���簡
	if (pTrade->Pos.cSide[0] == DEF_BUY)
	{
		if (atof(pTrade->Pos.zPosPrc) > dPrc) {
			bClear = TRUE;
			dPosPrc = atof(pTrade->Pos.zPosPrc) - pTrade->dTickSize;	// 1tick ����
			sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak]���԰��ر�MATCH.�ż�������.(���:%10.5f)(���簡:%10.5f)(�ֹ���:%10.5f)", 
				LOG1_STRAT_RQMNT, LOG2_STRAT,atof(pTrade->Pos.zPosPrc), dPrc, dPosPrc);
		}
	}

	// �ŵ�������. ���԰� < ���簡
	if (pTrade->Pos.cSide[0] == DEF_SELL)
	{
		if (atof(pTrade->Pos.zPosPrc) < dPrc) {
			bClear = TRUE;
			dPosPrc = atof(pTrade->Pos.zPosPrc) + pTrade->dTickSize;	// 1tick ����
			sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak]���԰�����MATCH.�ŵ�������.(���:%10.5f)(���簡:%10.5f)(�ֹ���:%10.5f)",
				LOG1_STRAT_RQMNT, LOG2_STRAT, atof(pTrade->Pos.zPosPrc), dPrc, dPosPrc);
		}
	}
	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);

	// û���ֹ� ����
	if (bClear) {

		// ���Ժ�û��Ұ�
		// ���Ժ������� û������ �ʴ´�.
		if (pTrade->IsSameChartOfOpen(zChartNm))
		{
			sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak](%s)���԰��ر� û���ֹ��� �ؾ��ϳ�, ���Ժ�(%.*s)�� �����(%.*s)�� �����Ƿ� ���� �ʴ´�.",
				LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), LEN_CHART_NM, pTrade->Pos.zChartNm, LEN_CHART_NM, zChartNm);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);

			return;
		}

		///////////////////////////////////////////////////////////
		// �����ֹ���Һ��� �Ѵ�.
		sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak]���԰��ر� û�������� �ֹ� ���� �� ��Һ��� �Ѵ�(%s)(%s)",
			LOG1_STRAT_ORD, LOG2_CNCL_SEND, AcntNo(), sSymbol.GetBuffer());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
		CancelAllOrd_Symbol(sSymbol);

		char zPrc[32];
		char cSide = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
		//sprintf(zPrc, "%f", dPosPrc);
		FORMAT_PRC(dPosPrc, pTrade->nDotCnt, zPrc);
		
		sprintf(msg, "%s%s[Strat_Clr_OpenPrcBreak][���԰��ر� û���������ֹ�](%s)(%s)(%s)[������Ʈ:%.*s][������Ʈ:%.*s](SIDE:%c)(���԰�:%s)(�ֹ���:%s)",
			LOG1_STRAT_ORD, LOG2_LIMIT_SEND,AcntNo(), zGroupKey, sSymbol.GetBuffer(),
			LEN_CHART_NM, zOpenChartNm, LEN_CHART_NM, zChartNm,
			pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, zPrc);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_LIMIT_SEND, msg);

		DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, zPrc, cSide, pTrade->Pos.nOpenQty, sSymbol, NULL);
		pTrade->mClrOpenBreak = TRUE;

		
		//m_pStratLog->Logging(
		//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_SISE,
		//	CHK_ENT,RY_DOWN, "������Ʈ/���԰���", zOpenChartNm, pTrade->Pos.zPosPrc, "������Ʈ/���簡��", zChartNm, pPack->close,
		//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
		//	ORD_STOP_SET, cSide, zPrc, "", "���԰��ݺر��� ���� û���ֹ� ����"
		//);
	} // if (bClear)
}


/*
	Strategy-1
	
1) ���� ������ ���� �ð��� ���� 9������ �ð��� �񱳸� �Ѵ�.
2) ��ġ�ϴ� �ð��� ����� �ð��̸� �ż�, ������ �ð��̸� �ŵ�
3) ��, ĵ�鸶�� 1���� �����Ѵ�.

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

	// �̹� ������ ���������� ������.  CHECK
	// sDone(1201) ���� zNowChartNm(1201 or 1200) �� ���ų� ����.
	CString sDone = pTrade->mStratOpenDone;
	if ( sDone.Compare(pzNowChartNm)>=0 ) {
		return;
	}

	// ���� ��Ʈ�� 9���� �ȵǸ� pass
	int nChartCnt = m_shm.GetCurrStructCnt((char*)pzGroupKey);
	if (nChartCnt < DEF_CHECK_CHART_CNT) {
		//g_log.log(LOGTP_SUCC, "��Ʈ�� %d���� �ȵȴ�.(%.5s)(%d)", DEF_CHECK_CHART_CNT, pzGroupKey, nChartCnt);
		return;
	}

	/*
		���� 9���� ��Ʈ�� ��,���� ����, �ð� �� �����ͼ�
		1. ������Ʈ�� �ð��� ����ð����� �ְ�, �����ð����� ������ pass
		2. ���� ����� �ð��� ��ġ�ϴ°� ������ �ż��ֹ�
		3. ���� ������ �ð��� ��ġ�ϴ°� ������ �ŵ��ֹ�
	*/
	
	ST_SHM_CHART_UNIT	chart[DEF_CHECK_CHART_CNT];	//	�Ϸ�� ��Ʈ 9��

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
			g_log.log(LOGTP_ERR, "[Strat_Open][%s][%s]��Ʈ�� SHM �� ����", prevChartNm, pzGroupKey);
			continue;
		}
		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, chart[i].prevNm);

		//g_log.log(LOGTP_SUCC, "[strat_open:%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, chart[i].Nm, chart[i].gb[0], atof(chart[i].open));

		// ������ ����� �ƴ� ���
		if (chart[i].gb[0] != DEF_CANDLE_PLUS && chart[i].gb[0] != DEF_CANDLE_MINUS) {
			continue;
		}

		// �ð��� ��ġ�ϴ°�?
		//if (atof(nowChart.open) != atof(chart[i].open))
		if(!IsSamePrc(nowChart.open, sizeof(nowChart.open), chart[i].open, sizeof(chart[i].open), pTrade->nDotCnt, temp))
			continue;

		// ��� �Ǵ� �����ΰ�?
		if (chart[i].gb[0] == DEF_CANDLE_PLUS)	nFoundPlus = i;
		if (chart[i].gb[0] == DEF_CANDLE_MINUS)	nFoundMinus = i;

	}
	//g_log.log(LOGTP_SUCC, "--------------------------------------------------------");

	// �ߺ����� ����	
	pTrade->mStratOpenDone = pzNowChartNm;

	if ((nFoundPlus == -1) && (nFoundMinus == -1))
	{
		return;
	}

	// �� �������� ������ û���ֹ��� ����
	BOOL bMustClrOrd = FALSE; 
	

	// �Ѵ� ��ġ. ��ġ�ϴ� ��Ʈ�� �־ ����, ��� ��� ��ġ�ϸ� �ȵȴ�.
	// ��, û������� ������.
	
	if (nFoundPlus > -1 && nFoundMinus > -1)
	{
		if (pTrade->IsPosOpened())
			bMustClrOrd = TRUE;
		else
			return;
	}

	char msg[1024];
	/////////////////////////////////////////////////////////
	// �����ֹ� ����
	ORD_TP ordTp;
	char side;
	if (nFoundPlus > -1 || nFoundMinus>-1) 
	{
		int nPos = (nFoundPlus > -1) ? nFoundPlus : nFoundMinus;
		
		// ����,��� ��� ��ġ�ϰ� ���� �������� ������ û���ֹ� �Ѵ�.
		if(bMustClrOrd)
		{
			ordTp = ORD_LIMIT_CLR;
			side = (pTrade->Pos.cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
			sprintf(msg, "%s%s[Strat_Open][OPEN�� ����]����,������ ��ġ�ϳ� û���ֹ�(%s)(%s)(����� �ð�:[%.*s]Open:%7.5f) (��ġ:[%.*s][%c]Open:%7.5f)",
				LOG1_STRAT_RQMNT, LOG2_STRAT,AcntNo(), pzGroupKey,
				LEN_CHART_NM, nowChart.Nm, atof(nowChart.open),
				LEN_CHART_NM, chart[nPos].Nm, chart[nPos].gb[0], atof(chart[nPos].open)
			);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		}
		else 
		{
			// �ż��ֹ�
			if (nFoundPlus > -1)
			{
				// �ż�Signal ǥ�� (0606)
				if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
					SetSignal(DEF_BUY, pzGroupKey, pzNowChartNm);
					sprintf(msg, "%s%s[Strat_Open][SetSignal]�ż���ȣ�߻�(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
				}

				// �̹� �ż������� �ߴ��� ����
				if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_BUY))
					return;
				else if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_SELL))
					ordTp = ORD_LIMIT_CLR;
				else
					ordTp = ORD_LIMIT_ENTRY;
			}

			// �ŵ��ֹ�
			if (nFoundMinus > -1)
			{
				// �ŵ�Signal ǥ�� (0606)
				if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
					SetSignal(DEF_SELL, pzGroupKey, pzNowChartNm);
					sprintf(msg, "%s%s[Strat_Open][SetSignal]�ŵ���ȣ�߻�(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
				}


				// �̹� �ŵ������� �ߴ��� ����
				if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_SELL))
					return;
				else if ((pTrade->Pos.nOpenQty > 0) && (pTrade->Pos.cSide[0] == DEF_BUY))
					ordTp = ORD_LIMIT_CLR;
				else
					ordTp = ORD_LIMIT_ENTRY;
			}

			side = (chart[nPos].gb[0] == DEF_CANDLE_PLUS) ? DEF_BUY : DEF_SELL;

			char zOrdTp[32] = { 0, };
			if (ordTp == ORD_LIMIT_CLR)	sprintf(zOrdTp, "û���ȣ");
			if (ordTp == ORD_LIMIT_ENTRY)	sprintf(zOrdTp, "���Խ�ȣ");
			sprintf(msg, "%s%s[Strat_Open][OPEN�� ����](%s)(%s)(%s)(����� �ð�:[%.*s]Open:%7.5f) (��ġ:[%.*s][%c]Open:%7.5f)",
				LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzGroupKey, zOrdTp,
				LEN_CHART_NM, nowChart.Nm, atof(nowChart.open),
				LEN_CHART_NM, chart[nPos].Nm, chart[nPos].gb[0], atof(chart[nPos].open));

			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		} // else of if(bMustClrOrd)

		// �����ֹ��̸� CROSS �� ����. ����ũ�ν�:�ż����Ը�, ���ũ�ν�:�ŵ����Ը�. (0606)
		if (ordTp == ORD_LIMIT_ENTRY)
		{
			//	����û����� ���� ���̸� �������� �ʴ´�.
			if (pTrade->IsSameChartOfClr(nowChart.Nm))
			{
				sprintf(msg, "%s%s[Strat_Open](%s)[�ð���ġ]�� �ֹ��� �ؾ��ϳ� ����û���(%.*s)�� �����(%.*s)�� ����.",
					LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), LEN_CHART_NM, pTrade->mLastClrChartNm.GetBuffer(), LEN_CHART_NM, nowChart.Nm);

				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
				return;
			}

			// �ż������� ���, ������� �������� ����� ���� �ż������Ѵ�.
			if (side == DEF_BUY)
			{
				if (chart[0].gb[0] != DEF_CANDLE_PLUS) // ����, ���ں� ��� skip.	
				{
					sprintf(msg, "%s%s[Strat_Open](%s)[�ð���ġ]�� �ż������ֹ��� �ؾ��ϳ� ������(%.*s)�� ����� �ƴϹǷ� �������� �ʴ´�.",
						LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), LEN_CHART_NM, chart[0].Nm);

					g_log.log(LOGTP_SUCC, msg);
					m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
					return;
				}
			}


			// �ŵ������� ���, ������� �������� ������ ���� �ŵ������Ѵ�.
			if (side == DEF_SELL)
			{
				if (chart[0].gb[0] != DEF_CANDLE_MINUS)	// ���, ���ں� ��� SKIP
				{
					sprintf(msg, "%s%s[Strat_Open](%s)[�ð���ġ]�� �ŵ������ֹ��� �ؾ��ϳ� ������(%.*s)�� ������ �ƴϹǷ� �������� �ʴ´�.",
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
				sprintf(msg, "%s%s[Strat_Open]OPEN PRC ��ġ. �׷���, CROSS ��ǿ� ���� �ֹ�SKIP(%s)(ũ�ν�:%d)(SIDE:%c)", 
					LOG1_STRAT_RQMNT, LOG2_REFER, AcntNo(), (int)crossTp, side);
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
				return;
			}
		} // if (ordTp == ORD_LIMIT_ENTRY)
		else if (ordTp == ORD_LIMIT_CLR)	// û���ֹ��� ��� ���Ժ����� û���� �ȵǴ�.
		{
			if (pTrade->IsSameChartOfOpen(nowChart.Nm))
			{
				sprintf(msg, "%s%s[Strat_Open]�ð���ġ�� ���� û�������̳� ���Ժ�(%.*s), �����(%.*s) �� ��ġ�Ͽ� skip �Ѵ�.",
					LOG1_STRAT_RQMNT, LOG2_REFER, LEN_CHART_NM, pTrade->Pos.zChartNm, LEN_CHART_NM, nowChart.Nm);
				g_log.log(LOGTP_SUCC, msg);
				m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_REFER, msg);
				return;
			}
		}

		//if (pTrade->IsExistSameSideLimitOrd(side))
		//{
		//	g_log.log(LOGTP_SUCC, "�ð���ġ�� �ֹ��ؾ� �ϳ� �̹� ������ ��ü���� �����Ƿ� SKIP(����:%s)(�ɺ�:%s)(����:%c)",
		//		AcntNo(), sSymbol.GetBuffer(), side);
		//	return;
		//}
		DoOrder(ORD_TYPE_NEW, FALSE/*STOP*/, chart[nPos].open, side, m_CommFactor.nOrdQTy, sSymbol, NULL);

		char zSameChart[32]; sprintf(zSameChart, "%.*s", sizeof(chart[nPos].Nm), chart[nPos].Nm);
		//m_pStratLog->Logging(
		//	m_sAcnt.GetBuffer(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_SISE,
		//	CHK_OPENPRC, "����ĵ��(mmddhhmm)/�ð�", (char*)pzNowChartNm, (char*)nowChart.open, "��ġĵ��(mmddhhmm)/����", zSameChart, chart[nPos].open,
		//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
		//	ordTp, side, (char*)nowChart.open, "", "�ð���ġ�� ���� �ֹ�"
		//);
	}
		
}


/*
	��ȣ�� �߻��� ���� ǥ��
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
	������ǥ����(����)
	- ������ ���� ĵ�� ������ �� ���� 9���� ĵ�� ���� ��
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

	// �̹� ������ ���������� ������.  CHECK
	// sDone(1201) ���� zNowChartNm(1201 or 1200) �� ���ų� ����.
	CString sDone = pTrade->mStratCloseDone;
	if (sDone.Compare(pzNowChartNm) >= 0) {
		return;
	}

	//// ���� ��Ʈ�� 10���� �ȵǸ� pass (���� �������� 9�� �̹Ƿ�)
	int nChartCnt = m_shm.GetCurrStructCnt((char*)pzGroupKey);
	if (nChartCnt < DEF_CHECK_CHART_CNT + 1) {
		//g_log.log(LOGTP_SUCC, "��Ʈ�� %d���� �ȵȴ�.(%.5s)(%d)", DEF_CHECK_CHART_CNT + 1, pzGroupKey, nChartCnt);
		return;
	}

	/*
		1. ���� ĵ���� ������ �����´�.
		2. �� ���� 9���� ĵ���� ������ ���Ѵ�.
		3. ����� ���, ������ ����
	*/
	ST_SHM_CHART_UNIT	checkChart;
	INT nFound = -1;
	BOOL bSame;

	if (lastChart.gb[0] != '+' && lastChart.gb[0] != '-')
	{
		//g_log.log(LOGTP_SUCC, "[STRAT_CLOSE] ���, ���� ��� �ƴϹǷ� skip(%s)(%s)(%.*s)(%10.5f)", 
		//			AcntNo(), pzGroupKey, LEN_CHART_NM, lastChart.Nm, atof(lastChart.close) );
		pTrade->mStratCloseDone = pzNowChartNm;
		return;
	}

	char prevChartNm[LEN_CHART_NM + 1];
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, lastChart.prevNm);
	//g_log.log(LOGTP_SUCC, "STRAT_CLOSE[%s][�񱳱�����Ʈ:%.*s](%c)(%10.5f)",
	//	 pzGroupKey, LEN_CHART_NM, lastChart.Nm, lastChart.gb[0], atof(lastChart.close));

	// ���� ĵ�� ���� 9��
	int nLoop = DEF_CHECK_CHART_CNT;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_shm.GetData((char*)pzGroupKey, (char*)prevChartNm, (char*)&checkChart))
		{
			g_log.log(LOGTP_ERR, "[STRAT_CLOSE][%s][%s]��Ʈ�� SHM �� ����", prevChartNm, pzGroupKey);
			continue;
		}
		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, checkChart.prevNm);

		//g_log.log(LOGTP_SUCC, "start_close[%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, checkChart.Nm, checkChart.gb[0], atof(checkChart.close));

		// ��ȣ�� �ٸ��� PASS
		if (lastChart.gb[0] != checkChart.gb[0])
			continue;

		// ���� ��ġ�ϴ°�?
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

	// �ߺ����� ����	
	pTrade->mStratCloseDone = pzNowChartNm;

	if (nFound == -1)
		return;
	
	// ������ǥ���� ���� �ð� 
	double dNowOpen = S2D((char*)nowChart.open, sizeof(nowChart.open));
	pTrade->mAssist.dwAssistClock = GetTickCount();
	double dAssistPrc = atof(lastChart.close);
	char msg[1024];
	sprintf(msg, "%s%s[Strat_AssistSet][������ǥ����](%s)(%s)(�ֱ�����:[%.*s][%c]Close:%7.5f)(��ġ:[%.*s][%c]Close:%7.5f)(�ð�:%10.5f)",
		LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzGroupKey,
		LEN_CHART_NM, lastChart.Nm, lastChart.gb[0], dAssistPrc,
		LEN_CHART_NM, checkChart.Nm, checkChart.gb[0], dAssistPrc, dNowOpen);

	g_log.log(LOGTP_SUCC, msg);
	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);

	// ���� ������ȣ�� ���� STOP ���
	m_pStopOrd->RemoveOrder(STOPORD_ASSIST_BUY);
	m_pStopOrd->RemoveOrder(STOPORD_ASSIST_SELL);

	//	�ŵ��ż��� �̸� �ɾ���´�.
	//	������ ������ǥ +/- 1ƽ
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
		sprintf(msg, "%s%s[Strat_AssistSet][������ǥ����.���ż�STOP����].�׷��� CROSS ��ǿ� ���� ����.(%s)(%s)(���ũ�ν�)(�ż��ֹ�)",
			LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);		
	}
	else
	{
		// ������ȣ > �ð� ==> �ż�
		if (CUtil::CompPrc(dAssistPrc, dNowOpen, pTrade->nDotCnt, LEN_PRC) < 0)
		{
			sprintf(msg, "%s%s[Strat_AssistSet][������ǥ����.���ż�STOP����].�׷��� [������ȣ<�ð�] �� �ż��ֹ�SKIP.(%s)(%s)(��ǥ:%10.5f)(�ð�:%10.5f)",
				LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), dAssistPrc, dNowOpen);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);
		}
		else
		{
			if (AddNewStopOrder(sSymbol, dOrdPrc, cSide, m_CommFactor.nOrdQTy, pTrade->nDotCnt, zPrc, zChartNm, STOPORD_ASSIST_BUY, pTrade, &nCnt))
			{
				sprintf(msg, "%s%s[Strat_AssistSet][������ǥ����.���ż�STOP����](%s)(%s)(%s)(������ǥ:%10.5f)(�ֹ�����:%10.5f)(�ֹ�side:%c)(�ֹ�����:%10.5f)(�Ѱ���:%d)",
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
		sprintf(msg, "%s%s[Strat_AssistSet][������ǥ����.���ŵ�STOP����].�׷��� CROSS ��ǿ� ���� ����.(%s)(%s)(����ũ�ν�)(�ŵ��ֹ�)",
			LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer());
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);
	}
	else
	{
		// ������ȣ < �ð� ==> �ŵ�
		if (CUtil::CompPrc(dAssistPrc, dNowOpen, pTrade->nDotCnt, LEN_PRC) > 0)
		{
			sprintf(msg, "%s%s[Strat_AssistSet][������ǥ����.���ŵ�STOP����].�׷��� [������ȣ>�ð�] �� �ŵ��ֹ�SKIP.(%s)(%s)(��ǥ:%10.5f)(�ð�:%10.5f)",
				LOG1_STRAT_ORD, LOG2_STOP_RJCT, AcntNo(), sSymbol.GetBuffer(), dAssistPrc, dNowOpen);
			g_log.log(LOGTP_SUCC, msg);
			m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_RJCT, msg);
		}
		else
		{
			if (AddNewStopOrder(sSymbol, dOrdPrc, cSide, m_CommFactor.nOrdQTy, pTrade->nDotCnt, zPrc, zChartNm, STOPORD_ASSIST_SELL, pTrade, &nCnt))
			{
				sprintf(msg, "%s%s[Strat_AssistSet][������ǥ����.���ŵ�STOP����](%s)(%s)(%s)(������ǥ:%10.5f)(�ֹ�����:%10.5f)(�ֹ�side:%c)(�ֹ�����:%10.5f)(�Ѱ���:%d)",
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
		sprintf(msg, "[GetCross]<NONE_CROSS-�������̻�> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
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


// ���� ���� ���� ��Ʈ�� ���� ��Ʈ ��������
INT CFBOrdThread::GetLastChartSHM(char* pzGroupKey, char* pzChartNm, ST_SHM_CHART_UNIT& shmLast, ST_SHM_CHART_UNIT& shmNow)
{
	char szChart[LEN_CHART_NM + 1];

	// ���� ��Ʈ ���� ��������
	BOOL bExist = m_shm.GetData(pzGroupKey, pzChartNm, (char*)&shmNow);

	// 1. ���� ��Ʈ�� ���� ��� ==> ������.
	if (FALSE == bExist)
	{
		// ���ݽð� ��Ʈ�� ���µ�, ���ݽð� ü�ᰡ�� ���� ��Ȳ�̶�� ��Ʈ �̻��̴�.
		g_log.log(LOGTP_ERR, "(%s)[%s][%s] ���簡�����ߴµ� ������Ʈ�� ����.", AcntNo(), pzGroupKey, pzChartNm);
		return -1;
	} // if(!bExist)


	// 2. ���� ��Ʈ�� �ִ� ��� ���� ��Ʈ ��������	  
	sprintf(szChart, "%.*s", LEN_CHART_NM, shmNow.prevNm);
	if (strcmp(szChart, "NONE") == 0)
		return -3;

	if (FALSE == m_shm.GetData(pzGroupKey, szChart, (char*)&shmLast))
	{
		g_log.log(LOGTP_ERR, "(%s)(%s)(%.*s) ���� ��Ʈ�� SHM �� ����", AcntNo(), pzGroupKey, LEN_CHART_NM, szChart);
		return -2;
	}
	//g_log.log(LOGTP_SUCC, "������Ʈ(%.4s)", chart[0].Nm);


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


// ���� �ɷ��ִ� �ֹ��� ���ó�� (�ش�����)
VOID	CFBOrdThread::CancelAllOrd_Symbol(_In_ CString& sSymbol)
{
	ACNT_TRADE_INFO* pTrade = GetTradeInfo(sSymbol);
	if (pTrade == NULL) {
		//g_log.log(LOGTP_ERR, "(%s)[%s]���������� ���µ� ��ü��ҽõ�", AcntNo(), sSymbol.GetBuffer());
		return;
	}

	// ������ STOP ���
	//int nRemoveCnt = m_pStopOrd->RemoveOrder(sSymbol);
	int nRemoveCnt = m_pStopOrd->RemoveAllOrder();
	if (nRemoveCnt > 0) {
		char cnt[32]; sprintf(cnt, "%d", nRemoveCnt);
		char msg[1024]; 
		sprintf(msg, "%s%s[CancelAllOrd_Symbol]STOP�ֹ����ó��(%s)(%s)(%d ���ֹ�)", LOG1_STRAT_ORD, LOG2_STOP_CNCL, AcntNo(), sSymbol.GetBuffer(), nRemoveCnt);
		g_log.log(LOGTP_SUCC, msg);
		m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_ORD, LOG2_STOP_CNCL, msg);
	}


	LIMIT_ORDER stOrd;
	while (pTrade->LimitOrd_GetFirstOrd(&stOrd))
	{
		char szOrigNo[32];
		sprintf(szOrigNo, "%-d", stOrd.nOrdNo);

		//g_log.log(LOGTP_SUCC, "[CancelAllOrd_Symbol][LimitOrd_GetFirstOrd]�����ֹ����ó��(%s)(%s)(�ֹ���ȣ:%d)", AcntNo(), sSymbol.GetBuffer(), stOrd.nOrdNo);

		DoOrder(ORD_TYPE_CNCL, FALSE/*STOP*/, stOrd.zPrc, stOrd.cSide[0], stOrd.nRemainQty, sSymbol, szOrigNo);

		//m_pStratLog->Logging(
		//	AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), EVENT_CLR,
		//	CHK_LIMIT_CNCL, "", "", "", "", "", "",
		//	pTrade->Pos.cSide[0], pTrade->Pos.zPosPrc, pTrade->Pos.nOpenQty,
		//	ORD_LIMIT_CNCL, 0x00, "", szOrigNo, "û����ſ� ���� STOP ���"
		//);

	}

	UpdateTradeMap(sSymbol, pTrade, FALSE);

}


// ���� ���� ������ �ֹ� ���
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

			sprintf(msg, "%s%s[ü������� �����������������](ACC:%s)(����:%s)(����:%c)(����:%d)(���ֹ�:%s)",
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

	// ���¹�ȣ
	memcpy(lpTrIn->Account, m_sAcnt, m_sAcnt.GetLength());

	// ��й�ȣ
	GET_APIPTR->GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), m_sAcnt, m_sPwd);

	// �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)
	lpTrIn->Order_Type[0] = cOrdType;

	if (cOrdType == ORD_TYPE_CNCL)
		memcpy(lpTrIn->Order_Org_No, sOrigNo, min(sizeof(lpTrIn->Order_Org_No), strlen(sOrigNo)));

	// �����ڵ�
	memcpy(lpTrIn->ShortCode, sSymbol.GetBuffer(), sSymbol.GetLength());

	// �ŵ��ż����� (1.�ż� 2.�ŵ�). 
	lpTrIn->BuySell_Type[0] = cSide;

	// ��������	(1.������ 2.���尡). 
	// ü������ (���尡�ϰ��(3) �������ϰ��(1))
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
	// �ֹ�����
	sprintf(temp, "%*d", sizeof(lpTrIn->Order_Volume), nQty);
	memcpy(lpTrIn->Order_Volume, temp, sizeof(lpTrIn->Order_Volume));
	
	// ����ֹ�����
	GET_APIPTR->GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

	// �ֹ��������� (1.�Ϲ��ֹ� 2.STOP�ֹ�)
	if (bStop) {
		lpTrIn->Stop_Type[0] = '2';
		memcpy(lpTrIn->Stop_Price, zPrc, LEN_PRC);	// STOP�ֹ����� (STOP�ֹ��̸� �Է� �ƴϸ� 0 ����)
	}
	else
		lpTrIn->Stop_Type[0] = '1';


	// �ֹ����۱��� 
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
	sprintf(msg, "%s%s[�ֹ�����:%c](ACC:%.*s)(����:%.*s)(����:%.*s)(����:%.*s)(PRC:%.*s)(���ֹ�:%.*s)",
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

	// DB ����
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
	//	g_log.log(LOGTP_ERR, "�ֹ��α����� ����(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
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

// �������� ��ȸ
BOOL	CFBOrdThread::LoadCommonFactor()
{
	//char sApplyYN[2];

	CDBHandler db(GET_DBPTR->Get());

	CString sQ;
	sQ.Format("EXEC FB_GET_STRATEGY_FACTOR");
	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE==db->Exec())
	{
		m_bDoStrategy = FALSE;
		g_log.log(LOGTP_ERR, "FB_GET_STRATEGY_FACTOR ���� ����(%s)(%s)(%s)", AcntNo(), sQ.GetBuffer(), db->GetLastError());
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
		g_log.log(LOGTP_ERR, "FB_GET_STRATEGY_FACTOR �����Ͱ� ����");
		//m_bDoStrategy = FALSE;
		return FALSE;
	}


	//char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH], temp[128];
	//CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);
	//
	//// �ּ� ��Ʈ ����
	//CUtil::GetConfig(szConfig, "CHART_INFO", "MIN_CHART_CNT", temp);
	//m_nMinChartCnt = atoi(temp);

	//CUtil::GetConfig(szConfig, "CHART_INFO", "MAX_CHART_CNT", temp);
	//m_nMaxChartCnt = atoi(temp);

	return TRUE;
}



// ���� �������� ��ȸ
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
		g_log.log(LOGTP_ERR, "FB_GET_ACNT_STRATEGY_INFO ���� ����(%s)(%s)(%s)", AcntNo(), sQ.GetBuffer(), db->GetLastError());
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
		
		sprintf(msg, "%s%s[LOAD ACNT INFO](%s)(%s)(CHART_TP:%d)(��������:%c)(CROSS_YN:%c)(������������/�ս�ƽ��:%d/%d)(�������ƽ:%d)",
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



// ������ǥ ���� �ð��� �������� ����
BOOL CFBOrdThread::IsPassedAssistTime(DWORD dStartClock, DWORD* pdGap)
{
	DWORD result = (DWORD)(GetTickCount() - dStartClock) / CLOCKS_PER_SEC;
	*pdGap = result;
	if (result > DEF_CHECK_STRAT_SEC)
		return TRUE;

	return FALSE;
}


// Ư���ʰ� �������� ����.
BOOL CFBOrdThread::IsPassedSecs(const char *pzChartNm, const UINT nCheckSecs)
{
	char tmp[32];
	time_t tmNow = time(NULL);
	struct tm *stTm = localtime(&tmNow);
	sprintf(tmp, "%.2s", pzChartNm+4);		stTm->tm_hour = atoi(tmp);
	sprintf(tmp, "%.2s", pzChartNm + 6);	stTm->tm_min = atoi(tmp);
	sprintf(tmp, "01" );			stTm->tm_sec = atoi(tmp);	// chart name �� 4�ڸ�

	time_t tmChart = mktime(stTm);
	double past = difftime(tmNow, tmChart);	// sec
	//g_log.log(LOGTP_SUCC, "�ð�üũ.(��Ʈ�ð�:%s)(GAP:%f ��)", pzChartNm, past);
	return ((UINT)past >= nCheckSecs);
}

