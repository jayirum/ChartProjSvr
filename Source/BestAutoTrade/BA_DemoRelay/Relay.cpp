#include "Relay.h"
#include "../Common/Demo.h"


CRelay::CRelay()
{
}


CRelay::~CRelay()
{
}


BOOL CRelay::Begin()
{
	if (!publisher.Begin("MyAccNo", DEF_SEND_TIMEOUT, TP_INTERPROC, FALSE))
	{
		strcpy(m_zMsg, publisher.GetMsg());
		printf("publisher.Begin error(%s)\n", m_zMsg);
		return FALSE;
	}
	printf("[%s]publisher.Begin Ok\n", publisher.ChannelNm());
	if (!publisher.Connect())
	{
		strcpy(m_zMsg, publisher.GetMsg());
		printf("publisher.Connect error(%s)\n", m_zMsg);
		return -1;
	}
	printf("[%s]publisher.Connect Ok\n", publisher.ChannelNm());

	if (!CNanoQReader::Begin(NULL, DEF_RECV_TIMEOUT, TP_INTERPROC, TRUE))
		return FALSE;

	if (!Connect())
	{
		strcpy(m_zMsg, GetMsg());
		printf("pipeReader.Connect error(%s)\n", m_zMsg);
		return -1;
	}
	printf("[%s]pipeReader.Connect Ok\n", ChannelNm());


	RegisterCallBack((LPVOID)this, (PRecvCallBack) RecvProc);

	return TRUE;
}

// class instance pointer, recv data, return value
void WINAPI CRelay::RecvProc(void *pClassInstance, char *pRecvBuf, int nRecvLen)
{
	printf("[RECV]%.*s\n", nRecvLen, pRecvBuf);

	CRelay* pThis = (CRelay*)pClassInstance;

	char zBuffer[1024] = { 0, };
	memcpy(zBuffer, pRecvBuf, nRecvLen);

	int nRet, nErrCode;
	nRet = pThis->publisher.SendData(zBuffer, nRecvLen, &nErrCode);
	if (nRet == Q_ERROR)
	{
		if (nErrCode == Q_ERROR)
		{
			printf("publisher.SendData error (%s)\n", pThis->publisher.GetMsg());
		}
		else
		{
			printf("publisher.SendData timeout.....\n");
		}
	}
	else
		printf("[SEND]%.*s\n", nRecvLen, zBuffer);

}