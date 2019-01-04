/////////////////////////////////////////////////////////////////////////
//
// CSizingControlBarG           Version 2.42
// 
// Created: Jan 24, 1998        Last Modified: Feb 10, 2000
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2000 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

// sizecbar.cpp : implementation file
//

#include "stdafx.h"
#include "scbarg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CSizingControlBarG

IMPLEMENT_DYNAMIC(CSizingControlBarG, baseCSizingControlBarG);

CSizingControlBarG::CSizingControlBarG()
{
    m_cyGripper = 18;
    m_bActive	= FALSE;

//	m_biMenu.SetWindowText( "¡¿" );
//	m_biHide.SetWindowText( "¢º" );
	m_biMenu.SetWindowText( "" );
	m_biHide.SetWindowText( "¡¿" );
}

CSizingControlBarG::~CSizingControlBarG()
{
}

BEGIN_MESSAGE_MAP(CSizingControlBarG, baseCSizingControlBarG)
    //{{AFX_MSG_MAP(CSizingControlBarG)
    ON_WM_NCLBUTTONUP()
    //ON_WM_NCHITTEST()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
// CSizingControlBarG message handlers

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//

void CSizingControlBarG::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	switch( nHitTest )
	{
	case HTCLOSE : m_pDockSite->ShowControlBar( this, FALSE, FALSE ); break;
//	case HTZOOM  : ShowMenu();	break;
	}

    baseCSizingControlBarG::OnNcLButtonUp(nHitTest, point);
}

void CSizingControlBarG::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
    // subtract edges
    baseCSizingControlBarG::NcCalcClient(pRc, nDockBarID);

    if (!HasGripper()) return;

    CRect rc(pRc);

    BOOL bHorz = (nDockBarID == AFX_IDW_DOCKBAR_TOP) || (nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);

    if (bHorz)	rc.DeflateRect( m_cyGripper, 0, 0, 0 );
    else		rc.DeflateRect( 0, m_cyGripper, 0, 0 );

    // set position for the "x" (hide bar) button
    CPoint ptOrgBtn = bHorz ? CPoint( rc.left - 9, rc.top + 8 ) : CPoint( rc.right - 8, rc.top - 11 );
	CPoint pt		= ptOrgBtn - CRect( pRc ).TopLeft();
	
	m_biHide.Move( pt );
	
	if( bHorz ) pt.Offset(   0, 12 );
	else		pt.Offset( -12,  0 );
    m_biMenu.Move( pt );

    *pRc = rc;
}

// gradient defines (if not already defined)
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28
#define SPI_GETGRADIENTCAPTIONS         0x1008
#endif

void CSizingControlBarG::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    if( !HasGripper() ) return;

    // compute the caption rectangle
    BOOL  bHorz  = IsHorzDocked();
    CRect rcGrip = rcClient;
    CRect rcBtn  = m_biMenu.GetRect();
    
	if( bHorz )
    {   // right side gripper
        rcGrip.left  -= m_cyGripper  +  1;
        rcGrip.right  = rcGrip.left  + m_cyGripper - 1;
    }
    else
    {   // gripper at top
        rcGrip.top    -= m_cyGripper +  1;
        rcGrip.bottom  = rcGrip.top  + m_cyGripper - 1;
    }
    rcGrip.InflateRect( bHorz ? 1 : 0, bHorz ? 0 : 1 );
	
	pDC->FillSolidRect( rcGrip, ::GetSysColor( COLOR_3DSHADOW ) );
	COLORREF clrOldText	= pDC->SetTextColor( ::GetSysColor( COLOR_CAPTIONTEXT ) );
	pDC->FillSolidRect( rcGrip.left, rcGrip.top, rcGrip.Width(), 1, ::GetSysColor( COLOR_BTNTEXT ) );
	if( m_bActive )
	{
//		pDC->FillSolidRect( rcGrip, ::GetSysColor( COLOR_ACTIVECAPTION ) );
	}
	else
	{
//		pDC->FillSolidRect( rcGrip, ::GetSysColor( COLOR_BTNFACE ) );
//		pDC->Draw3dRect( rcGrip, ::GetSysColor( COLOR_3DSHADOW ), ::GetSysColor( COLOR_3DSHADOW ) );
//		clrOldText	= pDC->SetTextColor( GetSysColor( COLOR_3DSHADOW ) );
	}
/*
    // draw the caption background
    COLORREF clrCptn = m_bActive ? ::GetSysColor(COLOR_ACTIVECAPTION) : ::GetSysColor(COLOR_INACTIVECAPTION);

    // query gradient info (usually TRUE for Win98/Win2k)
    BOOL bGradient = FALSE;
//    ::SystemParametersInfo( SPI_GETGRADIENTCAPTIONS, 0, &bGradient, 0 );
    
    if( !bGradient )
	{
		pDC->FillSolidRect(&rcGrip, clrCptn); // solid color
	}
    else
    {
        // gradient from left to right or from bottom to top
        // get second gradient color (the right end)
        COLORREF clrCptnRight = ::GetSysColor( m_bActive ? COLOR_GRADIENTACTIVECAPTION : COLOR_GRADIENTINACTIVECAPTION );

        // this will make 2^6 = 64 fountain steps
        int nShift = 6;
        int nSteps = 1 << nShift;

        for (int i = 0; i < nSteps; i++)
        {
            // do a little alpha blending
            int nR = ( GetRValue( clrCptn ) * ( nSteps - i ) + GetRValue( clrCptnRight ) * i ) >> nShift;
            int nG = ( GetGValue( clrCptn ) * ( nSteps - i ) + GetGValue( clrCptnRight ) * i ) >> nShift;
            int nB = ( GetBValue( clrCptn ) * ( nSteps - i ) + GetBValue( clrCptnRight ) * i ) >> nShift;

            COLORREF cr = RGB(nR, nG, nB);

            // then paint with the resulting color
            CRect r2 = rcGrip;
            if (bHorz)
            {
                r2.bottom = rcGrip.bottom - ( ( i		  * rcGrip.Height() ) >> nShift );
                r2.top	  = rcGrip.bottom - ( ( ( i + 1 ) * rcGrip.Height() ) >> nShift );
                if( r2.Height() > 0 ) pDC->FillSolidRect( r2, cr );
            }
            else
            {
                r2.left  = rcGrip.left + ( ( i		  * rcGrip.Width() ) >> nShift );
                r2.right = rcGrip.left + ( ( ( i + 1) * rcGrip.Width() ) >> nShift );
                if( r2.Width() > 0 ) pDC->FillSolidRect( r2, cr );
            }
        }
    }
*/
	CFont font;
    font.CreatePointFont( 90, "µ¸¿ò" );
	LOGFONT logFont;
	font.GetLogFont( &logFont );
	logFont.lfWeight = FW_BOLD;
	font.DeleteObject();
	font.CreateFontIndirect( &logFont );

    if( bHorz )
    {
        // rotate text 90 degrees CCW if horizontally docked
		LOGFONT lf;
        font.GetLogFont( &lf );
        font.DeleteObject();
        lf.lfEscapement = 900;
        font.CreateFontIndirect(&lf);
    }

    CFont*	 pOldFont	= pDC->SelectObject( &font );
    int		 nOldBkMode	= pDC->SetBkMode(TRANSPARENT);

	CString sTitle;
    GetWindowText(sTitle);

    CPoint ptOrg = bHorz ? CPoint( rcGrip.left + 3, rcGrip.bottom - 5 ) : CPoint( rcGrip.left + 3, rcGrip.top + 4 );

//	pDC->DrawText( sTitle, rcGrip, DT_VCENTER | DT_SINGLELINE | DT_LEFT );
    pDC->ExtTextOut( ptOrg.x, ptOrg.y, ETO_CLIPPED, rcGrip, sTitle, NULL );

    pDC->SelectObject( pOldFont   );
    pDC->SetBkMode	 ( nOldBkMode );
    pDC->SetTextColor( clrOldText );

    // draw the button
    m_biMenu.Paint( pDC, m_bActive );
	m_biHide.Paint( pDC, m_bActive );
}

UINT CSizingControlBarG::OnNcHitTest(CPoint point)
{
	CRect rcBar;
	GetWindowRect(rcBar);

	UINT nRet = baseCSizingControlBarG::OnNcHitTest(point);
	if (nRet != HTCLIENT)
		return nRet;

	CRect rc;
	
	rc = m_biHide.GetRect();
	rc.OffsetRect( rcBar.TopLeft() );
	if ( rc.PtInRect(point) ) return HTCLOSE;

	rc = m_biMenu.GetRect();
	rc.OffsetRect(rcBar.TopLeft());
	if (rc.PtInRect(point)) return HTZOOM;

	return HTCLIENT;
}

/////////////////////////////////////////////////////////////////////////
// CSizingControlBarG implementation helpers

void CSizingControlBarG::OnUpdateCmdUI(CFrameWnd* pTarget,
                                      BOOL bDisableIfNoHndler)
{
    UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);

    if( !HasGripper() ) return;

    CPoint pt; 
	::GetCursorPos(&pt);
    
	UINT nHitTest			= OnNcHitTest( pt );
	BOOL bHide				= nHitTest == HTCLOSE;
	BOOL bMenu				= nHitTest == HTZOOM;
    BOOL bLButtonDown		= (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasHidePushed		= m_biMenu.m_bPushed;
    m_biMenu.m_bPushed		= bHide && bLButtonDown;

    BOOL bWasHideRaised		= m_biMenu.m_bRaised;
    m_biMenu.m_bRaised		= bHide && !bLButtonDown;

    BOOL bWasMenuPushed		= m_biHide.m_bPushed;
    m_biHide.m_bPushed		= bMenu && bLButtonDown;

    BOOL bWasMenuRaised		= m_biHide.m_bRaised;
    m_biHide.m_bRaised		= bMenu && !bLButtonDown;

    CWnd* pFocus			= GetFocus();
    BOOL  bActiveOld		= m_bActive;
    m_bActive				= ( pFocus->GetSafeHwnd() && IsChild( pFocus ) );

    BOOL bNeedPaint			= ( m_biMenu.m_bPushed ^ bWasHidePushed ) || 
							  ( m_biMenu.m_bRaised ^ bWasHidePushed ) || 
							  ( m_biHide.m_bPushed ^ bWasMenuPushed ) || 
							  ( m_biHide.m_bRaised ^ bWasMenuPushed ) || 
							  ( m_bActive != bActiveOld );

    if ( bNeedPaint ) SendMessage( WM_NCPAINT );
}

BOOL CSizingControlBarG::HasGripper() const
{
#if defined(_SCB_MINIFRAME_CAPTION) || !defined(_SCB_REPLACE_MINIFRAME)
    // if the miniframe has a caption, don't display the gripper
    if (IsFloating())
        return FALSE;
#endif //_SCB_MINIFRAME_CAPTION

    return TRUE;
}

LRESULT CSizingControlBarG::OnSetText(WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = baseCSizingControlBarG::OnSetText(wParam, lParam);

    SendMessage( WM_NCPAINT );

    return lResult;
}

/////////////////////////////////////////////////////////////////////////
// CSCBButton

void CSCBButton::Paint( CDC* pDC, BOOL bActive )
{
    CRect rc = GetRect();

	int		 nOffset = 0;
	
//	if( m_bPushed )
//	{
//		pDC->Draw3dRect( rc, ::GetSysColor( COLOR_BTNSHADOW ), ::GetSysColor( COLOR_BTNHIGHLIGHT ) );    
//		nOffset = 1;
//	}
//    else if( m_bRaised )
//	{
//		pDC->Draw3dRect( rc, ::GetSysColor( COLOR_BTNHIGHLIGHT ), ::GetSysColor( COLOR_BTNSHADOW ) );
//	}

//	COLORREF clrText = bActive ? ::GetSysColor( COLOR_CAPTIONTEXT ) : ::GetSysColor( COLOR_3DSHADOW );
	COLORREF clrText = ::GetSysColor( COLOR_CAPTIONTEXT );

    CFont font;
    font.CreatePointFont( 85, _T("±¼¸²Ã¼") );

    CFont*		pOldFont	= pDC->SelectObject	( &font );
    int			nPrevBkMode	= pDC->SetBkMode	( TRANSPARENT );
    COLORREF	clrOldText	= pDC->SetTextColor	( clrText  );
	pDC->TextOut	 ( m_ptOrg.x + nOffset, m_ptOrg.y + nOffset, m_strTitle );

    pDC->SelectObject( pOldFont		);
    pDC->SetBkMode	 ( nPrevBkMode	);
    pDC->SetTextColor( clrOldText	);
}
