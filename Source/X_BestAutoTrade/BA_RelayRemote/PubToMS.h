#pragma once
#include "../Common/BARelayInc.h"
#include "../Common/NanoQPubSub.h"
#include <string>

#define SEND_TIMEOUT_MS	100

using namespace _BA_RELAY;
using namespace std;

class CPubToMS
{
public:
	CPubToMS();
	~CPubToMS();

	BOOL InitChannel(CLIENT_TP clTp);
	void DeInitChannel();

	void PubData(char* pData, int nSendLen);

private:
	char		m_zChannelNm[128];
	CNanoQPub*	m_pub;
	CLIENT_TP	m_clTp;
	string		m_sMyType;
};

