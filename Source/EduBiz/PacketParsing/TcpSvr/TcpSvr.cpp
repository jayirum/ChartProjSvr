// TcpSrv.cpp : Defines the entry point for the console application.
//
#pragma warning(disable:4996)

#include <WinSock2.h>// # socket functions header file
#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include "../Common/LogMsg.h"
#include "../Common/utilfunc.h"

#pragma comment(lib, "ws2_32.lib")

BOOL	WINAPI ControlHandler(DWORD dwCtrlType);
VOID	Terminate();
#define SERVER_BUFFER 513

CLogMsg g_log;
SOCKET g_sockAccept = INVALID_SOCKET;	// socket for accept only.

void main(int argc, char **argv)
{
	if (argc < 2) {
		printf("\n<USEAGE>--------------------------\n");
		printf("TCPSrv portnumber\n");
		printf("--------------------------------\n");
	}


	g_log.OpenLog("TcpServer");

	// crtl+c
	SetConsoleCtrlHandler(ControlHandler, TRUE);

	
	char szClientIP[128];
	char szRecvBuff[SERVER_BUFFER];
	int nPortNumber;
	int nRecvSize;
	int nLen;

	struct sockaddr_in addrClient;
	struct sockaddr_in addr;

	nPortNumber = atoi(argv[1]);
	printf(">>>TCP simple server start. Port No. is (%d)\n", nPortNumber);

	WSADATA wsaData;
	BYTE nMajor = 2, nMinor = 2;
	if (WSAStartup(MAKEWORD(nMajor, nMinor), &wsaData) != 0) {
		DumpErr("WSAStartup", GetLastError());
		return;
	}

	if (LOBYTE(wsaData.wVersion) != nMajor ||
		HIBYTE(wsaData.wVersion) != nMinor) {
		WSACleanup();
		return;
	}


	// create socket
	g_sockAccept = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_sockAccept == INVALID_SOCKET) {
		DumpErr("socket create", GetLastError());
		Terminate();
	}
	printf(">>>TCP Server socket created. Socket Handle : %d\n", g_sockAccept);

	// bind	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNumber);				// host to network short
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// host to network long
	if (SOCKET_ERROR == bind(g_sockAccept, (struct sockaddr*)&addr, sizeof(addr)))
	{
		DumpErr("bind", GetLastError());
		Terminate();
	}
	printf(">>>TCP Server socket bind succeeded. Port is %d\n", nPortNumber);


	// listen
	if (SOCKET_ERROR == listen(g_sockAccept, SOMAXCONN))
	{
		DumpErr("listen", GetLastError());
		Terminate();
	}
	printf(">>>TCP Server socket listen succeeded with SOMAXCONN\n");

	int nLoop = 0;
	while (TRUE)
	{
		memset(&addrClient, 0x00, sizeof(addrClient));
		nLen = sizeof(addrClient);

		// new socket dedicated to the client.
		SOCKET sockClient = INVALID_SOCKET;
		sockClient = accept(g_sockAccept, (struct sockaddr*)&addrClient, &nLen);
		if (INVALID_SOCKET == sockClient) {
			DumpErr("socket create", GetLastError());
			Terminate();
		}

		sprintf(szClientIP, inet_ntoa(addrClient.sin_addr));
		printf(">>>accept succeeded. new socket is %d, client ip is %s\n\n", sockClient, szClientIP);

		nLoop = 0;
		while (TRUE)
		{
			memset(szRecvBuff, 0x00, sizeof(szRecvBuff));
			nRecvSize = recv(sockClient, szRecvBuff, sizeof(szRecvBuff), 0);
			if (SOCKET_ERROR == nRecvSize)
			{
				DumpErr("recv", GetLastError());
				closesocket(sockClient);
				break;
			}
			else
			{
				nLoop++;
				g_log.log(LOGTP_INFO, "<%d>[%d](%.*s)", nLoop, nRecvSize, nRecvSize,szRecvBuff);// , nRecvSize, szRecvBuff);

				if (nRecvSize != 316)
				{
					printf("Unexpected!!! [RecvLen:%d] [RecvCount:%d]\n", nRecvSize, nLoop);
				}
				Sleep(1);
			}
		}
		closesocket(sockClient);
	}

	
	closesocket(g_sockAccept);
	return;
}




BOOL WINAPI ControlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate  
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode  
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		printf("Stopping ...\n");
		return FALSE;

	}
	return FALSE;
}

VOID	Terminate()
{
	if (g_sockAccept != INVALID_SOCKET)
		closesocket(g_sockAccept);
	WSACleanup();
	exit(0);
}
