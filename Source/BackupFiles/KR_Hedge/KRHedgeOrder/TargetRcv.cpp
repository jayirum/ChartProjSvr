#include "stdafx.h"
#include "TargetRcv.h"


CTargetRcv::CTargetRcv()
{
}


CTargetRcv::~CTargetRcv()
{
	CloseSock();
}


BOOL CTargetRcv::InitSock(char* ip, char* port)
{
	/*  Create the socket. */
	m_sock = nn_socket(AF_SP, NN_PULL);
	if (m_sock < 0) {
		sprintf(m_szMsg, "nn_socket err: %s", nn_strerror(nn_errno()));
		return FALSE;
	}

	/*  Bind to the URL.  This will bind to the address and listen
	synchronously; new clients will be accepted asynchronously
	without further action from the calling program. */
	char url[32];
	sprintf(url, "tcp://%s:%s", ip, port);
	if (nn_connect(m_sock, url) < 0) {
		sprintf(m_szMsg, "nn_bind error: %s", nn_strerror(nn_errno()));
		nn_close(m_sock);
		return FALSE;
	}
	sprintf(m_szMsg, "connect ok. sockfd:%d, url(%s)", m_sock, url);
	return TRUE;
}

VOID CTargetRcv::CloseSock()
{
	nn_close(m_sock);
}

int	CTargetRcv::RecvData(_Out_ void* pRecvBuf)
{
	void* pBuf;
	int nRecvLen = nn_recv(m_sock, &pBuf, NN_MSG, 0);
	if (nRecvLen < 1)
		return nRecvLen;
	memcpy(pRecvBuf, pBuf, nRecvLen);
	nn_freemsg(pBuf);
	return nRecvLen;
}

INT	 CTargetRcv::RecvData(_In_ int nBufLen, _Out_ void* pRecvBuf)
{
	int nRecvLen = nn_recv(m_sock, pRecvBuf, nBufLen, 0);
	
	return nRecvLen;
}