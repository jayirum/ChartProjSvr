// Dlg_BM_.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_BM_.h"

#include "../../include/API_ETK/BM_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_BM_ dialog

IMPLEMENT_DYNCREATE(CDlg_BM_, CDialog)

CDlg_BM_::CDlg_BM_(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_BM_::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_BM_)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_BM_::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_BM_)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_BM_, CDialog)
	//{{AFX_MSG_MAP(CDlg_BM_)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_BM_ message handlers

BOOL CDlg_BM_::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_BM_::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

void CDlg_BM_::OnButtonRequest() 
{
	UnadviseData();
	AdviseData();
}


//--------------------------------------------------------------------------------------
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_BM_::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "필드  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드              " );
	m_ctrlOutBlock.InsertItem( nRow++, "수신시간                " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량             " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량             " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수           " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수 직전대비  " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금           " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금           " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수         " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수 직전대비" );
	m_ctrlOutBlock.InsertItem( nRow++, "업종코드                " );
}

//--------------------------------------------------------------------------------------
// 데이터 Advise
//--------------------------------------------------------------------------------------
void CDlg_BM_::AdviseData()
{
	TCHAR	szTrCode[]		= "BM_";

	//-----------------------------------------------------------
	// 데이터 취득
    CString strCode; GetDlgItemText( IDC_EDIT_CODE, strCode );

	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		strCode,					// 종목코드
		sizeof( BM__InBlock )		// 종목코드 길이
		);

	//-----------------------------------------------------------
	// 에러체크
	if( bSuccess == FALSE )
	{
		MessageBox( "조회실패", "에러", MB_ICONSTOP );
	}
	else
	{
		m_strCode = strCode;
	}
}

//--------------------------------------------------------------------------------------
// 데이터 Unadvise
//--------------------------------------------------------------------------------------
void CDlg_BM_::UnadviseData()
{
	//-----------------------------------------------------------
	// 이전에 등록한 종목이 없다면 Return한다.
	if( m_strCode.IsEmpty() )
	{
		return;
	}

	TCHAR	szTrCode[]		= "BM_";
	
	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		m_strCode,					// 종목코드
		sizeof( BM__InBlock )		// 종목코드 길이
		);
	
	//-----------------------------------------------------------
	// 에러체크
	if( bSuccess == FALSE )
	{
		MessageBox( "조회실패", "에러", MB_ICONSTOP );
	}
	else
	{
		m_strCode.Empty();
	}
}

//--------------------------------------------------------------------------------------
// 데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CDlg_BM_::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "BM_" ) == 0 )
	{
		LPBM__OutBlock pOutBlock = (LPBM__OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode , sizeof( pOutBlock->tjjcode  ), DATA_TYPE_STRING ) );    // 투자자코드              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjtime , sizeof( pOutBlock->tjjtime  ), DATA_TYPE_STRING ) );    // 수신시간                
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume, sizeof( pOutBlock->msvolume ), DATA_TYPE_LONG   ) );    // 매수 거래량             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume, sizeof( pOutBlock->mdvolume ), DATA_TYPE_LONG   ) );    // 매도 거래량             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol   , sizeof( pOutBlock->msvol    ), DATA_TYPE_LONG   ) );    // 거래량 순매수           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->p_msvol , sizeof( pOutBlock->p_msvol  ), DATA_TYPE_LONG   ) );    // 거래량 순매수 직전대비  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue , sizeof( pOutBlock->msvalue  ), DATA_TYPE_LONG   ) );    // 매수 거래대금           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue , sizeof( pOutBlock->mdvalue  ), DATA_TYPE_LONG   ) );    // 매도 거래대금           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval   , sizeof( pOutBlock->msval    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->p_msval , sizeof( pOutBlock->p_msval  ), DATA_TYPE_LONG   ) );    // 거래대금 순매수 직전대비
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->upcode  , sizeof( pOutBlock->upcode   ), DATA_TYPE_STRING ) );    // 업종코드                
	}

	return 0L;
}
