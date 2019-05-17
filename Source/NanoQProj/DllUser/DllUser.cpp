// DllUser.cpp : Defines the entry point for the console application.
//

#include "../../IRUM_UTIL/NanoQ/NanoQDll.h"
#include <Windows.h>
#include <stdio.h>
#pragma warning(disable:4996)

BOOL WINAPI ControlHandler(DWORD dwCtrlType);

typedef int (*QWDll_Begin)(char *, int);
QWDll_Begin	QW_Begin;

typedef int(*QWDll_Connect)();
QWDll_Connect	QW_Connect;

typedef int(*QWDll_SendData)(char*, int);
QWDll_SendData	QW_SendData;


typedef int(*SWDll_Close)();
SWDll_Close	QW_Close;

typedef void(*QWDll_GetLastMsg)(char*);
QWDll_GetLastMsg	QW_GetLastMsg;


typedef int (*RQDll_Begin)(char *, int );
RQDll_Begin	QR_Begin;

typedef int (*QRDll_RecvData)(char* , int );
QRDll_RecvData QR_RecvData;

typedef void (*QRDll_Close)();
QRDll_Close	QR_Close;

typedef void(*QRDll_GetLastMsg)(char*);
QRDll_GetLastMsg	QR_GetLastMsg;


BOOL InitDll();
HMODULE g_hIns = NULL;

char g_zMsg[1024];
BOOL g_bContinue = TRUE;


void Write()
{
	if (!InitDll())
		return ;

	int ret = QW_Begin("relay", 1000);
	if (ret == Q_ERROR)
	{
		//QW_GetLastMsg(g_zMsg);
		printf("QW_Begin error(%s)\n", g_zMsg);
		return ;
	}


	ret = QW_Connect();
	if (ret == Q_ERROR)
	{
		QW_GetLastMsg(g_zMsg);
		printf("connect error(%s)\n", g_zMsg);
		return ;
	}

	printf("type index:");
	int idx;
	scanf("%d", &idx);

	char buf[1024] = { 0, };
	SYSTEMTIME st;
	while (g_bContinue)
	{
		GetLocalTime(&st);
		sprintf(buf, "[%02d:%02d:%02d.%03d][%d]send data......\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,idx);
		size_t data_len = strlen(buf);

		ret = QW_SendData(buf, data_len);

		if (ret == Q_TIMEOUT)
		{
			printf("send time out.....\n");
			Sleep(3000);
		}
		else if (ret == Q_ERROR)
		{
			QW_GetLastMsg(g_zMsg);
			printf("send err(%s)\n", g_zMsg);
			Sleep(3000);
		}
		else
		{
			printf("send success[%d](%s)\n", idx, buf);
			Sleep(100);
		}

		//getchar();
	}
}


void Read()
{
	if (!InitDll())
		return;

	int ret = QR_Begin("relay", 1000);
	if (ret == Q_ERROR)
	{
		QR_GetLastMsg(g_zMsg);
		printf("QW_Begin error(%s)\n", g_zMsg);
		return;
	}
	printf("ready to receive...\n");
	

	char buf[1024] = { 0, };
	char msg[1024];
	int nBufSize = sizeof(buf);
	while (g_bContinue)
	{
		ZeroMemory(buf, nBufSize);
		int nRecvLen = QR_RecvData(buf, nBufSize);
		if (nRecvLen == Q_ERROR)
		{
			QR_GetLastMsg(msg);
			printf("recv error(%s)\n",msg );
		}
		else if (nRecvLen == Q_TIMEOUT)
		{
			printf("receive timeout......\n");
		}
		else
		{
			printf("recv success[%d](%s)\n", nRecvLen, buf);
		}

	}
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage : R => Reader, W => Writer");
		return 0;
	}

	SetConsoleCtrlHandler(ControlHandler, TRUE);

	if (*argv[1] == 'R' || *argv[1] == 'r')
	{
		Read();
	}
	if (*argv[1] == 'W' || *argv[1] == 'w')
	{
		Write();
	}

	
    return 0;
}

BOOL InitDll()
{
	g_hIns = GetModuleHandleA("NanoQDll.dll");

	if (g_hIns)
	{
		return TRUE;
	}
	g_hIns = LoadLibraryA("NanoQDll.dll");
	if (!g_hIns)
	{
		printf("DLL not found !\n");
		return TRUE ;
	}
	

	//ShowMessage(IntToStr((int)sizeof(TFutExec)));

	QW_Begin = NULL;
	QW_Begin = (QWDll_Begin)GetProcAddress(g_hIns, "W_Begin");
	if (QW_Begin == NULL)
	{
		printf("S_Begin function not found in the DLL !\n");
		return TRUE;
	}

	QW_Connect = NULL;
	QW_Connect = (QWDll_Connect)GetProcAddress(g_hIns, "W_Connect");
	if (QW_Connect == NULL)
	{
		printf("S_Connect function not found in the DLL !\n");
		return TRUE;
	}

	QW_SendData = NULL;
	QW_SendData = (QWDll_SendData)GetProcAddress(g_hIns, "W_SendData");
	if (QW_SendData == NULL)
	{
		printf("S_SendData function not found in the DLL !\n");
		return TRUE;
	}

	QW_Close = NULL;
	QW_Close = (SWDll_Close)GetProcAddress(g_hIns, "W_Close");
	if (QW_Close == NULL)
	{
		printf("S_Close function not found in the DLL !\n");
		return TRUE;
	}

	
	QW_GetLastMsg = NULL;
	QW_GetLastMsg = (QWDll_GetLastMsg)GetProcAddress(g_hIns, "W_GetLastMsg");
	if (QW_GetLastMsg == NULL)
	{
		printf("S_GetLastMsg function not found in the DLL !\n");
		return TRUE;
	}

	QR_Begin = NULL;
	QR_Begin = (RQDll_Begin)GetProcAddress(g_hIns, "R_Begin");
	if (QR_Begin == NULL)
	{
		printf("R_Begin function not found in the DLL !\n");
		return TRUE;
	}

	QR_RecvData = NULL;
	QR_RecvData = (QRDll_RecvData)GetProcAddress(g_hIns, "R_RecvData");
	if (QR_RecvData == NULL)
	{
		printf("R_RecvData function not found in the DLL !\n");
		return TRUE;
	}

	QR_Close = NULL;
	QR_Close = (QRDll_Close)GetProcAddress(g_hIns, "R_Close");
	if (QR_Close == NULL)
	{
		printf("R_Close function not found in the DLL !\n");
		return TRUE;
	}

	QR_GetLastMsg = NULL;
	QR_GetLastMsg = (QRDll_GetLastMsg)GetProcAddress(g_hIns, "R_GetLastMsg");
	if (QR_GetLastMsg == NULL)
	{
		printf("R_GetLastMsg function not found in the DLL !\n");
		return TRUE;
	}


	return TRUE;
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
		printf("\t\tStopping...\n");
		g_bContinue = FALSE;
		QW_Close();
		QR_Close();
		FreeLibrary(g_hIns);
		break;

	}
	return FALSE;
}

