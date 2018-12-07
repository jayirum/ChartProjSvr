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
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_t1301::InitCtrls()
{
	//-----------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "필드명", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );

	// List Control에 OutBlock 필드명을 넣어준다.
	m_ctrlOutBlock.InsertItem(  0, "시간CTS" );
	
	//-----------------------------------------------------------
	// OutBlock1
	m_ctrlOutBlock1.InsertColumn(  0, "시간        ", LVCFMT_LEFT  , 50 );
	m_ctrlOutBlock1.InsertColumn(  1, "현재가      ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  2, "전일대비구분", LVCFMT_LEFT  , 50 );
	m_ctrlOutBlock1.InsertColumn(  3, "전일대비    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  4, "등락율      ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  5, "체결수량    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  6, "체결강도    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  7, "거래량      ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  8, "매도체결수량", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn(  9, "매도체결건수", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 10, "매수체결수량", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 11, "매수체결건수", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 12, "순체결량    ", LVCFMT_RIGHT , 50 );
	m_ctrlOutBlock1.InsertColumn( 13, "순체결건수  ", LVCFMT_RIGHT , 50 );
}

//--------------------------------------------------------------------------------------
// 데이터 조회
//--------------------------------------------------------------------------------------
void CDlg_t1301::RequestData( BOOL bNext )
{
	//-----------------------------------------------------------
	// 주식현재가호가조회(t1101) ( attr,block,headtype=A )
	t1301InBlock	pckInBlock;

	TCHAR			szTrNo[]	= "t1301";
	char			szNextKey[]	= "";

	//-----------------------------------------------------------
	// 데이터 취득
	CString str_shcode   ;		GetDlgItemText( IDC_EDIT_INBLOCK_SHCODE   , str_shcode    );		// 단축코드   
	CString str_cvolume  ;		GetDlgItemText( IDC_EDIT_INBLOCK_CVOLUME  , str_cvolume   );		// 특이거래량 
	CString str_starttime;		GetDlgItemText( IDC_EDIT_INBLOCK_STARTTIME, str_starttime );		// 시작시간   
	CString str_endtime  ;		GetDlgItemText( IDC_EDIT_INBLOCK_ENDTIME  , str_endtime   );		// 종료시간   
	CString str_cts_time ;		GetDlgItemText( IDC_EDIT_INBLOCK_CTS_TIME , str_cts_time  );		// 시간CTS    

	// 다음 조회일경우에 다음 조회시 사용하는 Field는 데이터를 넣으면 안된다.
	if( bNext == FALSE )
	{
		str_cts_time = "";
	}
	
	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode   , sizeof( pckInBlock.shcode    ), str_shcode   , DATA_TYPE_STRING );				// 단축코드   
	SetPacketData( pckInBlock.cvolume  , sizeof( pckInBlock.cvolume   ), str_cvolume  , DATA_TYPE_LONG   );				// 특이거래량 
	SetPacketData( pckInBlock.starttime, sizeof( pckInBlock.starttime ), str_starttime, DATA_TYPE_STRING );				// 시작시간   
	SetPacketData( pckInBlock.endtime  , sizeof( pckInBlock.endtime   ), str_endtime  , DATA_TYPE_STRING );				// 종료시간   
	SetPacketData( pckInBlock.cts_time , sizeof( pckInBlock.cts_time  ), str_cts_time , DATA_TYPE_STRING );				// 시간CTS    
	
	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		30							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);
	
	if( bNext == FALSE )
	{
		m_ctrlOutBlock1.DeleteAllItems();
	}

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		MessageBox( "조회실패", "에러", MB_ICONSTOP );
	}
}

//--------------------------------------------------------------------------------------
// 데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CDlg_t1301::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data를 받음
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;

		// Header가 A Type 일 경우에는 Block 명이 들어온다.
		if( strcmp( pRpData->szBlockName, NAME_t1301OutBlock ) == 0 )
		{
			LPt1301OutBlock pOutBlock = (LPt1301OutBlock)pRpData->lpData;
			m_ctrlOutBlock.SetItemText(  0, 1, GetDispData( pOutBlock->cts_time , sizeof( pOutBlock->cts_time      ), DATA_TYPE_STRING ) );
		}
		else if( strcmp( pRpData->szBlockName, NAME_t1301OutBlock1 ) == 0 )
		{
			LPt1301OutBlock1 pOutBlock1 = (LPt1301OutBlock1)pRpData->lpData;

			// Occurs 일 경우
			// Header가 'A' 이면 전체길이에서 OutBlock의 길이를 나눠서 갯수를 구한다.
			int nCount = pRpData->nDataLength / sizeof( t1301OutBlock1 );

			for( int i=0; i<nCount; i++ )
			{
				m_ctrlOutBlock1.InsertItem( i, "" );
				m_ctrlOutBlock1.SetItemText( i,  0, GetDispData( pOutBlock1[i].chetime  , sizeof( pOutBlock1[i].chetime       ), DATA_TYPE_STRING ) );		// 시간        
				m_ctrlOutBlock1.SetItemText( i,  1, GetDispData( pOutBlock1[i].price    , sizeof( pOutBlock1[i].price         ), DATA_TYPE_LONG   ) );		// 현재가      
				m_ctrlOutBlock1.SetItemText( i,  2, GetDispData( pOutBlock1[i].sign     , sizeof( pOutBlock1[i].sign          ), DATA_TYPE_STRING ) );		// 전일대비구분
				m_ctrlOutBlock1.SetItemText( i,  3, GetDispData( pOutBlock1[i].change   , sizeof( pOutBlock1[i].change        ), DATA_TYPE_LONG   ) );		// 전일대비    
				m_ctrlOutBlock1.SetItemText( i,  4, GetDispData( pOutBlock1[i].diff     , sizeof( pOutBlock1[i].diff          ), DATA_TYPE_FLOAT  , 2 ) );	// 등락율      
				m_ctrlOutBlock1.SetItemText( i,  5, GetDispData( pOutBlock1[i].cvolume  , sizeof( pOutBlock1[i].cvolume       ), DATA_TYPE_LONG   ) );		// 체결수량    
				m_ctrlOutBlock1.SetItemText( i,  6, GetDispData( pOutBlock1[i].chdegree , sizeof( pOutBlock1[i].chdegree      ), DATA_TYPE_FLOAT  , 2 ) );	// 체결강도    
				m_ctrlOutBlock1.SetItemText( i,  7, GetDispData( pOutBlock1[i].volume   , sizeof( pOutBlock1[i].volume        ), DATA_TYPE_LONG   ) );		// 거래량      
				m_ctrlOutBlock1.SetItemText( i,  8, GetDispData( pOutBlock1[i].mdvolume , sizeof( pOutBlock1[i].mdvolume      ), DATA_TYPE_LONG   ) );		// 매도체결수량
				m_ctrlOutBlock1.SetItemText( i,  9, GetDispData( pOutBlock1[i].mdchecnt , sizeof( pOutBlock1[i].mdchecnt      ), DATA_TYPE_LONG   ) );		// 매도체결건수
				m_ctrlOutBlock1.SetItemText( i, 10, GetDispData( pOutBlock1[i].msvolume , sizeof( pOutBlock1[i].msvolume      ), DATA_TYPE_LONG   ) );		// 매수체결수량
				m_ctrlOutBlock1.SetItemText( i, 11, GetDispData( pOutBlock1[i].mschecnt , sizeof( pOutBlock1[i].mschecnt      ), DATA_TYPE_LONG   ) );		// 매수체결건수
				m_ctrlOutBlock1.SetItemText( i, 12, GetDispData( pOutBlock1[i].revolume , sizeof( pOutBlock1[i].revolume      ), DATA_TYPE_LONG   ) );		// 순체결량    
				m_ctrlOutBlock1.SetItemText( i, 13, GetDispData( pOutBlock1[i].rechecnt , sizeof( pOutBlock1[i].rechecnt      ), DATA_TYPE_LONG   ) );		// 순체결건수  
			}
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
LRESULT CDlg_t1301::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}