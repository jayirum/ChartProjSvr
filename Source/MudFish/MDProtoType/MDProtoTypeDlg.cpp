
// MDProtoTypeDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MDProtoType.h"
#include "MDProtoTypeDlg.h"
#include "afxdialogex.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "Poco/Util/xmlconfiguration.h"
#include "Poco/Logger.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EXENAME		"MDProtoType.exe"
CLogMsg				g_log;


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

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


// CMDProtoTypeDlg ��ȭ ����



CMDProtoTypeDlg::CMDProtoTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MDPROTOTYPE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMDProtoTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_lstMsg);
}

BEGIN_MESSAGE_MAP(CMDProtoTypeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMDProtoTypeDlg �޽��� ó����

BOOL CMDProtoTypeDlg::OnInitDialog()
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

	// �̹� ���������� Ȯ��
	if (m_Dup.CheckDuplicate("MDProtoType") == -1)
	{
		AfxMessageBox("<MDProtoType>�̹� ���μ����� �������Դϴ�.");
		//OnBnClickedButtonTerminate();
		exit(0);
	}

	//	GET LOG DIR
	char szDir[_MAX_PATH];
	char szXmlCnfg[128];
	CProp prop;
	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	strcpy(szDir, prop.GetValue("CONFIG_DIR_CHART"));

	CUtil::GetCnfgXMLFileNm(szDir, EXENAME, szXmlCnfg);
	std::string cfgFile = szXmlCnfg; 
	g_log.OpenLog(szDir, EXENAME);

	try
	{
		Poco::Util::AbstractConfiguration *cfg = new Poco::Util::XMLConfiguration(cfgFile);
		int nPort = atoi(cfg->getString("CHART_SOCKET_INFO.PORT").c_str());
		
		cfg->release();
	}
	catch (const Poco::Exception& e)
	{

		return false;
	}

	m_myTray.m_bHide = m_bHide = false;
	m_myTray.AddTrayIcon(GetSafeHwnd());




	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMDProtoTypeDlg::OnPaint()
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
HCURSOR CMDProtoTypeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// TRAYICON Below onward
void CMDProtoTypeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	if (nID == SC_CLOSE)	// || nID == SC_MINIMIZE)
		OnDialogShow();
	else
		CDialogEx::OnSysCommand(nID, lParam);
}


long CMDProtoTypeDlg::OnTrayIcon(WPARAM wParam, LPARAM IParam)
{
	m_myTray.ProcTrayMsg(GetSafeHwnd(), wParam, IParam);
	return 0;
}

void CMDProtoTypeDlg::OnAppExit(void)
{
	m_myTray.DelTrayIcon(GetSafeHwnd());
	CDialog::OnCancel();
}

void CMDProtoTypeDlg::OnDialogShow(void)
{
	if (!m_bHide) ShowWindow(false);
	else ShowWindow(true);
	m_bHide = !m_bHide;
	m_myTray.m_bHide = m_bHide;
}
