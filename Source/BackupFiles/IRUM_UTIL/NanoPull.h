#pragma once


/*
	KRChart �� ���� �ü� �����͸� pull �Ѵ�.

	- KRChart : NanoMsg �� Push
	- here : NanoMsg �� Pull
*/
#include "nanomsg/nn.h"
#include "nanomsg/pipeline.h"
#include "nanomsg/tcp.h"


#pragma warning(disable:4996)


class CNanoPull 
{
public:
	CNanoPull();
	~CNanoPull();

	bool	Begin(char* ip, char* port);
	void	End();
	int		RecvData(void* pRecvBuf);
	char*	GetMsg() { return m_szMsg; }
	
private:
	int				m_sock;
	char			m_szMsg[1024];
};

