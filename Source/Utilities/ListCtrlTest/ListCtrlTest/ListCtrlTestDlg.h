
// ListCtrlTestDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include <map>

// CListCtrlTestDlg 대화 상자
class CListCtrlTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CListCtrlTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTCTRLTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
