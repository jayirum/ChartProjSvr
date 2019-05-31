// FBI_DealManager.cpp : Defines the entry point for the console application.
//

#include "FBIMainProc.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/IRExcept.h"
#include "../common/FBIInc.h"

extern BOOL		g_bContinue ;	// flag whether continue process or not
extern CLogMsg	g_log;
extern char		g_zConfig[_MAX_PATH];
extern CMemPool	g_memPool;

CFBIMainProc::CFBIMainProc()
{
	m_pDBPool = NULL;
	
	InitializeCriticalSection(&m_csDM);
	InitializeCriticalSection(&m_csDMTenOp);
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

	if (!InitializeSM())
		return FALSE;

	// thread for sending deal result to client
	m_hToClient = (HANDLE)_beginthreadex(NULL, 0, &SendToClientThread, this, 0, &m_unToClient);
	
	if (!LoadStkCode()) {
		g_log.log(ERR/*NOTIFY*/, "LoadStkCode Error. Terminate Application");
		return FALSE;
	}

	return TRUE;
}


BOOL CFBIMainProc::InitializeSM()
{
	char zIp1[32], zPort1[32];
	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "SMSERVER_IP", zIp1);
	CUtil::GetConfig(g_zConfig, "SMSERVER_INFO", "SMSERVER_PORT", zPort1);

	if (!m_smClient.Begin())
	{
		g_log.log(NOTIFY, "SM Server Begin Error. Terminate Application");
		return FALSE;
	}
	if (!m_smClient.ConnectSMSrv(zIp1, atoi(zPort1)))
	{
		g_log.log(NOTIFY, "SM connect Error(IP:%s)(Port:%s) Terminate Application", zIp1, zPort1);
		return FALSE;
	}

	return TRUE;
}

void CFBIMainProc::Finalize()
{
	ClearDealMap();

	SAFE_DELETE(m_pDBPool);
	DeleteCriticalSection(&m_csDM);
	DeleteCriticalSection(&m_csDMTenOp);
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

			//Sleep(100);
			char zStCd[128];	
			char zArtcCd[128];
			
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

			//TODO
			/*if (strncmp(zArtcCd, "CL", 2) != 0) {
				db->Next();
				continue;
			}*/
			//////////////////////////////////////////////////////////////////////////////

			//TODO
			CDealManager* p = new CDealManager(zStCd, zArtcCd, nIdx, m_unToClient);
			if (!p->Initialize()) {
				CDBHandlerAdo db2(m_pDBPool->Get());
				sprintf(zQ, "EXEC AA_LOG_PROCESS_STATUS 'DealManager', '[%s] 초기화 실패.프로세스종료.' ", zStCd);
				if (FALSE == db2->ExecQuery(zQ))
				{
					g_log.log(ERR/*NOTIFY*/, "AA_LOG_PROCESS_STATUS Error(%s)", zQ);
				}

				g_log.log(NOTIFY, "[%s] Initialize failed", zStCd);
				delete p;
				exit(0);
				return FALSE;
			}

			EnterCriticalSection(&m_csDM);
			m_mapDealManager[zStCd] = p;
			LeaveCriticalSection(&m_csDM);

			//////////////////////////////////////////////////////////////////////////////
			//TODO
			CDealManagerTenOp* pTenOp = new CDealManagerTenOp(zStCd, zArtcCd, nIdx, m_unToClient);
			if (!pTenOp->Initialize()) 
			{
				CDBHandlerAdo db3(m_pDBPool->Get());
				sprintf(zQ, "EXEC AA_LOG_PROCESS_STATUS 'DealManager', '[%s] 초기화 실패.프로세스종료(TENOP).' ", zStCd);
				if (FALSE == db3->ExecQuery(zQ))
				{
					g_log.log(ERR/*NOTIFY*/, "AA_LOG_PROCESS_STATUS Error(%s)", zQ);
				}

				g_log.log(NOTIFY, "[%s] Initialize failed", zStCd);
				delete pTenOp;
				exit(0);
				return FALSE;
			}

			EnterCriticalSection(&m_csDMTenOp);
			m_mapDealManagerTenOp[zStCd] = pTenOp;
			LeaveCriticalSection(&m_csDMTenOp);

			
			g_log.log(INFO, "LoadSymbol(%s)", zStCd);
			printf("LoadSymbol(%s)\n", zStCd);

			CDBHandlerAdo db2(m_pDBPool->Get());
			sprintf(zQ, "EXEC AA_LOG_PROCESS_STATUS 'DealManager', '[%s] 초기화 성공.' ", zStCd);
			if (FALSE == db2->ExecQuery(zQ))
			{
				g_log.log(ERR/*NOTIFY*/, "AA_LOG_PROCESS_STATUS Error(%s)", zQ);
			}
			db2->Close();
			
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
	std::map<std::string, CDealManager*>::iterator it;
	EnterCriticalSection(&m_csDM);
	for (it = m_mapDealManager.begin(); it != m_mapDealManager.end(); ++it)
	{
		CDealManager* s = (*it).second;
		delete s;
	}
	m_mapDealManager.clear();
	LeaveCriticalSection(&m_csDM);

	std::map<std::string, CDealManagerTenOp*>::iterator itTenOp;
	EnterCriticalSection(&m_csDMTenOp);
	for (itTenOp = m_mapDealManagerTenOp.begin(); itTenOp != m_mapDealManagerTenOp.end(); ++itTenOp)
	{
		CDealManagerTenOp* s = (*itTenOp).second;
		delete s;
	}
	m_mapDealManagerTenOp.clear();
	LeaveCriticalSection(&m_csDMTenOp);
}

unsigned WINAPI CFBIMainProc::SendToClientThread(LPVOID lp)
{
	CFBIMainProc* pThis = (CFBIMainProc*)lp;
	char SMHeaderMsg[32];
	//char z[128];
	char zSymbol[128];
	char zDealSeq[128];
	char zDealTp[128];
	char zDealStatus[128];
	char zOrdResult[128];
	char zTime[128];
	char zCandleTM[128];
	char zPrc[128];
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == _FBI::WM_RESULT_START)
		{
			_FBI::PT_DEAL_STATUS* p = (_FBI::PT_DEAL_STATUS*)msg.lParam;

			try
			{
				if (p->DealTp[0] == _FBI::DEAL_TP_ONE_OP_C)	strcpy(SMHeaderMsg, MSGR_DEAL_STATUS);
				else if (p->DealTp[0] == _FBI::DEAL_TP_TEN_OP_C)	strcpy(SMHeaderMsg, MSGR_DEAL_STATUS_TENOP);
				else {
					g_log.log(ERR, "[SendToClientThread]DealTp Error(%c)", p->DealTp[0]);
				}
						
				ASSERT_BOOL(pThis->m_smClient.SetHeaderOfSMessage(Delivery_Push, (char*)EXEC_REAL_GW, SMHeaderMsg, 0), pThis->m_smClient.GetMsg());

				sprintf(zSymbol, "%.*s", sizeof(p->ArtcCd), p->ArtcCd);
				CUtil::RTrim(zSymbol, strlen(zSymbol));
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldSymbol, zSymbol), pThis->m_smClient.GetMsg());
				
				sprintf(zDealSeq, "%.*s", sizeof(p->DealSeq), p->DealSeq);
				CUtil::RTrim(zDealSeq, strlen(zDealSeq));
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldReqSeq, zDealSeq), pThis->m_smClient.GetMsg());

				sprintf(zDealTp, "%.*s", sizeof(p->DealTp), p->DealTp);
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldGoodsType, zDealTp), pThis->m_smClient.GetMsg());
				
				sprintf(zDealStatus, "%.*s", sizeof(p->DealStatus), p->DealStatus);
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldRecStatus, zDealStatus), pThis->m_smClient.GetMsg());

				sprintf(zOrdResult, "%.*s", sizeof(p->OrdResult), p->OrdResult);
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldRealOrdFlag, zOrdResult), pThis->m_smClient.GetMsg());
				
				sprintf(zTime, "%.*s", sizeof(p->Time), p->Time);
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldFxCloseTime, zTime), pThis->m_smClient.GetMsg());

				sprintf(zCandleTM, "%.*s", sizeof(p->CandleTime), p->CandleTime);
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldCMECloseTime, zCandleTM), pThis->m_smClient.GetMsg());

				sprintf(zPrc, "%.*s", sizeof(p->ClosePrc), p->ClosePrc);
				CUtil::RTrim(zPrc, strlen(zPrc));
				ASSERT_BOOL(pThis->m_smClient.SetStringFieldValue((char*)fldFXExec, zPrc), pThis->m_smClient.GetMsg());


				ASSERT_BOOL(pThis->m_smClient.SendData(), pThis->m_smClient.GetMsg());
			}
			catch (CIRExcept e)
			{
				g_log.log(ERR, "[SendToClient]SM Error(%s)", e.GetMsg());
				g_memPool.release((char*)p);
				continue;
			}
			//g_log.log(INFO, "[SendToClient][%s][%s](Symbol:%s)(DealSeq:%s)(DealTp:%s)(DealStatus:%s)(OrdResult:%s)(CandleTM:%s)",
			//	EXEC_REAL_GW, SMHeaderMsg, zSymbol, zDealSeq, zDealTp, zDealStatus, zOrdResult, zCandleTM);

			g_memPool.release((char*)p);
		} //if (msg.message == _FBI::WM_ORD_FIRED)
	} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))

	return 0;
}