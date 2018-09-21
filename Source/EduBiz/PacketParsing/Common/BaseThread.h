 // BaseThread.h: interface for the CBaseThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASETHREAD_H__2B8AC972_6959_4C37_BF19_E5683E3F9AA9__INCLUDED_)
#define AFX_BASETHREAD_H__2B8AC972_6959_4C37_BF19_E5683E3F9AA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <stdio.h>
#include <process.h>
#pragma warning(disable:4996)


class CBaseThread  
{
public:
	
	CBaseThread(char* pzName=NULL, BOOL bSuspend = TRUE)
	{
		ZeroMemory(m_zName, sizeof(m_zName));
		if(pzName)
			strcpy(m_zName, pzName);
		m_dwThreadID = 0;
		m_hDie = CreateEvent(NULL, FALSE, FALSE, NULL);	
		if(bSuspend)
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadProc, this, CREATE_SUSPENDED, &m_dwThreadID);
		else {
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadProc, this, 0, &m_dwThreadID);
			m_bContinue = TRUE;
		}
	}

	virtual ~CBaseThread()
	{
		printf("CBaseThread destructor(%d)(%s)\n", m_dwThreadID, m_zName);
		StopThread();
	}

	inline VOID StopThread()
	{
		m_bContinue = FALSE;
		if(!m_hThread || !m_hDie )	return;

		SetEvent(m_hDie);
		if (WaitForSingleObject(m_hThread, 3000) != WAIT_OBJECT_0){
			DWORD dwExitCode = 0;
			TerminateThread(m_hThread, dwExitCode);
			printf("Terminate Thread in BaseThread(%d)(%s)\n", m_dwThreadID, m_zName);
		}
		CloseHandle(m_hDie);
		CloseHandle(m_hThread);
		m_hDie = m_hThread = NULL;
	};
	
	inline VOID ResumeThread()
	{
		::ResumeThread(m_hThread);
		m_bContinue = TRUE;
	};

	//	Thread start point
	static unsigned WINAPI ThreadProc(LPVOID lp)
	{
		CBaseThread* p = (CBaseThread*)lp;
		p->ThreadFunc();
		return 0;
	};


	//The ThreadFunc function
	virtual VOID	ThreadFunc()=0;

	// return my thread id
	inline unsigned int	GetMyThreadID()
	{
		return m_dwThreadID;
	};

	// check thread stop event
	inline BOOL		Is_TimeOfStop(int nTime=10)
	{
		return (WaitForSingleObject(m_hDie, nTime)==WAIT_OBJECT_0);
	};




public:

	HANDLE		m_hThread;
	HANDLE		m_hDie;
	unsigned int		m_dwThreadID;


	char m_zName[128];
	BOOL m_bContinue;
};


#endif // !defined(AFX_BASETHREAD_H__2B8AC972_6959_4C37_BF19_E5683E3F9AA9__INCLUDED_)
