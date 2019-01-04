// TRBar.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "TRBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTRBar

CTRBar::CTRBar()
{
}

CTRBar::~CTRBar()
{
}


BEGIN_MESSAGE_MAP(CTRBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CTRBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTRBar message handlers

int CTRBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_dlgTRBar.Create( CDlgTRBar::IDD, this );
	CRect rcClient; GetClientRect( rcClient );
	m_dlgTRBar.MoveWindow( rcClient );
	m_dlgTRBar.ShowWindow( SW_SHOW );
	return 0;
}

void CTRBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	
	m_dlgTRBar.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE );
	
}
