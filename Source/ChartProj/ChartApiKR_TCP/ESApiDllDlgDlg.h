
// ESApiDllDlgDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "math.h"
#include "afxwin.h"
#include <map>
#include "TrayIconMng.h"
#include "../../IRUM_INC/IRUM_Common.h"
//#include "../../IRUM_UTIL/SharedMem.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/BlockDup.h"
//TODO #include "NanoSocket.h"
//#include "../../IRUM_UTIL/NanoPubSub.h"
//#include "../../IRUM_UTIL/MCastSend.h"
#include "../../IRUM_UTIL/TcpSrv.h"
#include "Poco/MemoryPool.h"
#include "Poco/Data/Session.h"



//
using namespace Poco;

#define MEMPOOL_BLOCK_SIZE	sizeof(At0933O_st_Ex)
#define	MEMPOOL_MAX_UNIT	10000

#define WM_SISE_RECV	WM_USER + 9898
#define WM_SISE_DIE		WM_USER + 9999

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

	// members
	CString m_sDir, m_sModule, m_sConfig;
	char	m_cRealYN;

	std::map<CString,CString>	m_mapSymbol;	// STK, ARTC
	long				m_siseSeq;

	Poco::MemoryPool*	m_pMemPool;
	//CDBPoolAdo			*m_pDBPool;
	VOID				*m_pDBPool;
	std::string			m_sConnStr;
	//TODO 	CNanoSocket			*m_pServerSock;
	//CNanoPubSub			*m_pServerSock;
	//CMCastSend			*m_pTcpSvr;
	CTcpSrv				*m_pTcpSvr;
	CBlockDup			m_Dup;
	HANDLE				m_hSaveThread;
	unsigned int		m_unSaveThread;
	HANDLE				m_hSendThread;
	unsigned int		m_unSendThread;
	BOOL				m_bContinue;
	BOOL				m_bInit;


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
	BOOL	IR_LoadSiseSymbol();

	VOID	IR_PrintMsg(LOGMSG_TP tp, BOOL bShow, char* pMsg, ...);
	//BOOL	InitSHM();
	//VOID	CloseSHM();
	BOOL	IR_InitMemPool();
	VOID	IR_CloseMemPool();
	
	//BOOL	IR_InitThread();
	//VOID	IR_ResumeThreads();
	//VOID	IR_CloseThread();

	BOOL	IR_DBOpen();
	VOID	IR_DBClose();
	BOOL	IR_CheckDBPool();

	static unsigned WINAPI SavePrcThread(LPVOID lp);
	static unsigned WINAPI SendPrcThread(LPVOID lp);

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
	//다이얼로그감추기/보이기
#define WM_APP_EXIT		WM_USER + 102	
	//다이얼로그종료
#define WM_TRAYICON		WM_USER + 103	
	//트레이아이콘메시지

	long OnTrayIcon(WPARAM wParam, LPARAM lParam);
	void OnAppExit(void);
	void OnDialogShow(void);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
