// Dlg_FC0.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "Dlg_FC0_1.h"

#include "../../include/API_ETK/FC0.h"
#include "../../include/API_ETK/FH0.h"
#include "../../include/API_ETK/OC0.h"
#include "../../include/API_ETK/OH0.h"

//CME
#include "../../include/API_ETK/NC0.h"
#include "../../include/API_ETK/NH0.h"

//해외선물
#include "../../include/API_ETK/OVH.h"
#include "../../include/API_ETK/OVC.h"

#include "../../libsrc/CVT_ETK.h"

#include <assert.h>

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

	m_sockFront		= NULL;
	m_sockLosscut	= NULL;
	m_dwThreadID = 0;
	m_hDie			= NULL;
	m_hThread		= NULL;
	//InitializeCriticalSection(&m_cs);

	m_hThdDummySise	= NULL;	// skeo 2011-01-17

	//SMILOR.2스레드
	InitializeCriticalSection(&m_csSise);
	m_hSiseThr = NULL;

	// 시세중계
	//m_clntSend = NULL;
	//m_clntSend_KS = NULL;

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
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest_1)
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

	//-----------------------------------------------------------------------------------------
	//	LOG FIL OPEN
	//-----------------------------------------------------------------------------------------
	
	char szMsg[512];
	if( !CUtil::GetLogDirCnfg(szPath, szMsg) )
	{
		return FALSE;
	}
	
	m_logF.OpenLog(szPath, "ETK_F_X_v3.0");
	//m_logInner.OpenLog(szPath, "ETK_INNER_SISE");

	CUtil::LogEx('I', &m_logF, NULL, FALSE, "ETK_F_X_TCPRELAY");

	//////////////////////////////////////////////////////////////////////////
	//	SISE SHM OPEN
	char szSHMName[128], szMutexName[128];
	if( !CUtil::GetCnfgFile("common", "SHM", "SHM_SISE_FU", szSHMName) ||
		!CUtil::GetCnfgFile("common", "SHM", "MUTEX_SISE_FU", szMutexName) )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "FU SHM/MUTEX NAME 가져오기 에러 [SHM:%s][MUTEX:%s]", szSHMName, szMutexName );
		return FALSE;
	}
	if ( !m_shm.Open( szSHMName, szMutexName ) )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM_NAME OPEN 에러(%s)", szSHMName);
		return FALSE;
	}
	CUtil::LogEx('I', &m_logF, NULL, FALSE, "CSHMUpdate_FU : SHM OPEN 성공(%s)(%s)", szSHMName, szMutexName);


	//////////////////////////////////////////////////////////////////////////
	//	DB OPEN 
	if( !CUtil::DBOpen(&m_db) )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "DB OPEN ERROR(%s)", m_db.GetLastError() );
		m_db.free();
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//	UDP SOCKET INIT
	if( !SockInit() ){
		m_db.free();
		return FALSE;
	}

	// 시세중계
	/*BOOL bRet;
	m_clntSend = new CSiseClntSend_FU();
	bRet = m_clntSend->Init();
	CUtil::LogEx('I', &m_logF, &g_logEvent, FALSE, "%s", m_clntSend->GetMsg() );
	if( !bRet )	return FALSE;*/

	//TCPRELAY
	if( !Relay_Begin() )
		return FALSE;




	//////////////////////////////////////////////////////////////////////////
	//	종목정보 가져오기
	if( !GetStkCodeList_FU() ){
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "선물 종목 가져오기 실패");
		m_db.free();
		return	FALSE;
	}

#ifdef _DEF_INTEGRATE
	GetStkCodeList_KS();
#endif

	//	현재가 저장한다.
#if 0
	list<CString>::iterator it;
	for( it = m_sStkList.begin(); it!=m_sStkList.end(); it++ )
	{
		CString sStkCode = *it;
		if( !GetNowPrc(sStkCode) )
			return FALSE;
	}
#endif

	//SMILOR.2스레드. 시세수신과 처리 분리
#if 0	// THREAD 오류로 막아놓음
	DWORD id;
	if( (m_hSiseThr = CreateThread(NULL,0,SiseThread,this,0,&id)) == NULL )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "시세 처리 THREAD 생성 실패");
		return FALSE;
	}	
#endif

	OnButtonRequest();

	//prop.SetBaseKey(HKEY_LOCAL_MACHINE, REG_ROOT);

	sprintf( szMsg, "해외선물버전_TCPRELAY_v3.0(%s)", __DATE__ );
	CUtil::LogEx('I', &m_logF, NULL, FALSE, szMsg);
	m_lstMsg.InsertString(0, szMsg);

	//prop_hoga.SetBaseKey(HKEY_LOCAL_MACHINE, REG_ROOT);

#ifdef _DEF_INTEGRATE	// 국내, 해외 통합
	InitSHM_KS();

	//m_clntSend_KS = new CSiseClntSend_KS();
	//bRet = m_clntSend_KS->Init();
	//CUtil::LogEx('I', &m_logF, &g_logEvent, FALSE, "%s", m_clntSend_KS->GetMsg() );
	//if( !bRet )	return FALSE;
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// 국내 SHM INIT
BOOL CDlg_FC0_1::InitSHM_KS()
{
	char szSHMName[128], szMutexName[128];
	if( !CUtil::GetCnfgFile("common", "SHM", "SHM_SISE_KS", szSHMName) ||
		!CUtil::GetCnfgFile("common", "SHM", "MUTEX_SISE_KS", szMutexName) )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "KS SHM/MUTEX NAME 가져오기 에러 [SHM:%s][MUTEX:%s]", szSHMName, szMutexName );
		return FALSE;
	}
	if ( !m_shmKS.Open( szSHMName, szMutexName ) )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM_NAME OPEN 에러(%s)", szSHMName);
		return FALSE;
	}
	CUtil::LogEx('I', &m_logF, NULL, FALSE, "CSHMUpdate_KS : SHM OPEN 성공(%s)(%s)", szSHMName, szMutexName);

	return TRUE;
}

//	현재가를 가져와서 저장한다.
BOOL CDlg_FC0_1::GetNowPrc( CString i_sStkCode )
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

//	DB 에서 해외선물 종목리스트를 가져온다.
BOOL CDlg_FC0_1::GetStkCodeList_FU()
{
	if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_FU_TRADABLE" ) )
 			return FALSE;
	if( !m_db.Exec() )
		return FALSE;
	
	char szStkList[1024]	= {0,};
	char szStk[128]			= {0,};
	int nRs = 0;
	while( m_db. nextRow() )
	{
		strcpy( szStk, m_db.Get("GLID_CODE") );

		CUtil::TrimAll( szStk, strlen(szStk) );
		
		if( strlen(szStk)==0 )
			break;
		
		CString sStk = szStk;
		m_sStkList.push_back(sStk);
		CUtil::LogEx('I', &m_logF, NULL, FALSE, "[HT_P_GET_STK_INFO_FU_TRADABLE](%s)", szStk);
	}
	m_db.Close();
	
	return TRUE;
}

BOOL CDlg_FC0_1::GetStkCodeList_KS()
{
	if( !m_db.Cmd("EXEC HT_P_GET_STK_INFO_KS_TRADABLE" ) )
 			return FALSE;
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
		if(szStk[0]=='1')
			m_sStkList.push_back(sStk);
		else
			m_sStkList.push_back(sStk);
	}
	m_db.Close();
	
	return TRUE;
}

VOID CDlg_FC0_1::SockClose()
{
 	//EnterCriticalSection(&m_cs);
 	//m_lstData.clear();
 	//LeaveCriticalSection(&m_cs);

	//char msg[128];
	//sprintf(msg, "[FRONT_F_CLEAR:%d]\n", m_lstData.size());

	SAFE_DELETE(m_sockFront);
	SAFE_DELETE(m_sockLosscut);
}

/*********************************
	mcast socket init
*********************************/
BOOL CDlg_FC0_1::SockInit()
{
	//SockClose(i_nTarget);
	char szTemp[512] = {0,};

	CUtil::GetCnfgFile( "common", "IP", "PUBLIC_IP", m_szLocalIP );
	
	CUtil::GetCnfgFile("common", "D_REAL_X.exe", "UDPPORT_OVERSEA", szTemp);
	m_nUdpPortReal = atoi(szTemp);
	
	CUtil::GetCnfgFile("common", "D_LC_X.exe", "UDPPORT_OVERSEA", szTemp);
	m_nUdpPortLC = atoi(szTemp);
 
	///////////////////////////////////////////////////////////////////////////
	//	UDP SOCKET 생성
	if(!m_sockFront)	m_sockFront = new CUdpSend;
	if( !m_sockFront->Init_Sock(m_szLocalIP, m_nUdpPortReal) )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE,"UDP SOCKET INIT ERROR");
		return FALSE;
	}
		
	CUtil::LogEx('I', &m_logF, NULL, FALSE, "[FRONT UDP SOCKET INIT OK](%s)(%d)", m_szLocalIP, m_nUdpPortReal);
	
	//lable:LOSSCUT_SISE
	if(!m_sockLosscut)	m_sockLosscut = new CUdpSend;
	if( !m_sockLosscut->Init_Sock(m_szLocalIP, m_nUdpPortLC) )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE,"UDP SOCKET INIT ERROR");
		return FALSE;
	}

	CUtil::LogEx('I', &m_logF, NULL, FALSE, "[LOSSCUT UDP SOCKET INIT OK](%s)(%d)", m_szLocalIP, m_nUdpPortLC);

	return TRUE;
}

void CDlg_FC0_1::OnDestroy()
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
	//DeleteCriticalSection(&m_cs);
	DeleteCriticalSection(&m_csSise);

	m_db.Free();
	
	m_shm.Close();
	m_shmKS.Close();

	// skeo 2011-01-17 [[
	if(WaitForSingleObject(m_hThdDummySise, 1000) != WAIT_OBJECT_0)
		TerminateThread(m_hThdDummySise, 0);	
	if(m_hThdDummySise)	CloseHandle(m_hThdDummySise);	
	m_hThdDummySise	= NULL;
	// ]]

	//TCPRELAY
	Relay_End();
	//SAFE_DELETE(m_clntSend);
	//SAFE_DELETE(m_clntSend_KS);

	CDialog::OnDestroy();
}

/***************************************
	시세 요청
***************************************/
void CDlg_FC0_1::OnButtonRequest() 
{
	//	선물 시세/호가 요청
	list<CString>::iterator it;
	for( it=m_sStkList.begin(); it!=m_sStkList.end(); it++ )
	{
		CString sStk = *it;
		//UnadviseData('Y'/*시세*/, sStk, GDS_CODE_KSF);
		AdviseData('Y'/*시세*/, sStk);

		//UnadviseData('N'/*호가*/, sStk, GDS_CODE_KSF);
		AdviseData('N'/*호가*/, sStk);

		//break;
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

	SetDlgItemText(IDC_BUTTON_TEST, "FILE TEST Start");	// skeo 2011-01-17
}

//--------------------------------------------------------------------------------------
// 데이터 Advise
//--------------------------------------------------------------------------------------
void CDlg_FC0_1::AdviseData(char i_cSiseYN, CString sStk)
{
	TCHAR	szTrCode[32];
	int		nSize;

	OVC_InBlock inOVC;
	OVH_InBlock inOVH;

	if( i_cSiseYN=='Y' )
	{
		strcpy( szTrCode, ETK_REAL_SISE_FU);
		nSize = sizeof( inOVC.symbol );
	}
	else
	{
		strcpy( szTrCode, ETK_REAL_HOGA_FU);
		nSize = sizeof( inOVH.symbol );
	}

	CString stk;
	stk.Format("%-*.*s", nSize, nSize, (LPCSTR)sStk);
	//-----------------------------------------------------------
	// 데이터 전송
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// 데이터를 받을 윈도우, XM_RECEIVE_REAL_DATA 으로 온다.
		szTrCode,					// TR 번호
		stk,						// 종목코드
		nSize						// 종목코드 길이
		);

	//-----------------------------------------------------------
	// 에러체크
	char msg[512];
	if( bSuccess == FALSE )
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s]시세 요청 실패", (LPCSTR)stk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s]호가 요청 실패", (LPCSTR)stk );
	}
	else
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s]시세 요청 성공", (LPCSTR)stk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s]호가 요청 성공", (LPCSTR)stk );
	}
	CUtil::LogEx('I', &m_logF, NULL, FALSE, msg );
	m_lstMsg.InsertString(0, msg);

	
}

//--------------------------------------------------------------------------------------
// 데이터 Unadvise
//--------------------------------------------------------------------------------------
void CDlg_FC0_1::UnadviseData(char i_cSiseYN, CString sStk)
{
	TCHAR	szTrCode[32];
	int		nSize;

	if( i_cSiseYN=='Y' )
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
												sStk,						// 종목코드
												nSize						// 종목코드 길이
												);
	
	//-----------------------------------------------------------
	// 에러체크
	char msg[512];
	if( bSuccess == FALSE )
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s]시세 중지 요청 실패", (LPCSTR)sStk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s]호가 중지 요청 실패", (LPCSTR)sStk );
	}
	else
	{
		if(i_cSiseYN=='Y')	sprintf(msg,  "[%s]시세 중지 요청 성공", (LPCSTR)sStk );
		if(i_cSiseYN=='N')	sprintf(msg,  "[%s]호가 중지 요청 성공", (LPCSTR)sStk );
	}
	CUtil::LogEx('I', &m_logF, NULL, FALSE, msg );
	m_lstMsg.InsertString(0, msg);
}


//--------------------------------------------------------------------------------------
// REAL 데이터를 받음
//--------------------------------------------------------------------------------------
LRESULT CDlg_FC0_1::OnXMReceiveRealData( WPARAM wParam, LPARAM lParam )
{
	char msg[4096] = {0,};
	char szSendBuf[MAX_SENDSIZE] = {0,};
	//char szSendBuf_toFRONT[MAX_SENDSIZE] = {0,};
	char cStk;
	INT nLen;
	BOOL bHoga = FALSE;	
	BOOL bFind = FALSE;
	BOOL bDongsi = FALSE;
	
	LPRECV_REAL_PACKET pRealPacket = (LPRECV_REAL_PACKET)lParam;

#ifdef _DEF_INTEGRATE
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F ) == 0		||	//선물체결(시세)
		strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F ) == 0		||	//선물호가
		strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F_CME ) == 0	||	//CME시세
		strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F_CME ) == 0	||	//CME호가
		strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_O ) == 0		||	//옵션시세
		strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_O ) == 0			//옵션호가
		)
	{
		SiseProc_KS(pRealPacket);
		return 0L;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	//	해외선물체결(시세)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_FU ) == 0 )
	{
		cStk = 'F';
		
		//	내부패킷으로 변경 후 멀티캐스트 전송
		CCVT_ETK cvt;
		double dErrSise = 0;
		
		if( !cvt.de_FU_SiseEx( pRealPacket->pszData, szSendBuf, &dErrSise ) )
		{
			//SYSTEMTIME st; GetLocalTime(&st);
			//char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			//if( strcmp(szTime, "09:00")>0 ){
			//	CUtil::LogEx(&m_logF, &g_logEvent, FALSE, "현재가이상!!!(%f)", dErrSise );
			//}
			return 0L;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//	해외선물호가
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_FU ) == 0 )
	{

		bHoga = TRUE;
		cStk = 'F';
		
		//	내부패킷으로 변경 후 멀티캐스트 전송
		CCVT_ETK cvt;
		if( !cvt.de_FU_Hoga( pRealPacket->pszData, szSendBuf ) )
		{
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "호가CONVERT 오류" );
			return 0L;
		}
	}


	//	SHM 에 저장
	SaveSHM( szSendBuf );
	
	//	FRONT 에게 전송		
	nLen = strlen(szSendBuf);

	int nRet = m_sockFront->Send_Data( szSendBuf,  nLen);
	if( nRet<=0 )
	{
		CUtil::LogEx('E', &m_logF, NULL, FALSE, "시세전송이상!!!(%s)(%d)",GetLastError(), szSendBuf );
		//ASSERT(nRet>0);
	}

	//lable:LOSSCUT_SISE
	if( !bHoga )
	{
		int nRet = m_sockLosscut->Send_Data( szSendBuf,  nLen);
		if( nRet<=0 )
		{
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "LOSSCUT현재가전송이상!!!(%d)(%s)",GetLastError(), szSendBuf );
			//ASSERT(nRet>0);
		}
	}

	// 시세중계
	//TCPRELAY m_clntSend->SendData(szSendBuf, strlen(szSendBuf));
	Relay_PostData(szSendBuf,  nLen);
	
	return 0L;
}

VOID CDlg_FC0_1::SiseProc_KS(LPRECV_REAL_PACKET pRealPacket)
{
	char msg[4096] = {0,};
	char szSendBuf[MAX_SENDSIZE] = {0,};
	char szSendBuf_toFRONT[MAX_SENDSIZE] = {0,};
	char cStk;
	INT nLen;
	BOOL bHoga = FALSE;	
	BOOL bFind = FALSE;
	BOOL bDongsi = FALSE;
	

	//////////////////////////////////////////////////////////////////////////
	//	선물체결(시세)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F ) == 0 )
	{
		cStk = 'F';
		
		//	내부패킷으로 변경 후 멀티캐스트 전송
		CCVT_ETK cvt;
		//시세오류고려.		cvt.de_KS_F_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT );
		double dErrSise = 0;
		if( !cvt.de_KS_F_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT, &dErrSise ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			if( strcmp(szTime, "10:00")>0 ){
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "현재가이상!!!(%f)", dErrSise );
			}
			return ;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//	선물호가
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F ) == 0 )
	{

		bHoga = TRUE;
		cStk = 'F';
		
		//	내부패킷으로 변경 후 멀티캐스트 전송
		CCVT_ETK cvt;

		if( !cvt.de_KS_F_HogaDongsi( pRealPacket->pszData, szSendBuf, &bDongsi ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			if( strcmp(szTime, "10:00")>0 ){
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "시세이상처리!!!(%.*s)"
								, sizeof(FH0_OutBlock), pRealPacket->pszData );
			}
			return ;
		}
		//strcpy( szSendBuf_toFRONT, szSendBuf );
	}

	//CME
	//////////////////////////////////////////////////////////////////////////
	//	CME체결(시세)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_F_CME ) == 0 )
	{
		cStk = 'F';
		
		//	내부패킷으로 변경 후 멀티캐스트 전송
		CCVT_ETK cvt;
		cvt.de_KS_F_SiseEx_CME( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT );
	}
	
	///////////////////////////////////////////////////////////////////////////
	//	CME호가
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_F_CME ) == 0 )
	{
		
		bHoga = TRUE;
		cStk = 'F';
		
		//	내부패킷으로 변경 후 멀티캐스트 전송
		CCVT_ETK cvt;
		
		if( !cvt.de_KS_F_Hoga_CME( pRealPacket->pszData, szSendBuf ) )
		{
			SYSTEMTIME st; GetLocalTime(&st);
			char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
			if( strcmp(szTime, "10:00")>0 ){
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "시세이상처리!!!(%.*s)"
					, sizeof(FH0_OutBlock), pRealPacket->pszData );
			}
			return ;
		}
		//strcpy( szSendBuf_toFRONT, szSendBuf );
	}


	//////////////////////////////////////////////////////////////////////////
	//	옵션 체결(시세)
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_SISE_O ) == 0 )
	{
		cStk = 'O';
		
		//	내부패킷으로 변경 
		CCVT_ETK cvt;
		cvt.de_KS_O_SiseEx( pRealPacket->pszData, szSendBuf, szSendBuf_toFRONT);
	}
	///////////////////////////////////////////////////////////////////////////
	//	옵션 호가
	if( strcmp( pRealPacket->szTrCode, ETK_REAL_HOGA_O ) == 0 )
	{
		cStk = 'O';
		bHoga = TRUE;
		
		//	내부패킷으로 변경 
		CCVT_ETK cvt;
		cvt.de_KS_O_Hoga( pRealPacket->pszData, szSendBuf );		
		//strcpy( szSendBuf_toFRONT, szSendBuf );
	}

	//	혹시 futures, option 을 나눌것을 대비하여 if 한다.
	//if( cStk=='F' || cStk=='O' )
	{
		/*if(bHoga)
		{
			if( (prop.GetLongValue("RUN1")==0)  )
				return 0;
		}*/

		

		// v2.3 로스컷에게 전송
		
		//	FRONT 에게 전송		
		int nRet;
		if( bHoga ){
			nLen = strlen(szSendBuf);
			nRet = m_sockFront->Send_Data( szSendBuf,  nLen);
		}
		else
		{
			nLen = strlen(szSendBuf_toFRONT);
			nRet = m_sockFront->Send_Data( szSendBuf_toFRONT,  nLen);
			if( nRet<=0 )
			{
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "시세전송이상!!!(%s)", szSendBuf_toFRONT );
				//ASSERT(nRet>0);
			}

			nRet = m_sockLosscut->Send_Data( szSendBuf_toFRONT,  nLen);
			if( nRet<=0 )
			{
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "LOSSCUT현재가전송이상!!!(%s)", szSendBuf_toFRONT );
				//ASSERT(nRet>0);
			}
		}
	}

	//	SHM 에 저장
	//	동시호가는 저장하지 않는다.
	if (!bDongsi)
		SaveSHM_KS( szSendBuf );

	
	// 시세중계. 옵션은 중계하지 않는다.
	/*TCPRELAY
	if( cStk!='O'){
		if( bHoga )
			m_clntSend_KS->SendData(szSendBuf, strlen(szSendBuf));
		else
		{
			m_clntSend_KS->SendData(szSendBuf_toFRONT, strlen(szSendBuf_toFRONT));
			m_clntSend_KS->SendData(szSendBuf, strlen(szSendBuf));
		}
	}
	*/



	return ;
}

VOID CDlg_FC0_1::SaveSHM( char* pSise )
{
	
	REAL_SISE_FU* pSiseFU = (REAL_SISE_FU*)pSise;
	REAL_HOGA_FU* pHogaFU = (REAL_HOGA_FU*)pSise;
	
	int		nRet			= 0;
	char	szStkCode[MAX_STK_CODE+1] = {0,};
	char	szMsgCode[5+1] = {0,};

	memset( szStkCode, 0x20, sizeof(szStkCode) );	// skeo 2011-06-07

	// key value;
	CopyMemory( szStkCode,				pSiseFU->stk_code, MAX_STK_CODE );	// STK_CODE
	
	// msg code
	CopyMemory( szMsgCode, pSiseFU->Header.Code, sizeof(pSiseFU->Header.Code) );


	//////////////////////////////////////////////////////////////////////////
	//	GROUP 이 없으면 GROUP INSERT
	if ( m_shm.FindGroup( szStkCode ) == FALSE )
	{
		if ( !m_shm.InsertGroup( szStkCode ) )
		{
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM InsertGroup error (%s)\n", szStkCode );
			return;
		}
		
		if ( (strcmp(szMsgCode, CODE_REAL_SISE_FU_S)==0) ) //시세인 경우만
		{
			if ( !m_shm.InsertData( szStkCode, (char*)pSiseFU->stk_code ) )
			{
				CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM SISE InsertData error-1 (%s)\n", szStkCode );
				return;
			}
		}
		else
		{
			// 호가지만 시세패킷으로 업데이트
			REAL_SISE_FU stSiseFU;
			memset( &stSiseFU, 0x20, sizeof(stSiseFU) );
			CopyMemory( &(stSiseFU.stk_code), szStkCode, MAX_STK_CODE );
			
			if ( !m_shm.InsertData( szStkCode, (char*)&(stSiseFU.stk_code) ) )
			{
				CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM SISE(HOGA) InsertData error (%s)\n", szStkCode );
				return;
			}
		}
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 호가 저장
	if ( (strcmp(szMsgCode, CODE_REAL_HOGA_FU_S)==0) ) // ||	(strcmp(szMsgCode, "82103")==0) || (strcmp(szMsgCode, "83103")==0) )
	{
		if ( m_shm.FindData( szStkCode, szStkCode ) != TRUE )
		{
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM에 호가 종목이 없음-2(%s)", szStkCode);
			return;
		}
		// SharedMemory is Sise Structure
		int nSellStartPos =
			sizeof(pSiseFU->stk_code) +
			sizeof(pSiseFU->tm) +
			sizeof(pSiseFU->nttn_code) +
			sizeof(pSiseFU->now_prc) +
			sizeof(pSiseFU->open) +
			sizeof(pSiseFU->high) +
			sizeof(pSiseFU->low) +
			sizeof(pSiseFU->last_close) +
			sizeof(pSiseFU->setl_price) +
			sizeof(pSiseFU->acml_cntr_qty) +
			sizeof(pSiseFU->acml_buy_cntr_qty) +
			sizeof(pSiseFU->acml_amt) +
			sizeof(pSiseFU->daebi_sign) +
			sizeof(pSiseFU->chg) +
			sizeof(pSiseFU->chg_rate) +
			sizeof(pSiseFU->cntr_qty);
		int nLen =
			sizeof(pSiseFU->o_hoga_1) +
			sizeof(pSiseFU->o_hoga_2) +
			sizeof(pSiseFU->o_hoga_3) +
			sizeof(pSiseFU->o_hoga_4) +
			sizeof(pSiseFU->o_hoga_5) +
			sizeof(pSiseFU->o_rmnq_1) +
			sizeof(pSiseFU->o_rmnq_2) +
			sizeof(pSiseFU->o_rmnq_3) +
			sizeof(pSiseFU->o_rmnq_4) +
			sizeof(pSiseFU->o_rmnq_5) +
			sizeof(pSiseFU->b_trmnq) +
			sizeof(pSiseFU->o_cnt_1) +
			sizeof(pSiseFU->o_cnt_2) +
			sizeof(pSiseFU->o_cnt_3) +
			sizeof(pSiseFU->o_cnt_4) +
			sizeof(pSiseFU->o_cnt_5) +
			sizeof(pSiseFU->o_tot_cnt);
			/*+
			sizeof(pSiseFU->o_hoga_cnt_chg_1) +
			sizeof(pSiseFU->o_hoga_cnt_chg_2) +
			sizeof(pSiseFU->o_hoga_cnt_chg_3) +
			sizeof(pSiseFU->o_hoga_cnt_chg_4) +
			sizeof(pSiseFU->o_hoga_cnt_chg_5);
		**/
		int nBuyStartPos =
			nSellStartPos + nLen +
			sizeof(pSiseFU->o_hoga_cnt_chg_1) +
			sizeof(pSiseFU->o_hoga_cnt_chg_2) +
			sizeof(pSiseFU->o_hoga_cnt_chg_3) +
			sizeof(pSiseFU->o_hoga_cnt_chg_4) +
			sizeof(pSiseFU->o_hoga_cnt_chg_5);
						int nTimeStartPos = sizeof(pSiseFU->stk_code);

		//m_shm.Lock();		// LOCK
		if ( !m_shm.UpdateData( szStkCode, szStkCode, nSellStartPos, nLen, (char*)pHogaFU->o_hoga_1 ) )
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData-1 error (%s)\n", szStkCode );

		if ( !m_shm.UpdateData( szStkCode, szStkCode, nBuyStartPos, nLen-1, (char*)pHogaFU->b_hoga_1 ) )
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData-2 error (%s)\n", szStkCode );

		if ( !m_shm.UpdateData( szStkCode, szStkCode, nTimeStartPos, sizeof(pHogaFU->tm), (char*)pHogaFU->tm ) )
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData error-3 (%s)\n", szStkCode );

		//m_shm.Unlock();	// UNLOCK
	
	}	//	if ( (strcmp(szMsgCode, CODE_REAL_HOGA_FU_S)==0) ) 호가


	//////////////////////////////////////////////////////////////////////////
	// 시세
	if ( (strcmp(szMsgCode, CODE_REAL_SISE_FU_S)==0) )
	{
		//m_shm.Lock();		// LOCK
		if ( !m_shm.FindData( szStkCode, szStkCode ) )
		{
			if ( !m_shm.InsertData( szStkCode, (char*)pSiseFU->stk_code ) )
				CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM SISE InsertData error (%s)\n", szStkCode );
			
			return;
		}

		int nDataCnt = m_shm.GetCurrStructCnt( szStkCode );
		if ( nDataCnt == 0 )
		{
			if ( !m_shm.InsertData( szStkCode, (char*)pSiseFU->stk_code ) )
			{
				CUtil::LogEx('E', &m_logF, NULL, FALSE, "시세를 SHM INSERT ERROR (%s)\n", szStkCode );
				return;
			}
		}
		else
		{
			//int nDataCnt = m_shm.GetCurrStructCnt( szStkCode );
			//if ( nDataCnt == 0 )
			//{
			//	if ( !m_shm.InsertData( szStkCode, (char*)pSiseFU->stk_code ) )
			//		CUtil::LogEx(&m_logF,&g_logEvent, FALSE, "SHM SISE InsertData error (%s)\n", szStkCode );
			//}
			//else
			//{
				//	시세 시작점
			int nSiseStartPos = sizeof(pSiseFU->stk_code) + sizeof(pSiseFU->tm) + sizeof(pSiseFU->nttn_code);
			int nSiseLen =	sizeof(pSiseFU->now_prc) +
							sizeof(pSiseFU->open) +
							sizeof(pSiseFU->high) +
							sizeof(pSiseFU->low) +
							sizeof(pSiseFU->acml_cntr_qty) +
							sizeof(pSiseFU->acml_buy_cntr_qty) +
							sizeof(pSiseFU->acml_amt) +
							sizeof(pSiseFU->daebi_sign) +
							sizeof(pSiseFU->chg) +
							sizeof(pSiseFU->chg_rate) +
							sizeof(pSiseFU->cntr_qty);
				if ( !m_shm.UpdateData( szStkCode, szStkCode, nSiseStartPos, nSiseLen, pSiseFU->now_prc ) )
					CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM SISE UpdateData error (%s)\n", szStkCode );
			//}
		}
	}
		//m_shm.Unlock();	// UNLOCK
}




VOID CDlg_FC0_1::SaveSHM_KS( char* pSise )
{
	REAL_SISE_KS* pSiseKS = (REAL_SISE_KS*)pSise;
	REAL_HOGA_KS* pHogaKS = (REAL_HOGA_KS*)pSise;
	
	int		nRet			= 0;
	char	szStkCode[MAX_GDS_CODE+MAX_STK_CODE+1] = {0,};
	char	szMsgCode[5+1] = {0,};

	memset( szStkCode, 0x20, MAX_GDS_CODE+MAX_STK_CODE );


	// key value;
	CopyMemory( szStkCode,				pSiseKS->stk_code, MAX_STK_CODE );	// STK_CODE
	CopyMemory( szStkCode+MAX_STK_CODE,	pSiseKS->gds_code, MAX_GDS_CODE );	// GDS_CODE
	
	// msg code
	CopyMemory( szMsgCode, pSiseKS->Header.Code, sizeof(pSiseKS->Header.Code) );

	//////////////////////////////////////////////////////////////////////////
	//	GROUP 이 없으면 GROUP INSERT
	if ( m_shmKS.FindGroup( szStkCode ) == FALSE )
	{
		if ( !m_shmKS.InsertGroup( szStkCode ) )
		{
			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM InsertGroup error (%s)\n", szStkCode );
			return;
		}
		
		if ( (strcmp(szMsgCode, CODE_REAL_SISE_KS_S)==0) ) //시세인 경우만
		{
			if ( !m_shmKS.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
			{
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM SISE InsertData error-1 (%s)\n", szStkCode );
				return;
			}
		}
		else
		{
			// 호가지만 시세패킷으로 업데이트
			REAL_SISE_KS stSiseKS;
			memset( &stSiseKS, 0x20, sizeof(REAL_SISE_KS) );
			CopyMemory( &(stSiseKS.stk_code), szStkCode, MAX_GDS_CODE+MAX_STK_CODE );
			
			if ( !m_shmKS.InsertData( szStkCode, (char*)&(stSiseKS.stk_code) ) )
			{
				CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM SISE(HOGA) InsertData error (%s)\n", szStkCode );
				return;
			}
		}
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 호가 저장
	if ( (strcmp(szMsgCode, CODE_REAL_HOGA_KS_S)==0) ) // ||	(strcmp(szMsgCode, "82103")==0) || (strcmp(szMsgCode, "83103")==0) )
	{
		if ( m_shmKS.FindData( szStkCode, szStkCode ) != TRUE )
		{
			CUtil::LogEx('E', &m_logF, NULL, FALSE, "SHM에 호가 종목이 없음-2(%s)", szStkCode);
			return;
		}
		// SharedMemory is Sise Structure
		int nSellStartPos =
			sizeof(pSiseKS->stk_code) +
			sizeof(pSiseKS->gds_code) +
			sizeof(pSiseKS->tm) +
			sizeof(pSiseKS->now_prc) +
			sizeof(pSiseKS->open) +
			sizeof(pSiseKS->high) +
			sizeof(pSiseKS->low) +
			sizeof(pSiseKS->acml_cntr_qty) +
			sizeof(pSiseKS->acml_buy_cntr_qty) +
			sizeof(pSiseKS->acml_amt) +
			sizeof(pSiseKS->daebi_sign) +
			sizeof(pSiseKS->chg) +
			sizeof(pSiseKS->chg_rate) +
			sizeof(pSiseKS->cntr_qty);
		int nLen =
			sizeof(pSiseKS->o_hoga_1) +
			sizeof(pSiseKS->o_hoga_2) +
			sizeof(pSiseKS->o_hoga_3) +
			sizeof(pSiseKS->o_hoga_4) +
			sizeof(pSiseKS->o_hoga_5) +
			sizeof(pSiseKS->o_rmnq_1) +
			sizeof(pSiseKS->o_rmnq_2) +
			sizeof(pSiseKS->o_rmnq_3) +
			sizeof(pSiseKS->o_rmnq_4) +
			sizeof(pSiseKS->o_rmnq_5) +
			sizeof(pSiseKS->b_trmnq) +
			sizeof(pSiseKS->o_cnt_1) +
			sizeof(pSiseKS->o_cnt_2) +
			sizeof(pSiseKS->o_cnt_3) +
			sizeof(pSiseKS->o_cnt_4) +
			sizeof(pSiseKS->o_cnt_5) +
			sizeof(pSiseKS->o_tot_cnt);
		int nBuyStartPos =
			nSellStartPos + nLen +
			sizeof(pSiseKS->o_hoga_cnt_chg_1) +
			sizeof(pSiseKS->o_hoga_cnt_chg_2) +
			sizeof(pSiseKS->o_hoga_cnt_chg_3) +
			sizeof(pSiseKS->o_hoga_cnt_chg_4) +
			sizeof(pSiseKS->o_hoga_cnt_chg_5);
		int nTimeStartPos = sizeof(pSiseKS->stk_code) + sizeof(pSiseKS->gds_code);

		//m_shm.Lock();		// LOCK
		if ( !m_shmKS.UpdateData( szStkCode, szStkCode, nSellStartPos, nLen, (char*)pHogaKS->o_hoga_1 ) )
			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData-1 error (%s)\n", szStkCode );

		if ( !m_shmKS.UpdateData( szStkCode, szStkCode, nBuyStartPos, nLen-1, (char*)pHogaKS->b_hoga_1 ) )
			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData-2 error (%s)\n", szStkCode );

		if ( !m_shmKS.UpdateData( szStkCode, szStkCode, nTimeStartPos, sizeof(pHogaKS->tm), (char*)pHogaKS->tm ) )
			CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM HOGA UpdateData error-3 (%s)\n", szStkCode );

		//m_shm.Unlock();	// UNLOCK
	
	}	//	if ( (strcmp(szMsgCode, CODE_REAL_HOGA_KS_S)==0) ) 호가


	//////////////////////////////////////////////////////////////////////////
	// 시세
	if ( (strcmp(szMsgCode, CODE_REAL_SISE_KS_S)==0) )
	{
		//m_shm.Lock();		// LOCK
		if ( !m_shmKS.FindData( szStkCode, szStkCode ) )
		{
			if ( !m_shmKS.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM SISE InsertData error (%s)\n", szStkCode );
			
			return;
		}

		int nDataCnt = m_shmKS.GetCurrStructCnt( szStkCode );
		if ( nDataCnt == 0 )
		{
			if ( !m_shmKS.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
			{
				CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "시세를 SHM INSERT ERROR (%s)\n", szStkCode );
				return;
			}
		}
		else
		{
			//int nDataCnt = m_shm.GetCurrStructCnt( szStkCode );
			//if ( nDataCnt == 0 )
			//{
			//	if ( !m_shm.InsertData( szStkCode, (char*)pSiseKS->stk_code ) )
			//		CUtil::LogEx(&m_logF,&g_logEvent, FALSE, "SHM SISE InsertData error (%s)\n", szStkCode );
			//}
			//else
			//{
				//	시세 시작점
				int nSiseStartPos = sizeof(pSiseKS->stk_code) + sizeof(pSiseKS->gds_code) + sizeof(pSiseKS->tm);
				int nSiseLen =	sizeof(pSiseKS->now_prc) +
								sizeof(pSiseKS->open) +
								sizeof(pSiseKS->high) +
								sizeof(pSiseKS->low) +
								sizeof(pSiseKS->acml_cntr_qty) +
								sizeof(pSiseKS->acml_buy_cntr_qty) +
								sizeof(pSiseKS->acml_amt) +
								sizeof(pSiseKS->daebi_sign) +
								sizeof(pSiseKS->chg) +
								sizeof(pSiseKS->chg_rate) +
								sizeof(pSiseKS->cntr_qty)+1;
				//	시고저종,체결량 등 저장
				if ( !m_shmKS.UpdateData( szStkCode, szStkCode, nSiseStartPos, nSiseLen, pSiseKS->now_prc ) )
					CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM SISE UpdateData error (%s)\n", szStkCode );
				else
				{
					char msg[512];
					sprintf(msg, "SISE UPD[prc:%.*s][QTY:%.*s]\n", 
								sizeof(pSiseKS->now_prc), pSiseKS->now_prc, 
								sizeof(pSiseKS->cntr_qty), pSiseKS->cntr_qty);
					OutputDebugString(msg);
				}

				//	KS200 저장
				int nHogaLen =	(sizeof(pSiseKS->o_hoga_1)	* 5)	+ 
								(sizeof(pSiseKS->o_rmnq_1)	* 6)	+	//	o_trmnq 포함
								(sizeof(pSiseKS->o_cnt_1)	* 5)	+
								(sizeof(pSiseKS->o_tot_cnt)	)		+
								(sizeof(pSiseKS->o_hoga_cnt_chg_1)) * 5;
				
				nHogaLen *=2;	//	매도매수 고려
				
				nSiseStartPos = nSiseStartPos + nSiseLen + nHogaLen;

				nSiseLen =	sizeof(pSiseKS->ks200) +
							sizeof(pSiseKS->hoga_trmnq_chg) +
							sizeof(pSiseKS->qty_chg) +
							sizeof(pSiseKS->amt_chg) +
							sizeof(pSiseKS->theory_prc) +
							sizeof(pSiseKS->open_yak) +
							sizeof(pSiseKS->open_yak_chg) +
							sizeof(pSiseKS->gyuri_rate) +
							sizeof(pSiseKS->basis) ;
							
				//char* p = pSise + nSiseStartPos;

				//if ( !m_shm.UpdateData( szStkCode, szStkCode, nSiseStartPos, sizeof(pSiseKS->ks200), pSiseKS->ks200 ) )4
				if ( !m_shmKS.UpdateData( szStkCode, szStkCode, nSiseStartPos, nSiseLen, pSiseKS->ks200 ) )
					CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "SHM KS200 UpdateData error (%s)\n", szStkCode );

				//else
				//	CUtil::LogEx(&m_logF, FALSE, "[SHM SISE](%s)", pSiseKS->now_prc );
			//}
		}
	}
		//m_shmKS.Unlock();	// UNLOCK
}

/*
	front / shm / match server 에게 전송할 스레드
*/
// DWORD WINAPI CDlg_FC0_1::ConnectThread(LPVOID lp)
// {
// 	CDlg_FC0_1* p = (CDlg_FC0_1*)lp;
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
// // 	//	최초 소켓 생성
// // 	if( !p->SockInit() ){
// // 		PostThreadMessage(GetCurrentThreadId(), MSG_RE_CONNECT, (WPARAM)0, (LPARAM)0 );
// // 		sprintf(msg, "[%s] 초기 connect 실패로 PostThreadMessage", target);
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
// // 				sprintf(msg,"[%s] SOCKET INIT 실패로 retry ",target);
// // 				CUtil::LogEx(&p->m_logF, &g_logEvent, FALSE, msg);
// // 				OutputDebugString(msg); OutputDebugString("\n");
// // 
// // 				if( WaitForSingleObject(p->m_hDie[idx],1000)==WAIT_OBJECT_0 )
// // 					return 0;
// // 			}
// // 			SYSTEMTIME st; GetLocalTime(&st);
// // 			sprintf(msg,"[%02d:%02d:%02d.%03d][%s] INIT thread 에서 INIT 시도 후 성공 "
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
// DWORD WINAPI CDlg_FC0_1::SendThread_SHM(LPVOID lp)
// {
// 	CDlg_FC0_1* p = (CDlg_FC0_1*)lp;
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
// 		//	연결이 안된 상태이면 계속 연결 시도
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

void CDlg_FC0_1::OnButtonUnadvise() 
{
	// TODO: Add your control notification handler code here
	//	선물 시세/호가 해제
	list<CString>::iterator it;
	for( it=m_sStkList.begin(); it!=m_sStkList.end(); it++ )
	{
		CString sStk = *it;
		UnadviseData('Y'/*시세*/, sStk);		
		UnadviseData('N'/*호가*/, sStk);
	}
}

DWORD WINAPI CDlg_FC0_1::ProcDummySiseTest(LPVOID lp) 
{
	CDlg_FC0_1 *pT = (CDlg_FC0_1*)lp;

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
		if ( strstr(szBuff, "시세 요청") ||  strstr(szBuff, "호가 요청") || strstr(szBuff, "중지"))
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
		//	선물체결(시세)
		if( strstr(szBuff, "F_SISE" ) > 0 )
		{
			cStk = 'F';
			FC0_OutBlock* p = (FC0_OutBlock*)(&szBuff[23]);
			//	내부패킷으로 변경 후 멀티캐스트 전송
			CCVT_ETK cvt;
			//시세이상고려. cvt.de_KS_F_SiseEx( (char*)p, szSendBuf, szSendBuf_toFRONT );
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
		//	선물호가
		else if( strstr(szBuff, "F_HOGA" ) > 0 )
		{
			cStk = 'F';
			FH0_OutBlock* p = (FH0_OutBlock*)(&szBuff[23]);
			
			//	내부패킷으로 변경 후 멀티캐스트 전송
			CCVT_ETK cvt;
			cvt.de_KS_F_Hoga( (char*)p, szSendBuf );
			strcpy(szSendBuf_toFRONT, szSendBuf);
// 			REAL_SISE_KS* pack = (REAL_SISE_KS*)szSendBuf;
// 			if( strncmp(pack->tm, "09",2)<0 )	continue;
		}
		
		//////////////////////////////////////////////////////////////////////////
		//	옵션 체결(시세)
		else if( strstr(szBuff, "O_SISE" ) > 0 )
		{
			cStk = 'O';
			//if( strstr(szBuff, "201E9232")==0 && strstr(szBuff, "301E9232")==0 )
		//		continue;
			OC0_OutBlock* p = (OC0_OutBlock*)(&szBuff[23]);

			//	내부패킷으로 변경 후 멀티캐스트 전송
			CCVT_ETK cvt;
			cvt.de_KS_O_Sise( (char*)p, szSendBuf );
 			REAL_SISE_KS* pack = (REAL_SISE_KS*)szSendBuf;
			//if( pack->stk_code[4]=='1' )
			//	pack->stk_code[4]='1';
		}
		///////////////////////////////////////////////////////////////////////////
		//	옵션 호가
		else if( strstr(szBuff, "O_HOGA" ) > 0 )
		{
			cStk = 'O';
			//if( strstr(szBuff, "201E9232")==0 && strstr(szBuff, "301E9232")==0 )
			//	continue;
			OH0_OutBlock* p = (OH0_OutBlock*)(&szBuff[23]);

			//	내부패킷으로 변경 후 멀티캐스트 전송
			CCVT_ETK cvt;
			cvt.de_KS_O_Hoga( (char*)p, szSendBuf );
 			REAL_HOGA_KS* pSend = (REAL_HOGA_KS*)szSendBuf;
			//if( pSend->stk_code[4]=='1' )
			//	pack->stk_code[4]='1';
		}
		else
			continue;
		
		//	혹시 futures, option 을 나눌것을 대비하여 if 한다.
		int nErrCode = 0;
		if( cStk=='F' || cStk=='O' )
		{
			//	FRONT 에게 전송
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

			pT->m_sockFront->Send_Data(szSendBuf_toFRONT, strlen(szSendBuf_toFRONT) );
			//lable:LOSSCUT_SISE
			pT->m_sockLosscut->Send_Data(szSendBuf_toFRONT, strlen(szSendBuf_toFRONT) );
		}
		
		//	SHM 에게 전송
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
		
		//	SHM 에 저장
		pT->SaveSHM( szSendBuf );
		
		Sleep(100);
	}

	delete[] data;

}



void CDlg_FC0_1::OnButtonTest_1() 
{
	static BOOL OnOffDummySise = TRUE;
	//------------------------------
	//	스레드 생성
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
BOOL CDlg_FC0_1::Relay_Begin()
{
	/////////////////////////////////////////////////////////////
	//	SOCKET INIT
	m_sockListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(m_sockListen == INVALID_SOCKET)
	{
		shutdown(m_sockListen,SD_SEND);
		closesocket(m_sockListen);
		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay ACCEPT SOCKET CRETAE Error.ErrCode:%d", GetLastError() );
		return FALSE;
	}
	
	INT nTcpPort;
	char szTemp[128];
	CUtil::GetCnfgFile("api_if", "ETK_F", "LISTEN_PORT", szTemp);
	nTcpPort = atoi(szTemp);

	////	local address
	m_localsin.sin_family      = AF_INET;
    m_localsin.sin_port        = htons(nTcpPort);
    m_localsin.sin_addr.s_addr = inet_addr(m_szLocalIP);
	
	BOOL opt = TRUE;
	int optlen = sizeof(opt);
	//	setsockopt(m_sockListen,SOL_SOCKET,SO_REUSEADDR,(const char far *)&opt,optlen);
	
	if (::bind(m_sockListen, (LPSOCKADDR) &m_localsin, sizeof(m_localsin)) == SOCKET_ERROR)
	{
		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay bind Error.ErrCode:%d", GetLastError() );
		return FALSE;
	}
	CUtil::LogEx( 'I', &m_logF, NULL, FALSE, "relay BIND OK : %s , %d", m_szLocalIP, nTcpPort);
	
	if(listen(m_sockListen, SOMAXCONN) == SOCKET_ERROR)
	{
		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay listen Error.ErrCode:%d", GetLastError() );
		return FALSE;
	}
	
	m_wsaAccept = WSACreateEvent();
	if(WSAEventSelect(m_sockListen, m_wsaAccept, FD_ACCEPT))
	{
		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "relay listen socket WSAEventSelect Error.ErrCode:%d", GetLastError() );
		return FALSE;
	}

	//	OVERLAPPED 를 위한 event
	m_wsaSend = WSACreateEvent();	

	InitializeCriticalSection(&m_csRelaySock);
	InitializeCriticalSection(&m_csRelayData);

	// CREATE THREAD
	if( (m_hRelayThread = CreateThread(NULL,0,Relay_SendDThread,this,0,&m_dwRelayThread)) == NULL )
	//if( (m_hRelayThread = CreateThread(NULL,0,Relay_SendDThread,this,CREATE_SUSPENDED,&m_dwRelayThread)) == NULL )
	{
		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "RELAY THREAD 생성 실패.ErrCode:%d", GetLastError() );
		return FALSE;
	}	


	m_bRelayContinue = TRUE;
	
	CUtil::LogEx( 'I', &m_logF, NULL, FALSE, "TCP RELAY 초기화 성공" );

	return TRUE;
}

//TCPRELAY
VOID CDlg_FC0_1::Relay_End()
{
	m_bRelayContinue = FALSE;

	if( WaitForSingleObject(m_hRelayThread, 3000) != WAIT_OBJECT_0 )
		TerminateThread(m_hRelayThread,0);
	
	SAFE_CLOSEHANDLE(m_hRelayThread);
	SAFE_CLOSEHANDLE(m_wsaAccept);
	SAFE_CLOSEHANDLE(m_wsaSend);
	SAFE_CLOSESOCKET(m_sockListen);

	DeleteCriticalSection(&m_csRelaySock);
	DeleteCriticalSection(&m_csRelayData);

}


//TCPRELAY
DWORD WINAPI  CDlg_FC0_1::Relay_SendDThread(LPVOID lp)
{
	CDlg_FC0_1* p = (CDlg_FC0_1*)lp;
	MSG msg;
	char*	pData;
	char	szData[MAX_SENDSIZE];
	int		nLen;
	SOCKADDR_IN			sinClient;
	int	sinSize = sizeof(sinClient);
	while(p->m_bRelayContinue)
	{
		DWORD dw = WSAWaitForMultipleEvents(1, &p->m_wsaAccept, TRUE, 1, FALSE);
		if( dw== WSA_WAIT_EVENT_0)
		{
			WSAResetEvent(p->m_wsaAccept);
			
			SOCKET client = accept(p->m_sockListen, (LPSOCKADDR)&sinClient, &sinSize);
			if( client == INVALID_SOCKET)
			{
				CUtil::LogEx( 'E', &p->m_logF, NULL, FALSE, "RELAY accept error(%d)", WSAGetLastError());
			}
			else
			{
				
				//	client public ip 추출
				SOCKADDR_IN peer_addr;
				int			peer_addr_len = sizeof(peer_addr);

				if (getpeername( client, (sockaddr*)&peer_addr, &peer_addr_len) == 0 )
				{
					char* pIP	= inet_ntoa(peer_addr.sin_addr);
					CUtil::LogEx( 'I', &p->m_logF, NULL, FALSE, "accept[%s]", pIP);
				}
				EnterCriticalSection(&p->m_csRelaySock);
				p->m_setRelaySock.insert(client);
				LeaveCriticalSection(&p->m_csRelaySock);
				
			}
		}
		
		/*CASE 1
		while( PeekMessage(&msg, NULL, 0,0, PM_REMOVE) )
		{
			switch(msg.message)
			{
			case WM_SISE_DATA:
				
				nLen  = (int) msg.wParam;
				pData = (char*) msg.lParam;
				sprintf(szData, "%.*s", nLen, pData );
				delete[] pData;
				//p->m_lstMsg.InsertString(0, szData);
				//CUtil::LogEx( 'I', &p->m_logF, NULL, FALSE, "SEND[%s]", szData);
				p->Relay_SendData(szData, nLen);
	
				break;
			default:
				break;
			}
		}
		*/

		EnterCriticalSection(&p->m_csRelayData);
		if( !p->m_lstRelayData.empty() ){
			CString data = *p->m_lstRelayData.begin();
			p->m_lstRelayData.pop_front();
			p->Relay_SendData((LPSTR)data.GetBuffer(), data.GetLength());
		}
		LeaveCriticalSection(&p->m_csRelayData);
	}
	return 0;
}

//TCPRELAY

VOID CDlg_FC0_1::Relay_PostData(LPSTR lpData, INT nLen)
{
	//char* pData = new char[nLen];
	//memcpy( pData, lpData, nLen );
	//CASE 1 PostThreadMessage( m_dwRelayThread, WM_SISE_DATA, (WPARAM)nLen,	(LPARAM)pData );

	EnterCriticalSection(&m_csRelayData);
	m_lstRelayData.push_back(lpData);
	LeaveCriticalSection(&m_csRelayData);
}

// TCPRELAY
VOID CDlg_FC0_1::Relay_SendData(LPSTR lpData, INT nLen)
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
				Relay_RequestSendIO(sockClient, lpData, nLen);
			}
			
			if( ++itSet==m_setRelaySock.end() )
				break;
			
		}	//	if( !pStkSockSet->setSock.empty() )
		LeaveCriticalSection(&m_csRelaySock);
	}
	catch(...)
	{
		LeaveCriticalSection(&m_csRelaySock);
		CUtil::LogEx( 'E', &m_logF, NULL, FALSE, "RELAY SEND EXCEPTION-2");
	}
}


//TCPRELAY
BOOL CDlg_FC0_1::Relay_RequestSendIO( SOCKET sock, char* pSendBuf, int nSendLen )
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
