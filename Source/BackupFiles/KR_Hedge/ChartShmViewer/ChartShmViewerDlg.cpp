
// ChartShmViewerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ChartShmViewer.h"
#include "ChartShmViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
char buf[128];
char* cvt(char* input)
{
	sprintf(buf, "%10.5f", atof(input));
	return buf;
}

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


// CChartShmViewerDlg 대화 상자



CChartShmViewerDlg::CChartShmViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHARTSHMVIEWER_DIALOG, pParent)
	, m_sNowTM(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDBPool = NULL;
}

void CChartShmViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LASTSHM, m_ListLastShm);
	DDX_Control(pDX, IDC_LIST_CHART, m_ListChart);
	DDX_Control(pDX, IDC_COMBO_SYMBOL, m_ComboSymbol);
	DDX_Control(pDX, IDC_COMBO_CHART_TP, m_ComboChartTP);
	DDX_Text(pDX, IDC_EDIT_TM, m_sNowTM);
}

BEGIN_MESSAGE_MAP(CChartShmViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_SYMBOL, &CChartShmViewerDlg::OnCbnSelchangeComboSymbol)
	ON_CBN_SELCHANGE(IDC_COMBO_CHART_TP, &CChartShmViewerDlg::OnCbnSelchangeComboChartTp)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CChartShmViewerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SEL, &CChartShmViewerDlg::OnBnClickedButtonSel)
END_MESSAGE_MAP()


// CChartShmViewerDlg 메시지 처리기

BOOL CChartShmViewerDlg::OnInitDialog()
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
	CRect rc;
	int   cx = 0;

	m_ListChart.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListChart.GetWindowRect(&rc);
	cx = (rc.Width() - ::GetSystemMetrics(SM_CXVSCROLL) - ::GetSystemMetrics(SM_CXBORDER) * 4) / 8;


	//m_ListChart.InsertColumn(0, "차트명", LVCFMT_CENTER, cx-30);
	//m_ListChart.InsertColumn(1, "직전차트", LVCFMT_CENTER, cx-30);
	//m_ListChart.InsertColumn(2, "음양봉", LVCFMT_CENTER, cx-30);
	//m_ListChart.InsertColumn(3, "시가", LVCFMT_RIGHT, cx+15);
	//m_ListChart.InsertColumn(4, "고가", LVCFMT_RIGHT, cx + 15);
	//m_ListChart.InsertColumn(5, "저가", LVCFMT_RIGHT, cx + 15);
	//m_ListChart.InsertColumn(6, "종가", LVCFMT_RIGHT, cx + 15);
	////m_ListChart.InsertColumn(7, "소숫점", LVCFMT_CENTER, cx);
	//m_ListChart.InsertColumn(7, "SEQ", LVCFMT_CENTER, cx);
	////m_ListChart.InsertColumn(8, "전략YN", LVCFMT_CENTER, cx);

	m_ListChart.InsertColumn(0, "차트명", LVCFMT_CENTER, 80);
	m_ListChart.InsertColumn(1, "패킷시각", LVCFMT_CENTER, 100);
	m_ListChart.InsertColumn(2, "직전차트", LVCFMT_CENTER, 80);
	m_ListChart.InsertColumn(3, "음양봉", LVCFMT_CENTER, 40);
	m_ListChart.InsertColumn(4, "시가", LVCFMT_RIGHT, 155);
	m_ListChart.InsertColumn(5, "고가", LVCFMT_RIGHT, 155);
	m_ListChart.InsertColumn(6, "저가", LVCFMT_RIGHT, 155);
	m_ListChart.InsertColumn(7, "종가", LVCFMT_RIGHT, 155);
	m_ListChart.InsertColumn(8, "LMA", LVCFMT_CENTER, 155);
	m_ListChart.InsertColumn(9, "SMA", LVCFMT_CENTER, 155);
	m_ListChart.InsertColumn(10, "Signal", LVCFMT_CENTER, 40);

	m_ListLastShm.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListLastShm.GetWindowRect(&rc);
	cx = (rc.Width() - ::GetSystemMetrics(SM_CXVSCROLL) - ::GetSystemMetrics(SM_CXBORDER) * 4) / 2;

	m_ListLastShm.InsertColumn(0, "G KEY", LVCFMT_CENTER, cx);
	m_ListLastShm.InsertColumn(1, "CHART NM", LVCFMT_CENTER, cx);


	//if (!InitShm()) {
	//	AfxMessageBox("SHM INIT ERROR");
	//	return FALSE;
	//}
		

	if (!InitDB()) {
		AfxMessageBox("DB INIT ERROR");
		return FALSE;
	}

	if (!LoadSiseSymbol()) {
		AfxMessageBox("Load Symbol ERROR");
		return FALSE;
	}


	((CWnd*)GetDlgItem(IDC_BUTTON_SEL))->SetFocus();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


BOOL CChartShmViewerDlg::InitShm()
{
	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
	CUtil::GetConfig(CONFIG_FILE, "SHM_INFO", "SHM_NAME", szSHMName);
	CUtil::GetConfig(CONFIG_FILE, "SHM_INFO", "MUTEX_NAME", szMutexName);

	//	OPEN SHM
	if (!m_shm[CHART].Open(szSHMName, szMutexName))
	{	
		return FALSE;
	}


	CUtil::GetConfig(CONFIG_FILE, "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
	CUtil::GetConfig(CONFIG_FILE, "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);
	if (!m_shm[LASTNM].Open(szSHMName, szMutexName))
	{
		return FALSE;
	}
	return TRUE;
}

VOID CChartShmViewerDlg::CloseShm()
{
	m_shm[CHART].Close();
	m_shm[LASTNM].Close();
}


void CChartShmViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChartShmViewerDlg::OnPaint()
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
HCURSOR CChartShmViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChartShmViewerDlg::OnCbnSelchangeComboSymbol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CChartShmViewerDlg::OnCbnSelchangeComboChartTp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CChartShmViewerDlg::LoadSiseSymbol()
{
	CDBHandler db(m_pDBPool->Get());
	CString sQ("SELECT SYMBOL FROM SYMBOL_INFO WHERE FB_ORD_YN='Y'");

	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		return FALSE;
	}

	while (db->NextRow())
	{
		CString symbol(db->Get("SYMBOL"));
		m_ComboSymbol.AddString(symbol);
	}
	if (m_ComboSymbol.GetCount() > 0)
		m_ComboSymbol.SetCurSel(0);

	db->Close();

	sQ = "SELECT CHART_1MIN_YN, CHART_5MIN_YN, CHART_10MIN_YN, CHART_30MIN_YN, CHART_60MIN_YN FROM FB_STRATEGY_FACTOR";
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		return FALSE;
	}
	if (db->NextRow())
	{
		if (strncmp("Y", db->Get("CHART_1MIN_YN"), 1) == 0)	m_ComboChartTP.AddString("1분차트");
		if (strncmp("Y", db->Get("CHART_5MIN_YN"), 1) == 0) m_ComboChartTP.AddString("5분차트");
		if (strncmp("Y", db->Get("CHART_10MIN_YN"), 1) == 0) m_ComboChartTP.AddString("10분차트");
		if (strncmp("Y", db->Get("CHART_30MIN_YN"), 1) == 0) m_ComboChartTP.AddString("30분차트");
		if (strncmp("Y", db->Get("CHART_60MIN_YN"), 1) == 0) m_ComboChartTP.AddString("60분차트");
	}

	if (m_ComboChartTP.GetCount() > 0)
		m_ComboChartTP.SetCurSel(0);


	return TRUE;

}



void CChartShmViewerDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CloseShm();
	CloseDB();
	CDialogEx::OnClose();
}


BOOL CChartShmViewerDlg::InitDB()
{
	char ip[32], id[32], pwd[32], cnt[32];
	CUtil::GetConfig(CONFIG_FILE, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(CONFIG_FILE, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(CONFIG_FILE, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(CONFIG_FILE, "DBINFO", "DB_POOL_CNT", cnt);

	BOOL bUseODBC = FALSE;
	m_pDBPool = new CDBPool(bUseODBC, ip, id, pwd, "KRHedge");
	if (!m_pDBPool->Init(atoi(cnt)))
	{
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}


	return TRUE;
}


VOID CChartShmViewerDlg::CloseDB()
{
	SAFE_DELETE(m_pDBPool);
	return VOID();
}


void CChartShmViewerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

CHART_TP CChartShmViewerDlg::GetChartTp(char* tpStr)
{
	CHART_TP tp;
	if (strcmp(tpStr, "1분차트") == 0)	tp = TP_1MIN;
	if (strcmp(tpStr, "5분차트") == 0)	tp = TP_5MIN;
	if (strcmp(tpStr, "10분차트") == 0)	tp = TP_10MIN;
	if (strcmp(tpStr, "30분차트") == 0)	tp = TP_30MIN;
	if (strcmp(tpStr, "60분차트") == 0)	tp = TP_60MIN;

	return tp;
}

void CChartShmViewerDlg::OnBnClickedButtonSel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sNowTM.Trim();

	char zTM[32] = { 0, };
	if (m_sNowTM.GetLength() > 0) {
		sprintf(zTM, "%.2s%.2s%.2s%.2s",
			m_sNowTM.GetBuffer(), m_sNowTM.GetBuffer() + 2,
			m_sNowTM.GetBuffer() + 4, m_sNowTM.GetBuffer() + 6
		);
	}


	m_ListLastShm.DeleteAllItems();
	m_ListChart.DeleteAllItems();

	CString sSymbol; m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);
	CString sTp; m_ComboChartTP.GetLBText(m_ComboChartTP.GetCurSel(), sTp);

	char zGroupKey[LEN_SHM_GROUP_KEY + 1] = { 0, };
	char zChartNm[LEN_CHART_NM + 1], zPrevNm[LEN_CHART_NM + 1];

	// GROUP KEY
	GET_GROUP_KEY(sSymbol.GetBuffer(), GetChartTp(sTp.GetBuffer()), zGroupKey);

	if (!InitShm()) {
		AfxMessageBox("SHM INIT ERROR");
		return ;
	}

	// 마지막 저장 차트 이름
	ST_SHM_LASTCHART lastNmSHM;
	if( !m_shm[LASTNM].GetData(zGroupKey, zGroupKey, (char*)&lastNmSHM) )
	{
		CloseShm();
		return ;
	}
	sprintf(zChartNm,"%.*s", sizeof(lastNmSHM.LastChartNm), lastNmSHM.LastChartNm);
	m_ListLastShm.InsertItem(0, zGroupKey);
	m_ListLastShm.SetItemText(0, 1, zChartNm);	// 직전차트

	ST_SHM_CHART_UNIT shmNow;
	// CHART NAME
	if (m_sNowTM.GetLength() == 0)
	{
		if (!m_shm[CHART].GetData(zGroupKey, zChartNm, (char*)&shmNow)) {
			CloseShm();
			return;
		}
	}
	else
	{
		CHART_TP tp = GetChartTp(sTp.GetBuffer());
		//GET_CHART_NM(zTM, tp, zChartNm);
		strcpy(zChartNm, zTM);
		if (!m_shm[CHART].GetData(zGroupKey, zChartNm, (char*)&shmNow)) {
			CloseShm();
			return;
		}
	}
	sprintf(zPrevNm, "%.*s", LEN_CHART_NM, shmNow.prevNm);	

	m_ListChart.InsertItem(0, zChartNm);	//차트명
	m_ListChart.SetItemText(0, 1, SUBSTR(shmNow.seq, 0, sizeof(shmNow.seq)));
	m_ListChart.SetItemText(0, 2, zPrevNm);	// 직전차트
	m_ListChart.SetItemText(0, 3, SUBSTR(shmNow.gb,0,1));	// 음양봉
	m_ListChart.SetItemText(0, 4, cvt(SUBSTR(shmNow.open, 0, LEN_PRC)));
	m_ListChart.SetItemText(0, 5, cvt(SUBSTR(shmNow.high, 0, LEN_PRC)));
	m_ListChart.SetItemText(0, 6, cvt(SUBSTR(shmNow.low, 0, LEN_PRC)));
	m_ListChart.SetItemText(0, 7, cvt(SUBSTR(shmNow.close, 0, LEN_PRC)));
	m_ListChart.SetItemText(0, 8, cvt(SUBSTR(shmNow.sma_long, 0, LEN_PRC)));
	m_ListChart.SetItemText(0, 9, cvt(SUBSTR(shmNow.sma_short, 0, LEN_PRC)));
	m_ListChart.SetItemText(0, 10, SUBSTR(shmNow.signalGb, 0, 1));


	for (int i = 0; i < 10; i++)
	{
		ZeroMemory(&shmNow, sizeof(shmNow));
		BOOL bFind = m_shm[CHART].GetData(zGroupKey, zPrevNm, (char*)&shmNow);
		m_ListChart.InsertItem(i+1, zPrevNm);	//차트명
		m_ListChart.SetItemText(i + 1, 1, SUBSTR(shmNow.seq, 0, sizeof(shmNow.seq)));

		sprintf(zPrevNm, "%.*s", LEN_CHART_NM, shmNow.prevNm);
		m_ListChart.SetItemText(i + 1, 2, zPrevNm);	// 직전차트
		
		m_ListChart.SetItemText(i + 1, 3, SUBSTR(shmNow.gb, 0, 1));	// 음양봉
		m_ListChart.SetItemText(i + 1, 4, cvt(SUBSTR(shmNow.open, 0, LEN_PRC)));
		m_ListChart.SetItemText(i + 1, 5, cvt(SUBSTR(shmNow.high, 0, LEN_PRC)));
		m_ListChart.SetItemText(i + 1, 6, cvt(SUBSTR(shmNow.low, 0, LEN_PRC)));
		m_ListChart.SetItemText(i + 1, 7, cvt(SUBSTR(shmNow.close, 0, LEN_PRC)));
		m_ListChart.SetItemText(i + 1, 8, cvt(SUBSTR(shmNow.sma_long, 0, LEN_PRC)));
		m_ListChart.SetItemText(i + 1, 9, cvt(SUBSTR(shmNow.sma_short, 0, LEN_PRC)));
		m_ListChart.SetItemText(i + 1, 10, SUBSTR(shmNow.signalGb, 0, 1));
	}

	CloseShm();

	UpdateData(FALSE);
}


