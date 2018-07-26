
// HDFCommClientDlg.h : 헤더 파일
//

#pragma once
#include "hdfcommagent.h"
#include "afxwin.h"



#include "../../IRUM_UTIL/LogMsg.h"
#include <set>	
#include "../../IRUM_UTIL/BlockDup.h"

// CHDFCommClientDlg 대화 상자
class CHDFCommClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CHDFCommClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_HDFCOMMCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


////////////////////////////////////////////////////////////////////// JAY
public:
	VOID	IR_PrintMsg(LOGMSG_TP tp, BOOL bShow, char* pMsg, ...);
	VOID	IR_APILOCK() { EnterCriticalSection(&m_csAPI); }
	VOID	IR_APIUNLOCK() { LeaveCriticalSection(&m_csAPI); }
	char*	IR_ID() { return m_sUserId.GetBuffer(); };
	BOOL	IR_DBOpen();
	

	// Customizing
private:
	BOOL				m_bContinue;
	CString				m_sDir, m_sModule, m_sConfig;
	CRITICAL_SECTION	m_csApi;
	char				m_cRealYN;
	char				m_szMsg[1024];
	BOOL				m_bInit;

	CRITICAL_SECTION	m_csAPI;		// API TR SEND 가 thread-safe 한지 확실치 않다.
	VOID				*m_pDBPool;
	CBlockDup			m_Dup;

	//CString m_szUserID;
	//CString m_szUserPass;
	//CString m_szCertPass;
	CString m_sDllPath;

	CString		m_sLastChart, m_sLastTmGb;
	int m_nLastChartTp;
	CString m_sSymbol;
////////////////////////////////////////////////////////////////////// JAY

public:
	CString m_sUserId;
	CString m_sUserPw;
	CString m_sAuthPw;
	CString m_sCode;

	CString m_strNextKey;

	//CEdit m_edOutput;
	CEdit m_edTrCode;
	CEdit m_edInput;
	CEdit m_edTrCode2;
	CEdit m_edInput2;
	CEdit m_edInput3;
	CEdit m_edAccNo;
	CEdit m_edJumunInput;
	CEdit m_edJumunTR;
	CEdit m_edJomgmok;
	CEdit m_edGubun;
	CEdit m_editUserID;
	CEdit m_editPwd;
	CEdit m_editAuthPwd;
	CEdit m_edGetInfo;

	//CEdit m_edSeriesO; // Fid & Real시 표시되는 값 종목
	//CEdit m_edTimeO  ; // Fid & Real시 표시되는 값 체결시간
	//CEdit m_edClosePO; // Fid & Real시 표시되는 값 현재가
	//CEdit m_edVolumeO; // Fid & Real시 표시되는 값 누적거래량

	CListBox m_ListOutMsg;

	CHDFCommAgent m_CommAgent;

	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void IR_OnBnClickedLogin();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedRqtest();
	afx_msg void OnBnClickedSbtest();
	afx_msg void OnBnClickedSbremove();
	afx_msg void IR_OnBnClickedInitcomm();
	afx_msg void OnBnClickedTerminate();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void IR_OnBnClickedJumunchesummit();
	afx_msg void OnBnClickedJumunchecancle();
	afx_msg void OnBnClickedJumunChe();


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	DECLARE_EVENTSINK_MAP()
	afx_msg void OnDataRecv(CString sTrCode, LONG nRqID);
	afx_msg void OnGetBroadData(CString strKey, LONG nRealType);
	afx_msg void OnGetMsg(CString strCode, CString strMsg);
	afx_msg void OnGetMsgWithRqId(int nRqId, CString strCode, CString strMsg);

private:
	//void WriteLog(CString strMsg);
	CString m_strOrdNo;
	CString FillStr(CString strSrc, int nTotLen, BOOL bRightAdd=TRUE, char chPad=' ');
	void ShowHWOrdCtrl(BOOL bShow);

public:
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBusinessday();
	afx_msg void OnBnClickedFrbusinessday();
	afx_msg void OnBnClickedCommgetinfo();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnSavelog();
	afx_msg void OnBnClickedBtnLogclear();
	afx_msg void OnBnClickedOrdApply();
	afx_msg void OnCbnSelchangeOrdComboOrdertr();

//////////////////////////////////////////////////////////////////////
// JAY
public:
	void OnAppExit(void);
	void OnDialogShow(void);
	afx_msg void OnBnClickedButtonTerminate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonChart();
	afx_msg void OnBnClickedButtonChart2();

};
