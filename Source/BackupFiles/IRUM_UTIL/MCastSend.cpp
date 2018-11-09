
#include "MCastSend.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMCastSend::CMCastSend()
{
	m_hSock = INVALID_SOCKET;
}

CMCastSend::~CMCastSend()
{
	End();
}

void CMCastSend::End()
{
	if (m_hSock != INVALID_SOCKET) {
		struct ip_mreq	mreq;
		mreq.imr_multiaddr.s_addr = inet_addr(m_szMCastIP);
		mreq.imr_interface.s_addr = inet_addr(m_szLocalIP);
		setsockopt(m_hSock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq));

		struct linger ling;
		ling.l_onoff = 1;   // 0 ? use default, 1 ? use new value
		ling.l_linger = 0;  // close session in this time
		setsockopt(m_hSock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
		//-We can avoid TIME_WAIT on both of client and server side as we code above.
		closesocket(m_hSock);
	}
	m_hSock = INVALID_SOCKET;

	WSACleanup();
}


BOOL CMCastSend::Begin(char *pzLocalIP, char* pzMCastIP, int nPort)
{
	strcpy(m_szLocalIP, pzLocalIP);
	strcpy(m_szMCastIP, pzMCastIP);
	m_nMCastPort = nPort;


	m_sinMcast.sin_family = AF_INET;
	m_sinMcast.sin_port = htons(m_nMCastPort);
	m_sinMcast.sin_addr.s_addr = inet_addr(m_szMCastIP);

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		DumpErr("WSAStartup", GetLastError(), m_zMsg);
		return FALSE;
	}

	m_hSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_hSock == INVALID_SOCKET)
	{
		DumpErr("WSASocket", GetLastError(), m_zMsg);
		return FALSE;
	}

	unsigned char TTL = 0;     /* time to live (hop count) */
	if ((setsockopt(m_hSock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&TTL, sizeof(TTL))) < 0) {
		DumpErr("setsockopt TTL", GetLastError(), m_zMsg);
		return FALSE;
	}


	// Add membership to multicast group
	//struct ip_mreq	mreq;
	//mreq.imr_multiaddr.s_addr = inet_addr(m_szMCastIP);
	//mreq.imr_interface.s_addr = inet_addr(m_szLocalIP);

	//if (setsockopt(m_hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) == SOCKET_ERROR) {
	//	DumpErr("setsockopt ADD MEMBERSHIP", GetLastError(), m_zMsg);
	//	return FALSE;
	//}

	return TRUE;
}



BOOL CMCastSend::SendData(char* psData, int nSendLen)
{
	int nTotLen = nSendLen;
	int nSentLen = 0;
	int nRet = 0;
	while (nTotLen > 0)
	{
		nRet = sendto(m_hSock, psData, nTotLen, 0, (LPSOCKADDR)&m_sinMcast, sizeof(m_sinMcast));
		if (nRet == SOCKET_ERROR)
		{
			DumpErr("sendto", GetLastError(), m_zMsg);
			return -1;
		}
		nTotLen -= nRet;
		nSentLen += nRet;
		psData += nRet;
		Sleep(1);
	}

	return nSentLen;
}





//
//BOOL CMCastSend::Begin(char *pzLocalIP, char* pzMCastIP, int nPort)
//{
//	strcpy(m_szLocalIP, pzLocalIP);
//	strcpy(m_szMCastIP, pzMCastIP);
//	m_nMCastPort = nPort;
//	
//	m_sinMcast.sin_family = AF_INET;
//	m_sinMcast.sin_port = htons(m_nMCastPort);
//	m_sinMcast.sin_addr.s_addr = inet_addr(m_szMCastIP);
//
//
//	WSADATA wsaData;
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		DumpErr("WSAStartup", GetLastError(), m_zMsg);
//		return FALSE;
//	}
//
//	m_hSock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0,
//		WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED);
//	if (m_hSock == INVALID_SOCKET)
//	{
//		DumpErr("WSASocket", GetLastError(), m_zMsg);
//		return FALSE;
//	}
//
//	struct sockaddr_in sockAddr;
//	sockAddr.sin_family = AF_INET;
//	sockAddr.sin_port = m_nMCastPort;
//	sockAddr.sin_addr.s_addr = inet_addr(m_szLocalIP);
//
//	BOOL opt = TRUE;
//	int optlen = sizeof(opt);
//	setsockopt(m_hSock, SOL_SOCKET, SO_REUSEADDR, (const char far *)&opt, optlen);
//
//	if (bind(m_hSock, (struct sockaddr FAR *) &sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
//	{
//		DumpErr("bind", GetLastError(), m_zMsg);
//		return FALSE;
//	}
//
//	int nTTL = 1;
//	DWORD dwRet;
//	if (WSAIoctl(m_hSock, SIO_MULTICAST_SCOPE, &nTTL, sizeof(nTTL), NULL, 0, &dwRet, NULL, NULL) == SOCKET_ERROR)
//	{
//		DumpErr("WSAIoctl TTL", GetLastError(), m_zMsg);
//		return FALSE;
//	}
//
//	struct ip_mreq	mreq;
//
//	mreq.imr_multiaddr.s_addr = inet_addr(m_szMCastIP);
//	mreq.imr_interface.s_addr = inet_addr(m_szLocalIP);
//
//	if (setsockopt(m_hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq,
//		sizeof(mreq)) == SOCKET_ERROR) {
//		DumpErr("setsockopt IP_ADD_MEMBERSHIP", GetLastError(), m_zMsg);
//		return FALSE;
//	}
//
//	
//
//	return TRUE;
//}