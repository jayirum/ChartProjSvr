// NanoQDll.cpp : Defines the exported functions for the DLL application.
//
#include <windows.h>
#include <process.h>

#define NANOQAPI_EXPORTS


#include "../../IRUM_UTIL/NanoQ/NanoQDll.h"
#include "../../IRUM_UTIL/NanoQ/NanoQReader.h"
#include "../../IRUM_UTIL/NanoQ/NanoQWriter.h"
#include "./utils/err.c"	//important


CNanoQWriter	queWriter;
CNanoQReader	queReader;
char			g_sMsgW[1024] = { 0, };
char			g_sMsgR[1024] = { 0, };
BOOL			g_bContinue		= FALSE;
HANDLE			g_hThread		= NULL;
unsigned int	g_dwThreadID	= 0;


void(*g_CallBack)(int, char*);	// return, message
unsigned WINAPI RecvThread(LPVOID lp);


void W_GetLastMsg(char* pMsg)
{
	strcpy(pMsg, g_sMsgW);
}

void R_GetLastMsg(char* pMsg)
{
	strcpy(pMsg, g_sMsgR);
}

int W_Begin(char *pzChannelNm, int nSendTimeout, int nQueueTp)
{
	if (!queWriter.Begin(pzChannelNm, nSendTimeout, nQueueTp))
	{
		strcpy(g_sMsgW, queWriter.GetMsg());
		return Q_ERROR;
	}
	return Q_SUCCESS;
}

int W_Connect()
{
	if (!queWriter.Connect())
	{
		strcpy(g_sMsgW, queWriter.GetMsg());
		return Q_ERROR;
	}
	return Q_SUCCESS;
}


int W_SendData(char* pData, int nSendLen)
{
	int nErrCode = Q_SUCCESS;
	int ret = queWriter.SendData(pData, nSendLen, &nErrCode);
	if(ret== Q_ERROR)
	{
		strcpy(g_sMsgW, queWriter.GetMsg());
	}
	return ret;
}

void W_Close()
{
	queWriter.End();
}

/////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////
int R_Begin(char *pzChannelNm, int nRecvTimeout, int nQueueTp)
{
	if (!queReader.Begin(pzChannelNm, nRecvTimeout, nQueueTp))
	{
		strcpy(g_sMsgR, queReader.GetMsg());
		return Q_ERROR;
	}
	return Q_SUCCESS;
}

int R_RecvData(char* pData, int nBuffLen)
{
	int nErrCode = Q_SUCCESS;
	int ret = queReader.RecvData(pData, nBuffLen, &nErrCode);
	if (ret == Q_ERROR)
	{
		if (nErrCode == Q_TIMEOUT)
			return Q_TIMEOUT;
		strcpy(g_sMsgR, queReader.GetMsg());
	}
	return ret;
}

void R_SetCallback(char* pData, int nBuffLen, void* pCallBackFn)
{
	g_CallBack = (void(__cdecl *)(int, char*))pCallBackFn;
	g_hThread = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, 0, 0, &g_dwThreadID);

}

unsigned WINAPI RecvThread(LPVOID lp)
{
	int nErrCode = 0;
	int nRet = 0;
	char zRecvMsg[1024];
	int nBuffLen = sizeof(zRecvMsg);

	while(g_bContinue)
	{
		ZeroMemory(zRecvMsg, sizeof(zRecvMsg));

		int nErrCode = Q_SUCCESS;
		nRet = queReader.RecvData(zRecvMsg, nBuffLen, &nErrCode);
		if (nRet == Q_ERROR)
		{
			//strcpy(g_sMsgR, queReader.GetMsg());
		}
		else if (nRet == Q_SUCCESS)
		{
			g_CallBack(nRet, zRecvMsg);
		}
	}

	return 0;
}


void R_StopRecv()
{
	g_bContinue = FALSE;
}

void R_Close()
{
	R_StopRecv();
	queReader.End();
}
