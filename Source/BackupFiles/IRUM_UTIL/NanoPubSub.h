#pragma once


/*
	KRChart 로 부터 수신받은 시세를 내부 스레드에게 publish / subscribe 한다.
*/
#include "nanomsg/nn.h"
#include "nanomsg/pubsub.h"
#include "nanomsg/inproc.h"
#pragma warning(disable:4996)

enum TP_PUBSUB {TP_PUB, TP_SUB};
//#define DEF_PUBSUB_URL	"ipc://FBORD"
class CNanoPubSub 
{
public:
	CNanoPubSub(TP_PUBSUB tp);
	~CNanoPubSub();

	bool	Begin(char* pzChannelNm);
	bool	BeginTcp(char* pzIP, char* pzPort);
	void	End();

	int		Publish(char* pSendBuf, int nSendSize);
	int		Subscribe(void* pRecvBuf);
	char*	GetMsg() {return m_szMsg; }
	int 	DumpErr();
private:
	bool	InitPub();
	bool	InitSub();
private:
	TP_PUBSUB		m_tp;
	int				m_sock;
	char			m_szChannelNm[256];
	char			m_szMsg[1024];
};

