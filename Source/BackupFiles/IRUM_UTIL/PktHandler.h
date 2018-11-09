#pragma once

#include <windows.h>
#include <string>
using namespace std;

class CPktHandler  
{
public:
	LPCSTR GetStrBuff();
	CPktHandler();
	virtual ~CPktHandler();

	BOOL	AddPkt(char* pBuf, int nSize);
	int		GetOnePkt(/*out*/char* pOutBuf);

private:
	int		GetOnePktFn(/*out*/char* pOutBuf);
	VOID	Erase(int nStartPos, int nLen);
	VOID	RemoveAll(BOOL bLock);
	
	VOID	Lock() { EnterCriticalSection(&m_cs); }
	VOID	Unlock() { LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION	m_cs;
	string				m_strBuf;

		

};
