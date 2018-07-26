
// ESApiDllDlgDlg.h : 헤더 파일
//

#pragma once

//#include "afxcmn.h"
//#include "math.h"
//#include "afxwin.h"
//
//typedef BOOL (__cdecl *ESISSERVERCONNECT)(HWND hWnd);
//typedef void (__cdecl *ESDISCONNECTSERVER)(HWND hWnd);
//typedef BOOL (__cdecl *ESCONNECTSERVER)(HWND hWnd, LPCTSTR szUserID, LPCTSTR szPasswd, LPCTSTR szCertPasswd);
//typedef BOOL (__cdecl *ESGETFULLCODE)(LPSTR szFullCode, LPCTSTR szShortCode);
//typedef BOOL (__cdecl *ESGETSHORTCODE)(LPSTR szShortCode, LPCTSTR szFullCode);
//typedef BOOL (__cdecl *ESGETCODEINDEX)(LPSTR szCodeIndex, LPCTSTR szCode);
//typedef void (__cdecl *ESGETCOMMUNICATIONTYPE)(LPSTR lpszData, int nLen);
//typedef BOOL (__cdecl *ESGETENCODEPASSWORD)(LPSTR szDesPass, int nLen, LPCTSTR szAcct, LPCTSTR szSrcPass);
//typedef BOOL (__cdecl *ESSENDTRDATA)(int nTrCode, LPSTR lpszData, int nLen);
//typedef BOOL (__cdecl *ESSETAUTOUPDATE)(BOOL bSet, BOOL bAccount, LPSTR szAutoKey);

// CESApiDllDlgDlg 대화 상자
class CESApiDllDlgDlg : public CDialogEx
{
// 생성입니다.
public:
	CESApiDllDlgDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ESAPIDLLDLG_DIALOG };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	CString			GetCurCode();
	CString			GetCurAccount();
	CString			GetAccount(int nSel);

	CESApi			m_Api;

	CString			m_szAutoAcctKey;
	CString			m_szAutoCodeKey;

	// members
	CLog	*m_log;
	CString m_sDir, m_sModule, m_sConfig;
	CString	m_sHedgeAcc;
	char	m_cRealYN;
	//

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL		OnInitDialog();
	afx_msg void		OnPaint();
	afx_msg HCURSOR		OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CComboBox			m_cmbAcct;
	CComboBox			m_cmbCode;

	CArrayAcct			m_aryAcct;
	CArrayFCAcct		m_aryFCAcct;
	CArrayCode			m_aryCode;

	CString				m_szHedgePass;
	CString				m_szOrdP;
	CString				m_szOrdV;
	CString				m_szOrgNo;

	CListCtrl			m_List1;
	CListCtrl			m_List2;
	CListCtrl			m_listNearSeries;
	CListCtrl			m_listSurenet;

	CString				GetAccountName(LPSTR lpszAccount, int nLen, BOOL bFreeCap);
	int					GetDecimalLen(LPSTR lpszCode, int nLen);
	BOOL				GetCodeInfo(lpESCodeInfo_st lpCode, LPSTR lpszCode, int nLen);
	void				GetConvertPrice(char chType, char *pPrice, int nLen, int nDigitLen, CString *szResult);
	BOOL				GetConvertPrice(char chType, double dValue, int nDigitLen, CString *szResult);
	void				InsertComma(LPCTSTR lpszData, TCHAR* szOUT, int nLen);
	void				InsertComma(CString &szData, BOOL bNonZero = FALSE);
	void				InsertComma(LPSTR lpszData, BOOL bNonZero = FALSE);
	double				MyAtof(char *ibuf, int len);


	//FUNCs
	VOID	SendHedgeOrder(lpAt0985O_st p);
	VOID	PrintMsg(BOOL bSucc,BOOL bShow, char* pMsg, ...);

	//
	afx_msg void		OnBnClickedOk();
	afx_msg void		OnBnClickedButton1();
	afx_msg void		OnBnClickedCancel();
	afx_msg void		OnBnClickedButton2();
	afx_msg void		OnBnClickedButton3();
	afx_msg void		OnBnClickedButtonExit();
	afx_msg void		OnBnClickedButton4();	
	afx_msg void		OnBnClickedButton5();
	afx_msg void		OnBnClickedButton6();
	afx_msg void		OnBnClickedButton7();
	afx_msg void		OnBnClickedButton8();

	afx_msg LRESULT		OnRecvData			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnAcctList			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnFCAcctList		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnCodeList			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnServerConnect		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnServerDisConnect	(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton9();
	CString m_szUserID;
	CString m_szUserPass;
	CString m_szCertPass;
	CString m_szCurPrice;
	CString m_szCurDebi;
	CString m_szNowVol;
	CString m_szVolume;
//	CListCtrl m_szListChe;
	CListCtrl m_ListChe;
//	CEdit m_szDHoka1;
	CString m_szDHoka1;
//	CEdit m_szDHoka2;
	CString m_szDHoka2;
	CString m_szDHoka3;
	CString m_szSHoka1;
	CString m_szSHoka2;
	CString m_szSHoka3;
	afx_msg void OnSelchangeComboAcct();
	afx_msg void OnSelchangeComboCode();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	CString m_sDllPath;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedButton12();

	afx_msg LRESULT OnTestMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedBtnNearseries();
	afx_msg void OnBnClickedBtn5801();
	afx_msg void OnBnClickedBtn5802();
	afx_msg void OnBnClickedBtn5634();

	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	CListBox m_lstMsg;
	afx_msg void OnClickedButtonAuto();
	CStatic m_sRealYN;
};
