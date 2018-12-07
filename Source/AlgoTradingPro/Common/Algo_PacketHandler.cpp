#include "Algo_PacketHandler.h"
#include "../../IRUM_UTIL/Irum_Common.h"
#include "../../IRUM_UTIL/LogMsg.h"

extern CLogMsg	g_log;

CPacketHandler::CPacketHandler()
{
	InitializeCriticalSection(&m_cs);
}


CPacketHandler::~CPacketHandler()
{
	m_buf.clear();
	DeleteCriticalSection(&m_cs);
}


void CPacketHandler::add(char* pRecvData, int nRecvLen)
{
	try {
		lock();
		m_buf += std::string(pRecvData, nRecvLen);
	}
	catch (...)
	{
		g_log.log(LOGTP_ERR, "CPacketHandler::add try catch....");
		unlock();
		return;
	}

	unlock();
}

int CPacketHandler::getonepacket(_Out_ char* pzOutBuf)
{
	lock();
	int ret;
	try {
		ret = getpack(pzOutBuf);
	}
	catch(...)
	{
		g_log.log(LOGTP_ERR, "CPacketHandler::getonepacket try catch....");
		unlock();
		return 0;
	}
	unlock();
	return ret;
}


/*
	패킷상 len 에는 EOL 은 포함하지 않는다.
*/
int CPacketHandler::getpack(_Out_ char* pzOutBuf)
{
	if (m_buf.size() ==0 )
		return 0;

	//제일 마지막은 EOL 이다. EOL 이 없으면 나간다.
	int eol = m_buf.find_first_of(DEF_EOL);
	if (eol == m_buf.npos)
		return 0;

	// 첫 3바이트는 패킷 길이이다.
	char zLen[4] = { 0, };
	sprintf(zLen, "%.3s", m_buf.c_str());
	int nPackLen = atoi(zLen);

	// 길이 이상. 패킷 버린다.
	if (nPackLen <= 1) {
		g_log.log(LOGTP_ERR, "[getpack]길이이상.버린다.(%.*s)", eol + 1, m_buf.c_str());
		m_buf.erase(0, eol);
		return 0;
	}

	// 패킷상 길이와 eol 이 일치하면 OK
	if (m_buf[nPackLen] == DEF_EOL)
	{
		strncpy(pzOutBuf, m_buf.c_str(), nPackLen);
		m_buf.erase(0, nPackLen+1);	//EOL 까지 지운다.
		return nPackLen;
	}

	//// 실제길이
	//int nRealLen = m_buf.size();

	//// 실제길이가 짧으면 아직 미완의 패킷이므로 기다린다.
	//if (nPackLen > nRealLen)
	//	return 0;

	//

	//// EOL 제거
	//int eol = m_buf.find_first_of(DEF_EOL);
	//if (eol != m_buf.npos)
	//{
	//	m_buf.erase(eol,1);
	//}

	return 0;
}
