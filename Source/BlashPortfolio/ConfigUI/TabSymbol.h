#pragma once


// CTabSymbol ��ȭ �����Դϴ�.

class CTabSymbol : public CDialogEx
{
	DECLARE_DYNAMIC(CTabSymbol)

public:
	CTabSymbol(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabSymbol();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SYMBOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
