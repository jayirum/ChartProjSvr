// Log.cpp: implementation of the CLogMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "LogMsg.h"
#include <stdio.h>
#include <share.h>
#include "UtilFunc.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CLogMsg::CLogMsg():CBaseThread("CLogMsg")
{
	m_fd = 0;
	ZeroMemory(m_szFileName, sizeof(m_szFileName));
	ZeroMemory(m_szDate, sizeof(m_szDate));
	ZeroMemory(m_szPureFileName, sizeof(m_szPureFileName) );
	InitializeCriticalSection(&m_cs);

	m_pool = new CLogMsgPool;

	ResumeThread();
}

CLogMsg::~CLogMsg()
{
	StopThread();
	Close();
	DeleteCriticalSection(&m_cs);
	delete m_pool;
}

BOOL CLogMsg::OpenLog(char* pFileName)
{
	lstrcpy( m_szPureFileName, pFileName );

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(m_szDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);	
	sprintf_s(m_szFileName, "%s_%s.log", pFileName, m_szDate);
	Close();

	//errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT|_O_APPEND|_O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT | _O_TRUNC | _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	if( err < 0 ){
		return FALSE;
	}

	return TRUE;
}




BOOL CLogMsg::ReOpen()
{
	char szToday[8+1];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(szToday, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	if( strcmp(szToday, m_szDate)>0 )
	{
		return OpenLog( m_szPureFileName );
		
	}
	return	TRUE;
}



VOID CLogMsg::log(LOGMSG_TP tp, char* pMsg, ...)
{
	ST_LOGMSG* p = NULL;
	__try
	{
		LOCK();
		__try
		{
			
			p = m_pool->Get();
			if (p == NULL)
				return;

			va_list argptr;

			if (lstrlen(pMsg) >= DEF_LOG_LEN)
				*(pMsg + DEF_LOG_LEN - 1) = 0x00;

			va_start(argptr, pMsg);
			vsprintf_s(p->msg, pMsg, argptr);
			va_end(argptr);

			p->tp = tp;

			PostThreadMessage((DWORD)m_dwThreadID, WM_LOGMSG_LOG, (WPARAM)0, (LPARAM)p);
		}
		__except (ReportException(GetExceptionCode(), "LogMsg::log", m_szMsg))
		{
			m_pool->Restore(p);
		}
	}
	__finally
	{
		UNLOCK();
	}
}

VOID	CLogMsg::logMsg(ST_LOGMSG* p)
{
	__try
	{
		__try
		{
			LOCK();
			ReOpen();

			if (m_fd <= 0) {
				UNLOCK();
				return;
			}


			char buff[DEF_LOG_LEN];
			SYSTEMTIME	st;

			GetLocalTime(&st);
			if (p->tp == LOGTP_INFO)	strcpy(buff, "[I]");
			if (p->tp == LOGTP_ERR)		strcpy(buff, "[E]");
			if (p->tp == LOGTP_FATAL)	strcpy(buff, "[F]");

			sprintf(buff + 3, "[%02d:%02d:%02d.%03d]%.*s\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, DEF_LOG_LEN - 20, p->msg);
			_write(m_fd, buff, strlen(buff));
			m_pool->Restore(p);
		}
		__except (ReportException(GetExceptionCode(), "LogMsg::logMsg", m_szMsg))
		{
		}
	}
	__finally
	{
		UNLOCK();
	}

}

VOID CLogMsg::ThreadFunc()
{
	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			Sleep(1000);
			continue;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_LOGMSG_LOG:
			{
				logMsg((ST_LOGMSG*)msg.lParam);
				break;
			}
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////

CLogMsgPool::CLogMsgPool()
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
	for (int i = 0; i < MIN_POOL; i++)
	{
		ST_LOGMSG* p = new ST_LOGMSG;
		ZeroMemory(p, sizeof(ST_LOGMSG));
		m_logPool.push_back(p);
	}
}
CLogMsgPool::~CLogMsgPool()
{
	DeleteCriticalSection(&m_cs);
}

ST_LOGMSG* CLogMsgPool::Get()
{
	ST_LOGMSG *p;
	EnterCriticalSection(&m_cs);
	if (m_logPool.empty() == TRUE) {
		p = new ST_LOGMSG;
		ZeroMemory(p, sizeof(ST_LOGMSG));
	}
	else
	{
		p = *(m_logPool.begin());
		m_logPool.pop_front();
	}
	LeaveCriticalSection(&m_cs);
	return p;
}
VOID CLogMsgPool::Restore(ST_LOGMSG* p)
{
	EnterCriticalSection(&m_cs);
	if (m_logPool.size() >= MAX_POOL)
		delete p;
	else 
	{
		ZeroMemory(p, sizeof(ST_LOGMSG));
		m_logPool.push_back(p);
	}
	LeaveCriticalSection(&m_cs);
}



VOID CLogMsg::Close()
{
	if( m_fd > 0){
		_close(m_fd);
		m_fd = 0;
	}
}
