#pragma once


// CTabSymbol 대화 상자입니다.

class CTabSymbol : public CDialogEx
{
	DECLARE_DYNAMIC(CTabSymbol)

public:
	CTabSymbol(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabSymbol();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SYMBOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
