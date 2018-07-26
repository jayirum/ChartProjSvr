
// ESApiDllDlgDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "math.h"
#include "afxwin.h"

#include "TrayIconMng.h"
#include "../../IRUM_UTIL/Screwdb.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/BlockDup.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "TargetRcv.h"
#include "CopyOrder.h"
#include <map>

#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Random.h"
#include "Poco/AutoPtr.h"
#include "Poco/MemoryPool.h"
//
using namespace Poco;

#define MEMPOOL_BLOCK_SIZE	sizeof(COPYORD_TARGET_CNTR)+10
#define	MEMPOOL_MAX_UNIT	10000



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

	//CLog	*m_log;
	CString m_sDir, m_sModule, m_sConfig;
	//CString				m_sHedgeAcc;
	char				m_cRealYN;
	//char				m_cHedgeYN;
	CRITICAL_SECTION	m_csAPI;		// API TR SEND �� thread-safe ���� Ȯ��ġ �ʴ�.
	std::map<CString, CCopyOrder*> m_mapAcnt;

	Poco::RunnableAdapter<CESApiDllDlgDlg> *m_targetRcvThread;	// �ü����� �����带 ����
	CTargetRcv				m_targetRecv;
	Poco::Thread			m_targetRecvWrapper;

	Poco::RunnableAdapter<CESApiDllDlgDlg> *m_notedThread;	// �����ڷ� ���� �뺸 �޴� �����带 ����
	Poco::Thread							m_notedWrapper;

	Poco::MemoryPool*		m_pMemPool;

	CDBPool				*m_pDBPool;
	BOOL				m_bContinue;
	CTargetRcv			m_targetRcv;
	CBlockDup			m_Dup;

public:

	//FUNCs
	//VOID	LoadAutoAcnt(BOOL bReset);	// �ڵ������� �� ���¹�ȣ ���ε�
	VOID	IR_PrintMsg(BOOL bSucc, BOOL bShow, char* pMsg, ...);


	// ���º��� �ϳ��� �Ҵ��ϴ� COPY�ֹ� ���� ������
	//static unsigned WINAPI IR_CopyOrderThreadWrapper(LPVOID lp);
	//VOID IR_CopyOrderThread();


	// �����ڷκ��� ������Ʈ �ޱ� ����
	BOOL	IR_InitNotedThread();
	VOID	IR_CloseNotedThread();
	VOID	IR_NotedThread();

	BOOL	IR_DBOpen();
	VOID	IR_DBClose();
	BOOL	IR_CheckDBPool();

	BOOL	IR_InitMemPool();
	VOID	IR_CloseMemPool();
	
	LRESULT	IR_OnAcctList(WPARAM wParam, LPARAM lParam);		//	�α��ν� ���¸�� ����
	LRESULT IR_OnServerConnect(WPARAM wParam, LPARAM lParam);	//	��������
	void	IR_OnClickedButtonAuto();							//	�ǽð�ü�� ���

	// �α��ν� ���ŵ� �� ���¸�� ����
	BOOL	IR_SaveAcntInfo(CString &acnt, CString &nm);

	// �����ֹ� �������� ��ȸ
	BOOL	IR_LoadCopyOrderAcnt();

	// �����ŷ� ���� ����
	BOOL	IR_SaveTradeInfo(CString& sShortCode, char* pTarget, char* pHedge);

	void	IR_Login();
	BOOL	IR_InitTargetRecv();
	VOID	IR_CloseTargetRecv();
	VOID	IR_TargetRecvThread();

	//BOOL	IR_IsApiSend() { return (m_cRealYN == 'Y'); }
	VOID	IR_APILOCK() { EnterCriticalSection(&m_csAPI); };
	VOID	IR_APIUNLOCK() { LeaveCriticalSection(&m_csAPI); }


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
	CString				m_sAcntPass;
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

	/////////////////////////////////////////////////////////////
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
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonHedgeReload();
};
