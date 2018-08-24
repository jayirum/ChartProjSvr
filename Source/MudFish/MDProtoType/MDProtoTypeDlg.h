
// MDProtoTypeDlg.h : 헤더 파일
//

#pragma once
#pragma warning(disable:4996)

#include "../../IRUM_UTIL/BlockDup.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/TcpClient.h"
#include "StratMaker.h"
#include "TrayIconMng.h"
#include "afxwin.h"
#include <map>

// CMDProtoTypeDlg 대화 상자
class CMDProtoTypeDlg : public CDialogEx
{
// 생성입니다.
public:
	CMDProtoTypeDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MDPROTOTYPE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	static unsigned WINAPI Thread_TickDataRecv(LPVOID lp);
	static unsigned WINAPI Thread_SendOrd(LPVOID lp);

	BOOL	Begin();
	VOID	End();

private:
	CBlockDup		m_Dup;
	CMemPool		*m_pMemPool;
	CTcpClient		*m_pApiRecv;

	std::string		m_sApiIP;
	int				m_nApiPort;
	std::string		m_cfgFile;

	HANDLE			m_hRecvThread, m_hOrdSendThread;
	unsigned int	m_unRecvThread, m_unOrdSendThread;

	std::map<std::string, CStratMaker*>		m_mapStrat;
	CStratMaker* m_pStrat;
	CString m_sSymbol;
	CString m_sOpenPrc;
	CString m_sCloseTime;
	CString m_sMaxSLCnt;
	//TRAYICON
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
	CListBox m_lstMsg;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CEdit m_edCurrPrc;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCurr();
	afx_msg void OnBnClickedButtonClosetm();
};
