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

//�ؿܼ���
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
//char g_CnfgIdx[32]={0,};	// main ���� parameter �� �޴� ��

//SMILOR.2������
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
	//	�������� ��������
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
	//	���� �ü�/ȣ�� ��û
	list<CString>::iterator it;
	for( it=m_lstSymbols.begin(); it!=m_lstSymbols.end(); it++ )
	{
		CString sStk = *it;
		AdviseData('Y'/*TICK*/, sStk);

		AdviseData('N'/*Quote*/, sStk);
	}

}


//--------------------------------------------------------------------------------------
// ��Ʈ�� �ʱ�ȭ
//--------------------------------------------------------------------------------------
void CDlg_FC0_1::InitCtrls()
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

//	SetDlgItemText(IDC_BUTTON_TEST, "FILE TEST Start");	// skeo 2011-01-17
}

//--------------------------------------------------------------------------------------
// ������ Advise
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
	// ������ ����
	BOOL bSuccess = g_iXingAPI.AdviseRealData( 
		GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
		szTrCode,					// TR ��ȣ
		symbol,						// �����ڵ�
		nSize						// �����ڵ� ����
		);

	//-----------------------------------------------------------
	// ����üũ
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
// ������ Unadvise
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
	// ������ ����
	BOOL bSuccess = g_iXingAPI.UnadviseRealData( 
												GetSafeHwnd(),				// �����͸� ���� ������, XM_RECEIVE_REAL_DATA ���� �´�.
												szTrCode,					// TR ��ȣ
		i_sSymbol,						// �����ڵ�
												nSize						// �����ڵ� ����
												);
	
	//-----------------------------------------------------------
	// ����üũ
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
// REAL �����͸� ����
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
	//	�ؿܼ���ȣ��
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
	//	���� �ü�/ȣ�� ����
	list<CString>::iterator it;
	for( it=m_lstSymbols.begin(); it!=m_lstSymbols.end(); it++ )
	{
		CString sStk = *it;
		UnadviseData('Y'/*�ü�*/, sStk);		
		UnadviseData('N'/*ȣ��*/, sStk);
	}
}


