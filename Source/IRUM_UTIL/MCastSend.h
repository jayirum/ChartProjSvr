#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma warning(disable : 4786)
#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")

class CMCastSend  
{
public:
	CMCastSend();
	virtual ~CMCastSend();

	BOOL Begin(char *pzLocalIP, char* pzMCastIP, int nPort);
	VOID End();

	BOOL SendData(char* psData, int nSendLen);

	char* GetMsg() { return m_zMsg; };
protected:
	SOCKET					m_hSock;
	int						m_nMCastPort;
	char					m_szMCastIP[128];
	char					m_szLocalIP[128];
	struct sockaddr_in		m_sinMcast;
	char					m_zMsg[1024];
};
