// TcpSrv.cpp : Defines the entry point for the console application.
//
#pragma warning(disable:4996)

#include <WinSock2.h>// # socket functions header file
#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

BOOL	WINAPI ControlHandler(DWORD dwCtrlType);
VOID	DumpErr(char* pzSrc, int nErr);
VOID	Terminate();

SOCKET g_hSockAccept = INVALID_SOCKET;


static unsigned WINAPI Thread_ResultProcByChart(LPVOID lp)
{

	printf("client thread start....\n");
	SOCKET sock = (SOCKET)lp;
	SYSTEMTIME st;
	char time[32];
	char sendbuf[128];
	while (1)
	{
		Sleep(100);
		GetLocalTime(&st);
		sprintf(time, "%02d", st.wSecond);

		if (strncmp(time, "00", 2) == 0)
		{
			sprintf(sendbuf, "TIMER/%02d:%02d:%02d/59", st.wHour, st.wMinute, st.wSecond);
			int len = send(sock, sendbuf, strlen(sendbuf), 0);
			if (len == SOCKET_ERROR)
			{
				printf("send error\n");
				Sleep(10000);
				continue;
			}
			else {
				printf("send(%s)\n", sendbuf);
				Sleep(1000);
			}
		}
	}
	return 0;
}

void main(int argc, char **argv)
{
	//if (argc < 2) {
	//	printf("\n<USEAGE>--------------------------\n");
	//	printf("TCPSrv portnumber\n");
	//	printf("--------------------------------\n");
	//}
	char zPortNumber[32]="10101";
	char szClientIP[128];
	char szRecvBuff[1024];
	char szTemp[32];
	int nPortNumber;
	int nLenWanted, nRecvSize;
	int nLen;
	struct sockaddr_in addrClient;
	struct sockaddr_in addr;

	//strcpy(zPortNumber, argv[1]);
	nPortNumber = atoi(zPortNumber);

	printf("---------------------------------------------------------------\n");
	printf(">>>TCP server start. Port No. is (%s)\n", zPortNumber);
	printf("---------------------------------------------------------------\n\n");

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

	// crtl+c
	SetConsoleCtrlHandler(ControlHandler, TRUE);

	// create socket
	g_hSockAccept = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_hSockAccept == INVALID_SOCKET) {
		DumpErr("socket create", GetLastError());
		Terminate();
	}
	//printf("---------------------------------------------------------------\n");
	printf(">>>TCP Server socket created. Socket Handle : %d\n", g_hSockAccept);
	//printf("---------------------------------------------------------------\n\n");


	// bind	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNumber);				// host to network short
													//addr.sin_addr.s_addr = inet_addr("112.216.247.163");
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// host to network long
	if (SOCKET_ERROR == bind(g_hSockAccept, (struct sockaddr*)&addr, sizeof(addr)))
	{
		DumpErr("bind", GetLastError());
		Terminate();
	}
	printf("---------------------------------------------------------------\n");
	printf(">>>TCP Server socket bind succeeded. Port is %d\n", nPortNumber);
	printf("---------------------------------------------------------------\n\n");


	// listen
	if (SOCKET_ERROR == listen(g_hSockAccept, SOMAXCONN))
	{
		DumpErr("listen", GetLastError());
		Terminate();
	}
	printf(">>>TCP Server socket listen succeeded with SOMAXCONN\n");

	while (TRUE)
	{
		memset(&addrClient, 0x00, sizeof(addrClient));
		nLen = sizeof(addrClient);
		SOCKET sockClient = INVALID_SOCKET;
		sockClient = accept(g_hSockAccept, (struct sockaddr*)&addrClient, &nLen);
		if (INVALID_SOCKET == sockClient) {
			DumpErr("socket create", GetLastError());
			Sleep(10000);
			continue;
		}

		printf("accept ok....\n");

		unsigned int id;
		(HANDLE)_beginthreadex(NULL, 0, &Thread_ResultProcByChart, (LPVOID)sockClient, 0, &id);

	}

	closesocket(g_hSockAccept);
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
	if (g_hSockAccept != INVALID_SOCKET)
		closesocket(g_hSockAccept);
	WSACleanup();
	exit(0);
}


VOID	DumpErr(char* pzSrc, int nErr)
{
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

	printf("[%s:%d]%s", pzSrc, nErr, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
