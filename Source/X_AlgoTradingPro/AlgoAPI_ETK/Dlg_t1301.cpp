// Dlg_t1301.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t1301.h"

#include "../../include/API_ETK/t1301.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t1301 dialog

IMPLEMENT_DYNCREATE(CDlg_t1301, CDialog)

CDlg_t1301::CDlg_t1301(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t1301::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t1301)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t1301::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t1301)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
	DDX_Control(pDX, IDC_LIST_OUTBLOCK1, m_ctrlOutBlock1);
}


BEGIN_MESSAGE_MAP(CDlg_t1301, CDialog)
	//{{AFX_MSG_MAP(CDlg_t1301)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,		  OnButtonRequest	  )
	ON_BN_CLICKED( IDC_BUTTON_REQUEST_NEXT,	  OnButtonRequestNext )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t1301 message handlers

BOOL CDlg_t1301::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_t1301::OnButtonRequest() 
{
	RequestData( FALSE );
}

void CDlg_t1301::OnButtonRequestNext() 
{
	RequestData( TRUE );	
}

//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_t1301::InitCtrls()
{
	//-----------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "�ʵ��", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );

	// List Control�� OutBlock �ʵ���� �־��ش�.
	m_ctrlOutBlock.InsertItem(  0, "�ð�CTS" );
	
	//-----------------------------------------------------------
	// OutBlock1
	m_ctrlOutBlock1.InsertColumn(  0, "�ð�        ", LVCFMT_LEFT  , 50 );
	m_ctrlOutBlock1.InsertColumn(  1, "���簡      ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  2, "���ϴ�񱸺�", LVCFMT_LEFT  , 50 );
	m_ctrlOutBlock1.InsertColumn(  3, "���ϴ��    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  4, "�����      ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  5, "ü�����    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  6, "ü�ᰭ��    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  7, "�ŷ���      ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  8, "�ŵ�ü�����", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  9, "�ŵ�ü��Ǽ�", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 10, "�ż�ü�����", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 11, "�ż�ü��Ǽ�", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 12, "��ü�ᷮ    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 13, "��ü��Ǽ�  ", LVCFMT_RIGHT , 50 );
}

//--------------------------------------------------------------------------------------
// ������ ��ȸ
//--------------------------------------------------------------------------------------
void CDlg_t1301::RequestData( BOOL bNext )
{
	//-----------------------------------------------------------
	// �ֽ����簡ȣ����ȸ(t1101) ( attr,block,headtype=A )
	t1301InBlock	pckInBlock;

	TCHAR			szTrNo[]	= "t1301";
	char			szNextKey[]	= "";

	//-----------------------------------------------------------
	// ������ ���
	CString str_shcode   ;		GetDlgItemText( IDC_EDIT_INBLOCK_SHCODE   , str_shcode    );		// �����ڵ�   
	CString str_cvolume  ;		GetDlgItemText( IDC_EDIT_INBLOCK_CVOLUME  , str_cvolume   );		// Ư�̰ŷ��� 
	CString str_starttime;		GetDlgItemText( IDC_EDIT_INBLOCK_STARTTIME, str_starttime );		// ���۽ð�   
	CString str_endtime  ;		GetDlgItemText( IDC_EDIT_INBLOCK_ENDTIME  , str_endtime   );		// ����ð�   
	CString str_cts_time ;		GetDlgItemText( IDC_EDIT_INBLOCK_CTS_TIME , str_cts_time  );		// �ð�CTS    

	// ���� ��ȸ�ϰ�쿡 ���� ��ȸ�� ����ϴ� Field�� �����͸� ������ �ȵȴ�.
	if( bNext == FALSE )
	{
		str_cts_time = "";
	}
	
	//-----------------------------------------------------------
	// ������ �ʱ�ȭ
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	
	//-----------------------------------------------------------
	// ������ �Է�
	SetPacketData( pckInBlock.shcode   , sizeof( pckInBlock.shcode    ), str_shcode   , DATA_TYPE_STRING );				// �����ڵ�   
	SetPacketData( pckInBlock.cvolume  , sizeof( pckInBlock.cvolume   ), str_cvolume  , DATA_TYPE_LONG   );				// Ư�̰ŷ��� 
	SetPacketData( pckInBlock.starttime, sizeof( pckInBlock.starttime ), str_starttime, DATA_TYPE_STRING );				// ���۽ð�   
	SetPacketData( pckInBlock.endtime  , sizeof( pckInBlock.endtime   ), str_endtime  , DATA_TYPE_STRING );				// ����ð�   
	SetPacketData( pckInBlock.cts_time , sizeof( pckInBlock.cts_time  ), str_cts_time , DATA_TYPE_STRING );				// �ð�CTS    
	
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
	
	if( bNext == FALSE )
	{
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
LRESULT CDlg_t1301::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data�� ����
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;

		// Header�� A Type �� ��쿡�� Block ���� ���´�.
		if( strcmp( pRpData->szBlockName, NAME_t1301OutBlock ) == 0 )
		{
			LPt1301OutBlock pOutBlock = (LPt1301OutBlock)pRpData->lpData;
			m_ctrlOutBlock.SetItemText(  0, 1, GetDispData( pOutBlock->cts_time , sizeof( pOutBlock->cts_time      ), DATA_TYPE_STRING ) );
		}
		else if( strcmp( pRpData->szBlockName, NAME_t1301OutBlock1 ) == 0 )
		{
			LPt1301OutBlock1 pOutBlock1 = (LPt1301OutBlock1)pRpData->lpData;

			// Occurs �� ���
			// Header�� 'A' �̸� ��ü���̿��� OutBlock�� ���̸� ������ ������ ���Ѵ�.
			int nCount = pRpData->nDataLength / sizeof( t1301OutBlock1 );

			for( int i=0; i<nCount; i++ )
			{
				m_ctrlOutBlock1.InsertItem( i, "" );
				m_ctrlOutBlock1.SetItemText( i,  0, GetDispData( pOutBlock1[i].chetime  , sizeof( pOutBlock1[i].chetime       ), DATA_TYPE_STRING ) );		// �ð�        
				m_ctrlOutBlock1.SetItemText( i,  1, GetDispData( pOutBlock1[i].price    , sizeof( pOutBlock1[i].price         ), DATA_TYPE_LONG   ) );		// ���簡      
				m_ctrlOutBlock1.SetItemText( i,  2, GetDispData( pOutBlock1[i].sign     , sizeof( pOutBlock1[i].sign          ), DATA_TYPE_STRING ) );		// ���ϴ�񱸺�
				m_ctrlOutBlock1.SetItemText( i,  3, GetDispData( pOutBlock1[i].change   , sizeof( pOutBlock1[i].change        ), DATA_TYPE_LONG   ) );		// ���ϴ��    
				m_ctrlOutBlock1.SetItemText( i,  4, GetDispData( pOutBlock1[i].diff     , sizeof( pOutBlock1[i].diff          ), DATA_TYPE_FLOAT  , 2 ) );	// �����      
				m_ctrlOutBlock1.SetItemText( i,  5, GetDispData( pOutBlock1[i].cvolume  , sizeof( pOutBlock1[i].cvolume       ), DATA_TYPE_LONG   ) );		// ü�����    
				m_ctrlOutBlock1.SetItemText( i,  6, GetDispData( pOutBlock1[i].chdegree , sizeof( pOutBlock1[i].chdegree      ), DATA_TYPE_FLOAT  , 2 ) );	// ü�ᰭ��    
				m_ctrlOutBlock1.SetItemText( i,  7, GetDispData( pOutBlock1[i].volume   , sizeof( pOutBlock1[i].volume        ), DATA_TYPE_LONG   ) );		// �ŷ���      
				m_ctrlOutBlock1.SetItemText( i,  8, GetDispData( pOutBlock1[i].mdvolume , sizeof( pOutBlock1[i].mdvolume      ), DATA_TYPE_LONG   ) );		// �ŵ�ü�����
				m_ctrlOutBlock1.SetItemText( i,  9, GetDispData( pOutBlock1[i].mdchecnt , sizeof( pOutBlock1[i].mdchecnt      ), DATA_TYPE_LONG   ) );		// �ŵ�ü��Ǽ�
				m_ctrlOutBlock1.SetItemText( i, 10, GetDispData( pOutBlock1[i].msvolume , sizeof( pOutBlock1[i].msvolume      ), DATA_TYPE_LONG   ) );		// �ż�ü�����
				m_ctrlOutBlock1.SetItemText( i, 11, GetDispData( pOutBlock1[i].mschecnt , sizeof( pOutBlock1[i].mschecnt      ), DATA_TYPE_LONG   ) );		// �ż�ü��Ǽ�
				m_ctrlOutBlock1.SetItemText( i, 12, GetDispData( pOutBlock1[i].revolume , sizeof( pOutBlock1[i].revolume      ), DATA_TYPE_LONG   ) );		// ��ü�ᷮ    
				m_ctrlOutBlock1.SetItemText( i, 13, GetDispData( pOutBlock1[i].rechecnt , sizeof( pOutBlock1[i].rechecnt      ), DATA_TYPE_LONG   ) );		// ��ü��Ǽ�  
			}
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
LRESULT CDlg_t1301::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}