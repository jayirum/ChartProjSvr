
// ESApiDllDlgDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ESApiDllDlg.h"
#include "ESApiDllDlgDlg.h"
#include "afxdialogex.h"
#include "../../IRUM_UTIL/Util.h"
#include <iostream>
#include <debugapi.h>
#include <windows.h>
#include "../../IRUM_UTIL/LogMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ESM_TEST	WM_USER + 0x9000
#define ID_TIME_LOGON			101	// ���� �α��� �õ��ϴ� Ÿ�̸�

CLogMsg g_log;
#define APP_VERSION	"20170728_������ǥ�ر�����û�� �߰�"


CESApiDllDlgDlg::CESApiDllDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CESApiDllDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szUserID		= _T("");
	m_szUserPass	= _T("");
	m_szCertPass	= _T("");
	m_sDllPath		= _T("");

	m_pMemPool		= NULL;
	m_pDBPool		= NULL;
	m_siseSeq		= 0;
	m_publish		= NULL;
	m_pNoteSock = NULL;

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
	//ON_BN_CLICKED(IDC_BUTTON_AUTO, &CESApiDllDlgDlg::OnClickedButtonAuto)
	//ON_BN_CLICKED(IDC_BUTTON_SISE, &CESApiDllDlgDlg::OnBnClickedButtonSise)


	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_COMMAND(WM_APP_EXIT, OnAppExit)
	ON_COMMAND(WM_DIALOG_SHOW, OnDialogShow)
	ON_WM_SYSCOMMAND()
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
	// �̹� ���������� Ȯ��
	if (m_Dup.CheckDuplicate("KRFBOrd") == -1)
	{
		AfxMessageBox("<KRFBOrd>�̹� ���μ����� �������Դϴ�.");
		//OnBnClickedButtonTerminate();
		exit(0);
	}


	m_myTray.m_bHide = m_bHide = false;
	m_myTray.AddTrayIcon(GetSafeHwnd());


	// config file
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir( szDir, szModule, szConfig);
	m_sDir = szDir;
	m_sModule = szModule;
	m_sConfig = szConfig;

	// LOG FILE
	CUtil::GetConfig(m_sConfig.GetBuffer(),  "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, m_sModule.GetBuffer());

	// API ���
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DIR", "API", szDir);
	m_sDllPath = szDir;
	g_log.log(LOGTP_SUCC, "API ����� ���� : %s", m_sDllPath.GetBuffer());

	// VERSION
	g_log.log(LOGTP_SUCC, "--------------------------------------------------------------------------");
	g_log.log(LOGTP_SUCC, "����[%s]%s", __DATE__, APP_VERSION);
	g_log.log(LOGTP_SUCC, "--------------------------------------------------------------------------");

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

	//if (!InitChartSHM()) {
	//	AfxMessageBox("SHM OPEN ����.KRChart���������� Ȯ�� �ʿ�");
	//	return FALSE;
	//}

	if (!InitMemPool()) {
		AfxMessageBox("�޸� Pool OPEN ����");
		return FALSE;
	}

	if (!DBOpen()) {
		AfxMessageBox("DB OPEN ����");
		return FALSE;
	}


	// �ü����� ���ϰ� publishing ���� �ʱ�ȭ
	if (!InitSock()) {
		AfxMessageBox("�ü����� ���� Init ����");
		return FALSE;
	}

	// ���Ź��� �ü��� ���� �����忡�� publishing
	InitPublish();
	
	InitializeCriticalSectionAndSpinCount(&m_csApi, 2000);
	m_apiObj.api = &m_Api;
	m_apiObj.hWnd = &m_hWnd;
	//m_apiObj.shm[CHART] = &m_shm[CHART];
	//m_apiObj.shm[LASTNM] = &m_shm[LASTNM];
	//m_apiObj.pShm = &m_shm;
	m_apiObj.pDB = m_pDBPool;
	m_apiObj.csApi = &m_csApi;

	// Strategy Log Thread
	InitStratLog();


	// ������ notify �� socket
	char ip[32], port[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "NOTIFY_SOCKET_INFO", "IP", ip);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "NOTIFY_SOCKET_INFO", "PORT", port);
	m_pNoteSock = new CTcpSrv;
	m_pNoteSock->Initialize(atoi(port), ip, TRUE);


	// ������ action ó���� ���� polling thread
	if (!InitNotedThread()) {
		AfxMessageBox("�뺸 ������ Init ����");
		return FALSE;
	}


	// 1�� ���� Ÿ�̸�
	SetTimer(ID_TIME_LOGON, 1000, NULL);

	UpdateData(FALSE);
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}



BOOL	CESApiDllDlgDlg::InitStratLog()
{
	m_pStratLog = new CStratLog(m_pDBPool);
	m_pStratLog->Initialize();
	return TRUE;
}

VOID CESApiDllDlgDlg::CloseStratLog()
{
	SAFE_DELETE(m_pStratLog);
}


BOOL CESApiDllDlgDlg::InitPublish()
{
	m_publish = new CSisePubSub(TP_PUB);
	return m_publish->InitSock();
}

VOID CESApiDllDlgDlg::CESApiDllDlgDlg::ClosePublish()
{
	SAFE_DELETE(m_publish);
}


VOID CESApiDllDlgDlg::ResumeThreads()
{
	std::map<CString, void*>::iterator it;

	for (it = m_mapAcnt.begin(); it != m_mapAcnt.end(); it++)
	{
		CFBOrdThread* p = (CFBOrdThread*)(*it).second;
		p->ResumeThread();
	}
}
BOOL CESApiDllDlgDlg::InitMemPool()
{
	m_pMemPool = new Poco::MemoryPool(MEMPOOL_BLOCK_SIZE, 100, MEMPOOL_MAX_UNIT);

	return TRUE;
}

VOID CESApiDllDlgDlg::CloseMemPool()
{
	SAFE_DELETE( m_pMemPool );
}
//

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

	BOOL bUseODBC = FALSE;
	m_pDBPool = new CDBPool(bUseODBC, ip, id, pwd, "KRHedge");
	if (!m_pDBPool->Init(atoi(cnt)))
	{
		PrintMsg(LOGTP_FATAL, TRUE, "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
		PrintMsg(LOGTP_FATAL, TRUE, "(IP:%s)(ID:%s)(PWD:%s)(DB:KRHedgae)", ip, id, pwd);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}

	return TRUE;
}


VOID CESApiDllDlgDlg::CloseUserThread()
{
	std::map<CString, void*>::iterator it;

	for (it = m_mapAcnt.begin(); it != m_mapAcnt.end(); it++)
	{
		CFBOrdThread* p = (CFBOrdThread*)(*it).second;
		delete p;
	}
	m_mapAcnt.clear();
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
		PrintMsg(LOGTP_FATAL, TRUE, m_sDllPath.GetBuffer());
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


	PrintMsg(LOGTP_SUCC, TRUE, "API LOGIN �õ�(ID:%s)(���Ӻ��:%s)(�������:%s)(RealServerYN:%c)",
		m_szUserID.GetBuffer(), m_szUserPass.GetBuffer(), m_szCertPass.GetBuffer(), m_cRealYN);
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



void CESApiDllDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		break;
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
			PrintMsg(LOGTP_SUCC, TRUE, "REAL ������ ����Ǿ����ϴ�.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("REAL ����");
		}
		else {
			PrintMsg(LOGTP_SUCC, TRUE, "TEST ������ ����Ǿ����ϴ�.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("TEST ����");
		}

		// �������� ��ȸ
		LoadAcntAndUserThread();


		// thread ���� �����.
		ResumeThreads();

		// auto update
		OnClickedButtonAuto();

		// �������뺸 ������ ����
		m_notedWrapper.start(*m_notedThread);
	}
	else
	{
		LPCTSTR lpstrMsg = (LPCTSTR)lParam;
		if(lpstrMsg != NULL){
			//AfxMessageBox(lpstrMsg);
			PrintMsg(LOGTP_FATAL, TRUE, "������ ���� �����߽��ϴ�(%s)", lpstrMsg);

			//TODO
			// �������� ���� ���𰡸� �ؾ� �Ѵ�.
			OnBnClickedOk();
		}

		PostMessage(WM_ESM_TEST);
	}

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnServerDisConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("������ ������ ���� �Ǿ����ϴ�.");
	PrintMsg(LOGTP_SUCC, TRUE, "������ ������ ���� �Ǿ����ϴ�.");

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
	
	//PrintMsg(LOGTP_ERR, FALSE, "RECV API CODE(%d)",wParam);

	sprintf(szErrCode, "%.*s", sizeof(pHeader->ErrCode),pHeader->ErrCode);
	if( atoi(szErrCode)!=0 )
	{
		lpESErr_st pErr = (lpESErr_st)lParam;
		PrintMsg(LOGTP_ERR, TRUE, "ERROR!!! (CODE:%d) (MSG:%.50s)", pErr->Head.ErrCode, pErr->ErrMsg);
		//if (wParam == AUTO_0985)
		//{
		//	lpAt0985O_st lpTrOut = (lpAt0985O_st)lParam;
		//	PrintMsg(FALSE, FALSE, "ERROR!!! (%.*s) (%.*s)",
		//		sizeof(lpTrOut->Account), lpTrOut->Account, sizeof(lpTrOut->FullCode), lpTrOut->FullCode);
		//}
		//if (wParam == ESID_5601)
		//{
		//	lpEs5601I_st lpTrOut = (lpEs5601I_st)lParam;
		//	PrintMsg(FALSE, FALSE, "ERROR!!! (ACNT:%.*s) (SYMBOL:%.*s)",
		//		sizeof(lpTrOut->Account), lpTrOut->Account, sizeof(lpTrOut->ShortCode), lpTrOut->ShortCode);
		//}
		return 0L;
	}


	switch(wParam)
	{
	
	case AUTO_0985: // �ܰ�/ü�� �ǽð� 
	{
		lpAt0985O_st lpTrOut = (lpAt0985O_st)lParam;
		//if (lpTrOut->ReplyType[0] == '2')
		{
			CString sAcnt;
			sAcnt.Format("%.*s", sizeof(lpTrOut->Account), lpTrOut->Account);
			std::map<CString, void*>::iterator it = m_mapAcnt.find(sAcnt);
			if (it == m_mapAcnt.end())
			{
				break;
			}
			At0985O_st* p = new At0985O_st;
			memcpy(p, (void*)lParam, sizeof(At0985O_st));
			CFBOrdThread* pWorker = (CFBOrdThread*)(*it).second;
			PostThreadMessage(pWorker->GetMyThreadID(), WM_NOTI_MATCH, 0, (LPARAM)p);
			
			
			CString sFullCode;	sFullCode.Format("%.*s", sizeof(lpTrOut->FullCode), lpTrOut->FullCode);
			CString sSymbol = m_Api.GetShortCode(sFullCode);
			//sprintf(p->zSymbol, "%.*s", sSymbol.GetLength(), sSymbol.GetBuffer());
			//p->cSide[0] = lpTrOut->Open_tp[0];
			//p->nPosQty = S2N(lpTrOut->Open_q, sizeof(lpTrOut->Open_q) );					//	�ܰ����
			//sprintf(p->zPosPrc, "%.*s", sizeof(lpTrOut->ExecPrice), lpTrOut->ExecPrice);	//	ü�ᰡ��
			//sprintf(p->zOpenTM, "%.*s", sizeof(lpTrOut->TradeTime), lpTrOut->TradeTime);

			
			//CString s;
			//s.Format("<ü��ǽð�>[ACC:%.*s] [����:%s] [ü���ȣ:%.*s] [�ܰ����:%c][�ܰ����:%d][ü�ᰡ��:%.*s]",
			//	sizeof(lpTrOut->Account), lpTrOut->Account, sSymbol.GetBuffer(), sizeof(lpTrOut->Trd_no), lpTrOut->Trd_no
			//	, p->cSide[0], p->nPosQty, p->zPosPrc);

			//PrintMsg(TRUE, FALSE, "���ʼ��� Ÿ��(%c) �ֹ�����(%.*s)�ܷ�(%.*s)",
			//	lpTrOut->ReplyType[0],
			//	sizeof(lpTrOut->Qty), lpTrOut->Qty,
			//	sizeof(lpTrOut->RemainQty), lpTrOut->RemainQty);
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

		//szPwd.Format("%.*s", sizeof(lpAcct->AccPass), lpAcct->AccPass);

		// add memory
		memset (&lpMst, 0x20, SZ_ESAccntInfo);
		memcpy (&lpMst, lpAcct, SZ_ESAccntInfo);

		m_aryAcct.Add(lpMst);

		// DB ����
		SaveAcntInfo(szAcct, szName);

		

	}

	if (m_cmbAcct.GetCount() > 0)
		m_cmbAcct.SetCurSel(0);

	PrintMsg(LOGTP_SUCC, TRUE, "���¹�ȣ����Ʈ ������");

	return 0L;
}


BOOL	CESApiDllDlgDlg::SaveAcntInfo(CString &acnt, CString &nm/*, CString& pwd*/)
{
	//if (!CheckDBPool())
	//{
	//	PrintMsg(FALSE, TRUE, "DB OPEN ����");
	//	return FALSE;
	//}

	CString sQ;
	try
	{
		CDBHandler db(m_pDBPool->Get());
		
		sQ.Format("EXEC FB_SAVE_ACNT_INFO '%s', '%s' ", acnt.GetBuffer(), nm.GetBuffer());
		if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
		{
			PrintMsg(LOGTP_ERR, TRUE, "�������� ���� ����(%s)", sQ.GetBuffer());
			return FALSE;
		}
	}
	catch (...)
	{
		PrintMsg(LOGTP_ERR, TRUE, "�������� ���� ���� TRY CATCH(%s)", sQ.GetBuffer());
		return FALSE;
	}
	PrintMsg(LOGTP_SUCC, FALSE, "���¸���Ʈ �����ͼ� ����(%s)(%s)", acnt.GetBuffer(), sQ.GetBuffer());
	return TRUE;
}

LRESULT CESApiDllDlgDlg::OnFCAcctList(WPARAM wParam, LPARAM lParam)
{
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
	return;



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
	return;

	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy (lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '3';

	/*
	if (!m_szAutoCodeKey.IsEmpty()) {
		memcpy (lpAuto->AutoKey, m_szAutoCodeKey, m_szAutoCodeKey.GetLength());
		m_Api.SetAutoUpdate(FALSE, FALSE, szBuff);
	}

	m_szAutoCodeKey.Empty();
	*/
	CString szCode = GetCurCode();

	GetDlgItem(IDC_BUTTON3)->SetWindowText("�ü�������Ʈ �̵���");
}

void CESApiDllDlgDlg::OnBnClickedOk2()
{
	UpdateData(TRUE);

	CFolderPickerDialog	fd;

	if (fd.DoModal() == IDOK) {

		m_sDllPath = fd.GetPathName();
		PrintMsg(LOGTP_SUCC, TRUE, "���� �α��� �õ�...");
	}

	UpdateData(FALSE);
}

LRESULT CESApiDllDlgDlg::OnTestMsg(WPARAM wParam, LPARAM lParam)
{
	if(m_Api.IsServerConnect(m_hWnd))
		m_Api.DisConnectServer(m_hWnd);

	return 0L;
}

// �ü����� ���ϰ� PUBLISH ���� �ʱ�ȭ
BOOL CESApiDllDlgDlg::InitSock()
{

	// server socket init
	char ip[32], port[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "CHART_SOCKET_INFO", "IP", ip);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "CHART_SOCKET_INFO", "PORT", port);
	if (!m_siseRecv.InitSock(ip, port))
	{
		PrintMsg(LOGTP_FATAL, TRUE, "%s", m_siseRecv.GetMsg());
		return FALSE;
	}
	PrintMsg(LOGTP_SUCC, TRUE, m_siseRecv.GetMsg());
	m_bContinue = TRUE;
	m_siseThread = new Poco::RunnableAdapter<CESApiDllDlgDlg>(*this, &CESApiDllDlgDlg::SiseRecvThread);
	m_siseRecvWrapper.start(*m_siseThread);
	PrintMsg(LOGTP_SUCC, TRUE, "IP(%s) PORT(%s)�� �����ؼ� �ü��� �޴´�.", ip, port);

	return TRUE;
}



void CESApiDllDlgDlg::CloseSock()
{
	m_siseRecv.CloseSock();
	SAFE_DELETE(m_siseThread);
}



// ������ action �� �����ϴ� q table polling
BOOL CESApiDllDlgDlg::InitNotedThread()
{
	m_notedThread = new Poco::RunnableAdapter<CESApiDllDlgDlg>(*this, &CESApiDllDlgDlg::NotedThread);
	m_notedWrapper.start(*m_notedThread);
	PrintMsg(LOGTP_SUCC, TRUE, "������ �뺸�� ���� POLLING ������ ����");

	return TRUE;
}


void CESApiDllDlgDlg::CloseNotedThread()
{
	SAFE_DELETE(m_notedThread);
}


void CESApiDllDlgDlg::OnClose()
{
	m_bContinue = FALSE;
	ClosePublish();
	CloseMemPool();
	CloseUserThread();
	//CloseChartSHM();
	DBClose();
	CloseSock();
	SAFE_DELETE(m_pNoteSock);
	DeleteCriticalSection(&m_csApi);
	CDialogEx::OnClose();
}


void CESApiDllDlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	
}



VOID CESApiDllDlgDlg::PrintMsg(LOGMSG_TP tp, BOOL bShow,char* pMsg, ...)
{
	char buff[DEF_LOG_LEN];
	va_list argptr;
	
	va_start(argptr, pMsg);
	vsprintf_s(buff, pMsg, argptr);
	va_end(argptr);

	g_log.log(tp, buff);
	
	if (bShow) {
		m_lstMsg.InsertString(0, buff);
	}
}

/*
	KRChart �� ���� ���簡 �����͸� PULL �Ѵ�.
*/
VOID CESApiDllDlgDlg::SiseRecvThread()
{
	PrintMsg(LOGTP_SUCC, FALSE, "SiseRecvThread start....");
	while (m_bContinue)
	{
		void* pBuf = m_pMemPool->get();
		int nLen = m_siseRecv.RecvData(pBuf);
		if (nLen < 1)
		{
			PrintMsg(LOGTP_ERR, FALSE, "SISE RECV ERROR:%s", m_siseRecv.GetMsg());
			m_pMemPool->release(pBuf);
			Sleep(1000);
			continue;
		}
		//PrintMsg(TRUE, FALSE, "RECV(%s)", pBuf);

		// publishing
		m_publish->Publish(pBuf, nLen);

		m_pMemPool->release(pBuf);
	}
}



VOID CESApiDllDlgDlg::SocketNoteProc(int nTimeout)
{
	WSANETWORKEVENTS NetworkEvents;
	int start = 0, index = 0;

	int nPos = WSAWaitForMultipleEvents(m_pNoteSock->m_nTotSockets, m_pNoteSock->m_wsaEvents, FALSE, nTimeout, FALSE);
	// i��° ���Ͽ��� �̺�Ʈ �߻�
	if (nPos == WSA_WAIT_FAILED)
	{
		m_pNoteSock->SetSockErrMsg("WSAWaitForMultipleEvents");
		PrintMsg(LOGTP_ERR, FALSE, "[WSAWaitForMultipleEvents ERROR](%s)", m_pNoteSock->GetMsg());
		return;
	}
	else if (nPos == WSA_WAIT_TIMEOUT)
		return;

	//WSAResetEvent(m_wsaEvents[index]);

	start = nPos - WSA_WAIT_EVENT_0;

	for (index = start; index < m_pNoteSock->m_nTotSockets; index++)
	{
		int sigEventIdx = WSAWaitForMultipleEvents(1, &m_pNoteSock->m_wsaEvents[index], TRUE, nTimeout, FALSE);
		// signaled �̺�Ʈ ������Ʈ�� üũ�ϱ�
		if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
			continue;

		int retval = WSAEnumNetworkEvents(m_pNoteSock->m_sockArray[index], m_pNoteSock->m_wsaEvents[index], &NetworkEvents);
		if (retval == SOCKET_ERROR)
		{
			m_pNoteSock->SetSockErrMsg("WSAEnumNetworkEvents");
			PrintMsg(LOGTP_ERR, FALSE, "[WSAEnumNetworkEvents ERROR](%s)", m_pNoteSock->GetMsg());
			return;
		}

		if (NetworkEvents.lNetworkEvents == FD_ACCEPT)
		{
			m_pNoteSock->Accept(index);
			PrintMsg(LOGTP_ERR, FALSE, "[accept](%s)", m_pNoteSock->GetMsg());
			continue;
		}

		if (NetworkEvents.lNetworkEvents == (FD_ACCEPT | FD_CLOSE))
		{
			//PrintMsg(LOGTP_SUCC, FALSE, "[CLOSE event] (FD_ACCEPT | FD_CLOSE) ����");
			m_pNoteSock->m_lstTobeClosed.push_back(index);
			continue;
		}

		if ((NetworkEvents.lNetworkEvents == FD_READ) || (NetworkEvents.lNetworkEvents == (FD_READ | FD_CLOSE)))
		{
			char zRecvBuff[1024] = { 0, };
			int nRecvLen = m_pNoteSock->RecvData(index, sizeof(zRecvBuff), zRecvBuff);
			if (nRecvLen < 0) {
				PrintMsg(LOGTP_ERR, FALSE, "%s", m_pNoteSock->GetMsg());
				break;
			}
			if (nRecvLen > 0)
			{
				PrintMsg(LOGTP_SUCC, FALSE, "[NOTIFY_SOCK][%d](%s)", nRecvLen, zRecvBuff);
				std::map<CString, void*>::iterator it;
				for (it = m_mapAcnt.begin(); it != m_mapAcnt.end(); it++)
				{
					ST_MANAGER_SOCKNOTIFY* p = new ST_MANAGER_SOCKNOTIFY;
					ZeroMemory(p, sizeof(ST_MANAGER_SOCKNOTIFY));
					CopyMemory(p, zRecvBuff, sizeof(ST_MANAGER_SOCKNOTIFY));

					CFBOrdThread* pWorker = (CFBOrdThread*)(*it).second;
					PostThreadMessage(pWorker->GetMyThreadID(), WM_MANAGER_NOTIFY_SOCK, 0, (LPARAM)p);
				}
				m_pNoteSock->m_lstTobeClosed.push_back(index);
			}
		}

		if (NetworkEvents.lNetworkEvents == FD_CLOSE)
		{
			//PrintMsg(LOGTP_SUCC, FALSE, "Close socket");
			m_pNoteSock->m_lstTobeClosed.push_back(index);
		}
	} // for (index = start; index < m_pNoteSock->m_nTotSockets; index++)

	std::list<int>::iterator it;
	for (it = m_pNoteSock->m_lstTobeClosed.begin(); it != m_pNoteSock->m_lstTobeClosed.end(); it++)
	{
		m_pNoteSock->CloseClient((*it));
	}
	m_pNoteSock->m_lstTobeClosed.clear();	
}


/*
	�������뺸 Q Table polling
*/
VOID CESApiDllDlgDlg::NotedThread()
{
	PrintMsg(LOGTP_SUCC, FALSE, "NotedThread start....");
	int nLoopCnt=0;
	CString sQ;

	while(m_bContinue)
	{
		SocketNoteProc(10);

		//if (nLoopCnt++ < 100)	//1 secs
		//	continue;

		// DB �� 10�ʿ� �ѹ� ��ȸ�Ѵ�.
		nLoopCnt = 0;
		if (m_pDBPool->Available() < 1) {
			Sleep(1000);
			continue;
		}

		CDBHandler db(m_pDBPool->Get());
		sQ.Format("EXEC MANAGER_NOTIFY_READ '%s'", COMP_FB);
		if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
		{
			PrintMsg(LOGTP_ERR, TRUE, "������ �۾� �뺸 ��ȸ ����(%s)", sQ.GetBuffer());
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

			// ������ ����û�� �� �������
			if ((p->nWorkTp == MNG_FB_CLEAR_ALL)|| (p->nWorkTp == MNG_FB_CNCL_ALL))
			{
				std::map<CString, void*>::iterator it;
				for (it = m_mapAcnt.begin(); it != m_mapAcnt.end(); it++)
				{
					ST_MANAGER_NOTIFY* pp = new ST_MANAGER_NOTIFY;
					ZeroMemory(pp, sizeof(ST_MANAGER_NOTIFY));
					CopyMemory(pp, p, sizeof(ST_MANAGER_NOTIFY));

					CFBOrdThread* pWorker = (CFBOrdThread*)(*it).second;
					PostThreadMessage(pWorker->GetMyThreadID(), WM_MANAGER_NOTIFY, 0, (LPARAM)pp);
				}
				delete p;
			}
			else
			{

				//PrintMsg(LOGTP_SUCC, TRUE, "<NotedThread>(%d)(%s)(%s)", p->nSeqNo, p->zAcntNo, p->zWorkNote);
				// ���ο� ���� �߰��� ���. WorkVal �� ����� ���´�.
				if (p->nWorkTp == MNG_FB_ACNT_ADD)
				{
					m_mapAcnt[CString(p->zAcntNo)] = new CFBOrdThread(p->zAcntNo, p->zWorkVal, &m_apiObj, m_pStratLog);
					//PrintMsg(LOGTP_SUCC, TRUE, "�����߰�. ������ ����(%s)(%s)", p->zAcntNo, p->zWorkVal);
					continue;
				}

				// �ش� �����忡�� ����
				CString sAcntNo(p->zAcntNo);
				std::map<CString, void*>::iterator it = m_mapAcnt.find(sAcntNo);
				if (it == m_mapAcnt.end())
				{
					PrintMsg(LOGTP_ERR, FALSE, "�۾��������� �����尡 ����.(%s)", sAcntNo.GetBuffer());
					continue;
				}
				CFBOrdThread* pWorker = (CFBOrdThread*)(*it).second;
				PostThreadMessage(pWorker->GetMyThreadID(), WM_MANAGER_NOTIFY, 0, (LPARAM)p);
			}
		} // while (db->NextRow())

		db->Close();

	} // while (m_bContinue)


}



/*
	���� �ڵ�������Ʈ
*/
void CESApiDllDlgDlg::OnClickedButtonAuto()
{
	CString			szTmp, sAcnt;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	memset(szBuff, 0x00, sizeof(szBuff));
	//memset (szBuff, 0x20, sizeof(szBuff));

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy(lpAuto->Head.WinId, szTmp, szTmp.GetLength());

	lpAuto->Head.PCKey[0] = '2';

	// ���� �ڵ�������Ʈ(AUTO_0985 ���̵�� ����)
	for (int ii = 0; ii < m_cmbAcct.GetCount(); ++ii)
	{
		memset(lpAuto->AutoKey, 0x20, sizeof(lpAuto->AutoKey));
		sAcnt = GetAccount(ii);
		memcpy(lpAuto->AutoKey, sAcnt, sAcnt.GetLength());
		m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);
		PrintMsg(LOGTP_SUCC, TRUE, "[%s] ���� �ڵ�������Ʈ ����", sAcnt.GetBuffer());
	}

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


void CESApiDllDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	if (nID == SC_CLOSE || nID==SC_MINIMIZE)
		OnDialogShow();
	else
		CDialogEx::OnSysCommand(nID, lParam);
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



// ���¹�ȣ load
BOOL CESApiDllDlgDlg::LoadAcntAndUserThread()
{

	//if (!CheckDBPool())
	//{
	//	PrintMsg(FALSE, TRUE, "[LoadSiseSymbol] DB OPEN ����");
	//	return FALSE;
	//}

	//if (m_pDBPool->Available() < 1) {
	//	return FALSE;
	//}

	CDBHandler db(m_pDBPool->Get());

	CString sQ;
	sQ.Format("SELECT ACNT_NO, ACNT_PWD FROM FB_ACNT_INFO");
	//TODO sQ.Format("SELECT ACNT_NO, ACNT_PWD FROM FB_ACNT_INFO WHERE ACNT_NO='10151500648'");
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		PrintMsg(LOGTP_ERR, TRUE, "LoadAcntAndUserThread ��ȸ ����(%s)", sQ.GetBuffer());
		return FALSE;;
	}

	char sAcnt[32], sPwd[32];
	while (db->NextRow())
	{
		strcpy(sAcnt, db->Get("ACNT_NO"));
		strcpy(sPwd, db->Get("ACNT_PWD"));

		//sSymbol = rs.value(2).convert<std::string>();
		m_mapAcnt[CString(sAcnt)] = new CFBOrdThread(sAcnt, sPwd, &m_apiObj, m_pStratLog);

		PrintMsg(LOGTP_SUCC, TRUE, "���º� ������ ����(%s)(%s)", sAcnt, sPwd);
	}

	return TRUE;
}




#if 0
BOOL CESApiDllDlgDlg::InitUserThread()
{
	std::map<CString, CFBOrdThread*>::iterator it;

	for (it = m_mapAcnt.begin(); it != m_mapAcnt.end(); it++)
	{
		CString sAcnt = (*it).first;
		(*it).second = new CFBOrdThread(sAcnt, &m_apiObj);
	}

	return TRUE;
}
#endif



//BOOL CESApiDllDlgDlg::InitChartSHM()
//{
//	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_INFO", "SHM_NAME", szSHMName);
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_INFO", "MUTEX_NAME", szMutexName);
//
//	//	OPEN SHM
//	//if (!m_shm[CHART].Open(szSHMName, szMutexName))
//	if (!m_shm.Open(szSHMName, szMutexName))
//	{
//		PrintMsg(FALSE, FALSE, "CHART SHM OPEN ����(%s)", szSHMName);
//		return FALSE;
//	}
//	
//	
//	//CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
//	//CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);
//	//if (!m_shm[LASTNM].Open(szSHMName, szMutexName))
//	//{
//	//	PrintMsg(FALSE, FALSE, "LASTNM SHM OPEN ����(%s)", szSHMName);
//	//	return FALSE;
//	//}
//	//PrintMsg(FALSE, FALSE, "SHM OPEN ����(%s)", szSHMName);
//
//	return TRUE;
//}
//
//VOID CESApiDllDlgDlg::CloseChartSHM()
//{
//	m_shm.Close();
//	//m_shm[LASTNM].Close();
//}