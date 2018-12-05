// Dlg_t0431.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t0431.h"

#include "../../include/API_ETK/t0431.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t0431 dialog

IMPLEMENT_DYNCREATE(CDlg_t0431, CDialog)

CDlg_t0431::CDlg_t0431(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t0431::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t0431)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t0431::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t0431)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
	DDX_Control(pDX, IDC_LIST_OUTBLOCK1, m_ctrlOutBlock1);
}


BEGIN_MESSAGE_MAP(CDlg_t0431, CDialog)
	//{{AFX_MSG_MAP(CDlg_t0431)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,		  OnButtonRequest	  )
	ON_BN_CLICKED( IDC_BUTTON_REQUEST_NEXT,	  OnButtonRequestNext )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t0431 message handlers

BOOL CDlg_t0431::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_t0431::OnButtonRequest() 
{
	RequestData( FALSE );
}

void CDlg_t0431::OnButtonRequestNext() 
{
	RequestData( TRUE );	
}

//--------------------------------------------------------------------------------------
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_t0431::InitCtrls()
{
	//-------------------------------------------------------------------------
	// InBlock : 기본값 세팅
	
	// 계좌번호 세팅
	char szAccount[20];
	int nCount = g_iXingAPI.GetAccountListCount();
	for( int i=0; i<nCount; i++ )
	{
		if( g_iXingAPI.GetAccountList( i, szAccount, sizeof( szAccount ) ) == FALSE )
		{
			continue;
		}
		
		// 상품코드를 확인하여 선물옵션계좌(51)만 처리한다.
		if( strncmp( szAccount+3, "51", 2 ) == 0 )
		{
			((CComboBox*)GetDlgItem( IDC_COMBO_INBLOCK_ACCNO ))->AddString( szAccount );
		}
	}
	((CComboBox*)GetDlgItem( IDC_COMBO_INBLOCK_ACCNO       ))->SetCurSel( 0 );	// 계좌번호

	//-----------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "매매손익합계", LVCFMT_LEFT,  100 );
	m_ctrlOutBlock.InsertColumn( 1, "CTS_종목번호", LVCFMT_LEFT,  100 );
	m_ctrlOutBlock.InsertColumn( 2, "CTS_매매구분", LVCFMT_LEFT,  100 );
	m_ctrlOutBlock.InsertColumn( 3, "평가금액    ", LVCFMT_RIGHT, 100 );
	m_ctrlOutBlock.InsertColumn( 4, "평가손익    ", LVCFMT_RIGHT, 100 );
	
	//-----------------------------------------------------------
	// OutBlock1
	m_ctrlOutBlock1.InsertColumn(  0, "종목번호    ", LVCFMT_LEFT  , 100 );
	m_ctrlOutBlock1.InsertColumn(  1, "구분        ", LVCFMT_LEFT  , 100 );
	m_ctrlOutBlock1.InsertColumn(  2, "잔고수량    ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  3, "청산가능수량", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  4, "평균단가    ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  5, "총매입금액  ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  6, "매매구분    ", LVCFMT_LEFT  , 100 );
	m_ctrlOutBlock1.InsertColumn(  7, "매매손익    ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  8, "처리순번    ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn(  9, "현재가      ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn( 10, "평가금액    ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn( 11, "평가손익    ", LVCFMT_RIGHT , 100 );
	m_ctrlOutBlock1.InsertColumn( 12, "수익률      ", LVCFMT_RIGHT , 100 );
}

//--------------------------------------------------------------------------------------
// 데이터 조회
//--------------------------------------------------------------------------------------
void CDlg_t0431::RequestData( BOOL bNext )
{
	//-----------------------------------------------------------
	// 선물/옵션잔고평가(주문팝업)(t0431) ( attr,tuxcode=t0431,headtype=D )
	t0431InBlock	pckInBlock;

	TCHAR			szTrNo[]	= "t0431";
	char			szNextKey[]	= "";

	//-----------------------------------------------------------
	// 데이터 취득
	CString str_accno		; GetDlgItemText( IDC_COMBO_INBLOCK_ACCNO	  , str_accno		);		// 계좌번호
	CString str_passwd		; GetDlgItemText( IDC_EDIT_INBLOCK_PASSWD	  , str_passwd		);		// 비밀번호
	CString str_cts_expcode	; GetDlgItemText( IDC_EDIT_INBLOCK_CTS_EXPCODE, str_cts_expcode );		// CTS_종목번호
	CString str_cts_medocd	; GetDlgItemText( IDC_EDIT_INBLOCK_CTS_MEDOCD , str_cts_medocd  );		// CTS_매매구분

	// 다음 조회일경우에 다음 조회시 사용하는 Field는 데이터를 넣으면 안된다.
	if( bNext == FALSE )
	{
		str_cts_expcode = "";
		str_cts_medocd = "";
	}
	
	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.accno		 , sizeof( pckInBlock.accno		 ), str_accno		, DATA_TYPE_STRING );	// 계좌번호
	SetPacketData( pckInBlock.passwd	 , sizeof( pckInBlock.passwd	 ), str_passwd		, DATA_TYPE_STRING );	// 비밀번호
	SetPacketData( pckInBlock.cts_expcode, sizeof( pckInBlock.cts_expcode), str_cts_expcode , DATA_TYPE_STRING );	// CTS_종목번호
	SetPacketData( pckInBlock.cts_medocd , sizeof( pckInBlock.cts_medocd ), str_cts_medocd	, DATA_TYPE_STRING );	// CTS_매매구분
	
	//-----------------------------------------------------------
	// 데이터 전송
	HWND h = GetSafeHwnd();
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
		m_ctrlOutBlock.DeleteAllItems();
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
LRESULT CDlg_t0431::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data를 받음
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;

		typedef struct  
		{
			t0431OutBlock	outBlock;
			char			sOutBlock1_Count[5];
			t0431OutBlock1	outBlock1[1];
		} t0431AllOutBlock, *LPt0431AllOutBlock;

		int nDataLength = pRpData->nDataLength;
		char szCount[6] = { 0 };

		LPt0431AllOutBlock pAllOutBlock = (LPt0431AllOutBlock)pRpData->lpData;

		// OutBlock 처리
		if( sizeof( pAllOutBlock->outBlock ) > nDataLength )
		{
			return 0L;
		}

		m_ctrlOutBlock.InsertItem( 0, "" );
		m_ctrlOutBlock.SetItemText(  0, 0, GetDispData( pAllOutBlock->outBlock.tdtsunik	  , sizeof( pAllOutBlock->outBlock.tdtsunik	   ), DATA_TYPE_LONG   ) );		// 매매손익합계
		m_ctrlOutBlock.SetItemText(  0, 1, GetDispData( pAllOutBlock->outBlock.cts_expcode, sizeof( pAllOutBlock->outBlock.cts_expcode ), DATA_TYPE_STRING ) );		// CTS_종목번호
		m_ctrlOutBlock.SetItemText(  0, 2, GetDispData( pAllOutBlock->outBlock.cts_medocd , sizeof( pAllOutBlock->outBlock.cts_medocd  ), DATA_TYPE_STRING ) );		// CTS_매매구분
		m_ctrlOutBlock.SetItemText(  0, 3, GetDispData( pAllOutBlock->outBlock.tappamt	  , sizeof( pAllOutBlock->outBlock.tappamt	   ), DATA_TYPE_LONG   ) );		// 평가금액
		m_ctrlOutBlock.SetItemText(  0, 4, GetDispData( pAllOutBlock->outBlock.tsunik	  , sizeof( pAllOutBlock->outBlock.tsunik	   ), DATA_TYPE_LONG   ) );		// 평가손익

		nDataLength -= sizeof( pAllOutBlock->outBlock );

		// OutBlock1 의 Count 처리
		if( sizeof( pAllOutBlock->sOutBlock1_Count ) > nDataLength )
		{
			return 0L;
		}

		ZeroMemory( szCount, sizeof( szCount ) );
		CopyMemory( szCount, pAllOutBlock->sOutBlock1_Count, sizeof( pAllOutBlock->sOutBlock1_Count ) );
		int nOutBlock1Count = atoi( szCount );

		// OutBlock1 의 처리
		for( int i=0; i<nOutBlock1Count; i++ )
		{
			m_ctrlOutBlock1.InsertItem( i, "" );
			m_ctrlOutBlock1.SetItemText( i,  0, GetDispData( pAllOutBlock->outBlock1[i].expcode   , sizeof( pAllOutBlock->outBlock1[i].expcode    ), DATA_TYPE_STRING   ) );		// 종목번호       
			m_ctrlOutBlock1.SetItemText( i,  1, GetDispData( pAllOutBlock->outBlock1[i].medosu    , sizeof( pAllOutBlock->outBlock1[i].medosu     ), DATA_TYPE_STRING   ) );		// 구분           
			m_ctrlOutBlock1.SetItemText( i,  2, GetDispData( pAllOutBlock->outBlock1[i].jqty      , sizeof( pAllOutBlock->outBlock1[i].jqty       ), DATA_TYPE_LONG     ) );		// 잔고수량       
			m_ctrlOutBlock1.SetItemText( i,  3, GetDispData( pAllOutBlock->outBlock1[i].cqty      , sizeof( pAllOutBlock->outBlock1[i].cqty       ), DATA_TYPE_LONG     ) );		// 청산가능수량   
			m_ctrlOutBlock1.SetItemText( i,  4, GetDispData( pAllOutBlock->outBlock1[i].pamt      , sizeof( pAllOutBlock->outBlock1[i].pamt       ), DATA_TYPE_FLOAT, 2 ) );		// 평균단가       
			m_ctrlOutBlock1.SetItemText( i,  5, GetDispData( pAllOutBlock->outBlock1[i].mamt      , sizeof( pAllOutBlock->outBlock1[i].mamt       ), DATA_TYPE_LONG     ) );		// 총매입금액     
			m_ctrlOutBlock1.SetItemText( i,  6, GetDispData( pAllOutBlock->outBlock1[i].medocd    , sizeof( pAllOutBlock->outBlock1[i].medocd     ), DATA_TYPE_STRING   ) );		// 매매구분       
			m_ctrlOutBlock1.SetItemText( i,  7, GetDispData( pAllOutBlock->outBlock1[i].dtsunik   , sizeof( pAllOutBlock->outBlock1[i].dtsunik    ), DATA_TYPE_LONG     ) );		// 매매손익       
			m_ctrlOutBlock1.SetItemText( i,  8, GetDispData( pAllOutBlock->outBlock1[i].sysprocseq, sizeof( pAllOutBlock->outBlock1[i].sysprocseq ), DATA_TYPE_LONG     ) );		// 처리순번       
			m_ctrlOutBlock1.SetItemText( i,  9, GetDispData( pAllOutBlock->outBlock1[i].price     , sizeof( pAllOutBlock->outBlock1[i].price      ), DATA_TYPE_FLOAT, 2 ) );		// 현재가         
			m_ctrlOutBlock1.SetItemText( i, 10, GetDispData( pAllOutBlock->outBlock1[i].appamt    , sizeof( pAllOutBlock->outBlock1[i].appamt     ), DATA_TYPE_LONG     ) );		// 평가금액       
			m_ctrlOutBlock1.SetItemText( i, 11, GetDispData( pAllOutBlock->outBlock1[i].dtsunik1  , sizeof( pAllOutBlock->outBlock1[i].dtsunik1   ), DATA_TYPE_LONG     ) );		// 평가손익       
			m_ctrlOutBlock1.SetItemText( i, 12, GetDispData( pAllOutBlock->outBlock1[i].sunikrt   , sizeof( pAllOutBlock->outBlock1[i].sunikrt    ), DATA_TYPE_FLOAT, 2 ) );		// 수익율         
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
LRESULT CDlg_t0431::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}