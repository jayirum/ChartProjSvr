// TcpSrv.cpp : Defines the entry point for the console application.
//
#pragma warning(disable:4996)

#include <WinSock2.h>// # socket functions header file
#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include "../Common/LogMsg.h"
#include "TcpParser.h"

#pragma comment(lib, "ws2_32.lib")

BOOL	WINAPI ControlHandler(DWORD dwCtrlType);
VOID	DumpErr(char* pzSrc, int nErr);
VOID	Terminate();

// worker thread function and variables
HANDLE		g_hThread = NULL;
HANDLE		g_hDie = NULL;
unsigned int		g_dwThreadID = 0;
unsigned int __stdcall MainWorkThread(LPVOID lp);	

//socket handle
SOCKET	sockAccept = INVALID_SOCKET;


// parser
CTcpParser	g_Parser;


CLogMsg		g_log;

void main(int argc, char **argv)
{
	if (argc < 2) {
		printf("\n<USEAGE>--------------------------\n");
		printf("TCPSrvEx portnumber\n");
		printf("--------------------------------\n");
	}

	// open log file
	g_log.OpenLog("TcpServerEx");

	// crtl+c
	SetConsoleCtrlHandler(ControlHandler, TRUE);


	// set the sizeoflength
	g_Parser.Begin(4);

	// create main logic thread
	g_hDie = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hThread = (HANDLE)_beginthreadex(NULL, 0, &MainWorkThread, 0, 0, &g_dwThreadID);

	//SOCKET	sockAccept = INVALID_SOCKET;
	char szClientIP[128];
	char szRecvBuff[1024];
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
	sockAccept = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockAccept == INVALID_SOCKET) {
		DumpErr("socket create", GetLastError());
		Terminate();
	}
	printf(">>>TCP Server socket created. Socket Handle : %d\n", sockAccept);

	// bind	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNumber);				// host to network short
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// host to network long
	if (SOCKET_ERROR == bind(sockAccept, (struct sockaddr*)&addr, sizeof(addr)))
	{
		DumpErr("bind", GetLastError());
		Terminate();
	}
	printf(">>>TCP Server socket bind succeeded. Port is %d\n", nPortNumber);


	// listen
	if (SOCKET_ERROR == listen(sockAccept, SOMAXCONN))
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
		SOCKET sockClient = INVALID_SOCKET;
		sockClient = accept(sockAccept, (struct sockaddr*)&addrClient, &nLen);
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
				if (nRecvSize == 0)
					continue;

				// add recv data into Parser buffer
				g_Parser.AddTcpData(szRecvBuff, nRecvSize);				
				Sleep(1);
			}
		}
		closesocket(sockClient);
	}

	closesocket(sockAccept);
	return;
}

unsigned int __stdcall MainWorkThread(LPVOID lp)
{

	char szReadBuff[1024];
	int nReadLen = 0;

	while (WaitForSingleObject(g_hDie, 1) != WAIT_OBJECT_0)
	{
		ZeroMemory(szReadBuff, sizeof(szReadBuff));
		nReadLen = g_Parser.GetOnePacket(szReadBuff);
		if (nReadLen < 0)
		{
			g_log.log(LOGTP_ERR, "Packet is wrong:%s", g_Parser.GetMsg());
			continue;
		}
		else if (nReadLen == 0)
		{
			continue;
		}

		g_log.log(LOGTP_INFO, "<%d>[%.*s]", nReadLen, nReadLen, szReadBuff);

		if (nReadLen != 316)
		{
			printf("Unexpected!!! [RecvLen:%d] \n", nReadLen);
		}
	}

	return 0;
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
		SetEvent(g_hDie);
		return FALSE;

	}
	return FALSE;
}

VOID	Terminate()
{
	if (sockAccept != INVALID_SOCKET)
		closesocket(sockAccept);
	WSACleanup();
	exit(0);
}

