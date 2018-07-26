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
#include "BaseThread.h"
#include <list>

#define DEF_LOG_LEN 4096


enum LOGMSG_TP { LOGTP_SUCC, LOGTP_ERR, LOGTP_FATAL=9};
#define WM_LOGMSG_LOG	WM_USER + 555

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


class CLogMsg : public CBaseThread
{
public:
	CLogMsg();
	virtual ~CLogMsg();

	virtual VOID	ThreadFunc();	// RECV 를 위한 스레드

	BOOL	OpenLog(char* psPath, char* pFileName);	
	VOID	log(LOGMSG_TP, char* pMsg, ...);

private:
	VOID	logMsg(ST_LOGMSG* p);

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
	char	m_szFileName[_MAX_PATH];
	char	m_szPureFileName[_MAX_PATH];
	char	m_szDate	[8+1];
	char	m_szMsg[1024];
	CRITICAL_SECTION m_cs;

	CLogMsgPool		*m_pool;
	
};
