// Log.h: interface for the CLogMsg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include "BaseThread.h" //todo after completion - remove ../
#include <list>
#include "TcpClient.h"

#define DEF_LOG_LEN 4096


enum LOGMSG_TP { LOGTP_SUCC, LOGTP_ERR, INFO,ERR, DEBUG_, ENTER_, NOTIFY,  LOGTP_FATAL = 9, DATA, DATA_DT};

#define INFO_BUFFER_SIZE 32767
#define WM_LOGMSG_LOG	WM_USER + 555
#define TCP_TIMEOUT 10

typedef struct _ST_LOGMSG
{
	LOGMSG_TP	tp;
	char	msg[DEF_LOG_LEN];
}ST_LOGMSG;

class CLogMsgPool
{
public:
	enum {MIN_POOL=20, MAX_POOL=50};
	CLogMsgPool();
	virtual ~CLogMsgPool();

	ST_LOGMSG* Get();
	VOID	Restore(ST_LOGMSG* p);

private:
	std::list<ST_LOGMSG*>	m_logPool;
	CRITICAL_SECTION	m_cs;

};

class CSendMsg : public CBaseThread
{
public:
	CSendMsg();
	virtual ~CSendMsg();
	VOID	setNotifyServerIP(char* szIP) { strcpy(m_szNotifyServerIP, szIP); }
	VOID	setNotifyServerPort(int nPort) { m_nNotifyServerPort = nPort; }
	int		getMyThreadID() { return GetMyThreadID();}
	BOOL	fn_SendMessage(char* szMessage, int nTimeOut);

	virtual VOID	ThreadFunc();
private:

	CRITICAL_SECTION m_cs;
	char	m_szMsg[DEF_LOG_LEN];
	char	m_szNotifyServerIP[_MAX_PATH];
	int		m_nNotifyServerPort;
	char	m_szAppName[DEF_LOG_LEN];
	CTcpClient *m_pMonitorClient;

	VOID	LOCK() { EnterCriticalSection(&m_cs); };
	VOID	UNLOCK() { LeaveCriticalSection(&m_cs); };
};

class CLogMsg : public CBaseThread
{
public:
	CLogMsg();
	virtual ~CLogMsg();


	virtual VOID	ThreadFunc();	// RECV 를 위한 스레드

	BOOL	OpenLog(char* psPath, char* pFileName);	
	BOOL	OpenLogEx(char* psPath, char* pFileName, char* szIP, int nPort, char* szApplicationName);
	VOID	log(LOGMSG_TP, char* pMsg, ...);
	//VOID	logD(char* pMsg, ...);
	VOID	enter();
	//VOID	setNotifyServerIP(char* szIP) { strcpy(m_szNotifyServerIP, szIP);m_csmNotifyThread.setNotifyServerIP(m_szNotifyServerIP); }
	//VOID	setNotifyServerPort(int nPort) { m_nNotifyServerPort = nPort; m_csmNotifyThread.setNotifyServerPort(m_nNotifyServerPort); }
	//VOID	setNotifyServerOwnHostName() { GetComputerNameIntoString(); }
	//VOID	setNotifyOwnApplicationName(char* szApplicationName) { strcpy(m_szNotifyServerOwnApplicationName, szApplicationName); }


	char	m_szFileName[_MAX_PATH];
private:
	VOID	logMsg(ST_LOGMSG* p);
	//BOOL	fn_SendMessage(char* szMessage, int nTimeOut);
	VOID	GetComputerNameIntoString();
	BOOL	ReOpen();
	
	VOID	Close();
	
	//VOID	log(char* pMsg);
	//VOID	WriteErr(int nErrCode, char* pMsg, ...);
	VOID	LOCK(){ EnterCriticalSection(&m_cs); };
	VOID	UNLOCK(){ LeaveCriticalSection(&m_cs); };
	//BOOL	isOpenLog(){ return (m_fd>0); };
	
//private:
	int		m_fd;
	char	m_szPath	[_MAX_PATH];
	//char	m_szFileName[_MAX_PATH];
	char	m_szPureFileName[_MAX_PATH];
	char	m_szDate	[8+1];
	char	m_szMsg[1024];
	char	m_szNotifyServerOwnHostName[_MAX_PATH];
	char	m_szNotifyServerOwnApplicationName[_MAX_PATH];
	char	m_szNotifyServerIP[_MAX_PATH];
	int		m_nNotifyServerPort;

	CRITICAL_SECTION m_cs;

	CLogMsgPool		*m_pool;

	CSendMsg		m_csmNotifyThread;
};
