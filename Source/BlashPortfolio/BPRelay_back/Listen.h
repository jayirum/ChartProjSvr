/*
	Overlapped I/O ¸ðµ¨

	- accept

	- recv

	- pass to Dispatch thread
*/

#pragma once
#include <winsock2.h>
#include "../../IRUM_UTIL/BaseThread.h"
#include "Dispatch.h"

#define		ACCPET_TIMEOUT	10

class CListen : public CBaseThread
{
public:
	CListen();
	~CListen();

	BOOL	Initialzie();
	VOID	DeInitialize();
	BOOL	CreateSocket();
	VOID	CloseSocket();

	VOID	ThreadFunc();

private:
	SOCKET			m_sock;
	SOCKADDR_IN		m_localsin;
	WSAEVENT		m_hwsa;

	char			m_zListenIp[32];
	int				m_nListenPort;

	char			m_zMsg[1024];

private:
	CDispatch		m_dispatch;
};

