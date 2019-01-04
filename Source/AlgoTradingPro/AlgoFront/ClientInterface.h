#pragma once

/*
	1) accept 
		- Do not add in the map before this user succeeds in log on
	2) receive close event
		- mark on bCloseRequested
	3) bCloseRequested && refCnt==0
		=> delete 
*/

#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )


#include <winsock2.h>
#include <windows.h>
#include <map>
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Irum_Common.h"

#include "../Common/Algo_PacketHandler.h"
#include "../Common/Algo_common.h"
#pragma comment( lib , "ws2_32.lib" )

#define BUF_LEN	2048

enum { CTX_DIE = 990, CTX_RQST_INNER_TICK, CTX_RQST_INNER_QUOTE, CTX_RQST_SEND, CTX_RQST_RECV };
enum { CK_TYPE_NORMAL, CK_TYPE_COMMUNICATION };

typedef struct _IO_CONTEXT
{
	WSAOVERLAPPED	overLapped;
	WSABUF			wsaBuf;
	char			buf[BUF_LEN];
	int				context;
}IO_CONTEXT;
#define CONTEXT_SIZE sizeof(IO_CONTEXT)


typedef enum {	CLIENT_TP_INNER	//	Thread which receive market data from ETK. Inner Thread
				, CLIENT_TP_OUTER	//	Clients who want to subscribe data feed
} EN_CLIENT_TP;

typedef enum {STATUS_NON_LOGON, STATUS_LOGGEDON, STATUS_CLOSING} EN_CLIENT_STATUS;
//typedef struct _COMPLETION_KEY
//{
//	SOCKET			sock;
//	EN_CLIENT_TP	clientTp;
//	BOOL			bLoggedOn;
//	std::string		userID;
//} CUser;

class CUser
{
public:
	CUser() {
		refCnt = 0; sockUser = INVALID_SOCKET; status = STATUS_NON_LOGON; clientTp = CLIENT_TP_OUTER;
	}
	void addRef() { InterlockedIncrement(&refCnt); }
	void releaseRef() { InterlockedDecrement(&refCnt); }
	bool hasRefCnt() { return (refCnt > 0); }
	void SetInitData(SOCKET sock, char* psIp) { sockUser = sock; clientIp = std::string(psIp); }
	void markCloseRequested() { status = STATUS_CLOSING; }
	bool isClosing() { return (status == STATUS_CLOSING); }
	bool isLoggedOn() { return (status == STATUS_LOGGEDON); }
	bool isNonLoggedOn() { return (status == STATUS_NON_LOGON); }

public:
	unsigned long	refCnt;
	SOCKET			sockUser;
	EN_CLIENT_STATUS	status;
	std::string		userID;
	std::string		clientIp;
	EN_CLIENT_TP	clientTp;
};


class CClientInterface //: public CMT4Helper
{
public:
	CClientInterface();
	virtual ~CClientInterface();

	BOOL Initialize();
	BOOL InitListen();

	void CloseListenSock();
	void Finalize();
	VOID SendMessageToIocpThread(int Message);

	static unsigned WINAPI ListenThread(LPVOID lp);
	static unsigned WINAPI IocpWorkerThread(LPVOID lp);

	// 내부(Application) 의 데이터 수신을 위해
	BOOL Inner_Init();
	VOID Inner_PostData(char* pData, int nDataLen, BOOL bTick);

	void	RequestSendIO(CUser* pCK, char* pSendBuf, int nSendLen);
	void 	RequestRecvIO(CUser* pCK);

	void OnClose(CUser *pCompletionKey);
	
	void lock(CRITICAL_SECTION* cs){ EnterCriticalSection(cs); }
	void unlock(CRITICAL_SECTION* cs) { LeaveCriticalSection(cs); }

	VOID SendDataFeed(IO_CONTEXT* pCtx);
	VOID SendToClients(char* pSendData, int nDataLen);

	void OnClientRequest(CUser* pCK, IO_CONTEXT* pCtx);
	VOID AfterClientRequest(CUser* pCK, IO_CONTEXT* pCtx);
	VOID DeleteUser(CUser* pCK);
	VOID ExecuteByRequest(CUser* pCK, IO_CONTEXT* pCtx);
	
protected:
	HANDLE	m_hCompletionPort;
	std::map<std::string, CUser*>	m_mapUser;	//userid, userinfo including CK
	std::map<std::string, CUser*>	m_mapRequest;	//userid, userinfo including CK
	CRITICAL_SECTION	m_csUser, m_csRequest;
	CRITICAL_SECTION	m_csRecvData;

	char 	m_zServerIP[32];
	int 	m_nListenPort;
	WSAEVENT	m_hListenEvent;

	DWORD m_dwThreadCount;

	SOCKET	m_sockListen;
	HANDLE	m_hListenThread;
	unsigned int	m_dListenThread;

	BOOL 	m_bRun;
	

	// 내부데이터를 전달받기 위해
	SOCKET	m_innerSock;
	CUser* m_pInnerCKey;

	CPacketHandler	*m_pack;

	char m_zMsg[1024];
};