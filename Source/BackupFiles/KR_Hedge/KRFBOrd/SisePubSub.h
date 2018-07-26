#pragma once


/*
	KRChart �� ���� ���Ź��� �ü��� ���� �����忡�� publish / subscribe �Ѵ�.
*/
#include "nanomsg/nn.h"
#include "nanomsg/pubsub.h"
#include "nanomsg/inproc.h"

enum TP_PUBSUB {TP_PUB, TP_SUB};
#define DEF_PUBSUB_URL	"ipc://FBORD"
class CSisePubSub 
{
public:
	CSisePubSub(TP_PUBSUB tp);
	~CSisePubSub();

	BOOL	InitSock();
	VOID	CloseSock();

	int		Publish(void* pSendBuf, int nSendSize);
	int		Subscribe(void* pRecvBuf);

private:
	BOOL	InitPub();
	BOOL	InitSub();
private:
	TP_PUBSUB		m_tp;
	int				m_sock;
	char			m_szMsg[1024];
};

