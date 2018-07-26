#pragma once
#pragma warning(disable:4996)

/*
	KRChart 로 부터 수신받은 시세를 내부 스레드에게 publish / subscribe 한다.
*/
#include "nanomsg/nn.h"
#include "nanomsg/pubsub.h"
#include "nanomsg/inproc.h"


enum TP_PUBSUB {TP_PUB, TP_SUB};
#define DEF_PUBSUB_URL	"ipc://IRUMChart"
class CSisePubSub 
{
public:
	CSisePubSub(TP_PUBSUB tp);
	~CSisePubSub();

	bool	InitSock();
	void	CloseSock();

	int		Publish(void* pSendBuf, int nSendSize);
	int		Subscribe(void* pRecvBuf);

private:
	bool	InitPub();
	bool	InitSub();
private:
	TP_PUBSUB		m_tp;
	int				m_sock;
	char			m_szMsg[1024];
};

