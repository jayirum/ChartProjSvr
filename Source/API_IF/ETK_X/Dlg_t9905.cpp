// Dlg_t9905.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t9905.h"

#include "../../include/API_ETK/t9905.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t9905 dialog

IMPLEMENT_DYNCREATE(CDlg_t9905, CDialog)

CDlg_t9905::CDlg_t9905(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t9905::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t9905)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t9905::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t9905)
	DDX_Control(pDX, IDC_LIST_t9905, m_lst_t9905);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_t9905, CDialog)
	//{{AFX_MSG_MAP(CDlg_t9905)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t9905 message handlers

BOOL CDlg_t9905::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlg_t9905::OnOK() 
{
	// TODO: Add extra validation here
	
	RequestData( FALSE );

	//CDialog::OnOK();
}

void CDlg_t9905::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


//--------------------------------------------------------------------------------------
// 데이터 조회
//--------------------------------------------------------------------------------------
void CDlg_t9905::RequestData( BOOL bNext )
{

	t9905InBlock pckInBlock;

	TCHAR			szTrNo[]	= "t9905";
	char			szNextKey[]	= "";

	CString sDummy = "";
	SetPacketData( pckInBlock.dummy , sizeof( pckInBlock.dummy ), sDummy	, DATA_TYPE_STRING );	// CTS_매매구분


	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : //		Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		30							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
		);
	
	if( bNext == FALSE )
	{
		//m_ctrlOutBlock.DeleteAllItems();
		//m_ctrlOutBlock1.DeleteAllItems();
	}
	
	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		MessageBox( "t9905 조회실패", "에러", MB_ICONSTOP );
	}

}


//--------------------------------------------------------------------------------------
// 데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CDlg_t9905::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	char msg[128];

	//-------------------------------------------------------------------------------------
	// Data를 받음
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;

		int nBlocSize	= sizeof(t9905OutBlock1);
		int nDataLength = pRpData->nDataLength;
		int nCnt		= nDataLength / nBlocSize;

		// OutBlock1 의 처리
		t9905OutBlock1* pArray;
		
		sprintf(msg, "총 count:%d", nCnt);
		m_lst_t9905.InsertString(0, msg);
		m_lst_t9905.InsertString(0, (char*)pRpData->lpData);
		for( int i=0; i<nCnt; i++ )
		{
			pArray = (t9905OutBlock1*) (pRpData->lpData+( nBlocSize*i) );

			sprintf( msg, "[shcode:%.*s]  [expcode:%.*s]  [hname:%.*s]"
						, sizeof(pArray->shcode), pArray->shcode 
						, sizeof(pArray->expcode), pArray->expcode
						, sizeof(pArray->hname), pArray->hname
						);

			m_lst_t9905.InsertString(0, msg);

		}
	}
	
	//-------------------------------------------------------------------------------------
	// 메시지를 받음
	else if( wParam == MESSAGE_DATA )
	{
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;

		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		
		
		g_iXingAPI.ReleaseMessageData( lParam );
	}
	
	//-------------------------------------------------------------------------------------
	// System Error를 받음
	else if( wParam == SYSTEM_ERROR_DATA )
	{
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		

		g_iXingAPI.ReleaseMessageData( lParam );
	}

	//-------------------------------------------------------------------------------------
	// Release Data를 받음
	else if( wParam == RELEASE_DATA )
	{
		g_iXingAPI.ReleaseRequestData( (int)lParam );
	}

	return 0L;
}

//--------------------------------------------------------------------------------------
// Timeout 발생
//--------------------------------------------------------------------------------------
LRESULT CDlg_t9905::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}
