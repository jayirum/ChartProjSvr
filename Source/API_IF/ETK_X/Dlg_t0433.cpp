// Dlg_t0433.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t0433.h"

#include "../../include/API_ETK/t0433.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t0433 dialog

IMPLEMENT_DYNCREATE(CDlg_t0433, CDialog)

CDlg_t0433::CDlg_t0433(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t0433::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t0433)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t0433::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t0433)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
	DDX_Control(pDX, IDC_LIST_OUTBLOCK1, m_ctrlOutBlock1);
}


BEGIN_MESSAGE_MAP(CDlg_t0433, CDialog)
	//{{AFX_MSG_MAP(CDlg_t0433)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,		  OnButtonRequest	  )
	ON_BN_CLICKED( IDC_BUTTON_REQUEST_NEXT,	  OnButtonRequestNext )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t0433 message handlers

BOOL CDlg_t0433::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_t0433::OnButtonRequest() 
{
	RequestData( FALSE );
}

void CDlg_t0433::OnButtonRequestNext() 
{
	RequestData( TRUE );	
}

//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_t0433::InitCtrls()
{
	//-------------------------------------------------------------------------
	// InBlock : �⺻�� ����
	
	// ���¹�ȣ ����
	char szAccount[20];
	int nCount = g_iXingAPI.GetAccountListCount();
	for( int i=0; i<nCount; i++ )
	{
		if( g_iXingAPI.GetAccountList( i, szAccount, sizeof( szAccount ) ) == FALSE )
		{
			continue;
		}
		
		// ��ǰ�ڵ带 Ȯ���Ͽ� �����ɼ�(51)���¸� ó���Ѵ�.
		if( strncmp( szAccount+3, "51", 2 ) == 0 )
		{
			((CComboBox*)GetDlgItem( IDC_COMBO_INBLOCK_ACCNO ))->AddString( szAccount );
		}
	}
	((CComboBox*)GetDlgItem( IDC_COMBO_INBLOCK_ACCNO       ))->SetCurSel( 0 );	// ���¹�ȣ

	//-----------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�Ÿż����հ�   ", LVCFMT_LEFT,  100 );
	m_ctrlOutBlock.InsertColumn( 1, "�̰��������հ� ", LVCFMT_RIGHT, 100 );
	m_ctrlOutBlock.InsertColumn( 2, "CTS_�����ȣ   ", LVCFMT_LEFT,  100 );
	m_ctrlOutBlock.InsertColumn( 3, "CTS_�Ÿű���   ", LVCFMT_LEFT,  100 );
	
	//-----------------------------------------------------------
	// OutBlock1
	m_ctrlOutBlock1.InsertColumn(  0, "�����ȣ  ", LVCFMT_LEFT  , 100 );
	m_ctrlOutBlock1.InsertColumn(  1, "����      ", LVCFMT_LEFT  , 100 );
	m_ctrlOutBlock1.InsertColumn(  2, "�ܰ����  ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  3, "��մܰ�  ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  4, "�Ÿű���  ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  5, "�Ѹ��Աݾ�", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  6, "ó������  ", LVCFMT_LEFT  , 100 );
}

//--------------------------------------------------------------------------------------
// ������ ��ȸ
//--------------------------------------------------------------------------------------
void CDlg_t0433::RequestData( BOOL bNext )
{
	//-----------------------------------------------------------
	// ����/�ɼǹ̰����ܰ�(t0433) ( tuxcode=t0433,headtype=D )
	t0433InBlock	pckInBlock;

	TCHAR			szTrNo[]	= "t0433";
	char			szNextKey[]	= "";

	//-----------------------------------------------------------
	// ������ ���
	CString str_accno		; GetDlgItemText( IDC_COMBO_INBLOCK_ACCNO	  , str_accno		);		// ���¹�ȣ
	CString str_passwd		; GetDlgItemText( IDC_EDIT_INBLOCK_PASSWD	  , str_passwd		);		// ��й�ȣ
	CString str_cts_expcode	; GetDlgItemText( IDC_EDIT_INBLOCK_CTS_EXPCODE, str_cts_expcode );		// CTS_�����ȣ
	CString str_cts_medocd	; GetDlgItemText( IDC_EDIT_INBLOCK_CTS_MEDOCD , str_cts_medocd  );		// CTS_�Ÿű���

	// ���� ��ȸ�ϰ�쿡 ���� ��ȸ�� ����ϴ� Field�� �����͸� ������ �ȵȴ�.
	if( bNext == FALSE )
	{
		str_cts_expcode = "";
		str_cts_medocd = "";
	}
	
	//-----------------------------------------------------------
	// ������ �ʱ�ȭ
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	
	//-----------------------------------------------------------
	// ������ �Է�
	SetPacketData( pckInBlock.accno		 , sizeof( pckInBlock.accno		 ), str_accno		, DATA_TYPE_STRING );	// ���¹�ȣ
	SetPacketData( pckInBlock.passwd	 , sizeof( pckInBlock.passwd	 ), str_passwd		, DATA_TYPE_STRING );	// ��й�ȣ
	SetPacketData( pckInBlock.cts_expcode, sizeof( pckInBlock.cts_expcode), str_cts_expcode , DATA_TYPE_STRING );	// CTS_�����ȣ
	SetPacketData( pckInBlock.cts_medocd , sizeof( pckInBlock.cts_medocd ), str_cts_medocd	, DATA_TYPE_STRING );	// CTS_�Ÿű���
	
	//-----------------------------------------------------------
	// ������ ����
	int nRqID = g_iXingAPI.Request( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_DATA ���� �´�.
		szTrNo,						// TR ��ȣ
		&pckInBlock,				// InBlock ������
		sizeof( pckInBlock ),		// InBlock ������ ũ��
		bNext,						// ������ȸ ���� : ������ȸ�� ��쿡 �����Ѵ�.
		szNextKey,					// ������ȸ Key : //		Header Type�� B �� ��쿣 ���� ��ȸ�� ���� Next Key�� �־��ش�.
		30							// Timeout(��) : �ش� �ð�(��)���� �����Ͱ� ���� ������ Timeout�� �߻��Ѵ�. XM_TIMEOUT_DATA �޽����� �߻��Ѵ�.
	);
	
	if( bNext == FALSE )
	{
		m_ctrlOutBlock.DeleteAllItems();
		m_ctrlOutBlock1.DeleteAllItems();
	}

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
LRESULT CDlg_t0433::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data�� ����
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;

		typedef struct  
		{
			t0433OutBlock	outBlock;
			char			sOutBlock1_Count[5];
			t0433OutBlock1	outBlock1[1];
		} t0433AllOutBlock, *LPt0433AllOutBlock;

		int nDataLength = pRpData->nDataLength;
		char szCount[6] = { 0 };

		LPt0433AllOutBlock pAllOutBlock = (LPt0433AllOutBlock)pRpData->lpData;

		// OutBlock ó��
		if( sizeof( pAllOutBlock->outBlock ) > nDataLength )
		{
			return 0L;
		}

		m_ctrlOutBlock.InsertItem( 0, "" );
		m_ctrlOutBlock.SetItemText(  0, 0, GetDispData( pAllOutBlock->outBlock.tdtsunik   , sizeof( pAllOutBlock->outBlock.tdtsunik    ), DATA_TYPE_LONG   ) );		// �Ÿż����հ�  
		m_ctrlOutBlock.SetItemText(  0, 1, GetDispData( pAllOutBlock->outBlock.tmamt      , sizeof( pAllOutBlock->outBlock.tmamt       ), DATA_TYPE_LONG   ) );		// �̰��������հ�
		m_ctrlOutBlock.SetItemText(  0, 2, GetDispData( pAllOutBlock->outBlock.cts_expcode, sizeof( pAllOutBlock->outBlock.cts_expcode ), DATA_TYPE_STRING ) );		// CTS_�����ȣ  
		m_ctrlOutBlock.SetItemText(  0, 3, GetDispData( pAllOutBlock->outBlock.cts_medocd , sizeof( pAllOutBlock->outBlock.cts_medocd  ), DATA_TYPE_STRING ) );		// CTS_�Ÿű���  

		nDataLength -= sizeof( pAllOutBlock->outBlock );

		// OutBlock1 �� Count ó��
		if( sizeof( pAllOutBlock->sOutBlock1_Count ) > nDataLength )
		{
			return 0L;
		}

		ZeroMemory( szCount, sizeof( szCount ) );
		CopyMemory( szCount, pAllOutBlock->sOutBlock1_Count, sizeof( pAllOutBlock->sOutBlock1_Count ) );
		int nOutBlock1Count = atoi( szCount );

		// OutBlock1 �� ó��
		for( int i=0; i<nOutBlock1Count; i++ )
		{
			m_ctrlOutBlock1.InsertItem( i, "" );
			m_ctrlOutBlock1.SetItemText( i,  0, GetDispData( pAllOutBlock->outBlock1[i].expcode   , sizeof( pAllOutBlock->outBlock1[i].expcode       ), DATA_TYPE_STRING    ) );		// �����ȣ    
			m_ctrlOutBlock1.SetItemText( i,  1, GetDispData( pAllOutBlock->outBlock1[i].medosu    , sizeof( pAllOutBlock->outBlock1[i].medosu        ), DATA_TYPE_STRING    ) );		// ����        
			m_ctrlOutBlock1.SetItemText( i,  2, GetDispData( pAllOutBlock->outBlock1[i].jqty      , sizeof( pAllOutBlock->outBlock1[i].jqty          ), DATA_TYPE_LONG      ) );		// �ܰ����    
			m_ctrlOutBlock1.SetItemText( i,  3, GetDispData( pAllOutBlock->outBlock1[i].pamt      , sizeof( pAllOutBlock->outBlock1[i].pamt          ), DATA_TYPE_FLOAT , 2 ) );		// ��մܰ�    
			m_ctrlOutBlock1.SetItemText( i,  4, GetDispData( pAllOutBlock->outBlock1[i].medocd    , sizeof( pAllOutBlock->outBlock1[i].medocd        ), DATA_TYPE_STRING    ) );		// �Ÿű���    
			m_ctrlOutBlock1.SetItemText( i,  5, GetDispData( pAllOutBlock->outBlock1[i].mamt      , sizeof( pAllOutBlock->outBlock1[i].mamt          ), DATA_TYPE_LONG      ) );		// �Ѹ��Աݾ�  
			m_ctrlOutBlock1.SetItemText( i,  6, GetDispData( pAllOutBlock->outBlock1[i].sysprocseq, sizeof( pAllOutBlock->outBlock1[i].sysprocseq    ), DATA_TYPE_LONG      ) );		// ó������    
		}
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
LRESULT CDlg_t0433::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}
