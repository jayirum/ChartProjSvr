#include "Listen.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"

extern char		g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;



CListen::CListen()
{
	m_sock = INVALID_SOCKET;;
	m_hwsa = NULL;;

	ZeroMemory(m_zListenIp, sizeof(m_zListenIp));
	m_nListenPort = 0;
}


CListen::~CListen()
{
	
}


BOOL CListen::Initialzie()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		DumpErr("WSAStartup", GetLastError(), m_zMsg);
		return FALSE;
	}

	char zTemp[1024] = { 0, };
	CUtil::GetConfig(g_zConfig, "NETWORK", "LISTEN_IP", m_zListenIp);
	CUtil::GetConfig(g_zConfig, "NETWORK", "LISTEN_PORT", zTemp);
	m_nListenPort = atoi(zTemp);

	m_localsin.sin_family		= AF_INET;
	m_localsin.sin_port			= htons(m_nListenPort);
	m_localsin.sin_addr.s_addr	= inet_addr(m_zListenIp);

	return CreateSocket();
}

VOID CListen::DeInitialize()
{
	CloseSocket();
}

VOID CListen::CloseSocket()
{
	if (m_sock != INVALID_SOCKET) {
		struct linger ling;
		ling.l_onoff = 1;   // 0 ? use default, 1 ? use new value
		ling.l_linger = 0;  // close session in this time
		setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
		//-We can avoid TIME_WAIT on both of client and server side as we code above.
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
	}
	if (m_hwsa) {
		WSACloseEvent(m_hwsa);
		m_hwsa = NULL;
	}

}


BOOL CListen::CreateSocket()
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		DumpErr("socket", GetLastError(), m_zMsg);
		return FALSE;
	}

	m_hwsa = WSACreateEvent();
	if (WSAEventSelect(m_sock, m_hwsa, FD_ACCEPT))
	{
		DumpErr("WSAEventSelect", GetLastError(), m_zMsg);
		return FALSE;
	}
	return TRUE;
}


// accept
VOID CListen::ThreadFunc()
{

}