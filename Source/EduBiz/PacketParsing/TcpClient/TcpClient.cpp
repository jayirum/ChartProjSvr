#pragma warning(disable:4996)
#include <winsock2.h>	// # socket functions header file
#include <stdio.h>
#include "../Common/LogMsg.h"
#include "../Common/utilfunc.h"


// instead of this code
// input ws2_32.lib in Project > Linker > Input > Additional Dependencies 
#pragma comment(lib, "ws2_32.lib")


BOOL	WINAPI ControlHandler(DWORD dwCtrlType);

CLogMsg g_log;
char g_zOneData[] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";	// 62bytes

// packet structure
typedef struct _MYPACKET
{
	char START[1];	//STX
	char LEN[4];
	char Data[310];	// 62 * 5
	char END[1];	//ETX
}MYPACKET;

void main(int argc, char** argv)
{
	if (argc < 3) {
		printf("\n<USEAGE>--------------------------\n");
		printf("Input Server IP and Port\n");
		printf("--------------------------------\n");
		return;
	}

	// Create / Open log file
	g_log.OpenLog("TcpClient");


	///
	///	compose packet
	//
	MYPACKET myPack;
	
	int nBufSize = strlen(g_zOneData);
	int nTotalSize = sizeof(MYPACKET);
	memset(&myPack, 0x20, nTotalSize);
	myPack.START[0] = 0x02;
	sprintf(myPack.LEN, "%4d", nTotalSize);

	for (int i = 0; i < 5; i++)
		memcpy(myPack.Data+(i*nBufSize), g_zOneData, nBufSize);

	myPack.END[0] = 0x03;


	///
	///	SOCKET
	///
	SOCKET hSock = INVALID_SOCKET;	//# socket handle
	char zServerIP[128];
	int nServerPort;
	int nSendingNum;
	struct sockaddr_in addrDest;

	int nSentSize;
	int nLoop;
	SYSTEMTIME st;


	// IP and Port 
	strcpy(zServerIP, argv[1]);
	nServerPort = atoi(argv[2]);

	printf(">>>TCP Client starts.(ServerIP:%s, Port:%d)\n", zServerIP, nServerPort);


	// crtl+c
	SetConsoleCtrlHandler(ControlHandler, TRUE);



	//////////////////////////////////////////////////////////
	// initialize

	// # Windows provide windows socket as dll, should initialize the dll to use.
	WSADATA wsaData;
	BYTE nMajor = 2, nMinor = 2;	// major and minor version of windows socket.
	if (WSAStartup(MAKEWORD(nMajor, nMinor), &wsaData) != 0) {
		DumpErr("WSAStartup", GetLastError());
		return;
	}

	if (LOBYTE(wsaData.wVersion) != nMajor ||
		HIBYTE(wsaData.wVersion) != nMinor) {
		WSACleanup();
		return;
	}

	//////////////////////////////////////////////////////////
	// # create a new socket
	hSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSock == INVALID_SOCKET)
	{
		DumpErr("socket create", GetLastError());
		WSACleanup();
		return;
	}
	printf(">>>TCP client socket created. Socket Handle : %d\n", hSock);
	
	//////////////////////////////////////////////////////////
	// # set server information
	memset(&addrDest, 0x00, sizeof(addrDest));	//# initialize
	addrDest.sin_family = AF_INET;
	addrDest.sin_port = htons(nServerPort);
	addrDest.sin_addr.s_addr = inet_addr(zServerIP);

	//////////////////////////////////////////////////////////
	// # try to connect
	int nRet = connect(hSock, (struct sockaddr*)&addrDest, sizeof(addrDest));
	if (SOCKET_ERROR == nRet)
	{
		DumpErr("connect", GetLastError());
		closesocket(hSock);
		WSACleanup();
		return;
	}
	printf(">>>connect succeeded\n");

	//////////////////////////////////////////////////////////
	// # send data
	while (1)
	{
		//////////////////////////////////////////////////////////
		// # How many data would you like to send per second?
		// 	
		printf("\n>>>How many data would you like to send ? \n");
		scanf("%d", &nSendingNum);

		GetLocalTime(&st);
		printf("------------Start(%02d:%02d:%02d) DataLen:%d  SendingCnt:%d\n", st.wHour, st.wMinute, st.wSecond, nTotalSize, nSendingNum);
		g_log.log(LOGTP_INFO, "Start. SendingNum : %d", nSendingNum);
		for (nLoop = 0; nLoop < nSendingNum; nLoop++)
		{
			nSentSize = send(hSock, (CHAR*)&myPack, nTotalSize, 0);
			if (SOCKET_ERROR == nSentSize)
			{
				DumpErr("send", GetLastError());
				break;
			}
			Sleep(1);
		}
		GetLocalTime(&st);
		printf("------------End(%02d:%02d:%02d) Total sent number : %d\n", st.wHour, st.wMinute, st.wSecond, nLoop);
		g_log.log(LOGTP_INFO, "Total sent number : %d", nLoop);
	}

	closesocket(hSock);

	// release windows socket
	WSACleanup();
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

