
// ESApiDllDlgDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "math.h"
#include "afxwin.h"
#include <map>
#include "TrayIconMng.h"
#include "../Common/KRCommon.h"
//#include "../../IRUM_UTIL/SharedMem.h"
#include "../../IRUM_UTIL/Screwdb.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/BlockDup.h"
#include "ChartThread.h"
#include "NanoSocket.h"

#include "Poco/MemoryPool.h"
#include "Poco/Data/Session.h"



//
using namespace Poco;

#define MEMPOOL_BLOCK_SIZE	sizeof(At0933O_st_Ex)
#define	MEMPOOL_MAX_UNIT	10000

#define WM_SISE_RECV	WM_USER + 9898

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



// CESApiDllDlgDlg ��ȭ ����
class CESApiDllDlgDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CESApiDllDlgDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ESAPIDLLDLG_DIALOG };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	CString			GetCurCode();
	CString			GetCurAccount();
	CString			GetAccount(int nSel);

	CESApi			m_Api;

	// members
	CString m_sDir, m_sModule, m_sConfig;
	char	m_cRealYN;

	//CSharedMem			m_shm[2];
	std::map<CString, CChartThread*>	m_mapSymbol;
	long				m_siseSeq;

	Poco::MemoryPool*	m_pMemPool;
	CDBPool				*m_pDBPool;
	std::string			m_sConnStr;

	CNanoSocket			*m_pServerSock;

	CBlockDup			m_Dup;
	char				m_chartTp[CHART_TP_CNT];

	HANDLE				m_hSaveThread;
	unsigned int		m_unSaveThread;
	BOOL				m_bContinue;



// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
	BOOL	LoadSiseSymbol();

	VOID	PrintMsg(LOGMSG_TP tp, BOOL bShow, char* pMsg, ...);
	//BOOL	InitSHM();
	//VOID	CloseSHM();
	BOOL	InitMemPool();
	VOID	CloseMemPool();
	BOOL	InitThread();
	VOID	ResumeThreads();
	VOID	CloseThread();

	BOOL	DBOpen();
	VOID	DBClose();
	BOOL	CheckDBPool();
	BOOL	SaveAcntInfo(CString &acnt, CString &nm, CString& pwd);

	static unsigned WINAPI SavePrcThread(LPVOID lp);

	//
	afx_msg void		OnBnClickedOk();
	afx_msg void		OnBnClickedCancel();
	afx_msg void		OnBnClickedButtonExit();

	afx_msg LRESULT		OnRecvData			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnAcctList			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnFCAcctList		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnCodeList			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnServerConnect		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnServerDisConnect	(WPARAM wParam, LPARAM lParam);

	CString m_szUserID;
	CString m_szUserPass;
	CString m_szCertPass;
//	CString m_szCurPrice;
//	CString m_szCurDebi;
//	CString m_szNowVol;
//	CString m_szVolume;
//	CListCtrl m_szListChe;
//	CListCtrl m_ListChe;
//	CEdit m_szDHoka1;
//	CString m_szDHoka1;
//	CEdit m_szDHoka2;
//	CString m_szDHoka2;
//	CString m_szDHoka3;
//	CString m_szSHoka1;
//	CString m_szSHoka2;
//	CString m_szSHoka3;
	afx_msg void OnSelchangeComboAcct();
	afx_msg void OnSelchangeComboCode();
	CString m_sDllPath;
	afx_msg void OnBnClickedOk2();

	afx_msg LRESULT OnTestMsg(WPARAM wParam, LPARAM lParam);

	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	CListBox m_lstMsg;
	afx_msg void OnClickedButtonAuto();
	CStatic m_sRealYN;
	afx_msg void OnBnClickedButtonSise();


public:

	bool m_bHide;
	CTrayIconMng m_myTray;

#define WM_DIALOG_SHOW	WM_USER + 101	
	//���̾�αװ��߱�/���̱�
#define WM_APP_EXIT		WM_USER + 102	
	//���̾�α�����
#define WM_TRAYICON		WM_USER + 103	
	//Ʈ���̾����ܸ޽���

	long OnTrayIcon(WPARAM wParam, LPARAM lParam);
	void OnAppExit(void);
	void OnDialogShow(void);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
