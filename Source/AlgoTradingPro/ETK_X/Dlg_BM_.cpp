// Dlg_BM_.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_BM_.h"

#include "../../include/API_ETK/BM_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_BM_ dialog

IMPLEMENT_DYNCREATE(CDlg_BM_, CDialog)

CDlg_BM_::CDlg_BM_(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_BM_::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_BM_)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_BM_::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_BM_)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_BM_, CDialog)
	//{{AFX_MSG_MAP(CDlg_BM_)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_BM_ message handlers

BOOL CDlg_BM_::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_BM_::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

void CDlg_BM_::OnButtonRequest() 
{
	UnadviseData();
	AdviseData();
}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_BM_::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�              " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Žð�                " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż� �������  " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż� �������" );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�                " );
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_BM_::AdviseData()
{
	TCHAR	szTrCode[]		= "BM_";

	//-----------------------------------------------------------
	// ������ ���
    CString strCode; GetDlgItemText( IDC_EDIT_CODE, strCode );

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		strCode,					// �����ڵ�
		sizeof( BM__InBlock )		// �����ڵ� ����
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
void CDlg_BM_::UnadviseData()
{
	//-----------------------------------------------------------
	// ������ ����� ������ ���ٸ� Return�Ѵ�.
	if( m_strCode.IsEmpty() )
	{
		return;
	}

	TCHAR	szTrCode[]		= "BM_";
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		m_strCode,					// �����ڵ�
		sizeof( BM__InBlock )		// �����ڵ� ����
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
LRESULT CDlg_BM_::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "BM_" ) == 0 )
	{
		LPBM__OutBlock pOutBlock = (LPBM__OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode , sizeof( pOutBlock->tjjcode  ), DATA_TYPE_STRING ) );    // �������ڵ�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjtime , sizeof( pOutBlock->tjjtime  ), DATA_TYPE_STRING ) );    // ���Žð�                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume, sizeof( pOutBlock->msvolume ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume, sizeof( pOutBlock->mdvolume ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol   , sizeof( pOutBlock->msvol    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->p_msvol , sizeof( pOutBlock->p_msvol  ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż� �������  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue , sizeof( pOutBlock->msvalue  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue , sizeof( pOutBlock->mdvalue  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval   , sizeof( pOutBlock->msval    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->p_msval , sizeof( pOutBlock->p_msval  ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż� �������
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->upcode  , sizeof( pOutBlock->upcode   ), DATA_TYPE_STRING ) );    // �����ڵ�                
	}

	return 0L;
}
