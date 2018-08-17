
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

#define MEM_BLOCK_SIZE	256
#define MEM_PRE_ALLOC	100
#define MEM_MAX_ALLOC	1000

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
	DDX_Control(pDX, IDC_EDIT_CURRPRC, m_edCurrPrc);
}

BEGIN_MESSAGE_MAP(CMDProtoTypeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CMDProtoTypeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMDProtoTypeDlg::OnBnClickedButtonStop)
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

	try
	{
		Poco::Util::AbstractConfiguration *cfg = new Poco::Util::XMLConfiguration(cfgFile);

		m_sApiIP = cfg->getString("CHART_SOCKET_INFO.IP").c_str();
		m_nApiPort = atoi(cfg->getString("CHART_SOCKET_INFO.PORT").c_str());

		std::string sLogDir = cfg->getString("DIR.log").c_str();
		g_log.OpenLog((LPSTR)sLogDir.c_str(), EXENAME);
		
		cfg->release();
	}
	catch (const Poco::Exception& e)
	{

		return false;
	}

	m_myTray.m_bHide = m_bHide = false;
	m_myTray.AddTrayIcon(GetSafeHwnd());

	m_pStrat = NULL;

	// DEFAULT VALUE
	((CEdit *)GetDlgItem(IDC_EDIT_SYMBOL))->SetWindowText("CLU18");
	((CEdit *)GetDlgItem(IDC_EDIT_CLOSETM))->SetWindowText("05:00");
	((CEdit *)GetDlgItem(IDC_EDIT_MAXSLCNT))->SetWindowText("2");

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


BOOL CMDProtoTypeDlg::Begin()
{
	// tcp init
	m_pApiRecv = new CTcpClient;
	if (!m_pApiRecv->Begin((char*)m_sApiIP.c_str(), m_nApiPort, 10))
	{
		//TODO LOG
		AfxMessageBox("Init socket failed");
		return FALSE;
	}

	// mem pool init
	m_pMemPool = new CMemPool(MEM_PRE_ALLOC, MEM_MAX_ALLOC, MEM_BLOCK_SIZE);

	if (m_pMemPool->available() == 0)
	{
		//TODO LOG
		AfxMessageBox("Init MemPool failed");
		return FALSE;
	}

	// SYMBOL	
	((CEdit *)GetDlgItem(IDC_EDIT_SYMBOL))->GetWindowText(m_sSymbol);
	((CEdit *)GetDlgItem(IDC_EDIT_OPENPRC))->GetWindowText(m_sOpenPrc);
	((CEdit *)GetDlgItem(IDC_EDIT_CLOSETM))->GetWindowText(m_sCloseTime);
	((CEdit *)GetDlgItem(IDC_EDIT_MAXSLCNT))->GetWindowText(m_sMaxSLCnt);

	// create recv thread
	m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_TickDataRecv, this, 0, &m_unRecvThread);

	// create ord thread
	m_hOrdSendThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_SendOrd, this, 0, &m_unOrdSendThread);

	m_pApiRecv->StartRecvData();

	return TRUE;
}


VOID	CMDProtoTypeDlg::End()
{
	PostThreadMessage(m_unRecvThread, WM_DIE, 0, 0);
	PostThreadMessage(m_unOrdSendThread, WM_DIE, 0, 0);

	if (WaitForSingleObject(m_hRecvThread, 1000) != WAIT_OBJECT_0) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hRecvThread, dwExitCode);
	}

	
	if (WaitForSingleObject(m_hOrdSendThread, 1000) != WAIT_OBJECT_0) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hOrdSendThread, dwExitCode);
	}

	SAFE_CLOSEHANDLE(m_hRecvThread);
	SAFE_CLOSEHANDLE(m_hOrdSendThread);
	SAFE_DELETE(m_pApiRecv);
	SAFE_DELETE(m_pMemPool);
	
}


unsigned WINAPI CMDProtoTypeDlg::Thread_SendOrd(LPVOID lp)
{
	CMDProtoTypeDlg* p = (CMDProtoTypeDlg*)lp;
	char zMsg[1024];
	while (TRUE)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				return 0;
			}

			if (msg.message == WM_SENDORD_API)
			{
				//TODO SEND ORDER TO API

				// LOGGING
				ST_MF_STRAT_ORD* pPack = (ST_MF_STRAT_ORD*)msg.lParam;
				sprintf(zMsg, "%.*s", sizeof(pPack->Note), pPack->Note);
				g_log.log(LOGTP_SUCC, zMsg);
				p->m_lstMsg.InsertString(0, zMsg);
				p->m_pMemPool->release((char*)msg.lParam);
			}
		}
	}

	return 0;
}


unsigned WINAPI CMDProtoTypeDlg::Thread_TickDataRecv(LPVOID lp)
{
	CMDProtoTypeDlg* p = (CMDProtoTypeDlg*)lp;
	ST_PACK2CHART_EX* pSise;
	char zSymbol[128];
	char tm[9];
	char zCurrPrc[32];
	while (TRUE)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				return 0;
			}
		}
		
		if (p->m_pApiRecv->HappenedRecvError())
		{
			g_log.log(LOGTP_ERR, "TCP RECV ERROR:%s", p->m_pApiRecv->GetMsg());
			printf("TCP RECV ERROR:%s\n", p->m_pApiRecv->GetMsg());
			continue;
		}
		char* pBuf = NULL;;
		if (!p->m_pMemPool->get(&pBuf))
		{
			g_log.log(LOGTP_ERR, "memory pool get error");
			printf("memory pool get error\n");
			continue;
		}
		int nLen = p->m_pApiRecv->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			p->m_pMemPool->release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			g_log.log(LOGTP_ERR, "PAKCET �̻�(%s)(%s)", pBuf, p->m_pApiRecv->GetMsg());
			printf("PAKCET �̻�(%s)\n", p->m_pApiRecv->GetMsg());
			p->m_pMemPool->release(pBuf);
			continue;
		}

		if (nLen > 0)
		{
			pSise = (ST_PACK2CHART_EX*)pBuf;
			sprintf(tm, "%.2s:%.2s:%.2s", pSise->Time, pSise->Time + 2, pSise->Time + 4);
			memcpy(pSise->Time, tm, sizeof(pSise->Time));
			sprintf(zCurrPrc, "%.*s", sizeof(pSise->Close), pSise->Close);

			sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
			CUtil::TrimAll(zSymbol, strlen(zSymbol));
			std::string sSymbol = zSymbol;


			int nComp = p->m_sSymbol.Compare(zSymbol);
			//std::map<std::string, CStratMaker*>::iterator it = p->m_mapStrat.find(sSymbol);
			//if (it == p->m_mapStrat.end())
			if(nComp!=0)
			{
				p->m_pMemPool->release(pBuf);
				//g_log.log(LOGTP_ERR, "[%s] ������ ��û�� ������ �ƴϴ�.", sSymbol.c_str());
			}
			else
			{
				if (!p->m_pStrat)
				{
					//char* pzSymbol, char* pzOpenPrc, CMemPool* pMemPool, unsigned dwSaveThread, unsigned dwSendThread);
					p->m_pStrat = new CStratMaker(
						zSymbol, 
						(LPSTR)(LPCSTR)p->m_sOpenPrc, 
						p->m_pMemPool,
						0, 
						p->m_unOrdSendThread, 
						(LPSTR)(LPCSTR)p->m_sCloseTime,
						(LPSTR)(LPCSTR)p->m_sMaxSLCnt
					);
				}
				p->m_edCurrPrc.SetWindowText(zCurrPrc);
				//CStratMaker* p = (*it).second;
				PostThreadMessage(p->m_pStrat->GetStratThreadId(), WM_RECV_API_MD, 0, (LPARAM)pBuf);
				//printf("[RECV](%s)\n", pBuf);
				//g_log.log(LOGTP_SUCC, "[RECV-2](%.80s)", pBuf);
			}
		}
	}
	return 0;
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
	{
		//OnDialogShow();
	}
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


void CMDProtoTypeDlg::OnBnClickedButtonStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Begin();

}


void CMDProtoTypeDlg::OnBnClickedButtonStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	End();
}
