// FBI_DealManager.cpp : Defines the entry point for the console application.
//

#include "FBIMainProc.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/TimeInterval.h"
#include "../common/FBIInc.h"
#include <string.h>
#include <memory.h>

extern BOOL		g_bContinue ;	// flag whether continue process or not
extern CLogMsg	g_log;
extern char		g_zConfig[_MAX_PATH];
extern CMemPool	g_memPool;

CFBIMainProc::CFBIMainProc():CBaseThread("FBIMain", TRUE)
{
	m_unSaveData = 0;
	m_hSaveData = NULL;
	m_pDBPool = NULL;
	m_bContinue	= TRUE;
	
	InitializeCriticalSection(&m_csStkOrd);
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

	// SAVE THREAD
	m_hSaveData = (HANDLE)_beginthreadex(NULL, 0, &SaveResultThread, NULL, 0, &m_unSaveData);
	
	if (!LoadStkInfo()) {
		g_log.log(NOTIFY, "LoadStkInfo Error. Terminate Application");
		return FALSE;
	}

	if (!CreateStkOrders()) {
		g_log.log(NOTIFY, "CreateStkOrders Error. Terminate Application");
		return FALSE;
	}

	//TODO.
	// Init SM

	ResumeThread();

	//TODO
	//CDBHandlerAdo db2(m_pDBPool->Get());
	//sprintf(zQ, "EXEC AA_LOG_PROCESS_STATUS 'DealManager', '[%s] 초기화 성공.' ", zStCd);
	//if (FALSE == db2->ExecQuery(zQ))
	//{
	//	g_log.log(ERR/*NOTIFY*/, "AA_LOG_PROCESS_STATUS Error(%s)", zQ);
	//}
	//db2->Close();

	return TRUE;
}

void CFBIMainProc::Finalize()
{
	DeInitializeSM();
	SAFE_DELETE(m_pDBPool);
	ClearDealMap();
	DeleteCriticalSection(&m_csStkOrd);
}


BOOL CFBIMainProc::InitializeSM()
{
	if (!m_smPrc.Begin())
	{
		//TODO. LOG
		return FALSE;
	}

	char zIp[32], zPort[32];
	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "MARKET_IP", zIp);
	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "MARKET_PORT", zPort);
	
	//if(!m_smPrc.AddDest())

	//[SMSERVER_INFO]
	//MARKET_IP = 110.4.89.204
	//	MARKET_PORT = 7999
	//	ORD_IP = 110.4.89.204
	//	ORD_PORT = 6253

	if (!m_smOrd.Begin())
	{
		//TODO. LOG
		return FALSE;
	}
}
VOID CFBIMainProc::DeInitializeSM()
{}

BOOL CFBIMainProc::CreateStkOrders()
{
	BOOL bResult = TRUE;

	std::list<_FBI::ST_STK_INFO>::iterator it;
	for (it = m_lstStkInfo.begin(); it != m_lstStkInfo.end(); it++)
	{
		_FBI::ST_STK_INFO info = *it;
		CStkOrd* p = new CStkOrd(info.sArtcCd);
		if (!p->Initialize(&info, m_unSaveData))
			return FALSE;
		m_mapStkOrd[info.sArtcCd] = p;
	}

	return TRUE;
}


long __stdcall CFBIMainProc::CallBackSMPrc(int index, char* WorkThread, char* Message)
{
	/*
	TODO
	new ST_PRC_INFO
	GetSymbol
	Get StkOrd pointer
	PostMsg to both threads
	PostMsg to both threads => RelayOrdAndPrc(_FBI::WM_RECV_PRC, Pointer of Data)
	*/

	return 0;
}

long __stdcall CFBIMainProc::CallBackSMOrd(int index, char* WorkThread, char* Message)
{
	/*
	TODO
	_FBI::ST_SLORD *pDn = new _FBI::ST_SLORD;

	GetSymbol
	Get StkOrd pointer
	PostMsg to both threads => RelayOrdAndPrc(_FBI::WM_RECV_ORD, Pointer of Data)
	*/
	

	return 0;
}


/*
- Noti the Client
- Save into DB

const int WM_ORD_FIRED		= WM_USER + 519;

*/
unsigned WINAPI CFBIMainProc::SaveResultThread(LPVOID lp)
{
	CFBIMainProc* pThis = (CFBIMainProc*)lp;
	char zQ[1024];
	
	while (g_bContinue)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == _FBI::WM_ORD_FIRED)
			{
				_FBI::ST_SLORD* pSlOrd = (_FBI::ST_SLORD*)msg.lParam;
				CDBHandlerAdo db(pThis->m_pDBPool->Get());

				sprintf(zQ, "EXEC AA_ORD_RESULT_SL "
					"%d"		//@I_ORD_NO		INT
					,",'%s'"	//@I_FIRED_PRC	VARCHAR(20)
					,",'%s'"	//@I_WINLOSE		CHAR(1)		--W, L
					,
					pSlOrd->OrdNo
					, pSlOrd->sFiredPrc.c_str()
					, pSlOrd->cWinLose
				);
				if (FALSE == db->ExecQuery(zQ))
				{
					g_log.log(LOGTP_ERR, "SL Ord Result Err(%s)(%s)", db->GetError(), zQ);
					break;
				}
				else
				{
					if (!db->IsNextRow())
					{
						g_log.log(NOTIFY, "Failed to read AA_ORD_RESULT_SL recordset(%s)", db->GetError());
						break;
					}
					char zWinLose[128] = { 0, };
					if (db->GetStrWithLen("WINLOSE", 10, zWinLose) == NULL)
					{
						g_log.log(NOTIFY, "Failed to read AA_ORD_RESULT_SL recordset(%s)", db->GetError());
						break;
					}

					char zClientSendBuf[1024] = { 0, };
					_FBI::PT_DEAL_STATUS *cPack = (_FBI::PT_DEAL_STATUS *)zClientSendBuf;

					FillMemory(zClientSendBuf, sizeof(_FBI::PT_DEAL_STATUS), 0x20);
					cPack->STX[0] = _FBI::STX;

					char len[32];
					sprintf(len, "%d", sizeof(_FBI::PT_DEAL_STATUS));
					memcpy(cPack->Len, len, strlen(len));
					memcpy(cPack->ArtcCd, pSlOrd->sArtcCd.c_str(), min(sizeof(cPack->ArtcCd), pSlOrd->sArtcCd.size()));
					memcpy(cPack->StkCd, pSlOrd->sStkCd.c_str(), min(sizeof(cPack->StkCd), pSlOrd->sStkCd.size()));
					//memcpy(cPack->DealSeq, z, strlen(z));
					cPack->DealStatus[0] = '5';
					cPack->OrdResult[0] = pSlOrd->cWinLose;
					//memcpy(cPack->Time, pDeal->tm_end, sizeof(cPack->Time));
					memcpy(cPack->ClosePrc, pSlOrd->sFiredPrc.c_str(), min(sizeof(cPack->ClosePrc), pSlOrd->sFiredPrc.size()));

					// yyyymmddhhmm => hh:mm
					//sprintf(pThis->m_zNextCandleTm, "%.2s:%.2s", zChartNm + 8, zChartNm + 10);
					//sprintf(cPack->CandleTime, "%.2s:%.2s", zChartNm + 8, zChartNm + 10);
					//memcpy(cPack->CandleTime, pDeal->tm_chart, sizeof(pDeal->tm_chart));

					cPack->ETX[0] = _FBI::ETX;
					*(cPack->ETX + 1) = 0x00;

					//TODO. pThis->SendToClient(cPack, 0);

				}

				g_memPool.release((char*)pSlOrd);
				db->Close();
			} //if (msg.message == _FBI::WM_ORD_FIRED)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}


BOOL CFBIMainProc::LoadStkInfo()
{
	char zQ[1024];
	int nIdx = 0;
	CDBHandlerAdo db(m_pDBPool->Get());

	sprintf(zQ, "EXEC AA_GET_SYMBOL");

	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load Symbol Error(%s)", zQ);
	}
	else
	{
		while (db->IsNextRow())
		{
			char zStCd[128];	
			char zArtcCd[128];
			double dTickSize;
			long lDotCnt;
			
			if (db->GetStrWithLen("STK_CD", 10, zStCd) == NULL)
			{
				g_log.log(NOTIFY, "LoadSymbol(STK_CD) Error(%s)", db->GetError());
				return FALSE;
			}

			if (db->GetStrWithLen("ARTC_CD", 10, zArtcCd) == NULL)
			{
				g_log.log(NOTIFY, "LoadSymbol(ARTC_CD) Error(%s)", db->GetError());
				return FALSE;
			}

			dTickSize = db->GetDbl("TICK_SIZE");
			lDotCnt = db->GetLong("DOT_CNT");

			//TODO
			//if (strncmp(zArtcCd, "CL", 2) != 0) {
			//	db->Next();
			//	continue;
			//}
			//////////////////////////////////////////////////////////////////////////////

			/*
			std::string	Symbol;
			double		dTickSize;
			int			nDotCnt;
			*/
			_FBI::ST_STK_INFO info;
			ZeroMemory(&info, sizeof(info));
			info.sStkCd		= zStCd;
			info.sArtcCd	= zArtcCd;
			info.dTickSize	= dTickSize;
			info.lDotCnt	= lDotCnt;

			m_lstStkInfo.push_back(info);
			g_log.log(INFO, "LoadSymbol(%s)", zStCd);
			printf("LoadSymbol(%s)\n", zStCd);

			db->Next();
			nIdx++;

		} // while (db->IsNextRow())
	} // else
	db->Close();
	g_log.log(INFO, "Succeeded in LoadStkCode...");
	return TRUE;
}


VOID CFBIMainProc::ClearDealMap()
{
	std::map<std::string, CStkOrd*>::iterator it;
	EnterCriticalSection(&m_csStkOrd);
	for (it = m_mapStkOrd.begin(); it != m_mapStkOrd.end(); ++it)
	{
		CStkOrd* s = (*it).second;
		delete s;
	}
	m_mapStkOrd.clear();
	LeaveCriticalSection(&m_csStkOrd);

}

void CFBIMainProc::ThreadFunc()
{
	WaitForSingleObject(CBaseThread::m_hDie, INFINITE);
}
