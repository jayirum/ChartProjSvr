// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "XingAPI_Sample.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/IRUM_Common.h"

CLogMsg g_log;
char	g_zConfig[_MAX_PATH];



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WMU_SHOW_TR, OnShowTR )
	ON_MESSAGE( WM_USER + XM_DISCONNECT, OnDisconnect )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if( !m_wndTRBar.Create( "", this, 2000, WS_CHILD | WS_VISIBLE | CBRS_LEFT ) )
	{
		TRACE( "Failed to create tr bar\n" );
		return -1;		// fail to create
	}	
	EnableDocking(CBRS_ALIGN_ANY);


	//	GET LOG DIR
	char	msg[512] = { 0, };
	CHAR	szDir[_MAX_PATH] = { 0 };
	CProp prop;
	if (!prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY))
	{
		printf("registry open error\n");
		return 0;
	}

	strcpy(szDir, prop.GetValue("CONFIG_DIR_ETK_X"));
	if (szDir[0] == 0)
	{
		printf("registry GetValue error\n");
		return 0;
	}

	char szNotificationServer[32], szNotificationPort[32];
	CUtil::GetCnfgFileNm(szDir, "ETK_X", g_zConfig);
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	CUtil::GetConfig(g_zConfig, "NOTIFICATION", "NOTIFICATION_SERVER_IP", szNotificationServer);
	CUtil::GetConfig(g_zConfig, "NOTIFICATION", "NOTIFICATION_SERVER_PORT", szNotificationPort);

	g_log.OpenLogEx(szDir, "ETK_X", szNotificationServer, atoi(szNotificationPort), "ETK_X");



	//-----------------------------------------------------------------------------------------
	// X-ing API �ʱ�ȭ
	
	// ���� Directroy���� ���� �ʱ�ȭ ���Ŀ� �����ϸ�
	if( g_iXingAPI.Init() == FALSE )
	{
		// ���� Directory�� ã���Ŀ� 
		CString strPath;
		int nLen = (int)GetModuleFileName( AfxGetInstanceHandle(), strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		strPath.ReleaseBuffer( nLen );
		int nFind = strPath.ReverseFind( '\\' );
		if( nFind > 0 )
		{
			strPath = strPath.Left( nFind );
		}
		
		// ���� Directory�� �Է��Ͽ� �ʱ�ȭ �Ѵ�.
		if( g_iXingAPI.Init( strPath ) == FALSE )
		{
			MessageBox( "X-ingAPI DLL�� Load �� �� �����ϴ�.");
			g_log.log(NOTIFY, "X-ingAPI DLL�� Load �� �� �����ϴ�.(%s)", (LPSTR)(LPCSTR)strPath);
			return -1;
		}
	}
	//-----------------------------------------------------------------------------------------
	
	return 0;
}

void CMainFrame::OnClose() 
{
	// Logout �Ѵ�
	//		���� ������ ���� �ʿ�� ����.
	//		Login�� ���� ���� ���¿��� Logout�� �ϸ� FALSE�� Return �ǹǷ� Login ���θ� üũ���� �ʾƵ� �ȴ�.
	g_iXingAPI.Logout( GetSafeHwnd() );
	
	CMDIFrameWnd::OnClose();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


LRESULT CMainFrame::OnShowTR( WPARAM wParam, LPARAM lParam )
{
	LPCTSTR pszTR = (LPCTSTR)wParam;
	TRACE( "Show Window : %s\n", pszTR );
	
#if 0	
	CChildFrame* pWnd = (CChildFrame*)CreateNewChild( RUNTIME_CLASS(CChildFrame), IDR_XINGAPTYPE, NULL, NULL );
	pWnd->SetTR( pszTR );
#else
	
	// skeo 2011-01-11
	map< LPCTSTR, CChildFrame * >::iterator it = m_CFrmMap.find( pszTR );
	if (it == m_CFrmMap.end())
	{
		CChildFrame* pWnd = (CChildFrame*)CreateNewChild( RUNTIME_CLASS(CChildFrame), IDR_XINGAPTYPE, NULL, NULL );
		pWnd->SetTR( pszTR );
		
		m_CFrmMap[ pszTR ] = pWnd;
	}
#endif

	return 0L;
}

//--------------------------------------------------------------------------------------
// �������� ����
//--------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDisconnect( WPARAM wParam, LPARAM lParam )
{
	g_log.log(INFO, "Disconnected from ebest");
	//CUtil::PrintLogEx(&m_logF, FALSE, " �������� ������ ����Ǿ����ϴ�!!");
	
	SetTimer(1000, 15*1000, NULL);	// skeo. �翬��õ� Ÿ�̸� ����

	return 0L;
}

// skeo 2011-01-11
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1000)
	{
		// Login. 
		CDlgLogin dlg( this );
		if( dlg.DoModal() == IDCANCEL )
		{
			//this->DestroyWindow();
			return;
		}			
	}	
	// �׽�Ʈ Ÿ�̸�
	else if(nIDEvent == 1001)
	{
		// Logout �Ѵ�
		//		���� ������ ���� �ʿ�� ����.
		//		Login�� ���� ���� ���¿��� Logout�� �ϸ� FALSE�� Return �ǹǷ� Login ���θ� üũ���� �ʾƵ� �ȴ�.
		//g_iXingAPI.Logout( GetSafeHwnd() );
		if(g_iXingAPI.IsConnected() == TRUE)
		{
			if(g_iXingAPI.Logout( GetSafeHwnd() ) == TRUE)
			{
				if(g_iXingAPI.Disconnect() == TRUE){					
					OnDisconnect( (WPARAM)(LPCTSTR)"FC0", NULL );
					OnDisconnect( (WPARAM)(LPCTSTR)"FC0_1", NULL );
				}
			}
			else{
				g_iXingAPI.Disconnect();
			}			
		}
	}
	
	CMDIFrameWnd::OnTimer(nIDEvent);
}
