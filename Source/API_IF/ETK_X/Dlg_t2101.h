#if !defined(AFX_DLG_T2101_H__5DD76FF8_8938_458E_ADC8_6A671F60B572__INCLUDED_)
#define AFX_DLG_T2101_H__5DD76FF8_8938_458E_ADC8_6A671F60B572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_t2101.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_t2101 dialog

#include "../../include/UtilLibInc.h"

class CDlg_t2101 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_t2101 )
// Construction
public:
	CDlg_t2101(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_t2101)
	enum { IDD = IDD_t2101 };
	CListBox	m_lstResult;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_t2101)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CLog	m_logF;
	list<CString>		m_sStkList_KSF;
	list<CString>		m_sStkList_KSO;
	ScrewDB				m_db;

	CString				m_sLastSendStk;
	int					m_nLastSendCnt_F, m_nLastSendCnt_O;
	char				m_szMsg[1024];
// Implementation
protected:
	BOOL	GetStkCodeList_KS(char* psGdsCode);
	void	RequestData(const char* i_psStkCode);
	char*	get_double_withdot( char* i_psOrg, int i_nOrgLen, int i_nDotCnt, int i_nOutLen, char* o_psOut );
	VOID	Request_F();
	VOID	Request_O();

	// Generated message map functions
	//{{AFX_MSG(CDlg_t2101)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnButtonEnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	afx_msg	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT		OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_T2101_H__5DD76FF8_8938_458E_ADC8_6A671F60B572__INCLUDED_)
