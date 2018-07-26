// Log.h: interface for the CLogMsg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once 

#include <stdio.h>
#include <list>



class CMemPool
{
public:
	CMemPool(int nPreAlloc, int nMaxAlloc, int nBlockSize);
	virtual ~CMemPool();

	char*	get();
	void	release(char* ptr);
	int		available() { return m_listPool.size(); }
private:
	void	clear();

private:

	std::size_t			m_nPrevAlloc, m_nMaxAlloc;
	std::size_t			m_nBlockSize;
	std::list<char*>		m_listPool;
	
		
};
