#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BLASHPF_API_EXPORTS
#include "TcpClientDll.h"

int DEF_SEND_RETRY = 3;

struct SOCK_INFO
{
	SOCKET		sock;
	WSAEVENT	hwsa;
	bool		bConn;
	char		msg[BUF_SIZE];
	char		zServerIP[128];
	int			nServerPort;
	int			nRecvTimeout;
	int			nSendTimeout;
	SOCKADDR_IN		sin;
};
SOCK_INFO	g_sock;

int BPAPI_Init(char* pzServerIp, int nServerPort, int nRecvTimeout, int nSendTimeout)
{
	ZeroMemory(&g_sock, sizeof(g_sock));
	strcpy(g_sock.zServerIP, pzServerIp);
	g_sock.nServerPort = nServerPort;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		sprintf(g_sock.msg, "WSAStartup error:%d", GetLastError());
		return -1;
	}
	
	g_sock.sin.sin_family = AF_INET;
	g_sock.sin.sin_port = htons(nServerPort);
	g_sock.sin.sin_addr.s_addr = inet_addr(pzServerIp);

	g_sock.nRecvTimeout = nRecvTimeout;
	g_sock.nSendTimeout = nSendTimeout;

	return CreateSock();
}

int CreateSock()
{
	g_sock.sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock.sock == INVALID_SOCKET)
	{
		DumpErr("create socket");
		return -1;
	}
	
	g_sock.hwsa = WSACreateEvent();
	if (WSAEventSelect(g_sock.sock, g_sock.hwsa, FD_READ | FD_CLOSE))
	{
		DumpErr("WSAEventSelect");
		return -1;
	}


	setsockopt(g_sock.sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&g_sock.nSendTimeout, sizeof(g_sock.nSendTimeout));

	return 1;
}


bool BPAPI_IsConnected()
{
	return g_sock.bConn;
}

int BPAPI_DeInit()
{
	BPAPI_Disconnect();
	return 1;
}

int BPAPI_Disconnect()
{
	if (g_sock.sock != INVALID_SOCKET) {
		struct linger ling;
		ling.l_onoff = 1;   // 0 ? use default, 1 ? use new value
		ling.l_linger = 0;  // close session in this time
		setsockopt(g_sock.sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
		//-We can avoid TIME_WAIT on both of client and server side as we code above.
		closesocket(g_sock.sock);
		g_sock.sock = INVALID_SOCKET;
	}
	if (g_sock.hwsa) {
		WSACloseEvent(g_sock.hwsa);
		g_sock.hwsa = NULL;
	}
	
	g_sock.bConn	= false;
	return 0;
}

int BPAPI_Connect()
{
	if (g_sock.sock == INVALID_SOCKET) {
		if (CreateSock() < 0)
			return -1;
	}


	if (connect(g_sock.sock, (LPSOCKADDR)&g_sock.sin, sizeof(g_sock.sin)) == SOCKET_ERROR)
	{
		g_sock.bConn = FALSE;
		DumpErr("connect");
		BPAPI_Disconnect();
		return -1;
	}

	if (WSAEventSelect(g_sock.sock, g_sock.hwsa, FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		g_sock.bConn = FALSE;
		DumpErr("WSAEventSelect");
		BPAPI_Disconnect();
		return -1;
	}

	sprintf(g_sock.msg, "connect ok(%s)(%d)", g_sock.zServerIP, g_sock.nServerPort);
	g_sock.bConn = true;
	return 1;
}

int BPAPI_RecvData(char* pRecvData, int nBuffSize)
{
	if (!BPAPI_IsConnected()) {
		sprintf(g_sock.msg, "[RecvData]socket is not connected!");
		return -1;
	}

	DWORD dwRet = WSAWaitForMultipleEvents(1, &g_sock.hwsa, FALSE, g_sock.nRecvTimeout, TRUE);
	if (dwRet != WAIT_OBJECT_0)
	{
		if (dwRet == WAIT_TIMEOUT)
		{
			return 0;
		}
		else 
		{
			DumpErr("WSAWaitForMultipleEvents");
			return -1;
		}
	}

	//		LOCK_RECV();

	WSAResetEvent(g_sock.hwsa);

	WSANETWORKEVENTS enumEvent;
	if (WSAEnumNetworkEvents(g_sock.sock, g_sock.hwsa, &enumEvent) == SOCKET_ERROR)
	{
		DumpErr( "WSAEnumNetworkEvents Err");
		return -1;
	}

	//	SMILOR MODIFY 2004-12-08
	if ((enumEvent.lNetworkEvents == FD_CLOSE) ||
		(enumEvent.lNetworkEvents == (FD_CLOSE | FD_READ)))
	{
		sprintf(g_sock.msg, "Receive Close Event");
		return -1;
	}


	ZeroMemory(pRecvData, nBuffSize);

	int nRet = recv(g_sock.sock, pRecvData, nBuffSize, 0);
	if (nRet == SOCKET_ERROR)
	{
		DumpErr("recv");
		return -1;
		//int nErr = GetLastError();
		//if (nErr == WSAEWOULDBLOCK)

	}
	else if (nRet == 0) {
		sprintf(g_sock.msg, "recv 0 byte");
		return -1;
	}

	return nRet;
}
int BPAPI_SendData(char* pSendData, int nSendSize)
{
	if (!BPAPI_IsConnected()) {
		sprintf(g_sock.msg, "[SendData]socket is not connected!");
		return -1;
	}
	int Ret;
	int nRetryCnt = 0, nRetryBlock = 0;

	while (1)
	{
		Ret = send(g_sock.sock, pSendData, nSendSize, 0);
		if (Ret > 0) {
			return Ret;
		}

		//	if(Ret == SOCKET_ERROR)		
		int nErr = GetLastError();
		if (nErr == WSAETIMEDOUT)
		{
			//char tmp[128];
			//SYSTEMTIME st; GetLocalTime(&st);
			//sprintf(tmp, "[%02d:%02d:%02d.%03d] \tWSAETIMEDOUT========\n",
			//	st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			//printf("%s\n", tmp);
			if (++nRetryCnt > DEF_SEND_RETRY)
			{
				sprintf(g_sock.msg, "[SendData]WSAETIMEDOUT %d회 반복으로 에러 리턴", nRetryCnt);
				return -1;
			}
			continue;
		}
		else if (nErr == WSAEWOULDBLOCK)
		{
			if (++nRetryBlock > DEF_SEND_RETRY)
			{
				sprintf(g_sock.msg, "[SendData]WSAEWOULDBLOCK %d회 반복으로 에러 리턴", nRetryBlock);
				return -1;
			}
			continue;
		}
		else
		{
			DumpErr("send");
			return -1;
		}
	}

	return Ret;
}

char* BPAPI_GetMsg()
{
	return g_sock.msg;
}



void DumpErr(char* pSrc)
{
	int nErr = GetLastError();
	LPVOID lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	sprintf(g_sock.msg, "[%s](%d)%s", pSrc, nErr,(char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}