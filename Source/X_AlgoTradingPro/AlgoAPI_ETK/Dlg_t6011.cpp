// Dlg_t6011.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t6011.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t6011 dialog

IMPLEMENT_DYNCREATE(CDlg_t6011, CDialog)

CDlg_t6011::CDlg_t6011(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t6011::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t6011)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t6011::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t6011)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_t6011, CDialog)
	//{{AFX_MSG_MAP(CDlg_t6011)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t6011 message handlers
