
// ESApiDllDlgDlg.cpp : 구현 파일
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

#include <synchapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ESM_TEST	WM_USER + 0x9000
#define ID_TIME_LOGON			101	// 최초 로그인 시도하는 타이머

#define DBPoolPointer ((CDBPoolAdo*)m_pDBPool)
CLogMsg g_log;


CESApiDllDlgDlg::CESApiDllDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CESApiDllDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szUserID		= _T("");
	m_szUserPass	= _T("");
	m_szCertPass	= _T("");

	m_pDBPool		= NULL;
	m_bAllChecked	= FALSE;
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
	DDX_Text(pDX, IDC_EDIT_USERID, m_szUserID);
	DDV_MaxChars(pDX, m_szUserID, 8);
	DDX_Text(pDX, IDC_EDIT_USERPASS, m_szUserPass);
	DDV_MaxChars(pDX, m_szUserPass, 8);
	DDX_Text(pDX, IDC_EDIT_CERTPASS, m_szCertPass);
	DDX_Text(pDX, IDC_EDIT1, m_sDllPath);
	DDX_Control(pDX, IDC_MSG, m_lstMsg);
	DDX_Control(pDX, IDC_STATIC_REAL, m_sRealYN);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BUTTON_ALLCHECK, m_btnAllChecked);
	DDX_Control(pDX, IDC_COMBO1, m_ComboSymbol);
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
	ON_BN_CLICKED(IDC_BUTTON3, &CESApiDllDlgDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CESApiDllDlgDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_ALLCHECK, &CESApiDllDlgDlg::OnBnClickedButtonAllcheck)
	ON_BN_CLICKED(IDC_BUTTON9, &CESApiDllDlgDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_CNCLSYMBOL, &CESApiDllDlgDlg::OnBnClickedButtonCnclsymbol)
	ON_BN_CLICKED(IDC_BUTTON_CLRSYMBOL, &CESApiDllDlgDlg::OnBnClickedButtonClrsymbol)
	ON_BN_CLICKED(IDC_BUTTON_CNCL_ALL, &CESApiDllDlgDlg::OnBnClickedButtonCnclAll)
	ON_BN_CLICKED(IDC_BUTTON_CLR_ALL, &CESApiDllDlgDlg::OnBnClickedButtonClrAll)
	ON_BN_CLICKED(IDC_BUTTON_SEL_ORD, &CESApiDllDlgDlg::OnBnClickedButtonSelOrd)
	ON_BN_CLICKED(IDC_BUTTON_SEL_POS, &CESApiDllDlgDlg::OnBnClickedButtonSelPos)
END_MESSAGE_MAP()


// CESApiDllDlgDlg 메시지 처리기

BOOL CESApiDllDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 이미 실행중인지 확인
	if (m_Dup.CheckDuplicate("FBOrderClient") == -1)
	{
		AfxMessageBox("<FB일괄주문>이미 프로세스가 실행중입니다.");
		//OnBnClickedButtonTerminate();
		exit(0);
	}


	m_myTray.m_bHide = m_bHide = false;
	m_myTray.AddTrayIcon(GetSafeHwnd());

	// config file
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);
	m_sDir = szDir;
	m_sModule = szModule;
	m_sConfig = szConfig;

	// LOG FILE
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, m_sModule.GetBuffer());

	// API 경로
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DIR", "API", szDir);
	m_sDllPath = szDir;
	g_log.log(LOGTP_SUCC, "API 저장된 폴더 : %s", m_sDllPath.GetBuffer());

	// API 계정
	//CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ID", szDir);
	//m_szUserID = szDir;
	m_szUserID = "dktks7";

	//CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "USER_PWD", szDir);
	//m_szUserPass = szDir;
	m_szUserPass = "eoqkr7";

	//CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "CERT_PWD", szDir);
	//m_szCertPass = szDir;
	m_szCertPass = "dudwnseoqkr7*";

	//CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ACNT_PWD", szDir);
	//m_szHedgePass = szDir;

	// 접속서버
	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "REAL_YN", szDir);
	m_cRealYN = szDir[0];

	if (!DBOpen()){
		AfxMessageBox("DB OPEN 실패");
		exit(0);
	}

	InitializeCriticalSectionAndSpinCount(&m_csApi, 2000);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("선택"), LVCFMT_RIGHT, 50, -1);
	m_list.InsertColumn(1, _T("계좌번호"), LVCFMT_CENTER, 100, -1);
	m_list.InsertColumn(2, _T("계좌비번"), LVCFMT_CENTER, 100, -1);
	//m_list.InsertColumn(3, _T("미청산잔고"), LVCFMT_RIGHT, 80, -1);

	// 1초 간격 타이머
	SetTimer(ID_TIME_LOGON, 1000, NULL);

	UpdateData(FALSE);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


BOOL CESApiDllDlgDlg::DBOpen()
{
	char ip[32], id[32], pwd[32], cnt[32], dbname[32];
	//CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_IP", ip);
	//CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_ID", id);
	//CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_PWD", pwd);
	//CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_NAME", dbname);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_POOL_CNT", cnt);
	strcpy(ip, "211.247.235.227,9520");
	strcpy(id, "fbsa");
	strcpy(pwd, "fbsa123!@#");
	strcpy(dbname, "KRHEDGE");

	m_pDBPool = (VOID*)new CDBPoolAdo(ip, id, pwd, dbname);
	if (!DBPoolPointer->Init(atoi(cnt)))
	{
		PrintMsg(LOGTP_FATAL, TRUE, "DB OPEN FAIL(MSG:%s)", ((CDBPoolAdo*)m_pDBPool)->GetMsg());
		PrintMsg(LOGTP_FATAL, TRUE, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, dbname);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}

	return TRUE;
}


VOID CESApiDllDlgDlg::DBClose()
{
	SAFE_DELETE(m_pDBPool);
}

BOOL CESApiDllDlgDlg::LoadAcntInfo()
{ 
	CDBHandlerAdo db(DBPoolPointer->Get());

	CString sQ;
	sQ.Format("SELECT ACNT_NO, ACNT_PWD FROM FB_ACNT_INFO WHERE ACNT_NO NOT IN  ('10150503245','10150800915') ORDER BY ACNT_NO DESC");
	if (!db->QrySelect(sQ.GetBuffer()))
	{
		PrintMsg(LOGTP_ERR, TRUE, "LoadAcntInfo 조회 오류(%s)", sQ.GetBuffer());
		return FALSE;;
	}

	char sAcnt[32] = { 0, }, sPwd[32] = { 0, };
	int ii = 0;
	m_list.DeleteAllItems();
	while (db->IsNextRow())
	{
		db->GetStr("ACNT_NO", sAcnt);
		db->GetStr("ACNT_PWD", sPwd);

		USER_INFO* p = new USER_INFO;
		p->sAcntPwd = sPwd;
		m_mapAcnt[CString(sAcnt)] = p;


		// LIST 추가
		m_list.InsertItem(ii, "");
		m_list.SetItem(ii, 1, LVIF_TEXT, sAcnt, 0, 0, 0, NULL);
		m_list.SetItem(ii, 2, LVIF_TEXT, sPwd, 0, 0, 0, NULL);
		//m_list.SetItem(ii, 3, LVIF_TEXT, "0", 0, 0, 0, NULL);
		
		PrintMsg(LOGTP_SUCC, TRUE, "계좌정보 조회(%s)(%s)", sAcnt, sPwd);

		db->Next();
	}
	
	

	UpdateData(FALSE);

	return TRUE;
}


BOOL CESApiDllDlgDlg::LoadSiseSymbol()
{
	CDBHandlerAdo db(DBPoolPointer->Get());
	CString sQ("SELECT SYMBOL FROM SYMBOL_INFO WHERE FB_ORD_YN='Y'");


	char zTemp[32];
	while(m_ComboSymbol.GetCount()>0)
		m_ComboSymbol.DeleteString(0);

	if (!db->QrySelect(sQ.GetBuffer()))
	{
		PrintMsg(LOGTP_ERR, TRUE, "LoadAcntInfo 조회 오류(%s)", sQ.GetBuffer());
		return FALSE;;
	}

	char sAcnt[32] = { 0, }, sPwd[32] = { 0, };
	int ii = 0;
	m_list.DeleteAllItems();
	while (db->IsNextRow())
	{
		db->GetStr(0, zTemp);

		//ir_cvtcode_uro_6e(zTemp, zSymbol);

		//// KR 은 CLQ7, 다른곳은 CLQ17
		//ir_cvtcode_HD_KR(zSymbol, zTemp);
		CString symbol = zTemp;


		m_ComboSymbol.AddString(symbol);

		db->Next();
	}
	if (m_ComboSymbol.GetCount() > 0)
		m_ComboSymbol.SetCurSel(0);

	UpdateData(FALSE);
}

VOID CESApiDllDlgDlg::CloseAcntInfo()
{
	std::map<CString, USER_INFO*>::iterator it;
	for (it = m_mapAcnt.begin(); it != m_mapAcnt.end(); it++)
		delete (*it).second;

	m_mapAcnt.clear();
}


VOID	CESApiDllDlgDlg::CreateOrderThread()
{
	m_pOrdThread = new CFBOrdThread(&m_Api, &m_csApi, &m_hWnd);
	m_pOrdThread->ResumeThread();
}

VOID	CESApiDllDlgDlg::CloseOrderThread()
{
	SAFE_DELETE(m_pOrdThread);
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CESApiDllDlgDlg::OnPaint()
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

// 문자형 ->double
double CESApiDllDlgDlg::MyAtof(char *ibuf, int len)
{
	char    tbuf[32];

	memset (tbuf, '\0', sizeof (tbuf));
	memcpy (tbuf, ibuf, len);
	return atof (tbuf);
}

//해외선물 가격변환
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

	if (chType == OUTSCALE_10) { //10진법
		CString szFormat, szTmp;
		szFormat.Format("%%.0%df", nDigitLen);
		szResult->Format(szFormat, MyAtof(pPrice, nLen));
		InsertComma(*szResult, FALSE);
		return;
	}

	if (chType == OUTSCALE_4 || chType == OUTSCALE_8) {	// 4진법 or 8진법
		memcpy (szPrice, szDn, nLen);

		nDigitLen -= 2;
		sprintf (szPrice, "%.*f", nDigitLen, MyAtof(pPrice, nLen) * 100.);

		szUp.Format("%.0f", floor(MyAtof(szPrice, nLen)));
		InsertComma(szUp);

		//소숫점 아래 분리
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

		//소숫점 아래 분리
		for (ii = 0; ii < nCnt; ii++) {
			if (*(pPrice + ii) == '.') {
				memcpy(szDn, pPrice + (ii + 1), MIN(nLen - (ii + 1), nDigitLen) );
				break;
			}
		}
	}

	switch (chType) {
	case OUTSCALE_4 : //4진법
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_4 + 0.5);
		switch (nDn) {
		case 0 :	szResult->Format("%s'0", szUp);	break;
		case 1 :	szResult->Format("%s'2", szUp);	break;
		case 2 :	szResult->Format("%s'4", szUp);	break;
		case 3 :	szResult->Format("%s'6", szUp);	break;
		}
		break;
	case OUTSCALE_8 : //8진법
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_8 + 0.5);
		szResult->Format("%s'%d", szUp, nDn);
		break;
	case OUTSCALE_32 : //32진법
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
	case OUTSCALE_64 : //64진법
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
	case OUTSCALE_128 : //128진법
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

	if (chType == OUTSCALE_10) { //10진법
		szResult->Format("%s", szOrg);
		InsertComma(*szResult, FALSE);
		return TRUE;
	}

	if (chType == OUTSCALE_4 || chType == OUTSCALE_8) {	// 4진법 or 8진법

		nDigitLen -= 2;
		sprintf (pPrice, "%.*f", nDigitLen, dValue * 100.);
		dValue = atof(pPrice);
	}

	szUp.Format("%.0f", floor(dValue));
	InsertComma(szUp);

	//소숫점 아래 분리
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
	case OUTSCALE_4 : //4진법
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
	case OUTSCALE_8 : //8진법
		bRtn = TRUE;
		strncat(szDn, "00000000", nDigitLen - strlen(szDn));
		nDn = (int)floor(atof(szDn) / (double)TICKVALUE_8 + 0.5);
		szResult->Format("%s'%d", szUp, nDn);
		break;
	case OUTSCALE_32 : //32진법
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
	case OUTSCALE_64 : //64진법
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
	case OUTSCALE_128 : //128진법
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

//숫자 Data에 Comma 표시
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
		AfxMessageBox("ESApi.dll 파일의 경로 선택이 필요합니다.");
		return;
	}

	// 로드할 dll의 전체경로
	BOOL bRet = m_Api.LoadDll(m_sDllPath, "ESApi.dll");
	if (!bRet) {
		PrintMsg(LOGTP_FATAL, TRUE, m_sDllPath.GetBuffer());
		return;
	}

	// FreeCap, SignalMaker 아닐경우  OnlyFreeCap, OnlySignalMaker 함수 사용하지 않음
	// FreeCap 사용할 경우
	//m_Api.OnlyFreeCap(TRUE);

	// SignalMaker 사용할 경우
	//m_Api.OnlySignalMaker(TRUE);	

	// server connect
	if( m_cRealYN=='Y')
		m_Api.ConnectServer(m_hWnd, m_szUserID, m_szUserPass, m_szCertPass, SERVER_REAL);
	else
		m_Api.ConnectServer(m_hWnd, m_szUserID, m_szUserPass, m_szCertPass, SERVER_DEMO);


	PrintMsg(LOGTP_SUCC, TRUE, "API LOGIN 시도(ID:%s)(접속비번:%s)(인증비번:%s)(RealServerYN:%c)",
		m_szUserID.GetBuffer(), m_szUserPass.GetBuffer(), m_szCertPass.GetBuffer(), m_cRealYN);
}

void CESApiDllDlgDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

// 이벤트 처리
LRESULT CESApiDllDlgDlg::OnServerConnect(WPARAM wParam, LPARAM lParam)
{
	PrintMsg(LOGTP_SUCC, TRUE, "OnServerConnect.");

	int nErrCode = (int)wParam;

	if(nErrCode == ERR_NONE)
	{
		//AfxMessageBox(_T("서버와 연결되었습니다."));
		if (m_cRealYN == 'Y') {
			PrintMsg(LOGTP_SUCC, TRUE, "REAL 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("REAL 접속");
		}
		else {
			PrintMsg(LOGTP_SUCC, TRUE, "TEST 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("TEST 접속");
		}

		// 종목정보 조회
		LoadSiseSymbol();

		// 계좌정보 조회
		LoadAcntInfo();


		// 주문 스레드 생성
		CreateOrderThread();

		// auto update
		OnClickedButtonAuto();

		// 미체결 및 잔고 TR 조회
		OnBnClickedButtonAllcheck();
		OnBnClickedButtonSelOrd();
		OnBnClickedButtonSelPos();
		PrintMsg(LOGTP_SUCC, TRUE, "미체결 및 잔고 조회 요청");
		OnBnClickedButtonAllcheck();

		// 로그파일 삭제
		WinExec("logdel.bat", SW_SHOW);

	}
	else
	{
		LPCTSTR lpstrMsg = (LPCTSTR)lParam;
		if(lpstrMsg != NULL){
			//AfxMessageBox(lpstrMsg);
			PrintMsg(LOGTP_FATAL, TRUE, "서버와 연결 실패했습니다(%s)", lpstrMsg);

			//TODO
			// 재접속을 위한 무언가를 해야 한다.
			OnBnClickedOk();
		}

		PostMessage(WM_ESM_TEST);
	}

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnServerDisConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("서버와 연결이 종료 되었습니다.");
	PrintMsg(LOGTP_SUCC, TRUE, "서버와 연결이 종료 되었습니다.");

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString	szBuf, szTmp, szCode;

	// ERROR 메세지
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
	
	case AUTO_0985: // 잔고/체결 실시간 
	{
		lpAt0985O_st lpTrOut = (lpAt0985O_st)lParam;
		//if (lpTrOut->ReplyType[0] == '2')
		{
			CString sAcnt;
			sAcnt.Format("%.*s", sizeof(lpTrOut->Account), lpTrOut->Account);			
			At0985O_st* p = new At0985O_st;
			memcpy(p, (void*)lParam, sizeof(At0985O_st));
			m_pOrdThread->APIRecvReal(p);
		}
		
	}break;
	case ESID_5611:	//미체결 조회
	{
		lpEs5611O_st p = new Es5611O_st;
		memcpy(p, (void*)lParam, sizeof(Es5611O_st));
		m_pOrdThread->APIRecvTr(1, (void*)p);
	}break;

	case ESID_5612:	//실잔고. 잔고 조회시 서버로 전송
	{
		lpEs5612O_st p = new Es5612O_st;
		memcpy(p, (void*)lParam, sizeof(Es5612O_st));
		m_pOrdThread->APIRecvTr(2, (void*)p);
		
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
	}

	if (m_cmbAcct.GetCount() > 0)
		m_cmbAcct.SetCurSel(0);

	PrintMsg(LOGTP_SUCC, TRUE, "계좌번호리스트 가져옴");

	return 0L;
}


//BOOL	CESApiDllDlgDlg::SaveAcntInfo(CString &acnt, CString &nm/*, CString& pwd*/)
//{
//	//if (!CheckDBPool())
//	//{
//	//	PrintMsg(FALSE, TRUE, "DB OPEN 오류");
//	//	return FALSE;
//	//}
//
//	CString sQ;
//	try
//	{
//		CDBHandler db(m_pDBPool->Get());
//		
//		sQ.Format("EXEC FB_SAVE_ACNT_INFO '%s', '%s' ", acnt.GetBuffer(), nm.GetBuffer());
//		if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
//		{
//			PrintMsg(LOGTP_ERR, TRUE, "계좌정보 저장 오류(%s)", sQ.GetBuffer());
//			return FALSE;
//		}
//	}
//	catch (...)
//	{
//		PrintMsg(LOGTP_ERR, TRUE, "계좌정보 저장 오류 TRY CATCH(%s)", sQ.GetBuffer());
//		return FALSE;
//	}
//	PrintMsg(LOGTP_SUCC, FALSE, "계좌리스트 가져와서 저장(%s)(%s)", acnt.GetBuffer(), sQ.GetBuffer());
//	return TRUE;
//}

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
		// 프리캡 협의수수료 정보 추가
		// N: None(등급없음), C: Challenger, P: Pro, B: Best, S: SuperStar
		szGrade.Format("%.*s", sizeof(lpAcct->leader_grade), lpAcct->leader_grade);
		// Y: 시스템, N: 일반
		szSystem.Format("%.*s", sizeof(lpAcct->system_yn), lpAcct->system_yn);
		// 일반, 미니상품 수수료
		szCommon.Format("%.*s", sizeof(lpAcct->common_nego_fee), lpAcct->common_nego_fee);
		// 마이크로상품 수수료
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

	GetDlgItem(IDC_BUTTON2)->SetWindowText("계좌업데이트 미동작");
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

	GetDlgItem(IDC_BUTTON3)->SetWindowText("시세업데이트 미동작");
}

void CESApiDllDlgDlg::OnBnClickedOk2()
{
	UpdateData(TRUE);

	CFolderPickerDialog	fd;

	if (fd.DoModal() == IDOK) {

		m_sDllPath = fd.GetPathName();
		PrintMsg(LOGTP_SUCC, TRUE, "서버 로그인 시도...");
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
	m_bContinue = FALSE;
	DBClose();
	CloseOrderThread();
	CloseAcntInfo();
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
		CreateHorizontalScroll();
	}
}



/*
	계좌 자동업데이트
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

	// 계좌 자동업데이트(AUTO_0985 아이디로 받음)
	for (int ii = 0; ii < m_cmbAcct.GetCount(); ++ii)
	{
		memset(lpAuto->AutoKey, 0x20, sizeof(lpAuto->AutoKey));
		sAcnt = GetAccount(ii);
		memcpy(lpAuto->AutoKey, sAcnt, sAcnt.GetLength());
		m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);
		PrintMsg(LOGTP_SUCC, TRUE, "[%s] 계좌 자동업데이트 설정", sAcnt.GetBuffer());
	}

#if 0
	// HEDGE TARGET 계정
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
		PrintMsg(TRUE, TRUE, "[%s] 계좌 자동업데이트 설정", acnt_no);
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



// 계좌번호 load
//BOOL CESApiDllDlgDlg::LoadAcntAndUserThread()
//{
//
//	//if (!CheckDBPool())
//	//{
//	//	PrintMsg(FALSE, TRUE, "[LoadSiseSymbol] DB OPEN 오류");
//	//	return FALSE;
//	//}
//
//	//if (m_pDBPool->Available() < 1) {
//	//	return FALSE;
//	//}
//
//	CDBHandler db(m_pDBPool->Get());
//
//	CString sQ;
//	sQ.Format("SELECT ACNT_NO, ACNT_PWD FROM FB_ACNT_INFO");
//	//TODO sQ.Format("SELECT ACNT_NO, ACNT_PWD FROM FB_ACNT_INFO WHERE ACNT_NO='10151500648'");
//	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
//	{
//		PrintMsg(LOGTP_ERR, TRUE, "LoadAcntAndUserThread 조회 오류(%s)", sQ.GetBuffer());
//		return FALSE;;
//	}
//
//	char sAcnt[32], sPwd[32];
//	while (db->NextRow())
//	{
//		strcpy(sAcnt, db->Get("ACNT_NO"));
//		strcpy(sPwd, db->Get("ACNT_PWD"));
//
//		//sSymbol = rs.value(2).convert<std::string>();
//		m_mapAcnt[CString(sAcnt)] = new CFBOrdThread(sAcnt, sPwd, &m_apiObj, m_pStratLog);
//
//		PrintMsg(LOGTP_SUCC, TRUE, "계좌별 스레드 생성(%s)(%s)", sAcnt, sPwd);
//	}
//
//	return TRUE;
//}





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
//		PrintMsg(FALSE, FALSE, "CHART SHM OPEN 에러(%s)", szSHMName);
//		return FALSE;
//	}
//	
//	
//	//CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
//	//CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);
//	//if (!m_shm[LASTNM].Open(szSHMName, szMutexName))
//	//{
//	//	PrintMsg(FALSE, FALSE, "LASTNM SHM OPEN 에러(%s)", szSHMName);
//	//	return FALSE;
//	//}
//	//PrintMsg(FALSE, FALSE, "SHM OPEN 성공(%s)", szSHMName);
//
//	return TRUE;
//}
//
//VOID CESApiDllDlgDlg::CloseChartSHM()
//{
//	m_shm.Close();
//	//m_shm[LASTNM].Close();
//}

void CESApiDllDlgDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadAcntInfo();
}


void CESApiDllDlgDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString s;
	CHeaderCtrl* pHeaderCtrl = m_list.GetHeaderCtrl();
	//int idx = pHeaderCtrl->getsele

	s.Format("%d", m_list.GetItemCount());
	AfxMessageBox(s);
}


void CESApiDllDlgDlg::OnBnClickedButtonAllcheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for( int i=0; i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i, !m_bAllChecked);
	}
	m_bAllChecked = !m_bAllChecked;

	if (m_bAllChecked)
		m_btnAllChecked.SetWindowTextA("전체 해제");
	else
		m_btnAllChecked.SetWindowTextA("전체 선택");

	UpdateData(TRUE);
}


void CESApiDllDlgDlg::OnBnClickedButton9()
{
	UpdateData(TRUE);

	//if(m_list.GetSelectedCount()==0)
	//{
	//	AfxMessageBox("주문할 계좌를 선택하세요.");
	//	return;
	//}

	char cSide = 0x00, cTp=0x00;
	
	if (IsDlgButtonChecked(IDC_RADIO_BUY))
		cSide = DEF_BUY;
	if (IsDlgButtonChecked(IDC_RADIO_SELL))
		cSide = DEF_SELL;
	if(cSide==0x00)
	{ 
		AfxMessageBox("매수 매도를 선택하세요.");
		return;
	}

	if (IsDlgButtonChecked(IDC_RADIO_LIMIT))
		cTp = DEF_LIMIT_ORD;
	if (IsDlgButtonChecked(IDC_RADIO_MARKET))
		cTp = DEF_MARKET_ORD;

	if (cTp == 0x00)
	{
		AfxMessageBox("시장가 지정가를 선택하세요.");
		return;
	}

	m_szOrdP.Trim();
	if (cTp == DEF_LIMIT_ORD)
	{
		if (m_szOrdP.GetLength() == 0)
		{
			AfxMessageBox("지정가 주문은 가격을 입력해야 합니다.");
			return;
		}
	}

	m_szOrdV.Trim();
	if(m_szOrdV.GetLength()==0)
	{
		AfxMessageBox("수량을 입력하세요.");
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			CString sAcntNo = m_list.GetItemText(i, 1);
			CString sPwd = m_list.GetItemText(i, 2);
			CString sSymbol;
			m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);

			m_pOrdThread->OrdSend_General(sAcntNo, sSymbol, sPwd, atoi(m_szOrdV.GetBuffer()), m_szOrdP.GetBuffer(), cSide);

			
		}
	}

	//m_szOrdV = "";
	m_szOrdP = "";
	//((CButton*)GetDlgItem(IDC_RADIO_SELL))->SetCheck(BST_UNCHECKED);
	//((CButton*)GetDlgItem(IDC_RADIO_BUY))->SetCheck(BST_UNCHECKED);
	//((CButton*)GetDlgItem(IDC_RADIO_LIMIT))->SetCheck(BST_UNCHECKED);
	//((CButton*)GetDlgItem(IDC_RADIO_MARKET))->SetCheck(BST_UNCHECKED);

	UpdateData(FALSE);
}


// 전체취소
void CESApiDllDlgDlg::OnBnClickedButtonCnclAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//if (m_list.GetSelectedCount() == 0)
	//{
	//	AfxMessageBox("주문할 계좌를 선택하세요.");
	//	return;
	//}

	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			CString sAcntNo = m_list.GetItemText(i, 1);
			CString sPwd = m_list.GetItemText(i, 2);
			CString sSymbol;
			m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);

			m_pOrdThread->OrderSend(ORDSEND_CNCL_ACNT, sAcntNo, CString(""), sPwd, 0);
		}
	}
}

// 종목취소
void CESApiDllDlgDlg::OnBnClickedButtonCnclsymbol()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//if (m_list.GetSelectedCount() == 0)
	//{
	//	AfxMessageBox("주문할 계좌를 선택하세요.");
	//	return;
	//}

	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			CString sAcntNo = m_list.GetItemText(i, 1);
			CString sPwd = m_list.GetItemText(i, 2);
			CString sSymbol;
			m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);
			m_pOrdThread->OrderSend(ORDSEND_CNCL_SYMBOL, sAcntNo, sSymbol, sPwd, 0);
		}
	}
}

// 
void CESApiDllDlgDlg::OnBnClickedButtonClrsymbol()
{
	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			CString sAcntNo = m_list.GetItemText(i, 1);
			CString sPwd = m_list.GetItemText(i, 2);
			CString sSymbol;
			m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);
			m_pOrdThread->OrderSend(ORDSEND_CLR_SYMBOL, sAcntNo, sSymbol, sPwd, 0);
		}
	}
	
}

// 전체청산
void CESApiDllDlgDlg::OnBnClickedButtonClrAll()
{
	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			CString sAcntNo = m_list.GetItemText(i, 1);
			CString sPwd = m_list.GetItemText(i, 2);
			CString sSymbol;
			m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);
			m_pOrdThread->OrderSend(ORDSEND_CLR_ACNT, sAcntNo, CString(""), sPwd, 0);
		}
	}
}


// 미체결주문조회
void CESApiDllDlgDlg::OnBnClickedButtonSelOrd()
{
	UpdateData(TRUE);
	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			CString sAcntNo = m_list.GetItemText(i, 1);
			CString sPwd = m_list.GetItemText(i, 2);
			CString sSymbol;
			m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);
			RequestRemainOrd(sAcntNo, sPwd);
			Sleep(100);
		}
	}
	UpdateData(FALSE);
}


VOID	CESApiDllDlgDlg::RequestRemainOrd(CString sAcnt, CString sPwd)
{

	CString			szTmp;
	char			szBuff[4096];
	CString			szCode = GetCurCode();
	CString			szAcct = GetCurAccount();
	lpEs5611I_st	lpTrIn = (lpEs5611I_st)szBuff;

	memset(szBuff, 0x00, 4096);
	memset(szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy(lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// 계좌번호
	memcpy(lpTrIn->Account, sAcnt.GetBuffer(), sAcnt.GetLength());

	// 계좌비밀번호
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), sAcnt, sPwd);

	// 체결구분 (0:전체 1:체결2:미체결)
	lpTrIn->Trd_gb[0] = '2';

	// 주문일자 
	char zToday[32];
	CUtil::GetToday(zToday);
	memcpy(lpTrIn->Base_dt, zToday, strlen(zToday));

	// 조회순서 (1:정순 2:역순)
	lpTrIn->Gubn[0] = '1';

	EnterCriticalSection(&m_csApi);
	m_Api.SendTrData(ESID_5611, szBuff, SZ_Es5611I);
	LeaveCriticalSection(&m_csApi);

	UpdateData(TRUE);


}


void CESApiDllDlgDlg::OnBnClickedButtonSelPos()
{

	UpdateData(TRUE);
	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			CString sAcntNo = m_list.GetItemText(i, 1);
			CString sPwd = m_list.GetItemText(i, 2);
			CString sSymbol;
			m_ComboSymbol.GetLBText(m_ComboSymbol.GetCurSel(), sSymbol);
			RequestRemainPos(sAcntNo, sPwd);
			Sleep(100);
		}
	}
	UpdateData(FALSE);
}

VOID CESApiDllDlgDlg::RequestRemainPos(CString sAcnt, CString sPwd)
{

	CString			szTmp;
	char			szBuff[4096] = { 0, };
	CString			szCode = GetCurCode();
	CString			szAcct = GetCurAccount();
	lpEs5612I_st	lpTrIn = (lpEs5612I_st)szBuff;

	memset(szBuff, 0x20, 4096);

	szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
	memcpy(lpTrIn->Head.WinId, szTmp, szTmp.GetLength());

	// 계좌번호
	memcpy(lpTrIn->Account, sAcnt.GetBuffer(), sAcnt.GetLength());

	// 비밀번호
	m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), sAcnt, sPwd);

	EnterCriticalSection(&m_csApi);
	m_Api.SendTrData(ESID_5612, szBuff, SZ_Es5612I);
	LeaveCriticalSection(&m_csApi);
}
void CESApiDllDlgDlg::CreateHorizontalScroll()
{
	CString str; CSize sz; int dx = 0;
	CDC *pDC = m_list.GetDC();

	for (int i = 0; i < m_lstMsg.GetCount(); i++)
	{
		m_lstMsg.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_list.ReleaseDC(pDC);

	if (m_lstMsg.GetHorizontalExtent() < dx)
	{
		m_lstMsg.SetHorizontalExtent(dx);
		ASSERT(m_at_list.GetHorizontalExtent() == dx);
	}
}
