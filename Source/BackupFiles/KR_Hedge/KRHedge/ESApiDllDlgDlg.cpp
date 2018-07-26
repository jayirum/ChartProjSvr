
// ESApiDllDlgDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ESApiDllDlg.h"
#include "ESApiDllDlgDlg.h"
#include "afxdialogex.h"
#include "../Common/KRCommon.h"
#include "../../IRUM_UTIL/Util.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ESM_TEST	WM_USER + 0x9000

// CESApiDllDlgDlg ��ȭ ����

#define DEF_LOG_LEN 4096


#define ID_TIME_LOGON			101	// ���� �α��� �õ��ϴ� Ÿ�̸�



CESApiDllDlgDlg::CESApiDllDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CESApiDllDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szUserID = _T("");
	m_szUserPass = _T("");
	m_szCertPass = _T("");
	
	//m_szAutoAcctKey =
	//m_szAutoCodeKey = _T("");

	m_sDllPath = _T("");

	m_log = NULL;
	m_thrPool = NULL;
	m_runAdpter = NULL;
	m_pDBPool = NULL;
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
	DDX_Text(pDX, IDC_EDIT_USERID, m_szUserID);
	DDV_MaxChars(pDX, m_szUserID, 8);
	DDX_Text(pDX, IDC_EDIT_USERPASS, m_szUserPass);
	DDV_MaxChars(pDX, m_szUserPass, 8);
	DDX_Text(pDX, IDC_EDIT_CERTPASS, m_szCertPass);
	DDX_Text(pDX, IDC_EDIT1, m_sDllPath);
	DDX_Control(pDX, IDC_MSG, m_lstMsg);
	DDX_Control(pDX, IDC_STATIC_REAL, m_sRealYN);
}

BEGIN_MESSAGE_MAP(CESApiDllDlgDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDOK, &CESApiDllDlgDlg::OnBnClickedOk)

	ON_BN_CLICKED(IDCANCEL, &CESApiDllDlgDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CESApiDllDlgDlg::OnBnClickedButtonExit)

	ON_MESSAGE(WM_ESM_RECVDATA, OnRecvData)
	ON_MESSAGE(WM_ESM_ACCTLIST, OnAcctList)
	ON_MESSAGE(WM_ESM_FCACCTLIST, OnFCAcctList)
	ON_MESSAGE(WM_ESM_CODELIST, OnCodeList)
	ON_MESSAGE(WM_ESM_CONNECT,	OnServerConnect)
	ON_MESSAGE(WM_ESM_DISCONNECT, OnServerDisConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCT, &CESApiDllDlgDlg::OnSelchangeComboAcct)
	ON_CBN_SELCHANGE(IDC_COMBO_CODE, &CESApiDllDlgDlg::OnSelchangeComboCode)
	ON_BN_CLICKED(IDOK2, &CESApiDllDlgDlg::OnBnClickedOk2)

	ON_MESSAGE(WM_ESM_TEST, OnTestMsg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CESApiDllDlgDlg::OnClickedButtonAuto)


	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_COMMAND(WM_APP_EXIT, OnAppExit)
	ON_COMMAND(WM_DIALOG_SHOW, OnDialogShow)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON3, &CESApiDllDlgDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_HEDGE_RELOAD, &CESApiDllDlgDlg::OnBnClickedButtonHedgeReload)
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


	m_myTray.m_bHide = m_bHide = false;
	m_myTray.AddTrayIcon(GetSafeHwnd());

	InitializeCriticalSectionAndSpinCount(&m_csAPI, 2000);

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
	
	// Hedge �ֹ� ����
	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE", "HEDGE_YN", szDir);
	m_cHedgeYN = szDir[0];

	PrintMsg(TRUE, TRUE, "�����ֹ�����:%c", m_cHedgeYN);

	if (!DBOpen()) {
		AfxMessageBox("DB OPEN ����");
		return FALSE;
	}

	if(!LoadHedgeOrderAcnt()) {
		AfxMessageBox("�����ֹ����� ��ȸ ����");
		return FALSE;
	}

	// ������� ���� load
	if(!LoadHedgeTarget(TRUE/*RESET*/)) {
		AfxMessageBox("���������� ��ȸ ����");
		return FALSE;
	}

	// thread pool
	if( !InitThreadPool()) {
		AfxMessageBox("Thread Pool ����");
		return FALSE;
	}


	// ������ action ó���� ���� polling thread
	if (!InitNotedThread()) {
		AfxMessageBox("�뺸 ������ Init ����");
		return FALSE;
	}

	m_bContinue = TRUE;

	// 1�� ���� Ÿ�̸�
	SetTimer(ID_TIME_LOGON, 1000, NULL);


	UpdateData(FALSE);
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


// ������ action �� �����ϴ� q table polling
BOOL CESApiDllDlgDlg::InitNotedThread()
{
	m_notedThread = new Poco::RunnableAdapter<CESApiDllDlgDlg>(*this, &CESApiDllDlgDlg::NotedThread);
	//m_notedWrapper.start(*m_notedThread);
	PrintMsg(TRUE, TRUE, "������ �뺸�� ���� POLLING ������ ����");

	return TRUE;
}


void CESApiDllDlgDlg::CloseNotedThread()
{
	SAFE_DELETE(m_notedThread);
}


VOID CESApiDllDlgDlg::DBClose()
{
	SAFE_DELETE(m_pDBPool);
}

BOOL CESApiDllDlgDlg::CheckDBPool()
{
	return (m_pDBPool->Available() > 0);
}

BOOL CESApiDllDlgDlg::DBOpen()
{
	char ip[32], id[32], pwd[32], cnt[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_ID", id);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_POOL_CNT", cnt);

	char temp[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MIN_CNT", temp);
	m_unMinThread = atoi(temp);

	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MAX_CNT", temp);
	m_unMaxThread = atoi(temp);
	

	BOOL bUseODBC = FALSE;
	m_pDBPool = new CDBPool(bUseODBC, ip, id, pwd, "KRHedge");
	if (!m_pDBPool->Init(atoi(cnt)))
	{
		PrintMsg(FALSE, TRUE, "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
		PrintMsg(FALSE, TRUE, "(IP:%s)(ID:%s)(PWD:%s)(DB:KRHedgae)", ip, id, pwd);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}


	return TRUE;
}

BOOL CESApiDllDlgDlg::InitThreadPool()
{
	char temp[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MIN_CNT", temp);
	m_unMinThread = atoi(temp);

	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MAX_CNT", temp);
	m_unMaxThread = atoi(temp);

	// thread pool
	m_runAdpter = new Poco::RunnableAdapter<CESApiDllDlgDlg>(*this, &CESApiDllDlgDlg::HedgeOrderThread);
	m_thrPool = new Poco::ThreadPool(m_unMinThread, m_unMaxThread);
	for (int i = 0; i<m_unMaxThread; i++)
		m_thrPool->start(*m_runAdpter);

	return TRUE;
}


VOID CESApiDllDlgDlg::CloseThreadPool()
{
	if (m_thrPool) {
		m_thrPool->joinAll();
		SAFE_DELETE(m_runAdpter);
		SAFE_DELETE(m_thrPool);
	}
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
				memcpy(szDn, szPrice + (ii + 1), MIN(nLen - (ii + 1), nDigitLen) );
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
				memcpy(szDn, pPrice + (ii + 1), MIN(nLen - (ii + 1), nDigitLen) );
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
			case OUTSCALE_4		: memcpy(szDn, pPrice + (ii + 1), MIN(nCnt - (ii + 1), 2) ); break;
			case OUTSCALE_8		: memcpy(szDn, pPrice + (ii + 1), MIN(nCnt - (ii + 1), 3) ); break;
			case OUTSCALE_32	: memcpy(szDn, pPrice + (ii + 1), MIN(nCnt - (ii + 1), 5) ); break;
			case OUTSCALE_64	: memcpy(szDn, pPrice + (ii + 1), MIN(nCnt - (ii + 1), 6) ); break;
			case OUTSCALE_128	: memcpy(szDn, pPrice + (ii + 1), MIN(nCnt - (ii + 1), 7) ); break;
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





void CESApiDllDlgDlg::OnBnClickedButtonExit()
{
	if (m_Api.IsServerConnect(m_hWnd))
		OnBnClickedCancel();

	CDialogEx::OnOK();
}


// �����ֹ� �ֹ�
void CESApiDllDlgDlg::HedgeOrderThread()
{
	//UpdateData(TRUE);
	char temp[1024];
	int iRslt;

	PrintMsg(TRUE, TRUE, "%s Start...", Poco::Thread::current()->name().c_str());
	while (TRUE)
	{
		Poco::AutoPtr<QRealApiData> pTNf = dynamic_cast<QRealApiData*>(m_queue.waitDequeueNotification());
		if (!pTNf) {
			PrintMsg(TRUE, TRUE, "HedgeOrderThread-break");
			break;
		}
		//PrintMsg(TRUE, TRUE, "HedgeOrderThread:%d", m_queue.size());
		lpAt0985O_st p = (lpAt0985O_st)pTNf->get();		

		char			szBuff[4096];
		lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

		memset(szBuff, 0x00, 4096);
		memset(szBuff, 0x20, 4096);

		sprintf(temp, "%010.0f", (double)(unsigned long)m_hWnd);
		memcpy(lpTrIn->Head.WinId, temp, strlen(temp));

		// ���¹�ȣ
		memcpy(lpTrIn->Account, m_sHedgeAcc, m_sHedgeAcc.GetLength());

		// ��й�ȣ
		m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), m_sHedgeAcc, m_szHedgePass);

		// �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)
		lpTrIn->Order_Type[0] = '1';

		// �����ڵ�
		sprintf(temp, "%.*s", sizeof(p->FullCode), p->FullCode);
		CString sCode = m_Api.GetShortCode(temp);
		memcpy(lpTrIn->ShortCode, sCode, sCode.GetLength());

		// �ŵ��ż����� (1.�ż� 2.�ŵ�). �ݴ�� �Ѵ�.
		if (p->Side[0] == '1')	lpTrIn->BuySell_Type[0] = '2';
		if (p->Side[0] == '2')	lpTrIn->BuySell_Type[0] = '1';

		// ��������	(1.������ 2.���尡). ������ ���尡
		lpTrIn->Price_Type[0] = '2';

		// ü������ (���尡�ϰ��(3) �������ϰ��(1))
		lpTrIn->Trace_Type[0] = '3';
		// �ֹ�����
		sprintf(temp, "%.*s", sizeof(p->ExecQty), p->ExecQty);
		int nQty = atoi(temp);
		sprintf(temp, "%0*d", sizeof(lpTrIn->Order_Volume), nQty);
		memcpy(lpTrIn->Order_Volume, temp, sizeof(lpTrIn->Order_Volume));

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
		APILOCK();
		iRslt = m_Api.SendTrData(ESID_5601, szBuff, SZ_Es5601I);
		APIUNLOCK();
		PrintMsg(TRUE, TRUE, "%s[����ü������:%d](ACC:%.*s)(����:%.*s)(����:%.*s)(����:%.*s)",
			Poco::Thread::current()->name().c_str(),iRslt,
			sizeof(lpTrIn->Account), lpTrIn->Account,
			sizeof(lpTrIn->ShortCode), lpTrIn->ShortCode,
			sizeof(lpTrIn->BuySell_Type), lpTrIn->BuySell_Type, sizeof(lpTrIn->Order_Volume), lpTrIn->Order_Volume);

		// DB ����
		SaveTradeInfo(sCode,(char*)p, szBuff);
	}// end of while
}


BOOL CESApiDllDlgDlg::SaveTradeInfo(CString& sShortCode, char* pTarget, char* pHedge)
{
	// Get DB Session
	if (!CheckDBPool())
	{
		PrintMsg(FALSE, TRUE, "DB OPEN ����");
		return FALSE;
	}
	lpAt0985O_st t = (lpAt0985O_st)pTarget;
	lpEs5601I_st h = (lpEs5601I_st)pHedge;

	std::string symbol;
	int trade_qty, t_trd_no;
	std::string t_acnt, t_side, t_ord_tp, t_dt, t_tm;
	double t_prc;
	std::string h_acnt, h_side, h_dt, h_tm;

	symbol = std::string(sShortCode.GetBuffer());
	trade_qty = S2N(t->ExecQty, sizeof(t->ExecQty));

	t_acnt = SUBSTR(t->Account, 0, sizeof(t->Account));
	t_trd_no = S2N(t->Trd_no, sizeof(t->Trd_no));
	t_side = SUBSTR(t->Side, 0, sizeof(t->Side));
	t_ord_tp = SUBSTR(t->Modality, 0, sizeof(t->Modality));
	t_dt = SUBSTR(t->Trd_date, 0, sizeof(t->Trd_date));
	t_tm = SUBSTR(t->TradeTime, 0, sizeof(t->TradeTime));
	t_prc = S2D(t->ExecPrice, sizeof(t->ExecPrice));


	h_acnt = SUBSTR(h->Account, 0, sizeof(h->Account));
	h_side = SUBSTR(h->BuySell_Type, 0, sizeof(h->BuySell_Type));

	SYSTEMTIME st;
	char temp[32];
	GetLocalTime(&st);
	
	sprintf(temp, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	h_dt = std::string(temp);

	sprintf(temp, "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	h_tm = std::string(temp);
	CString sQ;
	
	CDBHandler db(m_pDBPool->Get());

	sQ.Format("INSERT INTO TRADE_INFO(SYMBOL, TRADE_QTY, "
		" TARGET_ACNT_NO, TARGET_TRADE_NO, TARGET_TRADE_SIDE, TARGET_ORD_TP, TARGET_PRICE, TARGET_TRADE_DT, TARGET_TRADE_TM, "
		" HEDGE_ACNT_NO, HEDGE_TRADE_SIDE, HEDGE_TRADE_DT, HEDGE_TRADE_TM )"
		" VALUES('%s',%d,"
		" '%s',%d,'%s','%s',%f,'%s','%s',"
		" '%s','%s','%s','%s')"
		, symbol.c_str(), trade_qty
		, t_acnt.c_str(), t_trd_no, t_side.c_str(), t_ord_tp.c_str(), t_prc, t_dt.c_str(), t_tm.c_str()
		, h_acnt.c_str(), h_side.c_str(), h_dt.c_str(), h_tm.c_str());
		
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		PrintMsg(FALSE, TRUE, "ü������ ���� ����<%s>(MSG:%s)", sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}
	PrintMsg(TRUE, TRUE, "%s[DB����](ACC:%.*s)(����:%.*s)(����:%.*s)(����:%.*s)",
		Poco::Thread::current()->name().c_str(), 
		sizeof(h->Account), h->Account,
		sizeof(h->ShortCode), h->ShortCode,
		sizeof(h->BuySell_Type), h->BuySell_Type, sizeof(h->Order_Volume), h->Order_Volume);
	return TRUE;
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
		OnClickedButtonAuto();
		m_notedWrapper.start(*m_notedThread);
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


	switch(wParam)
	{
	
	case AUTO_0985: // // �ܰ�/ü�� �ǽð� 
	{
		lpAt0985O_st lpTrOut = (lpAt0985O_st)lParam;
		//TRACE ("0985 : [%.*s]\n", SZ_At0985O, (LPSTR)lParam);

		CString s;
		/*
		s.Format("<ü��ǽð�>ESID [%04d][��������:%c] [ACC:%.*s] [%.*s] [�ֹ���ȣ:%.*s] [ü���ȣ:%.*s]",
			wParam, lpTrOut->ReplyType[0],
			sizeof(lpTrOut->Account), lpTrOut->Account, sizeof(lpTrOut->FullCode), lpTrOut->FullCode,
			sizeof(lpTrOut->Ord_no), lpTrOut->Ord_no, sizeof(lpTrOut->Trd_no), lpTrOut->Trd_no);
			*/

		
		
		/*
		1.�ֹ����� 2.ü�� 3.����Ȯ�� 4.���Ȯ��
		5.�ű԰ź� 6.�����ź� 7.��Ұź� 0.��������
		*/
		if (lpTrOut->ReplyType[0] == '2') 
		{

			// �ڱ��ڽſ� ���� ������ ���� �ʴ´�.
			CString sAcnt;
			sAcnt.Format( "%.*s", sizeof(lpTrOut->Account), lpTrOut->Account);
			if (m_sHedgeAcc.Compare(sAcnt) == 0)
				break;

			// ��������� �ƴϸ� skip
			if (!m_hedgeTarget.IsTarget(sAcnt))
				break;

			s.Format("<ü��ǽð�>[ACC:%.*s] [%.*s] [�ֹ���ȣ:%.*s] [ü���ȣ:%.*s]",
				sizeof(lpTrOut->Account), lpTrOut->Account, sizeof(lpTrOut->FullCode), lpTrOut->FullCode,
				sizeof(lpTrOut->Ord_no), lpTrOut->Ord_no, sizeof(lpTrOut->Trd_no), lpTrOut->Trd_no);

			PrintMsg(TRUE, FALSE, s.GetBuffer());

			if (m_cHedgeYN != 'Y')
				break;
			// ����ó���� ���� notification queue �� ����. ==> ������Ǯ���� ó���Ѵ�.
			m_queue.enqueueNotification(new QRealApiData((void*)lParam, sizeof(At0985O_st)));
			//PrintMsg(TRUE, TRUE, "ENQ:%d", m_queue.size());
		}

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
	CString				szAdd, szAcct, szName, szPwd;
	lpESAccntInfo_st	lpAcct = (lpESAccntInfo_st)lParam;

	

	for ( unsigned int ii = 0; ii < wParam; ii++, lpAcct++)
	{
		szAcct.Format("%.*s", sizeof(lpAcct->AccNo), lpAcct->AccNo);
		//szAcct.Insert(5, '-');	szAcct.Insert(3, '-');

		szName.Format("%.*s", sizeof(lpAcct->AccNm), lpAcct->AccNm);
		szName.TrimRight();
		szAdd.Format("%s | %s", szAcct, szName);

		m_cmbAcct.AddString(szAdd);

		szPwd.Format("%.*s", sizeof(lpAcct->AccPass), lpAcct->AccPass);

		// add memory
		memset (&lpMst, 0x20, SZ_ESAccntInfo);
		memcpy (&lpMst, lpAcct, SZ_ESAccntInfo);

		m_aryAcct.Add(lpMst);

		// DB ����
		SaveAcntInfo(szAcct, szName, szPwd);

	}

	if (m_cmbAcct.GetCount() > 0)
		m_cmbAcct.SetCurSel(0);

	PrintMsg( TRUE, TRUE, "���¹�ȣ����Ʈ ������");

	return 0L;
}



// ���������� ��ȸ
BOOL CESApiDllDlgDlg::LoadHedgeTarget(BOOL bReset)
{
	CString sQ;
	CDBHandler db(m_pDBPool->Get());

	sQ.Format("SELECT ACNT_NO FROM ACNT_INFO WHERE ACNT_TP=1");
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		PrintMsg(FALSE, TRUE, "������� �������� ��ȸ ����(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}


	if (bReset)
		m_hedgeTarget.Reset();
	
	while (db->NextRow())
	{
		CString sAcnt(db->Get("ACNT_NO"));
		m_hedgeTarget.Add(sAcnt, sAcnt, TRUE/*LOCK*/);
	}
	return TRUE;
}

// �����ֹ��������� ��ȸ
BOOL	CESApiDllDlgDlg::LoadHedgeOrderAcnt()
{
	CString sQ;
	CDBHandler db(m_pDBPool->Get());

	sQ.Format("SELECT ACNT_NO, ACNT_PWD FROM ACNT_INFO WHERE ACNT_TP=9");
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		PrintMsg(FALSE, TRUE, "�����ֹ��������� ��ȸ ����(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}

	while (db->NextRow())
	{
		m_sHedgeAcc.Format("%s", db->Get("ACNT_NO"));
		m_szHedgePass.Format("%s", db->Get("ACNT_PWD"));
	}

	PrintMsg(TRUE, TRUE, "�����ֹ�����(%s)", m_sHedgeAcc.GetBuffer());
	return TRUE;
}

BOOL	CESApiDllDlgDlg::SaveAcntInfo(CString &acnt, CString &nm, CString& pwd)
{
	//if (!CheckDBPool())
	//{
	//	PrintMsg(FALSE, TRUE, "DB OPEN ����");
	//	return FALSE;
	//}

	CString sQ;
	CDBHandler db(m_pDBPool->Get());

	sQ.Format("EXEC SAVE_ACNT_INFO '%s', '%s', '%s' ", acnt.GetBuffer(), nm.GetBuffer(), pwd.GetBuffer());
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		PrintMsg(FALSE, TRUE, "�������� ���� ����(%s)", sQ.GetBuffer());
		return FALSE;
	}

	return TRUE;
}

LRESULT CESApiDllDlgDlg::OnFCAcctList(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	for (int ii = 0; ii < m_cmbAcct.GetCount(); ii++)
		m_cmbAcct.DeleteString(ii);

	ESAccntInfoFC_st	lpMst;
	CString				szAdd, szAcct, szName, szGrade, szSystem, szCommon, szMicro;
	lpESAccntInfoFC_st	lpAcct = (lpESAccntInfoFC_st)lParam;

	for ( UINT  ii = 0; ii < wParam; ii++, lpAcct++)
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

	for (UINT ii = 0; ii < wParam; ii++, lpCode++)
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
	/////////////////////////////////////////////////////////
	return;
	//////////////////////////////////////////////////////////


	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '2';

	//if (!m_szAutoAcctKey.IsEmpty()) {
	//	memcpy (lpAuto->AutoKey, m_szAutoAcctKey, m_szAutoAcctKey.GetLength());
	//	m_Api.SetAutoUpdate(FALSE, TRUE, szBuff);
	//}

	//m_szAutoAcctKey.Empty();

	GetDlgItem(IDC_BUTTON2)->SetWindowText("���¾�����Ʈ �̵���");
}


void CESApiDllDlgDlg::OnSelchangeComboCode()
{
	////////////////////////////////////////////////////////////////////
	return;
	////////////////////////////////////////////////////////////////////


	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '3';

	//if (!m_szAutoCodeKey.IsEmpty()) {
	//	memcpy (lpAuto->AutoKey, m_szAutoCodeKey, m_szAutoCodeKey.GetLength());
	//	m_Api.SetAutoUpdate(FALSE, FALSE, szBuff);
	//}

	//m_szAutoCodeKey.Empty();

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


void CESApiDllDlgDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CloseNotedThread();
	DBClose();
	DeleteCriticalSection(&m_csAPI);
	SAFE_DELETE(m_log);
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

	memset(szBuff, 0x00, sizeof(szBuff));
	//memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy(lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '2';

	int nCnt = m_hedgeTarget.GetCount();
	if (nCnt > 0) {
		CString *pAcnt = new CString[nCnt];
		m_hedgeTarget.GetAllData(pAcnt);
		for (int i = 0; i < nCnt; i++)
		{
			memset(lpAuto->AutoKey, 0x20, sizeof(lpAuto->AutoKey));
			memcpy(lpAuto->AutoKey, pAcnt[0], pAcnt[0].GetLength());
			m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);
			PrintMsg(TRUE, TRUE, "[%s] ���� �ڵ�������Ʈ ����", pAcnt[0].GetBuffer());
		}
		delete[] pAcnt;
	}

#if 0
	// ���� �ڵ�������Ʈ(AUTO_0985 ���̵�� ����)
	for (int ii = 0; ii < m_cmbAcct.GetCount(); ++ii)
	{
		memset(lpAuto->AutoKey, 0x20, sizeof(lpAuto->AutoKey));
		m_szAutoAcctKey = GetAccount(ii);
		memcpy(lpAuto->AutoKey, m_szAutoAcctKey, m_szAutoAcctKey.GetLength());
		m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);
		PrintMsg(TRUE, TRUE, "[%s] ���� �ڵ�������Ʈ ����", m_szAutoAcctKey.GetBuffer());
	}
#endif

#if 0
	// HEDGE TARGET ����
	CHAR acnt_header[32] = { 0, };
	CHAR acnt_body[32] = { 0, };
	CHAR acnt_no[32] = { 0, };
	CHAR acnt_cnt[32] = { 0, };

	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE_TARGET", "ACNT_HEADER", acnt_header);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE_TARGET", "COUNT", acnt_cnt);

	char tmp[32];
	for (int i = 0; i < atoi(acnt_cnt); i++)
	{
		sprintf(tmp, "%d", i);
		CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE_TARGET", tmp, acnt_body);
		sprintf(acnt_no, "%s%s", acnt_header, acnt_body);

		memcpy(lpAuto->AutoKey, acnt_no, strlen(acnt_no));
		m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);	//At0985
		PrintMsg(TRUE, TRUE, "[%s] ���� �ڵ�������Ʈ ����", acnt_no);
	}
#endif
}


long CESApiDllDlgDlg::OnTrayIcon(WPARAM wParam, LPARAM IParam)
{
	m_myTray.ProcTrayMsg(GetSafeHwnd(), wParam, IParam);
	return 0;
}

void CESApiDllDlgDlg::OnAppExit(void)
{
	m_myTray.DelTrayIcon(GetSafeHwnd());
	CDialog::OnCancel();
}

void CESApiDllDlgDlg::OnDialogShow(void)
{
	if (!m_bHide) ShowWindow(false);
	else ShowWindow(true);
	m_bHide = !m_bHide;
	m_myTray.m_bHide = m_bHide;
}

void CESApiDllDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nID == SC_CLOSE)
		OnDialogShow();
	else
		CDialogEx::OnSysCommand(nID, lParam);
}


void CESApiDllDlgDlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CESApiDllDlgDlg::OnBnClickedButtonHedgeReload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadHedgeTarget(TRUE);

}



/*
�������뺸 Q Table polling
*/
VOID CESApiDllDlgDlg::NotedThread()
{
	PrintMsg(TRUE, FALSE, "NotedThread start....");
	CString sQ;
	while (m_bContinue)
	{
		Sleep(1000);
		if (m_pDBPool->Available() < 1) {
			Sleep(1000);
			continue;
		}

		CDBHandler db(m_pDBPool->Get());

		sQ.Format("EXEC MANAGER_NOTIFY_READ '%s'", COMP_RCS);
		if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
		{
			PrintMsg(FALSE, TRUE, "������ �۾� �뺸 ��ȸ ����(%s)", sQ.GetBuffer());
			Sleep(3000);
		}

		/*
		@SEQNO
		,@COMP
		,@ACNT_NO
		,@WORK_TP
		,@WORK_VAL
		,@WORK_NOTE
		,@ADMIN_ID
		,@UPDATE_TM
		*/
		while (db->NextRow())
		{
			ST_MANAGER_NOTIFY* p = new ST_MANAGER_NOTIFY;
			ZeroMemory(p, sizeof(ST_MANAGER_NOTIFY));

			p->nSeqNo = db->GetInt("SEQNO");
			//sComp.Format("%.*s", LEN_SYMBOL, rs.value(1).convert<std::string>().c_str());
			sprintf(p->zAcntNo, "%.*s", LEN_ACNT_NO, db->Get("ACNT_NO"));
			p->nWorkTp = db->GetInt("WORK_TP");
			strcpy(p->zWorkVal, db->Get("WORK_VAL"));
			strcpy(p->zWorkNote, db->Get("WORK_NOTE"));
			strcpy(p->zAdminID, db->Get("ADMIN_ID"));

			// ���ο� ���� �߰��� ���. WorkVal �� ����� ���´�.
			if (p->nWorkTp == MNG_ACNT_ADD)
			{
				CString sAcnt(p->zAcntNo);
				m_hedgeTarget.Add(sAcnt, sAcnt, TRUE);
				PrintMsg(TRUE, TRUE, "������� �����߰�(%s)", p->zAcntNo );
				continue;
			}
			else if (p->nWorkTp == MNG_ACNT_AUTOORD_SET)
			{
				CString sAcnt(p->zAcntNo);
				m_hedgeTarget.Add(sAcnt, sAcnt, TRUE);
				PrintMsg(TRUE, TRUE, "������� �����߰�(%s)", p->zAcntNo);
				continue;
			}
			else if (p->nWorkTp == MNG_ACNT_AUTOORD_RELEASE)
			{
				CString sAcnt(p->zAcntNo);
				m_hedgeTarget.Delete(sAcnt, TRUE);
				PrintMsg(TRUE, TRUE, "������� ��������(%s)", p->zAcntNo);
				continue;
			}
		} // while (db->NextRow())

	} // while (m_bContinue)


}
