
// MudFishOrdDlg.cpp : implementation file
//


#include "AnalyseCor.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/IRUM_Common.h"
#include "../../IRUM_UTIL/TimeInterval.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "Main.h"

extern CLogMsg		g_log;
extern char			g_zConfig[_MAX_PATH];
extern BOOL			g_bContinue;
extern CMemPool		g_memPool;


CAnalyseCor::CAnalyseCor()
{
	m_pApiClient = NULL;
}	

CAnalyseCor::~CAnalyseCor()
{
	Finalize();
}


VOID CAnalyseCor::Finalize()
{
	PostThreadMessage(m_track->GetMyThreadID(), WM_DIE, 0, 0);
	delete m_track;
}


BOOL CAnalyseCor::Initialize()
{

	if (!LoadSymbolInfo()) {
		g_log.log(NOTIFY, "LoadSymbolInfo Error. Terminate Application");
		return FALSE;
	}
	InitApiSocket();

	m_track = new CTrackDiff;
	if (!m_track->Initialize())
		return FALSE;
	
	ResumeThread();
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CAnalyseCor::ThreadFunc()
{
	CTimeInterval interval;

	while (Is_TimeOfStop(5000) == FALSE)
	{
		if (!m_pApiClient->Begin(m_zApiIP, atoi(m_zApiPort), 10))
		{
			g_log.log(NOTIFY, "API DataFeed Socket Error[%s][%s]", m_zApiIP, m_zApiPort);
			continue;
		}

		g_log.log(INFO, "API DataFeed Socket OK[%s][%s]", m_zApiIP, m_zApiPort);

		m_pApiClient->StartRecvData();


		interval.start();

		while (Is_TimeOfStop(1) == FALSE)
		{
			if (interval.isPassed(MODE_MIN, 30))
			{
				g_log.log(ERR, "30 mins passed without receiving any market data");
				Sleep(1000);
			}


			if (m_pApiClient->HappenedRecvError())
			{
				g_log.log(ERR, "TICK DATA RECV ERROR:%s", m_pApiClient->GetMsg());
				continue;
			}
			char* pBuf = NULL;;
			if (!g_memPool.get(&pBuf))
			{
				continue;
			}
			int nLen = m_pApiClient->GetOneRecvedPacket(pBuf);
			if (nLen == 0) {
				g_memPool.release(pBuf);
				continue;
			}
			if (nLen < 0)
			{
				g_log.log(ERR,"TICK DATA PAKCET 이상(%s)(%s)", pBuf, m_pApiClient->GetMsg());
				g_memPool.release(pBuf);
				continue;
			}

			if (nLen > 0)
			{
				interval.nomore_check();

				//pSise = (ST_PACK2CHART_EX*)pBuf;
				//sprintf(tm, "%.2s:%.2s:%.2s", pSise->Time, pSise->Time + 2, pSise->Time + 4);
				//memcpy(pSise->Time, tm, sizeof(pSise->Time));
				//sprintf(zCurrPrc, "%.*s", sizeof(pSise->Close), pSise->Close);

				PostThreadMessage(m_track->GetMyThreadID(), WM_RECV_API_MD, (WPARAM)(sizeof(ST_PACK2CHART_EX)), (LPARAM)pBuf);
				
			}
		} // while (Is_TimeOfStop(1) == FALSE)
	} // while (Is_TimeOfStop(10) == FALSE) 
	CloseApiSocket();
	return ;
}

#if 0
unsigned WINAPI CAnalyseCor::Thread_SaveDBLog(LPVOID lp)
{
	CAnalyseCor* p = (CAnalyseCor*)lp;

	CDBHandlerAdo db(p->m_pDBPool->Get());
	char zQ[1024];
	while (p->m_bContinue)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				return 0;
			}
			if (msg.message == WM_STRAT_LOGGING)
			{
				ABOTLOG_NO1 *log = (ABOTLOG_NO1*)msg.lParam;
				int nLen = msg.wParam;

				sprintf(zQ, "EXEC ABORT_NO1_SAVELOG "
					"'%s'"
					",'%s'"	//@I_SYMBOL	varchar(11)
					",'%s'"	//@I_START_ID varchar(20)
					",'%.1s'"	//@I_FIRE_YN char(1)
					",'%.1s'"	//@I_OPEN_CLOS char(1)
					",'%.1s'"	//, @I_PL_TP	CHAR(1)		--P, L
					",'%.1s'"	//@I_BS_TP char(1)
					",'%s'"	//@@I_ORD_QTY varchar(10)
					",'%s'"	//@I_CURR_PRC varchar(20)
					",'%s'"	//@I_STRAT_PRC varchar(20)
					",'%s'"	//@I_OPEN_PRC varchar(20)
					",'%s'"	//@I_ENTRY_PRC varchar(20)
					",'%s'"	//@I_ENTRY_PERCENT varchar(10)
					",'%s'"	//@I_CROSS_1MIN varchar(10)
					",'%s'"	//@I_CROSS_3MIN varchar(10)
					",'%s'"	//@I_CROSS_5MIN varchar(10)
					",'%s'"	//, @I_MAX_PRC	VARCHAR(20)
					",'%s'"	//, @I_PTCLR_PERCENT VARCHAR(10)
					",'%s'"	//@I_API_TM varchar(9)
					",'%s'"	//@I_MSG varchar(256)
					,
					p->m_zServerID
					, log->zSymbol
					, log->zStratID
					, log->FireYN
					, log->OpenClose
					, log->PLTp
					, log->BsTp
					, log->zOrdQty
					, log->zCurrPrc
					, log->zStratPrc
					, log->zOpenPrc
					, log->zEntryPrc
					, log->zEntryPercent
					, log->zCross_1min
					, log->zCross_3min
					, log->zCross_5min
					, log->zMaxPrc
					, log->zPtClrTriggerPercent
					, log->zApiTM
					, log->zMsg
				);
				if (FALSE == db->ExecQuery(zQ))
				{
					p->showMsg(FALSE, "ABORT_NO1_SAVELOG Error(%s)", zQ);
				}
				db->Close();
				p->m_pMemPool->release((char*)log);
			}// if (msg.message == WM_STRAT_LOGGING)
		
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	}
	return 0;
}
#endif



BOOL CAnalyseCor::LoadSymbolInfo()
{
	CUtil::GetConfig(g_zConfig, "SYMBOLS", "6E", m_zEur);
	CUtil::GetConfig(g_zConfig, "SYMBOLS", "6B", m_zGbp);

	g_log.log(INFO, "조회종목코드(%s)(%s)", m_zEur, m_zGbp);
	return TRUE;
}



void CAnalyseCor::InitApiSocket()
{
	CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "SISE_IP", m_zApiIP);
	CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "SISE_PORT", m_zApiPort);

	if (m_pApiClient == NULL)
		m_pApiClient = new CTcpClient("MD");

}


void CAnalyseCor::CloseApiSocket()
{
	SAFE_DELETE(m_pApiClient);
}


