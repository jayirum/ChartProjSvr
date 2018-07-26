// LogDebug.cpp: implementation of the CLogDebug class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include <share.h>
//#include "../include/common.h"
#include "LogDebug.h"
#include "util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogDebug::CLogDebug()
{
	ZeroMemory(m_szServiceName,sizeof(m_szServiceName));
	RegEventSrc();
}


CLogDebug::CLogDebug(LPCTSTR lpSvcName):m_hEventSource(NULL),m_nLog(0)
{
	lstrcpy(m_szServiceName, lpSvcName);
	RegEventSrc();
}

CLogDebug::~CLogDebug()
{
	if(m_hEventSource != NULL)
		DeregisterEventSource(m_hEventSource);
	if(m_nLog>0)
		CloseLogFile();
}


/**	\brief	로그파일 생성

	\param	Dir		파일을 생성할 PATH

	\return	void
*/
errno_t CLogDebug::OpenLogFile(char* Dir)
{
	lstrcpy(m_szFileName, Dir);	
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(Dir, MAX_PATH, "\\%s_%04d%02d%02d.log", m_szServiceName, st.wYear, st.wMonth, st.wDay);
	lstrcat(m_szFileName, Dir);

	errno_t err = _sopen_s(&m_nLog, m_szFileName, _O_CREAT|_O_APPEND|_O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	return err;
}

/**	\brief	Log파일에 메시지 저장

	\param	Msg		저장할 문자열

	\return	void	
*/
void CLogDebug::WriteLogFile(char* Msg)
{
	if( m_nLog >0 )
		_write(m_nLog, Msg,strlen(Msg));
}

/**	\brief	로그파일 종료

	\return	void	
*/
void CLogDebug::CloseLogFile()
{
	_close(m_nLog);
}

/**	\brief	이벤트 소스 추가

	\return	BOOL

*/
BOOL CLogDebug::AddEventSource()
{
	HKEY  hk;//, hkDLSvr; 
    DWORD dwData; 
    UCHAR szBuf[256]; 
	// Get Exe File Path
	char sztBuf[256];
	lstrcpy(sztBuf, "SYSTEM\\CurrentControlSet\\Services\\");
	lstrcat(sztBuf, m_szServiceName);

    ////
	//// 이벤트 메세지 실행파일 등록
	////
	wsprintf((char *)szBuf,"SYSTEM\\CurrentControlSet\\Services" 
            "\\EventLog\\Application\\%s",m_szServiceName);
    if (RegCreateKey(HKEY_LOCAL_MACHINE, (const char *)szBuf, &hk))     //key 생성 
        return FALSE;

	/*
	CProp	clsProp;
	memset(szMsgDll, 0x00, sizeof(szMsgDll));
	clsProp.SetBaseKey(HKEY_LOCAL_MACHINE, REG_EVENTMSG_DLL);
	lstrcpy(szMsgDll, clsProp.GetValue("EventMsgDll"));
	clsProp.Close();
	*/
	char szMsgDll[128];
	CUtil::GetCnfgFile("common", "EVENTMSG", "EVENTMSGDLL", szMsgDll);
	////
	//// 이벤트 메세지 실행파일 등록
	////
 
    // Add the name to the EventMessageFile subkey.  
	DWORD 	dwRtc = RegSetValueEx(hk, "EventMessageFile", 0, REG_EXPAND_SZ,
			(LPBYTE) szMsgDll, strlen(szMsgDll) + 1);
	if (dwRtc != ERROR_SUCCESS)
	{
		RegCloseKey(hk);
		return FALSE;
	}
	
 
    // Set the supported event types in the TypesSupported subkey. 
    dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE; 
 
    if (RegSetValueEx(hk,      // subkey handle 
            "TypesSupported",  // value name 
            0,                 // must be zero 
            REG_DWORD,         // value type 
            (LPBYTE) &dwData,  // pointer to value data 
            sizeof(DWORD)))    // length of value data 
	{
		RegCloseKey(hk);
		return FALSE;
	}
 
    RegCloseKey(hk); 
	return TRUE;
} 


void CLogDebug::LogEventLowData(int Type,DWORD dwEventID,DWORD dwLowDataSize,LPVOID lpRawData,LPCTSTR pFormat, ...)
{
	if(!m_hEventSource) return;
	
	TCHAR    chMsg[512];
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf_s(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;
	ReportEvent(m_hEventSource,                  
			    Type, 0,dwEventID, NULL, 1, dwLowDataSize, (LPCTSTR*) &lpszStrings[0], lpRawData);
}

void CLogDebug::LogEvent(int Type,DWORD dwEventID,LPCTSTR pFormat, ...)
{
	if(!m_hEventSource) return;
	
	TCHAR    chMsg[512];
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf_s(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;
	ReportEvent(m_hEventSource, 
			    Type, 0,dwEventID, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
}


void CLogDebug::LogEventErr(DWORD dwEventID,LPCTSTR pFormat, ...)
{	
	if(!m_hEventSource) return;
	
	TCHAR    chMsg[4096];
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf_s(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;
	ReportEvent(m_hEventSource, 
		EVENTLOG_ERROR_TYPE, 0,(dwEventID == -1) ? LOG_ERROR : dwEventID, 
		NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
}

void CLogDebug::LogEventInf(DWORD dwEventID,LPCTSTR pFormat, ...)
{
	if(!m_hEventSource) return;
	
	TCHAR    chMsg[2048];
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf_s(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;
	ReportEvent(m_hEventSource, 
		EVENTLOG_INFORMATION_TYPE, 0,(dwEventID == -1) ? LOG_INFORMATION : dwEventID, 
		NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
}

void CLogDebug::LogEventWarning(DWORD dwEventID,LPCTSTR pFormat, ...)
{
	if(!m_hEventSource) return;
	
	TCHAR    chMsg[512];
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf_s(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;
	ReportEvent(m_hEventSource, 
		EVENTLOG_WARNING_TYPE, 0,(dwEventID == -1) ? LOG_WARNING : dwEventID, 
		NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
}

void CLogDebug::RegEventSrc()
{
	if(m_hEventSource)
		DeregisterEventSource(m_hEventSource);
	m_hEventSource = RegisterEventSource(NULL,m_szServiceName); //RegisterEventSource:evnetlog handle 반환       
	AddEventSource();
}

void CLogDebug::SetServiceName(char* lpSvcName)
{
	lstrcpy( m_szServiceName, lpSvcName );
	RegEventSrc();
}