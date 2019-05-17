// DlgBase.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "DlgBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBase dialog


CDlgBase::CDlgBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBase, CDialog)
	//{{AFX_MSG_MAP(CDlgBase)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBase message handlers
