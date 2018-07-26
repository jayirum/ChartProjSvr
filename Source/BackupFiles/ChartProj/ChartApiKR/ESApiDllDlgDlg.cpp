
// ESApiDllDlgDlg.cpp : 구현 파일
//


#include "stdafx.h"
#include "ESApiDllDlg.h"
#include "ESApiDllDlgDlg.h"
#include "afxdialogex.h"
#include "../../IRUM_UTIL/Util.h"
#include <iostream>
#include "../../IRUM_INC/IRUM_Common.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ESM_TEST	WM_USER + 0x9000
#define ID_TIME_LOGON			101	// 최초 로그인 시도하는 타이머

CLogMsg g_log;


#define __APP_VERSION "V3.0_MCAST"

CESApiDllDlgDlg::CESApiDllDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CESApiDllDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szUserID = _T("");
	m_szUserPass = _T("");
	m_szCertPass = _T("");
	
	m_sDllPath = _T("");

	m_pMemPool = NULL;
	m_pDBPool = NULL;
	m_siseSeq = 0;

	m_hSaveThread = NULL;
	m_pMcastSend = NULL;
	m_bInit = TRUE;
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
	ON_BN_CLICKED(IDC_BUTTON_SISE, &CESApiDllDlgDlg::OnBnClickedButtonSise)


	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_COMMAND(WM_APP_EXIT, OnAppExit)
	ON_COMMAND(WM_DIALOG_SHOW, OnDialogShow)
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

	

	m_myTray.m_bHide = m_bHide = false;
	m_myTray.AddTrayIcon(GetSafeHwnd(), "ChartApiKR");

	if (m_Dup.CheckDuplicate("ChartApiKR") == -1)
	{
		AfxMessageBox("<ChartApiKR>이미 프로세스가 실행중입니다.");
		//OnBnClickedButtonExit();
		exit(0);
	}


	m_bContinue = TRUE;

	// config file
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir( szDir, szModule, szConfig);
	m_sDir = szDir;
	m_sModule = szModule;
	m_sConfig = szConfig;

	// LOG FILE
	CUtil::GetConfig(m_sConfig.GetBuffer(),  "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, m_sModule.GetBuffer());
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------------------");
	g_log.log(LOGTP_SUCC, "버전[%s]%s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------------------");

	// API 경로
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DIR", "API", szDir);
	m_sDllPath = szDir;
	g_log.log(LOGTP_SUCC, "API 저장된 폴더 : %s", m_sDllPath.GetBuffer());


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


	if (!IR_DBOpen()) {
		AfxMessageBox("DB OPEN 실패");
		return FALSE;
	}


	//if (!InitSHM()) {
	//	AfxMessageBox("SHM OPEN 실패");
	//	return FALSE;
	//}
		

	if (!IR_InitMemPool()) {
		AfxMessageBox("Memory Pool OPEN 실패");
		return FALSE;
	}


	if (!IR_LoadSiseSymbol()) {
		AfxMessageBox("종목정보 조회 실패");
		return FALSE;
	}

	// server socket init
	char zLocalIP[32], zMcastIP[32], port[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "CHART_SOCKET_INFO", "IP", zLocalIP);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "CHART_SOCKET_INFO", "IP_MCAST", zMcastIP);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "CHART_SOCKET_INFO", "PORT", port);

	m_pMcastSend = new CMCastSend();
	if (!m_pMcastSend->Begin(zLocalIP, zMcastIP, atoi(port)))
	{
		AfxMessageBox("멀티캐스팅 초기화실패");
		return FALSE;
	}

	g_log.log(LOGTP_SUCC, "멀티캐스트송신 초기화 성공(LOCAL:%s)(MCAST:%s)(PORT:%s)", zLocalIP, zMcastIP, port);

	////TODO 
	//m_pServerSock = new CNanoSocket;
	//if (!m_pServerSock->InitServerSock(ip,port))
	//{
	//	IR_PrintMsg(LOGTP_SUCC, TRUE, "%s", m_pServerSock->GetMsg());
	//	AfxMessageBox("Subscribe 용 소켓 Init 실패");
	//	return FALSE;
	//}
	//IR_PrintMsg(LOGTP_SUCC, TRUE, m_pServerSock->GetMsg());

	//TODO
	//m_pServerSock = new CNanoPubSub(TP_PUB);
	//if(!m_pServerSock->BeginTcp(ip,port))
	//{
	//	IR_PrintMsg(LOGTP_SUCC, TRUE, "%s", m_pServerSock->GetMsg());
	//	AfxMessageBox("publish 용 소켓 Init 실패");
	//	return FALSE;
	//}

	// thread pool
	//if (!IR_InitThread()) {
	//	AfxMessageBox("Thread Poos OPEN 실패");
	//	return FALSE;
	//}


	// 현재가 저장 스레드 생성
	m_hSaveThread = (HANDLE)_beginthreadex(NULL, 0, &SavePrcThread, this, 0, &m_unSaveThread);

	// 현재가 전송 스레드 생성
	m_hSendThread = (HANDLE)_beginthreadex(NULL, 0, &SendPrcThread, this, 0, &m_unSendThread);

	// 1초 간격 타이머
	SetTimer(ID_TIME_LOGON, 1000, NULL);


	UpdateData(FALSE);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


BOOL CESApiDllDlgDlg::IR_LoadSiseSymbol()
{
	CDBHandlerAdo db(DBPoolPtr(m_pDBPool)->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC CHART_GET_SYMBOL");

	if (!db->ExecQuery(zQ))
	{
		IR_PrintMsg(LOGTP_ERR, TRUE, "종목정보 오류(%s)(%s)", zQ, db->GetError());
		return FALSE;
	}

	char yearLen[32], euroDollar[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "SYMBOL_TYPE", "YEAR_LENGTH", yearLen);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "SYMBOL_TYPE", "EURO_DOLLAR", euroDollar);


	char zTemp[32], zSymbol[32];
	while (db->IsNextRow())
	{
		db->GetStr("SYMBOL", zTemp);

		ir_cvtcode_uro_6e(zTemp, zSymbol);
				
		// KR 은 CLQ7, 다른곳은 CLQ17 
		if(strcmp(yearLen,"1")==0)
			ir_cvtcode_HD_KR(zSymbol, zTemp);
		
		CString symbol = zTemp;

		db->GetStr("ARTC_CODE", zTemp);
		
		m_mapSymbol[symbol] = CString(zTemp);
		IR_PrintMsg(LOGTP_SUCC, TRUE, "[%s][%s] 차트구성종목", zTemp, symbol.GetBuffer());

		db->Next();
	}
	db->Close();

	return TRUE;

}

BOOL CESApiDllDlgDlg::IR_InitMemPool()
{
	m_pMemPool = new Poco::MemoryPool(MEMPOOL_BLOCK_SIZE, 100, MEMPOOL_MAX_UNIT);

	return TRUE;
}

VOID CESApiDllDlgDlg::IR_CloseMemPool()
{
	delete m_pMemPool;
}



//

VOID CESApiDllDlgDlg::IR_DBClose()
{
	SAFE_DELETE(m_pDBPool);
}

BOOL CESApiDllDlgDlg::IR_CheckDBPool()
{
	if (!m_pDBPool)	return FALSE;

//	if (!m_pDBPool->isActive())
//		return FALSE;

	return (DBPoolPtr(m_pDBPool)->Available() > 0);
}

BOOL CESApiDllDlgDlg::IR_DBOpen()
{
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_ID", id);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_POOL_CNT", cnt);

	m_pDBPool = (VOID*)new CDBPoolAdo(ip, id, pwd, name);
	if (!DBPoolPtr(m_pDBPool)->Init(atoi(cnt)))
	{
		IR_PrintMsg(LOGTP_ERR, TRUE, "DB OPEN FAIL(MSG:%s)", DBPoolPtr(m_pDBPool)->GetMsg());
		IR_PrintMsg(LOGTP_ERR, TRUE, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}
	IR_PrintMsg(LOGTP_SUCC, TRUE, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);

	return TRUE;
}

//


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
		IR_PrintMsg(LOGTP_ERR, TRUE, m_sDllPath.GetBuffer());
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
	m_Api.DisConnectServer(m_hWnd);
}





void CESApiDllDlgDlg::OnBnClickedButtonExit()
{

	PostThreadMessage(m_unSendThread, WM_SISE_DIE, 0, (LPARAM)0);
	PostThreadMessage(m_unSaveThread, WM_SISE_DIE, 0, (LPARAM)0);

	if (m_Api.IsServerConnect(m_hWnd))
		OnBnClickedCancel();

	CDialogEx::OnOK();
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
			IR_PrintMsg(LOGTP_SUCC, TRUE, "REAL 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("REAL 접속");
		}
		else {
			IR_PrintMsg(LOGTP_SUCC, TRUE, "TEST 서버와 연결되었습니다.");
			GetDlgItem(IDC_STATIC_REAL)->SetWindowText("TEST 접속");
		}

		// thread 들을 깨운다.
		//IR_ResumeThreads();

		// 시세 AUTO UPDATE
		OnBnClickedButtonSise();
	}
	else
	{
		LPCTSTR lpstrMsg = (LPCTSTR)lParam;
		if(lpstrMsg != NULL){
			//AfxMessageBox(lpstrMsg);
			IR_PrintMsg(LOGTP_ERR, TRUE, "서버와 연결 실패했습니다(%s)", lpstrMsg);
		}

		PostMessage(WM_ESM_TEST);
	}

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnServerDisConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("서버와 연결이 종료 되었습니다.");
	IR_PrintMsg(LOGTP_SUCC, TRUE, "서버와 연결이 종료 되었습니다.");

	return 0L;
}


LRESULT CESApiDllDlgDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{

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
		IR_PrintMsg(LOGTP_ERR, TRUE, "ERROR!!! (CODE:%d) (MSG:%.50s)",  pErr->Head.ErrCode, sizeof(pErr->ErrMsg), pErr->ErrMsg);
	}


	switch(wParam)
	{
	
	case AUTO_0933:// 종목 시세 실시간
	{
		lpAt0933O_st	lpTrOut = (lpAt0933O_st)lParam;
		//int				nDecLen = GetDecimalLen(lpTrOut->FullCode, sizeof(lpTrOut->FullCode));
		CString sFullCode, sShortCode;
		sFullCode.Format("%.*s", sizeof(lpTrOut->FullCode), lpTrOut->FullCode);
		sShortCode = m_Api.GetShortCode(sFullCode);

		//IR_PrintMsg(LOGTP_SUCC, FALSE, "수신(%s)", sShortCode.GetBuffer());

		// 스레드에게 전달
		
		std::map<CString, CString>::iterator it = m_mapSymbol.find(sShortCode);
		if (it == m_mapSymbol.end())
		{
			IR_PrintMsg(LOGTP_ERR, TRUE, "[%s] 종목은 요청한 종목이 아니다.", sShortCode.GetBuffer());
		}
		else
		{
			At0933O_st_Ex* pData = (At0933O_st_Ex*)m_pMemPool->get();
			memcpy(pData->shortCode, sShortCode.GetBuffer(), LEN_SYMBOL );
			memcpy(&pData->org, lpTrOut, sizeof(At0933O_st));
			pData->seq = ++m_siseSeq;

			//// 시세 전송
			PostThreadMessage(m_unSendThread, WM_SISE_RECV, 0, (LPARAM)pData);

			//// DB 저장을 위해
			At0933O_st_Ex* pData2 = (At0933O_st_Ex*)m_pMemPool->get();
			memcpy(pData2, pData, sizeof(At0933O_st_Ex));
			PostThreadMessage(m_unSaveThread, WM_SISE_RECV, 0, (LPARAM)pData2);

			//IR_PrintMsg(LOGTP_SUCC, TRUE, "MD RECV(%s)\n", sShortCode.GetBuffer());
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

		szPwd.Format("%.*s", sizeof(lpAcct->AccPass), lpAcct->AccPass);

		// add memory
		memset (&lpMst, 0x20, SZ_ESAccntInfo);
		memcpy (&lpMst, lpAcct, SZ_ESAccntInfo);

		m_aryAcct.Add(lpMst);


	}

	if (m_cmbAcct.GetCount() > 0)
		m_cmbAcct.SetCurSel(0);

	//IR_PrintMsg(LOGTP_SUCC, TRUE, "계좌번호리스트 가져옴");

	return 0L;
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
		IR_PrintMsg(LOGTP_SUCC, TRUE, "서버 로그인 시도...");
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
	//TODO m_pServerSock->CloseServerSock();
	SAFE_DELETE(m_pMcastSend);
	IR_CloseMemPool();
	//CloseSHM();
	IR_DBClose();

	SAFE_CLOSEHANDLE(m_hSaveThread);
	CDialogEx::OnClose();
}


void CESApiDllDlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	
}



VOID CESApiDllDlgDlg::IR_PrintMsg(LOGMSG_TP tp, BOOL bShow, char* pMsg, ...)
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
주문 체결 자동업데이트
*/
void CESApiDllDlgDlg::OnClickedButtonAuto()
{
	///////////////////////////////////////////////////////////////////////////////
	return;
	///////////////////////////////////////////////////////////////////////////////

	CString			szTmp;
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
		//m_szAutoAcctKey = GetAccount(ii);
		//memcpy(lpAuto->AutoKey, m_szAutoAcctKey, m_szAutoAcctKey.GetLength());
		//m_Api.SetAutoUpdate(TRUE, TRUE, szBuff);
		//IR_PrintMsg(TRUE, TRUE, "[%s] 계좌 자동업데이트 설정", m_szAutoAcctKey.GetBuffer());
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
		IR_PrintMsg(TRUE, TRUE, "[%s] 계좌 자동업데이트 설정", acnt_no);
	}
#endif
}

// 시세 자동 업데이트
void CESApiDllDlgDlg::OnBnClickedButtonSise()
{

	CString			szTmp;
	char			szBuff[4096];
	lpESAutoKey_st	lpAuto = (lpESAutoKey_st)szBuff;

	
	
	std::map<CString,CString>::iterator it;
	for (it = m_mapSymbol.begin(); it != m_mapSymbol.end(); ++it)
	{
		CString sSymbol = (*it).first;
		CString szCode = m_Api.GetFullCode(sSymbol);

		memset(szBuff, 0x20, sizeof(szBuff));

		szTmp.Format("%010.0f", (double)(unsigned long)m_hWnd);
		memcpy(lpAuto->Head.WinId, szTmp, szTmp.GetLength());

		lpAuto->Head.PCKey[0] = '3';
		memcpy(lpAuto->AutoKey, szCode, szCode.GetLength());

		m_Api.SetAutoUpdate(TRUE, FALSE, szBuff);
		IR_PrintMsg(LOGTP_SUCC, TRUE, "%s 시세 요청 (%s)", sSymbol.GetBuffer(), szCode.GetBuffer());

		Sleep(10);
	}
}


void CESApiDllDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	

	if (nID == SC_CLOSE || nID== SC_MINIMIZE)
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

//BOOL CESApiDllDlgDlg::InitSHM()
//{
//	// CHART SHM
//	char szMaxSymbolCnt[128] = { 0, }, szMaxChartKind[128] = { 0, };
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_INFO", "MAX_SYMBOL_CNT", szMaxSymbolCnt);
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_INFO", "MAX_CHART_KIND", szMaxChartKind);
//
//	INIT_DATA	shmData;
//	shmData.lMaxGroupCnt = atoi(szMaxSymbolCnt) * atoi(szMaxChartKind);	// 500개 
//	shmData.lGroupKeySize = LEN_SHM_GROUP_KEY;				// SYMBOL + 차트종류
//	shmData.lHeaderSize = 0;				//	Header 는 없다.
//	shmData.lStructSize = sizeof(_ST_SHM_CHART_UNIT);
//	shmData.lStructKeySize = LEN_SHM_STRUCT_KEY;
//	shmData.lMaxStructCnt = MAX_CHART_UNIT_CNT;
//
//											//	GET SHM NAME
//	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_INFO", "SHM_NAME", szSHMName);
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_INFO", "MUTEX_NAME", szMutexName);
//
//	//	CREATE SHM
//	int nRet = m_shm[CHART].Create(NULL, &shmData, szSHMName, szMutexName);
//	if (nRet<1)
//	{
//		IR_PrintMsg(FALSE, TRUE, "[SHM CREATE ERROR](%s)(%s)(%s)", szSHMName, szMutexName, m_shm[CHART].GetErr());
//		return FALSE;
//	}
//	if (!m_shm[CHART].SetInitSize(shmData.lMaxGroupCnt
//		, shmData.lGroupKeySize
//		, shmData.lHeaderSize
//		, shmData.lStructSize
//		, shmData.lStructKeySize
//		, shmData.lMaxStructCnt)
//		)
//	{
//		IR_PrintMsg(FALSE, TRUE, "[SHM SetInitSize ERROR]");
//		return FALSE;
//	}
//
//	// LAST CHART NAME SHM
//	// GROUPKEY (종목+CHART TP) 마다 1개의 최종차트 이름이 있다.
//	shmData.lMaxGroupCnt = atoi(szMaxSymbolCnt) * atoi(szMaxChartKind);	// 500개 
//	shmData.lGroupKeySize = LEN_SHM_GROUP_KEY;				// SYMBOL + 차트종류
//	shmData.lHeaderSize = 0;				//	Header 는 없다.
//	shmData.lStructSize = sizeof(ST_SHM_LASTCHART);
//	shmData.lStructKeySize = LEN_SHM_GROUP_KEY;
//	shmData.lMaxStructCnt = 1;
//
//	//	GET SHM NAME
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
//	CUtil::GetConfig(m_sConfig.GetBuffer(), "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);
//
//	//	CREATE SHM
//	nRet = m_shm[LASTNM].Create(NULL, &shmData, szSHMName, szMutexName);
//	if (nRet<1)
//	{
//		IR_PrintMsg(FALSE, TRUE, "[SHM CREATE ERROR](%s)(%s)(%s)", szSHMName, szMutexName, m_shm[LASTNM].GetErr());
//		return FALSE;
//	}
//	if (!m_shm[LASTNM].SetInitSize(shmData.lMaxGroupCnt
//		, shmData.lGroupKeySize
//		, shmData.lHeaderSize
//		, shmData.lStructSize
//		, shmData.lStructKeySize
//		, shmData.lMaxStructCnt)
//		)
//	{
//		IR_PrintMsg(FALSE, TRUE, "[SHM SetInitSize ERROR]");
//		return FALSE;
//	}
//	return TRUE;
//}
//
//VOID CESApiDllDlgDlg::CloseSHM()
//{
//	m_shm[CHART].Close();
//	m_shm[LASTNM].Close();
//}


/*
시세를 ChartMaker 에게 전송
*/
unsigned WINAPI CESApiDllDlgDlg::SendPrcThread(LPVOID lp)
{
	CESApiDllDlgDlg* p = (CESApiDllDlgDlg*)lp;
	At0933O_st_Ex* pSise;
//	char tmp[32];

	int size = sizeof(ST_PACK2CHART);
	char sendMsg[1024];
	while (p->m_bContinue)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_SISE_RECV:
				pSise = (At0933O_st_Ex*)msg.lParam;	
				
				//void *sendMsg = nn_allocmsg(size, 0);
				ST_PACK2CHART* pSend = (ST_PACK2CHART*)sendMsg;
				memset(pSend, 0x20, sizeof(ST_PACK2CHART));
				memcpy(pSend->Date, pSise->org.Date, sizeof(pSend->Date));
				memcpy(pSend->Time, pSise->org.Time, sizeof(pSend->Time));
				memcpy(pSend->ShortCode, pSise->shortCode, MIN(sizeof(pSend->ShortCode),strlen(pSise->shortCode)));
				memcpy(pSend->Close, pSise->org.ClosePrice, MIN(sizeof(pSend->Close),strlen(pSise->org.ClosePrice)));
				memcpy(pSend->CntrQty, pSise->org.ContQty, MIN(sizeof(pSend->CntrQty),strlen(pSise->org.ContQty)));
				memcpy(pSend->DecLen, pSise->org.DecLen, MIN(sizeof(pSend->DecLen),strlen(pSise->org.DecLen)));

				//TODO int nLen = p->m_pServerSock->SendData(sendMsg, size);
				//int nLen = p->m_pServerSock->Publish(sendMsg, size);
				//if (nLen < 0)
				//	g_log.log(LOGTP_ERR, "[SEND ERROR:%s]", p->m_pServerSock->GetMsg());
				//else
				//	g_log.log(LOGTP_SUCC, "[SEND](%.80s)",sendMsg);

				int nLen = p->m_pMcastSend->SendData(sendMsg, size);
				if (nLen < 0) {
					g_log.log(LOGTP_ERR, "[SEND ERROR:%s]", p->m_pMcastSend->GetMsg());
				}
				//else
				//	g_log.log(LOGTP_SUCC, "[SEND]%.*s", size, sendMsg);
				p->m_pMemPool->release((void*)msg.lParam);
				break;

			}
		}
	}
	
	return 0;
	
}

/*
시세를 DB 저장
*/
unsigned WINAPI CESApiDllDlgDlg::SavePrcThread(LPVOID lp)
{
	CESApiDllDlgDlg* p = (CESApiDllDlgDlg*)lp;
#define DBPoolPointer ((CDBPoolAdo*)p->m_pDBPool)

	int nCnt = 0;
	char zQ[1024];
	At0933O_st_Ex* pSise;
	while (p->m_bContinue)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_SISE_RECV:
				pSise = (At0933O_st_Ex*)msg.lParam;
				CDBHandlerAdo db(DBPoolPointer->Get());
				sprintf(zQ, "EXEC CHART_SAVE_NOWPRC '%.*s', %10.5f, %d, '%c' ",
					sizeof(pSise->shortCode), pSise->shortCode,
					S2D(pSise->org.ClosePrice, sizeof(pSise->org.ClosePrice)),
					S2N(pSise->org.ContQty, sizeof(pSise->org.ContQty)),
					(pSise->org.MatchKind[0]=='+')? '1':'2'
				);

				if (!db->ExecQuery(zQ))
				{
					p->IR_PrintMsg(LOGTP_ERR, TRUE, "시세정보 저장 오류(%s)(%s)", zQ, db->GetError());
				}
				p->m_pMemPool->release((void*)msg.lParam);

				if (++nCnt == 10)
				{
					nCnt = 0;
					Sleep(10);	// DB 부하를 생각해서
				}
				break;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}

//BOOL CESApiDllDlgDlg::IR_InitThread()
//{
//	std::map<CString, ST_CHART_THREAD*>::iterator it;
//
//	for (it = m_mapSymbol.begin(); it != m_mapSymbol.end(); it++)
//	{
//		CString symbol = (*it).first;
//		ST_CHART_THREAD* p = (*it).second;
//		p->pChart = new CChartThread(symbol, p->zArtcCode, m_pMemPool, m_pServerSock);
//		m_mapSymbol[symbol] = p;
//	}
//
//	return TRUE;
//}
//
//
//VOID CESApiDllDlgDlg::IR_CloseThread()
//{
//	std::map<CString, ST_CHART_THREAD*>::iterator it;
//
//	for (it = m_mapSymbol.begin(); it != m_mapSymbol.end(); it++)
//	{
//		ST_CHART_THREAD* p = (*it).second;
//		PostThreadMessage(p->pChart->GetMyThreadID(), WM_CHART_ALL_KILL, 0, 0);
//		delete p->pChart;
//		delete p;
//	}
//	m_mapSymbol.clear();
//}


//
//VOID CESApiDllDlgDlg::IR_ResumeThreads()
//{
//	std::map<CString, ST_CHART_THREAD*>::iterator it;
//
//	for (it = m_mapSymbol.begin(); it != m_mapSymbol.end(); it++)
//	{
//		ST_CHART_THREAD* p = (*it).second;
//		p->pChart->ResumeThread();
//	}
//}