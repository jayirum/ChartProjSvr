
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


#define ID_TIME_INIT 101


enum {I_SYMBOL=0, I_NAME, I_TICKVALUE, I_TICKSIZE, I_DOTCNT, I_CURRPRC, I_OPENPRC, I_ORDQTY,
		I_ENDTM,	//장마감시간
		I_MAXSL,	//손절제한횟수
		I_MAXPT,	//익절제한횟수
		I_ENTRY_SPREAD,	//진입SPREAD		0.1%
		I_CLR_SPREAD,	//익절SPAREAD	0.5%
		I_PT_POINT	//최고점 찍고 떨어질때 손절 50%
};
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

	m_unApiTick = m_unApiOrd = m_unSaveData = 0;
	m_hApiTick = m_hApiOrd = m_hSaveData = NULL;
	m_pMemPool = NULL;
	m_pDBPool = NULL;
	m_bContinue = TRUE;
	m_pApiClient[0] = m_pApiClient[1] = NULL;
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
	ON_BN_CLICKED(IDC_BUTTON_STRAT_START, &CMudFishOrdDlg::OnBnClickedButtonStratStart)
	ON_BN_CLICKED(IDC_BUTTON_STRAT_STOP, &CMudFishOrdDlg::OnBnClickedButtonStratStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CMudFishOrdDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMudFishOrdDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_POS, &CMudFishOrdDlg::OnBnClickedButtonClosePos)
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

	//	GET LOG DIR
	char szDir[_MAX_PATH] = { 0, };
	CProp prop;
	if (!prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY))
	{
		AfxMessageBox("Registry Open Failed");
		return FALSE;
	}
	strcpy(szDir, prop.GetValue("CONFIG_DIR_MRYOON"));
	if(szDir[0]==0)
	{
		AfxMessageBox("Registry value Failed");
		return FALSE;
	}
	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, EXENAME);

	GetDlgItem(IDC_EDIT_HIDDEN)->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);
	InitSymbolList();
	InitRealPLList();

	// mem pool init
	m_pMemPool = new CMemPool(MEM_PRE_ALLOC, MEM_MAX_ALLOC, MEM_BLOCK_SIZE);

	if (m_pMemPool->available() == 0)
	{
		showMsg(FALSE, "Init MemPool failed");
		return FALSE;
	}


	SetTimer(ID_TIME_INIT, 1000, NULL);


	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CMudFishOrdDlg::InitSymbolList()
{
	m_ctlSymbol.SetExtendedStyle(m_ctlSymbol.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_DOUBLEBUFFER
		| LVS_EX_CHECKBOXES);


	m_ctlSymbol.InsertColumn(I_SYMBOL, _T("Symbol"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_NAME, _T("Name"), LVCFMT_CENTER, 129, -1);
	m_ctlSymbol.InsertColumn(I_TICKVALUE, _T("TickValue"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_TICKSIZE, _T("TickSize"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_DOTCNT, _T("DotCnt"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_CURRPRC, _T("CurrPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_OPENPRC, _T("OpenPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_ORDQTY, _T("OrdQty"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_ENDTM, _T("EndTime"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_MAXSL, _T("MAXCNT_SL"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_MAXPT, _T("MAXCNT_PT"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_ENTRY_SPREAD, _T("EntrySpread"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_CLR_SPREAD, _T("ClrSpread"), LVCFMT_CENTER, 90, -1);
	m_ctlSymbol.InsertColumn(I_PT_POINT, _T("PT Point"), LVCFMT_CENTER, 90, -1);
}



void CMudFishOrdDlg::InitRealPLList()
{
	m_ctlRealPl.SetExtendedStyle(m_ctlRealPl.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_DOUBLEBUFFER
		);


	m_ctlRealPl.InsertColumn(N_SYBOL, _T("Symbol"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_BS, _T("BuySell"), LVCFMT_CENTER, 129, -1);
	m_ctlRealPl.InsertColumn(N_OPENPRC, _T("OpenPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_ENTRYPRC, _T("EntryPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_CURRPRC, _T("CurrPrc"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_PLTICK, _T("PL Tick"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_QTY, _T("Qty"), LVCFMT_CENTER, 90, -1);
	m_ctlRealPl.InsertColumn(N_PL, _T("PL"), LVCFMT_CENTER, 90, -1);
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




unsigned WINAPI CMudFishOrdDlg::Thread_ApiTick(LPVOID lp)
{

	CMudFishOrdDlg* p = (CMudFishOrdDlg*)lp;

	ST_PACK2CHART_EX* pSise;
	char zSymbol[128];
	char tm[9];
	char zCurrPrc[32];

	if (!p->m_pApiClient[API_TICK]->Begin((LPSTR)p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK], 10))
	{
		p->showMsg(FALSE, "API DataFeed Socket Error[%s][%d]", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);
		Sleep(5000);
	}
	else
		p->showMsg(TRUE, "API DataFeed Socket OK(%s)(%d)", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);

	p->m_pApiClient[API_TICK]->StartRecvData();

	while (p->m_bContinue)
	{
		Sleep(1);
		//connect
		//if (p->m_pApiClient[API_TICK]) {
		//	if (!p->m_pApiClient[API_TICK]->IsConnected())
		//	{
		//		sprintf(temp, "socket:%d, m_bConnect:%d\n", p->m_pApiClient[API_TICK]->m_sock, 
		//			p->m_pApiClient[API_TICK]->m_bConn);
		//		OutputDebugString(temp);

		//		if (!p->m_pApiClient[API_TICK]->Connect())
		//		{
		//			p->showMsg(FALSE, "API DataFeed Socket connect Error[%s][%d]", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);
		//			Sleep(5000);
		//			continue;
		//		}
		//		p->showMsg(TRUE, "API DataFeed Socket connect OK(%s)(%d)", p->m_sApiIP[API_TICK].c_str(), p->m_nApiPort[API_TICK]);
		//		Sleep(1000);
		//	}
		//}
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				p->CloseApiSocket(API_TICK);
				p->showMsg(TRUE, "CloseApiSocket(API_TICK)-1");
				return 0;
			}
		}

		if (p->m_pApiClient[API_TICK]->HappenedRecvError())
		{
			p->showMsg(FALSE, "TICK DATA RECV ERROR:%s", p->m_pApiClient[API_TICK]->GetMsg());
			continue;
		}
		char* pBuf = NULL;;
		if (!p->m_pMemPool->get(&pBuf))
		{
			p->showMsg(FALSE, "memory pool get error");
			continue;
		}
		int nLen = p->m_pApiClient[API_TICK]->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			p->m_pMemPool->release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			p->showMsg(FALSE,"TICK DATA PAKCET 이상(%s)(%s)", pBuf, p->m_pApiClient[API_TICK]->GetMsg());
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

			//char buf[128]; sprintf(buf,"tick[%s]\n", zSymbol);
			//OutputDebugString(buf);
			ITMAP_STRAT it = p->m_mapStrat.find(sSymbol);
			if (it == p->m_mapStrat.end())
			{
				p->m_pMemPool->release(pBuf);
				continue;
				//g_log.log(LOGTP_ERR, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
			}

			ST_STRAT* pStrat = (*it).second;

			// 화면에 표시
			p->DatafeedProc(sSymbol, pBuf, pStrat->bFirstFeed);

			// strat class 에 전달
			
			if (!pStrat->bFirstFeed) {
				pStrat->h->SetOpenPrc(zCurrPrc);
				pStrat->bFirstFeed = TRUE;
				p->m_mapStrat[sSymbol] = pStrat;
			}
			else {
				PostThreadMessage(pStrat->m->GetStratThreadId(), WM_RECV_API_MD, 0, (LPARAM)pBuf);
			}
		}
	}
	p->CloseApiSocket(API_TICK);
	p->showMsg(TRUE, "CloseApiSocket(API_TICK)-2");
	return 0;
}

//화면에 표시, 평가손익
void CMudFishOrdDlg::DatafeedProc(std::string sSymbol, char* pPacket, BOOL bFirstFeedSaved)
{
	ST_PACK2CHART_EX	*pPack = (ST_PACK2CHART_EX*)pPacket;
	CString symbol = sSymbol.c_str();
	std::map<CString, UINT>::iterator it = m_mapIdxSymbol.find(symbol);
	if (it == m_mapIdxSymbol.end())
	{
		return ;
	}
	UINT idx = (*it).second;
	char zCurrr[32]; sprintf(zCurrr, "%.*s", sizeof(pPack->Close), pPack->Close);
	m_ctlSymbol.SetItem(idx, I_CURRPRC, LVIF_TEXT, _T(zCurrr), 0, 0, 0, NULL);

	// 처음 체결가를 오픈가로 한다.
	if (!bFirstFeedSaved)
	{
		m_ctlSymbol.SetItem(idx, I_OPENPRC, LVIF_TEXT, _T(zCurrr), 0, 0, 0, NULL);
	}
	//UpdateData(FALSE);
}

unsigned WINAPI CMudFishOrdDlg::Thread_ApiOrd(LPVOID lp)
{
	CMudFishOrdDlg* p = (CMudFishOrdDlg*)lp;

	char zSymbol[128];
	int nErrCode;

	if (!p->m_pApiClient[API_ORD]->Begin((LPSTR)p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD], 10))
	{
		p->showMsg(FALSE, "API API_ORD Socket Error[%s][%d](%s)",
			p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD], p->m_pApiClient[API_ORD]->GetMsg());
		Sleep(5000);
	}
	else
		p->showMsg(TRUE, "API API_ORD Socket OK(%s)(%d)", p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD]);

	p->m_pApiClient[API_ORD]->StartRecvData();
	

	while (p->m_bContinue)
	{
		Sleep(1);

		//connect
		//if (p->m_pApiClient[API_ORD])
		//{
		//	if (!p->m_pApiClient[API_ORD]->IsConnected())
		//	{
		//		sprintf(temp, "socket:%d, m_bConnect:%d\n", p->m_pApiClient[API_ORD]->m_sock,
		//			p->m_pApiClient[API_ORD]->m_bConn);
		//		OutputDebugString(temp);

		//		if (!p->m_pApiClient[API_ORD]->Connect())
		//		{
		//			p->showMsg(FALSE, "API API_ORD Socket Connect Error[%s][%d](%s)",
		//				p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD], p->m_pApiClient[API_ORD]->GetMsg());
		//			Sleep(5000);
		//			continue;
		//		}
		//		p->showMsg(TRUE, "API API_ORD Socket connect(%s)(%d)", p->m_sApiIP[API_ORD].c_str(), p->m_nApiPort[API_ORD]);
		//		Sleep(1000);
		//	}
		//}
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_DIE)
			{
				p->CloseApiSocket(API_ORD);
				p->showMsg(TRUE, "CloseApiSocket(API_ORD)-1");
				return 0;
			}
			if (msg.message == WM_SENDORD_API)
			{
				//SEND ORDER
				char* pData = (char*)msg.lParam;
				int res = p->m_pApiClient[API_ORD]->SendData(pData, (int)msg.wParam, &nErrCode);
				if (res<=0)
				{
					p->showMsg(FALSE,"API_ORD DATA Send ERROR:%.*s", (int)msg.wParam, pData);
					continue;
				}
				p->showMsg(TRUE, "API_ORD SEND(%.*s)", (int)msg.wParam, pData);
			}
		}

		if (p->m_pApiClient[API_ORD]->HappenedRecvError())
		{
			p->showMsg(FALSE,"API_ORD DATA RECV ERROR:%s", p->m_pApiClient[API_ORD]->GetMsg());
			continue;
		}
		char* pBuf = NULL;;
		if (!p->m_pMemPool->get(&pBuf))
		{
			p->showMsg(FALSE, "memory pool get error");
			continue;
		}
		int nLen = p->m_pApiClient[API_ORD]->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			p->m_pMemPool->release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			p->showMsg(FALSE, "API_ORD DATA PAKCET 이상(%s)(%s)", pBuf, p->m_pApiClient[API_ORD]->GetMsg());
			p->m_pMemPool->release(pBuf);
			continue;
		}

		//p->showMsg(TRUE, "API_ORD응답수신(%s)", pBuf);

		ITMAP_STRAT it;
		ST_API_ORD_RESPONSE *pData = (ST_API_ORD_RESPONSE*)pBuf;

		// API REJECT
		if( strncmp(pData->Code, CDAPI_ERROR, strlen(CDAPI_ERROR))==0)
		{
			p->ApiOrd_Err(pBuf);
			p->m_pMemPool->release(pBuf);
			continue;
		}
		else
		{
			sprintf(zSymbol, "%.*s", sizeof(pData->Symbol), pData->Symbol);
			CUtil::TrimAll(zSymbol, strlen(zSymbol));
			std::string sSymbol = zSymbol;

			it = p->m_mapStrat.find(sSymbol);
			if (it == p->m_mapStrat.end())
			{
				p->m_pMemPool->release(pBuf);
				p->showMsg(FALSE, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
				continue;
			}
		}

		if (strncmp(pData->Code, CDAPI_ORD_ACPT, strlen(CDAPI_ORD_ACPT)) == 0)
		{
			p->ApiOrd_Acpt(pBuf);
		}

		if (strncmp(pData->Code, CDAPI_ORD_REAL, strlen(CDAPI_ORD_REAL)) == 0)
		{
			p->ApiOrd_RealOrd(pBuf);
		}

		if (strncmp(pData->Code, CDAPI_CNTR_REAL, strlen(CDAPI_CNTR_REAL)) == 0)
		{
			p->ApiOrd_RealCntr(pBuf);
			// strat class 에 전달
			ST_STRAT* pStrat = (*it).second;
			UINT msg = WM_RECV_API_CNTR;
			PostThreadMessage(pStrat->m->GetMyThreadID(), msg, 0, (LPARAM)pBuf);
		}

		
	}
	p->CloseApiSocket(API_ORD);
	p->showMsg(TRUE, "CloseApiSocket(API_ORD)-2");
	return 0;
}


/*

typedef struct _ST_API_ERROR
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];	//999999
char	ApiMsgCd[10];
char	ApiMsg[128];
char	Date[8];
char	UUID[32];
char	TM[9];	//hhmmssmmm};
char	ETX[1];
}ST_API_ERROR;

*/
VOID CMudFishOrdDlg::ApiOrd_Err(char* pPacket)
{
	ST_API_ERROR* p = (ST_API_ERROR*)pPacket;
	showMsg(TRUE, "[주문오류](API CODE:%.*s) MSG(%.*s) UUID(%.*s)",
		sizeof(p->ApiMsgCd), p->ApiMsgCd, sizeof(p->ApiMsg), p->ApiMsg, sizeof(p->UUID), p->UUID);

	//TODO. SAVE
	PostThreadMessage(m_unSaveData, WM_SAVE_API_ORD, 0, (LPARAM)pPacket);
}


/*

typedef struct _ST_API_ORD_ACPT
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];	//100101
char	Symbol[LEN_SYMBOL];
char	OrdNo[LEN_ORD_NO];
char	OrdPrc[LEN_PRC];
char	OrdQty[LEN_QTY];
char	OrdProcTp[1];
char	RjctTp[1];		//0:정상 1:거부
char	UUID[32];
char	ApiDT[8];
char	ApiTM[9];		// HH:MM:SS
char	ETX[1];		//
}ST_API_ORD_RESPONSE;
*/
VOID CMudFishOrdDlg::ApiOrd_Acpt(char* pPacket)
{
	ST_API_ORD_RESPONSE* p = (ST_API_ORD_RESPONSE*)pPacket;

	g_log.log(LOGTP_ERR, "[주문접수](%.*s) No(%.*s) OrdProcTp(%c) ApiDT(%.8s) ApiTM(%.9S) UUID(%.32s)",
		sizeof(p->Symbol), p->Symbol,
		sizeof(p->OrdNo), p->OrdNo,
		p->OrdProcTp[0],
		p->ApiDT,
		p->ApiTM,
		p->UUID);

	//TODO. SAVE ?
	PostThreadMessage(m_unSaveData, WM_SAVE_API_ORD, 0, (LPARAM)pPacket);
}


/*
typedef struct _ST_API_ORD_REAL
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];
char	Symbol[LEN_SYMBOL];
char	OrdNo[LEN_ORD_NO];
char	Side[1];
char	OrdPrc[LEN_PRC];
char	OrdQty[LEN_QTY];
char	RemnQty[LEN_QTY];
char	OrgOrdNo[LEN_ORD_NO];
char	ApiDT[8];
char	ApiTM[9];		// HH:MM:SS
char	ETX[1];		//

}ST_API_ORD_REAL;
*/
VOID CMudFishOrdDlg::ApiOrd_RealOrd(char* pPacket)
{
	ST_API_ORD_REAL* p = (ST_API_ORD_REAL*)pPacket;
	//showMsg(TRUE, "[주문REAL수신](%.*s) No(%.*s) Side(%c) Prc(%.*s) Qty(%.*s) Rmnq(%.*s) DT(%.8s) TM(%.9s)",
	//	sizeof(p->Symbol), p->Symbol,
	//	sizeof(p->OrdNo), p->OrdNo,
	//	p->Side[0],
	//	sizeof(p->OrdPrc), p->OrdPrc,
	//	sizeof(p->OrdQty), p->OrdQty,
	//	sizeof(p->RemnQty), p->RemnQty,
	//	p->ApiDT, p->ApiTM);
}

/*
typedef struct _ST_API_CNTR_REAL
{
char	STX[1];
char	Len[SIZE_PACKET_LEN];
char	Code[LEN_PACKET_CODE];
char	Symbol[LEN_SYMBOL];
char	OrdNo[LEN_ORD_NO];
char	Side[1];
char	OrdPrc[LEN_PRC];
char	OrdQty[LEN_QTY];
char	CntrPrc[LEN_PRC];
char	CntrQty[LEN_QTY];
char	EngagedAmt[LEN_MONEY];	//약정금액
char	Cmsn[LEN_MONEY];
char	ApiOrdDT[8];
char	ApiOrdTM[9];		// HH:MM:SS
char	ApiCntrDT[8];
char	ApiCntrTM[9];		// HH:MM:SS
char	ETX[1];		//

}ST_API_CNTR_REAL;
*/
VOID CMudFishOrdDlg::ApiOrd_RealCntr(char* pPacket)
{
	ST_API_CNTR_REAL* p = (ST_API_CNTR_REAL*)pPacket;
	showMsg(TRUE, "[체결REAL수신](%.*s) No(%.*s) Side(%c) OPrc(%.*s) CPrc(%.*s)"
							"OQty(%.*s) CQty(%.*s) EnggAmt(%.*s) DT(%.8s) TM(%.9s)",
		sizeof(p->Symbol), p->Symbol,
		sizeof(p->OrdNo), p->OrdNo,
		p->Side[0],
		sizeof(p->OrdPrc), p->OrdPrc,
		sizeof(p->CntrPrc), p->CntrPrc,
		sizeof(p->OrdQty), p->OrdQty,
		sizeof(p->CntrQty), p->CntrQty,
		sizeof(p->EngagedAmt), p->EngagedAmt,
		p->ApiCntrDT, p->ApiCntrTM);

	CString symbol, bs, cntrprc, qty;
	symbol.Format("%.*s", sizeof(p->Symbol), p->Symbol);
	bs.Format("%.*s", sizeof(p->Side), p->Side);
	cntrprc.Format("%.*s", sizeof(p->CntrPrc), p->CntrPrc);
	qty.Format("%.*s", sizeof(p->CntrQty), p->CntrQty);


	//// grid 
	//UINT idx;
	//std::map<CString, UINT>::iterator it = m_mapIdxPl.find(symbol);
	//if (it == m_mapIdxPl.end())
	//{
	//	idx = m_ctlRealPl.GetItemCount();
	//	m_ctlRealPl.InsertItem(idx, symbol);
	//	m_ctlRealPl.SetItem(idx, N_BS, LVIF_TEXT, bs, 0, 0, 0, NULL);
	//	m_ctlRealPl.SetItem(idx, N_OPENPRC, LVIF_TEXT, _T("80.10"), 0, 0, 0, NULL);
	//	m_ctlRealPl.SetItem(idx, N_ENTRYPRC, LVIF_TEXT, cntrprc, 0, 0, 0, NULL);
	//	m_ctlRealPl.SetItem(idx, N_CURRPRC, LVIF_TEXT, cntrprc, 0, 0, 0, NULL);
	//	m_ctlRealPl.SetItem(idx, N_PLTICK, LVIF_TEXT, _T(""), 0, 0, 0, NULL);
	//	m_ctlRealPl.SetItem(idx, N_QTY, LVIF_TEXT, qty, 0, 0, 0, NULL);
	//	m_ctlRealPl.SetItem(idx, N_PL, LVIF_TEXT, _T(""), 0, 0, 0, NULL);

	//	m_mapIdxPl[symbol] = idx;

	//	return;
	//}
	//idx = (*it).second;

	//// bs
	//CString bs_bf_s, entr_bf_s, qty_bf_s;
	//double entr_bf, qty_bf;
	//double dCntrPrc = atof(cntrprc.GetString());
	//bs_bf_s = m_ctlRealPl.GetItemText(idx, N_BS);
	//entr_bf_s = m_ctlRealPl.GetItemText(idx, N_ENTRYPRC);
	//qty_bf_s = m_ctlRealPl.GetItemText(idx, N_QTY);

	//// 같은 방향이면 update
	//if (bs_bf_s = bs)
	//{
	//	double dAmt1 = atof(entr_bf_s.GetString()) * atof(qty_bf_s.GetString());
	//	double dAmt2 = dCntrPrc * atof(qty.GetString());
	//	double dQty = atof(qty.GetString()) + atof(qty_bf_s.GetString());
	//	double dAvg = (dAmt1 + dAmt2) / dQty;

	//	double dGap = 0;
	//	//if(bs=="B")	dGap = (dCntrPrc- dAvg) / 
	//}
}

BOOL CMudFishOrdDlg::GetSymbolInfo(CString& symbol, double* pTickVal, double* pTickSize, 
	double* pCurrPrc, int* pDotCnt)
{
	std::map<CString, UINT>::iterator it = m_mapIdxSymbol.find(symbol);
	if (it == m_mapIdxPl.end())
	{
		return FALSE;
	}
	UINT idx = (*it).second;

	*pTickVal = atof((m_ctlSymbol.GetItemText(idx, I_TICKVALUE)).GetString());
	*pTickSize = atof((m_ctlSymbol.GetItemText(idx, I_TICKSIZE)).GetString());
	*pCurrPrc = atof(m_ctlSymbol.GetItemText(idx, I_CURRPRC).GetString());
	*pDotCnt = atoi(m_ctlSymbol.GetItemText(idx, I_DOTCNT).GetString());

	return TRUE;
}


unsigned WINAPI CMudFishOrdDlg::Thread_SaveData(LPVOID lp)
{

	//CMudFishOrdDlg* p = (CMudFishOrdDlg*)lp;

	//ST_PACK2CHART_EX* pSise;
	//char zSymbol[128];
	//char tm[9];
	//char zCurrPrc[32];
	//int nErrCode, res;
	//while (p->m_bContinue)
	//{
	//	Sleep(1);
	//	MSG msg;
	//	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	//	{
	//		if (msg.message == WM_DIE)
	//		{
	//			return 0;
	//		}
	//		if (msg.message == WM_SENDORD_API)
	//		{
	//			char* pData = (char*)msg.lParam;
	//			int nLen = msg.wParam;

	//			res = p->m_pApiClient[API_ORD]->SendData(pData, nLen, &nErrCode);
	//			p->m_pMemPool->release(pData);
	//			if(res<=0)
	//			{
	//				p->showMsg(FALSE, "주문전송 에러(%d)(%s)", nErrCode, p->m_pApiClient[API_ORD]->GetMsg());
	//				continue;
	//			}
	//			p->showMsg(TRUE, "주문전송 성공(%d)(%.*s)", res,nLen, pData);
	//		}
	//	}

	//	if (p->m_pApiClient[API_ORD]->HappenedRecvError())
	//	{
	//		p->showMsg(FALSE, "API_ORD DATA RECV ERROR:%s", p->m_pApiClient[API_ORD]->GetMsg());
	//		continue;
	//	}
	//	char* pBuf = NULL;;
	//	if (!p->m_pMemPool->get(&pBuf))
	//	{
	//		p->showMsg(FALSE, "memory pool get error");
	//		continue;
	//	}
	//	int nLen = p->m_pApiClient[API_ORD]->GetOneRecvedPacket(pBuf);
	//	if (nLen == 0) {
	//		p->m_pMemPool->release(pBuf);
	//		continue;
	//	}
	//	if (nLen < 0)
	//	{
	//		p->showMsg(FALSE, "API_ORD DATA PAKCET 이상(%s)(%s)", pBuf, p->m_pApiClient[API_ORD]->GetMsg());
	//		p->m_pMemPool->release(pBuf);
	//		continue;
	//	}

	//	if (nLen > 0)
	//	{
	//		pSise = (ST_PACK2CHART_EX*)pBuf;
	//		sprintf(tm, "%.2s:%.2s:%.2s", pSise->Time, pSise->Time + 2, pSise->Time + 4);
	//		memcpy(pSise->Time, tm, sizeof(pSise->Time));
	//		sprintf(zCurrPrc, "%.*s", sizeof(pSise->Close), pSise->Close);

	//		sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
	//		CUtil::TrimAll(zSymbol, strlen(zSymbol));
	//		std::string sSymbol = zSymbol;

	//		ITMAP_STRAT it = p->m_mapStrat.find(sSymbol);
	//		if (it == p->m_mapStrat.end())
	//		{
	//			p->m_pMemPool->release(pBuf);
	//			continue;
	//			//g_log.log(LOGTP_ERR, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
	//		}

	//		// 화면에 표시
	//		//p->DatafeedProc(sSymbol, pBuf);
	//		p->showMsg(TRUE, "주문수신(%d)(%.*s)", nLen, nLen, pBuf);

	//		// strat class 에 전달
	//		ST_STRAT* pStrat = (*it).second;
	//		PostThreadMessage(pStrat->m->GetStratThreadId(), WM_RECV_API_ORD, 0, (LPARAM)pBuf);
	//	}
	//}
	return 0;
}


void CMudFishOrdDlg::ReSetSymbolInfo()
{
	int nCount = m_ctlSymbol.GetItemCount();
	if (nCount == 0)
		return;

	ITMAP_STRAT it;

	for (int i = 0; i < nCount; i++)
	{
		CString sSymbol = m_ctlSymbol.GetItemText(i, I_SYMBOL);
		CString sOpenPrc = m_ctlSymbol.GetItemText(i, I_OPENPRC);
		CString sOrdQty = m_ctlSymbol.GetItemText(i, I_ORDQTY);
		CString sEndTM = m_ctlSymbol.GetItemText(i, I_ENDTM);
		CString sMaxSLCnt = m_ctlSymbol.GetItemText(i, I_MAXSL);
		CString sMaxPTCnt = m_ctlSymbol.GetItemText(i, I_MAXPT);
		CString sTickVal = m_ctlSymbol.GetItemText(i, I_TICKVALUE);
		CString sTickSize = m_ctlSymbol.GetItemText(i, I_TICKSIZE);
		CString sDotCnt = m_ctlSymbol.GetItemText(i, I_DOTCNT);
		CString sEntrySpread = m_ctlSymbol.GetItemText(i, I_ENTRY_SPREAD);
		CString sClrSpread = m_ctlSymbol.GetItemText(i, I_CLR_SPREAD);
		CString sPtPoint = m_ctlSymbol.GetItemText(i, I_PT_POINT);

		std::string symbol = (LPCSTR)sSymbol;
		it = m_mapStrat.find(symbol);
		if (it != m_mapStrat.end())
		{
			ST_STRAT* s = (*it).second;
			s->h->SetInitInfo(
				atof(sTickVal.GetString()),
				atof(sTickSize.GetString()),
				atoi(sDotCnt.GetString()),
				(LPSTR)sOpenPrc.GetString(),
				atoi(sOrdQty.GetString()),
				(LPSTR)sEndTM.GetString(),
				atoi(sMaxSLCnt.GetString()),
				atoi(sMaxPTCnt.GetString()),
				atof(sEntrySpread.GetString()),
				atof(sClrSpread.GetString()),
				atof(sPtPoint.GetString())
			);

			s->m->SetInitInfo(
				m_pMemPool,
				m_unSaveData,
				m_unApiOrd);
			m_mapStrat[symbol] = s;
		}
	} //for (int i = 0; i < nCount; i++)
}

BOOL CMudFishOrdDlg::LoadSymbolInfo(BOOL bCreateStrat)
{
	showMsg(TRUE, "trying to LoadSymbolInfo...");
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	if (!m_pDBPool)
	{
		m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
		if (!m_pDBPool->Init(atoi(cnt)))
		{
			showMsg(FALSE, "DB OPEN FAILED.(%s)(%s)(%s)", ip, id, pwd);
			return FALSE;
		}
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

		char data[128], zSymbol[128];
		double dData;
		int nDotCnt;
		int nRow = 0;
		while (db->IsNextRow())
		{
			db->GetStr("SYMBOL", zSymbol);
			m_ctlSymbol.InsertItem(nRow, zSymbol);

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

			//db->GetStr("OPEN_PRC", data);
			//m_ctlSymbol.SetItem(nRow, I_OPENPRC, LVIF_TEXT, _T(data), 0, 0, 0, NULL);

			db->GetStr("ORD_QTY", data);
			m_ctlSymbol.SetItem(nRow, I_ORDQTY, LVIF_TEXT, _T(data), 0, 0, 0, NULL);

			db->GetStr("END_TM", data);
			m_ctlSymbol.SetItem(nRow, I_ENDTM, LVIF_TEXT, _T(data), 0, 0, 0, NULL);

			nDotCnt = db->GetLong("MAXCNT_SL");
			sprintf(data, "%d", nDotCnt);
			m_ctlSymbol.SetItem(nRow, I_MAXSL, LVIF_TEXT, data, 0, 0, 0, NULL);

			nDotCnt = db->GetLong("MAXCNT_PT");
			sprintf(data, "%d", nDotCnt);
			m_ctlSymbol.SetItem(nRow, I_MAXPT, LVIF_TEXT, data, 0, 0, 0, NULL);

			dData = db->GetDouble("ENTRY_SPREAD");
			sprintf(data, "%.5f", dData);
			m_ctlSymbol.SetItem(nRow, I_ENTRY_SPREAD, LVIF_TEXT, data, 0, 0, 0, NULL);

			dData = db->GetDouble("CLR_SPREAD");
			sprintf(data, "%.5f", dData);
			m_ctlSymbol.SetItem(nRow, I_CLR_SPREAD, LVIF_TEXT, data, 0, 0, 0, NULL);

			dData = db->GetDouble("PT_POINT");
			sprintf(data, "%.5f", dData);
			m_ctlSymbol.SetItem(nRow, I_PT_POINT, LVIF_TEXT, data, 0, 0, 0, NULL);

			if (bCreateStrat)
			{
				ST_STRAT* st = new ST_STRAT;
				st->h = new CStratHistManager(zSymbol);
				st->m = new CStratMaker(zSymbol, st->h);
				st->bFirstFeed = FALSE;
				m_mapStrat[zSymbol] = st;
				ReSetSymbolInfo();
			}

			m_mapIdxSymbol[CString(zSymbol)] = nRow;


			nRow++;
			db->Next();

		}

	}

	db->Close();

	showMsg(TRUE, "Succeeded in LoadSymbolInfo...");
	return TRUE;
}

VOID CMudFishOrdDlg::End()
{
	OnBnClickedButtonStratStop();
	SAFE_DELETE(m_pMemPool);
	SAFE_DELETE(m_pDBPool);
	ClearStratMap();
}


VOID CMudFishOrdDlg::ClearStratMap()
{
	ITMAP_STRAT it;
	for (it = m_mapStrat.begin(); it != m_mapStrat.end();++it )
	{
		ST_STRAT* s = (*it).second;
		delete s->h;
		delete s->m;
		delete s;
	}
	m_mapStrat.clear();
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
	if(bSucc)	g_log.log(INFO, buff1);
	else		g_log.log(ERR, buff1);
}

void CMudFishOrdDlg::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	showMsg(TRUE, "종목정보를 Loding 합니다....");
	if (!LoadSymbolInfo(FALSE))
	{
		showMsg(FALSE, "종목정보 Loding 에 실패");
	}
	else
		showMsg(TRUE, "종목정보 Loding 에 성공");
}


void CMudFishOrdDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	int nCount = m_ctlSymbol.GetItemCount();
	if (nCount == 0)
		return;

	CString sSymbol, sOpenPrc, sOrdQty, sEndTM, sMaxSL, sMaxPT, sEntrySpread, sClrSpread, sPtPoint;

	for (int i = 0; i < nCount; i++)
	{
		if (m_ctlSymbol.GetCheck(i))
		{
			sSymbol = m_ctlSymbol.GetItemText(i, I_SYMBOL);
			sOpenPrc = m_ctlSymbol.GetItemText(i, I_OPENPRC);
			sOrdQty = m_ctlSymbol.GetItemText(i, I_ORDQTY);
			sEndTM = m_ctlSymbol.GetItemText(i, I_ENDTM);
			sMaxSL = m_ctlSymbol.GetItemText(i, I_MAXSL);
			sMaxPT = m_ctlSymbol.GetItemText(i, I_MAXPT);
			sEntrySpread = m_ctlSymbol.GetItemText(i, I_ENTRY_SPREAD);
			sClrSpread = m_ctlSymbol.GetItemText(i, I_CLR_SPREAD);
			sPtPoint = m_ctlSymbol.GetItemText(i, I_PT_POINT);

			CDBHandlerAdo db(m_pDBPool->Get());
			char zQ[1024];
			sprintf(zQ, "EXEC MF_SYMBOL_TRADE_CONFIG_SAVE "
				"'%s', "	//sSymbol
				"'%s', "	//sOpenPrc
				"'%s', "	//sOrdQty
				"'%s', "	//sEndTM
				"'%s', "	//sMaxSL
				"'%s', "	//sMaxPT
				"'%s', "	//ENTRY_SPREAD
				"'%s', "	//CLR_SPREAD
				"'%s' "	//PT_POINT
				,
				(LPCSTR)sSymbol,
				(LPCSTR)sOpenPrc,
				(LPCSTR)sOrdQty,
				(LPCSTR)sEndTM,
				(LPCSTR)sMaxSL,
				(LPCSTR)sMaxPT,
				(LPCSTR)sEntrySpread,
				(LPCSTR)sClrSpread,
				(LPCSTR)sPtPoint
			);
			if (FALSE == db->ExecQuery(zQ))
			{
				showMsg(FALSE, "MF_SYMBOL_TRADE_CONFIG_SAVE Err (%s)(%s)", db->GetError(), zQ);
			}
			else
			{
				showMsg(TRUE, "MF_SYMBOL_TRADE_CONFIG_SAVE OK(%s)", zQ);
			}
			db->Close();
		} // if (m_ctlSymbol.GetCheck(i))

	} //for (int i = 0; i < nCount; i++)


	// STRAT_HIST 에 저장
	
	ITMAP_STRAT it = m_mapStrat.find(std::string(sSymbol.GetBuffer()));
	if (it != m_mapStrat.end())
	{
		ST_STRAT* p = (*it).second;
		p->h->SetConfigInfo((LPCSTR)sOpenPrc, (LPCSTR)sOrdQty, (LPCSTR)sEndTM
			, (LPCSTR)sMaxSL, (LPCSTR)sMaxPT, (LPCSTR)sEntrySpread, (LPCSTR)sClrSpread,
			(LPCSTR)sPtPoint);
	}
	showMsg(TRUE, "종목정보 저장하였습니다...");
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
				if (nChar == VK_RETURN || nChar==VK_TAB)
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


void CMudFishOrdDlg::OnBnClickedButtonStratStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	showMsg(TRUE, "=======================================================");
	showMsg(TRUE, "전략을 시작합니다!!!!!");
	showMsg(TRUE, "=======================================================");
	m_bContinue = TRUE;
	if (!LoadSymbolInfo(FALSE))
	{
		return;
	}

	InitApiSocket(API_ORD);
	InitApiSocket(API_TICK);

	// create recv thread
	m_hApiTick = (HANDLE)_beginthreadex(NULL, 0, &Thread_ApiTick, this, 0, &m_unApiTick);

	// create ord thread
	m_hApiOrd = (HANDLE)_beginthreadex(NULL, 0, &Thread_ApiOrd, this, 0, &m_unApiOrd);

	// create save thread
	m_hSaveData = (HANDLE)_beginthreadex(NULL, 0, &Thread_SaveData, this, 0, &m_unSaveData);

	ITMAP_STRAT it;
	for (it = m_mapStrat.begin(); it != m_mapStrat.end(); ++it)
	{
		ST_STRAT* p = (*it).second;
		p->m->SetInitInfo(
			m_pMemPool,
			m_unSaveData,
			m_unApiOrd);
	}
}


void CMudFishOrdDlg::OnBnClickedButtonStratStop()
{
	showMsg(TRUE, "전략을 중단합니다.");
	m_bContinue = FALSE;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PostThreadMessage(m_unApiTick, WM_DIE, 0, 0);
	PostThreadMessage(m_unApiOrd, WM_DIE, 0, 0);
	PostThreadMessage(m_unSaveData, WM_DIE, 0, 0);

	SAFE_CLOSEHANDLE(m_hApiTick);
	SAFE_CLOSEHANDLE(m_hApiOrd);
	SAFE_CLOSEHANDLE(m_hSaveData);
	ClearStratMap();
}



void CMudFishOrdDlg::InitApiSocket(int Idx)
{
	char ip[32], port[32];

	if (Idx == API_ORD)
	{
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "ORD_IP", ip);
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "ORD_PORT", port);
		m_sApiIP[API_ORD] = ip;
		m_nApiPort[API_ORD] = atoi(port);

		if (m_pApiClient[API_ORD] == NULL)
			m_pApiClient[API_ORD] = new CTcpClient("API_ORD");
	}

	if (Idx == API_TICK)
	{
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "SISE_IP", ip);
		CUtil::GetConfig(g_zConfig, "API_SOCKET_INFO", "SISE_PORT", port);
		m_sApiIP[API_TICK] = ip;
		m_nApiPort[API_TICK] = atoi(port);

		if (m_pApiClient[API_TICK] == NULL)
			m_pApiClient[API_TICK] = new CTcpClient("MD");
	}
}

void CMudFishOrdDlg::CloseApiSocket(int Idx)
{
	if (Idx == API_ORD) {
		//showMsg(TRUE, "CloseApiSocket ORD");
		SAFE_DELETE(m_pApiClient[API_ORD]);
	}

	if (Idx == API_TICK)
	{
		//showMsg(TRUE, "CloseApiSocket API_TICK");
		SAFE_DELETE(m_pApiClient[API_TICK]);
	}
}

void CMudFishOrdDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case ID_TIME_INIT:
		{
			KillTimer(ID_TIME_INIT);
			LoadSymbolInfo(TRUE);
			
			// tcp init
			InitApiSocket(API_ORD);
			InitApiSocket(API_TICK);
		}
	}
	CDialog::OnTimer(nIDEvent);
}




void CMudFishOrdDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	End();
	CDialogEx::OnCancel();
}


void CMudFishOrdDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	End();
	CDialogEx::OnOK();
}


void CMudFishOrdDlg::OnBnClickedButtonClosePos()
{
	int result = AfxMessageBox("현재 포지션을 청산합니다.청산 후에는 재실행 전까지 해당 종목은 운용되지 않습니다.", MB_YESNO, 0);

	if (result == IDNO)
		return;

	int nCount = m_ctlSymbol.GetItemCount();
	if (nCount == 0) {
		AfxMessageBox("청산할 종목을 선택하셔야 합니다.", MB_OK);
		return;
	}

	CString sSymbol;

	for (int i = 0; i < nCount; i++)
	{
		if (m_ctlSymbol.GetCheck(i))
		{
			sSymbol = m_ctlSymbol.GetItemText(i, I_SYMBOL);

			ITMAP_STRAT it = m_mapStrat.find(std::string(sSymbol.GetBuffer()));
			if (it != m_mapStrat.end())
			{
				ST_STRAT* p = (*it).second;
				PostThreadMessage(p->m->GetStratThreadId(), WM_CLOSE_POSITION, 0,0);
			}
		}
	}
}
	