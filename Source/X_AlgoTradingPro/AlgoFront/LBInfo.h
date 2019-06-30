#pragma once
#include <WinSock2.h>


class CClientInterface;

typedef void(CClientInterface::*RECONNCALLBACK)();


class CLBInfo
{
public:

	SOCKET			m_sock;
	char			m_ip[20];
	int				m_port;
	RECONNCALLBACK	m_ReconnCallBack;

private:
	BOOL	m_bConn;
public:
	CLBInfo();
	~CLBInfo();

	BOOL Initialize(RECONNCALLBACK pReconnCallback);
	BOOL Connect();
	void Finalize();
	void TellUserClose(char* pzUserId);
	void TellUserLogon(char* pzUserId);
	void RequestRecvIO();

	BOOL IsConnected() { return m_bConn; }
	VOID MarkDisconnected() { m_bConn = FALSE; }

	VOID ReConnLB();
	static DWORD WINAPI ReconnThread(LPVOID lp);
};

