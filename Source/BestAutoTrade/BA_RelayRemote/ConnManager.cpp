#include "ConnManager.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"
#include "../Common/BAProtocol.h"
extern CLogMsg		g_log;


CConnManager::CConnManager()
{
	InitializeCriticalSection(&m_csMasters);
	InitializeCriticalSection(&m_csSess);

	CBaseThread::ResumeThread();
}


CConnManager::~CConnManager()
{
	DeleteCriticalSection(&m_csMasters);
	DeleteCriticalSection(&m_csSess);
}


VOID CConnManager::RegUnregMaster(string sUserId, char cTpReg)
{
	char zMessage[128] = { 0, };
	LockMasters();
	LockSess();
	ITMAP_MASTER it = m_mapMasters.find(sUserId);

	if (it == m_mapMasters.end())
	{
		if (cTpReg == TP_REG)
		{
			//TODO.	Get Master LimitCount from DB
			// insert into map
			ST_MASTER_COUNT st; ZeroMemory(&st, sizeof(st));
			m_mapMasters[sUserId]	= st;
			m_mapSess	[sUserId]	= GetTickCount();

			CProtoSet	ProtoSet;

			ProtoSet.Begin();
			ProtoSet.SetVal(FDS_CODE,			CODE_LOGON);
			ProtoSet.SetVal(FDS_COMMAND,		TP_COMMAND);
			ProtoSet.SetVal(FDS_SYS,			"RELAY");
			ProtoSet.SetVal(FDS_TM_HEADER,		Now(zMessage));
			ProtoSet.SetVal(FDS_USERID_MASTER,	sUserId);
			ProtoSet.SetVal(FDS_USERID_MINE,	sUserId);
			ProtoSet.SetVal(FDN_PUBSCOPE_TP,	PUBSCOPE_TP::ONEMASTER_WITH_ID);
			//ProtoSet.SetVal(FDN_ACCNO_MY,		m_nMyAccNo);
			//ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
			//ProtoSet.SetVal(FDS_BROKER,		AccountInfoString(ACCOUNT_COMPANY));

			sprintf(zMessage, "Succeeded in Logon.Master[%s]", sUserId.c_str());
			ProtoSet.SetVal(FDS_LOGONOFF_MSG, zMessage);

			char *pSendBuf = new char[BUF_SIZE];
			int nSendLen = ProtoSet.Complete(pSendBuf);

			m_closeCallback.fn(CLIENT_TP::MASTR, m_closeCallback.lpClassInstancePtr, pSendBuf, nSendLen);

			g_log.log(INFO, "[MasterID:%s]Register Master 1st Time(Total Master:%d)", sUserId.c_str(), m_mapMasters.size());
		}
		if (cTpReg == TP_UNREG)
		{
			g_log.log(ERR, "[MasterID:%s]UnRegister Master but there isn't the same Master ID", sUserId.c_str());
		}
	}

	// This master has been registered already.
	else
	{
		if (cTpReg == TP_REG)
		{
			g_log.log(INFO, "[MasterID:%s]Register Master. The same Master already exists. Do thing", sUserId.c_str());
		}
		if (cTpReg == TP_UNREG)
		{
			// remove from Master List (map)
			CProtoSet	ProtoSet;

			ProtoSet.Begin();
			ProtoSet.SetVal(FDS_CODE,			CODE_LOGOFF);
			ProtoSet.SetVal(FDS_COMMAND,		TP_COMMAND);
			ProtoSet.SetVal(FDS_SYS,			"RELAY");
			ProtoSet.SetVal(FDS_TM_HEADER,		Now(zMessage));
			ProtoSet.SetVal(FDS_USERID_MASTER,	sUserId);
			ProtoSet.SetVal(FDN_PUBSCOPE_TP,	PUBSCOPE_TP::ALLSLAVES_UNDER_ONEMASTER);
			//ProtoSet.SetVal(FDS_USERID_MINE,	m_sMyId);
			//ProtoSet.SetVal(FDN_ACCNO_MY,		m_nMyAccNo);
			//ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
			//ProtoSet.SetVal(FDS_BROKER,		AccountInfoString(ACCOUNT_COMPANY));

			sprintf(zMessage, "Master[%s] is closed. So Every Slaves should be closed", sUserId.c_str());
			ProtoSet.SetVal(FDS_LOGONOFF_MSG, zMessage);

			char *pSendBuf = new char[BUF_SIZE];
			int nSendLen = ProtoSet.Complete(pSendBuf);
			
			m_closeCallback.fn(CLIENT_TP::SLAVE, m_closeCallback.lpClassInstancePtr, pSendBuf, nSendLen);

			m_mapMasters.erase(sUserId);
			m_mapSess.erase(sUserId);
			
			g_log.log(INFO, "[MasterID:%s]UnRegister Master.Publish to All Slaves and remove(Total Master:%d)"
				, sUserId.c_str(), m_mapMasters.size());
		}
	}
	UnLockMasters();
	UnLockSess();
}

BOOL CConnManager::RegUnregSlave(string sMasterId, string sSlaveId, char cTpReg)
{
	char zMessage[128] = { 0, };
	LockMasters();
	LockSess();
	ITMAP_MASTER it = m_mapMasters.find(sMasterId);

	if (it == m_mapMasters.end())
	{
		g_log.log(ERR, "[MasterId:%s][SlaveId:%s]RegUnreg[%c] Slave. But there isn't the MasterId", 
			sMasterId.c_str(), sSlaveId.c_str(), cTpReg);

		UnLockMasters();
		UnLockSess();
		return FALSE;
	}

	ST_MASTER_COUNT st = (*it).second;
	if (cTpReg == TP_REG)
	{
		if (st.unLimit == st.unCurrCnt)
		{
			CProtoSet	ProtoSet;

			ProtoSet.Begin();
			ProtoSet.SetVal(FDS_CODE,			CODE_LOGOFF);
			ProtoSet.SetVal(FDS_COMMAND,		TP_COMMAND);
			ProtoSet.SetVal(FDS_SYS,			"RELAY");
			ProtoSet.SetVal(FDS_TM_HEADER,		Now(zMessage));
			ProtoSet.SetVal(FDS_USERID_MASTER,	sMasterId);
			ProtoSet.SetVal(FDN_PUBSCOPE_TP,	PUBSCOPE_TP::ONESLAVE_WITH_ID);
			ProtoSet.SetVal(FDS_USERID_MINE,	sSlaveId);
			//ProtoSet.SetVal(FDN_ACCNO_MY,		m_nMyAccNo);
			//ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
			//ProtoSet.SetVal(FDS_BROKER,		AccountInfoString(ACCOUNT_COMPANY));

			sprintf(zMessage, "The Max count of the Master[%s] is alread full. Be rejected(%s)", sMasterId.c_str(), sSlaveId.c_str());
			ProtoSet.SetVal(FDS_LOGONOFF_MSG, zMessage);

			char *pSendBuf = new char[BUF_SIZE];
			int nSendLen = ProtoSet.Complete(pSendBuf);

			m_closeCallback.fn(CLIENT_TP::SLAVE, m_closeCallback.lpClassInstancePtr, pSendBuf, nSendLen);
			g_log.log(INFO, zMessage);
		}
		else
		{
			st.unCurrCnt++;
			CProtoSet	ProtoSet;

			ProtoSet.Begin();
			ProtoSet.SetVal(FDS_CODE,			CODE_LOGON);
			ProtoSet.SetVal(FDS_COMMAND,		TP_COMMAND);
			ProtoSet.SetVal(FDS_SYS,			"RELAY");
			ProtoSet.SetVal(FDS_TM_HEADER,		Now(zMessage));
			ProtoSet.SetVal(FDS_USERID_MASTER,	sMasterId);
			ProtoSet.SetVal(FDS_USERID_MINE,	sSlaveId);
			ProtoSet.SetVal(FDN_PUBSCOPE_TP,	PUBSCOPE_TP::ONESLAVE_WITH_ID);
			//ProtoSet.SetVal(FDN_ACCNO_MY,		m_nMyAccNo);
			//ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
			//ProtoSet.SetVal(FDS_BROKER,		AccountInfoString(ACCOUNT_COMPANY));

			sprintf(zMessage, "Succeeded in Logon.Slave[%s]", sMasterId.c_str());
			ProtoSet.SetVal(FDS_LOGONOFF_MSG, zMessage);

			char *pSendBuf = new char[BUF_SIZE];
			int nSendLen = ProtoSet.Complete(pSendBuf);

			m_closeCallback.fn(CLIENT_TP::SLAVE, m_closeCallback.lpClassInstancePtr, pSendBuf, nSendLen);

			m_mapMasters[(*it).first] = st;
			m_mapSess[sSlaveId] = GetTickCount();
			g_log.log(INFO, "[MasterId:%s][SlaveId:%s]Register Slave.Increase SlaveCnt(%d)", sMasterId.c_str(), sSlaveId.c_str(), st.unCurrCnt);
		}

	} // if (cTpReg == TP_REG)
	if (cTpReg == TP_UNREG)
	{
		if (--st.unCurrCnt<0)	st.unCurrCnt = 0;
		m_mapMasters[(*it).first] = st;
		m_mapSess.erase(sSlaveId);
		g_log.log(INFO, "[MasterId:%s][SlaveId:%s] UnRegister Slave.Decrease SlaveCnt(%d)", sMasterId.c_str(), sSlaveId.c_str(), st.unCurrCnt);
	}
	UnLockMasters();
	UnLockSess();

	return TRUE;
}

void CConnManager::ThreadFunc()
{
	while (!Is_TimeOfStop(500))
	{
		// check the last ping tick.

		// if timeout
		/*m_closeCallback.fn(m_closeCallback.lpClassInstancePtr, CLOSING_TP::ALL_SLAVE, sUserId.c_str());
		m_mapMasters.erase(sUserId);
		m_mapSess.erase(sUserId);*/
	}
}

VOID CConnManager::UpdatePing(string sUserId)
{
	LockSess();
	ITMAP_SESSION it = m_mapSess.find(sUserId);
	if (it == m_mapSess.end())
	{
		g_log.log(ERR, "Even receive ping, but there is no ID(%s) in the map", sUserId.c_str());
	}
	else
	{
		m_mapSess[(*it).first] = GetTickCount();
	}
	UnLockSess();
}

void CConnManager::RegCallBackForCloseSess(LPVOID lpClassInstancePtr, PResultCallBack lpCallBack)
{
	m_closeCallback.lpClassInstancePtr = lpClassInstancePtr;
	m_closeCallback.fn = lpCallBack;
}