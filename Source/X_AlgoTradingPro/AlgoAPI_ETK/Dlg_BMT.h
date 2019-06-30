#if !defined(AFX_DLG_BMT_H__BD282EB5_91F1_4ADE_ABC9_87D322E0A4BA__INCLUDED_)
#define AFX_DLG_BMT_H__BD282EB5_91F1_4ADE_ABC9_87D322E0A4BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_BMT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_BMT dialog

class CDlg_BMT : public CDialog
{
	DECLARE_DYNCREATE( CDlg_BMT )
		// Construction
public:
	CDlg_BMT(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDlg_BMT)
	enum { IDD = IDD_BMT };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_BMT)
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
	//{{AFX_MSG(CDlg_BMT)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_BMT_H__BD282EB5_91F1_4ADE_ABC9_87D322E0A4BA__INCLUDED_)
