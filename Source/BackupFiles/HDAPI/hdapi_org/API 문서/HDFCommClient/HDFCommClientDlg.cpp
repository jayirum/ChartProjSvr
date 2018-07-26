
// HDFCommClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "HDFCommClient.h"
#include "HDFCommClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����
const CString DEF_ACCT_INFO = "g11004.AQ0101%";

// ����
const CString DEF_TR_CODE = "g11002.DQ0622&";
const CString DEF_FID_CODE = "s20001";
const CString DEF_ORD_CODE_NEW = "g12001.DO1601&";
const CString DEF_ORD_CODE_MOD = "g12001.DO1901&";
const CString DEF_ORD_CODE_CNL = "g12001.DO1701&";

const CString DEF_ORD_CODE_NEW_CME = "g12001.DO2201&";	//CME �ű��ֹ�
const CString DEF_ORD_CODE_CNL_CME = "g12001.DO2001&";	//CME ���
const CString DEF_ORD_CODE_MOD_CME = "g12001.DO2101&";	//CME ����(����,����)�ֹ�

const CString DEF_CHE_LIST = "g11002.DQ0107&";
const CString DEF_MICHE_LIST = "g11002.DQ0104&";

// �ؿ�
const CString DEF_HW_JANGO    = "g11004.AQ0605%";
const CString DEF_HW_FID_CODE = "o51000";
const CString DEF_HW_ORD_CODE_NEW = "g12003.AO0401%";
const CString DEF_HW_ORD_CODE_MOD = "g12003.AO0402%";
const CString DEF_HW_ORD_CODE_CNL = "g12003.AO0403%";
const CString DEF_HW_MSTINFO  = "o51211";

// FX����	//@lhe 2012.06.22
const CString DEF_FX_JANGO    = "g11004.AQ0901%";		// �ڻ곻��
const CString DEF_FX_FID_CODE = "x00001";				// FX������
const CString DEF_FX_ORD_CODE_NEW = "g12003.AO0501%";	// ����,���尡,û���ֹ�
const CString DEF_FX_ORD_CODE_MOD = "g12003.AO0502%";	// ����
const CString DEF_FX_ORD_CODE_CNL = "g12003.AO0503%";	// ����ֹ�

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHDFCommClientDlg ��ȭ ����
CHDFCommClientDlg::CHDFCommClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHDFCommClientDlg::IDD, pParent)
	, m_sUserId(_T(""))
	, m_sUserPw(_T(""))
	, m_sAuthPw(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHDFCommClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUTPUT, m_edOutput);
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

	DDX_Control(pDX, IDC_EDIT1, m_edSeriesO);
	DDX_Control(pDX, IDC_EDIT2, m_edTimeO  );
	DDX_Control(pDX, IDC_EDIT3, m_edClosePO);
	DDX_Control(pDX, IDC_EDIT4, m_edVolumeO);

	DDX_Control(pDX, IDC_GETINFO_INPUT, m_edGetInfo);
}

BEGIN_MESSAGE_MAP(CHDFCommClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_LOGIN, &CHDFCommClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(ID_LOGOUT, &CHDFCommClientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_RQTEST, &CHDFCommClientDlg::OnBnClickedRqtest)
	ON_BN_CLICKED(IDC_SBTEST, &CHDFCommClientDlg::OnBnClickedSbtest)
	ON_BN_CLICKED(IDC_SBREMOVE, &CHDFCommClientDlg::OnBnClickedSbremove)
	ON_BN_CLICKED(IDC_INITCOMM, &CHDFCommClientDlg::OnBnClickedInitcomm)
	ON_BN_CLICKED(IDC_TERMINATE, &CHDFCommClientDlg::OnBnClickedTerminate)
	ON_BN_CLICKED(IDC_RADIO1, &CHDFCommClientDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CHDFCommClientDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_JUMUNCHESUMMIT, &CHDFCommClientDlg::OnBnClickedJumunchesummit)
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
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CHDFCommClientDlg, CDialogEx)
	ON_EVENT(CHDFCommClientDlg, -1, 3, OnDataRecv, VTS_BSTR VTS_I4)
	ON_EVENT(CHDFCommClientDlg, -1, 4, OnGetBroadData, VTS_BSTR VTS_I4)
	ON_EVENT(CHDFCommClientDlg, -1, 5, OnGetMsg, VTS_BSTR VTS_BSTR)
	ON_EVENT(CHDFCommClientDlg, -1, 6, OnGetMsgWithRqId, VTS_I4 VTS_BSTR VTS_BSTR)
	
END_EVENTSINK_MAP()

// CHDFCommClientDlg �޽��� ó����

BOOL CHDFCommClientDlg::OnInitDialog()
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.


	//m_CommAgent.Create("HDF CommAgent", WS_CHILD, CRect(0,0,0,0), this, 1);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);

	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(0);		//@lhe 2012.06.22
	
	// �Ϲ� ��ȸ
	OnBnClickedRadio3();


	m_strOrdNo = "";

	m_strNextKey = "";

	m_edTrCode2.EnableWindow(FALSE);
	m_edInput2.EnableWindow(FALSE);

	m_edGetInfo.SetWindowText("�ý��۱���");

	////
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_BYSLTP);
	pCombo->ResetContent();
	pCombo->AddString("1.�ż�");
	pCombo->AddString("2.�ŵ�");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_PRCETP);
	pCombo->ResetContent();
	pCombo->AddString("1.������");
	pCombo->AddString("2.���尡");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_TRDCOND);
	pCombo->ResetContent();
	pCombo->AddString("1.FAS");
	pCombo->AddString("2.FOK");
	pCombo->AddString("3.FAK");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_STRTP);
	pCombo->ResetContent();
	pCombo->AddString("1.�Ϲ�");
	pCombo->AddString("2.STOP");
	pCombo->AddString("3.OCO");
	pCombo->AddString("4.���ݿ���");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	pCombo->ResetContent();
	pCombo->AddString("�ű�");
	pCombo->AddString("����");
	pCombo->AddString("���");
	pCombo->AddString("CME�ű�");
	pCombo->AddString("CME����");
	pCombo->AddString("CME���");
	pCombo->SetCurSel(0);

	ShowHWOrdCtrl(FALSE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


void CHDFCommClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CHDFCommClientDlg::OnPaint()
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
HCURSOR CHDFCommClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHDFCommClientDlg::OnClose()
{
	CDialog::OnClose();
}

void CHDFCommClientDlg::OnBnClickedInitcomm()
{
	char msg[1024];

	// ����� �ʱ�ȭ �Ѵ�.
	if ( !m_CommAgent.GetSafeHwnd() )
	{
		m_CommAgent.Create("HDF CommAgent", WS_CHILD, CRect(0,0,0,0), this, 2286);
	}

	if ( m_CommAgent.GetSafeHwnd() )
	{
		int nRet = m_CommAgent.CommInit(1);
		
		if ( nRet < 0 )
		{
			sprintf(msg, "������α׷� ���� ����:%d", nRet);
			WriteLog(msg);
		}
		else
		{
			sprintf(msg, "������α׷� ���� ����:%d", nRet);
			WriteLog(msg);
		}
	}
}

void CHDFCommClientDlg::OnBnClickedTerminate()
{
	// ����� ����� �����Ѵ�.
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

	WriteLog("������α׷� ���� ����");
}

void CHDFCommClientDlg::OnDestroy()
{
	OnBnClickedTerminate();

	CDialog::OnDestroy();
}


void CHDFCommClientDlg::OnBnClickedLogin()
{
	// �α����Ѵ�.
	m_editUserID.GetWindowText(m_sUserId);
	m_editPwd.GetWindowText(m_sUserPw);
	m_editAuthPwd.GetWindowText(m_sAuthPw);

	int nRet = m_CommAgent.CommLogin(m_sUserId, m_sUserPw, m_sAuthPw);
	if ( nRet > 0 )
	{
		WriteLog("�α��� ����");

		// �α��� �ݵ�� ȣ��...
		//m_CommAgent.CommAccInfo();
	}
	else
	{
		CString strRet;
		strRet.Format("[%d]", nRet);
		WriteLog("�α��� ����" + strRet);
	}
}

void CHDFCommClientDlg::OnBnClickedLogout()
{
	// �α׾ƿ��Ѵ�.
	int nRet = m_CommAgent.CommLogout(m_sUserId);

	CString strRet;
	strRet.Format("[%d]", nRet);
	if ( nRet< 0 )
	{
		WriteLog("�α׾ƿ� ����" + strRet);
	}
	else
	{
		WriteLog("�α׾ƿ� ����" + strRet);
	}
}

// TR���
void CHDFCommClientDlg::OnBnClickedRadio1()
{
	m_edTrCode.EnableWindow(TRUE);
	m_edInput.EnableWindow(TRUE);
	m_edTrCode2.EnableWindow(FALSE);
	m_edInput2.EnableWindow(FALSE);
	m_edInput3.EnableWindow(FALSE);
}

// FID���
void CHDFCommClientDlg::OnBnClickedRadio2()
{
	m_edTrCode.EnableWindow(FALSE);
	m_edInput.EnableWindow(FALSE);
	m_edTrCode2.EnableWindow(TRUE);
	m_edInput2.EnableWindow(TRUE);
	m_edInput3.EnableWindow(TRUE);
}

// ��ȸ ����
void CHDFCommClientDlg::OnBnClickedRqtest()
{
	int nResult;
	CString sTrCode, sInput;
	CString sFidCode, sFidInput;

	CString strMsg;
	// ��ȸ�� �����͸� ��û�Ѵ�.
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

	strMsg.Format("��û�� RQID[%d]", nResult );
	WriteLog(strMsg);
}

// �ǽð� ���
// ���� : 51:����ȣ��, 51:�ɼ�ȣ��, 65:����ü��, 66:�ɼ�ü��
// �ؿ� : 76:����ȣ��, 82:����ü��
void CHDFCommClientDlg::OnBnClickedSbtest()
{
	int nResult;
	CString sJongmok, sGubun;

	m_edJomgmok.GetWindowText(sJongmok);
	m_edGubun.GetWindowText(sGubun);

	int nGubun = atol(sGubun);

	// ����(�ǽð�) ����Ÿ�� ��û�Ѵ�.
	nResult = m_CommAgent.CommSetBroad((LPCTSTR)sJongmok, nGubun);

	CString strMsg;
	strMsg.Format("�ǽð� ����[%s] ����[%s] ��ϰ��[%d]", sJongmok, sGubun, nResult);
	WriteLog(strMsg);
}

void CHDFCommClientDlg::OnBnClickedSbremove()
{
	CString sJongmok, sGubun;

	m_edJomgmok.GetWindowText(sJongmok);
	m_edGubun.GetWindowText(sGubun);

	int nGubun = atol(sGubun);

	// �ǽð� ������ ������ �����Ѵ�.
	m_CommAgent.CommRemoveBroad(sJongmok, nGubun);
	// ��� �ǽð� ������ ������ ������ ������ "" ��
	//m_CommAgent.CommRemoveBroad("", nGubun);
}

// �ֹ�ü�� ���
void CHDFCommClientDlg::OnBnClickedJumunchesummit()
{
	//���
	CString sID, sAccNo;
	m_editUserID.GetWindowText(sID);
	m_edAccNo.GetWindowText(sAccNo);
	m_CommAgent.CommSetJumunChe(sID, sAccNo);

	WriteLog("�ǽð� �ֹ�ü��/��ü�� ���");
}

// �ֹ�ü�� ����
void CHDFCommClientDlg::OnBnClickedJumunchecancle()
{
	//����
	CString sID, sAccNo;
	m_editUserID.GetWindowText(sID);
	m_edAccNo.GetWindowText(sAccNo);
	m_CommAgent.CommRemoveJumunChe(sID, sAccNo);

	WriteLog("�ǽð� �ֹ�ü��/��ü�� ����");
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
			strMsg.Format("�ֹ���û ��û��ȣ[%d]", nResult);
			WriteLog(strMsg);
		}
	}
	***/

	/***/
	m_edJumunTR.GetWindowText(sTrCode);
	m_edJumunInput.GetWindowText(sInput);
	m_sCode = sTrCode;

	nResult = m_CommAgent.CommJumunSvr(sTrCode, sInput);
	CString strMsg;
	strMsg.Format("�ֹ���û ��û��ȣ[%d]", nResult);
	WriteLog(strMsg);
	/***/
}

void CHDFCommClientDlg::WriteLog(CString strMsg)
{
	CString str = CTime::GetCurrentTime().Format("[%p %I:%M:%S]  ") + strMsg;
	int nCnt = m_ListOutMsg.GetCount();
	m_ListOutMsg.InsertString(nCnt, str);
	if ( nCnt > 0 )
	{
		m_ListOutMsg.SetCurSel(nCnt);
	}
}

void CHDFCommClientDlg::OnDataRecv(CString sTrCode, LONG nRqID)
{
	CString strMsg;

	///////////////////////////////////////////////////////////////////////////////////////////
	// ����
	///////////////////////////////////////////////////////////////////////////////////////////
	if ( sTrCode == DEF_TR_CODE )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		if ( nRepeatCnt > 0 )
		{
			for (int i= 0; i<nRepeatCnt; i++ )
			{
				CString sData = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�����ڵ�");

				WriteLog(sData);
			}

			// CommGetDataDirect ���� �߰�. - 20140416 sivas
			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * 32, 0, "A");
			WriteLog(strBuff);
		}
	}
	else if ( sTrCode == DEF_ORD_CODE_NEW )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString strExchTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������");
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó������");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
		CString strOrdNo  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ֹ���ȣ");
		CString strSeries = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");

		CString strMsg;
		strMsg.Format("�ֹ����� ��ȣ[%d][%s]ó��[%s]���¹�ȣ[%s]�ֹ���ȣ[%s]", nRqID, strExchTp, strProcTp, strAcctNo, strOrdNo);
		WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_FID_CODE )
	{
		// FID�� parsing
		//�����ڵ�(8)+ȣ���ð�(6)+����(9)+�����ŷ���(7)
		/*
		CString strSeries = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");
		CString strTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ȣ�����Žð�");
		CString strCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���簡");
		CString strVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŷ���");

		CString strLastP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����������");
		CString strDate = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������");
		CString strHighP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��");

		m_edSeriesO.SetWindowText(strSeries);
		m_edTimeO.SetWindowText(strTime  );
		m_edClosePO.SetWindowText(strCloseP);
		m_edVolumeO.SetWindowText(strVolume);

		CString strMsg;
		strMsg.Format("RQID[%d] FID ����[%s]�ð�[%s]���簡[%s]�ŷ���[%s]", nRqID, strSeries, strTime, strCloseP, strVolume);
		WriteLog(strMsg);
		*/

		CString	strData000	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");
		CString	strData001	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");
		CString	strData002	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ѱ������");
		CString	strData003	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ѱ۾��");
		CString	strData004	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���������");
		CString	strData005	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������");
		CString	strData006	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������");
		CString	strData007	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ϼ�");
		CString	strData008	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ϼ�");
		CString	strData009	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŷ���");
		CString	strData010	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŷ��ϱ���");
		CString	strData011	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���������̷���ġ");
		CString	strData012	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ذ��ݱ���");
		CString	strData013	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ذ���");
		CString	strData014	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̷а�");
		CString	strData015	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̷а�");
		CString	strData016	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "CD�ݸ�");
		CString	strData017	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ѱ�");
		CString	strData018	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ѱ�");
		CString	strData019	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������갡����");
		CString	strData020	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������갡");
		CString	strData021	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������������");
		CString	strData022	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������");
		CString	strData023	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����ü�����");
		CString	strData024	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϱٿ���ü�����");
		CString	strData025	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ͽ�����ü�����");
		CString	strData026	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����ü����");
		CString	strData027	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϱٿ����ŷ����");
		CString	strData028	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ͽ������ŷ����");
		CString	strData029	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ϲ̰�������������");
		CString	strData030	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������ְ�_SIGN����");
		CString	strData031	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������ְ���");
		CString	strData032	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������ְ�����");
		CString	strData033	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������������_SIGN����");
		CString	strData034	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������������");
		CString	strData035	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����������������");
		CString	strData036	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��ȸ����");
		CString	strData037	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���尡��뱸��");
		CString	strData038	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ǻ���������뱸��");
		CString	strData039	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������������뱸��");
		CString	strData040	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����������");
		CString	strData041	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������������");
		CString	strData042	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "CB������Ѱ�");
		CString	strData043	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "CB�������Ѱ�");
		CString	strData044	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���������Ÿ�ü��ð�");
		CString	strData045	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ͻ�������ٿ�������");
		CString	strData046	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ͻ����������������");
		CString	strData047	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������ٿ���ǥ���ڵ�");
		CString	strData048	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������������ǥ���ڵ�");
		CString	strData049	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŷ�����");
		CString	strData050	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ü��ð�");
		CString	strData051	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���簡");
		CString	strData052	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ð�");
		CString	strData053	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��");
		CString	strData054	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����");
		CString	strData055	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϴ�񱸺�");
		CString	strData056	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϴ��");
		CString	strData057	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŷ���");
		CString	strData058	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŷ����");
		CString	strData059	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϰŷ������");
		CString	strData060	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����");
		CString	strData061	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ǻ�ü�����");
		CString	strData062	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ü�ᱸ��");
		CString	strData063	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ������ŷ���");
		CString	strData064	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż������ŷ���");
		CString	strData065	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�����ü��Ǽ�");
		CString	strData066	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�����ü��Ǽ�");
		CString	strData067	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ٿ���������������");
		CString	strData068	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������������������");
		CString	strData069	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ٿ���ü�����");
		CString	strData070	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������ü�����");
		CString	strData071	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ٿ����ŷ����");
		CString	strData072	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������ŷ����");
		CString	strData073	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ü�����_ȣ������");
		CString	strData074	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ü�����_ȣ������");
		CString	strData075	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ȣ�����Žð�");
		CString	strData076	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ��1");
		CString	strData077	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ��1");
		CString	strData078	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ������1");
		CString	strData079	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ������1");
		CString	strData080	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ���Ǽ�1");
		CString	strData081	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ���Ǽ�1");
		CString	strData082	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ��2");
		CString	strData083	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ��2");
		CString	strData084	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ������2");
		CString	strData085	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ������2");
		CString	strData086	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ���Ǽ�2");
		CString	strData087	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ���Ǽ�2");
		CString	strData088	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ��3");
		CString	strData089	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ��3");
		CString	strData090	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ������3");
		CString	strData091	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ������3");
		CString	strData092	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ���Ǽ�3");
		CString	strData093	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ���Ǽ�3");
		CString	strData094	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ��4");
		CString	strData095	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ��4");
		CString	strData096	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ������4");
		CString	strData097	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ������4");
		CString	strData098	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ���Ǽ�4");
		CString	strData099	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ���Ǽ�4");
		CString	strData100	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ��5");
		CString	strData101	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ��5");
		CString	strData102	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ������5");
		CString	strData103	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ������5");
		CString	strData104	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ���Ǽ�5");
		CString	strData105	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ���Ǽ�5");
		CString	strData106	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ���Ѽ���");
		CString	strData107	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ���Ѽ���");
		CString	strData108	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ���ѰǼ�");
		CString	strData109	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ���ѰǼ�");
		CString	strData110	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���뱸��");
		CString	strData111	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ü������");
		CString	strData112	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̰�����������");
		CString	strData113	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���갡��");
		CString	strData114	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���갡����");
		CString	strData115	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "KOSPI200����");
		CString	strData116	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̷а�");
		CString	strData117	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������");
		CString	strData118	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����BASIS");
		CString	strData119	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̷�BASIS");
		CString	strData120	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̰�����������");
		CString	strData121	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̰��������ð�");
		CString	strData122	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̰���������");
		CString	strData123	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̰�����������");
		CString	strData124	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̰�����������");
		CString	strData125	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "2������");
		CString	strData126	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "1������");
		CString	strData127	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ǻ���");
		CString	strData128	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "1������");
		CString	strData129	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "2������");
		CString	strData130	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����SEQ��ȣ");
		CString	strData131	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ɼ�����");
		CString	strData132	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŷ��������");
		CString	strData133	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������");
		CString	strData134	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��簡��");
		CString	strData135	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��ȸ����");
		CString	strData136	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ǹ������������");
		CString	strData137	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ֱٿ�������");
		CString	strData138	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ATM����");
		CString	strData139	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ʰŷ���");
		CString	strData140	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�űԱ���");
		CString	strData141	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ű�����");
		CString	strData142	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ͻð�");
		CString	strData143	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϰ�");
		CString	strData144	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������");
		CString	strData145	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ÿ����űݱ��ذ�");
		CString	strData146	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ÿ����űݱ��ذ�����");
		CString	strData147	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����ȣ������(����ġ)");
		CString	strData148	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����ȣ������(����ġ)");
		CString	strData149	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ݴ������ذ�(3.00)");
		CString	strData150	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ȣ������(0.01)");
		CString	strData151	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ȣ������(0.05)");
		CString	strData152	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ְ���");
		CString	strData153	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ְ�");
		CString	strData154	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������������");
		CString	strData155	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����������");
		CString	strData156	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϳ��纯����");
		CString	strData157	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ÿ����űݱ��ذ�");
		CString	strData158	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ÿ����űݱ��ذ�����");
		CString	strData159	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "KOSPI�");
		CString	strData160	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��ǥ����ð�");
		CString	strData161	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ÿ");
		CString	strData162	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����");
		CString	strData163	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ÿ");
		CString	strData164	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����");
		CString	strData165	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ο�");
		CString	strData166	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���纯����");
		CString	strData167	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���簡ġ");
		CString	strData168	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ð���ġ");
		CString	strData169	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ�����纯����");
		CString	strData170	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ�����纯����");
		CString	strData171	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������������");
		CString	strData172	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����̷а����׸�");
		CString	strData173	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ÿ���׸�");
		CString	strData174	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������׸�");
		CString	strData175	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ÿ���׸�");
		CString	strData176	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������׸�");
		CString	strData177	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ο����׸�");
		CString	strData178	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŷ���Cal���س��纯����");
		CString	strData179	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŷ����Cal���س��纯����");
		CString	strData180	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŷ���Put���س��纯����");
		CString	strData181	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŷ����Put���س��纯����");
		CString	strData182	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������ð����帶����Upd");
		CString	strData183	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����������񿩺�");
		CString	strData184	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "BATCH���̰�����ϼ�");
		CString	strData185	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ż��ܷ�1");
		CString	strData186	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ż��ܷ�2");
		CString	strData187	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ż��ܷ�3");
		CString	strData188	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ż��ܷ�4");
		CString	strData189	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ż��ܷ�5");
		CString	strData190	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ѽ��ż��ܷ�");
		CString	strData191	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ�������1");
		CString	strData192	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ�������2");
		CString	strData193	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ�������3");
		CString	strData194	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ�������4");
		CString	strData195	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�ȣ�������5");
		CString	strData196	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ�������1");
		CString	strData197	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ�������2");
		CString	strData198	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ�������3");
		CString	strData199	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ�������4");
		CString	strData200	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�ȣ�������5");
		CString	strData201	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "������");
		CString	strData202	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����������ϼ�");
		CString	strData203	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ�����Ѽ���");
		CString	strData204	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż�����Ѽ���");
		CString	strData205	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ�����ѰǼ�");
		CString	strData206	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż�����ѰǼ�");
		CString	strData207	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ͽ�����Ѱ�");
		CString	strData208	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���Ͽ������Ѱ�");
		CString	strData209	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������ֹ�����");
		CString	strData210	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ�������1");
		CString	strData211	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż�������1");
		CString	strData212	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ����Ǽ�1");
		CString	strData213	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż����Ǽ�1");
		CString	strData214	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ�������2");
		CString	strData215	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż�������2");
		CString	strData216	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ����Ǽ�2");
		CString	strData217	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż����Ǽ�2");
		CString	strData218	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ�������3");
		CString	strData219	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż�������3");
		CString	strData220	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ����Ǽ�3");
		CString	strData221	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż����Ǽ�3");
		CString	strData222	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ�������4");
		CString	strData223	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż�������4");
		CString	strData224	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ����Ǽ�4");
		CString	strData225	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż����Ǽ�4");
		CString	strData226	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ�������5");
		CString	strData227	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż�������5");
		CString	strData228	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŵ����Ǽ�5");
		CString	strData229	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ż����Ǽ�5");
		CString	strData230	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����ü�ᰡ��");
		CString	strData231	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̷а���񱸺�");
		CString	strData232	=	m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�̷а����");
		

		WriteLog("[�����ڵ�]" + strData000.Trim());
		WriteLog("[�����ڵ�]" + strData001.Trim());
		WriteLog("[�ѱ������]" + strData002.Trim());
		WriteLog("[�����ѱ۾��]" + strData003.Trim());
		WriteLog("[���������]" + strData004.Trim());
		WriteLog("[��������]" + strData005.Trim());
		WriteLog("[������]" + strData006.Trim());
		WriteLog("[�����ϼ�]" + strData007.Trim());
		WriteLog("[�����ϼ�]" + strData008.Trim());
		WriteLog("[�����ŷ���]" + strData009.Trim());
		WriteLog("[�����ŷ��ϱ���]" + strData010.Trim());
		WriteLog("[���������̷���ġ]" + strData011.Trim());
		WriteLog("[���ذ��ݱ���]" + strData012.Trim());
		WriteLog("[���ذ���]" + strData013.Trim());
		WriteLog("[�̷а�]" + strData014.Trim());
		WriteLog("[�̷а�]" + strData015.Trim());
		WriteLog("[CD�ݸ�]" + strData016.Trim());
		WriteLog("[���Ѱ�]" + strData017.Trim());
		WriteLog("[���Ѱ�]" + strData018.Trim());
		WriteLog("[�������갡����]" + strData019.Trim());
		WriteLog("[�������갡]" + strData020.Trim());
		WriteLog("[������������]" + strData021.Trim());
		WriteLog("[��������]" + strData022.Trim());
		WriteLog("[����ü�����]" + strData023.Trim());
		WriteLog("[���ϱٿ���ü�����]" + strData024.Trim());
		WriteLog("[���Ͽ�����ü�����]" + strData025.Trim());
		WriteLog("[����ü����]" + strData026.Trim());
		WriteLog("[���ϱٿ����ŷ����]" + strData027.Trim());
		WriteLog("[���Ͽ������ŷ����]" + strData028.Trim());
		WriteLog("[���Ϲ̰�������������]" + strData029.Trim());
		WriteLog("[�������ְ�_SIGN����]" + strData030.Trim());
		WriteLog("[�������ְ���]" + strData031.Trim());
		WriteLog("[�������ְ�����]" + strData032.Trim());
		WriteLog("[������������_SIGN����]" + strData033.Trim());
		WriteLog("[��������������]" + strData034.Trim());
		WriteLog("[����������������]" + strData035.Trim());
		WriteLog("[��ȸ����]" + strData036.Trim());
		WriteLog("[���尡��뱸��]" + strData037.Trim());
		WriteLog("[���Ǻ���������뱸��]" + strData038.Trim());
		WriteLog("[��������������뱸��]" + strData039.Trim());
		WriteLog("[����������]" + strData040.Trim());
		WriteLog("[��������������]" + strData041.Trim());
		WriteLog("[CB������Ѱ�]" + strData042.Trim());
		WriteLog("[CB�������Ѱ�]" + strData043.Trim());
		WriteLog("[���������Ÿ�ü��ð�]" + strData044.Trim());
		WriteLog("[���Ͻ�������ٿ�������]" + strData045.Trim());
		WriteLog("[���Ͻ����������������]" + strData046.Trim());
		WriteLog("[��������ٿ���ǥ���ڵ�]" + strData047.Trim());
		WriteLog("[�������������ǥ���ڵ�]" + strData048.Trim());
		WriteLog("[�ŷ�����]" + strData049.Trim());
		WriteLog("[ü��ð�]" + strData050.Trim());
		WriteLog("[���簡]" + strData051.Trim());
		WriteLog("[�ð�]" + strData052.Trim());
		WriteLog("[��]" + strData053.Trim());
		WriteLog("[����]" + strData054.Trim());
		WriteLog("[���ϴ�񱸺�]" + strData055.Trim());
		WriteLog("[���ϴ��]" + strData056.Trim());
		WriteLog("[�����ŷ���]" + strData057.Trim());
		WriteLog("[�����ŷ����]" + strData058.Trim());
		WriteLog("[���ϰŷ������]" + strData059.Trim());
		WriteLog("[�����]" + strData060.Trim());
		WriteLog("[�Ǻ�ü�����]" + strData061.Trim());
		WriteLog("[ü�ᱸ��]" + strData062.Trim());
		WriteLog("[�ŵ������ŷ���]" + strData063.Trim());
		WriteLog("[�ż������ŷ���]" + strData064.Trim());
		WriteLog("[�ŵ�����ü��Ǽ�]" + strData065.Trim());
		WriteLog("[�ż�����ü��Ǽ�]" + strData066.Trim());
		WriteLog("[�ٿ���������������]" + strData067.Trim());
		WriteLog("[������������������]" + strData068.Trim());
		WriteLog("[�ٿ���ü�����]" + strData069.Trim());
		WriteLog("[������ü�����]" + strData070.Trim());
		WriteLog("[�ٿ����ŷ����]" + strData071.Trim());
		WriteLog("[�������ŷ����]" + strData072.Trim());
		WriteLog("[�ŵ�ü�����_ȣ������]" + strData073.Trim());
		WriteLog("[�ż�ü�����_ȣ������]" + strData074.Trim());
		WriteLog("[ȣ�����Žð�]" + strData075.Trim());
		WriteLog("[�ŵ�ȣ��1]" + strData076.Trim());
		WriteLog("[�ż�ȣ��1]" + strData077.Trim());
		WriteLog("[�ŵ�ȣ������1]" + strData078.Trim());
		WriteLog("[�ż�ȣ������1]" + strData079.Trim());
		WriteLog("[�ŵ�ȣ���Ǽ�1]" + strData080.Trim());
		WriteLog("[�ż�ȣ���Ǽ�1]" + strData081.Trim());
		WriteLog("[�ŵ�ȣ��2]" + strData082.Trim());
		WriteLog("[�ż�ȣ��2]" + strData083.Trim());
		WriteLog("[�ŵ�ȣ������2]" + strData084.Trim());
		WriteLog("[�ż�ȣ������2]" + strData085.Trim());
		WriteLog("[�ŵ�ȣ���Ǽ�2]" + strData086.Trim());
		WriteLog("[�ż�ȣ���Ǽ�2]" + strData087.Trim());
		WriteLog("[�ŵ�ȣ��3]" + strData088.Trim());
		WriteLog("[�ż�ȣ��3]" + strData089.Trim());
		WriteLog("[�ŵ�ȣ������3]" + strData090.Trim());
		WriteLog("[�ż�ȣ������3]" + strData091.Trim());
		WriteLog("[�ŵ�ȣ���Ǽ�3]" + strData092.Trim());
		WriteLog("[�ż�ȣ���Ǽ�3]" + strData093.Trim());
		WriteLog("[�ŵ�ȣ��4]" + strData094.Trim());
		WriteLog("[�ż�ȣ��4]" + strData095.Trim());
		WriteLog("[�ŵ�ȣ������4]" + strData096.Trim());
		WriteLog("[�ż�ȣ������4]" + strData097.Trim());
		WriteLog("[�ŵ�ȣ���Ǽ�4]" + strData098.Trim());
		WriteLog("[�ż�ȣ���Ǽ�4]" + strData099.Trim());
		WriteLog("[�ŵ�ȣ��5]" + strData100.Trim());
		WriteLog("[�ż�ȣ��5]" + strData101.Trim());
		WriteLog("[�ŵ�ȣ������5]" + strData102.Trim());
		WriteLog("[�ż�ȣ������5]" + strData103.Trim());
		WriteLog("[�ŵ�ȣ���Ǽ�5]" + strData104.Trim());
		WriteLog("[�ż�ȣ���Ǽ�5]" + strData105.Trim());
		WriteLog("[�ŵ�ȣ���Ѽ���]" + strData106.Trim());
		WriteLog("[�ż�ȣ���Ѽ���]" + strData107.Trim());
		WriteLog("[�ŵ�ȣ���ѰǼ�]" + strData108.Trim());
		WriteLog("[�ż�ȣ���ѰǼ�]" + strData109.Trim());
		WriteLog("[���뱸��]" + strData110.Trim());
		WriteLog("[ü������]" + strData111.Trim());
		WriteLog("[�̰�����������]" + strData112.Trim());
		WriteLog("[���갡��]" + strData113.Trim());
		WriteLog("[���갡����]" + strData114.Trim());
		WriteLog("[KOSPI200����]" + strData115.Trim());
		WriteLog("[�̷а�]" + strData116.Trim());
		WriteLog("[������]" + strData117.Trim());
		WriteLog("[����BASIS]" + strData118.Trim());
		WriteLog("[�̷�BASIS]" + strData119.Trim());
		WriteLog("[�̰�����������]" + strData120.Trim());
		WriteLog("[�̰��������ð�]" + strData121.Trim());
		WriteLog("[�̰���������]" + strData122.Trim());
		WriteLog("[�̰�����������]" + strData123.Trim());
		WriteLog("[�̰�����������]" + strData124.Trim());
		WriteLog("[2������]" + strData125.Trim());
		WriteLog("[1������]" + strData126.Trim());
		WriteLog("[�Ǻ���]" + strData127.Trim());
		WriteLog("[1������]" + strData128.Trim());
		WriteLog("[2������]" + strData129.Trim());
		WriteLog("[����SEQ��ȣ]" + strData130.Trim());
		WriteLog("[�ɼ�����]" + strData131.Trim());
		WriteLog("[�ŷ��������]" + strData132.Trim());
		WriteLog("[������]" + strData133.Trim());
		WriteLog("[��簡��]" + strData134.Trim());
		WriteLog("[��ȸ����]" + strData135.Trim());
		WriteLog("[�Ǹ������������]" + strData136.Trim());
		WriteLog("[�ֱٿ�������]" + strData137.Trim());
		WriteLog("[ATM����]" + strData138.Trim());
		WriteLog("[���ʰŷ���]" + strData139.Trim());
		WriteLog("[�űԱ���]" + strData140.Trim());
		WriteLog("[�ű�����]" + strData141.Trim());
		WriteLog("[���Ͻð�]" + strData142.Trim());
		WriteLog("[���ϰ�]" + strData143.Trim());
		WriteLog("[��������]" + strData144.Trim());
		WriteLog("[�Ÿ����űݱ��ذ�]" + strData145.Trim());
		WriteLog("[�Ÿ����űݱ��ذ�����]" + strData146.Trim());
		WriteLog("[����ȣ������(����ġ)]" + strData147.Trim());
		WriteLog("[����ȣ������(����ġ)]" + strData148.Trim());
		WriteLog("[���ݴ������ذ�(3.00)]" + strData149.Trim());
		WriteLog("[ȣ������(0.01)]" + strData150.Trim());
		WriteLog("[ȣ������(0.05)]" + strData151.Trim());
		WriteLog("[�����ְ���]" + strData152.Trim());
		WriteLog("[�����ְ�]" + strData153.Trim());
		WriteLog("[������������]" + strData154.Trim());
		WriteLog("[����������]" + strData155.Trim());
		WriteLog("[���ϳ��纯����]" + strData156.Trim());
		WriteLog("[�Ÿ����űݱ��ذ�]" + strData157.Trim());
		WriteLog("[�Ÿ����űݱ��ذ�����]" + strData158.Trim());
		WriteLog("[KOSPI�]" + strData159.Trim());
		WriteLog("[��ǥ����ð�]" + strData160.Trim());
		WriteLog("[��Ÿ]" + strData161.Trim());
		WriteLog("[����]" + strData162.Trim());
		WriteLog("[��Ÿ]" + strData163.Trim());
		WriteLog("[����]" + strData164.Trim());
		WriteLog("[�ο�]" + strData165.Trim());
		WriteLog("[���纯����]" + strData166.Trim());
		WriteLog("[���簡ġ]" + strData167.Trim());
		WriteLog("[�ð���ġ]" + strData168.Trim());
		WriteLog("[�ŵ�ȣ�����纯����]" + strData169.Trim());
		WriteLog("[�ŵ�ȣ�����纯����]" + strData170.Trim());
		WriteLog("[������������]" + strData171.Trim());
		WriteLog("[�����̷а����׸�]" + strData172.Trim());
		WriteLog("[��Ÿ���׸�]" + strData173.Trim());
		WriteLog("[�������׸�]" + strData174.Trim());
		WriteLog("[��Ÿ���׸�]" + strData175.Trim());
		WriteLog("[�������׸�]" + strData176.Trim());
		WriteLog("[�ο����׸�]" + strData177.Trim());
		WriteLog("[�ŷ���Cal���س��纯����]" + strData178.Trim());
		WriteLog("[�ŷ����Cal���س��纯����]" + strData179.Trim());
		WriteLog("[�ŷ���Put���س��纯����]" + strData180.Trim());
		WriteLog("[�ŷ����Put���س��纯����]" + strData181.Trim());
		WriteLog("[�������ð����帶����Upd]" + strData182.Trim());
		WriteLog("[�����������񿩺�]" + strData183.Trim());
		WriteLog("[BATCH���̰�����ϼ�]" + strData184.Trim());
		WriteLog("[���ż��ܷ�1]" + strData185.Trim());
		WriteLog("[���ż��ܷ�2]" + strData186.Trim());
		WriteLog("[���ż��ܷ�3]" + strData187.Trim());
		WriteLog("[���ż��ܷ�4]" + strData188.Trim());
		WriteLog("[���ż��ܷ�5]" + strData189.Trim());
		WriteLog("[�Ѽ��ż��ܷ�]" + strData190.Trim());
		WriteLog("[�ŵ�ȣ�������1]" + strData191.Trim());
		WriteLog("[�ŵ�ȣ�������2]" + strData192.Trim());
		WriteLog("[�ŵ�ȣ�������3]" + strData193.Trim());
		WriteLog("[�ŵ�ȣ�������4]" + strData194.Trim());
		WriteLog("[�ŵ�ȣ�������5]" + strData195.Trim());
		WriteLog("[�ż�ȣ�������1]" + strData196.Trim());
		WriteLog("[�ż�ȣ�������2]" + strData197.Trim());
		WriteLog("[�ż�ȣ�������3]" + strData198.Trim());
		WriteLog("[�ż�ȣ�������4]" + strData199.Trim());
		WriteLog("[�ż�ȣ�������5]" + strData200.Trim());
		WriteLog("[������]" + strData201.Trim());
		WriteLog("[�����������ϼ�]" + strData202.Trim());
		WriteLog("[�����ŵ�����Ѽ���]" + strData203.Trim());
		WriteLog("[�����ż�����Ѽ���]" + strData204.Trim());
		WriteLog("[�����ŵ�����ѰǼ�]" + strData205.Trim());
		WriteLog("[�����ż�����ѰǼ�]" + strData206.Trim());
		WriteLog("[���Ͽ�����Ѱ�]" + strData207.Trim());
		WriteLog("[���Ͽ������Ѱ�]" + strData208.Trim());
		WriteLog("[��������ֹ�����]" + strData209.Trim());
		WriteLog("[�����ŵ�������1]" + strData210.Trim());
		WriteLog("[�����ż�������1]" + strData211.Trim());
		WriteLog("[�����ŵ����Ǽ�1]" + strData212.Trim());
		WriteLog("[�����ż����Ǽ�1]" + strData213.Trim());
		WriteLog("[�����ŵ�������2]" + strData214.Trim());
		WriteLog("[�����ż�������2]" + strData215.Trim());
		WriteLog("[�����ŵ����Ǽ�2]" + strData216.Trim());
		WriteLog("[�����ż����Ǽ�2]" + strData217.Trim());
		WriteLog("[�����ŵ�������3]" + strData218.Trim());
		WriteLog("[�����ż�������3]" + strData219.Trim());
		WriteLog("[�����ŵ����Ǽ�3]" + strData220.Trim());
		WriteLog("[�����ż����Ǽ�3]" + strData221.Trim());
		WriteLog("[�����ŵ�������4]" + strData222.Trim());
		WriteLog("[�����ż�������4]" + strData223.Trim());
		WriteLog("[�����ŵ����Ǽ�4]" + strData224.Trim());
		WriteLog("[�����ż����Ǽ�4]" + strData225.Trim());
		WriteLog("[�����ŵ�������5]" + strData226.Trim());
		WriteLog("[�����ż�������5]" + strData227.Trim());
		WriteLog("[�����ŵ����Ǽ�5]" + strData228.Trim());
		WriteLog("[�����ż����Ǽ�5]" + strData229.Trim());
		WriteLog("[����ü�ᰡ��]" + strData230.Trim());
		WriteLog("[�̷а���񱸺�]" + strData231.Trim());
		WriteLog("[�̷а����]" + strData232.Trim());



		//AfxMessageBox(strLastP + "[" + strHighP + "]");
	}
	else if ( sTrCode == DEF_CHE_LIST )
	{
		CString strMsg;
		strMsg.Format("���� ü�᳻�� : ��ȸ[%s]", DEF_CHE_LIST);
		WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ֹ���ȣ");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");
			CString strMntMgn  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ÿű���");

			strAcctNm.TrimRight();
			strTrstMgn.TrimRight();
			strMsg.Format("���¹�ȣ[%s] �ֹ���ȣ[%s] ����[%s] �Ÿű���[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			WriteLog(strMsg);
		}
		strMsg.Format("ü�����[%d]", nRepeatCnt);
		WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_MICHE_LIST )
	{
		CString strMsg;
		strMsg.Format("���� ��ü�᳻�� : ��ȸ[%s]", DEF_MICHE_LIST);
		WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ֹ���ȣ");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");
			CString strMntMgn  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�Ÿű���");

			strAcctNm.TrimRight();
			strTrstMgn.TrimRight();
			strMsg.Format("���¹�ȣ[%s] �ֹ���ȣ[%s] ����[%s] �Ÿű���[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			WriteLog(strMsg);
		}
		strMsg.Format("��ü�����[%d]", nRepeatCnt);
		WriteLog(strMsg);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	// �ؿ�
	///////////////////////////////////////////////////////////////////////////////////////////
	else if ( sTrCode == DEF_HW_JANGO )
	{
		CString strMsg;
		strMsg.Format("�ؿ� ��Ź�ڻ� �� ���ű� ����[%s]", DEF_HW_JANGO);
		WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "��ȭ����");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "��Ź���Ѿ�");
			CString strEntrCh = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "��Ź���ܾ�");

			strAcctNm.TrimRight();
			strMsg.Format("��ȭ����[%s] ��Ź���Ѿ�[%s] ��Ź���ܾ�[%s]", strAcctNm, strEntrTot, strEntrCh);
			WriteLog(strMsg);
		}
	}
	else if ( sTrCode == DEF_HW_FID_CODE )
	{
		CString strMsg;
		strMsg.Format("�ؿ� ���簡�ü�����[%s]", DEF_HW_FID_CODE);
		WriteLog(strMsg);

		CString strSeries = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�"); 
		CString strTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ð�"); //�ؿܽü� ��ȸ ���� ���� - 20160830 sivas
		CString strCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���簡");
		CString strVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ŷ���");

		m_edSeriesO.SetWindowText(strSeries);
		m_edTimeO.SetWindowText(strTime  );
		CString strFormatPrice = strCloseP;
		strFormatPrice = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);
		m_edClosePO.SetWindowText(strFormatPrice);
		m_edVolumeO.SetWindowText(strVolume);

		strMsg.Format("FID ����[%s]�ð�[%s]���簡[%s]�ŷ���[%s]", strSeries, strTime, strCloseP, strVolume);
		WriteLog(strMsg);

		CString strType1, strType2, strType3, strType4;
		strCloseP = strCloseP;//"1241300";
		strType1 = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);
		strType2 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType1, 1);
		strType3 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType2, 2);
		strType4 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType3, 0);
		strMsg.Format("FID Recv[%s]->[%s]->[%s]->[%s]->[%s]", strCloseP, strType1, strType2, strType3, strType4);

		WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_HW_ORD_CODE_NEW )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString strExchTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������");
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���ڵ�");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���޽���");
		CString strOrdNo  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ֹ���ȣ");

		CString strMsg;
		strMsg.Format("�ֹ����� ��ȣ[%d][%s]ó��[%s]���¹�ȣ[%s]�ֹ���ȣ[%s]", nRqID, strExchTp, strProcTp, strAcctNo, strOrdNo);
		WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_HW_MSTINFO )
	{
		// OCX���� /mst/JMCODE.cod ������ ������Ų��.
		WriteLog("�ؿ� ���� ���� ��û �Ϸ�!!!");
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	// FX����		//@lhe 2012.06.22
	///////////////////////////////////////////////////////////////////////////////////////////
	else if ( sTrCode == DEF_FX_JANGO )
	{
		CString strMsg;
		strMsg.Format("FX �������� : �ڻ곻����ȸ[%s]", DEF_FX_JANGO);
		WriteLog(strMsg);

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¸�");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ź�Ѿ�");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ź���ű�");
			CString strMntMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������ű�");

			strAcctNm.TrimRight();
			strMsg.Format("���¸�[%s] ��Ź�Ѿ�[%s] ��Ź���ű�[%s] �������ű�[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			WriteLog(strMsg);
		}
	}
	else if ( sTrCode == DEF_FX_FID_CODE )
	{
		CString strMsg;
		strMsg.Format("FX �����͵���Ÿ[%s]", DEF_FX_FID_CODE);
		WriteLog(strMsg);

		CString strSeries = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");
		CString strOffer  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ŵ�����");
		CString strBid	  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ż�����");
		CString strTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ȣ���ð�");

		m_edSeriesO.SetWindowText(strSeries);
		m_edTimeO.SetWindowText(strTime  );
		m_edClosePO.SetWindowText(strOffer);
		m_edVolumeO.SetWindowText(strBid);

		strMsg.Format("FID ����[%s]�ð�[%s]�ŵ�����[%s]�ż�����[%s]", strSeries, strTime, strOffer, strBid);
		WriteLog(strMsg);
	}
	else if ( sTrCode == DEF_FX_ORD_CODE_NEW )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString strExchTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������");
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���ڵ�");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���޽���");
		CString strOrdNo  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ֹ���ȣ");

		CString strMsg;
		strMsg.Format("�ֹ����� ��ȣ[%d][%s]ó��[%s]���¹�ȣ[%s]�ֹ���ȣ[%s]", nRqID, strExchTp, strProcTp, strAcctNo, strOrdNo);
		WriteLog(strMsg);
	}
	else if (sTrCode == "o44005" )
	{
		// �ؿ� ��Ʈ ��ȸ
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec2");

		CString strMsg;
		strMsg.Format("���䰳��[%d]", nRepeatCnt);
		WriteLog(strMsg);

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "����");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "�ð�");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "����");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "ü�ᷮ");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			WriteLog(strMsg);
		}
	}
	// 20140328 �������� ���� �κ� �߰�. - 20140328 sivas
	else if ( sTrCode == DEF_ACCT_INFO )
	{
		CString strMsg = "�������� ���ſϷ�!!!";
		WriteLog(strMsg);

		// ���� ���� �߰�. - 20140331 sivas
		typedef	struct
		{
			char 	szAcctNo[11];		// ���¹�ȣ
			char	szAcctNm[30];		// ���¸�
			char	szAcctGb[01];		// ���±���  '1': �ؿ�, '2': FX, '9':����
		}HDF_ACCOUNT_UNIT;

		typedef struct  
		{
			char szCount[ 5];
			HDF_ACCOUNT_UNIT *pHdfAccUnit;
		}HDF_ACCOUNT_INFO;

		HDF_ACCOUNT_INFO *pHdfAccInfo = NULL;
		HDF_ACCOUNT_UNIT *pHdfAccUnit = NULL;
		CString strRcvBuff = m_CommAgent.CommGetAccInfo();

		pHdfAccInfo = (HDF_ACCOUNT_INFO *)strRcvBuff.GetBuffer();
		CString strCount(pHdfAccInfo->szCount, sizeof(pHdfAccInfo->szCount));
		for ( int i=0; i<atoi(strCount); i++)
		{
			pHdfAccUnit = (HDF_ACCOUNT_UNIT *)(pHdfAccInfo->szCount + sizeof(pHdfAccInfo->szCount) +(sizeof(HDF_ACCOUNT_UNIT) * i));
			CString strAcctNo(pHdfAccUnit->szAcctNo, sizeof(pHdfAccUnit->szAcctNo));
			CString strAcctNm(pHdfAccUnit->szAcctNm, sizeof(pHdfAccUnit->szAcctNm));
			CString strAcctGb(pHdfAccUnit->szAcctGb, sizeof(pHdfAccUnit->szAcctGb));// ���� ���� �߰�. - 20140331 sivas

			strMsg.Format("[%s][%s][%s]", strAcctNo, strAcctNm, strAcctGb);
			WriteLog(strMsg);
		}
	}
	else if ( sTrCode == "l41601" )
	{
		// input = "1012014051620020"
		// req = "000001002003004005006007008009"
		int nRepeatCnt = 20;	// ��ȸ�� ��û�� ����
		if ( nRepeatCnt > 0 )
		{
			typedef struct 
			{
				char		baseid[8];		/*  [�����ڻ�ID]�����ڻ�ID     */
				char		date[8];		/*  [����]����     */
				char		price[6];		/*  [���簡]���簡     */
				char		sign[1];		/*  [��񱸺�]��񱸺�     */
				char		change[6];		/*  [���]���     */
				char		open[6];		/*  [�ð�]�ð�     */
				char		high[6];		/*  [��]��     */
				char		low[6];			/*  [����]����     */
				char		volume[15];		/*  [�����ŷ���]�����ŷ���     */
				char		jnilclose[6];	/*  [��������]     */

			}HDF_I41601;
			int nBuffSize = sizeof(HDF_I41601);
			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");

			// Struct��ŭ �߶� ����ϼ���...

			WriteLog(strBuff);
		}
	}
	else if ( sTrCode == "g11004.AQ0207%" )
	{
		CString strProcTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���ڵ�");
		CString strProMsg = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���޽���");
		CString strAcctNo = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
		CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¸�");
		CString strTp = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����");
		CString strYN  = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��ġ����");

		CString strBuff;
		strBuff.Format("[%s][%s][%s][%s][%s][%s]", strProcTp, strProMsg, strAcctNo, strAcctNm, strTp, strYN);

		WriteLog(strBuff);
	}
	else if ( sTrCode == "o51100" )
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�����ڵ�");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�����");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "���簡");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "���ϴ�񱸺�");

			CString strMsg;
			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			WriteLog(strMsg);
		}
	}
	else if (sTrCode == "o51200" )
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "����");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ð�");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "����");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "ü�ᷮ");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			WriteLog(strMsg);
		}

		int nBuffSize = 129 + 16;	// Fid��ȸ�� �ʵ� �������� ������ 1�ڸ��� �����Ƿ� �� �ʵ� ��ŭ �����ش�.
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		WriteLog(strMsg);

		CString strNextKey = m_CommAgent.CommGetNextKey(nRqID, "");
		if ( strNextKey != "" )
		{
			int nRet = MessageBox( "��ȸ�� ��� �ұ��?", "���뼱��", MB_YESNO);
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
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ���ȣ");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "���¹�ȣ");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�����ڵ�");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ�����");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			WriteLog(strMsg);
		}

		int nBuffSize = 226;
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		WriteLog(strMsg);
	}
	else if ( sTrCode == "g11004.AQ0408%" )
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sData01 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "ó������");
			CString sData02 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ���ȣ");
			CString sData03 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ�/ü�ᱸ��");
			CString sData04 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�����ڵ�");
			CString sData05 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�Ÿű���");
			CString sData06 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ�����");
			CString sData07 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "��������");
			CString sData08 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "��Ҽ���");
			CString sData09 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "ü�����");
			CString sData10 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ��ܷ�");
			CString sData11 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ�/ü�ᰡ��");
			CString sData12 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "��������");
			CString sData13 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "����ֹ�����");
			CString sData14 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ֹ���������");
			CString sData15 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ŷ�������/�źα���");
			CString sData16 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ŷ�������/�ź�ó���ð�");
			CString sData17 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ŷ���������ȣ/�ź��ڵ�");
			CString sData18 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�ŷ����ֹ�����");
			CString sData19 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�۾�����");
			CString sData20 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�۾��ð�");
			CString sData21 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�۾����");
			CString sData22 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�缳IP");
			CString sData23 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "����IP");
			CString sData24 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "ó������");
			CString sData25 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "���ʿ��ֹ���ȣ");
			CString sData26 = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "stop����");


			strMsg.Format("[%s][%s][%s][%s]", sData01, sData02, sData03, sData04);
			WriteLog(strMsg);
		}

		int nBuffSize = 226;
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		WriteLog(strMsg);


		CString strNextKey = m_CommAgent.CommGetNextKey(nRqID, "");
		if ( strNextKey != "" )
		{
			int nRet = MessageBox( "��ȸ�� ��� �ұ��?", "���뼱��", MB_YESNO);
			if ( nRet == IDYES )
			{
				m_strNextKey = strNextKey;
				OnBnClickedRqtest();
			}
		}
	}
	else if ( sTrCode == "o44011" )	// ���� ����ð�
	{
		CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��������");
		CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ð�");

		strMsg.Format("������¥[%s]�����ð�[%s]", sDate, sTime);
		WriteLog(strMsg);
	}
	else if ( sTrCode == "n51003") 
	{
		// �������� ��ȸ
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����");

		strMsg.Format("[%s]", sTemp1);
		WriteLog(strMsg);

		CString strNextKey = m_CommAgent.CommGetNextKey(nRqID, "");
		if ( strNextKey != "" )
		{
			int nRet = MessageBox( "n51003 ��ȸ�� ��� �ұ��?", "���뼱��", MB_YESNO);
			if ( nRet == IDYES )
			{
				m_strNextKey = strNextKey;
				OnBnClickedRqtest();
			}
		}
	}
	else if ( sTrCode == "g11002.DQ0321&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���ڵ�");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ó���޽���");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
		CString sTemp4   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��ġ����");
		CString sTemp5   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���±���");

		strMsg.Format("ó���ڵ�[%s]ó���޽���[%s]���¹�ȣ[%s]��ġ����[%s]���±���[%s]", sTemp1, sTemp2, sTemp3, sTemp4, sTemp5);
		WriteLog(strMsg);
	}
	else if ( sTrCode == "g11002.DQ0110&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¸�");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����");

		strMsg.Format("���¹�ȣ[%s]���¸�[%s]����[%s]", sTemp1, sTemp2, sTemp3);
		WriteLog(strMsg);
	}
	else if ( sTrCode == "n51006" )
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		CString sLen   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��ü������");

		int nBuffSize = 10+1+255+atoi(sLen);
		CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nBuffSize, 0, "A");
		strMsg.Format("[%s]", strBuff);
		WriteLog(strMsg);
		AfxMessageBox(strMsg);
	}
	else if ( sTrCode == "l41600" )
	{
		/***
		if ( nRepeatCnt > 0 )
		{
			typedef struct 
			{
				char		baseid[8];		//  [�����ڻ�ID]�����ڻ�ID
				char		date[8];		//  [����]����
				char		price[6];		//  [���簡]���簡
				char		sign[1];		//  [��񱸺�]��񱸺�
				char		change[6];		//  [���]���
				char		open[6];		//  [�ð�]�ð�
				char		high[6];		//  [��]��
				char		low[6];			//  [����]����
				char		volume[15];		//  [�����ŷ���]�����ŷ���
				char		jnilclose[6];	//  [��������]

			}HDF_I41601;
			int nBuffSize = sizeof(HDF_I41601);
			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
			
			// Struct��ŭ �߶� ����ϼ���...
			WriteLog(strBuff);
		}
		***/

		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		CString sLen1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ǰ���");
		CString sLen2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "ü��ð�");
		CString sLen3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���簡");

		WriteLog(sLen1 + sLen2 + sLen3);
	}
	else if ( sTrCode ==  "g11002.CQ0101&")
	{
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");
		for (int i=0; i<nRepeatCnt; i++)
		{
			CString strAcctNm = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ID");
			CString strEntrTot = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
			CString strTrstMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ⱸ��");
			CString strMntMgn = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "TMP1");

			strAcctNm.TrimRight();
			strMsg.Format("�����ID[%s] ���¹�ȣ[%s] ���ⱸ��[%s] TMP1[%s]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
			WriteLog(strMsg);
		}
	}
	else if ( sTrCode == "o44010" )
	{
		// �ؿ� ��Ʈ ��ȸ
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec2");

		CString sRcvCnt   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ź�����");
		
		nRepeatCnt = atoi(sRcvCnt);

		CString strMsg;
		strMsg.Format("���䰳��[%d]", nRepeatCnt);
		WriteLog(strMsg);

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDate   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "��������");
			CString sTime   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "���۽ð�");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "����");
			CString sVolume = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "ü�ᷮ");

			strMsg.Format("[%s][%s][%s][%s]", sDate, sTime, sCloseP, sVolume);
			WriteLog(strMsg);
		}
	}
	else if ( sTrCode == "v90001" )
	{
		// �������� ��û ���.
		long nFileSize = atol(m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����ũ��"));
		CString strMsg;
		strMsg.Format("����ũ��[%d]", nFileSize);
		WriteLog(strMsg);


		CString strFileNm =  m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���ϸ�");
		CString strProcCd =  m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�����ڵ�");

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
					strMsg.Format("%sȭ�� ������ �����Ͽ����ϴ�. ", strCommonFileName);
					WriteLog(strMsg);
					return;
				}
			}

			CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 128+4+8, nFileSize, 0, "A");
			commonfile.Write(strBuff, nFileSize);
			commonfile.Close();
		}

		WriteLog("ó���Ϸ�");
	}
	else if ( sTrCode == "g11002.DQ0242&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ź�ݾ�-�Ѿ�");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�ֹ����ɱݾ�");

		strMsg.Format("���¹�ȣ[%s]��Ź�ݾ�-���ϼ���[%s]�ֹ����ɱݾ�[%s]", sTemp1, sTemp2, sTemp3);
		WriteLog(strMsg);
	}
	else if ( sTrCode == "g11002.DQ1305&" || sTrCode == "g11002.DQ1306&" ) 
	{
		CString strMsg;
		int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec1");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "���¹�ȣ");
			CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "����");
			CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", i, "�򰡼���");

			strMsg.Format("���¹�ȣ[%s]����[%s]�򰡼���[%s]", sTemp1, sTemp2, sTemp3);
			WriteLog(strMsg);
		}
	}
	else if ( sTrCode == "g11002.DQ1303&") 
	{
		CString sTemp1   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "���¹�ȣ");
		CString sTemp2   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�������ݾ�");
		CString sTemp3   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "�򰡿�Ź�Ѿ�");

		strMsg.Format("���¹�ȣ[%s]��Ź�ݾ�����[%s]�򰡿�Ź�Ѿ�[%s]", sTemp1, sTemp2, sTemp3);
		WriteLog(strMsg);
	}
	else if ( sTrCode == "v90003" )
	{
		int nRepeatCnt = atoi(m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "��Ʈī��Ʈ"));

		CString strNextKey = m_CommAgent.CommGetData(sTrCode, -1, "OutRec1", 0, "����Ű");
		CString strMsg;
		strMsg.Format("���䰳��[%d]����Ű[%s]", nRepeatCnt, strNextKey);
		WriteLog(strMsg);

		nRepeatCnt = m_CommAgent.CommGetRepeatCnt(sTrCode, -1, "OutRec2");

		for (int i=0; i<nRepeatCnt; i++)
		{
			CString sDtTm   = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "��¥�ð�");
			CString sCloseP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "����");
			CString sOP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "�ð�");
			CString sHP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "��");
			CString sLP = m_CommAgent.CommGetData(sTrCode, -1, "OutRec2", i, "����");

			strMsg.Format("DtTm[%s] C[%s] O[%s] H[%s] L[%s]", sDtTm, sCloseP, sOP, sHP, sLP);
			WriteLog(strMsg);
		}
	}
}

void CHDFCommClientDlg::OnGetBroadData(CString strKey, LONG nRealType)
{
	switch(nRealType)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////
		// ����
		/////////////////////////////////////////////////////////////////////////////////////////////////
		case 51:	// ����ȣ��
			{
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
			}
			break;
		case 52:	// �ɼ�ȣ��
			{
				
			}
			break;
		case 58: //��ǰ����ȣ���ǽð�
			{
				CString kfutcode = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "kfutcode");
				CString hotime   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "hotime");
				CString offerho1 = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "offerho1");
				CString bidho1   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "bidho1");

				kfutcode.TrimRight();
				hotime.TrimRight();
				offerho1.TrimRight();
				bidho1.TrimRight();

				CString strMsg;
				strMsg.Format("ȣ���ǽð� kfutcode[%s]hotime[%s]offerho1[%s]bidho1[%s]", kfutcode, hotime, offerho1, bidho1);
				WriteLog(strMsg);
			}
			break;
		case 65 :	// ����ü��
		case 71 :	// ��ǰ����ü��
			{
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strTime   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ü��ð�");
				CString strCloseP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "���簡");
				CString strVolume = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ŷ���");

				m_edSeriesO.SetWindowText(strSeries);
				m_edTimeO.SetWindowText(strTime  );
				m_edClosePO.SetWindowText(strCloseP);
				m_edVolumeO.SetWindowText(strVolume);
			}
			break;
		case 66:	// �ɼ�ü��
			{

			}
			break;
		case 181:	// �ֹ�����
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");

				strOrdNo.TrimRight();
				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();

				CString strMsg;
				strMsg.Format("�ֹ����� ��ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
				WriteLog(strMsg);

				m_strOrdNo = strOrdNo;
			}
			break;
		case 182:	// ��ü��
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strRemQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ܷ�");

				strOrdNo.TrimRight();
				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();
				strRemQ.TrimRight();

				if ( m_strOrdNo == strOrdNo )
				{
					CString strMsg;
					strMsg.Format("��ü�� �ֹ���ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]�ܷ�[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ, strRemQ);
					WriteLog(strMsg);
				}
			}
			break;
		case 183:	// �̰���
			{
				CString strMsg;
				strMsg.Format("�����ȣ[%d] ����!", nRealType);
				WriteLog(strMsg);
			}
			break;
		case 185:	// ü��
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strTrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ü�ᰡ��");
				CString strTrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ü�����");
				CString strTrdTm  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ü��ð�");

				if ( m_strOrdNo == strOrdNo )
				{
					CString strMsg;
					strMsg.Format("ü�� �ֹ���ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]ü��ð�[%s]", strOrdNo, strSeries, strBySlTp, strTrdP, strTrdQ, strTrdTm);
					WriteLog(strMsg);
				}
			}
			break;									        /////////////////////////////////////////////////////////////////////////////////////////
		// �ؿ�				        /////////////////////////////////////////////////////////////////////////////////////////
		case 75 :	//�ؿ�ȣ��
			{

			}
			break;
		case 82 :	//�ؿ�ü��
			{
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strTime   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "����ü��ð�");
				CString strCloseP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ü�ᰡ");
				CString strVolume = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ŷ���");

				m_edSeriesO.SetWindowText(strSeries);
				m_edTimeO.SetWindowText(strTime  );
				CString strFormatPrice = strCloseP;
				strFormatPrice = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);
				m_edClosePO.SetWindowText(strFormatPrice);
				m_edVolumeO.SetWindowText(strVolume);

				CString strType1, strType2, strType3, strType4;
				strCloseP = strCloseP;//"1241300";
				strType1 = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);
				strType2 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType1, 1);
				strType3 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType2, 2);
				strType4 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType3, 0);

				CString strMsg;
				strMsg.Format("Real 82 Recv[%s]->[%s]->[%s]->[%s]->[%s]", strCloseP, strType1, strType2, strType3, strType4);

				WriteLog(strMsg);

				CString strBuff = m_CommAgent.CommGetDataDirect(strKey, nRealType, 0, 230, 0, "A");
				CString strSeries1(strBuff, 10);
				strMsg = strSeries1;
				WriteLog(strMsg);
			}
			break;
		case 196 : //�ؿ��ֹ�����
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");

				strOrdNo.Trim();
				strSeries.Trim();
				strOrdP.Trim();
				strOrdQ.Trim();

				CString strErrCd = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ó���ڵ�");
				CString strErrMsg= m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ó���޼���");
				strErrCd.Trim();
				strErrMsg.Trim();

				CString strMsg;

				strMsg.Format("[%d] �ֹ������ڵ�[%s][%s]", nRealType, strErrCd, strErrMsg);
				WriteLog(strMsg);

				strMsg.Format("[%d] �ֹ����� ��ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]", nRealType, strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
				WriteLog(strMsg);

				m_strOrdNo = strOrdNo;
			}
			break;
		case 186 : //�ؿܹ�ü�᳻��
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");

				strOrdNo.TrimRight();
				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();

				if ( m_strOrdNo == strOrdNo )
				{
					CString strMsg;
					strMsg.Format("��ü�� �ֹ���ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					//WriteLog(strMsg);
				}
			}
			break;
		case 187 : //�ؿܹ̰�������
			{

			}
			break;
		case 189 : //�ؿ�ü�᳻��
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");

				strOrdNo.TrimRight();
				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();

				if ( m_strOrdNo == strOrdNo )
				{
					CString strMsg;
					strMsg.Format("ü�� �ֹ���ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					WriteLog(strMsg);
				}
			}
			break;
		// FX����		//@lhe 2012.05.16
		case 171 : //FX �ü�
			{
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strTime   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ȣ���ð�");
				CString strOffer = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ŵ�ȣ��");
				CString strBid = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ż�ȣ��");

				m_edSeriesO.SetWindowText(strSeries);
				m_edTimeO.SetWindowText(strTime  );
				m_edClosePO.SetWindowText(strOffer);
				m_edVolumeO.SetWindowText(strBid);

				CString strMsg;
				strMsg.Format("FX�ü�  ����[%s] ȣ���ð�[%s] �ŵ�ȣ��[%s]�ż�ȣ��[%s]",strSeries, strTime, strOffer, strBid);

				WriteLog(strMsg);
			}
			break;
		case 191 : //FX��ü�᳻��
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");

				strOrdNo.TrimRight();
				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();

				if ( m_strOrdNo == strOrdNo )
				{
					CString strMsg;
					strMsg.Format("��ü�� �ֹ���ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					WriteLog(strMsg);
				}
			}
			break;
		case 192 : //FX��û�곻��
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "���԰���");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "���Լ���");

				strOrdNo.TrimRight();
				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();

				if ( m_strOrdNo == strOrdNo )
				{
					CString strMsg;
					strMsg.Format("��û�� �ֹ���ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					WriteLog(strMsg);
				}
			}
			break;
		case 193 : //FXû�곻��
			{
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "û�갡��");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "û�����");

				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();

				CString strMsg;
				strMsg.Format("û�� ����[%s]�Ÿ�[%s]����[%s]����[%s]", strSeries, strBySlTp, strOrdP, strOrdQ);
				WriteLog(strMsg);
			}
			break;
		case 197 : //FX�ֹ�����
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strErrcd = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ó���ڵ�");
				CString strErrMsg = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "ó���޼���");

				strOrdNo.TrimRight();
				strErrcd.TrimRight();
				strErrMsg.TrimRight();

				CString strMsg;
				strMsg.Format("�ֹ����� ��ȣ[%s]ó���ڵ�[%s]ó���޽���[%s]", strOrdNo, strErrcd, strErrMsg);
				WriteLog(strMsg);

				m_strOrdNo = strOrdNo;
			}
			break;
		case 101:
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "����");
				CString strErrcd = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "���Ϻ�");
				CString strErrMsg = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����");

				strOrdNo.TrimRight();
				strErrcd.TrimRight();
				strErrMsg.TrimRight();

				CString strMsg;
				strMsg.Format("����[%s]���Ϻ�[%s]�����[%s]", strOrdNo, strErrcd, strErrMsg);
				WriteLog(strMsg);
			}
			break;
		case 199:
			{
				CString strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ���ȣ");
				CString strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�����ڵ�");
				CString strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�Ÿű���");
				CString strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");
				CString strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ�����");

				strOrdNo.TrimRight();
				strSeries.TrimRight();
				strOrdP.TrimRight();
				strOrdQ.TrimRight();

				CString strMsg;
				strMsg.Format("Yes��ü�� �ֹ���ȣ[%s]����[%s]�Ÿ�[%s]����[%s]����[%s]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
				WriteLog(strMsg);
			}
			break;
		case 208:
			{
				CString strBuff = m_CommAgent.CommGetDataDirect("0", nRealType, 0, 15, 0, "A");
				WriteLog(strBuff);
			}
			break;
		case 184 :	// �����ǽð��ܰ�
		case 188 :	// �ؿܽǽð��ܰ�
			{
				CString strAccno  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "���¹�ȣ");
				CString strAmt	= m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "�ֹ����ɱݾ�");
				CString strMsg;
				strMsg.Format("�����ȣ[%d]���¹�ȣ[%s]�ֹ����ɱݾ�[%s]", nRealType, strAccno, strAmt);
				WriteLog(strMsg);
			}
			break;
		default:
			{
				CString strMsg;
				strMsg.Format("[%d] �˼����� �ǽð� ��û��", nRealType);
				WriteLog(strMsg);
			}
			break;
	}
}

void CHDFCommClientDlg::OnGetMsg(CString strCode, CString strMsg)
{
	CString strLog;
	strLog.Format("[%s][%s]", strCode, strMsg);
	//WriteLog(strLog);
}

void CHDFCommClientDlg::OnGetMsgWithRqId(int nRqId, CString strCode, CString strMsg)
{
	CString strLog;
	strLog.Format("RQID[%d][%s][%s]", nRqId, strCode, strMsg);
	WriteLog(strLog);
}

void CHDFCommClientDlg::OnBnClickedRadio3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_edTrCode.SetWindowText(DEF_TR_CODE);
	m_edInput.SetWindowText("101F");

	// FID��ȸ
	m_edTrCode2.SetWindowText(DEF_FID_CODE);
	m_edInput2.SetWindowText("101M300020001");
	//m_edInput3.SetWindowText("000075051057");//�����ڵ�+ȣ���ð�+����+�����ŷ���
	//��ü��û.
	m_edInput3.SetWindowText("000001002003004005006007008009010011012013014015016017018019020021022023024025026027028029030031032033034035036037038039040041042043044045046047048049050051052053054055056057058059060061062063064065066067068069070071072073074075076077078079080081082083084085086087088089090091092093094095096097098099100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232");

	// �ǽð� ����/�ɼ� ü��/ȣ�� ���
	m_edJomgmok.SetWindowText("101G3000");
	m_edGubun.SetWindowText("65");

	m_edAccNo.SetWindowText("05359001   ");

	m_edJumunTR.SetWindowText(DEF_ORD_CODE_NEW);
	//m_edJumunInput.SetWindowTextA("05359001   1234    101JC000                        11123390        1");
	CString strOrderBuff = "   1234    201JB245                        11111            1";
	m_edJumunInput.SetWindowTextA("04001001   1234    201JB245                        111110           1");

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_PRCETP);
	pCombo->ResetContent();
	pCombo->AddString("1.������");
	pCombo->AddString("2.���尡");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_TRDCOND);
	pCombo->ResetContent();
	pCombo->AddString("1.FAS");
	pCombo->AddString("2.FOK");
	pCombo->AddString("3.FAK");
	pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	pCombo->ResetContent();
	pCombo->AddString("�ű�");
	pCombo->AddString("����");
	pCombo->AddString("���");
	pCombo->AddString("CME�ű�");
	pCombo->AddString("CME����");
	pCombo->AddString("CME���");
	pCombo->SetCurSel(0);

	ShowHWOrdCtrl(FALSE);
}


void CHDFCommClientDlg::OnBnClickedRadio4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_edTrCode.SetWindowText(DEF_HW_JANGO);
	m_edInput.SetWindowText("0112278520    ");

	// FID��ȸ
	// �ؿܽü� ��ȸ ���� ���� - 20160830 sivas
	CString strSeries = "ESU16                           ";
	m_edTrCode2.SetWindowText(DEF_HW_FID_CODE);
	m_edInput2.SetWindowText(strSeries);
	m_edInput3.SetWindowText("000001002003004005006007008009010011012013014015016017018019020021022023024025026027028029030031032033034035036037");

	// �ǽð� ����/�ɼ� ü��/ȣ�� ���
	m_edJomgmok.SetWindowText(strSeries);
	m_edGubun.SetWindowText("82");

	m_edAccNo.SetWindowText("011227     ");

	m_edJumunTR.SetWindowText(DEF_HW_ORD_CODE_NEW);
	m_edJumunInput.SetWindowText("0124311234    ESU16                           1200              1         1                                  ");

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_PRCETP);
	pCombo->ResetContent();
	pCombo->AddString("1.������");
	pCombo->AddString("2.���尡");
	pCombo->AddString("3.STOP");
	pCombo->AddString("4.STOP������");
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
	pCombo->AddString("�ű�");
	pCombo->AddString("����");
	pCombo->AddString("���");
	pCombo->SetCurSel(0);

	ShowHWOrdCtrl(TRUE);
}

//@lhe 2012.06.22 FX����
void CHDFCommClientDlg::OnBnClickedRadio5()
{
	m_edTrCode.SetWindowText(DEF_FX_JANGO);
	m_edInput.SetWindowText("1sivas99 0119861234                        ");

	// FID��ȸ
	CString strSeries = "EUR/USD   ";
	m_edTrCode2.SetWindowText(DEF_FX_FID_CODE);
	m_edInput2.SetWindowText(strSeries);
	m_edInput3.SetWindowText("000001002010");//�����ڵ�+�ŵ�����+�ż�����+ȣ���ð�

	// �ǽð� ����/�ɼ� ü��/ȣ�� ���
	m_edJomgmok.SetWindowText(strSeries);
	m_edGubun.SetWindowText("171");

	m_edAccNo.SetWindowText("011986     ");

	m_edJumunTR.SetWindowText(DEF_FX_ORD_CODE_NEW);
	m_edJumunInput.SetWindowText("0119861234    EUR/USD                         11.24633             1              CEO");	//�����ֹ�

	ShowHWOrdCtrl(FALSE);

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_ORD_COMBO_ORDERTR);
	pCombo->ResetContent();
	pCombo->AddString("�ű�");
	pCombo->AddString("����");
	pCombo->AddString("���");
	pCombo->SetCurSel(0);
}


void CHDFCommClientDlg::OnBnClickedButton1()
{

	CString strMsg;
	strMsg.Format("�ؿ� ���MST���� ���� ���� ��û");
	WriteLog(strMsg);


	//��ҹ��� �������� ����. ���������� ��ȯ�Ͽ� ���� - 20150417 anigate
	/*!!!!�߿�!!!!*/
	// ��ü��û�� ���Ͽ�û�� �Բ� ������!
	// Ȥ�ö� ����ϰ� �Ǹ� ���Ͽ�û�̵� ��ü��û�̵� ��� ���䰪�� ���� �Ŀ� ȣ���� ��! - 20150417 anigate
	//m_CommAgent.CommReqMakeCod("aLL");
	//m_CommAgent.CommReqMakeCod("jmCode|cMe");
	//m_CommAgent.CommReqMakeCod("opmCode");

	m_CommAgent.CommReqMakeCod("all");
}


void CHDFCommClientDlg::OnBnClickedBusinessday()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WriteLog(m_CommAgent.CommGetBusinessDay(3));
}


void CHDFCommClientDlg::OnBnClickedFrbusinessday()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WriteLog(m_CommAgent.CommGetBusinessDay(1));
}


void CHDFCommClientDlg::OnBnClickedCommgetinfo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString getInfo = "";
	m_edGetInfo.GetWindowText(getInfo);

	WriteLog(m_CommAgent.CommGetInfo(getInfo));
	WriteLog(m_CommAgent.CommGetInfo(getInfo, "1"));
}


void CHDFCommClientDlg::OnBnClickedButton3()
{
	CString strMsg;
	strMsg.Format("�ؿ� ����MST���� ���� ���� ��û");
	WriteLog(strMsg);

	m_CommAgent.CommReqMakeCod("futures");
}


void CHDFCommClientDlg::OnBnClickedButton2()
{
	CString strMsg;
	strMsg.Format("�ؿ� �ɼ�MST���� ���� ���� ��û");
	WriteLog(strMsg);

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
			strMsg.Format("%sȭ�� ������ �����Ͽ����ϴ�. ", strCommonFileName);
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_ListOutMsg.ResetContent();
}

CString CHDFCommClientDlg::FillStr(CString strSrc, int nTotLen, BOOL bRightAdd, char chPad)
{
	strSrc.Trim();

	// ����ڵ� �߰�. - 20150804 sivas
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	CString strStrTp	= "";	// ��������
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
		case 0 :	// �ű�
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
		case 1 :	// ����
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
		case 2 :	// ���
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		case 0 : // �ű�
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
		case 1 : // ����
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
