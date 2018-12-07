// Dlg_FC0.cpp : implementation file
//

#include "stdafx.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "XingAPI_Sample.h"
#include "Dlg_FC0_1.h"

#include <assert.h>
//#include "../../include/API_ETK/FC0.h"
//#include "../../include/API_ETK/FH0.h"
//#include "../../include/API_ETK/OC0.h"
//#include "../../include/API_ETK/OH0.h"
//
////CME
//#include "../../include/API_ETK/NC0.h"
//#include "../../include/API_ETK/NH0.h"

//해외선물
#include "../Common/Inc_ETK/OVH.h"
#include "../Common/Inc_ETK/OVC.h"

#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_util/IRUM_Common.h"

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CLogDebug g_logEvent("ETK_F_X");
//char g_CnfgIdx[32]={0,};	// main 에서 parameter 로 받는 값

//SMILOR.2스레드
#include <list>
using namespace std;
//list<LPRECV_REAL_PACKET>		g_lstSise;

//CProp prop;
//CProp prop_hoga;

//int g_nCnt = 0;
/////////////////////////////////////////////////////////////////////////////
// CDlg_FC0_1 dialog

IMPLEMENT_DYNCREATE(CDlg_FC0_1, CDialog)

CDlg_FC0_1::CDlg_FC0_1(CWnd* pParent /*=NULL*/)	: CDialog(CDlg_FC0_1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_FC0_1)
	//}}AFX_DATA_INIT
}


void CDlg_FC0_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_FC0_1)
	DDX_Control(pDX, IDC_LIST_SISEHOGA, m_lstMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_FC0_1, CDialog)
	//{{AFX_MSG_MAP(CDlg_FC0_1)
	ON_BN_CLICKED(IDC_BUTTON_UNADVISE, OnButtonUnadvise)
	//ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest_1)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_FC0_1 message handlers

BOOL CDlg_FC0_1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();


	//////////////////////////////////////////////////////////////////////////
	//	종목정보 가져오기
	if( !LoadSymbols() ){
		g_log.log(NOTIFY, "Failed to get symbols info");
		return	FALSE;
	}

	// Create IOCP Threads for sending data to clients
	m_pClientIF = new CClientInterface();
	if(!m_pClientIF->Initialize()) {
		g_log.log(NOTIFY, "Failed to Initialize ClientIF");
		return	FALSE;
	}

	// request market data to ETK
	OnButtonRequest();

	char szMsg[128];
	sprintf( szMsg, "CME Futures Market Data(%s)", __DATE__ );
	g_log.log(LOGTP_SUCC,szMsg);
	m_lstMsg.InsertString(0, szMsg);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//	Load symbols to be subscribed from DB
BOOL CDlg_FC0_1::LoadSymbols()
{
	//////////////////////////////////////////////////////////////////////////
	//	DB OPEN 
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);
	CDBPoolAdo DBPool(ip, id, pwd, name);
	if (!DBPool.Init(1))
	{
		g_log.log(ERR, "DB Connect Error.(IP:%s)(ID:%s)(PWD:%s)(DBNAME:%s)", ip, id, pwd, name);
		return FALSE;
	}

	CDBHandlerAdo db(DBPool.Get());
	char zQ[1024];
	sprintf(zQ, "EXEC P_GET_STK_INFO ");
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR, "Load Symbol Error(%s)", zQ);
		return FALSE;
	}

	char szStkList[1024] = { 0, };
	char szStk[128] = { 0, };
	int nRs = 0;
	while (db->IsNextRow())
	{
		db->GetStr("GLID_CODE", szStk);

		CUtil::TrimAll(szStk, strlen(szStk));

		if (strlen(szStk) == 0)
			break;

		CString sStk = szStk;
		m_lstSymbols.push_back(sStk);
		g_log.log(LOGTP_SUCC, "SYMBOL(%s)", szStk);

		db->Next();
	}
	db->Close();

	return TRUE;
}


void CDlg_FC0_1::OnDestroy()
{
	OnButtonUnadvise();
	CDialog::OnDestroy();
}

/***************************************
	Request Market Data
***************************************/
void CDlg_FC0_1::OnButtonRequest() 
{
	//	선물 시세/호가 요청
	list<CString>::iterator it;
	for( it=m_lstSymbols.begin(); it!=m_lstSymbols.end(); it++ )
	{
		CString sStk = *it;
		AdviseData('Y'/*TICK*/, sStk);

		AdviseData('N'/*Quote*/, sStk);
	}

}


//--------------------------------------------------------------------------------------
// 컨트롤 초기화
//--------------------------------------------------------------------------------------
void CDlg_FC0_1::InitCtrls()
{
	//-------------------------------------------------------------------------
	// 선물 체결 용
// 	m_ctrlOutBlock.InsertColumn( 0, "필드  ", LVCFMT_LEFT, 150 );
// 	m_ctrlOutBlock.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );
// 
// 	int nRow = 0;
// 	m_ctrlOutBlock.InsertItem( nRow++, "체결시간          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "전일대비구분      " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "전일대비          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "등락율            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "현재가            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "시가              " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "고가              " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "저가              " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "체결구분          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "체결량            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "누적거래량        " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "누적거래대금      " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "매도누적체결량    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "매도누적체결건수  " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "매수누적체결량    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "매수누적체결건수  " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "체결강도          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "매도호가1         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "매수호가1         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "미결제약정수량    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "KOSPI200지수      " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "이론가            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "괴리율            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "시장BASIS         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "이론BASIS         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "미결제약정증감    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "장운영정보        " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "전일동시간대거래량" );
// 	m_ctrlOutBlock.InsertItem( nRow++, "단축코드          " );
// 
// 	//-------------------------------------------------------------------------
// 	// 선물 호가 용
// 	m_ctrlOutBlock_H.InsertColumn( 0, "필드  ", LVCFMT_LEFT, 150 );
// 	m_ctrlOutBlock_H.InsertColumn( 1, "데이터", LVCFMT_LEFT, 200 );
// 	
// 	nRow = 0;
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "호가시간          " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가1         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가2         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가3         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가4         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가5         " );	
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가1         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가2         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가3         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가4         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가5         " );
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가건수1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가건수2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가건수3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가건수4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가건수5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가총건수    " );	
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가건수1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가건수2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가건수3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가건수4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가건수5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가총건수    " );
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가수량1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가수량2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가수량3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가수량4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가수량5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매도호가총수량    " );	
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가수량1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가수량2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가수량3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가수량4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가수량5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "매수호가총수량    " );	
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "단축코드          " );

//	SetDlgItemText(IDC_BUTTON_TEST, "FILE TEST Start");	// skeo 2011-01-17
}

//--------------------------------------------------------------------------------------
// 데이터 Advise
//--------------------------------------------------------------------------------------
void CDlg_FC0_1::AdviseData(char i_cTickYN, CString i_sSymbol)
{
	TCHAR	szTrCode[32];
	int		nSize;

	OVC_InBlock inOVC;
	OVH_InBlock inOVH;

	if(i_cTickYN =='Y' )
	{
		strcpy( szTrCode, ETK_REAL_SISE_FU);
		nSize = sizeof( inOVC.symbol );
	}
	else
	{
		strcpy( szTrCode, ETK_REAL_HOGA_FU);
		nSize = sizeof( inOVH.symbol );
	}

	CString symbol;
	symbol.Format("%-*.*s", nSize, nSize, (LPCSTR)i_sSymbol);
	
	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		symbol,						// 종목코드
		nSize						// 종목코드 길이
		);

	//-----------------------------------------------------------
	// 에러체크
	char msg[512];
	if( bSuccess == FALSE )
	{
		if(i_cTickYN =='Y')	sprintf(msg,  "[%s] Faild to Request Tick", (LPCSTR)symbol);
		if(i_cTickYN =='N')	sprintf(msg,  "[%s] Faild to Request Quote", (LPCSTR)symbol);
		g_log.log(NOTIFY, msg);
	}
	else
	{
		if(i_cTickYN =='Y')	sprintf(msg,  "[%s] Succeed to Request Tick", (LPCSTR)symbol);
		if(i_cTickYN =='N')	sprintf(msg,  "[%s] Succeed to Request Quote", (LPCSTR)symbol);
		g_log.log(INFO, msg);
	}
	
	m_lstMsg.InsertString(0, msg);
}

//--------------------------------------------------------------------------------------
// 데이터 Unadvise
//--------------------------------------------------------------------------------------
void CDlg_FC0_1::UnadviseData(char i_cTickYN, CString i_sSymbol)
{
	TCHAR	szTrCode[32];
	int		nSize;

	if(i_cTickYN =='Y' )
	{
		strcpy( szTrCode, ETK_REAL_SISE_FU);
		nSize = sizeof( OVC_OutBlock );
	}
	else
	{
		strcpy( szTrCode, ETK_REAL_HOGA_FU);
		nSize = sizeof( OVH_OutBlock );
	}
	
	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
												GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
												szTrCode,					// TR 번호
		i_sSymbol,						// 종목코드
												nSize						// 종목코드 길이
												);
	
	//-----------------------------------------------------------
	// 에러체크
	char msg[512];
	if( bSuccess == FALSE )
	{
		if(i_cTickYN =='Y')	sprintf(msg,  "[%s] Failed to Unsubscribe Tick", (LPCSTR)i_sSymbol);
		if(i_cTickYN =='N')	sprintf(msg,  "[%s] Failed to Unsubscribe Quote", (LPCSTR)i_sSymbol);
	}
	else
	{
		if (i_cTickYN == 'Y')	sprintf(msg, "[%s] Succeed to Unsubscribe Tick", (LPCSTR)i_sSymbol);
		if (i_cTickYN == 'N')	sprintf(msg, "[%s] Succeed to Unsubscribe Quote", (LPCSTR)i_sSymbol);
	}
	g_log.log(INFO, msg );
	m_lstMsg.InsertString(0, msg);
}


//--------------------------------------------------------------------------------------
// REAL 데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CDlg_FC0_1::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	//////////////////////////////////////////////////////////////////////////
	//	TICK DATA
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_FU ) == 0 )
	{
		m_pClientIF->Inner_PostData(pRealPacket->pszData, sizeof(OVC_OutBlock), TRUE);
	}

	///////////////////////////////////////////////////////////////////////////
	//	해외선물호가
	//TODO
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_FU ) == 0 )
	{
		ETK_CME_QUOTE quote;
		quote.STX[0] = 0x02;
		quote.ETX[0] = 0x03;
		SET_LEN(sizeof(ETK_CME_QUOTE), quote.Header.Len);
		memcpy(quote.Header.Code, CD_PACKET_DATAFEED_ETK_CME_QOUTE, sizeof(quote.Header.Code));
		memcpy(&quote.etkQuote, pRealPacket->pszData, sizeof(OVH_OutBlock));
		m_pClientIF->Inner_PostData((char*)&quote, sizeof(ETK_CME_QUOTE), FALSE);
	}

	return 0L;
}



void CDlg_FC0_1::OnButtonUnadvise() 
{
	// TODO: Add your control notification handler code here
	//	선물 시세/호가 해제
	list<CString>::iterator it;
	for( it=m_lstSymbols.begin(); it!=m_lstSymbols.end(); it++ )
	{
		CString sStk = *it;
		UnadviseData('Y'/*시세*/, sStk);		
		UnadviseData('N'/*호가*/, sStk);
	}
}


