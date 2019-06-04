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
	LockMasters();
	LockSess();
	ITMAP_SESSION it = m_mapMasters.find(sUserId);

	if (it == m_mapMasters.end())
	{
		if (cTpReg == TP_REG)
		{
			// insert into map
			m_mapMasters[sUserId] = 0;
			m_mapSess[sUserId] = GetTickCount();
			g_log.log(INFO, "[MasterID:%s]Register Master 1st Time", sUserId.c_str());
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
			m_closeCallback.fn(m_closeCallback.lpClassInstancePtr, PUBSCOPE_TP::ALLSLAVES_UNDER_ONEMASTER, sUserId.c_str());
			m_mapMasters.erase(sUserId);
			m_mapSess.erase(sUserId);
			g_log.log(INFO, "[MasterID:%s]UnRegister Master.Publish to All Slaves and remove.", sUserId.c_str());
		}
	}
	UnLockMasters();
	UnLockSess();
}

BOOL CConnManager::RegUnregSlave(string sMasterId, string sSlaveId, char cTpReg)
{
	LockMasters();
	LockSess();
	ITMAP_SESSION it = m_mapMasters.find(sMasterId);

	if (it == m_mapMasters.end())
	{
		g_log.log(ERR, "[MasterId:%s][SlaveId:%s]RegUnreg[%c] Slave. But there isn't the MasterId", sMasterId.c_str(), sSlaveId.c_str(), cTpReg);

		UnLockMasters();
		UnLockSess();
		return FALSE;
	}

	UINT cnt = (*it).second;
	if (cTpReg == TP_REG)
	{
		cnt++;
		m_mapMasters[(*it).first] = cnt;
		m_mapSess[sSlaveId] = GetTickCount();
		g_log.log(INFO, "[MasterId:%s][SlaveId:%s]Register Slave.Increase SlaveCnt(%d)", sMasterId.c_str(), sSlaveId.c_str(), cnt);
	}
	if (cTpReg == TP_UNREG)
	{
		cnt--; if (cnt < 0) cnt = 0;
		m_mapMasters[(*it).first] = cnt;
		m_mapSess.erase(sSlaveId);
		g_log.log(INFO, "[MasterId:%s][SlaveId:%s] UnRegister Slave.Decrease SlaveCnt(%d)", sMasterId.c_str(), sSlaveId.c_str(), cnt);

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

void CConnManager::RegCallBackForCloseSess(LPVOID lpClassInstancePtr, PClosingCallBack lpCallBack)
{
	m_closeCallback.lpClassInstancePtr = lpClassInstancePtr;
	m_closeCallback.fn = lpCallBack;
}