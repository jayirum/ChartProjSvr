
// Sample_xingACEDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Sample_xingACE.h"
#include "Sample_xingACEDlg.h"
#include "afxdialogex.h"

#include "../../IRUM_INC/eBest/CSPAT00600.h"	//현물 정상주문
#include "../../IRUM_INC/eBest/CSPAT00700.h"	//현물 정정주문
#include "../../IRUM_INC/eBest/CSPAT00800.h"	//현물 취소주문
#include "../../IRUM_INC/eBest/CSPAQ03700.h"	//현물계좌 주문체결내역 조회 
#include "../../IRUM_INC/eBest/CSPAQ02300.h"	// 현물계좌 잔고내역 조회 
#include "../../IRUM_INC/eBest/SC0.h"	//REAL.주식주문접수
#include "../../IRUM_INC/eBest/SC1.h"	//REAL.주식주문체결
#include "../../IRUM_INC/eBest/SC2.h"	//REAL.주식주문정정
#include "../../IRUM_INC/eBest/SC3.h"	//REAL.주식주문취소
#include "../../IRUM_INC/eBest/SC4.h"	//REAL.주식주문거부
//#include "../../IRUM_INC/eBest/S3_.h"	//REAL.KOSPI체결
//#include "../../IRUM_INC/eBest/H1_.h"	//REAL.KOSPI호가잔량
//#include "../../IRUM_INC/eBest/K3_.h"	//REAL.KOSDAQ체결
//#include "../../IRUM_INC/eBest/HA_.h"	//REAL.KOSDAQ호가잔량

#include "../../IRUM_INC/eBest/CIDBQ01500.h"	//해외선물 미결제 잔고내역 조회
#include "../../IRUM_INC/eBest/CIDBQ03000.h"	//해ㅚ선물 예수금/잔고현황
#include "../../IRUM_INC/eBest/CIDBQ05300.h"	//해외선물 계좌예탁자산 조회
#include "../../IRUM_INC/eBest/CIDBT00100.h"	//해외선물 신규주문
#include "../../IRUM_INC/eBest/CIDBT00900.h"	//해외선물 정정주문
#include "../../IRUM_INC/eBest/CIDBT01000.h"	//해외선물 취소주문
#include "../../IRUM_INC/eBest/OVC.H"			//해외선물 TICK
#include "../../IRUM_INC/eBest/OVH.H"			//해외선물 호가
#include "../../IRUM_INC/eBest/TC1.H"			//해외선물 주문접수실시간
#include "../../IRUM_INC/eBest/TC2.H"			//해외선물 주문응답실시간
#include "../../IRUM_INC/eBest/TC3.H"			//해외선물 주문체결실시간


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSample_xingACEDlg 대화 상자
CSample_xingACEDlg::CSample_xingACEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSample_xingACEDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSample_xingACEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOGA, m_ctrlHoga);
	DDX_Control(pDX, IDC_LIST_CHE, m_ctrlChe);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT, m_ctrlAccount);
	DDX_Control(pDX, IDC_COMBO_MEME, m_ctrlMeme);
	DDX_Control(pDX, IDC_LIST_ORDER, m_ctrlOrderList);
	DDX_Control(pDX, IDC_LIST_JANGO, m_ctrlJangoList);
	DDX_Control(pDX, IDC_LIST_LOG, m_ctrlLog);
	DDX_Control(pDX, IDC_LIST_LOG_DETAIL, m_ctrlDetailLog);
}

BEGIN_MESSAGE_MAP(CSample_xingACEDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CSample_xingACEDlg::OnClickedButtonLogin)

	ON_MESSAGE( WM_USER + XM_LOGIN,				OnXMLogin			)
	ON_MESSAGE( WM_USER + XM_DISCONNECT,		OnXMDisconnect		)
	ON_MESSAGE( WM_USER + XM_RECEIVE_DATA,		OnXMReceiveData		)
	ON_MESSAGE( WM_USER + XM_RECEIVE_REAL_DATA, OnXMReceiveRealData )
	ON_CBN_SELCHANGE(IDC_COMBO_MEME, &CSample_xingACEDlg::OnSelchangeComboMeme)
	ON_BN_CLICKED(IDC_BUTTON_ORDER, &CSample_xingACEDlg::OnClickedButtonOrder)
	ON_BN_CLICKED(IDC_BUTTON_LIST_ORDER, &CSample_xingACEDlg::OnClickedButtonListOrder)
	ON_BN_CLICKED(IDC_BUTTON_LIST_JANGO, &CSample_xingACEDlg::OnClickedButtonListJango)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOG, &CSample_xingACEDlg::OnItemchangedListLog)
END_MESSAGE_MAP()


// CSample_xingACEDlg 메시지 처리기

BOOL CSample_xingACEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//-----------------------------------------------------------------------------------
	// 초기화
	if( m_iXingAPI.Init() == FALSE )
	{
		MessageBox( "xingAPI.dll 을 찾을 수 없습니다.\n 실행파일과 같은 위치에 xingAPI.dll 이 있어야 합니다.", "xingACE 샘플", MB_ICONSTOP );
		PostMessage( WM_CLOSE );
		return TRUE;
	}
	InitCtrls();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSample_xingACEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSample_xingACEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSample_xingACEDlg::InitCtrls()
{
	int nCol;
	
	m_ctrlHoga.InsertColumn( 0, "잔량", LVCFMT_RIGHT, 60 );
	m_ctrlHoga.InsertColumn( 1, "호가", LVCFMT_RIGHT, 70 );
	m_ctrlHoga.InsertColumn( 2, "잔량", LVCFMT_RIGHT, 60 );
	m_ctrlHoga.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	for( int i=0; i<10; i++ )
	{
		m_ctrlHoga.InsertItem( i, "" );
	}

	m_ctrlChe.InsertColumn( 0, "체결가", LVCFMT_RIGHT, 70 );
	m_ctrlChe.InsertColumn( 1, "수량",   LVCFMT_RIGHT, 60 );
	m_ctrlChe.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlOrderList.InsertColumn( nCol++, "주문번호"  , LVCFMT_LEFT , 70 );    // [long  ,   10] 주문번호        
	m_ctrlOrderList.InsertColumn( nCol++, "원주문번호", LVCFMT_LEFT , 70 );    // [long  ,   10] 원주문번호      
	m_ctrlOrderList.InsertColumn( nCol++, "종목명"    , LVCFMT_LEFT , 70 );    // [string,   40] 종목명          
	m_ctrlOrderList.InsertColumn( nCol++, "구분"      , LVCFMT_LEFT , 70 );    // [string,   10] 매매구분명      
	m_ctrlOrderList.InsertColumn( nCol++, "정정취소"  , LVCFMT_LEFT , 70 );    // [string,   10] 정정취소구분명  
	m_ctrlOrderList.InsertColumn( nCol++, "주문수량"  , LVCFMT_RIGHT, 70 );    // [long  ,   16] 주문수량        
	m_ctrlOrderList.InsertColumn( nCol++, "주문가격"  , LVCFMT_RIGHT, 70 );    // [double, 15.2] 주문가격        
	m_ctrlOrderList.InsertColumn( nCol++, "체결수량"  , LVCFMT_RIGHT, 70 );    // [long  ,   16] 체결수량        
	m_ctrlOrderList.InsertColumn( nCol++, "체결가 "   , LVCFMT_RIGHT, 70 );    // [double, 15.2] 체결가          
	m_ctrlOrderList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlJangoList.InsertColumn( nCol++, "종목명"  , LVCFMT_LEFT , 70 );    // [string,   40] 종목명              
	m_ctrlJangoList.InsertColumn( nCol++, "잔고수량", LVCFMT_RIGHT, 70 );    // [long  ,   16] 잔고수량            
	m_ctrlJangoList.InsertColumn( nCol++, "평균단가", LVCFMT_LEFT , 70 );    // [double, 13.2] 평균단가            
	m_ctrlJangoList.InsertColumn( nCol++, "평가손익", LVCFMT_RIGHT, 70 );    // [long  ,   16] 평가손익            
	m_ctrlJangoList.InsertColumn( nCol++, "손익율"  , LVCFMT_RIGHT, 70 );    // [double, 18.6] 손익율              
	m_ctrlJangoList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlLog.InsertColumn( nCol++, "시간", LVCFMT_LEFT,  60 );
	m_ctrlLog.InsertColumn( nCol++, "TR"  , LVCFMT_LEFT,  40 );
	m_ctrlLog.InsertColumn( nCol++, "Data", LVCFMT_LEFT, 420 );
	m_ctrlLog.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlDetailLog.InsertColumn( nCol++, "필드", LVCFMT_LEFT, 110 );
	m_ctrlDetailLog.InsertColumn( nCol++, "내용", LVCFMT_LEFT,  70 );
	m_ctrlDetailLog.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	SetDlgItemText( IDC_EDIT_LOGIN_SERVER, "127.0.0.1" );

	m_ctrlMeme.SetCurSel( 1 );
	OnSelchangeComboMeme();
}

LRESULT CSample_xingACEDlg::OnXMLogin( WPARAM wParam, LPARAM lParam )
{
	CString strCode = (LPCTSTR)wParam;
	CString strMsg  = (LPCTSTR)lParam;

	if( strCode != "0000" )
	{
		MessageBox( strCode + " : " + strMsg, "로그인 실패", MB_ICONSTOP );
	}
	else
	{
		m_ctrlAccount.ResetContent();

		int nCount = m_iXingAPI.GetAccountListCount();
		for( int i=0; i<nCount; i++ )
		{
			char szAccount[20] = { 0 };
			m_iXingAPI.GetAccountList( i, szAccount, sizeof( szAccount ) );
			m_ctrlAccount.AddString( szAccount );
		}
		if( m_ctrlAccount.GetCount() > 0 )
		{
			m_ctrlAccount.SetCurSel( 0 );
		}

		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "SC0", "", 0 );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "SC1", "", 0 );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "SC2", "", 0 );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "SC3", "", 0 );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "SC4", "", 0 );

	}
	return 0L;
}

LRESULT CSample_xingACEDlg::OnXMDisconnect( WPARAM wParam, LPARAM lParam )
{
	return 0L;
}


//주문TR 응답 수신
LRESULT CSample_xingACEDlg::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pPacket = (LPRECV_PACKET)lParam;
		if( strcmp( pPacket->szTrCode, "CSPAT00600" ) == 0 ) 
			ReceiveCSPAT00600( pPacket->lpData, pPacket->nDataLength );
		else if( strcmp( pPacket->szTrCode, "CSPAT00700" ) == 0 ) ReceiveCSPAT00700( pPacket->lpData, pPacket->nDataLength );
		else if( strcmp( pPacket->szTrCode, "CSPAT00800" ) == 0 ) ReceiveCSPAT00800( pPacket->lpData, pPacket->nDataLength );
		else if( strcmp( pPacket->szTrCode, "CSPAQ03700" ) == 0 ) ReceiveCSPAQ03700( pPacket->lpData, pPacket->nDataLength );
		else if( strcmp( pPacket->szTrCode, "CSPAQ02300" ) == 0 ) ReceiveCSPAQ02300( pPacket->lpData, pPacket->nDataLength );
	}
	else if( wParam == MESSAGE_DATA )
	{
		LPMSG_PACKET pPacket = (LPMSG_PACKET)lParam;
		CString strMsg;
		strMsg.Format( "[%s] %s", pPacket->szMsgCode, CString( (char*)pPacket->lpszMessageData, pPacket->nMsgLength ) );
		MessageBox( strMsg, "확인", MB_OK );
	}
	else if( wParam == SYSTEM_ERROR_DATA )
	{
	}
	else if( wParam == RELEASE_DATA )
	{
		m_iXingAPI.ReleaseRequestData( (int)lParam );
	}
	return 0L;
}


// 주문체결 실시간
LRESULT CSample_xingACEDlg::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

		 if( strcmp( pRealPacket->szTrCode, "S3_" ) == 0 ) ReceiveRealS3_( pRealPacket->pszData );
	else if( strcmp( pRealPacket->szTrCode, "H1_" ) == 0 ) ReceiveRealH1_( pRealPacket->pszData );
	else if( strcmp( pRealPacket->szTrCode, "K3_" ) == 0 ) ReceiveRealK3_( pRealPacket->pszData );
	else if( strcmp( pRealPacket->szTrCode, "HA_" ) == 0 ) ReceiveRealHA_( pRealPacket->pszData );

	char szData[2048] = { 0 };
	CopyMemory( szData, pRealPacket->pszData, pRealPacket->nDataLength );

	m_ctrlLog.InsertItem( 0, "" );
	m_ctrlLog.SetItemText( 0, 0, CTime::GetCurrentTime().Format( "%H:%M:%S" ) );
	m_ctrlLog.SetItemText( 0, 1, pRealPacket->szTrCode );
	m_ctrlLog.SetItemText( 0, 2, szData );
     
	return 0L;
}

void CSample_xingACEDlg::OnClickedButtonLogin()
{
	CString strID;	   GetDlgItemText( IDC_EDIT_LOGIN_ID,     strID     );
	CString strPwd;	   GetDlgItemText( IDC_EDIT_LOGIN_PWD,    strPwd    );
	CString strCert;   GetDlgItemText( IDC_EDIT_LOGIN_CERT,   strCert   );
	CString strServer; GetDlgItemText( IDC_EDIT_LOGIN_SERVER, strServer );

	if( m_iXingAPI.Connect( GetSafeHwnd(), strServer, 0, WM_USER ) == FALSE )
	{
		CString strMsg;
		strMsg += "서버연결에 실패하였습니다.\n";
		strMsg += "xingACE가 실행중인지 확인하시기 바랍니다.";
		MessageBox( strMsg, "에러", MB_ICONSTOP );
		return;
	}

	if( m_iXingAPI.Login( GetSafeHwnd(), strID, strPwd, strCert, 0 ) == FALSE )
	{
		CString strMsg;
		strMsg += "로그인 전송에 실패하였습니다.\n";
		strMsg += "xingACE가 실행중인지 확인하시기 바랍니다.";
		MessageBox( strMsg, "에러", MB_ICONSTOP );
		return;
	}
}

void CSample_xingACEDlg::OnSelchangeComboMeme()
{
	int nSel = m_ctrlMeme.GetCurSel();
	((CWnd*)GetDlgItem( IDC_EDIT_ORG_ORDER_NO ))->EnableWindow( nSel < 2 ? FALSE : TRUE );
	((CWnd*)GetDlgItem( IDC_EDIT_QTY		  ))->EnableWindow( nSel == 3 ? FALSE : TRUE );
}

void CSample_xingACEDlg::OnClickedButtonOrder()
{
	int nSel = m_ctrlMeme.GetCurSel();
	switch( nSel )
	{
	case 0 :
	case 1 : RequestCSPAT00600(); break;
	case 2 : RequestCSPAT00700(); break;
	case 3 : RequestCSPAT00800(); break;
	}
}

void CSample_xingACEDlg::OnClickedButtonListOrder()
{
	RequestCSPAQ03700();
}

void CSample_xingACEDlg::OnClickedButtonListJango()
{
	RequestCSPAQ02300();
}


//해외선물 신규주문
void CSample_xingACEDlg::RequestCIDBT00100()
{

	CString strCode;
	GetDlgItemText(IDC_EDIT_CODE, strCode);
	if (strCode.GetLength() > 6)
	{
		strCode = strCode.Right(6);
	}

	

	CString strMeme = m_ctrlMeme.GetCurSel() == 0 ? "1" : "2";
	
	char tmp[128];
	
	CIDBT00100InBlock1 inBlock;
	FillMemory(&inBlock, sizeof(inBlock), ' ');
	SetPacketData(inBlock.RecCnt, sizeof(inBlock.RecCnt), this, 1, DATA_TYPE_LONG);    // [long  ] 주문수량     
	
	char zToday[9]; SYSTEMTIME st; GetLocalTime(&st);
	sprintf(zToday, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	
	SetPacketData(inBlock.OrdDt, sizeof(inBlock.OrdDt), zToday, DATA_TYPE_STRING);    // [string] 계좌번호     
	
	sprintf(tmp, "%*.*s", sizeof(inBlock.BrnCode), sizeof(inBlock.BrnCode), " ");
	SetPacketData(inBlock.BrnCode, sizeof(inBlock.BrnCode), tmp, DATA_TYPE_STRING);    // [string] 계좌번호     

	SetPacketData(inBlock.AcntNo, sizeof(inBlock.AcntNo), this, IDC_COMBO_ACCOUNT, DATA_TYPE_STRING);    // [string] 계좌번호     
	SetPacketData(inBlock.Pwd, sizeof(inBlock.Pwd), this, IDC_EDIT_PWD, DATA_TYPE_STRING);    // [string] 입력비밀번호 
	SetPacketData(inBlock.IsuCodeVal, sizeof(inBlock.IsuCodeVal), this, IDC_EDIT_CODE, DATA_TYPE_STRING);    // [string] 종목번호     
	SetPacketData(inBlock.FutsOrdTpCode, sizeof(inBlock.FutsOrdTpCode), "1", DATA_TYPE_STRING);    // [string] 종목번호     
	SetPacketData(inBlock.BnsTpCode, sizeof(inBlock.BnsTpCode), strMeme, DATA_TYPE_STRING);    // [string] 매매구분     

	// 1:시장가, 2:지정가
	SetPacketData(inBlock.AbrdFutsOrdPtnCode, sizeof(inBlock.AbrdFutsOrdPtnCode), "2", DATA_TYPE_STRING);    // [string] 계좌번호     

	sprintf(tmp, "%*.*s", sizeof(inBlock.CrcyCode), sizeof(inBlock.CrcyCode), " ");
	SetPacketData(inBlock.CrcyCode, sizeof(inBlock.CrcyCode), tmp, DATA_TYPE_STRING);    // [string] 계좌번호     
	
	SetPacketData(inBlock.OvrsDrvtOrdPrc, sizeof(inBlock.OvrsDrvtOrdPrc), this, IDC_EDIT_PRICE, DATA_TYPE_DOT, 8);    // [double] 주문가       
	//TODO SetPacketData(inBlock.CndiOrdPrc, sizeof(inBlock.CndiOrdPrc), 0, DATA_TYPE_DOT, 8);    // [double] 주문가       

	SetPacketData(inBlock.OrdQty, sizeof(inBlock.OrdQty), this, IDC_EDIT_QTY, DATA_TYPE_LONG);    // [long  ] 주문수량     
	
	// 상품코드
	sprintf(tmp, "%*.*s", sizeof(inBlock.PrdtCode), sizeof(inBlock.PrdtCode), " ");
	SetPacketData(inBlock.PrdtCode, sizeof(inBlock.PrdtCode), tmp, DATA_TYPE_STRING);    // [string] 계좌번호     

	//만기년월
	sprintf(tmp, "%*.*s", sizeof(inBlock.DueYymm), sizeof(inBlock.DueYymm), " ");
	SetPacketData(inBlock.DueYymm, sizeof(inBlock.DueYymm), tmp, DATA_TYPE_STRING);    // [string] 계좌번호     
	
	//거래소코드
	sprintf(tmp, "%*.*s", sizeof(inBlock.ExchCode), sizeof(inBlock.ExchCode), " ");
	SetPacketData(inBlock.ExchCode, sizeof(inBlock.ExchCode), tmp, DATA_TYPE_STRING);    // [string] 계좌번호     

	int nReqID = m_iXingAPI.Request(GetSafeHwnd(), "CIDBT00100", &inBlock, sizeof(inBlock));
	if (nReqID < 0)
	{
		CString strMsg;
		strMsg += "데이터 전송에 실패하였습니다.";
		MessageBox(strMsg, "전송에러", MB_ICONSTOP);
	}
}

void CSample_xingACEDlg::RequestCSPAT00600()
{
	CString strCode;
	GetDlgItemText( IDC_EDIT_CODE, strCode );
	if( strCode.GetLength() > 6 )
	{
		strCode = strCode.Right( 6 );
	}

	if( m_strRegCode != strCode )
	{
		if( m_strRegCode.IsEmpty() == FALSE )
		{
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "S3_", m_strRegCode, m_strRegCode.GetLength() );
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "H1_", m_strRegCode, m_strRegCode.GetLength() );
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "K3_", m_strRegCode, m_strRegCode.GetLength() );
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "HA_", m_strRegCode, m_strRegCode.GetLength() );
		}

		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "S3_", strCode, strCode.GetLength() );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "H1_", strCode, strCode.GetLength() );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "K3_", strCode, strCode.GetLength() );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "HA_", strCode, strCode.GetLength() );
		m_strRegCode = strCode;

		for( int i=0; i<10; i++ )
		{
			m_ctrlHoga.SetItemText( i, 0, "" );
			m_ctrlHoga.SetItemText( i, 1, "" );
			m_ctrlHoga.SetItemText( i, 2, "" );
		}
		m_ctrlChe.DeleteAllItems();
	}

	CString strMeme = m_ctrlMeme.GetCurSel() == 0 ? "1" : "2";

	CSPAT00600InBlock1 inBlock;
	FillMemory( &inBlock, sizeof( inBlock ), ' ' );
	SetPacketData( inBlock.AcntNo       , sizeof( inBlock.AcntNo        ), this, IDC_COMBO_ACCOUNT, DATA_TYPE_STRING    );    // [string] 계좌번호     
	SetPacketData( inBlock.InptPwd      , sizeof( inBlock.InptPwd       ), this, IDC_EDIT_PWD     , DATA_TYPE_STRING    );    // [string] 입력비밀번호 
	SetPacketData( inBlock.IsuNo        , sizeof( inBlock.IsuNo         ), this, IDC_EDIT_CODE    , DATA_TYPE_STRING    );    // [string] 종목번호     
	SetPacketData( inBlock.OrdQty       , sizeof( inBlock.OrdQty        ), this, IDC_EDIT_QTY     , DATA_TYPE_LONG      );    // [long  ] 주문수량     
	SetPacketData( inBlock.OrdPrc       , sizeof( inBlock.OrdPrc        ), this, IDC_EDIT_PRICE   , DATA_TYPE_DOT   , 2 );    // [double] 주문가       
	SetPacketData( inBlock.BnsTpCode    , sizeof( inBlock.BnsTpCode     ), strMeme                , DATA_TYPE_STRING    );    // [string] 매매구분     
	SetPacketData( inBlock.OrdprcPtnCode, sizeof( inBlock.OrdprcPtnCode ), "00"                   , DATA_TYPE_STRING    );    // [string] 호가유형코드 
	SetPacketData( inBlock.MgntrnCode   , sizeof( inBlock.MgntrnCode    ), "000"                  , DATA_TYPE_STRING    );    // [string] 신용거래코드 
	SetPacketData( inBlock.LoanDt       , sizeof( inBlock.LoanDt        ), "        "             , DATA_TYPE_STRING    );    // [string] 대출일       
	SetPacketData( inBlock.OrdCndiTpCode, sizeof( inBlock.OrdCndiTpCode ), "0"                    , DATA_TYPE_STRING    );    // [string] 주문조건구분 

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAT00600", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "데이터 전송에 실패하였습니다.";
		MessageBox( strMsg, "전송에러", MB_ICONSTOP );
	}
}

void CSample_xingACEDlg::RequestCSPAT00700()
{
	CString strCode;
	GetDlgItemText( IDC_EDIT_CODE, strCode );
	if( strCode.GetLength() > 6 )
	{
		strCode = strCode.Right( 6 );
	}

	if( m_strRegCode != strCode )
	{
		if( m_strRegCode.IsEmpty() == FALSE )
		{
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "S3_", m_strRegCode, m_strRegCode.GetLength() );
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "H1_", m_strRegCode, m_strRegCode.GetLength() );
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "K3_", m_strRegCode, m_strRegCode.GetLength() );
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "HA_", m_strRegCode, m_strRegCode.GetLength() );
		}

		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "S3_", strCode, strCode.GetLength() );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "H1_", strCode, strCode.GetLength() );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "K3_", strCode, strCode.GetLength() );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "HA_", strCode, strCode.GetLength() );
		m_strRegCode = strCode;

		for( int i=0; i<10; i++ )
		{
			m_ctrlHoga.SetItemText( i, 0, "" );
			m_ctrlHoga.SetItemText( i, 1, "" );
			m_ctrlHoga.SetItemText( i, 2, "" );
		}
		m_ctrlChe.DeleteAllItems();
	}

	CSPAT00700InBlock1 inBlock;
	FillMemory( &inBlock, sizeof( inBlock ), ' ' );
	SetPacketData( inBlock.OrgOrdNo     , sizeof( inBlock.OrgOrdNo      ), this, IDC_EDIT_ORG_ORDER_NO, DATA_TYPE_LONG      );    // [long  ] 원주문번호
	SetPacketData( inBlock.AcntNo       , sizeof( inBlock.AcntNo        ), this, IDC_COMBO_ACCOUNT    , DATA_TYPE_STRING    );    // [string] 계좌번호     
	SetPacketData( inBlock.InptPwd      , sizeof( inBlock.InptPwd       ), this, IDC_EDIT_PWD         , DATA_TYPE_STRING    );    // [string] 입력비밀번호 
	SetPacketData( inBlock.IsuNo        , sizeof( inBlock.IsuNo         ), this, IDC_EDIT_CODE        , DATA_TYPE_STRING    );    // [string] 종목번호     
	SetPacketData( inBlock.OrdQty       , sizeof( inBlock.OrdQty        ), this, IDC_EDIT_QTY         , DATA_TYPE_LONG      );    // [long  ] 주문수량     
	SetPacketData( inBlock.OrdprcPtnCode, sizeof( inBlock.OrdprcPtnCode ), "00"                       , DATA_TYPE_STRING    );    // [string] 호가유형코드       
	SetPacketData( inBlock.OrdCndiTpCode, sizeof( inBlock.OrdCndiTpCode ), "0"                        , DATA_TYPE_STRING    );    // [string] 주문조건구분       
	SetPacketData( inBlock.OrdPrc       , sizeof( inBlock.OrdPrc        ), this, IDC_EDIT_PRICE       , DATA_TYPE_DOT,    2 );    // [double] 주문가       

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAT00700", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "데이터 전송에 실패하였습니다.";
		MessageBox( strMsg, "전송에러", MB_ICONSTOP );
	}
}

void CSample_xingACEDlg::RequestCSPAT00800()
{
	CString strCode;
	GetDlgItemText( IDC_EDIT_CODE, strCode );
	if( strCode.GetLength() > 6 )
	{
		strCode = strCode.Right( 6 );
	}

	if( m_strRegCode != strCode )
	{
		if( m_strRegCode.IsEmpty() == FALSE )
		{
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "S3_", m_strRegCode, m_strRegCode.GetLength() );
//			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "H1_", m_strRegCode, m_strRegCode.GetLength() );
			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "K3_", m_strRegCode, m_strRegCode.GetLength() );
//			m_iXingAPI.UnadviseRealData( GetSafeHwnd(), "HA_", m_strRegCode, m_strRegCode.GetLength() );
		}

		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "S3_", strCode, strCode.GetLength() );
//		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "H1_", strCode, strCode.GetLength() );
		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "K3_", strCode, strCode.GetLength() );
//		m_iXingAPI.AdviseRealData( GetSafeHwnd(), "HA_", strCode, strCode.GetLength() );
		m_strRegCode = strCode;

		for( int i=0; i<10; i++ )
		{
			m_ctrlHoga.SetItemText( i, 0, "" );
			m_ctrlHoga.SetItemText( i, 1, "" );
			m_ctrlHoga.SetItemText( i, 2, "" );
		}
		m_ctrlChe.DeleteAllItems();
	}

	CSPAT00800InBlock1 inBlock;
	FillMemory( &inBlock, sizeof( inBlock ), ' ' );
	SetPacketData( inBlock.OrgOrdNo     , sizeof( inBlock.OrgOrdNo      ), this, IDC_EDIT_ORG_ORDER_NO, DATA_TYPE_LONG   );    // [long  ] 원주문번호
	SetPacketData( inBlock.AcntNo       , sizeof( inBlock.AcntNo        ), this, IDC_COMBO_ACCOUNT    , DATA_TYPE_STRING );    // [string] 계좌번호     
	SetPacketData( inBlock.InptPwd      , sizeof( inBlock.InptPwd       ), this, IDC_EDIT_PWD         , DATA_TYPE_STRING );    // [string] 입력비밀번호 
	SetPacketData( inBlock.IsuNo        , sizeof( inBlock.IsuNo         ), this, IDC_EDIT_CODE        , DATA_TYPE_STRING );    // [string] 종목번호     
	SetPacketData( inBlock.OrdQty       , sizeof( inBlock.OrdQty        ), this, IDC_EDIT_QTY         , DATA_TYPE_LONG   );    // [long  ] 주문수량     

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAT00800", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "데이터 전송에 실패하였습니다.";
		MessageBox( strMsg, "전송에러", MB_ICONSTOP );
	}
}

void CSample_xingACEDlg::RequestCSPAQ03700()
{
	CString strDate = CTime::GetCurrentTime().Format( "%Y%m%d" );

	CSPAQ03700InBlock1 inBlock;
	FillMemory( &inBlock, sizeof( inBlock ), ' ' );
	SetPacketData( inBlock.RecCnt     , sizeof( inBlock.RecCnt      ), "1"                     , DATA_TYPE_LONG   );    // [long  ,    5] 레코드갯수   
	SetPacketData( inBlock.AcntNo     , sizeof( inBlock.AcntNo      ), this, IDC_COMBO_ACCOUNT , DATA_TYPE_STRING );    // [string,   20] 계좌번호     
	SetPacketData( inBlock.InptPwd    , sizeof( inBlock.InptPwd     ), this, IDC_EDIT_PWD      , DATA_TYPE_STRING );    // [string,    8] 입력비밀번호 
	SetPacketData( inBlock.OrdMktCode , sizeof( inBlock.OrdMktCode  ), "00"                    , DATA_TYPE_STRING );    // [string,    2] 주문시장코드 
	SetPacketData( inBlock.BnsTpCode  , sizeof( inBlock.BnsTpCode   ), "0"                     , DATA_TYPE_STRING );    // [string,    1] 매매구분코드 
	SetPacketData( inBlock.IsuNo      , sizeof( inBlock.IsuNo       ), ""                      , DATA_TYPE_STRING );    // [string,   12] 종목번호     
	SetPacketData( inBlock.ExecYn     , sizeof( inBlock.ExecYn      ), "0"                     , DATA_TYPE_STRING );    // [string,    1] 체결여부     
	SetPacketData( inBlock.OrdDt      , sizeof( inBlock.OrdDt       ), strDate                 , DATA_TYPE_STRING );    // [string,    8] 주문일       
	SetPacketData( inBlock.SrtOrdNo2  , sizeof( inBlock.SrtOrdNo2   ), "9999999999"            , DATA_TYPE_LONG   );    // [long  ,   10] 시작주문번호2
	SetPacketData( inBlock.BkseqTpCode, sizeof( inBlock.BkseqTpCode ), "0"                     , DATA_TYPE_STRING );    // [string,    1] 역순구분     
	SetPacketData( inBlock.OrdPtnCode , sizeof( inBlock.OrdPtnCode  ), "00"                    , DATA_TYPE_STRING );    // [string,    2] 주문유형코드 

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAQ03700", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "데이터 전송에 실패하였습니다.";
		MessageBox( strMsg, "전송에러", MB_ICONSTOP );
	}
}

void CSample_xingACEDlg::RequestCSPAQ02300()
{
	CSPAQ02300InBlock1 inBlock;
	FillMemory( &inBlock, sizeof( inBlock ), ' ' );
	SetPacketData( inBlock.RecCnt        , sizeof( inBlock.RecCnt         ), "1"                    , DATA_TYPE_LONG   );    // [long  ,    5] 레코드갯수        
	SetPacketData( inBlock.AcntNo        , sizeof( inBlock.AcntNo         ), this, IDC_COMBO_ACCOUNT, DATA_TYPE_STRING );    // [string,   20] 계좌번호           
	SetPacketData( inBlock.Pwd           , sizeof( inBlock.Pwd            ), this, IDC_EDIT_PWD     , DATA_TYPE_STRING );    // [string,    8] 비밀번호           
	SetPacketData( inBlock.BalCreTp      , sizeof( inBlock.BalCreTp       ), "0"                    , DATA_TYPE_STRING );    // [string,    1] 잔고생성구분       
	SetPacketData( inBlock.CmsnAppTpCode , sizeof( inBlock.CmsnAppTpCode  ), "0"                    , DATA_TYPE_STRING );    // [string,    1] 수수료적용구분     
	SetPacketData( inBlock.D2balBaseQryTp, sizeof( inBlock.D2balBaseQryTp ), "1"                    , DATA_TYPE_STRING );    // [string,    1] D2잔고기준조회구분
	SetPacketData( inBlock.UprcTpCode    , sizeof( inBlock.UprcTpCode     ), "0"                    , DATA_TYPE_STRING );    // [string,    1] 단가구분           

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAQ02300", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "데이터 전송에 실패하였습니다.";
		MessageBox( strMsg, "전송에러", MB_ICONSTOP );
	}
}

void CSample_xingACEDlg::ReceiveCSPAT00600( LPVOID pData, int nLen )
{	
}

void CSample_xingACEDlg::ReceiveCSPAT00700( LPVOID pData, int nLen )
{
}

void CSample_xingACEDlg::ReceiveCSPAT00800( LPVOID pData, int nLen )
{
}

void CSample_xingACEDlg::ReceiveCSPAQ03700( LPVOID pData, int nLen )
{
	typedef struct
	{
		CSPAQ03700OutBlock1	outBlock1;
		CSPAQ03700OutBlock2	outBlock2;
		char				sOutBlock3Count[5];
		CSPAQ03700OutBlock3	outBlock3[1];
	} CSPAQ03700OutBlock, *LPCSPAQ03700OutBlock;
	LPCSPAQ03700OutBlock pOutBlock = (LPCSPAQ03700OutBlock)pData;

	m_ctrlOrderList.DeleteAllItems();
	int nCount = atoi( CString( pOutBlock->sOutBlock3Count, sizeof( pOutBlock->sOutBlock3Count ) ) );
	for( int i=0; i<nCount; i++ )
	{
		int nCol = 0;
		LPCSPAQ03700OutBlock3 pOutBlock3 = &pOutBlock->outBlock3[i];
		m_ctrlOrderList.InsertItem( i, "" );
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrdNo   , sizeof( pOutBlock3->OrdNo    ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   10] 주문번호        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrgOrdNo, sizeof( pOutBlock3->OrgOrdNo ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   10] 원주문번호      
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->IsuNm   , sizeof( pOutBlock3->IsuNm    ), DATA_TYPE_STRING, 0 ) );    // [string,   40] 종목명          
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->BnsTpNm , sizeof( pOutBlock3->BnsTpNm  ), DATA_TYPE_STRING, 0 ) );    // [string,   10] 매매구분명      
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->MrcTpNm , sizeof( pOutBlock3->MrcTpNm  ), DATA_TYPE_STRING, 0 ) );    // [string,   10] 정정취소구분명  
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrdQty  , sizeof( pOutBlock3->OrdQty   ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] 주문수량        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrdPrc  , sizeof( pOutBlock3->OrdPrc   ), DATA_TYPE_DOUBLE, 2 ) );    // [double, 15.2] 주문가격        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->ExecQty , sizeof( pOutBlock3->ExecQty  ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] 체결수량        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->ExecPrc , sizeof( pOutBlock3->ExecPrc  ), DATA_TYPE_DOUBLE, 2 ) );    // [double, 15.2] 체결가          
	}
}

void CSample_xingACEDlg::ReceiveCSPAQ02300( LPVOID pData, int nLen )
{
	typedef struct
	{
		CSPAQ02300OutBlock1	outBlock1;
		CSPAQ02300OutBlock2	outBlock2;
		char				sOutBlock3Count[5];
		CSPAQ02300OutBlock3	outBlock3[1];
	} CSPAQ02300OutBlock, *LPCSPAQ02300OutBlock;
	LPCSPAQ02300OutBlock pOutBlock = (LPCSPAQ02300OutBlock)pData;

	m_ctrlJangoList.DeleteAllItems();
	int nCount = atoi( CString( pOutBlock->sOutBlock3Count, sizeof( pOutBlock->sOutBlock3Count ) ) );
	for( int i=0; i<nCount; i++ )
	{
		int nCol = 0;
		LPCSPAQ02300OutBlock3 pOutBlock3 = &pOutBlock->outBlock3[i];
		m_ctrlJangoList.InsertItem( i, "" );
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->IsuNm  , sizeof( pOutBlock3->IsuNm   ), DATA_TYPE_STRING, 0 ) );    // [string,   40] 종목명              
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->BalQty , sizeof( pOutBlock3->BalQty  ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] 잔고수량            
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->AvrUprc, sizeof( pOutBlock3->AvrUprc ), DATA_TYPE_DOUBLE, 2 ) );    // [double, 13.2] 평균단가            
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->EvalPnl, sizeof( pOutBlock3->EvalPnl ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] 평가손익            
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->PnlRat , sizeof( pOutBlock3->PnlRat  ), DATA_TYPE_DOUBLE, 6 ) );    // [double, 18.6] 손익율              
	}
}

int CSample_xingACEDlg::GetSelectedItem( CListCtrl& ctrl )
{
	POSITION pos = ctrl.GetFirstSelectedItemPosition();
	return ( pos != NULL ) ? ctrl.GetNextSelectedItem( pos ) : -1;
}

void CSample_xingACEDlg::OnItemchangedListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	char szTrCode[20] = { 0 };
	char szData[2048] = { 0 };
	int nItem = GetSelectedItem( m_ctrlLog );	
	if( nItem >= 0 )
	{
		m_ctrlLog.GetItemText( nItem, 1, szTrCode, sizeof( szTrCode ) );
		m_ctrlLog.GetItemText( nItem, 2, szData,   sizeof( szData   ) );
	}

	ProcessDetailLog( szTrCode, szData );
}

void CSample_xingACEDlg::ProcessDetailLog( LPCTSTR pszTrCode, LPCTSTR pszData )
{
	m_ctrlDetailLog.DeleteAllItems();

		 if( strcmp( pszTrCode, "SC0" ) == 0 ) DetailLogSC0( pszData );
	else if( strcmp( pszTrCode, "SC1" ) == 0 ) DetailLogSC1( pszData );
	else if( strcmp( pszTrCode, "SC2" ) == 0 ) DetailLogSC2( pszData );
	else if( strcmp( pszTrCode, "SC3" ) == 0 ) DetailLogSC3( pszData );
	else if( strcmp( pszTrCode, "SC4" ) == 0 ) DetailLogSC4( pszData );
	else if( strcmp( pszTrCode, "S3_" ) == 0 ) DetailLogS3_( pszData );
	else if( strcmp( pszTrCode, "H1_" ) == 0 ) DetailLogH1_( pszData );
	else if( strcmp( pszTrCode, "K3_" ) == 0 ) DetailLogK3_( pszData );
	else if( strcmp( pszTrCode, "HA_" ) == 0 ) DetailLogHA_( pszData );
}

void CSample_xingACEDlg::SetDetailLogData( LPCTSTR pszDesc, LPCTSTR pszData, int nDataSize, int nDataType, int nDataDotPos )
{
	CString strDesc = pszDesc;
	strDesc.TrimRight();
	int nItem = m_ctrlDetailLog.InsertItem( m_ctrlDetailLog.GetItemCount(), strDesc );
	m_ctrlDetailLog.SetItemText( nItem, 1, GetDispData( (char*)pszData , nDataSize, nDataType, nDataDotPos ) );
}

void CSample_xingACEDlg::DetailLogSC0( LPCTSTR pszData )
{/*
	LPSC0_OutBlock pOutBlock = (LPSC0_OutBlock)pszData;

	SetDetailLogData( "라인일련번호        ", pOutBlock->lineseq        , sizeof( pOutBlock->lineseq         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "계좌번호            ", pOutBlock->accno          , sizeof( pOutBlock->accno           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "조작자ID            ", pOutBlock->user           , sizeof( pOutBlock->user            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "헤더길이            ", pOutBlock->len            , sizeof( pOutBlock->len             ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "헤더구분            ", pOutBlock->gubun          , sizeof( pOutBlock->gubun           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "압축구분            ", pOutBlock->compress       , sizeof( pOutBlock->compress        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "암호구분            ", pOutBlock->encrypt        , sizeof( pOutBlock->encrypt         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "공통시작지점        ", pOutBlock->offset         , sizeof( pOutBlock->offset          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "TRCODE              ", pOutBlock->trcode         , sizeof( pOutBlock->trcode          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "이용사번호          ", pOutBlock->compid         , sizeof( pOutBlock->compid          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "사용자ID            ", pOutBlock->userid         , sizeof( pOutBlock->userid          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "접속매체            ", pOutBlock->media          , sizeof( pOutBlock->media           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "I/F일련번호         ", pOutBlock->ifid           , sizeof( pOutBlock->ifid            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "전문일련번호        ", pOutBlock->seq            , sizeof( pOutBlock->seq             ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "TR추적ID            ", pOutBlock->trid           , sizeof( pOutBlock->trid            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "공인IP              ", pOutBlock->pubip          , sizeof( pOutBlock->pubip           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "사설IP              ", pOutBlock->prvip          , sizeof( pOutBlock->prvip           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "처리지점번호        ", pOutBlock->pcbpno         , sizeof( pOutBlock->pcbpno          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "지점번호            ", pOutBlock->bpno           , sizeof( pOutBlock->bpno            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "단말번호            ", pOutBlock->termno         , sizeof( pOutBlock->termno          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "언어구분            ", pOutBlock->lang           , sizeof( pOutBlock->lang            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "AP처리시간          ", pOutBlock->proctm         , sizeof( pOutBlock->proctm          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "메세지코드          ", pOutBlock->msgcode        , sizeof( pOutBlock->msgcode         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "메세지출력구분      ", pOutBlock->outgu          , sizeof( pOutBlock->outgu           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "압축요청구분        ", pOutBlock->compreq        , sizeof( pOutBlock->compreq         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "기능키              ", pOutBlock->funckey        , sizeof( pOutBlock->funckey         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "요청레코드개수      ", pOutBlock->reqcnt         , sizeof( pOutBlock->reqcnt          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "예비영역            ", pOutBlock->filler         , sizeof( pOutBlock->filler          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "연속구분            ", pOutBlock->cont           , sizeof( pOutBlock->cont            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "연속키값            ", pOutBlock->contkey        , sizeof( pOutBlock->contkey         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "가변시스템길이      ", pOutBlock->varlen         , sizeof( pOutBlock->varlen          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "가변해더길이        ", pOutBlock->varhdlen       , sizeof( pOutBlock->varhdlen        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "가변메시지길이      ", pOutBlock->varmsglen      , sizeof( pOutBlock->varmsglen       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "조회발원지          ", pOutBlock->trsrc          , sizeof( pOutBlock->trsrc           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "I/F이벤트ID         ", pOutBlock->eventid        , sizeof( pOutBlock->eventid         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "I/F정보             ", pOutBlock->ifinfo         , sizeof( pOutBlock->ifinfo          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "예비영역            ", pOutBlock->filler1        , sizeof( pOutBlock->filler1         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "주문체결구분        ", pOutBlock->ordchegb       , sizeof( pOutBlock->ordchegb        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "시장구분            ", pOutBlock->marketgb       , sizeof( pOutBlock->marketgb        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "주문구분            ", pOutBlock->ordgb          , sizeof( pOutBlock->ordgb           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "원주문번호          ", pOutBlock->orgordno       , sizeof( pOutBlock->orgordno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "계좌번호            ", pOutBlock->accno1         , sizeof( pOutBlock->accno1          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "계좌번호            ", pOutBlock->accno2         , sizeof( pOutBlock->accno2          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "비밀번호            ", pOutBlock->passwd         , sizeof( pOutBlock->passwd          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "종목번호            ", pOutBlock->expcode        , sizeof( pOutBlock->expcode         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "단축종목번호        ", pOutBlock->shtcode        , sizeof( pOutBlock->shtcode         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "종목명              ", pOutBlock->hname          , sizeof( pOutBlock->hname           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "주문수량            ", pOutBlock->ordqty         , sizeof( pOutBlock->ordqty          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문가격            ", pOutBlock->ordprice       , sizeof( pOutBlock->ordprice        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문조건            ", pOutBlock->hogagb         , sizeof( pOutBlock->hogagb          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "호가유형코드        ", pOutBlock->etfhogagb      , sizeof( pOutBlock->etfhogagb       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "프로그램호가구분    ", pOutBlock->pgmtype        , sizeof( pOutBlock->pgmtype         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "공매도호가구분      ", pOutBlock->gmhogagb       , sizeof( pOutBlock->gmhogagb        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "공매도가능여부      ", pOutBlock->gmhogayn       , sizeof( pOutBlock->gmhogayn        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "신용구분            ", pOutBlock->singb          , sizeof( pOutBlock->singb           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "대출일              ", pOutBlock->loandt         , sizeof( pOutBlock->loandt          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "반대매매주문구분    ", pOutBlock->cvrgordtp      , sizeof( pOutBlock->cvrgordtp       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "전략코드            ", pOutBlock->strtgcode      , sizeof( pOutBlock->strtgcode       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "그룹ID              ", pOutBlock->groupid        , sizeof( pOutBlock->groupid         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "주문회차            ", pOutBlock->ordseqno       , sizeof( pOutBlock->ordseqno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "포트폴리오번호      ", pOutBlock->prtno          , sizeof( pOutBlock->prtno           ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "바스켓번호          ", pOutBlock->basketno       , sizeof( pOutBlock->basketno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "트렌치번호          ", pOutBlock->trchno         , sizeof( pOutBlock->trchno          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "아아템번호          ", pOutBlock->itemno         , sizeof( pOutBlock->itemno          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "차입구분            ", pOutBlock->brwmgmyn       , sizeof( pOutBlock->brwmgmyn        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "회원사번호          ", pOutBlock->mbrno          , sizeof( pOutBlock->mbrno           ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "처리구분            ", pOutBlock->procgb         , sizeof( pOutBlock->procgb          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "관리지점번호        ", pOutBlock->admbrchno      , sizeof( pOutBlock->admbrchno       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "선물계좌번호        ", pOutBlock->futaccno       , sizeof( pOutBlock->futaccno        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "선물상품구분        ", pOutBlock->futmarketgb    , sizeof( pOutBlock->futmarketgb     ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "통신매체구분        ", pOutBlock->tongsingb      , sizeof( pOutBlock->tongsingb       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "유동성공급자구분    ", pOutBlock->lpgb           , sizeof( pOutBlock->lpgb            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "DUMMY               ", pOutBlock->dummy          , sizeof( pOutBlock->dummy           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "주문번호            ", pOutBlock->ordno          , sizeof( pOutBlock->ordno           ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문시각            ", pOutBlock->ordtm          , sizeof( pOutBlock->ordtm           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "모주문번호          ", pOutBlock->prntordno      , sizeof( pOutBlock->prntordno       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "관리사원번호        ", pOutBlock->mgempno        , sizeof( pOutBlock->mgempno         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "원주문미체결수량    ", pOutBlock->orgordundrqty  , sizeof( pOutBlock->orgordundrqty   ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "원주문정정수량      ", pOutBlock->orgordmdfyqty  , sizeof( pOutBlock->orgordmdfyqty   ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "원주문취소수량      ", pOutBlock->ordordcancelqty, sizeof( pOutBlock->ordordcancelqty ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "비회원사송신번호    ", pOutBlock->nmcpysndno     , sizeof( pOutBlock->nmcpysndno      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문금액            ", pOutBlock->ordamt         , sizeof( pOutBlock->ordamt          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "매매구분            ", pOutBlock->bnstp          , sizeof( pOutBlock->bnstp           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "예비주문번호        ", pOutBlock->spareordno     , sizeof( pOutBlock->spareordno      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "반대매매일련번호    ", pOutBlock->cvrgseqno      , sizeof( pOutBlock->cvrgseqno       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "예약주문번호        ", pOutBlock->rsvordno       , sizeof( pOutBlock->rsvordno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "복수주문일련번호    ", pOutBlock->mtordseqno     , sizeof( pOutBlock->mtordseqno      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "예비주문수량        ", pOutBlock->spareordqty    , sizeof( pOutBlock->spareordqty     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문사원번호        ", pOutBlock->orduserid      , sizeof( pOutBlock->orduserid       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "실물주문수량        ", pOutBlock->spotordqty     , sizeof( pOutBlock->spotordqty      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "재사용주문수량      ", pOutBlock->ordruseqty     , sizeof( pOutBlock->ordruseqty      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "현금주문금액        ", pOutBlock->mnyordamt      , sizeof( pOutBlock->mnyordamt       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문대용금액        ", pOutBlock->ordsubstamt    , sizeof( pOutBlock->ordsubstamt     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "재사용주문금액      ", pOutBlock->ruseordamt     , sizeof( pOutBlock->ruseordamt      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "수수료주문금액      ", pOutBlock->ordcmsnamt     , sizeof( pOutBlock->ordcmsnamt      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "사용신용담보재사용금", pOutBlock->crdtuseamt     , sizeof( pOutBlock->crdtuseamt      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "잔고수량            ", pOutBlock->secbalqty      , sizeof( pOutBlock->secbalqty       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "실물가능수량        ", pOutBlock->spotordableqty , sizeof( pOutBlock->spotordableqty  ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "재사용가능수량(매도)", pOutBlock->ordableruseqty , sizeof( pOutBlock->ordableruseqty  ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "변동수량            ", pOutBlock->flctqty        , sizeof( pOutBlock->flctqty         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "잔고수량(D2)        ", pOutBlock->secbalqtyd2    , sizeof( pOutBlock->secbalqtyd2     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "매도주문가능수량    ", pOutBlock->sellableqty    , sizeof( pOutBlock->sellableqty     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "미체결매도주문수량  ", pOutBlock->unercsellordqty, sizeof( pOutBlock->unercsellordqty ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "평균매입가          ", pOutBlock->avrpchsprc     , sizeof( pOutBlock->avrpchsprc      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "매입금액            ", pOutBlock->pchsamt        , sizeof( pOutBlock->pchsamt         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "예수금              ", pOutBlock->deposit        , sizeof( pOutBlock->deposit         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "대용금              ", pOutBlock->substamt       , sizeof( pOutBlock->substamt        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "위탁증거금현금      ", pOutBlock->csgnmnymgn     , sizeof( pOutBlock->csgnmnymgn      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "위탁증거금대용      ", pOutBlock->csgnsubstmgn   , sizeof( pOutBlock->csgnsubstmgn    ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "신용담보재사용금    ", pOutBlock->crdtpldgruseamt, sizeof( pOutBlock->crdtpldgruseamt ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문가능현금        ", pOutBlock->ordablemny     , sizeof( pOutBlock->ordablemny      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "주문가능대용        ", pOutBlock->ordablesubstamt, sizeof( pOutBlock->ordablesubstamt ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "재사용가능금액      ", pOutBlock->ruseableamt    , sizeof( pOutBlock->ruseableamt     ), DATA_TYPE_LONG  , 0 );*/
}

void CSample_xingACEDlg::DetailLogSC1( LPCTSTR pszData )
{
	//LPSC1_OutBlock pOutBlock = (LPSC1_OutBlock)pszData;

	//SetDetailLogData( "라인일련번호        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "조작자ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "헤더길이            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "헤더구분            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축구분            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "암호구분            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공통시작지점        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "이용사번호          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사용자ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "접속매체            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F일련번호         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전문일련번호        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR추적ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공인IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사설IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "처리지점번호        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "지점번호            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "단말번호            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "언어구분            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "AP처리시간          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "메세지코드          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "메세지출력구분      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축요청구분        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "기능키              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "요청레코드개수      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속구분            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속키값            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "가변시스템길이      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변해더길이        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변메시지길이      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "조회발원지          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F이벤트ID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F정보             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문체결유형코드    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문시장코드        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문유형코드        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리지점번호        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌명              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목번호            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목명              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문번호            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문번호          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결번호            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문수량            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가격            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결수량            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결가격            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인수량        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인가격        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "취소확인수량        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "거부수량            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문처리유형코드    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "복수주문일련번호    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문조건            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "호가유형코드        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "비저축체결수량      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "단축종목번호        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "운용지시번호        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "반대매매주문구분    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "미체결수량(주문)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문미체결수량    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문정정수량      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문취소수량      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문평균체결가격    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문금액            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "표준종목번호        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전표준종목번호      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "매매구분            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문거래유형코드    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "신용거래코드        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "수수료합산코드      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "통신매체코드        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "대출일              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "회원/비회원사번호   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문계좌번호        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "집계지점번호        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리사원번호        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계지점번호    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계계좌번호    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물시장구분        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "등록시장코드        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "현금증거금률        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용증거금률        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "현금체결금액        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용체결금액        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "수수료체결금액      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보체결금액    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용체결금액        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전일재사용체결금액  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "금일재사용체결금액  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물체결수량        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "공매도체결수량      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전략코드            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "그룹Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문회차            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "포트폴리오번호      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "바스켓번호          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "트렌치번호          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "아이템번호          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문자Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "차입관리여부        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "외국인고유번호      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래세징수구분      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "유동성공급자구분    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "체결시각            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래소수신체결시각  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "잔여대출금액        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물가능수량        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능수량(매도)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "변동수량            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도주문가능수량    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "미체결매도주문수량  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "평균매입가          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매입금액            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예수금              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용금              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금현금      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금대용      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보재사용금    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능현금        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능대용        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능금액      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogSC2( LPCTSTR pszData )
{
	//LPSC2_OutBlock pOutBlock = (LPSC2_OutBlock)pszData;

	//SetDetailLogData( "라인일련번호        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "조작자ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "헤더길이            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "헤더구분            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축구분            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "암호구분            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공통시작지점        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "이용사번호          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사용자ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "접속매체            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F일련번호         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전문일련번호        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR추적ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공인IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사설IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "처리지점번호        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "지점번호            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "단말번호            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "언어구분            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "AP처리시간          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "메세지코드          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "메세지출력구분      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축요청구분        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "기능키              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "요청레코드개수      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속구분            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속키값            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "가변시스템길이      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변해더길이        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변메시지길이      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "조회발원지          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F이벤트ID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F정보             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문체결유형코드    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문시장코드        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문유형코드        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리지점번호        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌명              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목번호            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목명              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문번호            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문번호          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결번호            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문수량            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가격            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결수량            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결가격            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인수량        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인가격        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "취소확인수량        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "거부수량            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문처리유형코드    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "복수주문일련번호    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문조건            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "호가유형코드        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "비저축체결수량      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "단축종목번호        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "운용지시번호        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "반대매매주문구분    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "미체결수량(주문)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문미체결수량    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문정정수량      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문취소수량      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문평균체결가격    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문금액            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "표준종목번호        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전표준종목번호      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "매매구분            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문거래유형코드    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "신용거래코드        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "수수료합산코드      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "통신매체코드        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "대출일              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "회원/비회원사번호   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문계좌번호        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "집계지점번호        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리사원번호        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계지점번호    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계계좌번호    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물시장구분        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "등록시장코드        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "현금증거금률        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용증거금률        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "현금체결금액        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용체결금액        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "수수료체결금액      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보체결금액    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용체결금액        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전일재사용체결금액  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "금일재사용체결금액  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물체결수량        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "공매도체결수량      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전략코드            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "그룹Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문회차            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "포트폴리오번호      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "바스켓번호          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "트렌치번호          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "아이템번호          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문자Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "차입관리여부        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "외국인고유번호      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래세징수구분      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "유동성공급자구분    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "체결시각            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래소수신체결시각  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "잔여대출금액        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물가능수량        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능수량(매도)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "변동수량            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도주문가능수량    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "미체결매도주문수량  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "평균매입가          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매입금액            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예수금              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용금              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금현금      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금대용      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보재사용금    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능현금        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능대용        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능금액      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogSC3( LPCTSTR pszData )
{
	//LPSC3_OutBlock pOutBlock = (LPSC3_OutBlock)pszData;

	//SetDetailLogData( "라인일련번호        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "조작자ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "헤더길이            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "헤더구분            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축구분            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "암호구분            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공통시작지점        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "이용사번호          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사용자ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "접속매체            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F일련번호         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전문일련번호        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR추적ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공인IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사설IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "처리지점번호        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "지점번호            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "단말번호            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "언어구분            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "AP처리시간          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "메세지코드          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "메세지출력구분      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축요청구분        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "기능키              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "요청레코드개수      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속구분            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속키값            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "가변시스템길이      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변해더길이        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변메시지길이      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "조회발원지          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F이벤트ID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F정보             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문체결유형코드    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문시장코드        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문유형코드        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리지점번호        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌명              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목번호            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목명              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문번호            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문번호          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결번호            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문수량            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가격            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결수량            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결가격            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인수량        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인가격        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "취소확인수량        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "거부수량            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문처리유형코드    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "복수주문일련번호    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문조건            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "호가유형코드        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "비저축체결수량      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "단축종목번호        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "운용지시번호        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "반대매매주문구분    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "미체결수량(주문)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문미체결수량    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문정정수량      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문취소수량      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문평균체결가격    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문금액            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "표준종목번호        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전표준종목번호      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "매매구분            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문거래유형코드    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "신용거래코드        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "수수료합산코드      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "통신매체코드        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "대출일              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "회원/비회원사번호   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문계좌번호        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "집계지점번호        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리사원번호        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계지점번호    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계계좌번호    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물시장구분        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "등록시장코드        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "현금증거금률        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용증거금률        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "현금체결금액        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용체결금액        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "수수료체결금액      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보체결금액    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용체결금액        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전일재사용체결금액  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "금일재사용체결금액  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물체결수량        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "공매도체결수량      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전략코드            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "그룹Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문회차            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "포트폴리오번호      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "바스켓번호          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "트렌치번호          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "아이템번호          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문자Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "차입관리여부        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "외국인고유번호      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래세징수구분      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "유동성공급자구분    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "체결시각            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래소수신체결시각  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "잔여대출금액        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물가능수량        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능수량(매도)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "변동수량            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도주문가능수량    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "미체결매도주문수량  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "평균매입가          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매입금액            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예수금              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용금              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금현금      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금대용      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보재사용금    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능현금        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능대용        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능금액      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogSC4( LPCTSTR pszData )
{
	//LPSC4_OutBlock pOutBlock = (LPSC4_OutBlock)pszData;

	//SetDetailLogData( "라인일련번호        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "조작자ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "헤더길이            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "헤더구분            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축구분            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "암호구분            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공통시작지점        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "이용사번호          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사용자ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "접속매체            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F일련번호         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전문일련번호        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR추적ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "공인IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "사설IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "처리지점번호        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "지점번호            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "단말번호            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "언어구분            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "AP처리시간          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "메세지코드          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "메세지출력구분      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "압축요청구분        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "기능키              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "요청레코드개수      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속구분            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "연속키값            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "가변시스템길이      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변해더길이        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "가변메시지길이      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "조회발원지          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F이벤트ID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F정보             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "예비영역            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문체결유형코드    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문시장코드        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문유형코드        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리지점번호        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌번호            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "계좌명              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목번호            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "종목명              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문번호            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문번호          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결번호            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문수량            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가격            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결수량            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결가격            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인수량        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "정정확인가격        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "취소확인수량        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "거부수량            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문처리유형코드    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "복수주문일련번호    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문조건            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "호가유형코드        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "비저축체결수량      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "단축종목번호        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "운용지시번호        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "반대매매주문구분    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "미체결수량(주문)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문미체결수량    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문정정수량      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "원주문취소수량      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문평균체결가격    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문금액            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "표준종목번호        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전표준종목번호      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "매매구분            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문거래유형코드    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "신용거래코드        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "수수료합산코드      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "통신매체코드        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "대출일              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "회원/비회원사번호   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문계좌번호        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "집계지점번호        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "관리사원번호        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계지점번호    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물연계계좌번호    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "선물시장구분        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "등록시장코드        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "현금증거금률        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용증거금률        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "현금체결금액        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용체결금액        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "수수료체결금액      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보체결금액    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용체결금액        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전일재사용체결금액  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "금일재사용체결금액  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물체결수량        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "공매도체결수량      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "전략코드            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "그룹Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "주문회차            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "포트폴리오번호      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "바스켓번호          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "트렌치번호          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "아이템번호          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문자Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "차입관리여부        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "외국인고유번호      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래세징수구분      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "유동성공급자구분    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "체결시각            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "거래소수신체결시각  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "잔여대출금액        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "실물가능수량        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능수량(매도)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "변동수량            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "잔고수량(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도주문가능수량    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "미체결매도주문수량  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "평균매입가          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매입금액            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "예수금              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "대용금              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금현금      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "위탁증거금대용      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "신용담보재사용금    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능현금        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "주문가능대용        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "재사용가능금액      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogS3_( LPCTSTR pszData )
{
	//LPS3__OutBlock pOutBlock = (LPS3__OutBlock)pszData;

	//SetDetailLogData( "체결시간          ", pOutBlock->chetime   , sizeof( pOutBlock->chetime    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전일대비구분      ", pOutBlock->sign      , sizeof( pOutBlock->sign       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전일대비          ", pOutBlock->change    , sizeof( pOutBlock->change     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "등락율            ", pOutBlock->drate     , sizeof( pOutBlock->drate      ), DATA_TYPE_FLOAT , 2 );
	//SetDetailLogData( "현재가            ", pOutBlock->price     , sizeof( pOutBlock->price      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "시가시간          ", pOutBlock->opentime  , sizeof( pOutBlock->opentime   ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "시가              ", pOutBlock->open      , sizeof( pOutBlock->open       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "고가시간          ", pOutBlock->hightime  , sizeof( pOutBlock->hightime   ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "고가              ", pOutBlock->high      , sizeof( pOutBlock->high       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "저가시간          ", pOutBlock->lowtime   , sizeof( pOutBlock->lowtime    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "저가              ", pOutBlock->low       , sizeof( pOutBlock->low        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결구분          ", pOutBlock->cgubun    , sizeof( pOutBlock->cgubun     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "체결량            ", pOutBlock->cvolume   , sizeof( pOutBlock->cvolume    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "누적거래량        ", pOutBlock->volume    , sizeof( pOutBlock->volume     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "누적거래대금      ", pOutBlock->value     , sizeof( pOutBlock->value      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도누적체결량    ", pOutBlock->mdvolume  , sizeof( pOutBlock->mdvolume   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도누적체결건수  ", pOutBlock->mdchecnt  , sizeof( pOutBlock->mdchecnt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수누적체결량    ", pOutBlock->msvolume  , sizeof( pOutBlock->msvolume   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수누적체결건수  ", pOutBlock->mschecnt  , sizeof( pOutBlock->mschecnt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "체결강도          ", pOutBlock->cpower    , sizeof( pOutBlock->cpower     ), DATA_TYPE_FLOAT , 2 );
	//SetDetailLogData( "가중평균가        ", pOutBlock->w_avrg    , sizeof( pOutBlock->w_avrg     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가          ", pOutBlock->offerho   , sizeof( pOutBlock->offerho    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가          ", pOutBlock->bidho     , sizeof( pOutBlock->bidho      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "장정보            ", pOutBlock->status    , sizeof( pOutBlock->status     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "전일동시간대거래량", pOutBlock->jnilvolume, sizeof( pOutBlock->jnilvolume ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "단축코드          ", pOutBlock->shcode    , sizeof( pOutBlock->shcode     ), DATA_TYPE_STRING, 0 );
}

void CSample_xingACEDlg::DetailLogH1_( LPCTSTR pszData )
{
	//LPH1__OutBlock pOutBlock = (LPH1__OutBlock)pszData;

	//SetDetailLogData( "호가시간      ", pOutBlock->hotime     , sizeof( pOutBlock->hotime      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "매도호가1     ", pOutBlock->offerho1   , sizeof( pOutBlock->offerho1    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가1     ", pOutBlock->bidho1     , sizeof( pOutBlock->bidho1      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량1 ", pOutBlock->offerrem1  , sizeof( pOutBlock->offerrem1   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량1 ", pOutBlock->bidrem1    , sizeof( pOutBlock->bidrem1     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가2     ", pOutBlock->offerho2   , sizeof( pOutBlock->offerho2    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가2     ", pOutBlock->bidho2     , sizeof( pOutBlock->bidho2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량2 ", pOutBlock->offerrem2  , sizeof( pOutBlock->offerrem2   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량2 ", pOutBlock->bidrem2    , sizeof( pOutBlock->bidrem2     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가3     ", pOutBlock->offerho3   , sizeof( pOutBlock->offerho3    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가3     ", pOutBlock->bidho3     , sizeof( pOutBlock->bidho3      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량3 ", pOutBlock->offerrem3  , sizeof( pOutBlock->offerrem3   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량3 ", pOutBlock->bidrem3    , sizeof( pOutBlock->bidrem3     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가4     ", pOutBlock->offerho4   , sizeof( pOutBlock->offerho4    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가4     ", pOutBlock->bidho4     , sizeof( pOutBlock->bidho4      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량4 ", pOutBlock->offerrem4  , sizeof( pOutBlock->offerrem4   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량4 ", pOutBlock->bidrem4    , sizeof( pOutBlock->bidrem4     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가5     ", pOutBlock->offerho5   , sizeof( pOutBlock->offerho5    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가5     ", pOutBlock->bidho5     , sizeof( pOutBlock->bidho5      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량5 ", pOutBlock->offerrem5  , sizeof( pOutBlock->offerrem5   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량5 ", pOutBlock->bidrem5    , sizeof( pOutBlock->bidrem5     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가6     ", pOutBlock->offerho6   , sizeof( pOutBlock->offerho6    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가6     ", pOutBlock->bidho6     , sizeof( pOutBlock->bidho6      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량6 ", pOutBlock->offerrem6  , sizeof( pOutBlock->offerrem6   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량6 ", pOutBlock->bidrem6    , sizeof( pOutBlock->bidrem6     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가7     ", pOutBlock->offerho7   , sizeof( pOutBlock->offerho7    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가7     ", pOutBlock->bidho7     , sizeof( pOutBlock->bidho7      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량7 ", pOutBlock->offerrem7  , sizeof( pOutBlock->offerrem7   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량7 ", pOutBlock->bidrem7    , sizeof( pOutBlock->bidrem7     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가8     ", pOutBlock->offerho8   , sizeof( pOutBlock->offerho8    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가8     ", pOutBlock->bidho8     , sizeof( pOutBlock->bidho8      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량8 ", pOutBlock->offerrem8  , sizeof( pOutBlock->offerrem8   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량8 ", pOutBlock->bidrem8    , sizeof( pOutBlock->bidrem8     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가9     ", pOutBlock->offerho9   , sizeof( pOutBlock->offerho9    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가9     ", pOutBlock->bidho9     , sizeof( pOutBlock->bidho9      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량9 ", pOutBlock->offerrem9  , sizeof( pOutBlock->offerrem9   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량9 ", pOutBlock->bidrem9    , sizeof( pOutBlock->bidrem9     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가10    ", pOutBlock->offerho10  , sizeof( pOutBlock->offerho10   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가10    ", pOutBlock->bidho10    , sizeof( pOutBlock->bidho10     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매도호가잔량10", pOutBlock->offerrem10 , sizeof( pOutBlock->offerrem10  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "매수호가잔량10", pOutBlock->bidrem10   , sizeof( pOutBlock->bidrem10    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "총매도호가잔량", pOutBlock->totofferrem, sizeof( pOutBlock->totofferrem ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "총매수호가잔량", pOutBlock->totbidrem  , sizeof( pOutBlock->totbidrem   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "동시호가구분  ", pOutBlock->donsigubun , sizeof( pOutBlock->donsigubun  ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "단축코드      ", pOutBlock->shcode     , sizeof( pOutBlock->shcode      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "배분적용구분  ", pOutBlock->alloc_gubun, sizeof( pOutBlock->alloc_gubun ), DATA_TYPE_STRING, 0 );
}

void CSample_xingACEDlg::DetailLogK3_( LPCTSTR pszData )
{
//	LPK3__OutBlock pOutBlock = (LPK3__OutBlock)pszData;
//
//	SetDetailLogData( "체결시간          ", pOutBlock->chetime   , sizeof( pOutBlock->chetime    ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "전일대비구분      ", pOutBlock->sign      , sizeof( pOutBlock->sign       ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "전일대비          ", pOutBlock->change    , sizeof( pOutBlock->change     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "등락율            ", pOutBlock->drate     , sizeof( pOutBlock->drate      ), DATA_TYPE_FLOAT , 2 );
//	SetDetailLogData( "현재가            ", pOutBlock->price     , sizeof( pOutBlock->price      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "시가시간          ", pOutBlock->opentime  , sizeof( pOutBlock->opentime   ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "시가              ", pOutBlock->open      , sizeof( pOutBlock->open       ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "고가시간          ", pOutBlock->hightime  , sizeof( pOutBlock->hightime   ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "고가              ", pOutBlock->high      , sizeof( pOutBlock->high       ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "저가시간          ", pOutBlock->lowtime   , sizeof( pOutBlock->lowtime    ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "저가              ", pOutBlock->low       , sizeof( pOutBlock->low        ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "체결구분          ", pOutBlock->cgubun    , sizeof( pOutBlock->cgubun     ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "체결량            ", pOutBlock->cvolume   , sizeof( pOutBlock->cvolume    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "누적거래량        ", pOutBlock->volume    , sizeof( pOutBlock->volume     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "누적거래대금      ", pOutBlock->value     , sizeof( pOutBlock->value      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도누적체결량    ", pOutBlock->mdvolume  , sizeof( pOutBlock->mdvolume   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도누적체결건수  ", pOutBlock->mdchecnt  , sizeof( pOutBlock->mdchecnt   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수누적체결량    ", pOutBlock->msvolume  , sizeof( pOutBlock->msvolume   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수누적체결건수  ", pOutBlock->mschecnt  , sizeof( pOutBlock->mschecnt   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "체결강도          ", pOutBlock->cpower    , sizeof( pOutBlock->cpower     ), DATA_TYPE_FLOAT , 2 );
//	SetDetailLogData( "가중평균가        ", pOutBlock->w_avrg    , sizeof( pOutBlock->w_avrg     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가          ", pOutBlock->offerho   , sizeof( pOutBlock->offerho    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가          ", pOutBlock->bidho     , sizeof( pOutBlock->bidho      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "장정보            ", pOutBlock->status    , sizeof( pOutBlock->status     ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "전일동시간대거래량", pOutBlock->jnilvolume, sizeof( pOutBlock->jnilvolume ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "단축코드          ", pOutBlock->shcode    , sizeof( pOutBlock->shcode     ), DATA_TYPE_STRING, 0 );
}
//
void CSample_xingACEDlg::DetailLogHA_( LPCTSTR pszData)
{
//	LPHA__OutBlock pOutBlock = (LPHA__OutBlock)pszData;
//
//	SetDetailLogData( "호가시간      ", pOutBlock->hotime     , sizeof( pOutBlock->hotime      ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "매도호가1     ", pOutBlock->offerho1   , sizeof( pOutBlock->offerho1    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가1     ", pOutBlock->bidho1     , sizeof( pOutBlock->bidho1      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량1 ", pOutBlock->offerrem1  , sizeof( pOutBlock->offerrem1   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량1 ", pOutBlock->bidrem1    , sizeof( pOutBlock->bidrem1     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가2     ", pOutBlock->offerho2   , sizeof( pOutBlock->offerho2    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가2     ", pOutBlock->bidho2     , sizeof( pOutBlock->bidho2      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량2 ", pOutBlock->offerrem2  , sizeof( pOutBlock->offerrem2   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량2 ", pOutBlock->bidrem2    , sizeof( pOutBlock->bidrem2     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가3     ", pOutBlock->offerho3   , sizeof( pOutBlock->offerho3    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가3     ", pOutBlock->bidho3     , sizeof( pOutBlock->bidho3      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량3 ", pOutBlock->offerrem3  , sizeof( pOutBlock->offerrem3   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량3 ", pOutBlock->bidrem3    , sizeof( pOutBlock->bidrem3     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가4     ", pOutBlock->offerho4   , sizeof( pOutBlock->offerho4    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가4     ", pOutBlock->bidho4     , sizeof( pOutBlock->bidho4      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량4 ", pOutBlock->offerrem4  , sizeof( pOutBlock->offerrem4   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량4 ", pOutBlock->bidrem4    , sizeof( pOutBlock->bidrem4     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가5     ", pOutBlock->offerho5   , sizeof( pOutBlock->offerho5    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가5     ", pOutBlock->bidho5     , sizeof( pOutBlock->bidho5      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량5 ", pOutBlock->offerrem5  , sizeof( pOutBlock->offerrem5   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량5 ", pOutBlock->bidrem5    , sizeof( pOutBlock->bidrem5     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가6     ", pOutBlock->offerho6   , sizeof( pOutBlock->offerho6    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가6     ", pOutBlock->bidho6     , sizeof( pOutBlock->bidho6      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량6 ", pOutBlock->offerrem6  , sizeof( pOutBlock->offerrem6   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량6 ", pOutBlock->bidrem6    , sizeof( pOutBlock->bidrem6     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가7     ", pOutBlock->offerho7   , sizeof( pOutBlock->offerho7    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가7     ", pOutBlock->bidho7     , sizeof( pOutBlock->bidho7      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량7 ", pOutBlock->offerrem7  , sizeof( pOutBlock->offerrem7   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량7 ", pOutBlock->bidrem7    , sizeof( pOutBlock->bidrem7     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가8     ", pOutBlock->offerho8   , sizeof( pOutBlock->offerho8    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가8     ", pOutBlock->bidho8     , sizeof( pOutBlock->bidho8      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량8 ", pOutBlock->offerrem8  , sizeof( pOutBlock->offerrem8   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량8 ", pOutBlock->bidrem8    , sizeof( pOutBlock->bidrem8     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가9     ", pOutBlock->offerho9   , sizeof( pOutBlock->offerho9    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가9     ", pOutBlock->bidho9     , sizeof( pOutBlock->bidho9      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량9 ", pOutBlock->offerrem9  , sizeof( pOutBlock->offerrem9   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량9 ", pOutBlock->bidrem9    , sizeof( pOutBlock->bidrem9     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가10    ", pOutBlock->offerho10  , sizeof( pOutBlock->offerho10   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가10    ", pOutBlock->bidho10    , sizeof( pOutBlock->bidho10     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매도호가잔량10", pOutBlock->offerrem10 , sizeof( pOutBlock->offerrem10  ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "매수호가잔량10", pOutBlock->bidrem10   , sizeof( pOutBlock->bidrem10    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "총매도호가잔량", pOutBlock->totofferrem, sizeof( pOutBlock->totofferrem ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "총매수호가잔량", pOutBlock->totbidrem  , sizeof( pOutBlock->totbidrem   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "동시호가구분  ", pOutBlock->donsigubun , sizeof( pOutBlock->donsigubun  ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "단축코드      ", pOutBlock->shcode     , sizeof( pOutBlock->shcode      ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "배분적용구분  ", pOutBlock->alloc_gubun, sizeof( pOutBlock->alloc_gubun ), DATA_TYPE_STRING, 0 );
}

void CSample_xingACEDlg::ReceiveRealS3_( LPCTSTR pszData )
{
	//LPS3__OutBlock pOutBlock = (LPS3__OutBlock)pszData;

	//m_ctrlChe.InsertItem( 0, "" );
	//m_ctrlChe.SetItemText( 0, 0, GetDispData( pOutBlock->price     , sizeof( pOutBlock->price      ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlChe.SetItemText( 0, 1, GetDispData( pOutBlock->cvolume   , sizeof( pOutBlock->cvolume    ), DATA_TYPE_LONG  , 0 ) );
}

void CSample_xingACEDlg::ReceiveRealH1_( LPCTSTR pszData )
{
//	LPH1__OutBlock pOutBlock = (LPH1__OutBlock)pszData;
//
//
//	m_ctrlHoga.SetItemText( 0, 0, GetDispData( pOutBlock->offerrem10, sizeof( pOutBlock->offerrem10 ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 1, 0, GetDispData( pOutBlock->offerrem9 , sizeof( pOutBlock->offerrem9  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 2, 0, GetDispData( pOutBlock->offerrem8 , sizeof( pOutBlock->offerrem8  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 3, 0, GetDispData( pOutBlock->offerrem7 , sizeof( pOutBlock->offerrem7  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 4, 0, GetDispData( pOutBlock->offerrem6 , sizeof( pOutBlock->offerrem6  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 5, 0, GetDispData( pOutBlock->offerrem5 , sizeof( pOutBlock->offerrem5  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 6, 0, GetDispData( pOutBlock->offerrem4 , sizeof( pOutBlock->offerrem4  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 7, 0, GetDispData( pOutBlock->offerrem3 , sizeof( pOutBlock->offerrem3  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 8, 0, GetDispData( pOutBlock->offerrem2 , sizeof( pOutBlock->offerrem2  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 9, 0, GetDispData( pOutBlock->offerrem1 , sizeof( pOutBlock->offerrem1  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(10, 2, GetDispData( pOutBlock->bidrem1   , sizeof( pOutBlock->bidrem1    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(11, 2, GetDispData( pOutBlock->bidrem2   , sizeof( pOutBlock->bidrem2    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(12, 2, GetDispData( pOutBlock->bidrem3   , sizeof( pOutBlock->bidrem3    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(13, 2, GetDispData( pOutBlock->bidrem4   , sizeof( pOutBlock->bidrem4    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(14, 2, GetDispData( pOutBlock->bidrem5   , sizeof( pOutBlock->bidrem5    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(15, 2, GetDispData( pOutBlock->bidrem6   , sizeof( pOutBlock->bidrem6    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(16, 2, GetDispData( pOutBlock->bidrem7   , sizeof( pOutBlock->bidrem7    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(17, 2, GetDispData( pOutBlock->bidrem8   , sizeof( pOutBlock->bidrem8    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(18, 2, GetDispData( pOutBlock->bidrem9   , sizeof( pOutBlock->bidrem9    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(19, 2, GetDispData( pOutBlock->bidrem10  , sizeof( pOutBlock->bidrem10   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 0, 1, GetDispData( pOutBlock->offerho10 , sizeof( pOutBlock->offerho10  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 1, 1, GetDispData( pOutBlock->offerho9  , sizeof( pOutBlock->offerho9   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 2, 1, GetDispData( pOutBlock->offerho8  , sizeof( pOutBlock->offerho8   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 3, 1, GetDispData( pOutBlock->offerho7  , sizeof( pOutBlock->offerho7   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 4, 1, GetDispData( pOutBlock->offerho6  , sizeof( pOutBlock->offerho6   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 5, 1, GetDispData( pOutBlock->offerho5  , sizeof( pOutBlock->offerho5   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 6, 1, GetDispData( pOutBlock->offerho4  , sizeof( pOutBlock->offerho4   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 7, 1, GetDispData( pOutBlock->offerho3  , sizeof( pOutBlock->offerho3   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 8, 1, GetDispData( pOutBlock->offerho2  , sizeof( pOutBlock->offerho2   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 9, 1, GetDispData( pOutBlock->offerho1  , sizeof( pOutBlock->offerho1   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(10, 1, GetDispData( pOutBlock->bidho1    , sizeof( pOutBlock->bidho1     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(11, 1, GetDispData( pOutBlock->bidho2    , sizeof( pOutBlock->bidho2     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(12, 1, GetDispData( pOutBlock->bidho3    , sizeof( pOutBlock->bidho3     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(13, 1, GetDispData( pOutBlock->bidho4    , sizeof( pOutBlock->bidho4     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(14, 1, GetDispData( pOutBlock->bidho5    , sizeof( pOutBlock->bidho5     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(15, 1, GetDispData( pOutBlock->bidho6    , sizeof( pOutBlock->bidho6     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(16, 1, GetDispData( pOutBlock->bidho7    , sizeof( pOutBlock->bidho7     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(17, 1, GetDispData( pOutBlock->bidho8    , sizeof( pOutBlock->bidho8     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(18, 1, GetDispData( pOutBlock->bidho9    , sizeof( pOutBlock->bidho9     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText(19, 1, GetDispData( pOutBlock->bidho10   , sizeof( pOutBlock->bidho10    ), DATA_TYPE_LONG  , 0 ) );
//*/
//	m_ctrlHoga.SetItemText( 0, 0, GetDispData( pOutBlock->offerrem5 , sizeof( pOutBlock->offerrem5  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 1, 0, GetDispData( pOutBlock->offerrem4 , sizeof( pOutBlock->offerrem4  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 2, 0, GetDispData( pOutBlock->offerrem3 , sizeof( pOutBlock->offerrem3  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 3, 0, GetDispData( pOutBlock->offerrem2 , sizeof( pOutBlock->offerrem2  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 4, 0, GetDispData( pOutBlock->offerrem1 , sizeof( pOutBlock->offerrem1  ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 5, 2, GetDispData( pOutBlock->bidrem1   , sizeof( pOutBlock->bidrem1    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 6, 2, GetDispData( pOutBlock->bidrem2   , sizeof( pOutBlock->bidrem2    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 7, 2, GetDispData( pOutBlock->bidrem3   , sizeof( pOutBlock->bidrem3    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 8, 2, GetDispData( pOutBlock->bidrem4   , sizeof( pOutBlock->bidrem4    ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 9, 2, GetDispData( pOutBlock->bidrem5   , sizeof( pOutBlock->bidrem5    ), DATA_TYPE_LONG  , 0 ) );
//	
//	m_ctrlHoga.SetItemText( 0, 1, GetDispData( pOutBlock->offerho5  , sizeof( pOutBlock->offerho5   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 1, 1, GetDispData( pOutBlock->offerho4  , sizeof( pOutBlock->offerho4   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 2, 1, GetDispData( pOutBlock->offerho3  , sizeof( pOutBlock->offerho3   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 3, 1, GetDispData( pOutBlock->offerho2  , sizeof( pOutBlock->offerho2   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 4, 1, GetDispData( pOutBlock->offerho1  , sizeof( pOutBlock->offerho1   ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 5, 1, GetDispData( pOutBlock->bidho1    , sizeof( pOutBlock->bidho1     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 6, 1, GetDispData( pOutBlock->bidho2    , sizeof( pOutBlock->bidho2     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 7, 1, GetDispData( pOutBlock->bidho3    , sizeof( pOutBlock->bidho3     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 8, 1, GetDispData( pOutBlock->bidho4    , sizeof( pOutBlock->bidho4     ), DATA_TYPE_LONG  , 0 ) );
//	m_ctrlHoga.SetItemText( 9, 1, GetDispData( pOutBlock->bidho5    , sizeof( pOutBlock->bidho5     ), DATA_TYPE_LONG  , 0 ) );
}

void CSample_xingACEDlg::ReceiveRealK3_( LPCTSTR pszData )
{
	//LPK3__OutBlock pOutBlock = (LPK3__OutBlock)pszData;

	//m_ctrlChe.InsertItem( 0, "" );
	//m_ctrlChe.SetItemText( 0, 0, GetDispData( pOutBlock->price     , sizeof( pOutBlock->price      ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlChe.SetItemText( 0, 1, GetDispData( pOutBlock->cvolume   , sizeof( pOutBlock->cvolume    ), DATA_TYPE_LONG  , 0 ) );
}

void CSample_xingACEDlg::ReceiveRealHA_( LPCTSTR pszData )
{
	//LPHA__OutBlock pOutBlock = (LPHA__OutBlock)pszData;

	//m_ctrlHoga.SetItemText( 0, 0, GetDispData( pOutBlock->offerrem5 , sizeof( pOutBlock->offerrem5  ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 1, 0, GetDispData( pOutBlock->offerrem4 , sizeof( pOutBlock->offerrem4  ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 2, 0, GetDispData( pOutBlock->offerrem3 , sizeof( pOutBlock->offerrem3  ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 3, 0, GetDispData( pOutBlock->offerrem2 , sizeof( pOutBlock->offerrem2  ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 4, 0, GetDispData( pOutBlock->offerrem1 , sizeof( pOutBlock->offerrem1  ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 5, 2, GetDispData( pOutBlock->bidrem1   , sizeof( pOutBlock->bidrem1    ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 6, 2, GetDispData( pOutBlock->bidrem2   , sizeof( pOutBlock->bidrem2    ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 7, 2, GetDispData( pOutBlock->bidrem3   , sizeof( pOutBlock->bidrem3    ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 8, 2, GetDispData( pOutBlock->bidrem4   , sizeof( pOutBlock->bidrem4    ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 9, 2, GetDispData( pOutBlock->bidrem5   , sizeof( pOutBlock->bidrem5    ), DATA_TYPE_LONG  , 0 ) );
	//
	//m_ctrlHoga.SetItemText( 0, 1, GetDispData( pOutBlock->offerho5  , sizeof( pOutBlock->offerho5   ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 1, 1, GetDispData( pOutBlock->offerho4  , sizeof( pOutBlock->offerho4   ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 2, 1, GetDispData( pOutBlock->offerho3  , sizeof( pOutBlock->offerho3   ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 3, 1, GetDispData( pOutBlock->offerho2  , sizeof( pOutBlock->offerho2   ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 4, 1, GetDispData( pOutBlock->offerho1  , sizeof( pOutBlock->offerho1   ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 5, 1, GetDispData( pOutBlock->bidho1    , sizeof( pOutBlock->bidho1     ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 6, 1, GetDispData( pOutBlock->bidho2    , sizeof( pOutBlock->bidho2     ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 7, 1, GetDispData( pOutBlock->bidho3    , sizeof( pOutBlock->bidho3     ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 8, 1, GetDispData( pOutBlock->bidho4    , sizeof( pOutBlock->bidho4     ), DATA_TYPE_LONG  , 0 ) );
	//m_ctrlHoga.SetItemText( 9, 1, GetDispData( pOutBlock->bidho5    , sizeof( pOutBlock->bidho5     ), DATA_TYPE_LONG  , 0 ) );
}
