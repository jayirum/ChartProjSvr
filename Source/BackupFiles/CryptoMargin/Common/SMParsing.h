#pragma once


#include <Windows.h>
#include "SMDefine.h"

#define MAX_BUF_LEN	10240


class CSMessage
{
public:
	CSMessage();
	virtual ~CSMessage();
	void	SetMsg(char* pzMsg);
	void	SetField(char* pzField);
	void	SetData(void* pData, int nDataSize);

	char m_zMsg[SM_MSG_LEN];
	char m_zField[SM_FIELD_LEN];
	char* m_pData;
};


class CSMParsing
{
public:
	CSMParsing();
	virtual ~CSMParsing();

	BOOL Begin();
	void End();

	int	GetOneSMessage(_Out_ CSMessage* pOut);
	int AddPacket(_In_ char* pRecvPacket, _In_ int nPackSize);
	void RemoveAll();
private:
	VOID Lock() { EnterCriticalSection(&m_cs); }
	VOID Unlock() { LeaveCriticalSection(&m_cs); }
private:
	char*	m_p;
	char*	m_pCurr;
	CRITICAL_SECTION	m_cs;
};