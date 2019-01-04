#if !defined(AFX_DLG_OC0_H__7AA720D2_DC24_4C8F_9E60_BE847E5CDF96__INCLUDED_)
#define AFX_DLG_OC0_H__7AA720D2_DC24_4C8F_9E60_BE847E5CDF96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_OC0.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_OC0 dialog

class CDlg_OC0 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_OC0 )
// Construction
public:
	CDlg_OC0(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_OC0)
	enum { IDD = IDD_OC0 };
	CListCtrl	m_ctrlOutBlock;
	CListCtrl	m_ctrlOutBlock_H;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_OC0)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString				m_strCode, m_strCode_H;
//	CListCtrl			m_ctrlOutBlock;
	void				InitCtrls();
	void				AdviseData(char i_cSiseYN);
	void				UnadviseData(char i_cSiseYN);

	// Generated message map functions
	//{{AFX_MSG(CDlg_OC0)
	afx_msg void OnButtonRequest();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRequestH();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_OC0_H__7AA720D2_DC24_4C8F_9E60_BE847E5CDF96__INCLUDED_)
