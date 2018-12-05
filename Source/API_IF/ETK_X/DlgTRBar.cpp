// DlgTRBar.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "DlgTRBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTRBar dialog


CDlgTRBar::CDlgTRBar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTRBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTRBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTRBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTRBar)
	DDX_Control(pDX, IDC_TREE_TR, m_ctrlTR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTRBar, CDialog)
	//{{AFX_MSG_MAP(CDlgTRBar)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_TR, OnDblclkTreeTr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTRBar message handlers

BOOL CDlgTRBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LoadTRList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTRBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( m_ctrlTR.GetSafeHwnd() == NULL )
	{
		return;
	}

	CRect rcClient; GetClientRect( rcClient );
	
	// BUTTON
	CRect rcButton; GetDlgItem( IDC_BUTTON_SEARCH )->GetWindowRect( rcButton );
	ScreenToClient( rcButton );
	GetDlgItem( IDC_BUTTON_SEARCH )->SetWindowPos( NULL, cx - rcButton.Width(), rcButton.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	
	// COMBO
	CRect rcCombo; GetDlgItem( IDC_COMBO_TR )->GetWindowRect( rcCombo );
	ScreenToClient( rcCombo );
	GetDlgItem( IDC_COMBO_TR )->SetWindowPos( NULL, 0, 0, cx - rcButton.Width() - rcCombo.left - 2, rcCombo.Height(), SWP_NOZORDER | SWP_NOMOVE );

	// TREE
	CRect rcTree; m_ctrlTR.GetWindowRect( rcTree );
	ScreenToClient( rcTree );
	rcTree.bottom = cy;
	rcTree.right = cx;
	m_ctrlTR.MoveWindow( rcTree );
}

void CDlgTRBar::LoadTRList()
{
	CStdioFile file;
	if( file.Open( "trlist.txt", CFile::modeRead ) == FALSE )
	{
		//MessageBox( "TR List File 이 없습니다." );
		return;
	}

	CStringArray arrToken;
	CString strRead;
	CString strTR;
	while( file.ReadString( strRead ) )
	{
		GetToken( strRead, arrToken );
		int nCount = arrToken.GetSize() - 2;
		if( nCount < 0 )
		{
			continue;
		}

		HTREEITEM hItem = NULL;
		int i;
		for( i=0; i<nCount; i++ )
		{
			HTREEITEM hFind = FindItem( hItem, arrToken[i] );
			if( hFind == NULL )
			{
				hFind = m_ctrlTR.InsertItem( arrToken[i], hItem ? hItem : TVI_ROOT );
			}
			hItem = hFind;
		}

		strTR.Format( "[%s] %s", arrToken[i], arrToken[i+1] );
		m_ctrlTR.InsertItem( strTR, hItem ? hItem : TVI_ROOT );
	}
}

void CDlgTRBar::GetToken( CString strRead, CStringArray& arrToken )
{
	arrToken.RemoveAll();

	strRead.TrimLeft();

	if( strRead.GetLength() < 2 )
	{
		return;
	}

	if( strRead[0] == '/' && strRead[1] == '/' )
	{
		return;
	}

	int nFind;
	CString strToken;
	while( TRUE )
	{
		nFind = strRead.Find( ',' );
		if( nFind < 0 )
		{
			strRead.TrimRight();
			if( strRead.GetLength() > 0 )
			{
				arrToken.Add( strRead );
			}
			return;
		}

		strToken = strRead.Left( nFind );
		strToken.TrimRight();
		arrToken.Add( strToken );
		strRead = strRead.Mid( nFind + 1 );
		strRead.TrimLeft();
	}
}

HTREEITEM CDlgTRBar::FindItem( HTREEITEM hItem, LPCTSTR pszData )
{
	if( hItem != NULL )
	{
		hItem = m_ctrlTR.GetChildItem( hItem );
	}
	else
	{
		hItem = m_ctrlTR.GetRootItem();
	}

	while( hItem )
	{
		if( m_ctrlTR.GetItemText( hItem ) == pszData )
		{
			break;
		}
		
		hItem = m_ctrlTR.GetNextSiblingItem( hItem );
	}

	return hItem;
}

void CDlgTRBar::OnDblclkTreeTr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = m_ctrlTR.GetSelectedItem();
	if( hItem )
	{
		if( m_ctrlTR.GetChildItem( hItem ) == NULL )
		{
			CString strData = m_ctrlTR.GetItemText( hItem );
			if( strData.GetLength() > 1 && strData[0] == '[' )
			{
				strData = strData.Mid( 1 );
				int nFind = strData.Find( ']' );
				if( nFind > 0 )
				{
					strData = strData.Left( nFind );
					if( strData.GetLength() > 0 )
					{
						AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)strData, NULL );
					}
				}
			}
		}
	}
	
	*pResult = 0;
}
