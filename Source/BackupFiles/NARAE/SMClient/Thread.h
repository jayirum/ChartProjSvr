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
/**	\brief	Thread�� �⺻ �Լ�

*/
class TThread  
{
public:
	
	TThread(BOOL bSuspend=FALSE)
	{
		m_dwThreadID = 0;
		m_hDie = CreateEvent(NULL, FALSE, FALSE, NULL);	
		if(bSuspend)
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadProc, this, CREATE_SUSPENDED, &m_dwThreadID);
		else
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadProc, this, 0, &m_dwThreadID);
	}

	virtual ~TThread()
	{
		StopThread();
	}


/**	\brief	The StopThread function


	\return	inline VOID

	*	Thread�� �����ų�� ����ϴ� �Լ�
*/
	inline VOID StopThread()
	{
		if(!m_hThread || !m_hDie )	return;

		SetEvent(m_hDie);
		if (WaitForSingleObject(m_hThread, 3000) != WAIT_OBJECT_0){
			DWORD dwExitCode = 0;
			TerminateThread(m_hThread, dwExitCode);
			//printf("Terminate Thread in BaseThread");
		}
		CloseHandle(m_hDie);
		CloseHandle(m_hThread);
		m_hDie = m_hThread = NULL;
	};


/**	\brief	The ResumeThread function


	\return	inline VOID

	*	Suspend�� Thread�� ��� �����ų�� ����ϴ� �Լ�
*/

	inline VOID ResumeThread()
	{
		::ResumeThread(m_hThread);
	};



/**	\brief	The ThreadProc function

	\param	lp	a parameter of type LPVOID

	\return	static DWORD WINAPI

	*	Thread�� start point
*/

	static unsigned WINAPI ThreadProc(LPVOID lp)
	{
		TThread* p = (TThread*)lp;
		p->Execute();
		return 0;
	};



/**	\brief	The ThreadFunc function


	\return	virtual VOID

	*	Inheritance ���� child class���� ����� Thread start point
*/

	virtual VOID	Execute()=0;

	
/**	\brief	The Get_ThreadID function


	\return	inline DWORD

	*	�� Thread�� ID�� �����ϴ� �Լ�
	
*/
	inline unsigned int	GetMyThreadID()
	{
		return m_dwThreadID;
	};



/**	\brief	The Is_TimeOfStop function

	\param	nTime	a parameter of type int	Ÿ�Ӿƿ�(�ʱⰪ 10)

	\return	inline BOOL

	*	���ѽð���ŭ Thread�� ���Ḧ ����ϴ� �Լ�
	
*/

	inline BOOL		Is_TimeOfStop(int nTime=10)
	{
		return (WaitForSingleObject(m_hDie, nTime)==WAIT_OBJECT_0);
	};




/**	\brief	The Is_Alive function


	\return	inline  BOOL

	*	Thread�� ���Ӽ��� Ȯ���ϴ� �Լ�
*/

	inline	BOOL	Is_Alive()
	{
		DWORD dw;
		GetExitCodeThread(m_hThread, &dw);
		return (dw==STILL_ACTIVE);
	};


public:
/**	\brief Thread Handle
*/
	HANDLE		m_hThread;
/**	\brief Thread ���� Handle
*/
	HANDLE		m_hDie;
/**	\brief Thread ID
*/
	unsigned int		m_dwThreadID;
};


#endif // !defined(AFX_BASETHREAD_H__2B8AC972_6959_4C37_BF19_E5683E3F9AA9__INCLUDED_)
