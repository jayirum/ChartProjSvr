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
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_t1101::InitCtrls()
{
	m_ctrlOutBlock.InsertColumn( 0, "필드명", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );

	// List Control에 OutBlock 필드명을 넣어준다.
	m_ctrlOutBlock.InsertItem(  0, "한글명            " );
	m_ctrlOutBlock.InsertItem(  1, "현재가            " );
	m_ctrlOutBlock.InsertItem(  2, "전일대비구분      " );
	m_ctrlOutBlock.InsertItem(  3, "전일대비          " );
	m_ctrlOutBlock.InsertItem(  4, "등락율            " );
	m_ctrlOutBlock.InsertItem(  5, "누적거래량        " );
	m_ctrlOutBlock.InsertItem(  6, "전일종가          " );
	m_ctrlOutBlock.InsertItem(  7, "매도호가1         " );
	m_ctrlOutBlock.InsertItem(  8, "매수호가1         " );
	m_ctrlOutBlock.InsertItem(  9, "매도호가수량1     " );
	m_ctrlOutBlock.InsertItem( 10, "매수호가수량1     " );
	m_ctrlOutBlock.InsertItem( 11, "직전매도대비수량1 " );
	m_ctrlOutBlock.InsertItem( 12, "직전매수대비수량1 " );
	m_ctrlOutBlock.InsertItem( 13, "매도호가2         " );
	m_ctrlOutBlock.InsertItem( 14, "매수호가2         " );
	m_ctrlOutBlock.InsertItem( 15, "매도호가수량2     " );
	m_ctrlOutBlock.InsertItem( 16, "매수호가수량2     " );
	m_ctrlOutBlock.InsertItem( 17, "직전매도대비수량2 " );
	m_ctrlOutBlock.InsertItem( 18, "직전매수대비수량2 " );
	m_ctrlOutBlock.InsertItem( 19, "매도호가3         " );
	m_ctrlOutBlock.InsertItem( 20, "매수호가3         " );
	m_ctrlOutBlock.InsertItem( 21, "매도호가수량3     " );
	m_ctrlOutBlock.InsertItem( 22, "매수호가수량3     " );
	m_ctrlOutBlock.InsertItem( 23, "직전매도대비수량3 " );
	m_ctrlOutBlock.InsertItem( 24, "직전매수대비수량3 " );
	m_ctrlOutBlock.InsertItem( 25, "매도호가4         " );
	m_ctrlOutBlock.InsertItem( 26, "매수호가4         " );
	m_ctrlOutBlock.InsertItem( 27, "매도호가수량4     " );
	m_ctrlOutBlock.InsertItem( 28, "매수호가수량4     " );
	m_ctrlOutBlock.InsertItem( 29, "직전매도대비수량4 " );
	m_ctrlOutBlock.InsertItem( 30, "직전매수대비수량4 " );
	m_ctrlOutBlock.InsertItem( 31, "매도호가5         " );
	m_ctrlOutBlock.InsertItem( 32, "매수호가5         " );
	m_ctrlOutBlock.InsertItem( 33, "매도호가수량5     " );
	m_ctrlOutBlock.InsertItem( 34, "매수호가수량5     " );
	m_ctrlOutBlock.InsertItem( 35, "직전매도대비수량5 " );
	m_ctrlOutBlock.InsertItem( 36, "직전매수대비수량5 " );
	m_ctrlOutBlock.InsertItem( 37, "매도호가6         " );
	m_ctrlOutBlock.InsertItem( 38, "매수호가6         " );
	m_ctrlOutBlock.InsertItem( 39, "매도호가수량6     " );
	m_ctrlOutBlock.InsertItem( 40, "매수호가수량6     " );
	m_ctrlOutBlock.InsertItem( 41, "직전매도대비수량6 " );
	m_ctrlOutBlock.InsertItem( 42, "직전매수대비수량6 " );
	m_ctrlOutBlock.InsertItem( 43, "매도호가7         " );
	m_ctrlOutBlock.InsertItem( 44, "매수호가7         " );
	m_ctrlOutBlock.InsertItem( 45, "매도호가수량7     " );
	m_ctrlOutBlock.InsertItem( 46, "매수호가수량7     " );
	m_ctrlOutBlock.InsertItem( 47, "직전매도대비수량7 " );
	m_ctrlOutBlock.InsertItem( 48, "직전매수대비수량7 " );
	m_ctrlOutBlock.InsertItem( 49, "매도호가8         " );
	m_ctrlOutBlock.InsertItem( 50, "매수호가8         " );
	m_ctrlOutBlock.InsertItem( 51, "매도호가수량8     " );
	m_ctrlOutBlock.InsertItem( 52, "매수호가수량8     " );
	m_ctrlOutBlock.InsertItem( 53, "직전매도대비수량8 " );
	m_ctrlOutBlock.InsertItem( 54, "직전매수대비수량8 " );
	m_ctrlOutBlock.InsertItem( 55, "매도호가9         " );
	m_ctrlOutBlock.InsertItem( 56, "매수호가9         " );
	m_ctrlOutBlock.InsertItem( 57, "매도호가수량9     " );
	m_ctrlOutBlock.InsertItem( 58, "매수호가수량9     " );
	m_ctrlOutBlock.InsertItem( 59, "직전매도대비수량9 " );
	m_ctrlOutBlock.InsertItem( 60, "직전매수대비수량9 " );
	m_ctrlOutBlock.InsertItem( 61, "매도호가10        " );
	m_ctrlOutBlock.InsertItem( 62, "매수호가10        " );
	m_ctrlOutBlock.InsertItem( 63, "매도호가수량10    " );
	m_ctrlOutBlock.InsertItem( 64, "매수호가수량10    " );
	m_ctrlOutBlock.InsertItem( 65, "직전매도대비수량10" );
	m_ctrlOutBlock.InsertItem( 66, "직전매수대비수량10" );
	m_ctrlOutBlock.InsertItem( 67, "매도호가수량합    " );
	m_ctrlOutBlock.InsertItem( 68, "매수호가수량합    " );
	m_ctrlOutBlock.InsertItem( 69, "직전매도대비수량합" );
	m_ctrlOutBlock.InsertItem( 70, "직전매수대비수량합" );
	m_ctrlOutBlock.InsertItem( 71, "수신시간          " );
	m_ctrlOutBlock.InsertItem( 72, "예상체결가격      " );
	m_ctrlOutBlock.InsertItem( 73, "예상체결수량      " );
	m_ctrlOutBlock.InsertItem( 74, "예상체결전일구분  " );
	m_ctrlOutBlock.InsertItem( 75, "예상체결전일대비  " );
	m_ctrlOutBlock.InsertItem( 76, "예상체결등락율    " );
	m_ctrlOutBlock.InsertItem( 77, "시간외매도잔량    " );
	m_ctrlOutBlock.InsertItem( 78, "시간외매수잔량    " );
	m_ctrlOutBlock.InsertItem( 79, "동시구분          " );
	m_ctrlOutBlock.InsertItem( 80, "단축코드          " );
	m_ctrlOutBlock.InsertItem( 81, "상한가            " );
	m_ctrlOutBlock.InsertItem( 82, "하한가            " );
	m_ctrlOutBlock.InsertItem( 83, "시가              " );
	m_ctrlOutBlock.InsertItem( 84, "고가              " );
	m_ctrlOutBlock.InsertItem( 85, "저가              " );
}

//--------------------------------------------------------------------------------------
// 데이터 조회
//--------------------------------------------------------------------------------------
void CDlg_t1101::RequestData( BOOL bNext )
{
	//-----------------------------------------------------------
	// 주식현재가호가조회(t1101) ( attr,block,headtype=A )
	t1101InBlock	pckInBlock;

	TCHAR			szTrNo[]	= "t1101";
	char			szNextKey[]	= "";

	//-----------------------------------------------------------
	// 데이터 취득
	CString str_shcode;		GetDlgItemText( IDC_EDIT_INBLOCK_SHCODE, str_shcode );		// 단축코드

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode, sizeof( pckInBlock.shcode ), str_shcode, DATA_TYPE_STRING );				// 단축코드

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
LRESULT CDlg_t1101::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data를 받음
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
LRESULT CDlg_t1101::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );

	return 0L;
}