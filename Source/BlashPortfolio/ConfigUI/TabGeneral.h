#pragma once


// CTabGeneral 대화 상자입니다.

class CTabGeneral : public CDialogEx
{
	DECLARE_DYNAMIC(CTabGeneral)

public:
	CTabGeneral(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabGeneral();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GENERAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
