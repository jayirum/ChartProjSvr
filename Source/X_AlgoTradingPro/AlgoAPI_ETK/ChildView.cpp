// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

#define HEIGHT_MSG_BAR	24

CChildView::CChildView()
{
	m_pDlg = NULL;
}

CChildView::~CChildView()
{
	if( m_pDlg )
	{
		delete m_pDlg;
	}
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WMU_SET_MESSAGE, OnSetMessage )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPCHILDREN;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

BOOL CChildView::SetTR( CRuntimeClass* pClass, UINT uID )
{
	CDialog* pDlg = (CDialog*)pClass->CreateObject();
	pDlg->Create( uID, this );
	
	CRect rcChild; pDlg->GetWindowRect( rcChild );
	CRect rcClient; GetClientRect( rcClient );
	CRect rcWindow; GetParent()->GetWindowRect( rcWindow );

	rcWindow.right  += rcChild.Width()  - rcClient.Width();
	rcWindow.bottom += rcChild.Height() - rcClient.Height();
	GetParent()->SetWindowPos( NULL, 0, 0, rcWindow.Width(), rcWindow.Height()+HEIGHT_MSG_BAR, SWP_NOMOVE | SWP_NOZORDER );

	pDlg->ShowWindow( SW_SHOW );
	m_pDlg = pDlg;
	
	return FALSE;
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	if( m_pDlg )
	{
		m_pDlg->SetWindowPos( NULL, 0, 0, cx, cy-HEIGHT_MSG_BAR, SWP_NOZORDER | SWP_NOMOVE );
	}
	
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);

	CRect rcClient; GetClientRect( rcClient );
	rcClient.top += rcClient.bottom - ( HEIGHT_MSG_BAR );
	
	dc.FillSolidRect( rcClient, GetSysColor( COLOR_3DFACE ) );
	rcClient.DeflateRect( 1, 1 );
	dc.Draw3dRect( rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), GetSysColor( COLOR_3DDKSHADOW ), GetSysColor( COLOR_3DHILIGHT ) );

	rcClient.DeflateRect( 2, 2 );

	CFont font; font.CreatePointFont( 90, "±¼¸²Ã¼" );
	CFont* pOldFont = dc.SelectObject( &font );
	dc.SetTextColor( GetSysColor( COLOR_BTNTEXT ) );
	dc.SetBkColor( GetSysColor( COLOR_3DFACE ) );
	dc.DrawText( m_strMsg, rcClient, DT_VCENTER | DT_SINGLELINE );
	dc.SelectObject( pOldFont );
}

LRESULT CChildView::OnSetMessage( WPARAM wParam, LPARAM lParam )
{
	m_strMsg.Format( "[%s] %s", wParam, lParam );
	Invalidate();

	return 0L;
}