// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "XingAPI_Sample.h"
#include "DlgLogin.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"


extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgLogin dialog


CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLogin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLogin)
	DDX_Control(pDX, IDC_CHECK_SAVE_ID, m_ctrlSaveID);
	DDX_Control(pDX, IDC_CHECK_SHOW_CERT_ERR_DLG, m_ctrlShowCertErrDlg);
	DDX_Control(pDX, IDC_CHECK_CONNECT_TIMEOUT, m_ctrlConnectTimeout);
	DDX_Control(pDX, IDC_CHECK_SEND_PACKET_SIZE, m_ctrlSendPacketSize);
	DDX_Control(pDX, IDC_EDIT_SERVER_IP, m_ctrlServerIP);
	DDX_Control(pDX, IDC_EDIT_PWD, m_ctrlPwd);
	DDX_Control(pDX, IDC_EDIT_ID, m_ctrlID);
	DDX_Control(pDX, IDC_EDIT_CERT_PWD, m_ctrlCertPwd);
	DDX_Control(pDX, IDC_COMBO_SERVER_PORT, m_ctrlServerPort);
	DDX_Control(pDX, IDC_COMBO_SERVER_ADDR_TYPE, m_ctrlServerAddrType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	//{{AFX_MSG_MAP(CDlgLogin)
	ON_BN_CLICKED(IDC_RADIO_REAL, OnRadioReal)
	ON_BN_CLICKED(IDC_RADIO_SIMUL, OnRadioSimul)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER_ADDR_TYPE, OnSelchangeComboServerAddrType)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_CHECK_SEND_PACKET_SIZE, OnCheckSendPacketSize)
	ON_BN_CLICKED(IDC_CHECK_CONNECT_TIMEOUT, OnCheckConnectTimeout)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER + XM_LOGIN,  OnLogin  )

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin message handlers

BOOL CDlgLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// ���������� �о� ��Ʈ�ѵ��� �ʱ�ȭ
	LoadProfile();

	OnButtonLogin();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLogin::OnOK() 
{
	// CDialog::OnOK() �� �����Ͽ� Enter Key �Է½� ȭ���� ����Ǵ� ���� ����
}

//================================================================================================
// �����
//================================================================================================
void CDlgLogin::OnClose() 
{
	// ���� ����
	SaveProfile();

	CDialog::EndDialog( IDCANCEL );
}

//================================================================================================
// �α��� ����
//================================================================================================
void CDlgLogin::OnButtonLogin() 
{
	EnableWindow( FALSE );

	// ��������
	if( ConnectServer() == FALSE )
	{
		EnableWindow( TRUE );
		return;
	}

	// �α���
	if( !Login() )
	{
	//	OnClose();
	}
}

//================================================================================================
// ���� ����
//================================================================================================
BOOL CDlgLogin::ConnectServer()
{	

	//-----------------------------------------------------------------------------
	// �̹� ���ӵǾ� ������ ������ �����Ѵ�.
	if( g_iXingAPI.IsConnected() )
	{
		g_iXingAPI.Disconnect();
	}
	
	
	g_iXingAPI.Logout( GetSafeHwnd() );

	//-----------------------------------------------------------------------------
	// ���� IP
	CString strServerIP;
	m_ctrlServerIP.GetWindowText( strServerIP );
	
	//-----------------------------------------------------------------------------
	// ���� Port
	CString strServerPort;
	m_ctrlServerPort.GetWindowText( strServerPort );
	int nServerPort = atoi( strServerPort );
	
	//-----------------------------------------------------------------------------
	// ������ ���
	int nSendPacketSize = -1;		// -1 �� �����ϸ� �⺻���� ����Ѵ�.
	if( m_ctrlSendPacketSize.GetCheck() == 1 )
	{
		nSendPacketSize = GetDlgItemInt( IDC_COMBO_SEND_PACKET_SIZE );
	}
	
	//-----------------------------------------------------------------------------
	// ��������ð�
	int nConnectTimeOut = -1;		// -1 �� �����ϸ� �⺻���� ����Ѵ�.
	if( m_ctrlConnectTimeout.GetCheck() == 1 )
	{
		nConnectTimeOut = GetDlgItemInt( IDC_EDIT_CONNECT_TIMEOUT );
		nConnectTimeOut *= 1000;		// �ð��� Millisecond(1/1000��) ������ �־�� �Ѵ�.
	}
	
	//-----------------------------------------------------------------------------
	// ��������
	BOOL bResult = g_iXingAPI.Connect( 
		AfxGetMainWnd()->GetSafeHwnd(),			// Connect�� �� ���Ŀ� Disconnect �޽����� ���� ������ Handle
												// Login ������� Login �� ó���ϴ� ������ �̹Ƿ� Disconnect�� ���������쿡�� �޴´�.

		strServerIP,							// �����ּ�

		nServerPort,							// ������Ʈ

		WM_USER,								// XingAPI���� ����ϴ� �޽����� ���۹�ȣ, Windows������ ����ڸ޽����� 0x400(=WM_USER) �̻���
												// ����ؾ� ��. �⺻�����δ� WM_USER�� ����ϸ� ������ ���α׷� ���ο��� �޽��� ID�� ��ġ�� �Ǹ�
												// �� ���� �����Ͽ� �޽��� ID �浹�� ���Ҽ� ����

		nConnectTimeOut,						// ������ �ð��̻�(1/1000 �� ����)���� �ð��� �ɸ��� �� ��� ������з� ������

		nSendPacketSize							// ���������� Packet Size, -1 �̸� �⺻�� ���
												// ���ͳ� ���������� Ư�� ũ�� �̻��� �����͸� �ѹ��� ������ ������ �������� ��찡 �߻�
												// �̷� ��쿡 �ѹ��� ������ Packet Size�� �����Ͽ� �� �̻� �Ǵ� Packet�� �������� ���� ����
	);

	//-----------------------------------------------------------------------------
	// ���ӽ��� ó��
	if( bResult == FALSE )
	{
		int nErrorCode = g_iXingAPI.GetLastError();
		CString strMsg = g_iXingAPI.GetErrorMessage( nErrorCode )+'['+strServerIP+']';
		MessageBox( strMsg, "�������ӽ���", MB_ICONSTOP );
		return FALSE;
	}

	return TRUE;
}

//================================================================================================
// �α���
//================================================================================================
BOOL CDlgLogin::Login()
{
	//char zApiID[128], zConnPwd[128], zCrtfPwd[128];
	CUtil::GetConfig(g_zConfig, "API_INFO", "API_ID", g_stAPI.API_USERID);
	CUtil::GetConfig(g_zConfig, "API_INFO", "API_CONN_PWD", g_stAPI.API_CONN_PWD);
	CUtil::GetConfig(g_zConfig, "API_INFO", "API_CRTF_PWD", g_stAPI.API_CRTF_PWD);


	// ���̵�
	CString strID = CString(g_stAPI.API_USERID); //; GetDlgItemText( IDC_EDIT_ID, strID );
	CString strPwd = CString(g_stAPI.API_CONN_PWD); // ; GetDlgItemText( IDC_EDIT_PWD, strPwd );
	CString strCertPwd = CString(g_stAPI.API_CRTF_PWD); //; GetDlgItemText( IDC_EDIT_CERT_PWD, strCertPwd );

	//	��ȭ����
	//TODO strcpy( g_stAPI.API_ACNT_NO, db.Get("API_ACNT_NO"));
	//TODO strcpy( g_stAPI.API_ACNT_PWD, db.Get("API_ACNT_PWD"));

	//	�α��� ����
	g_stAPI.LOGON_YN[0] = 'N';

	//-----------------------------------------------------------------------------
	// ���� Type
	int nServerType = GetCheckedRadioButton( IDC_RADIO_REAL, IDC_RADIO_SIMUL ) == IDC_RADIO_REAL ? 0 : 1;
	
	//-----------------------------------------------------------------------------
	// �������� ������ ���� Dialog ǥ�ÿ���
	BOOL bShowCertErrDlg = m_ctrlShowCertErrDlg.GetCheck();

	//-----------------------------------------------------------------------------
	// �α���
	BOOL bResult = g_iXingAPI.Login( 
		GetSafeHwnd(),							// Login �������� �޽����� ���� ������
		strID,									// ����� ID
		strPwd,									// ����� ��й�ȣ
		strCertPwd,								// �������� ��й�ȣ
		nServerType,							// 0 : �Ǽ���, 1 : �������ڼ���
		bShowCertErrDlg							// �α��� �߿� �������� ������ �߻��� �����޽��� ǥ�ÿ���
	);

	//-----------------------------------------------------------------------------
	// �α��� ���� �߻�,
	//		�̰��� �α��� �����ܰ迡�� �߻��� �����̸� �α��� �������� �߻��� ������
	//		�޽����� �˷��ش�.
	if( bResult == FALSE )
	{
		EnableWindow( TRUE );

		int nErrorCode = g_iXingAPI.GetLastError();
		CString strMsg = g_iXingAPI.GetErrorMessage( nErrorCode );
		g_log.log(LOGTP_ERR, "ebest login error(ID:%s)(CONN_PWD:%s)(CRTF_PWD:%s)", g_stAPI.API_USERID, g_stAPI.API_CONN_PWD, g_stAPI.API_CRTF_PWD);
		g_log.log(NOTIFY, "ebest login error");
		MessageBox(strMsg, "�α��� ����", MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

//================================================================================================
// �α��� ���
//================================================================================================
LRESULT CDlgLogin::OnLogin( WPARAM wParam, LPARAM lParam )
{
	LPCSTR pszCode = (LPCSTR)wParam;
	LPCSTR pszMsg  = (LPCSTR)lParam;

	CString strMsg;
	strMsg.Format( "[%s] %s", pszCode, pszMsg );

	EnableWindow( TRUE );

	if( atoi( pszCode ) == 0 )
	{
		// �α��� ����
		//MessageBox( strMsg, "�α��� ����", MB_ICONINFORMATION );

		// ���� ����
		SaveProfile();
		
		// �α��� ����
		CDialog::EndDialog( IDOK );

		//���� AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)"FC0", NULL );
		AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)"FC0_1", NULL );

// #ifndef _SITE_SISE
// 		AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)"t5511", NULL );	//	�ֹ�TR / �ֹ�ü��REAL
// #endif
		g_stAPI.LOGON_YN[0] = 'Y';

		//AfxGetMainWnd()->KillTimer(1000);	// skeo. DLL�κ��� �α��θ޼��������� �翬��Ÿ�̸� �����Ѵ�
	}
	else
	{
		// �α��� ����
		g_log.log(NOTIFY, "ebest login error");
		MessageBox( strMsg, "�α��� ����", MB_ICONSTOP );
		g_stAPI.LOGON_YN[0] = 'N';
	}

	return 0L;
}

//================================================================================================
// ���������� ���ϸ��� ���
//================================================================================================
CString CDlgLogin::GetProfileFileName()
{
	CString strPath;
	int nLen = (int)GetModuleFileName( AfxGetInstanceHandle(), strPath.GetBuffer( 256 ), 256 );
	strPath.ReleaseBuffer( nLen );
	int nFind = strPath.ReverseFind( '\\' );
	if( nFind >= 0 ) strPath = strPath.Left( nFind );
	
	return strPath + "\\Login.ini";
}

//================================================================================================
// ���������� �о� ��Ʈ�ѵ��� �ʱ�ȭ
//================================================================================================
void CDlgLogin::LoadProfile()
{
	CString strFileName = GetProfileFileName();
	TCHAR	szData[256];

	//-----------------------------------------------------------------------------
	// ���̵� ����
// 	GetPrivateProfileString( "LOGIN", "SAVE_ID", "0", szData, sizeof( szData ), strFileName );
// 	if( atoi( szData ) == 0 )
// 	{
// 		m_ctrlSaveID.SetCheck( 0 );
// 	}
// 	else
// 	{
// 		m_ctrlSaveID.SetCheck( 1 );
// 
// 		//-----------------------------------------------------------------------------
// 		// ���̵�
// 		GetPrivateProfileString( "LOGIN", "ID", "", szData, sizeof( szData ), strFileName );
// 		SetDlgItemText( IDC_EDIT_ID, szData );
// 	}

	//-----------------------------------------------------------------------------
	// �������� ������ ���� Dialog ǥ��
// 	GetPrivateProfileString( "LOGIN", "SHOW_CERT_ERR_DLG", "1", szData, sizeof( szData ), strFileName );
// 	m_ctrlShowCertErrDlg.SetCheck( atoi( szData ) == 0 ? 0 : 1 );

	//-----------------------------------------------------------------------------
	// �Ǽ���/���Ǽ���
	GetPrivateProfileString( "COMM", "SERVER_TYPE", "", szData, sizeof( szData ), strFileName );
	BOOL bSimul = stricmp( szData, "SIMUL" ) == 0;
	CheckRadioButton( IDC_RADIO_REAL, IDC_RADIO_SIMUL, bSimul ? IDC_RADIO_SIMUL : IDC_RADIO_REAL );
	SetServerAddrType( bSimul == FALSE );

	//-----------------------------------------------------------------------------
	// ���� �ּ� TYPE
	GetPrivateProfileString( "COMM", "SERVER_ADDR_TYPE", "", szData, sizeof( szData ), strFileName );
	SetServerAddrTypeProfile( szData );
	GetPrivateProfileString( "COMM", "SERVER_IP", "", szData, sizeof( szData ), strFileName );
	CString strData; m_ctrlServerAddrType.GetWindowText( strData );
	SetServerIP( strData, szData );

	//-----------------------------------------------------------------------------
	// ���� ��Ʈ
	GetPrivateProfileString( "COMM", "SERVER_PORT", "", szData, sizeof( szData ), strFileName );
	SetPortProfile( atoi( szData ) );

	//-----------------------------------------------------------------------------
	// �ִ�����ũ��
	GetPrivateProfileString( "COMM", "USE_SEND_MAX_PACKET_SIZE", "", szData, sizeof( szData ), strFileName );
	if( atoi( szData ) == 0 )
	{
		m_ctrlSendPacketSize.SetCheck( 0 );
		GetDlgItem( IDC_COMBO_SEND_PACKET_SIZE )->EnableWindow( FALSE );
		SetDlgItemInt( IDC_COMBO_SEND_PACKET_SIZE, 512 );
	}
	else
	{
		m_ctrlSendPacketSize.SetCheck( 1 );
		GetDlgItem( IDC_COMBO_SEND_PACKET_SIZE )->EnableWindow( TRUE );

		GetPrivateProfileString( "COMM", "SEND_MAX_PACKET_SIZE", "", szData, sizeof( szData ), strFileName );
		SetDlgItemInt( IDC_COMBO_SEND_PACKET_SIZE, atoi( szData ) );
	}

	//-----------------------------------------------------------------------------
	// �������ӽð�
	GetPrivateProfileString( "COMM", "USE_CONNECT_TIMEOUT", "", szData, sizeof( szData ), strFileName );
	if( atoi( szData ) == 0 )
	{
		m_ctrlConnectTimeout.SetCheck( 0 );
		GetDlgItem( IDC_EDIT_CONNECT_TIMEOUT )->EnableWindow( FALSE );
		SetDlgItemInt( IDC_EDIT_CONNECT_TIMEOUT, 30 );
	}
	else
	{
		m_ctrlConnectTimeout.SetCheck( 1 );
		GetDlgItem( IDC_EDIT_CONNECT_TIMEOUT )->EnableWindow( TRUE );
		
		GetPrivateProfileString( "COMM", "CONNECT_TIMEOUT", "", szData, sizeof( szData ), strFileName );
		SetDlgItemInt( IDC_EDIT_CONNECT_TIMEOUT, atoi( szData ) );
	}
}

//================================================================================================
// �������Ͽ� ���� ���� �ּ� Type�� ������ �޺��ڽ����� ã�Ƽ� ����
//================================================================================================
void CDlgLogin::SetServerAddrTypeProfile( LPCTSTR pszData )
{
	int		nCount = m_ctrlServerAddrType.GetCount();
	CString	strData;
	for( int i=0; i<nCount; i++ )
	{
		m_ctrlServerAddrType.GetLBText( i, strData );
		if( strData == pszData )
		{
			m_ctrlServerAddrType.SetCurSel( i );
			return;
		}
	}
	m_ctrlServerAddrType.SetCurSel( 0 );
}

//================================================================================================
// �������Ͽ� ���� ��Ʈ�� ������ �޺��ڽ����� ã�Ƽ� ����
//================================================================================================
void CDlgLogin::SetPortProfile( int nPort )
{
	int			nCount = m_ctrlServerPort.GetCount();
	CString		strPort;
	for( int i=0; i<nCount; i++ )
	{
		m_ctrlServerPort.GetLBText( i, strPort );
		if( atoi( strPort ) == nPort )
		{
			m_ctrlServerPort.SetCurSel( i );
			return;
		}
	}
	m_ctrlServerPort.SetCurSel( 0 );
}

//================================================================================================
// �������Ͽ� �����Ѵ�.
//================================================================================================
void CDlgLogin::SaveProfile()
{
	CString strFileName = GetProfileFileName();
	CString strData;
	
	//-----------------------------------------------------------------------------
	// ���̵� ����
	if( m_ctrlSaveID.GetCheck() == 0 )
	{
		WritePrivateProfileString( "LOGIN", "SAVE_ID", "0", strFileName );
		WritePrivateProfileString( "LOGIN", "ID", "", strFileName );
	}
	else
	{
		GetDlgItemText( IDC_EDIT_ID, strData );
		WritePrivateProfileString( "LOGIN", "SAVE_ID", "1", strFileName );
		WritePrivateProfileString( "LOGIN", "ID", strData, strFileName );
	}

	//-----------------------------------------------------------------------------
	// �������� ������ ���� Dialog ǥ��
	strData = m_ctrlShowCertErrDlg.GetCheck() == 0 ? "0" : "1";
	WritePrivateProfileString( "LOGIN", "SHOW_CERT_ERR_DLG", strData, strFileName );
	
	//-----------------------------------------------------------------------------
	// �Ǽ���/���Ǽ���
	strData = GetCheckedRadioButton( IDC_RADIO_REAL, IDC_RADIO_SIMUL ) == IDC_RADIO_REAL ? "REAL" : "SIMUL";
	WritePrivateProfileString( "COMM", "SERVER_TYPE", strData, strFileName );
	
	//-----------------------------------------------------------------------------
	// ���� �ּ� TYPE
	GetDlgItemText( IDC_COMBO_SERVER_ADDR_TYPE, strData );
	WritePrivateProfileString( "COMM", "SERVER_ADDR_TYPE", strData, strFileName );

	//-----------------------------------------------------------------------------
	// ���� �ּ�
	GetDlgItemText( IDC_EDIT_SERVER_IP, strData );
	WritePrivateProfileString( "COMM", "SERVER_IP", strData, strFileName );
	
	//-----------------------------------------------------------------------------
	// ���� ��Ʈ
	GetDlgItemText( IDC_COMBO_SERVER_PORT, strData );
	WritePrivateProfileString( "COMM", "SERVER_PORT", strData, strFileName );

	//-----------------------------------------------------------------------------
	// �ִ�����ũ��
	strData = m_ctrlSendPacketSize.GetCheck() == 0 ? "0" : "1";
	WritePrivateProfileString( "COMM", "USE_SEND_MAX_PACKET_SIZE", strData, strFileName );
	GetDlgItemText( IDC_COMBO_SEND_PACKET_SIZE, strData );
	WritePrivateProfileString( "COMM", "SEND_MAX_PACKET_SIZE", strData, strFileName );
	OnCheckSendPacketSize();

	//-----------------------------------------------------------------------------
	// �������ӽð�
	strData = m_ctrlConnectTimeout.GetCheck() == 0 ? "0" : "1";
	WritePrivateProfileString( "COMM", "USE_CONNECT_TIMEOUT", strData, strFileName );
	GetDlgItemText( IDC_EDIT_CONNECT_TIMEOUT, strData );
	WritePrivateProfileString( "COMM", "CONNECT_TIMEOUT", strData, strFileName );
	OnCheckConnectTimeout();
}

//================================================================================================
// �Ǽ��� ���� ��Ʈ�� ����
//================================================================================================
void CDlgLogin::OnRadioReal() 
{
	SetServerAddrType( TRUE );
}

//================================================================================================
// �������� ���� ��Ʈ�� ����
//================================================================================================
void CDlgLogin::OnRadioSimul() 
{
	SetServerAddrType( FALSE );
}

//================================================================================================
// ����Type ����� ó��
//================================================================================================
void CDlgLogin::SetServerAddrType( BOOL bReal )
{
	m_ctrlServerAddrType.ResetContent();

	if( bReal )
	{
		m_ctrlServerAddrType.AddString( "HTS"	   );
		m_ctrlServerAddrType.AddString( "KT"       );
		m_ctrlServerAddrType.AddString( "KT IP"	   );
		m_ctrlServerAddrType.AddString( "PRISM"    );
		m_ctrlServerAddrType.AddString( "PRISM IP" );

		GetDlgItem( IDC_EDIT_CERT_PWD )->EnableWindow( TRUE );
	}
	else
	{
		m_ctrlServerAddrType.AddString( "��������"  );
		m_ctrlServerAddrType.AddString( "��������1" );
		m_ctrlServerAddrType.AddString( "��������2" );
		
		GetDlgItem( IDC_EDIT_CERT_PWD )->EnableWindow( FALSE );
	}
	m_ctrlServerAddrType.AddString( "�����Է�" );
	m_ctrlServerAddrType.SetCurSel( 0 );

	OnSelchangeComboServerAddrType();
}

//================================================================================================
// �����ּ�Type �޺� ��Ʈ�� ����
//================================================================================================
void CDlgLogin::OnSelchangeComboServerAddrType() 
{
	CString strData;
	m_ctrlServerAddrType.GetWindowText( strData );
	SetServerIP( strData, NULL );
}

//================================================================================================
// �ִ�����ũ�� �޺� ��Ʈ�� ����
//================================================================================================
void CDlgLogin::OnCheckSendPacketSize() 
{
	GetDlgItem( IDC_COMBO_SEND_PACKET_SIZE )->EnableWindow( m_ctrlSendPacketSize.GetCheck() == 0 ? FALSE : TRUE );	
}

//================================================================================================
// �������ӽð� �޺� ��Ʈ�� ����
//================================================================================================
void CDlgLogin::OnCheckConnectTimeout() 
{
	GetDlgItem( IDC_EDIT_CONNECT_TIMEOUT )->EnableWindow( m_ctrlConnectTimeout.GetCheck() == 0 ? FALSE : TRUE );	
}

//================================================================================================
// �����ּ� ��Ʈ�ѿ� ���� IP ����
//================================================================================================
void CDlgLogin::SetServerIP( LPCTSTR pszAddrType, LPCTSTR pszServerIP )
{
	CString strIP;
		 if( strcmp( pszAddrType, "HTS"       ) == 0 ) strIP = "hts.etrade.co.kr";
	else if( strcmp( pszAddrType, "KT"        ) == 0 ) strIP = "kt.etrade.co.kr";
	else if( strcmp( pszAddrType, "PRISM"     ) == 0 ) strIP = "prism.etrade.co.kr";
	else if( strcmp( pszAddrType, "KT IP"     ) == 0 ) strIP = "222.122.225.11";
	else if( strcmp( pszAddrType, "PRISM IP"  ) == 0 ) strIP = "61.106.5.131";
	else if( strcmp( pszAddrType, "��������"  ) == 0 ) strIP = "mis.etrade.co.kr";
	else if( strcmp( pszAddrType, "��������1" ) == 0 ) strIP = "211.216.52.131";
	else if( strcmp( pszAddrType, "��������2" ) == 0 ) strIP = "211.216.52.133";
	else if( strcmp( pszAddrType, "�����Է�"  ) == 0 && pszServerIP != NULL ) strIP = pszServerIP;

	m_ctrlServerIP.SetWindowText( strIP );
	if( strcmp( pszAddrType, "�����Է�"  ) == 0 )
	{
		m_ctrlServerIP.SetReadOnly( FALSE );
	}
	else
	{
		m_ctrlServerIP.SetReadOnly( TRUE );
	}
}

//================================================================================================
// ��Ʈ�ѿ��� ��Ŀ�� �̵�ó��
//================================================================================================
BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg) 
{
// 	// Enter�� �ԷµǾ��� ��� ...
// 	if( pMsg->message == WM_KEYUP && pMsg->wParam == VK_RETURN )
// 	{
// 		// ID �Է�â���� �߻�������
// 		if( pMsg->hwnd == ::GetDlgItem( GetSafeHwnd(), IDC_EDIT_ID ) )
// 		{
// 			// ���� �Է�â���� Focus�� �̵�
// 			NextDlgCtrl();
// 		}
// 		// ��й�ȣ �Է�â���� �߻�������
// 		else if( pMsg->hwnd == ::GetDlgItem( GetSafeHwnd(), IDC_EDIT_PWD ) )
// 		{
// 			// �������� ��й�ȣ �Է�â�� Ȱ��ȭ�Ǿ� ���� ������
// 			if( GetDlgItem( IDC_EDIT_CERT_PWD )->IsWindowEnabled() == FALSE )
// 			{
// 				// �α��� ��ư�� ������ó�� �޽����� �����Ͽ� �α��� ����
// 				PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_LOGIN, BN_CLICKED ), (LPARAM)::GetDlgItem( GetSafeHwnd(), IDC_BUTTON_LOGIN ) );
// 			}
// 			// �������� ��й�ȣ �Է�â�� Ȱ��ȭ�Ǿ� ������
// 			else
// 			{
// 				// ���� �Է�â���� Focus�� �̵�
// 				NextDlgCtrl();
// 			}
// 		}
// 		// �������� ��й�ȣ �Է�â���� �߻�������
// 		else if( pMsg->hwnd == ::GetDlgItem( GetSafeHwnd(), IDC_EDIT_CERT_PWD ) )
// 		{
// 			// �α��� ��ư�� ������ó�� �޽����� �����Ͽ� �α��� ����
// 			PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_LOGIN, BN_CLICKED ), (LPARAM)::GetDlgItem( GetSafeHwnd(), IDC_BUTTON_LOGIN ) );
// 		}
// 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
