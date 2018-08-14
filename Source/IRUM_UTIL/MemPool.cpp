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

//bool CMemPool::Begin()
//{}

void CMemPool::End()
{
	clear();
	DeleteCriticalSection(&g_cs);
}

CMemPool::~CMemPool()
{
	End();
}

void CMemPool::clear()
{
	LOCK();
	try {
		std::list<char*>::iterator it;
		for (it = m_listPool.begin(); it != m_listPool.end(); it++)
		{
			char* p = *it;
			delete[] p;
		}
		m_listPool.clear();
	}
	catch(...) {
		UNLOCK();
		return;
	}
	UNLOCK();
}

bool	CMemPool::get(_Out_ char** pBuf)
{
	
	LOCK();
	try
	{
		if (m_listPool.size()==0)
		{
			*pBuf = new char[m_nBlockSize];
			//printf("new.....%x\n", *pBuf);
		}
		else
		{
			
			char* p = *m_listPool.begin();
			m_listPool.pop_front();

			//printf("\tmem pop:%d.%x\n", m_listPool.size(), p);
			if (!p)
				*pBuf = new char[m_nBlockSize];
			else
				*pBuf = p;

		}
			
		ZeroMemory(*pBuf, m_nBlockSize);
	}
	catch(...)
	{
		UNLOCK();
		return (*pBuf!=NULL);
	}
	UNLOCK();
	return (*pBuf!=NULL);
}

void	CMemPool::release(char* ptr)
{
//	printf("\t\t-----------release(%x)\n", ptr);
	if (!ptr)
		return;
	LOCK();
	__try {
		if (m_listPool.size() < m_nMaxAlloc) {
			m_listPool.push_back(ptr);
			//printf("mem push:%d.%x\n", m_listPool.size(),ptr);
		}			
		else
			delete[] ptr;
	}
	__finally {
		UNLOCK();
	}
}
