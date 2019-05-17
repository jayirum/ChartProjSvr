
#include "MCastRecv.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMCastRecv::CMCastRecv()
{
	m_hSock = INVALID_SOCKET;
}

CMCastRecv::~CMCastRecv()
{
	End();
}


void CMCastRecv::End()
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



BOOL CMCastRecv::Begin(char *pzLocalIP, char* pzMCastIP, int nPort)
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

	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(m_nMCastPort);
	sockAddr.sin_addr.s_addr = inet_addr(m_szLocalIP);

	BOOL opt = TRUE;
	int optlen = sizeof(opt);
	setsockopt(m_hSock, SOL_SOCKET, SO_REUSEADDR, (const char far *)&opt, optlen);

	if (bind(m_hSock, (struct sockaddr FAR *) &sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		DumpErr("bind", GetLastError(), m_zMsg);
		return FALSE;
	}
	
	// Add membership to multicast group
	struct ip_mreq	mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(m_szMCastIP);
	mreq.imr_interface.s_addr = inet_addr(m_szLocalIP);

	if (setsockopt(m_hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) == SOCKET_ERROR) {
		DumpErr("setsockopt ADD MEMBERSHIP", GetLastError(), m_zMsg);
		return FALSE;
	}

	return TRUE;
}




//
//BOOL CMCastRecv::Begin(char *pzLocalIP, char* pzMCastIP, int nPort)
//{
//	strcpy(m_szLocalIP, pzLocalIP);
//	strcpy(m_szMCastIP, pzMCastIP);
//	m_nMCastPort = nPort;
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
//	//int nTTL = 0;
//	//DWORD dwRet;
//	//if (WSAIoctl(m_hSock, SIO_MULTICAST_SCOPE, &nTTL, sizeof(nTTL), NULL, 0, &dwRet, NULL, NULL) == SOCKET_ERROR)
//	//{
//	//	DumpErr("WSAIoctl", GetLastError(), m_zMsg);
//	//	return FALSE;
//	//}
//
//	struct sockaddr_in sinMulti;
//	sinMulti.sin_family = AF_INET;
//	sinMulti.sin_port = m_nMCastPort;
//	sinMulti.sin_addr.s_addr = inet_addr(m_szMCastIP);
//	DWORD dwFlag = JL_RECEIVER_ONLY;
//	if (WSAJoinLeaf(m_hSock, (SOCKADDR *)&sinMulti, sizeof(sinMulti),NULL, NULL, NULL, NULL, dwFlag) == INVALID_SOCKET)
//	{
//		DumpErr("WSAJoinLeaf", GetLastError(), m_zMsg);
//		sprintf(m_zMsg, "%s(%s)(%d)", m_zMsg, m_szMCastIP, m_nMCastPort);
//		return FALSE;
//	}
//
//	// Add membership to multicast group
//	//struct ip_mreq	mreq;
//	//mreq.imr_multiaddr.s_addr = inet_addr(m_szMCastIP);
//	//mreq.imr_interface.s_addr = inet_addr(m_szLocalIP);
//
//	//if (setsockopt(m_hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) == SOCKET_ERROR) {
//	//	DumpErr("setsockopt ADD MEMBERSHIP", GetLastError(), m_zMsg);
//	//	return FALSE;
//	//}
//
//	return TRUE;
//}

int  CMCastRecv::RecvData(_Out_ char *pzRecvBuf, int nBufLen)
{
	struct sockaddr_in sockFrom;
	int	nFrom = sizeof(sockFrom);
	int nRet = 0;
	if ((nRet = recvfrom(m_hSock, pzRecvBuf, nBufLen, 0,(struct sockaddr FAR *)&sockFrom, &nFrom)) == SOCKET_ERROR)
	{
		DumpErr("RecvData", GetLastError(), m_zMsg);
		return -1;
	}
	return nRet;
}