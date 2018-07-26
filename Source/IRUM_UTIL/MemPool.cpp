// Log.cpp: implementation of the CLogMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "MemPool.h"
#include <Windows.h>

#pragma warning(disable:4996)

CRITICAL_SECTION   g_cs;
#define LOCK() {EnterCriticalSection(&g_cs);}
#define UNLOCK() {LeaveCriticalSection(&g_cs);}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMemPool::CMemPool(int nPreAlloc, int nMaxAlloc, int nBlockSize)
{
	m_nBlockSize = nBlockSize;
	m_nPrevAlloc = nPreAlloc;
	m_nMaxAlloc = nMaxAlloc;
	InitializeCriticalSection(&g_cs);

	LOCK();
	for (std::size_t i = 0; i < m_nPrevAlloc; i++)
	{
		char* p = new char[nBlockSize];
		ZeroMemory(p, m_nBlockSize);
		m_listPool.push_back(p);
	}
	UNLOCK();

}

CMemPool::~CMemPool()
{
	clear();
	DeleteCriticalSection(&g_cs);
}

void CMemPool::clear()
{
	LOCK();
	__try {
		std::list<char*>::iterator it;
		for (it = m_listPool.begin(); it != m_listPool.end(); it++)
		{
			char* p = *it;
			delete[] p;
		}
		m_listPool.clear();
	}
	__finally {
		UNLOCK();
	}
}

char*	CMemPool::get()
{
	char* p;
	LOCK();
	__try
	{
		if (m_listPool.empty())
		{
			p = new char[m_nBlockSize];
		}
		else
		{
			
			p = *m_listPool.begin();
			m_listPool.pop_front();
		}
		ZeroMemory(p, m_nBlockSize);
	}
	__finally
	{
		UNLOCK();
	}
	return p;
}

void	CMemPool::release(char* ptr)
{
	LOCK();
	__try {
		if (m_listPool.size() < m_nMaxAlloc)
			m_listPool.push_back(ptr);
		else
			delete[] ptr;
	}
	__finally {
		UNLOCK();
	}
}
