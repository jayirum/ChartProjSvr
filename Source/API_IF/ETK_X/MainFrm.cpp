// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "XingAPI_Sample.h"

#include "MainFrm.h"
#include "ChildFrm.h"

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

	//-----------------------------------------------------------------------------------------
	// X-ing API 초기화
	
	// 현재 Directroy에서 먼저 초기화 한후에 실패하면
	if( g_iXingAPI.Init() == FALSE )
	{
		// 메인 Directory를 찾은후에 
		CString strPath;
		int nLen = (int)GetModuleFileName( AfxGetInstanceHandle(), strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		strPath.ReleaseBuffer( nLen );
		int nFind = strPath.ReverseFind( '\\' );
		if( nFind > 0 )
		{
			strPath = strPath.Left( nFind );
		}
		
		// 메인 Directory를 입력하여 초기화 한다.
		if( g_iXingAPI.Init( strPath ) == FALSE )
		{
			MessageBox( "X-ingAPI DLL을 Load 할 수 없습니다.");
			return -1;
		}
	}
	//-----------------------------------------------------------------------------------------
	
		//SetTimer(1000, 1000, NULL);		// 테스트 타이머. skeo 2011-01-11
		//SetTimer(1001, 30000, NULL);		// 테스트 타이머. skeo 2011-01-11
	return 0;
}

void CMainFrame::OnClose() 
{
	// Logout 한다
	//		굳이 응답을 받을 필요는 없다.
	//		Login을 하지 않은 상태에서 Logout을 하면 FALSE가 Return 되므로 Login 여부를 체크하지 않아도 된다.
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
// 서버연결 종료
//--------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDisconnect( WPARAM wParam, LPARAM lParam )
{
	//MessageBox( "서버와의 연결이 종료되었습니다.", "통신에러", MB_ICONSTOP );

	// skeo 2011-01-11 [[
	//----------------------------------------
	//	LOG FIL OPEN
	//----------------------------------------
	char szPath[_MAX_PATH] = {0,};
	char szMsg[256];
	if( !CUtil::GetLogDir(szPath, szMsg) ){
		return FALSE;	
	}
	m_logF.OpenLog(szPath, "ETK_DISCONNECT");
	CUtil::PrintLogEx(&m_logF, FALSE, " 서버와의 연결이 종료되었습니다!!");
	
	SetTimer(1000, 15*1000, NULL);	// skeo. 재연결시도 타이머 생성
	// ]]
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
	// 테스트 타이머
	else if(nIDEvent == 1001)
	{
		// Logout 한다
		//		굳이 응답을 받을 필요는 없다.
		//		Login을 하지 않은 상태에서 Logout을 하면 FALSE가 Return 되므로 Login 여부를 체크하지 않아도 된다.
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
