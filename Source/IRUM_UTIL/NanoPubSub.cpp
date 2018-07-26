#include "NanoPubsub.h"
#include <stdio.h>
#include <memory.h>

CNanoPubSub::CNanoPubSub(TP_PUBSUB tp)
{
	m_tp = tp;
}


CNanoPubSub::~CNanoPubSub()
{
	End();
}


bool CNanoPubSub::Begin(char* pzChannelNm)
{
	sprintf(m_szChannelNm, "ipc://%s", pzChannelNm);

	/*  Create the socket. */
	if (m_tp == TP_PUB)
		return InitPub();
	return InitSub();

}


bool CNanoPubSub::BeginTcp(char* pzIP, char* pzPort)
{
	sprintf(m_szChannelNm, "tcp://%s:%s", pzIP, pzPort);
	/*  Create the socket. */
	if (m_tp == TP_PUB)
		return InitPub();
	return InitSub();

}


bool CNanoPubSub::InitPub()
{
	m_sock = nn_socket(AF_SP, NN_PUB);
	if (m_sock < 0) {
		sprintf(m_szMsg, "nn_socket err: %s", nn_strerror(nn_errno()));
		return false;
	}

	/*  Bind to the URL.  This will bind to the address and listen
	synchronously; new clients will be accepted asynchronously
	without further action from the calling program. */
	if (nn_bind(m_sock, m_szChannelNm) < 0) {
		sprintf(m_szMsg, "nn_bind error: %s", nn_strerror(nn_errno()));
		nn_close(m_sock);
		return false;
	}

	sprintf(m_szMsg, "Init Pub OK(%s)", m_szChannelNm);
	return true;
}



bool CNanoPubSub::InitSub()
{
	m_sock = nn_socket(AF_SP, NN_SUB);
	if (m_sock < 0) {
		sprintf(m_szMsg, "nn_socket err: %s", nn_strerror(nn_errno()));
		return false;
	}

	/*  Bind to the URL.  This will bind to the address and listen
	synchronously; new clients will be accepted asynchronously
	without further action from the calling program. */
	if (nn_connect(m_sock, m_szChannelNm) < 0) {
		sprintf(m_szMsg, "nn_bind error: %s", nn_strerror(nn_errno()));
		nn_close(m_sock);
		return false;
	}

	if (nn_setsockopt(m_sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0) {
		sprintf(m_szMsg, "nn_setsockopt error: %s", nn_strerror(nn_errno()));
		nn_close(m_sock);
		return false;
	}

	sprintf(m_szMsg, "Init Sub OK(%s)", m_szChannelNm);
	return true;
}



void CNanoPubSub::End()
{
	nn_close(m_sock);
}

int	CNanoPubSub::Publish(char* pSendBuf, int nSendSize)
{
	//buffer 를 생성해서 해보자
	void *buf = nn_allocmsg(nSendSize, 0);
	memcpy(buf, pSendBuf, nSendSize);

	//int nRecvLen = nn_send(m_sock, buf, nSendSize, NN_DONTWAIT);
	int nRecvLen = nn_send(m_sock, buf, nSendSize, 0);
	if (nRecvLen < 0)
	{
		nRecvLen = DumpErr();
	}
	return nRecvLen;
}


int	CNanoPubSub::Subscribe(void* pRecvBuf)
{
	void* pBuf;
	//int nRecvLen = nn_recv(m_sock, &pBuf, NN_MSG, NN_DONTWAIT);
	int nRecvLen = nn_recv(m_sock, &pBuf, NN_MSG, 0);
	if (nRecvLen > 0) {
		memcpy(pRecvBuf, pBuf, nRecvLen);
		nn_freemsg(pBuf);
	}
	else
	{
		nRecvLen = DumpErr();
	}
	return nRecvLen;
}

int CNanoPubSub::DumpErr()
{
	int nRet = -1;
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
	case 0: nRet = 0; break;
	default:sprintf(m_szMsg, "SEND ERROR - %d(%s)", err, nn_strerror(err)); break;
	}
	return nRet;
}
