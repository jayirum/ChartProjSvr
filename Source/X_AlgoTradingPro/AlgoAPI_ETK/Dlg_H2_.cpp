// Dlg_H2_.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_H2_.h"

#include "../Common/Inc_ETK/H2_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_H2_ dialog

IMPLEMENT_DYNCREATE(CDlg_H2_, CDialog)

CDlg_H2_::CDlg_H2_(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_H2_::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_H2_)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_H2_::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_H2_)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_H2_, CDialog)
	//{{AFX_MSG_MAP(CDlg_H2_)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_H2_ message handlers

BOOL CDlg_H2_::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_H2_::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

void CDlg_H2_::OnButtonRequest() 
{
	UnadviseData();
	AdviseData();
}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_H2_::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "ȣ���ð�              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ð��ܸŵ��ܷ�        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ð��ܸż��ܷ�        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ð��ܸŵ������������" );
	m_ctrlOutBlock.InsertItem( nRow++, "�ð��ܸż������������" );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�              " );
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_H2_::AdviseData()
{
	TCHAR	szTrCode[]		= "H2_";

	//-----------------------------------------------------------
	// ������ ���
    CString strCode; GetDlgItemText( IDC_EDIT_CODE, strCode );

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		strCode,					// �����ڵ�
		sizeof( H2__InBlock )		// �����ڵ� ����
		);

	//-----------------------------------------------------------
	// ����üũ
	if( bSuccess == FALSE )
	{
		MessageBox( "��ȸ����", "����", MB_ICONSTOP );
	}
	else
	{
		m_strCode = strCode;
	}
}

//--------------------------------------------------------------------------------------
// ������ Unadvise
//--------------------------------------------------------------------------------------
void CDlg_H2_::UnadviseData()
{
	//-----------------------------------------------------------
	// ������ ����� ������ ���ٸ� Return�Ѵ�.
	if( m_strCode.IsEmpty() )
	{
		return;
	}

	TCHAR	szTrCode[]		= "H2_";
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		m_strCode,					// �����ڵ�
		sizeof( H2__InBlock )		// �����ڵ� ����
		);
	
	//-----------------------------------------------------------
	// ����üũ
	if( bSuccess == FALSE )
	{
		MessageBox( "��ȸ����", "����", MB_ICONSTOP );
	}
	else
	{
		m_strCode.Empty();
	}
}

//--------------------------------------------------------------------------------------
// �����͸� ����
//--------------------------------------------------------------------------------------
LRESULT CDlg_H2_::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "H2_" ) == 0 )
	{
		LPH2__OutBlock pOutBlock = (LPH2__OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->hotime       , sizeof( pOutBlock->hotime       ), DATA_TYPE_STRING ) );    // ȣ���ð�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tmofferrem   , sizeof( pOutBlock->tmofferrem   ), DATA_TYPE_LONG   ) );    // �ð��ܸŵ��ܷ�        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tmbidrem     , sizeof( pOutBlock->tmbidrem     ), DATA_TYPE_LONG   ) );    // �ð��ܸż��ܷ�        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pretmoffercha, sizeof( pOutBlock->pretmoffercha), DATA_TYPE_LONG   ) );    // �ð��ܸŵ������������
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pretmbidcha  , sizeof( pOutBlock->pretmbidcha  ), DATA_TYPE_LONG   ) );    // �ð��ܸż������������
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->shcode       , sizeof( pOutBlock->shcode       ), DATA_TYPE_STRING ) );    // �����ڵ�              
	}

	return 0L;
}
