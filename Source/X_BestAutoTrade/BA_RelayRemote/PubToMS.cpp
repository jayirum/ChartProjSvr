
#include "PubToMS.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"
#include "../Common/BAProtocol.h"

extern char		g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;


CPubToMS::CPubToMS()
{
}


CPubToMS::~CPubToMS()
{
	delete m_pub;
}


BOOL CPubToMS::InitChannel(CLIENT_TP clTp)
{
	m_pub = new CNanoQPub;
	m_clTp = clTp;

	char szIp[32], szPort[32], szChannel[32];
	CUtil::GetConfig(g_zConfig, "RELAY_CHANNEL_INFO", "RELAY_IP", szIp);

	if (m_clTp == CLIENT_TP::MASTR) {
		CUtil::GetConfig(g_zConfig, "RELAY_CHANNEL_INFO", "PORT_PUB2MASTER", szPort);
		m_sMyType = "MASTER";
	}
	else {
		CUtil::GetConfig(g_zConfig, "RELAY_CHANNEL_INFO", "PORT_PUB2SLAVE", szPort);
		m_sMyType = "SLAVE";
	}

	sprintf(szChannel, "%s:%s", szIp, szPort);

	if (!m_pub->Begin(szChannel, SEND_TIMEOUT_MS, QUEUE_TP::TP_TCP, FALSE))
	{
		g_log.log(ERR, "CPubToMS(%s) InitChannel error(%s)\n",m_sMyType.c_str(), szChannel, m_pub->GetMsg());
		return FALSE;
	}
	g_log.log(INFO, "CPubToMS(%s) InitChannel Ok(%s)\n", m_sMyType.c_str(), szChannel);

	return TRUE;
}

void CPubToMS::DeInitChannel()
{
	m_pub->End();
}

// ALL = 0, SPECIFIC_ID
void CPubToMS::PubData(char* pData, int nSendLen)
{
	int nErr;
	int ret = m_pub->SendData(pData, nSendLen, &nErr);
	g_log.log(INFO, "[Publish %s ](%s)", m_sMyType.c_str(), pData);
}