// Dlg_H1_.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_H1_.h"

#include "../Common/Inc_ETK/H1_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_H1_ dialog

IMPLEMENT_DYNCREATE(CDlg_H1_, CDialog)

CDlg_H1_::CDlg_H1_(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_H1_::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_H1_)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_H1_::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_H1_)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_H1_, CDialog)
	//{{AFX_MSG_MAP(CDlg_H1_)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_H1_ message handlers

BOOL CDlg_H1_::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_H1_::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

void CDlg_H1_::OnButtonRequest() 
{
	UnadviseData();
	AdviseData();
}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_H1_::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "ȣ���ð�      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��1     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��1     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�1 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�1 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��2     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��2     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�2 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�2 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��3     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��3     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�3 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�3 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��4     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��4     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�4 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�4 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��5     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��5     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�5 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�5 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��6     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��6     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�6 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�6 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��7     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��7     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�7 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�7 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��8     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��8     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�8 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�8 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��9     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��9     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�9 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�9 " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��10    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��10    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ���ܷ�10" );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ���ܷ�10" );
	m_ctrlOutBlock.InsertItem( nRow++, "�Ѹŵ�ȣ���ܷ�" );
	m_ctrlOutBlock.InsertItem( nRow++, "�Ѹż�ȣ���ܷ�" );
	m_ctrlOutBlock.InsertItem( nRow++, "����ȣ������  " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�      " );
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_H1_::AdviseData()
{
	TCHAR	szTrCode[]		= "H1_";

	//-----------------------------------------------------------
	// ������ ���
    CString strCode; GetDlgItemText( IDC_EDIT_CODE, strCode );

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		strCode,					// �����ڵ�
		sizeof( H1__InBlock )		// �����ڵ� ����
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
void CDlg_H1_::UnadviseData()
{
	//-----------------------------------------------------------
	// ������ ����� ������ ���ٸ� Return�Ѵ�.
	if( m_strCode.IsEmpty() )
	{
		return;
	}

	TCHAR	szTrCode[]		= "H1_";
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		m_strCode,					// �����ڵ�
		sizeof( H1__InBlock )		// �����ڵ� ����
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
LRESULT CDlg_H1_::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "H1_" ) == 0 )
	{
		LPH1__OutBlock pOutBlock = (LPH1__OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->hotime     , sizeof( pOutBlock->hotime      ), DATA_TYPE_STRING ) );    // ȣ���ð�       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho1   , sizeof( pOutBlock->offerho1    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��1      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho1     , sizeof( pOutBlock->bidho1      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��1      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem1  , sizeof( pOutBlock->offerrem1   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�1  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem1    , sizeof( pOutBlock->bidrem1     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�1  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho2   , sizeof( pOutBlock->offerho2    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��2      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho2     , sizeof( pOutBlock->bidho2      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��2      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem2  , sizeof( pOutBlock->offerrem2   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�2  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem2    , sizeof( pOutBlock->bidrem2     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�2  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho3   , sizeof( pOutBlock->offerho3    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��3      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho3     , sizeof( pOutBlock->bidho3      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��3      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem3  , sizeof( pOutBlock->offerrem3   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�3  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem3    , sizeof( pOutBlock->bidrem3     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�3  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho4   , sizeof( pOutBlock->offerho4    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��4      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho4     , sizeof( pOutBlock->bidho4      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��4      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem4  , sizeof( pOutBlock->offerrem4   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�4  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem4    , sizeof( pOutBlock->bidrem4     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�4  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho5   , sizeof( pOutBlock->offerho5    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��5      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho5     , sizeof( pOutBlock->bidho5      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��5      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem5  , sizeof( pOutBlock->offerrem5   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�5  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem5    , sizeof( pOutBlock->bidrem5     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�5  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho6   , sizeof( pOutBlock->offerho6    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��6      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho6     , sizeof( pOutBlock->bidho6      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��6      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem6  , sizeof( pOutBlock->offerrem6   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�6  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem6    , sizeof( pOutBlock->bidrem6     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�6  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho7   , sizeof( pOutBlock->offerho7    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��7      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho7     , sizeof( pOutBlock->bidho7      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��7      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem7  , sizeof( pOutBlock->offerrem7   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�7  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem7    , sizeof( pOutBlock->bidrem7     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�7  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho8   , sizeof( pOutBlock->offerho8    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��8      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho8     , sizeof( pOutBlock->bidho8      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��8      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem8  , sizeof( pOutBlock->offerrem8   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�8  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem8    , sizeof( pOutBlock->bidrem8     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�8  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho9   , sizeof( pOutBlock->offerho9    ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��9      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho9     , sizeof( pOutBlock->bidho9      ), DATA_TYPE_LONG   ) );    // �ż�ȣ��9      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem9  , sizeof( pOutBlock->offerrem9   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�9  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem9    , sizeof( pOutBlock->bidrem9     ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�9  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho10  , sizeof( pOutBlock->offerho10   ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ��10     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho10    , sizeof( pOutBlock->bidho10     ), DATA_TYPE_LONG   ) );    // �ż�ȣ��10     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem10 , sizeof( pOutBlock->offerrem10  ), DATA_TYPE_LONG   ) );    // �ŵ�ȣ���ܷ�10 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem10   , sizeof( pOutBlock->bidrem10    ), DATA_TYPE_LONG   ) );    // �ż�ȣ���ܷ�10 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totofferrem, sizeof( pOutBlock->totofferrem ), DATA_TYPE_LONG   ) );    // �Ѹŵ�ȣ���ܷ� 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totbidrem  , sizeof( pOutBlock->totbidrem   ), DATA_TYPE_LONG   ) );    // �Ѹż�ȣ���ܷ� 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->donsigubun , sizeof( pOutBlock->donsigubun  ), DATA_TYPE_STRING ) );    // ����ȣ������   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->shcode     , sizeof( pOutBlock->shcode      ), DATA_TYPE_STRING ) );    // �����ڵ�       
	}

	return 0L;
}

void CDlg_H1_::OnButtonTest() 
{
	// TODO: Add your control notification handler code here
	
}
