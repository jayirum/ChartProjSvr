
// ChartShmViewerDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

#include "../../IRUM_UTIL/SharedMem.h"
#include "../../IRUM_UTIL/Screwdb.h"
#include "../../IRUM_UTIL/util.h"
#include "../Common/KRCommon.h"
#include "afxwin.h"

#define CONFIG_FILE	"D:\\IRUM_VS\\KR_Hedge\\bin\\KRChart.ini"

// CChartShmViewerDlg 대화 상자
class CChartShmViewerDlg : public CDialogEx
{
// 생성입니다.
public:
	CChartShmViewerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARTSHMVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CDBPool				*m_pDBPool;
	CSharedMem			m_shm[2];

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListLastShm;
	CListCtrl m_ListChart;
	afx_msg void OnCbnSelchangeComboSymbol();
	afx_msg void OnCbnSelchangeComboChartTp();
	BOOL LoadSiseSymbol();
	
	CHART_TP GetChartTp(char* tpStr);
	BOOL InitShm();
	VOID CloseShm();
	
	afx_msg void OnClose();
	BOOL InitDB();
	VOID CloseDB();
	CComboBox m_ComboSymbol;
	CComboBox m_ComboChartTP;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSel();
	CString m_sNowTM;
};
