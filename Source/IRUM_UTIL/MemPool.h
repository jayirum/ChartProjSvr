// Log.h: interface for the CLogMsg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once 

#include <stdio.h>
#include <list>
#include <Windows.h>


class CMemPool
{
public:
	CMemPool(int nPreAlloc, int nMaxAlloc, int nBlockSize);
	virtual ~CMemPool();

	//bool	Begin();
	void	End();
	bool	get(_Out_ char** pBuf);
	char*	get();
	void	release(char* ptr);
	int		available() { return m_listPool.size(); }
private:
	void	clear();


	void LOCK() {EnterCriticalSection(&m_cs);}
	void UNLOCK() {LeaveCriticalSection(&m_cs);}
private:

	std::size_t			m_nPrevAlloc, m_nMaxAlloc;
	std::size_t			m_nBlockSize;
	std::list<char*>		m_listPool;
	

	CRITICAL_SECTION   m_cs;
};
