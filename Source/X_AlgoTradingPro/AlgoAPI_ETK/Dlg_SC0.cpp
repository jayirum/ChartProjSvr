// Dlg_SC0.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_SC0.h"

#include "../../include/API_ETK/SC0.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC0 dialog

IMPLEMENT_DYNCREATE(CDlg_SC0, CDialog)

CDlg_SC0::CDlg_SC0(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SC0::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_SC0)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_SC0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_SC0)
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_SC0, CDialog)
	//{{AFX_MSG_MAP(CDlg_SC0)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC0 message handlers

BOOL CDlg_SC0::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �ֹ��� ���õ� �ǽð�TR�� �Է°��� �����Ƿ� ȭ���� ���µɶ� ����Ѵ�.
	AdviseData();

	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_SC0::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_SC0::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "�����Ϸù�ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���¹�ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ID            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���౸��            " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ȣ����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����������        " );
	m_ctrlOutBlock.InsertItem( nRow++, "TRCODE              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�̿���ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ID            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Ӹ�ü            " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F�Ϸù�ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����Ϸù�ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "TR����ID            " );
	m_ctrlOutBlock.InsertItem( nRow++, "����IP              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�缳IP              " );
	m_ctrlOutBlock.InsertItem( nRow++, "ó��������ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܸ���ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "APó���ð�          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�޼����ڵ�          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�޼�����±���      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����û����        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Ű              " );
	m_ctrlOutBlock.InsertItem( nRow++, "��û���ڵ尳��      " );
	m_ctrlOutBlock.InsertItem( nRow++, "���񿵿�            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ӱ���            " );
	m_ctrlOutBlock.InsertItem( nRow++, "����Ű��            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ý��۱���      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ش�����        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����޽�������      " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ȸ�߿���          " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F�̺�ƮID         " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���񿵿�            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�ü�ᱸ��        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���屸��            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "���¹�ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���¹�ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "��й�ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���������ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "ȣ�������ڵ�        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���α׷�ȣ������    " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ŵ�ȣ������      " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ŵ����ɿ���      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ſ뱸��            " );
	m_ctrlOutBlock.InsertItem( nRow++, "������              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ݴ�Ÿ��ֹ�����    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�׷�ID              " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�ȸ��            " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ʈ��������ȣ      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ٽ��Ϲ�ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "Ʈ��ġ��ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ƾ��۹�ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Ա���            " );
	m_ctrlOutBlock.InsertItem( nRow++, "ȸ�����ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "ó������            " );
	m_ctrlOutBlock.InsertItem( nRow++, "����������ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������¹�ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ǰ����        " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ÿ�ü����        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����������ڱ���    " );
	m_ctrlOutBlock.InsertItem( nRow++, "DUMMY               " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ���ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ���ȣ            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ��ð�            " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "���������ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ü�����    " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���������      " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���Ҽ���      " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ȸ����۽Ź�ȣ    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ��ݾ�            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�Ÿű���            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ���ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ���ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ݴ�Ÿ��Ϸù�ȣ    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ���ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ���ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ��Ϸù�ȣ    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ�����        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ������ȣ        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ǹ��ֹ�����        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ�����      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ��ݾ�        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ����ݾ�        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ��ݾ�      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ֹ��ݾ�      " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ſ�㺸�����" );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܰ����            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ǹ����ɼ���        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���밡�ɼ���(�ŵ�)" );
	m_ctrlOutBlock.InsertItem( nRow++, "��������            " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܰ����(D2)        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ��ֹ����ɼ���    " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ü��ŵ��ֹ�����  " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ո��԰�          " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Աݾ�            " );
	m_ctrlOutBlock.InsertItem( nRow++, "������              " );
	m_ctrlOutBlock.InsertItem( nRow++, "����              " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ź���ű�����      " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ź���űݴ��      " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ſ�㺸�����    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ���������        " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ����ɴ��        " );
	m_ctrlOutBlock.InsertItem( nRow++, "���밡�ɱݾ�      " );
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_SC0::AdviseData()
{
	TCHAR	szTrCode[]		= "SC0";

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		"",							// SC0�� �� �����͸� �Ѱܾ� �Ѵ�.
		0							// SC0�� ������ ���̴� 0�̴�.
		);

	//-----------------------------------------------------------
	// ����üũ
	if( bSuccess == FALSE )
	{
		MessageBox( "��ȸ����", "����", MB_ICONSTOP );
	}
}

//--------------------------------------------------------------------------------------
// ������ Unadvise
//--------------------------------------------------------------------------------------
void CDlg_SC0::UnadviseData()
{
	TCHAR	szTrCode[]		= "SC0";
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		"",							// SC0�� �� �����͸� �Ѱܾ� �Ѵ�.
		0							// SC0�� ������ ���̴� 0�̴�.
		);
	
	//-----------------------------------------------------------
	// ����üũ
	if( bSuccess == FALSE )
	{
		MessageBox( "��ȸ����", "����", MB_ICONSTOP );
	}
}

//--------------------------------------------------------------------------------------
// �����͸� ����
//--------------------------------------------------------------------------------------
LRESULT CDlg_SC0::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "SC0" ) == 0 )
	{
		LPSC0_OutBlock pOutBlock = (LPSC0_OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lineseq         , sizeof( pOutBlock->lineseq         ), DATA_TYPE_LONG      ) );    // �����Ϸù�ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno           , sizeof( pOutBlock->accno           ), DATA_TYPE_STRING    ) );    // ���¹�ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->user            , sizeof( pOutBlock->user            ), DATA_TYPE_STRING    ) );    // ������ID                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->len             , sizeof( pOutBlock->len             ), DATA_TYPE_LONG      ) );    // �������                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->gubun           , sizeof( pOutBlock->gubun           ), DATA_TYPE_STRING    ) );    // �������                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compress        , sizeof( pOutBlock->compress        ), DATA_TYPE_STRING    ) );    // ���౸��                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->encrypt         , sizeof( pOutBlock->encrypt         ), DATA_TYPE_STRING    ) );    // ��ȣ����                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offset          , sizeof( pOutBlock->offset          ), DATA_TYPE_LONG      ) );    // �����������            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trcode          , sizeof( pOutBlock->trcode          ), DATA_TYPE_STRING    ) );    // TRCODE                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compid          , sizeof( pOutBlock->compid          ), DATA_TYPE_STRING    ) );    // �̿���ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->userid          , sizeof( pOutBlock->userid          ), DATA_TYPE_STRING    ) );    // �����ID                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->media           , sizeof( pOutBlock->media           ), DATA_TYPE_STRING    ) );    // ���Ӹ�ü                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ifid            , sizeof( pOutBlock->ifid            ), DATA_TYPE_STRING    ) );    // I/F�Ϸù�ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->seq             , sizeof( pOutBlock->seq             ), DATA_TYPE_STRING    ) );    // �����Ϸù�ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trid            , sizeof( pOutBlock->trid            ), DATA_TYPE_STRING    ) );    // TR����ID                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pubip           , sizeof( pOutBlock->pubip           ), DATA_TYPE_STRING    ) );    // ����IP                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prvip           , sizeof( pOutBlock->prvip           ), DATA_TYPE_STRING    ) );    // �缳IP                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno          ), DATA_TYPE_STRING    ) );    // ó��������ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bpno            , sizeof( pOutBlock->bpno            ), DATA_TYPE_STRING    ) );    // ������ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->termno          , sizeof( pOutBlock->termno          ), DATA_TYPE_STRING    ) );    // �ܸ���ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lang            , sizeof( pOutBlock->lang            ), DATA_TYPE_STRING    ) );    // ����                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->proctm          , sizeof( pOutBlock->proctm          ), DATA_TYPE_LONG      ) );    // APó���ð�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msgcode         , sizeof( pOutBlock->msgcode         ), DATA_TYPE_STRING    ) );    // �޼����ڵ�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->outgu           , sizeof( pOutBlock->outgu           ), DATA_TYPE_STRING    ) );    // �޼�����±���          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compreq         , sizeof( pOutBlock->compreq         ), DATA_TYPE_STRING    ) );    // �����û����            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->funckey         , sizeof( pOutBlock->funckey         ), DATA_TYPE_STRING    ) );    // ���Ű                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt          ), DATA_TYPE_LONG      ) );    // ��û���ڵ尳��          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->filler          , sizeof( pOutBlock->filler          ), DATA_TYPE_STRING    ) );    // ���񿵿�                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cont            , sizeof( pOutBlock->cont            ), DATA_TYPE_STRING    ) );    // ���ӱ���                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->contkey         , sizeof( pOutBlock->contkey         ), DATA_TYPE_STRING    ) );    // ����Ű��                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varlen          , sizeof( pOutBlock->varlen          ), DATA_TYPE_LONG      ) );    // �����ý��۱���          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen        ), DATA_TYPE_LONG      ) );    // �����ش�����            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen       ), DATA_TYPE_LONG      ) );    // �����޽�������          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trsrc           , sizeof( pOutBlock->trsrc           ), DATA_TYPE_STRING    ) );    // ��ȸ�߿���              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->eventid         , sizeof( pOutBlock->eventid         ), DATA_TYPE_STRING    ) );    // I/F�̺�ƮID             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo          ), DATA_TYPE_STRING    ) );    // I/F����                 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->filler1         , sizeof( pOutBlock->filler1         ), DATA_TYPE_STRING    ) );    // ���񿵿�                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordchegb        , sizeof( pOutBlock->ordchegb        ), DATA_TYPE_STRING    ) );    // �ֹ�ü�ᱸ��            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->marketgb        , sizeof( pOutBlock->marketgb        ), DATA_TYPE_STRING    ) );    // ���屸��                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordgb           , sizeof( pOutBlock->ordgb           ), DATA_TYPE_STRING    ) );    // �ֹ�����                
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordno1       , sizeof( pOutBlock->orgordno1       ), DATA_TYPE_LONG      ) );    // ���ֹ���ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordno        , sizeof( pOutBlock->orgordno        ), DATA_TYPE_LONG      ) );    // ���ֹ���ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno1          , sizeof( pOutBlock->accno1          ), DATA_TYPE_STRING    ) );    // ���¹�ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno2          , sizeof( pOutBlock->accno2          ), DATA_TYPE_STRING    ) );    // ���¹�ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->passwd          , sizeof( pOutBlock->passwd          ), DATA_TYPE_STRING    ) );    // ��й�ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->expcode         , sizeof( pOutBlock->expcode         ), DATA_TYPE_STRING    ) );    // �����ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->shtcode         , sizeof( pOutBlock->shtcode         ), DATA_TYPE_STRING    ) );    // ���������ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->hname           , sizeof( pOutBlock->hname           ), DATA_TYPE_STRING    ) );    // �����                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordqty          , sizeof( pOutBlock->ordqty          ), DATA_TYPE_LONG      ) );    // �ֹ�����                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordprice        , sizeof( pOutBlock->ordprice        ), DATA_TYPE_LONG      ) );    // �ֹ�����                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->hogagb          , sizeof( pOutBlock->hogagb          ), DATA_TYPE_STRING    ) );    // �ֹ�����                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->etfhogagb       , sizeof( pOutBlock->etfhogagb       ), DATA_TYPE_STRING    ) );    // ȣ�������ڵ�            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pgmtype         , sizeof( pOutBlock->pgmtype         ), DATA_TYPE_LONG      ) );    // ���α׷�ȣ������        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->gmhogagb        , sizeof( pOutBlock->gmhogagb        ), DATA_TYPE_LONG      ) );    // ���ŵ�ȣ������          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->gmhogayn        , sizeof( pOutBlock->gmhogayn        ), DATA_TYPE_LONG      ) );    // ���ŵ����ɿ���          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->singb           , sizeof( pOutBlock->singb           ), DATA_TYPE_STRING    ) );    // �ſ뱸��                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->loandt          , sizeof( pOutBlock->loandt          ), DATA_TYPE_STRING    ) );    // ������                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp       ), DATA_TYPE_STRING    ) );    // �ݴ�Ÿ��ֹ�����        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode       ), DATA_TYPE_STRING    ) );    // �����ڵ�                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->groupid         , sizeof( pOutBlock->groupid         ), DATA_TYPE_STRING    ) );    // �׷�ID                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno        ), DATA_TYPE_LONG      ) );    // �ֹ�ȸ��                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prtno           , sizeof( pOutBlock->prtno           ), DATA_TYPE_LONG      ) );    // ��Ʈ��������ȣ          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->basketno        , sizeof( pOutBlock->basketno        ), DATA_TYPE_LONG      ) );    // �ٽ��Ϲ�ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trchno          , sizeof( pOutBlock->trchno          ), DATA_TYPE_LONG      ) );    // Ʈ��ġ��ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->itemno          , sizeof( pOutBlock->itemno          ), DATA_TYPE_LONG      ) );    // �ƾ��۹�ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->brwmgmyn        , sizeof( pOutBlock->brwmgmyn        ), DATA_TYPE_LONG      ) );    // ���Ա���                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mbrno           , sizeof( pOutBlock->mbrno           ), DATA_TYPE_LONG      ) );    // ȸ�����ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->procgb          , sizeof( pOutBlock->procgb          ), DATA_TYPE_STRING    ) );    // ó������                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->admbrchno       , sizeof( pOutBlock->admbrchno       ), DATA_TYPE_STRING    ) );    // ����������ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futaccno        , sizeof( pOutBlock->futaccno        ), DATA_TYPE_STRING    ) );    // �������¹�ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futmarketgb     , sizeof( pOutBlock->futmarketgb     ), DATA_TYPE_STRING    ) );    // ������ǰ����            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tongsingb       , sizeof( pOutBlock->tongsingb       ), DATA_TYPE_STRING    ) );    // ��Ÿ�ü����            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lpgb            , sizeof( pOutBlock->lpgb            ), DATA_TYPE_STRING    ) );    // �����������ڱ���        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->dummy           , sizeof( pOutBlock->dummy           ), DATA_TYPE_STRING    ) );    // DUMMY                   
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordno1          , sizeof( pOutBlock->ordno1          ), DATA_TYPE_LONG      ) );    // �ֹ���ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordno           , sizeof( pOutBlock->ordno           ), DATA_TYPE_LONG      ) );    // �ֹ���ȣ                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordtm           , sizeof( pOutBlock->ordtm           ), DATA_TYPE_STRING    ) );    // �ֹ��ð�                
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prntordno1      , sizeof( pOutBlock->prntordno1      ), DATA_TYPE_LONG      ) );    // ���ֹ���ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prntordno       , sizeof( pOutBlock->prntordno       ), DATA_TYPE_LONG      ) );    // ���ֹ���ȣ              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mgempno         , sizeof( pOutBlock->mgempno         ), DATA_TYPE_STRING    ) );    // ���������ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordundrqty   , sizeof( pOutBlock->orgordundrqty   ), DATA_TYPE_LONG      ) );    // ���ֹ���ü�����        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty   ), DATA_TYPE_LONG      ) );    // ���ֹ���������          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordordcancelqty , sizeof( pOutBlock->ordordcancelqty ), DATA_TYPE_LONG      ) );    // ���ֹ���Ҽ���          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->nmcpysndno      , sizeof( pOutBlock->nmcpysndno      ), DATA_TYPE_LONG      ) );    // ��ȸ����۽Ź�ȣ        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordamt          , sizeof( pOutBlock->ordamt          ), DATA_TYPE_LONG      ) );    // �ֹ��ݾ�                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bnstp           , sizeof( pOutBlock->bnstp           ), DATA_TYPE_STRING    ) );    // �Ÿű���                
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spareordno1     , sizeof( pOutBlock->spareordno1     ), DATA_TYPE_LONG      ) );    // �����ֹ���ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spareordno      , sizeof( pOutBlock->spareordno      ), DATA_TYPE_LONG      ) );    // �����ֹ���ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cvrgseqno       , sizeof( pOutBlock->cvrgseqno       ), DATA_TYPE_LONG      ) );    // �ݴ�Ÿ��Ϸù�ȣ        
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rsvordno1       , sizeof( pOutBlock->rsvordno1       ), DATA_TYPE_LONG      ) );    // �����ֹ���ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rsvordno        , sizeof( pOutBlock->rsvordno        ), DATA_TYPE_LONG      ) );    // �����ֹ���ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mtordseqno      , sizeof( pOutBlock->mtordseqno      ), DATA_TYPE_LONG      ) );    // �����ֹ��Ϸù�ȣ        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spareordqty     , sizeof( pOutBlock->spareordqty     ), DATA_TYPE_LONG      ) );    // �����ֹ�����            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orduserid       , sizeof( pOutBlock->orduserid       ), DATA_TYPE_STRING    ) );    // �ֹ������ȣ            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spotordqty      , sizeof( pOutBlock->spotordqty      ), DATA_TYPE_LONG      ) );    // �ǹ��ֹ�����            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordruseqty      , sizeof( pOutBlock->ordruseqty      ), DATA_TYPE_LONG      ) );    // �����ֹ�����          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mnyordamt       , sizeof( pOutBlock->mnyordamt       ), DATA_TYPE_LONG      ) );    // �����ֹ��ݾ�            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordsubstamt     , sizeof( pOutBlock->ordsubstamt     ), DATA_TYPE_LONG      ) );    // �ֹ����ݾ�            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ruseordamt      , sizeof( pOutBlock->ruseordamt      ), DATA_TYPE_LONG      ) );    // �����ֹ��ݾ�          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordcmsnamt      , sizeof( pOutBlock->ordcmsnamt      ), DATA_TYPE_LONG      ) );    // �������ֹ��ݾ�          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtuseamt      , sizeof( pOutBlock->crdtuseamt      ), DATA_TYPE_LONG      ) );    // ���ſ�㺸�����    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty       ), DATA_TYPE_LONG      ) );    // �ܰ����                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty  ), DATA_TYPE_LONG      ) );    // �ǹ����ɼ���            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty  ), DATA_TYPE_LONG      ) );    // ���밡�ɼ���(�ŵ�)    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->flctqty         , sizeof( pOutBlock->flctqty         ), DATA_TYPE_LONG      ) );    // ��������                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2     ), DATA_TYPE_LONG      ) );    // �ܰ����(D2)            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty     ), DATA_TYPE_LONG      ) );    // �ŵ��ֹ����ɼ���        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty ), DATA_TYPE_LONG      ) );    // ��ü��ŵ��ֹ�����      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc      ), DATA_TYPE_LONG      ) );    // ��ո��԰�              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pchsamt         , sizeof( pOutBlock->pchsamt         ), DATA_TYPE_LONG      ) );    // ���Աݾ�                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->deposit         , sizeof( pOutBlock->deposit         ), DATA_TYPE_LONG      ) );    // ������                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->substamt        , sizeof( pOutBlock->substamt        ), DATA_TYPE_LONG      ) );    // ����                  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn      ), DATA_TYPE_LONG      ) );    // ��Ź���ű�����          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn    ), DATA_TYPE_LONG      ) );    // ��Ź���űݴ��          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt ), DATA_TYPE_LONG      ) );    // �ſ�㺸�����        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny      ), DATA_TYPE_LONG      ) );    // �ֹ���������            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt ), DATA_TYPE_LONG      ) );    // �ֹ����ɴ��            
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt     ), DATA_TYPE_LONG      ) );    // ���밡�ɱݾ�          
	} 
     
	return 0L;
}
