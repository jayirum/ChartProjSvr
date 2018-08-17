
// Sample_xingACEDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Sample_xingACE.h"
#include "Sample_xingACEDlg.h"
#include "afxdialogex.h"

#include "../../IRUM_INC/eBest/CSPAT00600.h"	//���� �����ֹ�
#include "../../IRUM_INC/eBest/CSPAT00700.h"	//���� �����ֹ�
#include "../../IRUM_INC/eBest/CSPAT00800.h"	//���� ����ֹ�
#include "../../IRUM_INC/eBest/CSPAQ03700.h"	//�������� �ֹ�ü�᳻�� ��ȸ 
#include "../../IRUM_INC/eBest/CSPAQ02300.h"	// �������� �ܰ��� ��ȸ 
#include "../../IRUM_INC/eBest/SC0.h"	//REAL.�ֽ��ֹ�����
#include "../../IRUM_INC/eBest/SC1.h"	//REAL.�ֽ��ֹ�ü��
#include "../../IRUM_INC/eBest/SC2.h"	//REAL.�ֽ��ֹ�����
#include "../../IRUM_INC/eBest/SC3.h"	//REAL.�ֽ��ֹ����
#include "../../IRUM_INC/eBest/SC4.h"	//REAL.�ֽ��ֹ��ź�
//#include "../../IRUM_INC/eBest/S3_.h"	//REAL.KOSPIü��
//#include "../../IRUM_INC/eBest/H1_.h"	//REAL.KOSPIȣ���ܷ�
//#include "../../IRUM_INC/eBest/K3_.h"	//REAL.KOSDAQü��
//#include "../../IRUM_INC/eBest/HA_.h"	//REAL.KOSDAQȣ���ܷ�

#include "../../IRUM_INC/eBest/CIDBQ01500.h"	//�ؿܼ��� �̰��� �ܰ��� ��ȸ
#include "../../IRUM_INC/eBest/CIDBQ03000.h"	//�ؤʼ��� ������/�ܰ���Ȳ
#include "../../IRUM_INC/eBest/CIDBQ05300.h"	//�ؿܼ��� ���¿�Ź�ڻ� ��ȸ
#include "../../IRUM_INC/eBest/CIDBT00100.h"	//�ؿܼ��� �ű��ֹ�
#include "../../IRUM_INC/eBest/CIDBT00900.h"	//�ؿܼ��� �����ֹ�
#include "../../IRUM_INC/eBest/CIDBT01000.h"	//�ؿܼ��� ����ֹ�
#include "../../IRUM_INC/eBest/OVC.H"			//�ؿܼ��� TICK
#include "../../IRUM_INC/eBest/OVH.H"			//�ؿܼ��� ȣ��
#include "../../IRUM_INC/eBest/TC1.H"			//�ؿܼ��� �ֹ������ǽð�
#include "../../IRUM_INC/eBest/TC2.H"			//�ؿܼ��� �ֹ�����ǽð�
#include "../../IRUM_INC/eBest/TC3.H"			//�ؿܼ��� �ֹ�ü��ǽð�


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSample_xingACEDlg ��ȭ ����
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


// CSample_xingACEDlg �޽��� ó����

BOOL CSample_xingACEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	//-----------------------------------------------------------------------------------
	// �ʱ�ȭ
	if( m_iXingAPI.Init() == FALSE )
	{
		MessageBox( "xingAPI.dll �� ã�� �� �����ϴ�.\n �������ϰ� ���� ��ġ�� xingAPI.dll �� �־�� �մϴ�.", "xingACE ����", MB_ICONSTOP );
		PostMessage( WM_CLOSE );
		return TRUE;
	}
	InitCtrls();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSample_xingACEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSample_xingACEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSample_xingACEDlg::InitCtrls()
{
	int nCol;
	
	m_ctrlHoga.InsertColumn( 0, "�ܷ�", LVCFMT_RIGHT, 60 );
	m_ctrlHoga.InsertColumn( 1, "ȣ��", LVCFMT_RIGHT, 70 );
	m_ctrlHoga.InsertColumn( 2, "�ܷ�", LVCFMT_RIGHT, 60 );
	m_ctrlHoga.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	for( int i=0; i<10; i++ )
	{
		m_ctrlHoga.InsertItem( i, "" );
	}

	m_ctrlChe.InsertColumn( 0, "ü�ᰡ", LVCFMT_RIGHT, 70 );
	m_ctrlChe.InsertColumn( 1, "����",   LVCFMT_RIGHT, 60 );
	m_ctrlChe.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlOrderList.InsertColumn( nCol++, "�ֹ���ȣ"  , LVCFMT_LEFT , 70 );    // [long  ,   10] �ֹ���ȣ        
	m_ctrlOrderList.InsertColumn( nCol++, "���ֹ���ȣ", LVCFMT_LEFT , 70 );    // [long  ,   10] ���ֹ���ȣ      
	m_ctrlOrderList.InsertColumn( nCol++, "�����"    , LVCFMT_LEFT , 70 );    // [string,   40] �����          
	m_ctrlOrderList.InsertColumn( nCol++, "����"      , LVCFMT_LEFT , 70 );    // [string,   10] �Ÿű��и�      
	m_ctrlOrderList.InsertColumn( nCol++, "�������"  , LVCFMT_LEFT , 70 );    // [string,   10] ������ұ��и�  
	m_ctrlOrderList.InsertColumn( nCol++, "�ֹ�����"  , LVCFMT_RIGHT, 70 );    // [long  ,   16] �ֹ�����        
	m_ctrlOrderList.InsertColumn( nCol++, "�ֹ�����"  , LVCFMT_RIGHT, 70 );    // [double, 15.2] �ֹ�����        
	m_ctrlOrderList.InsertColumn( nCol++, "ü�����"  , LVCFMT_RIGHT, 70 );    // [long  ,   16] ü�����        
	m_ctrlOrderList.InsertColumn( nCol++, "ü�ᰡ "   , LVCFMT_RIGHT, 70 );    // [double, 15.2] ü�ᰡ          
	m_ctrlOrderList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlJangoList.InsertColumn( nCol++, "�����"  , LVCFMT_LEFT , 70 );    // [string,   40] �����              
	m_ctrlJangoList.InsertColumn( nCol++, "�ܰ����", LVCFMT_RIGHT, 70 );    // [long  ,   16] �ܰ����            
	m_ctrlJangoList.InsertColumn( nCol++, "��մܰ�", LVCFMT_LEFT , 70 );    // [double, 13.2] ��մܰ�            
	m_ctrlJangoList.InsertColumn( nCol++, "�򰡼���", LVCFMT_RIGHT, 70 );    // [long  ,   16] �򰡼���            
	m_ctrlJangoList.InsertColumn( nCol++, "������"  , LVCFMT_RIGHT, 70 );    // [double, 18.6] ������              
	m_ctrlJangoList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlLog.InsertColumn( nCol++, "�ð�", LVCFMT_LEFT,  60 );
	m_ctrlLog.InsertColumn( nCol++, "TR"  , LVCFMT_LEFT,  40 );
	m_ctrlLog.InsertColumn( nCol++, "Data", LVCFMT_LEFT, 420 );
	m_ctrlLog.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	nCol = 0;
	m_ctrlDetailLog.InsertColumn( nCol++, "�ʵ�", LVCFMT_LEFT, 110 );
	m_ctrlDetailLog.InsertColumn( nCol++, "����", LVCFMT_LEFT,  70 );
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
		MessageBox( strCode + " : " + strMsg, "�α��� ����", MB_ICONSTOP );
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


//�ֹ�TR ���� ����
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
		MessageBox( strMsg, "Ȯ��", MB_OK );
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


// �ֹ�ü�� �ǽð�
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
		strMsg += "�������ῡ �����Ͽ����ϴ�.\n";
		strMsg += "xingACE�� ���������� Ȯ���Ͻñ� �ٶ��ϴ�.";
		MessageBox( strMsg, "����", MB_ICONSTOP );
		return;
	}

	if( m_iXingAPI.Login( GetSafeHwnd(), strID, strPwd, strCert, 0 ) == FALSE )
	{
		CString strMsg;
		strMsg += "�α��� ���ۿ� �����Ͽ����ϴ�.\n";
		strMsg += "xingACE�� ���������� Ȯ���Ͻñ� �ٶ��ϴ�.";
		MessageBox( strMsg, "����", MB_ICONSTOP );
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


//�ؿܼ��� �ű��ֹ�
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
	SetPacketData(inBlock.RecCnt, sizeof(inBlock.RecCnt), this, 1, DATA_TYPE_LONG);    // [long  ] �ֹ�����     
	
	char zToday[9]; SYSTEMTIME st; GetLocalTime(&st);
	sprintf(zToday, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	
	SetPacketData(inBlock.OrdDt, sizeof(inBlock.OrdDt), zToday, DATA_TYPE_STRING);    // [string] ���¹�ȣ     
	
	sprintf(tmp, "%*.*s", sizeof(inBlock.BrnCode), sizeof(inBlock.BrnCode), " ");
	SetPacketData(inBlock.BrnCode, sizeof(inBlock.BrnCode), tmp, DATA_TYPE_STRING);    // [string] ���¹�ȣ     

	SetPacketData(inBlock.AcntNo, sizeof(inBlock.AcntNo), this, IDC_COMBO_ACCOUNT, DATA_TYPE_STRING);    // [string] ���¹�ȣ     
	SetPacketData(inBlock.Pwd, sizeof(inBlock.Pwd), this, IDC_EDIT_PWD, DATA_TYPE_STRING);    // [string] �Էº�й�ȣ 
	SetPacketData(inBlock.IsuCodeVal, sizeof(inBlock.IsuCodeVal), this, IDC_EDIT_CODE, DATA_TYPE_STRING);    // [string] �����ȣ     
	SetPacketData(inBlock.FutsOrdTpCode, sizeof(inBlock.FutsOrdTpCode), "1", DATA_TYPE_STRING);    // [string] �����ȣ     
	SetPacketData(inBlock.BnsTpCode, sizeof(inBlock.BnsTpCode), strMeme, DATA_TYPE_STRING);    // [string] �Ÿű���     

	// 1:���尡, 2:������
	SetPacketData(inBlock.AbrdFutsOrdPtnCode, sizeof(inBlock.AbrdFutsOrdPtnCode), "2", DATA_TYPE_STRING);    // [string] ���¹�ȣ     

	sprintf(tmp, "%*.*s", sizeof(inBlock.CrcyCode), sizeof(inBlock.CrcyCode), " ");
	SetPacketData(inBlock.CrcyCode, sizeof(inBlock.CrcyCode), tmp, DATA_TYPE_STRING);    // [string] ���¹�ȣ     
	
	SetPacketData(inBlock.OvrsDrvtOrdPrc, sizeof(inBlock.OvrsDrvtOrdPrc), this, IDC_EDIT_PRICE, DATA_TYPE_DOT, 8);    // [double] �ֹ���       
	//TODO SetPacketData(inBlock.CndiOrdPrc, sizeof(inBlock.CndiOrdPrc), 0, DATA_TYPE_DOT, 8);    // [double] �ֹ���       

	SetPacketData(inBlock.OrdQty, sizeof(inBlock.OrdQty), this, IDC_EDIT_QTY, DATA_TYPE_LONG);    // [long  ] �ֹ�����     
	
	// ��ǰ�ڵ�
	sprintf(tmp, "%*.*s", sizeof(inBlock.PrdtCode), sizeof(inBlock.PrdtCode), " ");
	SetPacketData(inBlock.PrdtCode, sizeof(inBlock.PrdtCode), tmp, DATA_TYPE_STRING);    // [string] ���¹�ȣ     

	//������
	sprintf(tmp, "%*.*s", sizeof(inBlock.DueYymm), sizeof(inBlock.DueYymm), " ");
	SetPacketData(inBlock.DueYymm, sizeof(inBlock.DueYymm), tmp, DATA_TYPE_STRING);    // [string] ���¹�ȣ     
	
	//�ŷ����ڵ�
	sprintf(tmp, "%*.*s", sizeof(inBlock.ExchCode), sizeof(inBlock.ExchCode), " ");
	SetPacketData(inBlock.ExchCode, sizeof(inBlock.ExchCode), tmp, DATA_TYPE_STRING);    // [string] ���¹�ȣ     

	int nReqID = m_iXingAPI.Request(GetSafeHwnd(), "CIDBT00100", &inBlock, sizeof(inBlock));
	if (nReqID < 0)
	{
		CString strMsg;
		strMsg += "������ ���ۿ� �����Ͽ����ϴ�.";
		MessageBox(strMsg, "���ۿ���", MB_ICONSTOP);
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
	SetPacketData( inBlock.AcntNo       , sizeof( inBlock.AcntNo        ), this, IDC_COMBO_ACCOUNT, DATA_TYPE_STRING    );    // [string] ���¹�ȣ     
	SetPacketData( inBlock.InptPwd      , sizeof( inBlock.InptPwd       ), this, IDC_EDIT_PWD     , DATA_TYPE_STRING    );    // [string] �Էº�й�ȣ 
	SetPacketData( inBlock.IsuNo        , sizeof( inBlock.IsuNo         ), this, IDC_EDIT_CODE    , DATA_TYPE_STRING    );    // [string] �����ȣ     
	SetPacketData( inBlock.OrdQty       , sizeof( inBlock.OrdQty        ), this, IDC_EDIT_QTY     , DATA_TYPE_LONG      );    // [long  ] �ֹ�����     
	SetPacketData( inBlock.OrdPrc       , sizeof( inBlock.OrdPrc        ), this, IDC_EDIT_PRICE   , DATA_TYPE_DOT   , 2 );    // [double] �ֹ���       
	SetPacketData( inBlock.BnsTpCode    , sizeof( inBlock.BnsTpCode     ), strMeme                , DATA_TYPE_STRING    );    // [string] �Ÿű���     
	SetPacketData( inBlock.OrdprcPtnCode, sizeof( inBlock.OrdprcPtnCode ), "00"                   , DATA_TYPE_STRING    );    // [string] ȣ�������ڵ� 
	SetPacketData( inBlock.MgntrnCode   , sizeof( inBlock.MgntrnCode    ), "000"                  , DATA_TYPE_STRING    );    // [string] �ſ�ŷ��ڵ� 
	SetPacketData( inBlock.LoanDt       , sizeof( inBlock.LoanDt        ), "        "             , DATA_TYPE_STRING    );    // [string] ������       
	SetPacketData( inBlock.OrdCndiTpCode, sizeof( inBlock.OrdCndiTpCode ), "0"                    , DATA_TYPE_STRING    );    // [string] �ֹ����Ǳ��� 

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAT00600", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "������ ���ۿ� �����Ͽ����ϴ�.";
		MessageBox( strMsg, "���ۿ���", MB_ICONSTOP );
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
	SetPacketData( inBlock.OrgOrdNo     , sizeof( inBlock.OrgOrdNo      ), this, IDC_EDIT_ORG_ORDER_NO, DATA_TYPE_LONG      );    // [long  ] ���ֹ���ȣ
	SetPacketData( inBlock.AcntNo       , sizeof( inBlock.AcntNo        ), this, IDC_COMBO_ACCOUNT    , DATA_TYPE_STRING    );    // [string] ���¹�ȣ     
	SetPacketData( inBlock.InptPwd      , sizeof( inBlock.InptPwd       ), this, IDC_EDIT_PWD         , DATA_TYPE_STRING    );    // [string] �Էº�й�ȣ 
	SetPacketData( inBlock.IsuNo        , sizeof( inBlock.IsuNo         ), this, IDC_EDIT_CODE        , DATA_TYPE_STRING    );    // [string] �����ȣ     
	SetPacketData( inBlock.OrdQty       , sizeof( inBlock.OrdQty        ), this, IDC_EDIT_QTY         , DATA_TYPE_LONG      );    // [long  ] �ֹ�����     
	SetPacketData( inBlock.OrdprcPtnCode, sizeof( inBlock.OrdprcPtnCode ), "00"                       , DATA_TYPE_STRING    );    // [string] ȣ�������ڵ�       
	SetPacketData( inBlock.OrdCndiTpCode, sizeof( inBlock.OrdCndiTpCode ), "0"                        , DATA_TYPE_STRING    );    // [string] �ֹ����Ǳ���       
	SetPacketData( inBlock.OrdPrc       , sizeof( inBlock.OrdPrc        ), this, IDC_EDIT_PRICE       , DATA_TYPE_DOT,    2 );    // [double] �ֹ���       

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAT00700", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "������ ���ۿ� �����Ͽ����ϴ�.";
		MessageBox( strMsg, "���ۿ���", MB_ICONSTOP );
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
	SetPacketData( inBlock.OrgOrdNo     , sizeof( inBlock.OrgOrdNo      ), this, IDC_EDIT_ORG_ORDER_NO, DATA_TYPE_LONG   );    // [long  ] ���ֹ���ȣ
	SetPacketData( inBlock.AcntNo       , sizeof( inBlock.AcntNo        ), this, IDC_COMBO_ACCOUNT    , DATA_TYPE_STRING );    // [string] ���¹�ȣ     
	SetPacketData( inBlock.InptPwd      , sizeof( inBlock.InptPwd       ), this, IDC_EDIT_PWD         , DATA_TYPE_STRING );    // [string] �Էº�й�ȣ 
	SetPacketData( inBlock.IsuNo        , sizeof( inBlock.IsuNo         ), this, IDC_EDIT_CODE        , DATA_TYPE_STRING );    // [string] �����ȣ     
	SetPacketData( inBlock.OrdQty       , sizeof( inBlock.OrdQty        ), this, IDC_EDIT_QTY         , DATA_TYPE_LONG   );    // [long  ] �ֹ�����     

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAT00800", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "������ ���ۿ� �����Ͽ����ϴ�.";
		MessageBox( strMsg, "���ۿ���", MB_ICONSTOP );
	}
}

void CSample_xingACEDlg::RequestCSPAQ03700()
{
	CString strDate = CTime::GetCurrentTime().Format( "%Y%m%d" );

	CSPAQ03700InBlock1 inBlock;
	FillMemory( &inBlock, sizeof( inBlock ), ' ' );
	SetPacketData( inBlock.RecCnt     , sizeof( inBlock.RecCnt      ), "1"                     , DATA_TYPE_LONG   );    // [long  ,    5] ���ڵ尹��   
	SetPacketData( inBlock.AcntNo     , sizeof( inBlock.AcntNo      ), this, IDC_COMBO_ACCOUNT , DATA_TYPE_STRING );    // [string,   20] ���¹�ȣ     
	SetPacketData( inBlock.InptPwd    , sizeof( inBlock.InptPwd     ), this, IDC_EDIT_PWD      , DATA_TYPE_STRING );    // [string,    8] �Էº�й�ȣ 
	SetPacketData( inBlock.OrdMktCode , sizeof( inBlock.OrdMktCode  ), "00"                    , DATA_TYPE_STRING );    // [string,    2] �ֹ������ڵ� 
	SetPacketData( inBlock.BnsTpCode  , sizeof( inBlock.BnsTpCode   ), "0"                     , DATA_TYPE_STRING );    // [string,    1] �Ÿű����ڵ� 
	SetPacketData( inBlock.IsuNo      , sizeof( inBlock.IsuNo       ), ""                      , DATA_TYPE_STRING );    // [string,   12] �����ȣ     
	SetPacketData( inBlock.ExecYn     , sizeof( inBlock.ExecYn      ), "0"                     , DATA_TYPE_STRING );    // [string,    1] ü�Ῡ��     
	SetPacketData( inBlock.OrdDt      , sizeof( inBlock.OrdDt       ), strDate                 , DATA_TYPE_STRING );    // [string,    8] �ֹ���       
	SetPacketData( inBlock.SrtOrdNo2  , sizeof( inBlock.SrtOrdNo2   ), "9999999999"            , DATA_TYPE_LONG   );    // [long  ,   10] �����ֹ���ȣ2
	SetPacketData( inBlock.BkseqTpCode, sizeof( inBlock.BkseqTpCode ), "0"                     , DATA_TYPE_STRING );    // [string,    1] ��������     
	SetPacketData( inBlock.OrdPtnCode , sizeof( inBlock.OrdPtnCode  ), "00"                    , DATA_TYPE_STRING );    // [string,    2] �ֹ������ڵ� 

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAQ03700", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "������ ���ۿ� �����Ͽ����ϴ�.";
		MessageBox( strMsg, "���ۿ���", MB_ICONSTOP );
	}
}

void CSample_xingACEDlg::RequestCSPAQ02300()
{
	CSPAQ02300InBlock1 inBlock;
	FillMemory( &inBlock, sizeof( inBlock ), ' ' );
	SetPacketData( inBlock.RecCnt        , sizeof( inBlock.RecCnt         ), "1"                    , DATA_TYPE_LONG   );    // [long  ,    5] ���ڵ尹��        
	SetPacketData( inBlock.AcntNo        , sizeof( inBlock.AcntNo         ), this, IDC_COMBO_ACCOUNT, DATA_TYPE_STRING );    // [string,   20] ���¹�ȣ           
	SetPacketData( inBlock.Pwd           , sizeof( inBlock.Pwd            ), this, IDC_EDIT_PWD     , DATA_TYPE_STRING );    // [string,    8] ��й�ȣ           
	SetPacketData( inBlock.BalCreTp      , sizeof( inBlock.BalCreTp       ), "0"                    , DATA_TYPE_STRING );    // [string,    1] �ܰ��������       
	SetPacketData( inBlock.CmsnAppTpCode , sizeof( inBlock.CmsnAppTpCode  ), "0"                    , DATA_TYPE_STRING );    // [string,    1] ���������뱸��     
	SetPacketData( inBlock.D2balBaseQryTp, sizeof( inBlock.D2balBaseQryTp ), "1"                    , DATA_TYPE_STRING );    // [string,    1] D2�ܰ������ȸ����
	SetPacketData( inBlock.UprcTpCode    , sizeof( inBlock.UprcTpCode     ), "0"                    , DATA_TYPE_STRING );    // [string,    1] �ܰ�����           

	int nReqID = m_iXingAPI.Request( GetSafeHwnd(), "CSPAQ02300", &inBlock, sizeof( inBlock ) );
	if( nReqID < 0 )
	{
		CString strMsg;
		strMsg += "������ ���ۿ� �����Ͽ����ϴ�.";
		MessageBox( strMsg, "���ۿ���", MB_ICONSTOP );
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
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrdNo   , sizeof( pOutBlock3->OrdNo    ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   10] �ֹ���ȣ        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrgOrdNo, sizeof( pOutBlock3->OrgOrdNo ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   10] ���ֹ���ȣ      
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->IsuNm   , sizeof( pOutBlock3->IsuNm    ), DATA_TYPE_STRING, 0 ) );    // [string,   40] �����          
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->BnsTpNm , sizeof( pOutBlock3->BnsTpNm  ), DATA_TYPE_STRING, 0 ) );    // [string,   10] �Ÿű��и�      
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->MrcTpNm , sizeof( pOutBlock3->MrcTpNm  ), DATA_TYPE_STRING, 0 ) );    // [string,   10] ������ұ��и�  
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrdQty  , sizeof( pOutBlock3->OrdQty   ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] �ֹ�����        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->OrdPrc  , sizeof( pOutBlock3->OrdPrc   ), DATA_TYPE_DOUBLE, 2 ) );    // [double, 15.2] �ֹ�����        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->ExecQty , sizeof( pOutBlock3->ExecQty  ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] ü�����        
		m_ctrlOrderList.SetItemText( i, nCol++, GetDispData( pOutBlock3->ExecPrc , sizeof( pOutBlock3->ExecPrc  ), DATA_TYPE_DOUBLE, 2 ) );    // [double, 15.2] ü�ᰡ          
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
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->IsuNm  , sizeof( pOutBlock3->IsuNm   ), DATA_TYPE_STRING, 0 ) );    // [string,   40] �����              
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->BalQty , sizeof( pOutBlock3->BalQty  ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] �ܰ����            
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->AvrUprc, sizeof( pOutBlock3->AvrUprc ), DATA_TYPE_DOUBLE, 2 ) );    // [double, 13.2] ��մܰ�            
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->EvalPnl, sizeof( pOutBlock3->EvalPnl ), DATA_TYPE_LONG  , 0 ) );    // [long  ,   16] �򰡼���            
		m_ctrlJangoList.SetItemText( i, nCol++, GetDispData( pOutBlock3->PnlRat , sizeof( pOutBlock3->PnlRat  ), DATA_TYPE_DOUBLE, 6 ) );    // [double, 18.6] ������              
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

	SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->lineseq        , sizeof( pOutBlock->lineseq         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno          , sizeof( pOutBlock->accno           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "������ID            ", pOutBlock->user           , sizeof( pOutBlock->user            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�������            ", pOutBlock->len            , sizeof( pOutBlock->len             ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�������            ", pOutBlock->gubun          , sizeof( pOutBlock->gubun           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���౸��            ", pOutBlock->compress       , sizeof( pOutBlock->compress        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "��ȣ����            ", pOutBlock->encrypt        , sizeof( pOutBlock->encrypt         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����������        ", pOutBlock->offset         , sizeof( pOutBlock->offset          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "TRCODE              ", pOutBlock->trcode         , sizeof( pOutBlock->trcode          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�̿���ȣ          ", pOutBlock->compid         , sizeof( pOutBlock->compid          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����ID            ", pOutBlock->userid         , sizeof( pOutBlock->userid          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���Ӹ�ü            ", pOutBlock->media          , sizeof( pOutBlock->media           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "I/F�Ϸù�ȣ         ", pOutBlock->ifid           , sizeof( pOutBlock->ifid            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->seq            , sizeof( pOutBlock->seq             ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "TR����ID            ", pOutBlock->trid           , sizeof( pOutBlock->trid            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "����IP              ", pOutBlock->pubip          , sizeof( pOutBlock->pubip           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�缳IP              ", pOutBlock->prvip          , sizeof( pOutBlock->prvip           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "ó��������ȣ        ", pOutBlock->pcbpno         , sizeof( pOutBlock->pcbpno          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "������ȣ            ", pOutBlock->bpno           , sizeof( pOutBlock->bpno            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ܸ���ȣ            ", pOutBlock->termno         , sizeof( pOutBlock->termno          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "����            ", pOutBlock->lang           , sizeof( pOutBlock->lang            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "APó���ð�          ", pOutBlock->proctm         , sizeof( pOutBlock->proctm          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�޼����ڵ�          ", pOutBlock->msgcode        , sizeof( pOutBlock->msgcode         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�޼�����±���      ", pOutBlock->outgu          , sizeof( pOutBlock->outgu           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����û����        ", pOutBlock->compreq        , sizeof( pOutBlock->compreq         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���Ű              ", pOutBlock->funckey        , sizeof( pOutBlock->funckey         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "��û���ڵ尳��      ", pOutBlock->reqcnt         , sizeof( pOutBlock->reqcnt          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���񿵿�            ", pOutBlock->filler         , sizeof( pOutBlock->filler          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���ӱ���            ", pOutBlock->cont           , sizeof( pOutBlock->cont            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "����Ű��            ", pOutBlock->contkey        , sizeof( pOutBlock->contkey         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����ý��۱���      ", pOutBlock->varlen         , sizeof( pOutBlock->varlen          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����ش�����        ", pOutBlock->varhdlen       , sizeof( pOutBlock->varhdlen        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����޽�������      ", pOutBlock->varmsglen      , sizeof( pOutBlock->varmsglen       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��ȸ�߿���          ", pOutBlock->trsrc          , sizeof( pOutBlock->trsrc           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "I/F�̺�ƮID         ", pOutBlock->eventid        , sizeof( pOutBlock->eventid         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "I/F����             ", pOutBlock->ifinfo         , sizeof( pOutBlock->ifinfo          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���񿵿�            ", pOutBlock->filler1        , sizeof( pOutBlock->filler1         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ֹ�ü�ᱸ��        ", pOutBlock->ordchegb       , sizeof( pOutBlock->ordchegb        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���屸��            ", pOutBlock->marketgb       , sizeof( pOutBlock->marketgb        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ֹ�����            ", pOutBlock->ordgb          , sizeof( pOutBlock->ordgb           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���ֹ���ȣ          ", pOutBlock->orgordno       , sizeof( pOutBlock->orgordno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno1         , sizeof( pOutBlock->accno1          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno2         , sizeof( pOutBlock->accno2          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "��й�ȣ            ", pOutBlock->passwd         , sizeof( pOutBlock->passwd          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����ȣ            ", pOutBlock->expcode        , sizeof( pOutBlock->expcode         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���������ȣ        ", pOutBlock->shtcode        , sizeof( pOutBlock->shtcode         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����              ", pOutBlock->hname          , sizeof( pOutBlock->hname           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ֹ�����            ", pOutBlock->ordqty         , sizeof( pOutBlock->ordqty          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ�����            ", pOutBlock->ordprice       , sizeof( pOutBlock->ordprice        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ�����            ", pOutBlock->hogagb         , sizeof( pOutBlock->hogagb          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "ȣ�������ڵ�        ", pOutBlock->etfhogagb      , sizeof( pOutBlock->etfhogagb       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���α׷�ȣ������    ", pOutBlock->pgmtype        , sizeof( pOutBlock->pgmtype         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���ŵ�ȣ������      ", pOutBlock->gmhogagb       , sizeof( pOutBlock->gmhogagb        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���ŵ����ɿ���      ", pOutBlock->gmhogayn       , sizeof( pOutBlock->gmhogayn        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ſ뱸��            ", pOutBlock->singb          , sizeof( pOutBlock->singb           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "������              ", pOutBlock->loandt         , sizeof( pOutBlock->loandt          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ݴ�Ÿ��ֹ�����    ", pOutBlock->cvrgordtp      , sizeof( pOutBlock->cvrgordtp       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����ڵ�            ", pOutBlock->strtgcode      , sizeof( pOutBlock->strtgcode       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�׷�ID              ", pOutBlock->groupid        , sizeof( pOutBlock->groupid         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ֹ�ȸ��            ", pOutBlock->ordseqno       , sizeof( pOutBlock->ordseqno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��Ʈ��������ȣ      ", pOutBlock->prtno          , sizeof( pOutBlock->prtno           ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ٽ��Ϲ�ȣ          ", pOutBlock->basketno       , sizeof( pOutBlock->basketno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "Ʈ��ġ��ȣ          ", pOutBlock->trchno         , sizeof( pOutBlock->trchno          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ƾ��۹�ȣ          ", pOutBlock->itemno         , sizeof( pOutBlock->itemno          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���Ա���            ", pOutBlock->brwmgmyn       , sizeof( pOutBlock->brwmgmyn        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "ȸ�����ȣ          ", pOutBlock->mbrno          , sizeof( pOutBlock->mbrno           ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "ó������            ", pOutBlock->procgb         , sizeof( pOutBlock->procgb          ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "����������ȣ        ", pOutBlock->admbrchno      , sizeof( pOutBlock->admbrchno       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�������¹�ȣ        ", pOutBlock->futaccno       , sizeof( pOutBlock->futaccno        ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "������ǰ����        ", pOutBlock->futmarketgb    , sizeof( pOutBlock->futmarketgb     ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "��Ÿ�ü����        ", pOutBlock->tongsingb      , sizeof( pOutBlock->tongsingb       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����������ڱ���    ", pOutBlock->lpgb           , sizeof( pOutBlock->lpgb            ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "DUMMY               ", pOutBlock->dummy          , sizeof( pOutBlock->dummy           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ֹ���ȣ            ", pOutBlock->ordno          , sizeof( pOutBlock->ordno           ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ��ð�            ", pOutBlock->ordtm          , sizeof( pOutBlock->ordtm           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���ֹ���ȣ          ", pOutBlock->prntordno      , sizeof( pOutBlock->prntordno       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���������ȣ        ", pOutBlock->mgempno        , sizeof( pOutBlock->mgempno         ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "���ֹ���ü�����    ", pOutBlock->orgordundrqty  , sizeof( pOutBlock->orgordundrqty   ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���ֹ���������      ", pOutBlock->orgordmdfyqty  , sizeof( pOutBlock->orgordmdfyqty   ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���ֹ���Ҽ���      ", pOutBlock->ordordcancelqty, sizeof( pOutBlock->ordordcancelqty ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��ȸ����۽Ź�ȣ    ", pOutBlock->nmcpysndno     , sizeof( pOutBlock->nmcpysndno      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ��ݾ�            ", pOutBlock->ordamt         , sizeof( pOutBlock->ordamt          ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�Ÿű���            ", pOutBlock->bnstp          , sizeof( pOutBlock->bnstp           ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�����ֹ���ȣ        ", pOutBlock->spareordno     , sizeof( pOutBlock->spareordno      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ݴ�Ÿ��Ϸù�ȣ    ", pOutBlock->cvrgseqno      , sizeof( pOutBlock->cvrgseqno       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����ֹ���ȣ        ", pOutBlock->rsvordno       , sizeof( pOutBlock->rsvordno        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����ֹ��Ϸù�ȣ    ", pOutBlock->mtordseqno     , sizeof( pOutBlock->mtordseqno      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����ֹ�����        ", pOutBlock->spareordqty    , sizeof( pOutBlock->spareordqty     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ������ȣ        ", pOutBlock->orduserid      , sizeof( pOutBlock->orduserid       ), DATA_TYPE_STRING, 0 );
	SetDetailLogData( "�ǹ��ֹ�����        ", pOutBlock->spotordqty     , sizeof( pOutBlock->spotordqty      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����ֹ�����      ", pOutBlock->ordruseqty     , sizeof( pOutBlock->ordruseqty      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����ֹ��ݾ�        ", pOutBlock->mnyordamt      , sizeof( pOutBlock->mnyordamt       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ����ݾ�        ", pOutBlock->ordsubstamt    , sizeof( pOutBlock->ordsubstamt     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�����ֹ��ݾ�      ", pOutBlock->ruseordamt     , sizeof( pOutBlock->ruseordamt      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�������ֹ��ݾ�      ", pOutBlock->ordcmsnamt     , sizeof( pOutBlock->ordcmsnamt      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���ſ�㺸�����", pOutBlock->crdtuseamt     , sizeof( pOutBlock->crdtuseamt      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ܰ����            ", pOutBlock->secbalqty      , sizeof( pOutBlock->secbalqty       ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ǹ����ɼ���        ", pOutBlock->spotordableqty , sizeof( pOutBlock->spotordableqty  ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���밡�ɼ���(�ŵ�)", pOutBlock->ordableruseqty , sizeof( pOutBlock->ordableruseqty  ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��������            ", pOutBlock->flctqty        , sizeof( pOutBlock->flctqty         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ܰ����(D2)        ", pOutBlock->secbalqtyd2    , sizeof( pOutBlock->secbalqtyd2     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ŵ��ֹ����ɼ���    ", pOutBlock->sellableqty    , sizeof( pOutBlock->sellableqty     ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��ü��ŵ��ֹ�����  ", pOutBlock->unercsellordqty, sizeof( pOutBlock->unercsellordqty ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��ո��԰�          ", pOutBlock->avrpchsprc     , sizeof( pOutBlock->avrpchsprc      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���Աݾ�            ", pOutBlock->pchsamt        , sizeof( pOutBlock->pchsamt         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "������              ", pOutBlock->deposit        , sizeof( pOutBlock->deposit         ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "����              ", pOutBlock->substamt       , sizeof( pOutBlock->substamt        ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��Ź���ű�����      ", pOutBlock->csgnmnymgn     , sizeof( pOutBlock->csgnmnymgn      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "��Ź���űݴ��      ", pOutBlock->csgnsubstmgn   , sizeof( pOutBlock->csgnsubstmgn    ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ſ�㺸�����    ", pOutBlock->crdtpldgruseamt, sizeof( pOutBlock->crdtpldgruseamt ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ���������        ", pOutBlock->ordablemny     , sizeof( pOutBlock->ordablemny      ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "�ֹ����ɴ��        ", pOutBlock->ordablesubstamt, sizeof( pOutBlock->ordablesubstamt ), DATA_TYPE_LONG  , 0 );
	SetDetailLogData( "���밡�ɱݾ�      ", pOutBlock->ruseableamt    , sizeof( pOutBlock->ruseableamt     ), DATA_TYPE_LONG  , 0 );*/
}

void CSample_xingACEDlg::DetailLogSC1( LPCTSTR pszData )
{
	//LPSC1_OutBlock pOutBlock = (LPSC1_OutBlock)pszData;

	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�������            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���౸��            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ȣ����            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�̿���ȣ          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ӹ�ü            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�Ϸù�ȣ         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR����ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�缳IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ó��������ȣ        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ȣ            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܸ���ȣ            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "APó���ð�          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�޼����ڵ�          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�޼�����±���      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����û����        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ű              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��û���ڵ尳��      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���ӱ���            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����Ű��            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ý��۱���      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ش�����        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����޽�������      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ȸ�߿���          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�̺�ƮID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F����             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ü�������ڵ�    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¸�              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ȣ            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ���ȣ            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ȣ          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü���ȣ            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�����            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�ᰡ��            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�μ���        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�ΰ���        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Ȯ�μ���        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�źμ���            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�ó�������ڵ�    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ֹ��Ϸù�ȣ    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȣ�������ڵ�        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ü�����      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ù�ȣ        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ݴ�Ÿ��ֹ�����    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ü�����(�ֹ�)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ü�����    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���������      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���Ҽ���      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ü�ᰡ��    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ��ݾ�            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ǥ�������ȣ        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ǥ�������ȣ      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�Ÿű���            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ��ŷ������ڵ�    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ſ�ŷ��ڵ�        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������ջ��ڵ�      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ÿ�ü�ڵ�        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȸ��/��ȸ�����ȣ   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����¹�ȣ        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��������������ȣ    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������¹�ȣ    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������屸��        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ͻ����ڵ�        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������űݷ�        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������űݷ�        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����ü��ݾ�        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ü��ݾ�        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������ü��ݾ�      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸ü��ݾ�    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�ü��ݾ�        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ�ü�����        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ŵ�ü�����      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ڵ�            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�׷�Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ȸ��            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ʈ��������ȣ      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ٽ��Ϲ�ȣ          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "Ʈ��ġ��ȣ          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����۹�ȣ          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���԰�������        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܱ��ΰ�����ȣ      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ���¡������      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������ڱ���    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ü��ð�            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ��Ҽ���ü��ð�  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܿ�����ݾ�        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ����ɼ���        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɼ���(�ŵ�)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ��ֹ����ɼ���    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ü��ŵ��ֹ�����  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ո��԰�          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Աݾ�            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���ű�����      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���űݴ��      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸�����    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���������        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ɴ��        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɱݾ�      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogSC2( LPCTSTR pszData )
{
	//LPSC2_OutBlock pOutBlock = (LPSC2_OutBlock)pszData;

	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�������            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���౸��            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ȣ����            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�̿���ȣ          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ӹ�ü            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�Ϸù�ȣ         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR����ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�缳IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ó��������ȣ        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ȣ            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܸ���ȣ            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "APó���ð�          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�޼����ڵ�          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�޼�����±���      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����û����        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ű              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��û���ڵ尳��      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���ӱ���            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����Ű��            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ý��۱���      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ش�����        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����޽�������      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ȸ�߿���          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�̺�ƮID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F����             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ü�������ڵ�    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¸�              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ȣ            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ���ȣ            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ȣ          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü���ȣ            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�����            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�ᰡ��            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�μ���        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�ΰ���        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Ȯ�μ���        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�źμ���            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�ó�������ڵ�    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ֹ��Ϸù�ȣ    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȣ�������ڵ�        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ü�����      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ù�ȣ        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ݴ�Ÿ��ֹ�����    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ü�����(�ֹ�)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ü�����    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���������      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���Ҽ���      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ü�ᰡ��    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ��ݾ�            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ǥ�������ȣ        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ǥ�������ȣ      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�Ÿű���            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ��ŷ������ڵ�    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ſ�ŷ��ڵ�        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������ջ��ڵ�      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ÿ�ü�ڵ�        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȸ��/��ȸ�����ȣ   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����¹�ȣ        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��������������ȣ    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������¹�ȣ    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������屸��        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ͻ����ڵ�        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������űݷ�        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������űݷ�        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����ü��ݾ�        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ü��ݾ�        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������ü��ݾ�      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸ü��ݾ�    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�ü��ݾ�        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ�ü�����        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ŵ�ü�����      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ڵ�            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�׷�Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ȸ��            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ʈ��������ȣ      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ٽ��Ϲ�ȣ          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "Ʈ��ġ��ȣ          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����۹�ȣ          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���԰�������        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܱ��ΰ�����ȣ      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ���¡������      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������ڱ���    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ü��ð�            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ��Ҽ���ü��ð�  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܿ�����ݾ�        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ����ɼ���        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɼ���(�ŵ�)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ��ֹ����ɼ���    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ü��ŵ��ֹ�����  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ո��԰�          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Աݾ�            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���ű�����      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���űݴ��      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸�����    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���������        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ɴ��        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɱݾ�      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogSC3( LPCTSTR pszData )
{
	//LPSC3_OutBlock pOutBlock = (LPSC3_OutBlock)pszData;

	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�������            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���౸��            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ȣ����            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�̿���ȣ          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ӹ�ü            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�Ϸù�ȣ         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR����ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�缳IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ó��������ȣ        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ȣ            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܸ���ȣ            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "APó���ð�          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�޼����ڵ�          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�޼�����±���      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����û����        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ű              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��û���ڵ尳��      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���ӱ���            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����Ű��            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ý��۱���      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ش�����        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����޽�������      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ȸ�߿���          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�̺�ƮID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F����             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ü�������ڵ�    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¸�              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ȣ            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ���ȣ            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ȣ          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü���ȣ            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�����            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�ᰡ��            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�μ���        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�ΰ���        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Ȯ�μ���        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�źμ���            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�ó�������ڵ�    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ֹ��Ϸù�ȣ    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȣ�������ڵ�        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ü�����      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ù�ȣ        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ݴ�Ÿ��ֹ�����    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ü�����(�ֹ�)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ü�����    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���������      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���Ҽ���      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ü�ᰡ��    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ��ݾ�            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ǥ�������ȣ        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ǥ�������ȣ      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�Ÿű���            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ��ŷ������ڵ�    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ſ�ŷ��ڵ�        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������ջ��ڵ�      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ÿ�ü�ڵ�        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȸ��/��ȸ�����ȣ   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����¹�ȣ        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��������������ȣ    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������¹�ȣ    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������屸��        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ͻ����ڵ�        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������űݷ�        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������űݷ�        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����ü��ݾ�        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ü��ݾ�        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������ü��ݾ�      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸ü��ݾ�    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�ü��ݾ�        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ�ü�����        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ŵ�ü�����      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ڵ�            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�׷�Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ȸ��            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ʈ��������ȣ      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ٽ��Ϲ�ȣ          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "Ʈ��ġ��ȣ          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����۹�ȣ          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���԰�������        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܱ��ΰ�����ȣ      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ���¡������      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������ڱ���    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ü��ð�            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ��Ҽ���ü��ð�  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܿ�����ݾ�        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ����ɼ���        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɼ���(�ŵ�)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ��ֹ����ɼ���    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ü��ŵ��ֹ�����  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ո��԰�          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Աݾ�            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���ű�����      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���űݴ��      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸�����    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���������        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ɴ��        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɱݾ�      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogSC4( LPCTSTR pszData )
{
	//LPSC4_OutBlock pOutBlock = (LPSC4_OutBlock)pszData;

	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->lineseq         , sizeof( pOutBlock->lineseq          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno           , sizeof( pOutBlock->accno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ID            ", pOutBlock->user            , sizeof( pOutBlock->user             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������            ", pOutBlock->len             , sizeof( pOutBlock->len              ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�������            ", pOutBlock->gubun           , sizeof( pOutBlock->gubun            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���౸��            ", pOutBlock->compress        , sizeof( pOutBlock->compress         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ȣ����            ", pOutBlock->encrypt         , sizeof( pOutBlock->encrypt          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������        ", pOutBlock->offset          , sizeof( pOutBlock->offset           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "TRCODE              ", pOutBlock->trcode          , sizeof( pOutBlock->trcode           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�̿���ȣ          ", pOutBlock->compid          , sizeof( pOutBlock->compid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ID            ", pOutBlock->userid          , sizeof( pOutBlock->userid           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ӹ�ü            ", pOutBlock->media           , sizeof( pOutBlock->media            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�Ϸù�ȣ         ", pOutBlock->ifid            , sizeof( pOutBlock->ifid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����Ϸù�ȣ        ", pOutBlock->seq             , sizeof( pOutBlock->seq              ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "TR����ID            ", pOutBlock->trid            , sizeof( pOutBlock->trid             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����IP              ", pOutBlock->pubip           , sizeof( pOutBlock->pubip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�缳IP              ", pOutBlock->prvip           , sizeof( pOutBlock->prvip            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ó��������ȣ        ", pOutBlock->pcbpno          , sizeof( pOutBlock->pcbpno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ȣ            ", pOutBlock->bpno            , sizeof( pOutBlock->bpno             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܸ���ȣ            ", pOutBlock->termno          , sizeof( pOutBlock->termno           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����            ", pOutBlock->lang            , sizeof( pOutBlock->lang             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "APó���ð�          ", pOutBlock->proctm          , sizeof( pOutBlock->proctm           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�޼����ڵ�          ", pOutBlock->msgcode         , sizeof( pOutBlock->msgcode          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�޼�����±���      ", pOutBlock->outgu           , sizeof( pOutBlock->outgu            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����û����        ", pOutBlock->compreq         , sizeof( pOutBlock->compreq          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���Ű              ", pOutBlock->funckey         , sizeof( pOutBlock->funckey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��û���ڵ尳��      ", pOutBlock->reqcnt          , sizeof( pOutBlock->reqcnt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler          , sizeof( pOutBlock->filler           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���ӱ���            ", pOutBlock->cont            , sizeof( pOutBlock->cont             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����Ű��            ", pOutBlock->contkey         , sizeof( pOutBlock->contkey          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ý��۱���      ", pOutBlock->varlen          , sizeof( pOutBlock->varlen           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ش�����        ", pOutBlock->varhdlen        , sizeof( pOutBlock->varhdlen         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����޽�������      ", pOutBlock->varmsglen       , sizeof( pOutBlock->varmsglen        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ȸ�߿���          ", pOutBlock->trsrc           , sizeof( pOutBlock->trsrc            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F�̺�ƮID         ", pOutBlock->eventid         , sizeof( pOutBlock->eventid          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "I/F����             ", pOutBlock->ifinfo          , sizeof( pOutBlock->ifinfo           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���񿵿�            ", pOutBlock->filler1         , sizeof( pOutBlock->filler1          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ü�������ڵ�    ", pOutBlock->ordxctptncode   , sizeof( pOutBlock->ordxctptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordmktcode      , sizeof( pOutBlock->ordmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ������ڵ�        ", pOutBlock->ordptncode      , sizeof( pOutBlock->ordptncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->mgmtbrnno       , sizeof( pOutBlock->mgmtbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno1          , sizeof( pOutBlock->accno1           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¹�ȣ            ", pOutBlock->accno2          , sizeof( pOutBlock->accno2           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���¸�              ", pOutBlock->acntnm          , sizeof( pOutBlock->acntnm           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ȣ            ", pOutBlock->Isuno           , sizeof( pOutBlock->Isuno            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����              ", pOutBlock->Isunm           , sizeof( pOutBlock->Isunm            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ���ȣ            ", pOutBlock->ordno           , sizeof( pOutBlock->ordno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ȣ          ", pOutBlock->orgordno        , sizeof( pOutBlock->orgordno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü���ȣ            ", pOutBlock->execno          , sizeof( pOutBlock->execno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordqty          , sizeof( pOutBlock->ordqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordprc          , sizeof( pOutBlock->ordprc           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�����            ", pOutBlock->execqty         , sizeof( pOutBlock->execqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�ᰡ��            ", pOutBlock->execprc         , sizeof( pOutBlock->execprc          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�μ���        ", pOutBlock->mdfycnfqty      , sizeof( pOutBlock->mdfycnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����Ȯ�ΰ���        ", pOutBlock->mdfycnfprc      , sizeof( pOutBlock->mdfycnfprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Ȯ�μ���        ", pOutBlock->canccnfqty      , sizeof( pOutBlock->canccnfqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�źμ���            ", pOutBlock->rjtqty          , sizeof( pOutBlock->rjtqty           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�ó�������ڵ�    ", pOutBlock->ordtrxptncode   , sizeof( pOutBlock->ordtrxptncode    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ֹ��Ϸù�ȣ    ", pOutBlock->mtiordseqno     , sizeof( pOutBlock->mtiordseqno      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ�����            ", pOutBlock->ordcndi         , sizeof( pOutBlock->ordcndi          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȣ�������ڵ�        ", pOutBlock->ordprcptncode   , sizeof( pOutBlock->ordprcptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ü�����      ", pOutBlock->nsavtrdqty      , sizeof( pOutBlock->nsavtrdqty       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->shtnIsuno       , sizeof( pOutBlock->shtnIsuno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������ù�ȣ        ", pOutBlock->opdrtnno        , sizeof( pOutBlock->opdrtnno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ݴ�Ÿ��ֹ�����    ", pOutBlock->cvrgordtp       , sizeof( pOutBlock->cvrgordtp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ü�����(�ֹ�)    ", pOutBlock->unercqty        , sizeof( pOutBlock->unercqty         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���ü�����    ", pOutBlock->orgordunercqty  , sizeof( pOutBlock->orgordunercqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���������      ", pOutBlock->orgordmdfyqty   , sizeof( pOutBlock->orgordmdfyqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ֹ���Ҽ���      ", pOutBlock->orgordcancqty   , sizeof( pOutBlock->orgordcancqty    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ü�ᰡ��    ", pOutBlock->ordavrexecprc   , sizeof( pOutBlock->ordavrexecprc    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ��ݾ�            ", pOutBlock->ordamt          , sizeof( pOutBlock->ordamt           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ǥ�������ȣ        ", pOutBlock->stdIsuno        , sizeof( pOutBlock->stdIsuno         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��ǥ�������ȣ      ", pOutBlock->bfstdIsuno      , sizeof( pOutBlock->bfstdIsuno       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�Ÿű���            ", pOutBlock->bnstp           , sizeof( pOutBlock->bnstp            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ��ŷ������ڵ�    ", pOutBlock->ordtrdptncode   , sizeof( pOutBlock->ordtrdptncode    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ſ�ŷ��ڵ�        ", pOutBlock->mgntrncode      , sizeof( pOutBlock->mgntrncode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������ջ��ڵ�      ", pOutBlock->adduptp         , sizeof( pOutBlock->adduptp          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ÿ�ü�ڵ�        ", pOutBlock->commdacode      , sizeof( pOutBlock->commdacode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������              ", pOutBlock->Loandt          , sizeof( pOutBlock->Loandt           ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ȸ��/��ȸ�����ȣ   ", pOutBlock->mbrnmbrno       , sizeof( pOutBlock->mbrnmbrno        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����¹�ȣ        ", pOutBlock->ordacntno       , sizeof( pOutBlock->ordacntno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������ȣ        ", pOutBlock->agrgbrnno       , sizeof( pOutBlock->agrgbrnno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���������ȣ        ", pOutBlock->mgempno         , sizeof( pOutBlock->mgempno          ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��������������ȣ    ", pOutBlock->futsLnkbrnno    , sizeof( pOutBlock->futsLnkbrnno     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����������¹�ȣ    ", pOutBlock->futsLnkacntno   , sizeof( pOutBlock->futsLnkacntno    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������屸��        ", pOutBlock->futsmkttp       , sizeof( pOutBlock->futsmkttp        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��Ͻ����ڵ�        ", pOutBlock->regmktcode      , sizeof( pOutBlock->regmktcode       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�������űݷ�        ", pOutBlock->mnymgnrat       , sizeof( pOutBlock->mnymgnrat        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������űݷ�        ", pOutBlock->substmgnrat     , sizeof( pOutBlock->substmgnrat      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����ü��ݾ�        ", pOutBlock->mnyexecamt      , sizeof( pOutBlock->mnyexecamt       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ü��ݾ�        ", pOutBlock->ubstexecamt     , sizeof( pOutBlock->ubstexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������ü��ݾ�      ", pOutBlock->cmsnamtexecamt  , sizeof( pOutBlock->cmsnamtexecamt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸ü��ݾ�    ", pOutBlock->crdtpldgexecamt , sizeof( pOutBlock->crdtpldgexecamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�ü��ݾ�        ", pOutBlock->crdtexecamt     , sizeof( pOutBlock->crdtexecamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->prdayruseexecval, sizeof( pOutBlock->prdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������ü��ݾ�  ", pOutBlock->crdayruseexecval, sizeof( pOutBlock->crdayruseexecval ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ�ü�����        ", pOutBlock->spotexecqty     , sizeof( pOutBlock->spotexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ŵ�ü�����      ", pOutBlock->stslexecqty     , sizeof( pOutBlock->stslexecqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ڵ�            ", pOutBlock->strtgcode       , sizeof( pOutBlock->strtgcode        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�׷�Id              ", pOutBlock->grpId           , sizeof( pOutBlock->grpId            ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ֹ�ȸ��            ", pOutBlock->ordseqno        , sizeof( pOutBlock->ordseqno         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ʈ��������ȣ      ", pOutBlock->ptflno          , sizeof( pOutBlock->ptflno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ٽ��Ϲ�ȣ          ", pOutBlock->bskno           , sizeof( pOutBlock->bskno            ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "Ʈ��ġ��ȣ          ", pOutBlock->trchno          , sizeof( pOutBlock->trchno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����۹�ȣ          ", pOutBlock->itemno          , sizeof( pOutBlock->itemno           ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���Id            ", pOutBlock->orduserId       , sizeof( pOutBlock->orduserId        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���԰�������        ", pOutBlock->brwmgmtYn       , sizeof( pOutBlock->brwmgmtYn        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܱ��ΰ�����ȣ      ", pOutBlock->frgrunqno       , sizeof( pOutBlock->frgrunqno        ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ���¡������      ", pOutBlock->trtzxLevytp     , sizeof( pOutBlock->trtzxLevytp      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����������ڱ���    ", pOutBlock->lptp            , sizeof( pOutBlock->lptp             ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ü��ð�            ", pOutBlock->exectime        , sizeof( pOutBlock->exectime         ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŷ��Ҽ���ü��ð�  ", pOutBlock->rcptexectime    , sizeof( pOutBlock->rcptexectime     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ܿ�����ݾ�        ", pOutBlock->rmndLoanamt     , sizeof( pOutBlock->rmndLoanamt      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����            ", pOutBlock->secbalqty       , sizeof( pOutBlock->secbalqty        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ǹ����ɼ���        ", pOutBlock->spotordableqty  , sizeof( pOutBlock->spotordableqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɼ���(�ŵ�)", pOutBlock->ordableruseqty  , sizeof( pOutBlock->ordableruseqty   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��������            ", pOutBlock->flctqty         , sizeof( pOutBlock->flctqty          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ܰ����(d2)        ", pOutBlock->secbalqtyd2     , sizeof( pOutBlock->secbalqtyd2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ��ֹ����ɼ���    ", pOutBlock->sellableqty     , sizeof( pOutBlock->sellableqty      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ü��ŵ��ֹ�����  ", pOutBlock->unercsellordqty , sizeof( pOutBlock->unercsellordqty  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��ո��԰�          ", pOutBlock->avrpchsprc      , sizeof( pOutBlock->avrpchsprc       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���Աݾ�            ", pOutBlock->pchsant         , sizeof( pOutBlock->pchsant          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������              ", pOutBlock->deposit         , sizeof( pOutBlock->deposit          ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����              ", pOutBlock->substamt        , sizeof( pOutBlock->substamt         ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���ű�����      ", pOutBlock->csgnmnymgn      , sizeof( pOutBlock->csgnmnymgn       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "��Ź���űݴ��      ", pOutBlock->csgnsubstmgn    , sizeof( pOutBlock->csgnsubstmgn     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ſ�㺸�����    ", pOutBlock->crdtpldgruseamt , sizeof( pOutBlock->crdtpldgruseamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ���������        ", pOutBlock->ordablemny      , sizeof( pOutBlock->ordablemny       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ֹ����ɴ��        ", pOutBlock->ordablesubstamt , sizeof( pOutBlock->ordablesubstamt  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���밡�ɱݾ�      ", pOutBlock->ruseableamt     , sizeof( pOutBlock->ruseableamt      ), DATA_TYPE_LONG  , 0 );
}

void CSample_xingACEDlg::DetailLogS3_( LPCTSTR pszData )
{
	//LPS3__OutBlock pOutBlock = (LPS3__OutBlock)pszData;

	//SetDetailLogData( "ü��ð�          ", pOutBlock->chetime   , sizeof( pOutBlock->chetime    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���ϴ�񱸺�      ", pOutBlock->sign      , sizeof( pOutBlock->sign       ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���ϴ��          ", pOutBlock->change    , sizeof( pOutBlock->change     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����            ", pOutBlock->drate     , sizeof( pOutBlock->drate      ), DATA_TYPE_FLOAT , 2 );
	//SetDetailLogData( "���簡            ", pOutBlock->price     , sizeof( pOutBlock->price      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ð��ð�          ", pOutBlock->opentime  , sizeof( pOutBlock->opentime   ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ð�              ", pOutBlock->open      , sizeof( pOutBlock->open       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "���ð�          ", pOutBlock->hightime  , sizeof( pOutBlock->hightime   ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "��              ", pOutBlock->high      , sizeof( pOutBlock->high       ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ð�          ", pOutBlock->lowtime   , sizeof( pOutBlock->lowtime    ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "����              ", pOutBlock->low       , sizeof( pOutBlock->low        ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�ᱸ��          ", pOutBlock->cgubun    , sizeof( pOutBlock->cgubun     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "ü�ᷮ            ", pOutBlock->cvolume   , sizeof( pOutBlock->cvolume    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ŷ���        ", pOutBlock->volume    , sizeof( pOutBlock->volume     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ŷ����      ", pOutBlock->value     , sizeof( pOutBlock->value      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�����ü�ᷮ    ", pOutBlock->mdvolume  , sizeof( pOutBlock->mdvolume   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�����ü��Ǽ�  ", pOutBlock->mdchecnt  , sizeof( pOutBlock->mdchecnt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�����ü�ᷮ    ", pOutBlock->msvolume  , sizeof( pOutBlock->msvolume   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�����ü��Ǽ�  ", pOutBlock->mschecnt  , sizeof( pOutBlock->mschecnt   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "ü�ᰭ��          ", pOutBlock->cpower    , sizeof( pOutBlock->cpower     ), DATA_TYPE_FLOAT , 2 );
	//SetDetailLogData( "������հ�        ", pOutBlock->w_avrg    , sizeof( pOutBlock->w_avrg     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��          ", pOutBlock->offerho   , sizeof( pOutBlock->offerho    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��          ", pOutBlock->bidho     , sizeof( pOutBlock->bidho      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "������            ", pOutBlock->status    , sizeof( pOutBlock->status     ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "���ϵ��ð���ŷ���", pOutBlock->jnilvolume, sizeof( pOutBlock->jnilvolume ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�����ڵ�          ", pOutBlock->shcode    , sizeof( pOutBlock->shcode     ), DATA_TYPE_STRING, 0 );
}

void CSample_xingACEDlg::DetailLogH1_( LPCTSTR pszData )
{
	//LPH1__OutBlock pOutBlock = (LPH1__OutBlock)pszData;

	//SetDetailLogData( "ȣ���ð�      ", pOutBlock->hotime     , sizeof( pOutBlock->hotime      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�ŵ�ȣ��1     ", pOutBlock->offerho1   , sizeof( pOutBlock->offerho1    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��1     ", pOutBlock->bidho1     , sizeof( pOutBlock->bidho1      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�1 ", pOutBlock->offerrem1  , sizeof( pOutBlock->offerrem1   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�1 ", pOutBlock->bidrem1    , sizeof( pOutBlock->bidrem1     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��2     ", pOutBlock->offerho2   , sizeof( pOutBlock->offerho2    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��2     ", pOutBlock->bidho2     , sizeof( pOutBlock->bidho2      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�2 ", pOutBlock->offerrem2  , sizeof( pOutBlock->offerrem2   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�2 ", pOutBlock->bidrem2    , sizeof( pOutBlock->bidrem2     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��3     ", pOutBlock->offerho3   , sizeof( pOutBlock->offerho3    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��3     ", pOutBlock->bidho3     , sizeof( pOutBlock->bidho3      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�3 ", pOutBlock->offerrem3  , sizeof( pOutBlock->offerrem3   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�3 ", pOutBlock->bidrem3    , sizeof( pOutBlock->bidrem3     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��4     ", pOutBlock->offerho4   , sizeof( pOutBlock->offerho4    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��4     ", pOutBlock->bidho4     , sizeof( pOutBlock->bidho4      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�4 ", pOutBlock->offerrem4  , sizeof( pOutBlock->offerrem4   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�4 ", pOutBlock->bidrem4    , sizeof( pOutBlock->bidrem4     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��5     ", pOutBlock->offerho5   , sizeof( pOutBlock->offerho5    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��5     ", pOutBlock->bidho5     , sizeof( pOutBlock->bidho5      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�5 ", pOutBlock->offerrem5  , sizeof( pOutBlock->offerrem5   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�5 ", pOutBlock->bidrem5    , sizeof( pOutBlock->bidrem5     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��6     ", pOutBlock->offerho6   , sizeof( pOutBlock->offerho6    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��6     ", pOutBlock->bidho6     , sizeof( pOutBlock->bidho6      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�6 ", pOutBlock->offerrem6  , sizeof( pOutBlock->offerrem6   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�6 ", pOutBlock->bidrem6    , sizeof( pOutBlock->bidrem6     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��7     ", pOutBlock->offerho7   , sizeof( pOutBlock->offerho7    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��7     ", pOutBlock->bidho7     , sizeof( pOutBlock->bidho7      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�7 ", pOutBlock->offerrem7  , sizeof( pOutBlock->offerrem7   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�7 ", pOutBlock->bidrem7    , sizeof( pOutBlock->bidrem7     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��8     ", pOutBlock->offerho8   , sizeof( pOutBlock->offerho8    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��8     ", pOutBlock->bidho8     , sizeof( pOutBlock->bidho8      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�8 ", pOutBlock->offerrem8  , sizeof( pOutBlock->offerrem8   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�8 ", pOutBlock->bidrem8    , sizeof( pOutBlock->bidrem8     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��9     ", pOutBlock->offerho9   , sizeof( pOutBlock->offerho9    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��9     ", pOutBlock->bidho9     , sizeof( pOutBlock->bidho9      ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�9 ", pOutBlock->offerrem9  , sizeof( pOutBlock->offerrem9   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�9 ", pOutBlock->bidrem9    , sizeof( pOutBlock->bidrem9     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ��10    ", pOutBlock->offerho10  , sizeof( pOutBlock->offerho10   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ��10    ", pOutBlock->bidho10    , sizeof( pOutBlock->bidho10     ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ŵ�ȣ���ܷ�10", pOutBlock->offerrem10 , sizeof( pOutBlock->offerrem10  ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�ż�ȣ���ܷ�10", pOutBlock->bidrem10   , sizeof( pOutBlock->bidrem10    ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�Ѹŵ�ȣ���ܷ�", pOutBlock->totofferrem, sizeof( pOutBlock->totofferrem ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "�Ѹż�ȣ���ܷ�", pOutBlock->totbidrem  , sizeof( pOutBlock->totbidrem   ), DATA_TYPE_LONG  , 0 );
	//SetDetailLogData( "����ȣ������  ", pOutBlock->donsigubun , sizeof( pOutBlock->donsigubun  ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "�����ڵ�      ", pOutBlock->shcode     , sizeof( pOutBlock->shcode      ), DATA_TYPE_STRING, 0 );
	//SetDetailLogData( "������뱸��  ", pOutBlock->alloc_gubun, sizeof( pOutBlock->alloc_gubun ), DATA_TYPE_STRING, 0 );
}

void CSample_xingACEDlg::DetailLogK3_( LPCTSTR pszData )
{
//	LPK3__OutBlock pOutBlock = (LPK3__OutBlock)pszData;
//
//	SetDetailLogData( "ü��ð�          ", pOutBlock->chetime   , sizeof( pOutBlock->chetime    ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "���ϴ�񱸺�      ", pOutBlock->sign      , sizeof( pOutBlock->sign       ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "���ϴ��          ", pOutBlock->change    , sizeof( pOutBlock->change     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�����            ", pOutBlock->drate     , sizeof( pOutBlock->drate      ), DATA_TYPE_FLOAT , 2 );
//	SetDetailLogData( "���簡            ", pOutBlock->price     , sizeof( pOutBlock->price      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ð��ð�          ", pOutBlock->opentime  , sizeof( pOutBlock->opentime   ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "�ð�              ", pOutBlock->open      , sizeof( pOutBlock->open       ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "���ð�          ", pOutBlock->hightime  , sizeof( pOutBlock->hightime   ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "��              ", pOutBlock->high      , sizeof( pOutBlock->high       ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�����ð�          ", pOutBlock->lowtime   , sizeof( pOutBlock->lowtime    ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "����              ", pOutBlock->low       , sizeof( pOutBlock->low        ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "ü�ᱸ��          ", pOutBlock->cgubun    , sizeof( pOutBlock->cgubun     ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "ü�ᷮ            ", pOutBlock->cvolume   , sizeof( pOutBlock->cvolume    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�����ŷ���        ", pOutBlock->volume    , sizeof( pOutBlock->volume     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�����ŷ����      ", pOutBlock->value     , sizeof( pOutBlock->value      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�����ü�ᷮ    ", pOutBlock->mdvolume  , sizeof( pOutBlock->mdvolume   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�����ü��Ǽ�  ", pOutBlock->mdchecnt  , sizeof( pOutBlock->mdchecnt   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�����ü�ᷮ    ", pOutBlock->msvolume  , sizeof( pOutBlock->msvolume   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�����ü��Ǽ�  ", pOutBlock->mschecnt  , sizeof( pOutBlock->mschecnt   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "ü�ᰭ��          ", pOutBlock->cpower    , sizeof( pOutBlock->cpower     ), DATA_TYPE_FLOAT , 2 );
//	SetDetailLogData( "������հ�        ", pOutBlock->w_avrg    , sizeof( pOutBlock->w_avrg     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��          ", pOutBlock->offerho   , sizeof( pOutBlock->offerho    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��          ", pOutBlock->bidho     , sizeof( pOutBlock->bidho      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "������            ", pOutBlock->status    , sizeof( pOutBlock->status     ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "���ϵ��ð���ŷ���", pOutBlock->jnilvolume, sizeof( pOutBlock->jnilvolume ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�����ڵ�          ", pOutBlock->shcode    , sizeof( pOutBlock->shcode     ), DATA_TYPE_STRING, 0 );
}
//
void CSample_xingACEDlg::DetailLogHA_( LPCTSTR pszData)
{
//	LPHA__OutBlock pOutBlock = (LPHA__OutBlock)pszData;
//
//	SetDetailLogData( "ȣ���ð�      ", pOutBlock->hotime     , sizeof( pOutBlock->hotime      ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "�ŵ�ȣ��1     ", pOutBlock->offerho1   , sizeof( pOutBlock->offerho1    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��1     ", pOutBlock->bidho1     , sizeof( pOutBlock->bidho1      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�1 ", pOutBlock->offerrem1  , sizeof( pOutBlock->offerrem1   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�1 ", pOutBlock->bidrem1    , sizeof( pOutBlock->bidrem1     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��2     ", pOutBlock->offerho2   , sizeof( pOutBlock->offerho2    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��2     ", pOutBlock->bidho2     , sizeof( pOutBlock->bidho2      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�2 ", pOutBlock->offerrem2  , sizeof( pOutBlock->offerrem2   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�2 ", pOutBlock->bidrem2    , sizeof( pOutBlock->bidrem2     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��3     ", pOutBlock->offerho3   , sizeof( pOutBlock->offerho3    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��3     ", pOutBlock->bidho3     , sizeof( pOutBlock->bidho3      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�3 ", pOutBlock->offerrem3  , sizeof( pOutBlock->offerrem3   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�3 ", pOutBlock->bidrem3    , sizeof( pOutBlock->bidrem3     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��4     ", pOutBlock->offerho4   , sizeof( pOutBlock->offerho4    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��4     ", pOutBlock->bidho4     , sizeof( pOutBlock->bidho4      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�4 ", pOutBlock->offerrem4  , sizeof( pOutBlock->offerrem4   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�4 ", pOutBlock->bidrem4    , sizeof( pOutBlock->bidrem4     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��5     ", pOutBlock->offerho5   , sizeof( pOutBlock->offerho5    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��5     ", pOutBlock->bidho5     , sizeof( pOutBlock->bidho5      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�5 ", pOutBlock->offerrem5  , sizeof( pOutBlock->offerrem5   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�5 ", pOutBlock->bidrem5    , sizeof( pOutBlock->bidrem5     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��6     ", pOutBlock->offerho6   , sizeof( pOutBlock->offerho6    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��6     ", pOutBlock->bidho6     , sizeof( pOutBlock->bidho6      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�6 ", pOutBlock->offerrem6  , sizeof( pOutBlock->offerrem6   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�6 ", pOutBlock->bidrem6    , sizeof( pOutBlock->bidrem6     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��7     ", pOutBlock->offerho7   , sizeof( pOutBlock->offerho7    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��7     ", pOutBlock->bidho7     , sizeof( pOutBlock->bidho7      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�7 ", pOutBlock->offerrem7  , sizeof( pOutBlock->offerrem7   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�7 ", pOutBlock->bidrem7    , sizeof( pOutBlock->bidrem7     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��8     ", pOutBlock->offerho8   , sizeof( pOutBlock->offerho8    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��8     ", pOutBlock->bidho8     , sizeof( pOutBlock->bidho8      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�8 ", pOutBlock->offerrem8  , sizeof( pOutBlock->offerrem8   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�8 ", pOutBlock->bidrem8    , sizeof( pOutBlock->bidrem8     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��9     ", pOutBlock->offerho9   , sizeof( pOutBlock->offerho9    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��9     ", pOutBlock->bidho9     , sizeof( pOutBlock->bidho9      ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�9 ", pOutBlock->offerrem9  , sizeof( pOutBlock->offerrem9   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�9 ", pOutBlock->bidrem9    , sizeof( pOutBlock->bidrem9     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ��10    ", pOutBlock->offerho10  , sizeof( pOutBlock->offerho10   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ��10    ", pOutBlock->bidho10    , sizeof( pOutBlock->bidho10     ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ŵ�ȣ���ܷ�10", pOutBlock->offerrem10 , sizeof( pOutBlock->offerrem10  ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�ż�ȣ���ܷ�10", pOutBlock->bidrem10   , sizeof( pOutBlock->bidrem10    ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�Ѹŵ�ȣ���ܷ�", pOutBlock->totofferrem, sizeof( pOutBlock->totofferrem ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "�Ѹż�ȣ���ܷ�", pOutBlock->totbidrem  , sizeof( pOutBlock->totbidrem   ), DATA_TYPE_LONG  , 0 );
//	SetDetailLogData( "����ȣ������  ", pOutBlock->donsigubun , sizeof( pOutBlock->donsigubun  ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "�����ڵ�      ", pOutBlock->shcode     , sizeof( pOutBlock->shcode      ), DATA_TYPE_STRING, 0 );
//	SetDetailLogData( "������뱸��  ", pOutBlock->alloc_gubun, sizeof( pOutBlock->alloc_gubun ), DATA_TYPE_STRING, 0 );
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
