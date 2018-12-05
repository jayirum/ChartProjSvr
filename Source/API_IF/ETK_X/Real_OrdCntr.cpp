// Real_OrdCntr.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Real_OrdCntr.h"

#include "../../include/API_ETK/O01.h"
#include "../../include/API_ETK/H01.h"
#include "../../include/API_ETK/C01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReal_OrdCntr dialog


CReal_OrdCntr::CReal_OrdCntr(CWnd* pParent /*=NULL*/)
	: CDialog(CReal_OrdCntr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReal_OrdCntr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReal_OrdCntr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReal_OrdCntr)
	DDX_Control(pDX, IDC_LIST1, m_lstMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReal_OrdCntr, CDialog)
	//{{AFX_MSG_MAP(CReal_OrdCntr)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveRealData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReal_OrdCntr message handlers

BOOL CReal_OrdCntr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//-----------------------------------------------------------------------------------------
	//	LOG FIL OPEN
	//-----------------------------------------------------------------------------------------
	char szPath[_MAX_PATH] = {0,};
	char szMsg[512];
	if( !CUtil::GetLogDir(szPath, szMsg) )
	{
		return FALSE;
	}
	
	m_logF.OpenLog(szPath, "ETK_ORD_CNTR_REAL");
	
	//////////////////////////////////////////////////////////////////////////
	//	DB OPEN 
	if( !CUtil::DBOpen(&m_db) )
	{
		CUtil::PrintLog(&m_logF, FALSE, "DB OPEN ERROR(%s)", m_db.GetLastError() );
		m_db.free();
		return FALSE;
	}

#if 0
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//	GET API USERID 
	if( !CUtil::GetAPIInfo( MY_API_CODE, m_szAPI_UserID, m_szAPI_AcntNo) )
	{
		CUtil::PrintLog(&m_logF, FALSE, "API INFO 가져오기 실패");
		return FALSE;
	}
	
	char	szTemp			[128]	= {0,};
	char	szMQRslt_Cntr	[128]	= {0,};
	char	szMQRslt_Ord	[128]	= {0,};
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//	MQ INIT
	if( !CUtil::GetCNFG( "MQ_NM", "CNTR_RSLT_KS",  szTemp) ){
		CUtil::PrintLog(&m_logF, FALSE, "체결결과Q 이름 가져오기오류");
		return FALSE;
	}
	SET_MQNAME(szTemp, szMQRslt_Cntr);
	
	//	주문결과
	if( !CUtil::GetCNFG( "MQ_NM", "ORD_RSLT_KS",  szTemp) ){
		CUtil::PrintLog(&m_logF, FALSE, "주문결과Q 이름 가져오기오류");
		return FALSE;
	}
	SET_MQNAME(szTemp, szMQRslt_Ord);
	
	
	//	체결결과 MQ OPEN
	HRESULT hr = m_MQRslt_Cntr.SetPathName( szMQRslt_Cntr );
	if( FAILED(hr) )
	{
		CUtil::PrintLog(&m_logF, FALSE, "체결결과Q SetPathName 오류");
		return FALSE;
	}
	hr = m_MQRslt_Cntr.MQOpen(RECEIVE);
	if( FAILED(hr) ){
		CUtil::PrintLog(&m_logF, FALSE, "체결결과Q OPEN 오류");
		return FALSE;
	}
	
	//	주문결과 MQ OPEN
	hr = m_MQRslt_Ord.SetPathName( szMQRslt_Ord );
	if( FAILED(hr) )
	{
		CUtil::PrintLog(&m_logF, FALSE, "주문결과Q SetPathName 오류");
		return FALSE;
	}
	hr = m_MQRslt_Ord.MQOpen(SEND);
	if( FAILED(hr) ){
		CUtil::PrintLog(&m_logF, FALSE, "주문결과Q OPEN 오류");
		return FALSE;
	}
#endif
	// 	//////////////////////////////////////////////////////////////////////////
	//	종목정보 가져오기
	if( !GetStkCodeList_KS(GDS_CODE_KSF) ){
		CUtil::PrintLog(&m_logF, FALSE, "선물 종목 가져오기 실패");
		m_db.free();
		return	FALSE;
	}
	if( !GetStkCodeList_KS(GDS_CODE_KSO) ){
		CUtil::PrintLog(&m_logF, FALSE, "옵션 종목 가져오기 실패");
		m_db.free();
		return	FALSE;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//	DB 에서 선물/OPTION 종목리스트를 가져온다.
BOOL CReal_OrdCntr::GetStkCodeList_KS(char* psGdsCode)
{
	BOOL bKSF;	// 선물, 옵션 여부
	
	if( strcmp(psGdsCode, GDS_CODE_KSO)==0 )
	{
		bKSF = FALSE;
		
		if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KSO" ) )
			return FALSE;
	}
	if( strcmp(psGdsCode, GDS_CODE_KSF)==0 )
	{
		bKSF = TRUE;
		
		if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KSF" ) )
			return FALSE;
	}
	
	if( !m_db.Exec() )
		return FALSE;
	
	char szStkList[1024]	= {0,};
	char szStk[128]			= {0,};
	int nRs = 0;
	while( m_db. nextRow() )
	{
		strcpy( szStk, m_db.Get(1) );
		CUtil::TrimAll( szStk, strlen(szStk) );
		
		if( strlen(szStk)==0 )
			break;
		
		CString sStk = szStk;
		if(bKSF)
			m_sStkList_KSF.push_back(sStk);
		else
			m_sStkList_KSO.push_back(sStk);
	}
	m_db.Close();
	
	return TRUE;
}


/**********************************************************
	실시간 tr 을 등록한다.
**********************************************************/
void CReal_OrdCntr::RegistRealData(BOOL bAdvise, CString sStk)
{
	TCHAR	szTrCode[32];
	int		nSize;
	BOOL	bSuccess;
	char	msg[512];
	//////////////////////////////////////////////////////////////////////////
	//	주문
	//////////////////////////////////////////////////////////////////////////
	strcpy( szTrCode, ETK_REAL_ORD);
	nSize = sizeof(O01_InBlock);

	// 데이터 전송
	if(bAdvise)
	{
		bSuccess = g_iXingAPI.AdviseRealData( 
									GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
									szTrCode,					// TR 번호
									sStk,						// 종목코드
									nSize						// 종목코드 길이
									);	
		// 에러체크
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)주문 실시간 등록 실패", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)주문 실시간 등록 성공", (LPCSTR)sStk );
	}
	else
	{
		bSuccess = g_iXingAPI.UnadviseRealData( 
			GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
			szTrCode,					// TR 번호
			sStk,						// 종목코드
			nSize						// 종목코드 길이
			);	
		// 에러체크
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)주문 실시간 해제 실패", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)주문 실시간 해제 성공", (LPCSTR)sStk );
	}

	//////////////////////////////////////////////////////////////////////////
	//	정정/취소 주문
	//////////////////////////////////////////////////////////////////////////
	strcpy( szTrCode, ETK_REAL_MDFYCNCL);
	nSize = sizeof(H01_InBlock);
	
	// 데이터 전송
	if(bAdvise)
	{
		bSuccess = g_iXingAPI.AdviseRealData( 
									GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
									szTrCode,					// TR 번호
									sStk,						// 종목코드
									nSize						// 종목코드 길이
									);
		// 에러체크
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)정정취소 실시간 등록 실패", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)정정취소 실시간 등록 성공", (LPCSTR)sStk );
	}
	else
	{
		bSuccess = g_iXingAPI.UnadviseRealData( 
									GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
									szTrCode,					// TR 번호
									sStk,						// 종목코드
									nSize						// 종목코드 길이
									);
		// 에러체크
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)정정취소 실시간 해제 실패", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)정정취소 실시간 해제 성공", (LPCSTR)sStk );
	}

	//////////////////////////////////////////////////////////////////////////
	//	체결
	//////////////////////////////////////////////////////////////////////////
	strcpy( szTrCode, ETK_REAL_CNTR);
	nSize = sizeof(C01_InBlock);
	
	// 데이터 전송
	if(bAdvise)
	{
		bSuccess = g_iXingAPI.AdviseRealData( 
								GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
								szTrCode,					// TR 번호
								sStk,						// 종목코드
								nSize						// 종목코드 길이
								);
		// 에러체크
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)체결 실시간 등록 실패", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)체결 실시간 등록 성공", (LPCSTR)sStk );
	}
	else
	{
		bSuccess = g_iXingAPI.UnadviseRealData( 
								GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
								szTrCode,					// TR 번호
								sStk,						// 종목코드
								nSize						// 종목코드 길이
								);
		// 에러체크
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)체결 실시간 해제 실패", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)체결 실시간 해제 성공", (LPCSTR)sStk );
	}
}



void CReal_OrdCntr::OnDestroy()
{
	//	주문/체결 real 등록 해제
	BOOL bAdvise = TRUE;
	
	list<CString>::iterator it;
	for( it=m_sStkList_KSF.begin(); it!=m_sStkList_KSF.end(); it++ )
	{
		CString sStk = *it;
		RegistRealData(!bAdvise, sStk );
	}
	
	//	옵션 시세 요청
	for( it=m_sStkList_KSO.begin(); it!=m_sStkList_KSO.end(); it++ )
	{
		CString sStk = *it;
		RegistRealData(!bAdvise, sStk );
	}
		
	m_db.Free();
	
	CDialog::OnDestroy();
}

void CReal_OrdCntr::OnButton1() 
{
	BOOL bAdvise = TRUE;

	list<CString>::iterator it;
	for( it=m_sStkList_KSF.begin(); it!=m_sStkList_KSF.end(); it++ )
	{
		CString sStk = *it;
		RegistRealData(!bAdvise, sStk );
		RegistRealData(bAdvise, sStk );
	}
	
	//	옵션 시세 요청
	for( it=m_sStkList_KSO.begin(); it!=m_sStkList_KSO.end(); it++ )
	{
		CString sStk = *it;
		RegistRealData(!bAdvise, sStk );
		RegistRealData(bAdvise, sStk );
	}
}



//--------------------------------------------------------------------------------------
// 데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CReal_OrdCntr::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	char msg[4096];
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	//////////////////////////////////////////////////////////////////////////
	//	주문접수
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_ORD ) == 0 )
	{
		sprintf( msg, "[REAL_ORD ][%s]",pRealPacket->pszData );
		m_lstMsg.InsertString(0, msg);
		CUtil::PrintLog(&m_logF, FALSE, msg);
	}

	///////////////////////////////////////////////////////////////////////////
	//	정정취소
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_MDFYCNCL ) == 0 )
	{
		sprintf( msg, "[REAL_CNCL][%s]",pRealPacket->pszData );
		m_lstMsg.InsertString(0, msg);
		CUtil::PrintLog(&m_logF, FALSE, msg);
	}

	//////////////////////////////////////////////////////////////////////////
	//	체결
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_CNTR ) == 0 )
	{
		sprintf( msg, "[REAL_CNTR][%s]",pRealPacket->pszData );
		m_lstMsg.InsertString(0, msg);
		CUtil::PrintLog(&m_logF, FALSE, msg);
	}

	return 0L;
}