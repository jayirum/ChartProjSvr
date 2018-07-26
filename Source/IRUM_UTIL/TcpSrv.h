// TcpSocket.h: interface for the CTcpSrv class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#include <stdio.h>
#include <list>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define SOCKBUF_SIZE	1024

typedef enum _EN_RECV_RET
{
	RECV_RET_ERROR		= -1
	,RECV_RET_TIMEOUT	= 0
	,RECV_RET_OK		= 1
	,RECV_RET_CLOSE		= 2
}EN_RECV_RET;


enum NETEVENT_TP{ NETEVENT_NONE=0, NETEVENT_ACCEPT, NETEVENT_READ, NETEVENT_CLOSE, NETEVENT_READCLOSE, NETEVENT_TIMEOUT, NETEVENT_ERROR};

//#define MAX_CLIENT	WSA_MAXIMUM_WAIT_EVENTS-1
#define ACCEPT_TIMEOUT	10
#define WSABUF_SIZE	1024
#define THREAD_CNT	2
enum {IDX_ACPT, IDX_RECV, IDX_SEND};

typedef struct _SOCKET_INFORMATION {
	WSAOVERLAPPED Overlapped;
	WSABUF wsaBuf;
	CHAR Buffer[WSABUF_SIZE];
	SOCKET Socket;
	DWORD	SendRecvBit;	// IDX_RECV, IDX_SEND
	DWORD BytesSEND;
	DWORD BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

class CTcpSrv  
{
public:
	CTcpSrv();
	virtual ~CTcpSrv();

	int		Begin(int Port, char* IP, BOOL bWinsockStart = TRUE);	// 서버용 소켓 초기화
	VOID	End();
	int		SetSockErrMsg(const char* pzMsg);
	//INT		CheckAccept();
	BOOL	OnAccept();
	VOID	CloseClient(int nEventIdx); 
	BOOL 	SendData(int nIdx, char* psData, int nDataLen);
	BOOL 	SendDataAllClient(char* psData, int nDataLen);
	BOOL	RequestAcept();
	//INT		RecvData(_In_ int nIndex, int nBufLen, _Out_ char* psRecvBuf);
	LPSTR	GetMsg() { return m_zMsg; };

private:
	static unsigned WINAPI AcptThread(LPVOID lp);
	unsigned AcptThreadFn();

private:
	
	// 에러 메시지 버퍼 
	char	m_zMsg[1024];
	SOCKET			m_sockListen;
	SOCKET			m_sockAcpted;
	//SOCKET			m_sockClient[MAX_CLIENT];

	WSAEVENT		m_hWsaEvents[WSA_MAXIMUM_WAIT_EVENTS];	//accept event 포함
	//WSAEVENT		*m_pWsaEvents;
	LPSOCKET_INFORMATION m_pArr[WSA_MAXIMUM_WAIT_EVENTS];
	//LPSOCKET_INFORMATION SI;
	WSAOVERLAPPED	m_ovAccept;


	char			m_sAcceptBuf[(sizeof(SOCKADDR_IN) + 16) * 2];
	int				m_nEventCnt;
	int		m_nPort;
	char	m_zIP[16];	
	SOCKADDR_IN		m_sock_addr;
	int				m_nlen_addr;
	
	BOOL			m_bWinsockStartUp;	//	winsock library start up 을 했는지 여부
	std::list<int>	m_lstTobeClosed;

	HANDLE		m_hThread[2];
	HANDLE		m_hDie[2];
	unsigned int m_dwThreadID[2];
		
};