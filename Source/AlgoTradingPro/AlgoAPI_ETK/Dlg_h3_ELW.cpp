// Dlg_h3_ELW.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_h3_ELW.h"

#include "../../include/API_ETK/h3_4ELW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_h3_ELW dialog

IMPLEMENT_DYNCREATE(CDlg_h3_ELW, CDialog)

CDlg_h3_ELW::CDlg_h3_ELW(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_h3_ELW::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_h3_ELW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_h3_ELW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_h3_ELW)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_h3_ELW, CDialog)
	//{{AFX_MSG_MAP(CDlg_h3_ELW)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_h3_ELW message handlers

BOOL CDlg_h3_ELW::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_h3_ELW::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

void CDlg_h3_ELW::OnButtonRequest() 
{
	UnadviseData();
	AdviseData();
}


//--------------------------------------------------------------------------------------
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_h3_ELW::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "필드  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "호가시간        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가1       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가1       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량1   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량1   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량1 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량1 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가2       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가2       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량2   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량2   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량2 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량2 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가3       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가3       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량3   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량3   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량3 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량3 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가4       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가4       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량4   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량4   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량4 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량4 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가5       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가5       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량5   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량5   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량5 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량5 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가6       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가6       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량6   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량6   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량6 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량6 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가7       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가7       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량7   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량7   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량7 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량7 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가8       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가8       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량8   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량8   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량8 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량8 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가9       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가9       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량9   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량9   " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량9 " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량9 " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가10      " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가10      " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도호가잔량10  " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수호가잔량10  " );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매도호가수량10" );
	m_ctrlOutBlock.InsertItem( nRow++, "LP매수호가수량10" );
	m_ctrlOutBlock.InsertItem( nRow++, "총매도호가잔량  " );
	m_ctrlOutBlock.InsertItem( nRow++, "총매수호가잔량  " );
	m_ctrlOutBlock.InsertItem( nRow++, "동시호가구분    " );
	m_ctrlOutBlock.InsertItem( nRow++, "스프레드        " );
	m_ctrlOutBlock.InsertItem( nRow++, "단축코드        " );
}

//--------------------------------------------------------------------------------------
// 데이터 Advise
//--------------------------------------------------------------------------------------
void CDlg_h3_ELW::AdviseData()
{
	TCHAR	szTrCode[]		= "h3_";

	//-----------------------------------------------------------
	// 데이터 취득
    CString strCode; GetDlgItemText( IDC_EDIT_CODE, strCode );

	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		strCode,					// 종목코드
		sizeof( h3__InBlock )		// 종목코드 길이
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
void CDlg_h3_ELW::UnadviseData()
{
	//-----------------------------------------------------------
	// 이전에 등록한 종목이 없다면 Return한다.
	if( m_strCode.IsEmpty() )
	{
		return;
	}

	TCHAR	szTrCode[]		= "h3_";
	
	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		m_strCode,					// 종목코드
		sizeof( h3__InBlock )		// 종목코드 길이
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
LRESULT CDlg_h3_ELW::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "h3_" ) == 0 )
	{
		LPh3__OutBlock pOutBlock = (LPh3__OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->hotime      , sizeof( pOutBlock->hotime       ), DATA_TYPE_STRING ) );    // 호가시간        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho1    , sizeof( pOutBlock->offerho1     ), DATA_TYPE_LONG   ) );    // 매도호가1       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho1      , sizeof( pOutBlock->bidho1       ), DATA_TYPE_LONG   ) );    // 매수호가1       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem1   , sizeof( pOutBlock->offerrem1    ), DATA_TYPE_LONG   ) );    // 매도호가잔량1   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem1     , sizeof( pOutBlock->bidrem1      ), DATA_TYPE_LONG   ) );    // 매수호가잔량1   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho1 , sizeof( pOutBlock->lp_offerho1  ), DATA_TYPE_LONG   ) );    // LP매도호가수량1 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho1   , sizeof( pOutBlock->lp_bidho1    ), DATA_TYPE_LONG   ) );    // LP매수호가수량1 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho2    , sizeof( pOutBlock->offerho2     ), DATA_TYPE_LONG   ) );    // 매도호가2       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho2      , sizeof( pOutBlock->bidho2       ), DATA_TYPE_LONG   ) );    // 매수호가2       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem2   , sizeof( pOutBlock->offerrem2    ), DATA_TYPE_LONG   ) );    // 매도호가잔량2   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem2     , sizeof( pOutBlock->bidrem2      ), DATA_TYPE_LONG   ) );    // 매수호가잔량2   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho2 , sizeof( pOutBlock->lp_offerho2  ), DATA_TYPE_LONG   ) );    // LP매도호가수량2 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho2   , sizeof( pOutBlock->lp_bidho2    ), DATA_TYPE_LONG   ) );    // LP매수호가수량2 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho3    , sizeof( pOutBlock->offerho3     ), DATA_TYPE_LONG   ) );    // 매도호가3       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho3      , sizeof( pOutBlock->bidho3       ), DATA_TYPE_LONG   ) );    // 매수호가3       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem3   , sizeof( pOutBlock->offerrem3    ), DATA_TYPE_LONG   ) );    // 매도호가잔량3   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem3     , sizeof( pOutBlock->bidrem3      ), DATA_TYPE_LONG   ) );    // 매수호가잔량3   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho3 , sizeof( pOutBlock->lp_offerho3  ), DATA_TYPE_LONG   ) );    // LP매도호가수량3 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho3   , sizeof( pOutBlock->lp_bidho3    ), DATA_TYPE_LONG   ) );    // LP매수호가수량3 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho4    , sizeof( pOutBlock->offerho4     ), DATA_TYPE_LONG   ) );    // 매도호가4       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho4      , sizeof( pOutBlock->bidho4       ), DATA_TYPE_LONG   ) );    // 매수호가4       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem4   , sizeof( pOutBlock->offerrem4    ), DATA_TYPE_LONG   ) );    // 매도호가잔량4   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem4     , sizeof( pOutBlock->bidrem4      ), DATA_TYPE_LONG   ) );    // 매수호가잔량4   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho4 , sizeof( pOutBlock->lp_offerho4  ), DATA_TYPE_LONG   ) );    // LP매도호가수량4 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho4   , sizeof( pOutBlock->lp_bidho4    ), DATA_TYPE_LONG   ) );    // LP매수호가수량4 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho5    , sizeof( pOutBlock->offerho5     ), DATA_TYPE_LONG   ) );    // 매도호가5       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho5      , sizeof( pOutBlock->bidho5       ), DATA_TYPE_LONG   ) );    // 매수호가5       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem5   , sizeof( pOutBlock->offerrem5    ), DATA_TYPE_LONG   ) );    // 매도호가잔량5   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem5     , sizeof( pOutBlock->bidrem5      ), DATA_TYPE_LONG   ) );    // 매수호가잔량5   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho5 , sizeof( pOutBlock->lp_offerho5  ), DATA_TYPE_LONG   ) );    // LP매도호가수량5 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho5   , sizeof( pOutBlock->lp_bidho5    ), DATA_TYPE_LONG   ) );    // LP매수호가수량5 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho6    , sizeof( pOutBlock->offerho6     ), DATA_TYPE_LONG   ) );    // 매도호가6       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho6      , sizeof( pOutBlock->bidho6       ), DATA_TYPE_LONG   ) );    // 매수호가6       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem6   , sizeof( pOutBlock->offerrem6    ), DATA_TYPE_LONG   ) );    // 매도호가잔량6   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem6     , sizeof( pOutBlock->bidrem6      ), DATA_TYPE_LONG   ) );    // 매수호가잔량6   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho6 , sizeof( pOutBlock->lp_offerho6  ), DATA_TYPE_LONG   ) );    // LP매도호가수량6 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho6   , sizeof( pOutBlock->lp_bidho6    ), DATA_TYPE_LONG   ) );    // LP매수호가수량6 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho7    , sizeof( pOutBlock->offerho7     ), DATA_TYPE_LONG   ) );    // 매도호가7       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho7      , sizeof( pOutBlock->bidho7       ), DATA_TYPE_LONG   ) );    // 매수호가7       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem7   , sizeof( pOutBlock->offerrem7    ), DATA_TYPE_LONG   ) );    // 매도호가잔량7   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem7     , sizeof( pOutBlock->bidrem7      ), DATA_TYPE_LONG   ) );    // 매수호가잔량7   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho7 , sizeof( pOutBlock->lp_offerho7  ), DATA_TYPE_LONG   ) );    // LP매도호가수량7 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho7   , sizeof( pOutBlock->lp_bidho7    ), DATA_TYPE_LONG   ) );    // LP매수호가수량7 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho8    , sizeof( pOutBlock->offerho8     ), DATA_TYPE_LONG   ) );    // 매도호가8       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho8      , sizeof( pOutBlock->bidho8       ), DATA_TYPE_LONG   ) );    // 매수호가8       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem8   , sizeof( pOutBlock->offerrem8    ), DATA_TYPE_LONG   ) );    // 매도호가잔량8   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem8     , sizeof( pOutBlock->bidrem8      ), DATA_TYPE_LONG   ) );    // 매수호가잔량8   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho8 , sizeof( pOutBlock->lp_offerho8  ), DATA_TYPE_LONG   ) );    // LP매도호가수량8 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho8   , sizeof( pOutBlock->lp_bidho8    ), DATA_TYPE_LONG   ) );    // LP매수호가수량8 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho9    , sizeof( pOutBlock->offerho9     ), DATA_TYPE_LONG   ) );    // 매도호가9       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho9      , sizeof( pOutBlock->bidho9       ), DATA_TYPE_LONG   ) );    // 매수호가9       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem9   , sizeof( pOutBlock->offerrem9    ), DATA_TYPE_LONG   ) );    // 매도호가잔량9   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem9     , sizeof( pOutBlock->bidrem9      ), DATA_TYPE_LONG   ) );    // 매수호가잔량9   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho9 , sizeof( pOutBlock->lp_offerho9  ), DATA_TYPE_LONG   ) );    // LP매도호가수량9 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho9   , sizeof( pOutBlock->lp_bidho9    ), DATA_TYPE_LONG   ) );    // LP매수호가수량9 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerho10   , sizeof( pOutBlock->offerho10    ), DATA_TYPE_LONG   ) );    // 매도호가10      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidho10     , sizeof( pOutBlock->bidho10      ), DATA_TYPE_LONG   ) );    // 매수호가10      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offerrem10  , sizeof( pOutBlock->offerrem10   ), DATA_TYPE_LONG   ) );    // 매도호가잔량10  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bidrem10    , sizeof( pOutBlock->bidrem10     ), DATA_TYPE_LONG   ) );    // 매수호가잔량10  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_offerho10, sizeof( pOutBlock->lp_offerho10 ), DATA_TYPE_LONG   ) );    // LP매도호가수량10
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lp_bidho10  , sizeof( pOutBlock->lp_bidho10   ), DATA_TYPE_LONG   ) );    // LP매수호가수량10
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totofferrem , sizeof( pOutBlock->totofferrem  ), DATA_TYPE_LONG   ) );    // 총매도호가잔량  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->totbidrem   , sizeof( pOutBlock->totbidrem    ), DATA_TYPE_LONG   ) );    // 총매수호가잔량  
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->donsigubun  , sizeof( pOutBlock->donsigubun   ), DATA_TYPE_STRING ) );    // 동시호가구분    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spread      , sizeof( pOutBlock->spread       ), DATA_TYPE_FLOAT , 2 ) );    // 스프레드        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->shcode      , sizeof( pOutBlock->shcode       ), DATA_TYPE_STRING ) );    // 단축코드        
	}

	return 0L;
}
