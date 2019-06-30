#if !defined(AFX_DLG_SC4_H__710B3EC0_37CD_46A6_B0FC_4B721A0641E3__INCLUDED_)
#define AFX_DLG_SC4_H__710B3EC0_37CD_46A6_B0FC_4B721A0641E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_SC4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC4 dialog

class CDlg_SC4 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_SC4 )
// Construction
public:
	CDlg_SC4(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_SC4)
	enum { IDD = IDD_SC4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_SC4)
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
	//{{AFX_MSG(CDlg_SC4)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SC4_H__710B3EC0_37CD_46A6_B0FC_4B721A0641E3__INCLUDED_)
