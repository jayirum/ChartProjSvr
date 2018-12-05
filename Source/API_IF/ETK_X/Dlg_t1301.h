#if !defined(AFX_DLG_T1301_H__EE3783E2_3C4B_4FB2_8149_48764B1F626E__INCLUDED_)
#define AFX_DLG_T1301_H__EE3783E2_3C4B_4FB2_8149_48764B1F626E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_t1301.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_t1301 dialog

class CDlg_t1301 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_t1301 )
// Construction
public:
	CDlg_t1301(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_t1301)
	enum { IDD = IDD_t1301 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_t1301)
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
	//{{AFX_MSG(CDlg_t1301)
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

#endif // !defined(AFX_DLG_T1301_H__EE3783E2_3C4B_4FB2_8149_48764B1F626E__INCLUDED_)
