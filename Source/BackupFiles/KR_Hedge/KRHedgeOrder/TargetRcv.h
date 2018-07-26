#pragma once


/*
KRChart �� ���� �ü� �����͸� pull �Ѵ�.

- KRChart : NanoMsg �� Push
- here : NanoMsg �� Pull
*/
#include "nanomsg/nn.h"
#include "nanomsg/pipeline.h"
#include "nanomsg/tcp.h"




class CTargetRcv
{
public:
	CTargetRcv();
	~CTargetRcv();

	BOOL	InitSock(char* ip, char* port);
	VOID	CloseSock();
	INT		RecvData(_Out_ void* pRecvBuf);
	INT		RecvData(_In_ int nBufLen, _Out_ void* pRecvBuf);
	CHAR*	GetMsg() { return m_szMsg; }

private:
	int				m_sock;
	char			m_szMsg[1024];
};

