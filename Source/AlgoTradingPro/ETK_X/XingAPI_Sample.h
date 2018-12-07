// XingAPI_Sample.h : main header file for the XINGAPI_SAMPLE application
//

#if !defined(AFX_XINGAPI_SAMPLE_H__73167BA5_C571_4427_BCA3_992EA088BE8F__INCLUDED_)
#define AFX_XINGAPI_SAMPLE_H__73167BA5_C571_4427_BCA3_992EA088BE8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXingAPI_SampleApp:
// See XingAPI_Sample.cpp for the implementation of this class
//

class CXingAPI_SampleApp : public CWinApp
{
public:
	CXingAPI_SampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXingAPI_SampleApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;
	
	HANDLE		m_hDie;
	HANDLE		m_hThread;

	
	static	DWORD WINAPI  ThreadProc(LPVOID lp);
	VOID	StopThread();

public:
	//{{AFX_MSG(CXingAPI_SampleApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XINGAPI_SAMPLE_H__73167BA5_C571_4427_BCA3_992EA088BE8F__INCLUDED_)
