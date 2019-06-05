#pragma once


#include "../../IRUM_UTIL/BaseThread.h"
#include <map>
#include <string>
#include <list>
#include "../Common/BARelayInc.h"

using namespace _BA_RELAY;
using namespace std;

struct ST_MASTER_COUNT
{
	UINT	unLimit;
	UINT	unCurrCnt;
};

typedef map<string, ST_MASTER_COUNT>				MAP_MASTER;
typedef map<string, ST_MASTER_COUNT>::iterator		ITMAP_MASTER;

typedef map<string, UINT>					MAP_SESSION;
typedef map<string, UINT>::iterator			ITMAP_SESSION;


// class instance pointer, type, closing ID, message

typedef  void(__stdcall *PResultCallBack)(
	CLIENT_TP,		//	{ MASTR=0, SLAVE};
	void*,			//	pClassInstance:class instance pointer
	const char*,	//	pPacket
	int				//	packet len
	);

struct SESS_TOBE_CLOSED
{
	PResultCallBack	fn;
	LPVOID			lpClassInstancePtr;
};

class CConnManager : public CBaseThread
{
public:
	CConnManager();
	~CConnManager();

	VOID RegUnregMaster(string sUserId, char cTpReg);
	BOOL RegUnregSlave(string sMasterId, string sSlaveId, char cTpReg);
	VOID UpdatePing(string sUserId);

	void ThreadFunc();
	void RegCallBackForCloseSess(LPVOID lpClassInstancePtr, PResultCallBack lpCallBack);

private:
	inline void LockMasters() { EnterCriticalSection(&m_csMasters); }
	inline void UnLockMasters() { LeaveCriticalSection(&m_csMasters); }
	inline void LockSess() { EnterCriticalSection(&m_csSess); }
	inline void UnLockSess() { LeaveCriticalSection(&m_csSess); }
private:
	MAP_MASTER			m_mapMasters;	//	master id, slave count
	MAP_SESSION			m_mapSess;		//	master/slave id, last tickcount

	CRITICAL_SECTION	m_csMasters;
	CRITICAL_SECTION	m_csSess;

	SESS_TOBE_CLOSED	m_closeCallback;
};

