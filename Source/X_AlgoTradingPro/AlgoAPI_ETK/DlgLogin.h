#if !defined(AFX_DLGLOGIN_H__B9BAABB1_9203_4818_B1DE_2B7F4EDBA5C7__INCLUDED_)
#define AFX_DLGLOGIN_H__B9BAABB1_9203_4818_B1DE_2B7F4EDBA5C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLogin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin dialog

class CDlgLogin : public CDialog
{
// Construction
public:
	CDlgLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLogin)
	enum { IDD = IDD_LOGIN };
	CButton	m_ctrlSaveID;
	CButton	m_ctrlShowCertErrDlg;
	CButton	m_ctrlConnectTimeout;
	CButton	m_ctrlSendPacketSize;
	CEdit	m_ctrlServerIP;
	CEdit	m_ctrlPwd;
	CEdit	m_ctrlID;
	CEdit	m_ctrlCertPwd;
	CComboBox	m_ctrlServerPort;
	CComboBox	m_ctrlServerAddrType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLogin)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	ConnectServer		();
	BOOL	Login				();

	CString GetProfileFileName	();

	void	LoadProfile			();
	void	SetServerAddrTypeProfile	( LPCTSTR pszData );
	void	SetPortProfile		( int nPort );

	void	SaveProfile			();
	
	void	SetServerAddrType	( BOOL bReal );
	void	SetServerIP			( LPCTSTR pszAddrType, LPCTSTR pszServerIP );

	// Generated message map functions
	//{{AFX_MSG(CDlgLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioReal();
	afx_msg void OnRadioSimul();
	afx_msg void OnSelchangeComboServerAddrType();
	virtual void OnOK();
	afx_msg void OnButtonLogin();
	afx_msg void OnCheckSendPacketSize();
	afx_msg void OnCheckConnectTimeout();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnLogin		( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGIN_H__B9BAABB1_9203_4818_B1DE_2B7F4EDBA5C7__INCLUDED_)
