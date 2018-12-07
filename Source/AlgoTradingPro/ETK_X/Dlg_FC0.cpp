// Dlg_FC0.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_FC0.h"

#include "../Inc_ETK/FC0.h"
#include "../Inc_ETK/FH0.h"
#include "../Inc_ETK/OC0.h"
#include "../Inc_ETK/OH0.h"

//CME
//#include "../../include/API_ETK/NC0.h"
//#include "../../include/API_ETK/NH0.h"

#include "../Inc_ETK/CVT_ETK.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CLogDebug g_logEvent("ETK_X");


//CME
BOOL g_bCME = FALSE;

char g_CnfgIdx[32]={0,};	// main ���� parameter �� �޴� ��

//////////////////////////////////////////////////////////////////////////
//	ȣ�� �ʴ����� ���� ������
int		g_nMaxCnt_F		= 0;
int		g_nSendCnt_F	= 0;
char	g_sLastTime_F	[7]={0,};

int		g_nMaxCnt_O		= 0;
int		g_nSendCnt_O	= 0;
char	g_sLastTime_O	[7]={0,};

//SMILOR.2������
#include <list>
using namespace std;
list<LPRECV_REAL_PACKET>		g_lstSise;

//CProp prop;

//int g_nCnt = 0;
/////////////////////////////////////////////////////////////////////////////
// CDlg_FC0 dialog

IMPLEMENT_DYNCREATE(CDlg_FC0, CDialog)

CDlg_FC0::CDlg_FC0(CWnd* pParent /*=NULL*/)	: CDialog(CDlg_FC0::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_FC0)
	//}}AFX_DATA_INIT

	//m_sockFront		= NULL;
	//m_sockLosscut	= NULL;
	m_dwThreadID = 0;
	m_hDie			= NULL;
	m_hThread		= NULL;
	InitializeCriticalSection(&m_cs);

	m_hThdDummySise	= NULL;	// skeo 2011-01-17

	//SMILOR.2������
	InitializeCriticalSection(&m_csSise);
	m_hSiseThr = NULL;

}


void CDlg_FC0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_FC0)
	DDX_Control(pDX, IDC_LIST_SISEHOGA, m_lstMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_FC0, CDialog)
	//{{AFX_MSG_MAP(CDlg_FC0)
	ON_BN_CLICKED(IDC_BUTTON_UNADVISE, OnButtonUnadvise)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest_1)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY  ()
	ON_BN_CLICKED( IDC_BUTTON_REQUEST,				OnButtonRequest	    )
	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_REAL_DATA,	OnXMReceiveRealData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_FC0 message handlers

BOOL CDlg_FC0::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrls();

	//-----------------------------------------------------------------------------------------
	//	LOG FIL OPEN
	//-----------------------------------------------------------------------------------------
	
	char szMsg[512];
	//TODO
	//if( !CUtil::GetLogDirCnfg(szPath, szMsg) )
	//{
	//	return FALSE;
	//}
	//
	//m_logF.OpenLog(szPath, "ETK_KS_X_v2.0");
	////m_logInner.OpenLog(szPath, "ETK_INNER_SISE");

	//CUtil::LogEx('I', &m_logF, NULL, FALSE, "���� ����");

	//////////////////////////////////////////////////////////////////////////
	//	SISE SHM OPEN
	char szSHMName[128], szMutexName[128];
	//if( !CUtil::GetCnfgFile( "common", "SHM", "SHM_SISE_KS", szSHMName) ||
	//	!CUtil::GetCnfgFile( "common", "SHM", "MUTEX_SISE_KS", szMutexName) )
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "KS200 SHM/MUTEX NAME �������� ���� [SHM:%s][MUTEX:%s]", szSHMName, szMutexName );
	//	return FALSE;
	//}
	//if ( !m_shm.Open( szSHMName, szMutexName ) )
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM_NAME OPEN ����(%s)", szSHMName);
	//	return FALSE;
	//}
	//CUtil::LogEx('I', &m_logF,  NULL, FALSE, "CSHMUpdate_KS : SHM OPEN ����(%s)(%s)", szSHMName, szMutexName);


	////////////////////////////////////////////////////////////////////////////
	////	DB OPEN 
	//if( !CUtil::DBOpen(&m_db) )
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "DB OPEN ERROR(%s)", m_db.GetLastError() );
	//	m_db.free();
	//	return FALSE;
	//}

	//////////////////////////////////////////////////////////////////////////
	//	UDP SOCKET INIT
	if( !SockInit() ){
		//m_db.free();
		return FALSE;
	}
	
	// �ü��߰�
	/*TCPRELAY
	BOOL bRet;
	m_clntSend = new CSiseClntSend_KS();
	bRet = m_clntSend->Init();
	CUtil::LogEx('I', &m_logF, NULL, FALSE, "%s", m_clntSend->GetMsg() );
	if( !bRet )	return FALSE;
	*/

	//TCPRELAY
	if( !Relay_Begin() )
		return FALSE;


	//CME. 17�� ���ĸ� CME ��ȸ�Ѵ�.
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(szMsg, "%02d:%02d", st.wHour, st.wMinute);
	if( strncmp( szMsg, "16:59", 5)>0 )
		g_bCME = TRUE;
	else
	{
		if( strncmp( szMsg, "06:00", 5)<0 )
			g_bCME = TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//	�������� ��������
	//TODO
	//if( !GetStkCodeList_KS(GDS_CODE_KSF) ){
	//	CUtil::LogEx(  'E', &m_logF, NULL, FALSE, "���� ���� �������� ����");
	//	m_db.free();
	//	return	FALSE;
	//}

	//	���簡 �����Ѵ�.
	list<CString>::iterator it;
	for( it = m_sStkList_KSF.begin(); it!=m_sStkList_KSF.end(); it++ )
	{
		CString sStkCode = *it;
		if( !GetNowPrc(sStkCode) )
			return FALSE;
	}

	//SMILOR.2������. �ü����Ű� ó�� �и�
#if 0	// THREAD ������ ���Ƴ���
	DWORD id;
	if( (m_hSiseThr = CreateThread(NULL,0,SiseThread,this,0,&id)) == NULL )
	{
		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü� ó�� THREAD ���� ����");
		return FALSE;
	}	
#endif

	//////////////////////////////////////////////////////////////////////////
	//	ȣ���� 1�ʿ� ���� MAX COUNT �� �����´�.
	//TODO
	//CUtil::GetCNFG("MAX_CNT", "HOGA_SEND_F", szMsg );
	//g_nMaxCnt_F = atoi(szMsg);

	//CUtil::GetCNFG("MAX_CNT", "HOGA_SEND_O", szMsg );
	//g_nMaxCnt_O = atoi(szMsg);

	//OnButtonRequest();

	////prop.SetBaseKey(HKEY_LOCAL_MACHINE, REG_ROOT);

	//sprintf( szMsg, "������������_TCPRELAY_v2.0(%s)", __DATE__ );
	//CUtil::LogEx('I', &m_logF, NULL, FALSE, szMsg);
	m_lstMsg.InsertString(0, szMsg);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//	���簡�� �����ͼ� �����Ѵ�.
BOOL CDlg_FC0::GetNowPrc( CString i_sStkCode )
{
// 	char szQ[32];
// 	char t[128];
// 	CString sNowPrc;
// 
// 	sprintf( szQ, "SELECT NOW_PRC FROM HT_T_PRC_LIST_KS WHERE STK_CODE='%s' ", psStkCode );
// 	if( !m_db.Cmd(szQ) || !m_db.Exec() )
// 		return FALSE;
// 
// 	if( !m_db.NextRow() )
// 		return FALSE;
// 
// 	strcpy( t, "%.2f", m_db.GetDouble(1));
// 	m_db.close();
// 
// 	sNowPrc = t;
// 	m_mapNowPrc[i_sStkCode] = sNowPrc;
 	return	TRUE;
}

//	DB ���� ����/OPTION ���񸮽�Ʈ�� �����´�.
BOOL CDlg_FC0::GetStkCodeList_KS(char* psGdsCode)
{
//	BOOL bKSF;	// ����, �ɼ� ����

// 	if( strcmp(psGdsCode, GDS_CODE_KSO)==0 )
// 	{
// 		bKSF = FALSE;
// 
// 		if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KSO" ) )
// 			return FALSE;
// 	}
// 	if( strcmp(psGdsCode, GDS_CODE_KSF)==0 )
// 	{
// 		bKSF = TRUE;
// 
// 		if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KS_TRADABLE" ) )
// 			return FALSE;
// 	}
// 	

//TODO
	//if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KS_TRADABLE_SVR" ) )
 //			return FALSE;
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
	//	
	//	if( strlen(szStk)==0 )
	//		break;
	//	
	//	CString sStk = szStk;
	//	if(szStk[0]=='1')
	//		m_sStkList_KSF.push_back(sStk);
	//	else
	//		m_sStkList_KSO.push_back(sStk);
	//}
	//m_db.Close();
	
	return TRUE;
}

VOID CDlg_FC0::SockClose()
{
 	EnterCriticalSection(&m_cs);
 	m_lstData.clear();
 	LeaveCriticalSection(&m_cs);

	char msg[128];
	sprintf(msg, "[FRONT_F_CLEAR:%d]\n", m_lstData.size());

	//TODO
	//SAFE_DELETE(m_sockFront);
	//SAFE_DELETE(m_sockLosscut);
}

/*********************************
	mcast socket init
*********************************/
BOOL CDlg_FC0::SockInit()
{
	//SockClose(i_nTarget);
	char szTemp[512] = {0,};
	//TODO
	//CUtil::GetCnfgFile( "common", "IP", "PUBLIC_IP", m_szLocalIP );
	//CUtil::GetCnfgFile( "common", "D_REAL_X.exe", "UDPPORT_KSF", szTemp);
	//m_nUdpPortReal = atoi(szTemp);

	//CUtil::GetCnfgFile( "common", "D_LC_X.exe", "UDPPORT_KSF", szTemp);
	//m_nUdpPortLC = atoi(szTemp);
 //
	/////////////////////////////////////////////////////////////////////////////
	////	UDP SOCKET ����
	//if(!m_sockFront)	m_sockFront = new CUdpSend;
	//if( !m_sockFront->Init_Sock(m_szLocalIP, m_nUdpPortReal) )
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE,"UDP SOCKET INIT ERROR");
	//	return FALSE;
	//}
	//	
	//CUtil::LogEx('I', &m_logF, NULL, FALSE, "[REAL UDP SOCKET INIT OK](%s)(%d)", m_szLocalIP, m_nUdpPortReal);
	//
	////lable:LOSSCUT_SISE
	//if(!m_sockLosscut)	m_sockLosscut = new CUdpSend;
	//if( !m_sockLosscut->Init_Sock(m_szLocalIP, m_nUdpPortLC) )
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE,"UDP SOCKET INIT ERROR");
	//	return FALSE;
	//}

	//CUtil::LogEx( 'I', &m_logF, NULL, FALSE, "[LOSSCUT UDP SOCKET INIT OK](%s)(%d)", m_szLocalIP, m_nUdpPortLC);

	return TRUE;
}

void CDlg_FC0::OnDestroy()
{

	OnButtonUnadvise();

//	int idx;
	SetEvent(m_hDie);
	
	if(WaitForSingleObject(m_hThread, 1000)!=WAIT_OBJECT_0)
		TerminateThread(m_hThread,0);

	if(WaitForSingleObject(m_hSiseThr, 1000)!=WAIT_OBJECT_0)
		TerminateThread(m_hSiseThr,0);

	CloseHandle(m_hThread);
	CloseHandle(m_hSiseThr);

	SockClose();
	DeleteCriticalSection(&m_cs);
	DeleteCriticalSection(&m_csSise);
	//TODO
	//m_db.Free();
	//
	//m_shm.Close();

	// skeo 2011-01-17 [[
	if(WaitForSingleObject(m_hThdDummySise, 1000) != WAIT_OBJECT_0)
		TerminateThread(m_hThdDummySise, 0);	
	if(m_hThdDummySise)	CloseHandle(m_hThdDummySise);	
	m_hThdDummySise	= NULL;
	// ]]

	//TCPRELAY if(m_clntSend)	delete m_clntSend;
	//TCPRELAY
	Relay_End();

	CDialog::OnDestroy();
}

/***************************************
	�ü� ��û
***************************************/
void CDlg_FC0::OnButtonRequest() 
{
	//	���� �ü�/ȣ�� ��û
	list<CString>::iterator it;
	for( it=m_sStkList_KSF.begin(); it!=m_sStkList_KSF.end(); it++ )
	{
		CString sStk = *it;
		//UnadviseData('Y'/*�ü�*/, sStk, GDS_CODE_KSF);
		AdviseData('Y'/*�ü�*/, sStk, GDS_CODE_KSF);

		//UnadviseData('N'/*ȣ��*/, sStk, GDS_CODE_KSF);
		AdviseData('N'/*ȣ��*/, sStk, GDS_CODE_KSF);

		//break;
	}

	//	�ɼ� �ü� ��û
	for( it=m_sStkList_KSO.begin(); it!=m_sStkList_KSO.end(); it++ )
	{
		CString sStk = *it;
		//UnadviseData('Y'/*�ü�*/, sStk, GDS_CODE_KSO);
		AdviseData('Y'/*�ü�*/, sStk, GDS_CODE_KSO);
		
		//UnadviseData('N'/*ȣ��*/, sStk, GDS_CODE_KSO);
		AdviseData('N'/*ȣ��*/, sStk, GDS_CODE_KSO);

		//break;
	}
}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_FC0::InitCtrls()
{
	//-------------------------------------------------------------------------
	// ���� ü�� ��
// 	m_ctrlOutBlock.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
// 	m_ctrlOutBlock.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );
// 
// 	int nRow = 0;
// 	m_ctrlOutBlock.InsertItem( nRow++, "ü��ð�          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "���ϴ�񱸺�      " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "���ϴ��          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�����            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "���簡            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�ð�              " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "��              " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "����              " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "ü�ᱸ��          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "ü�ᷮ            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�����ŷ���        " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�����ŷ����      " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�����ü�ᷮ    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�����ü��Ǽ�  " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�ż�����ü�ᷮ    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�ż�����ü��Ǽ�  " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "ü�ᰭ��          " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�ŵ�ȣ��1         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�ż�ȣ��1         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�̰�����������    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "KOSPI200����      " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�̷а�            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "������            " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "����BASIS         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�̷�BASIS         " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�̰�����������    " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "������        " );
// 	m_ctrlOutBlock.InsertItem( nRow++, "���ϵ��ð���ŷ���" );
// 	m_ctrlOutBlock.InsertItem( nRow++, "�����ڵ�          " );
// 
// 	//-------------------------------------------------------------------------
// 	// ���� ȣ�� ��
// 	m_ctrlOutBlock_H.InsertColumn( 0, "�ʵ�  ", LVCFMT_LEFT, 150 );
// 	m_ctrlOutBlock_H.InsertColumn( 1, "������", LVCFMT_LEFT, 200 );
// 	
// 	nRow = 0;
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "ȣ���ð�          " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��1         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��2         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��3         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��4         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ��5         " );	
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��1         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��2         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��3         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��4         " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ��5         " );
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ǽ�5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���ѰǼ�    " );	
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ǽ�5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���ѰǼ�    " );
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ������5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ŵ�ȣ���Ѽ���    " );	
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������1     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������2     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������3     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������4     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ������5     " );
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�ż�ȣ���Ѽ���    " );	
// 
// 	m_ctrlOutBlock_H.InsertItem( nRow++, "�����ڵ�          " );

	SetDlgItemText(IDC_BUTTON_TEST, "FILE TEST Start");	// skeo 2011-01-17
}

//--------------------------------------------------------------------------------------
// ������ Advise
//--------------------------------------------------------------------------------------
void CDlg_FC0::AdviseData(char i_cSiseYN, CString sStk, char* i_psGDS_CODE)
{
	TCHAR	szTrCode[32];
	int		nSize;
	char	Exch[32] = "KRX";

	if( i_cSiseYN=='Y' )
	{
		if( strcmp(i_psGDS_CODE, GDS_CODE_KSF)==0 )
		{
			//CME
			if( !g_bCME )
			{
				strcpy( szTrCode, ETK_REAL_SISE_F);
				/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
				nSize = sizeof( FC0_InBlock );
			}
			//else
			//{
			//	strcpy( szTrCode, ETK_REAL_SISE_F_CME);
			//	/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
			//	nSize = sizeof( NC0_InBlock );
			//	strcpy( Exch, "CME");

			//}
		}
		else
		{
			strcpy( szTrCode, ETK_REAL_SISE_O);
			/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
			nSize = sizeof( OC0_InBlock );

		}
	}
	else
	{
		//CME
		if( strcmp(i_psGDS_CODE, GDS_CODE_KSF)==0 )
		{
			if( !g_bCME ){
				strcpy( szTrCode, ETK_REAL_HOGA_F);
				/*GetDlgItemText( IDC_EDIT_CODE_H, strCode );*/
				nSize = sizeof( FH0_InBlock );
			}
			//else
			//{
			//	strcpy( szTrCode, ETK_REAL_HOGA_F_CME);
			//	/*GetDlgItemText( IDC_EDIT_CODE_H, strCode );*/
			//	nSize = sizeof( NH0_InBlock );
			//	strcpy( Exch, "CME");
			//}
		}
		else
		{
			strcpy( szTrCode, ETK_REAL_HOGA_O);
			/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
			nSize = sizeof( OH0_InBlock );
		}
	}

	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		sStk,						// �����ڵ�
		nSize						// �����ڵ� ����
		);

	//-----------------------------------------------------------
	// ����üũ
	char msg[512];
	if( bSuccess == FALSE )
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s](%s)�ü� ��û ����", Exch, (LPCSTR)sStk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s](%s)ȣ�� ��û ����", Exch, (LPCSTR)sStk );
	}
	else
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s](%s)�ü� ��û ����", Exch, (LPCSTR)sStk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s](%s)ȣ�� ��û ����", Exch, (LPCSTR)sStk );
	}
	//CUtil::LogEx('I', &m_logF, NULL, FALSE, msg );
	m_lstMsg.InsertString(0, msg);
}

//--------------------------------------------------------------------------------------
// ������ Unadvise
//--------------------------------------------------------------------------------------
void CDlg_FC0::UnadviseData(char i_cSiseYN, CString sStk, char* i_psGDS_CODE)
{
	TCHAR	szTrCode[32];
	int		nSize;
	char	Exch[32] = "KRX";
	
	if( i_cSiseYN=='Y' )
	{
		if( strcmp(i_psGDS_CODE, GDS_CODE_KSF)==0 )
		{
			//CME
			if( !g_bCME ){
				strcpy( szTrCode, ETK_REAL_SISE_F);
				/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
				nSize = sizeof( FC0_InBlock );
			}
			//else{
			//	strcpy( szTrCode, ETK_REAL_SISE_F_CME);
			//	/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
			//	nSize = sizeof( NC0_InBlock );
			//	strcpy( Exch, "CME");
			//}
		}
		else
		{
			strcpy( szTrCode, ETK_REAL_SISE_O);
			/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
			nSize = sizeof( OC0_InBlock );
		}
	}
	else
	{
		if( strcmp(i_psGDS_CODE, GDS_CODE_KSF)==0 )
		{
			//CME
			if( !g_bCME ){
				strcpy( szTrCode, ETK_REAL_HOGA_F);
				/*GetDlgItemText( IDC_EDIT_CODE_H, strCode );*/
				nSize = sizeof( FH0_InBlock );
			}
			//else
			//{
			//	strcpy( szTrCode, ETK_REAL_HOGA_F_CME);
			//	/*GetDlgItemText( IDC_EDIT_CODE_H, strCode );*/
			//	nSize = sizeof( NH0_InBlock );
			//	strcpy( Exch, "CME");
			//}
		}
		else
		{
			strcpy( szTrCode, ETK_REAL_HOGA_O);
			/*GetDlgItemText( IDC_EDIT_CODE, strCode );*/
			nSize = sizeof( OH0_InBlock );
		}
	}
	
	//-----------------------------------------------------------
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
												GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
												szTrCode,					// TR ��ȣ
												sStk,						// �����ڵ�
												nSize						// �����ڵ� ����
												);
	
	//-----------------------------------------------------------
	// ����üũ
	char msg[512];
	if( bSuccess == FALSE )
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s](%s)�ü� ���� ��û ����", Exch, (LPCSTR)sStk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s](%s)ȣ�� ���� ��û ����", Exch, (LPCSTR)sStk );
	}
	else
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s](%s)�ü� ���� ��û ����", Exch, (LPCSTR)sStk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s](%s)ȣ�� ���� ��û ����", Exch, (LPCSTR)sStk );
	}
	//CUtil::LogEx('I', &m_logF, NULL, FALSE, msg );
	m_lstMsg.InsertString(0, msg);
}

//--------------------------------------------------------------------------------------
// REAL �����͸� ����
//--------------------------------------------------------------------------------------
//SMILOR.2������. ���Ž�����(ProcessANM)�� ó��������(SiseThread) �и�
#if 0
LRESULT CDlg_FC0::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	LPRECV_REAL_PACKET p = new RECV_REAL_PACKET;

	memcpy( p, pRealPacket, sizeof(RECV_REAL_PACKET) );

	EnterCriticalSection(&m_csSise);
	g_lstSise.push_back(p);
	LeaveCriticalSection(&m_csSise); 
	return 0L;
}
#endif



#if 0
//SMILOR.2������. �ü� ó�� ������
DWORD WINAPI CDlg_FC0::SiseThread(LPVOID lp)
{
	CDlg_FC0* p = (CDlg_FC0*)lp;
	LPRECV_REAL_PACKET pData;

	while(1)
	{
		if(g_lstSise.empty())
		{
			Sleep(1);
			continue;
		}
		EnterCriticalSection(&p->m_csSise);
		pData = g_lstSise.front();
		g_lstSise.pop_front();
		LeaveCriticalSection(&p->m_csSise);
		p->SiseProc(pData);
		delete pData;
	}
	
	
}
#endif


#if 0
//SMILOR.2������. �ü�ó�� �Լ�
VOID CDlg_FC0::SiseProc(LPRECV_REAL_PACKET p)
{
	char msg[4096] = {0,};
	char szSendBuf[MAX_SENDSIZE] = {0,};
	char szSendBuf_toFRONT[MAX_SENDSIZE] = {0,};
	char cStk;
	BOOL bHoga = FALSE;
	
	BOOL bFind = FALSE;
	
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)p;

	//////////////////////////////////////////////////////////////////////////
	//	����ü��(�ü�)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F ) == 0 )
	{
		cStk = 'F';
		//sprintf( msg, "[F_SISE][%.*s]", sizeof(FC0_OutBlock), pRealPacket->pszData );
		//m_lstMsg.InsertString(0, msg);
		//CUtil::LogEx(&m_logF, FALSE, msg);

		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;
		//�ü��������.		cvt.de_KS_F_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT );
		double dErrSise = 0;
		if( !cvt.de_KS_F_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT, &dErrSise ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			if( strcmp(szTime, "10:00")>0 ){
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "���簡�̻�!!!(%f)", dErrSise );
			}
			return ;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//	����ȣ��
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F ) == 0 )
	{

		bHoga = TRUE;
		cStk = 'F';
		//sprintf( msg, "[F_HOGA][%.*s]", sizeof(FH0_OutBlock), pRealPacket->pszData );
		//m_lstMsg.InsertString(0, msg);
		//CUtil::LogEx(&m_logF, FALSE, msg);

		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;

		if( !cvt.de_KS_F_Hoga( pRealPacket->pszData, szSendBuf ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			if( strcmp(szTime, "10:00")>0 ){
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü��̻�ó��!!!(%.*s)"
								, sizeof(FH0_OutBlock), pRealPacket->pszData );
			}
			return ;
		}
		strcpy( szSendBuf_toFRONT, szSendBuf );
	}

	//CME
	//////////////////////////////////////////////////////////////////////////
	//	CMEü��(�ü�)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F_CME ) == 0 )
	{
		cStk = 'F';
		//sprintf( msg, "[CME_F_SISE][%.*s]", sizeof(FC0_OutBlock), pRealPacket->pszData );
		//m_lstMsg.InsertString(0, msg);
		//CUtil::LogEx(&m_logF, FALSE, msg);
		
		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;
		cvt.de_KS_F_SiseEx_CME( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT );
	}
	
	///////////////////////////////////////////////////////////////////////////
	//	CMEȣ��
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F_CME ) == 0 )
	{
		
		bHoga = TRUE;
		cStk = 'F';
		//sprintf( msg, "[CME_F_HOGA][%.*s]", sizeof(FH0_OutBlock), pRealPacket->pszData );
		//m_lstMsg.InsertString(0, msg);
		//CUtil::LogEx(&m_logF, FALSE, msg);
		
		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;
		
		if( !cvt.de_KS_F_Hoga_CME( pRealPacket->pszData, szSendBuf ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			if( strcmp(szTime, "10:00")>0 ){
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü��̻�ó��!!!(%.*s)"
					, sizeof(FH0_OutBlock), pRealPacket->pszData );
			}
			return ;
		}
		strcpy( szSendBuf_toFRONT, szSendBuf );
	}


	//////////////////////////////////////////////////////////////////////////
	//	�ɼ� ü��(�ü�)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_O ) == 0 )
	{
		cStk = 'O';
		//sprintf( msg, "[O_SISE][%.*s]", sizeof(OC0_OutBlock), pRealPacket->pszData );
	//	//m_lstMsg.InsertString(0, msg);
		//CUtil::LogEx(&m_logF, FALSE, msg);

		//	������Ŷ���� ���� 
		CCVT_ETK cvt;
		cvt.de_KS_O_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT);
		//cvt.de_KS_O_Sise( pRealPacket->pszData, szSendBuf);
	}
	///////////////////////////////////////////////////////////////////////////
	//	�ɼ� ȣ��
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_O ) == 0 )
	{
		cStk = 'O';
		//sprintf( msg, "[O_HOGA][%.*s]", sizeof(OH0_OutBlock), pRealPacket->pszData );
		//m_lstMsg.InsertString(0, msg);
		//CUtil::LogEx(&m_logF, FALSE, msg);

		//	������Ŷ���� ���� 
		CCVT_ETK cvt;
		//cvt.de_KS_O_Hoga( pRealPacket->pszData, szSendBuf );
		//strcpy( szSendBuf_toFRONT, szSendBuf );
		cvt.de_KS_O_Hoga( pRealPacket->pszData, szSendBuf_toFRONT );		
	}

//	int nErrCode;

	//	Ȥ�� futures, option �� �������� ����Ͽ� if �Ѵ�.
	//if( cStk=='F' || cStk=='O' )
	{
		if(bHoga)
		{
			if( (prop.GetLongValue("RUN1")==0)  )
				return ;
		}

		//	FRONT ���� ����		
		int nRet = m_sockFront->Send_Data( szSendBuf_toFRONT, strlen(szSendBuf_toFRONT) );
		//int nRet = m_sockFront->Send_Data( szSendBuf_toFRONT, sizeof(REAL_HOGA_KS) );
		if( nRet<=0 )
		{
			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü������̻�!!!(%s)", szSendBuf_toFRONT );
			ASSERT(nRet>0);
		}
	}

	//	SHM �� ����
	SaveSHM( szSendBuf );
	
	
	return ;
}
#endif



//--------------------------------------------------------------------------------------
// REAL �����͸� ����
//--------------------------------------------------------------------------------------
#if 1
LRESULT CDlg_FC0::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	char msg[4096] = {0,};
	char szSendBuf[MAX_SENDSIZE] = {0,};
	char szSendBuf_toFRONT[MAX_SENDSIZE] = {0,};
	char cStk;
	INT nLen;
	BOOL bHoga = FALSE;	
	BOOL bFind = FALSE;
	BOOL bDongsi = FALSE;
	
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

	//////////////////////////////////////////////////////////////////////////
	//	����ü��(�ü�)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F ) == 0 )
	{
		cStk = 'F';
		
		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;
		//�ü��������.		cvt.de_KS_F_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT );
		double dErrSise = 0;
		if( !cvt.de_KS_F_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT, &dErrSise ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			//if( strcmp(szTime, "10:00")>0 ){
			//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "���簡�̻�!!!(%f)", dErrSise );
			//}
			return 0L;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//	����ȣ��
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F ) == 0 )
	{

		bHoga = TRUE;
		cStk = 'F';
		
		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;

		if( !cvt.de_KS_F_HogaDongsi( pRealPacket->pszData, szSendBuf, &bDongsi ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			//if( strcmp(szTime, "10:00")>0 ){
			//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü��̻�ó��!!!(%.*s)"
			//					, sizeof(FH0_OutBlock), pRealPacket->pszData );
			//}
			return 0L;
		}
		//strcpy( szSendBuf_toFRONT, szSendBuf );
	}

	//CME
	//////////////////////////////////////////////////////////////////////////
	//	CMEü��(�ü�)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F_CME ) == 0 )
	{
		cStk = 'F';
		
		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;
		cvt.de_KS_F_SiseEx_CME( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT );
	}
	
	///////////////////////////////////////////////////////////////////////////
	//	CMEȣ��
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F_CME ) == 0 )
	{
		
		bHoga = TRUE;
		cStk = 'F';
		
		//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
		CCVT_ETK cvt;
		
		if( !cvt.de_KS_F_Hoga_CME( pRealPacket->pszData, szSendBuf ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			//if( strcmp(szTime, "10:00")>0 ){
			//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü��̻�ó��!!!(%.*s)"
			//		, sizeof(FH0_OutBlock), pRealPacket->pszData );
			//}
			return 0L;
		}
		//strcpy( szSendBuf_toFRONT, szSendBuf );
	}


	//////////////////////////////////////////////////////////////////////////
	//	�ɼ� ü��(�ü�)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_O ) == 0 )
	{
		cStk = 'O';
		
		//	������Ŷ���� ���� 
		CCVT_ETK cvt;
		cvt.de_KS_O_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT);
	}
	///////////////////////////////////////////////////////////////////////////
	//	�ɼ� ȣ��
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_O ) == 0 )
	{
		cStk = 'O';
		bHoga = TRUE;
		
		//	������Ŷ���� ���� 
		CCVT_ETK cvt;
		cvt.de_KS_O_Hoga( pRealPacket->pszData, szSendBuf );		
		//strcpy( szSendBuf_toFRONT, szSendBuf );
	}

	//	Ȥ�� futures, option �� �������� ����Ͽ� if �Ѵ�.
	//if( cStk=='F' || cStk=='O' )
	{
		/*if(bHoga)
		{
			if( (prop.GetLongValue("RUN1")==0)  )
				return 0;
		}*/

		

		// v2.3 �ν��ƿ��� ����
		
		//	FRONT ���� ����		
		int nRet;
		if( bHoga ){
			nLen = strlen(szSendBuf);
			//TODO nRet = m_sockFront->Send_Data( szSendBuf,  nLen);
		}
		else
		{
			nLen = strlen(szSendBuf_toFRONT);
			//nRet = m_sockFront->Send_Data( szSendBuf_toFRONT,  nLen);
			//if( nRet<=0 )
			//{
			//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü������̻�!!!(%s)", szSendBuf_toFRONT );
			//	//ASSERT(nRet>0);
			//}

			//nRet = m_sockLosscut->Send_Data( szSendBuf_toFRONT,  nLen);
			//if( nRet<=0 )
			//{
			//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "LOSSCUT���簡�����̻�!!!(%s)", szSendBuf_toFRONT );
			//	//ASSERT(nRet>0);
			//}
		}
	}

	//	SHM �� ����
	//	����ȣ���� �������� �ʴ´�.
	if (!bDongsi)
		SaveSHM( szSendBuf );

	
	// �ü��߰�. �ɼ��� �߰����� �ʴ´�.
	/*//TCPRELAY 
	if( cStk!='O'){
		if( bHoga )
			m_clntSend->SendData(szSendBuf, strlen(szSendBuf));
		else
		{
			m_clntSend->SendData(szSendBuf_toFRONT, strlen(szSendBuf_toFRONT));
			m_clntSend->SendData(szSendBuf, strlen(szSendBuf));
		}
	}
	*/
	if( cStk!='O')
	{
		PostRelayData(szSendBuf,  nLen);
	}



	return 0L;
}

#endif

VOID CDlg_FC0::SaveSHM( char* pSise )
{
	//REAL_SISE_KS* pSiseKS = (REAL_SISE_KS*)pSise;
	//REAL_HOGA_KS* pHogaKS = (REAL_HOGA_KS*)pSise;
	//
	//int		nRet			= 0;
	//char	szStkCode[MAX_GDS_CODE+MAX_STK_CODE+1] = {0,};
	//char	szMsgCode[5+1] = {0,};

	//memset( szStkCode, 0x20, MAX_GDS_CODE+MAX_STK_CODE );


	//// key value;
	//CopyMemory( szStkCode,				pSiseKS->stk_code, MAX_STK_CODE );	// STK_CODE
	//CopyMemory( szStkCode+MAX_STK_CODE,	pSiseKS->gds_code, MAX_GDS_CODE );	// GDS_CODE
	//
	//// msg code
	//CopyMemory( szMsgCode, pSiseKS->Header.Code, sizeof(pSiseKS->Header.Code) );

	////////////////////////////////////////////////////////////////////////////
	////	GROUP �� ������ GROUP INSERT
	////if ( m_shm.FindGroup( szStkCode ) == FALSE )
	////{
	////	if ( !m_shm.InsertGroup( szStkCode ) )
	////	{
	////		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM InsertGroup error (%s)\n", szStkCode );
	////		return;
	////	}
	////	
	////	if ( (strcmp(szMsgCode, CODE_REAL_SISE_KS_S)==0) ) //�ü��� ��츸
	////	{
	////		if ( !m_shm.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
	////		{
	////			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM SISE InsertData error-1 (%s)\n", szStkCode );
	////			return;
	////		}
	////	}
	////	else
	////	{
	////		// ȣ������ �ü���Ŷ���� ������Ʈ
	////		REAL_SISE_KS stSiseKS;
	////		memset( &stSiseKS, 0x20, sizeof(REAL_SISE_KS) );
	////		CopyMemory( &(stSiseKS.stk_code), szStkCode, MAX_GDS_CODE+MAX_STK_CODE );
	////		
	////		if ( !m_shm.InsertData( szStkCode, (char*)&(stSiseKS.stk_code) ) )
	////		{
	////			CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM SISE(HOGA) InsertData error (%s)\n", szStkCode );
	////			return;
	////		}
	////	}
	////	return;
	////}

	////////////////////////////////////////////////////////////////////////////
	//// ȣ�� ����
	//if ( (strcmp(szMsgCode, CODE_REAL_HOGA_KS_S)==0) ) // ||	(strcmp(szMsgCode, "82103")==0) || (strcmp(szMsgCode, "83103")==0) )
	//{
	//	//if ( m_shm.FindData( szStkCode, szStkCode ) != TRUE )
	//	//{
	//	//	CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM�� ȣ�� ������ ����-2(%s)", szStkCode);
	//	//	return;
	//	//}
	//	// SharedMemory is Sise Structure
	//	int nSellStartPos =
	//		sizeof(pSiseKS->stk_code) +
	//		sizeof(pSiseKS->gds_code) +
	//		sizeof(pSiseKS->tm) +
	//		sizeof(pSiseKS->now_prc) +
	//		sizeof(pSiseKS->open) +
	//		sizeof(pSiseKS->high) +
	//		sizeof(pSiseKS->low) +
	//		sizeof(pSiseKS->acml_cntr_qty) +
	//		sizeof(pSiseKS->acml_buy_cntr_qty) +
	//		sizeof(pSiseKS->acml_amt) +
	//		sizeof(pSiseKS->daebi_sign) +
	//		sizeof(pSiseKS->chg) +
	//		sizeof(pSiseKS->chg_rate) +
	//		sizeof(pSiseKS->cntr_qty);
	//	int nLen =
	//		sizeof(pSiseKS->o_hoga_1) +
	//		sizeof(pSiseKS->o_hoga_2) +
	//		sizeof(pSiseKS->o_hoga_3) +
	//		sizeof(pSiseKS->o_hoga_4) +
	//		sizeof(pSiseKS->o_hoga_5) +
	//		sizeof(pSiseKS->o_rmnq_1) +
	//		sizeof(pSiseKS->o_rmnq_2) +
	//		sizeof(pSiseKS->o_rmnq_3) +
	//		sizeof(pSiseKS->o_rmnq_4) +
	//		sizeof(pSiseKS->o_rmnq_5) +
	//		sizeof(pSiseKS->b_trmnq) +
	//		sizeof(pSiseKS->o_cnt_1) +
	//		sizeof(pSiseKS->o_cnt_2) +
	//		sizeof(pSiseKS->o_cnt_3) +
	//		sizeof(pSiseKS->o_cnt_4) +
	//		sizeof(pSiseKS->o_cnt_5) +
	//		sizeof(pSiseKS->o_tot_cnt);
	//	int nBuyStartPos =
	//		nSellStartPos + nLen +
	//		sizeof(pSiseKS->o_hoga_cnt_chg_1) +
	//		sizeof(pSiseKS->o_hoga_cnt_chg_2) +
	//		sizeof(pSiseKS->o_hoga_cnt_chg_3) +
	//		sizeof(pSiseKS->o_hoga_cnt_chg_4) +
	//		sizeof(pSiseKS->o_hoga_cnt_chg_5);
	//	int nTimeStartPos = sizeof(pSiseKS->stk_code) + sizeof(pSiseKS->gds_code);

	//	//m_shm.Lock();		// LOCK
	//	//if ( !m_shm.UpdateData( szStkCode, szStkCode, nSellStartPos, nLen, (char*)pHogaKS->o_hoga_1 ) )
	//	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData-1 error (%s)\n", szStkCode );

	//	//if ( !m_shm.UpdateData( szStkCode, szStkCode, nBuyStartPos, nLen-1, (char*)pHogaKS->b_hoga_1 ) )
	//	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData-2 error (%s)\n", szStkCode );

	//	//if ( !m_shm.UpdateData( szStkCode, szStkCode, nTimeStartPos, sizeof(pHogaKS->tm), (char*)pHogaKS->tm ) )
	//	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData error-3 (%s)\n", szStkCode );

	//	//m_shm.Unlock();	// UNLOCK
	//
	//}	//	if ( (strcmp(szMsgCode, CODE_REAL_HOGA_KS_S)==0) ) ȣ��


	////////////////////////////////////////////////////////////////////////////
	//// �ü�
	//if ( (strcmp(szMsgCode, CODE_REAL_SISE_KS_S)==0) )
	//{
	//	//m_shm.Lock();		// LOCK
	//	//if ( !m_shm.FindData( szStkCode, szStkCode ) )
	//	//{
	//	//	if ( !m_shm.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
	//	//		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM SISE InsertData error (%s)\n", szStkCode );
	//	//	
	//	//	return;
	//	//}

	//	//int nDataCnt = m_shm.GetCurrStructCnt( szStkCode );
	//	//if ( nDataCnt == 0 )
	//	//{
	//	//	if ( !m_shm.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
	//	//	{
	//	//		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "�ü��� SHM INSERT ERROR (%s)\n", szStkCode );
	//	//		return;
	//	//	}
	//	//}
	//	//else
	//	//{
	//	//	//int nDataCnt = m_shm.GetCurrStructCnt( szStkCode );
	//	//	//if ( nDataCnt == 0 )
	//	//	//{
	//	//	//	if ( !m_shm.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
	//	//	//		CUtil::LogEx(&m_logF,&g_logEvent, FALSE, "SHM SISE InsertData error (%s)\n", szStkCode );
	//	//	//}
	//	//	//else
	//	//	//{
	//	//		//	�ü� ������
	//	//		int nSiseStartPos = sizeof(pSiseKS->stk_code) + sizeof(pSiseKS->gds_code) + sizeof(pSiseKS->tm);
	//	//		int nSiseLen =	sizeof(pSiseKS->now_prc) +
	//	//						sizeof(pSiseKS->open) +
	//	//						sizeof(pSiseKS->high) +
	//	//						sizeof(pSiseKS->low) +
	//	//						sizeof(pSiseKS->acml_cntr_qty) +
	//	//						sizeof(pSiseKS->acml_buy_cntr_qty) +
	//	//						sizeof(pSiseKS->acml_amt) +
	//	//						sizeof(pSiseKS->daebi_sign) +
	//	//						sizeof(pSiseKS->chg) +
	//	//						sizeof(pSiseKS->chg_rate) +
	//	//						sizeof(pSiseKS->cntr_qty)+1;
	//	//		//	�ð�����,ü�ᷮ �� ����
	//	//		if ( !m_shm.UpdateData( szStkCode, szStkCode, nSiseStartPos, nSiseLen, pSiseKS->now_prc ) )
	//	//			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM SISE UpdateData error (%s)\n", szStkCode );
	//	//		else
	//	//		{
	//	//			char msg[512];
	//	//			sprintf(msg, "SISE UPD[prc:%.*s][QTY:%.*s]\n", 
	//	//						sizeof(pSiseKS->now_prc), pSiseKS->now_prc, 
	//	//						sizeof(pSiseKS->cntr_qty), pSiseKS->cntr_qty);
	//	//			OutputDebugString(msg);
	//	//		}

	//	//		//	KS200 ����
	//	//		int nHogaLen =	(sizeof(pSiseKS->o_hoga_1)	* 5)	+ 
	//	//						(sizeof(pSiseKS->o_rmnq_1)	* 6)	+	//	o_trmnq ����
	//	//						(sizeof(pSiseKS->o_cnt_1)	* 5)	+
	//	//						(sizeof(pSiseKS->o_tot_cnt)	)		+
	//	//						(sizeof(pSiseKS->o_hoga_cnt_chg_1)) * 5;
	//	//		
	//	//		nHogaLen *=2;	//	�ŵ��ż� ���
	//	//		
	//	//		nSiseStartPos = nSiseStartPos + nSiseLen + nHogaLen;

	//	//		nSiseLen =	sizeof(pSiseKS->ks200) +
	//	//					sizeof(pSiseKS->hoga_trmnq_chg) +
	//	//					sizeof(pSiseKS->qty_chg) +
	//	//					sizeof(pSiseKS->amt_chg) +
	//	//					sizeof(pSiseKS->theory_prc) +
	//	//					sizeof(pSiseKS->open_yak) +
	//	//					sizeof(pSiseKS->open_yak_chg) +
	//	//					sizeof(pSiseKS->gyuri_rate) +
	//	//					sizeof(pSiseKS->basis) ;
	//	//					
	//	//		//char* p = pSise + nSiseStartPos;

	//	//		//if ( !m_shm.UpdateData( szStkCode, szStkCode, nSiseStartPos, sizeof(pSiseKS->ks200), pSiseKS->ks200 ) )4
	//	//		if ( !m_shm.UpdateData( szStkCode, szStkCode, nSiseStartPos, nSiseLen, pSiseKS->ks200 ) )
	//	//			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM KS200 UpdateData error (%s)\n", szStkCode );

	//			//else
	//			//	CUtil::LogEx(&m_logF, FALSE, "[SHM SISE](%s)", pSiseKS->now_prc );
	//		//}
	//	}
	//}
		//m_shm.Unlock();	// UNLOCK
}

/*
	front / shm / match server ���� ������ ������
*/
// DWORD WINAPI CDlg_FC0::ConnectThread(LPVOID lp)
// {
// 	CDlg_FC0* p = (CDlg_FC0*)lp;
// 	
// // 	char msg[1024];
// // 	DWORD dwID = GetCurrentThreadId();
// // 	sprintf(msg, "[CURRENT THREAD ID:%d]", dwID);
// // 	//OutputDebugString(msg);
// // 	CUtil::LogEx(&p->m_logF, FALSE, msg);
// // 
// // 	int idx;
// // 	char target[32];
// // 	if( dwID==p->m_dwThreadID[CONN_TARGET_FRONT_F] )
// // 	{
// // 		idx = CONN_TARGET_FRONT_F;
// // 		strcpy(target, "FRONT_F_THREAD");
// // 	}
// // 	else if( dwID==p->m_dwThreadID[CONN_TARGET_FRONT_O] )
// // 	{
// // 		idx = CONN_TARGET_FRONT_O;
// // 		strcpy(target, "FRONT_O_THREAD");
// // 	}
// // 	else if( dwID==p->m_dwThreadID[CONN_TARGET_SHM] )
// // 	{
// // 		idx = CONN_TARGET_SHM;
// // 		strcpy(target, "SHM_THREAD");
// // 	}
// // // 	else if( dwID==p->m_dwThreadID[CONN_TARGET_MT] )
// // // 	{
// // // 		idx = CONN_TARGET_MT;
// // // 		strcpy(target, "MT_THREAD");
// // // 	}
// // 
// // 	sprintf(msg, "[%s] START.....\n", target); OutputDebugString(msg);
// // 	CUtil::LogEx(&p->m_logF, FALSE, msg);
// // 
// // 	//	���� ���� ����
// // 	if( !p->SockInit() ){
// // 		PostThreadMessage(GetCurrentThreadId(), MSG_RE_CONNECT, (WPARAM)0, (LPARAM)0 );
// // 		sprintf(msg, "[%s] �ʱ� connect ���з� PostThreadMessage", target);
// // 		CUtil::LogEx(&p->m_logF, &g_logEvent, FALSE, msg);
// // 		OutputDebugString(msg);OutputDebugString("\n");
// // 	}
// // 
// // 	BOOL bConn = FALSE;
// // 	MSG wMsg;
// // 	while(1)
// // 	{
// // 		if( WaitForSingleObject(p->m_hDie[idx],1)==WAIT_OBJECT_0 ){
// // 			break;
// // 		}
// // 
// // 		while(PeekMessage(&wMsg, NULL, 0,0, PM_REMOVE))
// // 		{
// // 			if( wMsg.message!=MSG_RE_CONNECT )
// // 					break;
// // 
// // 			while( !p->SockInit() )
// // 			{
// // 				sprintf(msg,"[%s] SOCKET INIT ���з� retry ",target);
// // 				CUtil::LogEx(&p->m_logF, &g_logEvent, FALSE, msg);
// // 				OutputDebugString(msg); OutputDebugString("\n");
// // 
// // 				if( WaitForSingleObject(p->m_hDie[idx],1000)==WAIT_OBJECT_0 )
// // 					return 0;
// // 			}
// // 			SYSTEMTIME st; GetLocalTime(&st);
// // 			sprintf(msg,"[%02d:%02d:%02d.%03d][%s] INIT thread ���� INIT �õ� �� ���� "
// // 							,st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,target);
// // 			CUtil::LogEx(&p->m_logF, &g_logEvent, FALSE, msg);
// // 			OutputDebugString(msg); OutputDebugString("\n");
// // 
// // 		}
// // 	}
// 	return 0;
// }
// 

// 
// DWORD WINAPI CDlg_FC0::SendThread_SHM(LPVOID lp)
// {
// 	CDlg_FC0* p = (CDlg_FC0*)lp;
// 	
// 	char msg[1024];
// 	DWORD dwID = GetCurrentThreadId();
// 	sprintf(msg, "[CURRENT THREAD ID:%d]", dwID);
// 	OutputDebugString(msg);
// 	
// 	int idx;
// 	char target[32];
// 	if( dwID==p->m_dwThreadID[CONN_TARGET_FRONT] )
// 	{
// 		idx = CONN_TARGET_FRONT;
// 		strcpy(target, "FRONT_THREAD");
// 	}
// 	else if( dwID==p->m_dwThreadID[CONN_TARGET_SHM] )
// 	{
// 		idx = CONN_TARGET_SHM;
// 		strcpy(target, "SHM_THREAD");
// 	}
// 	else if( dwID==p->m_dwThreadID[CONN_TARGET_MT] )
// 	{
// 		idx = CONN_TARGET_MT;
// 		strcpy(target, "MT_THREAD");
// 	}
// 	
// 	sprintf(msg, "[%s] START.....\n", target); OutputDebugString(msg);
// 	
// 	char szSendBuf[MAX_SENDSIZE];
// 	int nLen;
// 	BOOL bConn = FALSE;
// 	while(1)
// 	{
// 		if( WaitForSingleObject(p->m_hDie[idx],1)==WAIT_OBJECT_0 )
// 			break;
// 		
// 		//	������ �ȵ� �����̸� ��� ���� �õ�
// 		if(!bConn)
// 		{
// 			p->SockClose(idx);
// 			if( p->SockInit(idx) ){
// 				sprintf(msg, "[%s] CONNECT SUCCESS.....\n", target); OutputDebugString(msg);
// 				bConn = TRUE;
// 			}
// 			else{
// 				bConn = FALSE;
// 				sprintf(msg, "[%s] CONNECT FAIL RETRY.....\n", target); OutputDebugString(msg);
// 				Sleep(1000);
// 				continue;	
// 			}
// 		}
// 		
// 		if( p->m_lstData[idx].empty() )
// 			continue;
// 		
// 		memset( szSendBuf, 0x00, sizeof(szSendBuf) );
// 		
// 		EnterCriticalSection(&p->m_cs[idx]);
// 		strcpy( szSendBuf, (LPCSTR)(*p->m_lstData[idx].begin()) );
// 		p->m_lstData[idx].pop_front();
// 		LeaveCriticalSection(&p->m_cs[idx]);
// 		nLen = strlen(szSendBuf);
// 		if( !p->m_pSock[idx]->Send( szSendBuf, nLen) )
// 		{
// 			sprintf(msg, "[%s] SEND FAIL.....\n", target); OutputDebugString(msg);
// 			Sleep(1000);
// 			continue;
// 		}
// 		sprintf(msg, ".........................[%s SEND_COUNT:%d]\n",target, p->m_lstData[idx].size());
// 		OutputDebugString(msg);
// 	}
// 	
// 	p->SockClose(idx);
// 	return 0;
// }

void CDlg_FC0::OnButtonUnadvise() 
{
	// TODO: Add your control notification handler code here
	//	���� �ü�/ȣ�� ����
	list<CString>::iterator it;
	for( it=m_sStkList_KSF.begin(); it!=m_sStkList_KSF.end(); it++ )
	{
		CString sStk = *it;
		UnadviseData('Y'/*�ü�*/, sStk, GDS_CODE_KSF);		
		UnadviseData('N'/*ȣ��*/, sStk, GDS_CODE_KSF);
	}
	
	//	�ɼ� �ü�/ȣ�� ����
	for( it=m_sStkList_KSO.begin(); it!=m_sStkList_KSO.end(); it++ )
	{
		CString sStk = *it;
		UnadviseData('Y'/*�ü�*/, sStk, GDS_CODE_KSO);		
		UnadviseData('N'/*ȣ��*/, sStk, GDS_CODE_KSO);
	}
}

DWORD WINAPI CDlg_FC0::ProcDummySiseTest(LPVOID lp) 
{
	CDlg_FC0 *pT = (CDlg_FC0*)lp;

	char	szBuff[MAX_BUFSIZE] = {0,};
	char	szSendBuf[MAX_BUFSIZE] = {0,};
	char szSendBuf_toFRONT[MAX_SENDSIZE] = {0,};
//	char	*pFind;//, *pData;
	char	szMsg[512] = {0,};
	char	msg[4096]={0,};
	char cStk;

	INT MAX_CNT = 20000;
	INT MAX_SIZE = 300;
	int nSize = MAX_CNT * MAX_SIZE;
	char	*data = new char[nSize];

	int nCnt = 0;
	BOOL bFind = FALSE;

	
	FILE *	m_pFile;
	m_pFile = fopen( "ETK_SISE.log", "r" );
	if(!m_pFile)	return 0;

	char time[6];
	int i = 0;
	while( fgets(szBuff, sizeof(szBuff), m_pFile) )
	{
		Sleep(0);
		if ( strstr(szBuff, "�ü� ��û") ||  strstr(szBuff, "ȣ�� ��û") || strstr(szBuff, "����"))
			continue;
 		//if(  strstr(szBuff, "101F6000")==0)
   		//	continue;

		sprintf(time, "%.5s", szBuff+1);
		if( strcmp(time, "09:01")<0 )
			continue;

		strcpy( &data[ i++ * MAX_SIZE ], szBuff );
		
		if( i==MAX_CNT )
			break;
	}
	fclose(m_pFile);

	int nIdx = 0;
	while(1)
	{
		if( ++nIdx==MAX_CNT )
		{
			nIdx=0;
		}

		strcpy( szBuff, data+(nIdx*MAX_SIZE) );

		//////////////////////////////////////////////////////////////////////////
		//	����ü��(�ü�)
		if( strstr(szBuff, "F_SISE" ) > 0 )
		{
			cStk = 'F';
			FC0_OutBlock* p = (FC0_OutBlock*)(&szBuff[23]);
			//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
			CCVT_ETK cvt;
			//�ü��̻���. cvt.de_KS_F_SiseEx( (char*)p, szSendBuf, szSendBuf_toFRONT );
			double dErrSise = 0;
			if( !cvt.de_KS_F_SiseEx( (char*)p, szSendBuf, szSendBuf_toFRONT, &dErrSise ) )
			{
				
				return 0L;
			}

//			OutputDebugString(szSendBuf_toFRONT);OutputDebugString("\n");
// 			REAL_SISE_KS* pack = (REAL_SISE_KS*)szSendBuf;
// 			
// 			if( strncmp(pack->tm, "09",2)<0 )	continue;
		}
		
		///////////////////////////////////////////////////////////////////////////
		//	����ȣ��
		else if( strstr(szBuff, "F_HOGA" ) > 0 )
		{
			cStk = 'F';
			FH0_OutBlock* p = (FH0_OutBlock*)(&szBuff[23]);
			
			//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
			CCVT_ETK cvt;
			cvt.de_KS_F_Hoga( (char*)p, szSendBuf );
			strcpy(szSendBuf_toFRONT, szSendBuf);
// 			REAL_SISE_KS* pack = (REAL_SISE_KS*)szSendBuf;
// 			if( strncmp(pack->tm, "09",2)<0 )	continue;
		}
		
		//////////////////////////////////////////////////////////////////////////
		//	�ɼ� ü��(�ü�)
		else if( strstr(szBuff, "O_SISE" ) > 0 )
		{
			cStk = 'O';
			//if( strstr(szBuff, "201E9232")==0 && strstr(szBuff, "301E9232")==0 )
		//		continue;
			OC0_OutBlock* p = (OC0_OutBlock*)(&szBuff[23]);

			//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
			CCVT_ETK cvt;
			cvt.de_KS_O_Sise( (char*)p, szSendBuf );
 			REAL_SISE_KS* pack = (REAL_SISE_KS*)szSendBuf;
			//if( pack->stk_code[4]=='1' )
			//	pack->stk_code[4]='1';
		}
		///////////////////////////////////////////////////////////////////////////
		//	�ɼ� ȣ��
		else if( strstr(szBuff, "O_HOGA" ) > 0 )
		{
			cStk = 'O';
			//if( strstr(szBuff, "201E9232")==0 && strstr(szBuff, "301E9232")==0 )
			//	continue;
			OH0_OutBlock* p = (OH0_OutBlock*)(&szBuff[23]);

			//	������Ŷ���� ���� �� ��Ƽĳ��Ʈ ����
			CCVT_ETK cvt;
			cvt.de_KS_O_Hoga( (char*)p, szSendBuf );
 			REAL_HOGA_KS* pSend = (REAL_HOGA_KS*)szSendBuf;
			//if( pSend->stk_code[4]=='1' )
			//	pack->stk_code[4]='1';
		}
		else
			continue;
		
		//	Ȥ�� futures, option �� �������� ����Ͽ� if �Ѵ�.
		int nErrCode = 0;
		if( cStk=='F' || cStk=='O' )
		{
			//	FRONT ���� ����
// 			if( pT->m_sockFront[0] )
// 			{
// 				if( pT->m_pSock[0]->IsConnected() )
// 				{
// 					if( pT->m_pSock[0]->SendData(szSendBuf_toFRONT, strlen(szSendBuf_toFRONT), 100,&nErrCode)==SOCKET_ERROR )
// 					{
// 						SYSTEMTIME st;
// 						GetLocalTime(&st);
// 						sprintf(msg, "[%02d:%02d:%02d.%03d][FRONT] SEND FAIL.....(%s)\n",
// 							st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
// 							pT->m_pSock[0]->GetSockErr()); //OutputDebugString(msg);
// 						CUtil::LogEx(&pT->m_logF, &g_logEvent, FALSE, msg);
// 						OutputDebugString(msg);
// 						pT->SockClose(0);
// 						//m_pSock[0]->Disconnect();
// 						PostThreadMessage( pT->m_dwThreadID[0], MSG_RE_CONNECT, (WPARAM)0, (LPARAM)0 );
// 					}
// 					//m_pSock[0]->SendData(szSendBuf, strlen(szSendBuf), 50,&nErrCode);
// 				}
//			}

			//pT->m_sockFront->Send_Data(szSendBuf_toFRONT, strlen(szSendBuf_toFRONT) );
			////lable:LOSSCUT_SISE
			//pT->m_sockLosscut->Send_Data(szSendBuf_toFRONT, strlen(szSendBuf_toFRONT) );
		}
		
		//	SHM ���� ����
		/*
		if(m_pSock[CONN_TARGET_SHM])
		{
			if( m_pSock[CONN_TARGET_SHM]->IsConnected() )
			{
				if( m_pSock[CONN_TARGET_SHM]->SendData(szSendBuf, strlen(szSendBuf), 100, &nErrCode)==SOCKET_ERROR )
				{
					SYSTEMTIME st;
					GetLocalTime(&st);
					sprintf(msg, "[%02d:%02d:%02d.%03d][SHM] SEND FAIL.....(%s)\n",
						st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
						m_pSock[CONN_TARGET_SHM]->GetSockErr()); //OutputDebugString(msg);
					
					CUtil::LogEx(&m_logF, &g_logEvent, FALSE, msg);
					OutputDebugString(msg);
					SockClose(CONN_TARGET_SHM);
					//m_pSock[CONN_TARGET_SHM]->Disconnect();
					PostThreadMessage( m_dwThreadID[CONN_TARGET_SHM], MSG_RE_CONNECT, (WPARAM)0, (LPARAM)0 );
				}
				//m_pSock[CONN_TARGET_SHM]->SendData(szSendBuf, strlen(szSendBuf), 100, &nErrCode);
			}
		}
		*/
		
		//	SHM �� ����
		pT->SaveSHM( szSendBuf );
		
		Sleep(100);
	}

	delete[] data;

}



void CDlg_FC0::OnButtonTest_1() 
{
	static BOOL OnOffDummySise = TRUE;
	//------------------------------
	//	������ ����
	//------------------------------
	DWORD dw;
	if(NULL == m_hThdDummySise)
	{
		m_hThdDummySise = CreateThread( NULL, 0, ProcDummySiseTest, this, CREATE_SUSPENDED, &dw );		
		if( !m_hThdDummySise )
			return;
	}
	if(NULL != m_hThdDummySise)
	{
		if(OnOffDummySise == TRUE)
		{
			ResumeThread( m_hThdDummySise );
			SetDlgItemText(IDC_BUTTON_TEST, "FILE TEST Stop");
		}
		else
		{
			SuspendThread( m_hThdDummySise );
			SetDlgItemText(IDC_BUTTON_TEST, "FILE TEST Start");
		}
		OnOffDummySise = !OnOffDummySise;
	}
}



//TCPRELAY
BOOL CDlg_FC0::Relay_Begin()
{
	/////////////////////////////////////////////////////////////
	//	SOCKET INIT
	//m_sockListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	//if(m_sockListen == INVALID_SOCKET)
	//{
	//	shutdown(m_sockListen,SD_SEND);
	//	closesocket(m_sockListen);
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay ACCEPT SOCKET CRETAE Error.ErrCode:%d", GetLastError() );
	//	return FALSE;
	//}
	//
	//INT nTcpPort;
	//char szTemp[128];
	//CUtil::GetCnfgFile("api_if", "ETK_KS", "LISTEN_PORT", szTemp);
	//nTcpPort = atoi(szTemp);

	//////	local address
	//m_localsin.sin_family      = AF_INET;
 //   m_localsin.sin_port        = htons(nTcpPort);
 //   m_localsin.sin_addr.s_addr = inet_addr(m_szLocalIP);
	//
	//BOOL opt = TRUE;
	//int optlen = sizeof(opt);
	////	setsockopt(m_sockListen,SOL_SOCKET,SO_REUSEADDR,(const char far *)&opt,optlen);
	//
	//if (::bind(m_sockListen, (LPSOCKADDR) &m_localsin, sizeof(m_localsin)) == SOCKET_ERROR)
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay bind Error.ErrCode:%d", GetLastError() );
	//	return FALSE;
	//}
	//CUtil::LogEx( 'I', &m_logF, NULL, FALSE, "relay BIND OK : %s , %d", m_szLocalIP, nTcpPort);
	//
	//if(listen(m_sockListen, SOMAXCONN) == SOCKET_ERROR)
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay listen Error.ErrCode:%d", GetLastError() );
	//	return FALSE;
	//}
	//
	//m_wsaAccept = WSACreateEvent();
	//if(WSAEventSelect(m_sockListen, m_wsaAccept, FD_ACCEPT))
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay listen socket WSAEventSelect Error.ErrCode:%d", GetLastError() );
	//	return FALSE;
	//}

	////	OVERLAPPED �� ���� event
	//m_wsaSend = WSACreateEvent();	

	//InitializeCriticalSection(&m_csRelaySock);
	//InitializeCriticalSection(&m_csRelayData);

	//// CREATE THREAD
	//if( (m_hRelayThread = CreateThread(NULL,0,Relay_SendDThread,this,0,&m_dwRelayThread)) == NULL )
	////if( (m_hRelayThread = CreateThread(NULL,0,Relay_SendDThread,this,CREATE_SUSPENDED,&m_dwRelayThread)) == NULL )
	//{
	//	CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "RELAY THREAD ���� ����.ErrCode:%d", GetLastError() );
	//	return FALSE;
	//}	


	//m_bRelayContinue = TRUE;
	//
	//CUtil::LogEx( 'I', &m_logF, NULL, FALSE, "TCP RELAY �ʱ�ȭ ����" );

	return TRUE;
}

//TCPRELAY
VOID CDlg_FC0::Relay_End()
{
	m_bRelayContinue = FALSE;

	if( WaitForSingleObject(m_hRelayThread, 3000) != WAIT_OBJECT_0 )
		TerminateThread(m_hRelayThread,0);
	
	//SAFE_CLOSEHANDLE(m_hRelayThread);
	//SAFE_CLOSEHANDLE(m_wsaAccept);
	//SAFE_CLOSEHANDLE(m_wsaSend);
	//SAFE_CLOSESOCKET(m_sockListen);

	DeleteCriticalSection(&m_csRelaySock);
	DeleteCriticalSection(&m_csRelayData);

}


//TCPRELAY
DWORD WINAPI  CDlg_FC0::Relay_SendDThread(LPVOID lp)
{
	CDlg_FC0* p = (CDlg_FC0*)lp;
//	MSG msg;
//	char*	pData;
//	char	szData[MAX_SENDSIZE];
//	int		nLen;
	SOCKADDR_IN			sinClient;
	int	sinSize = sizeof(sinClient);
	//while(p->m_bRelayContinue)
	//{
	//	DWORD dw = WSAWaitForMultipleEvents(1, &p->m_wsaAccept, TRUE, 1, FALSE);
	//	if( dw== WSA_WAIT_EVENT_0)
	//	{
	//		WSAResetEvent(p->m_wsaAccept);
	//		
	//		SOCKET client = accept(p->m_sockListen, (LPSOCKADDR)&sinClient, &sinSize);
	//		if( client == INVALID_SOCKET)
	//		{
	//			CUtil::LogEx( 'E', &p->m_logF, NULL, FALSE, "RELAY accept error(%d)", WSAGetLastError());
	//		}
	//		else
	//		{
	//			
	//			//	client public ip ����
	//			SOCKADDR_IN peer_addr;
	//			int			peer_addr_len = sizeof(peer_addr);

	//			if (getpeername( client, (sockaddr*)&peer_addr, &peer_addr_len) == 0 )
	//			{
	//				char* pIP	= inet_ntoa(peer_addr.sin_addr);
	//				CUtil::LogEx( 'I', &p->m_logF, NULL, FALSE, "accept[%s]", pIP);
	//			}
	//			EnterCriticalSection(&p->m_csRelaySock);
	//			p->m_setRelaySock.insert(client);
	//			LeaveCriticalSection(&p->m_csRelaySock);
	//			
	//		}
	//	}
	//	
	//	/*
	//	while( PeekMessage(&msg, NULL, 0,0, PM_REMOVE) )
	//	{
	//		switch(msg.message)
	//		{
	//		case WM_SISE_DATA:
	//			
	//			nLen  = (int) msg.wParam;
	//			pData = (char*) msg.lParam;
	//			//sprintf(szData, "%.*s", nLen, pData );
	//			sprintf(szData, "%.10s", pData );
	//			delete[] pData;
	//			//p->m_lstMsg.InsertString(0, szData);
	//			//CUtil::LogEx( 'I', &p->m_logF, NULL, FALSE, "SEND[%s]", szData);
	//			p->SendRelay(szData, nLen);
	//
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//	*/
	//	EnterCriticalSection(&p->m_csRelayData);
	//	if( !p->m_lstRelayData.empty() ){
	//		CString data = *p->m_lstRelayData.begin();
	//		p->m_lstRelayData.pop_front();
	//		p->SendRelay((LPSTR)data.GetBuffer(), data.GetLength());
	//	}
	//	LeaveCriticalSection(&p->m_csRelayData);
	//}
	return 0;
}

//TCPRELAY

VOID CDlg_FC0::PostRelayData(LPSTR lpData, INT nLen)
{
	/*char* pData = new char[nLen];
	memcpy( pData, lpData, nLen );
	PostThreadMessage( m_dwRelayThread, WM_SISE_DATA, (WPARAM)nLen,	(LPARAM)pData );*/
	EnterCriticalSection(&m_csRelayData);
	m_lstRelayData.push_back(lpData);
	LeaveCriticalSection(&m_csRelayData);
}

// TCPRELAY
VOID CDlg_FC0::SendRelay(LPSTR lpData, INT nLen)
{
	set<SOCKET>::iterator itSet;

	EnterCriticalSection(&m_csRelaySock);
	

	try{
		
		for( itSet=m_setRelaySock.begin(); itSet!=m_setRelaySock.end();)
		{
			if(!m_bRelayContinue)	break;

			
			SOCKET sockClient = INVALID_SOCKET;
			sockClient = *itSet;

			if( sockClient != INVALID_SOCKET )
			{
				RequestSendIO(sockClient, lpData, nLen);
			}
			
			if( ++itSet==m_setRelaySock.end() )
				break;
			
		}	//	if( !pStkSockSet->setSock.empty() )
		LeaveCriticalSection(&m_csRelaySock);
	}
	catch(...)
	{
		LeaveCriticalSection(&m_csRelaySock);
		//CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "RELAY SEND EXCEPTION-2");
	}
}


//TCPRELAY
BOOL CDlg_FC0::RequestSendIO( SOCKET sock, char* pSendBuf, int nSendLen )
{
	BOOL			bSuccess = FALSE;
	WSAOVERLAPPED	overlapped;
	WSABUF			wsaBuf;
	DWORD			dwFlags		= 0;
	DWORD			dwSendBytes	= 0;
	int				nErr		= 0;
	
	memset( &overlapped, 0, sizeof( overlapped ) );
	overlapped.hEvent = m_wsaSend;
	wsaBuf.len = nSendLen;
	wsaBuf.buf = pSendBuf;
	
	
	if( WSASend( sock, &wsaBuf, 1, &dwSendBytes, 0, &overlapped, NULL ) == SOCKET_ERROR )
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
			// 			CUtil::PrintLogEx(&g_Log, NULL, "[SEND REAL]WSARecv ERROR");
			// 			CUtil::PrintErr(&g_Log, NULL, WSAGetLastError() );
			return FALSE;
		}
	}
	
	
	return TRUE;
}
