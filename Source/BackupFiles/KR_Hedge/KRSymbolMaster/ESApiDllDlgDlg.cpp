
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ESM_TEST	WM_USER + 0x9000
#define ID_TIME_LOGON			101	// 최초 로그인 시도하는 타이머
#define ID_TIME_SYMBOLMASTER	201

CLog g_log;


CESApiDllDlgDlg::CESApiDllDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CESApiDllDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szUserID		= _T("");
	m_szUserPass	= _T("");
	m_szCertPass	= _T("");
	m_sDllPath		= _T("");
	m_pDBPool		= NULL;
	m_bStart = FALSE;
	m_bFirst = FALSE;
}

void CESApiDllDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//  DDX_Control(pDX, IDC_COMBO_ACCT, m_cmbAcct);
	DDX_Control(pDX, IDC_COMBO_CODE, m_cmbCode);
	//  DDX_Text(pDX, IDC_EDIT_PASS, m_szHedgePass);
	//  DDV_MaxChars(pDX, m_szHedgePass, 8);
	//  DDX_Text(pDX, IDC_EDIT_PASS2, m_szOrdP);
	//  DDV_MaxChars(pDX, m_szOrdP, 20);
	//  DDX_Text(pDX, IDC_EDIT_PASS3, m_szOrdV);
	//  DDV_MaxChars(pDX, m_szOrdV, 20);
	//  DDX_Text(pDX, IDC_EDIT_PASS4, m_szOrgNo);
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



	ON_WM_SYSCOMMAND()
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
	
	// config file
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir( szDir, szModule, szConfig);
	m_sDir = szDir;
	m_sModule = szModule;
	m_sConfig = szConfig;

	// LOG FILE
	CUtil::GetConfig(m_sConfig.GetBuffer(),  "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, m_sModule.GetBuffer());

	// API 경로
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DIR", "API", szDir);
	m_sDllPath = szDir;
	CUtil::LogMsg(&g_log, TRUE, "API 저장된 폴더 : %s", m_sDllPath.GetBuffer());



	// API 계정
	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ID", szDir);
	m_szUserID = szDir;

	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "USER_PWD", szDir);
	m_szUserPass = szDir;

	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "CERT_PWD", szDir);
	m_szCertPass = szDir;

	//CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ACNT_PWD", szDir);
	//m_szHedgePass = szDir;
	
	// 접속서버
	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "REAL_YN", szDir);
	m_cRealYN = szDir[0];

	if (!DBOpen()) {
		AfxMessageBox("DB OPEN 실패");
		return FALSE;
	}
	
	InitializeCriticalSectionAndSpinCount(&m_csApi, 2000);
	

	// 1초 간격 타이머
	SetTimer(ID_TIME_LOGON, 1000, NULL);

	UpdateData(FALSE);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
		PrintMsg(FALSE, TRUE, m_sDllPath.GetBuffer());
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
		case ID_TIME_LOGON:
			PrintMsg(TRUE, TRUE, "TIMER................ LOGON");
			KillTimer(ID_TIME_LOGON);
			OnBnClickedOk();			
			break;
		case ID_TIME_SYMBOLMASTER:
			PrintMsg(TRUE, TRUE, "TIMER................ SYMBOL");
			KillTimer(ID_TIME_SYMBOLMASTER);
			if (SaveSymbolMaster(NULL, FALSE, TRUE)) {
				PrintMsg(TRUE, TRUE, "종목배치처리 성공. 프로그램을 종료합니다.");
				PostQuitMessage(WM_QUIT);
			}
			else
			{
				AfxMessageBox("종목배치처리 오류!!!");
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

// 이벤트 처리
LRESULT CESApiDllDlgDlg::OnServerConnect(WPARAM wParam, LPARAM lParam)
{
	int nErrCode = (int)wParam;

	if(nErrCode == ERR_NONE)
	{
		//AfxMessageBox(_T("서버와 연결되었습니다."));
		if (m_cRealYN == 'Y') {
			PrintMsg(TRUE, TRUE, "REAL 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("REAL 접속");
		}
		else {
			PrintMsg(TRUE, TRUE, "TEST 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("TEST 접속");
		}


		// auto update
		RequestSymbolMaster();

	}
	else
	{
		LPCTSTR lpstrMsg = (LPCTSTR)lParam;
		if(lpstrMsg != NULL){
			//AfxMessageBox(lpstrMsg);
			PrintMsg(FALSE, TRUE, "서버와 연결 실패했습니다(%s)", lpstrMsg);
		}

		PostMessage(WM_ESM_TEST);
	}

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnServerDisConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("서버와 연결이 종료 되었습니다.");
	PrintMsg(TRUE, TRUE, "서버와 연결이 종료 되었습니다."); 

	return 0L;
}


VOID	CESApiDllDlgDlg::RequestSymbolMaster()
{
	char temp[1024];
	int iRslt;
	char			szBuff[4096];
	lpEs5501I_st	lpTrIn = (lpEs5501I_st)szBuff;

	memset(szBuff, 0x00, 4096);
	memset(szBuff, 0x20, 4096);
	sprintf(temp, "%010.0f", (double)(unsigned long)m_hWnd);
	memcpy(lpTrIn->Head.WinId, temp, strlen(temp));

	for (int ii = 0; ii < m_cmbCode.GetCount(); ii++)
		m_cmbCode.DeleteString(ii);

	std::map<CString, CString>::iterator it;
	for (it=m_mapSymbol.begin(); it!=m_mapSymbol.end(); ++it)
	{
		CString sFullCode, sIndex;
		sFullCode = (*it).first;
		sIndex = (*it).first;
		
		memcpy(lpTrIn->FullCode, sFullCode.GetBuffer(), sizeof(lpTrIn->FullCode));
		memcpy(lpTrIn->Index, sIndex.GetBuffer(), sizeof(lpTrIn->Index));

		//m_Api.GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));
		iRslt = m_Api.SendTrData(ESID_5501, szBuff, SZ_Es5501I);

		Sleep(10);
		PrintMsg(TRUE, TRUE, "종목마스터 요청(종목:%s)(INDEX:%s)", sFullCode.GetBuffer(), sIndex.GetBuffer());
	}
}



LRESULT CESApiDllDlgDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString	szBuf, szTmp, szCode;

	// ERROR 메세지
	char szErrCode[5];
	lpESHeader_st pHeader = (lpESHeader_st)lParam;
	
	//PrintMsg(TRUE, FALSE, "[%d]RECV ORG_DATA[ERRCODE:%.4s][%.256s]",(int)wParam, pHeader->ErrCode, (char*)lParam);

	sprintf(szErrCode, "%.*s", sizeof(pHeader->ErrCode),pHeader->ErrCode);
	if( atoi(szErrCode)!=0 )
	{
		lpESErr_st pErr = (lpESErr_st)lParam;
		PrintMsg(FALSE, TRUE, "ERROR!!! (CODE:%d) (MSG:%.50s)",  pErr->Head.ErrCode, pErr->ErrMsg);
	}


	switch(wParam)
	{
	
	case ESID_5501: // 종목마스터 
	{
		if (!m_bStart) {
			SetTimer(ID_TIME_SYMBOLMASTER, 10000, NULL);
			m_bStart = TRUE;
			m_bFirst = TRUE;
			PrintMsg(TRUE, TRUE, "진행중입니다. 성공하는 경우 자동종료됩니다......\n\n");
		}
		else
			m_bFirst = FALSE;

		Es5501O_st* lpTrOut = (Es5501O_st*)lParam;
		ESHeader_st* pH = (ESHeader_st*)lParam;
		
		CString sFullCode; 
		CString sCode; 
		CString sExp, sTickSize;

		sFullCode.Format("%.*s", sizeof(lpTrOut->FullCode), lpTrOut->FullCode);
		sCode = m_Api.GetShortCode(sFullCode);
		sExp.Format("%.*s", sizeof(lpTrOut->ExpiryDate), lpTrOut->ExpiryDate);
		sTickSize.Format("%.*s", sizeof(lpTrOut->Scn_Tick_size), lpTrOut->Scn_Tick_size);

		PrintMsg(TRUE, FALSE, "SYMBOL:%s, 만기일:%.*s, TICK_SIZE:%.*s"
			, sCode.GetBuffer()
			, sizeof(lpTrOut->ExpiryDate), lpTrOut->ExpiryDate
			, sizeof(lpTrOut->Scn_Tick_size), lpTrOut->Scn_Tick_size
		);

		SaveSymbolMaster(lpTrOut, m_bFirst, FALSE);
		
		
	}break;
	default:
		break;
	} /* End of switch(wParam) */

	UpdateData(FALSE);

	return 0L;
}

BOOL CESApiDllDlgDlg::SaveSymbolMaster(Es5501O_st* lpTrOut, BOOL bFirst, BOOL bLast)
{
	CDBHandler db(m_pDBPool->Get());
	CString sQ;

	CString sFullCode;
	CString sCode;
	CString sExp, sTickSize;
	CString sClsPrc1, sClsPrc2;

	if (lpTrOut == NULL)
	{
		sQ.Format("EXEC SYMBOL_MASTER '', '', '', '', '%c', '%c', '', ''",
			 (bFirst) ? 'Y' : 'N'
			, (bLast) ? 'Y' : 'N'
		);
	}
	else
	{
		sFullCode.Format("%.*s", sizeof(lpTrOut->FullCode), lpTrOut->FullCode);
		sCode = m_Api.GetShortCode(sFullCode);

		sExp.Format("%.*s", sizeof(lpTrOut->ExpiryDate), lpTrOut->ExpiryDate);
		sTickSize.Format("%.*s", sizeof(lpTrOut->Scn_Tick_size), lpTrOut->Scn_Tick_size);

		sClsPrc1.Format("%.*s", sizeof(lpTrOut->ClosePrice_1), lpTrOut->ClosePrice_1);
		sClsPrc2.Format("%.*s", sizeof(lpTrOut->ClosePrice_2), lpTrOut->ClosePrice_2);

		sQ.Format("EXEC SYMBOL_MASTER '%s', '%s', '%s', '%s', '%c', '%c', '%s', '%s'",
			sCode.GetBuffer()
			, sFullCode.GetBuffer()
			, sExp.GetBuffer()
			, sTickSize.GetBuffer()
			, (bFirst) ? 'Y' : 'N'
			, (bLast) ? 'Y' : 'N'
			, sClsPrc1.GetBuffer()
			, sClsPrc2.GetBuffer()
		);
	}

	if (FALSE == db->Cmd(sQ.GetBuffer()) || FALSE == db->Exec())
	{
		PrintMsg(FALSE, TRUE, "(%s)(%s)", db->GetLastError(), sQ.GetBuffer());
		return FALSE;
	}
	PrintMsg(TRUE, FALSE, "%s", sQ.GetBuffer());
	return TRUE;

}

LRESULT CESApiDllDlgDlg::OnAcctList(WPARAM wParam, LPARAM lParam)
{

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnFCAcctList(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.


	return 0L;
}

LRESULT CESApiDllDlgDlg::OnCodeList(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	for (int ii = 0; ii < m_cmbCode.GetCount(); ii++)
		m_cmbCode.DeleteString(ii);

	ESCodeInfo_st	lpMst;
	CString			szAdd, szCode, szName, sFullCode, sIndex;
	lpESCodeInfo_st	lpCode = (lpESCodeInfo_st)lParam;

	for (UINT ii = 0; ii < wParam; ii++, lpCode++)
	{
		sFullCode.Format("%.*s", sizeof(lpCode->FullCode), lpCode->FullCode);
		sIndex.Format("%.*s", sizeof(lpCode->Index), lpCode->Index);
		m_mapSymbol[sFullCode] = sIndex;


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

}

void CESApiDllDlgDlg::OnBnClickedOk2()
{
	UpdateData(TRUE);

	CFolderPickerDialog	fd;

	if (fd.DoModal() == IDOK) {

		m_sDllPath = fd.GetPathName();
		PrintMsg(TRUE, TRUE, "서버 로그인 시도...");
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DBClose();
	DeleteCriticalSection(&m_csApi);
	CDialogEx::OnClose();
}


void CESApiDllDlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



VOID CESApiDllDlgDlg::PrintMsg(BOOL bSucc, BOOL bShow,char* pMsg, ...)
{
	char buff[DEF_LOG_LEN];
	va_list argptr;
	
	va_start(argptr, pMsg);
	vsprintf_s(buff, pMsg, argptr);
	va_end(argptr);
	CUtil::LogMsgEx(&g_log, bSucc, buff, buff);
	
	if (bShow) {
		m_lstMsg.InsertString(0, buff);
	}
}



/*
	계좌 자동업데이트
*/
void CESApiDllDlgDlg::OnClickedButtonAuto()
{
	///////////////////////////////////////////////////////////////////////////////
	return;
	///////////////////////////////////////////////////////////////////////////////


}


void CESApiDllDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nID == SC_CLOSE || nID==SC_MINIMIZE)
		OnDialogShow();
	else
		CDialogEx::OnSysCommand(nID, lParam);
}



void CESApiDllDlgDlg::OnAppExit(void)
{
	CDialog::OnCancel();
}

void CESApiDllDlgDlg::OnDialogShow(void)
{
}



