// Dlg_BMT.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_BMT.h"

#include "../../include/API_ETK/BMT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_BMT dialog

IMPLEMENT_DYNCREATE(CDlg_BMT, CDialog)

CDlg_BMT::CDlg_BMT(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_BMT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_BMT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_BMT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_BMT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_BMT, CDialog)
	//{{AFX_MSG_MAP(CDlg_BMT)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_BMT message handlers

BOOL CDlg_BMT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_BMT::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

void CDlg_BMT::OnButtonRequest() 
{
	UnadviseData();
	AdviseData();
}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_BMT::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "���Žð�              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�1(����)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���1          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���1          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�1        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����1        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����1        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�1      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�2(�ܱ���)   " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���2          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���2          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�2        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����2        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����2        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�2      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�3(�����)   " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���3          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���3          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�3        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����3        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����3        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�3      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�4(����)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���4          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���4          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�4        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����4        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����4        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�4      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�5(����)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���5          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���5          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�5        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����5        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����5        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�5      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�6(����)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���6          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���6          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�6        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����6        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����6        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�6      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�7(����)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���7          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���7          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�7        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����7        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����7        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�7      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�8(����)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���8          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���8          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�8        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����8        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����8        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�8      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�9(���)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���9          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���9          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�9        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����9        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����9        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�9      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�10(��������)" );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���10         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���10         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�10       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����10       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����10       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�10     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�11(��Ÿ)    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���11         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���11         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�11       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����11       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����11       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�11     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ڵ�0(����ݵ�) " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ���0          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ���0          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��� ���ż�0        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż� �ŷ����0        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ� �ŷ����0        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���� ���ż�0      " );
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_BMT::AdviseData()
{
	TCHAR	szTrCode[]		= "BMT";

	//-----------------------------------------------------------
	// ������ ���
    CString strCode; GetDlgItemText( IDC_EDIT_CODE, strCode );

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		strCode,					// �����ڵ�
		sizeof( BMT_InBlock )		// �����ڵ� ����
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
void CDlg_BMT::UnadviseData()
{
	//-----------------------------------------------------------
	// ������ ����� ������ ���ٸ� Return�Ѵ�.
	if( m_strCode.IsEmpty() )
	{
		return;
	}

	TCHAR	szTrCode[]		= "BMT";
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		m_strCode,					// �����ڵ�
		sizeof( BMT_InBlock )		// �����ڵ� ����
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
LRESULT CDlg_BMT::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "BMT" ) == 0 )
	{
		LPBMT_OutBlock pOutBlock = (LPBMT_OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjtime  , sizeof( pOutBlock->tjjtime   ), DATA_TYPE_STRING ) );    // ���Žð�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode1 , sizeof( pOutBlock->tjjcode1  ), DATA_TYPE_STRING ) );    // �������ڵ�1(����)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume1, sizeof( pOutBlock->msvolume1 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���1          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume1, sizeof( pOutBlock->mdvolume1 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���1          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol1   , sizeof( pOutBlock->msvol1    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�1        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue1 , sizeof( pOutBlock->msvalue1  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����1        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue1 , sizeof( pOutBlock->mdvalue1  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����1        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval1   , sizeof( pOutBlock->msval1    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�1      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode2 , sizeof( pOutBlock->tjjcode2  ), DATA_TYPE_STRING ) );    // �������ڵ�2(�ܱ���)   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume2, sizeof( pOutBlock->msvolume2 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���2          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume2, sizeof( pOutBlock->mdvolume2 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���2          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol2   , sizeof( pOutBlock->msvol2    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�2        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue2 , sizeof( pOutBlock->msvalue2  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����2        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue2 , sizeof( pOutBlock->mdvalue2  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����2        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval2   , sizeof( pOutBlock->msval2    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�2      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode3 , sizeof( pOutBlock->tjjcode3  ), DATA_TYPE_STRING ) );    // �������ڵ�3(�����)   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume3, sizeof( pOutBlock->msvolume3 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���3          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume3, sizeof( pOutBlock->mdvolume3 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���3          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol3   , sizeof( pOutBlock->msvol3    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�3        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue3 , sizeof( pOutBlock->msvalue3  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����3        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue3 , sizeof( pOutBlock->mdvalue3  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����3        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval3   , sizeof( pOutBlock->msval3    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�3      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode4 , sizeof( pOutBlock->tjjcode4  ), DATA_TYPE_STRING ) );    // �������ڵ�4(����)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume4, sizeof( pOutBlock->msvolume4 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���4          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume4, sizeof( pOutBlock->mdvolume4 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���4          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol4   , sizeof( pOutBlock->msvol4    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�4        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue4 , sizeof( pOutBlock->msvalue4  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����4        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue4 , sizeof( pOutBlock->mdvalue4  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����4        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval4   , sizeof( pOutBlock->msval4    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�4      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode5 , sizeof( pOutBlock->tjjcode5  ), DATA_TYPE_STRING ) );    // �������ڵ�5(����)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume5, sizeof( pOutBlock->msvolume5 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���5          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume5, sizeof( pOutBlock->mdvolume5 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���5          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol5   , sizeof( pOutBlock->msvol5    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�5        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue5 , sizeof( pOutBlock->msvalue5  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����5        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue5 , sizeof( pOutBlock->mdvalue5  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����5        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval5   , sizeof( pOutBlock->msval5    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�5      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode6 , sizeof( pOutBlock->tjjcode6  ), DATA_TYPE_STRING ) );    // �������ڵ�6(����)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume6, sizeof( pOutBlock->msvolume6 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���6          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume6, sizeof( pOutBlock->mdvolume6 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���6          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol6   , sizeof( pOutBlock->msvol6    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�6        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue6 , sizeof( pOutBlock->msvalue6  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����6        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue6 , sizeof( pOutBlock->mdvalue6  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����6        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval6   , sizeof( pOutBlock->msval6    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�6      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode7 , sizeof( pOutBlock->tjjcode7  ), DATA_TYPE_STRING ) );    // �������ڵ�7(����)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume7, sizeof( pOutBlock->msvolume7 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���7          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume7, sizeof( pOutBlock->mdvolume7 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���7          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol7   , sizeof( pOutBlock->msvol7    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�7        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue7 , sizeof( pOutBlock->msvalue7  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����7        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue7 , sizeof( pOutBlock->mdvalue7  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����7        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval7   , sizeof( pOutBlock->msval7    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�7      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode8 , sizeof( pOutBlock->tjjcode8  ), DATA_TYPE_STRING ) );    // �������ڵ�8(����)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume8, sizeof( pOutBlock->msvolume8 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���8          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume8, sizeof( pOutBlock->mdvolume8 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���8          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol8   , sizeof( pOutBlock->msvol8    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�8        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue8 , sizeof( pOutBlock->msvalue8  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����8        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue8 , sizeof( pOutBlock->mdvalue8  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����8        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval8   , sizeof( pOutBlock->msval8    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�8      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode9 , sizeof( pOutBlock->tjjcode9  ), DATA_TYPE_STRING ) );    // �������ڵ�9(���)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume9, sizeof( pOutBlock->msvolume9 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���9          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume9, sizeof( pOutBlock->mdvolume9 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���9          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol9   , sizeof( pOutBlock->msvol9    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�9        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue9 , sizeof( pOutBlock->msvalue9  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����9        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue9 , sizeof( pOutBlock->mdvalue9  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����9        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval9   , sizeof( pOutBlock->msval9    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�9      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode10, sizeof( pOutBlock->tjjcode10 ), DATA_TYPE_STRING ) );    // �������ڵ�10(��������)
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume1, sizeof( pOutBlock->msvolume1 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���10         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume1, sizeof( pOutBlock->mdvolume1 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���10         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol10  , sizeof( pOutBlock->msvol10   ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�10       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue10, sizeof( pOutBlock->msvalue10 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����10       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue10, sizeof( pOutBlock->mdvalue10 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����10       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval10  , sizeof( pOutBlock->msval10   ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�10     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode11, sizeof( pOutBlock->tjjcode11 ), DATA_TYPE_STRING ) );    // �������ڵ�11(��Ÿ)    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume1, sizeof( pOutBlock->msvolume1 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���11         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume1, sizeof( pOutBlock->mdvolume1 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���11         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol11  , sizeof( pOutBlock->msvol11   ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�11       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue11, sizeof( pOutBlock->msvalue11 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����11       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue11, sizeof( pOutBlock->mdvalue11 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����11       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval11  , sizeof( pOutBlock->msval11   ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�11     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->upcode   , sizeof( pOutBlock->upcode    ), DATA_TYPE_STRING ) );    // �����ڵ�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode0 , sizeof( pOutBlock->tjjcode0  ), DATA_TYPE_STRING ) );    // �������ڵ�0(����ݵ�) 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume0, sizeof( pOutBlock->msvolume0 ), DATA_TYPE_LONG   ) );    // �ż� �ŷ���0          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume0, sizeof( pOutBlock->mdvolume0 ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ���0          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol0   , sizeof( pOutBlock->msvol0    ), DATA_TYPE_LONG   ) );    // �ŷ��� ���ż�0        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue0 , sizeof( pOutBlock->msvalue0  ), DATA_TYPE_LONG   ) );    // �ż� �ŷ����0        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue0 , sizeof( pOutBlock->mdvalue0  ), DATA_TYPE_LONG   ) );    // �ŵ� �ŷ����0        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval0   , sizeof( pOutBlock->msval0    ), DATA_TYPE_LONG   ) );    // �ŷ���� ���ż�0      
	}

	return 0L;
}
