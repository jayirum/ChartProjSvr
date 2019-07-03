#pragma once
#include "afxcmn.h"


class CTabGeneral;
class CTabSymbol;

// ConfigDlg ��ȭ �����Դϴ�.

class ConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfigDlg)

public:
	ConfigDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ConfigDlg();

	CTabGeneral	*m_pTabGeneral;
	CTabSymbol	*m_pTabSymbol;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIGDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
//	CTabCtrl m_tabGeneral;
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTabGeneral(NMHDR *pNMHDR, LRESULT *pResult);
};
