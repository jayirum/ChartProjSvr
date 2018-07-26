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

	bool	InitSock(char* ip, char* port);
	void	CloseSock();
	int		RecvData(void* pRecvBuf);
	char*	GetMsg() { return m_szMsg; }
	
private:
	int				m_sock;
	char			m_szMsg[1024];
};

