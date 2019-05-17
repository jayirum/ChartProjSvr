#if !defined(AFX_TRBAR_H__2FCBE724_65B1_4BCE_AD93_2A3F7ECAEDC7__INCLUDED_)
#define AFX_TRBAR_H__2FCBE724_65B1_4BCE_AD93_2A3F7ECAEDC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TRBar.h : header file
//

#include "scbarg.h"
#include "DlgTRBar.h"
/////////////////////////////////////////////////////////////////////////////
// CTRBar window

class CTRBar : public CSizingControlBarG
{
// Construction
public:
	CTRBar();

// Attributes
public:
	CDlgTRBar		m_dlgTRBar;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTRBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTRBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTRBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRBAR_H__2FCBE724_65B1_4BCE_AD93_2A3F7ECAEDC7__INCLUDED_)
