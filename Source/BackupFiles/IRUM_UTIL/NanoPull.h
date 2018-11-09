#pragma once


/*
	KRChart 로 부터 시세 데이터를 pull 한다.

	- KRChart : NanoMsg 의 Push
	- here : NanoMsg 의 Pull
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

