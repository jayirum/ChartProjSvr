#if !defined(AFX_DLG_H2_ELW_H__303EC02D_32A0_41A3_A3C2_C55501A2058D__INCLUDED_)
#define AFX_DLG_H2_ELW_H__303EC02D_32A0_41A3_A3C2_C55501A2058D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_h2_ELW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_h2_ELW dialog

class CDlg_h2_ELW : public CDialog
{
	DECLARE_DYNCREATE( CDlg_h2_ELW )
		// Construction
public:
	CDlg_h2_ELW(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDlg_h2_ELW)
	enum { IDD = IDD_FC0 };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_h2_ELW)
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
	//{{AFX_MSG(CDlg_h2_ELW)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H2_ELW_H__303EC02D_32A0_41A3_A3C2_C55501A2058D__INCLUDED_)
