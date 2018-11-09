#pragma once



#include "nanomsg/nn.h"
#include "nanomsg/pipeline.h"
#include "nanomsg/tcp.h"
#include <windows.h>

#pragma warning(disable:4996)

class CNanoPush
{
public:
	CNanoPush();
	~CNanoPush();

	BOOL	Begin(char* ip, char* port);
	VOID	End();
	INT		SendData(void* p, int size);
	CHAR*	GetMsg() { return m_szMsg; }
private:
	int				m_sockSvr;
	char			m_szMsg[1024];
	BOOL			m_bConn;
	char			m_zIP[64], m_zPort[64];
};

