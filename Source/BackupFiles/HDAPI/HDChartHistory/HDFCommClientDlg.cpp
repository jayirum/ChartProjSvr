
// HDFCommClientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "HDFCommClient.h"
#include "HDFCommClientDlg.h"
#include "afxdialogex.h"

#include "../../IRUM_INC/IRUM_Common.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/IRExcept.h"
#include "../../ChartProj/Common/ChartCommon.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 계좌
const CString DEF_ACCT_INFO = "g11004.AQ0101%";

// 국내
const CString DEF_TR_CODE = "g11002.DQ0622&";
const CString DEF_FID_CODE = "s20001";
const CString DEF_ORD_CODE_NEW = "g12001.DO1601&";
const CString DEF_ORD_CODE_MOD = "g12001.DO1901&";
const CString DEF_ORD_CODE_CNL = "g12001.DO1701&";

const CString DEF_ORD_CODE_NEW_CME = "g12001.DO2201&";	//CME 신규주문
const CString DEF_ORD_CODE_CNL_CME = "g12001.DO2001&";	//CME 취소
const CString DEF_ORD_CODE_MOD_CME = "g12001.DO2101&";	//CME 정정(가격,조건)주문

const CString DEF_CHE_LIST = "g11002.DQ0107&";
const CString DEF_MICHE_LIST = "g11002.DQ0104&";

// 해외
const CString DEF_HW_JANGO    = "g11004.AQ0605%";
const CString DEF_HW_FID_CODE = "o51000";
const CString DEF_HW_ORD_CODE_NEW = "g12003.AO0401%";
const CString DEF_HW_ORD_CODE_MOD = "g12003.AO0402%";
const CString DEF_HW_ORD_CODE_CNL = "g12003.AO0403%";
const CString DEF_HW_MSTINFO  = "o51211";

// FX마진	//@lhe 2012.06.22
const CString DEF_FX_JANGO    = "g11004.AQ0901%";		// 자산내역
const CString DEF_FX_FID_CODE = "x00001";				// FX마스터
const CString DEF_FX_ORD_CODE_NEW = "g12003.AO0501%";	// 예약,시장가,청산주문
const CString DEF_FX_ORD_CODE_MOD = "g12003.AO0502%";	// 정정
const CString DEF_FX_ORD_CODE_CNL = "g12003.AO0503%";	// 취소주문



//////////////////////////////////////////////////////////////
//jay

#define ID_TIME_LOGON			101	// 최초 로그인 시도하는 타이머

CLogMsg g_log;

int g_nChartFirstReqID;
CString g_strNextKey;
#define DBPoolPtr ((CDBPoolAdo*)m_pDBPool)
//////////////////////////////////////////////////////////////



///////////////////////////
// CHART DATA (o44005)
typedef struct _CHART_44005
{
	char keyvalue[18];
	char symbol[32];
	char dt[8];
	char tm[10];
	char next[1];	// 0:최초, 1:다음
	char dataGb[1];	// 	2:분, 3 : 일, 4 : 주, 5 : 월, 6 : 틱
	char tmGb[3];
	char dataCnt[5];
	char mktGb[1];	// 1:전산장, 2:본장
	char realGb[1];	// 허봉실봉구분	1: 실봉만, 0 : 허봉 + 실봉

}CHART_44005;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CHDFCommClientDlg 대화 상자
CHDFCommClientDlg::CHDFCommClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHDFCommClientDlg::IDD, pParent)
	, m_sUserId(_T(""))
	, m_sUserPw(_T(""))
	, m_sAuthPw(_T(""))
	, m_sSymbol(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bInit = FALSE;
	
}

void CHDFCommClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_OUTPUT, m_edOutput);
	DDX_Control(pDX, IDC_TRCODE, m_edTrCode);
	DDX_Control(pDX, IDC_INPUTTEXT, m_edInput);
	DDX_Control(pDX, IDC_JONGMOK, m_edJomgmok);
	DDX_Control(pDX, IDC_GUBUN, m_edGubun);
	DDX_Control(pDX, IDC_EDIT_ID, m_editUserID);
	DDX_Control(pDX, IDC_EDIT_PWD, m_editPwd);
	DDX_Control(pDX, IDC_EDIT_AUTH_PWD, m_editAuthPwd);
	DDX_Text(pDX, IDC_EDIT_ID, m_sUserId);
	DDX_Text(pDX, IDC_EDIT_PWD, m_sUserPw);
	DDX_Text(pDX, IDC_EDIT_AUTH_PWD, m_sAuthPw);
	DDX_Control(pDX, IDC_TRCODE2, m_edTrCode2);
	DDX_Control(pDX, IDC_INPUTTEXT2, m_edInput2);
	DDX_Control(pDX, IDC_INPUTTEXT3, m_edInput3);
	DDX_Control(pDX, IDC_ACC_NO, m_edAccNo);
	DDX_Control(pDX, IDC_JUMUN_INPUT, m_edJumunInput);
	DDX_Control(pDX, IDC_JUMUN_TR, m_edJumunTR);
	DDX_Control(pDX, IDC_LIST_OUTMSG, m_ListOutMsg);

	//DDX_Control(pDX, IDC_EDIT1, m_edSeriesO);
	//DDX_Control(pDX, IDC_EDIT2, m_edTimeO  );
	//DDX_Control(pDX, IDC_EDIT3, m_edClosePO);
	//DDX_Control(pDX, IDC_EDIT4, m_edVolumeO);

	DDX_Control(pDX, IDC_GETINFO_INPUT, m_edGetInfo);
	DDX_Text(pDX, IDC_EDIT_SYMBOL, m_sSymbol);
}

BEGIN_MESSAGE_MAP(CHDFCommClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_LOGIN, &CHDFCommClientDlg::IR_OnBnClickedLogin)
	ON_BN_CLICKED(ID_LOGOUT, &CHDFCommClientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_RQTEST, &CHDFCommClientDlg::OnBnClickedRqtest)
	ON_BN_CLICKED(IDC_SBTEST, &CHDFCommClientDlg::OnBnClickedSbtest)
	ON_BN_CLICKED(IDC_SBREMOVE, &CHDFCommClientDlg::OnBnClickedSbremove)
	ON_BN_CLICKED(IDC_INITCOMM, &CHDFCommClientDlg::IR_OnBnClickedInitcomm)
	ON_BN_CLICKED(IDC_TERMINATE, &CHDFCommClientDlg::OnBnClickedTerminate)
	ON_BN_CLICKED(IDC_RADIO1, &CHDFCommClientDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CHDFCommClientDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_JUMUNCHESUMMIT, &CHDFCommClientDlg::IR_OnBnClickedJumunchesummit)
	ON_BN_CLICKED(IDC_JUMUNCHECANCLE, &CHDFCommClientDlg::OnBnClickedJumunchecancle)
	ON_BN_CLICKED(IDC_JUMUN_CHE, &CHDFCommClientDlg::OnBnClickedJumunChe)
	ON_BN_CLICKED(IDC_RADIO3, &CHDFCommClientDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CHDFCommClientDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CHDFCommClientDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_BUTTON1, &CHDFCommClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUSINESSDAY, &CHDFCommClientDlg::OnBnClickedBusinessday)
	ON_BN_CLICKED(IDC_FRBUSINESSDAY, &CHDFCommClientDlg::OnBnClickedFrbusinessday)
	ON_BN_CLICKED(IDC_CommGetInfo, &CHDFCommClientDlg::OnBnClickedCommgetinfo)
	ON_BN_CLICKED(IDC_BUTTON3, &CHDFCommClientDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CHDFCommClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_SAVELOG, &CHDFCommClientDlg::OnBnClickedBtnSavelog)
	ON_BN_CLICKED(IDC_BTN_LOGCLEAR, &CHDFCommClientDlg::OnBnClickedBtnLogclear)
	ON_BN_CLICKED(IDC_ORD_APPLY, &CHDFCommClientDlg::OnBnClickedOrdApply)
	ON_CBN_SELCHANGE(IDC_ORD_COMBO_ORDERTR, &CHDFCommClientDlg::OnCbnSelchangeOrdComboOrdertr)
	ON_BN_CLICKED(IDC_BUTTON_TERMINATE, &CHDFCommClientDlg::OnBnClickedButtonTerminate)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BUTTON_CHART, &CHDFCommClientDlg::OnBnClickedButtonChart)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CHDFCommClientDlg, CDialogEx)
	ON_EVENT(CHDFCommClientDlg, -1, 3, OnDataRecv, VTS_BSTR VTS_I4)
	ON_EVENT(CHDFCommClientDlg, -1, 4, OnGetBroadData, VTS_BSTR VTS_I4)
	ON_EVENT(CHDFCommClientDlg, -1, 5, OnGetMsg, VTS_BSTR VTS_BSTR)
	ON_EVENT(CHDFCommClientDlg, -1, 6, OnGetMsgWithRqId, VTS_I4 VTS_BSTR VTS_BSTR)
	
END_EVENTSINK_MAP()

// CHDFCommClientDlg 메시지 처리기

BOOL CHDFCommClientDlg::OnInitDialog()
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 이미 실행중인지 확인
	if (m_Dup.CheckDuplicate("HDChartHistory") == -1)
	{
		AfxMessageBox("<HDChartHistory>이미 프로세스가 실행중입니다.");
		//OnBnClickedButtonTerminate();
		exit(0);
	}
			
	InitializeCriticalSectionAndSpinCount(&m_csAPI, 2000);


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
	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ID", szDir);
	m_sUserId = szDir;

	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "USER_PWD", szDir);
	m_sUserPw = szDir;

	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "CERT_PWD", szDir);
	m_sAuthPw = szDir;

	m_editUserID.SetWindowText(m_sUserId);
	m_editPwd.SetWindowText(m_sUserPw);
	m_editAuthPwd.SetWindowText(m_sAuthPw);


	//CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "ACNT_PWD", szDir);
	//m_szHedgePass = szDir;

	// 접속서버
	CUtil::GetConfig(m_sConfig.GetBuffer(), "APIINFO", "REAL_YN", szDir);
	m_cRealYN = szDir[0];

	if (!IR_DBOpen()) {
		AfxMessageBox("DB OPEN 실패");
		return FALSE;
	}

	m_bContinue = TRUE;

	// 1초 간격 타이머
	SetTimer(ID_TIME_LOGON, 500, NULL);

	m_bInit = TRUE;
	//m_CommAgent.Create("HDF CommAgent", WS_CHILD, CRect(0,0,0,0), this, 1);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);

	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(0);		//@lhe 2012.06.22
	
	// 일반 조회
	OnBnClickedRadio3();


	m_strOrdNo = "";

	m_strNextKey = "";

	m_edTrCode2.EnableWindow(FALSE);
	m_edInput2.EnableWindow(FALSE);

	m_edGetInfo.SetWindowText("시스템구분");

	////
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_BYSLTP);
	pCombo->ResetContent();
	pCombo->AddString("1.매수");
	pCombo->AddString("2.매도");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_PRCETP);
	pCombo->ResetContent();
	pCombo->AddString("1.지정가");
	pCombo->AddString("2.시장가");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_TRDCOND);
	pCombo->ResetContent();
	pCombo->AddString("1.FAS");
	pCombo->AddString("2.FOK");
	pCombo->AddString("3.FAK");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_STRTP);
	pCombo->ResetContent();
	pCombo->AddString("1.일반");
	pCombo->AddString("2.STOP");
	pCombo->AddString("3.OCO");
	pCombo->AddString("4.가격예약");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	pCombo->ResetContent();
	pCombo->AddString("신규");
	pCombo->AddString("정정");
	pCombo->AddString("취소");
	pCombo->AddString("CME신규");
	pCombo->AddString("CME정정");
	pCombo->AddString("CME취소");
	pCombo->SetCurSel(0);

	ShowHWOrdCtrl(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//
//void CHDFCommClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CDialogEx::OnSysCommand(nID, lParam);
//	}
//}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHDFCommClientDlg::OnPaint()
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
HCURSOR CHDFCommClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHDFCommClientDlg::OnClose()
{
	CDialog::OnClose();
}

void CHDFCommClientDlg::IR_OnBnClickedInitcomm()
{
	char msg[1024];

	// 통신을 초기화 한다.
	if ( !m_CommAgent.GetSafeHwnd() )
	{
		m_CommAgent.Create("HDF CommAgent", WS_CHILD, CRect(0,0,0,0), this, 2286);
	}

	if ( m_CommAgent.GetSafeHwnd() )
	{
		int nRet = m_CommAgent.CommInit(1);
		
		if ( nRet < 0 )
		{
			sprintf(msg, "통신프로그램 실행 오류:%d", nRet);
			IR_PrintMsg(LOGTP_FATAL, TRUE, msg);
			throw CIRExcept(ERR_COMM_INIT);
		}
		else
		{
			sprintf(msg, "통신프로그램 실행 성공:%d", nRet);
			IR_PrintMsg(LOGTP_SUCC, TRUE, msg);
			
		}
	}
}

void CHDFCommClientDlg::OnBnClickedTerminate()
{
	// 종료시 통신을 해제한다.
	if( m_CommAgent.GetSafeHwnd() )
	{
		if ( m_CommAgent.CommGetConnectState() == 1 )
		{
			if ( m_sUserId != "" )
				m_CommAgent.CommLogout(m_sUserId);
		}

		m_CommAgent.CommTerminate(TRUE);
		m_CommAgent.DestroyWindow();		
	}

	IR_PrintMsg(LOGTP_SUCC, TRUE, "통신프로그램 종료 성공");
}

void CHDFCommClientDlg::OnDestroy()
{
	CDialog::OnDestroy();
}


void CHDFCommClientDlg::IR_OnBnClickedLogin()
{
	// 로그인한다.
	m_editUserID.GetWindowText(m_sUserId);
	m_editPwd.GetWindowText(m_sUserPw);
	m_editAuthPwd.GetWindowText(m_sAuthPw);

	int nRet;
	try
	{
		// 통신연결
		IR_OnBnClickedInitcomm();

		//if (m_sAuthPw.GetLength() == 0)
		//	m_sAuthPw = " ";
		nRet = m_CommAgent.CommLogin(m_sUserId, m_sUserPw, m_sAuthPw);
		if (nRet > 0)
		{
			IR_PrintMsg(LOGTP_SUCC, TRUE, "로그인 성공(%s)(%s)(%s)"
				, m_sUserId.GetBuffer(), m_sUserPw.GetBuffer(), m_sAuthPw.GetBuffer());
			//WriteLog("로그인 성공");

			Sleep(500);


		}
		else
		{
			IR_PrintMsg(LOGTP_FATAL, TRUE, "로그인 실패[%d](%s)(%s)(%s)"
				, nRet, m_sUserId.GetBuffer(), m_sUserPw.GetBuffer(), m_sAuthPw.GetBuffer());
		}
	}
	catch (CIRExcept& ex)
	{
		IR_PrintMsg(LOGTP_FATAL, TRUE, ex.GetMsg());
	}
}

void CHDFCommClientDlg::OnBnClickedLogout()
{

	// 로그아웃한다.
	int nRet = m_CommAgent.CommLogout(m_sUserId);

	CString strRet;
	if ( nRet< 0 )
	{
		strRet.Format("로그아웃 실패[%d]", nRet);
		IR_PrintMsg(LOGTP_ERR,TRUE, strRet.GetBuffer());
	}
	else
	{
		strRet.Format("로그아웃 성공[%d]", nRet);
		IR_PrintMsg(LOGTP_SUCC,TRUE, strRet.GetBuffer());
	}

	// 통신종료
	OnBnClickedTerminate();
}

// TR통신
void CHDFCommClientDlg::OnBnClickedRadio1()
{
	m_edTrCode.EnableWindow(TRUE);
	m_edInput.EnableWindow(TRUE);
	m_edTrCode2.EnableWindow(FALSE);
	m_edInput2.EnableWindow(FALSE);
	m_edInput3.EnableWindow(FALSE);
}

// FID통신
void CHDFCommClientDlg::OnBnClickedRadio2()
{
	m_edTrCode.EnableWindow(FALSE);
	m_edInput.EnableWindow(FALSE);
	m_edTrCode2.EnableWindow(TRUE);
	m_edInput2.EnableWindow(TRUE);
	m_edInput3.EnableWindow(TRUE);
}

// 조회 관련
void CHDFCommClientDlg::OnBnClickedRqtest()
{
	int nResult;
	CString sTrCode, sInput;
	CString sFidCode, sFidInput;

	CString strMsg;
	// 조회성 데이터를 요청한다.
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		m_edTrCode.GetWindowText(sTrCode);
		m_edInput.GetWindowText(sInput);
		m_sCode = sTrCode;

		nResult = m_CommAgent.CommGetConnectState();
		if ( nResult == 1 )
		{
			if ( sTrCode == "g11002.DQ0104&" || sTrCode == "g11002.DQ0107&" )
			{
				CString strAcctNo(sInput, 11);
				CString strDealNo = m_CommAgent.CommGetDealNo(strAcctNo);
				CString strAcctPw = sInput.Mid(strAcctNo.GetLength() + strDealNo.GetLength(), 8);
				
				sInput = strAcctNo + strDealNo + strAcctPw;
			}
			nResult = m_CommAgent.CommRqData(sTrCode, sInput, sInput.GetLength(), m_strNextKey);
			m_strNextKey = "";
		}
	}
	else
	{
		CString sReqFidList;
		m_edTrCode2.GetWindowText(sFidCode);
		m_edInput2.GetWindowText(sFidInput);
		m_edInput3.GetWindowText(sReqFidList);
		m_sCode = sFidCode;

		nResult = m_CommAgent.CommFIDRqData(sFidCode, sFidInput, sReqFidList, sFidInput.GetLength(), m_strNextKey);
		m_strNextKey = "";
	}

	strMsg.Format("요청한 RQID[%d]", nResult );
	//WriteLog(strMsg);
}

// 실시간 등록
// 국내 : 51:선물호가, 51:옵션호가, 65:선물체결, 66:옵션체결
// 해외 : 76:선물호가, 82:선물체결

void CHDFCommClientDlg::OnBnClickedSbtest()
{
	int nResult;
	CString sJongmok, sGubun;

	m_edJomgmok.GetWindowText(sJongmok);
	m_edGubun.GetWindowText(sGubun);

	int nGubun = atol(sGubun);

	// 리얼(실시간) 데이타를 요청한다.
	nResult = m_CommAgent.CommSetBroad((LPCTSTR)sJongmok, nGubun);

	CString strMsg;
	strMsg.Format("실시간 종목[%s] 구분[%s] 등록결과[%d]", sJongmok, sGubun, nResult);
	//WriteLog(strMsg);
}

void CHDFCommClientDlg::OnBnClickedSbremove()
{
	CString sJongmok, sGubun;

	m_edJomgmok.GetWindowText(sJongmok);
	m_edGubun.GetWindowText(sGubun);

	int nGubun = atol(sGubun);

	// 실시간 데이터 수신을 해제한다.
	m_CommAgent.CommRemoveBroad(sJongmok, nGubun);
	// 모든 실시간 데이터 수신을 해제는 종목을 "" 로
	//m_CommAgent.CommRemoveBroad("", nGubun);
}

// 주문체결 등록
/*
0196	해외주문접수	AO01
CommSetJumunChe 1회 호출로 해당 실시간 전체가 등록

CommRemoveJumunChe 1회 호출로 해당 실시간 전체 해지
0186	해외주문미체결	RFO1
0187	해외주문미결제	RFO2
0188	해외주문잔고	RFO3
0189	해외주문체결	RFO4
0190	해외주문순미결제	RFO5
0296	해외주문접수	AO11
0286	해외주문미체결	RF11
0289	해외주문체결	RF14

*/
void CHDFCommClientDlg::IR_OnBnClickedJumunchesummit()
{
	// 실시간 체결정보수신 등록
	//int nCnt = m_hedgeTarget.GetCount();
	//if (nCnt > 0) {
	//	CString *pAcnt = new CString[nCnt];
	//	m_hedgeTarget.GetAllData(pAcnt);
	//	for (int i = 0; i < nCnt; i++)
	//	{
	//		m_CommAgent.CommSetJumunChe(IR_ID(), pAcnt->GetBuffer());
	//		IR_PrintMsg(LOGTP_SUCC, TRUE, "[%s] 계좌 자동업데이트 설정", pAcnt[i].GetBuffer());
	//	}
	//	delete[] pAcnt;
	//}

	//등록
	//CString sID, sAccNo;
	//m_editUserID.GetWindowText(sID);
	//m_edAccNo.GetWindowText(sAccNo);
	//m_CommAgent.CommSetJumunChe(sID, sAccNo);

	//WriteLog("실시간 주문체결/미체결 등록");
}

// 주문체결 해지
void CHDFCommClientDlg::OnBnClickedJumunchecancle()
{
	//해제
	CString sID, sAccNo;
	m_editUserID.GetWindowText(sID);
	m_edAccNo.GetWindowText(sAccNo);
	m_CommAgent.CommRemoveJumunChe(sID, sAccNo);

	//WriteLog("실시간 주문체결/미체결 해지");
}


const char *chAccList[] = {"02159001", "02177001", "03288001", "03391001", "03421001",
	"03514001", "03579001", "03735001", "00077001", "00729015",
	//"00017012", "00274010", "00276012", "00558015", "03454001",
};
void CHDFCommClientDlg::OnBnClickedJumunChe()
{
	int nResult;
	CString sTrCode, sInput;

	/***
	for (int nLoopCnt=0; nLoopCnt<1; nLoopCnt++)
	{
		int nCnt = sizeof(chAccList)/sizeof(chAccList[0]);
		for(int i=0; i<nCnt; i++)
		{
			CString strOrderBuff = "   1234    201JB245                        111110           1";
			strOrderBuff = chAccList[i] + strOrderBuff;
			m_edJumunInput.SetWindowTextA(strOrderBuff);

			m_edJumunTR.GetWindowText(sTrCode);
			m_edJumunInput.GetWindowText(sInput);
			m_sCode = sTrCode;

			nResult = m_CommAgent.CommJumunSvr(sTrCode, sInput);
			CString strMsg;
			strMsg.Format("주문요청 요청번호[%d]", nResult);
			//WriteLog(strMsg);
		}
	}
	***/

	/***/
	m_edJumunTR.GetWindowText(sTrCode);
	m_edJumunInput.GetWindowText(sInput);
	m_sCode = sTrCode;

	nResult = m_CommAgent.CommJumunSvr(sTrCode, sInput);
	CString strMsg;
	strMsg.Format("주문요청 요청번호[%d]", nResult);
	//WriteLog(strMsg);
	/***/
}



//조회TR 수신
void CHDFCommClientDlg::OnDataRecv(CString sTrCode, LONG nRqID)
{
	CString strMsg;

	// 계좌정보 수신 - 20140328 sivas. JAY
	if (sTrCode == DEF_ACCT_INFO)
	{
		//CString strMsg = "계좌정보 수신완료!!!";
		//WriteLog(strMsg);

		// 계좌 구분 추가. - 20140331 sivas
		typedef	struct
		{
			char 	szAcctNo[11];		// 계좌번호
			char	szAcctNm[30];		// 계좌명
			char	szAcctGb[01];		// 계좌구분  '1': 해외, '2': FX, '9':국내
		}HDF_ACCOUNT_UNIT;

		typedef struct
		{
			char szCount[5];
			HDF_ACCOUNT_UNIT *pHdfAccUnit;
		}HDF_ACCOUNT_INFO;

		HDF_ACCOUNT_INFO *pHdfAccInfo = NULL;
		HDF_ACCOUNT_UNIT *pHdfAccUnit = NULL;
		CString strRcvBuff = m_CommAgent.CommGetAccInfo();

		pHdfAccInfo = (HDF_ACCOUNT_INFO *)strRcvBuff.GetBuffer();
		CString strCount(pHdfAccInfo->szCount, sizeof(pHdfAccInfo->szCount));
		for (int i = 0; i<atoi(strCount); i++)
		{
			pHdfAccUnit = (HDF_ACCOUNT_UNIT *)(pHdfAccInfo->szCount + sizeof(pHdfAccInfo->szCount) + (sizeof(HDF_ACCOUNT_UNIT) * i));
			CString strAcctNo(pHdfAccUnit->szAcctNo, sizeof(pHdfAccUnit->szAcctNo));
			CString strAcctNm(pHdfAccUnit->szAcctNm, sizeof(pHdfAccUnit->szAcctNm));
			CString strAcctGb(pHdfAccUnit->szAcctGb, sizeof(pHdfAccUnit->szAcctGb));// 계좌 구분 추가. - 20140331 sivas

			//strMsg.Format("[%s][%s][%s]", strAcctNo, strAcctNm, strAcctGb);
			//IR_PrintMsg(LOGTP_SUCC, TRUE, strMsg.GetBuffer());

			////계좌정보저장
			//if (IR_SaveAcntInfo(strAcctNo, strAcctNm))
			//	IR_PrintMsg(LOGTP_SUCC, TRUE, "계좌번호 수신/저장 성공(%s)", strAcctNo.GetBuffer(), strAcctNm.GetBuffer());
			
		}

		// 헷지대상정보 load
		//IR_LoadHedgeTarget(TRUE);
		//SetTimer(ID_TIME_REG_CNTR_1, 5000, NULL);
		//SetTimer(ID_TIME_REG_CNTR_2, 10000, NULL);
		//SetTimer(ID_TIME_REG_CNTR_3, 15000, NULL);
	
	} // if (sTrCode == DEF_ACCT_INFO). 계좌정보 수신 완료



	///////////////////////////////////////////////////////////////////////////////////////////
	// 국내
	///////////////////////////////////////////////////////////////////////////////////////////
	if ( sTrCode == DEF_TR_CODE )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		if ( nRepeatCnt > 0 )
		{
			for (int i= 0; i<nRepeatCnt; i++ )
			{
				CString sData = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "종목코드");

				//WriteLog(sData);
			}

			// CommGetDataDirect 예제 추가. - 20140416 sivas
			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * 32, 0, "A");
			//WriteLog(strBuff);
		}
	}
	else if ( sTrCode == DEF_ORD_CODE_NEW )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString strExchTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "접수구분");
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리구분");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
		CString strOrdNo  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "주문번호");
		CString strSeries = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목코드");

		CString strMsg;
		strMsg.Format("주문응답 번호[%d][%s]처리[%s]계좌번호[%s]주문번호[%s]", nRqID, strExchTp, strProcTp, strAcctNo, strOrdNo);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_FID_CODE )
	{
		// FID값 parsing
		//종목코드(8)+호가시간(6)+종가(9)+누적거래량(7)
		/*
		CString strSeries = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "단축코드");
		CString strTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "호가수신시간");
		CString strCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "현재가");
		CString strVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "누적거래량");

		CString strLastP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최종결제가");
		CString strDate = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장일");
		CString strHighP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "고가");

		m_edSeriesO.SetWindowText(strSeries);
		m_edTimeO.SetWindowText(strTime  );
		m_edClosePO.SetWindowText(strCloseP);
		m_edVolumeO.SetWindowText(strVolume);

		CString strMsg;
		strMsg.Format("RQID[%d] FID 종목[%s]시간[%s]현재가[%s]거래량[%s]", nRqID, strSeries, strTime, strCloseP, strVolume);
		//WriteLog(strMsg);
		*/

		CString	strData000	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "단축코드");
		CString	strData001	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목코드");
		CString	strData002	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "한글종목명");
		CString	strData003	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목한글약명");
		CString	strData004	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "영문종목명");
		CString	strData005	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "축약종목명");
		CString	strData006	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장일");
		CString	strData007	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장일수");
		CString	strData008	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "잔존일수");
		CString	strData009	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최종거래일");
		CString	strData010	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최종거래일구분");
		CString	strData011	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "배당액지수미래가치");
		CString	strData012	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "기준가격구분");
		CString	strData013	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "기준가격");
		CString	strData014	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "이론가");
		CString	strData015	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "이론가");
		CString	strData016	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "CD금리");
		CString	strData017	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상한가");
		CString	strData018	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "하한가");
		CString	strData019	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일정산가구분");
		CString	strData020	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일정산가");
		CString	strData021	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일종가구분");
		CString	strData022	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일종가");
		CString	strData023	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일체결수량");
		CString	strData024	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일근월물체결수량");
		CString	strData025	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일원월물체결수량");
		CString	strData026	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일체결대금");
		CString	strData027	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일근월물거래대금");
		CString	strData028	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일원월물거래대금");
		CString	strData029	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일미결제약정수량　");
		CString	strData030	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장중최고가_SIGN포함");
		CString	strData031	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장중최고가일");
		CString	strData032	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장중최고가비율");
		CString	strData033	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장중최저가_SIGN포함");
		CString	strData034	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장중최저가일");
		CString	strData035	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "상장중최저가비율");
		CString	strData036	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "입회일자");
		CString	strData037	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "시장가허용구분");
		CString	strData038	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "조건부지정가허용구분");
		CString	strData039	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최유리지정가허용구분");
		CString	strData040	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최종결제가");
		CString	strData041	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최종결제가구분");
		CString	strData042	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "CB적용상한가");
		CString	strData043	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "CB적용하한가");
		CString	strData044	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일최종매매체결시각");
		CString	strData045	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일스프레드근월물종가");
		CString	strData046	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일스프레드원월물종가");
		CString	strData047	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "스프레드근월물표준코드");
		CString	strData048	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "스프레드원월물표준코드");
		CString	strData049	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "거래단위");
		CString	strData050	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "체결시간");
		CString	strData051	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "현재가");
		CString	strData052	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "시가");
		CString	strData053	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "고가");
		CString	strData054	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "저가");
		CString	strData055	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일대비구분");
		CString	strData056	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일대비　");
		CString	strData057	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "누적거래량");
		CString	strData058	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "누적거래대금");
		CString	strData059	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일거래량대비");
		CString	strData060	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "등락률");
		CString	strData061	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "건별체결수량");
		CString	strData062	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "체결구분");
		CString	strData063	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도누적거래량");
		CString	strData064	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수누적거래량");
		CString	strData065	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도누적체결건수");
		CString	strData066	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수누적체결건수");
		CString	strData067	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "근월물의제약정가격");
		CString	strData068	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "원월물의제약정가격");
		CString	strData069	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "근월물체결수량");
		CString	strData070	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "원월물체결수량");
		CString	strData071	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "근월물거래대금");
		CString	strData072	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "원월물거래대금");
		CString	strData073	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도체결수량_호가기준");
		CString	strData074	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수체결수량_호가기준");
		CString	strData075	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "호가수신시간");
		CString	strData076	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가1");
		CString	strData077	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가1");
		CString	strData078	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가수량1");
		CString	strData079	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가수량1");
		CString	strData080	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가건수1");
		CString	strData081	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가건수1");
		CString	strData082	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가2");
		CString	strData083	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가2");
		CString	strData084	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가수량2");
		CString	strData085	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가수량2");
		CString	strData086	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가건수2");
		CString	strData087	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가건수2");
		CString	strData088	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가3");
		CString	strData089	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가3");
		CString	strData090	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가수량3");
		CString	strData091	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가수량3");
		CString	strData092	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가건수3");
		CString	strData093	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가건수3");
		CString	strData094	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가4");
		CString	strData095	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가4");
		CString	strData096	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가수량4");
		CString	strData097	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가수량4");
		CString	strData098	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가건수4");
		CString	strData099	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가건수4");
		CString	strData100	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가5");
		CString	strData101	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가5");
		CString	strData102	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가수량5");
		CString	strData103	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가수량5");
		CString	strData104	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가건수5");
		CString	strData105	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가건수5");
		CString	strData106	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가총수량");
		CString	strData107	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가총수량");
		CString	strData108	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가총건수");
		CString	strData109	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가총건수");
		CString	strData110	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "장운용구분");
		CString	strData111	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "체결일자");
		CString	strData112	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "미결제약정구분");
		CString	strData113	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "정산가격");
		CString	strData114	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "정산가구분");
		CString	strData115	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "KOSPI200지수");
		CString	strData116	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "이론가");
		CString	strData117	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "괴리율");
		CString	strData118	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "시장BASIS");
		CString	strData119	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "이론BASIS");
		CString	strData120	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "미결제약정수량");
		CString	strData121	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "미결제약정시가");
		CString	strData122	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "미결제약정고가");
		CString	strData123	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "미결제약정저가");
		CString	strData124	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "미결제증감수량");
		CString	strData125	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "2차저항");
		CString	strData126	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "1차저항");
		CString	strData127	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "피봇값");
		CString	strData128	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "1차지지");
		CString	strData129	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "2차지지");
		CString	strData130	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목SEQ번호");
		CString	strData131	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "옵션종류");
		CString	strData132	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "거래대상종목");
		CString	strData133	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "만기년월");
		CString	strData134	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "행사가격");
		CString	strData135	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "조회순번");
		CString	strData136	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "권리행사유형구분");
		CString	strData137	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최근월물구분");
		CString	strData138	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ATM구분");
		CString	strData139	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "최초거래일");
		CString	strData140	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "신규구분");
		CString	strData141	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "신규일자");
		CString	strData142	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일시가");
		CString	strData143	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일고가");
		CString	strData144	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일저가");
		CString	strData145	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매매증거금기준가");
		CString	strData146	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매매증거금기준가구분");
		CString	strData147	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "정상호가범위(상한치)");
		CString	strData148	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "정상호가범위(하한치)");
		CString	strData149	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "가격단위기준값(3.00)");
		CString	strData150	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "호가단위(0.01)");
		CString	strData151	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "호가단위(0.05)");
		CString	strData152	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "연중최고가일");
		CString	strData153	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "연중최고가");
		CString	strData154	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "연중최저가일");
		CString	strData155	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "연중최저가");
		CString	strData156	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전일내재변동성");
		CString	strData157	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매매증거금기준가");
		CString	strData158	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매매증거금기준가구분");
		CString	strData159	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "KOSPI등가");
		CString	strData160	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "지표산출시간");
		CString	strData161	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "델타");
		CString	strData162	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "감마");
		CString	strData163	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "세타");
		CString	strData164	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "베가");
		CString	strData165	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "로우");
		CString	strData166	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "내재변동성");
		CString	strData167	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "내재가치");
		CString	strData168	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "시간가치");
		CString	strData169	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가내재변동성");
		CString	strData170	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가내재변동성");
		CString	strData171	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "역사적변동성");
		CString	strData172	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "장중이론가이항모델");
		CString	strData173	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "델타이항모델");
		CString	strData174	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "감마이항모델");
		CString	strData175	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "세타이항모델");
		CString	strData176	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "베가이항모델");
		CString	strData177	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "로우이항모델");
		CString	strData178	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "거래량Cal기준내재변동성");
		CString	strData179	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "거래대금Cal기준내재변동성");
		CString	strData180	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "거래량Put기준내재변동성");
		CString	strData181	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "거래대금Put기준내재변동성");
		CString	strData182	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "변동성시간대장마감을Upd");
		CString	strData183	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "시장조성종목여부");
		CString	strData184	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "BATCH없이경과된일수");
		CString	strData185	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "순매수잔량1");
		CString	strData186	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "순매수잔량2");
		CString	strData187	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "순매수잔량3");
		CString	strData188	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "순매수잔량4");
		CString	strData189	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "순매수잔량5");
		CString	strData190	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "총순매수잔량");
		CString	strData191	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가등락율1");
		CString	strData192	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가등락율2");
		CString	strData193	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가등락율3");
		CString	strData194	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가등락율4");
		CString	strData195	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매도호가등락율5");
		CString	strData196	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가등락율1");
		CString	strData197	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가등락율2");
		CString	strData198	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가등락율3");
		CString	strData199	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가등락율4");
		CString	strData200	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매수호가등락율5");
		CString	strData201	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "괴리도");
		CString	strData202	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "영업일잔존일수");
		CString	strData203	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비총수량");
		CString	strData204	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비총수량");
		CString	strData205	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비총건수");
		CString	strData206	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비총건수");
		CString	strData207	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "익일예상상한가");
		CString	strData208	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "익일예상하한가");
		CString	strData209	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "예약실제주문일자");
		CString	strData210	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비수량1");
		CString	strData211	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비수량1");
		CString	strData212	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비건수1");
		CString	strData213	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비건수1");
		CString	strData214	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비수량2");
		CString	strData215	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비수량2");
		CString	strData216	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비건수2");
		CString	strData217	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비건수2");
		CString	strData218	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비수량3");
		CString	strData219	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비수량3");
		CString	strData220	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비건수3");
		CString	strData221	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비건수3");
		CString	strData222	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비수량4");
		CString	strData223	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비수량4");
		CString	strData224	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비건수4");
		CString	strData225	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비건수4");
		CString	strData226	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비수량5");
		CString	strData227	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비수량5");
		CString	strData228	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매도대비건수5");
		CString	strData229	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "직전매수대비건수5");
		CString	strData230	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "예상체결가격");
		CString	strData231	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "이론가대비구분");
		CString	strData232	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "이론가대비");
		

		//WriteLog("[단축코드]" + strData000.Trim());
		//WriteLog("[종목코드]" + strData001.Trim());
		//WriteLog("[한글종목명]" + strData002.Trim());
		//WriteLog("[종목한글약명]" + strData003.Trim());
		//WriteLog("[영문종목명]" + strData004.Trim());
		//WriteLog("[축약종목명]" + strData005.Trim());
		//WriteLog("[상장일]" + strData006.Trim());
		//WriteLog("[상장일수]" + strData007.Trim());
		//WriteLog("[잔존일수]" + strData008.Trim());
		//WriteLog("[최종거래일]" + strData009.Trim());
		//WriteLog("[최종거래일구분]" + strData010.Trim());
		//WriteLog("[배당액지수미래가치]" + strData011.Trim());
		//WriteLog("[기준가격구분]" + strData012.Trim());
		//WriteLog("[기준가격]" + strData013.Trim());
		//WriteLog("[이론가]" + strData014.Trim());
		//WriteLog("[이론가]" + strData015.Trim());
		//WriteLog("[CD금리]" + strData016.Trim());
		//WriteLog("[상한가]" + strData017.Trim());
		//WriteLog("[하한가]" + strData018.Trim());
		//WriteLog("[전일정산가구분]" + strData019.Trim());
		//WriteLog("[전일정산가]" + strData020.Trim());
		//WriteLog("[전일종가구분]" + strData021.Trim());
		//WriteLog("[전일종가]" + strData022.Trim());
		//WriteLog("[전일체결수량]" + strData023.Trim());
		//WriteLog("[전일근월물체결수량]" + strData024.Trim());
		//WriteLog("[전일원월물체결수량]" + strData025.Trim());
		//WriteLog("[전일체결대금]" + strData026.Trim());
		//WriteLog("[전일근월물거래대금]" + strData027.Trim());
		//WriteLog("[전일원월물거래대금]" + strData028.Trim());
		//WriteLog("[전일미결제약정수량　]" + strData029.Trim());
		//WriteLog("[상장중최고가_SIGN포함]" + strData030.Trim());
		//WriteLog("[상장중최고가일]" + strData031.Trim());
		//WriteLog("[상장중최고가비율]" + strData032.Trim());
		//WriteLog("[상장중최저가_SIGN포함]" + strData033.Trim());
		//WriteLog("[상장중최저가일]" + strData034.Trim());
		//WriteLog("[상장중최저가비율]" + strData035.Trim());
		//WriteLog("[입회일자]" + strData036.Trim());
		//WriteLog("[시장가허용구분]" + strData037.Trim());
		//WriteLog("[조건부지정가허용구분]" + strData038.Trim());
		//WriteLog("[최유리지정가허용구분]" + strData039.Trim());
		//WriteLog("[최종결제가]" + strData040.Trim());
		//WriteLog("[최종결제가구분]" + strData041.Trim());
		//WriteLog("[CB적용상한가]" + strData042.Trim());
		//WriteLog("[CB적용하한가]" + strData043.Trim());
		//WriteLog("[전일최종매매체결시각]" + strData044.Trim());
		//WriteLog("[전일스프레드근월물종가]" + strData045.Trim());
		//WriteLog("[전일스프레드원월물종가]" + strData046.Trim());
		//WriteLog("[스프레드근월물표준코드]" + strData047.Trim());
		//WriteLog("[스프레드원월물표준코드]" + strData048.Trim());
		//WriteLog("[거래단위]" + strData049.Trim());
		//WriteLog("[체결시간]" + strData050.Trim());
		//WriteLog("[현재가]" + strData051.Trim());
		//WriteLog("[시가]" + strData052.Trim());
		//WriteLog("[고가]" + strData053.Trim());
		//WriteLog("[저가]" + strData054.Trim());
		//WriteLog("[전일대비구분]" + strData055.Trim());
		//WriteLog("[전일대비　]" + strData056.Trim());
		//WriteLog("[누적거래량]" + strData057.Trim());
		//WriteLog("[누적거래대금]" + strData058.Trim());
		//WriteLog("[전일거래량대비]" + strData059.Trim());
		//WriteLog("[등락률]" + strData060.Trim());
		//WriteLog("[건별체결수량]" + strData061.Trim());
		//WriteLog("[체결구분]" + strData062.Trim());
		//WriteLog("[매도누적거래량]" + strData063.Trim());
		//WriteLog("[매수누적거래량]" + strData064.Trim());
		//WriteLog("[매도누적체결건수]" + strData065.Trim());
		//WriteLog("[매수누적체결건수]" + strData066.Trim());
		//WriteLog("[근월물의제약정가격]" + strData067.Trim());
		//WriteLog("[원월물의제약정가격]" + strData068.Trim());
		//WriteLog("[근월물체결수량]" + strData069.Trim());
		//WriteLog("[원월물체결수량]" + strData070.Trim());
		//WriteLog("[근월물거래대금]" + strData071.Trim());
		//WriteLog("[원월물거래대금]" + strData072.Trim());
		//WriteLog("[매도체결수량_호가기준]" + strData073.Trim());
		//WriteLog("[매수체결수량_호가기준]" + strData074.Trim());
		//WriteLog("[호가수신시간]" + strData075.Trim());
		//WriteLog("[매도호가1]" + strData076.Trim());
		//WriteLog("[매수호가1]" + strData077.Trim());
		//WriteLog("[매도호가수량1]" + strData078.Trim());
		//WriteLog("[매수호가수량1]" + strData079.Trim());
		//WriteLog("[매도호가건수1]" + strData080.Trim());
		//WriteLog("[매수호가건수1]" + strData081.Trim());
		//WriteLog("[매도호가2]" + strData082.Trim());
		//WriteLog("[매수호가2]" + strData083.Trim());
		//WriteLog("[매도호가수량2]" + strData084.Trim());
		//WriteLog("[매수호가수량2]" + strData085.Trim());
		//WriteLog("[매도호가건수2]" + strData086.Trim());
		//WriteLog("[매수호가건수2]" + strData087.Trim());
		//WriteLog("[매도호가3]" + strData088.Trim());
		//WriteLog("[매수호가3]" + strData089.Trim());
		//WriteLog("[매도호가수량3]" + strData090.Trim());
		//WriteLog("[매수호가수량3]" + strData091.Trim());
		//WriteLog("[매도호가건수3]" + strData092.Trim());
		//WriteLog("[매수호가건수3]" + strData093.Trim());
		//WriteLog("[매도호가4]" + strData094.Trim());
		//WriteLog("[매수호가4]" + strData095.Trim());
		//WriteLog("[매도호가수량4]" + strData096.Trim());
		//WriteLog("[매수호가수량4]" + strData097.Trim());
		//WriteLog("[매도호가건수4]" + strData098.Trim());
		//WriteLog("[매수호가건수4]" + strData099.Trim());
		//WriteLog("[매도호가5]" + strData100.Trim());
		//WriteLog("[매수호가5]" + strData101.Trim());
		//WriteLog("[매도호가수량5]" + strData102.Trim());
		//WriteLog("[매수호가수량5]" + strData103.Trim());
		//WriteLog("[매도호가건수5]" + strData104.Trim());
		//WriteLog("[매수호가건수5]" + strData105.Trim());
		//WriteLog("[매도호가총수량]" + strData106.Trim());
		//WriteLog("[매수호가총수량]" + strData107.Trim());
		//WriteLog("[매도호가총건수]" + strData108.Trim());
		//WriteLog("[매수호가총건수]" + strData109.Trim());
		//WriteLog("[장운용구분]" + strData110.Trim());
		//WriteLog("[체결일자]" + strData111.Trim());
		//WriteLog("[미결제약정구분]" + strData112.Trim());
		//WriteLog("[정산가격]" + strData113.Trim());
		//WriteLog("[정산가구분]" + strData114.Trim());
		//WriteLog("[KOSPI200지수]" + strData115.Trim());
		//WriteLog("[이론가]" + strData116.Trim());
		//WriteLog("[괴리율]" + strData117.Trim());
		//WriteLog("[시장BASIS]" + strData118.Trim());
		//WriteLog("[이론BASIS]" + strData119.Trim());
		//WriteLog("[미결제약정수량]" + strData120.Trim());
		//WriteLog("[미결제약정시가]" + strData121.Trim());
		//WriteLog("[미결제약정고가]" + strData122.Trim());
		//WriteLog("[미결제약정저가]" + strData123.Trim());
		//WriteLog("[미결제증감수량]" + strData124.Trim());
		//WriteLog("[2차저항]" + strData125.Trim());
		//WriteLog("[1차저항]" + strData126.Trim());
		//WriteLog("[피봇값]" + strData127.Trim());
		//WriteLog("[1차지지]" + strData128.Trim());
		//WriteLog("[2차지지]" + strData129.Trim());
		//WriteLog("[종목SEQ번호]" + strData130.Trim());
		//WriteLog("[옵션종류]" + strData131.Trim());
		//WriteLog("[거래대상종목]" + strData132.Trim());
		//WriteLog("[만기년월]" + strData133.Trim());
		//WriteLog("[행사가격]" + strData134.Trim());
		//WriteLog("[조회순번]" + strData135.Trim());
		//WriteLog("[권리행사유형구분]" + strData136.Trim());
		//WriteLog("[최근월물구분]" + strData137.Trim());
		//WriteLog("[ATM구분]" + strData138.Trim());
		//WriteLog("[최초거래일]" + strData139.Trim());
		//WriteLog("[신규구분]" + strData140.Trim());
		//WriteLog("[신규일자]" + strData141.Trim());
		//WriteLog("[전일시가]" + strData142.Trim());
		//WriteLog("[전일고가]" + strData143.Trim());
		//WriteLog("[전일저가]" + strData144.Trim());
		//WriteLog("[매매증거금기준가]" + strData145.Trim());
		//WriteLog("[매매증거금기준가구분]" + strData146.Trim());
		//WriteLog("[정상호가범위(상한치)]" + strData147.Trim());
		//WriteLog("[정상호가범위(하한치)]" + strData148.Trim());
		//WriteLog("[가격단위기준값(3.00)]" + strData149.Trim());
		//WriteLog("[호가단위(0.01)]" + strData150.Trim());
		//WriteLog("[호가단위(0.05)]" + strData151.Trim());
		//WriteLog("[연중최고가일]" + strData152.Trim());
		//WriteLog("[연중최고가]" + strData153.Trim());
		//WriteLog("[연중최저가일]" + strData154.Trim());
		//WriteLog("[연중최저가]" + strData155.Trim());
		//WriteLog("[전일내재변동성]" + strData156.Trim());
		//WriteLog("[매매증거금기준가]" + strData157.Trim());
		//WriteLog("[매매증거금기준가구분]" + strData158.Trim());
		//WriteLog("[KOSPI등가]" + strData159.Trim());
		//WriteLog("[지표산출시간]" + strData160.Trim());
		//WriteLog("[델타]" + strData161.Trim());
		//WriteLog("[감마]" + strData162.Trim());
		//WriteLog("[세타]" + strData163.Trim());
		//WriteLog("[베가]" + strData164.Trim());
		//WriteLog("[로우]" + strData165.Trim());
		//WriteLog("[내재변동성]" + strData166.Trim());
		//WriteLog("[내재가치]" + strData167.Trim());
		//WriteLog("[시간가치]" + strData168.Trim());
		//WriteLog("[매도호가내재변동성]" + strData169.Trim());
		//WriteLog("[매도호가내재변동성]" + strData170.Trim());
		//WriteLog("[역사적변동성]" + strData171.Trim());
		//WriteLog("[장중이론가이항모델]" + strData172.Trim());
		//WriteLog("[델타이항모델]" + strData173.Trim());
		//WriteLog("[감마이항모델]" + strData174.Trim());
		//WriteLog("[세타이항모델]" + strData175.Trim());
		//WriteLog("[베가이항모델]" + strData176.Trim());
		//WriteLog("[로우이항모델]" + strData177.Trim());
		//WriteLog("[거래량Cal기준내재변동성]" + strData178.Trim());
		//WriteLog("[거래대금Cal기준내재변동성]" + strData179.Trim());
		//WriteLog("[거래량Put기준내재변동성]" + strData180.Trim());
		//WriteLog("[거래대금Put기준내재변동성]" + strData181.Trim());
		//WriteLog("[변동성시간대장마감을Upd]" + strData182.Trim());
		//WriteLog("[시장조성종목여부]" + strData183.Trim());
		//WriteLog("[BATCH없이경과된일수]" + strData184.Trim());
		//WriteLog("[순매수잔량1]" + strData185.Trim());
		//WriteLog("[순매수잔량2]" + strData186.Trim());
		//WriteLog("[순매수잔량3]" + strData187.Trim());
		//WriteLog("[순매수잔량4]" + strData188.Trim());
		//WriteLog("[순매수잔량5]" + strData189.Trim());
		//WriteLog("[총순매수잔량]" + strData190.Trim());
		//WriteLog("[매도호가등락율1]" + strData191.Trim());
		//WriteLog("[매도호가등락율2]" + strData192.Trim());
		//WriteLog("[매도호가등락율3]" + strData193.Trim());
		//WriteLog("[매도호가등락율4]" + strData194.Trim());
		//WriteLog("[매도호가등락율5]" + strData195.Trim());
		//WriteLog("[매수호가등락율1]" + strData196.Trim());
		//WriteLog("[매수호가등락율2]" + strData197.Trim());
		//WriteLog("[매수호가등락율3]" + strData198.Trim());
		//WriteLog("[매수호가등락율4]" + strData199.Trim());
		//WriteLog("[매수호가등락율5]" + strData200.Trim());
		//WriteLog("[괴리도]" + strData201.Trim());
		//WriteLog("[영업일잔존일수]" + strData202.Trim());
		//WriteLog("[직전매도대비총수량]" + strData203.Trim());
		//WriteLog("[직전매수대비총수량]" + strData204.Trim());
		//WriteLog("[직전매도대비총건수]" + strData205.Trim());
		//WriteLog("[직전매수대비총건수]" + strData206.Trim());
		//WriteLog("[익일예상상한가]" + strData207.Trim());
		//WriteLog("[익일예상하한가]" + strData208.Trim());
		//WriteLog("[예약실제주문일자]" + strData209.Trim());
		//WriteLog("[직전매도대비수량1]" + strData210.Trim());
		//WriteLog("[직전매수대비수량1]" + strData211.Trim());
		//WriteLog("[직전매도대비건수1]" + strData212.Trim());
		//WriteLog("[직전매수대비건수1]" + strData213.Trim());
		//WriteLog("[직전매도대비수량2]" + strData214.Trim());
		//WriteLog("[직전매수대비수량2]" + strData215.Trim());
		//WriteLog("[직전매도대비건수2]" + strData216.Trim());
		//WriteLog("[직전매수대비건수2]" + strData217.Trim());
		//WriteLog("[직전매도대비수량3]" + strData218.Trim());
		//WriteLog("[직전매수대비수량3]" + strData219.Trim());
		//WriteLog("[직전매도대비건수3]" + strData220.Trim());
		//WriteLog("[직전매수대비건수3]" + strData221.Trim());
		//WriteLog("[직전매도대비수량4]" + strData222.Trim());
		//WriteLog("[직전매수대비수량4]" + strData223.Trim());
		//WriteLog("[직전매도대비건수4]" + strData224.Trim());
		//WriteLog("[직전매수대비건수4]" + strData225.Trim());
		//WriteLog("[직전매도대비수량5]" + strData226.Trim());
		//WriteLog("[직전매수대비수량5]" + strData227.Trim());
		//WriteLog("[직전매도대비건수5]" + strData228.Trim());
		//WriteLog("[직전매수대비건수5]" + strData229.Trim());
		//WriteLog("[예상체결가격]" + strData230.Trim());
		//WriteLog("[이론가대비구분]" + strData231.Trim());
		//WriteLog("[이론가대비]" + strData232.Trim());



		//AfxMessageBox(strLastP + "[" + strHighP + "]");
	}
	else if ( sTrCode == DEF_CHE_LIST )
	{
		CString strMsg;
		strMsg.Format("국내 체결내역 : 조회[%s]", DEF_CHE_LIST);
		//WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "주문번호");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목코드");
			CString strMntMgn  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매매구분");

			strAcctNm.TrimRight();
			strTrstMgn.TrimRight();
			strMsg.Format("계좌번호[%s] 주문번호[%s] 종목[%s] 매매구분[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			//WriteLog(strMsg);
		}
		strMsg.Format("체결수량[%d]", nRepeatCnt);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_MICHE_LIST )
	{
		CString strMsg;
		strMsg.Format("국내 미체결내역 : 조회[%s]", DEF_MICHE_LIST);
		//WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "주문번호");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목코드");
			CString strMntMgn  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "매매구분");

			strAcctNm.TrimRight();
			strTrstMgn.TrimRight();
			strMsg.Format("계좌번호[%s] 주문번호[%s] 종목[%s] 매매구분[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			//WriteLog(strMsg);
		}
		strMsg.Format("미체결수량[%d]", nRepeatCnt);
		//WriteLog(strMsg);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	// 해외
	///////////////////////////////////////////////////////////////////////////////////////////
	else if ( sTrCode == DEF_HW_JANGO )
	{
		CString strMsg;
		strMsg.Format("해외 예탁자산 및 증거금 응답[%s]", DEF_HW_JANGO);
		//WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "통화구분");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "예탁금총액");
			CString strEntrCh = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "예탁금잔액");

			strAcctNm.TrimRight();
			strMsg.Format("통화구분[%s] 예탁금총액[%s] 예탁금잔액[%s]", strAcctNm, strEntrTot, strEntrCh);
			//WriteLog(strMsg);
		}
	}
	else if ( sTrCode == DEF_HW_ORD_CODE_NEW )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString strExchTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "접수구분");
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리코드");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리메시지");
		CString strOrdNo  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "주문번호");

		CString strMsg;
		strMsg.Format("주문응답 번호[%d][%s]처리[%s]계좌번호[%s]주문번호[%s]", nRqID, strExchTp, strProcTp, strAcctNo, strOrdNo);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_HW_MSTINFO )
	{
		// OCX에서 /mst/JMCODE.cod 파일을 생성시킨다.
		//WriteLog("해외 종목 서비스 요청 완료!!!");
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	// FX마진		//@lhe 2012.06.22
	///////////////////////////////////////////////////////////////////////////////////////////
	else if ( sTrCode == DEF_FX_JANGO )
	{
		CString strMsg;
		strMsg.Format("FX 계좌정보 : 자산내역조회[%s]", DEF_FX_JANGO);
		//WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌명");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "예탁총액");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "위탁증거금");
			CString strMntMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "유지증거금");

			strAcctNm.TrimRight();
			strMsg.Format("계좌명[%s] 예탁총액[%s] 위탁증거금[%s] 유지증거금[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			//WriteLog(strMsg);
		}
	}
	else if ( sTrCode == DEF_FX_ORD_CODE_NEW )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString strExchTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "접수구분");
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리코드");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리메시지");
		CString strOrdNo  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "주문번호");

		CString strMsg;
		strMsg.Format("주문응답 번호[%d][%s]처리[%s]계좌번호[%s]주문번호[%s]", nRqID, strExchTp, strProcTp, strAcctNo, strOrdNo);
		//WriteLog(strMsg);
	}
	else if (sTrCode == "o44005" )
	{
		// 해외 차트 조회
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec2");
		g_strNextKey = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "keyvalue");
		CString sSymbol = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목코드");

		CString strMsg;
		strMsg.Format("차트응답개수[%d]", nRepeatCnt);
		IR_PrintMsg(LOGTP_SUCC, FALSE, strMsg.GetBuffer());
		char zQ[1024];
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "일자");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "시간");
			CString sDomestDate = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "국내일자");
			CString sDomestTime = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "국내시간");
			CString sOpenP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "시가");
			CString sHighP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "고가");
			CString sLowP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "저가");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "종가");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "체결량");
			CString sGb = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "구분");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			IR_PrintMsg(LOGTP_SUCC, FALSE, strMsg.GetBuffer());

			CDBHandlerAdo db(DBPoolPtr->Get());
			sprintf(zQ, "EXEC CHART_SAVE_HIST_HYUNDAI "
				"'%.8s', "	//DT
				"'%.10s', "	//TM
				"'%.8s', "	//DOMEST_DT
				"'%.10s', "	//DOMEST_TM
				"'%s', "	//SYMBOL
				"%d, "	//CHART_TP
				"'%.10s', "	//OPEN_PRC
				"'%.10s', "	//HIGH_PRC
				"'%.10s', "	//LOW_PRC
				"'%.10s', "	//CLOSE_PRC
				"'%.10s', "	//CNTR_QTY
				"'%.1s'  "	//GB
				,
				(LPCSTR)sDate.Trim(),
				(LPCSTR)sTime.Trim(),
				(LPCSTR)sDomestDate.Trim(),
				(LPCSTR)sDomestTime.Trim(),
				(LPCSTR)sSymbol.Trim(),
				m_nLastChartTp,
				(LPCSTR)sOpenP.Trim(),
				(LPCSTR)sHighP.Trim(),
				(LPCSTR)sLowP.Trim(),
				(LPCSTR)sCloseP.Trim(),
				(LPCSTR)sVolume.Trim(),
				(LPCSTR)sGb.Trim()
			);
			if (!db->ExecQuery(zQ))
			{
				IR_PrintMsg(LOGTP_ERR, TRUE, "차트정보 저장 오류(%s)(%s)", zQ, db->GetError());
			}
			Sleep(0);
		}
		IR_PrintMsg(LOGTP_SUCC, FALSE, "--------------------------------------------------------");

		// 분, 일, 주, 월 
		if (m_sLastChart != "월")
		{
			OnBnClickedButtonChart();
		}
		else
		{
			IR_PrintMsg(LOGTP_SUCC, TRUE, "==================[%s] 종목의 조회가 완료되었습니다============================", m_sSymbol.GetBuffer());
			GetDlgItem(IDC_EDIT_SYMBOL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CHART)->EnableWindow(TRUE);
		}
	}
	
	else if ( sTrCode == "l41601" )
	{
		// input = "1012014051620020"
		// req = "000001002003004005006007008009"
		int nRepeatCnt = 20;	// 조회시 요청한 개수
		if ( nRepeatCnt > 0 )
		{
			typedef struct 
			{
				char		baseid[8];		/*  [기초자산ID]기초자산ID     */
				char		date[8];		/*  [일자]일자     */
				char		price[6];		/*  [현재가]현재가     */
				char		sign[1];		/*  [대비구분]대비구분     */
				char		change[6];		/*  [대비]대비     */
				char		open[6];		/*  [시가]시가     */
				char		high[6];		/*  [고가]고가     */
				char		low[6];			/*  [저가]저가     */
				char		volume[15];		/*  [누적거래량]누적거래량     */
				char		jnilclose[6];	/*  [전일종가]     */

			}HDF_I41601;
			int nBuffSize = sizeof(HDF_I41601);
			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");

			// Struct만큼 잘라서 사용하세요...

			//WriteLog(strBuff);
		}
	}
	else if ( sTrCode == "g11004.AQ0207%" )
	{
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리코드");
		CString strProMsg = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리메시지");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
		CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌명");
		CString strTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "구분");
		CString strYN  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "일치여부");

		CString strBuff;
		strBuff.Format("[%s][%s][%s][%s][%s][%s]", strProcTp, strProMsg, strAcctNo, strAcctNm, strTp, strYN);

		//WriteLog(strBuff);
	}
	else if ( sTrCode == "o51100" )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "종목코드");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "종목명");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "현재가");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "전일대비구분");

			CString strMsg;
			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			//WriteLog(strMsg);
		}
	}
	else if (sTrCode == "o51200" )
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "일자");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "시간");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "종가");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "체결량");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			//WriteLog(strMsg);
		}

		int nBuffSize = 129 + 16;	// Fid조회는 필드 마지막에 구분자 1자리가 있으므로 각 필드 만큼 더해준다.
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		//WriteLog(strMsg);

		CString strNextKey = m_CommAgent.CommGetNextKey(nRqID, "");
		if ( strNextKey != "" )
		{
			int nRet = MessageBox( "조회를 계속 할까요?", "현대선물", MB_YESNO);
			if ( nRet == IDYES )
			{
				m_strNextKey = strNextKey;
				OnBnClickedRqtest();
			}
		}
	}
	else if ( sTrCode == "g11004.AQ0401%" )
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문번호");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "계좌번호");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "종목코드");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문가격");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			//WriteLog(strMsg);
		}

		int nBuffSize = 226;
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == "g11004.AQ0408%" )
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sData01 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "처리일자");
			CString sData02 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문번호");
			CString sData03 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문/체결구분");
			CString sData04 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "종목코드");
			CString sData05 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "매매구분");
			CString sData06 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문수량");
			CString sData07 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "정정수량");
			CString sData08 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "취소수량");
			CString sData09 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "체결수량");
			CString sData10 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문잔량");
			CString sData11 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문/체결가격");
			CString sData12 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "가격조건");
			CString sData13 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "통신주문구분");
			CString sData14 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "주문전략구분");
			CString sData15 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "거래소접수/거부구분");
			CString sData16 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "거래소접수/거부처리시간");
			CString sData17 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "거래소접수번호/거부코드");
			CString sData18 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "거래소주문상태");
			CString sData19 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "작업일자");
			CString sData20 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "작업시간");
			CString sData21 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "작업사원");
			CString sData22 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "사설IP");
			CString sData23 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "공인IP");
			CString sData24 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "처리일자");
			CString sData25 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "최초원주문번호");
			CString sData26 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "stop가격");


			strMsg.Format("[%s][%s][%s][%s]", sData01, sData02, sData03, sData04);
			//WriteLog(strMsg);
		}

		int nBuffSize = 226;
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		//WriteLog(strMsg);


		CString strNextKey = m_CommAgent.CommGetNextKey(nRqID, "");
		if ( strNextKey != "" )
		{
			int nRet = MessageBox( "조회를 계속 할까요?", "현대선물", MB_YESNO);
			if ( nRet == IDYES )
			{
				m_strNextKey = strNextKey;
				OnBnClickedRqtest();
			}
		}
	}
	else if ( sTrCode == "o44011" )	// 서버 현재시간
	{
		CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "서버일자");
		CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "서버시간");

		strMsg.Format("서버날짜[%s]서버시간[%s]", sDate, sTime);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == "n51003") 
	{
		// 공지사항 조회
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "제목");

		strMsg.Format("[%s]", sTemp1);
		//WriteLog(strMsg);

		CString strNextKey = m_CommAgent.CommGetNextKey(nRqID, "");
		if ( strNextKey != "" )
		{
			int nRet = MessageBox( "n51003 조회를 계속 할까요?", "현대선물", MB_YESNO);
			if ( nRet == IDYES )
			{
				m_strNextKey = strNextKey;
				OnBnClickedRqtest();
			}
		}
	}
	else if ( sTrCode == "g11002.DQ0321&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리코드");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "처리메시지");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
		CString sTemp4   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "일치여부");
		CString sTemp5   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌구분");

		strMsg.Format("처리코드[%s]처리메시지[%s]계좌번호[%s]일치여부[%s]계좌구분[%s]", sTemp1, sTemp2, sTemp3, sTemp4, sTemp5);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == "g11002.DQ0110&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌명");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "종목");

		strMsg.Format("계좌번호[%s]계좌명[%s]종목[%s]", sTemp1, sTemp2, sTemp3);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == "n51006" )
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString sLen   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "전체사이즈");

		int nBuffSize = 10+1+255+atoi(sLen);
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		//WriteLog(strMsg);
		AfxMessageBox(strMsg);
	}
	else if ( sTrCode == "l41600" )
	{
		/***
		if ( nRepeatCnt > 0 )
		{
			typedef struct 
			{
				char		baseid[8];		//  [기초자산ID]기초자산ID
				char		date[8];		//  [일자]일자
				char		price[6];		//  [현재가]현재가
				char		sign[1];		//  [대비구분]대비구분
				char		change[6];		//  [대비]대비
				char		open[6];		//  [시가]시가
				char		high[6];		//  [고가]고가
				char		low[6];			//  [저가]저가
				char		volume[15];		//  [누적거래량]누적거래량
				char		jnilclose[6];	//  [전일종가]

			}HDF_I41601;
			int nBuffSize = sizeof(HDF_I41601);
			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
			
			// Struct만큼 잘라서 사용하세요...
			//WriteLog(strBuff);
		}
		***/

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		CString sLen1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "품목명");
		CString sLen2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "체결시간");
		CString sLen3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "현재가");

		//WriteLog(sLen1 + sLen2 + sLen3);
	}
	else if ( sTrCode ==  "g11002.CQ0101&")
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "사용자ID");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "대출구분");
			CString strMntMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "TMP1");

			strAcctNm.TrimRight();
			strMsg.Format("사용자ID[%s] 계좌번호[%s] 대출구분[%s] TMP1[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			//WriteLog(strMsg);
		}
	}
	else if ( sTrCode == "o44010" )
	{
		// 해외 차트 조회
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec2");

		CString sRcvCnt   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "수신봉개수");
		
		nRepeatCnt = atoi(sRcvCnt);

		CString strMsg;
		strMsg.Format("응답개수[%d]", nRepeatCnt);
		//WriteLog(strMsg);

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "시작일자");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "시작시간");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "종가");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "체결량");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			//WriteLog(strMsg);
		}
	}
	else if ( sTrCode == "v90001" )
	{
		// 종목정보 요청 기능.
		long nFileSize = atol(m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "파일크기"));
		CString strMsg;
		strMsg.Format("파일크기[%d]", nFileSize);
		//WriteLog(strMsg);


		CString strFileNm =  m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "파일명");
		CString strProcCd =  m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "응답코드");

		if ( strProcCd == "REOK" )
		{
			TCHAR iniFileName[500];

			GetModuleFileName(NULL, iniFileName, MAX_PATH); 
			CString strFileName;
			const CString strExeName = "HDFCommClient.exe";
			strFileName.Format("%s", iniFileName);
			int nDel = strExeName.GetLength();
			int nFind = strFileName.Find(strExeName);
			strFileName.Delete(nFind, nDel);
			strFileName = strFileName + "mst/" + strFileNm;

			CString strCommonFileName = strFileName;

			CFile commonfile;
			// open file
			if(!commonfile.Open(strCommonFileName, CFile::modeWrite /*| CFile::typeBinary*/))
			{
				if( commonfile.Open(strCommonFileName, CFile::modeCreate | CFile::modeWrite /*| CFile::typeBinary*/) == FALSE)
				{
					CString strMsg;
					strMsg.Format("%s화일 생성에 실패하였습니다. ", strCommonFileName);
					//WriteLog(strMsg);
					return;
				}
			}

			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 128+4+8, nFileSize, 0, "A");
			commonfile.Write(strBuff, nFileSize);
			commonfile.Close();
		}

		//WriteLog("처리완료");
	}
	else if ( sTrCode == "g11002.DQ0242&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "예탁금액-총액");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "주문가능금액");

		strMsg.Format("계좌번호[%s]예탁금액-당일손익[%s]주문가능금액[%s]", sTemp1, sTemp2, sTemp3);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == "g11002.DQ1305&" || sTrCode == "g11002.DQ1306&" ) 
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "계좌번호");
			CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "종목");
			CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "평가손익");

			strMsg.Format("계좌번호[%s]종목[%s]평가손익[%s]", sTemp1, sTemp2, sTemp3);
			//WriteLog(strMsg);
		}
	}
	else if ( sTrCode == "g11002.DQ1303&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "계좌번호");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "가결제금액");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "평가예탁총액");

		strMsg.Format("계좌번호[%s]예탁금액현금[%s]평가예탁총액[%s]", sTemp1, sTemp2, sTemp3);
		//WriteLog(strMsg);
	}
	else if ( sTrCode == "v90003" )
	{
		int nRepeatCnt = atoi(m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "차트카운트"));

		CString strNextKey = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "이전키");
		CString strMsg;
		strMsg.Format("응답개수[%d]이전키[%s]", nRepeatCnt, strNextKey);
		//WriteLog(strMsg);

		nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec2");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDtTm   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "날짜시간");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "종가");
			CString sOP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "시가");
			CString sHP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "고가");
			CString sLP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "저가");

			strMsg.Format("DtTm[%s] C[%s] O[%s] H[%s] L[%s]", sDtTm, sCloseP, sOP, sHP, sLP);
			//WriteLog(strMsg);
		}
	}
}

/*
// 실시간 수신

- CommGetData 의 필드명(마지막파라미터)은 D:\IRUM_VS\HDAPI\bin\TrData 참조
*/
void CHDFCommClientDlg::OnGetBroadData(CString strKey, LONG nRealType)
{
	CString type; type.Format("실시간데이터:%d", nRealType);
	IR_PrintMsg(LOGTP_SUCC, FALSE, type.GetBuffer());
	switch (nRealType)
	{

		case 189: //해외체결내역
		{
			
		}
		break;
		//case 196: //해외주문접수
		//{
		//	CString strOrdNo = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
		//	CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
		//	CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
		//	CString strOrdP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
		//	CString strOrdQ = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

		//	strOrdNo.Trim();
		//	strSeries.Trim();
		//	strOrdP.Trim();
		//	strOrdQ.Trim();

		//	CString strErrCd = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "처리코드");
		//	CString strErrMsg = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "처리메세지");
		//	strErrCd.Trim();
		//	strErrMsg.Trim();

		//	CString strMsg;

		//	strMsg.Format("[%d] 주문접수코드[%s][%s]", nRealType, strErrCd, strErrMsg);
		//	IR_PrintMsg(LOGTP_SUCC, TRUE, strMsg.GetBuffer());

		//	strMsg.Format("[%d] 주문접수 번호[%s]종목[%s]매매[%s]가격[%s]수량[%s]", nRealType, strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
		//	IR_PrintMsg(LOGTP_SUCC, TRUE, strMsg.GetBuffer());

		//	//[I][20:30:03.871][196] 주문접수코드[00000][주문번호 17NC001979으로 정상접수처리 됨]
		//	//[I][20:30 : 03.871][196] 주문접수 번호[17NC001979]종목[CLQ17]매매[1]가격[46.25]수량[2]

		//	m_strOrdNo = strOrdNo;
		//}
		//break;
		//case 186: //해외미체결내역
		//{
		//	CString strOrdNo = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
		//	CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목");
		//	CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
		//	CString strOrdP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
		//	CString strOrdQ = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

		//	strOrdNo.TrimRight();
		//	strSeries.TrimRight();
		//	strOrdP.TrimRight();
		//	strOrdQ.TrimRight();

		//	if (m_strOrdNo == strOrdNo)
		//	{
		//		CString strMsg;
		//		strMsg.Format("미체결 주문번호[%s]종목[%s]매매[%s]가격[%s]수량[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
		//		IR_PrintMsg(LOGTP_SUCC, TRUE, strMsg.GetBuffer());
		//		//[I][20:30:03.879]미체결 주문번호[17NC001979]종목[]매매[1]가격[          46.25]수량[         2]
		//		//[I][20:30 : 03.973]미체결 주문번호[17NC001979]종목[]매매[1]가격[46.25]수량[2]

		//	}
		//}
		//break;
		//case 187: //해외미결제내역
		//{

		//}
		//break;

	} // switch (nRealType)

}

void CHDFCommClientDlg::OnGetMsg(CString strCode, CString strMsg)
{
	CString strLog;
	strLog.Format("[%s][%s]", strCode, strMsg);
	////WriteLog(strLog);
}

void CHDFCommClientDlg::OnGetMsgWithRqId(int nRqId, CString strCode, CString strMsg)
{
	CString strLog;
	if (strCode.Compare("00000")!= 0 && strCode.Compare("75001")) {
		strLog.Format("RQID[%d][%s][%s]", nRqId, strCode, strMsg);
		IR_PrintMsg(LOGTP_ERR, TRUE, "ERR=>%s", strLog.GetBuffer());
	}
}

void CHDFCommClientDlg::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_edTrCode.SetWindowText(DEF_TR_CODE);
	m_edInput.SetWindowText("101F");

	// FID조회
	m_edTrCode2.SetWindowText(DEF_FID_CODE);
	m_edInput2.SetWindowText("101M300020001");
	//m_edInput3.SetWindowText("000075051057");//종목코드+호가시간+종가+누적거래량
	//전체요청.
	m_edInput3.SetWindowText("000001002003004005006007008009010011012013014015016017018019020021022023024025026027028029030031032033034035036037038039040041042043044045046047048049050051052053054055056057058059060061062063064065066067068069070071072073074075076077078079080081082083084085086087088089090091092093094095096097098099100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232");

	// 실시간 선물/옵션 체결/호가 등록
	m_edJomgmok.SetWindowText("101G3000");
	m_edGubun.SetWindowText("65");

	m_edAccNo.SetWindowText("05359001   ");

	m_edJumunTR.SetWindowText(DEF_ORD_CODE_NEW);
	//m_edJumunInput.SetWindowTextA("05359001   1234    101JC000                        11123390        1");
	CString strOrderBuff = "   1234    201JB245                        11111            1";
	m_edJumunInput.SetWindowTextA("04001001   1234    201JB245                        111110           1");

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_PRCETP);
	pCombo->ResetContent();
	pCombo->AddString("1.지정가");
	pCombo->AddString("2.시장가");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_TRDCOND);
	pCombo->ResetContent();
	pCombo->AddString("1.FAS");
	pCombo->AddString("2.FOK");
	pCombo->AddString("3.FAK");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	pCombo->ResetContent();
	pCombo->AddString("신규");
	pCombo->AddString("정정");
	pCombo->AddString("취소");
	pCombo->AddString("CME신규");
	pCombo->AddString("CME정정");
	pCombo->AddString("CME취소");
	pCombo->SetCurSel(0);

	ShowHWOrdCtrl(FALSE);
}


void CHDFCommClientDlg::OnBnClickedRadio4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_edTrCode.SetWindowText(DEF_HW_JANGO);
	m_edInput.SetWindowText("0112278520    ");

	// FID조회
	// 해외시세 조회 오류 수정 - 20160830 sivas
	CString strSeries = "ESU16                           ";
	m_edTrCode2.SetWindowText(DEF_HW_FID_CODE);
	m_edInput2.SetWindowText(strSeries);
	m_edInput3.SetWindowText("000001002003004005006007008009010011012013014015016017018019020021022023024025026027028029030031032033034035036037");

	// 실시간 선물/옵션 체결/호가 등록
	m_edJomgmok.SetWindowText(strSeries);
	m_edGubun.SetWindowText("82");

	m_edAccNo.SetWindowText("011227     ");

	m_edJumunTR.SetWindowText(DEF_HW_ORD_CODE_NEW);
	m_edJumunInput.SetWindowText("0124311234    ESU16                           1200              1         1                                  ");

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_PRCETP);
	pCombo->ResetContent();
	pCombo->AddString("1.지정가");
	pCombo->AddString("2.시장가");
	pCombo->AddString("3.STOP");
	pCombo->AddString("4.STOP지정가");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_TRDCOND);
	pCombo->ResetContent();
	pCombo->AddString("0.DAY");
	pCombo->AddString("1.GTC");
	pCombo->AddString("3.IOC");
	pCombo->AddString("4.FOK");
	pCombo->AddString("6.GTD");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	pCombo->ResetContent();
	pCombo->AddString("신규");
	pCombo->AddString("정정");
	pCombo->AddString("취소");
	pCombo->SetCurSel(0);

	ShowHWOrdCtrl(TRUE);
}

//@lhe 2012.06.22 FX마진
void CHDFCommClientDlg::OnBnClickedRadio5()
{
	m_edTrCode.SetWindowText(DEF_FX_JANGO);
	m_edInput.SetWindowText("1sivas99 0119861234                        ");

	// FID조회
	CString strSeries = "EUR/USD   ";
	m_edTrCode2.SetWindowText(DEF_FX_FID_CODE);
	m_edInput2.SetWindowText(strSeries);
	m_edInput3.SetWindowText("000001002010");//종목코드+매도가격+매수가격+호가시간

	// 실시간 선물/옵션 체결/호가 등록
	m_edJomgmok.SetWindowText(strSeries);
	m_edGubun.SetWindowText("171");

	m_edAccNo.SetWindowText("011986     ");

	m_edJumunTR.SetWindowText(DEF_FX_ORD_CODE_NEW);
	m_edJumunInput.SetWindowText("0119861234    EUR/USD                         11.24633             1              CEO");	//예약주문

	ShowHWOrdCtrl(FALSE);

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	pCombo->ResetContent();
	pCombo->AddString("신규");
	pCombo->AddString("정정");
	pCombo->AddString("취소");
	pCombo->SetCurSel(0);
}


void CHDFCommClientDlg::OnBnClickedButton1()
{

	CString strMsg;
	strMsg.Format("해외 모든MST파일 정보 서비스 요청");
	//WriteLog(strMsg);


	//대소문자 구별하지 않음. 내부적으로 변환하여 사용됨 - 20150417 anigate
	/*!!!!중요!!!!*/
	// 전체요청과 단일요청을 함께 사용금지!
	// 혹시라도 사용하게 되면 단일요청이든 전체요청이든 모든 응답값을 수신 후에 호출할 것! - 20150417 anigate
	//m_CommAgent.CommReqMakeCod("aLL");
	//m_CommAgent.CommReqMakeCod("jmCode|cMe");
	//m_CommAgent.CommReqMakeCod("opmCode");

	m_CommAgent.CommReqMakeCod("all");
}


void CHDFCommClientDlg::OnBnClickedBusinessday()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//WriteLog(m_CommAgent.CommGetBusinessDay(3));
}


void CHDFCommClientDlg::OnBnClickedFrbusinessday()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//WriteLog(m_CommAgent.CommGetBusinessDay(1));
}


void CHDFCommClientDlg::OnBnClickedCommgetinfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString getInfo = "";
	m_edGetInfo.GetWindowText(getInfo);

	//WriteLog(m_CommAgent.CommGetInfo(getInfo));
	//WriteLog(m_CommAgent.CommGetInfo(getInfo, "1"));
}


void CHDFCommClientDlg::OnBnClickedButton3()
{
	CString strMsg;
	strMsg.Format("해외 선물MST파일 정보 서비스 요청");
	//WriteLog(strMsg);

	m_CommAgent.CommReqMakeCod("futures");
}


void CHDFCommClientDlg::OnBnClickedButton2()
{
	CString strMsg;
	strMsg.Format("해외 옵션MST파일 정보 서비스 요청");
	//WriteLog(strMsg);

	m_CommAgent.CommReqMakeCod("option");
}


void CHDFCommClientDlg::OnBnClickedBtnSavelog()
{
	TCHAR iniFileName[500];

	GetModuleFileName(NULL, iniFileName, MAX_PATH); 
	CString strFileName;
	CString strExeName = "HDFCommClient.exe";
	strFileName.Format("%s", iniFileName);
	int nDel = strExeName.GetLength();
	int nFind = strFileName.Find("HDFCommClient.exe");
	strFileName.Delete(nFind, nDel);
	strFileName = strFileName += "ocx_log.log";

	CString strCommonFileName = strFileName;

	CStdioFile commonfile;
	// open file
	if(!commonfile.Open(strCommonFileName, CFile::modeWrite /*| CFile::typeBinary*/))
	{
		if( commonfile.Open(strCommonFileName, CFile::modeCreate | CFile::modeWrite /*| CFile::typeBinary*/) == FALSE)
		{
			CString strMsg;
			strMsg.Format("%s화일 생성에 실패하였습니다. ", strCommonFileName);
			return;
		}
	}

	int nCnt = m_ListOutMsg.GetCount();
	CString strLine;
	for ( int i=0; i<nCnt; i++ )
	{
		m_ListOutMsg.GetText(i, strLine);

		commonfile.SeekToEnd();
		commonfile.WriteString((LPCSTR)strLine);
		commonfile.WriteString((LPCSTR)"\n");
	}

	commonfile.Close();
}


void CHDFCommClientDlg::OnBnClickedBtnLogclear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ListOutMsg.ResetContent();
}

CString CHDFCommClientDlg::FillStr(CString strSrc, int nTotLen, BOOL bRightAdd, char chPad)
{
	strSrc.Trim();

	// 방어코딩 추가. - 20150804 sivas
	int nPadLen = nTotLen - strSrc.GetLength();
	CString strTarget = strSrc;

	if ( nPadLen > 0 )
	{
		CString strPad(chPad, nPadLen);
		if ( bRightAdd == TRUE )
		{
			strTarget = strSrc + strPad;
		}
		else
		{
			strTarget = strPad + strSrc;
		}
	}

	return strTarget;
}

void CHDFCommClientDlg::OnBnClickedOrdApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton *pBtnFO = (CButton *)GetDlgItem(IDC_RADIO3);
	CButton *pBtnHW = (CButton *)GetDlgItem(IDC_RADIO4);
	CButton *pBtnFX = (CButton *)GetDlgItem(IDC_RADIO5);

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	int nSel = pCombo->GetCurSel();

	CString strAcctNo	= "";
	CString strAcctPw 	= "";
	CString strSeries 	= "";
	CString strByslTp 	= "";
	CString strPrceTp 	= "";
	CString strTrdCond	= "";
	CString strOrdP   	= "";
	CString strOrdQ   	= "";
	CString strReserved	= "";
	CString strOrdNo    = "";
	CString strStrTp	= "";	// 전략구분
	CString strStopP	= "";
	CString strIOCQty	= "";
	CString strOptionYn	= "";
	CString strOrdEftDt = "";

	CEdit *pEditAcctNo	 = (CEdit *)GetDlgItem(IDC_ORD_ACCTNO);
	CEdit *pEditAcctPw 	 = (CEdit *)GetDlgItem(IDC_ORD_ACCTPW);
	CEdit *pEditSeries 	 = (CEdit *)GetDlgItem(IDC_ORD_SERIES);
	CEdit *pEditOrdP   	 = (CEdit *)GetDlgItem(IDC_ORD_PRICE);
	CEdit *pEditOrdQ   	 = (CEdit *)GetDlgItem(IDC_ORD_ORDQTY);
	CEdit *pEditReserved = (CEdit *)GetDlgItem(IDC_ORD_RESERVED);
	CEdit *pEditOrdNo    = (CEdit *)GetDlgItem(IDC_ORD_ORDNO);
	CEdit *pEditStopP    = (CEdit *)GetDlgItem(IDC_ORD_STOPP);
	CEdit *pEditIOCQty   = (CEdit *)GetDlgItem(IDC_ORD_IOCQTY);
	CEdit *pEditOptionYn = (CEdit *)GetDlgItem(IDC_ORD_OPTIONYN);
	CEdit *pEditOrdEftDt = (CEdit *)GetDlgItem(IDC_ORD_ORDEFTDT);
	
	

	CComboBox *pComboByslTp 	 = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_BYSLTP);
	CComboBox *pComboPrceTp 	 = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_PRCETP);
	CComboBox *pComboTrdCond	 = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_TRDCOND);
	CComboBox *pComboStrTp		 = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_STRTP);
	

	pEditAcctNo	 ->GetWindowText(strAcctNo);  
	pEditAcctPw  ->GetWindowText(strAcctPw);
	pEditSeries  ->GetWindowText(strSeries);	
	pEditOrdP    ->GetWindowText(strOrdP   	);	
	pEditOrdQ    ->GetWindowText(strOrdQ   	);	
	pEditReserved->GetWindowText(strReserved);
	pEditOrdNo   ->GetWindowText(strOrdNo);
	pEditStopP   ->GetWindowText(strStopP);
	pEditIOCQty  ->GetWindowText(strIOCQty);
	pEditOptionYn->GetWindowText(strOptionYn);
	pEditOrdEftDt->GetWindowText(strOrdEftDt);

	pComboByslTp->GetWindowText(strByslTp);
	strByslTp = strByslTp.Left(1);

	pComboPrceTp->GetWindowText(strPrceTp);
	strPrceTp = strPrceTp.Left(1);

	pComboTrdCond->GetWindowText(strTrdCond);
	strTrdCond = strTrdCond.Left(1);

	pComboStrTp->GetWindowText(strStrTp);
	strStrTp = strStrTp.Left(1);

	CString strInput = "";
	switch (nSel)
	{
		case 0 :	// 신규
		case 3 :
			{
				if ( pBtnFO->GetCheck() == TRUE )
				{
					strInput  = FillStr(strAcctNo  , 11);
					strInput += FillStr(strAcctPw  ,  8);
					strInput += FillStr(strSeries  , 32);
					strInput += FillStr(strByslTp  ,  1);
					strInput += FillStr(strPrceTp  ,  1);
					strInput += FillStr(strTrdCond ,  1);
					strInput += FillStr(strOrdP    , 13);
					strInput += FillStr(strOrdQ    ,  5);
					strInput += FillStr(strReserved, 60);
				}
				else if ( pBtnHW->GetCheck() == TRUE )
				{
					strInput  = FillStr(strAcctNo  ,  6);
					strInput += FillStr(strAcctPw  ,  8);
					strInput += FillStr(strSeries  , 32);
					strInput += FillStr(strByslTp  ,  1);
					strInput += FillStr(strPrceTp  ,  1);
					strInput += FillStr(strTrdCond ,  1);
					strInput += FillStr(strOrdP    , 15);
					strInput += FillStr(strOrdQ    , 10);
					strInput += FillStr(strStrTp   ,  1);
					strInput += FillStr(strStopP   , 15);
					strInput += FillStr(strIOCQty  , 10);
					strInput += FillStr(strOptionYn,  1);
					strInput += FillStr(strOrdEftDt,  8);
					strInput += FillStr(strReserved, 60);
				}
				else if ( pBtnFX->GetCheck() == TRUE )
				{
					
				}
			}
			break;
		case 1 :	// 정정
		case 4 :
			{
				if ( pBtnFO->GetCheck() == TRUE )
				{
					strInput  = FillStr(strAcctNo , 11);
					strInput += FillStr(strAcctPw ,  8);
					strInput += FillStr(strSeries , 32);
					strInput += FillStr(strByslTp ,  1);
					strInput += FillStr(strPrceTp ,  1);
					strInput += FillStr(strTrdCond,  1);
					strInput += FillStr(strOrdP   , 13);
					strInput += FillStr(strOrdQ   ,  5);
					strInput += FillStr(strOrdNo  , 07);
					strInput += FillStr(strReserved, 60);
				}
				else if ( pBtnHW->GetCheck() == TRUE )
				{
					strInput  = FillStr(strAcctNo  ,  6);
					strInput += FillStr(strAcctPw  ,  8);
					strInput += FillStr(strSeries  , 32);
					strInput += FillStr(strPrceTp  ,  1);
					strInput += FillStr(strTrdCond ,  1);
					strInput += FillStr(strOrdP    , 15);
					strInput += FillStr(strOrdQ    , 10);
					strInput += FillStr(strOrdNo   , 10);
					strInput += FillStr(strStrTp   ,  1);
					strInput += FillStr(strStopP   , 15);
					strInput += FillStr(strIOCQty  , 10);
					strInput += FillStr(strReserved, 60);
				}
				else if ( pBtnFX->GetCheck() == TRUE )
				{

				}
			}
			break;
		case 2 :	// 취소
		case 5 :
			{
				if ( pBtnFO->GetCheck() == TRUE )
				{
					strInput  = FillStr(strAcctNo , 11);
					strInput += FillStr(strAcctPw ,  8);
					strInput += FillStr(strSeries , 32);
					strInput += FillStr(strByslTp ,  1);
					strInput += FillStr(strPrceTp ,  1);
					strInput += FillStr(strTrdCond,  1);
					strInput += FillStr(strOrdP   , 13);
					strInput += FillStr(strOrdQ   , 05);
					strInput += FillStr(strOrdNo  , 07);
					strInput += FillStr(strReserved, 60);
				}
				else if ( pBtnHW->GetCheck() == TRUE )
				{
					strInput  = FillStr(strAcctNo  ,  6);
					strInput += FillStr(strAcctPw  ,  8);
					strInput += FillStr(strSeries  , 32);
					strInput += FillStr(strPrceTp  ,  1);
					strInput += FillStr(strTrdCond ,  1);
					strInput += FillStr(strOrdP    , 15);
					strInput += FillStr(strOrdQ    , 10);
					strInput += FillStr(strOrdNo   , 10);
					strInput += FillStr(strStrTp   ,  1);
					strInput += FillStr(strStopP   , 15);
					strInput += FillStr(strIOCQty  , 10);
					strInput += FillStr(strReserved, 60);
				}
				else if ( pBtnFX->GetCheck() == TRUE )
				{
					strInput  = FillStr(strAcctNo  ,  6);
					strInput += FillStr(strAcctPw  ,  8);
					strInput += FillStr(strSeries  , 32);
					strInput += FillStr(strOrdNo  ,  20);
				}
			}
			break;
	}

	m_edJumunInput.SetWindowText(strInput);
}

void CHDFCommClientDlg::ShowHWOrdCtrl(BOOL bShow)
{
	GetDlgItem(IDC_STATIC_STRTP   )->ShowWindow(bShow);
	GetDlgItem(IDC_ORD_COMBO_STRTP)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_STOPP   )->ShowWindow(bShow);
	GetDlgItem(IDC_ORD_STOPP      )->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_IOCQTY  )->ShowWindow(bShow);
	GetDlgItem(IDC_ORD_IOCQTY     )->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_OPTIONYN)->ShowWindow(bShow);
	GetDlgItem(IDC_ORD_OPTIONYN   )->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_ORDEFTDT)->ShowWindow(bShow);
	GetDlgItem(IDC_ORD_ORDEFTDT   )->ShowWindow(bShow);
}

void CHDFCommClientDlg::OnCbnSelchangeOrdComboOrdertr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton *pBtnFO = (CButton *)GetDlgItem(IDC_RADIO3);
	CButton *pBtnHW = (CButton *)GetDlgItem(IDC_RADIO4);
	CButton *pBtnFX = (CButton *)GetDlgItem(IDC_RADIO5);

	CEdit *pOrdNo = (CEdit *)GetDlgItem(IDC_ORD_ORDNO);
	CStatic *pStaticOrdNO = (CStatic *)GetDlgItem(IDC_STATIC_ORDNO);
	pOrdNo->ShowWindow(SW_SHOW);
	pStaticOrdNO->ShowWindow(SW_SHOW);

	ShowHWOrdCtrl(FALSE);
	if ( pBtnHW->GetCheck() )
	{
		ShowHWOrdCtrl(TRUE);
	}

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	int nSel = pCombo->GetCurSel();
	switch (nSel)
	{
		case 0 : // 신규
			{
				pOrdNo->ShowWindow(SW_HIDE);
				pStaticOrdNO->ShowWindow(SW_HIDE);

				if ( pBtnFO->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_ORD_CODE_NEW);
				}
				else if ( pBtnHW->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_HW_ORD_CODE_NEW);
				}
				else if ( pBtnFX->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_FX_ORD_CODE_NEW);
				}
			}
			break;
		case 1 : // 정정
			{
				if ( pBtnFO->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_ORD_CODE_MOD);
				}
				else if ( pBtnHW->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_HW_ORD_CODE_MOD);
				}
				else if ( pBtnFX->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_FX_ORD_CODE_MOD);
				}
			}
			break;
		case 2 :
			{
				if ( pBtnFO->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_ORD_CODE_CNL);
				}
				else if ( pBtnHW->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_HW_ORD_CODE_CNL);
				}
				else if ( pBtnFX->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_FX_ORD_CODE_CNL);
				}
			}
			break;
		case 3 :
			{
				pOrdNo->ShowWindow(SW_HIDE);
				pStaticOrdNO->ShowWindow(SW_HIDE);

				if ( pBtnFO->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_ORD_CODE_NEW_CME);
				}
			}
			break;
		case 4 :
			{
				if ( pBtnFO->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_ORD_CODE_MOD_CME);
				}
			}
			break;
		case 5 :
			{
				if ( pBtnFO->GetCheck() == TRUE )
				{
					m_edJumunTR.SetWindowText(DEF_ORD_CODE_CNL_CME);
				}
			}
			break;
	}
}




VOID CHDFCommClientDlg::IR_PrintMsg(LOGMSG_TP tp, BOOL bShow, char* pMsg, ...)
{
	char buff[DEF_LOG_LEN];
	va_list argptr;

	va_start(argptr, pMsg);
	vsprintf_s(buff, pMsg, argptr);
	va_end(argptr);

	g_log.log(tp, buff);

	if (bShow) {
		m_ListOutMsg.InsertString(0, buff);
	}
}






void CHDFCommClientDlg::OnBnClickedButtonTerminate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bInit)
	{
		OnBnClickedLogout();
		DeleteCriticalSection(&m_csAPI);

		CHDFCommClientDlg::OnClose();
		CHDFCommClientDlg::OnDestroy();
	}
	::PostQuitMessage(WM_QUIT);
}

BOOL CHDFCommClientDlg::IR_DBOpen()
{
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_ID", id);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(m_sConfig.GetBuffer(), "DBINFO", "DB_POOL_CNT", cnt);

	m_pDBPool = (VOID*)new CDBPoolAdo(ip, id, pwd, name);
	if (!DBPoolPtr->Init(atoi(cnt)))
	{
		IR_PrintMsg(LOGTP_ERR, TRUE, "DB OPEN FAIL(MSG:%s)", DBPoolPtr->GetMsg());
		IR_PrintMsg(LOGTP_ERR, TRUE, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}
	IR_PrintMsg(LOGTP_ERR, TRUE, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);

	return TRUE;
}

void CHDFCommClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (nIDEvent)
	{
		case ID_TIME_LOGON:
		{
			KillTimer(ID_TIME_LOGON);
			IR_OnBnClickedLogin();
		}
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CHDFCommClientDlg::OnBnClickedButtonChart()
{
	UpdateData(TRUE);

	CString sTrCode = "o44005";
	char buff[1024] = { 0, };
	CHART_44005* p = (CHART_44005*)buff;
	memset(p, 0x20, sizeof(CHART_44005));
	memset(p->keyvalue, 0x20, sizeof(p->keyvalue));

	m_sSymbol.Trim();
	m_sSymbol.MakeUpper();
	memcpy(p->symbol, m_sSymbol.GetBuffer(), m_sSymbol.GetLength());
	memcpy(p->dt, "99999999", sizeof(p->dt));
	memcpy(p->tm, "9999999999", sizeof(p->tm));
	p->next[0] = '0';	//최초

	CString sNowChart;

	// 최초 조회
	if (m_sLastChart == "")
	{
		sNowChart = "분";

		p->dataGb[0] = '2';	// 분차트
		if (m_sLastTmGb == "") {
			GetDlgItem(IDC_EDIT_SYMBOL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CHART)->EnableWindow(FALSE);
			IR_PrintMsg(LOGTP_SUCC, TRUE, "================[%s] 차트조회를 시작합니다. 완료될때까지 기다려주세요========================", m_sSymbol.GetBuffer());

			memcpy(p->tmGb, "001", 3);
			m_sLastTmGb = "1";
			m_nLastChartTp = TP_1MIN;
		}
		else if (m_sLastTmGb == "1") {
			memcpy(p->tmGb, "003", 3);
			m_sLastTmGb = "3";
			m_nLastChartTp = TP_3MIN;
		}
		else if (m_sLastTmGb == "3") {
			memcpy(p->tmGb, "005", 3);
			m_sLastTmGb = "5";
			m_nLastChartTp = TP_5MIN;
		}
		else if (m_sLastTmGb == "5") {
			memcpy(p->tmGb, "010", 3);
			m_sLastTmGb = "10";
			m_nLastChartTp = TP_10MIN;
		}
		else if (m_sLastTmGb == "10") {
			memcpy(p->tmGb, "015", 3);
			m_sLastTmGb = "15";
			m_nLastChartTp = TP_15MIN;
		}
		else if (m_sLastTmGb == "15") {
			memcpy(p->tmGb, "020", 3);
			m_sLastTmGb = "20";
			m_nLastChartTp = TP_20MIN;
		}
		else if (m_sLastTmGb == "20") {
			memcpy(p->tmGb, "060", 3);
			m_sLastTmGb = "60";
			m_nLastChartTp = TP_60MIN;
		}
		else if (m_sLastTmGb == "60") {
			memcpy(p->tmGb, "120", 3);
			m_sLastTmGb = "120";
			m_nLastChartTp = TP_120MIN;

			m_sLastChart = "분";
		}		
	} // if (m_sLastChart == "")

	else if (m_sLastChart == "분")
	{
		sNowChart = "일";
		m_sLastChart = "일";
		p->dataGb[0] = '3';	// 일차트
		memcpy(p->tmGb, "000", 3);
		m_nLastChartTp = TP_DAY;
	}

	else if (m_sLastChart == "일")
	{
		sNowChart = "주";
		m_sLastChart = "주";
		p->dataGb[0] = '4';	// 주차트
		memcpy(p->tmGb, "000", 3);
		m_nLastChartTp = TP_WEEK;
	}

	else if (m_sLastChart == "주")
	{
		sNowChart = "월";
		m_sLastChart = "월";
		p->dataGb[0] = '5';	// 주차트
		memcpy(p->tmGb, "000", 3);
		m_nLastChartTp = TP_MON;
	}

	memcpy(p->dataCnt, "99999", 5);
	memcpy(p->mktGb, "1", 1);	//	전산장
	memcpy(p->realGb, "0", 1);	//	허봉 + 실봉

	//CString sInput = "                  CLU17                           999999999999999999040050200010";

	CString sInput;
	sInput.Format("%.*s", sizeof(CHART_44005), p);

	//CString strNextKey = m_CommAgent.CommGetNextKey(nRqID, "");
	g_nChartFirstReqID = m_CommAgent.CommRqData(sTrCode, sInput, sInput.GetLength(), m_strNextKey);
	IR_PrintMsg(LOGTP_SUCC, TRUE, "차트요청(%s)(%s)(%s)", sNowChart.GetBuffer(), m_sLastTmGb.GetBuffer(), sInput.GetBuffer());
}

