#pragma once


// CTabGeneral ��ȭ �����Դϴ�.

class CTabGeneral : public CDialogEx
{
	DECLARE_DYNAMIC(CTabGeneral)

public:
	CTabGeneral(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabGeneral();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GENERAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
