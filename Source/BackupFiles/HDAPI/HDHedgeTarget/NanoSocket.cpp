#include "stdafx.h"
#include "NanoSocket.h"


CNanoSocket::CNanoSocket()
{
}


CNanoSocket::~CNanoSocket()
{
	CloseServerSock();
}


BOOL CNanoSocket::InitServerSock(char* ip, char* port)
{
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
	sprintf(url, "tcp://%s:%s", ip, port);
	if (nn_bind(m_sockSvr, url) < 0) {
		sprintf(m_szMsg, "nn_bind error: %s", nn_strerror(nn_errno()));
		nn_close(m_sockSvr);
		return FALSE;
	}
	sprintf(m_szMsg, "Init ok(sockfd:%d, url:%s", m_sockSvr, url);
	return TRUE;
}

VOID CNanoSocket::CloseServerSock()
{
	nn_close(m_sockSvr);
}

INT	CNanoSocket::SendData(void* p, int size)
{
	/*
	to send a buffer allocated by nn_allocmsg(3) function set the buf parameter to point to the pointer to the buffer 
	and len parameter to NN_MSG constant. In this case the buffer will be deallocated by nn_send function. 
	Trying to deallocate it afterwards will result in undefined behaviour.
	*/
	void *msg = nn_allocmsg(size, 0);
	memcpy(msg, p, size);

	int len = nn_send(m_sockSvr, msg, size, NN_DONTWAIT);
	if (len < 0)
	{
		int err = errno;
		switch (err)
		{
		case EBADF:	sprintf(m_szMsg, "SEND ERROR - EBADF"); break;
		case ENOTSUP:	sprintf(m_szMsg, "SEND ERROR - ENOTSUP"); break;
		case EFSM:	sprintf(m_szMsg, "SEND ERROR - EFSM"); break;
		case EAGAIN:	sprintf(m_szMsg, "SEND ERROR - EAGAIN"); break;
		case EINTR:	sprintf(m_szMsg, "SEND ERROR - EINTR"); break;
		case ETIMEDOUT:	sprintf(m_szMsg, "SEND ERROR - ETIMEOUT"); break;
		case ETERM:	sprintf(m_szMsg, "SEND ERROR - ETERM"); break;
		case 0: len = 0; break;
		default:sprintf(m_szMsg, "SEND ERROR - %d(%s)", err, nn_strerror(err)); break;

		}
	}
	return len;
}