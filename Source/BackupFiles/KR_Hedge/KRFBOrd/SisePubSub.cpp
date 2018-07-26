#include "stdafx.h"
#include "SisePubsub.h"


CSisePubSub::CSisePubSub(TP_PUBSUB tp)
{
	m_tp = tp;
}


CSisePubSub::~CSisePubSub()
{
	CloseSock();
}


BOOL CSisePubSub::InitSock()
{
	/*  Create the socket. */
	if (m_tp == TP_PUB)
		return InitPub();
	return InitSub();

}


BOOL CSisePubSub::InitPub()
{
	m_sock = nn_socket(AF_SP, NN_PUB);
	if (m_sock < 0) {
		sprintf(m_szMsg, "nn_socket err: %s", nn_strerror(nn_errno()));
		return FALSE;
	}

	/*  Bind to the URL.  This will bind to the address and listen
	synchronously; new clients will be accepted asynchronously
	without further action from the calling program. */
	if (nn_bind(m_sock, DEF_PUBSUB_URL) < 0) {
		sprintf(m_szMsg, "nn_bind error: %s", nn_strerror(nn_errno()));
		nn_close(m_sock);
		return FALSE;
	}

	return TRUE;
}



BOOL CSisePubSub::InitSub()
{
	m_sock = nn_socket(AF_SP, NN_SUB);
	if (m_sock < 0) {
		sprintf(m_szMsg, "nn_socket err: %s", nn_strerror(nn_errno()));
		return FALSE;
	}

	/*  Bind to the URL.  This will bind to the address and listen
	synchronously; new clients will be accepted asynchronously
	without further action from the calling program. */
	if (nn_connect(m_sock, DEF_PUBSUB_URL) < 0) {
		sprintf(m_szMsg, "nn_bind error: %s", nn_strerror(nn_errno()));
		nn_close(m_sock);
		return FALSE;
	}

	if (nn_setsockopt(m_sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0) {
		sprintf(m_szMsg, "nn_setsockopt error: %s", nn_strerror(nn_errno()));
		nn_close(m_sock);
		return FALSE;
	}


	return TRUE;
}



VOID CSisePubSub::CloseSock()
{
	nn_close(m_sock);
}

int	CSisePubSub::Publish(void* pSendBuf, int nSendSize)
{
	int nRecvLen = nn_send(m_sock, pSendBuf, nSendSize, NN_DONTWAIT);
	return nRecvLen;
}


int	CSisePubSub::Subscribe(void* pRecvBuf)
{
	void* pBuf;
	int nRecvLen = nn_recv(m_sock, &pBuf, NN_MSG, NN_DONTWAIT);
	if (nRecvLen > 0) {
		memcpy(pRecvBuf, pBuf, nRecvLen);
		nn_freemsg(pBuf);
	}
	else
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
		case 0: nRecvLen = 0; break;
		default:sprintf(m_szMsg, "SEND ERROR - %d(%s)", err, nn_strerror(err)); break;

		}
	}
	return nRecvLen;
}

