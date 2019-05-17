#pragma once



#include "nanomsg/nn.h"
#include "nanomsg/pipeline.h"
#include "nanomsg/tcp.h"



class CNanoSocket
{
public:
	CNanoSocket();
	~CNanoSocket();

	BOOL	InitServerSock(char* ip, char* port);
	VOID	CloseServerSock();
	INT		SendData(void* p, int size);
	CHAR*	GetMsg() { return m_szMsg; }
private:
	int				m_sockSvr;
	char			m_szMsg[1024];
};

