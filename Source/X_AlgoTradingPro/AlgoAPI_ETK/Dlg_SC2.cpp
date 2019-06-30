// Dlg_SC2.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_SC2.h"

#include "../../include/API_ETK/SC2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC2 dialog

IMPLEMENT_DYNCREATE(CDlg_SC2, CDialog)

CDlg_SC2::CDlg_SC2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SC2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_SC2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_SC2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_SC2)
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_SC2, CDialog)
	//{{AFX_MSG_MAP(CDlg_SC2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC2 message handlers

BOOL CDlg_SC2::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �ֹ��� ���õ� �ǽð�TR�� �Է°��� �����Ƿ� ȭ���� ���µɶ� ����Ѵ�.
	AdviseData();

	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_SC2::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_SC2::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "�����Ϸù�ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���¹�ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ID             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���౸��             " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ȣ����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����������         " );
	m_ctrlOutBlock.InsertItem( nRow++, "TRCODE               " );
	m_ctrlOutBlock.InsertItem( nRow++, "�̿���ȣ           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ID             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Ӹ�ü             " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F�Ϸù�ȣ          " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����Ϸù�ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "TR����ID             " );
	m_ctrlOutBlock.InsertItem( nRow++, "����IP               " );
	m_ctrlOutBlock.InsertItem( nRow++, "�缳IP               " );
	m_ctrlOutBlock.InsertItem( nRow++, "ó��������ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܸ���ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "APó���ð�           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�޼����ڵ�           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�޼�����±���       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����û����         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Ű               " );
	m_ctrlOutBlock.InsertItem( nRow++, "��û���ڵ尳��       " );
	m_ctrlOutBlock.InsertItem( nRow++, "���񿵿�             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ӱ���             " );
	m_ctrlOutBlock.InsertItem( nRow++, "����Ű��             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ý��۱���       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ش�����         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����޽�������       " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ȸ�߿���           " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F�̺�ƮID          " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F����              " );
	m_ctrlOutBlock.InsertItem( nRow++, "���񿵿�             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�ü�������ڵ�     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ������ڵ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ������ڵ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "����������ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���¹�ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���¹�ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���¸�               " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����               " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ���ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ���ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ȣ           " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ȣ           " );
	m_ctrlOutBlock.InsertItem( nRow++, "ü���ȣ             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "ü�����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "ü�ᰡ��             " );
	m_ctrlOutBlock.InsertItem( nRow++, "����Ȯ�μ���         " );
	m_ctrlOutBlock.InsertItem( nRow++, "����Ȯ�ΰ���         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Ȯ�μ���         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�źμ���             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�ó�������ڵ�     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ֹ��Ϸù�ȣ     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "ȣ�������ڵ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ü�����       " );
	m_ctrlOutBlock.InsertItem( nRow++, "���������ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ù�ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ݴ�Ÿ��ֹ�����     " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ü�����(�ֹ�)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���ü�����     " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���������       " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ֹ���Ҽ���       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ����ü�ᰡ��     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ��ݾ�             " );
	m_ctrlOutBlock.InsertItem( nRow++, "ǥ�������ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ǥ�������ȣ       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�Ÿű���             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ��ŷ������ڵ�     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ſ�ŷ��ڵ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������ջ��ڵ�       " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ÿ�ü�ڵ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "������               " );
	m_ctrlOutBlock.InsertItem( nRow++, "ȸ��/��ȸ�����ȣ    " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ����¹�ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "����������ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���������ȣ         " );
	m_ctrlOutBlock.InsertItem( nRow++, "��������������ȣ     " );
	m_ctrlOutBlock.InsertItem( nRow++, "����������¹�ȣ     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������屸��         " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ͻ����ڵ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�������űݷ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "������űݷ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "����ü��ݾ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ü��ݾ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "������ü��ݾ�       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ſ�㺸ü��ݾ�     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ſ�ü��ݾ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "��������ü��ݾ�   " );
	m_ctrlOutBlock.InsertItem( nRow++, "��������ü��ݾ�   " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ǹ�ü�����         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���ŵ�ü�����       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�׷�Id               " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ�ȸ��             " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ʈ��������ȣ       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ٽ��Ϲ�ȣ           " );
	m_ctrlOutBlock.InsertItem( nRow++, "Ʈ��ġ��ȣ           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����۹�ȣ           " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ���Id             " );
	m_ctrlOutBlock.InsertItem( nRow++, "���԰�������         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܱ��ΰ�����ȣ       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ���¡������       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�����������ڱ���     " );
	m_ctrlOutBlock.InsertItem( nRow++, "ü��ð�             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŷ��Ҽ���ü��ð�   " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܿ�����ݾ�         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܰ����             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ǹ����ɼ���         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���밡�ɼ���(�ŵ�) " );
	m_ctrlOutBlock.InsertItem( nRow++, "��������             " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ܰ����(d2)         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ��ֹ����ɼ���     " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ü��ŵ��ֹ�����   " );
	m_ctrlOutBlock.InsertItem( nRow++, "��ո��԰�           " );
	m_ctrlOutBlock.InsertItem( nRow++, "���Աݾ�             " );
	m_ctrlOutBlock.InsertItem( nRow++, "������               " );
	m_ctrlOutBlock.InsertItem( nRow++, "����               " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ź���ű�����       " );
	m_ctrlOutBlock.InsertItem( nRow++, "��Ź���űݴ��       " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ſ�㺸�����     " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ���������         " );
	m_ctrlOutBlock.InsertItem( nRow++, "�ֹ����ɴ��         " );
	m_ctrlOutBlock.InsertItem( nRow++, "���밡�ɱݾ�       " );
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_SC2::AdviseData()
{
	TCHAR	szTrCode[]		= "SC2";

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		"",							// SC2�� �� �����͸� �Ѱܾ� �Ѵ�.
		0							// SC2�� ������ ���̴� 0�̴�.
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
void CDlg_SC2::UnadviseData()
{
	TCHAR	szTrCode[]		= "SC2";
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		"",							// SC2�� �� �����͸� �Ѱܾ� �Ѵ�.
		0							// SC2�� ������ ���̴� 0�̴�.
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
LRESULT CDlg_SC2::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "SC2" ) == 0 )
	{
		LPSC2_OutBlock pOutBlock = (LPSC2_OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lineseq             , sizeof( pOutBlock->lineseq             ), DATA_TYPE_LONG      ) );    // �����Ϸù�ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno               , sizeof( pOutBlock->accno               ), DATA_TYPE_STRING    ) );    // ���¹�ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->user                , sizeof( pOutBlock->user                ), DATA_TYPE_STRING    ) );    // ������ID             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->len                 , sizeof( pOutBlock->len                 ), DATA_TYPE_LONG      ) );    // �������             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->gubun               , sizeof( pOutBlock->gubun               ), DATA_TYPE_STRING    ) );    // �������             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compress            , sizeof( pOutBlock->compress            ), DATA_TYPE_STRING    ) );    // ���౸��             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->encrypt             , sizeof( pOutBlock->encrypt             ), DATA_TYPE_STRING    ) );    // ��ȣ����             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offset              , sizeof( pOutBlock->offset              ), DATA_TYPE_LONG      ) );    // �����������         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trcode              , sizeof( pOutBlock->trcode              ), DATA_TYPE_STRING    ) );    // TRCODE               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compid              , sizeof( pOutBlock->compid              ), DATA_TYPE_STRING    ) );    // �̿���ȣ           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->userid              , sizeof( pOutBlock->userid              ), DATA_TYPE_STRING    ) );    // �����ID             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->media               , sizeof( pOutBlock->media               ), DATA_TYPE_STRING    ) );    // ���Ӹ�ü             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ifid                , sizeof( pOutBlock->ifid                ), DATA_TYPE_STRING    ) );    // I/F�Ϸù�ȣ          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->seq                 , sizeof( pOutBlock->seq                 ), DATA_TYPE_STRING    ) );    // �����Ϸù�ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trid                , sizeof( pOutBlock->trid                ), DATA_TYPE_STRING    ) );    // TR����ID             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pubip               , sizeof( pOutBlock->pubip               ), DATA_TYPE_STRING    ) );    // ����IP               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prvip               , sizeof( pOutBlock->prvip               ), DATA_TYPE_STRING    ) );    // �缳IP               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pcbpno              , sizeof( pOutBlock->pcbpno              ), DATA_TYPE_STRING    ) );    // ó��������ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bpno                , sizeof( pOutBlock->bpno                ), DATA_TYPE_STRING    ) );    // ������ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->termno              , sizeof( pOutBlock->termno              ), DATA_TYPE_STRING    ) );    // �ܸ���ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lang                , sizeof( pOutBlock->lang                ), DATA_TYPE_STRING    ) );    // ����             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->proctm              , sizeof( pOutBlock->proctm              ), DATA_TYPE_LONG      ) );    // APó���ð�           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msgcode             , sizeof( pOutBlock->msgcode             ), DATA_TYPE_STRING    ) );    // �޼����ڵ�           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->outgu               , sizeof( pOutBlock->outgu               ), DATA_TYPE_STRING    ) );    // �޼�����±���       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compreq             , sizeof( pOutBlock->compreq             ), DATA_TYPE_STRING    ) );    // �����û����         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->funckey             , sizeof( pOutBlock->funckey             ), DATA_TYPE_STRING    ) );    // ���Ű               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->reqcnt              , sizeof( pOutBlock->reqcnt              ), DATA_TYPE_LONG      ) );    // ��û���ڵ尳��       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->filler              , sizeof( pOutBlock->filler              ), DATA_TYPE_STRING    ) );    // ���񿵿�             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cont                , sizeof( pOutBlock->cont                ), DATA_TYPE_STRING    ) );    // ���ӱ���             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->contkey             , sizeof( pOutBlock->contkey             ), DATA_TYPE_STRING    ) );    // ����Ű��             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varlen              , sizeof( pOutBlock->varlen              ), DATA_TYPE_LONG      ) );    // �����ý��۱���       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varhdlen            , sizeof( pOutBlock->varhdlen            ), DATA_TYPE_LONG      ) );    // �����ش�����         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varmsglen           , sizeof( pOutBlock->varmsglen           ), DATA_TYPE_LONG      ) );    // �����޽�������       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trsrc               , sizeof( pOutBlock->trsrc               ), DATA_TYPE_STRING    ) );    // ��ȸ�߿���           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->eventid             , sizeof( pOutBlock->eventid             ), DATA_TYPE_STRING    ) );    // I/F�̺�ƮID          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ifinfo              , sizeof( pOutBlock->ifinfo              ), DATA_TYPE_STRING    ) );    // I/F����              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->filler1             , sizeof( pOutBlock->filler1             ), DATA_TYPE_STRING    ) );    // ���񿵿�             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordxctptncode       , sizeof( pOutBlock->ordxctptncode       ), DATA_TYPE_STRING    ) );    // �ֹ�ü�������ڵ�     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordmktcode          , sizeof( pOutBlock->ordmktcode          ), DATA_TYPE_STRING    ) );    // �ֹ������ڵ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordptncode          , sizeof( pOutBlock->ordptncode          ), DATA_TYPE_STRING    ) );    // �ֹ������ڵ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mgmtbrnno           , sizeof( pOutBlock->mgmtbrnno           ), DATA_TYPE_STRING    ) );    // ����������ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno1              , sizeof( pOutBlock->accno1              ), DATA_TYPE_STRING    ) );    // ���¹�ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno2              , sizeof( pOutBlock->accno2              ), DATA_TYPE_STRING    ) );    // ���¹�ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->acntnm              , sizeof( pOutBlock->acntnm              ), DATA_TYPE_STRING    ) );    // ���¸�               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->Isuno               , sizeof( pOutBlock->Isuno               ), DATA_TYPE_STRING    ) );    // �����ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->Isunm               , sizeof( pOutBlock->Isunm               ), DATA_TYPE_STRING    ) );    // �����               
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordno1              , sizeof( pOutBlock->ordno1              ), DATA_TYPE_LONG      ) );    // �ֹ���ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordno               , sizeof( pOutBlock->ordno               ), DATA_TYPE_LONG      ) );    // �ֹ���ȣ             
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordno1           , sizeof( pOutBlock->orgordno1           ), DATA_TYPE_LONG      ) );    // ���ֹ���ȣ           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordno            , sizeof( pOutBlock->orgordno            ), DATA_TYPE_LONG      ) );    // ���ֹ���ȣ           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->execno              , sizeof( pOutBlock->execno              ), DATA_TYPE_LONG      ) );    // ü���ȣ             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordqty              , sizeof( pOutBlock->ordqty              ), DATA_TYPE_LONG      ) );    // �ֹ�����             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordprc              , sizeof( pOutBlock->ordprc              ), DATA_TYPE_LONG      ) );    // �ֹ�����             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->execqty             , sizeof( pOutBlock->execqty             ), DATA_TYPE_LONG      ) );    // ü�����             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->execprc             , sizeof( pOutBlock->execprc             ), DATA_TYPE_LONG      ) );    // ü�ᰡ��             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdfycnfqty          , sizeof( pOutBlock->mdfycnfqty          ), DATA_TYPE_LONG      ) );    // ����Ȯ�μ���         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdfycnfprc          , sizeof( pOutBlock->mdfycnfprc          ), DATA_TYPE_LONG      ) );    // ����Ȯ�ΰ���         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->canccnfqty          , sizeof( pOutBlock->canccnfqty          ), DATA_TYPE_LONG      ) );    // ���Ȯ�μ���         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rjtqty              , sizeof( pOutBlock->rjtqty              ), DATA_TYPE_LONG      ) );    // �źμ���             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordtrxptncode       , sizeof( pOutBlock->ordtrxptncode       ), DATA_TYPE_LONG      ) );    // �ֹ�ó�������ڵ�     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mtiordseqno         , sizeof( pOutBlock->mtiordseqno         ), DATA_TYPE_LONG      ) );    // �����ֹ��Ϸù�ȣ     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordcndi             , sizeof( pOutBlock->ordcndi             ), DATA_TYPE_STRING    ) );    // �ֹ�����             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordprcptncode       , sizeof( pOutBlock->ordprcptncode       ), DATA_TYPE_STRING    ) );    // ȣ�������ڵ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->nsavtrdqty          , sizeof( pOutBlock->nsavtrdqty          ), DATA_TYPE_LONG      ) );    // ������ü�����       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->shtnIsuno           , sizeof( pOutBlock->shtnIsuno           ), DATA_TYPE_STRING    ) );    // ���������ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->opdrtnno            , sizeof( pOutBlock->opdrtnno            ), DATA_TYPE_STRING    ) );    // ������ù�ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cvrgordtp           , sizeof( pOutBlock->cvrgordtp           ), DATA_TYPE_STRING    ) );    // �ݴ�Ÿ��ֹ�����     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->unercqty            , sizeof( pOutBlock->unercqty            ), DATA_TYPE_LONG      ) );    // ��ü�����(�ֹ�)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordunercqty      , sizeof( pOutBlock->orgordunercqty      ), DATA_TYPE_LONG      ) );    // ���ֹ���ü�����     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordmdfyqty       , sizeof( pOutBlock->orgordmdfyqty       ), DATA_TYPE_LONG      ) );    // ���ֹ���������       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordcancqty       , sizeof( pOutBlock->orgordcancqty       ), DATA_TYPE_LONG      ) );    // ���ֹ���Ҽ���       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordavrexecprc       , sizeof( pOutBlock->ordavrexecprc       ), DATA_TYPE_LONG      ) );    // �ֹ����ü�ᰡ��     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordamt              , sizeof( pOutBlock->ordamt              ), DATA_TYPE_LONG      ) );    // �ֹ��ݾ�             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->stdIsuno            , sizeof( pOutBlock->stdIsuno            ), DATA_TYPE_STRING    ) );    // ǥ�������ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bfstdIsuno          , sizeof( pOutBlock->bfstdIsuno          ), DATA_TYPE_STRING    ) );    // ��ǥ�������ȣ       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bnstp               , sizeof( pOutBlock->bnstp               ), DATA_TYPE_STRING    ) );    // �Ÿű���             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordtrdptncode       , sizeof( pOutBlock->ordtrdptncode       ), DATA_TYPE_STRING    ) );    // �ֹ��ŷ������ڵ�     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mgntrncode          , sizeof( pOutBlock->mgntrncode          ), DATA_TYPE_STRING    ) );    // �ſ�ŷ��ڵ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->adduptp             , sizeof( pOutBlock->adduptp             ), DATA_TYPE_STRING    ) );    // �������ջ��ڵ�       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->commdacode          , sizeof( pOutBlock->commdacode          ), DATA_TYPE_STRING    ) );    // ��Ÿ�ü�ڵ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->Loandt              , sizeof( pOutBlock->Loandt              ), DATA_TYPE_STRING    ) );    // ������               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mbrnmbrno           , sizeof( pOutBlock->mbrnmbrno           ), DATA_TYPE_LONG      ) );    // ȸ��/��ȸ�����ȣ    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordacntno           , sizeof( pOutBlock->ordacntno           ), DATA_TYPE_STRING    ) );    // �ֹ����¹�ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->agrgbrnno           , sizeof( pOutBlock->agrgbrnno           ), DATA_TYPE_STRING    ) );    // ����������ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mgempno             , sizeof( pOutBlock->mgempno             ), DATA_TYPE_STRING    ) );    // ���������ȣ         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futsLnkbrnno        , sizeof( pOutBlock->futsLnkbrnno        ), DATA_TYPE_STRING    ) );    // ��������������ȣ     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futsLnkacntno       , sizeof( pOutBlock->futsLnkacntno       ), DATA_TYPE_STRING    ) );    // ����������¹�ȣ     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futsmkttp           , sizeof( pOutBlock->futsmkttp           ), DATA_TYPE_STRING    ) );    // �������屸��         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->regmktcode          , sizeof( pOutBlock->regmktcode          ), DATA_TYPE_STRING    ) );    // ��Ͻ����ڵ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mnymgnrat           , sizeof( pOutBlock->mnymgnrat           ), DATA_TYPE_LONG      ) );    // �������űݷ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->substmgnrat         , sizeof( pOutBlock->substmgnrat         ), DATA_TYPE_LONG      ) );    // ������űݷ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mnyexecamt          , sizeof( pOutBlock->mnyexecamt          ), DATA_TYPE_LONG      ) );    // ����ü��ݾ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ubstexecamt         , sizeof( pOutBlock->ubstexecamt         ), DATA_TYPE_LONG      ) );    // ���ü��ݾ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cmsnamtexecamt      , sizeof( pOutBlock->cmsnamtexecamt      ), DATA_TYPE_LONG      ) );    // ������ü��ݾ�       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtpldgexecamt     , sizeof( pOutBlock->crdtpldgexecamt     ), DATA_TYPE_LONG      ) );    // �ſ�㺸ü��ݾ�     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtexecamt         , sizeof( pOutBlock->crdtexecamt         ), DATA_TYPE_LONG      ) );    // �ſ�ü��ݾ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prdayruseexecval    , sizeof( pOutBlock->prdayruseexecval    ), DATA_TYPE_LONG      ) );    // ��������ü��ݾ�   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdayruseexecval    , sizeof( pOutBlock->crdayruseexecval    ), DATA_TYPE_LONG      ) );    // ��������ü��ݾ�   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spotexecqty         , sizeof( pOutBlock->spotexecqty         ), DATA_TYPE_LONG      ) );    // �ǹ�ü�����         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->stslexecqty         , sizeof( pOutBlock->stslexecqty         ), DATA_TYPE_LONG      ) );    // ���ŵ�ü�����       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->strtgcode           , sizeof( pOutBlock->strtgcode           ), DATA_TYPE_STRING    ) );    // �����ڵ�             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->grpId               , sizeof( pOutBlock->grpId               ), DATA_TYPE_STRING    ) );    // �׷�Id               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordseqno            , sizeof( pOutBlock->ordseqno            ), DATA_TYPE_LONG      ) );    // �ֹ�ȸ��             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ptflno              , sizeof( pOutBlock->ptflno              ), DATA_TYPE_LONG      ) );    // ��Ʈ��������ȣ       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bskno               , sizeof( pOutBlock->bskno               ), DATA_TYPE_LONG      ) );    // �ٽ��Ϲ�ȣ           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trchno              , sizeof( pOutBlock->trchno              ), DATA_TYPE_LONG      ) );    // Ʈ��ġ��ȣ           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->itemno              , sizeof( pOutBlock->itemno              ), DATA_TYPE_LONG      ) );    // �����۹�ȣ           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orduserId           , sizeof( pOutBlock->orduserId           ), DATA_TYPE_STRING    ) );    // �ֹ���Id             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->brwmgmtYn           , sizeof( pOutBlock->brwmgmtYn           ), DATA_TYPE_LONG      ) );    // ���԰�������         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->frgrunqno           , sizeof( pOutBlock->frgrunqno           ), DATA_TYPE_STRING    ) );    // �ܱ��ΰ�����ȣ       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trtzxLevytp         , sizeof( pOutBlock->trtzxLevytp         ), DATA_TYPE_STRING    ) );    // �ŷ���¡������       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lptp                , sizeof( pOutBlock->lptp                ), DATA_TYPE_STRING    ) );    // �����������ڱ���     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->exectime            , sizeof( pOutBlock->exectime            ), DATA_TYPE_STRING    ) );    // ü��ð�             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rcptexectime        , sizeof( pOutBlock->rcptexectime        ), DATA_TYPE_STRING    ) );    // �ŷ��Ҽ���ü��ð�   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rmndLoanamt         , sizeof( pOutBlock->rmndLoanamt         ), DATA_TYPE_LONG      ) );    // �ܿ�����ݾ�         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->secbalqty           , sizeof( pOutBlock->secbalqty           ), DATA_TYPE_LONG      ) );    // �ܰ����             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spotordableqty      , sizeof( pOutBlock->spotordableqty      ), DATA_TYPE_LONG      ) );    // �ǹ����ɼ���         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordableruseqty      , sizeof( pOutBlock->ordableruseqty      ), DATA_TYPE_LONG      ) );    // ���밡�ɼ���(�ŵ�) 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->flctqty             , sizeof( pOutBlock->flctqty             ), DATA_TYPE_LONG      ) );    // ��������             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->secbalqtyd2         , sizeof( pOutBlock->secbalqtyd2         ), DATA_TYPE_LONG      ) );    // �ܰ����(d2)         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->sellableqty         , sizeof( pOutBlock->sellableqty         ), DATA_TYPE_LONG      ) );    // �ŵ��ֹ����ɼ���     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->unercsellordqty     , sizeof( pOutBlock->unercsellordqty     ), DATA_TYPE_LONG      ) );    // ��ü��ŵ��ֹ�����   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->avrpchsprc          , sizeof( pOutBlock->avrpchsprc          ), DATA_TYPE_LONG      ) );    // ��ո��԰�           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pchsant             , sizeof( pOutBlock->pchsant             ), DATA_TYPE_LONG      ) );    // ���Աݾ�             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->deposit             , sizeof( pOutBlock->deposit             ), DATA_TYPE_LONG      ) );    // ������               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->substamt            , sizeof( pOutBlock->substamt            ), DATA_TYPE_LONG      ) );    // ����               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->csgnmnymgn          , sizeof( pOutBlock->csgnmnymgn          ), DATA_TYPE_LONG      ) );    // ��Ź���ű�����       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->csgnsubstmgn        , sizeof( pOutBlock->csgnsubstmgn        ), DATA_TYPE_LONG      ) );    // ��Ź���űݴ��       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtpldgruseamt     , sizeof( pOutBlock->crdtpldgruseamt     ), DATA_TYPE_LONG      ) );    // �ſ�㺸�����     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordablemny          , sizeof( pOutBlock->ordablemny          ), DATA_TYPE_LONG      ) );    // �ֹ���������         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordablesubstamt     , sizeof( pOutBlock->ordablesubstamt     ), DATA_TYPE_LONG      ) );    // �ֹ����ɴ��         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ruseableamt         , sizeof( pOutBlock->ruseableamt         ), DATA_TYPE_LONG      ) );    // ���밡�ɱݾ�       
	} 
     
	return 0L;
}
