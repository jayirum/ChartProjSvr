
// ESApiDllDlgDlg.cpp : ���� ����
//
#include "stdafx.h"
#include "ESApiDllDlg.h"
#include "ESApiDllDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ESM_TEST	WM_USER + 0x9000

// CESApiDllDlgDlg ��ȭ ����

#define DEF_LOG_LEN 4096


#define ID_TIME_LOGON			101	// ���� �α��� �õ��ϴ� Ÿ�̸�
#define ID_TIME_SYMBOL_MASTER	201



CESApiDllDlgDlg::CESApiDllDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CESApiDllDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szUserID = _T("");
	m_szUserPass = _T("");
	m_szCertPass = _T("");
	m_szCurPrice = _T("");
	m_szCurDebi = _T("");
	m_szNowVol = _T("");
	m_szVolume = _T("");
	m_szDHoka1 = _T("");
	m_szDHoka2 = _T("");
	m_szDHoka3 = _T("");
	m_szSHoka1 = _T("");
	m_szSHoka2 = _T("");
	m_szSHoka3 = _T("");

	m_szAutoAcctKey	= 
	m_szAutoCodeKey	= _T("");	

	m_sDllPath = _T("");

	m_log = NULL;
}

void CESApiDllDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_ACCT, m_cmbAcct);
	DDX_Control(pDX, IDC_COMBO_CODE, m_cmbCode);
	DDX_Text(pDX, IDC_EDIT_PASS, m_szHedgePass);
	DDV_MaxChars(pDX, m_szHedgePass, 8);
	DDX_Text(pDX, IDC_EDIT_PASS2, m_szOrdP);
	DDV_MaxChars(pDX, m_szOrdP, 20);
	DDX_Text(pDX, IDC_EDIT_PASS3, m_szOrdV);
	DDV_MaxChars(pDX, m_szOrdV, 20);
	DDX_Text(pDX, IDC_EDIT_PASS4, m_szOrgNo);
	DDX_Control(pDX, IDC_LIST1, m_List1);
	DDX_Control(pDX, IDC_LIST2, m_List2);
	DDX_Control(pDX, IDC_LIST_NEARSERIES, m_listNearSeries);
	DDX_Control(pDX, IDC_LIST_SURENET, m_listSurenet);
	DDX_Text(pDX, IDC_EDIT_USERID, m_szUserID);
	DDV_MaxChars(pDX, m_szUserID, 8);
	DDX_Text(pDX, IDC_EDIT_USERPASS, m_szUserPass);
	DDV_MaxChars(pDX, m_szUserPass, 8);
	DDX_Text(pDX, IDC_EDIT_CERTPASS, m_szCertPass);
	DDX_Text(pDX, IDC_EDIT_CURPRIC, m_szCurPrice);
	DDX_Text(pDX, IDC_EDIT_DEBI, m_szCurDebi);
	DDX_Text(pDX, IDC_EDIT_NOWVOL, m_szNowVol);
	DDX_Text(pDX, IDC_EDIT_VOLUME, m_szVolume);
	//  DDX_Control(pDX, IDC_LIST_CHE, m_szListChe);
	DDX_Control(pDX, IDC_LIST_CHE, m_ListChe);
	//  DDX_Control(pDX, IDC_EDIT_DHOKA1, m_szDHoka1);
	DDX_Text(pDX, IDC_EDIT_DHOKA1, m_szDHoka1);
	//  DDX_Control(pDX, IDC_EDIT_DHOKA2, m_szDHoka2);
	DDX_Text(pDX, IDC_EDIT_DHOKA2, m_szDHoka2);
	DDX_Text(pDX, IDC_EDIT_DHOKA3, m_szDHoka3);
	DDX_Text(pDX, IDC_EDIT_SHOKA1, m_szSHoka1);
	DDX_Text(pDX, IDC_EDIT_SHOKA2, m_szSHoka2);
	DDX_Text(pDX, IDC_EDIT_SHOKA3, m_szSHoka3);
	DDX_Text(pDX, IDC_EDIT1, m_sDllPath);
	DDX_Control(pDX, IDC_MSG, m_lstMsg);
	DDX_Control(pDX, IDC_STATIC_REAL, m_sRealYN);
}

BEGIN_MESSAGE_MAP(CESApiDllDlgDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDOK, &CESApiDllDlgDlg::OnBnClickedOk)

	ON_BN_CLICKED(IDC_BUTTON1, &CESApiDllDlgDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CESApiDllDlgDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3, &CESApiDllDlgDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CESApiDllDlgDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON4, &CESApiDllDlgDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CESApiDllDlgDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CESApiDllDlgDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CESApiDllDlgDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CESApiDllDlgDlg::OnBnClickedButton8)

	ON_MESSAGE(WM_ESM_RECVDATA, OnRecvData)
	ON_MESSAGE(WM_ESM_ACCTLIST, OnAcctList)
	ON_MESSAGE(WM_ESM_FCACCTLIST, OnFCAcctList)
	ON_MESSAGE(WM_ESM_CODELIST, OnCodeList)
	ON_MESSAGE(WM_ESM_CONNECT,	OnServerConnect)
	ON_MESSAGE(WM_ESM_DISCONNECT, OnServerDisConnect)
	ON_BN_CLICKED(IDC_BUTTON9, &CESApiDllDlgDlg::OnBnClickedButton9)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCT, &CESApiDllDlgDlg::OnSelchangeComboAcct)
	ON_CBN_SELCHANGE(IDC_COMBO_CODE, &CESApiDllDlgDlg::OnSelchangeComboCode)
	ON_BN_CLICKED(IDC_BUTTON10, &CESApiDllDlgDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CESApiDllDlgDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDOK2, &CESApiDllDlgDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDC_BUTTON12, &CESApiDllDlgDlg::OnBnClickedButton12)

	ON_MESSAGE(WM_ESM_TEST, OnTestMsg)
	ON_BN_CLICKED(IDC_BUTTON13, &CESApiDllDlgDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BTN_NEARSERIES, &CESApiDllDlgDlg::OnBnClickedBtnNearseries)
	ON_BN_CLICKED(IDC_BTN_5801, &CESApiDllDlgDlg::OnBnClickedBtn5801)
	ON_BN_CLICKED(IDC_BTN_5802, &CESApiDllDlgDlg::OnBnClickedBtn5802)
	ON_BN_CLICKED(IDC_BTN_5634, &CESApiDllDlgDlg::OnBnClickedBtn5634)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CESApiDllDlgDlg::OnClickedButtonAuto)
END_MESSAGE_MAP()


// CESApiDllDlgDlg �޽��� ó����

BOOL CESApiDllDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CRect rc;
	int   cx=0;

	m_List1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List1.GetWindowRect(&rc);
	cx=(rc.Width()-::GetSystemMetrics(SM_CXVSCROLL)-::GetSystemMetrics(SM_CXBORDER)*4)/8;

	m_List1.InsertColumn(0,"����",		LVCFMT_CENTER,	cx + 18);
	m_List1.InsertColumn(1,"���¸�",	LVCFMT_LEFT,	cx + 15);
	m_List1.InsertColumn(2,"�ڵ�",		LVCFMT_CENTER,	cx - 15);
	m_List1.InsertColumn(3,"����",		LVCFMT_CENTER,	cx - 15);
	m_List1.InsertColumn(4,"�ֹ���",	LVCFMT_RIGHT,	cx);
	m_List1.InsertColumn(5,"��ü��",	LVCFMT_RIGHT,	cx);
	m_List1.InsertColumn(6,"ü��",		LVCFMT_RIGHT,	cx);
	m_List1.InsertColumn(7,"�ֹ�ȣ",	LVCFMT_RIGHT,	cx);

	m_List2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List2.GetWindowRect(&rc);
	cx=(rc.Width()-::GetSystemMetrics(SM_CXVSCROLL)-::GetSystemMetrics(SM_CXBORDER)*4)/8;

	m_List2.InsertColumn(0,"����",		LVCFMT_CENTER,	cx + 18);
	m_List2.InsertColumn(1,"���¸�",	LVCFMT_LEFT,	cx + 15);
	m_List2.InsertColumn(2,"�ڵ�",		LVCFMT_CENTER,	cx - 15);
	m_List2.InsertColumn(3,"����",		LVCFMT_CENTER,	cx - 15);
	m_List2.InsertColumn(4,"�ܰ�",		LVCFMT_RIGHT,	cx - 10);
	m_List2.InsertColumn(5,"����",		LVCFMT_RIGHT,	cx - 10);
	m_List2.InsertColumn(6,"ü�ᰡ",	LVCFMT_RIGHT,	cx);
	m_List2.InsertColumn(7,"�򰡼���",	LVCFMT_RIGHT,	cx + 20);

	m_ListChe.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListChe.GetWindowRect(&rc);
	cx=(rc.Width()-::GetSystemMetrics(SM_CXVSCROLL)-::GetSystemMetrics(SM_CXBORDER)*4)/5;

	m_ListChe.InsertColumn(0,"�ð�",	LVCFMT_CENTER,	cx);
	m_ListChe.InsertColumn(1,"ü�ᰡ",	LVCFMT_RIGHT,	cx);
	m_ListChe.InsertColumn(2,"���",	LVCFMT_RIGHT,	cx);
	m_ListChe.InsertColumn(3,"ü�ᷮ",	LVCFMT_RIGHT,	cx);
	m_ListChe.InsertColumn(4,"�ŷ���",	LVCFMT_RIGHT,	cx);

	m_listNearSeries.InsertColumn(0, _T("Full Code"), LVCFMT_CENTER);
	m_listNearSeries.InsertColumn(1, _T("Short Code"), LVCFMT_CENTER);

	m_listSurenet.InsertColumn( 0, _T(" 0"));
	m_listSurenet.InsertColumn( 1, _T(" 1"));
	m_listSurenet.InsertColumn( 2, _T(" 2"));
	m_listSurenet.InsertColumn( 3, _T(" 3"));
	m_listSurenet.InsertColumn( 4, _T(" 4"));
	m_listSurenet.InsertColumn( 5, _T(" 5"));
	m_listSurenet.InsertColumn( 6, _T(" 6"));
	m_listSurenet.InsertColumn( 7, _T(" 7"));
	m_listSurenet.InsertColumn( 8, _T(" 8"));
	m_listSurenet.InsertColumn( 9, _T(" 9"));
	m_listSurenet.InsertColumn(10, _T("10"));
	m_listSurenet.InsertColumn(11, _T("11"));
	m_listSurenet.InsertColumn(12, _T("12"));
	m_listSurenet.InsertColumn(13, _T("13"));

	// config file
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir( szDir, szModule, szConfig);
	m_sDir = szDir;
	m_sModule = szModule;
	m_sConfig = szConfig;

	// LOG FILE
	CUtil::GetConfig(m_sConfig.GetBuffer(),  "DIR", "LOG", szDir);
	m_log = new CLog();
	m_log->OpenLog(szDir, m_sModule.GetBuffer());

	// API ���
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DIR", "API", szDir);
	m_sDllPath = szDir;
	CUtil::LogMsg(m_log, TRUE, "API ����� ���� : %s", m_sDllPath.GetBuffer());


	// API ����
	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ID", szDir);
	m_szUserID = szDir;

	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "USER_PWD", szDir);
	m_szUserPass = szDir;

	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "CERT_PWD", szDir);
	m_szCertPass = szDir;

	//CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ACNT_PWD", szDir);
	//m_szHedgePass = szDir;
	
	// ���Ӽ���
	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "REAL_YN", szDir);
	m_cRealYN = szDir[0];

	// Hedge ����
	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE", "HEDGE_ACC", szDir);
	m_sHedgeAcc = szDir;

	// Hedge ���
	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE", "HEDGE_PWD", szDir);
	m_szHedgePass = szDir;
	PrintMsg(TRUE, TRUE, "��������:%s, �������:%s", m_sHedgeAcc.GetBuffer(), m_szHedgePass.GetBuffer());



	// 1�� ���� Ÿ�̸�
	SetTimer(ID_TIME_LOGON, 1000, NULL);


	UpdateData(FALSE);
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}




// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CESApiDllDlgDlg::OnPaint()
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
HCURSOR CESApiDllDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString CESApiDllDlgDlg::GetCurCode()
{
	int nFind;
	CString szTmp, szCode;

	m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), szTmp);
	nFind	= szTmp.Find('|');
	szCode	= szTmp.Left(nFind-1);

	szCode.TrimRight();

	return szCode;
}

CString CESApiDllDlgDlg::GetCurAccount()
{
	int nFind;
	CString szTmp, szAcct;
	m_cmbAcct.GetLBText(m_cmbAcct.GetCurSel(), szTmp);

	nFind	= szTmp.Find('|');
	szAcct	= szTmp.Left(nFind-1);
	szAcct.Remove('-');

	return szAcct;
}

CString CESApiDllDlgDlg::GetAccount(int nSel)
{
	int nFind;
	CString szTmp, szAcct;
	m_cmbAcct.GetLBText(nSel, szTmp);

	nFind	= szTmp.Find('|');
	szAcct	= szTmp.Left(nFind-1);
	szAcct.Remove('-');

	return szAcct;
}

CString CESApiDllDlgDlg::GetAccountName(LPSTR lpszAccount, int nLen, BOOL bFreeCap)
{
	CString szTmp;

	if (!bFreeCap)
	{
		for (int idx = 0; idx < m_aryAcct.GetSize(); idx++)
		{
			if (memcmp (lpszAccount, m_aryAcct[idx].AccNo, nLen) == 0) {
				szTmp.Format("%.*s", sizeof(m_aryAcct[idx].AccNm), m_aryAcct[idx].AccNm);
				szTmp.TrimRight();
				return szTmp;
			}
		}
	}
	else
	{
		for (int idx = 0; idx < m_aryFCAcct.GetSize(); idx++)
		{
			if (memcmp (lpszAccount, m_aryFCAcct[idx].AccNo, nLen) == 0) {
				szTmp.Format("%.*s", sizeof(m_aryFCAcct[idx].AccNm), m_aryFCAcct[idx].AccNm);
				szTmp.TrimRight();
				return szTmp;
			}
		}
	}	

	return "";
}

int CESApiDllDlgDlg::GetDecimalLen(LPSTR lpszCode, int nLen)
{
	CString			szTmp;
	ESCodeInfo_st	lpMst;
	int				nDecLen = 0;

	if (GetCodeInfo(&lpMst, lpszCode, nLen))
	{
		szTmp.Format("%.*s", sizeof(lpMst.DecimalInfo), lpMst.DecimalInfo);
		szTmp.TrimLeft(); szTmp.TrimRight();

		nDecLen = atoi(szTmp);
	}

	return nDecLen;
}

BOOL CESApiDllDlgDlg::GetCodeInfo(lpESCodeInfo_st lpCode, LPSTR lpszCode, int nLen)
{
	CString szCmpA, szCmpB, szCmpC;

	szCmpA.Format("%.*s", nLen, lpszCode);
	szCmpA.TrimRight();

	for (int idx = 0; idx < m_aryCode.GetSize(); idx++)
	{
		szCmpB.Format("%.*s", sizeof(m_aryCode[idx].FullCode), m_aryCode[idx].FullCode);
		szCmpB.TrimRight();

		szCmpC.Format("%.*s", sizeof(m_aryCode[idx].ShortCode), m_aryCode[idx].ShortCode);
		szCmpC.TrimRight();

		if (szCmpA.Compare(szCmpB) == 0 || szCmpA.Compare(szCmpC) == 0)
		{
			memcpy (lpCode, &m_aryCode[idx], SZ_ESCodeInfo);

			return TRUE;
		}
	}

	return FALSE;
}

// ������ ->double
double CESApiDllDlgDlg::MyAtof(char *ibuf, int len)
{
	char    tbuf[32];

	memset (tbuf, '\0', sizeof (tbuf));
	memcpy (tbuf, ibuf, len);
	return atof (tbuf);
}

//�ؿܼ��� ���ݺ�ȯ
void CESApiDllDlgDlg::GetConvertPrice(char chType, char *pPrice, int nLen, int nDigitLen, CString *szResult)
{
	char	szDn[64];
	CString szUp;
	int		ii, nCnt, nDn, nGap;
	char	szPrice[128];

	szResult->Empty();
	memset(szDn,		0x00, sizeof(szDn));
	memset(szPrice,		0x00, sizeof(szPrice));

	nCnt = nLen;

	if (chType == OUTSCALE_10) { //10����
		CString szFormat, szTmp;
		szFormat.Format("%%.0%df", nDigitLen);
		szResult->Format(szFormat, MyAtof(pPrice, nLen));
		InsertComma(*szResult, FALSE);
		return;
	}

	if (chType == OUTSCALE_4 || chType == OUTSCALE_8) {	// 4���� or 8����
		memcpy (szPrice, szDn, nLen);

		nDigitLen -= 2;
		sprintf (szPrice, "%.*f", nDigitLen, MyAtof(pPrice, nLen) * 100.);

		szUp.Format("%.0f", floor(MyAtof(szPrice, nLen)));
		InsertComma(szUp);

		//�Ҽ��� �Ʒ� �и�
		for (ii = 0; ii < nCnt; ii++) {
			if (*(szPrice + ii) == '.') {
				memcpy(szDn, szPrice + (ii + 1), min(nLen - (ii + 1), nDigitLen) );
				break;
			}
		}	
	}
	else {
		szUp.Format("%.0f", floor(MyAtof(pPrice, nLen)));
		InsertComma(szUp);

		//�Ҽ��� �Ʒ� �и�
		for (ii = 0; ii < nCnt; ii++) {
			if (*(pPrice + ii) == '.') {
				memcpy(szDn, pPrice + (ii + 1), min(nLen - (ii + 1), nDigitLen) );
				break;
			}
		}
	}

	switch (chType) {
	case OUTSCALE_4 : //4����
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_4 + 0.5);
		switch (nDn) {
		case 0 :	szResult->Format("%s'0", szUp);	break;
		case 1 :	szResult->Format("%s'2", szUp);	break;
		case 2 :	szResult->Format("%s'4", szUp);	break;
		case 3 :	szResult->Format("%s'6", szUp);	break;
		}
		break;
	case OUTSCALE_8 : //8����
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_8 + 0.5);
		szResult->Format("%s'%d", szUp, nDn);
		break;
	case OUTSCALE_32 : //32����
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		//nDn = atoi(szDn) / TICKVALUE_32;
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_32 + 0.5);
		if(nDn >= 32) {
			nGap = nDn / 32;
			nDn = nDn % 32;
			szResult->Format("%d'%02d",   atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn);
		}
		else szResult->Format("%s'%02d",   szUp, nDn);
		break;
	case OUTSCALE_64 : //64����
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		//nDn = atoi(szDn) / TICKVALUE_64;
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_64 + 0.5);
		if(nDn >= 64) {
			nGap = nDn / 64;
			nDn = nDn % 64;
			switch (nDn % 2) {
			case 0 :	szResult->Format("%d'%02d.0", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 2);	break;
			case 1 :	szResult->Format("%d'%02d.5", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 2);	break;
			}
		}
		else {
			switch (nDn % 2) {
			case 0 :	szResult->Format("%s'%02d.0", szUp, nDn / 2);	break;
			case 1 :	szResult->Format("%s'%02d.5", szUp, nDn / 2);	break;
			}
		}
		break;
	case OUTSCALE_128 : //128����
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		//nDn = atoi(szDn) / TICKVALUE_128;
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_128 + 0.5);
		if(nDn >= 128) {
			nGap = nDn / 128;
			nDn = nDn % 128;
			switch (nDn % 4) {
			case 0 :	szResult->Format("%d'%02d.00", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			case 1 :	szResult->Format("%d'%02d.25", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			case 2 :	szResult->Format("%d'%02d.50", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			case 3 :	szResult->Format("%d'%02d.75", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			}
		}
		else {
			switch (nDn % 4) {
			case 0 :	szResult->Format("%s'%02d.00", szUp, nDn / 4);	break;
			case 1 :	szResult->Format("%s'%02d.25", szUp, nDn / 4);	break;
			case 2 :	szResult->Format("%s'%02d.50", szUp, nDn / 4);	break;
			case 3 :	szResult->Format("%s'%02d.75", szUp, nDn / 4);	break;
			}
		}
		break;
	}
}

BOOL CESApiDllDlgDlg::GetConvertPrice(char chType, double dValue, int nDigitLen, CString *szResult)
{
	char	szDn[64], szOrg[64];
	CString szUp, szTmp, szFormat; 
	char	*pPrice = szOrg;
	int		ii, nCnt, nDn, nGap;
	BOOL	bRtn = FALSE;

	szResult->Empty();
	memset(szDn,	0x00, sizeof(szDn));
	memset(szOrg,	0x00, sizeof(szOrg));

	szFormat.Format("%%.0%df", nDigitLen);
	szTmp.Format(szFormat, dValue);
	nCnt = szTmp.GetLength();
	strcpy(szOrg, szTmp);

	if (chType == OUTSCALE_10) { //10����
		szResult->Format("%s", szOrg);
		InsertComma(*szResult, FALSE);
		return TRUE;
	}

	if (chType == OUTSCALE_4 || chType == OUTSCALE_8) {	// 4���� or 8����

		nDigitLen -= 2;
		sprintf (pPrice, "%.*f", nDigitLen, dValue * 100.);
		dValue = atof(pPrice);
	}

	szUp.Format("%.0f", floor(dValue));
	InsertComma(szUp);

	//�Ҽ��� �Ʒ� �и�
	for (ii = 0; ii < nCnt; ii++) {
		if (*(pPrice + ii) == '.') {
			switch (chType) {
			case OUTSCALE_4		: memcpy(szDn, pPrice + (ii + 1), min(nCnt - (ii + 1), 2) ); break;
			case OUTSCALE_8		: memcpy(szDn, pPrice + (ii + 1), min(nCnt - (ii + 1), 3) ); break;
			case OUTSCALE_32	: memcpy(szDn, pPrice + (ii + 1), min(nCnt - (ii + 1), 5) ); break;
			case OUTSCALE_64	: memcpy(szDn, pPrice + (ii + 1), min(nCnt - (ii + 1), 6) ); break;
			case OUTSCALE_128	: memcpy(szDn, pPrice + (ii + 1), min(nCnt - (ii + 1), 7) ); break;
			}
			break;
		}
	}

	switch (chType) {
	case OUTSCALE_4 : //4����
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_4 + 0.5);
		bRtn = TRUE;
		switch (nDn) {
		case 0 :	szResult->Format("%s'0", szUp);	break;
		case 1 :	szResult->Format("%s'2", szUp);	break;
		case 2 :	szResult->Format("%s'4", szUp);	break;
		case 3 :	szResult->Format("%s'6", szUp);	break;
		}
		break;
	case OUTSCALE_8 : //8����
		bRtn = TRUE;
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_8 + 0.5);
		szResult->Format("%s'%d", szUp, nDn);
		break;
	case OUTSCALE_32 : //32����
		strncat(szDn, "00000000", 5 - strlen(szDn));
		if (atoi(szDn) % TICKVALUE_32)	bRtn = FALSE;
		else							bRtn = TRUE;
		//nDn = atoi(szDn) / TICKVALUE_32;
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_32 + 0.5);
		if(nDn >= 32) {
			nGap = nDn / 32;
			nDn = nDn % 32;
			szResult->Format("%d'%02d",   atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn);
		}
		else szResult->Format("%s'%02d",   szUp, nDn);
		break;
	case OUTSCALE_64 : //64����
		strncat(szDn, "00000000", 6 - strlen(szDn));
		if (atoi(szDn) % TICKVALUE_64)	bRtn = FALSE;
		else							bRtn = TRUE;
		//nDn = atoi(szDn) / TICKVALUE_64;
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_64 + 0.5);
		if(nDn >= 64) {
			nGap = nDn / 64;
			nDn = nDn % 64;
			switch (nDn % 2) {
			case 0 :	szResult->Format("%d'%02d.0", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 2);	break;
			case 1 :	szResult->Format("%d'%02d.5", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 2);	break;
			}
		}
		else {
			switch (nDn % 2) {
			case 0 :	szResult->Format("%s'%02d.0", szUp, nDn / 2);	break;
			case 1 :	szResult->Format("%s'%02d.5", szUp, nDn / 2);	break;
			}
		}
		break;
	case OUTSCALE_128 : //128����
		strncat(szDn, "00000000", 7 - strlen(szDn));
		if (atoi(szDn) % TICKVALUE_128)	bRtn = FALSE;
		else							bRtn = TRUE;
		//nDn = atoi(szDn) / TICKVALUE_128;
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_128 + 0.5);
		if(nDn >= 128) {
			nGap = nDn / 128;
			nDn = nDn % 128;
			switch (nDn % 4) {
			case 0 :	szResult->Format("%d'%02d.00", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			case 1 :	szResult->Format("%d'%02d.25", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			case 2 :	szResult->Format("%d'%02d.50", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			case 3 :	szResult->Format("%d'%02d.75", atoi((LPSTR)(LPCTSTR)szUp)+nGap, nDn / 4);	break;
			}
		}
		else {
			switch (nDn % 4) {
			case 0 :	szResult->Format("%s'%02d.00", szUp, nDn / 4);	break;
			case 1 :	szResult->Format("%s'%02d.25", szUp, nDn / 4);	break;
			case 2 :	szResult->Format("%s'%02d.50", szUp, nDn / 4);	break;
			case 3 :	szResult->Format("%s'%02d.75", szUp, nDn / 4);	break;
			}
		}
		break;
	}

	return bRtn;
}

//���� Data�� Comma ǥ��
void CESApiDllDlgDlg::InsertComma(LPCTSTR lpszData, LPSTR lpszOUT, int nLen)
{
	NUMBERFMT nFmt = { 2, 1, 3, ".", ",", 1 };
	CString	szData;
	szData	= lpszData;
	int nPos = szData.Find('.');
	if (nPos < 0)
		nFmt.NumDigits	= 0;
	else
		nFmt.NumDigits	= szData.GetLength() - nPos - 1;

	memset(lpszOUT, 0x00, nLen);
	::GetNumberFormat(NULL, NULL, lpszData, &nFmt, lpszOUT, nLen);
}

void CESApiDllDlgDlg::InsertComma(CString &szData, BOOL bNonZero /* = FALSE */)
{
	if (atof(szData) == 0.0) {
		if (bNonZero)
			szData = "";
		else
			szData = "0";
		return;
	}

	TCHAR szOUT[20];
	InsertComma(szData, szOUT, sizeof(szOUT));

	szData.Format("%s", szOUT);
}

void CESApiDllDlgDlg::InsertComma(LPSTR lpszData, BOOL bNonZero /* = FALSE */)
{
	char	szOrg[128];

	strcpy(szOrg, lpszData);

	if (atof(lpszData) == 0.0) {
		if (bNonZero)
			*lpszData = 0;
		else
			strcpy (lpszData, "0");
		return;
	}

	TCHAR szOUT[20];
	InsertComma(lpszData, szOUT, sizeof(szOUT));

	sprintf (lpszData, "%s", szOUT);
}

void CESApiDllDlgDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_sDllPath.IsEmpty()) {
		AfxMessageBox("ESApi.dll ������ ��� ������ �ʿ��մϴ�.");
		return;
	}

	// �ε��� dll�� ��ü���
	BOOL bRet = m_Api.LoadDll(m_sDllPath, "ESApi.dll");
	if (!bRet) {
		PrintMsg(FALSE, TRUE, m_sDllPath.GetBuffer());
		return;
	}

	// FreeCap, SignalMaker �ƴҰ��  OnlyFreeCap, OnlySignalMaker �Լ� ������� ����
	// FreeCap ����� ���
	//m_Api.OnlyFreeCap(TRUE);

	// SignalMaker ����� ���
	//m_Api.OnlySignalMaker(TRUE);	

	// server connect
	if( m_cRealYN=='Y')
		m_Api.ConnectServer(m_hWnd, m_szUserID, m_szUserPass, m_szCertPass, SERVER_REAL);
	else
		m_Api.ConnectServer(m_hWnd, m_szUserID, m_szUserPass, m_szCertPass, SERVER_DEMO);
}

void CESApiDllDlgDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_Api.DisConnectServer(m_hWnd);
}



void CESApiDllDlgDlg::OnBnClickedButton3()
{
	// ���� �ڵ�������Ʈ(AUTO_0931, AUTO_0932, AUTO_0933 ���̵�� ����)

	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '3';

	if (!m_szAutoCodeKey.IsEmpty()) {
		memcpy (lpAuto->AutoKey, m_szAutoCodeKey, m_szAutoCodeKey.GetLength());
		m_Api.SetAutoUpdate(FALSE, FALSE, szBuff);
		GetDlgItem(IDC_BUTTON3)->SetWindowText("�ü�������Ʈ �̵���");

		m_szAutoCodeKey.Empty();

		return;
	}

	m_szAutoCodeKey = GetCurCode();

	CString szCode = m_Api.GetFullCode(m_szAutoCodeKey);
	m_szAutoCodeKey = szCode;

	memcpy (lpAuto->AutoKey, m_szAutoCodeKey, m_szAutoCodeKey.GetLength());

	if (m_Api.SetAutoUpdate(TRUE, FALSE, szBuff)) {
		//GetDlgItem(IDC_BUTTON3)->SetWindowText("�ü�������Ʈ ������");
		PrintMsg(TRUE, TRUE, "%s �ü� ��û ����", m_szAutoCodeKey.GetBuffer());
	}
	else {
		//GetDlgItem(IDC_BUTTON3)->SetWindowText("�ü�������Ʈ �̵���");
		PrintMsg(TRUE, TRUE, "%s �ü� ��û ����", m_szAutoCodeKey.GetBuffer());
		m_szAutoCodeKey.Empty();
	}
}


void CESApiDllDlgDlg::OnBnClickedButtonExit()
{
	if (m_Api.IsServerConnect(m_hWnd))
		OnBnClickedCancel();

	CDialogEx::OnOK();
}

// �ű� �ֹ�
void CESApiDllDlgDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	memcpy (lpTrIn->Account, szAcct, szAcct.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), szAcct, m_szHedgePass);

	// �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)
	lpTrIn->Order_Type[0] = '1';

	// �����ڵ�
	memcpy(lpTrIn->ShortCode, szCode, szCode.GetLength());

	// �ŵ��ż����� (1.�ż� 2.�ŵ�)
	lpTrIn->BuySell_Type[0] = '1';

	// ��������	(1.������ 2.���尡)
	lpTrIn->Price_Type[0] = '1';

	// ü������ (���尡�ϰ��(3) �������ϰ��(1))
	lpTrIn->Trace_Type[0] = '1';

	// �ֹ�����
	memcpy (lpTrIn->Order_Price, m_szOrdP, m_szOrdP.GetLength());

	// �ֹ�����
	memcpy (lpTrIn->Order_Volume, m_szOrdV, m_szOrdV.GetLength());

	// ����ֹ�����
	m_Api.GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

	// �ֹ��������� (1.�Ϲ��ֹ� 2.STOP�ֹ�)
	lpTrIn->Stop_Type[0] = '1';

	// STOP�ֹ����� (STOP�ֹ��̸� �Է� �ƴϸ� 0 ����)
	lpTrIn->Stop_Price;

	// �ֹ����۱��� (�׻� 1��)
	lpTrIn->Oper_Type[0] = '1';

	m_Api.SendTrData(ESID_5601, szBuff, SZ_Es5601I);	
}

// ���� �ֹ�
void CESApiDllDlgDlg::OnBnClickedButton4()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	memcpy (lpTrIn->Account, szAcct, szAcct.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), szAcct, m_szHedgePass);

	// �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)
	lpTrIn->Order_Type[0] = '2';

	// �����ڵ�
	memcpy(lpTrIn->ShortCode, szCode, szCode.GetLength());

	// �ŵ��ż����� (1.�ż� 2.�ŵ�)
	lpTrIn->BuySell_Type[0] = '1';

	// ��������	(1.������ 2.���尡)
	lpTrIn->Price_Type[0] = '1';

	// ü������ (���尡�ϰ��(3) �������ϰ��(1))
	lpTrIn->Trace_Type[0] = '1';

	// �ֹ�����
	memcpy (lpTrIn->Order_Price, m_szOrdP, m_szOrdP.GetLength());

	// �ֹ�����
	memcpy (lpTrIn->Order_Volume, m_szOrdV, m_szOrdV.GetLength());

	// ���ֹ���ȣ (����/��ҽø�)
	memcpy (lpTrIn->Order_Org_No, m_szOrgNo, m_szOrgNo.GetLength());

	// ����ֹ�����
	m_Api.GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

	// �ֹ��������� (1.�Ϲ��ֹ� 2.STOP�ֹ�)
	lpTrIn->Stop_Type[0] = '1';

	// STOP�ֹ����� (STOP�ֹ��̸� �Է� �ƴϸ� 0 ����)
	lpTrIn->Stop_Price;

	// �ֹ����۱��� (�׻� 1��)
	lpTrIn->Oper_Type[0] = '2';

	m_Api.SendTrData(ESID_5602, szBuff, SZ_Es5601I);
}

// ��� �ֹ�
void CESApiDllDlgDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	memcpy (lpTrIn->Account, szAcct, szAcct.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), szAcct, m_szHedgePass);

	// �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)
	lpTrIn->Order_Type[0] = '3';

	// �����ڵ�
	memcpy(lpTrIn->ShortCode, szCode, szCode.GetLength());

	// �ŵ��ż����� (1.�ż� 2.�ŵ�)
	lpTrIn->BuySell_Type[0] = '1';

	// ��������	(1.������ 2.���尡)
	lpTrIn->Price_Type[0] = '1';

	// ü������ (���尡�ϰ��(3) �������ϰ��(1))
	lpTrIn->Trace_Type[0] = '1';

	// �ֹ�����
	memcpy (lpTrIn->Order_Price, m_szOrdP, m_szOrdP.GetLength());

	// �ֹ�����
	memcpy (lpTrIn->Order_Volume, m_szOrdV, m_szOrdV.GetLength());

	// ���ֹ���ȣ (����/��ҽø�)
	memcpy (lpTrIn->Order_Org_No, m_szOrgNo, m_szOrgNo.GetLength());

	// ����ֹ�����
	m_Api.GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

	// �ֹ��������� (1.�Ϲ��ֹ� 2.STOP�ֹ�)
	lpTrIn->Stop_Type[0] = '1';

	// STOP�ֹ����� (STOP�ֹ��̸� �Է� �ƴϸ� 0 ����)
	lpTrIn->Stop_Price;

	// �ֹ����۱��� (�׻� 1��)
	lpTrIn->Oper_Type[0] = '3';

	m_Api.SendTrData(ESID_5603, szBuff, SZ_Es5601I);
}



// �����ֹ� �ֹ�
void CESApiDllDlgDlg::SendHedgeOrder(lpAt0985O_st p)
{
	UpdateData(TRUE);

	char temp[1024];
	sprintf(temp, "%.*s", sizeof(p->Account), p->Account);
	int iRslt;

	// �ڱ��ڽſ� ���� ������ ���� �ʴ´�.
	if( m_sHedgeAcc.Compare(temp)==0 )
		return;

	// ü���� �ƴϸ� ���� �ʴ´�.
	if( p->ReplyType[0]!='2')
		return;

	char			szBuff[4096];
	lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	sprintf(temp, "%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, temp, strlen(temp));

	// ���¹�ȣ
	memcpy (lpTrIn->Account, m_sHedgeAcc, m_sHedgeAcc.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), m_sHedgeAcc, m_szHedgePass);

	// �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)
	lpTrIn->Order_Type[0] = '1';

	// �����ڵ�
	sprintf(temp, "%.*s", sizeof(p->FullCode), p->FullCode);
	CString sCode = m_Api.GetShortCode(temp);
	memcpy(lpTrIn->ShortCode, sCode, sCode.GetLength());

	// �ŵ��ż����� (1.�ż� 2.�ŵ�). �ݴ�� �Ѵ�.
	if( p->Side[0]=='1' )	lpTrIn->BuySell_Type[0] = '2';
	if( p->Side[0]=='2' )	lpTrIn->BuySell_Type[0] = '1';

	// ��������	(1.������ 2.���尡). ������ ���尡
	lpTrIn->Price_Type[0] = '2';

	// ü������ (���尡�ϰ��(3) �������ϰ��(1))
	lpTrIn->Trace_Type[0] = '3';
	// �ֹ�����
	sprintf(temp, "%.*s", sizeof(p->ExecQty), p->ExecQty );
	int nQty = atoi(temp);
	sprintf(temp, "%0*d", sizeof(lpTrIn->Order_Volume), nQty);
	memcpy (lpTrIn->Order_Volume, temp, sizeof(lpTrIn->Order_Volume));

	// �ֹ�����
	//memcpy (lpTrIn->Order_Price, m_szOrdP, m_szOrdP.GetLength());


	// ����ֹ�����
	m_Api.GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

	// �ֹ��������� (1.�Ϲ��ֹ� 2.STOP�ֹ�)
	lpTrIn->Stop_Type[0] = '1';

	// STOP�ֹ����� (STOP�ֹ��̸� �Է� �ƴϸ� 0 ����)
	//lpTrIn->Stop_Price;

	// �ֹ����۱��� (�׻� 1��)
	lpTrIn->Oper_Type[0] = '1';

/*
	PrintMsg(TRUE, TRUE, "[���ü��](ACC:%.*s)(����:%.*s)(����:%.*s)(����:%.*s)", 
			sizeof(p->Account), p->Account, sizeof(p->FullCode), p->FullCode, sizeof(p->Side), p->Side, sizeof(p->Qty), p->Qty);
*/

	iRslt = m_Api.SendTrData(ESID_5601, szBuff, SZ_Es5601I);

	PrintMsg(TRUE, FALSE, "[����ü��:%d](ACC:%.*s)(����:%.*s)(����:%.*s)(����:%.*s)",
		iRslt,
		sizeof(lpTrIn->Account), lpTrIn->Account, 
		sizeof(lpTrIn->ShortCode), lpTrIn->ShortCode,
		sizeof(lpTrIn->BuySell_Type), lpTrIn->BuySell_Type, sizeof(lpTrIn->Order_Volume), lpTrIn->Order_Volume);

	//AfxMessageBox(szBuff);
}


// ���� ��ȸ
void CESApiDllDlgDlg::OnBnClickedButton6()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode = GetCurCode();
	lpEs5501I_st	lpTrIn = (lpEs5501I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	lpTrIn->Head.PCKey[0] = '5';

	m_szCurPrice = m_szCurDebi = m_szNowVol = m_szVolume = "";

	szTmp = m_Api.GetFullCode(szCode);
	memcpy (lpTrIn->FullCode, szTmp, szTmp.GetLength());

	szTmp = m_Api.GetCodeIndex(szCode);
	memcpy (lpTrIn->Index, szTmp, szTmp.GetLength());

	m_Api.SendTrData(ESID_5501, szBuff, SZ_Es5501I);
	m_Api.SendTrData(ESID_5502, szBuff, SZ_Es5502I);
	m_Api.SendTrData(ESID_5503, szBuff, SZ_Es5503I);

	UpdateData(FALSE);
}

void CESApiDllDlgDlg::OnBnClickedButton9()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode = GetCurCode();
	lpEs5511I_st	lpTrIn = (lpEs5511I_st)szBuff;

	m_ListChe.DeleteAllItems();

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	szTmp = m_Api.GetFullCode(szCode);
	memcpy (lpTrIn->FullCode, szTmp, szTmp.GetLength());

	szTmp = m_Api.GetCodeIndex(szCode);
	memcpy (lpTrIn->Index, szTmp, szTmp.GetLength());

	m_Api.SendTrData(ESID_5511, szBuff, SZ_Es5511I);
}

// ��Ʈ
void CESApiDllDlgDlg::OnBnClickedButton12()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode = GetCurCode();
	lpEs5522I_st	lpTrIn = (lpEs5522I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	szTmp = m_Api.GetFullCode(szCode);
	memcpy (lpTrIn->FullCode, szTmp, szTmp.GetLength());

	szTmp = m_Api.GetCodeIndex(szCode);
	memcpy (lpTrIn->Index, szTmp, szTmp.GetLength());

	// �ؿ� ������
	lpTrIn->JongUp[0]	= '5';

	// 1:�� 2:�� 3:�� 4:tick�� 5:�к�
	lpTrIn->DataGb[0]	= '4';

	// ����, �Ϻ��� �׸���� ���� ���Ѵ�: '0'���� ����
	lpTrIn->DiviGb[0]	= '0';

	// ��������
	memcpy (lpTrIn->InxDay, "20160225", 8);

	// ���ڰ���
	szTmp.Format("%04d", 120);
	memcpy (lpTrIn->DayCnt, szTmp, szTmp.GetLength());

	// tick, �п��� ������ ����
	szTmp.Format("%03d", 200);
	memcpy (lpTrIn->Summary, szTmp, szTmp.GetLength());

	// ���ἱ������ Y/N(�Ϻ�)
	lpTrIn->Linked[0] = 'N';

	// 1.������ 2.���� (��/ƽ��)
	lpTrIn->JunBonGb[0] = '1';

	m_Api.SendTrData(ESID_5522, szBuff, SZ_Es5522I);
}

// ü����ȸ
void CESApiDllDlgDlg::OnBnClickedButton7()
{
	UpdateData(TRUE);

	m_List1.DeleteAllItems();

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5611I_st	lpTrIn = (lpEs5611I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	//memcpy (lpTrIn->Account, szAcct, szAcct.GetLength());
	memcpy(lpTrIn->Account, m_sHedgeAcc.GetBuffer(), m_sHedgeAcc.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), m_sHedgeAcc, m_szHedgePass);

	// ü�ᱸ�� (0:��ü 1:ü��2:��ü��)
	lpTrIn->Trd_gb[0] = '0';

	// �ֹ�����
	memcpy (lpTrIn->Base_dt, "20170407", strlen("20170407"));

	// ��ȸ���� (1:���� 2:����)
	lpTrIn->Gubn[0] = '2';

	m_Api.SendTrData(ESID_5611, szBuff, SZ_Es5611I);

	UpdateData(TRUE);
}

void CESApiDllDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case ID_TIME_LOGON:
		{
			KillTimer(ID_TIME_LOGON);
			OnBnClickedOk();
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}

// �ܰ���ȸ
void CESApiDllDlgDlg::OnBnClickedButton8()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5612I_st	lpTrIn = (lpEs5612I_st)szBuff;

	m_List2.DeleteAllItems();

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	memcpy (lpTrIn->Account, szAcct, szAcct.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), szAcct, m_szHedgePass);

	m_Api.SendTrData(ESID_5612, szBuff, SZ_Es5612I);
}

void CESApiDllDlgDlg::OnBnClickedButton11()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5614I_st	lpTrIn = (lpEs5614I_st)szBuff;

	m_List2.DeleteAllItems();

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	memcpy (lpTrIn->Account, szAcct, szAcct.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), szAcct, m_szHedgePass);

	// ü�ᱸ�� (0.��ü 1.ü�� 2.��ü�� 3.�������� 4.������� 5.�ֹ��ź� 6.ü��+��ü��)
	lpTrIn->Trd_gb[0]	= '0';

	// �ֹ�����
	memcpy (lpTrIn->Base_dt, "20151223", strlen("20151223"));

	// ��ȸ���� (1:���� 2:����)
	lpTrIn->Gubn[0] = '2';

	// �����ֹ���ȣ

	// ��û����
	memcpy (lpTrIn->Dtno, "00023", sizeof(lpTrIn->Dtno));

	m_Api.SendTrData(ESID_5614, szBuff, SZ_Es5614I);
}

// ��Ź�� ��ȸ
void CESApiDllDlgDlg::OnBnClickedButton10()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5615I_st	lpTrIn = (lpEs5615I_st)szBuff;

	m_List2.DeleteAllItems();

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	memcpy (lpTrIn->Account, szAcct, szAcct.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), szAcct, m_szHedgePass);

	// ��ȭ�ڵ�
	memcpy (lpTrIn->Crc_cd, "USD", sizeof(lpTrIn->Crc_cd));

	m_Api.SendTrData(ESID_5615, szBuff, SZ_Es5615I);
}

// �̺�Ʈ ó��
LRESULT CESApiDllDlgDlg::OnServerConnect(WPARAM wParam, LPARAM lParam)
{
	int nErrCode = (int)wParam;

	if(nErrCode == ERR_NONE)
	{
		//AfxMessageBox(_T("������ ����Ǿ����ϴ�."));
		if (m_cRealYN == 'Y') {
			PrintMsg(TRUE, TRUE, "REAL ������ ����Ǿ����ϴ�.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("REAL ����");
		}
		else {
			PrintMsg(TRUE, TRUE, "TEST ������ ����Ǿ����ϴ�.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("TEST ����");
		}

		// auto update
		//todo OnClickedButtonAuto();
	}
	else
	{
		LPCTSTR lpstrMsg = (LPCTSTR)lParam;
		if(lpstrMsg != NULL){
			//AfxMessageBox(lpstrMsg);
			PrintMsg(FALSE, TRUE, "������ ���� �����߽��ϴ�(%s)", lpstrMsg);
		}

		PostMessage(WM_ESM_TEST);
	}

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnServerDisConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("������ ������ ���� �Ǿ����ϴ�.");
	PrintMsg(TRUE, TRUE, "������ ������ ���� �Ǿ����ϴ�."); 

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString	szBuf, szTmp, szCode;

	// ERROR �޼���
	char szErrCode[5];
	lpESHeader_st pHeader = (lpESHeader_st)lParam;
	
	//PrintMsg(TRUE, FALSE, "RECV ORG_DATA[ERRCODE:%.4s][%.256s]", pHeader->ErrCode, (char*)lParam);

	sprintf(szErrCode, "%.*s", sizeof(pHeader->ErrCode),pHeader->ErrCode);
	if( atoi(szErrCode)!=0 )
	{
		lpESErr_st pErr = (lpESErr_st)lParam;
		PrintMsg(FALSE, TRUE, "ERROR!!! (CODE:%d) (MSG:%.50s)",  pErr->Head.ErrCode, sizeof(pErr->ErrMsg), pErr->ErrMsg);
	}


	switch(wParam){
	case ESID_5611:	// ��ü��. ü��, ��ü�� ��ȸ�� ������ ����
		{
			int				nDecLen;
			lpEs5611O_st	lpTrOut = (lpEs5611O_st)lParam;

			szTmp.Format("%.*s", sizeof(lpTrOut->Dtno), lpTrOut->Dtno);
			szTmp.TrimRight();

			int				nDCnt	= atoi(szTmp);
			int				nRCnt	= m_List1.GetItemCount();

			for (int ii = nRCnt; ii < nRCnt+nDCnt; ii++)
			{
				nDecLen = GetDecimalLen(lpTrOut->Sub[ii-nRCnt].ShortCode, sizeof(lpTrOut->Sub[ii-nRCnt].ShortCode));

				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Account), lpTrOut->Sub[ii-nRCnt].Account);
				szTmp.Insert(5, '-');	szTmp.Insert(3, '-');
				m_List1.InsertItem(ii, szTmp);

				szTmp.Format("%.*s", sizeof(lpTrOut->AcctNm), lpTrOut->AcctNm);
				szTmp.TrimLeft();	szTmp.TrimRight();
				m_List1.SetItemText(ii, 1, szTmp);

				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].ShortCode), lpTrOut->Sub[ii-nRCnt].ShortCode);
				szTmp.TrimRight();
				m_List1.SetItemText(ii, 2, szTmp);

				// �Ÿű����ڵ� (1.�ż� 2.�ŵ� 3.�ż����� 4.�ŵ����� 5.�ż���� 6.�ŵ���� ' '.��Ÿ)
				switch (lpTrOut->Sub[ii-nRCnt].Bysl_tp[0]) {
				case '1': case '3': case '5':	szTmp = "�ż�";	break;
				case '2': case '4': case '6':	szTmp = "�ŵ�";	break;
				}
				m_List1.SetItemText(ii, 3, szTmp);

				// �ֹ����� or ü������
				szBuf.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Ord_p), lpTrOut->Sub[ii-nRCnt].Ord_p);
				szBuf.TrimRight();
				szTmp.Format("%.*f", nDecLen, atof(szBuf));
				m_List1.SetItemText(ii, 4, szTmp);

				// ��ü�����
				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Mcg_q), lpTrOut->Sub[ii-nRCnt].Mcg_q);
				szTmp.TrimLeft(0x30);	szTmp.TrimRight();
				m_List1.SetItemText(ii, 5, szTmp);

				// ü�����
				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Trd_q), lpTrOut->Sub[ii-nRCnt].Trd_q);
				szTmp.TrimLeft(0x30);	szTmp.TrimRight();
				m_List1.SetItemText(ii, 6, szTmp);

				// �ֹ��ֹ�ȣ
				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Ord_No), lpTrOut->Sub[ii-nRCnt].Ord_No);
				szTmp.TrimLeft(0x30);	szTmp.TrimRight();
				m_List1.SetItemText(ii, 7, szTmp);
			} /* End of for(nRCnt+nDCnt) */
		}break;
	case ESID_5612:	//���ܰ�. �ܰ� ��ȸ�� ������ ����
		{
			int				nDecLen;
			lpEs5612O_st	lpTrOut = (lpEs5612O_st)lParam;

			szTmp.Format("%.*s", sizeof(lpTrOut->Dtno), lpTrOut->Dtno);
			szTmp.TrimRight();

			int				nDCnt	= atoi(szTmp);
			int				nRCnt	= m_List2.GetItemCount();

			for (int ii = nRCnt, idx = nRCnt; ii < nRCnt+nDCnt; ii++)
			{
				nDecLen = GetDecimalLen(lpTrOut->Sub[ii-nRCnt].FullCode, sizeof(lpTrOut->Sub[ii-nRCnt].FullCode));

				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].FullCode), lpTrOut->Sub[ii-nRCnt].FullCode);
				szTmp.TrimRight();
				szCode = m_Api.GetShortCode(szTmp);
				if (szCode.IsEmpty())
					continue;

				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Account), lpTrOut->Sub[ii-nRCnt].Account);
				szTmp.Insert(5, '-');	szTmp.Insert(3, '-');
				m_List2.InsertItem(idx, szTmp);

				szTmp.Format("%.*s", sizeof(lpTrOut->AcctNm), lpTrOut->AcctNm);
				szTmp.TrimLeft();	szTmp.TrimRight();
				m_List2.SetItemText(idx, 1, szTmp);

				m_List2.SetItemText(idx, 2, szCode);

				// �Ÿű��� (1.�ż� 2.�ŵ�)
				switch (lpTrOut->Sub[ii-nRCnt].Bysl_tp[0]) {
				case '1':	szTmp = "�ż�";	break;
				case '2':	szTmp = "�ŵ�";	break;
				}
				m_List2.SetItemText(idx, 3, szTmp);

				// �̰�������
				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Open_q), lpTrOut->Sub[ii-nRCnt].Open_q);
				szTmp.TrimLeft(0x30);	szTmp.TrimRight();
				m_List2.SetItemText(idx, 4, szTmp);

				// û�갡�ɼ���
				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Rsrb_q), lpTrOut->Sub[ii-nRCnt].Rsrb_q);
				szTmp.TrimLeft(0x30);	szTmp.TrimRight();
				m_List2.SetItemText(idx, 5, szTmp);

				// ��հ�
				szBuf.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Avgt_p), lpTrOut->Sub[ii-nRCnt].Avgt_p);
				szBuf.TrimRight();
				szTmp.Format("%.*f", nDecLen, atof(szBuf));
				m_List2.SetItemText(idx, 6, szTmp);

				// �򰡼���
				szTmp.Format("%.*s", sizeof(lpTrOut->Sub[ii-nRCnt].Open_pl)-1, (LPSTR)&lpTrOut->Sub[ii-nRCnt].Open_pl[1]);
				szTmp.TrimLeft(0x30);	szTmp.TrimRight();
				if (lpTrOut->Sub[ii-nRCnt].Open_pl[0] == '-')	szTmp.Insert(0, '-');
				m_List2.SetItemText(idx, 7, szTmp);

				idx++;
			} /* End of for(nRCnt+nDCnt) */
		}break;
	case ESID_5614: // ���º� �ֹ�ü����Ȳ(USD)
		{
			lpEs5614O_st	lpTrOut = (lpEs5614O_st)lParam;

			CString s;
			s.Format("[%.*s] [%.5s] [%c]", SZ_Es5614O, (LPSTR)lpTrOut, lpTrOut->Dtno, lpTrOut->Head.NextKind[0]);
			AfxMessageBox(s);
		}break;
	case ESID_5615: // ��Ź�ڻ�� ���ű�
		{
			CString			szTmp, szMsg;
			lpEs5615O_st	lpTrOut = (lpEs5615O_st)lParam;

			szTmp.Format("%.*s", sizeof(lpTrOut->Head.ErrCode), lpTrOut->Head.ErrCode);
			if (atoi(szTmp) > 0)
			{
				lpESErr_st	lpErr = (lpESErr_st)lParam;
				szMsg.Format("ESID [%04d] [%04d] %.*s", wParam, atoi(szTmp), sizeof(lpErr->ErrMsg), lpErr->ErrMsg);
				AfxMessageBox(szMsg);
			}
			else
			{
				CString s;
				s.Format("%.*s", SZ_Es5615O, (LPSTR)lpTrOut);
				AfxMessageBox(s);
			}
		}break;
	case ESID_5715: // ��Ź�ڻ�� ���ű�(��ȭ)
		{
			CString			szTmp, szMsg;
			lpEs5715O_st	lpTrOut = (lpEs5715O_st)lParam;

			szTmp.Format("%.*s", sizeof(lpTrOut->Head.ErrCode), lpTrOut->Head.ErrCode);
			if (atoi(szTmp) > 0)
			{
				lpESErr_st	lpErr = (lpESErr_st)lParam;
				szMsg.Format("ESID [%04d] [%04d] %.*s", wParam, atoi(szTmp), sizeof(lpErr->ErrMsg), lpErr->ErrMsg);
				AfxMessageBox(szMsg);
			}
			else
			{
				CString s;
				s.Format("%.*s", SZ_Es5715O, (LPSTR)lpTrOut);
				AfxMessageBox(s);
			}
		}break;
	case AUTO_0985: // // �ܰ�/ü�� �ǽð� 
		{
			lpAt0985O_st lpTrOut = (lpAt0985O_st)lParam;
			//TRACE ("0985 : [%.*s]\n", SZ_At0985O, (LPSTR)lParam);

			CString s;
			s.Format("<ü��ǽð�>[ACC:%.*s] [%.*s] [�ֹ���ȣ:%.*s] [ü���ȣ:%.*s]", 
				sizeof(lpTrOut->Account), lpTrOut->Account, sizeof(lpTrOut->FullCode), lpTrOut->FullCode,
				sizeof(lpTrOut->Ord_no), lpTrOut->Ord_no, sizeof(lpTrOut->Trd_no), lpTrOut->Trd_no);

			//AfxMessageBox(s);
			/*
			1.�ֹ����� 2.ü�� 3.����Ȯ�� 4.���Ȯ�� 
			5.�ű԰ź� 6.�����ź� 7.��Ұź� 0.��������	
			*/
			if (lpTrOut->ReplyType[0] == '2')
			{
				PrintMsg(TRUE, TRUE, s.GetBuffer());

				// ����ó��
				//todo SendHedgeOrder(lpTrOut);
			}
		}break;
	case ESID_5501: // // ���� Master
		{
			lpEs5501O_st	lpTrOut = (lpEs5501O_st)lParam;
			int				nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));

			// ����
			szTmp.Format("%.*s", sizeof(lpTrOut->PrevTotQty), lpTrOut->PrevTotQty);
			szTmp.TrimRight();

			m_szVolume.Format("%.*f", nDecLen, atof(szTmp));
			m_szVolume.TrimLeft(); m_szVolume.TrimRight(); 
		}break;
	case ESID_5502: // // ���� �ü�
		{
			lpEs5502O_st	lpTrOut = (lpEs5502O_st)lParam;
			int				nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));

			// ���簡
			szTmp.Format("%.*s", sizeof(lpTrOut->ClosePrice), lpTrOut->ClosePrice);
			szTmp.TrimRight();
			m_szCurPrice.Format("%.*f", nDecLen, atof(szTmp));
			m_szCurPrice.TrimLeft(); m_szCurPrice.TrimRight(); 

			// ���
			szTmp.Format("%.*s", sizeof(lpTrOut->CmpRate), lpTrOut->CmpRate);
			szTmp.TrimRight();
			m_szCurDebi.Format("%.2f", atof(szTmp));
			m_szCurDebi.TrimLeft(); m_szCurDebi.TrimRight(); 

			// �ŷ���
			szTmp.Format("%.*s", sizeof(lpTrOut->TotQty), lpTrOut->TotQty);
			szTmp.TrimRight();
			m_szNowVol.Format("%d", atoi(szTmp));
			m_szNowVol.TrimLeft(); m_szNowVol.TrimRight();
		}break;
	case ESID_5503:// ���� ȣ��
		{
			lpEs5503O_st lpTrOut = (lpEs5503O_st)lParam;

			int	nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));

			// �ż�ȣ��
			szTmp.Format("%.*s", sizeof(lpTrOut->Ary[0].BuyPrice), lpTrOut->Ary[0].BuyPrice);
			szTmp.TrimRight();
			m_szSHoka1.Format("%.*f", nDecLen, atof(szTmp));	m_szSHoka1.TrimLeft();	m_szSHoka1.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Ary[1].BuyPrice), lpTrOut->Ary[1].BuyPrice);
			szTmp.TrimRight();
			m_szSHoka2.Format("%.*f", nDecLen, atof(szTmp));	m_szSHoka2.TrimLeft();	m_szSHoka2.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Ary[2].BuyPrice), lpTrOut->Ary[2].BuyPrice);
			szTmp.TrimRight();
			m_szSHoka3.Format("%.*f", nDecLen, atof(szTmp));	m_szSHoka3.TrimLeft();	m_szSHoka3.TrimRight();

			// �ŵ�ȣ��
			szTmp.Format("%.*s", sizeof(lpTrOut->Ary[0].SellPrice), lpTrOut->Ary[0].SellPrice);
			szTmp.TrimRight();
			m_szDHoka1.Format("%.*f", nDecLen, atof(szTmp));	m_szDHoka1.TrimLeft();	m_szDHoka1.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Ary[1].SellPrice), lpTrOut->Ary[1].SellPrice);
			szTmp.TrimRight();
			m_szDHoka2.Format("%.*f", nDecLen, atof(szTmp));	m_szDHoka2.TrimLeft();	m_szDHoka2.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Ary[2].SellPrice), lpTrOut->Ary[2].SellPrice);
			szTmp.TrimRight();
			m_szDHoka3.Format("%.*f", nDecLen, atof(szTmp));	m_szDHoka3.TrimLeft();	m_szDHoka3.TrimRight();
		}break;
	case ESID_5511:// ���� ü�᳻��
		{
			lpEs5511O_st lpTrOut = (lpEs5511O_st)lParam;

			szTmp.Format("%.*s", sizeof(lpTrOut->DataCnt), lpTrOut->DataCnt);
			szTmp.TrimRight();

			int	nDCnt	= atoi(szTmp);
			int	nRCnt	= m_ListChe.GetItemCount();
			int	nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));
			char chJinbubGb	= lpTrOut->JinbubGb[0];

			for(int ii = nRCnt; ii < nRCnt+nDCnt; ii++)
			{
				// �ð�
				szTmp.Format("%.*s", sizeof(lpTrOut->Ary[ii-nRCnt].Time), lpTrOut->Ary[ii-nRCnt].Time);
				m_ListChe.InsertItem(ii, szTmp);

				// ü�ᰡ				
				GetConvertPrice(chJinbubGb, lpTrOut->Ary[ii-nRCnt].ClosePrice, sizeof(lpTrOut->Ary[ii-nRCnt].ClosePrice), nDecLen, &szTmp);
				//szBuf.Format("%.*s", sizeof(lpTrOut->Ary[ii-nRCnt].ClosePrice), lpTrOut->Ary[ii-nRCnt].ClosePrice);
				//szTmp.Format("%.*f", nDecLen, atof(szBuf));
				m_ListChe.SetItemText(ii, 1, szTmp);

				// ���
				szBuf.Format("%.*s", sizeof(lpTrOut->Ary[ii-nRCnt].CmpRate), lpTrOut->Ary[ii-nRCnt].CmpRate);
				szTmp.Format("%.2f", atof(szBuf));
				m_ListChe.SetItemText(ii, 2, szTmp);

				// ü�ᷮ
				szBuf.Format("%.*s", sizeof(lpTrOut->Ary[ii-nRCnt].ContQty), lpTrOut->Ary[ii-nRCnt].ContQty);
				szTmp.Format("%d", atoi(szBuf));
				m_ListChe.SetItemText(ii, 3, szTmp);

				// �ŷ���
				szBuf.Format("%.*s", sizeof(lpTrOut->Ary[ii-nRCnt].TotQty), lpTrOut->Ary[ii-nRCnt].TotQty);
				szTmp.Format("%d", atoi(szBuf));
				m_ListChe.SetItemText(ii, 4, szTmp);
			}
		}break;
	case ESID_5522:// ���� CHART DATA
		{
			lpEs5522O_st lpTrOut = (lpEs5522O_st)lParam;
		}break;
	case ESID_5560:// �ֱٿ��� ��� ��ȸ
		{
			lpEs5560O_st lpEs5560O = (lpEs5560O_st)lParam;

			CString strDataCount(lpEs5560O->DataCnt, _countof(lpEs5560O->DataCnt));
			int nDataCount = _ttoi(strDataCount);

			for(int nLoop(0); nLoop < nDataCount; nLoop++)
			{
				lpEs5560OSub_st lpEs5560OSub = (lpEs5560OSub_st)(lParam + SZ_Es5560O + (SZ_Es5560OSub * nLoop));
				CString strValue;

				strValue.Format(_T("%.*s"), _countof(lpEs5560OSub->StockCd), lpEs5560OSub->StockCd);
				m_listNearSeries.InsertItem(nLoop, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5560OSub->MarketCd), lpEs5560OSub->MarketCd);
				m_listNearSeries.SetItemText(nLoop, 1, strValue);
			} /* End of for(nDataCount) */
		}break;
	case ESID_5801:// Sure-net ���ڴ�ȸ ������ȸ
		{
			lpEs5801O_st lpEs5801O = (lpEs5801O_st)lParam;

			CString strDataCount(lpEs5801O->dtno, _countof(lpEs5801O->dtno));
			int nDataCount = _ttoi(strDataCount);

			for(int nLoop(0); nLoop < nDataCount; nLoop++)
			{
				lpEs5801OSub_st lpEs5801OSub = (lpEs5801OSub_st)(lParam + sizeof(Es5801O_st) + (sizeof(Es5801OSub_st) * nLoop));
				CString strValue;

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->leader_nm), lpEs5801OSub->leader_nm);
				m_listSurenet.InsertItem(nLoop, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->cntr_yn), lpEs5801OSub->cntr_yn);
				m_listSurenet.SetItemText(nLoop, 1, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->mrkt_cd), lpEs5801OSub->mrkt_cd);
				m_listSurenet.SetItemText(nLoop, 2, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->exp_yyyymm), lpEs5801OSub->exp_yyyymm);
				m_listSurenet.SetItemText(nLoop, 3, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->prce), lpEs5801OSub->prce);
				m_listSurenet.SetItemText(nLoop, 4, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->bysl_tp), lpEs5801OSub->bysl_tp);
				m_listSurenet.SetItemText(nLoop, 5, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->qty), lpEs5801OSub->qty);
				m_listSurenet.SetItemText(nLoop, 6, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->win_point), lpEs5801OSub->win_point);
				m_listSurenet.SetItemText(nLoop, 7, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->part_nm), lpEs5801OSub->part_nm);
				m_listSurenet.SetItemText(nLoop, 8, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->day_modi_pl), lpEs5801OSub->day_modi_pl);
				m_listSurenet.SetItemText(nLoop, 9, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->week_modi_pl), lpEs5801OSub->week_modi_pl);
				m_listSurenet.SetItemText(nLoop, 10, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->day_pl), lpEs5801OSub->day_pl);
				m_listSurenet.SetItemText(nLoop, 11, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->week_pl), lpEs5801OSub->week_pl);
				m_listSurenet.SetItemText(nLoop, 12, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5801OSub->accum_pl), lpEs5801OSub->accum_pl);
				m_listSurenet.SetItemText(nLoop, 13, strValue);
			} /* End of for(nDataCount) */
		}break;
	case ESID_5802:// Sure-net ���ڴ�ȸ �Ÿų���
		{
			lpEs5802O_st lpEs5802O = (lpEs5802O_st)lParam;

			CString strDataCount(lpEs5802O->dtno, _countof(lpEs5802O->dtno));
			int nDataCount = _ttoi(strDataCount);

			for(int nLoop(0); nLoop < nDataCount; nLoop++)
			{
				lpEs5802OSub_st lpEs5802OSub = (lpEs5802OSub_st)(lParam + sizeof(Es5802O_st) + (sizeof(Es5802OSub_st) * nLoop));
				CString strValue;

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->date), lpEs5802OSub->date);
				m_listSurenet.InsertItem(nLoop, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->time), lpEs5802OSub->time);
				m_listSurenet.SetItemText(nLoop, 1, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->leader_nm), lpEs5802OSub->leader_nm);
				m_listSurenet.SetItemText(nLoop, 2, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->mrkt_cd), lpEs5802OSub->mrkt_cd);
				m_listSurenet.SetItemText(nLoop, 3, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->exp_yyyymm), lpEs5802OSub->exp_yyyymm);
				m_listSurenet.SetItemText(nLoop, 4, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->bysl_tp), lpEs5802OSub->bysl_tp);
				m_listSurenet.SetItemText(nLoop, 5, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->qty), lpEs5802OSub->qty);
				m_listSurenet.SetItemText(nLoop, 6, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->prce), lpEs5802OSub->prce);
				m_listSurenet.SetItemText(nLoop, 7, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->last_prce), lpEs5802OSub->last_prce);
				m_listSurenet.SetItemText(nLoop, 8, strValue);

				strValue.Format(_T("%.*s"), _countof(lpEs5802OSub->open_pl), lpEs5802OSub->open_pl);
				m_listSurenet.SetItemText(nLoop, 9, strValue);
			} /* End of for(nDataCount) */
		}break;
	case ESID_5634:// �Ⱓ�������ڻ꺯������
		{
			lpEs5634O_st	lpTrOut = (lpEs5634O_st)lParam;

			szTmp.Format("%.*s", sizeof(lpTrOut->Dtno), lpTrOut->Dtno);
			szTmp.TrimRight();
			int nDCnt = atoi(szTmp);

			CString s;
			s.Format("%.*s", SZ_Es5634O, (LPSTR)lpTrOut);
			AfxMessageBox(s);

			for (int ii = 0; ii < nDCnt; ii++)
			{
			}
		}break;
	case AUTO_0930://shjang 20160406 >>  �ڵ� Update : ��ް��� <<
		{
			lpAt0930O_st	lpTrOut = (lpAt0930O_st)lParam;
			char*	pData = (char *)lParam;
			CString szTime, szNotice;
			int		nCnt;
			szTmp.Format("%.*s", sizeof(lpTrOut->TotSize), lpTrOut->TotSize);
			szTmp.TrimRight();
			nCnt	= atoi(szTmp);
			szNotice.Format("%.*s", nCnt , pData + SZ_At0930O);
			TRACE("%s\n", szNotice);
		}break;
	case AUTO_0931:// ���� Master �ǽð�
		{
			lpAt0931O_st	lpTrOut = (lpAt0931O_st)lParam;
			int				nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));

			// ���簡
			szTmp.Format("%.*s", sizeof(lpTrOut->ClosePrice), lpTrOut->ClosePrice);
			szTmp.TrimRight();

			m_szCurPrice.Format("%.*f", nDecLen, atof(szTmp));

			// ���
			szTmp.Format("%.*s", sizeof(lpTrOut->CmpRate), lpTrOut->CmpRate);
			szTmp.TrimRight();

			m_szCurDebi.Format("%.2f", atof(szTmp));

			// �ŷ���
			szTmp.Format("%.*s", sizeof(lpTrOut->TotQty), lpTrOut->TotQty);
			szTmp.TrimRight();

			m_szNowVol.Format("%d", atoi(szTmp));
		}break;
	case AUTO_0932:// ���� ȣ�� �ǽð�
		{
			lpAt0932O_st	lpTrOut = (lpAt0932O_st)lParam;
			int				nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));

			// �ż�ȣ��
			szTmp.Format("%.*s", sizeof(lpTrOut->Arr[0].BuyPrice), lpTrOut->Arr[0].BuyPrice);
			szTmp.TrimRight();
			m_szSHoka1.Format("%.*f", nDecLen, atof(szTmp));	m_szSHoka1.TrimLeft();	m_szSHoka1.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Arr[1].BuyPrice), lpTrOut->Arr[1].BuyPrice);
			szTmp.TrimRight();
			m_szSHoka2.Format("%.*f", nDecLen, atof(szTmp));	m_szSHoka2.TrimLeft();	m_szSHoka2.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Arr[2].BuyPrice), lpTrOut->Arr[2].BuyPrice);
			szTmp.TrimRight();
			m_szSHoka3.Format("%.*f", nDecLen, atof(szTmp));	m_szSHoka3.TrimLeft();	m_szSHoka3.TrimRight();

			// �ŵ�ȣ��
			szTmp.Format("%.*s", sizeof(lpTrOut->Arr[0].SellPrice), lpTrOut->Arr[0].SellPrice);
			szTmp.TrimRight();
			m_szDHoka1.Format("%.*f", nDecLen, atof(szTmp));	m_szDHoka1.TrimLeft();	m_szDHoka1.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Arr[1].SellPrice), lpTrOut->Arr[1].SellPrice);
			szTmp.TrimRight();
			m_szDHoka2.Format("%.*f", nDecLen, atof(szTmp));	m_szDHoka2.TrimLeft();	m_szDHoka2.TrimRight();

			szTmp.Format("%.*s", sizeof(lpTrOut->Arr[2].SellPrice), lpTrOut->Arr[2].SellPrice);
			szTmp.TrimRight();
			m_szDHoka3.Format("%.*f", nDecLen, atof(szTmp));	m_szDHoka3.TrimLeft();	m_szDHoka3.TrimRight();

			CString szTemp1, szTemp2,szTemp3,szTemp4;
			szTemp1.Format("%.*s", sizeof(lpTrOut->TotSellQty), lpTrOut->TotSellQty);szTemp1.TrimRight();
			szTemp1.Format("%.*f", nDecLen, atof(szTemp1));	szTemp1.TrimLeft();	szTemp1.TrimRight();

			szTemp2.Format("%.*s", sizeof(lpTrOut->TotBuyQty), lpTrOut->TotBuyQty);szTemp2.TrimRight();
			szTemp2.Format("%.*f", nDecLen, atof(szTemp2));	szTemp2.TrimLeft();	szTemp2.TrimRight();

			szTemp3.Format("%.*s", sizeof(lpTrOut->TotSellNo), lpTrOut->TotSellNo);szTemp3.TrimRight();
			szTemp3.Format("%.*f", nDecLen, atof(szTemp3));	szTemp3.TrimLeft();	szTemp3.TrimRight();

			szTemp4.Format("%.*s", sizeof(lpTrOut->TotBuyNo), lpTrOut->TotBuyNo);szTemp4.TrimRight();
			szTemp4.Format("%.*f", nDecLen, atof(szTemp4));	szTemp4.TrimLeft();	szTemp4.TrimRight();

			TRACE("%s %s %s %s\n", szTemp3, szTemp1, szTemp2, szTemp4);
		}break;
	case AUTO_0933:// ���� �ü� �ǽð�
		{
			lpAt0933O_st	lpTrOut = (lpAt0933O_st)lParam;
			int				nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));

			// �ð�
			szTmp.Format("%.*s", sizeof(lpTrOut->Time), lpTrOut->Time);
			m_ListChe.InsertItem(0, szTmp);

			// ü�ᰡ
			szBuf.Format("%.*s", sizeof(lpTrOut->ClosePrice), lpTrOut->ClosePrice);
			szTmp.Format("%.*f", nDecLen, atof(szBuf));
			m_ListChe.SetItemText(0, 1, szTmp);

			// ���
			szBuf.Format("%.*s", sizeof(lpTrOut->CmpRate), lpTrOut->CmpRate);
			szTmp.Format("%.2f", atof(szBuf));
			m_ListChe.SetItemText(0, 2, szTmp);

			// ü�ᷮ
			szBuf.Format("%.*s", sizeof(lpTrOut->ContQty), lpTrOut->ContQty);
			szTmp.Format("%d", atoi(szBuf));
			m_ListChe.SetItemText(0, 3, szTmp);

			// �ŷ���
			szBuf.Format("%.*s", sizeof(lpTrOut->TotQty), lpTrOut->TotQty);
			szTmp.Format("%d", atoi(szBuf));
			m_ListChe.SetItemText(0, 4, szTmp);
		}break;
	default:
		break;
	} /* End of switch(wParam) */

	UpdateData(FALSE);

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnAcctList(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	for (int ii = 0; ii < m_cmbAcct.GetCount(); ii++)
		m_cmbAcct.DeleteString(ii);

	ESAccntInfo_st		lpMst;
	CString				szAdd, szAcct, szName;
	lpESAccntInfo_st	lpAcct = (lpESAccntInfo_st)lParam;

	for ( unsigned int ii = 0; ii < wParam; ii++, lpAcct++)
	{
		szAcct.Format("%.*s", sizeof(lpAcct->AccNo), lpAcct->AccNo);
		szAcct.Insert(5, '-');	szAcct.Insert(3, '-');

		szName.Format("%.*s", sizeof(lpAcct->AccNm), lpAcct->AccNm);
		szName.TrimRight();

		szAdd.Format("%s | %s", szAcct, szName);

		m_cmbAcct.AddString(szAdd);

		// add memory
		memset (&lpMst, 0x20, SZ_ESAccntInfo);
		memcpy (&lpMst, lpAcct, SZ_ESAccntInfo);

		m_aryAcct.Add(lpMst);
	}

	if (m_cmbAcct.GetCount() > 0)
		m_cmbAcct.SetCurSel(0);

	PrintMsg( TRUE, TRUE, "���¹�ȣ����Ʈ ������");

	return 0L;
}

LRESULT CESApiDllDlgDlg::OnFCAcctList(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	for (int ii = 0; ii < m_cmbAcct.GetCount(); ii++)
		m_cmbAcct.DeleteString(ii);

	ESAccntInfoFC_st	lpMst;
	CString				szAdd, szAcct, szName, szGrade, szSystem, szCommon, szMicro;
	lpESAccntInfoFC_st	lpAcct = (lpESAccntInfoFC_st)lParam;

	for (int ii = 0; ii < wParam; ii++, lpAcct++)
	{
		szAcct.Format("%.*s", sizeof(lpAcct->AccNo), lpAcct->AccNo);
		szAcct.Insert(5, '-');	szAcct.Insert(3, '-');

		szName.Format("%.*s", sizeof(lpAcct->AccNm), lpAcct->AccNm);
		szName.TrimRight();

		szAdd.Format("%s | %s", szAcct, szName);

		///////////////////////////////////////////////////////////////////////////////
		// ����ĸ ���Ǽ����� ���� �߰�
		// N: None(��޾���), C: Challenger, P: Pro, B: Best, S: SuperStar
		szGrade.Format("%.*s", sizeof(lpAcct->leader_grade), lpAcct->leader_grade);
		// Y: �ý���, N: �Ϲ�
		szSystem.Format("%.*s", sizeof(lpAcct->system_yn), lpAcct->system_yn);
		// �Ϲ�, �̴ϻ�ǰ ������
		szCommon.Format("%.*s", sizeof(lpAcct->common_nego_fee), lpAcct->common_nego_fee);
		// ����ũ�λ�ǰ ������
		szMicro.Format("%.*s", sizeof(lpAcct->micro_nego_fee), lpAcct->micro_nego_fee);
		///////////////////////////////////////////////////////////////////////////////

		m_cmbAcct.AddString(szAdd);

		// add memory
		memset (&lpMst, 0x20, SZ_ESAccntInfoFC);
		memcpy (&lpMst, lpAcct, SZ_ESAccntInfoFC);

		m_aryFCAcct.Add(lpMst);
	}

	if (m_cmbAcct.GetCount() > 0)
		m_cmbAcct.SetCurSel(0);

	return 0L;
}

LRESULT CESApiDllDlgDlg::OnCodeList(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	for (int ii = 0; ii < m_cmbCode.GetCount(); ii++)
		m_cmbCode.DeleteString(ii);

	ESCodeInfo_st	lpMst;
	CString			szAdd, szCode, szName;
	lpESCodeInfo_st	lpCode = (lpESCodeInfo_st)lParam;

	for (int ii = 0; ii < wParam; ii++, lpCode++)
	{
		szCode.Format("%.*s", sizeof(lpCode->ShortCode), lpCode->ShortCode);
		szCode.TrimRight();

		szName.Format("%.*s", sizeof(lpCode->CodeName), lpCode->CodeName);
		szName.TrimRight();

		szAdd.Format("%s | %s", szCode, szName);

		m_cmbCode.AddString(szAdd);

		// add memory
		memset (&lpMst, 0x20, SZ_ESCodeInfo);
		memcpy (&lpMst, lpCode, SZ_ESCodeInfo);
		m_aryCode.Add(lpMst);
	}

	if (m_cmbCode.GetCount() > 0)
		m_cmbCode.SetCurSel(0);

	return 0L;
}


void CESApiDllDlgDlg::OnSelchangeComboAcct()
{
	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '2';

	if (!m_szAutoAcctKey.IsEmpty()) {
		memcpy (lpAuto->AutoKey, m_szAutoAcctKey, m_szAutoAcctKey.GetLength());
		m_Api.SetAutoUpdate(FALSE, TRUE, szBuff);
	}

	m_szAutoAcctKey.Empty();

	GetDlgItem(IDC_BUTTON2)->SetWindowText("���¾�����Ʈ �̵���");
}


void CESApiDllDlgDlg::OnSelchangeComboCode()
{
	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '3';

	if (!m_szAutoCodeKey.IsEmpty()) {
		memcpy (lpAuto->AutoKey, m_szAutoCodeKey, m_szAutoCodeKey.GetLength());
		m_Api.SetAutoUpdate(FALSE, FALSE, szBuff);
	}

	m_szAutoCodeKey.Empty();

	CString szCode = GetCurCode();

	GetDlgItem(IDC_BUTTON3)->SetWindowText("�ü�������Ʈ �̵���");
}

void CESApiDllDlgDlg::OnBnClickedOk2()
{
	UpdateData(TRUE);

	CFolderPickerDialog	fd;

	if (fd.DoModal() == IDOK) {

		m_sDllPath = fd.GetPathName();
		PrintMsg(TRUE, TRUE, "���� �α��� �õ�...");
	}

	UpdateData(FALSE);
}

LRESULT CESApiDllDlgDlg::OnTestMsg(WPARAM wParam, LPARAM lParam)
{
	if(m_Api.IsServerConnect(m_hWnd))
		m_Api.DisConnectServer(m_hWnd);

	return 0L;
}

void CESApiDllDlgDlg::OnBnClickedButton13()
{
	UpdateData(TRUE);

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5715I_st	lpTrIn = (lpEs5715I_st)szBuff;

	m_List2.DeleteAllItems();

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ���¹�ȣ
	memcpy (lpTrIn->Accnt, szAcct, szAcct.GetLength());

	// ��й�ȣ
	m_Api.GetEncodePassword(lpTrIn->Acct_pwd, sizeof(lpTrIn->Acct_pwd), szAcct, m_szHedgePass);

	// ��ȭ�ڵ�
	memcpy (lpTrIn->Crc_cd, "USD", sizeof(lpTrIn->Crc_cd));

	m_Api.SendTrData(ESID_5715, szBuff, SZ_Es5715I);
}

void CESApiDllDlgDlg::OnBnClickedBtnNearseries()
{
	m_listNearSeries.DeleteAllItems();

	Es5560I_st st_Es5506I;
	memset(&st_Es5506I, 0x20, SZ_Es5560I);

	CString strHWnd;
	strHWnd.Format("%010.0f", (double)(unsigned long)GetSafeHwnd());
	memcpy(st_Es5506I.Head.WinId, strHWnd, strHWnd.GetLength());

	char szSendBuff[4096];
	memset(szSendBuff, 0x20, 4096);
	memcpy(szSendBuff, &st_Es5506I, SZ_Es5560I);
	m_Api.SendTrData(ESID_5560, szSendBuff, SZ_Es5560I);
}


void CESApiDllDlgDlg::OnBnClickedBtn5801()
{
	m_listSurenet.DeleteAllItems();

	Es5801I_st st_Es5801I;
	memset(&st_Es5801I, 0x20, sizeof(Es5801I_st));

	CString strHWnd;
	strHWnd.Format("%010.0f", (double)(unsigned long)GetSafeHwnd());
	memcpy(st_Es5801I.Head.WinId, strHWnd, strHWnd.GetLength());

	// ��������   	
	CString szSvrDate = CTime::GetCurrentTime().Format(_T("%Y%m%d"));
	memcpy(st_Es5801I.base_dt, szSvrDate, szSvrDate.GetLength());

	// �����ID
	memcpy(st_Es5801I.user_id, m_szUserID, m_szUserID.GetLength());	

	// �� �̸� : A~Z, 0x20:��ü
	//memcpy(st_Es5801I.part_nm, szTmp, szTmp.GetLength());	

	// ��ǰ : CUR:��ȭ IDX:���� ENG:������ BND:ä�� MTL:�ݼ� AGR:��깰  0x20:��ü
	//memcpy(st_Es5801I.commodity_seq,	szTmp,	szTmp.GetLength());	

	char szSendBuff[4096];
	memset(szSendBuff, 0x20, 4096);
	memcpy(szSendBuff, &st_Es5801I, sizeof(Es5801I_st));
	m_Api.SendTrData(ESID_5801, szSendBuff, sizeof(Es5801I_st));
}


void CESApiDllDlgDlg::OnBnClickedBtn5802()
{
	m_listSurenet.DeleteAllItems();

	Es5802I_st st_Es5802I;
	memset(&st_Es5802I, 0x20, sizeof(Es5802I_st));

	CString strHWnd;
	strHWnd.Format("%010.0f", (double)(unsigned long)GetSafeHwnd());
	memcpy(st_Es5802I.Head.WinId, strHWnd, strHWnd.GetLength());

	// ���� ����
	CString szSvrDate = CTime::GetCurrentTime().Format(_T("%Y%m%d"));
	memcpy(st_Es5802I.st_dt, szSvrDate, szSvrDate.GetLength());

	// �� ����
	memcpy(st_Es5802I.ed_dt, szSvrDate, szSvrDate.GetLength());

	char szSendBuff[4096];
	memset(szSendBuff, 0x20, 4096);
	memcpy(szSendBuff, &st_Es5802I, sizeof(Es5802I_st));
	m_Api.SendTrData(ESID_5802, szSendBuff, sizeof(Es5802I_st));
}

// ü����ȸ
void CESApiDllDlgDlg::OnBnClickedBtn5634()
{
	UpdateData(TRUE);
	
	CString			szTmp;
	char			szBuff[4096];
	CString			szCode	= GetCurCode();
	CString			szAcct	= GetCurAccount();
	lpEs5634I_st	lpTrIn = (lpEs5634I_st)szBuff;

	memset (szBuff, 0x00, 4096);
	memset (szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// ��ü���� ��ȸ�ô� ���¹�ȣ, ��й�ȣ �� ����� �ø�
	// �������� ��ȸ�ô� ���¹�ȣ, ��й�ȣ �� ä���� �ø�
	// ���¹�ȣ
	//memcpy (lpTrIn->Accnt, szAcct, szAcct.GetLength());

	// ��й�ȣ
	//m_Api.GetEncodePassword(lpTrIn->Acct_pwd, sizeof(lpTrIn->Acct_pwd), szAcct, m_szHedgePass);

	// ��ȭ�ڵ�(USD, KRW)
	szTmp = "USD";
	memcpy (lpTrIn->crc_cd, szTmp, szTmp.GetLength());

	// ��������
	memcpy (lpTrIn->FBase_dt, "20170101", strlen("20170101"));

	// ��������
	memcpy (lpTrIn->TBase_dt, "20170224", strlen("20170224"));
	
	// ��û����
	memcpy (lpTrIn->Dtno, "00010", sizeof(lpTrIn->Dtno));

	m_Api.SendTrData(ESID_5634, szBuff, SZ_Es5634I);

	UpdateData(TRUE);
}



void CESApiDllDlgDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if(m_log)	delete m_log;
	CDialogEx::OnClose();
}


void CESApiDllDlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}



VOID CESApiDllDlgDlg::PrintMsg(BOOL bSucc, BOOL bShow,char* pMsg, ...)
{
	char buff[DEF_LOG_LEN];
	va_list argptr;
	
	va_start(argptr, pMsg);
	vsprintf_s(buff, pMsg, argptr);
	va_end(argptr);
	CUtil::LogMsgEx(m_log, bSucc, buff, buff);
	
	if (bShow) {
		m_lstMsg.InsertString(0, buff);
	}
}


void CESApiDllDlgDlg::OnClickedButtonAuto()
{
	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset (szBuff, 0x00, sizeof(szBuff));
	//memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '2';

	// ���� �ڵ�������Ʈ(AUTO_0985 ���̵�� ����)
	/*if (!m_szAutoAcctKey.IsEmpty()) {
		memcpy (lpAuto->AutoKey, m_szAutoAcctKey, m_szAutoAcctKey.GetLength());
		m_Api.SetAutoUpdate(FALSE, TRUE, szBuff);
		GetDlgItem(IDC_BUTTON2)->SetWindowText("���¾�����Ʈ �̵���");

		return;
	}*/

#if 1
	for (int ii = 0; ii < m_cmbAcct.GetCount(); ++ii)
	{
		memset (lpAuto->AutoKey, 0x20, sizeof(lpAuto->AutoKey));
		m_szAutoAcctKey = GetAccount(ii);

		memcpy (lpAuto->AutoKey, m_szAutoAcctKey, m_szAutoAcctKey.GetLength());
		m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);	//At0985

		PrintMsg(TRUE, TRUE, "%s   ���� �ڵ�������Ʈ ����", m_szAutoAcctKey.GetBuffer());
	}
#else


	memcpy(lpAuto->AutoKey, "10150502981", 11);
	m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);	//At0985
	PrintMsg(TRUE, TRUE, "[10150502981] ���� �ڵ�������Ʈ ����");

	memcpy(lpAuto->AutoKey, "10150502846", 11);
	m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);	//At0985
	PrintMsg(TRUE, TRUE, "[10150502846] ���� �ڵ�������Ʈ ����");

	memcpy(lpAuto->AutoKey, "10150502866", 11);
	m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);	//At0985
	PrintMsg(TRUE, TRUE, "[10150502866] ���� �ڵ�������Ʈ ����");

	memcpy(lpAuto->AutoKey, "10150503258", 11);
	m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);	//At0985
	PrintMsg(TRUE, TRUE, "[10150503258] ���� �ڵ�������Ʈ ����");


	//memcpy (lpAuto->AutoKey, m_szAutoAcctKey, m_szAutoAcctKey.GetLength());
	//m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);

	//memset (lpAuto->AutoKey, 0x20, sizeof(lpAuto->AutoKey));
	//CString szAcct = "10151500001";
	//memcpy (lpAuto->AutoKey, szAcct, szAcct.GetLength());
	//m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);

	/*if (m_Api.SetAutoUpdate(TRUE, TRUE, szBuff)) {
		GetDlgItem(IDC_BUTTON2)->SetWindowText("���¾�����Ʈ ������");
	}
	else {
		GetDlgItem(IDC_BUTTON2)->SetWindowText("���¾�����Ʈ �̵���");
		m_szAutoAcctKey.Empty();
	}*/
#endif
}
