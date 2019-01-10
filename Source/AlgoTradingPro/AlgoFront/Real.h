#pragma once

#include "../Common/AlgoInc.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include <map>
#include <set>
#include <string>


class CReal : public CBaseThread 
{
public:
	CReal();
	~CReal();

	BOOL Initialize();
	void Finalize();

	VOID ThreadFunc();
	static DWORD WINAPI SendThread(LPVOID lp);

	VOID SendData(std::set<SOCKET>* setSock, char* pData, int nLen);
	VOID AddClient(char* pzSymbol, SOCKET sock);
	VOID ReleaseClient(char* pzSymbol, SOCKET sock);
	void lock() { EnterCriticalSection(&m_cs); }
	void unlock() { LeaveCriticalSection(&m_cs); }

private:

	std::map<std::string, DWORD>	m_mapSymbol;	// SYMBOL, THREAD ID
	CRITICAL_SECTION	m_cs;
};