// TcpSock.cpp: implementation of the CTcpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "TcpClient.h"
#include "IRUM_Common.h" //todo after completion - remove ../NEW/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpClient::CTcpClient(char* pzName) :CBaseThread("TCP")
{
	strcpy(m_zMyName, pzName);
	m_sock = INVALID_SOCKET;
	m_bConn = FALSE;
	m_bRecvErr = FALSE;
}
CTcpClient::~CTcpClient()
{
	End();

}

VOID CTcpClient::End()
{
	Disconnect();
}

BOOL CTcpClient::Begin( char* pRemoteIP, int nPort, int nTimeOut)
{
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return FALSE;
	}

	if( m_sock == INVALID_SOCKET){
		m_sock = socket(AF_INET, SOCK_STREAM, 0);
		if(m_sock == INVALID_SOCKET)
		{
			DumpErr("create socket", GetLastError() );
			return FALSE;
		}
	}

	////	remote address
	m_sin.sin_family      = AF_INET;
    m_sin.sin_port        = htons(nPort);
    m_sin.sin_addr.s_addr = inet_addr(pRemoteIP);

	strcpy(m_zRemoteIP, pRemoteIP);
	m_nRemotePort = nPort;

	m_nTimeout = nTimeOut;

	CBaseThread::m_bContinue = TRUE;

	return Connect();
}


BOOL CTcpClient::Connect()
{
	if( m_sock == INVALID_SOCKET){
		m_sock = socket(AF_INET, SOCK_STREAM, 0);
		if(m_sock == INVALID_SOCKET)
		{
			DumpErr("socket create", GetLastError() );	
			return FALSE;
		}
	}

	//if (!CBaseThread::m_bContinue)
	//	return FALSE;

	if(connect(m_sock, (LPSOCKADDR)&m_sin, sizeof(m_sin)) == SOCKET_ERROR)
	{
		m_bConn = FALSE;
		DumpErr("connect", GetLastError() );
		Disconnect();
		return FALSE;
	}

	// send timeout
	setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&m_nTimeout, sizeof(m_nTimeout));
	setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&m_nTimeout, sizeof(m_nTimeout));

	sprintf(m_zMsg, "connect ok(%s)(%d)", m_zRemoteIP, m_nRemotePort);
	m_bConn  =TRUE;
	return TRUE;
}


// RECV 전용 스레드
VOID CTcpClient::ThreadFunc()
{
	RecvThread();
}

VOID CTcpClient::RecvThread()
{
	char zRecvBuff[LEN_BUFF_SIZE];
	printf("TcpClent thread:%d\n", m_dwThreadID);

	while (Is_TimeOfStop() == FALSE)
	{
		if (!IsConnected())
		{
			Sleep(1000);
			Connect();
			if (!CBaseThread::m_bContinue)
				return;


			continue;
		}
		ZeroMemory(zRecvBuff, LEN_BUFF_SIZE);
		m_bRecvErr = FALSE;
		int Ret = recv(m_sock, zRecvBuff, LEN_BUFF_SIZE, 0);
		if (Ret > 0)
		{
			ST_API_ORD_RESPONSE* p = (ST_API_ORD_RESPONSE*)zRecvBuff;
			if (strncmp(p->Code, CDAPI_ORD_ACPT, 6) == 0 ||
				strncmp(p->Code, CDAPI_ORD_REAL, 6) == 0 ||
				strncmp(p->Code, CDAPI_CNTR_REAL, 6) == 0
				)
			{
				//OutputDebugString(zRecvBuff);
				//OutputDebugString("\n");
			}
			//OutputDebugString(zRecvBuff);
			//OutputDebugString("\n");

			//printf("[RECV](%.*s)\n", Ret, zRecvBuff);
			m_pktHandler.AddPkt(zRecvBuff, Ret);			
		}
		else if (Ret == 0)
		{
			Disconnect();
		}
		else if (Ret == SOCKET_ERROR)
		{
			int nErr = GetLastError();
			if (nErr != WSAETIMEDOUT)
			{
				m_bRecvErr = FALSE;
				DumpErr("recv", nErr);
				Disconnect();
				Sleep(5000);
			}
		}
	} // while

	printf(" CTcpClient::RecvThread() end....\n");
}

INT CTcpClient::SendData( char* pInBuf, int nBufLen,  int *o_ErrCode )
{
	if (!IsConnected())
	{
		if (!Connect()) {
			Disconnect();
			return -1;
		}
	}

	
	int Ret;
	int nRetryCnt = 0, nRetryBlock=0;
	
	while(1)
	{
		Ret = send(m_sock, pInBuf, nBufLen, 0);
		if (Ret > 0) {
			*o_ErrCode = 0;
			printf("<SEND:%d>(%.*s)\n", Ret, Ret, pInBuf);
			break;
		}
		
	//	if(Ret == SOCKET_ERROR)		
		int nErr = GetLastError();
		if( nErr==WSAETIMEDOUT )
		{
			char tmp[128];
			SYSTEMTIME st; GetLocalTime(&st);
			sprintf( tmp, "[%02d:%02d:%02d.%03d] \tWSAETIMEDOUT========\n", 
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			printf("%s\n",tmp);
			//Sleep(10);
			if( ++nRetryCnt > DEF_SEND_RETRY)
			{
				*o_ErrCode = WSAETIMEDOUT;
				sprintf( m_zMsg, "WSAETIMEDOUT %d회 반복으로 에러 리턴", nRetryCnt);
				printf("%s\n", m_zMsg);
				Disconnect();
				return -1;
			}
			continue;
		}
		else if(nErr==WSAEWOULDBLOCK)
		{
			printf("WOULDBLOCK:%d\n", nRetryBlock);
			//Sleep(10);
			if( ++nRetryBlock > DEF_SEND_RETRY)
			{
				*o_ErrCode = WSAETIMEDOUT;
				sprintf( m_zMsg, "WSAEWOULDBLOCK %d회 반복으로 에러 리턴", nRetryBlock);
				printf("%s\n", m_zMsg);
				Disconnect();
				return -1;
			}
			continue;
		}
		else
		{
			*o_ErrCode = nErr;
			sprintf( m_zMsg, "Send Errr (%d)", nErr);
			printf("%s\n", m_zMsg);
			Disconnect();
			return -1;
		}
	}
	
	return Ret;
}

VOID CTcpClient::SetIP_Port(char* psIP, int nPort)
{
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(nPort);
	m_sin.sin_addr.s_addr = inet_addr(psIP);
}
VOID CTcpClient::SetNagle(BOOL bOn)
{
	if (m_sock == INVALID_SOCKET)
		return;

	DWORD value = 1;
	if(bOn==FALSE)
		setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char*) &value, sizeof(value));
}



int	 CTcpClient::GetOneRecvedPacket(char* pOutBuf)
{
	if (!pOutBuf)
		return -1;
	if (strcmp(m_zMyName, "ORD") == 0)
	{
		int a;
		a = 10;

	}
	return m_pktHandler.GetOnePkt(pOutBuf);
}

int CTcpClient::RecvData( char* pOutBuf, int nBufLen, int *o_ErrCode)
{
	int Ret = recv(m_sock, pOutBuf, nBufLen, 0);
	if( Ret == SOCKET_ERROR)
	{
		int nErr = GetLastError();
		if (nErr == WSAETIMEDOUT) {
			return 0;
		}
		else
		{
			*o_ErrCode = nErr;
			DumpErr("recv", nErr);
			Disconnect();
			return -1;
		}
	}
	else if(Ret==0){
		Disconnect();
		return 0;
	}

	return Ret;
}



VOID CTcpClient::Disconnect()
{
	if (m_sock != INVALID_SOCKET) {
		struct linger ling;
		ling.l_onoff = 1;   // 0 ? use default, 1 ? use new value
		ling.l_linger = 0;  // close session in this time
		setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
		//-We can avoid TIME_WAIT on both of client and server side as we code above.
		closesocket(m_sock);
	}
	m_sock = INVALID_SOCKET;
	m_bConn = FALSE;
}

VOID CTcpClient::DumpErr( char* pSrc, int nErr )
{
	LPVOID lpMsgBuf=NULL;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL );

	sprintf( m_zMsg, "[%s] %s", pSrc, (char*)lpMsgBuf );
	LocalFree( lpMsgBuf );
}


BOOL CTcpClient::IsConnected()
{
	if( m_sock==INVALID_SOCKET )
		return FALSE;

	return m_bConn;
}