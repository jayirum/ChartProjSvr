#pragma once

/*
	1) CLIENT to AlgoLB
	2) Server to User
*/


#include "LBInfo.h"
#include "UserInfo.h"
#include "Main.h"
#include <windows.h>
#include <map>
#include "../../IRUM_UTIL/Irum_Common.h"
#include "../Common/AlgoInc.h"
#include "../Common/Algo_PacketHandler.h"
#pragma comment( lib , "ws2_32.lib" )

#define IOCP_THREAD_CNT 10




class CClientInterface //: public CMT4Helper
{
private:
	HANDLE	m_hCompletionPort;
	std::map<std::string, CUserInfo*>	m_mapUser;	//userid, userinfo including CK
	std::map<std::string, CUserInfo*>	m_mapRequest;	//userid, userinfo including CK
	CRITICAL_SECTION				m_csUser, m_csRequest;
	CRITICAL_SECTION				m_csRecvData;

	char 			m_zServerIP[32];
	int 			m_nListenPort;
	WSAEVENT		m_hListenEvent;
	SOCKET			m_sockListen;
	HANDLE			m_hListenThread;
	unsigned int	m_dListenThread;

	DWORD			m_dwThreadCount;
	BOOL 			m_bRun;
	CPacketHandler	*m_pack;

	//CReal Thread ID
	DWORD	m_dwRealThreadId;

	// AlgoLB info
	CLBInfo	*m_lbInfo;

	char m_zMsg[1024];

public:
	CClientInterface(DWORD dwRealThreadId);
	virtual ~CClientInterface();

	BOOL Initialize();
	BOOL InitListen();

	void CloseListenSock();
	void Finalize();
	VOID SendMessageToIocpThread(int Message);

	static unsigned WINAPI ListenThread(LPVOID lp);
	static unsigned WINAPI IocpWorkerThread(LPVOID lp);
	
	void OnClose(void *pCompletionKey);
	void ReConnLB();
	void SendAllUserInfo();
	
	VOID SendToClients(char* pSendData, int nDataLen);

	void OnClientRequest(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	VOID AfterClientRequest(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	VOID DeleteUser(CUserInfo* pCK, BOOL bNeedLockUser, BOOL bNeedLockR);
	
	VOID ExecuteByRequest(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx);

	

	BOOL LoginProc(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	BOOL LoginDB(char* pzUserId, char* pzPwd, const char* pzIpAddress, const char* pzMacAddress, char* pzExtraMsg);
	VOID SubscribeSymbol(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	VOID UnsubscribeSymbol(SOCKET sockUser);
	//VOID TellRealThread(BOOL bAdd, SOCKET sockUser);

	void lock(CRITICAL_SECTION* cs) { EnterCriticalSection(cs); }
	void unlock(CRITICAL_SECTION* cs) { LeaveCriticalSection(cs); }
};