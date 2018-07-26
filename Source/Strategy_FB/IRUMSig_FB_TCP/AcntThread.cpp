
#include "AcntThread.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "FBDefine.h"
#include "FBStrat_OpenPrc.h"
#include "FBStrat_AssistSignal.h"

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];


CAcntThread::CAcntThread(char* pAcnt, char* pPwd)
{
	m_sAcnt = pAcnt;
	m_sPwd = pPwd;

	m_pStratLog = NULL;
	m_pShmQ = NULL;
	m_pNanoSub = NULL;
	m_pLimitOrd = NULL;
	m_pStopOrd = NULL;
	m_pPos = NULL;
	m_pDBPool = NULL;
	m_pStrat1 = NULL;
	m_pStrat2 = NULL;

	ZeroMemory(m_zShmNm, sizeof(m_zShmNm));
	ZeroMemory(m_zMutexNm, sizeof(m_zMutexNm));
}

CAcntThread::~CAcntThread()
{
	End();

}



BOOL CAcntThread::Begin()
{
	if (!Begin_DB())	return FALSE;
	if (!Begin_StratLog())	return FALSE;
	if (!Begin_NanoSub())	return FALSE;
	
	return TRUE;
}


BOOL CAcntThread::Begin_StratLog()
{
	SAFE_DELETE(m_pStratLog);
	m_pStratLog = new CStratLogDB(m_pDBPool);
	return m_pStratLog->Begin();
}

BOOL CAcntThread::Begin_Shm()
{
	SAFE_DELETE(m_pShmQ);

	if (strlen(m_zShmNm) == 0 || strlen(m_zMutexNm)==0)
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN 에러. SHM 이름이 없다.");
		return FALSE;
	}

	m_pShmQ = new CQueueShm();

	//	OPEN SHM
	if (!m_pShmQ->Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN 에러(%s)(%s)", m_zShmNm, m_pShmQ->GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN 성공(%s)", m_zMutexNm);


	return TRUE;
}

BOOL CAcntThread::Begin_NanoSub()
{
	SAFE_DELETE(m_pNanoSub);

	m_pNanoSub = new CNanoPubSub(TP_SUB);
	return (m_pNanoSub->Begin(INNER_CHANNEL_PUBSUB));
}

BOOL CAcntThread::Begin_DB()
{

	char ip[32], id[32], pwd[32], cnt[32], dbname[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", dbname);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	m_pDBPool = (VOID*)new CDBPoolAdo(ip, id, pwd, dbname);
	if (!DBPoolPtr(m_pDBPool)->Init(atoi(cnt)))
	{
		g_log.log(LOGTP_FATAL, "DB OPEN FAIL(MSG:%s)", ((CDBPoolAdo*)m_pDBPool)->GetMsg());
		g_log.log(LOGTP_FATAL, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, dbname);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}
	return TRUE;
}


VOID CAcntThread::End()
{
	ReleaseStrat();

	SAFE_DELETE(m_pStratLog);
	SAFE_DELETE(m_pShmQ);
	SAFE_DELETE(m_pNanoSub);
	SAFE_DELETE(m_pLimitOrd);
	SAFE_DELETE(m_pStopOrd);
	SAFE_DELETE(m_pPos);
	SAFE_DELETE(m_pDBPool);
}

VOID CAcntThread::ReleaseStrat()
{
	//TODO. 전략클래스 삭제
	SAFE_DELETE(m_pStrat1);
	SAFE_DELETE(m_pStrat2);

	ZeroMemory(m_zShmNm, sizeof(m_zShmNm));
	ZeroMemory(m_zMutexNm, sizeof(m_zMutexNm));
}

BOOL CAcntThread::LoadStrat()
{
	ReleaseStrat();

	if (!LoadCommonFactor())	return FALSE;
	if (!LoadAcntTradeInfo())	return FALSE;

	// SHM OPEN
	if (!Begin_Shm())	return FALSE;

	// TODO. 전략클래스들에게 SHM 포인터, 종목코드 재 전달

	return  TRUE;
}


// GET COMMON FACTOR FOR STRATEGY
BOOL CAcntThread::LoadCommonFactor()
{
	CDBHandlerAdo db(DBPoolPtr(m_pDBPool)->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC STRAT_COMP_FACTOR_FB");
	if (!db->ExecQuery(zQ))
	{
		g_log.log(LOGTP_ERR,  "공통정보조회 오류(%s)", zQ);
		return FALSE;
	}

	if (db->IsNextRow()) {
		char zTemp[128] = { 0, };
		strcpy(m_CommFactor.zCompCode, db->GetStr("COMP_CD", zTemp));
		m_CommFactor.nProfitCutTick = db->GetLong("PROFITCUT_TICK");
		m_CommFactor.nLossCutTick = db->GetLong("LOSSCUT_TICK");
		m_CommFactor.nTotProfitLimit = db->GetLong("TOT_PROFIT_LIMIT");
		m_CommFactor.nTotLossLimit = db->GetLong("TOT_LOSS_LIMIT");
	}
	else
	{
		g_log.log(LOGTP_ERR, "공통정보조회 오류-No record(%s)", zQ);
		return FALSE;
	}
	return TRUE;
}


BOOL CAcntThread::LoadAcntTradeInfo()
{
	// 종목정보 get
	// m_sSymbol

	// TODO.
	// m_zShmNm
	// m_zMutexNm

	// todo.
	// m_unStratNo = FBSTRAT_OPEN
	return TRUE;
}


VOID CAcntThread::ThreadFunc()
{
	__try {
		MainFunc();
	}
	__except (ReportException(GetExceptionCode(), "CAcntThread::ThreadFunc", m_szMsg))
	{
		g_log.log(LOGTP_FATAL, m_szMsg);
		exit(0);
	}

}


VOID CAcntThread::MainFunc()
{
	if (!LoadCommonFactor()) return ;
	if (!LoadAcntTradeInfo()) return ;


	//BOOL bMySise;
	char zSiseBuff[1024];
	char zTemp[1024] = { 0, };
	std::string sSymbol;
	CHART_TP chartTp;
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
			if(msg.message== WM_MANAGER_NOTIFY_SOCK)
			{
				ST_MANAGER_SOCKNOTIFY* p = (ST_MANAGER_SOCKNOTIFY*)msg.lParam;
				if (strncmp(p->AcntNo, AcntNo(), sizeof(p->AcntNo)) == 0) 
				{
					DoProc_ManagerNotify(p);
				}
				
			} // if(msg.message== WM_MANAGER_NOTIFY_SOCK)

			else if (msg.message == WM_NOTI_MATCH)
			{
				//if (IsDoStrategy())
				//{
				//	if (IsMySymbolData(FALSE, (char*)msg.lParam, /*out*/sSymbol, /*out*/idxChart))
				//		DoProc_APIRespose((At0985O_st*)msg.lParam, sSymbol);
				//}
			} // case WM_NOTI_MATCH:
			else if (msg.message == WM_STRAT_RESULT)
			{
				DoProc_StratResult((char*)msg.lParam);
			}
			delete ((void*)msg.lParam);

		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))


		memset(zSiseBuff, 0x00, sizeof(zSiseBuff));
		int nRecvLen = m_pNanoSub->Subscribe((void*)zSiseBuff);

		//bMySise = FALSE;
		// 시세를 수신한 경우
		if (nRecvLen > 0)
		{
			//if (FALSE == IsDoStrategy())
			//	continue;

			//g_log.log(LOGTP_SUCC, "[RECV](%.100s)", zSiseBuff);
			//if (IsMySymbolData(TRUE, zSiseBuff, /*out*/sSymbol, /*out*/idxChart))
			{
				//g_log.log(LOGTP_SUCC, zSiseBuff);
				DoProc_RecvMarketData(chartTp, zSiseBuff);
			}
		}

	} // while (TRUE)

}



VOID CAcntThread::DoProc_RecvMarketData(const CHART_TP idx, const char* pSise)
{
	__try
	{
		__try
		{
			// 지난 차트에 들어가있는 미체결주문 취소
			//Cancel_RemainOrd(pSise, sSymbol);

			// 진입즉시 설정한 STOP 주문 CHECK
			//Check_StopFire(pSise, sSymbol);

			
			CallStratWithMarketData(idx, pSise);
		}
		__except (ReportException(GetExceptionCode(), "CFBOrdThread::DoProc_RecvMarketData", m_szMsg))
		{
			g_log.log(LOGTP_FATAL, m_szMsg);
		}
	}
	__finally
	{
	}

}


VOID CAcntThread::CallStratWithMarketData(const CHART_TP idx, const char* pSise)
{

}


BOOL CAcntThread::IsMySymbol(std::string sSymbol)
{
	return TRUE;
}

VOID CAcntThread::DoProc_ManagerNotify(ST_MANAGER_SOCKNOTIFY* p)
{
	char zTemp[1024];
	std::string sSymbol;

	if (p->OrdTp[0] == NOTIFY_ORD_TP_NEW)
	{

		char zPrc[32] = { 0, }, zSide[32] = { 0, };
		sprintf(zTemp, "%.*s", sizeof(p->Symbol), p->Symbol);
		CUtil::TrimAll(zTemp, strlen(zTemp));
		sSymbol = std::string(zTemp);

		if (IsMySymbol(sSymbol) == FALSE) {
			/*sprintf(zMsg, "%s%s신규주문요청 수신.(%s) 그러나 선택종목(%s) 아니므로(%s) SKIP",
			LOG1_ADMIN, LOG2_ADMIN_NEWORD, AcntNo(), GetMySymbol(), sSymbol.GetBuffer());
			g_log.log(LOGTP_SUCC, zMsg);*/
			m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_NEWORD, m_szMsg);
			return;
		}
	}
	else if (p->OrdTp[0] == NOTIFY_ORD_TP_ALLCLR)
	{
		//TODO. ALL CLEAR
	}
	else if (p->OrdTp[0] == NOTIFY_ORD_TP_ALLCNCL)
	{
		//TODO. ALL CANCEL
	}

}


VOID CAcntThread::DoProc_APIRespose(At0985O_st* p)
{
	__try
	{
		__try
		{
			APIResponseProc(p);
		}
		__except (ReportException(GetExceptionCode(), "CAcntThread::DoProc_APIRespose", m_szMsg))
		{
			g_log.log(LOGTP_FATAL, m_szMsg);
		}
	}
	__finally
	{

	}
}

VOID CAcntThread::APIResponseProc(At0985O_st* p)
{}

// Logic after check every strategy
VOID CAcntThread::DoProc_StratResult(char* pResult)
{
	STRAT_RESULT* p = (STRAT_RESULT*)pResult;
}

// Send order to broker
VOID CAcntThread::SendOrderToAPI()
{

}


VOID CAcntThread::StrateryProc()
{
	if (m_unStratNo == FBSTRAT_OPEN)
		Strat1_OpenPrc();
	if (m_unStratNo == FBSTRAT_ASSIST)
		Strat2_Assist();
	if (m_unStratNo == FBSTRAT_MACROSS)
		Strat3_MACross();
	if (m_unStratNo == FBSTRAT_SIDEWALKCROSS)
		Strat4_SidewalkCross();
	if (m_unStratNo == FBSTRAT_COMPOSE_1)
		Strat5_Compose1();
	if (m_unStratNo == FBSTRAT_COMPOSE_2)
		Strat6_Compose2();
}

VOID CAcntThread::Strat1_OpenPrc()
{
	if (!m_pStrat1)
		m_pStrat1 = new CFBStrat_OpenPrc((DWORD)m_dwThreadID, m_sSymbol, m_pShmQ, m_pPos);

	m_pStrat1->DoStrategy(NULL);
}

VOID CAcntThread::Strat2_Assist()
{
	if (!m_pStrat1)
		m_pStrat1 = new CFBStrat_AssistSignal((DWORD)m_dwThreadID, m_sSymbol, m_pShmQ, m_pPos);

	m_pStrat1->DoStrategy(NULL);
}

VOID CAcntThread::Strat3_MACross()
{}
VOID CAcntThread::Strat4_SidewalkCross()
{}

VOID CAcntThread::Strat5_Compose1()
{
	if (!m_pStrat1)
		m_pStrat1 = new CFBStrat_OpenPrc((DWORD)m_dwThreadID, m_sSymbol, m_pShmQ, m_pPos);

	if (!m_pStrat2)
		m_pStrat2 = new CFBStrat_AssistSignal((DWORD)m_dwThreadID, m_sSymbol, m_pShmQ, m_pPos);

	m_pStrat1->DoStrategy(NULL);
	m_pStrat2->DoStrategy(NULL);
}
VOID CAcntThread::Strat6_Compose2()
{}