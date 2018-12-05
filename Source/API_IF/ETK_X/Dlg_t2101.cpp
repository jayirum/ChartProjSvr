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
	//	REAL DATA ������ �����ڵ� ��������
	if( !GetStkCodeList_KS(GDS_CODE_KSF) ){
	 	CUtil::PrintLog(&m_logF, FALSE, "���� ���� �������� ����");
	 	return	FALSE;
	 }
	if( !GetStkCodeList_KS(GDS_CODE_KSO) ){
	 	CUtil::PrintLog(&m_logF, FALSE, "�ɼ� ���� �������� ����");
	 	return	FALSE;
 	}
	
	OnOK();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//	��ȸ
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
	
	SetPacketData( pckInBlock.focode , sizeof( pckInBlock.focode ), i_psStkCode	, DATA_TYPE_STRING );	// CTS_�Ÿű���
		
	//-----------------------------------------------------------
	// ������ ����
	int nRqID = g_iXingAPI.Request( 
							GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_DATA ���� �´�.
							szTrNo,						// TR ��ȣ
							&pckInBlock,				// InBlock ������
							sizeof( pckInBlock ),		// InBlock ������ ũ��
							FALSE,						// ������ȸ ���� : ������ȸ�� ��쿡 �����Ѵ�.
							szNextKey,					// ������ȸ Key : //		Header Type�� B �� ��쿣 ���� ��ȸ�� ���� Next Key�� �־��ش�.
							30							// Timeout(��) : �ش� �ð�(��)���� �����Ͱ� ���� ������ Timeout�� �߻��Ѵ�. XM_TIMEOUT_DATA �޽����� �߻��Ѵ�.
							);
	

	//-----------------------------------------------------------
	// Request ID�� 0���� ���� ��쿡�� �����̴�.
	if( nRqID < 0 )
	{
		//MessageBox( "t2101 ��ȸ����", "����", MB_ICONSTOP );
		sprintf( m_szMsg,  "[%s] ��ȸ ����(%d)", i_psStkCode, nRqID );
	}
	else
	{
		sprintf( m_szMsg, "[%s] ��ȸ ����", i_psStkCode );
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
//	TR�� ���� ���䵥���͸� ����
//--------------------------------------------------------------------------------------
LRESULT CDlg_t2101::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
	//-------------------------------------------------------------------------------------
	// Data�� ����
	if( wParam == REQUEST_DATA )
	{
		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;
		LPt2101OutBlock p = (LPt2101OutBlock)pRpData->lpData;
		
		char szNowPrc[32], szKS200[32];
		double dNowPrc, dKS200;

		//	���簡
		get_double_withdot( p->price, sizeof(p->price), 2, sizeof(p->price), szNowPrc );
		sprintf( m_szMsg, "[%s](%s)", (LPCSTR)m_sLastSendStk, szNowPrc );
		m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
		CUtil::PrintLog(&m_logF, FALSE, m_szMsg );

		//	KS200����
		get_double_withdot( p->kospijisu, sizeof(p->kospijisu), 2, sizeof(p->kospijisu), szKS200 );

		dNowPrc = atof(szNowPrc);
		dKS200	= atof(szKS200);

		if( dNowPrc<=0. )
		{
			sprintf( m_szMsg, "[%s](%s)���簡 �̻�", (LPCSTR)m_sLastSendStk, szNowPrc );
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
			sprintf( m_szMsg, "[%s](%s)DB ���� ����(%s)", (LPCSTR)m_sLastSendStk, szNowPrc, m_db.GetLastError() );
			m_lstResult.InsertString(0, (LPCSTR)m_szMsg );
			CUtil::PrintLog(&m_logF, FALSE, m_szMsg );
			m_db.Close();
		}
		m_db.Close();

	}
	
	//-------------------------------------------------------------------------------------
	// �޽����� ����
	else if( wParam == MESSAGE_DATA )
	{
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;

		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		
		
		g_iXingAPI.ReleaseMessageData( lParam );
	}
	
	//-------------------------------------------------------------------------------------
	// System Error�� ����
	else if( wParam == SYSTEM_ERROR_DATA )
	{
		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
		
		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		

		g_iXingAPI.ReleaseMessageData( lParam );
	}

	//-------------------------------------------------------------------------------------
	// Release Data�� ����
	else if( wParam == RELEASE_DATA )
	{
		g_iXingAPI.ReleaseRequestData( (int)lParam );

		Sleep(1000);

		int nSize = 0;

		//	����
		if( m_sLastSendStk[0]=='1' )
		{
			nSize = m_sStkList_KSF.size();
			if( m_nLastSendCnt_F<nSize )
			{
				Request_F();
			}
			else
			{
				sprintf( m_szMsg, "�������� �Ϸ�");
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
		sprintf( m_szMsg, "�������� �Ϸ�");
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
		sprintf( m_szMsg, "�ɼ����� �Ϸ�");
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
// Timeout �߻�
//--------------------------------------------------------------------------------------
LRESULT CDlg_t2101::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );
	GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)"", (LPARAM)"Timeout" );		

	return 0L;
}


//	DB ���� ����/OPTION ���񸮽�Ʈ�� �����´�.
BOOL CDlg_t2101::GetStkCodeList_KS(char* psGdsCode)
{
	BOOL bKSF;	// ����, �ɼ� ����
	
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
	
	//	�Ҽ��� �ִ� ���
	if( strchr( t, '.' )>0 )
	{
		sprintf( t, "%.*s", i_nOrgLen, i_psOrg );
		dTemp = atof(t);
		sprintf( o_psOut, "%*.*f",i_nOutLen, i_nDotCnt, dTemp );
		return o_psOut;
	}
	
	//	�Ҽ����� ���� ���
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
