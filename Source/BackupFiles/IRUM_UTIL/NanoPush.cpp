#include "NanoPush.h"
#include <stdio.h>


CNanoPush::CNanoPush()
{
	m_bConn = FALSE;
}


CNanoPush::~CNanoPush()
{
	End();
}


BOOL CNanoPush::Begin(char* ip, char* port)
{
	strcpy(m_zIP, ip);
	strcpy(m_zPort, port);

	/*  Create the socket. */
	m_sockSvr = nn_socket(AF_SP, NN_PUSH);
	if (m_sockSvr < 0) {
		sprintf(m_szMsg, "nn_socket err: %s", nn_strerror(nn_errno()));
		return FALSE;
	}

	/*  Bind to the URL.  This will bind to the address and listen
	synchronously; new clients will be accepted asynchronously
	without further action from the calling program. */
	char url[32];
	sprintf(url, "tcp://%s:%s", m_zIP, m_zPort);
	if (nn_bind(m_sockSvr, url) < 0) {
		sprintf(m_szMsg, "nn_bind error: %s", nn_strerror(nn_errno()));
		nn_close(m_sockSvr);
		return FALSE;
	}
	m_bConn = TRUE;
	sprintf(m_szMsg, "Init ok(sockfd:%d, url:%s", m_sockSvr, url);
	return TRUE;
}

VOID CNanoPush::End()
{
	nn_close(m_sockSvr);
}

INT	CNanoPush::SendData(void* p, int size)
{
	if (!m_bConn)
	{
		if (!Begin(m_zIP, m_zPort))
			return -1;
	}

	int len = nn_send(m_sockSvr, p, size, NN_DONTWAIT);
	if (len < 0)
	{
		int err = nn_errno();
		switch (err)
		{
		case EBADF:	sprintf(m_szMsg, "SEND ERROR - EBADF"); break;
		case ENOTSUP:	sprintf(m_szMsg, "SEND ERROR - ENOTSUP"); break;
		case EFSM:	sprintf(m_szMsg, "SEND ERROR - EFSM"); break;
		case EAGAIN:	// non-block 에서는 오류 아니다.
			//sprintf(m_szMsg, "SEND ERROR - EAGAIN"); 
			len = 0;
			break;
		case EINTR:	sprintf(m_szMsg, "SEND ERROR - EINTR"); break;
		case ETIMEDOUT:	sprintf(m_szMsg, "SEND ERROR - ETIMEOUT"); break;
		case ETERM:	sprintf(m_szMsg, "SEND ERROR - ETERM"); break;
		case 0: len = 0; break;
		default:sprintf(m_szMsg, "SEND ERROR - %d(%s)", err, nn_strerror(err)); break;

		}
	}
	return len;
}