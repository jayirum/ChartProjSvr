
// FBI_DealManager.cpp : Defines the entry point for the console application.
//

#include "FBIMainProc.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/IRExcept.h"
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
	//m_bContinue	= TRUE;
	
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

	// Init SM
	if (!InitializeSM())
		return FALSE;

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
	m_hSaveData = (HANDLE)_beginthreadex(NULL, 0, &SaveResultThread, this, 0, &m_unSaveData);
	
	if (!LoadStkInfo()) {
		g_log.log(NOTIFY, "LoadStkInfo Error. Terminate Application");
		return FALSE;
	}

	if (!CreateStkOrdManagers()) {
		g_log.log(NOTIFY, "CreateStkOrdManagers Error. Terminate Application");
		return FALSE;
	}


	char zQ[512];
	CDBHandlerAdo db2(m_pDBPool->Get());
	sprintf(zQ, "EXEC AA_LOG_PROCESS_STATUS 'FBI_DealManagerSL', '초기화 성공.' ");
	if (FALSE == db2->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "AA_LOG_PROCESS_STATUS Error(%s)", zQ);
	}
	db2->Close();


	CBaseThread::ResumeThread();

	g_log.log(INFO, "Init OK");
	return TRUE;
}

void CFBIMainProc::Finalize()
{
	ClearDealMap();
	DeInitializeSM();
	SAFE_DELETE(m_pDBPool);
	DeleteCriticalSection(&m_csStkOrd);
}


VOID CFBIMainProc::ThreadFunc()
{
	LoadNcntrInfo();
	Is_TimeOfStop(INFINITE);
}


/*
// Load non-cntr orders
[ORD_NO]
,[USER_ID]
,[ARTC_CD]
,[STK_CD]
,[UPDOWN]
,[ORD_STATUS]
,[ORD_PRC]
,[SL_TICK]
*/
VOID CFBIMainProc::LoadNcntrInfo()
{
	char	zQ[1024]	= { 0, };
	char	zTemp[128]	= { 0, };
	int		nIdx		= 0;
	_FBI::ST_SLORD	stOrdInfo;

	CDBHandlerAdo db(m_pDBPool->Get());

	sprintf(zQ, "EXEC AA_LOAD_NCNTR_ORD");

	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load Non Cntr Error(%s)", zQ);
	}
	else
	{
		while (db->IsNextRow())
		{
			ZeroMemory(&stOrdInfo, sizeof(stOrdInfo));

			stOrdInfo.OrdNo = db->GetLong("ORD_NO");

			if (db->GetStrWithLen("USER_ID", 20, zTemp) == NULL)
			{
				g_log.log(NOTIFY, "LoadNcntrInfo(USER_ID) Error(%s)", db->GetError());
				return ;
			}
			CUtil::RTrim(zTemp,strlen(zTemp));
			stOrdInfo.sUserId = zTemp;

			if (db->GetStrWithLen("ARTC_CD", 20, zTemp) == NULL)
			{
				g_log.log(NOTIFY, "LoadNcntrInfo(ARTC_CD) Error(%s)", db->GetError());
				return;
			}
			CUtil::RTrim(zTemp, strlen(zTemp));
			stOrdInfo.sArtcCd = zTemp;

			if (db->GetStrWithLen("STK_CD", 20, zTemp) == NULL)
			{
				g_log.log(NOTIFY, "LoadNcntrInfo(STK_CD) Error(%s)", db->GetError());
				return;
			}
			CUtil::RTrim(zTemp, strlen(zTemp));
			stOrdInfo.sStkCd = zTemp;

			if (db->GetStrWithLen("UPDOWN", 20, zTemp) == NULL)
			{
				g_log.log(NOTIFY, "LoadNcntrInfo(UPDOWN) Error(%s)", db->GetError());
				return;
			}
			stOrdInfo.cUpDn = zTemp[0];

			stOrdInfo.nOrdStatus	= db->GetLong	("ORD_STATUS");
			stOrdInfo.dOrdPrc		= db->GetDouble	("ORD_PRC");
			stOrdInfo.nTickCnt		= db->GetLong	("SL_TICK");

			g_log.log(INFO, "LoadNcntr(UserId:%s)(OrdNo:%d)(ArtcCd:%s)(StkCd:%s)(UpDn:%c)(OrdStatus:%d)(OrdPrc:%f)(SLTick:%d)"
				, stOrdInfo.sUserId.c_str()
				, stOrdInfo.OrdNo
				, stOrdInfo.sArtcCd.c_str()
				, stOrdInfo.sStkCd.c_str()
				, stOrdInfo.cUpDn
				, stOrdInfo.nOrdStatus
				, stOrdInfo.dOrdPrc
				, stOrdInfo.nTickCnt
			);

			map<string, CStkOrdManager*>::iterator it = m_mapStkOrd.find(stOrdInfo.sStkCd);
			if (it == m_mapStkOrd.end())
			{
				g_log.log(ERR, "[LoadNcntr] This symbol is not related(%s)", stOrdInfo.sStkCd.c_str());
			}
			else
			{
				CStkOrdManager* pStkThread = ((*it).second);
				pStkThread->RelayOrdAndPrc(_FBI::WM_ORD_RECV, (char*)&stOrdInfo, sizeof(_FBI::ST_SLORD));
			}
			
			db->Next();

		} // while (db->IsNextRow())
	} // else
	db->Close();
	g_log.log(INFO, "Succeeded in LoadNcntr...");
}

BOOL CFBIMainProc::InitializeSM()
{
	char zIp1[32], zIp2[32], zPort1[32],zPort2[32];
	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "MARKET_IP", zIp1);
	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "MARKET_PORT", zPort1);

	m_smPrc = new CSmartMessage;

	if (!m_smPrc->Begin())
	{
		g_log.log(NOTIFY, "SM PRC Begin Error. Terminate Application");
		return FALSE;
	}
	if (!m_smPrc->ConnectSMSrv(zIp1, atoi(zPort1)))
	{
		g_log.log(NOTIFY, "SM PRC connect Error(IP:%s)(Port:%s) Terminate Application", zIp1, zPort1);
		return FALSE;
	}

	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "ORDER_IP", zIp2);
	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "ORDER_PORT", zPort2);
	m_smOrd = new CSmartMessage;
	if (!m_smOrd->Begin())
	{
		g_log.log(NOTIFY, "SM Ord Begin Error. Terminate Application");
		return FALSE;
	}
	if (!m_smOrd->ConnectSMSrv(zIp2, atoi(zPort2)))
	{
		g_log.log(NOTIFY, "SM Ord connect Error(IP:%s)(Port:%s) Terminate Application", zIp2, zPort2);
		return FALSE;
	}


	m_smPrc->AddDest((char*)SISE_GW, (char*)"");	// MSG_MKT_FX_EXEC);
	m_smPrc->SetRecvCallBackFn((LPVOID)this, (RECV_CALLBACK)CallBackSMPrc);
	g_log.log(NOTIFY, "SM PRC Connect & AddDest Ok(IP:%s)(Port:%s)", zIp1, zPort1);


	m_smOrd->AddDest((char*)ORD_REAL_GW, (char*)MSGR_GW_ORD_NEW);	// MSGR_GW_ORD_NEW);
	m_smOrd->SetRecvCallBackFn((LPVOID)this, (RECV_CALLBACK)CallBackSMOrd);
	g_log.log(NOTIFY, "SM ORD Connect & AddDest Ok(IP:%s)(Port:%s)(%s)(%s)", zIp2, zPort2, ORD_REAL_GW, MSGR_GW_ORD_NEW);

	return TRUE;
}
VOID CFBIMainProc::DeInitializeSM()
{
	m_smPrc->DisConnectSMSrv();
	m_smOrd->DisConnectSMSrv();
	delete m_smPrc;
	delete m_smOrd;
}

BOOL CFBIMainProc::CreateStkOrdManagers()
{
	BOOL bResult = TRUE;

	std::list<_FBI::ST_STK_INFO>::iterator it;
	for (it = m_lstStkInfo.begin(); it != m_lstStkInfo.end(); it++)
	{
		_FBI::ST_STK_INFO info = *it;
		CStkOrdManager* p = new CStkOrdManager(info.sArtcCd);
		if (!p->Initialize(&info, m_unSaveData))
			return FALSE;
		m_mapStkOrd[info.sStkCd] = p;
	}

	return TRUE;
}


long WINAPI CFBIMainProc::CallBackSMPrc(int index, char* pCustomPtr, char* Message)
{
	
	CFBIMainProc* pThis = (CFBIMainProc*)pCustomPtr;

	if (strncmp(Message, MSG_MKT_FX_EXEC, strlen(Message)) != 0)
		return 0;


	char pRecvData[_FBI::MEM_BLOCK_SIZE] = { 0, };
	//char* pRecvData = new char[_FBI::MEM_BLOCK_SIZE]
	int nSize = pThis->m_smPrc->GetBinaryFieldValueEx((char *)fldFXExec, 0, pRecvData);	// (char*)&FutExec);
	if (!nSize)
		return 0;

	_FBI::SMPACK_FX_EXEC* pack = (_FBI::SMPACK_FX_EXEC*)pRecvData;
	
	//g_log.log(INFO,"(%s)(cup:%s)(sip:%s)(hip:%s)(lip:%s)(vol;%s)(amt:%s)(time:%s)(side:%s)(ydiffSign:%s)(chgrate:%s)(execvol:%s)",
	//	pack->issue, pack->cup, pack->sip, pack->hip, pack->lip, pack->vol, pack->amt, pack->time, pack->side,
	//	pack->ydiffSign, pack->chgrate, pack->execvol);

	string sSymbol = string(pack->issue);
	map<string, CStkOrdManager*>::iterator it = pThis->m_mapStkOrd.find(sSymbol);
	if (it == pThis->m_mapStkOrd.end())
	{
		//g_log.log(ERR, "PrcData with non-related symbol(%s)", FutExec.issue);
		return -1;
	}

	((*it).second)->RelayOrdAndPrc(_FBI::WM_RECV_PRC, (void*)pRecvData, nSize);
	return 0;
}


/*
struct ST_SLORD
{
int		OrdNo;
int		nOrdStatus;
double	dOrdPrc;
char	cUpDn;
int		nTickCnt;
double	dWinPrc;
double	dLosePrc;
char	cWinLose;	// W, L
std::string sFiredPrc;
std::string sArtcCd;
std::string sStkCd;
BOOL	bMain;
};
*/
long __stdcall CFBIMainProc::CallBackSMOrd(int index, char* pCustomPtr, char* Message)
{
	CFBIMainProc* pThis = (CFBIMainProc*)pCustomPtr;
	if (strncmp(Message, MSGR_GW_ORD_NEW, strlen(Message)) != 0 &&
		strncmp(Message, MSG_ORD_FX_CXL, strlen(Message)) != 0
		)
	{
		printf("return(%s)\n", Message);
		return -1;
	}

	char pRecvData[_FBI::MEM_BLOCK_SIZE] = { 0, };
	char cOrdStatus	[32] = { 0, };
	char cUpDn		[32] = { 0, };
	char zSymbol	[32] = { 0, };
	char zUserId	[32] = { 0, };
	char zType		[32] = { 0, };
	char zSMMessage	[32] = { 0, };
	char zDealTp[32] = { 0, };
	strcpy(zSMMessage, Message);

	CStkOrdManager* pStkThread;

	_FBI::ST_SLORD *pOrd = (_FBI::ST_SLORD *)pRecvData;

	try
	{
		ASSERT_BOOL(pThis->m_smOrd->GetStringFieldValue((char*)fldOrdType, 0, zDealTp), pThis->m_smOrd->GetMsg());
		if (atoi(zDealTp) != _FBI::DEAL_TP_SL) {
			printf("Not sl(%s)\n", zDealTp);
			return -1;
		}

		ASSERT_BOOL(pThis->m_smOrd->GetStringFieldValue((char*)fldSymbol, 0, zSymbol), pThis->m_smOrd->GetMsg());
		pOrd->sStkCd = string(zSymbol);

		ASSERT_BOOL(pThis->m_smOrd->GetStringFieldValue((char*)fldHDUserID, 0, zUserId), pThis->m_smOrd->GetMsg());
		pOrd->sUserId = string(zUserId);
		
		
		map<string, CStkOrdManager*>::iterator it = pThis->m_mapStkOrd.find(pOrd->sStkCd);
		if (it == pThis->m_mapStkOrd.end())
		{
			throw CIRExcept("OrdData with non-related symbol");
		}
		pStkThread = ((*it).second);

		ASSERT_BOOL(pThis->m_smOrd->GetIntegerFieldValue((char*)fldOrderID, 0, &pOrd->OrdNo), pThis->m_smOrd->GetMsg());

		ASSERT_BOOL(pThis->m_smOrd->GetStringFieldValue((char*)fldOrdType, 1, cOrdStatus), pThis->m_smOrd->GetMsg());
		if (strncmp(zSMMessage, MSGR_GW_ORD_NEW, strlen(zSMMessage))==0)	//fvPlcType_NEW
		{
			pOrd->nOrdStatus = _FBI::ORD_STATUS_ORD;
			strcpy(zType, "NEW ORD");
		}
		if (strncmp(zSMMessage, MSG_ORD_FX_CXL, strlen(zSMMessage))==0)	//fvPlcType_CXL
		{
			pOrd->nOrdStatus = _FBI::ORD_STATUS_CNCL;
			strcpy(zType, "CNCL ORD");
		}
		ASSERT_BOOL(pThis->m_smOrd->GetDoubleFieldValue((char*)fldPrice, 0, &pOrd->dOrdPrc), pThis->m_smOrd->GetMsg());

		ASSERT_BOOL(pThis->m_smOrd->GetStringFieldValue((char*)fldSide, 2, cUpDn), pThis->m_smOrd->GetMsg());
		if (atoi(cUpDn) ==1)	//sell
			pOrd->cUpDn = 'D';
		else if(atoi(cUpDn) == 2)	//BUY
			pOrd->cUpDn = 'U';
		else {
			g_log.log(ERR, "SIDE Error(Side:%c)", pOrd->cUpDn);
		}
		ASSERT_BOOL(pThis->m_smOrd->GetIntegerFieldValue((char*)fldSTOPLossTick, 0, &pOrd->nTickCnt), pThis->m_smOrd->GetMsg());

		g_log.log(INFO, "---------------------------------------------------------");
		g_log.log(INFO, "[RECV ORD-%s](UserID:%s)(OrdNo:%d)(OrdPrc:%f)(UpDn:%c)(TickCnd:%d)",
			zType, pOrd->sUserId.c_str(), pOrd->OrdNo, pOrd->dOrdPrc, pOrd->cUpDn, pOrd->nTickCnt);
		pStkThread->RelayOrdAndPrc(_FBI::WM_ORD_RECV, pRecvData, sizeof(_FBI::ST_SLORD));
	}
	catch (CIRExcept e)
	{
		g_log.log(ERR, "%s", e.GetMsg());
		return -1;
	}

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

				sprintf(zQ, "EXEC AA_ORD_RESULT_SL %d, '%s','%c'"
					, pSlOrd->OrdNo
					, pSlOrd->sFiredPrc.c_str()
					, pSlOrd->cWinLose
				);

				g_log.log(INFO, "[DB QRY](%s)", zQ);

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
					
					pThis->SendToClient(pSlOrd);
				}

				g_memPool.release((char*)pSlOrd);
				db->Close();
			} //if (msg.message == _FBI::WM_ORD_FIRED)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}

VOID CFBIMainProc::SendToClient(_FBI::ST_SLORD* pOrdInfo)
{
	int nRet = 0;
	char z[128];
	SYSTEMTIME st;	char time[32];
	GetLocalTime(&st); sprintf(time, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	
	try
	{
		ASSERT_BOOL(m_smOrd->SetHeaderOfSMessage(Delivery_Push, (char*)EXEC_REAL_GW, (char*)MSGR_DEAL_STATUS_SL, 0), m_smOrd->GetMsg());

		//sprintf(z, "%.*s", sizeof(pOrdInfo->sUserId))

		ASSERT_BOOL(m_smOrd->SetStringFieldValue((char*)fldHDUserID, (char*)pOrdInfo->sUserId.c_str()), m_smOrd->GetMsg());
		ASSERT_BOOL(m_smOrd->SetStringFieldValue((char*)fldSymbol, (char*)pOrdInfo->sStkCd.c_str()), m_smOrd->GetMsg());

		sprintf(z, "%d", pOrdInfo->OrdNo);
		ASSERT_BOOL(m_smOrd->SetStringFieldValue((char*)fldOrderID, z), m_smOrd->GetMsg());

		sprintf(z, "%c", pOrdInfo->cWinLose);
		ASSERT_BOOL(m_smOrd->SetStringFieldValue((char*)fldRealOrdFlag, z), m_smOrd->GetMsg());

		ASSERT_BOOL(m_smOrd->SetStringFieldValue((char*)fldFxCloseTime, time), m_smOrd->GetMsg());
		

		ASSERT_BOOL(m_smOrd->SendData(), m_smOrd->GetMsg());
	}
	catch (CIRExcept e)
	{
		g_log.log(ERR, "[SendToClient]SM Error(%s)", e.GetMsg());
		return ;
	}
	g_log.log(INFO, "[SendToClient][%s][%s](ID:%s)(OrdNo:%d)(WinLose:%c)",
		EXEC_REAL_GW, MSGR_DEAL_STATUS_SL, pOrdInfo->sUserId.c_str(), pOrdInfo->OrdNo, pOrdInfo->cWinLose);
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
	std::map<std::string, CStkOrdManager*>::iterator it;
	EnterCriticalSection(&m_csStkOrd);
	for (it = m_mapStkOrd.begin(); it != m_mapStkOrd.end(); ++it)
	{
		CStkOrdManager* s = (*it).second;
		//TODO. delete s;
	}
	m_mapStkOrd.clear();
	LeaveCriticalSection(&m_csStkOrd);

}
