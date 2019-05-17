#pragma once




#ifdef NANOQAPI_EXPORTS
	#define NANOQAPI extern "C" __declspec(dllexport)
#else 
	#define NANOQAPI extern "C" __declspec(dllimport)
	enum { Q_ERROR = -1, Q_SUCCESS, Q_TIMEOUT };
#endif


NANOQAPI int W_Begin(char *pzChannelNm, int nSendTimeout);
NANOQAPI int W_Connect();
NANOQAPI int W_SendData(char* pData, int nSendLen);
NANOQAPI void W_Close();
NANOQAPI void W_GetLastMsg(char* pMsg);

NANOQAPI int R_Begin(char *pzChannelNm, int nRecvTimeout);
NANOQAPI int R_RecvData(char* pData, int nBuffLen);
NANOQAPI void R_SetCallback(char* pData, int nBuffLen, void* pCallBackFn);
NANOQAPI void R_StopRecv();
NANOQAPI void R_Close();
NANOQAPI void R_GetLastMsg(char* pMsg);
