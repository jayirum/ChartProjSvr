#include "SymbolPosition.h"


CSymbolPosition::CSymbolPosition()
{
	InitializeCriticalSection(&m_cs);
}


CSymbolPosition::~CSymbolPosition()
{
	DeleteCriticalSection(&m_cs);
}

// ü����Ŷ �޾Ƽ� ó��
CNTR_FLAG CSymbolPosition::CntrProc(_In_ char* pOrdPack)
{
	Lock();

	Unlock();
	return CNTR_OPEN;
}