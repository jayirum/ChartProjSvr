// Dlg_t5511.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_t5511.h"

#include "../Inc_ETK/t5511.h"	//	���������ֹ�
#include "../Inc_ETK/t5512.h"	//	���������ֹ�
#include "../Inc_ETK/t5513.h"	//	��������ֹ�
#include "../Inc_ETK/O01.h"
//#include "../../libsrc/CVT_ETK.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDlg_t5511 dialog
//CMQBase		g_MQReq_Ord;	//	�ֹ�TR ���� MQ
//CMQBase		g_MQRslt_Ord;	//	�ֹ���� ������ MQ
//CMQBase		g_MQRslt_Cntr;	//	ü���� ������ MQ

IMPLEMENT_DYNCREATE(CDlg_t5511, CDialog)

CDlg_t5511::CDlg_t5511(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_t5511::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_t5511)
	//}}AFX_DATA_INIT

	m_hDie		= NULL;
	m_hThread	= NULL;
}


void CDlg_t5511::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_t5511)
	DDX_Control(pDX, IDC_LIST_TR, m_lstTR);
	DDX_Control(pDX, IDC_LIST_REAL, m_lstR);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CDlg_t5511, CDialog)
	//{{AFX_MSG_MAP(CDlg_t5511)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,		  OnButtonRequest	  )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_t5511 message handlers

BOOL CDlg_t5511::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return FALSE;
	//----------------------------------------
	//	LOG FIL OPEN
	//----------------------------------------
	char szPath[_MAX_PATH] = {0,};
	//char szMsg[512];
	//if( !CUtil::GetLogDir(szPath, szMsg) )
	//{
	//	return FALSE;
	//}
	//
	//m_logF.OpenLog(szPath, "ETK_ORD");

	//
	////----------------------------------------
	////	GET API USERID 
	////----------------------------------------
	//if( !CUtil::GetAPIInfo( MY_API_CODE, m_szAPI_UserID, m_szAPI_AcntNo) )
	//{
	//	CUtil::PrintLogEx(&m_logF, FALSE, "API INFO �������� ����");
	//	return FALSE;
	//}
	
	char	szTemp			[128]	= {0,};
	char	szMQReq_Ord		[128]	= {0,};
	char	szMQRslt_Ord	[128]	= {0,};
	char	szMQRslt_Cntr	[128]	= {0,};

	
	//----------------------------------------
	//	MQ INIT
	//----------------------------------------

#if 0
	//	�ֹ�TR REAL 
	if( !CUtil::GetCNFG( "MQ_NM", "ORD_REQ_KS_ETK",  szTemp) ){
		CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ���ûQ �̸� �����������");
		return FALSE;
	}
	SET_MQNAME(szTemp, szMQReq_Ord);
	
	HRESULT hr = g_MQReq_Ord.SetPathName( szMQReq_Ord );
	if( FAILED(hr) )
	{
		CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ���ûQ SetPathName ����");
		return FALSE;
	}
	hr = g_MQReq_Ord.MQOpen(RECEIVE);
	if( FAILED(hr) ){
		CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ���ûQ OPEN ����");
		return FALSE;
	}

	
	//	�ֹ���� SEND
	if( !CUtil::GetCNFG( "MQ_NM", "ORD_RSLT_KS",  szTemp) ){
		CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ����Q �̸� �����������");
		return FALSE;
	}
	SET_MQNAME(szTemp, szMQRslt_Ord);
	hr = g_MQRslt_Ord.SetPathName( szMQRslt_Ord );
	if( FAILED(hr) )
	{
		CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ����Q SetPathName ����");
		return FALSE;
	}
	hr = g_MQRslt_Ord.MQOpen(SEND);
	if( FAILED(hr) ){
		CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ����Q OPEN ����");
		return FALSE;
	}


	//	ü���� SEND
	if( !CUtil::GetCNFG( "MQ_NM", "CNTR_RSLT_KS",  szTemp) ){
		CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ����Q �̸� �����������");
		return FALSE;
	}
	SET_MQNAME(szTemp, szMQRslt_Cntr);
	hr = g_MQRslt_Cntr.SetPathName( szMQRslt_Cntr );
	if( FAILED(hr) )
	{
		CUtil::PrintLogEx(&m_logF, FALSE, "ü����Q SetPathName ����");
		return FALSE;
	}
	hr = g_MQRslt_Cntr.MQOpen(SEND);
	if( FAILED(hr) ){
		CUtil::PrintLogEx(&m_logF, FALSE, "ü����Q OPEN ����");
		return FALSE;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	//	�ֹ� ��ȿ �ð�(��)�� �����´�.
	//CUtil::GetCNFG("TIME", "ORD_VALID_MINITUE", szTemp );
	//m_nOrdAliveMin = atoi(szTemp);

	////////////////////////////////////////////////////////////////////////////
	////	DB OPEN 
	//if( !CUtil::DBOpen(&m_db) )
	//{
	//	CUtil::PrintLogEx(&m_logF, FALSE, "DB OPEN ERROR(%s)", m_db.GetLastError() );
	//	m_db.free();
	//	return FALSE;
	//}

// 	//////////////////////////////////////////////////////////////////////////
// 	//	REAL DATA ������ �����ڵ� ��������
// 	if( !GetStkCodeList_KS(GDS_CODE_KSF) ){
// 		CUtil::PrintLogEx(&m_logF, FALSE, "���� ���� �������� ����");
// 		return	FALSE;
// 	}
// 	if( !GetStkCodeList_KS(GDS_CODE_KSO) ){
// 		CUtil::PrintLogEx(&m_logF, FALSE, "�ɼ� ���� �������� ����");
// 		return	FALSE;
// 	}
	
	//////////////////////////////////////////////////////////////////////////
	//	REAL ������ ���� ���
	//	--> ���񺰷� �� �ʿ� ����.
	BOOL bAdvise = TRUE;

	//	���� unadvise
	RegistRealData(!bAdvise, ETK_REAL_ACPT_ORD );
	RegistRealData(!bAdvise, ETK_REAL_ACPT_MDFYCNCL );
	RegistRealData(!bAdvise, ETK_REAL_ACPT_CNTR );

	if( !RegistRealData(bAdvise, ETK_REAL_ACPT_ORD ) )
		return FALSE;
	if( !RegistRealData(bAdvise, ETK_REAL_ACPT_MDFYCNCL ) )
		return FALSE;
	if( !RegistRealData(bAdvise, ETK_REAL_ACPT_CNTR ) )
		return FALSE;

/*	list<CString>::iterator it;*/

	//	����
// 	for( it=m_sStkList_KSF.begin(); it!=m_sStkList_KSF.end(); it++ )
// 	{
// 		CString sStk = *it;
// 		if( !RegistRealData(bAdvise, sStk ) )
// 			return FALSE;
// 
// 		m_sMsg.Format("[F:%s] REAL��û ����", (LPCSTR)sStk );
// 		m_lstR.InsertString( 0, m_sMsg );
// 		CUtil::PrintDebug(&m_logF, FALSE, (LPSTR)(LPCSTR)m_sMsg);
// 	}
// 	
// 	//	�ɼ�
// 	for( it=m_sStkList_KSO.begin(); it!=m_sStkList_KSO.end(); it++ )
// 	{
// 		CString sStk = *it;
// 		if( !RegistRealData(bAdvise, sStk ) ){
// 			m_sMsg.Format("[O:%s] REAL��û ����", (LPCSTR)sStk );
// 			m_lstR.InsertString( 0, m_sMsg );
// 			return FALSE;
// 		}
// 		m_sMsg.Format("[O:%s] REAL��û ����", (LPCSTR)sStk );
// 		m_lstR.InsertString( 0, m_sMsg );
// 		CUtil::PrintDebug(&m_logF, FALSE, (LPSTR)(LPCSTR)m_sMsg);
// 	}


	//////////////////////////////////////////////////////////////////////////
	//	MQ READ THREAD ����
	//DWORD id;
	//if( (m_hDie	= CreateEvent(NULL, TRUE, FALSE, NULL))==NULL )
	//{
	//	CUtil::PrintLogEx(&m_logF, FALSE, "MQ THREAD �� EVENT ���� ����");
	//	return FALSE;
	//}
	//
	//if( (m_hThread=CreateThread(NULL,0,ThreadProc,this,0,&id))==NULL )
	//{
	//	CUtil::PrintLogEx(&m_logF, FALSE, "MQ THREAD ���� ����");
	//	return FALSE;
	//}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlg_t5511::OnButtonRequest() 
{
	//RequestData();
}



/*--------------------------------------------------------------------------------------	
	�ֹ� ����
	- �Ϲ�
	- ����
	- ���
--------------------------------------------------------------------------------------*/
void CDlg_t5511::RequestData( char* i_psLabel, char* i_psBody )
{
	//char szTrNo[32];
	//char szOut[MAX_SENDSIZE];
	//char szNextKey[]	= "";
	//int	 nRqID;

	//ST_SV_ORD_CHK_RSLT *pIn = (ST_SV_ORD_CHK_RSLT*)i_psBody;

	//CCVT_ETK cvt;
	//cvt.en_KS_TR_Order( (ST_SV_ORD_CHK_RSLT*)i_psBody, (LPCSTR) g_iXingAPI.GetCommMedia(), szOut );

	//ST_SV_ORD_CHK_RSLT *pInPack = (ST_SV_ORD_CHK_RSLT*)i_psBody;

	///*------------------------------
	//	�ű��ֹ�
	//------------------------------*/
	//if( pInPack->stC.ORD_PROC_TP[0]==ORD_PROC_NEW )
	//{
	//	strcpy( szTrNo, ETK_TR_ORD_NEW );

 //		t5511InBlock pckInBlock;
	//	memcpy( &pckInBlock, szOut, sizeof(pckInBlock) );
	//	
	//	//-----------------------------------------------------------
	//	// ������ ����
	//	nRqID = g_iXingAPI.Request( 
	//							GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_DATA ���� �´�.
	//							szTrNo,						// TR ��ȣ
	//							&pckInBlock,				// InBlock ������
	//							sizeof( pckInBlock ),		// InBlock ������ ũ��
	//							FALSE,						// ������ȸ ���� : ������ȸ�� ��쿡 �����Ѵ�.
	//							szNextKey,					// ������ȸ Key : Header Type�� B �� ��쿣 ���� ��ȸ�� ���� Next Key�� �־��ش�.
	//							30							// Timeout(��) : �ش� �ð�(��)���� �����Ͱ� ���� ������ Timeout�� �߻��Ѵ�. XM_TIMEOUT_DATA �޽����� �߻��Ѵ�.
	//							);

	//}
	//
	///*------------------------------
	//	�����ֹ�
	//------------------------------*/
	//if( pInPack->stC.ORD_PROC_TP[0]==ORD_PROC_MDFY )
	//{
	//	strcpy( szTrNo, ETK_TR_ORD_MDFY );
	//	
	//	t5512InBlock pckInBlock;
	//	memcpy( &pckInBlock, szOut, sizeof(pckInBlock) );
	//	
	//	//-----------------------------------------------------------
	//	// ������ ����
	//	nRqID = g_iXingAPI.Request( 
	//						GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_DATA ���� �´�.
	//						szTrNo,						// TR ��ȣ
	//						&pckInBlock,				// InBlock ������
	//						sizeof( pckInBlock ),		// InBlock ������ ũ��
	//						FALSE,						// ������ȸ ���� : ������ȸ�� ��쿡 �����Ѵ�.
	//						szNextKey,					// ������ȸ Key : Header Type�� B �� ��쿣 ���� ��ȸ�� ���� Next Key�� �־��ش�.
	//						30							// Timeout(��) : �ش� �ð�(��)���� �����Ͱ� ���� ������ Timeout�� �߻��Ѵ�. XM_TIMEOUT_DATA �޽����� �߻��Ѵ�.
	//						);
	//}
	//
	///*------------------------------
	//	����ֹ�
	//------------------------------*/
	//if( pInPack->stC.ORD_PROC_TP[0]==ORD_PROC_CNCL )
	//{
	//	strcpy( szTrNo, ETK_TR_ORD_CNCL );
	//	
	//	t5513InBlock pckInBlock;
	//	memcpy( &pckInBlock, szOut, sizeof(pckInBlock) );
	//	
	//	//-----------------------------------------------------------
	//	// ������ ����
	//	nRqID = g_iXingAPI.Request( 
	//						GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_DATA ���� �´�.
	//						szTrNo,						// TR ��ȣ
	//						&pckInBlock,				// InBlock ������
	//						sizeof( pckInBlock ),		// InBlock ������ ũ��
	//						FALSE,						// ������ȸ ���� : ������ȸ�� ��쿡 �����Ѵ�.
	//						szNextKey,					// ������ȸ Key : Header Type�� B �� ��쿣 ���� ��ȸ�� ���� Next Key�� �־��ش�.
	//						30							// Timeout(��) : �ش� �ð�(��)���� �����Ͱ� ���� ������ Timeout�� �߻��Ѵ�. XM_TIMEOUT_DATA �޽����� �߻��Ѵ�.
	//						);
	//}

	//sprintf(m_szMsg, "[%*.*s]TR(%s)(REQID:%d)(%s)"
	//							, DEF_MSG_LEN, DEF_MSG_LEN
	//							, "2-���ǻ�����"
	//							,szTrNo, nRqID, i_psBody 
	//							);
	//CUtil::PrintLogEx( &m_logF, FALSE, m_szMsg );

	////-----------------------------------------------------------
	//// Request ID�� 0���� ���� ��쿡�� �����̴�.
	//if( nRqID < 0 )
	//{
	//	//	�ź���Ŷ ����
	//	ReturnRjct(RET_API_REJECT, pInPack->stC.GDS_CODE, pInPack->ClORD_NO, "���ǻ翡�� �ֹ��ź���");
	//	CUtil::PrintLogEx( &m_logF, FALSE, "[%*.*s](REQID:%d)",  DEF_MSG_LEN, DEF_MSG_LEN, "���ǻ����۽���", nRqID );
	//}
	//else
	//{
	//	MAP_ORD_INFO info;
	//	ZeroMemory(&info, sizeof(info));
	//	strcpy(info.TRCode, szTrNo);
	//	memcpy( info.GDS_CODE, pInPack->stC.GDS_CODE, sizeof(info.GDS_CODE) );

	//	sprintf(szTrNo, "%.*s", sizeof(pIn->ClORD_NO), pIn->ClORD_NO);
	//	memcpy( info.CLORD_NO, szTrNo, min(strlen(szTrNo),sizeof(info.CLORD_NO)) );
	//	m_mapRqID[nRqID] = info;
	//}

}

//--------------------------------------------------------------------------------------
//	TR�� ���� ���䵥���͸� ����
//--------------------------------------------------------------------------------------
LRESULT CDlg_t5511::OnXMReceiveData( WPARAM wParam, LPARAM lParam )
{
//	char szLabel[128]			= {0,};
//	char szBody[MAX_BUFSIZE]	= {0,};
//
//	//-------------------------------------------------------------------------------------
//	// Data�� ����
//	if( wParam == REQUEST_DATA )
//	{
//		LPRECV_PACKET pRpData = (LPRECV_PACKET)lParam;
//		int nRqID = pRpData->nRqID;
//
//		sprintf(m_szMsg, "[%*.*s](REQID:%d)(%s)"
//						, DEF_MSG_LEN, DEF_MSG_LEN
//						, "3-�ֹ�����TR ����"
//						, nRqID, pRpData->lpData 
//						);
//		CUtil::PrintLogEx( &m_logF, FALSE, m_szMsg );
//
//		map<int, MAP_ORD_INFO>::iterator it = m_mapRqID.find(nRqID);
//		if( it==m_mapRqID.end() )
//		{
//			CUtil::PrintLogEx( &m_logF, FALSE, "�ֹ�TR������ �޾�����, REQID �� ����.(REQID:%d)", nRqID );
//			return -1L;
//		}
//
//		MAP_ORD_INFO info;
//		info = (*it).second;
//		
//		if( strcmp(info.TRCode, ETK_TR_ORD_NEW)==0 )
//		{
//			typedef struct
//			{
//				t5511OutBlock	outBlock;
//				t5511OutBlock1	outBlock1;
//			} t5511AllOutBlock, *LPt5511AllOutBlock;
//
//			LPt5511AllOutBlock pAllOutBlock = (LPt5511AllOutBlock)pRpData->lpData;
//
//			// ���� ������ ũ�Ⱑ Block ũ�⺸�� �۴ٸ� ó������ �ʴ´�.
//			if( pRpData->nDataLength >= sizeof( t5511OutBlock ) )
//			{
//				CCVT_ETK cvt;
//				cvt.de_KS_TR_Order( (char*)&pAllOutBlock->outBlock1, info.GDS_CODE, info.CLORD_NO, szLabel, szBody );
//			}
//			
//// 			if( pRpData->nDataLength >= sizeof( t5511OutBlock ) + sizeof( t5511OutBlock1 ) )
//// 			{
//// 			}
//		}
//		if( strcmp(info.TRCode,ETK_TR_ORD_MDFY)==0 )
//		{
//			typedef struct
//			{
//				t5512OutBlock	outBlock;
//				t5512OutBlock1	outBlock1;
//			} t5512AllOutBlock, *LPt5512AllOutBlock;
//			
//			LPt5512AllOutBlock pAllOutBlock = (LPt5512AllOutBlock)pRpData->lpData;
//			
//			// ���� ������ ũ�Ⱑ Block ũ�⺸�� �۴ٸ� ó������ �ʴ´�.
//			if( pRpData->nDataLength >= sizeof( t5512OutBlock ) )
//			{
//				CCVT_ETK cvt;
//				cvt.de_KS_TR_Order_Mdfy( (char*)&pAllOutBlock->outBlock1, info.GDS_CODE, info.CLORD_NO, szLabel, szBody );
//			}
//			
//// 			if( pRpData->nDataLength >= sizeof( t5512OutBlock ) + sizeof( t5512OutBlock1 ) )
//// 			{
//// 			}
//		}
//		if( strcmp(info.TRCode,ETK_TR_ORD_CNCL)==0 )
//		{
//			typedef struct
//			{
//				t5513OutBlock	outBlock;
//				t5513OutBlock1	outBlock1;
//			} t5513AllOutBlock, *LPt5513AllOutBlock;
//			
//			LPt5513AllOutBlock pAllOutBlock = (LPt5513AllOutBlock)pRpData->lpData;
//			
//			// ���� ������ ũ�Ⱑ Block ũ�⺸�� �۴ٸ� ó������ �ʴ´�.
//			if( pRpData->nDataLength >= sizeof( t5513OutBlock ) )
//			{
//				CCVT_ETK cvt;
//				cvt.de_KS_TR_Order_Cncl( (char*)&pAllOutBlock->outBlock1, info.GDS_CODE, info.CLORD_NO, szLabel, szBody );
//			}
//			
//// 			if( pRpData->nDataLength >= sizeof( t5513OutBlock ) + sizeof( t5513OutBlock1 ) )
//// 			{
//// 			}
//		}
//
//#if 0
//		//////////////////////////////////////////////////////////////////////////
//		//	MQ ����
//		HRESULT hr = g_MQRslt_Ord.MQSend( szLabel, szBody ) ;
//		if( FAILED(hr) )
//		{
//			CUtil::PrintLogEx(&m_logF, FALSE, "[%*.*s](err:%s)", 
//											DEF_MSG_LEN, DEF_MSG_LEN, "�ֹ�TR�������۽���",g_MQRslt_Ord.GetLastError());
//			return -1L;
//		}
//		CUtil::PrintLogEx( &m_logF, FALSE, "[%*.*s](%s)"
//										, DEF_MSG_LEN, DEF_MSG_LEN
//										, "4-�ֹ�����TR MQ ����"
//										,szBody
//										);
//#endif
//	}
//	
//	//-------------------------------------------------------------------------------------
//	// �޽����� ����
//	else if( wParam == MESSAGE_DATA )
//	{
//		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
//
//		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
//		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		
//		
//		g_iXingAPI.ReleaseMessageData( lParam );
//	}
//	
//	//-------------------------------------------------------------------------------------
//	// System Error�� ����
//	else if( wParam == SYSTEM_ERROR_DATA )
//	{
//		LPMSG_PACKET pMsg = (LPMSG_PACKET)lParam;
//		
//		CString strMsg( (char*)pMsg->lpszMessageData, pMsg->nMsgLength );
//		GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)pMsg->szMsgCode, (LPARAM)(LPCTSTR)strMsg );		
//
//		g_iXingAPI.ReleaseMessageData( lParam );
//	}
//
//	//-------------------------------------------------------------------------------------
//	// Release Data�� ����
//	else if( wParam == RELEASE_DATA )
//	{
//		g_iXingAPI.ReleaseRequestData( (int)lParam );
//	}
//
	return 0L;
}

//--------------------------------------------------------------------------------------
// Timeout �߻�
//--------------------------------------------------------------------------------------
LRESULT CDlg_t5511::OnXMTimeoutData( WPARAM wParam, LPARAM lParam )
{
	g_iXingAPI.ReleaseRequestData( (int)lParam );
	GetParent()->SendMessage( WMU_SET_MESSAGE, (WPARAM)"", (LPARAM)"Timeout" );		

	return 0L;
}

void CDlg_t5511::OnDestroy() 
{
	BOOL bAdvise = TRUE;
	RegistRealData(!bAdvise, ETK_REAL_ACPT_ORD );
	RegistRealData(!bAdvise, ETK_REAL_ACPT_MDFYCNCL );
	RegistRealData(!bAdvise, ETK_REAL_ACPT_CNTR );

	StopThread();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

VOID CDlg_t5511::StopThread()
{
	SetEvent(m_hDie);
	if(WaitForSingleObject(m_hThread, 3000)!=WAIT_OBJECT_0)
		TerminateThread(m_hThread,0);
	
	if(m_hThread){
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if(m_hDie){
		CloseHandle(m_hDie);
		m_hDie = NULL;
	}
}




/*******************************************************
	�ֹ�MQ �� �д� ������
*******************************************************/
DWORD WINAPI CDlg_t5511::ThreadProc(LPVOID lp)
{
	CoInitialize(NULL);
	
	CDlg_t5511* p = (CDlg_t5511*)lp;
	
//	HRESULT hr;
	DWORD	dwRet	= 0;
//	char	szLabel		[MAX_BUFSIZE];	// ID
//	char	szBody		[MAX_TRSIZE];
///	char	szSendBuf	[MAX_TRSIZE];
//	char	szOrdTime	[32];
	int		nSendLen= 0;
	
	//CCVT_Dongbu	cvt;
	
	
#if 0
	BOOL bRet = TRUE;
	while(bRet)
	{		
		while( WaitForSingleObject(p->m_hDie, 10)!=WAIT_OBJECT_0 )
		{
			ZeroMemory( szLabel, sizeof(szLabel) );
			ZeroMemory( szBody, sizeof(szBody) );
			ZeroMemory( szSendBuf, sizeof(szSendBuf) );
			
			hr = g_MQReq_Ord.MQReceive(szLabel, szBody, 1);
			if( FAILED(hr) )
			{
				Sleep(1);
				continue;
			}
			
			//	DATA �� ������ �ٽ�
			if(!lstrlen(szBody))	
			{
				Sleep(1);
				continue;
			}
			
// 			//	����DRCM�� ������ �ʵǾ� ������ REJECT ����
// 			if( !p->m_bConn ){
// 				p->ReturnRjct( szBody, RET_API_DISCONNED);
// 				continue;
// 			}

			sprintf(p->m_szMsg, "[%*.*s](%s)"
								, DEF_MSG_LEN, DEF_MSG_LEN
								, "1-�ֹ�����(ST_SV_ORD_CHK_RSLT)"
								,szBody
								);
			CUtil::PrintLogEx( &p->m_logF, FALSE, p->m_szMsg );

			
			//	����ð��� �ֹ��ð��� ���Ͽ� ���̰� �ʹ� ũ�� REJECT
			ST_SV_ORD_CHK_RSLT* pOrd = (ST_SV_ORD_CHK_RSLT*)szBody;
			sprintf( szOrdTime, "%.*s", sizeof(pOrd->ORD_TM), pOrd->ORD_TM );
			if( CUtil::IsPassedMin(szOrdTime, p->m_nOrdAliveMin) )
			{
				//p->ReturnRjct(szBody, RET_PASSED_ORD_VALID_TIME);
				continue;
			}
			

			//	�α����� �ȵ� �����̸� �ź� ó��
			if( g_stAPI.LOGON_YN[0]=='N' )
			{
				sprintf(p->m_szMsg, "[%*.*s]�α׿��� �ȵȻ����̹Ƿ� �ź� ó��(%s)", 
									DEF_MSG_LEN, DEF_MSG_LEN, "��ü�ź�",szBody);
				
				CUtil::PrintLogEx(&p->m_logF, FALSE, p->m_szMsg );
				
				//	�ź�ó��
				p->ReturnRjct(RET_API_DISCONNED, pOrd->stC.GDS_CODE, pOrd->ClORD_NO, "���ǻ�API�� ������� ����");
				continue;
			}
			
			//	ETK CONVERT & send
			p->RequestData(szLabel, szBody);
			Sleep(0);
		}
	}
#endif

	CoUninitialize();
	return 0;
}


/*************************************************************************
* ������ �ʵǾ� �ְų�, �ֹ��ð� ���̰� �ʹ� ũ�� �źθ� ����
*************************************************************************/
VOID CDlg_t5511::ReturnRjct(int i_nErrCode, char* i_psGDS_CODE, char* i_psCLORD_NO, char* i_psErrMsg)
{
//	char	szLabel	[128]			= {0,};
//	char	szBody	[MAX_BUFSIZE]	= {0,};
//	
//	ST_MQLABEL_API_INFO* pLabel	= (ST_MQLABEL_API_INFO*)szLabel;
//	ST_IFRECV_TR_ORD* pBody		= (ST_IFRECV_TR_ORD*)szBody;
//	
//	memset( szLabel, 0x20, sizeof(szLabel) );
//	*( szLabel+sizeof(ST_MQLABEL_API_INFO) ) = 0x00;
//	
//	memset( szBody, 0x20, sizeof(szBody) );
//	*( szBody+sizeof(ST_IFRECV_TR_ORD) ) = 0x00;
//
//	memcpy( pLabel->API_CODE,	APIKEY_KS_ETK,	min(strlen(APIKEY_KS_ETK),sizeof(pLabel->API_CODE) ) );
//	memcpy( pLabel->CODE,		MQ_MSGTYPE_TR,	sizeof(pLabel->CODE) );
//	memcpy( pLabel->GDS_CODE,	i_psGDS_CODE,	sizeof(pLabel->GDS_CODE) );
//	memcpy( pLabel->NTTN_CODE,	NTTN_CODE_10,	sizeof(pLabel->NTTN_CODE) );
//	
//	memcpy( pBody->GDS_CODE,	pLabel->GDS_CODE,	sizeof(pBody->GDS_CODE) );
//	sprintf( pBody->CLORD_NO,	i_psCLORD_NO,		sizeof(pBody->CLORD_NO) );
//	
//// 	char szOrdNo[32];
//// 	sprintf( szOrdNo, "%.*s", sizeof(pIn->ordernum), pIn->ordernum );	
//// 	memcpy( pBody->ORD_NO, szOrdNo, min(strlen(szOrdNo),sizeof(pBody->ORD_NO) ) );
//	
//	sprintf( pBody->RSLT_CODE,	"%0*d",		sizeof(pBody->RSLT_CODE), i_nErrCode );
//	sprintf( pBody->API_CODE,	"%-*.*s",	sizeof(pLabel->API_CODE), sizeof(pLabel->API_CODE),	pLabel->API_CODE );
//	sprintf( pBody->API_MSGCODE,"%-*.*s",	sizeof(pBody->API_MSGCODE),	"000000" );
//	memcpy( pBody->API_MSG, i_psErrMsg, strlen(i_psErrMsg) );
//	
//
////	g_MQRslt_Ord.MQSend( szLabel, szBody );
//
//	CUtil::PrintLogEx( &m_logF, FALSE, "���� �ź� ó��(%s)", szBody );
//	
}



/**********************************************************
	�ǽð� tr �� ����Ѵ�.
**********************************************************/
BOOL CDlg_t5511::RegistRealData(BOOL bAdvise, char* i_psTRCode)
{
	//TCHAR	szTrCode[32];
	//int		nSize;
	//BOOL	bSuccess;
	//char	trname[32];
	//char	msg[512];

	//strcpy( szTrCode, i_psTRCode );

	//if( strcmp( i_psTRCode, ETK_REAL_ACPT_ORD)==0 )
	//{
	//	sprintf(trname, "�ֹ��ǽð�");
	//	nSize = 0;//sizeof(O01_InBlock);
	//}
	//else if ( strcmp( i_psTRCode, ETK_REAL_ACPT_MDFYCNCL)==0 )
	//{
	//	sprintf(trname, "������ҽǽð�");
	//	nSize = 0;//sizeof(O01_InBlock);
	//}
	//else if ( strcmp( i_psTRCode, ETK_REAL_ACPT_CNTR)==0 )
	//{
	//	sprintf(trname, "ü��ǽð�");
	//	nSize = 0;//sizeof(O01_InBlock);
	//}


	//// ������ ����
	//if(bAdvise)
	//{
	//	bSuccess = g_iXingAPI.AdviseRealData( 
	//								GetSafeHwnd(),	// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
	//								szTrCode,		// TR ��ȣ
	//								"",				// �����ڵ� - �ֹ�����, ������� ü���� ��쿡�� �� ���� ���ǹ��ϴ�....
	//								nSize			// �����ڵ� ����
	//								);	
	//	// ����üũ
	//	if( bSuccess == FALSE )	sprintf(msg,  "%s ��� ����", trname);
	//	else					sprintf(msg,  "%s ��� ����", trname);
	//}
	//else
	//{
	//	bSuccess = g_iXingAPI.UnadviseRealData( 
	//								GetSafeHwnd(),	// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
	//								szTrCode,		// TR ��ȣ
	//								"",				// �����ڵ� - �ֹ�����, ������� ü���� ��쿡�� �� ���� ���ǹ��ϴ�....
	//								nSize			// �����ڵ� ����
	//								);	
	//	// ����üũ
	//	if( bSuccess == FALSE )	sprintf(msg,  "%s ���� ����", trname);
	//	else					sprintf(msg,  "%s ���� ����", trname);
	//}
	//if( !bSuccess ){
	//	CUtil::PrintLogEx( &m_logF, FALSE, msg);
	//	return FALSE;
	//}

	return TRUE;
}



//	DB ���� ����/OPTION ���񸮽�Ʈ�� �����´�.
BOOL CDlg_t5511::GetStkCodeList_KS(char* psGdsCode)
{
	//BOOL bKSF;	// ����, �ɼ� ����
	
	//if( strcmp(psGdsCode, GDS_CODE_KSO)==0 )
	//{
	//	bKSF = FALSE;
	//	
	//	if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KSO" ) )
	//		return FALSE;
	//}
	//if( strcmp(psGdsCode, GDS_CODE_KSF)==0 )
	//{
	//	bKSF = TRUE;
	//	
	//	if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KSF" ) )
	//		return FALSE;
	//}
	//
	//if( !m_db.Exec() )
	//	return FALSE;
	//
	//char szStkList[1024]	= {0,};
	//char szStk[128]			= {0,};
	//int nRs = 0;
	//while( m_db. nextRow() )
	//{
	//	strcpy( szStk, m_db.Get(1) );
	//	CUtil::TrimAll( szStk, strlen(szStk) );

	//	CUtil::PrintDebug( &m_logF, FALSE, szStk );
	//	
	//	if( strlen(szStk)==0 )
	//		break;
	//	
	//	CString sStk = szStk;
	//	if(bKSF)
	//		m_sStkList_KSF.push_back(sStk);
	//	else
	//		m_sStkList_KSO.push_back(sStk);
	//}
	//m_db.Close();
	
	return TRUE;
}



/************************************************************
	�ֹ�/ü�� REAL �����͸� ����
	- C01  �����ֹ�ü��  
	- H01  �����ֹ��������  
	- O01  �����ֹ�����  
************************************************************/
LRESULT CDlg_t5511::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
//	char msg[4096];
//	char szSendBuf[MAX_SENDSIZE] = {0,};
//
////	HRESULT hr;
//	
//	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;
//	
//	
//	//////////////////////////////////////////////////////////////////////////
//	//	�ֹ�����
//	if( strcmp( pRealPacket->szTrCode, ETK_REAL_ACPT_ORD ) == 0 )
//	{
//		O01_OutBlock* pIn = (O01_OutBlock*)pRealPacket->pszData;
//
//		//	SONBT001:�ű��ֹ� / SONBT002:�����ֹ� / SONBT003:����ֹ�
//		if( strncmp(pIn->trcode, "SONBT001", 8 )!=0 )
//			return 0L;
//
//		CUtil::PrintLogEx( &m_logF, FALSE, "[%*.*s](%.1024s)"
//										, DEF_MSG_LEN, DEF_MSG_LEN
//										, "5-�ű��ֹ�REAL ����"
//										, pRealPacket->pszData 
//																);
//
//		CCVT_ETK cvt;
//		cvt.de_KS_RL_ORD( pRealPacket->pszData, szSendBuf );
////		hr = g_MQRslt_Ord.MQSend("", szSendBuf);
////		if(FAILED(hr))
////	{
////			CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ�REAL ���۽���(%s)", g_MQRslt_Ord.GetLastError() );
////		}
//	}
//	
//	///////////////////////////////////////////////////////////////////////////
//	//	�ֹ��������
//	if( strcmp( pRealPacket->szTrCode, ETK_REAL_ACPT_MDFYCNCL ) == 0 )
//	{
//		CUtil::PrintLogEx( &m_logF, FALSE, "[%*.*s](%.1024s)"
//										, DEF_MSG_LEN, DEF_MSG_LEN
//										, "5-���/�����ֹ�REAL ����"
//										, pRealPacket->pszData 
//																);
//		
//		//	������Ŷ���� ���� �� MQ ����
//		CCVT_ETK cvt;
//		cvt.de_KS_RL_MDFYCNCL( pRealPacket->pszData, szSendBuf );
////		hr = g_MQRslt_Ord.MQSend("", szSendBuf);
////		if(FAILED(hr))
////		{
////			CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ�REAL ���۽���(%s)", g_MQRslt_Ord.GetLastError() );
////		}
//	}
//	
//	//////////////////////////////////////////////////////////////////////////
//	//	ü��
//	if( strcmp( pRealPacket->szTrCode, ETK_REAL_ACPT_CNTR ) == 0 )
//	{
//		CUtil::PrintLogEx( &m_logF, FALSE, "[%*.*s](%.1024s)"
//										, DEF_MSG_LEN, DEF_MSG_LEN
//										, "5-ü��REAL ����"
//										, pRealPacket->pszData 
//																);
//		
//		//	������Ŷ���� ���� �� MQ ����
//		CCVT_ETK cvt;
//		cvt.de_KS_RL_CNTR( pRealPacket->pszData, szSendBuf );
////		hr = g_MQRslt_Cntr.MQSend("", szSendBuf);
////		if(FAILED(hr))
////		{
////			CUtil::PrintLogEx(&m_logF, FALSE, "�ֹ�REAL ���۽���(%s)", g_MQRslt_Ord.GetLastError() );
////		}
//	}
//
	return 0L;
}
