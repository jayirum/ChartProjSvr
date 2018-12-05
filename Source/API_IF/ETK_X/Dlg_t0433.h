#if !defined(AFX_DLG_T0433_H__7219892B_6D82_4612_B4F0_2179714F7414__INCLUDED_)
#define AFX_DLG_T0433_H__7219892B_6D82_4612_B4F0_2179714F7414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_t0433.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_t0433 dialog

class CDlg_t0433 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_t0433 )
// Construction
public:
	CDlg_t0433(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_t0433)
	enum { IDD = IDD_t0433 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_t0433)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListCtrl			m_ctrlOutBlock;
	CListCtrl			m_ctrlOutBlock1;
	void				InitCtrls();
	void				RequestData( BOOL bNext );

	// Generated message map functions
	//{{AFX_MSG(CDlg_t0433)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnButtonRequest		();
	afx_msg void		OnButtonRequestNext	();
	afx_msg	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT		OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_T0433_H__7219892B_6D82_4612_B4F0_2179714F7414__INCLUDED_)
