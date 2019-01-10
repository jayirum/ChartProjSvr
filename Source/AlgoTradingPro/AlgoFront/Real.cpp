#include "Main.h"
#include "Real.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/logmsg.h"

extern char		g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;
extern CMemPool	g_memPool;
extern BOOL		g_bContinue;


CReal::CReal()
{
	InitializeCriticalSection(&m_cs);
}


CReal::~CReal()
{
}

BOOL CReal::Initialize()
{
	ResumeThread();
	return TRUE;
}


void CReal::Finalize()
{
	DeleteCriticalSection(&m_cs);
}

VOID CReal::ThreadFunc()
{
	char zSymbol[32];

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0)
		{
			//g_log.log(ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
			continue;
		}

		char* pBuf = NULL;
		
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case _ALGOINC::WM_DATAFEED:
			{
				int len = (int)msg.wParam;
				_ALGOINC::ST_TICK_API* p = (_ALGOINC::ST_TICK_API*)msg.lParam;
				sprintf(zSymbol, "%.*s", sizeof(p->ShortCode), p->ShortCode);
				lock();
				std::map<std::string, DWORD>::iterator it = m_mapSymbol.find(std::string(zSymbol));
				if (it != m_mapSymbol.end())
				{
					char* pData = g_memPool.get();
					sprintf(pData, "%.*s", len, (char*)p);
					PostThreadMessage((*it).second, _ALGOINC::WM_DATAFEED, len, (LPARAM)pData);
				}
				g_memPool.release((char*)msg.lParam);
				break;
			}
			case _ALGOINC::WM_ADD_CLIENT:
				AddClient((char*)msg.lParam, (SOCKET)msg.wParam);
				break;
			case _ALGOINC::WM_RELEASE_CLIENT:
				ReleaseClient((char*)msg.lParam, (SOCKET)msg.wParam);
				break;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	}
}


DWORD WINAPI CReal::SendThread(LPVOID lp)
{
	CReal* p = (CReal*)lp;
	std::set<SOCKET> setSock;
	char zMySymbol[32];

	while (g_bContinue)
	{
		char* pBuf = NULL;

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case _ALGOINC::WM_DATAFEED:
			{
				int len = (int)msg.wParam;
				p->SendData(&setSock, pBuf, len);
				g_memPool.release((char*)msg.lParam);
				break;
			}
			case _ALGOINC::WM_ADD_CLIENT:
				strcpy(zMySymbol, (char*)msg.lParam);
				g_memPool.release((char*)msg.lParam);
				setSock.insert((SOCKET)msg.wParam);
				break;
			case _ALGOINC::WM_RELEASE_CLIENT:
				setSock.erase((SOCKET)msg.wParam);
				break;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	}
	return 0;
}


VOID CReal::SendData(std::set<SOCKET>* setSock, char* pData, int nLen)
{
	lock();
	std::set<SOCKET>::iterator it;
	for (it = setSock->begin(); it != setSock->end(); it++)
	{
		char* pIoBuffer = g_memPool.get();
		_ALGOINC::RequestSendIO(*it, _ALGOINC::SYSTP_FRONT, pData, nLen, pIoBuffer);
	}
}

VOID CReal::AddClient(char* pzSymbol, SOCKET sock)
{
	std::string symbol(pzSymbol);
	DWORD dwID;
	lock();
	std::map<std::string, DWORD>::iterator it = m_mapSymbol.find(symbol);
	if (it == m_mapSymbol.end())
	{
		CreateThread(NULL, 0, SendThread, this, 0, &dwID);
		m_mapSymbol[symbol] = dwID;
	}
	else
	{
		dwID = (*it).second;
	}

	unlock();

	char* pUserID = g_memPool.get();
	strcpy(pUserID, pzSymbol);
	PostThreadMessage(dwID, _ALGOINC::WM_ADD_CLIENT, (WPARAM)sock, (LPARAM)pUserID);
}

VOID CReal::ReleaseClient(char* pzSymbol, SOCKET sock)
{
	std::string symbol(pzSymbol);
	DWORD dwID;
	lock();
	std::map<std::string, DWORD>::iterator it = m_mapSymbol.find(symbol);
	if (it != m_mapSymbol.end())
	{
		dwID = (*it).second;

		char* pUserID = g_memPool.get();
		strcpy(pUserID, pzSymbol);
		PostThreadMessage(dwID, _ALGOINC::WM_RELEASE_CLIENT, (WPARAM)sock, (LPARAM)0);
	}
	unlock();
}


