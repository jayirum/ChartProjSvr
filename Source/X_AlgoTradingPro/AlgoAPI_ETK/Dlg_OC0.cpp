// Dlg_OC0.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_OC0.h"

#include "../../include/API_ETK/OC0.h"
#include "../../include/API_ETK/OH0.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_OC0 dialog
IMPLEMENT_DYNCREATE(CDlg_OC0, CDialog)

CDlg_OC0::CDlg_OC0(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_OC0::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_OC0)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_OC0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_OC0)
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
	DDX_Control(pDX, IDC_LIST_OUTBLOCK_H, m_ctrlOutBlock_H);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_OC0, CDialog)
	//{{AFX_MSG_MAP(CDlg_OC0)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST, OnButtonRequest)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_H, OnButtonRequestH)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_OC0 message handlers


BOOL CDlg_OC0::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_OC0::OnDestroy()
{
	UnadviseData('Y');
	UnadviseData('N');

	CDialog::OnDestroy();
}

void CDlg_OC0::OnButtonRequest() 
{
	UnadviseData('Y');
	AdviseData('Y');
}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_OC0::InitCtrls()
{
	//-------------------------------------------------------------------------
	// �ɼ� ü�� ��
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "ü��ð�          " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ϴ�񱸺�      " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ϴ��          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���簡            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ð�              " );
	m_ctrlOutBlock.InsertItem( nRow++, "��              " );
	m_ctrlOutBlock.InsertItem( nRow++, "����              " );
	m_ctrlOutBlock.InsertItem( nRow++, "ü�ᱸ��          " );
	m_ctrlOutBlock.InsertItem( nRow++, "ü�ᷮ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ŷ���        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ŷ����      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�����ü�ᷮ    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�����ü��Ǽ�  " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�����ü�ᷮ    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�����ü��Ǽ�  " );
	m_ctrlOutBlock.InsertItem( nRow++, "ü�ᰭ��          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��1         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��1         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�̰�����������    " );
	m_ctrlOutBlock.InsertItem( nRow++, "KOSPI200����      " );
	m_ctrlOutBlock.InsertItem( nRow++, "KOSPI200�      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�̷а�            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���纯����        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ð���ġ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�̰�����������    " );
	m_ctrlOutBlock.InsertItem( nRow++, "������        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ϵ��ð���ŷ���" );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�          " );

	//-------------------------------------------------------------------------
	// �ɼ� ȣ�� ��
	m_ctrlOutBlock_H.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock_H.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );
	
	nRow = 0;
	m_ctrlOutBlock_H.InsertItem( nRow++, "ȣ���ð�          " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��1         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��2         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��3         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��4         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��5         " );	
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��1         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��2         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��3         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��4         " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��5         " );

	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�1     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�2     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�3     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�4     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�5     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���ѰǼ�    " );	

	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�1     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�2     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�3     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�4     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�5     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���ѰǼ�    " );

	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������1     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������2     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������3     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������4     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������5     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ѽ���    " );	

	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������1     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������2     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������3     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������4     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������5     " );
	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ѽ���    " );	

	m_ctrlOutBlock_H.InsertItem( nRow++, "�����ڵ�          " );
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_OC0::AdviseData(char i_cSiseYN)
{
	TCHAR	szTrCode[32];
	CString strCode;
	int		nSize;

	if( i_cSiseYN=='Y' )
	{
		strcpy( szTrCode, "OC0");
		GetDlgItemText( IDC_EDIT_CODE, strCode );
		nSize = sizeof( OC0_InBlock );
	}
	else
	{
		strcpy( szTrCode, "OH0");
		GetDlgItemText( IDC_EDIT_CODE_H, strCode );
		nSize = sizeof( OH0_InBlock );
	}

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		strCode,					// �����ڵ�
		nSize						// �����ڵ� ����
		);

	//-----------------------------------------------------------
	// ����üũ
	if( bSuccess == FALSE )
	{
		MessageBox( "��ȸ����", "����", MB_ICONSTOP );
	}
	else
	{
		if( i_cSiseYN=='Y' )
			m_strCode = strCode;
		else
			m_strCode_H = strCode;
	}
}

//--------------------------------------------------------------------------------------
// ������ Unadvise
//--------------------------------------------------------------------------------------
void CDlg_OC0::UnadviseData(char i_cSiseYN)
{
	TCHAR	szTrCode[32];
	char	szStkCode[32];
	int		nSize;

	//-----------------------------------------------------------
	// ������ ����� ������ ���ٸ� Return�Ѵ�.

	if( i_cSiseYN=='Y')
	{
		if( m_strCode.IsEmpty() )
			return;
		strcpy( szTrCode, "OC0" );
		strcpy( szStkCode, (LPCSTR)m_strCode );
		nSize = sizeof( OC0_InBlock );
	}
	else
	{
		if( m_strCode_H.IsEmpty() )
			return;
		strcpy( szTrCode, "OH0" );
		strcpy( szStkCode, (LPCSTR)m_strCode_H );
		nSize = sizeof( OH0_InBlock );
	}
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		szStkCode,					// �����ڵ�
		nSize						// �����ڵ� ����
		);
	
	//-----------------------------------------------------------
	// ����üũ
	if( bSuccess == FALSE )
	{
		MessageBox( "��ȸ����", "����", MB_ICONSTOP );
	}
	else
	{
		if( i_cSiseYN=='Y' )
			m_strCode.Empty();
		else
			m_strCode_H.Empty();
	}
}

//--------------------------------------------------------------------------------------
// �����͸� ����
//--------------------------------------------------------------------------------------
LRESULT CDlg_OC0::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	//////////////////////////////////////////////////////////////////////////
	//	����ü��(�ü�)
	if( strcmp( pRealPacket->szTrCode, "OC0" ) == 0 )
	{
		LPOC0_OutBlock pOutBlock = (LPOC0_OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->chetime    , sizeof( pOutBlock->chetime     ), DATA_TYPE_STRING    ) );    // ü��ð�          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->sign       , sizeof( pOutBlock->sign        ), DATA_TYPE_STRING    ) );    // ���ϴ�񱸺�      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->change     , sizeof( pOutBlock->change      ), DATA_TYPE_FLOAT , 2 ) );    // ���ϴ��          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->drate      , sizeof( pOutBlock->drate       ), DATA_TYPE_FLOAT , 2 ) );    // �����            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->price      , sizeof( pOutBlock->price       ), DATA_TYPE_FLOAT , 2 ) );    // ���簡            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->open       , sizeof( pOutBlock->open        ), DATA_TYPE_FLOAT , 2 ) );    // �ð�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->high       , sizeof( pOutBlock->high        ), DATA_TYPE_FLOAT , 2 ) );    // ��              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->low        , sizeof( pOutBlock->low         ), DATA_TYPE_FLOAT , 2 ) );    // ����              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cgubun     , sizeof( pOutBlock->cgubun      ), DATA_TYPE_STRING    ) );    // ü�ᱸ��          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cvolume    , sizeof( pOutBlock->cvolume     ), DATA_TYPE_LONG      ) );    // ü�ᷮ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->volume     , sizeof( pOutBlock->volume      ), DATA_TYPE_LONG      ) );    // �����ŷ���        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->value      , sizeof( pOutBlock->value       ), DATA_TYPE_LONG      ) );    // �����ŷ����      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume   , sizeof( pOutBlock->mdvolume    ), DATA_TYPE_LONG      ) );    // �ŵ�����ü�ᷮ    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdchecnt   , sizeof( pOutBlock->mdchecnt    ), DATA_TYPE_LONG      ) );    // �ŵ�����ü��Ǽ�  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume   , sizeof( pOutBlock->msvolume    ), DATA_TYPE_LONG      ) );    // �ż�����ü�ᷮ    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mschecnt   , sizeof( pOutBlock->mschecnt    ), DATA_TYPE_LONG      ) );    // �ż�����ü��Ǽ�  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cpower     , sizeof( pOutBlock->cpower      ), DATA_TYPE_FLOAT , 2 ) );    // ü�ᰭ��          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho1   , sizeof( pOutBlock->offerho1    ), DATA_TYPE_FLOAT , 2 ) );    // �ŵ�ȣ��1         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho1     , sizeof( pOutBlock->bidho1      ), DATA_TYPE_FLOAT , 2 ) );    // �ż�ȣ��1         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->openyak    , sizeof( pOutBlock->openyak     ), DATA_TYPE_LONG      ) );    // �̰�����������    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->k200jisu   , sizeof( pOutBlock->k200jisu    ), DATA_TYPE_FLOAT , 2 ) );    // KOSPI200����      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->eqva	   , sizeof( pOutBlock->eqva	    ), DATA_TYPE_FLOAT , 2 ) );    // KOSPI200����      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->theoryprice	, sizeof( pOutBlock->theoryprice), DATA_TYPE_FLOAT , 2 ) );    // �̷а�            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->impv		, sizeof( pOutBlock->impv       ), DATA_TYPE_FLOAT , 2 ) );    // ������            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->openyakcha	, sizeof( pOutBlock->openyakcha ), DATA_TYPE_LONG      ) );    // �̰�����������    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->timevalue   , sizeof( pOutBlock->timevalue  ), DATA_TYPE_FLOAT    ) );    // ������        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->jgubun		, sizeof( pOutBlock->jgubun     ), DATA_TYPE_STRING    ) );    // ������        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->jnilvolume , sizeof( pOutBlock->jnilvolume  ), DATA_TYPE_LONG      ) );    // ���ϵ��ð���ŷ���
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->optcode    , sizeof( pOutBlock->optcode     ), DATA_TYPE_STRING    ) );    // �����ڵ�          
	}

	///////////////////////////////////////////////////////////////////////////
	//	����ȣ��
	if( strcmp( pRealPacket->szTrCode, "OH0" ) == 0 )
	{
		LPOH0_OutBlock pOutBlock = (LPOH0_OutBlock)pRealPacket->pszData;
		int nRow = 0;

		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->hotime    , sizeof( pOutBlock->hotime     ), DATA_TYPE_STRING    ) );

		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho1   , sizeof( pOutBlock->offerho1  ), DATA_TYPE_FLOAT    ) ); 
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho2   , sizeof( pOutBlock->offerho2  ), DATA_TYPE_FLOAT    ) ); 
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho3   , sizeof( pOutBlock->offerho3  ), DATA_TYPE_FLOAT    ) ); 
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho4   , sizeof( pOutBlock->offerho4  ), DATA_TYPE_FLOAT    ) ); 
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho5   , sizeof( pOutBlock->offerho5  ), DATA_TYPE_FLOAT    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho1   , sizeof( pOutBlock->bidho1  ), DATA_TYPE_FLOAT    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho2   , sizeof( pOutBlock->bidho2  ), DATA_TYPE_FLOAT    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho3   , sizeof( pOutBlock->bidho3  ), DATA_TYPE_FLOAT    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho4   , sizeof( pOutBlock->bidho4  ), DATA_TYPE_FLOAT    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho5   , sizeof( pOutBlock->bidho5  ), DATA_TYPE_FLOAT    ) );

		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offercnt1   , sizeof( pOutBlock->offercnt1  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offercnt2   , sizeof( pOutBlock->offercnt2  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offercnt3   , sizeof( pOutBlock->offercnt3  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offercnt4   , sizeof( pOutBlock->offercnt4  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offercnt5   , sizeof( pOutBlock->offercnt5  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totoffercnt   , sizeof( pOutBlock->totoffercnt  ), DATA_TYPE_LONG    ) );
		
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidcnt1   , sizeof( pOutBlock->bidcnt1  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidcnt2   , sizeof( pOutBlock->bidcnt2  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidcnt3   , sizeof( pOutBlock->bidcnt3  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidcnt4   , sizeof( pOutBlock->bidcnt4  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidcnt5   , sizeof( pOutBlock->bidcnt5  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totbidcnt , sizeof( pOutBlock->totbidcnt  ), DATA_TYPE_LONG    ) );

		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem1   , sizeof( pOutBlock->offerrem1  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem2   , sizeof( pOutBlock->offerrem2  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem3   , sizeof( pOutBlock->offerrem3  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem4   , sizeof( pOutBlock->offerrem4  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem5   , sizeof( pOutBlock->offerrem5  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totofferrem , sizeof( pOutBlock->totofferrem  ), DATA_TYPE_LONG    ) );
		
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem1   , sizeof( pOutBlock->bidrem1  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem2   , sizeof( pOutBlock->bidrem2  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem3   , sizeof( pOutBlock->bidrem3  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem4   , sizeof( pOutBlock->bidrem4  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem5   , sizeof( pOutBlock->bidrem5  ), DATA_TYPE_LONG    ) );
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totbidrem , sizeof( pOutBlock->totbidrem  ), DATA_TYPE_LONG  ) );
	
		m_ctrlOutBlock_H.SetItemText(  nRow++, 1, GetDispData( pOutBlock->optcode    , sizeof( pOutBlock->optcode     ), DATA_TYPE_STRING    ) );
	}

	return 0L;
}


void CDlg_OC0::OnButtonRequestH() 
{
	UnadviseData('N');
	AdviseData('N');	
}
