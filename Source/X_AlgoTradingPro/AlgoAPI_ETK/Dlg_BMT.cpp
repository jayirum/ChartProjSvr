// Dlg_BMT.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_BMT.h"

#include "../../include/API_ETK/BMT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_BMT dialog

IMPLEMENT_DYNCREATE(CDlg_BMT, CDialog)

CDlg_BMT::CDlg_BMT(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_BMT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_BMT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_BMT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_BMT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
}


BEGIN_MESSAGE_MAP(CDlg_BMT, CDialog)
	//{{AFX_MSG_MAP(CDlg_BMT)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_BMT message handlers

BOOL CDlg_BMT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_BMT::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

void CDlg_BMT::OnButtonRequest() 
{
	UnadviseData();
	AdviseData();
}


//--------------------------------------------------------------------------------------
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_BMT::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "필드  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "수신시간              " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드1(개인)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량1          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량1          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수1        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금1        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금1        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수1      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드2(외국인)   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량2          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량2          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수2        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금2        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금2        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수2      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드3(기관계)   " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량3          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량3          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수3        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금3        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금3        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수3      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드4(증권)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량4          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량4          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수4        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금4        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금4        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수4      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드5(투신)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량5          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량5          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수5        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금5        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금5        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수5      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드6(은행)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량6          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량6          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수6        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금6        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금6        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수6      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드7(보험)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량7          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량7          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수7        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금7        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금7        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수7      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드8(종금)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량8          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량8          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수8        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금8        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금8        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수8      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드9(기금)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량9          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량9          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수9        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금9        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금9        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수9      " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드10(선물업자)" );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량10         " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량10         " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수10       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금10       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금10       " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수10     " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드11(기타)    " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량11         " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량11         " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수11       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금11       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금11       " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수11     " );
	m_ctrlOutBlock.InsertItem( nRow++, "업종코드              " );
	m_ctrlOutBlock.InsertItem( nRow++, "투자자코드0(사모펀드) " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래량0          " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래량0          " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래량 순매수0        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매수 거래대금0        " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도 거래대금0        " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래대금 순매수0      " );
}

//--------------------------------------------------------------------------------------
// 데이터 Advise
//--------------------------------------------------------------------------------------
void CDlg_BMT::AdviseData()
{
	TCHAR	szTrCode[]		= "BMT";

	//-----------------------------------------------------------
	// 데이터 취득
    CString strCode; GetDlgItemText( IDC_EDIT_CODE, strCode );

	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		strCode,					// 종목코드
		sizeof( BMT_InBlock )		// 종목코드 길이
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
void CDlg_BMT::UnadviseData()
{
	//-----------------------------------------------------------
	// 이전에 등록한 종목이 없다면 Return한다.
	if( m_strCode.IsEmpty() )
	{
		return;
	}

	TCHAR	szTrCode[]		= "BMT";
	
	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		m_strCode,					// 종목코드
		sizeof( BMT_InBlock )		// 종목코드 길이
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
LRESULT CDlg_BMT::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "BMT" ) == 0 )
	{
		LPBMT_OutBlock pOutBlock = (LPBMT_OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjtime  , sizeof( pOutBlock->tjjtime   ), DATA_TYPE_STRING ) );    // 수신시간              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode1 , sizeof( pOutBlock->tjjcode1  ), DATA_TYPE_STRING ) );    // 투자자코드1(개인)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume1, sizeof( pOutBlock->msvolume1 ), DATA_TYPE_LONG   ) );    // 매수 거래량1          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume1, sizeof( pOutBlock->mdvolume1 ), DATA_TYPE_LONG   ) );    // 매도 거래량1          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol1   , sizeof( pOutBlock->msvol1    ), DATA_TYPE_LONG   ) );    // 거래량 순매수1        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue1 , sizeof( pOutBlock->msvalue1  ), DATA_TYPE_LONG   ) );    // 매수 거래대금1        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue1 , sizeof( pOutBlock->mdvalue1  ), DATA_TYPE_LONG   ) );    // 매도 거래대금1        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval1   , sizeof( pOutBlock->msval1    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수1      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode2 , sizeof( pOutBlock->tjjcode2  ), DATA_TYPE_STRING ) );    // 투자자코드2(외국인)   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume2, sizeof( pOutBlock->msvolume2 ), DATA_TYPE_LONG   ) );    // 매수 거래량2          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume2, sizeof( pOutBlock->mdvolume2 ), DATA_TYPE_LONG   ) );    // 매도 거래량2          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol2   , sizeof( pOutBlock->msvol2    ), DATA_TYPE_LONG   ) );    // 거래량 순매수2        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue2 , sizeof( pOutBlock->msvalue2  ), DATA_TYPE_LONG   ) );    // 매수 거래대금2        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue2 , sizeof( pOutBlock->mdvalue2  ), DATA_TYPE_LONG   ) );    // 매도 거래대금2        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval2   , sizeof( pOutBlock->msval2    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수2      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode3 , sizeof( pOutBlock->tjjcode3  ), DATA_TYPE_STRING ) );    // 투자자코드3(기관계)   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume3, sizeof( pOutBlock->msvolume3 ), DATA_TYPE_LONG   ) );    // 매수 거래량3          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume3, sizeof( pOutBlock->mdvolume3 ), DATA_TYPE_LONG   ) );    // 매도 거래량3          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol3   , sizeof( pOutBlock->msvol3    ), DATA_TYPE_LONG   ) );    // 거래량 순매수3        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue3 , sizeof( pOutBlock->msvalue3  ), DATA_TYPE_LONG   ) );    // 매수 거래대금3        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue3 , sizeof( pOutBlock->mdvalue3  ), DATA_TYPE_LONG   ) );    // 매도 거래대금3        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval3   , sizeof( pOutBlock->msval3    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수3      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode4 , sizeof( pOutBlock->tjjcode4  ), DATA_TYPE_STRING ) );    // 투자자코드4(증권)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume4, sizeof( pOutBlock->msvolume4 ), DATA_TYPE_LONG   ) );    // 매수 거래량4          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume4, sizeof( pOutBlock->mdvolume4 ), DATA_TYPE_LONG   ) );    // 매도 거래량4          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol4   , sizeof( pOutBlock->msvol4    ), DATA_TYPE_LONG   ) );    // 거래량 순매수4        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue4 , sizeof( pOutBlock->msvalue4  ), DATA_TYPE_LONG   ) );    // 매수 거래대금4        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue4 , sizeof( pOutBlock->mdvalue4  ), DATA_TYPE_LONG   ) );    // 매도 거래대금4        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval4   , sizeof( pOutBlock->msval4    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수4      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode5 , sizeof( pOutBlock->tjjcode5  ), DATA_TYPE_STRING ) );    // 투자자코드5(투신)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume5, sizeof( pOutBlock->msvolume5 ), DATA_TYPE_LONG   ) );    // 매수 거래량5          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume5, sizeof( pOutBlock->mdvolume5 ), DATA_TYPE_LONG   ) );    // 매도 거래량5          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol5   , sizeof( pOutBlock->msvol5    ), DATA_TYPE_LONG   ) );    // 거래량 순매수5        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue5 , sizeof( pOutBlock->msvalue5  ), DATA_TYPE_LONG   ) );    // 매수 거래대금5        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue5 , sizeof( pOutBlock->mdvalue5  ), DATA_TYPE_LONG   ) );    // 매도 거래대금5        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval5   , sizeof( pOutBlock->msval5    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수5      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode6 , sizeof( pOutBlock->tjjcode6  ), DATA_TYPE_STRING ) );    // 투자자코드6(은행)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume6, sizeof( pOutBlock->msvolume6 ), DATA_TYPE_LONG   ) );    // 매수 거래량6          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume6, sizeof( pOutBlock->mdvolume6 ), DATA_TYPE_LONG   ) );    // 매도 거래량6          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol6   , sizeof( pOutBlock->msvol6    ), DATA_TYPE_LONG   ) );    // 거래량 순매수6        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue6 , sizeof( pOutBlock->msvalue6  ), DATA_TYPE_LONG   ) );    // 매수 거래대금6        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue6 , sizeof( pOutBlock->mdvalue6  ), DATA_TYPE_LONG   ) );    // 매도 거래대금6        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval6   , sizeof( pOutBlock->msval6    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수6      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode7 , sizeof( pOutBlock->tjjcode7  ), DATA_TYPE_STRING ) );    // 투자자코드7(보험)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume7, sizeof( pOutBlock->msvolume7 ), DATA_TYPE_LONG   ) );    // 매수 거래량7          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume7, sizeof( pOutBlock->mdvolume7 ), DATA_TYPE_LONG   ) );    // 매도 거래량7          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol7   , sizeof( pOutBlock->msvol7    ), DATA_TYPE_LONG   ) );    // 거래량 순매수7        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue7 , sizeof( pOutBlock->msvalue7  ), DATA_TYPE_LONG   ) );    // 매수 거래대금7        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue7 , sizeof( pOutBlock->mdvalue7  ), DATA_TYPE_LONG   ) );    // 매도 거래대금7        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval7   , sizeof( pOutBlock->msval7    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수7      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode8 , sizeof( pOutBlock->tjjcode8  ), DATA_TYPE_STRING ) );    // 투자자코드8(종금)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume8, sizeof( pOutBlock->msvolume8 ), DATA_TYPE_LONG   ) );    // 매수 거래량8          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume8, sizeof( pOutBlock->mdvolume8 ), DATA_TYPE_LONG   ) );    // 매도 거래량8          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol8   , sizeof( pOutBlock->msvol8    ), DATA_TYPE_LONG   ) );    // 거래량 순매수8        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue8 , sizeof( pOutBlock->msvalue8  ), DATA_TYPE_LONG   ) );    // 매수 거래대금8        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue8 , sizeof( pOutBlock->mdvalue8  ), DATA_TYPE_LONG   ) );    // 매도 거래대금8        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval8   , sizeof( pOutBlock->msval8    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수8      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode9 , sizeof( pOutBlock->tjjcode9  ), DATA_TYPE_STRING ) );    // 투자자코드9(기금)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume9, sizeof( pOutBlock->msvolume9 ), DATA_TYPE_LONG   ) );    // 매수 거래량9          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume9, sizeof( pOutBlock->mdvolume9 ), DATA_TYPE_LONG   ) );    // 매도 거래량9          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol9   , sizeof( pOutBlock->msvol9    ), DATA_TYPE_LONG   ) );    // 거래량 순매수9        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue9 , sizeof( pOutBlock->msvalue9  ), DATA_TYPE_LONG   ) );    // 매수 거래대금9        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue9 , sizeof( pOutBlock->mdvalue9  ), DATA_TYPE_LONG   ) );    // 매도 거래대금9        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval9   , sizeof( pOutBlock->msval9    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수9      
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode10, sizeof( pOutBlock->tjjcode10 ), DATA_TYPE_STRING ) );    // 투자자코드10(선물업자)
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume1, sizeof( pOutBlock->msvolume1 ), DATA_TYPE_LONG   ) );    // 매수 거래량10         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume1, sizeof( pOutBlock->mdvolume1 ), DATA_TYPE_LONG   ) );    // 매도 거래량10         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol10  , sizeof( pOutBlock->msvol10   ), DATA_TYPE_LONG   ) );    // 거래량 순매수10       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue10, sizeof( pOutBlock->msvalue10 ), DATA_TYPE_LONG   ) );    // 매수 거래대금10       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue10, sizeof( pOutBlock->mdvalue10 ), DATA_TYPE_LONG   ) );    // 매도 거래대금10       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval10  , sizeof( pOutBlock->msval10   ), DATA_TYPE_LONG   ) );    // 거래대금 순매수10     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode11, sizeof( pOutBlock->tjjcode11 ), DATA_TYPE_STRING ) );    // 투자자코드11(기타)    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume1, sizeof( pOutBlock->msvolume1 ), DATA_TYPE_LONG   ) );    // 매수 거래량11         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume1, sizeof( pOutBlock->mdvolume1 ), DATA_TYPE_LONG   ) );    // 매도 거래량11         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol11  , sizeof( pOutBlock->msvol11   ), DATA_TYPE_LONG   ) );    // 거래량 순매수11       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue11, sizeof( pOutBlock->msvalue11 ), DATA_TYPE_LONG   ) );    // 매수 거래대금11       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue11, sizeof( pOutBlock->mdvalue11 ), DATA_TYPE_LONG   ) );    // 매도 거래대금11       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval11  , sizeof( pOutBlock->msval11   ), DATA_TYPE_LONG   ) );    // 거래대금 순매수11     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->upcode   , sizeof( pOutBlock->upcode    ), DATA_TYPE_STRING ) );    // 업종코드              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->tjjcode0 , sizeof( pOutBlock->tjjcode0  ), DATA_TYPE_STRING ) );    // 투자자코드0(사모펀드) 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvolume0, sizeof( pOutBlock->msvolume0 ), DATA_TYPE_LONG   ) );    // 매수 거래량0          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvolume0, sizeof( pOutBlock->mdvolume0 ), DATA_TYPE_LONG   ) );    // 매도 거래량0          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvol0   , sizeof( pOutBlock->msvol0    ), DATA_TYPE_LONG   ) );    // 거래량 순매수0        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msvalue0 , sizeof( pOutBlock->msvalue0  ), DATA_TYPE_LONG   ) );    // 매수 거래대금0        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdvalue0 , sizeof( pOutBlock->mdvalue0  ), DATA_TYPE_LONG   ) );    // 매도 거래대금0        
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msval0   , sizeof( pOutBlock->msval0    ), DATA_TYPE_LONG   ) );    // 거래대금 순매수0      
	}

	return 0L;
}
