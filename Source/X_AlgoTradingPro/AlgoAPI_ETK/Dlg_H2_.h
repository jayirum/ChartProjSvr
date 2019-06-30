#if !defined(AFX_DLG_H2__H__85739028_E32E_47A8_8DDD_4A7336C772F0__INCLUDED_)
#define AFX_DLG_H2__H__85739028_E32E_47A8_8DDD_4A7336C772F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_H2_.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_H2_ dialog

class CDlg_H2_ : public CDialog
{
	DECLARE_DYNCREATE( CDlg_H2_ )
		// Construction
public:
	CDlg_H2_(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDlg_H2_)
	enum { IDD = IDD_FC0 };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_H2_)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	CString				m_strCode;
	CListCtrl			m_ctrlOutBlock;
	void				InitCtrls();
	void				AdviseData();
	void				UnadviseData();
	
	// Generated message map functions
	//{{AFX_MSG(CDlg_H2_)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H2__H__85739028_E32E_47A8_8DDD_4A7336C772F0__INCLUDED_)
