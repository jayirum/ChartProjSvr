#if !defined(AFX_DLG_T9905_H__951A6563_5D78_4EF9_937E_B64695D505A3__INCLUDED_)
#define AFX_DLG_T9905_H__951A6563_5D78_4EF9_937E_B64695D505A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_t9905.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_t9905 dialog

class CDlg_t9905 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_t9905 )
// Construction
public:
	CDlg_t9905(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_t9905)
	enum { IDD = IDD_t9905 };
	CListBox	m_lst_t9905;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_t9905)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	void RequestData( BOOL bNext );


	// Generated message map functions
	//{{AFX_MSG(CDlg_t9905)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	
	afx_msg	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT		OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_T9905_H__951A6563_5D78_4EF9_937E_B64695D505A3__INCLUDED_)
