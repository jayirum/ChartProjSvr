
// ListCtrlTestDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include <map>

// CListCtrlTestDlg ��ȭ ����
class CListCtrlTestDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CListCtrlTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTCTRLTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctlRealPl;
	afx_msg void OnBnClickedButtonCntr();
	void calc();
private:
	std::map<CString, UINT>	m_mapIdx;
};
