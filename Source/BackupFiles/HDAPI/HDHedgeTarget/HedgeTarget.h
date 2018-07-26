#pragma once

#include "afxwin.h"
#include <map>

class CHedgeTarget
{
public:
	CHedgeTarget();
	~CHedgeTarget();

	VOID	Add(CString sAcnt, CString sData, BOOL bLock=TRUE);
	VOID	Delete(CString sAcnt, BOOL bLock = TRUE);
	VOID	Reset();
	BOOL	IsTarget(CString& i_acno);
	VOID	LOCK() { EnterCriticalSection(&m_cs); };
	VOID	UNLOCK() { LeaveCriticalSection(&m_cs); };
	INT		GetCount() { return m_map.size(); };
	INT		GetAllData(CString *pOut);

private:
	std::map<CString, CString>	m_map;	// ACNT, DATA => πÃ¡§
	CRITICAL_SECTION	m_cs;
};

