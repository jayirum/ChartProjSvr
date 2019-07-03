#pragma once


#include "../Common/BPInc.h"


#ifdef BLASHPF_API_EXPORTS
#define BLASHPF_API extern "C" __declspec(dllexport)
#else 
#define BLASHPF_API extern "C" __declspec(dllimport)
#endif


/*
	result
	-1 : error
	0  : timeout
	else : ok
*/

// timeout : millisecond
BLASHPF_API int BPAPI_Init(char* pzServerIp, int nServerPort, int nRecvTimeout, int nSendTimeout);
BLASHPF_API int BPAPI_DeInit();
BLASHPF_API int BPAPI_Disconnect();
BLASHPF_API int BPAPI_Connect();
BLASHPF_API int BPAPI_RecvData(char* pRecvData, int nBuffSize);
BLASHPF_API int BPAPI_SendData(char* pSendData, int nSendSize);
BLASHPF_API char* BPAPI_GetMsg();
BLASHPF_API bool BPAPI_IsConnected();

int CreateSock();
void DumpErr(char* pSrc);

