#pragma once

#include "../../IRUM_UTIL/BaseThread.h"
#include "Poco/MemoryPool.h"
#include "../../IRUM_UTIL/Screwdb.h"
//
using namespace Poco;


#define WM_COPYTHREAD_RECV	WM_USER + 5001
#define WM_COPYTHREAD_DIE	WM_USER + 5002


class CCopyOrder : public CBaseThread
{
public:
	CCopyOrder(char* zAcntNo, char* zAcntPwd, CESApi *Api, CRITICAL_SECTION* csApi, 
					Poco::MemoryPool* pMemPool, CDBPool *pDBPool, HWND hWnd, UINT unMultiple);
	~CCopyOrder();

	VOID	ThreadFunc();
	BOOL	SaveTradeInfo(CString& sShortCode, char* pTarget, char* pHedge);


	char* Acnt() { return m_zAcntNo; };
	char* Pwd() { return m_zAcntPwd; };
	unsigned int ThrId() { return m_dwThreadID; };
private:
	char m_zAcntNo[32];
	char m_zAcntPwd[32];
	CESApi				*m_Api;
	CRITICAL_SECTION	*m_csApi;
	Poco::MemoryPool*	m_pMemPool;
	HWND				m_hWnd;
	CDBPool				*m_pDBPool;
	UINT				m_unMultiple;
};