// Dlg_t9907.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t9907.h"

#include "../../include/API_ETK/t9907.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t9907 dialog

IMPLEMENT_DYNCREATE(CDlg_t9907, CDialog)

CDlg_t9907::CDlg_t9907(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t9907::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t9907)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t9907::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t9907)
	DDX_Control(pDX, IDC_LIST_t9907, m_lst_t9907);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_t9907, CDialog)
	//{{AFX_MSG_MAP(CDlg_t9907)
	//}}AFX_MSG_MAP
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t9907 message handlers

BOOL CDlg_t9907::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlg_t9907::OnOK() 
{
	// TODO: Add extra validation here
	
	RequestData( FALSE );

	//CDialog::OnOK();
}

void CDlg_t9907::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


//--------------------------------------------------------------------------------------
// ������ ��ȸ
//--------------------------------------------------------------------------------------
void CDlg_t9907::RequestData( BOOL bNext )
{

	t9907InBlock pckInBlock;

	TCHAR			szTrNo[]	= "t9907";
	char			szNextKey[]	= "";

	CString sDummy = "";
	SetPacketData( pckInBlock.dummy , sizeof( pckInBlock.dummy ), sDummy	, DATA_TYPE_STRING );	// CTS_�Ÿű���


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
		//m_ctrlOutBlock.DeleteAllItems();
		//m_ctrlOutBlock1.DeleteAllItems();
	}
	
	//-----------------------------------------------------------
	// Request ID�� 0���� ���� ��쿡�� �����̴�.
	if( nRqID < 0 )
	{
		MessageBox( "t9907 ��ȸ����", "����", MB_ICONSTOP );
	}

}


//--------------------------------------------------------------------------------------
// �����͸� ����
//--------------------------------------------------------------------------------------
LRESULT CDlg_t9907::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	char msg[128];

	//-------------------------------------------------------------------------------------
	// Data�� ����
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;

		int nBlocSize	= sizeof(t9907OutBlock1);
		int nDataLength = pRpData->nDataLength;
		int nCnt		= nDataLength / nBlocSize;

		// OutBlock1 �� ó��
		t9907OutBlock1* pArray;
		
		sprintf(msg, "�� count:%d", nCnt);
		m_lst_t9907.InsertString(0, msg);

		m_lst_t9907.InsertString(0, (char*)pRpData->lpData);
		for( int i=0; i<nCnt; i++ )
		{
			pArray = (t9907OutBlock1*) (pRpData->lpData+( nBlocSize*i) );

			sprintf( msg, "[lastym:%.*s]  [lastNM:%.*s]"
						, sizeof(pArray->lastym), pArray->lastym 
						, sizeof(pArray->lastnm), pArray->lastnm );

			m_lst_t9907.InsertString(0, msg);

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
LRESULT CDlg_t9907::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}
