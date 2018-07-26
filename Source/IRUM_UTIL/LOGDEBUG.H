// LogDebug.h: interface for the CLogDebug class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGDEBUG_H__6441E91E_F739_4189_A026_AE3A425921D8__INCLUDED_)
#define AFX_LOGDEBUG_H__6441E91E_F739_4189_A026_AE3A425921D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include "SvrLog.h"

class CLogDebug  
{
public:
	CLogDebug();
	CLogDebug(LPCTSTR lpSvcName) ;	
	virtual ~CLogDebug();
	void RegEventSrc();
	void LogEventWarning(DWORD dwEventID,LPCTSTR pFormat, ...);
	void LogEventInf(DWORD dwEventID,LPCTSTR pFormat, ...);
	void LogEventErr(DWORD dwEventID,LPCTSTR pFormat, ...);
	void LogEventLowData(int Type,DWORD dwEventID,DWORD dwLowDataSize,LPVOID lpRawData,LPCTSTR pFormat, ...);
	void LogEvent(int Type,DWORD dwEventID,LPCTSTR pFormat, ...);

	void SetServiceName(char* lpSvcName);
	errno_t OpenLogFile(char* Dir);
	void WriteLogFile(char* Msg);
	void CloseLogFile();

protected:
	HANDLE m_hEventSource;
	BOOL AddEventSource();
	char	m_szServiceName[128];
	char	m_szFileName[128];
	int		m_nLog;
};

#endif // !defined(AFX_LOGDEBUG_H__6441E91E_F739_4189_A026_AE3A425921D8__INCLUDED_)
