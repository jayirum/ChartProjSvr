#if !defined(AFX_DLG_BM__H__DB5FE936_C0D3_4A81_9C18_2DD207E245A1__INCLUDED_)
#define AFX_DLG_BM__H__DB5FE936_C0D3_4A81_9C18_2DD207E245A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_BM_.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_BM_ dialog

class CDlg_BM_ : public CDialog
{
	DECLARE_DYNCREATE( CDlg_BM_ )
		// Construction
public:
	CDlg_BM_(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDlg_BM_)
	enum { IDD = IDD_BM_ };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_BM_)
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
	//{{AFX_MSG(CDlg_BM_)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFco();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_BM__H__DB5FE936_C0D3_4A81_9C18_2DD207E245A1__INCLUDED_)
