#pragma once



#include "../Common/NanoQPipe.h"
#include "../Common/NanoQPubSub.h"

using namespace _BARELAYEA;

class CRelay : public CNanoQReader
{
public:
	CRelay();
	~CRelay();

	BOOL Begin();
	void End() {};
	static void WINAPI RecvProc(void *pClassInstance, char *pRecvBuf, int nRecvLen);	// class instance pointer, recv data, return value

private:
	CNanoQPub		publisher;
	char			m_zMsg[1024];

};

