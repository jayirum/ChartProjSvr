// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__124A47E2_E716_4D95_B88D_50C41838F37F__INCLUDED_)
#define AFX_LOG_H__124A47E2_E716_4D95_B88D_50C41838F37F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#define DEF_LOG_LEN 4096
enum { READ = _S_IREAD, WRITE = _S_IWRITE };
class CLog  
{
public:
	CLog();
	virtual ~CLog();

	BOOL	OpenLog(char* psPath, char* pFileName);
	BOOL	OpenTrunc(char* psPath, char* pFileName);
	BOOL	OpenMaster(char* psPath, char* pFileName);
	BOOL	OpenMasterTrunc(char* psPath, char* pFileName);
	BOOL	OpenLogAppend(char* psPath, char* pFileName);	
	BOOL	ReOpen();
	//BOOL	Open4Read(char* psPath, char* pFileName);

	//int		GetData(/*out*/char* o_pBuf, int nReadLen);
	VOID	Close();
	BOOL	Write(char* pMsg);
	BOOL	Writeln(char* pMsg);
	VOID	WriteMaster(char* pMsg);
	VOID	Log(char* pMsg,... );
	VOID	LogEx(char* pMsg );
	VOID	log(char* pMsg);

// 	VOID	WriteFormat( char* pTr, BOOL bSend, char* pMsg );
// 	VOID	WriteFormat( char* pTr, char* pMsg );
//	VOID	WriteFormat2(char* pMsg,... );
	VOID	WriteNonString(char* pData, int nLen);
	VOID	WriteErr( int nErrCode, char* pMsg, ... );
	VOID	WriteByByte(char* pData, int nLen);
	VOID	LOCK(){ EnterCriticalSection(&m_cs); };
	VOID	UNLOCK(){ LeaveCriticalSection(&m_cs); };
	BOOL	isOpenLog(){ return (m_fd>0); };
	
//private:
	int		m_fd;
	char	m_szPath	[_MAX_PATH];
	char	m_szFileName[_MAX_PATH];
	char	m_szPureFileName[_MAX_PATH];
	char	m_szDate	[8+1];
	CRITICAL_SECTION m_cs;

};

#endif // !defined(AFX_LOG_H__124A47E2_E716_4D95_B88D_50C41838F37F__INCLUDED_)
