// FBI_DealManager.cpp : Defines the entry point for the console application.
//

#include "FBIMainProc.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/TimeInterval.h"
#include "../common/FBIInc.h"

extern BOOL		g_bContinue ;	// flag whether continue process or not
extern CLogMsg	g_log;
extern char		g_zConfig[_MAX_PATH];
extern CMemPool	g_memPool;

CFBIMainProc::CFBIMainProc():CBaseThread("FBIMain", TRUE)
{
	m_unApiTick = m_unSaveData = 0;
	m_hApiTick = m_hSaveData = NULL;
	m_pDBPool = NULL;
	m_bContinue = TRUE;
	m_pApiClient = NULL;
	
	InitializeCriticalSection(&m_csDM);
}

CFBIMainProc::~CFBIMainProc()
{
	Finalize();
}

BOOL CFBIMainProc::Initialize()
{
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
			g_log.log(NOTIFY, "DB OPEN FAILED.(%s)(%s)(%s)", ip, id, pwd);
			return FALSE;
		}
	}

	if (!LoadSymbolInfo()) {
		g_log.log(NOTIFY, "LoadSymbolInfo Error. Terminate Application");
		return FALSE;
	}

	InitApiSocket();

	ResumeThread();

	// create recv thread
	m_hApiTick = (HANDLE)_beginthreadex(NULL, 0, &Thread_ApiChart, this, 0, &m_unApiTick);

	// create save thread
	m_hSaveData = (HANDLE)_beginthreadex(NULL, 0, &Thread_SaveChart, this, 0, &m_unSaveData);

	return TRUE;
}

void CFBIMainProc::Finalize()
{
	PostThreadMessage(m_unApiTick, _FBI::WM_TERMINATE, 0, 0);
	PostThreadMessage(m_unSaveData, _FBI::WM_TERMINATE, 0, 0);

	SAFE_CLOSEHANDLE(m_hApiTick);
	SAFE_CLOSEHANDLE(m_hSaveData);

	SAFE_DELETE(m_pDBPool);
	ClearDealMap();
	DeleteCriticalSection(&m_csDM);
}


BOOL CFBIMainProc::LoadSymbolInfo()
{
	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	sprintf(zQ, "SELECT ARTC_CD FROM AA_ARTC_MST WHERE USE_YN='Y'");
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(NOTIFY, "Load Symbol Error(%s)", zQ);
	}
	else
	{
		while (db->IsNextRow())
		{
			char zArtc[128];	db->GetStr("ARTC_CD", zArtc);
			CDealManager* p = new CDealManager(zArtc);
			EnterCriticalSection(&m_csDM);

			m_mapDealManager[zArtc] = p;
			LeaveCriticalSection(&m_csDM);

			g_log.log(LOGTP_SUCC, "LoadSymbol(%s)", zArtc);
			printf("LoadSymbol(%s)\n", zArtc);

			db->Next();
		}
	}
	db->Close();
	g_log.log(INFO, "Succeeded in LoadSymbolInfo...");
	return TRUE;
}


VOID CFBIMainProc::ClearDealMap()
{
	std::map<std::string, CDealManager*>::iterator it;
	EnterCriticalSection(&m_csDM);
	for (it = m_mapDealManager.begin(); it != m_mapDealManager.end(); ++it)
	{
		CDealManager* s = (*it).second;
		delete s;
	}
	m_mapDealManager.clear();
	LeaveCriticalSection(&m_csDM);
}


void CFBIMainProc::InitApiSocket()
{
	CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "API_TR_IP", m_zApiIP);
	CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "API_TR_PORT", m_zApiPort);

	if (m_pApiClient == NULL)
		m_pApiClient = new CTcpClient("API_TR");
}


void CFBIMainProc::CloseApiSocket()
{
	SAFE_DELETE(m_pApiClient);
}


unsigned WINAPI CFBIMainProc::Thread_ApiChart(LPVOID lp)
{
	CFBIMainProc* p = (CFBIMainProc*)lp;

	CTimeInterval interval;

	_FBI::ST_API_CHART* pSise;
	char zSymbol[128];
	//char tm[9];
	//schar zCurrPrc[32];

	while (TRUE)
	{
		if (!p->m_pApiClient->Begin((LPSTR)p->m_zApiIP, atoi(p->m_zApiPort), 10))
		{
			g_log.log(NOTIFY, "API DataFeed Socket Error[%s][%s]", p->m_zApiIP, p->m_zApiPort);
			Sleep(5000);
		}
		else {
			g_log.log(INFO, "API DataFeed Socket OK(%s)(%s)", p->m_zApiIP, p->m_zApiPort);
			break;
		}
	}
	p->m_pApiClient->StartRecvData();


	interval.start();

	while (p->m_bContinue)
	{
		Sleep(1);

		if (interval.isPassed(MODE_MIN, 30))
		{
			g_log.log(NOTIFY, "30 mins passed without receiving any market data");
			Sleep(1000);
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == _FBI::WM_TERMINATE)
			{
				p->CloseApiSocket();
				return 0;
			}
		}

		if (p->m_pApiClient->HappenedRecvError())
		{
			g_log.log(NOTIFY, "TICK DATA RECV ERROR:%s", p->m_pApiClient->GetMsg());
			continue;
		}
		char* pBuf = NULL;;
		if (!g_memPool.get(&pBuf))
		{
			g_log.log(ERR, "memory pool get error");
			continue;
		}
		int nLen = p->m_pApiClient->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			g_memPool.release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			g_log.log(ERR, "TICK DATA PAKCET 이상(%s)(%s)", pBuf, p->m_pApiClient->GetMsg());
			g_memPool.release(pBuf);
			continue;
		}

		if (nLen > 0)
		{
			interval.nomore_check();

			//printf("[RECV SISE](%.100s)\n", pBuf);

			pSise = (_FBI::ST_API_CHART*)pBuf;
			//sprintf(tm, "%.2s:%.2s:%.2s", pSise->Time, pSise->Time + 2, pSise->Time + 4);
			//memcpy(pSise->Time, tm, sizeof(pSise->Time));
			//sprintf(zCurrPrc, "%.*s", sizeof(pSise->Close), pSise->Close);

			//sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
			CUtil::TrimAll(zSymbol, strlen(zSymbol));
			std::string sSymbol = zSymbol;

			EnterCriticalSection(&p->m_csDM);
			std::map<std::string, CDealManager*>::iterator it = p->m_mapDealManager.find(sSymbol);
			if (it == p->m_mapDealManager.end())
			{
				LeaveCriticalSection(&p->m_csDM);
				g_memPool.release(pBuf);
				continue;
				//g_log.log(LOGTP_ERR, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
			}

			CDealManager* pDM= (*it).second;
			PostThreadMessage(pDM->GetMyThreadID(), _FBI::WM_RECV_API_CHART, 0, (LPARAM)pBuf);
			
			LeaveCriticalSection(&p->m_csDM);
		}
	}
	p->CloseApiSocket();
	return 0;
}


/*AA_SAVE_CHART
	@I_CHART_TP	INT
	,@I_SYMBOL	VARCHAR(10)
	,@I_CHART_DT	CHAR(8)	--YYMMDD
	,@I_CHART_TM	CHAR(4)	--HHMM
	,@I_OPEN	VARCHAR(20)
	,@I_HIGH	VARCHAR(20)
	,@I_LOW		VARCHAR(20)
	,@I_CLOSE	VARCHAR(20)
	,@I_VOL		VARCHAR(10)
	*/
unsigned WINAPI CFBIMainProc::Thread_SaveChart(LPVOID lp)
{
	CFBIMainProc* p = (CFBIMainProc*)lp;

	CDBHandlerAdo db(p->m_pDBPool->Get());
	char zQ[1024];
	while (p->m_bContinue)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == _FBI::WM_TERMINATE)
			{
				return 0;
			}
			if (msg.message == _FBI::WM_RECV_API_CHART)
			{
				_FBI::ST_API_CHART *chart = (_FBI::ST_API_CHART *)msg.lParam;
				int nLen = msg.wParam;

				sprintf(zQ, "EXEC AA_SAVE_CHART "
					"%d"	//@I_CHART_TP	INT
					", '%.*s'"	//	VARCHAR(10)
					", '%.*s'"	//	, @I_CHART_DT	CHAR(8)	--YYMMDD
					", '%.*s'"	//	, @I_CHART_TM	CHAR(4)	--HHMM
					", '%.*s'"	//	, @I_OPEN	VARCHAR(20)
					", '%.*s'"	//	, @I_HIGH	VARCHAR(20)
					", '%.*s'"	//	, @I_LOW		VARCHAR(20)
					", '%.*s'"	//	, @I_CLOSE	VARCHAR(20)
					", '%.*s'"	//	, @I_VOL		VARCHAR(10)
					,
					1			//TODO. 추후 PACKET 에서 추출해야 한다.
					,sizeof(chart->Symbol), chart->Symbol
					, sizeof(chart->Date), chart->Date
					, sizeof(chart->ChartTime), chart->ChartTime	//API 에서 어떻게 오는지 봐야 한다.
					, sizeof(chart->Open), chart->Open
					, sizeof(chart->High), chart->High
					, sizeof(chart->Low), chart->Low
					, sizeof(chart->Close), chart->Close
					, sizeof(chart->Volume), chart->Volume
				);
				if (FALSE == db->ExecQuery(zQ))
				{
					g_log.log(NOTIFY, "AA_SAVE_CHART Error(%s)", zQ);
				}
				db->Close();
				g_memPool.release((char*)chart);
			}


		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	}
	return 0;
}


void CFBIMainProc::ThreadFunc()
{
	WaitForSingleObject(CBaseThread::m_hDie, INFINITE);
}
