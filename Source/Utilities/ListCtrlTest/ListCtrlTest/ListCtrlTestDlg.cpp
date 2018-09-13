
// ListCtrlTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ListCtrlTest.h"
#include "ListCtrlTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum { N_SYBOL = 0, N_BS, N_OPENPRC, N_ENTRYPRC, N_CURRPRC, N_PLTICK, N_QTY, N_PL };
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CListCtrlTestDlg 대화 상자



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


// CListCtrlTestDlg 메시지 처리기

BOOL CListCtrlTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CListCtrlTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CListCtrlTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CListCtrlTestDlg::OnBnClickedButtonCntr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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