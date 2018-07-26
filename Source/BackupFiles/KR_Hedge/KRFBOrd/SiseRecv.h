#pragma once


/*
	KRChart �� ���� �ü� �����͸� pull �Ѵ�.

	- KRChart : NanoMsg �� Push
	- here : NanoMsg �� Pull
*/
#include "nanomsg/nn.h"
#include "nanomsg/pipeline.h"
#include "nanomsg/tcp.h"




class CSiseRecv 
{
public:
	CSiseRecv();
	~CSiseRecv();

	BOOL	InitSock(char* ip, char* port);
	VOID	CloseSock();
	INT		RecvData(void* pRecvBuf);
	CHAR*	GetMsg() { return m_szMsg; }
	
private:
	int				m_sock;
	char			m_szMsg[1024];
};

