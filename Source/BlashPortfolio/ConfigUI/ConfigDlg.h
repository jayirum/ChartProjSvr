#pragma once
#include "afxcmn.h"


class CTabGeneral;
class CTabSymbol;

// ConfigDlg 대화 상자입니다.

class ConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfigDlg)

public:
	ConfigDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ConfigDlg();

	CTabGeneral	*m_pTabGeneral;
	CTabSymbol	*m_pTabSymbol;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIGDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	CTabCtrl m_tabGeneral;
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTabGeneral(NMHDR *pNMHDR, LRESULT *pResult);
};
