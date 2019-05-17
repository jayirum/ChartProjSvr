#pragma once
#pragma warning(disable:4996)
#include <string>
#include <Windows.h>

class CPacketHandler
{
public:
	CPacketHandler();
	~CPacketHandler();

	void add(char* pRecvData, int nRecvLen);
	int getonepacket(_Out_ char* pzOutBuf);
	
private:
	int getpack(_Out_ char* pzOutBuf);
	void lock() { EnterCriticalSection(&m_cs); }
	void unlock() { LeaveCriticalSection(&m_cs); }

private:
	std::string	m_buf;
	int			m_totLen;
	int			m_pos;
	CRITICAL_SECTION	m_cs;
};

