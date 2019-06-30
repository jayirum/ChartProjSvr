#if !defined(AFX_DLG_H3_ELW_H__26C6A6BD_ADC8_4F6C_A230_6924EC818EB6__INCLUDED_)
#define AFX_DLG_H3_ELW_H__26C6A6BD_ADC8_4F6C_A230_6924EC818EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_h3_ELW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_h3_ELW dialog

class CDlg_h3_ELW : public CDialog
{
	DECLARE_DYNCREATE( CDlg_h3_ELW )
		// Construction
public:
	CDlg_h3_ELW(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDlg_h3_ELW)
	enum { IDD = IDD_FC0 };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_h3_ELW)
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
	//{{AFX_MSG(CDlg_h3_ELW)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H3_ELW_H__26C6A6BD_ADC8_4F6C_A230_6924EC818EB6__INCLUDED_)
