#pragma once




#ifdef NANOQAPI

#else 

#define NANOQAPI extern "C" __declspec(dllimport)

NANOQAPI int S_Begin(char *pRemoteChannel, int nSendTimeout);
NANOQAPI int S_Connect();
NANOQAPI int S_SendData(char* pData, int nSendLen);
NANOQAPI void S_Clsoe();

NANOQAPI int R_Begin(char *pMyChannel, int nRecvTimeout);
NANOQAPI int R_RecvData(char* pData, int nBuffLen);
NANOQAPI int R_SetCallback(char* pData, int nBuffLen, void* pCallBackFn);
NANOQAPI void R_StopRecv();
NANOQAPI void R_Clsoe();

NANOQAPI char* GetLastMsgS();
NANOQAPI char* GetLastMsgR();

#endif