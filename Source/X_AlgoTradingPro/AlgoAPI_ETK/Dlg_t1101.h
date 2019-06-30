#if !defined(AFX_DLG_T1101_H__011EDD60_0251_451C_ACE5_FE21B12283B7__INCLUDED_)
#define AFX_DLG_T1101_H__011EDD60_0251_451C_ACE5_FE21B12283B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_t1101.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_t1101 dialog

class CDlg_t1101 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_t1101 )
// Construction
public:
	CDlg_t1101();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_t1101)
	enum { IDD = IDD_t1101 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_t1101)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListCtrl			m_ctrlOutBlock;
	void				InitCtrls();
	void				RequestData( BOOL bNext=FALSE );

	DWORD				m_dwDataMngr;
	void				RequestData4DM( BOOL bNext=FALSE );

	// Generated message map functions
	//{{AFX_MSG(CDlg_t1101)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT		OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_T1101_H__011EDD60_0251_451C_ACE5_FE21B12283B7__INCLUDED_)
