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
		CUtil::PrintLog(&m_logF, FALSE, "API INFO �������� ����");
		return FALSE;
	}
	
	char	szTemp			[128]	= {0,};
	char	szMQRslt_Cntr	[128]	= {0,};
	char	szMQRslt_Ord	[128]	= {0,};
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//	MQ INIT
	if( !CUtil::GetCNFG( "MQ_NM", "CNTR_RSLT_KS",  szTemp) ){
		CUtil::PrintLog(&m_logF, FALSE, "ü����Q �̸� �����������");
		return FALSE;
	}
	SET_MQNAME(szTemp, szMQRslt_Cntr);
	
	//	�ֹ����
	if( !CUtil::GetCNFG( "MQ_NM", "ORD_RSLT_KS",  szTemp) ){
		CUtil::PrintLog(&m_logF, FALSE, "�ֹ����Q �̸� �����������");
		return FALSE;
	}
	SET_MQNAME(szTemp, szMQRslt_Ord);
	
	
	//	ü���� MQ OPEN
	HRESULT hr = m_MQRslt_Cntr.SetPathName( szMQRslt_Cntr );
	if( FAILED(hr) )
	{
		CUtil::PrintLog(&m_logF, FALSE, "ü����Q SetPathName ����");
		return FALSE;
	}
	hr = m_MQRslt_Cntr.MQOpen(RECEIVE);
	if( FAILED(hr) ){
		CUtil::PrintLog(&m_logF, FALSE, "ü����Q OPEN ����");
		return FALSE;
	}
	
	//	�ֹ���� MQ OPEN
	hr = m_MQRslt_Ord.SetPathName( szMQRslt_Ord );
	if( FAILED(hr) )
	{
		CUtil::PrintLog(&m_logF, FALSE, "�ֹ����Q SetPathName ����");
		return FALSE;
	}
	hr = m_MQRslt_Ord.MQOpen(SEND);
	if( FAILED(hr) ){
		CUtil::PrintLog(&m_logF, FALSE, "�ֹ����Q OPEN ����");
		return FALSE;
	}
#endif
	// 	//////////////////////////////////////////////////////////////////////////
	//	�������� ��������
	if( !GetStkCodeList_KS(GDS_CODE_KSF) ){
		CUtil::PrintLog(&m_logF, FALSE, "���� ���� �������� ����");
		m_db.free();
		return	FALSE;
	}
	if( !GetStkCodeList_KS(GDS_CODE_KSO) ){
		CUtil::PrintLog(&m_logF, FALSE, "�ɼ� ���� �������� ����");
		m_db.free();
		return	FALSE;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//	DB ���� ����/OPTION ���񸮽�Ʈ�� �����´�.
BOOL CReal_OrdCntr::GetStkCodeList_KS(char* psGdsCode)
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
	�ǽð� tr �� ����Ѵ�.
**********************************************************/
void CReal_OrdCntr::RegistRealData(BOOL bAdvise, CString sStk)
{
	TCHAR	szTrCode[32];
	int		nSize;
	BOOL	bSuccess;
	char	msg[512];
	//////////////////////////////////////////////////////////////////////////
	//	�ֹ�
	//////////////////////////////////////////////////////////////////////////
	strcpy( szTrCode, ETK_REAL_ORD);
	nSize = sizeof(O01_InBlock);

	// ������ ����
	if(bAdvise)
	{
		bSuccess = g_iXingAPI.AdviseRealData( 
									GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
									szTrCode,					// TR ��ȣ
									sStk,						// �����ڵ�
									nSize						// �����ڵ� ����
									);	
		// ����üũ
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)�ֹ� �ǽð� ��� ����", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)�ֹ� �ǽð� ��� ����", (LPCSTR)sStk );
	}
	else
	{
		bSuccess = g_iXingAPI.UnadviseRealData( 
			GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
			szTrCode,					// TR ��ȣ
			sStk,						// �����ڵ�
			nSize						// �����ڵ� ����
			);	
		// ����üũ
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)�ֹ� �ǽð� ���� ����", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)�ֹ� �ǽð� ���� ����", (LPCSTR)sStk );
	}

	//////////////////////////////////////////////////////////////////////////
	//	����/��� �ֹ�
	//////////////////////////////////////////////////////////////////////////
	strcpy( szTrCode, ETK_REAL_MDFYCNCL);
	nSize = sizeof(H01_InBlock);
	
	// ������ ����
	if(bAdvise)
	{
		bSuccess = g_iXingAPI.AdviseRealData( 
									GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
									szTrCode,					// TR ��ȣ
									sStk,						// �����ڵ�
									nSize						// �����ڵ� ����
									);
		// ����üũ
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)������� �ǽð� ��� ����", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)������� �ǽð� ��� ����", (LPCSTR)sStk );
	}
	else
	{
		bSuccess = g_iXingAPI.UnadviseRealData( 
									GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
									szTrCode,					// TR ��ȣ
									sStk,						// �����ڵ�
									nSize						// �����ڵ� ����
									);
		// ����üũ
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)������� �ǽð� ���� ����", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)������� �ǽð� ���� ����", (LPCSTR)sStk );
	}

	//////////////////////////////////////////////////////////////////////////
	//	ü��
	//////////////////////////////////////////////////////////////////////////
	strcpy( szTrCode, ETK_REAL_CNTR);
	nSize = sizeof(C01_InBlock);
	
	// ������ ����
	if(bAdvise)
	{
		bSuccess = g_iXingAPI.AdviseRealData( 
								GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
								szTrCode,					// TR ��ȣ
								sStk,						// �����ڵ�
								nSize						// �����ڵ� ����
								);
		// ����üũ
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)ü�� �ǽð� ��� ����", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)ü�� �ǽð� ��� ����", (LPCSTR)sStk );
	}
	else
	{
		bSuccess = g_iXingAPI.UnadviseRealData( 
								GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
								szTrCode,					// TR ��ȣ
								sStk,						// �����ڵ�
								nSize						// �����ڵ� ����
								);
		// ����üũ
		if( bSuccess == FALSE )	sprintf(msg,  "(%s)ü�� �ǽð� ���� ����", (LPCSTR)sStk );
		else					sprintf(msg,  "(%s)ü�� �ǽð� ���� ����", (LPCSTR)sStk );
	}
}



void CReal_OrdCntr::OnDestroy()
{
	//	�ֹ�/ü�� real ��� ����
	BOOL bAdvise = TRUE;
	
	list<CString>::iterator it;
	for( it=m_sStkList_KSF.begin(); it!=m_sStkList_KSF.end(); it++ )
	{
		CString sStk = *it;
		RegistRealData(!bAdvise, sStk );
	}
	
	//	�ɼ� �ü� ��û
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
	
	//	�ɼ� �ü� ��û
	for( it=m_sStkList_KSO.begin(); it!=m_sStkList_KSO.end(); it++ )
	{
		CString sStk = *it;
		RegistRealData(!bAdvise, sStk );
		RegistRealData(bAdvise, sStk );
	}
}



//--------------------------------------------------------------------------------------
// �����͸� ����
//--------------------------------------------------------------------------------------
LRESULT CReal_OrdCntr::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	char msg[4096];
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	//////////////////////////////////////////////////////////////////////////
	//	�ֹ�����
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_ORD ) == 0 )
	{
		sprintf( msg, "[REAL_ORD ][%s]",pRealPacket->pszData );
		m_lstMsg.InsertString(0, msg);
		CUtil::PrintLog(&m_logF, FALSE, msg);
	}

	///////////////////////////////////////////////////////////////////////////
	//	�������
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_MDFYCNCL ) == 0 )
	{
		sprintf( msg, "[REAL_CNCL][%s]",pRealPacket->pszData );
		m_lstMsg.InsertString(0, msg);
		CUtil::PrintLog(&m_logF, FALSE, msg);
	}

	//////////////////////////////////////////////////////////////////////////
	//	ü��
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_CNTR ) == 0 )
	{
		sprintf( msg, "[REAL_CNTR][%s]",pRealPacket->pszData );
		m_lstMsg.InsertString(0, msg);
		CUtil::PrintLog(&m_logF, FALSE, msg);
	}

	return 0L;
}