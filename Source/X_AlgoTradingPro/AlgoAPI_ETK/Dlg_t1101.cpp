// Dlg_t1101.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t1101.h"

#include "../../include/API_ETK/t1101.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t1101 dialog

IMPLEMENT_DYNCREATE(CDlg_t1101, CDialog)

// CDlg_t1101::CDlg_t1101(CWnd* pParent /*=NULL*/)
// : CDialog(CDlg_t1101::IDD, pParent)
CDlg_t1101::CDlg_t1101()
{
	//{{AFX_DATA_INIT(CDlg_t1101)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t1101::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t1101)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_t1101, CDialog)
	//{{AFX_MSG_MAP(CDlg_t1101)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,		  OnButtonRequest	  )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t1101 message handlers

BOOL CDlg_t1101::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_t1101::OnButtonRequest() 
{
	RequestData();
}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_t1101::InitCtrls()
{
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ��", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	// List Control�� OutBlock �ʵ���� �־��ش�.
	m_ctrlOutBlock.InsertItem(  0, "�ѱ۸�            " );
	m_ctrlOutBlock.InsertItem(  1, "���簡            " );
	m_ctrlOutBlock.InsertItem(  2, "���ϴ�񱸺�      " );
	m_ctrlOutBlock.InsertItem(  3, "���ϴ��          " );
	m_ctrlOutBlock.InsertItem(  4, "�����            " );
	m_ctrlOutBlock.InsertItem(  5, "�����ŷ���        " );
	m_ctrlOutBlock.InsertItem(  6, "��������          " );
	m_ctrlOutBlock.InsertItem(  7, "�ŵ�ȣ��1         " );
	m_ctrlOutBlock.InsertItem(  8, "�ż�ȣ��1         " );
	m_ctrlOutBlock.InsertItem(  9, "�ŵ�ȣ������1     " );
	m_ctrlOutBlock.InsertItem( 10, "�ż�ȣ������1     " );
	m_ctrlOutBlock.InsertItem( 11, "�����ŵ�������1 " );
	m_ctrlOutBlock.InsertItem( 12, "�����ż�������1 " );
	m_ctrlOutBlock.InsertItem( 13, "�ŵ�ȣ��2         " );
	m_ctrlOutBlock.InsertItem( 14, "�ż�ȣ��2         " );
	m_ctrlOutBlock.InsertItem( 15, "�ŵ�ȣ������2     " );
	m_ctrlOutBlock.InsertItem( 16, "�ż�ȣ������2     " );
	m_ctrlOutBlock.InsertItem( 17, "�����ŵ�������2 " );
	m_ctrlOutBlock.InsertItem( 18, "�����ż�������2 " );
	m_ctrlOutBlock.InsertItem( 19, "�ŵ�ȣ��3         " );
	m_ctrlOutBlock.InsertItem( 20, "�ż�ȣ��3         " );
	m_ctrlOutBlock.InsertItem( 21, "�ŵ�ȣ������3     " );
	m_ctrlOutBlock.InsertItem( 22, "�ż�ȣ������3     " );
	m_ctrlOutBlock.InsertItem( 23, "�����ŵ�������3 " );
	m_ctrlOutBlock.InsertItem( 24, "�����ż�������3 " );
	m_ctrlOutBlock.InsertItem( 25, "�ŵ�ȣ��4         " );
	m_ctrlOutBlock.InsertItem( 26, "�ż�ȣ��4         " );
	m_ctrlOutBlock.InsertItem( 27, "�ŵ�ȣ������4     " );
	m_ctrlOutBlock.InsertItem( 28, "�ż�ȣ������4     " );
	m_ctrlOutBlock.InsertItem( 29, "�����ŵ�������4 " );
	m_ctrlOutBlock.InsertItem( 30, "�����ż�������4 " );
	m_ctrlOutBlock.InsertItem( 31, "�ŵ�ȣ��5         " );
	m_ctrlOutBlock.InsertItem( 32, "�ż�ȣ��5         " );
	m_ctrlOutBlock.InsertItem( 33, "�ŵ�ȣ������5     " );
	m_ctrlOutBlock.InsertItem( 34, "�ż�ȣ������5     " );
	m_ctrlOutBlock.InsertItem( 35, "�����ŵ�������5 " );
	m_ctrlOutBlock.InsertItem( 36, "�����ż�������5 " );
	m_ctrlOutBlock.InsertItem( 37, "�ŵ�ȣ��6         " );
	m_ctrlOutBlock.InsertItem( 38, "�ż�ȣ��6         " );
	m_ctrlOutBlock.InsertItem( 39, "�ŵ�ȣ������6     " );
	m_ctrlOutBlock.InsertItem( 40, "�ż�ȣ������6     " );
	m_ctrlOutBlock.InsertItem( 41, "�����ŵ�������6 " );
	m_ctrlOutBlock.InsertItem( 42, "�����ż�������6 " );
	m_ctrlOutBlock.InsertItem( 43, "�ŵ�ȣ��7         " );
	m_ctrlOutBlock.InsertItem( 44, "�ż�ȣ��7         " );
	m_ctrlOutBlock.InsertItem( 45, "�ŵ�ȣ������7     " );
	m_ctrlOutBlock.InsertItem( 46, "�ż�ȣ������7     " );
	m_ctrlOutBlock.InsertItem( 47, "�����ŵ�������7 " );
	m_ctrlOutBlock.InsertItem( 48, "�����ż�������7 " );
	m_ctrlOutBlock.InsertItem( 49, "�ŵ�ȣ��8         " );
	m_ctrlOutBlock.InsertItem( 50, "�ż�ȣ��8         " );
	m_ctrlOutBlock.InsertItem( 51, "�ŵ�ȣ������8     " );
	m_ctrlOutBlock.InsertItem( 52, "�ż�ȣ������8     " );
	m_ctrlOutBlock.InsertItem( 53, "�����ŵ�������8 " );
	m_ctrlOutBlock.InsertItem( 54, "�����ż�������8 " );
	m_ctrlOutBlock.InsertItem( 55, "�ŵ�ȣ��9         " );
	m_ctrlOutBlock.InsertItem( 56, "�ż�ȣ��9         " );
	m_ctrlOutBlock.InsertItem( 57, "�ŵ�ȣ������9     " );
	m_ctrlOutBlock.InsertItem( 58, "�ż�ȣ������9     " );
	m_ctrlOutBlock.InsertItem( 59, "�����ŵ�������9 " );
	m_ctrlOutBlock.InsertItem( 60, "�����ż�������9 " );
	m_ctrlOutBlock.InsertItem( 61, "�ŵ�ȣ��10        " );
	m_ctrlOutBlock.InsertItem( 62, "�ż�ȣ��10        " );
	m_ctrlOutBlock.InsertItem( 63, "�ŵ�ȣ������10    " );
	m_ctrlOutBlock.InsertItem( 64, "�ż�ȣ������10    " );
	m_ctrlOutBlock.InsertItem( 65, "�����ŵ�������10" );
	m_ctrlOutBlock.InsertItem( 66, "�����ż�������10" );
	m_ctrlOutBlock.InsertItem( 67, "�ŵ�ȣ��������    " );
	m_ctrlOutBlock.InsertItem( 68, "�ż�ȣ��������    " );
	m_ctrlOutBlock.InsertItem( 69, "�����ŵ���������" );
	m_ctrlOutBlock.InsertItem( 70, "�����ż���������" );
	m_ctrlOutBlock.InsertItem( 71, "���Žð�          " );
	m_ctrlOutBlock.InsertItem( 72, "����ü�ᰡ��      " );
	m_ctrlOutBlock.InsertItem( 73, "����ü�����      " );
	m_ctrlOutBlock.InsertItem( 74, "����ü�����ϱ���  " );
	m_ctrlOutBlock.InsertItem( 75, "����ü�����ϴ��  " );
	m_ctrlOutBlock.InsertItem( 76, "����ü������    " );
	m_ctrlOutBlock.InsertItem( 77, "�ð��ܸŵ��ܷ�    " );
	m_ctrlOutBlock.InsertItem( 78, "�ð��ܸż��ܷ�    " );
	m_ctrlOutBlock.InsertItem( 79, "���ñ���          " );
	m_ctrlOutBlock.InsertItem( 80, "�����ڵ�          " );
	m_ctrlOutBlock.InsertItem( 81, "���Ѱ�            " );
	m_ctrlOutBlock.InsertItem( 82, "���Ѱ�            " );
	m_ctrlOutBlock.InsertItem( 83, "�ð�              " );
	m_ctrlOutBlock.InsertItem( 84, "��              " );
	m_ctrlOutBlock.InsertItem( 85, "����              " );
}

//--------------------------------------------------------------------------------------
// ������ ��ȸ
//--------------------------------------------------------------------------------------
void CDlg_t1101::RequestData( BOOL bNext )
{
	//-----------------------------------------------------------
	// �ֽ����簡ȣ����ȸ(t1101) ( attr,block,headtype=A )
	t1101InBlock	pckInBlock;

	TCHAR			szTrNo[]	= "t1101";
	char			szNextKey[]	= "";

	//-----------------------------------------------------------
	// ������ ���
	CString str_shcode;		GetDlgItemText( IDC_EDIT_INBLOCK_SHCODE, str_shcode );		// �����ڵ�

	//-----------------------------------------------------------
	// ������ �ʱ�ȭ
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// ������ �Է�
	SetPacketData( pckInBlock.shcode, sizeof( pckInBlock.shcode ), str_shcode, DATA_TYPE_STRING );				// �����ڵ�

	//-----------------------------------------------------------
	// ������ ����
	int nRqID = g_iXingAPI.Request( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_DATA ���� �´�.
		szTrNo,						// TR ��ȣ
		&pckInBlock,				// InBlock ������
		sizeof( pckInBlock ),		// InBlock ������ ũ��
		bNext,						// ������ȸ ���� : ������ȸ�� ��쿡 �����Ѵ�.
		szNextKey,					// ������ȸ Key : Header Type�� B �� ��쿣 ���� ��ȸ�� ���� Next Key�� �־��ش�.
		30							// Timeout(��) : �ش� �ð�(��)���� �����Ͱ� ���� ������ Timeout�� �߻��Ѵ�. XM_TIMEOUT_DATA �޽����� �߻��Ѵ�.
	);

	//-----------------------------------------------------------
	// Request ID�� 0���� ���� ��쿡�� �����̴�.
	if( nRqID < 0 )
	{
		MessageBox( "��ȸ����", "����", MB_ICONSTOP );
	}
}

//--------------------------------------------------------------------------------------
// �����͸� ����
//--------------------------------------------------------------------------------------
LRESULT CDlg_t1101::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data�� ����
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;

		LPt1101OutBlock pOutBlock = (LPt1101OutBlock)pRpData->lpData;

		m_ctrlOutBlock.SetItemText(  0, 1, GetDispData( pOutBlock->hname        , sizeof( pOutBlock->hname         ), DATA_TYPE_STRING ) );
		m_ctrlOutBlock.SetItemText(  1, 1, GetDispData( pOutBlock->price        , sizeof( pOutBlock->price         ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText(  2, 1, GetDispData( pOutBlock->sign         , sizeof( pOutBlock->sign          ), DATA_TYPE_STRING ) );
		m_ctrlOutBlock.SetItemText(  3, 1, GetDispData( pOutBlock->change       , sizeof( pOutBlock->change        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText(  4, 1, GetDispData( pOutBlock->diff         , sizeof( pOutBlock->diff          ), DATA_TYPE_FLOAT , 2 ) );
		m_ctrlOutBlock.SetItemText(  5, 1, GetDispData( pOutBlock->volume       , sizeof( pOutBlock->volume        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText(  6, 1, GetDispData( pOutBlock->jnilclose    , sizeof( pOutBlock->jnilclose     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText(  7, 1, GetDispData( pOutBlock->offerho1     , sizeof( pOutBlock->offerho1      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText(  8, 1, GetDispData( pOutBlock->bidho1       , sizeof( pOutBlock->bidho1        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText(  9, 1, GetDispData( pOutBlock->offerrem1    , sizeof( pOutBlock->offerrem1     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 10, 1, GetDispData( pOutBlock->bidrem1      , sizeof( pOutBlock->bidrem1       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 11, 1, GetDispData( pOutBlock->preoffercha1 , sizeof( pOutBlock->preoffercha1  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 12, 1, GetDispData( pOutBlock->prebidcha1   , sizeof( pOutBlock->prebidcha1    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 13, 1, GetDispData( pOutBlock->offerho2     , sizeof( pOutBlock->offerho2      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 14, 1, GetDispData( pOutBlock->bidho2       , sizeof( pOutBlock->bidho2        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 15, 1, GetDispData( pOutBlock->offerrem2    , sizeof( pOutBlock->offerrem2     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 16, 1, GetDispData( pOutBlock->bidrem2      , sizeof( pOutBlock->bidrem2       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 17, 1, GetDispData( pOutBlock->preoffercha2 , sizeof( pOutBlock->preoffercha2  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 18, 1, GetDispData( pOutBlock->prebidcha2   , sizeof( pOutBlock->prebidcha2    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 19, 1, GetDispData( pOutBlock->offerho3     , sizeof( pOutBlock->offerho3      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 20, 1, GetDispData( pOutBlock->bidho3       , sizeof( pOutBlock->bidho3        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 21, 1, GetDispData( pOutBlock->offerrem3    , sizeof( pOutBlock->offerrem3     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 22, 1, GetDispData( pOutBlock->bidrem3      , sizeof( pOutBlock->bidrem3       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 23, 1, GetDispData( pOutBlock->preoffercha3 , sizeof( pOutBlock->preoffercha3  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 24, 1, GetDispData( pOutBlock->prebidcha3   , sizeof( pOutBlock->prebidcha3    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 25, 1, GetDispData( pOutBlock->offerho4     , sizeof( pOutBlock->offerho4      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 26, 1, GetDispData( pOutBlock->bidho4       , sizeof( pOutBlock->bidho4        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 27, 1, GetDispData( pOutBlock->offerrem4    , sizeof( pOutBlock->offerrem4     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 28, 1, GetDispData( pOutBlock->bidrem4      , sizeof( pOutBlock->bidrem4       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 29, 1, GetDispData( pOutBlock->preoffercha4 , sizeof( pOutBlock->preoffercha4  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 30, 1, GetDispData( pOutBlock->prebidcha4   , sizeof( pOutBlock->prebidcha4    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 31, 1, GetDispData( pOutBlock->offerho5     , sizeof( pOutBlock->offerho5      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 32, 1, GetDispData( pOutBlock->bidho5       , sizeof( pOutBlock->bidho5        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 33, 1, GetDispData( pOutBlock->offerrem5    , sizeof( pOutBlock->offerrem5     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 34, 1, GetDispData( pOutBlock->bidrem5      , sizeof( pOutBlock->bidrem5       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 35, 1, GetDispData( pOutBlock->preoffercha5 , sizeof( pOutBlock->preoffercha5  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 36, 1, GetDispData( pOutBlock->prebidcha5   , sizeof( pOutBlock->prebidcha5    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 37, 1, GetDispData( pOutBlock->offerho6     , sizeof( pOutBlock->offerho6      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 38, 1, GetDispData( pOutBlock->bidho6       , sizeof( pOutBlock->bidho6        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 39, 1, GetDispData( pOutBlock->offerrem6    , sizeof( pOutBlock->offerrem6     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 40, 1, GetDispData( pOutBlock->bidrem6      , sizeof( pOutBlock->bidrem6       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 41, 1, GetDispData( pOutBlock->preoffercha6 , sizeof( pOutBlock->preoffercha6  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 42, 1, GetDispData( pOutBlock->prebidcha6   , sizeof( pOutBlock->prebidcha6    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 43, 1, GetDispData( pOutBlock->offerho7     , sizeof( pOutBlock->offerho7      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 44, 1, GetDispData( pOutBlock->bidho7       , sizeof( pOutBlock->bidho7        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 45, 1, GetDispData( pOutBlock->offerrem7    , sizeof( pOutBlock->offerrem7     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 46, 1, GetDispData( pOutBlock->bidrem7      , sizeof( pOutBlock->bidrem7       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 47, 1, GetDispData( pOutBlock->preoffercha7 , sizeof( pOutBlock->preoffercha7  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 48, 1, GetDispData( pOutBlock->prebidcha7   , sizeof( pOutBlock->prebidcha7    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 49, 1, GetDispData( pOutBlock->offerho8     , sizeof( pOutBlock->offerho8      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 50, 1, GetDispData( pOutBlock->bidho8       , sizeof( pOutBlock->bidho8        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 51, 1, GetDispData( pOutBlock->offerrem8    , sizeof( pOutBlock->offerrem8     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 52, 1, GetDispData( pOutBlock->bidrem8      , sizeof( pOutBlock->bidrem8       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 53, 1, GetDispData( pOutBlock->preoffercha8 , sizeof( pOutBlock->preoffercha8  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 54, 1, GetDispData( pOutBlock->prebidcha8   , sizeof( pOutBlock->prebidcha8    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 55, 1, GetDispData( pOutBlock->offerho9     , sizeof( pOutBlock->offerho9      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 56, 1, GetDispData( pOutBlock->bidho9       , sizeof( pOutBlock->bidho9        ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 57, 1, GetDispData( pOutBlock->offerrem9    , sizeof( pOutBlock->offerrem9     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 58, 1, GetDispData( pOutBlock->bidrem9      , sizeof( pOutBlock->bidrem9       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 59, 1, GetDispData( pOutBlock->preoffercha9 , sizeof( pOutBlock->preoffercha9  ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 60, 1, GetDispData( pOutBlock->prebidcha9   , sizeof( pOutBlock->prebidcha9    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 61, 1, GetDispData( pOutBlock->offerho10    , sizeof( pOutBlock->offerho10     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 62, 1, GetDispData( pOutBlock->bidho10      , sizeof( pOutBlock->bidho10       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 63, 1, GetDispData( pOutBlock->offerrem10   , sizeof( pOutBlock->offerrem10    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 64, 1, GetDispData( pOutBlock->bidrem10     , sizeof( pOutBlock->bidrem10      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 65, 1, GetDispData( pOutBlock->preoffercha10, sizeof( pOutBlock->preoffercha10 ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 66, 1, GetDispData( pOutBlock->prebidcha10  , sizeof( pOutBlock->prebidcha10   ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 67, 1, GetDispData( pOutBlock->offer        , sizeof( pOutBlock->offer         ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 68, 1, GetDispData( pOutBlock->bid          , sizeof( pOutBlock->bid           ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 69, 1, GetDispData( pOutBlock->preoffercha  , sizeof( pOutBlock->preoffercha   ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 70, 1, GetDispData( pOutBlock->prebidcha    , sizeof( pOutBlock->prebidcha     ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 71, 1, GetDispData( pOutBlock->hotime       , sizeof( pOutBlock->hotime        ), DATA_TYPE_STRING ) );
		m_ctrlOutBlock.SetItemText( 72, 1, GetDispData( pOutBlock->yeprice      , sizeof( pOutBlock->yeprice       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 73, 1, GetDispData( pOutBlock->yevolume     , sizeof( pOutBlock->yevolume      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 74, 1, GetDispData( pOutBlock->yesign       , sizeof( pOutBlock->yesign        ), DATA_TYPE_STRING ) );
		m_ctrlOutBlock.SetItemText( 75, 1, GetDispData( pOutBlock->yechange     , sizeof( pOutBlock->yechange      ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 76, 1, GetDispData( pOutBlock->yediff       , sizeof( pOutBlock->yediff        ), DATA_TYPE_FLOAT  , 2) );
		m_ctrlOutBlock.SetItemText( 77, 1, GetDispData( pOutBlock->tmoffer      , sizeof( pOutBlock->tmoffer       ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 78, 1, GetDispData( pOutBlock->tmbid        , sizeof( pOutBlock->tmbid         ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 79, 1, GetDispData( pOutBlock->ho_status    , sizeof( pOutBlock->ho_status     ), DATA_TYPE_STRING ) );
		m_ctrlOutBlock.SetItemText( 80, 1, GetDispData( pOutBlock->shcode       , sizeof( pOutBlock->shcode        ), DATA_TYPE_STRING ) );
		m_ctrlOutBlock.SetItemText( 81, 1, GetDispData( pOutBlock->uplmtprice   , sizeof( pOutBlock->uplmtprice    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 82, 1, GetDispData( pOutBlock->dnlmtprice   , sizeof( pOutBlock->dnlmtprice    ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 83, 1, GetDispData( pOutBlock->open         , sizeof( pOutBlock->open          ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 84, 1, GetDispData( pOutBlock->high         , sizeof( pOutBlock->high          ), DATA_TYPE_LONG   ) );
		m_ctrlOutBlock.SetItemText( 85, 1, GetDispData( pOutBlock->low          , sizeof( pOutBlock->low           ), DATA_TYPE_LONG   ) );
	}
	
	//-------------------------------------------------------------------------------------
	// �޽����� ����
	else if( wParam == MESSAGE_DATA )
	{
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;

		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		
		
		g_iXingAPI.ReleaseMessageData( lParam );
	}
	
	//-------------------------------------------------------------------------------------
	// System Error�� ����
	else if( wParam == SYSTEM_ERROR_DATA )
	{
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		

		g_iXingAPI.ReleaseMessageData( lParam );
	}

	//-------------------------------------------------------------------------------------
	// Release Data�� ����
	else if( wParam == RELEASE_DATA )
	{
		g_iXingAPI.ReleaseRequestData( (int)lParam );
	}

	return 0L;
}

//--------------------------------------------------------------------------------------
// Timeout �߻�
//--------------------------------------------------------------------------------------
LRESULT CDlg_t1101::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}