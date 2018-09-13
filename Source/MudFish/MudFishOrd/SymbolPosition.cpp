#include "SymbolPosition.h"


CSymbolPosition::CSymbolPosition()
{
	InitializeCriticalSection(&m_cs);
}


CSymbolPosition::~CSymbolPosition()
{
	DeleteCriticalSection(&m_cs);
}

// 체결패킷 받아서 처리
CNTR_FLAG CSymbolPosition::CntrProc(_In_ char* pOrdPack)
{
	Lock();

	Unlock();
	return CNTR_OPEN;
}