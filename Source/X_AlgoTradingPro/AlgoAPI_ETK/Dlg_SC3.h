#if !defined(AFX_DLG_SC3_H__29445288_B93B_4972_A7B8_63F3272DF90C__INCLUDED_)
#define AFX_DLG_SC3_H__29445288_B93B_4972_A7B8_63F3272DF90C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_SC3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC3 dialog

class CDlg_SC3 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_SC3 )
// Construction
public:
	CDlg_SC3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_SC3)
	enum { IDD = IDD_SC3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_SC3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListCtrl			m_ctrlOutBlock;
	void				InitCtrls();
	void				AdviseData();
	void				UnadviseData();
	
	// Generated message map functions
	//{{AFX_MSG(CDlg_SC3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SC3_H__29445288_B93B_4972_A7B8_63F3272DF90C__INCLUDED_)
