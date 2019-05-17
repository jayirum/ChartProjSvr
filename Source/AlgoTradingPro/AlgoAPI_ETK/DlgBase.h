#if !defined(AFX_DLGBASE_H__447D054D_7F9B_4B2F_93F3_F25FB526FB8B__INCLUDED_)
#define AFX_DLGBASE_H__447D054D_7F9B_4B2F_93F3_F25FB526FB8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBase dialog

class CDlgBase : public CDialog
{
// Construction
public:
	CDlgBase(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBase)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBase)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBASE_H__447D054D_7F9B_4B2F_93F3_F25FB526FB8B__INCLUDED_)
