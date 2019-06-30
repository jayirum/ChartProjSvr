#pragma once

/*
	1) CLIENT to AlgoLB
	2) Server to User
*/

#include "CompletionKey.h"
#include <windows.h>
#include <map>
#include "../../IRUM_UTIL/Irum_Common.h"
#include "../Common/AlgoInc.h"
#include "../Common/Algo_PacketHandler.h"
#include "SessionManager.h"
#pragma comment( lib , "ws2_32.lib" )

#define IOCP_THREAD_CNT 10




class CClientInterface //: public CMT4Helper
{
private:
	HANDLE	m_hCompletionPort;
	std::map<std::string, CCompletionKey*>	m_mapUser;	//userid, userinfo including CK
	std::map<std::string, CCompletionKey*>	m_mapRequest;	//userid, userinfo including CK
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

	CSessionManager	*m_ses;

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
	
	void CloseUser(CCompletionKey* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	void OnClose(void *pCompletionKey);
	void SendAllUserInfo();
	
	VOID SendToClients(char* pSendData, int nDataLen);

	void OnClientRequest(CCompletionKey* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	VOID AfterClientRequest(CCompletionKey* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	VOID DeleteUser(CCompletionKey* pCK, BOOL bNeedLockUser, BOOL bNeedLockR);
	
	VOID ExecuteByRequest(CCompletionKey* pCK, _ALGOINC::IO_CONTEXT* pCtx);



	VOID FrontDisconnect(CCompletionKey* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	BOOL FrontConnect();

	// front tells these 3 action
	VOID FrontTell_ClientLogon(char* pzFrontName, char* pzClientId);
	VOID FrontTell_ClientLogoff(char* pzFrontName, char* pzClientId);

	// once client send a correct credential, send back the front ip, port
	BOOL ClientLogonProc();
	BOOL LoginProc(CCompletionKey* pCK, _ALGOINC::IO_CONTEXT* pCtx);
	BOOL LoginDB(char* pzUserId, char* pzPwd, const char* pzIpAddress, const char* pzMacAddress, char* pzExtraMsg);

	// if the client connections excess limit, force to log off the oldest session
	VOID TellToFront_CloseClientSession();


	void lock(CRITICAL_SECTION* cs) { EnterCriticalSection(cs); }
	void unlock(CRITICAL_SECTION* cs) { LeaveCriticalSection(cs); }
};