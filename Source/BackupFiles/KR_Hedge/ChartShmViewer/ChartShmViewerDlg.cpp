
// ChartShmViewerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ChartShmViewer.h"
#include "ChartShmViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
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


// CChartShmViewerDlg ��ȭ ����



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


// CChartShmViewerDlg �޽��� ó����

BOOL CChartShmViewerDlg::OnInitDialog()
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
	CRect rc;
	int   cx = 0;

	m_ListChart.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListChart.GetWindowRect(&rc);
	cx = (rc.Width() - ::GetSystemMetrics(SM_CXVSCROLL) - ::GetSystemMetrics(SM_CXBORDER) * 4) / 8;


	//m_ListChart.InsertColumn(0, "��Ʈ��", LVCFMT_CENTER, cx-30);
	//m_ListChart.InsertColumn(1, "������Ʈ", LVCFMT_CENTER, cx-30);
	//m_ListChart.InsertColumn(2, "�����", LVCFMT_CENTER, cx-30);
	//m_ListChart.InsertColumn(3, "�ð�", LVCFMT_RIGHT, cx+15);
	//m_ListChart.InsertColumn(4, "��", LVCFMT_RIGHT, cx + 15);
	//m_ListChart.InsertColumn(5, "����", LVCFMT_RIGHT, cx + 15);
	//m_ListChart.InsertColumn(6, "����", LVCFMT_RIGHT, cx + 15);
	////m_ListChart.InsertColumn(7, "�Ҽ���", LVCFMT_CENTER, cx);
	//m_ListChart.InsertColumn(7, "SEQ", LVCFMT_CENTER, cx);
	////m_ListChart.InsertColumn(8, "����YN", LVCFMT_CENTER, cx);

	m_ListChart.InsertColumn(0, "��Ʈ��", LVCFMT_CENTER, 80);
	m_ListChart.InsertColumn(1, "��Ŷ�ð�", LVCFMT_CENTER, 100);
	m_ListChart.InsertColumn(2, "������Ʈ", LVCFMT_CENTER, 80);
	m_ListChart.InsertColumn(3, "�����", LVCFMT_CENTER, 40);
	m_ListChart.InsertColumn(4, "�ð�", LVCFMT_RIGHT, 155);
	m_ListChart.InsertColumn(5, "��", LVCFMT_RIGHT, 155);
	m_ListChart.InsertColumn(6, "����", LVCFMT_RIGHT, 155);
	m_ListChart.InsertColumn(7, "����", LVCFMT_RIGHT, 155);
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CChartShmViewerDlg::OnPaint()
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
HCURSOR CChartShmViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChartShmViewerDlg::OnCbnSelchangeComboSymbol()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CChartShmViewerDlg::OnCbnSelchangeComboChartTp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		if (strncmp("Y", db->Get("CHART_1MIN_YN"), 1) == 0)	m_ComboChartTP.AddString("1����Ʈ");
		if (strncmp("Y", db->Get("CHART_5MIN_YN"), 1) == 0) m_ComboChartTP.AddString("5����Ʈ");
		if (strncmp("Y", db->Get("CHART_10MIN_YN"), 1) == 0) m_ComboChartTP.AddString("10����Ʈ");
		if (strncmp("Y", db->Get("CHART_30MIN_YN"), 1) == 0) m_ComboChartTP.AddString("30����Ʈ");
		if (strncmp("Y", db->Get("CHART_60MIN_YN"), 1) == 0) m_ComboChartTP.AddString("60����Ʈ");
	}

	if (m_ComboChartTP.GetCount() > 0)
		m_ComboChartTP.SetCurSel(0);


	return TRUE;

}



void CChartShmViewerDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}

CHART_TP CChartShmViewerDlg::GetChartTp(char* tpStr)
{
	CHART_TP tp;
	if (strcmp(tpStr, "1����Ʈ") == 0)	tp = TP_1MIN;
	if (strcmp(tpStr, "5����Ʈ") == 0)	tp = TP_5MIN;
	if (strcmp(tpStr, "10����Ʈ") == 0)	tp = TP_10MIN;
	if (strcmp(tpStr, "30����Ʈ") == 0)	tp = TP_30MIN;
	if (strcmp(tpStr, "60����Ʈ") == 0)	tp = TP_60MIN;

	return tp;
}

void CChartShmViewerDlg::OnBnClickedButtonSel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// ������ ���� ��Ʈ �̸�
	ST_SHM_LASTCHART lastNmSHM;
	if( !m_shm[LASTNM].GetData(zGroupKey, zGroupKey, (char*)&lastNmSHM) )
	{
		CloseShm();
		return ;
	}
	sprintf(zChartNm,"%.*s", sizeof(lastNmSHM.LastChartNm), lastNmSHM.LastChartNm);
	m_ListLastShm.InsertItem(0, zGroupKey);
	m_ListLastShm.SetItemText(0, 1, zChartNm);	// ������Ʈ

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

	m_ListChart.InsertItem(0, zChartNm);	//��Ʈ��
	m_ListChart.SetItemText(0, 1, SUBSTR(shmNow.seq, 0, sizeof(shmNow.seq)));
	m_ListChart.SetItemText(0, 2, zPrevNm);	// ������Ʈ
	m_ListChart.SetItemText(0, 3, SUBSTR(shmNow.gb,0,1));	// �����
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
		m_ListChart.InsertItem(i+1, zPrevNm);	//��Ʈ��
		m_ListChart.SetItemText(i + 1, 1, SUBSTR(shmNow.seq, 0, sizeof(shmNow.seq)));

		sprintf(zPrevNm, "%.*s", LEN_CHART_NM, shmNow.prevNm);
		m_ListChart.SetItemText(i + 1, 2, zPrevNm);	// ������Ʈ
		
		m_ListChart.SetItemText(i + 1, 3, SUBSTR(shmNow.gb, 0, 1));	// �����
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


