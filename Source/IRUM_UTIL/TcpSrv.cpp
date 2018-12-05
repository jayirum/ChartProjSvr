// TcpSocket.cpp: implementation of the CTcpSrv class.
//\
//http://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancediomethod5f.html
//////////////////////////////////////////////////////////////////////

#include "TcpSrv.h"
#include <mswsock.h>
#include "Util.h"
#include "LogMsg.h"
extern CLogMsg g_log;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpSrv::CTcpSrv(): m_bWinsockStartUp(FALSE)
{
	m_sockListen = m_sockAcpted = INVALID_SOCKET;
	ZeroMemory(&m_hWsaEvents, sizeof(m_hWsaEvents));
	m_nEventCnt = 0;
}


CTcpSrv::~CTcpSrv()
{
	End();
}

VOID CTcpSrv::End()
{
	for (int i = IDX_ACPT + 1; i < m_nEventCnt + 1; i++)
		CloseClient(i);

	closesocket(m_sockListen);
	closesocket(m_sockAcpted);
	delete m_pArr[IDX_ACPT];

	if (m_bWinsockStartUp)
		WSACleanup();

	SetEvent(m_hDie[IDX_ACPT]);
	if (WaitForSingleObject(m_hThread[IDX_ACPT], 3000) != WAIT_OBJECT_0) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hThread[IDX_ACPT], dwExitCode);
		printf("Terminate Thread in BaseThread");
	}
	CloseHandle(m_hDie[IDX_ACPT]);
	CloseHandle(m_hThread[IDX_ACPT]);
	m_hDie[IDX_ACPT] = m_hThread[IDX_ACPT] = NULL;
}

VOID CTcpSrv::CloseClient(int nEventIdx)
{
	shutdown(m_pArr[nEventIdx]->Socket, SD_BOTH);
	struct linger structLinger;
	structLinger.l_onoff = TRUE;
	structLinger.l_linger = 0;
	setsockopt(m_pArr[nEventIdx]->Socket, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
	closesocket(m_pArr[nEventIdx]->Socket);
	delete m_pArr[nEventIdx];
	WSACloseEvent(m_hWsaEvents[nEventIdx]);

	
	// Cleanup m_pArr and p->m_hWsaEvents by removing the socket event handle
	// and socket information structure if they are not at the end of the arrays
	if (nEventIdx < m_nEventCnt)
	{
		for (int i = nEventIdx; i < m_nEventCnt; i++)
		{
			m_pArr[i] = m_pArr[i + 1];
			m_hWsaEvents[i] = m_hWsaEvents[i + 1];
		}
	}
	m_nEventCnt--;
	g_log.log(LOGTP_SUCC, "CloseClient. current client cnt : %d", m_nEventCnt-1);
}


BOOL CTcpSrv::Begin(int Port, char* IP, BOOL bWinsockStart)
{
	strcpy(m_zIP, IP);
	m_nPort = Port;
	m_bWinsockStartUp = bWinsockStart;


	if (m_bWinsockStartUp)
	{
		WORD	wVersionRequired;
		WSADATA	wsaData;

		//// WSAStartup
		wVersionRequired = MAKEWORD(2, 2);
		if (WSAStartup(wVersionRequired, &wsaData))
		{
			sprintf_s(m_zMsg, "%s", "WSAStartup error ");
			return FALSE;
		}

		//DumpWsaData(&wsaData);
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			sprintf_s(m_zMsg, "%s", "RequiredVersion not Usable ");
			return FALSE;
		}
	}

	m_sockListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_sockListen == INVALID_SOCKET)
	{
		SetSockErrMsg("socket create");
		return FALSE;
	}

	m_sock_addr.sin_family = AF_INET;
	m_sock_addr.sin_port = htons(m_nPort);
	m_sock_addr.sin_addr.s_addr = inet_addr(m_zIP);

	if (::bind(m_sockListen, (struct sockaddr*)&m_sock_addr, sizeof(m_sock_addr)) == SOCKET_ERROR)
	{
		SetSockErrMsg("bind");
		return FALSE;
	}

	if (listen(m_sockListen, SOMAXCONN) == SOCKET_ERROR)
	{
		SetSockErrMsg("listen");
		return FALSE;
	}

	// dummy socket info : EVENT 와 INFOR ARRAY 의 INDEX 를 일치시키기 위해
	m_pArr[IDX_ACPT] = new SOCKET_INFORMATION;
	
	m_dwThreadID[IDX_ACPT] = 0;
	m_hDie[IDX_ACPT] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThread[IDX_ACPT] = (HANDLE)_beginthreadex(NULL, 0, &AcptThread, this, 0, &m_dwThreadID[IDX_ACPT]);

	return RequestAcept();
}

BOOL CTcpSrv::RequestAcept()
{
	m_sockAcpted = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_sockAcpted == INVALID_SOCKET)
	{
		SetSockErrMsg("accept socket create");
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "accept Socket create: %d", m_sockAcpted);

	DWORD dwbytes;
	ZeroMemory(&m_ovAccept, sizeof(m_ovAccept));
	if (m_hWsaEvents[IDX_ACPT] == NULL) {
		m_hWsaEvents[IDX_ACPT] = WSACreateEvent();
		m_nEventCnt = 1;
	}
	else
		WSAResetEvent(m_hWsaEvents[IDX_ACPT]);

	m_ovAccept.hEvent = m_hWsaEvents[IDX_ACPT];

	BOOL b = AcceptEx(m_sockListen, m_sockAcpted, m_sAcceptBuf, 0, sizeof(m_sock_addr) + 16,
						sizeof(m_sock_addr) + 16, &dwbytes, &m_ovAccept);
	if (!b && WSAGetLastError() != ERROR_IO_PENDING)
	{
		SetSockErrMsg("AcceptEx");
		return FALSE;
	}
	return TRUE;
}


BOOL CTcpSrv::OnAccept()
{
	// Create a socket information structure to associate with the accepted socket
	m_pArr[m_nEventCnt] = (LPSOCKET_INFORMATION)new SOCKET_INFORMATION;

	// Fill in the details of our accepted socket			
	m_pArr[m_nEventCnt]->Socket = m_sockAcpted;
	ZeroMemory(&(m_pArr[m_nEventCnt]->Overlapped), sizeof(OVERLAPPED));
	m_pArr[m_nEventCnt]->SendRecvBit = IDX_RECV;
	m_pArr[m_nEventCnt]->BytesSEND = 0;
	m_pArr[m_nEventCnt]->BytesRECV = 0;
	m_pArr[m_nEventCnt]->wsaBuf.len = WSABUF_SIZE;
	m_pArr[m_nEventCnt]->wsaBuf.buf = m_pArr[m_nEventCnt]->Buffer;
	m_pArr[m_nEventCnt]->Overlapped.hEvent = m_hWsaEvents[m_nEventCnt] = WSACreateEvent();


	// Post a WSARecv request to begin receiving data on the socket
	DWORD RecvBytes=0, Flags=0;
	if (WSARecv(m_pArr[m_nEventCnt]->Socket, &(m_pArr[m_nEventCnt]->wsaBuf),
		1, &RecvBytes, &Flags, &(m_pArr[m_nEventCnt]->Overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			SetSockErrMsg("OnAccept.WSARecv");
			return FALSE;;
		}
	}
	m_nEventCnt++;
	g_log.log(LOGTP_SUCC, "Accept. client socket is (%d).Current cnt:%d", m_sockAcpted, m_nEventCnt-1);

	// Make a new socket for accepting future connections and post another AcceptEx call
	return RequestAcept();
}

unsigned WINAPI CTcpSrv::AcptThread(LPVOID lp)
{
	CTcpSrv* p = (CTcpSrv*)lp;
	return p->AcptThreadFn();

}

unsigned CTcpSrv::AcptThreadFn()
{	
	DWORD dwIdx;
	DWORD dwEventIdx;
	DWORD BytesTransferred, Flags;
	//DWORD dw = WSAWaitForMultipleEvents(1 + m_nEventCnt, m_hWsaEvents, FALSE, ACCEPT_TIMEOUT, FALSE);
	while (WaitForSingleObject(m_hDie[IDX_ACPT], 1) != WAIT_OBJECT_0)
	{
		dwIdx = WSAWaitForMultipleEvents( m_nEventCnt, m_hWsaEvents, FALSE, INFINITE, FALSE);
		dwEventIdx = dwIdx - WSA_WAIT_EVENT_0;

		// accept event
		// If the event triggered was zero then a connection attempt was made on our listening socket
		if (dwEventIdx == 0)
		{
			// Check the returns from the overlapped I/O operation on the listening socket
			if (WSAGetOverlappedResult(m_sockListen, &(m_ovAccept), &BytesTransferred, FALSE, &Flags) == FALSE)
			{
				SetSockErrMsg("AcptThreadFn.WSAGetOverlappedResult-1");
				continue;
			}

			g_log.log(LOGTP_SUCC, "Socket %d got connected...", m_sockAcpted);

			if (m_nEventCnt >= WSA_MAXIMUM_WAIT_EVENTS)
			{
				g_log.log(LOGTP_ERR, "Too many connections - closing socket.");
				closesocket(m_sockAcpted);
				RequestAcept();
				continue;
			}
			
			// Create a socket information structure to associate with the accepted socket
			OnAccept();

			continue;

		} // if ((dwIdx - WSA_WAIT_EVENT_0) == 0)

		/////////////////////////////////////////////////////////////////////////////
		// acept event 가 아닌 경우, 즉 WSASend or WSARecv
		WSAResetEvent(m_hWsaEvents[dwEventIdx]);

		if (WSAGetOverlappedResult(m_pArr[dwEventIdx]->Socket, &(m_pArr[dwEventIdx]->Overlapped), 
			&BytesTransferred, 
			FALSE, //  If TRUE, the function does not return until the operation has been completed. If FALSE and the operation is still pending, the function returns FALSE and the WSAGetLastError function returns WSA_IO_INCOMPLETE. 
			&Flags) == FALSE)
		{
			int nErr = GetLastError();
			if (nErr == WSA_IO_INCOMPLETE || nErr == 0)
				continue;

			SetSockErrMsg("AcptThreadFn.WSAGetOverlappedResult-2");
			CloseClient(dwEventIdx);			
			continue;
		}



		// First check to see if the peer has closed the connection and if so
		// then close the socket and cleanup the SOCKET_INFORMATION structure
		// associated with the socket
		if (BytesTransferred == 0 && m_pArr[dwEventIdx]->SendRecvBit == IDX_RECV)
		{
			g_log.log(LOGTP_SUCC, "Closing socket %d", m_pArr[dwEventIdx]->Socket);
			CloseClient(dwEventIdx);
			continue;
		} // if (BytesTransferred == 0)


		// WSARecv 인 경우
		if (m_pArr[dwEventIdx]->SendRecvBit == IDX_RECV)
		{
			m_pArr[dwEventIdx]->BytesRECV = BytesTransferred;

			///////////////////////////////////////////////////////
			// TODO : add packet paring list
			////////////////////////////////////////////////////////

			m_pArr[dwEventIdx]->BytesRECV = 0;
			Flags = 0;
			ZeroMemory(&(m_pArr[dwEventIdx]->Overlapped), sizeof(WSAOVERLAPPED));
			m_pArr[dwEventIdx]->Overlapped.hEvent = m_hWsaEvents[dwEventIdx];
			m_pArr[dwEventIdx]->wsaBuf.len = WSABUF_SIZE;
			m_pArr[dwEventIdx]->wsaBuf.buf = m_pArr[dwEventIdx]->Buffer;

			if (WSARecv(m_pArr[dwEventIdx]->Socket, &(m_pArr[dwEventIdx]->wsaBuf), 1, &BytesTransferred, &Flags, 
				&(m_pArr[dwEventIdx]->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					SetSockErrMsg("AcptThreadFn.WSARecv");
					continue;
				}
			}
		}

		// WSASend 인 경우
		if (m_pArr[dwEventIdx]->SendRecvBit == IDX_SEND)
		{
			//g_log.log(LOGTP_SUCC, "send done.client idx(%d).send len(%d)", dwEventIdx, BytesTransferred);
		}


	} // while (WaitForSingleObject(p->m_hDie[IDX_ACPT], 1) != WAIT_OBJECT_0)
	
	return 1;
}


int CTcpSrv::SetSockErrMsg(const char* pzMsg)
{
	int nErr = GetLastError();
	char msg[1024];
	CUtil::FormatErrMsg(nErr, msg);
	sprintf(m_zMsg, "[%s][%d]%s", pzMsg, nErr, msg);
	g_log.log(LOGTP_ERR, "%s", m_zMsg);
	return nErr;
}

BOOL  CTcpSrv::SendDataAllClient(char* psData, int nDataLen)
{
	for (int i = IDX_ACPT + 1; i < m_nEventCnt; i++)
	{
		SendData(i, psData, nDataLen);
	}
	return TRUE;
}


BOOL  CTcpSrv::SendData(int nIdx, char* psData, int nDataLen)
{
	DWORD SendBytes;

	ZeroMemory(&(m_pArr[nIdx]->Overlapped), sizeof(WSAOVERLAPPED));
	m_pArr[nIdx]->Overlapped.hEvent = m_hWsaEvents[nIdx];
	m_pArr[nIdx]->wsaBuf.buf = psData;
	m_pArr[nIdx]->wsaBuf.len = nDataLen;
	m_pArr[nIdx]->SendRecvBit = IDX_SEND;
	if (WSASend(m_pArr[nIdx]->Socket, &(m_pArr[nIdx]->wsaBuf), 1, &SendBytes, 0,
		&(m_pArr[nIdx]->Overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("WSASend() failed with error %d\n", WSAGetLastError());
			return FALSE;
		}
	}
	else
		printf("WSASend() is OK!\n");

	return TRUE;
}