// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "Log.h"
#include <stdio.h>
#include <share.h>

#define BUFLEN 4096L
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog():m_fd(0)
{
	ZeroMemory(m_szFileName, sizeof(m_szFileName));
	ZeroMemory(m_szDate, sizeof(m_szDate));
	ZeroMemory(m_szPureFileName, sizeof(m_szPureFileName) );
	InitializeCriticalSection(&m_cs);
}

CLog::~CLog()
{
	Close();
	DeleteCriticalSection(&m_cs);
}

BOOL CLog::OpenLog(char* psPath, char* pFileName)
{
	lstrcpy( m_szPureFileName, pFileName );

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(m_szDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	lstrcpy( m_szPath, psPath);
	sprintf_s(m_szFileName, "%s\\%s_%s.log", m_szPath, pFileName, m_szDate);
	//LOCK();
	Close();

	errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT|_O_APPEND|_O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	if( err < 0 ){
		//UNLOCK();
		return FALSE;
	}
	//UNLOCK();
	return TRUE;
}


BOOL CLog::OpenTrunc(char* psPath, char* pFileName)
{
	lstrcpy( m_szPureFileName, pFileName );

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(m_szDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	lstrcpy( m_szPath, psPath);
	sprintf_s(m_szFileName, "%s\\%s_%s.log", m_szPath, pFileName, m_szDate);
	//LOCK();
	Close();
	
	errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT|_O_TRUNC|_O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	
	if( err < 0 ){
		//UNLOCK();
		return FALSE;
	}
	//UNLOCK();
	return TRUE;
}

BOOL CLog::OpenMaster(char* psPath, char* pFileName)
{
	lstrcpy( m_szPureFileName, pFileName );

	lstrcpy( m_szPath, psPath);
	sprintf_s(m_szFileName, "%s\\%s", m_szPath, pFileName);
	//LOCK();
	Close();

	errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT|_O_APPEND|_O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	if( err < 0 ){
		//UNLOCK();
		return FALSE;
	}
	//UNLOCK();
	return TRUE;
}


BOOL CLog::OpenMasterTrunc(char* psPath, char* pFileName)
{
	lstrcpy( m_szPureFileName, pFileName );

	lstrcpy( m_szPath, psPath);
	sprintf_s(m_szFileName, "%s\\%s", m_szPath, pFileName);
	//LOCK();
	Close();

	errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT|_O_TRUNC|_O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	if( err < 0 ){
		//UNLOCK();
		return FALSE;
	}
	//UNLOCK();
	return TRUE;
}


BOOL CLog::OpenLogAppend(char* psPath, char* pFileName)
{
	lstrcpy( m_szPureFileName, pFileName );

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(m_szDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	lstrcpy( m_szPath, psPath);
	sprintf_s(m_szFileName, "%s\\%s_%s.log", m_szPath, pFileName, m_szDate);
	//LOCK();
	Close();

	errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT|_O_APPEND|_O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	if( err < 0 ){
	/*m_fd = _open(m_szFileName,  _O_APPEND, _S_IREAD | _S_IWRITE);
	if( m_fd == -1)
		m_fd = _creat(m_szFileName, _S_IREAD | _S_IWRITE);
	if( m_fd==-1 ){*/
		//UNLOCK();
		return FALSE;
	}
	//UNLOCK();
	return TRUE;
}

// 
// BOOL CLog::Open4Read(char* psPath, char* pFileName)
// {
// 	SYSTEMTIME st;
// 	GetLocalTime(&st);
// 	sprintf_s(m_szDate, "20100618");
// 	lstrcpy( m_szPath, psPath);
// 	sprintf_s(m_szFileName, "%s\\%s_%s.log", m_szPath, pFileName, m_szDate);
// 	//LOCK();
// 	Close();
// 	m_fd = _open(m_szFileName,  _O_RDONLY, _S_IREAD );
// 	if( m_fd == -1)
// 		m_fd = _creat(m_szFileName, _S_IREAD | _S_IWRITE);
// 	
// 	if( m_fd==-1 ){
// 		//UNLOCK();
// 		return FALSE;
// 	}
// 	//UNLOCK();
// 	return TRUE;
// }



BOOL CLog::ReOpen()
{
	char szToday[8+1];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(szToday, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	if( strcmp(szToday, m_szDate)>0 )
	{
		//return OpenLog( m_szPath, m_szFileName );
		return OpenLog( m_szPath, m_szPureFileName );
		
	}
	return	TRUE;
}

BOOL CLog::Write(char* pMsg)
{
	ReOpen();

	//LOCK();
	if( m_fd <= 0 ){
		//UNLOCK();
		return FALSE;
	}
	
	_write(m_fd, pMsg,strlen(pMsg));
	//UNLOCK();

	return TRUE;
}


VOID CLog::WriteMaster(char* pMsg)
{
	//LOCK();
	if( m_fd <= 0 ){
		//UNLOCK();
		return ;
	}
	
	_write(m_fd, pMsg,strlen(pMsg));
	//UNLOCK();
	
}


BOOL CLog::Writeln(char* pMsg)
{
	ReOpen();

	//LOCK();
	
	if( m_fd <= 0 ){
		//UNLOCK();
		return FALSE;
	}	
	char* pBuf	= new char[lstrlen(pMsg)+2];
	lstrcpy( pBuf, pMsg);
	lstrcat(pBuf, "\n");
	_write(m_fd, pBuf,strlen(pBuf));
	delete[] pBuf;

	//UNLOCK();
	return TRUE;
}



VOID CLog::Log(char* pMsg,... )
{
	ReOpen();

	//LOCK();
	if( m_fd <= 0 ){
		//UNLOCK();
		return;
	}

	char szBuf[BUFLEN];
	SYSTEMTIME st;
	GetLocalTime(&st);
	int nPos = sprintf_s( szBuf, "[%02d:%02d:%02d.%03d]", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	
	va_list argptr;
	va_start(argptr, pMsg);
	vsprintf_s(szBuf+nPos, BUFLEN - nPos, pMsg, argptr);
	va_end(argptr);
	lstrcat(szBuf, "\n");
	
	_write(m_fd, szBuf, strlen(szBuf));

	//UNLOCK();
}


VOID CLog::LogEx(char* pMsg )
{
	ReOpen();
	
	//LOCK();
	if( m_fd <= 0 ){
		//UNLOCK();
		return;
	}
	
	char szBuf[BUFLEN];
	SYSTEMTIME st;
	GetLocalTime(&st);
	int nPos = sprintf_s( szBuf, "[%02d:%02d:%02d.%03d]%.1024s\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, pMsg);

	_write(m_fd, szBuf, strlen(szBuf));
	
	//UNLOCK();
}


VOID CLog::log(char* pMsg)
{
	ReOpen();

	//LOCK();
	if (m_fd <= 0) {
		//UNLOCK();
		return;
	}

	
	_write(m_fd, pMsg, strlen(pMsg));

	//UNLOCK();
}


VOID CLog::WriteNonString(char* pData, int nLen)
{
	ReOpen();
	
	if( m_fd <= 0 ){
		return;
	}
	
	//LOCK();
	_write(m_fd, pData, nLen);
	//UNLOCK();
}

VOID CLog::WriteByByte(char* pData, int nLen)
{
	ReOpen();
	
	if( m_fd <= 0 ){
		return;
	}
	
	//LOCK();
	for(int i=0; i<nLen; i++)
		_write(m_fd, pData+i, 1);
	//UNLOCK();
}

VOID CLog::WriteErr( int nErrCode, char* pMsg, ... )
{
	ReOpen();
	
	//LOCK();
	if( m_fd <= 0 ){
		//UNLOCK();
		return;
	}
	
	char szBuf[BUFLEN];	
	SYSTEMTIME st;
	GetLocalTime(&st);

	LPVOID lpMsgBuf=NULL;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	
	int nPos = sprintf_s( szBuf, "[%02d:%02d:%02d.%03d](%s)", 
							st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, (char*)lpMsgBuf );
	LocalFree( lpMsgBuf );

	va_list argptr;
	va_start(argptr, pMsg);
	vsprintf_s(szBuf+nPos, BUFLEN- nPos, pMsg, argptr);
	va_end(argptr);
	lstrcat(szBuf, "\n");


	_write(m_fd, szBuf,strlen(szBuf));
	//UNLOCK();
}




VOID CLog::Close()
{
	if( m_fd > 0){
		_close(m_fd);
		m_fd = 0;
	}
}

// int CLog::GetData(/*out*/char* o_pBuf, int nReadLen)
// {
// 	int nRet = _read(m_fd, o_pBuf, nReadLen);
// 	return nRet;
// }

// VOID CLog::WriteFormat( char* pTr, BOOL bSend, char* pMsg )
// {
// 	//LOCK();
// 
// 	if( m_fd <= 0 ){
// 		//UNLOCK();
// 		return;
// 	}
// 	char szAct[5];
// 	(bSend)? lstrcpy( szAct, "SEND" ) : lstrcpy( szAct, "RECV");
// 
// 	char* pBuf = new char[strlen(pMsg)+128];
// 	sprintf_s( pBuf, "[%.5s][%.4s]%s\r\n", pTr, szAct, pMsg );
// 
// 	_write(m_fd, pBuf,strlen(pBuf));
// 	delete[] pBuf;
// 	
// 	//UNLOCK();
// }
// 
// 
// VOID CLog::WriteFormat( char* pTr, char* pMsg )
// {
// 
// 	//LOCK();
// 	if( m_fd <= 0 ){
// 		//UNLOCK();
// 		return;
// 	}
// 
// 	char* pBuf = new char[strlen(pMsg)+128];
// 	sprintf_s( pBuf, "[%.5s] %s\r\n", pTr, pMsg );
// 	
// 	_write(m_fd, pBuf,strlen(pBuf));
// 	delete[] pBuf;
// 
// 	//UNLOCK();
// }

// VOID CLog::WriteFormat2(char* pMsg,... )
// {
// 	//LOCK();
// 	if( m_fd <= 0 ){
// 		//UNLOCK();
// 		return;
// 	}
// 
// 
// 	SYSTEMTIME st;
// 	GetLocalTime(&st);
// 	char pBuf[BUFLEN+128];
// 	int nPos = sprintf_s( pBuf, "[%04d.%02d.%02d-%02d:%02d:%02d.%03d]\t", 
// 			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
// 	try
// 	{
// 		va_list argptr;
// 		va_start(argptr, pMsg);
// 		vsprintf_s(pBuf+nPos, pMsg, argptr);
// 		va_end(argptr);
// 		lstrcat_s(pBuf, "\n");
// 	}
// 	catch(...)
// 	{
// 		//UNLOCK();
// 	}
// 	_write(m_fd, pBuf, min(BUFLEN, (int)strlen(pBuf)));
// 
// 	//UNLOCK();
// }
