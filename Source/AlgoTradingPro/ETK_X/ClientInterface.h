#pragma once

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
typedef struct _COMPLETION_KEY
{
	SOCKET			sock;
	EN_CLIENT_TP	clientTp;
	BOOL			bLoggedOn;
	char			UserID[SIZE_USERID + 1];
} COMPLETION_KEY;


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

	void	RequestSendIO(COMPLETION_KEY* pCK, char* pSendBuf, int nSendLen);
	void 	RequestRecvIO(COMPLETION_KEY* pCK);

	void DeleteSocket(COMPLETION_KEY *pCompletionKey);

	void lockCK() { EnterCriticalSection(&m_csCK); }
	void unlockCK() { LeaveCriticalSection(&m_csCK); }

	void lockRecvData() { EnterCriticalSection(&m_csRecvData); }
	void unlockRecvData() { LeaveCriticalSection(&m_csRecvData); }

	VOID SendDataFeed(IO_CONTEXT* pCtx);
	VOID SendToClients(char* pSendData, int nDataLen);

	BOOL CheckLogin(COMPLETION_KEY* pCK, IO_CONTEXT* pCtx);
	
protected:
	HANDLE	m_hCompletionPort;
	std::map<std::string, COMPLETION_KEY*>	m_mapCK;	//socket, ck
	
	char 	m_zServerIP[32];
	int 	m_nListenPort;
	WSAEVENT	m_hListenEvent;

	DWORD m_dwThreadCount;

	SOCKET	m_sockListen;
	HANDLE	m_hListenThread;
	unsigned int	m_dListenThread;

	BOOL 	m_bRun;
	CRITICAL_SECTION	m_csCK;
	CRITICAL_SECTION	m_csRecvData;

	// 내부데이터를 전달받기 위해
	SOCKET	m_innerSock;
	COMPLETION_KEY* m_pInnerCKey;

	CPacketHandler	*m_pack;

	char m_zMsg[1024];
};