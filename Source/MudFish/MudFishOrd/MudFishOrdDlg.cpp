
// MudFishOrdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MudFishOrd.h"
#include "MudFishOrdDlg.h"
#include "afxdialogex.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_INC/IRUM_Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum {I_SYMBOL=0, I_NAME, I_TICKVALUE, I_TICKSIZE, I_DOTCNT, I_CURRPRC, I_OPENPRC, I_ENDTM, I_MAXSL, I_MAXPT};
enum {N_SYBOL=0, N_BS, N_OPENPRC, N_ENTRYPRC, N_CURRPRC, N_PLTICK, N_QTY, N_PL};
CLogMsg				g_log;
char	g_zConfig[_MAX_PATH];

#define MEM_BLOCK_SIZE	512
#define MEM_PRE_ALLOC	100
#define MEM_MAX_ALLOC	1000

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CMudFishOrdDlg dialog



CMudFishOrdDlg::CMudFishOrdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MUDFISHORD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_pMemPool = NULL;
	m_pApiRecv = NULL;
	m_pDBPool = NULL;
	m_symPos = NULL;
}	

void CMudFishOrdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_lstMsg);
	DDX_Control(pDX, IDC_LIST_SYMBOL, m_ctlSymbol);
	DDX_Control(pDX, IDC_LIST_REALPL, m_ctlRealPl);
}

BEGIN_MESSAGE_MAP(CMudFishOrdDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMudFishOrdDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMudFishOrdDlg::OnBnClickedButtonSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SYMBOL, &CMudFishOrdDlg::OnDblclkListSymbol)
END_MESSAGE_MAP()


// CMudFishOrdDlg message handlers

BOOL CMudFishOrdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	


	//	GET LOG DIR
	char szDir[_MAX_PATH];
	CProp prop;
	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	strcpy(szDir, prop.GetValue("CONFIG_DIR_CHART"));
	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, EXENAME);

	char port[32];
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP", m_zApiIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "PORT", port);
	m_nApiPort = atoi(port);


	GetDlgItem(IDC_EDIT_HIDDEN)->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);

	Begin();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CMudFishOrdDlg::InitSymbolList()
{
	m_ctlSymbol.SetExtendedStyle(m_ctlSymbol.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_DOUBLEBUFFER
		| LVS_EX_CHECKBOXES);


	m_ctlSymbol.InsertColumn(0, _T("Symbol"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(1, _T("Name"), LVCFMT_CENTER, 129, -1);
	m_ctlSymbol.InsertColumn(2, _T("TickValue"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(3, _T("TickSize"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(4, _T("DotCnt"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(5, _T("CurrPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(6, _T("OpenPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(7, _T("EndTime"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(8, _T("MAXCNT_SL"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(9, _T("MAXCNT_PT"), LVCFMT_CENTER, 90, -1);
}



void CMudFishOrdDlg::InitRealPLList()
{
	m_ctlRealPl.SetExtendedStyle(m_ctlSymbol.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_DOUBLEBUFFER
		);


	m_ctlRealPl.InsertColumn(0, _T("Symbol"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(1, _T("BuySell"), LVCFMT_CENTER, 129, -1);
	m_ctlRealPl.InsertColumn(2, _T("OpenPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(3, _T("EntryPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(4, _T("CurrPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(5, _T("PL Tick"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(6, _T("Qty"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(7, _T("PL"), LVCFMT_CENTER, 90, -1);
}


void CMudFishOrdDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMudFishOrdDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMudFishOrdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




unsigned WINAPI CMudFishOrdDlg::Thread_TickDataRecv(LPVOID lp)
{
	return 0;
}

unsigned WINAPI CMudFishOrdDlg::Thread_SendOrd(LPVOID lp)
{
	return 0;
}

BOOL CMudFishOrdDlg::LoadSymbolInfo(BOOL bCreateStrat)
{
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	if(!m_pDBPool)
		m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
	if (!m_pDBPool->Init(atoi(cnt)))
	{
		showMsg(FALSE, "DB OPEN FAILED.(%s)(%s)(%s)", ip, id, pwd);
		return FALSE;
	}

	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC MF_LOAD_SYBOL ");
	if (FALSE == db->ExecQuery(zQ))
	{
		showMsg(FALSE, "Load Symbol Error(%s)", zQ);
	}
	else
	{
		m_ctlSymbol.DeleteAllItems();

		char data[128];
		double dData;
		int nDotCnt;
		int nRow = 0;
		while (db->IsNextRow())
		{
			db->GetStr("SYMBOL", data);
			m_ctlSymbol.InsertItem(nRow, data);

			db->GetStr("NAME", data);
			m_ctlSymbol.SetItem(nRow, I_NAME, LVIF_TEXT, _T(data), 0, 0, 0, NULL);

			dData = db->GetDouble("TICK_VALUE");
			sprintf(data, "%.2f", dData);
			m_ctlSymbol.SetItem(nRow, I_TICKVALUE, LVIF_TEXT, data, 0, 0, 0, NULL);

			dData = db->GetDouble("TICK_SIZE");
			sprintf(data, "%.5f", dData);
			m_ctlSymbol.SetItem(nRow, I_TICKSIZE, LVIF_TEXT, data, 0, 0, 0, NULL);

			nDotCnt = db->GetLong("DOT_CNT");
			sprintf(data, "%d", nDotCnt);
			m_ctlSymbol.SetItem(nRow, I_DOTCNT, LVIF_TEXT, data, 0, 0, 0, NULL);

			db->GetStr("OPEN_PRC", data);
			m_ctlSymbol.SetItem(nRow, I_OPENPRC, LVIF_TEXT, _T(data), 0, 0, 0, NULL);

			db->GetStr("END_TM", data);
			m_ctlSymbol.SetItem(nRow, I_ENDTM, LVIF_TEXT, _T(data), 0, 0, 0, NULL);

			nDotCnt = db->GetLong("MAXCNT_SL");
			sprintf(data, "%d", nDotCnt);
			m_ctlSymbol.SetItem(nRow, I_MAXSL, LVIF_TEXT, data, 0, 0, 0, NULL);

			nDotCnt = db->GetLong("MAXCNT_PT");
			sprintf(data, "%d", nDotCnt);
			m_ctlSymbol.SetItem(nRow, I_MAXPT, LVIF_TEXT, data, 0, 0, 0, NULL);

			if (bCreateStrat)
			{
				//todo
				//char zSymbol[32], zOpenPrc[32], zCoseTM[32], zMaxSLCnt[32], zTickSize[32], zDotCnt[32];
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL1", "SYMBOL", zSymbol);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL1", "TICK_SIZE", zTickSize);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL1", "DOT_CNT", zDotCnt);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL1", "OPEN_PRC", zOpenPrc);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL1", "CLOSE_TM", zCoseTM);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL1", "MAX_SL_CNT", zMaxSLCnt);

				//CStratMaker *pStrat = new CStratMaker(zSymbol, zOpenPrc, zTickSize, zDotCnt, zCoseTM,
				//	zMaxSLCnt, m_pMemPool, 0, m_unOrdSendThread);

				//m_mapStrat[zSymbol] = pStrat;

				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL2", "SYMBOL", zSymbol);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL2", "TICK_SIZE", zTickSize);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL2", "DOT_CNT", zDotCnt);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL2", "OPEN_PRC", zOpenPrc);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL2", "CLOSE_TM", zCoseTM);
				//CUtil::GetConfig(g_zConfig, "TRADE_SYMBOL2", "MAX_SL_CNT", zMaxSLCnt);

				//CStratMaker *pStrat2 = new CStratMaker(zSymbol, zOpenPrc, zTickSize, zDotCnt, zCoseTM,
				//	zMaxSLCnt, m_pMemPool, 0, m_unOrdSendThread);

				//m_mapStrat[zSymbol] = pStrat2;
			}
			nRow++;
			db->Next();

		}

	}

	db->Close();
}

BOOL CMudFishOrdDlg::Begin()
{

	// tcp init
	m_pApiRecv = new CTcpClient;
	if (!m_pApiRecv->Begin(m_zApiIP, m_nApiPort, 10))
	{
		showMsg(FALSE, "Init Socket Error");
		return FALSE;
	}

	// mem pool init
	m_pMemPool = new CMemPool(MEM_PRE_ALLOC, MEM_MAX_ALLOC, MEM_BLOCK_SIZE);

	if (m_pMemPool->available() == 0)
	{
		showMsg(FALSE, "Init MemPool failed");
		return FALSE;
	}


	// create recv thread
	m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_TickDataRecv, this, 0, &m_unRecvThread);

	// create ord thread
	m_hOrdSendThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_SendOrd, this, 0, &m_unOrdSendThread);

	Sleep(3000);

	m_pApiRecv->StartRecvData();

	return TRUE;
}

VOID CMudFishOrdDlg::End()
{
	SAFE_DELETE(m_pMemPool);
	SAFE_DELETE(m_pApiRecv);
	SAFE_DELETE(m_pDBPool);
	SAFE_DELETE(m_symPos);
}

void CMudFishOrdDlg::showMsg(BOOL bSucc, char* pMsg, ...)
{
	char buff1[512], buff2[512];
	va_list argptr;
	SYSTEMTIME	st;


	va_start(argptr, pMsg);
	vsprintf_s(buff1, pMsg, argptr);
	va_end(argptr);


	GetLocalTime(&st);
	sprintf(buff2, "[%02d:%02d:%02d.%03d]%s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);

	m_lstMsg.InsertString(0, buff2);
	if(bSucc)	g_log.log(LOGTP_SUCC, buff2);
	else		g_log.log(LOGTP_ERR, buff2);
}

void CMudFishOrdDlg::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	LoadSymbolInfo();
}


void CMudFishOrdDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	int nCount = m_ctlSymbol.GetItemCount();
	if (nCount == 0)
		return;

	CString sSymbol, sOpenPrc, sEndTM, sMaxSL, sMaxPT;

	for (int i = 0; i < nCount; i++)
	{
		if (m_ctlSymbol.GetCheck(i))
		{
			sSymbol = m_ctlSymbol.GetItemText(i, I_SYMBOL);
			sOpenPrc = m_ctlSymbol.GetItemText(i, I_OPENPRC);
			sEndTM = m_ctlSymbol.GetItemText(i, I_ENDTM);
			sMaxSL = m_ctlSymbol.GetItemText(i, I_MAXSL);
			sMaxPT = m_ctlSymbol.GetItemText(i, I_MAXPT);

			CDBHandlerAdo db(m_pDBPool->Get());
			char zQ[1024];
			sprintf(zQ, "EXEC MF_SYMBOL_TRADE_CONFIG_SAVE "
				"'%s', "	//sSymbol
				"'%s', "	//sOpenPrc
				"'%s', "	//sEndTM
				"'%s', "	//sMaxSL
				"'%s' "	//sMaxPT
				,
				(LPCSTR)sSymbol,
				(LPCSTR)sOpenPrc,
				(LPCSTR)sEndTM,
				(LPCSTR)sMaxSL,
				(LPCSTR)sMaxPT
			);
			if (FALSE == db->ExecQuery(zQ))
			{
				showMsg(LOGTP_ERR, "MF_SYMBOL_TRADE_CONFIG_SAVE Err (%s)(%s)", db->GetError(), zQ);
			}
			else
			{
				showMsg(LOGTP_SUCC, "MF_SYMBOL_TRADE_CONFIG_SAVE OK(%s)", (LPSTR)(LPCSTR)sSymbol);
			}
			db->Close();
		} // if (m_ctlSymbol.GetCheck(i))

	} //for (int i = 0; i < nCount; i++)
}


void CMudFishOrdDlg::OnDblclkListSymbol(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pNMITEM = (LPNMITEMACTIVATE)pNMHDR;

	m_nItem = pNMITEM->iItem;
	m_nSubitem = pNMITEM->iSubItem;

	if (pNMITEM->iItem != -1 && m_nSubitem > 1)
	{
		CRect rect;

		if (pNMITEM->iSubItem == 0)
		{
			m_ctlSymbol.GetItemRect(pNMITEM->iItem, rect, LVIR_BOUNDS);
			rect.right = rect.left + m_ctlSymbol.GetColumnWidth(0);
		}
		else
		{
			m_ctlSymbol.GetSubItemRect(pNMITEM->iItem, pNMITEM->iSubItem, LVIR_BOUNDS, rect);
		}

		m_ctlSymbol.ClientToScreen(rect);
		this->ScreenToClient(rect);

		GetDlgItem(IDC_EDIT_HIDDEN)->SetWindowText(m_ctlSymbol.GetItemText(pNMITEM->iItem, pNMITEM->iSubItem));
		GetDlgItem(IDC_EDIT_HIDDEN)->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		GetDlgItem(IDC_EDIT_HIDDEN)->SetFocus();
	}

	*pResult = 0;

}

BOOL CMudFishOrdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN)
	{
		UINT nChar = pMsg->wParam;
		if (nChar == VK_RETURN)
		{
			int nFocusCtrl = GetFocus()->GetDlgCtrlID();

			//printf("input code : %d\n", nChar);
			if (nFocusCtrl == IDC_EDIT_HIDDEN)
			{
				if (nChar == VK_RETURN)
				{
					if (pMsg->hwnd == GetDlgItem(IDC_EDIT_HIDDEN)->GetSafeHwnd())
					{
						CString str;
						GetDlgItemText(IDC_EDIT_HIDDEN, str);
						m_ctlSymbol.SetItemText(m_nItem, m_nSubitem, str);

						GetDlgItem(IDC_EDIT_HIDDEN)->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);
					}

					return TRUE;                // Do not process further
				}
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
