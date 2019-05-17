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

#if !defined(__SCBARG_H__)
#define __SCBARG_H__

#include "sizecbar.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////
// CSCBButton (button info) helper class

class CSCBButton
{
public:
    CSCBButton() : m_bPushed( FALSE ), m_bRaised( FALSE ) {}

    void	Move			( CPoint ptTo )			{ m_ptOrg = ptTo; };
    CRect	GetRect			()						{ return CRect( m_ptOrg, CSize( 11, 11 ) ); };
    void	Paint			( CDC* pDC, BOOL bActive );
	void	SetWindowText	( LPCTSTR lpszTitle )	{ m_strTitle = lpszTitle; }

    BOOL    m_bPushed;
    BOOL    m_bRaised;
	CString	m_strTitle;

protected:
    CPoint  m_ptOrg;
};

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar control bar

#ifndef baseCSizingControlBarG
#define baseCSizingControlBarG CSizingControlBar
#endif

class CSizingControlBarG : public baseCSizingControlBarG
{
    DECLARE_DYNAMIC(CSizingControlBarG);

// Construction
public:
    CSizingControlBarG();

// Attributes
public:
    virtual BOOL HasGripper() const;

// Operations
public:

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSizingControlBarG)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CSizingControlBarG();
    
protected:
    // implementation helpers
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
    virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);
	virtual	void ShowMenu() {}

protected:
    int			m_cyGripper;
	BOOL		m_bActive;

    CSCBButton	m_biHide;
	CSCBButton	m_biMenu;

// Generated message map functions
protected:
    //{{AFX_MSG(CSizingControlBarG)
    afx_msg UINT OnNcHitTest(CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    //}}AFX_MSG
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SCBARG_H__)

