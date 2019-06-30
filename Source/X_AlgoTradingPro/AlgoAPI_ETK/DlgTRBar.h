#if !defined(AFX_DLGTRBAR_H__194C59B8_F2BE_4268_9CDA_78EFEDF07446__INCLUDED_)
#define AFX_DLGTRBAR_H__194C59B8_F2BE_4268_9CDA_78EFEDF07446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTRBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTRBar dialog

class CDlgTRBar : public CDialog
{
// Construction
public:
	CDlgTRBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTRBar)
	enum { IDD = IDD_TR_BAR };
	CTreeCtrl	m_ctrlTR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTRBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void		LoadTRList();
	HTREEITEM	FindItem( HTREEITEM hItem, LPCTSTR pszData );
	void		GetToken( CString strRead, CStringArray& arrToken );

	// Generated message map functions
	//{{AFX_MSG(CDlgTRBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkTreeTr(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRBAR_H__194C59B8_F2BE_4268_9CDA_78EFEDF07446__INCLUDED_)
