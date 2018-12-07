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
	��Ŷ�� len ���� EOL �� �������� �ʴ´�.
*/
int CPacketHandler::getpack(_Out_ char* pzOutBuf)
{
	if (m_buf.size() ==0 )
		return 0;

	//���� �������� EOL �̴�. EOL �� ������ ������.
	int eol = m_buf.find_first_of(DEF_EOL);
	if (eol == m_buf.npos)
		return 0;

	// ù 3����Ʈ�� ��Ŷ �����̴�.
	char zLen[4] = { 0, };
	sprintf(zLen, "%.3s", m_buf.c_str());
	int nPackLen = atoi(zLen);

	// ���� �̻�. ��Ŷ ������.
	if (nPackLen <= 1) {
		g_log.log(LOGTP_ERR, "[getpack]�����̻�.������.(%.*s)", eol + 1, m_buf.c_str());
		m_buf.erase(0, eol);
		return 0;
	}

	// ��Ŷ�� ���̿� eol �� ��ġ�ϸ� OK
	if (m_buf[nPackLen] == DEF_EOL)
	{
		strncpy(pzOutBuf, m_buf.c_str(), nPackLen);
		m_buf.erase(0, nPackLen+1);	//EOL ���� �����.
		return nPackLen;
	}

	//// ��������
	//int nRealLen = m_buf.size();

	//// �������̰� ª���� ���� �̿��� ��Ŷ�̹Ƿ� ��ٸ���.
	//if (nPackLen > nRealLen)
	//	return 0;

	//

	//// EOL ����
	//int eol = m_buf.find_first_of(DEF_EOL);
	//if (eol != m_buf.npos)
	//{
	//	m_buf.erase(eol,1);
	//}

	return 0;
}
