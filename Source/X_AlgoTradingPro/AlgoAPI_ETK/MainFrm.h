// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__99AF44AC_A2B6_46DD_9E54_D70DC67CF60A__INCLUDED_)
#define AFX_MAINFRM_H__99AF44AC_A2B6_46DD_9E54_D70DC67CF60A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TRBar.h"

// skeo 2011-01-17 [[
#pragma warning (disable : 4786)

//#include "../../include/UtilLibInc.h"
#include "DlgLogin.h"

class CChildFrame;

#include <map>
using namespace std;
// ]]

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	map< LPCTSTR, CChildFrame * >	m_CFrmMap;	// skeo 2011-01-17

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	CTRBar		m_wndTRBar;
	//CLog		m_logF;		// skeo 2011-01-17

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg	LRESULT	OnShowTR	( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnDisconnect( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__99AF44AC_A2B6_46DD_9E54_D70DC67CF60A__INCLUDED_)
