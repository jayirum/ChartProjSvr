// XingAPI_Sample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XingAPI_Sample.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "DlgLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXingAPI_SampleApp

BEGIN_MESSAGE_MAP(CXingAPI_SampleApp, CWinApp)
	//{{AFX_MSG_MAP(CXingAPI_SampleApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXingAPI_SampleApp construction

CXingAPI_SampleApp::CXingAPI_SampleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXingAPI_SampleApp object

CXingAPI_SampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CXingAPI_SampleApp initialization

BOOL CXingAPI_SampleApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMDIFrameWnd* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_XINGAPTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_XINGAPTYPE));



	// The main window has been initialized, so show and update it.
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	

	//-----------------------------------------------------------------------------------------
	// Login 표시
	CDlgLogin dlg( pFrame );	
	if( dlg.DoModal() == IDCANCEL )
	{
		pFrame->DestroyWindow();
		return TRUE;
	}
	//-----------------------------------------------------------------------------------------

	DWORD id;
	if( (m_hDie	= CreateEvent(NULL, TRUE, FALSE, NULL))==NULL )
	{
		return FALSE;
	}
	
	if( (m_hThread=::CreateThread(NULL,0,ThreadProc,this,0,&id))==NULL )
	{
		return FALSE;
	}


	return TRUE;
}

DWORD WINAPI CXingAPI_SampleApp::ThreadProc(LPVOID lp)
{	
	CXingAPI_SampleApp* p = (CXingAPI_SampleApp*)lp;

	SYSTEMTIME st;
	char szNow[32];
	while( WaitForSingleObject(p->m_hDie, 10000)!=WAIT_OBJECT_0 )
	{
		GetLocalTime(&st);
		sprintf( szNow, "%02d:%02d", st.wHour, st.wMinute );

		if( strcmp( szNow, "16:00" )>=0 && strcmp( szNow, "21:00" )<0 )
		{
			//	종가를 입력하도록 현재가 조회 창을 띄운다.
			//AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)"t2101", NULL );
			return 0;
		}
	}
	return 0;
}


VOID CXingAPI_SampleApp::StopThread()
{
	SetEvent(m_hDie);
	if(WaitForSingleObject(m_hThread, 3000)!=WAIT_OBJECT_0)
		TerminateThread(m_hThread,0);
	
	if(m_hThread){
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if(m_hDie){
		CloseHandle(m_hDie);
		m_hDie = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXingAPI_SampleApp message handlers

int CXingAPI_SampleApp::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

void CXingAPI_SampleApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_XINGAPTYPE, m_hMDIMenu, m_hMDIAccel);
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CXingAPI_SampleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CXingAPI_SampleApp message handlers

