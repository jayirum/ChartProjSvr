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

	// 설정파일을 읽어 컨트롤들을 초기화
	LoadProfile();

	OnButtonLogin();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLogin::OnOK() 
{
	// CDialog::OnOK() 를 삭제하여 Enter Key 입력시 화면이 종료되는 것을 막음
}

//================================================================================================
// 종료시
//================================================================================================
void CDlgLogin::OnClose() 
{
	// 상태 저장
	SaveProfile();

	CDialog::EndDialog( IDCANCEL );
}

//================================================================================================
// 로그인 시작
//================================================================================================
void CDlgLogin::OnButtonLogin() 
{
	EnableWindow( FALSE );

	// 서버접속
	if( ConnectServer() == FALSE )
	{
		EnableWindow( TRUE );
		return;
	}

	// 로그인
	if( !Login() )
	{
	//	OnClose();
	}
}

//================================================================================================
// 서버 접속
//================================================================================================
BOOL CDlgLogin::ConnectServer()
{	

	//-----------------------------------------------------------------------------
	// 이미 접속되어 있으면 접속을 종료한다.
	if( g_iXingAPI.IsConnected() )
	{
		g_iXingAPI.Disconnect();
	}
	
	
	g_iXingAPI.Logout( GetSafeHwnd() );

	//-----------------------------------------------------------------------------
	// 서버 IP
	CString strServerIP;
	m_ctrlServerIP.GetWindowText( strServerIP );
	
	//-----------------------------------------------------------------------------
	// 서버 Port
	CString strServerPort;
	m_ctrlServerPort.GetWindowText( strServerPort );
	int nServerPort = atoi( strServerPort );
	
	//-----------------------------------------------------------------------------
	// 공유기 사용
	int nSendPacketSize = -1;		// -1 로 설정하면 기본값을 사용한다.
	if( m_ctrlSendPacketSize.GetCheck() == 1 )
	{
		nSendPacketSize = GetDlgItemInt( IDC_COMBO_SEND_PACKET_SIZE );
	}
	
	//-----------------------------------------------------------------------------
	// 서버연결시간
	int nConnectTimeOut = -1;		// -1 로 설정하면 기본값을 사용한다.
	if( m_ctrlConnectTimeout.GetCheck() == 1 )
	{
		nConnectTimeOut = GetDlgItemInt( IDC_EDIT_CONNECT_TIMEOUT );
		nConnectTimeOut *= 1000;		// 시간은 Millisecond(1/1000초) 단위로 넣어야 한다.
	}
	
	//-----------------------------------------------------------------------------
	// 서버접속
	BOOL bResult = g_iXingAPI.Connect( 
		AfxGetMainWnd()->GetSafeHwnd(),			// Connect가 된 이후에 Disconnect 메시지를 받을 윈도우 Handle
												// Login 윈도우는 Login 만 처리하는 윈도우 이므로 Disconnect는 메인윈도우에서 받는다.

		strServerIP,							// 서버주소

		nServerPort,							// 서버포트

		WM_USER,								// XingAPI에서 사용하는 메시지의 시작번호, Windows에서는 사용자메시지를 0x400(=WM_USER) 이상을
												// 사용해야 함. 기본적으로는 WM_USER를 사용하면 되지만 프로그램 내부에서 메시지 ID가 겹치게 되면
												// 이 값을 조정하여 메시지 ID 충돌을 피할수 있음

		nConnectTimeOut,						// 지정한 시간이상(1/1000 초 단위)으로 시간이 걸리게 될 경우 연결실패로 간주함

		nSendPacketSize							// 보내어지는 Packet Size, -1 이면 기본값 사용
												// 인터넷 공유기등에서는 특정 크기 이상의 데이터를 한번에 보내면 에러가 떨어지는 경우가 발생
												// 이럴 경우에 한번에 보내는 Packet Size를 지정하여 그 이상 되는 Packet은 여러번에 걸쳐 전송
	);

	//-----------------------------------------------------------------------------
	// 접속실패 처리
	if( bResult == FALSE )
	{
		int nErrorCode = g_iXingAPI.GetLastError();
		CString strMsg = g_iXingAPI.GetErrorMessage( nErrorCode )+'['+strServerIP+']';
		MessageBox( strMsg, "서버접속실패", MB_ICONSTOP );
		return FALSE;
	}

	return TRUE;
}

//================================================================================================
// 로그인
//================================================================================================
BOOL CDlgLogin::Login()
{
	//char zApiID[128], zConnPwd[128], zCrtfPwd[128];
	CUtil::GetConfig(g_zConfig, "API_INFO", "API_ID", g_stAPI.API_USERID);
	CUtil::GetConfig(g_zConfig, "API_INFO", "API_CONN_PWD", g_stAPI.API_CONN_PWD);
	CUtil::GetConfig(g_zConfig, "API_INFO", "API_CRTF_PWD", g_stAPI.API_CRTF_PWD);


	// 아이디
	CString strID = CString(g_stAPI.API_USERID); //; GetDlgItemText( IDC_EDIT_ID, strID );
	CString strPwd = CString(g_stAPI.API_CONN_PWD); // ; GetDlgItemText( IDC_EDIT_PWD, strPwd );
	CString strCertPwd = CString(g_stAPI.API_CRTF_PWD); //; GetDlgItemText( IDC_EDIT_CERT_PWD, strCertPwd );

	//	원화계좌
	//TODO strcpy( g_stAPI.API_ACNT_NO, db.Get("API_ACNT_NO"));
	//TODO strcpy( g_stAPI.API_ACNT_PWD, db.Get("API_ACNT_PWD"));

	//	로그인 여부
	g_stAPI.LOGON_YN[0] = 'N';

	//-----------------------------------------------------------------------------
	// 서버 Type
	int nServerType = GetCheckedRadioButton( IDC_RADIO_REAL, IDC_RADIO_SIMUL ) == IDC_RADIO_REAL ? 0 : 1;
	
	//-----------------------------------------------------------------------------
	// 공인인증 에러시 에러 Dialog 표시여부
	BOOL bShowCertErrDlg = m_ctrlShowCertErrDlg.GetCheck();

	//-----------------------------------------------------------------------------
	// 로그인
	BOOL bResult = g_iXingAPI.Login( 
		GetSafeHwnd(),							// Login 성공여부 메시지를 받을 윈도우
		strID,									// 사용자 ID
		strPwd,									// 사용자 비밀번호
		strCertPwd,								// 공인인증 비밀번호
		nServerType,							// 0 : 실서버, 1 : 모의투자서버
		bShowCertErrDlg							// 로그인 중에 공인인증 에러가 발생시 에러메시지 표시여부
	);

	//-----------------------------------------------------------------------------
	// 로그인 에러 발생,
	//		이것은 로그인 사전단계에서 발생한 에러이며 로그인 과정에서 발생한 에러는
	//		메시지로 알려준다.
	if( bResult == FALSE )
	{
		EnableWindow( TRUE );

		int nErrorCode = g_iXingAPI.GetLastError();
		CString strMsg = g_iXingAPI.GetErrorMessage( nErrorCode );
		g_log.log(LOGTP_ERR, "ebest login error(ID:%s)(CONN_PWD:%s)(CRTF_PWD:%s)", g_stAPI.API_USERID, g_stAPI.API_CONN_PWD, g_stAPI.API_CRTF_PWD);
		g_log.log(NOTIFY, "ebest login error");
		MessageBox(strMsg, "로그인 실패", MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

//================================================================================================
// 로그인 결과
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
		// 로그인 성공
		//MessageBox( strMsg, "로그인 성공", MB_ICONINFORMATION );

		// 상태 저장
		SaveProfile();
		
		// 로그인 종료
		CDialog::EndDialog( IDOK );

		//국내 AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)"FC0", NULL );
		AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)"FC0_1", NULL );

// #ifndef _SITE_SISE
// 		AfxGetMainWnd()->SendMessage( WMU_SHOW_TR, (WPARAM)(LPCTSTR)"t5511", NULL );	//	주문TR / 주문체결REAL
// #endif
		g_stAPI.LOGON_YN[0] = 'Y';

		//AfxGetMainWnd()->KillTimer(1000);	// skeo. DLL로부터 로그인메세지받으면 재연결타이머 삭제한다
	}
	else
	{
		// 로그인 실패
		g_log.log(NOTIFY, "ebest login error");
		MessageBox( strMsg, "로그인 실패", MB_ICONSTOP );
		g_stAPI.LOGON_YN[0] = 'N';
	}

	return 0L;
}

//================================================================================================
// 설정파일의 파일명을 취득
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
// 설정파일을 읽어 컨트롤들을 초기화
//================================================================================================
void CDlgLogin::LoadProfile()
{
	CString strFileName = GetProfileFileName();
	TCHAR	szData[256];

	//-----------------------------------------------------------------------------
	// 아이디 저장
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
// 		// 아이디
// 		GetPrivateProfileString( "LOGIN", "ID", "", szData, sizeof( szData ), strFileName );
// 		SetDlgItemText( IDC_EDIT_ID, szData );
// 	}

	//-----------------------------------------------------------------------------
	// 공인인증 에러시 에러 Dialog 표시
// 	GetPrivateProfileString( "LOGIN", "SHOW_CERT_ERR_DLG", "1", szData, sizeof( szData ), strFileName );
// 	m_ctrlShowCertErrDlg.SetCheck( atoi( szData ) == 0 ? 0 : 1 );

	//-----------------------------------------------------------------------------
	// 실서버/모의서버
	GetPrivateProfileString( "COMM", "SERVER_TYPE", "", szData, sizeof( szData ), strFileName );
	BOOL bSimul = stricmp( szData, "SIMUL" ) == 0;
	CheckRadioButton( IDC_RADIO_REAL, IDC_RADIO_SIMUL, bSimul ? IDC_RADIO_SIMUL : IDC_RADIO_REAL );
	SetServerAddrType( bSimul == FALSE );

	//-----------------------------------------------------------------------------
	// 서버 주소 TYPE
	GetPrivateProfileString( "COMM", "SERVER_ADDR_TYPE", "", szData, sizeof( szData ), strFileName );
	SetServerAddrTypeProfile( szData );
	GetPrivateProfileString( "COMM", "SERVER_IP", "", szData, sizeof( szData ), strFileName );
	CString strData; m_ctrlServerAddrType.GetWindowText( strData );
	SetServerIP( strData, szData );

	//-----------------------------------------------------------------------------
	// 서버 포트
	GetPrivateProfileString( "COMM", "SERVER_PORT", "", szData, sizeof( szData ), strFileName );
	SetPortProfile( atoi( szData ) );

	//-----------------------------------------------------------------------------
	// 최대전송크기
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
	// 서버접속시간
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
// 설정파일에 읽은 서버 주소 Type을 가지고 콤보박스에서 찾아서 설정
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
// 설정파일에 읽은 포트를 가지고 콤보박스에서 찾아서 설정
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
// 설정파일에 저장한다.
//================================================================================================
void CDlgLogin::SaveProfile()
{
	CString strFileName = GetProfileFileName();
	CString strData;
	
	//-----------------------------------------------------------------------------
	// 아이디 저장
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
	// 공인인증 에러시 에러 Dialog 표시
	strData = m_ctrlShowCertErrDlg.GetCheck() == 0 ? "0" : "1";
	WritePrivateProfileString( "LOGIN", "SHOW_CERT_ERR_DLG", strData, strFileName );
	
	//-----------------------------------------------------------------------------
	// 실서버/모의서버
	strData = GetCheckedRadioButton( IDC_RADIO_REAL, IDC_RADIO_SIMUL ) == IDC_RADIO_REAL ? "REAL" : "SIMUL";
	WritePrivateProfileString( "COMM", "SERVER_TYPE", strData, strFileName );
	
	//-----------------------------------------------------------------------------
	// 서버 주소 TYPE
	GetDlgItemText( IDC_COMBO_SERVER_ADDR_TYPE, strData );
	WritePrivateProfileString( "COMM", "SERVER_ADDR_TYPE", strData, strFileName );

	//-----------------------------------------------------------------------------
	// 서버 주소
	GetDlgItemText( IDC_EDIT_SERVER_IP, strData );
	WritePrivateProfileString( "COMM", "SERVER_IP", strData, strFileName );
	
	//-----------------------------------------------------------------------------
	// 서버 포트
	GetDlgItemText( IDC_COMBO_SERVER_PORT, strData );
	WritePrivateProfileString( "COMM", "SERVER_PORT", strData, strFileName );

	//-----------------------------------------------------------------------------
	// 최대전송크기
	strData = m_ctrlSendPacketSize.GetCheck() == 0 ? "0" : "1";
	WritePrivateProfileString( "COMM", "USE_SEND_MAX_PACKET_SIZE", strData, strFileName );
	GetDlgItemText( IDC_COMBO_SEND_PACKET_SIZE, strData );
	WritePrivateProfileString( "COMM", "SEND_MAX_PACKET_SIZE", strData, strFileName );
	OnCheckSendPacketSize();

	//-----------------------------------------------------------------------------
	// 서버접속시간
	strData = m_ctrlConnectTimeout.GetCheck() == 0 ? "0" : "1";
	WritePrivateProfileString( "COMM", "USE_CONNECT_TIMEOUT", strData, strFileName );
	GetDlgItemText( IDC_EDIT_CONNECT_TIMEOUT, strData );
	WritePrivateProfileString( "COMM", "CONNECT_TIMEOUT", strData, strFileName );
	OnCheckConnectTimeout();
}

//================================================================================================
// 실서버 라디오 컨트롤 선택
//================================================================================================
void CDlgLogin::OnRadioReal() 
{
	SetServerAddrType( TRUE );
}

//================================================================================================
// 모의투자 라디오 컨트롤 선택
//================================================================================================
void CDlgLogin::OnRadioSimul() 
{
	SetServerAddrType( FALSE );
}

//================================================================================================
// 서버Type 변경시 처리
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
		m_ctrlServerAddrType.AddString( "모의투자"  );
		m_ctrlServerAddrType.AddString( "모의투자1" );
		m_ctrlServerAddrType.AddString( "모의투자2" );
		
		GetDlgItem( IDC_EDIT_CERT_PWD )->EnableWindow( FALSE );
	}
	m_ctrlServerAddrType.AddString( "직접입력" );
	m_ctrlServerAddrType.SetCurSel( 0 );

	OnSelchangeComboServerAddrType();
}

//================================================================================================
// 서버주소Type 콤보 컨트롤 선택
//================================================================================================
void CDlgLogin::OnSelchangeComboServerAddrType() 
{
	CString strData;
	m_ctrlServerAddrType.GetWindowText( strData );
	SetServerIP( strData, NULL );
}

//================================================================================================
// 최대전송크기 콤보 컨트롤 선택
//================================================================================================
void CDlgLogin::OnCheckSendPacketSize() 
{
	GetDlgItem( IDC_COMBO_SEND_PACKET_SIZE )->EnableWindow( m_ctrlSendPacketSize.GetCheck() == 0 ? FALSE : TRUE );	
}

//================================================================================================
// 서버접속시간 콤보 컨트롤 선택
//================================================================================================
void CDlgLogin::OnCheckConnectTimeout() 
{
	GetDlgItem( IDC_EDIT_CONNECT_TIMEOUT )->EnableWindow( m_ctrlConnectTimeout.GetCheck() == 0 ? FALSE : TRUE );	
}

//================================================================================================
// 서버주소 컨트롤에 서버 IP 설정
//================================================================================================
void CDlgLogin::SetServerIP( LPCTSTR pszAddrType, LPCTSTR pszServerIP )
{
	CString strIP;
		 if( strcmp( pszAddrType, "HTS"       ) == 0 ) strIP = "hts.etrade.co.kr";
	else if( strcmp( pszAddrType, "KT"        ) == 0 ) strIP = "kt.etrade.co.kr";
	else if( strcmp( pszAddrType, "PRISM"     ) == 0 ) strIP = "prism.etrade.co.kr";
	else if( strcmp( pszAddrType, "KT IP"     ) == 0 ) strIP = "222.122.225.11";
	else if( strcmp( pszAddrType, "PRISM IP"  ) == 0 ) strIP = "61.106.5.131";
	else if( strcmp( pszAddrType, "모의투자"  ) == 0 ) strIP = "mis.etrade.co.kr";
	else if( strcmp( pszAddrType, "모의투자1" ) == 0 ) strIP = "211.216.52.131";
	else if( strcmp( pszAddrType, "모의투자2" ) == 0 ) strIP = "211.216.52.133";
	else if( strcmp( pszAddrType, "직접입력"  ) == 0 && pszServerIP != NULL ) strIP = pszServerIP;

	m_ctrlServerIP.SetWindowText( strIP );
	if( strcmp( pszAddrType, "직접입력"  ) == 0 )
	{
		m_ctrlServerIP.SetReadOnly( FALSE );
	}
	else
	{
		m_ctrlServerIP.SetReadOnly( TRUE );
	}
}

//================================================================================================
// 컨트롤에서 포커스 이동처리
//================================================================================================
BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg) 
{
// 	// Enter가 입력되었을 경우 ...
// 	if( pMsg->message == WM_KEYUP && pMsg->wParam == VK_RETURN )
// 	{
// 		// ID 입력창에서 발생했으면
// 		if( pMsg->hwnd == ::GetDlgItem( GetSafeHwnd(), IDC_EDIT_ID ) )
// 		{
// 			// 다음 입력창으로 Focus를 이동
// 			NextDlgCtrl();
// 		}
// 		// 비밀번호 입력창에서 발생했으면
// 		else if( pMsg->hwnd == ::GetDlgItem( GetSafeHwnd(), IDC_EDIT_PWD ) )
// 		{
// 			// 공인인증 비밀번호 입력창이 활성화되어 있지 않으면
// 			if( GetDlgItem( IDC_EDIT_CERT_PWD )->IsWindowEnabled() == FALSE )
// 			{
// 				// 로그인 버튼이 눌린것처럼 메시지를 전송하여 로그인 진행
// 				PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_LOGIN, BN_CLICKED ), (LPARAM)::GetDlgItem( GetSafeHwnd(), IDC_BUTTON_LOGIN ) );
// 			}
// 			// 공인인증 비밀번호 입력창이 활성화되어 있으면
// 			else
// 			{
// 				// 다음 입력창으로 Focus를 이동
// 				NextDlgCtrl();
// 			}
// 		}
// 		// 공인인증 비밀번호 입력창에서 발생했으면
// 		else if( pMsg->hwnd == ::GetDlgItem( GetSafeHwnd(), IDC_EDIT_CERT_PWD ) )
// 		{
// 			// 로그인 버튼이 눌린것처럼 메시지를 전송하여 로그인 진행
// 			PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_LOGIN, BN_CLICKED ), (LPARAM)::GetDlgItem( GetSafeHwnd(), IDC_BUTTON_LOGIN ) );
// 		}
// 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
