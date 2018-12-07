// Dlg_SC2.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_SC2.h"

#include "../../include/API_ETK/SC2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC2 dialog

IMPLEMENT_DYNCREATE(CDlg_SC2, CDialog)

CDlg_SC2::CDlg_SC2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SC2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_SC2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_SC2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_SC2)
	DDX_Control(pDX, IDC_LIST_OUTBLOCK, m_ctrlOutBlock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_SC2, CDialog)
	//{{AFX_MSG_MAP(CDlg_SC2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_SC2 message handlers

BOOL CDlg_SC2::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 주문에 관련된 실시간TR은 입력값이 없으므로 화면이 오픈될때 등록한다.
	AdviseData();

	InitCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_SC2::OnDestroy()
{
	UnadviseData();

	CDialog::OnDestroy();
}

//--------------------------------------------------------------------------------------
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_SC2::InitCtrls()
{
	//-------------------------------------------------------------------------
	// OutBlock
	m_ctrlOutBlock.InsertColumn( 0, "필드  ", LVCFMT_LEFT, 150 );
	m_ctrlOutBlock.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );

	int nRow = 0;
	m_ctrlOutBlock.InsertItem( nRow++, "라인일련번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "계좌번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "조작자ID             " );
	m_ctrlOutBlock.InsertItem( nRow++, "헤더길이             " );
	m_ctrlOutBlock.InsertItem( nRow++, "헤더구분             " );
	m_ctrlOutBlock.InsertItem( nRow++, "압축구분             " );
	m_ctrlOutBlock.InsertItem( nRow++, "암호구분             " );
	m_ctrlOutBlock.InsertItem( nRow++, "공통시작지점         " );
	m_ctrlOutBlock.InsertItem( nRow++, "TRCODE               " );
	m_ctrlOutBlock.InsertItem( nRow++, "이용사번호           " );
	m_ctrlOutBlock.InsertItem( nRow++, "사용자ID             " );
	m_ctrlOutBlock.InsertItem( nRow++, "접속매체             " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F일련번호          " );
	m_ctrlOutBlock.InsertItem( nRow++, "전문일련번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "TR추적ID             " );
	m_ctrlOutBlock.InsertItem( nRow++, "공인IP               " );
	m_ctrlOutBlock.InsertItem( nRow++, "사설IP               " );
	m_ctrlOutBlock.InsertItem( nRow++, "처리지점번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "지점번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "단말번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "언어구분             " );
	m_ctrlOutBlock.InsertItem( nRow++, "AP처리시간           " );
	m_ctrlOutBlock.InsertItem( nRow++, "메세지코드           " );
	m_ctrlOutBlock.InsertItem( nRow++, "메세지출력구분       " );
	m_ctrlOutBlock.InsertItem( nRow++, "압축요청구분         " );
	m_ctrlOutBlock.InsertItem( nRow++, "기능키               " );
	m_ctrlOutBlock.InsertItem( nRow++, "요청레코드개수       " );
	m_ctrlOutBlock.InsertItem( nRow++, "예비영역             " );
	m_ctrlOutBlock.InsertItem( nRow++, "연속구분             " );
	m_ctrlOutBlock.InsertItem( nRow++, "연속키값             " );
	m_ctrlOutBlock.InsertItem( nRow++, "가변시스템길이       " );
	m_ctrlOutBlock.InsertItem( nRow++, "가변해더길이         " );
	m_ctrlOutBlock.InsertItem( nRow++, "가변메시지길이       " );
	m_ctrlOutBlock.InsertItem( nRow++, "조회발원지           " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F이벤트ID          " );
	m_ctrlOutBlock.InsertItem( nRow++, "I/F정보              " );
	m_ctrlOutBlock.InsertItem( nRow++, "예비영역             " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문체결유형코드     " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문시장코드         " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문유형코드         " );
	m_ctrlOutBlock.InsertItem( nRow++, "관리지점번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "계좌번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "계좌번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "계좌명               " );
	m_ctrlOutBlock.InsertItem( nRow++, "종목번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "종목명               " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "원주문번호           " );
	m_ctrlOutBlock.InsertItem( nRow++, "원주문번호           " );
	m_ctrlOutBlock.InsertItem( nRow++, "체결번호             " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문수량             " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문가격             " );
	m_ctrlOutBlock.InsertItem( nRow++, "체결수량             " );
	m_ctrlOutBlock.InsertItem( nRow++, "체결가격             " );
	m_ctrlOutBlock.InsertItem( nRow++, "정정확인수량         " );
	m_ctrlOutBlock.InsertItem( nRow++, "정정확인가격         " );
	m_ctrlOutBlock.InsertItem( nRow++, "취소확인수량         " );
	m_ctrlOutBlock.InsertItem( nRow++, "거부수량             " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문처리유형코드     " );
	m_ctrlOutBlock.InsertItem( nRow++, "복수주문일련번호     " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문조건             " );
	m_ctrlOutBlock.InsertItem( nRow++, "호가유형코드         " );
	m_ctrlOutBlock.InsertItem( nRow++, "비저축체결수량       " );
	m_ctrlOutBlock.InsertItem( nRow++, "단축종목번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "운용지시번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "반대매매주문구분     " );
	m_ctrlOutBlock.InsertItem( nRow++, "미체결수량(주문)     " );
	m_ctrlOutBlock.InsertItem( nRow++, "원주문미체결수량     " );
	m_ctrlOutBlock.InsertItem( nRow++, "원주문정정수량       " );
	m_ctrlOutBlock.InsertItem( nRow++, "원주문취소수량       " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문평균체결가격     " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문금액             " );
	m_ctrlOutBlock.InsertItem( nRow++, "표준종목번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "전표준종목번호       " );
	m_ctrlOutBlock.InsertItem( nRow++, "매매구분             " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문거래유형코드     " );
	m_ctrlOutBlock.InsertItem( nRow++, "신용거래코드         " );
	m_ctrlOutBlock.InsertItem( nRow++, "수수료합산코드       " );
	m_ctrlOutBlock.InsertItem( nRow++, "통신매체코드         " );
	m_ctrlOutBlock.InsertItem( nRow++, "대출일               " );
	m_ctrlOutBlock.InsertItem( nRow++, "회원/비회원사번호    " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문계좌번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "집계지점번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "관리사원번호         " );
	m_ctrlOutBlock.InsertItem( nRow++, "선물연계지점번호     " );
	m_ctrlOutBlock.InsertItem( nRow++, "선물연계계좌번호     " );
	m_ctrlOutBlock.InsertItem( nRow++, "선물시장구분         " );
	m_ctrlOutBlock.InsertItem( nRow++, "등록시장코드         " );
	m_ctrlOutBlock.InsertItem( nRow++, "현금증거금률         " );
	m_ctrlOutBlock.InsertItem( nRow++, "대용증거금률         " );
	m_ctrlOutBlock.InsertItem( nRow++, "현금체결금액         " );
	m_ctrlOutBlock.InsertItem( nRow++, "대용체결금액         " );
	m_ctrlOutBlock.InsertItem( nRow++, "수수료체결금액       " );
	m_ctrlOutBlock.InsertItem( nRow++, "신용담보체결금액     " );
	m_ctrlOutBlock.InsertItem( nRow++, "신용체결금액         " );
	m_ctrlOutBlock.InsertItem( nRow++, "전일재사용체결금액   " );
	m_ctrlOutBlock.InsertItem( nRow++, "금일재사용체결금액   " );
	m_ctrlOutBlock.InsertItem( nRow++, "실물체결수량         " );
	m_ctrlOutBlock.InsertItem( nRow++, "공매도체결수량       " );
	m_ctrlOutBlock.InsertItem( nRow++, "전략코드             " );
	m_ctrlOutBlock.InsertItem( nRow++, "그룹Id               " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문회차             " );
	m_ctrlOutBlock.InsertItem( nRow++, "포트폴리오번호       " );
	m_ctrlOutBlock.InsertItem( nRow++, "바스켓번호           " );
	m_ctrlOutBlock.InsertItem( nRow++, "트렌치번호           " );
	m_ctrlOutBlock.InsertItem( nRow++, "아이템번호           " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문자Id             " );
	m_ctrlOutBlock.InsertItem( nRow++, "차입관리여부         " );
	m_ctrlOutBlock.InsertItem( nRow++, "외국인고유번호       " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래세징수구분       " );
	m_ctrlOutBlock.InsertItem( nRow++, "유동성공급자구분     " );
	m_ctrlOutBlock.InsertItem( nRow++, "체결시각             " );
	m_ctrlOutBlock.InsertItem( nRow++, "거래소수신체결시각   " );
	m_ctrlOutBlock.InsertItem( nRow++, "잔여대출금액         " );
	m_ctrlOutBlock.InsertItem( nRow++, "잔고수량             " );
	m_ctrlOutBlock.InsertItem( nRow++, "실물가능수량         " );
	m_ctrlOutBlock.InsertItem( nRow++, "재사용가능수량(매도) " );
	m_ctrlOutBlock.InsertItem( nRow++, "변동수량             " );
	m_ctrlOutBlock.InsertItem( nRow++, "잔고수량(d2)         " );
	m_ctrlOutBlock.InsertItem( nRow++, "매도주문가능수량     " );
	m_ctrlOutBlock.InsertItem( nRow++, "미체결매도주문수량   " );
	m_ctrlOutBlock.InsertItem( nRow++, "평균매입가           " );
	m_ctrlOutBlock.InsertItem( nRow++, "매입금액             " );
	m_ctrlOutBlock.InsertItem( nRow++, "예수금               " );
	m_ctrlOutBlock.InsertItem( nRow++, "대용금               " );
	m_ctrlOutBlock.InsertItem( nRow++, "위탁증거금현금       " );
	m_ctrlOutBlock.InsertItem( nRow++, "위탁증거금대용       " );
	m_ctrlOutBlock.InsertItem( nRow++, "신용담보재사용금     " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문가능현금         " );
	m_ctrlOutBlock.InsertItem( nRow++, "주문가능대용         " );
	m_ctrlOutBlock.InsertItem( nRow++, "재사용가능금액       " );
}

//--------------------------------------------------------------------------------------
// 데이터 Advise
//--------------------------------------------------------------------------------------
void CDlg_SC2::AdviseData()
{
	TCHAR	szTrCode[]		= "SC2";

	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		"",							// SC2는 빈 데이터를 넘겨야 한다.
		0							// SC2의 데이터 길이는 0이다.
		);

	//-----------------------------------------------------------
	// 에러체크
	if( bSuccess == FALSE )
	{
		MessageBox( "조회실패", "에러", MB_ICONSTOP );
	}
}

//--------------------------------------------------------------------------------------
// 데이터 Unadvise
//--------------------------------------------------------------------------------------
void CDlg_SC2::UnadviseData()
{
	TCHAR	szTrCode[]		= "SC2";
	
	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		"",							// SC2는 빈 데이터를 넘겨야 한다.
		0							// SC2의 데이터 길이는 0이다.
		);
	
	//-----------------------------------------------------------
	// 에러체크
	if( bSuccess == FALSE )
	{
		MessageBox( "조회실패", "에러", MB_ICONSTOP );
	}
}

//--------------------------------------------------------------------------------------
// 데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CDlg_SC2::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	if( strcmp( pRealPacket->szTrCode, "SC2" ) == 0 )
	{
		LPSC2_OutBlock pOutBlock = (LPSC2_OutBlock)pRealPacket->pszData;
		int nRow = 0;
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lineseq             , sizeof( pOutBlock->lineseq             ), DATA_TYPE_LONG      ) );    // 라인일련번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno               , sizeof( pOutBlock->accno               ), DATA_TYPE_STRING    ) );    // 계좌번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->user                , sizeof( pOutBlock->user                ), DATA_TYPE_STRING    ) );    // 조작자ID             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->len                 , sizeof( pOutBlock->len                 ), DATA_TYPE_LONG      ) );    // 헤더길이             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->gubun               , sizeof( pOutBlock->gubun               ), DATA_TYPE_STRING    ) );    // 헤더구분             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compress            , sizeof( pOutBlock->compress            ), DATA_TYPE_STRING    ) );    // 압축구분             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->encrypt             , sizeof( pOutBlock->encrypt             ), DATA_TYPE_STRING    ) );    // 암호구분             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->offset              , sizeof( pOutBlock->offset              ), DATA_TYPE_LONG      ) );    // 공통시작지점         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trcode              , sizeof( pOutBlock->trcode              ), DATA_TYPE_STRING    ) );    // TRCODE               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compid              , sizeof( pOutBlock->compid              ), DATA_TYPE_STRING    ) );    // 이용사번호           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->userid              , sizeof( pOutBlock->userid              ), DATA_TYPE_STRING    ) );    // 사용자ID             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->media               , sizeof( pOutBlock->media               ), DATA_TYPE_STRING    ) );    // 접속매체             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ifid                , sizeof( pOutBlock->ifid                ), DATA_TYPE_STRING    ) );    // I/F일련번호          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->seq                 , sizeof( pOutBlock->seq                 ), DATA_TYPE_STRING    ) );    // 전문일련번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trid                , sizeof( pOutBlock->trid                ), DATA_TYPE_STRING    ) );    // TR추적ID             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pubip               , sizeof( pOutBlock->pubip               ), DATA_TYPE_STRING    ) );    // 공인IP               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prvip               , sizeof( pOutBlock->prvip               ), DATA_TYPE_STRING    ) );    // 사설IP               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pcbpno              , sizeof( pOutBlock->pcbpno              ), DATA_TYPE_STRING    ) );    // 처리지점번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bpno                , sizeof( pOutBlock->bpno                ), DATA_TYPE_STRING    ) );    // 지점번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->termno              , sizeof( pOutBlock->termno              ), DATA_TYPE_STRING    ) );    // 단말번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lang                , sizeof( pOutBlock->lang                ), DATA_TYPE_STRING    ) );    // 언어구분             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->proctm              , sizeof( pOutBlock->proctm              ), DATA_TYPE_LONG      ) );    // AP처리시간           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->msgcode             , sizeof( pOutBlock->msgcode             ), DATA_TYPE_STRING    ) );    // 메세지코드           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->outgu               , sizeof( pOutBlock->outgu               ), DATA_TYPE_STRING    ) );    // 메세지출력구분       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->compreq             , sizeof( pOutBlock->compreq             ), DATA_TYPE_STRING    ) );    // 압축요청구분         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->funckey             , sizeof( pOutBlock->funckey             ), DATA_TYPE_STRING    ) );    // 기능키               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->reqcnt              , sizeof( pOutBlock->reqcnt              ), DATA_TYPE_LONG      ) );    // 요청레코드개수       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->filler              , sizeof( pOutBlock->filler              ), DATA_TYPE_STRING    ) );    // 예비영역             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cont                , sizeof( pOutBlock->cont                ), DATA_TYPE_STRING    ) );    // 연속구분             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->contkey             , sizeof( pOutBlock->contkey             ), DATA_TYPE_STRING    ) );    // 연속키값             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varlen              , sizeof( pOutBlock->varlen              ), DATA_TYPE_LONG      ) );    // 가변시스템길이       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varhdlen            , sizeof( pOutBlock->varhdlen            ), DATA_TYPE_LONG      ) );    // 가변해더길이         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->varmsglen           , sizeof( pOutBlock->varmsglen           ), DATA_TYPE_LONG      ) );    // 가변메시지길이       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trsrc               , sizeof( pOutBlock->trsrc               ), DATA_TYPE_STRING    ) );    // 조회발원지           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->eventid             , sizeof( pOutBlock->eventid             ), DATA_TYPE_STRING    ) );    // I/F이벤트ID          
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ifinfo              , sizeof( pOutBlock->ifinfo              ), DATA_TYPE_STRING    ) );    // I/F정보              
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->filler1             , sizeof( pOutBlock->filler1             ), DATA_TYPE_STRING    ) );    // 예비영역             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordxctptncode       , sizeof( pOutBlock->ordxctptncode       ), DATA_TYPE_STRING    ) );    // 주문체결유형코드     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordmktcode          , sizeof( pOutBlock->ordmktcode          ), DATA_TYPE_STRING    ) );    // 주문시장코드         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordptncode          , sizeof( pOutBlock->ordptncode          ), DATA_TYPE_STRING    ) );    // 주문유형코드         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mgmtbrnno           , sizeof( pOutBlock->mgmtbrnno           ), DATA_TYPE_STRING    ) );    // 관리지점번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno1              , sizeof( pOutBlock->accno1              ), DATA_TYPE_STRING    ) );    // 계좌번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->accno2              , sizeof( pOutBlock->accno2              ), DATA_TYPE_STRING    ) );    // 계좌번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->acntnm              , sizeof( pOutBlock->acntnm              ), DATA_TYPE_STRING    ) );    // 계좌명               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->Isuno               , sizeof( pOutBlock->Isuno               ), DATA_TYPE_STRING    ) );    // 종목번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->Isunm               , sizeof( pOutBlock->Isunm               ), DATA_TYPE_STRING    ) );    // 종목명               
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordno1              , sizeof( pOutBlock->ordno1              ), DATA_TYPE_LONG      ) );    // 주문번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordno               , sizeof( pOutBlock->ordno               ), DATA_TYPE_LONG      ) );    // 주문번호             
//		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordno1           , sizeof( pOutBlock->orgordno1           ), DATA_TYPE_LONG      ) );    // 원주문번호           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordno            , sizeof( pOutBlock->orgordno            ), DATA_TYPE_LONG      ) );    // 원주문번호           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->execno              , sizeof( pOutBlock->execno              ), DATA_TYPE_LONG      ) );    // 체결번호             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordqty              , sizeof( pOutBlock->ordqty              ), DATA_TYPE_LONG      ) );    // 주문수량             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordprc              , sizeof( pOutBlock->ordprc              ), DATA_TYPE_LONG      ) );    // 주문가격             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->execqty             , sizeof( pOutBlock->execqty             ), DATA_TYPE_LONG      ) );    // 체결수량             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->execprc             , sizeof( pOutBlock->execprc             ), DATA_TYPE_LONG      ) );    // 체결가격             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdfycnfqty          , sizeof( pOutBlock->mdfycnfqty          ), DATA_TYPE_LONG      ) );    // 정정확인수량         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mdfycnfprc          , sizeof( pOutBlock->mdfycnfprc          ), DATA_TYPE_LONG      ) );    // 정정확인가격         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->canccnfqty          , sizeof( pOutBlock->canccnfqty          ), DATA_TYPE_LONG      ) );    // 취소확인수량         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rjtqty              , sizeof( pOutBlock->rjtqty              ), DATA_TYPE_LONG      ) );    // 거부수량             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordtrxptncode       , sizeof( pOutBlock->ordtrxptncode       ), DATA_TYPE_LONG      ) );    // 주문처리유형코드     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mtiordseqno         , sizeof( pOutBlock->mtiordseqno         ), DATA_TYPE_LONG      ) );    // 복수주문일련번호     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordcndi             , sizeof( pOutBlock->ordcndi             ), DATA_TYPE_STRING    ) );    // 주문조건             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordprcptncode       , sizeof( pOutBlock->ordprcptncode       ), DATA_TYPE_STRING    ) );    // 호가유형코드         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->nsavtrdqty          , sizeof( pOutBlock->nsavtrdqty          ), DATA_TYPE_LONG      ) );    // 비저축체결수량       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->shtnIsuno           , sizeof( pOutBlock->shtnIsuno           ), DATA_TYPE_STRING    ) );    // 단축종목번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->opdrtnno            , sizeof( pOutBlock->opdrtnno            ), DATA_TYPE_STRING    ) );    // 운용지시번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cvrgordtp           , sizeof( pOutBlock->cvrgordtp           ), DATA_TYPE_STRING    ) );    // 반대매매주문구분     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->unercqty            , sizeof( pOutBlock->unercqty            ), DATA_TYPE_LONG      ) );    // 미체결수량(주문)     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordunercqty      , sizeof( pOutBlock->orgordunercqty      ), DATA_TYPE_LONG      ) );    // 원주문미체결수량     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordmdfyqty       , sizeof( pOutBlock->orgordmdfyqty       ), DATA_TYPE_LONG      ) );    // 원주문정정수량       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orgordcancqty       , sizeof( pOutBlock->orgordcancqty       ), DATA_TYPE_LONG      ) );    // 원주문취소수량       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordavrexecprc       , sizeof( pOutBlock->ordavrexecprc       ), DATA_TYPE_LONG      ) );    // 주문평균체결가격     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordamt              , sizeof( pOutBlock->ordamt              ), DATA_TYPE_LONG      ) );    // 주문금액             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->stdIsuno            , sizeof( pOutBlock->stdIsuno            ), DATA_TYPE_STRING    ) );    // 표준종목번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bfstdIsuno          , sizeof( pOutBlock->bfstdIsuno          ), DATA_TYPE_STRING    ) );    // 전표준종목번호       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bnstp               , sizeof( pOutBlock->bnstp               ), DATA_TYPE_STRING    ) );    // 매매구분             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordtrdptncode       , sizeof( pOutBlock->ordtrdptncode       ), DATA_TYPE_STRING    ) );    // 주문거래유형코드     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mgntrncode          , sizeof( pOutBlock->mgntrncode          ), DATA_TYPE_STRING    ) );    // 신용거래코드         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->adduptp             , sizeof( pOutBlock->adduptp             ), DATA_TYPE_STRING    ) );    // 수수료합산코드       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->commdacode          , sizeof( pOutBlock->commdacode          ), DATA_TYPE_STRING    ) );    // 통신매체코드         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->Loandt              , sizeof( pOutBlock->Loandt              ), DATA_TYPE_STRING    ) );    // 대출일               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mbrnmbrno           , sizeof( pOutBlock->mbrnmbrno           ), DATA_TYPE_LONG      ) );    // 회원/비회원사번호    
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordacntno           , sizeof( pOutBlock->ordacntno           ), DATA_TYPE_STRING    ) );    // 주문계좌번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->agrgbrnno           , sizeof( pOutBlock->agrgbrnno           ), DATA_TYPE_STRING    ) );    // 집계지점번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mgempno             , sizeof( pOutBlock->mgempno             ), DATA_TYPE_STRING    ) );    // 관리사원번호         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futsLnkbrnno        , sizeof( pOutBlock->futsLnkbrnno        ), DATA_TYPE_STRING    ) );    // 선물연계지점번호     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futsLnkacntno       , sizeof( pOutBlock->futsLnkacntno       ), DATA_TYPE_STRING    ) );    // 선물연계계좌번호     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->futsmkttp           , sizeof( pOutBlock->futsmkttp           ), DATA_TYPE_STRING    ) );    // 선물시장구분         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->regmktcode          , sizeof( pOutBlock->regmktcode          ), DATA_TYPE_STRING    ) );    // 등록시장코드         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mnymgnrat           , sizeof( pOutBlock->mnymgnrat           ), DATA_TYPE_LONG      ) );    // 현금증거금률         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->substmgnrat         , sizeof( pOutBlock->substmgnrat         ), DATA_TYPE_LONG      ) );    // 대용증거금률         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->mnyexecamt          , sizeof( pOutBlock->mnyexecamt          ), DATA_TYPE_LONG      ) );    // 현금체결금액         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ubstexecamt         , sizeof( pOutBlock->ubstexecamt         ), DATA_TYPE_LONG      ) );    // 대용체결금액         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->cmsnamtexecamt      , sizeof( pOutBlock->cmsnamtexecamt      ), DATA_TYPE_LONG      ) );    // 수수료체결금액       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtpldgexecamt     , sizeof( pOutBlock->crdtpldgexecamt     ), DATA_TYPE_LONG      ) );    // 신용담보체결금액     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtexecamt         , sizeof( pOutBlock->crdtexecamt         ), DATA_TYPE_LONG      ) );    // 신용체결금액         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->prdayruseexecval    , sizeof( pOutBlock->prdayruseexecval    ), DATA_TYPE_LONG      ) );    // 전일재사용체결금액   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdayruseexecval    , sizeof( pOutBlock->crdayruseexecval    ), DATA_TYPE_LONG      ) );    // 금일재사용체결금액   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spotexecqty         , sizeof( pOutBlock->spotexecqty         ), DATA_TYPE_LONG      ) );    // 실물체결수량         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->stslexecqty         , sizeof( pOutBlock->stslexecqty         ), DATA_TYPE_LONG      ) );    // 공매도체결수량       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->strtgcode           , sizeof( pOutBlock->strtgcode           ), DATA_TYPE_STRING    ) );    // 전략코드             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->grpId               , sizeof( pOutBlock->grpId               ), DATA_TYPE_STRING    ) );    // 그룹Id               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordseqno            , sizeof( pOutBlock->ordseqno            ), DATA_TYPE_LONG      ) );    // 주문회차             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ptflno              , sizeof( pOutBlock->ptflno              ), DATA_TYPE_LONG      ) );    // 포트폴리오번호       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->bskno               , sizeof( pOutBlock->bskno               ), DATA_TYPE_LONG      ) );    // 바스켓번호           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trchno              , sizeof( pOutBlock->trchno              ), DATA_TYPE_LONG      ) );    // 트렌치번호           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->itemno              , sizeof( pOutBlock->itemno              ), DATA_TYPE_LONG      ) );    // 아이템번호           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->orduserId           , sizeof( pOutBlock->orduserId           ), DATA_TYPE_STRING    ) );    // 주문자Id             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->brwmgmtYn           , sizeof( pOutBlock->brwmgmtYn           ), DATA_TYPE_LONG      ) );    // 차입관리여부         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->frgrunqno           , sizeof( pOutBlock->frgrunqno           ), DATA_TYPE_STRING    ) );    // 외국인고유번호       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->trtzxLevytp         , sizeof( pOutBlock->trtzxLevytp         ), DATA_TYPE_STRING    ) );    // 거래세징수구분       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->lptp                , sizeof( pOutBlock->lptp                ), DATA_TYPE_STRING    ) );    // 유동성공급자구분     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->exectime            , sizeof( pOutBlock->exectime            ), DATA_TYPE_STRING    ) );    // 체결시각             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rcptexectime        , sizeof( pOutBlock->rcptexectime        ), DATA_TYPE_STRING    ) );    // 거래소수신체결시각   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->rmndLoanamt         , sizeof( pOutBlock->rmndLoanamt         ), DATA_TYPE_LONG      ) );    // 잔여대출금액         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->secbalqty           , sizeof( pOutBlock->secbalqty           ), DATA_TYPE_LONG      ) );    // 잔고수량             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->spotordableqty      , sizeof( pOutBlock->spotordableqty      ), DATA_TYPE_LONG      ) );    // 실물가능수량         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordableruseqty      , sizeof( pOutBlock->ordableruseqty      ), DATA_TYPE_LONG      ) );    // 재사용가능수량(매도) 
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->flctqty             , sizeof( pOutBlock->flctqty             ), DATA_TYPE_LONG      ) );    // 변동수량             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->secbalqtyd2         , sizeof( pOutBlock->secbalqtyd2         ), DATA_TYPE_LONG      ) );    // 잔고수량(d2)         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->sellableqty         , sizeof( pOutBlock->sellableqty         ), DATA_TYPE_LONG      ) );    // 매도주문가능수량     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->unercsellordqty     , sizeof( pOutBlock->unercsellordqty     ), DATA_TYPE_LONG      ) );    // 미체결매도주문수량   
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->avrpchsprc          , sizeof( pOutBlock->avrpchsprc          ), DATA_TYPE_LONG      ) );    // 평균매입가           
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->pchsant             , sizeof( pOutBlock->pchsant             ), DATA_TYPE_LONG      ) );    // 매입금액             
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->deposit             , sizeof( pOutBlock->deposit             ), DATA_TYPE_LONG      ) );    // 예수금               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->substamt            , sizeof( pOutBlock->substamt            ), DATA_TYPE_LONG      ) );    // 대용금               
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->csgnmnymgn          , sizeof( pOutBlock->csgnmnymgn          ), DATA_TYPE_LONG      ) );    // 위탁증거금현금       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->csgnsubstmgn        , sizeof( pOutBlock->csgnsubstmgn        ), DATA_TYPE_LONG      ) );    // 위탁증거금대용       
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->crdtpldgruseamt     , sizeof( pOutBlock->crdtpldgruseamt     ), DATA_TYPE_LONG      ) );    // 신용담보재사용금     
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordablemny          , sizeof( pOutBlock->ordablemny          ), DATA_TYPE_LONG      ) );    // 주문가능현금         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ordablesubstamt     , sizeof( pOutBlock->ordablesubstamt     ), DATA_TYPE_LONG      ) );    // 주문가능대용         
		m_ctrlOutBlock.SetItemText(  nRow++, 1, GetDispData( pOutBlock->ruseableamt         , sizeof( pOutBlock->ruseableamt         ), DATA_TYPE_LONG      ) );    // 재사용가능금액       
	} 
     
	return 0L;
}
