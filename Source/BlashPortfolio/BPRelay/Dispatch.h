/*
	- receive data from Master / Slave

	- Use only 1 thread

	- Dispatch to CMasterSlave thread
*/

#pragma once

#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )


#include <winsock2.h>
#include "MasterSlave.h"
#include <windows.h>
#include <map>
#include <string>
#include "inc.h"

using namespace std;

#define BUF_LEN	2048
#define WORKTHREAD_CNT	1


typedef struct _IO_CONTEXT
{
	WSAOVERLAPPED	overLapped;
	WSABUF			wsaBuf;
	char			buf[BUF_LEN];
	int				context;
}IO_CONTEXT;
#define CONTEXT_SIZE sizeof(IO_CONTEXT)


typedef struct _COMPLETION_KEY
{
	SOCKET		sock;
	// 저장하기를 원하는 데이터를 자유로이 추가한다.
	char* 		pData;
	int		Message;
} COMPLETION_KEY;


class CDispatch 
{
public:
	CDispatch();
	virtual ~CDispatch();

	BOOL Initialize();
	void Finalize();

private:
	BOOL ReadIPPOrt();
	BOOL InitListen();
	void CloseListenSock();
	VOID SendMessageToIocpThread(int Message);

	static unsigned WINAPI ListenThread(LPVOID lp);
	static unsigned WINAPI IocpWorkerThread(LPVOID lp);

	void	RequestSendIO(COMPLETION_KEY* pCK, char* pSendBuf, int nSendLen);
	void 	RequestRecvIO(COMPLETION_KEY* pCK);

	void DeleteSocket(COMPLETION_KEY *pCompletionKey);

	void lockCK() { EnterCriticalSection(&m_csCK); }
	void unlockCK() { LeaveCriticalSection(&m_csCK); }

	void lockRecvData() { EnterCriticalSection(&m_csRecvData); }
	void unlockRecvData() { LeaveCriticalSection(&m_csRecvData); }

	
private:
	HANDLE			m_hCompletionPort;
	SOCKET			m_sockListen;
	HANDLE			m_hListenThread;
	unsigned int	m_dListenThread;
	char 			m_zListenIP[32];
	int 			m_nListenPort;
	WSAEVENT		m_hListenEvent;

	std::map<std::string, COMPLETION_KEY*>	m_mapCK;	//socket, ck

	BOOL 	m_bRun;
	CRITICAL_SECTION	m_csCK;
	CRITICAL_SECTION	m_csRecvData;


	char m_zMsg[1024];

private:

	map<string, CMasterSlave*>	m_mapMaster;
};