// Dlg_t2101.cpp : implementation file
//

#include "stdafx.h"
#include "xingapi_sample.h"
#include "Dlg_t2101.h"
#include "../../libsrc/CVT_ETK.h"
#include "../../include/API_ETK/t2101.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_t2101 dialog

IMPLEMENT_DYNCREATE(CDlg_t2101, CDialog)

CDlg_t2101::CDlg_t2101(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t2101::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t2101)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_t2101::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t2101)
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_t2101, CDialog)
	//{{AFX_MSG_MAP(CDlg_t2101)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	//}}AFX_MSG_MAP
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t2101 message handlers


BOOL CDlg_t2101::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//----------------------------------------
	//	LOG FIL OPEN
	//----------------------------------------
	char szPath[_MAX_PATH] = {0,};
	char szMsg[512];
	if( !CUtil::GetLogDir(szPath, szMsg) )
	{
		return FALSE;
	}
	
	m_logF.OpenLog(szPath, "ETK_GET_ENDPRC");
	
	//////////////////////////////////////////////////////////////////////////
	//	DB OPEN 
	if( !CUtil::DBOpen(&m_db) )
	{
		CUtil::PrintLog(&m_logF, FALSE, "DB OPEN ERROR(%s)", m_db.GetLastError() );
		m_db.free();
		return FALSE;
	}

	// 	//////////////////////////////////////////////////////////////////////////
	//	REAL DATA 수신할 종목코드 가져오기
	if( !GetStkCodeList_KS(GDS_CODE_KSF) ){
	 	CUtil::PrintLog(&m_logF, FALSE, "선물 종목 가져오기 실패");
	 	return	FALSE;
	 }
	if( !GetStkCodeList_KS(GDS_CODE_KSO) ){
	 	CUtil::PrintLog(&m_logF, FALSE, "옵션 종목 가져오기 실패");
	 	return	FALSE;
 	}
	
	OnOK();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//	조회
void CDlg_t2101::OnOK() 
{
	m_sLastSendStk		= "";
	m_nLastSendCnt_F	= 0;
	m_nLastSendCnt_O	= 0;

	list<CString>::iterator	it = m_sStkList_KSF.begin();
	if( it!=m_sStkList_KSF.end() )
	{
		CString sStk = *it;
		RequestData( (LPCSTR)sStk );
		return;
	}

	else
	{
		it = m_sStkList_KSO.begin();
		CString sStk = *it;
		RequestData( (LPCSTR)sStk );
		return;
	}
}

void CDlg_t2101::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	OnDestroy();
}

void CDlg_t2101::RequestData(const char* i_psStkCode )
{
	t2101InBlock pckInBlock;
	
	TCHAR			szTrNo[]	= "t2101";
	char			szNextKey[]	= "";
	
	SetPacketData( pckInBlock.focode , sizeof( pckInBlock.focode ), i_psStkCode	, DATA_TYPE_STRING );	// CTS_매매구분
		
	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
							GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
							szTrNo,						// TR 번호
							&pckInBlock,				// InBlock 데이터
							sizeof( pckInBlock ),		// InBlock 데이터 크기
							FALSE,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
							szNextKey,					// 다음조회 Key : //		Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
							30							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
							);
	

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		//MessageBox( "t2101 조회실패", "에러", MB_ICONSTOP );
		sprintf( m_szMsg,  "[%s] 조회 실패(%d)", i_psStkCode, nRqID );
	}
	else
	{
		sprintf( m_szMsg, "[%s] 조회 성공", i_psStkCode );
		m_sLastSendStk = i_psStkCode;
		if( i_psStkCode[0]=='1' )
			m_nLastSendCnt_F++;
		else
			m_nLastSendCnt_O++;
	}
	CUtil::PrintLog(&m_logF, FALSE, m_szMsg);
	m_lstResult.InsertString(0, (LPCSTR)m_szMsg);

}

//--------------------------------------------------------------------------------------
//	TR에 대한 응답데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CDlg_t2101::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data를 받음
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;
		LPt2101OutBlock p = (LPt2101OutBlock)pRpData->lpData;
		
		char szNowPrc[32], szKS200[32];
		double dNowPrc, dKS200;

		//	현재가
		get_double_withdot( p->price, sizeof(p->price), 2, sizeof(p->price), szNowPrc );
		sprintf( m_szMsg, "[%s](%s)", (LPCSTR)m_sLastSendStk, szNowPrc );
		m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
		CUtil::PrintLog(&m_logF, FALSE, m_szMsg );

		//	KS200지수
		get_double_withdot( p->kospijisu, sizeof(p->kospijisu), 2, sizeof(p->kospijisu), szKS200 );

		dNowPrc = atof(szNowPrc);
		dKS200	= atof(szKS200);

		if( dNowPrc<=0. )
		{
			sprintf( m_szMsg, "[%s](%s)현재가 이상", (LPCSTR)m_sLastSendStk, szNowPrc );
			m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
			CUtil::PrintLog(&m_logF, FALSE, m_szMsg );
			return 0L;
		}

		char szQ[512];
		SYSTEMTIME st; GetLocalTime(&st);
		if (dKS200>0.)
		{
			sprintf( szQ, "	UPDATE	HT_T_PRC_LIST_KS SET NOW_PRC=%.2f, TM='%02d%02d%02d' "
							" WHERE STK_CODE='%s' "

							" UPDATE HT_T_STK_MASTER SET END_PRC=%.2f WHERE STK_CODE='%s'"

							" UPDATE HT_T_KS200 SET NOW_PRC=%.2f"
							, dNowPrc, st.wHour, st.wMinute, st.wSecond, (LPCSTR)m_sLastSendStk
							, dNowPrc, (LPCSTR)m_sLastSendStk
							, dKS200
							);
		}
		else
		{
			sprintf( szQ, "	UPDATE	HT_T_PRC_LIST_KS SET NOW_PRC=%.2f, TM='%02d%02d%02d' "
							" WHERE STK_CODE='%s' "
							
							" UPDATE HT_T_STK_MASTER SET END_PRC=%.2f WHERE STK_CODE='%s'"
							, dNowPrc, st.wHour, st.wMinute, st.wSecond, (LPCSTR)m_sLastSendStk
							, dNowPrc, (LPCSTR)m_sLastSendStk
							);
		}
		if( !m_db.Cmd(szQ) || !m_db.Exec() )
		{
			sprintf( m_szMsg, "[%s](%s)DB 저장 실패(%s)", (LPCSTR)m_sLastSendStk, szNowPrc, m_db.GetLastError() );
			m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
			CUtil::PrintLog(&m_logF, FALSE, m_szMsg );
			m_db.Close();
		}
		m_db.Close();

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

		Sleep(1000);

		int nSize = 0;

		//	선물
		if( m_sLastSendStk[0]=='1' )
		{
			nSize = m_sStkList_KSF.size();
			if( m_nLastSendCnt_F<nSize )
			{
				Request_F();
			}
			else
			{
				sprintf( m_szMsg, "선물종목 완료");
				m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
				CUtil::PrintLog(&m_logF, FALSE, m_szMsg );
				Request_O();
			}
		}

		//	OPTION
		else
		{
			Request_O();
		}
	}

	return 0L;
}


VOID CDlg_t2101::Request_F()
{
	if( m_nLastSendCnt_F==m_sStkList_KSF.size() ){
		sprintf( m_szMsg, "선물종목 완료");
		m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
		CUtil::PrintLog(&m_logF, FALSE, m_szMsg );
		return;
	}

	list<CString>::iterator it = m_sStkList_KSF.begin();

	for( int i=0; i<m_nLastSendCnt_F; i++ )
		it++;

	CString sStk = *it;

	RequestData((LPCSTR)sStk);
}


VOID CDlg_t2101::Request_O()
{
	if( m_nLastSendCnt_O==m_sStkList_KSO.size() ) {
		sprintf( m_szMsg, "옵션종목 완료");
		m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
		CUtil::PrintLog(&m_logF, FALSE, m_szMsg );
		return;
	}
	
	list<CString>::iterator it = m_sStkList_KSO.begin();
	
	for( int i=0; i<m_nLastSendCnt_O; i++ )
		it++;
	
	CString sStk = *it;
	
	RequestData((LPCSTR)sStk);
}



//--------------------------------------------------------------------------------------
// Timeout 발생
//--------------------------------------------------------------------------------------
LRESULT CDlg_t2101::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );
	GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)"", (LPARAM)"Timeout" );		

	return 0L;
}


//	DB 에서 선물/OPTION 종목리스트를 가져온다.
BOOL CDlg_t2101::GetStkCodeList_KS(char* psGdsCode)
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
		
		CUtil::PrintDebug( &m_logF, FALSE, szStk );
		
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


char* CDlg_t2101::get_double_withdot( char* i_psOrg, int i_nOrgLen, int i_nDotCnt, int i_nOutLen, char* o_psOut )
{
	char	t[128], t2[128];
	double	dTemp;
	
	sprintf( t, "%.*s", i_nOrgLen, i_psOrg );
	
	//	소숫점 있는 경우
	if( strchr( t, '.' )>0 )
	{
		sprintf( t, "%.*s", i_nOrgLen, i_psOrg );
		dTemp = atof(t);
		sprintf( o_psOut, "%*.*f",i_nOutLen, i_nDotCnt, dTemp );
		return o_psOut;
	}
	
	//	소숫점이 없는 경우
	int nPos = i_nOrgLen - i_nDotCnt;
	sprintf( t, "%.*s", i_nDotCnt, i_psOrg+nPos );
	sprintf( t2, "%.*s.%s", nPos, i_psOrg, t );
	dTemp = atof(t2);
	sprintf( o_psOut, "%*.*f", i_nOutLen, i_nDotCnt, dTemp );
	return o_psOut;
}

VOID CDlg_t2101::OnDestroy()
{
	m_db.Free();
	CDialog::OnDestroy();
}

void CDlg_t2101::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	
	OnDestroy();
}
