
// MDProtoTypeDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "../../IRUM_UTIL/ADOFunc.h"
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

#define MEM_BLOCK_SIZE	512
#define MEM_PRE_ALLOC	100
#define MEM_MAX_ALLOC	1000

CLogMsg				g_log;
CDBPoolAdo			*g_ado = NULL;

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
	ON_BN_CLICKED(IDCANCEL, &CMDProtoTypeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CURR, &CMDProtoTypeDlg::OnBnClickedButtonCurr)
	ON_BN_CLICKED(IDC_BUTTON_CLOSETM, &CMDProtoTypeDlg::OnBnClickedButtonClosetm)
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
	m_cfgFile = szXmlCnfg;

	try
	{
		Poco::Util::AbstractConfiguration *cfg = new Poco::Util::XMLConfiguration(m_cfgFile);

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
	((CEdit *)GetDlgItem(IDC_EDIT_SYMBOL))->SetWindowText("CLV18");
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

	// DB CONNECT
	std::string sIP, sID, sPwd, sName, sPoolCnt;
	Poco::Util::AbstractConfiguration *cfg = new Poco::Util::XMLConfiguration(m_cfgFile);

	sIP = cfg->getString("DBINFO.DB_IP").c_str();
	sID = cfg->getString("DBINFO.DB_ID").c_str();
	sPwd = cfg->getString("DBINFO.DB_PWD").c_str();
	sName = cfg->getString("DBINFO.DB_NAME").c_str();
	sPoolCnt = cfg->getString("DBINFO.DB_POOL_CNT").c_str();

	cfg->release();

	g_ado = new CDBPoolAdo((char*)sIP.c_str(),(char*)sID.c_str(), (char*)sPwd.c_str(), (char*)sName.c_str());
	if (!g_ado->Init(atoi(sPoolCnt.c_str())))
	{
		g_log.log(LOGTP_ERR, "DB OPEN FAIL(MSG:%s)", g_ado->GetMsg());
		SAFE_DELETE(g_ado);
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", 
		sIP.c_str(), sID.c_str(), sPwd.c_str(), sName.c_str());

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
	CMDProtoTypeDlg* pThis = (CMDProtoTypeDlg*)lp;
	char zMsg[1024];
	char zQ[1024];
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
				ST_MF_STRAT_ORD* p = (ST_MF_STRAT_ORD*)msg.lParam;

				//TODO SEND ORDER TO API

				// SAVE INTO DATABASE
				CDBHandlerAdo db(g_ado->Get());

				sprintf(zQ, "EXEC TEST_MUDFISH_SAVE "
					"'%.*s', "	//@I_SYMBOL	VARCHAR(10)
					"'%.*s', "	//@I_STRAT_ID	VARCHAR(20)
					"'%.*s', "	//@I_TICK_DT	CHAR(8)
					"'%.*s', "	//@I_TICK_TM CHAR(8)
					"'%.*s', "	//@I_OPEN_CLOSE_TP CHAR(1)
					"'%.*s', "	//@I_BS_TP CHAR(1)
					"'%.*s', "	//@I_OPEN_PRC VARCHAR(10)
					"'%.*s', "	//@I_CURR_PRC VARCHAR(10)
					"'%.*s', "	//@I_ORD_PRC VARCHAR(10)
					"'%.*s', "	//@I_BEST_PRC VARCHAR(10)
					"'%.*s', "	//@I_BASE_PRC VARCHAR(10)
					"'%.*s', "	//@I_ENTRY_PRC VARCHAR(10)
					"'%.*s' "	//@I_NOTE VARCHAR(100)
					,
					
					sizeof(p->Symbol), p->Symbol,
					sizeof(p->StratID), p->StratID,
					sizeof(p->ApiDT), p->ApiDT,
					sizeof(p->ApiTM), p->ApiTM,
					sizeof(p->ClrTp), p->ClrTp,
					sizeof(p->Side), p->Side,
					sizeof(p->OpenPrc), p->OpenPrc,
					sizeof(p->CurrPrc), p->CurrPrc,
					sizeof(p->OrdPrc), p->OrdPrc,
					sizeof(p->MaxPLPrc), p->MaxPLPrc,
					sizeof(p->BasePrc), p->BasePrc,
					sizeof(p->EntryPrc), p->EntryPrc,
					sizeof(p->Note), p->Note
				);
				if (FALSE == db->ExecQuery(zQ))
				{
					g_log.log(LOGTP_ERR, "MUDFISH DATA Save ����(%s)(%s)", db->GetError(), zQ);
					sprintf(zMsg, "MUDFISH DATA Save ����(%s)(%s)\n", db->GetError(), zQ);
					pThis->m_lstMsg.InsertString(0, zMsg);
				}
				else
				{
					g_log.log(LOGTP_SUCC, "DB SAVE(%s)", zQ);
				}

				db->Close();

				// LOGGING
				
				sprintf(zMsg, "%.*s", sizeof(p->Note), p->Note);
				g_log.log(LOGTP_SUCC, zMsg);
				pThis->m_lstMsg.InsertString(0, zMsg);
				pThis->m_pMemPool->release((char*)msg.lParam);
			}
		}
	}

	return 0;
}



void CMDProtoTypeDlg::OnBnClickedButtonClosetm()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sCurrPrc;
	((CEdit *)GetDlgItem(IDC_EDIT_CLOSETM))->GetWindowText(sCurrPrc);
	char* pBuf = NULL;;
	if (!m_pMemPool->get(&pBuf))
	{
		g_log.log(LOGTP_ERR, "memory pool get error");
		//printf("memory pool get error\n");
		return;
	}
	memcpy(pBuf, (LPCSTR)sCurrPrc, sCurrPrc.GetLength());

	PostThreadMessage(m_pStrat->GetStratThreadId(), WM_MARKET_CLOSE, 0, (LPARAM)pBuf);
}



/*
typedef struct _ST_PACK2CHART_EX
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];	// ��ü ����
char	Date[8];
char	Time[8];
char	ShortCode[LEN_SYMBOL];
char	Close[LEN_PRC];
char	CntrQty[LEN_QTY];
char	DecLen[5];				// �Ҽ��� �ڸ���
char	ETX[1];
}ST_PACK2CHART_EX;
*/
void CMDProtoTypeDlg::OnBnClickedButtonCurr()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sCurrPrc;
	((CEdit *)GetDlgItem(IDC_EDIT_CURRPRC2))->GetWindowText(sCurrPrc);

	ST_PACK2CHART_EX st;
	memset(&st, 0x20, sizeof(st));

	char dt[32], tm[32];
	CUtil::GetToday(dt);
	sprintf(tm, "%.8s", CUtil::Get_NowTime(FALSE));

	memcpy(st.Date, dt, sizeof(st.Date));
	memcpy(st.Time, tm, sizeof(st.Time));
	memcpy(st.ShortCode, (LPCSTR)m_sSymbol, m_sSymbol.GetLength());
	memcpy(st.Close, (LPCSTR)sCurrPrc, sCurrPrc.GetLength());

	char* pBuf = NULL;;
	if (!m_pMemPool->get(&pBuf))
	{
		g_log.log(LOGTP_ERR, "memory pool get error");
		//printf("memory pool get error\n");
		return;
	}
	memcpy(pBuf, &st, sizeof(st));
	ST_PACK2CHART_EX* p = (ST_PACK2CHART_EX*)pBuf;
	if (!m_pStrat)
	{
		//char* pzSymbol, char* pzOpenPrc, CMemPool* pMemPool, unsigned dwSaveThread, unsigned dwSendThread);
		m_pStrat = new CStratMaker(
			(LPSTR)(LPCSTR)m_sSymbol,
			(LPSTR)(LPCSTR)m_sOpenPrc,
			m_pMemPool,
			0,
			m_unOrdSendThread,
			(LPSTR)(LPCSTR)m_sCloseTime,
			(LPSTR)(LPCSTR)m_sMaxSLCnt
		);
		Sleep(1000);
	}
	//m_edCurrPrc.SetWindowText(zCurrPrc);
	//CStratMaker* p = (*it).second;
	PostThreadMessage(m_pStrat->GetStratThreadId(), WM_RECV_API_MD, 0, (LPARAM)pBuf);
	//CUtil::logOutput("[1]ThreadID(%d)(%s)", m_pStrat->GetStratThreadId(), pBuf);
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
					Sleep(3000);
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


void CMDProtoTypeDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	PostThreadMessage(m_unOrdSendThread, WM_DIE, 0, 0);
	PostThreadMessage(m_unRecvThread, WM_DIE, 0, 0);
	delete m_pStrat;
	Sleep(500);
	CDialogEx::OnCancel();
}


