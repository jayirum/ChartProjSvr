
// ESApiDllDlgDlg.cpp : 구현 파일
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

// CESApiDllDlgDlg 대화 상자

#define DEF_LOG_LEN 4096


#define ID_TIME_LOGON			101	// 최초 로그인 시도하는 타이머


CLogMsg g_log;

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

	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_COMMAND(WM_APP_EXIT, OnAppExit)
	ON_COMMAND(WM_DIALOG_SHOW, OnDialogShow)
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
	if (m_Dup.CheckDuplicate("KRHedgeOrder") == -1)
	{
		AfxMessageBox("<KRHedgeOrder>이미 프로세스가 실행중입니다.");
		//OnBnClickedButtonTerminate();
		exit(0);
	}

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

	// API 경로
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DIR", "API", szDir);
	m_sDllPath = szDir;
	CUtil::LogMsg(m_log, TRUE, "API 저장된 폴더 : %s", m_sDllPath.GetBuffer());


	if (!IR_InitTargetRecv()) {
		AfxMessageBox("헷지대상체결정보 수신 소켓 Init 실패");
		return FALSE;
	}



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
	
	// Hedge 주문 여부
	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE", "HEDGE_YN", szDir);
	m_cHedgeYN = szDir[0];

	IR_PrintMsg(TRUE, TRUE, "헷지주문여부:%c", m_cHedgeYN);

	if (!IR_DBOpen()) {
		AfxMessageBox("DB OPEN 실패");
		return FALSE;
	}
	// 헷지대상 계좌 load
	//if(!LoadHedgeTarget(TRUE/*RESET*/)) {
	//	AfxMessageBox("헷지대상계좌 조회 실패");
	//	return FALSE;
	//}

	// thread pool
	if( !IR_InitThreadPool()) {
		AfxMessageBox("Thread Pool 실패");
		return FALSE;
	}


	// 관리자 action 처리를 위한 polling thread
	if (!IR_InitNotedThread()) {
		AfxMessageBox("통보 스레드 Init 실패");
		return FALSE;
	}

	m_bContinue = TRUE;

	// 1초 간격 타이머
	SetTimer(ID_TIME_LOGON, 1000, NULL);


	UpdateData(FALSE);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


// 헷지주문계좌정보 조회
BOOL	CESApiDllDlgDlg::IR_LoadHedgeOrderAcnt()
{
	char tmp[128];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE", "HEDGE_ACC", tmp);
	m_sHedgeAcc = tmp;

	CUtil::GetConfig(m_sConfig.GetBuffer(), "HEDGE", "HEDGE_PWD", tmp);
	m_szHedgePass = tmp;
	m_sAcntPass = tmp;

	IR_PrintMsg(TRUE, TRUE, "헷지주문계좌(%s)", m_sHedgeAcc.GetBuffer());
	return TRUE;
}

// KRHedgeTarget 에서 수신받을 소켓 초기화
BOOL CESApiDllDlgDlg::IR_InitTargetRecv()
{

	// server socket init
	// 이 정보는 대응되는 KRHedgeTarget 또는 HDHedgeTarget 과 일치해야 한다. Target 이 서버이고 Order 가 client 이다.
	char ip[32], port[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "SOCKET_INFO", "IP", ip);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "SOCKET_INFO", "PORT", port);
	if (!m_targetRcv.InitSock(ip, port))
	{
		IR_PrintMsg(FALSE, TRUE, "%s", m_targetRcv.GetMsg());
		return FALSE;
	}
	IR_PrintMsg(TRUE, TRUE, m_targetRcv.GetMsg());
	m_bContinue = TRUE;
	m_targetRcvThread = new Poco::RunnableAdapter<CESApiDllDlgDlg>(*this, &CESApiDllDlgDlg::IR_TargetRecvThread);
	m_targetRecvWrapper.start(*m_targetRcvThread);
	IR_PrintMsg(TRUE, TRUE, "IP(%s) PORT(%s)로 접속해서 체결정보를 받는다.", ip, port);

	return TRUE;
}



void CESApiDllDlgDlg::IR_CloseTargetRecv()
{
	m_targetRcv.CloseSock();
	SAFE_DELETE(m_targetRcvThread);
}



// 관리자 action 을 저장하는 q table polling
BOOL CESApiDllDlgDlg::IR_InitNotedThread()
{
	m_notedThread = new Poco::RunnableAdapter<CESApiDllDlgDlg>(*this, &CESApiDllDlgDlg::IR_NotedThread);
	//m_notedWrapper.start(*m_notedThread);
	IR_PrintMsg(TRUE, TRUE, "관리자 통보를 위한 POLLING 스레드 생성");

	return TRUE;
}


void CESApiDllDlgDlg::IR_CloseNotedThread()
{
	SAFE_DELETE(m_notedThread);
}


VOID CESApiDllDlgDlg::IR_DBClose()
{
	SAFE_DELETE(m_pDBPool);
}

BOOL CESApiDllDlgDlg::IR_CheckDBPool()
{
	return (m_pDBPool->Available() > 0);
}

BOOL CESApiDllDlgDlg::IR_DBOpen()
{
	char ip[32], id[32], pwd[32], cnt[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_ID", id);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_POOL", cnt);

	char temp[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MIN_CNT", temp);
	m_unMinThread = atoi(temp);

	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MAX_CNT", temp);
	m_unMaxThread = atoi(temp);
	

	BOOL bUseODBC = FALSE;
	m_pDBPool = new CDBPool(bUseODBC, ip, id, pwd, "KRHedge");
	if (!m_pDBPool->Init(atoi(cnt)))
	{
		IR_PrintMsg(FALSE, TRUE, "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
		IR_PrintMsg(FALSE, TRUE, "(IP:%s)(ID:%s)(PWD:%s)(DB:KRHedgae)", ip, id, pwd);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}


	return TRUE;
}

BOOL CESApiDllDlgDlg::IR_InitThreadPool()
{
	char temp[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MIN_CNT", temp);
	m_unMinThread = atoi(temp);

	CUtil::GetConfig(m_sConfig.GetBuffer(), "THREADPOOL", "MAX_CNT", temp);
	m_unMaxThread = atoi(temp);

	// thread pool
	m_runAdpter = new Poco::RunnableAdapter<CESApiDllDlgDlg>(*this, &CESApiDllDlgDlg::IR_HedgeOrderThread);
	m_thrPool = new Poco::ThreadPool(m_unMinThread, m_unMaxThread);
	for (int i = 0; i<m_unMaxThread; i++)
		m_thrPool->start(*m_runAdpter);

	return TRUE;
}


VOID CESApiDllDlgDlg::IR_CloseThreadPool()
{
	if (m_thrPool) {
		m_thrPool->joinAll();
		SAFE_DELETE(m_runAdpter);
		SAFE_DELETE(m_thrPool);
	}
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
		IR_PrintMsg(FALSE, TRUE, m_sDllPath.GetBuffer());
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

	IR_PrintMsg(TRUE, TRUE, "API LOGIN 시도(ID:%s)(접속비번:%s)(인증비번:%s)(RealServerYN:%c)",
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



// 헷지주문 주문
void CESApiDllDlgDlg::IR_HedgeOrderThread()
{
	//UpdateData(TRUE);
	char temp[1024];
	int iRslt;

	IR_PrintMsg(TRUE, TRUE, "%s Start...", Poco::Thread::current()->name().c_str());
	while (TRUE)
	{
		Poco::AutoPtr<QRealApiData> pTNf = dynamic_cast<QRealApiData*>(m_queue.waitDequeueNotification());
		if (!pTNf) {
			IR_PrintMsg(TRUE, TRUE, "IR_HedgeOrderThread-break");
			break;
		}
		//IR_PrintMsg(TRUE, TRUE, "IR_HedgeOrderThread:%d", m_queue.size());
		HEDGE_TARGET_CNTR* p = (HEDGE_TARGET_CNTR*)pTNf->get();

		char			szBuff[4096];
		lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

		memset(szBuff, 0x00, 4096);
		memset(szBuff, 0x20, 4096);

		sprintf(temp, "%010.0f", (double)(unsigned long)m_hWnd);
		memcpy(lpTrIn->Head.WinId, temp, strlen(temp));

		// 계좌번호
		memcpy(lpTrIn->Account, m_sHedgeAcc, m_sHedgeAcc.GetLength());

		// 비밀번호
		m_Api.GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), m_sHedgeAcc, m_sAcntPass);

		// 주문구분	(1.신규주문 2.정정주문 3.취소주문)
		lpTrIn->Order_Type[0] = '1';

		// 종목코드
		CString sCode;
		if (ir_isbrokerHD(p->Broker))
		{
			CString sHd;
			char zKr[64];
			sHd.Format("%.*s", sizeof(p->symbol), p->symbol);
			sHd.Trim();
			sCode = ir_cvtcode_HD_KR(sHd.GetBuffer(), zKr);
			memcpy(lpTrIn->ShortCode, zKr, strlen(zKr));
		}
		else
		{
			sprintf(temp, "%.*s", sizeof(p->symbol), p->symbol);
			sCode = m_Api.GetShortCode(temp);
			memcpy(lpTrIn->ShortCode, sCode, sCode.GetLength());
		}


		// 매도매수구분 (1.매수 2.매도). 
		// copytrading 은 그대로, hedge trading 은 반대로 한다.
		if (p->TradeTp[0] == DEF_TRADEGB_COPY) {
			lpTrIn->BuySell_Type[0] = p->side[0];
		}
		else {
			if (p->side[0] == DEF_BUY)	lpTrIn->BuySell_Type[0] = DEF_SELL;
			if (p->side[0] == DEF_SELL)	lpTrIn->BuySell_Type[0] = DEF_BUY;
		}

		// 가격조건	(1.지정가 2.시장가). 무조건 시장가
		lpTrIn->Price_Type[0] = '2';

		// 체결조건 (시장가일경우(3) 지정가일경우(1))
		lpTrIn->Trace_Type[0] = '3';
		// 주문수량
		sprintf(temp, "%.*s", sizeof(p->qty), p->qty);
		int nQty = atoi(temp);
		sprintf(temp, "%0*d", sizeof(lpTrIn->Order_Volume), nQty);
		memcpy(lpTrIn->Order_Volume, temp, sizeof(lpTrIn->Order_Volume));

		// 주문가격
		//memcpy (lpTrIn->Order_Price, m_szOrdP, m_szOrdP.GetLength());


		// 통신주문구분
		m_Api.GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

		// 주문전략구분 (1.일반주문 2.STOP주문)
		lpTrIn->Stop_Type[0] = '1';

		// STOP주문가격 (STOP주문이면 입력 아니면 0 셋팅)
		//lpTrIn->Stop_Price;

		// 주문조작구분 (항상 1로)
		lpTrIn->Oper_Type[0] = '1';

		/*
		IR_PrintMsg(TRUE, TRUE, "[대상체결](ACC:%.*s)(종목:%.*s)(방향:%.*s)(수량:%.*s)",
		sizeof(p->Account), p->Account, sizeof(p->FullCode), p->FullCode, sizeof(p->Side), p->Side, sizeof(p->Qty), p->Qty);
		*/

		if (!IR_IsApiSend())
		{
			IR_PrintMsg(TRUE, TRUE, "ini 파일의 HEDGE_YN 이 N 이라서 API 전송하지 않는다.");
			continue;
		}

		IR_APILOCK();
		iRslt = m_Api.SendTrData(ESID_5601, szBuff, SZ_Es5601I);
		IR_APIUNLOCK();
		IR_PrintMsg(TRUE, TRUE, "%s[헷지체결전송:%d](ACC:%.*s)(종목:%.*s)(방향:%.*s)(수량:%.*s)(비번:%s)",
			Poco::Thread::current()->name().c_str(),iRslt,
			sizeof(lpTrIn->Account), lpTrIn->Account,
			sizeof(lpTrIn->ShortCode), lpTrIn->ShortCode,
			sizeof(lpTrIn->BuySell_Type), lpTrIn->BuySell_Type, 
			sizeof(lpTrIn->Order_Volume), lpTrIn->Order_Volume,
			m_sAcntPass.GetBuffer()
			);

		// DB 저장
		IR_SaveTradeInfo(sCode,(char*)p, szBuff);
	}// end of while
}


BOOL CESApiDllDlgDlg::IR_SaveTradeInfo(CString& sShortCode, char* pTarget, char* pHedge)
{
	// Get DB Session
	if (!IR_CheckDBPool())
	{
		IR_PrintMsg(FALSE, TRUE, "DB OPEN 오류");
		return FALSE;
	}

	CDBHandler db(m_pDBPool->Get());

	lpAt0985O_st t = (lpAt0985O_st)pTarget;
	lpEs5601I_st h = (lpEs5601I_st)pHedge;

	char symbol[64];
	char trade_qty[64], t_trd_no[64];
	char t_acnt[64], t_side[64], t_ord_tp[64], t_dt[64], t_tm[64];
	char t_prc[64];
	char h_acnt[64], h_side[64], h_dt[64], h_tm[64];

	sprintf(symbol, sShortCode.GetBuffer());
	sprintf(trade_qty, "%.*s", sizeof(t->ExecQty), t->ExecQty);
	sprintf(t_acnt, "%.*s", sizeof(t->Account), t->Account);
	sprintf(t_trd_no, "%.*s", sizeof(t->Trd_no), t->Trd_no);
	sprintf(t_side, "%.*s", sizeof(t->Side), t->Side);
	sprintf(t_ord_tp, "%.*s", sizeof(t->Modality), t->Modality);
	sprintf(t_dt, "%.*s", sizeof(t->Trd_date), t->Trd_date);
	sprintf(t_tm, "%.*s", sizeof(t->TradeTime), t->TradeTime);
	sprintf(t_prc, "%.*s", sizeof(t->ExecPrice), t->ExecPrice);
	sprintf(h_acnt, "%.*s", sizeof(h->Account), h->Account);
	sprintf(h_side, "%.*s", sizeof(h->BuySell_Type), h->BuySell_Type);

	SYSTEMTIME st;
	//char temp[32];
	GetLocalTime(&st);
	
	sprintf(h_dt, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	sprintf(h_tm, "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CString sQ;
	

	sQ.Format("INSERT INTO TRADE_INFO(SYMBOL, TRADE_QTY, "
		" TARGET_ACNT_NO, TARGET_TRADE_NO, TARGET_TRADE_SIDE, TARGET_ORD_TP, TARGET_PRICE, TARGET_TRADE_DT, TARGET_TRADE_TM, "
		" HEDGE_ACNT_NO, HEDGE_TRADE_SIDE, HEDGE_TRADE_DT, HEDGE_TRADE_TM )"
		" VALUES('%s',%d,"
		" '%s',%d,'%s','%s',%f,'%s','%s',"
		" '%s','%s','%s','%s')"
		, symbol, atoi(trade_qty)
		, t_acnt, atoi(t_trd_no), t_side, t_ord_tp, atof(t_prc), t_dt, t_tm
		, h_acnt, h_side, h_dt, h_tm);
		
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		IR_PrintMsg(FALSE, TRUE, "체결정보 저장 오류<%s>(MSG:%s)", sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}
	IR_PrintMsg(TRUE, FALSE, "%s[DB저장](ACC:%.*s)(종목:%.*s)(방향:%c)(수량:%.*s)",
		Poco::Thread::current()->name().c_str(), 
		sizeof(h->Account), h->Account,
		sizeof(h->ShortCode), h->ShortCode,
		h->BuySell_Type[0], sizeof(h->Order_Volume), h->Order_Volume);
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

// 이벤트 처리
LRESULT CESApiDllDlgDlg::OnServerConnect(WPARAM wParam, LPARAM lParam)
{
	int nErrCode = (int)wParam;

	if(nErrCode == ERR_NONE)
	{
		//AfxMessageBox(_T("서버와 연결되었습니다."));
		if (m_cRealYN == 'Y') {
			IR_PrintMsg(TRUE, TRUE, "REAL 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("REAL 접속");
		}
		else {
			IR_PrintMsg(TRUE, TRUE, "TEST 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("TEST 접속");
		}


		if (!IR_LoadHedgeOrderAcnt()) {
			AfxMessageBox("헷지주문계좌 조회 실패");
			return FALSE;
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
			IR_PrintMsg(FALSE, TRUE, "서버와 연결 실패했습니다(%s)", lpstrMsg);
		}

		PostMessage(WM_ESM_TEST);
	}

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnServerDisConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("서버와 연결이 종료 되었습니다.");
	IR_PrintMsg(TRUE, TRUE, "서버와 연결이 종료 되었습니다."); 

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
	
	//IR_PrintMsg(TRUE, FALSE, "RECV ORG_DATA[ERRCODE:%.4s][%.256s]", pHeader->ErrCode, (char*)lParam);

	sprintf(szErrCode, "%.*s", sizeof(pHeader->ErrCode),pHeader->ErrCode);
	if( atoi(szErrCode)!=0 )
	{
		lpESErr_st pErr = (lpESErr_st)lParam;
		IR_PrintMsg(FALSE, TRUE, "ERROR!!! (CODE:%.4s) (MSG:%.50s)",  pErr->Head.ErrCode,  pErr->ErrMsg);
	}


	switch(wParam)
	{
	
	case AUTO_0985: // // 잔고/체결 실시간 
	{
		lpAt0985O_st lpTrOut = (lpAt0985O_st)lParam;
		//TRACE ("0985 : [%.*s]\n", SZ_At0985O, (LPSTR)lParam);

		CString s;
		/*
		s.Format("<체결실시간>ESID [%04d][응답유형:%c] [ACC:%.*s] [%.*s] [주문번호:%.*s] [체결번호:%.*s]",
			wParam, lpTrOut->ReplyType[0],
			sizeof(lpTrOut->Account), lpTrOut->Account, sizeof(lpTrOut->FullCode), lpTrOut->FullCode,
			sizeof(lpTrOut->Ord_no), lpTrOut->Ord_no, sizeof(lpTrOut->Trd_no), lpTrOut->Trd_no);
			*/

		
		
		/*
		1.주문접수 2.체결 3.정정확인 4.취소확인
		5.신규거부 6.정정거부 7.취소거부 0.원장접수
		*/
		if (lpTrOut->ReplyType[0] == '2') 
		{

			// 자기자신(헷지주문계좌) 정보만 저장한다.
			CString sAcnt;
			sAcnt.Format( "%.*s", sizeof(lpTrOut->Account), lpTrOut->Account);
			if (m_sHedgeAcc.Compare(sAcnt) != 0)
				break;

			s.Format("<헷지계좌주문리턴>[ACC:%.*s] [%.*s] [주문번호:%.*s] [체결번호:%.*s]",
				sizeof(lpTrOut->Account), lpTrOut->Account, sizeof(lpTrOut->FullCode), lpTrOut->FullCode,
				sizeof(lpTrOut->Ord_no), lpTrOut->Ord_no, sizeof(lpTrOut->Trd_no), lpTrOut->Trd_no);

			IR_PrintMsg(TRUE, FALSE, s.GetBuffer());

		}

	}break;
	default:
		break;
	} /* End of switch(wParam) */

	UpdateData(FALSE);

	return 0L;
}




/*
	KRHedgeOrder 로 부터 데이터 수신
*/
VOID CESApiDllDlgDlg::IR_TargetRecvThread()
{
	IR_PrintMsg(TRUE, FALSE, "IR_TargetRecvThread start....");
	char sRecvBuff[1024];
	//int nBufLen = sizeof(sRecvBuff);
	while (m_bContinue)
	{
		ZeroMemory(sRecvBuff, sizeof(sRecvBuff));
		int nLen = m_targetRcv.RecvData(sizeof(sRecvBuff), sRecvBuff);
		if (nLen < 1)
		{
			IR_PrintMsg(FALSE, FALSE, "SISE RECV ERROR:%s", m_targetRcv.GetMsg());
			Sleep(1000);
			continue;
		}
		else if (nLen == 0) {
			Sleep(1);
			continue;
		}
		IR_PrintMsg(TRUE, FALSE, "RECV(%s)", sRecvBuff);

		// publishing
		m_queue.enqueueNotification(new QRealApiData((void*)sRecvBuff, nLen));

		Sleep(1);
	}
}




LRESULT CESApiDllDlgDlg::OnAcctList(WPARAM wParam, LPARAM lParam)
{
	////////////////////////////////////////////////////////////////////////////////
	//return 0L;
	////////////////////////////////////////////////////////////////////////////////

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

		// DB 저장
		//TODO IR_SaveAcntInfo(szAcct, szName);
		// 주문하는건 INI 에서 읽어온다. 굳이 저장하지 않는다.

	}

	if (m_cmbAcct.GetCount() > 0)
		m_cmbAcct.SetCurSel(0);

	IR_PrintMsg( TRUE, TRUE, "계좌번호리스트 가져옴");

	return 0L;
}



BOOL	CESApiDllDlgDlg::IR_SaveAcntInfo(CString &acnt, CString &nm)
{
	//if (!IR_CheckDBPool())
	//{
	//	IR_PrintMsg(FALSE, TRUE, "DB OPEN 오류");
	//	return FALSE;
	//}

	CString sQ;
	CDBHandler db(m_pDBPool->Get());

	sQ.Format("EXEC SAVE_ACNT_INFO '%s', '%s' ", acnt.GetBuffer(), nm.GetBuffer());
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		IR_PrintMsg(FALSE, TRUE, "계좌정보 저장 오류(%s)", sQ.GetBuffer());
		return FALSE;
	}

	return TRUE;
}

LRESULT CESApiDllDlgDlg::OnFCAcctList(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

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

	GetDlgItem(IDC_BUTTON2)->SetWindowText("계좌업데이트 미동작");
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

	GetDlgItem(IDC_BUTTON3)->SetWindowText("시세업데이트 미동작");
}

void CESApiDllDlgDlg::OnBnClickedOk2()
{
	UpdateData(TRUE);

	CFolderPickerDialog	fd;

	if (fd.DoModal() == IDOK) {

		m_sDllPath = fd.GetPathName();
		IR_PrintMsg(TRUE, TRUE, "서버 로그인 시도...");
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
	IR_CloseTargetRecv();
	IR_CloseNotedThread();
	IR_DBClose();
	DeleteCriticalSection(&m_csAPI);
	SAFE_DELETE(m_log);
	CDialogEx::OnClose();
}


void CESApiDllDlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



VOID CESApiDllDlgDlg::IR_PrintMsg(BOOL bSucc, BOOL bShow,char* pMsg, ...)
{

	char buff[DEF_LOG_LEN];
	va_list argptr;

	va_start(argptr, pMsg);
	vsprintf_s(buff, pMsg, argptr);
	va_end(argptr);

	if(bSucc)
		g_log.log(LOGTP_SUCC, buff);
	else
		g_log.log(LOGTP_ERR, buff);

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

	memset(lpAuto->AutoKey, 0x20, sizeof(lpAuto->AutoKey));
	memcpy(lpAuto->AutoKey, m_sHedgeAcc, m_sHedgeAcc.GetLength());
	m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);
	IR_PrintMsg(TRUE, TRUE, "[%s] 계좌 자동업데이트 설정", m_sHedgeAcc.GetBuffer());

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
	if (nID == SC_CLOSE || nID == SC_MINIMIZE)
		OnDialogShow();
	else
		CDialogEx::OnSysCommand(nID, lParam);
}


void CESApiDllDlgDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CESApiDllDlgDlg::OnBnClickedButtonHedgeReload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//LoadHedgeTarget(TRUE);

}



/*
관리자통보 Q Table polling
*/
VOID CESApiDllDlgDlg::IR_NotedThread()
{
	IR_PrintMsg(TRUE, FALSE, "IR_NotedThread start....");
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
			IR_PrintMsg(FALSE, TRUE, "관리자 작업 통보 조회 오류(%s)", sQ.GetBuffer());
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

			// 새로운 계좌 추가의 경우. WorkVal 에 비번이 들어온다.
			//if (p->nWorkTp == MNG_ACNT_ADD)
			//{
			//	CString sAcnt(p->zAcntNo);
			//	m_hedgeTarget.Add(sAcnt, sAcnt, TRUE);
			//	IR_PrintMsg(TRUE, TRUE, "헷지대상 계좌추가(%s)", p->zAcntNo );
			//	continue;
			//}
			//else if (p->nWorkTp == MNG_ACNT_AUTOORD_SET)
			//{
			//	CString sAcnt(p->zAcntNo);
			//	m_hedgeTarget.Add(sAcnt, sAcnt, TRUE);
			//	IR_PrintMsg(TRUE, TRUE, "헷지대상 계좌추가(%s)", p->zAcntNo);
			//	continue;
			//}
			//else if (p->nWorkTp == MNG_ACNT_AUTOORD_RELEASE)
			//{
			//	CString sAcnt(p->zAcntNo);
			//	m_hedgeTarget.Delete(sAcnt, TRUE);
			//	IR_PrintMsg(TRUE, TRUE, "헷지대상 계좌해제(%s)", p->zAcntNo);
			//	continue;
			//}
		} // while (db->NextRow())

	} // while (m_bContinue)


}

#if 0
// 헷지주문계좌정보 조회
BOOL	CESApiDllDlgDlg::IR_LoadHedgeOrderAcnt()
{
	CString sQ;
	CDBHandler db(m_pDBPool->Get());

	sQ.Format("SELECT ACNT_NO, ACNT_PWD FROM ACNT_INFO WHERE ACNT_TP=9");
	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		IR_PrintMsg(FALSE, TRUE, "헷지주문계좌정보 조회 오류(%s)(%s)", sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}

	while (db->NextRow())
	{
		m_sHedgeAcc.Format("%s", db->Get("ACNT_NO"));
		m_szHedgePass.Format("%s", db->Get("ACNT_PWD"));
		m_sAcntPass.Format("%s", db->Get("ACNT_PWD"));
	}

	IR_PrintMsg(TRUE, TRUE, "헷지주문계좌(%s)", m_sHedgeAcc.GetBuffer());
	return TRUE;
}
#endif