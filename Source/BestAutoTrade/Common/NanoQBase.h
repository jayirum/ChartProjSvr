#pragma once

// project option > VC++ Directories > Include Directories : C:\nanomsg-1.1.5\src;
// project option > VC++ Directories > Library Directories : diretory including nanomsg.lib
// C:\ChartProjSvr\Source\_Bin;
// project option > C/C++ > Preprocessor : NN_HAVE_WINDOWS;

#include "nn.h"
#include "./utils/attr.h"
#include "./utils/err.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "BARelayInc.h"
#include <Windows.h>


#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "nanomsg.lib")

using namespace _BA_RELAY;

typedef  void(__stdcall *PRecvCallBack)(void*, char*, int);	// class instance pointer, recv data, return value

class CNanoQ : public CBaseThread
{
public:
	CNanoQ();
	~CNanoQ();

	
	virtual BOOL	Begin(char* pzAccNo, int nTimeout, int nQueueTp, BOOL bCallbackUse=FALSE);
	virtual VOID	ThreadFunc() = 0;
	VOID	End();
	//void	ComposeChannelNm(char* pzAccNo);
	BOOL	Connect();
	int		SendData(char* pData, int nSendLen, int* pnErrCode);	
	void	RegisterCallBack(LPVOID lpClassInstancePtr, PRecvCallBack lpCallBack);
	int		RecvData(_Out_ char* pData, _In_ int nBuffLen, _Out_ int* pnErrCode);
	int		RecvDataEx();

	inline char* GetMsg() { return m_zMsg; }
	char* ChannelNm(); 

protected:
	BOOL	NeedsToCallConnect();

protected:
	char		m_zChannelNm[LEN_CHANNEL_NM + 1];
	QUEUE_TP	m_tpQueue;
	NANOMSG_TP	m_tpNanomsg;
	int			m_nTimeout;
	int			m_sock;
	char		m_zMsg[1024];

	// For use callback
	BOOL			m_bUseCallback;
	LPVOID			m_lpClassInstancePtr;
	PRecvCallBack	m_lpCallBackFn;

};