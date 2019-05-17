// NanoQDll.cpp : Defines the exported functions for the DLL application.
//
#include <windows.h>
#include <process.h>

#define NANOQAPI extern "C" __declspec(dllexport)


#include "NanoQDll.h"
#include "../../IRUM_UTIL/NanoQ/NanoQC.h"
#include "../../IRUM_UTIL/NanoQ/NanoQS.h"
#include "./utils/err.c"	//important


CNanoQC	g_senderQ;
CNanoQS g_recvQ;
char			g_sMsgS[1024] = { 0, };
char			g_sMsgR[1024] = { 0, };
BOOL			g_bContinue		= FALSE;
HANDLE			g_hThread		= NULL;
unsigned int	g_dwThreadID	= 0;


void(*g_CallBack)(int, char*);	// return, message
unsigned WINAPI RecvThread(LPVOID lp);


char* GetLastMsgS()
{
	return g_sMsgS;
}

char* GetLastMsgR()
{
	return g_sMsgR;
}

int S_Begin(char *pRemoteChannel, int nSendTimeout)
{
	if (!g_senderQ.Begin(pRemoteChannel, nSendTimeout))
	{
		strcpy(g_sMsgS, g_senderQ.GetMsg());
		return Q_ERROR;
	}
	return Q_SUCCESS;
}

int S_Connect()
{
	if (!g_senderQ.Connect())
	{
		strcpy(g_sMsgS, g_senderQ.GetMsg());
		return Q_ERROR;
	}
	return Q_SUCCESS;
}


int S_SendData(char* pData, int nSendLen)
{
	int nErrCode = Q_SUCCESS;
	int ret = g_senderQ.SendData(pData, nSendLen, &nErrCode);
	if(ret== Q_ERROR)
	{
		strcpy(g_sMsgS, g_senderQ.GetMsg());
	}
	return ret;
}

void S_Close()
{
	g_senderQ.End();
}

/////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////
int R_Begin(char *pMyChannel, int nRecvTimeout)
{
	if (!g_recvQ.Begin(pMyChannel, nRecvTimeout))
	{
		strcpy(g_sMsgR, g_recvQ.GetMsg());
		return Q_ERROR;
	}
	return Q_SUCCESS;
}




int R_RecvData(char* pData, int nBuffLen)
{
	int nErrCode = Q_SUCCESS;
	int ret = g_recvQ.RecvData(pData, nBuffLen, &nErrCode);
	if (ret == Q_ERROR)
	{
		strcpy(g_sMsgR, g_recvQ.GetMsg());
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
		nRet = g_recvQ.RecvData(zRecvMsg, nBuffLen, &nErrCode);
		if (nRet == Q_ERROR)
		{
			//strcpy(g_sMsgR, g_recvQ.GetMsg());
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
	g_recvQ.End();
}
