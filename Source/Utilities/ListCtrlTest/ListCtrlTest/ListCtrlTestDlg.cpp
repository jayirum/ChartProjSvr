
// ListCtrlTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ListCtrlTest.h"
#include "ListCtrlTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum { N_SYBOL = 0, N_BS, N_OPENPRC, N_ENTRYPRC, N_CURRPRC, N_PLTICK, N_QTY, N_PL };
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CListCtrlTestDlg ��ȭ ����



CListCtrlTestDlg::CListCtrlTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LISTCTRLTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListCtrlTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlRealPl);
}

BEGIN_MESSAGE_MAP(CListCtrlTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CNTR, &CListCtrlTestDlg::OnBnClickedButtonCntr)
END_MESSAGE_MAP()


// CListCtrlTestDlg �޽��� ó����

BOOL CListCtrlTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_ctlRealPl.SetExtendedStyle(m_ctlRealPl.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_DOUBLEBUFFER
	);


	m_ctlRealPl.InsertColumn(N_SYBOL, _T("Symbol"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_BS, _T("BuySell"), LVCFMT_CENTER, 129, -1);
	m_ctlRealPl.InsertColumn(N_OPENPRC, _T("OpenPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_ENTRYPRC, _T("EntryPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_CURRPRC, _T("CurrPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_PLTICK, _T("PL Tick"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_QTY, _T("Qty"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_PL, _T("PL"), LVCFMT_CENTER, 90, -1);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CListCtrlTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CListCtrlTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CListCtrlTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CListCtrlTestDlg::OnBnClickedButtonCntr()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString symbol, currprc;
	GetDlgItemText(IDC_EDIT_SYMBOL, symbol);
	GetDlgItemText(IDC_EDIT_CURRPRC, currprc);
	
	std::map<CString, UINT>::iterator it = m_mapIdx.find(symbol);
	if (it == m_mapIdx.end())
	{
		int nIdx = m_ctlRealPl.GetItemCount();

		m_ctlRealPl.InsertItem(nIdx, symbol);
		m_ctlRealPl.SetItem(nIdx, N_BS, LVIF_TEXT, _T("SELL"), 0, 0, 0, NULL);
		m_ctlRealPl.SetItem(nIdx, N_OPENPRC, LVIF_TEXT, _T("80.10"), 0, 0, 0, NULL);
		m_ctlRealPl.SetItem(nIdx, N_ENTRYPRC, LVIF_TEXT, _T("80.20"), 0, 0, 0, NULL);
		m_ctlRealPl.SetItem(nIdx, N_CURRPRC, LVIF_TEXT, currprc, 0, 0, 0, NULL);
		m_ctlRealPl.SetItem(nIdx, N_PLTICK, LVIF_TEXT, _T(""), 0, 0, 0, NULL);
		m_ctlRealPl.SetItem(nIdx, N_QTY, LVIF_TEXT, _T("1"), 0, 0, 0, NULL);
		m_ctlRealPl.SetItem(nIdx, N_PL, LVIF_TEXT, _T(""), 0, 0, 0, NULL);

		m_mapIdx[symbol] = nIdx;
	}
	else
	{
		calc();
	}
}

void CListCtrlTestDlg::calc()
{
	CString symbol;
	GetDlgItemText(IDC_EDIT_SYMBOL, symbol);
	std::map<CString, UINT>::iterator it = m_mapIdx.find(symbol);

	if (it == m_mapIdx.end())
		return;

	UINT idx = (*it).second;
	CString sBs, sEntry, sCurr, sQty;

	sBs = m_ctlRealPl.GetItemText(idx, N_BS);
	sEntry = m_ctlRealPl.GetItemText(idx, N_ENTRYPRC);
	sCurr = m_ctlRealPl.GetItemText(idx, N_CURRPRC);
	sQty = m_ctlRealPl.GetItemText(idx, N_QTY);

	double dTickGap = (atof(sEntry.GetString()) - atof(sCurr.GetString())) / 0.01;
	double dPL = atof(sQty.GetString()) * dTickGap * 12.5;

	char zGap[32], zPl[32];
	sprintf(zGap, "%.0f", dTickGap);
	sprintf(zPl, "%.0f", dPL);
	
	m_ctlRealPl.SetItem(idx, N_PLTICK, LVIF_TEXT, _T(zGap), 0, 0, 0, NULL);
	m_ctlRealPl.SetItem(idx, N_PL, LVIF_TEXT, _T(zPl), 0, 0, 0, NULL);
}