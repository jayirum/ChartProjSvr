
// ESApiDllDlgDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "math.h"
#include "afxwin.h"
#include <map>
#include "TrayIconMng.h"
#include "../Common/KRCommon.h"
#include "../../IRUM_UTIL/SharedMem.h"
#include "../../IRUM_UTIL/Screwdb.h"
#include "../../IRUM_UTIL/BlockDup.h"
#include "FBOrdThread.h"
#include "SiseRecv.h"
#include "SisePubSub.h"
#include "NotedThread.h"
#include "StratLog.h"

#include "Poco/MemoryPool.h"
#include "Poco/thread.h"
#include "Poco/Runnable.h"
#include "Poco/RunnableAdapter.h"
#include "../../IRUM_UTIL/TcpSrv.h"



//
using namespace Poco;

#define MEMPOOL_BLOCK_SIZE	sizeof(ST_PACK2ORD)
#define	MEMPOOL_MAX_UNIT	10000


// CESApiDllDlgDlg ��ȭ ����
class CESApiDllDlgDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CESApiDllDlgDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ESAPIDLLDLG_DIALOG};

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	CString			GetCurCode();
	CString			GetCurAccount();
	CString			GetAccount(int nSel);

	CESApi			m_Api;

	// members
	API_OBJ				m_apiObj;
	CRITICAL_SECTION	m_csApi;
	BOOL				m_bContinue;
	CString				m_sDir, m_sModule, m_sConfig;
	char				m_cRealYN;

	//CSharedMem			m_shm;// [2];
	std::map<CString, void*>	m_mapAcnt;	// acnt, thread
	long				m_siseSeq;

	Poco::MemoryPool*		m_pMemPool;
	CDBPool					*m_pDBPool;
	//std::string				m_sConnStr;

	Poco::RunnableAdapter<CESApiDllDlgDlg> *m_siseThread;	// �ü����� �����带 ����
	CSiseRecv				m_siseRecv;
	Poco::Thread			m_siseRecvWrapper;
	
	Poco::RunnableAdapter<CESApiDllDlgDlg> *m_notedThread;	// �����ڷ� ���� �뺸 �޴� �����带 ����
	Poco::Thread			m_notedWrapper;

	CSisePubSub				*m_publish;
	CStratLog				*m_pStratLog;	// �������� �α� - DB
	CTcpSrv					*m_pNoteSock;
	CBlockDup				m_Dup;
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
	BOOL	LoadAcntAndUserThread();
	VOID	ResumeThreads();
	VOID	CloseUserThread();

	VOID	PrintMsg(LOGMSG_TP tp, BOOL bShow, char* pMsg, ...);

	// FXChart ���� �ü� ������ ����
	BOOL	InitSock();
	VOID	CloseSock();
	VOID	SiseRecvThread();

	// �����ڷκ��� ������Ʈ �ޱ� ����
	BOOL	InitNotedThread();
	VOID	CloseNotedThread();
	VOID	NotedThread();
	VOID	SocketNoteProc(int nTimeout);
	
	//BOOL	InitChartSHM();
	//VOID	CloseChartSHM();
	
	BOOL	InitMemPool();
	VOID	CloseMemPool();
	
	BOOL	InitPublish();
	VOID	ClosePublish();

	BOOL	InitStratLog();
	VOID	CloseStratLog();

	BOOL	DBOpen();
	VOID	DBClose();
	BOOL	CheckDBPool();
	BOOL	SaveAcntInfo(CString &acnt, CString &nm/*, CString& pwd*/);
	
	
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
