#if !defined(AFX_DLG_SC1_H__3917C2E4_1A79_4DD0_93C3_ADC09D075583__INCLUDED_)
#define AFX_DLG_SC1_H__3917C2E4_1A79_4DD0_93C3_ADC09D075583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_SC1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC1 dialog

class CDlg_SC1 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_SC1 )
// Construction
public:
	CDlg_SC1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_SC1)
	enum { IDD = IDD_SC1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_SC1)
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
	//{{AFX_MSG(CDlg_SC1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SC1_H__3917C2E4_1A79_4DD0_93C3_ADC09D075583__INCLUDED_)
