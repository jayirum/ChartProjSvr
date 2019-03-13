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
	//m_unApiTick = m_unSaveData = 0;
	//m_hApiTick = m_hSaveData = NULL;
	m_pDBPool = NULL;
	m_bContinue = TRUE;
	//m_pApiClient = NULL;
	
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
			g_log.log(ERR/*NOTIFY*/, "DB OPEN FAILED.(%s)(%s)(%s)", ip, id, pwd);
			return FALSE;
		}
	}

	if (!LoadStkCode()) {
		g_log.log(ERR/*NOTIFY*/, "LoadStkCode Error. Terminate Application");
		return FALSE;
	}

	//InitApiSocket();

	ResumeThread();

	// create recv thread
	//m_hApiTick = (HANDLE)_beginthreadex(NULL, 0, &Thread_ApiChart, this, 0, &m_unApiTick);

	// create save thread
	//m_hSaveData = (HANDLE)_beginthreadex(NULL, 0, &Thread_SaveChart, this, 0, &m_unSaveData);

	return TRUE;
}

void CFBIMainProc::Finalize()
{
	//PostThreadMessage(m_unApiTick, _FBI::WM_TERMINATE, 0, 0);
	//PostThreadMessage(m_unSaveData, _FBI::WM_TERMINATE, 0, 0);

	//SAFE_CLOSEHANDLE(m_hApiTick);
	//SAFE_CLOSEHANDLE(m_hSaveData);

	SAFE_DELETE(m_pDBPool);
	ClearDealMap();
	DeleteCriticalSection(&m_csDM);
}


BOOL CFBIMainProc::LoadStkCode()
{
	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	int nIdx = 0;

	sprintf(zQ, "EXEC AA_GET_SYMBOL");

	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load Symbol Error(%s)", zQ);
	}
	else
	{
		while (db->IsNextRow())
		{
			char zStCd[128];	db->GetStr("STK_CD", zStCd);
			char zArtcCd[128];	db->GetStr("ARTC_CD", zArtcCd);
			
			CDealManager* p = new CDealManager(zStCd, zArtcCd, nIdx++);
			if (!p->Initialize()) {
				delete p;
				db->Next();
				continue;
			}

			EnterCriticalSection(&m_csDM);
			m_mapDealManager[zStCd] = p;
			LeaveCriticalSection(&m_csDM);

			g_log.log(LOGTP_SUCC, "LoadSymbol(%s)", zStCd);
			printf("LoadSymbol(%s)\n", zStCd);

			db->Next();
		}
	}
	db->Close();
	g_log.log(INFO, "Succeeded in LoadStkCode...");
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

void CFBIMainProc::ThreadFunc()
{
	WaitForSingleObject(CBaseThread::m_hDie, INFINITE);
}


#if 0
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


/*
struct PT_API_CHART
{
char	STX[1];
char	Len[4];
char	Symbol[FBILEN_SYMBOL];
char	Date[8];
char	ChartType[1];	// m:minute, h:hour, d:day
char	TimeFrame[3];	// 001: 1min, 005
char	ChartTime[5];	// hh:mm
char	Open[FBILEN_PRC];
char	High[FBILEN_PRC];
char	Low[FBILEN_PRC];
char	Close[FBILEN_PRC];
char	Volume[FBILEN_VOLUME];
char	ETX[1];
};
*/
void CFBIMainProc::testChart()
{
	char buffer[512];
	char zStkCd[] = "6AH19";
	SYSTEMTIME st;
	char z[128];
	double dBasePrc = 0.7107000000;

	while (m_bContinue)
	{
		Sleep(1000 * 60);	// 1min

		_FBI::PT_API_CHART* chart = (_FBI::PT_API_CHART*)buffer;
		memset(chart, 0x20, sizeof(_FBI::PT_API_CHART));
		buffer[sizeof(_FBI::PT_API_CHART)] = 0x00;

		memcpy(chart->StkCd, zStkCd, strlen(zStkCd));

		GetLocalTime(&st);
		sprintf(chart->Date, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
		chart->ChartType[0] = 'M';
		sprintf(chart->ChartTime, "%02d:%02d", st.wHour, st.wMinute);

		sprintf(z, "%.2f", dBasePrc);
		memcpy(chart->Open, z, strlen(z));

		sprintf(z, "%.2f", dBasePrc + 0.0025);
		memcpy(chart->High, z, strlen(z));

		sprintf(z, "%.2f", dBasePrc - 0.0025);
		memcpy(chart->Low, z, strlen(z));

		int remainder = (rand()*rand()/rand()) % 5;
		if (remainder == 0)	sprintf(z, "%.2f", dBasePrc + 0.002);
		if (remainder == 1)	sprintf(z, "%.2f", dBasePrc - 0.0015);
		if (remainder == 2)	sprintf(z, "%.2f", dBasePrc + 0.0013);
		if (remainder == 3)	sprintf(z, "%.2f", dBasePrc - 0.0016);
		if (remainder == 4)	sprintf(z, "%.2f", dBasePrc + 0.002);

		memcpy(chart->Close, z, strlen(z));

		CUtil::TrimAll(zStkCd, strlen(zStkCd));
		std::string sStk = zStkCd;

		EnterCriticalSection(&m_csDM);
		std::map<std::string, CDealManager*>::iterator it = m_mapDealManager.find(sStk);
		if (it != m_mapDealManager.end())
		{
			char* pBuf = g_memPool.get();
			memcpy(pBuf, (char*)chart, sizeof(_FBI::PT_API_CHART));
			CDealManager* pDM = (*it).second;
			PostThreadMessage(pDM->GetMyThreadID(), _FBI::WM_RECV_API_CHART, 0, (LPARAM)pBuf);

			char* pBuf2 = g_memPool.get();
			memcpy(pBuf2, (char*)chart, sizeof(_FBI::PT_API_CHART));
			PostThreadMessage(m_unSaveData, _FBI::WM_RECV_API_CHART, 0, (LPARAM)pBuf2);
		}
		LeaveCriticalSection(&m_csDM);
	}
}

unsigned WINAPI CFBIMainProc::Thread_ApiChart(LPVOID lp)
{
	//TODO
	CFBIMainProc* p = (CFBIMainProc*)lp;
	p->testChart();
	return 0;


	CTimeInterval interval;

	_FBI::PT_API_CHART* pSise;
	char zSymbol[128];
	//char tm[9];
	//schar zCurrPrc[32];

	while (TRUE)
	{
		if (!p->m_pApiClient->Begin((LPSTR)p->m_zApiIP, atoi(p->m_zApiPort), 10))
		{
			g_log.log(ERR/*NOTIFY*/, "API DataFeed Socket Error[%s][%s]", p->m_zApiIP, p->m_zApiPort);
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
			g_log.log(ERR/*NOTIFY*/, "30 mins passed without receiving any market data");
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
			g_log.log(ERR/*NOTIFY*/, "TICK DATA RECV ERROR:%s", p->m_pApiClient->GetMsg());
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

			pSise = (_FBI::PT_API_CHART*)pBuf;
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
				_FBI::PT_API_CHART *chart = (_FBI::PT_API_CHART *)msg.lParam;
				int nLen = msg.wParam;

				sprintf(zQ, "EXEC AA_CHART_SAVE "
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
					0			//TODO. 추후 PACKET 에서 추출해야 한다.
					,sizeof(chart->StkCd), chart->StkCd
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
					g_log.log(ERR/*NOTIFY*/, "AA_SAVE_CHART Error(%s)(%s)",db->GetError(), zQ);
				}
				else
				{
					g_log.log(INFO, "AA_SAVE_CHART OK(%s)", zQ);
				}
				db->Close();
				g_memPool.release((char*)chart);
			}


		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	}
	return 0;
}
#endif
