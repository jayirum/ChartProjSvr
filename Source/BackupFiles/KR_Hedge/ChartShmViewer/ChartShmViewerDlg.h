
// ChartShmViewerDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

#include "../../IRUM_UTIL/SharedMem.h"
#include "../../IRUM_UTIL/Screwdb.h"
#include "../../IRUM_UTIL/util.h"
#include "../Common/KRCommon.h"
#include "afxwin.h"

#define CONFIG_FILE	"D:\\IRUM_VS\\KR_Hedge\\bin\\KRChart.ini"

// CChartShmViewerDlg ��ȭ ����
class CChartShmViewerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CChartShmViewerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARTSHMVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	CDBPool				*m_pDBPool;
	CSharedMem			m_shm[2];

	// ������ �޽��� �� �Լ�
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
