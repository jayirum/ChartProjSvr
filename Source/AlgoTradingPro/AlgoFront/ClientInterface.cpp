#include "stdafx.h"
#include "ClientInterface.h"
#include <process.h>
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/Irum_Common.h"
#include "../Common/Algo_PacketHandler.h"
#include "../../IRUM_UTIL/logmsg.h"
#include "../Common/Inc_ETK/ETK_Common.h"
#include "../Common/Inc_ETK/CVT_ETK.h"

extern char	g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;

CClientInterface::CClientInterface()
{
	m_hCompletionPort = NULL;
	m_bRun = TRUE;
	m_hListenEvent = NULL;;

	m_dwThreadCount = 0;
	m_sockListen = INVALID_SOCKET;
	m_hListenThread = NULL;
	m_dListenThread = 0;
}
CClientInterface::~CClientInterface()
{
	Finalize();
}


BOOL CClientInterface::Initialize( )
{
	g_log.log(INFO, "CClientInterface::Initialize() starts........");
	char port[32];
	CUtil::GetConfig(g_zConfig, "CLIENT_IF", "LISTEN_IP", m_zServerIP);
	CUtil::GetConfig(g_zConfig, "CLIENT_IF", "LISTEN_PORT", port);
	m_nListenPort = atoi(port);


	//m_nOmniAcnt = nOmniAcnt;

	InitializeCriticalSection(&m_csCK);
	InitializeCriticalSection(&m_csRecvData);

	if (!InitListen()) {
		g_log.log(ERR, "Failed to Init Listen");
		return FALSE;
	}

	m_pack = new CPacketHandler;

	// CPU의 수를 알기 위해서....IOCP에서 사용할 쓰레드의 수는 cpu개수 또는 cpu*2
	SYSTEM_INFO         systemInfo;
	GetSystemInfo(&systemInfo);
	m_dwThreadCount = systemInfo.dwNumberOfProcessors;

	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (INVALID_HANDLE_VALUE == m_hCompletionPort)
	{
		g_log.log(LOGTP_ERR, "IOCP Create Error:%d", GetLastError());
		return FALSE;
	}

	// 실제로 recv와 send를 담당할 스레드를 생성한다.
	unsigned int dwID;
	for (DWORD n = 0; n<m_dwThreadCount; n++)
	{
		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, &IocpWorkerThread, this, 0, &dwID);
		CloseHandle(h);
	}


	// Create & Bind IOCP Key for Inner Data Transfer
	if (!Inner_Init()) {
		g_log.log(ERR, "Failed to Init Inner IOCP Key");
		return FALSE;
	}


	return TRUE;
}

BOOL CClientInterface::Inner_Init()
{
	//	CK 와 IOCP 연결
	m_pInnerCKey = new COMPLETION_KEY;
	ZeroMemory(m_pInnerCKey, sizeof(COMPLETION_KEY));
	m_pInnerCKey->sock = m_innerSock = socket(AF_INET, SOCK_STREAM, 0);
	m_pInnerCKey->clientTp = CLIENT_TP_INNER;

	// Device(sock) 과 CP 를 연결시키고, 이때 Key 에는 유저가 원하는 데이터를 넘겨서 i/o 통지때 같이 받을 수 있다.
	HANDLE h = CreateIoCompletionPort((HANDLE)m_pInnerCKey->sock,
		m_hCompletionPort,
		(DWORD)m_pInnerCKey,
		0);
	if (h == NULL)
	{
		delete m_pInnerCKey;
		closesocket(m_innerSock);
		return FALSE;
	}
	return TRUE;
}

BOOL CClientInterface::InitListen()
{
	g_log.log(INFO, "CClientInterface::InitListen() starts..........");
	CloseListenSock();

	WORD	wVersionRequired;
	WSADATA	wsaData;

	//// WSAStartup
	wVersionRequired = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequired, &wsaData))
	{
		g_log.log(LOGTP_ERR, "WSAStartup Error:%d", GetLastError());
		return FALSE;
	}

	//DumpWsaData(&wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		g_log.log(LOGTP_ERR, "RequiredVersion not Usable");
		return FALSE;
	}


	// Create a listening socket 
	if ((m_sockListen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		g_log.log(LOGTP_ERR, "create socket error: %d", WSAGetLastError());
		return FALSE;
	}

	SOCKADDR_IN InternetAddr;
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = inet_addr(m_zServerIP);
	InternetAddr.sin_port = htons(m_nListenPort);

	if (::bind(m_sockListen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		g_log.log(LOGTP_ERR, "bind error (ip:%s) (port:%d) (err:%d)", m_zServerIP, m_nListenPort, WSAGetLastError());
		return FALSE;
	}
	// Prepare socket for listening 
	if (listen(m_sockListen, 5) == SOCKET_ERROR)
	{
		g_log.log(LOGTP_ERR, "listen error: %d", WSAGetLastError());
		return FALSE;
	}

	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, &ListenThread, this, 0, &m_dListenThread);

	m_hListenEvent = WSACreateEvent();
	if (WSAEventSelect(m_sockListen, m_hListenEvent, FD_ACCEPT)) {

		g_log.log(LOGTP_ERR, "WSAEventSelect for accept error: %d", WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

/*
BOOL WINAPI PostQueuedCompletionStatus(
_In_     HANDLE       CompletionPort,
_In_     DWORD        dwNumberOfBytesTransferred,
_In_     ULONG_PTR    dwCompletionKey,
_In_opt_ LPOVERLAPPED lpOverlapped
);
*/


void CClientInterface::CloseListenSock()
{
	SAFE_CLOSEHANDLE(m_hListenEvent);
	if (m_sockListen != INVALID_SOCKET) {
		struct linger ling;
		ling.l_onoff = 1;   // 0 ? use default, 1 ? use new value
		ling.l_linger = 0;  // close session in this time
		setsockopt(m_sockListen, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
		//-We can avoid TIME_WAIT on both of client and server side as we code above.
		closesocket(m_sockListen);
	}
	SAFE_CLOSESOCKET(m_sockListen);
}

VOID CClientInterface::SendMessageToIocpThread(int Message)
{
	for( int i=0; i<m_dwThreadCount; i++)
	{
		PostQueuedCompletionStatus(
			m_hCompletionPort
			,0
			, (ULONG_PTR)Message
			, NULL
		);
	}
}

VOID CClientInterface::Inner_PostData(char* pData, int nDataLen, BOOL bTick)
{
	IO_CONTEXT* pSend = new IO_CONTEXT;

	ZeroMemory(pSend, sizeof(IO_CONTEXT));
	CopyMemory(pSend->buf, pData, nDataLen);
	//	pSend->sock	= sock;
	pSend->wsaBuf.buf = pSend->buf;
	pSend->wsaBuf.len = nDataLen;

	if(bTick)	pSend->context = CTX_RQST_INNER_TICK;
	else		pSend->context = CTX_RQST_INNER_QUOTE;

	PostQueuedCompletionStatus(
		m_hCompletionPort
		, nDataLen
		, (ULONG_PTR)m_pInnerCKey
		, (LPOVERLAPPED)pSend
	);
}

void CClientInterface::Finalize()
{
	m_bRun = FALSE;

	for (UINT i = 0; i < m_dwThreadCount; i++)
	{
		PostQueuedCompletionStatus(
			m_hCompletionPort
			, 0
			, NULL
			, NULL
		);
	}

	lockCK();
	std::map<std::string, COMPLETION_KEY*>::iterator it;
	for (it = m_mapCK.begin(); it != m_mapCK.end(); ++it)
	{
		COMPLETION_KEY* pCKey = (*it).second;
		shutdown(pCKey->sock, SD_BOTH);

		// TIME-WAIT 없도록
		struct linger structLinger;
		structLinger.l_onoff = TRUE;
		structLinger.l_linger = 0;
		setsockopt(pCKey->sock, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
		closesocket(pCKey->sock);
		delete pCKey;
	}
	m_mapCK.clear();
	unlockCK();

	CloseListenSock();

	//lockRecvData();
	//m_lstRecvData.clear();
	//unlockRecvData();
	SAFE_CLOSEHANDLE(m_hCompletionPort);

	DeleteCriticalSection(&m_csCK);
	DeleteCriticalSection(&m_csRecvData);
	SAFE_DELETE(m_pack);
	WSACleanup();
}

void CClientInterface::DeleteSocket(COMPLETION_KEY *pCompletionKey)
{
	lockCK();
	char zSock[32];
	sprintf(zSock, "%d", pCompletionKey->sock);

	std::map<std::string, COMPLETION_KEY*>::iterator it = m_mapCK.find(std::string(zSock));
	if (it == m_mapCK.end())
		return;

	shutdown(pCompletionKey->sock, SD_BOTH);

	// TIME-WAIT 없도록
	struct linger structLinger;
	structLinger.l_onoff = TRUE;
	structLinger.l_linger = 0;
	setsockopt(pCompletionKey->sock, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
	closesocket(pCompletionKey->sock);
	delete pCompletionKey;
	m_mapCK.erase(it);
	unlockCK();
}



/*
1.	WSASend / WSARecv 를 호출한 경우는 사용한 socket 과 CK 가 연결되어 있으므로
pCompletionKey 를 통해서 CK 의 포인터가 나온다.

2.	PostQueuedCompletionStatus 를 호출한 경우는 socket 이 사용되지 않으므로
이때는 WM_MSG 를 보내도록 한다.

3.	확장된 OVERLAPPED 에 context 필드가 있으므로 여기에 CTX_DIE, CTX_RQST_SEND, CTX_RQST_RECV 를 채워서 보낸다.

*/
unsigned WINAPI CClientInterface::IocpWorkerThread(LPVOID lp)
{
	CClientInterface* pThis = (CClientInterface*)lp;

	COMPLETION_KEY	 *pCKey = NULL;
	IO_CONTEXT		 *pIoContext = NULL;
	DWORD				 dwBytesTransferred = 0;
	DWORD                dwIoSize = 0;
	DWORD                dwRecvNumBytes = 0;
	DWORD                dwSendNumBytes = 0;
	DWORD                dwFlags = 0;
	LPOVERLAPPED		 pOverlap = NULL;
	BOOL bRet;
	int nDataCnt = 0;
	
	while (pThis->m_bRun)
	{
		bRet = GetQueuedCompletionStatus(pThis->m_hCompletionPort,
			&dwIoSize,
			(LPDWORD)&pCKey, 	//여기로는 실제 CK 는 던지지 않는다. 무조건 new, delete 하므로. 
			(LPOVERLAPPED *)&pOverlap,
			INFINITE);

		// Finalize 에서 PostQueuedCompletionStatus 에 NULL 입력
		if (pCKey == NULL) // 종료
		{
			break;
		}
		
		if (FALSE == bRet)
		{
			pThis->DeleteSocket(pCKey);
			g_log.log(LOGTP_ERR, "GetQueuedCompletionStatus error:%d", GetLastError());
			Sleep(3000);
			continue;
		}

		// Finalize 에서 PostQueuedCompletionStatus 에 NULL 입력
		if (pOverlap == NULL)
		{
			break;
		}

		pIoContext = (IO_CONTEXT*)pOverlap;

		if (dwIoSize == 0)
		{
			if (pIoContext->context == CTX_RQST_SEND || pIoContext->context == CTX_RQST_RECV
				|| pIoContext->context == CTX_RQST_INNER_TICK || pIoContext->context == CTX_RQST_INNER_QUOTE
				)
			{
				pThis->DeleteSocket(pCKey);
			}
			continue;
		}

		if (pIoContext->context == CTX_DIE)
		{
			break;
		}

		// receive market data from inner thread
		if (pIoContext->context == CTX_RQST_RECV || pIoContext->context == CTX_RQST_INNER_TICK || pIoContext->context == CTX_RQST_INNER_QUOTE)
		{
			//TODO
			pThis->m_pack->add(pIoContext->buf, dwIoSize);

			ZeroMemory(pIoContext->buf, sizeof(pIoContext->buf));
			int nBufLen = 0;
			while ((nBufLen = pThis->m_pack->getonepacket(pIoContext->buf)) > 0)
			{
				if (nDataCnt++ > 1000) {
					g_log.log(LOGTP_SUCC, "[%d][RECV](%.*s)", GetCurrentThreadId(), nBufLen, pIoContext->buf);
					nDataCnt = 0;
				}
				if (pIoContext->context != CTX_RQST_INNER_TICK && pIoContext->context != CTX_RQST_INNER_QUOTE)
				{
					if (!pThis->CheckLogin(pCKey, pIoContext))
						continue;
				}
				pThis->SendDataFeed(pIoContext);
			}
			if(pCKey->clientTp == CTX_RQST_RECV)
				pThis->RequestRecvIO(pCKey);
		}
		

		if (pIoContext->context == CTX_RQST_SEND)
		{
			//printf("RequestSendIO returned\n");
		}
		delete pIoContext;

	} // while


	return 0;
}


BOOL CClientInterface::CheckLogin(COMPLETION_KEY* pCK, IO_CONTEXT* pCtx)
{
	// Already logged on
	if (pCK->bLoggedOn)
		return TRUE;

	ALGO_LOGIN* p = (ALGO_LOGIN*)pCtx->buf;
	if (strncmp(p->Header.Code, CD_PACKET_LOGIN, sizeof(p->Header.Code)))
	{
		g_log.log(ERR, "This client must loggon first");
		return FALSE;
	}

	if (strncmp(p->Pwd, DEF_PASSWORD, strlen(DEF_PASSWORD)))
		return FALSE;

	sprintf(pCK->UserID, "%.*s", SIZE_USERID, p->UserID);

	return TRUE;
}

unsigned WINAPI CClientInterface::ListenThread(LPVOID lp)
{
	CClientInterface *pThis = (CClientInterface*)lp;

	SOCKADDR_IN			sinClient;
	int	sinSize = sizeof(sinClient);
	long nLoop = 0;
	int nTimeout = 1000 * 10;	//10 secs
	while (pThis->m_bRun)
	{
		DWORD dw = WSAWaitForMultipleEvents(1, &pThis->m_hListenEvent, TRUE, nTimeout, FALSE);
		if (dw != WSA_WAIT_EVENT_0) 
		{
			if (dw == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
		}

		WSAResetEvent(pThis->m_hListenEvent);		
		
		SOCKET sockClient = accept(pThis->m_sockListen, (LPSOCKADDR)&sinClient, &sinSize);
		if (sockClient == INVALID_SOCKET) {
			int nErr = WSAGetLastError();
			g_log.log(LOGTP_ERR, "accept error:%d", nErr);
			//if (nErr==10038) 
			{ // Socket operation on nonsocket.
				pThis->InitListen();
				Sleep(3000);
			}
			continue;
		}

		int nZero = 0;
		if (SOCKET_ERROR == setsockopt(sockClient, SOL_SOCKET, SO_SNDBUF, (const char*)&nZero, sizeof(int)))
		{
			shutdown(sockClient, SD_SEND);
			closesocket(sockClient);
			g_log.log(LOGTP_ERR, "setsockopt error : %d", WSAGetLastError);
			continue;;
		}


		//	CK 와 IOCP 연결
		COMPLETION_KEY* pCKey = new COMPLETION_KEY;
		ZeroMemory(pCKey, sizeof(COMPLETION_KEY));
		pCKey->sock = sockClient;
		pCKey->clientTp = CLIENT_TP_OUTER;

		// Device(sock) 과 CP 를 연결시키고, 이때 Key 에는 유저가 원하는 데이터를 넘겨서 i/o 통지때 같이 받을 수 있다.
		HANDLE h = CreateIoCompletionPort((HANDLE)pCKey->sock,
			pThis->m_hCompletionPort,
			(DWORD)pCKey,
			0);
		if (h == NULL)
		{
			delete pCKey;
			closesocket(sockClient);
			continue;
		}

		char zSocket[32]; sprintf(zSocket, "%d", sockClient);
		pThis->lockCK();
		pThis->m_mapCK[std::string(zSocket)] = pCKey;
		pThis->unlockCK();

		g_log.log(LOGTP_SUCC, "Accept & Add IOCP.socket:%d", sockClient);

		//	최초 RECV IO 요청
		pThis->RequestRecvIO(pCKey);

	}//while

	return 0;
}

VOID CClientInterface::SendDataFeed(IO_CONTEXT* pCtx)
{
	char szSendBuf[1024];
	int nSendLen = 0;
	CCVT_ETK cvt;

	//////////////////////////////////////////////////////////////////////////
	//	TICK DATA
	if (pCtx->context== CTX_RQST_INNER_TICK)
	{
		double dErrSise = 0;
		ETK_CME_TICK* pRecv = (ETK_CME_TICK*)pCtx->buf;
		nSendLen = cvt.de_FU_SiseEx((char*)&pRecv->etkTick, szSendBuf, &dErrSise);
		//g_log.log(INFO, "[SEND TICK](%.*s)", nSendLen, szSendBuf);
	}

	///////////////////////////////////////////////////////////////////////////
	//	QUOTE DATA
	if (pCtx->context == CTX_RQST_INNER_QUOTE)
	{
		ETK_CME_QUOTE* pRecv = (ETK_CME_QUOTE*)pCtx->buf;
		nSendLen = cvt.de_FU_Hoga((CHAR*)&pRecv->etkQuote, szSendBuf);
		//g_log.log(INFO, "[SEND QUOTE](%.*s)", nSendLen, szSendBuf);
	}

	//TODO SendToClients(szSendBuf, nSendLen);
}

/*
	모든 CLIENT 에게 다 보낸다.
*/
VOID CClientInterface::SendToClients(char* pSendData, int nDataLen)
{
	lockCK();
	std::map<std::string, COMPLETION_KEY*>::iterator it;
	for (it = m_mapCK.begin(); it != m_mapCK.end(); it++)
	{
		RequestSendIO((*it).second, pSendData, nDataLen);
	}
	unlockCK();
}

VOID CClientInterface::RequestSendIO(COMPLETION_KEY* pCKey, char* pSendBuf, int nSendLen)
{

	BOOL  bRet = TRUE;
	DWORD dwOutBytes = 0;
	DWORD dwFlags = 0;
	IO_CONTEXT* pSend = NULL;

	try {
		pSend = new IO_CONTEXT;

		ZeroMemory(pSend, sizeof(IO_CONTEXT));
		CopyMemory(pSend->buf, pSendBuf, nSendLen);
		//	pSend->sock	= sock;
		pSend->wsaBuf.buf = pSend->buf;
		pSend->wsaBuf.len = nSendLen;
		pSend->context = CTX_RQST_SEND;

		int nRet = WSASend(pCKey->sock
			, &pSend->wsaBuf	// wsaBuf 배열의 포인터
			, 1					// wsaBuf 포인터 갯수
			, &dwOutBytes		// 전송된 바이트 수
			, dwFlags
			, &pSend->overLapped	// overlapped 포인터
			, NULL);
		if (nRet == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				g_log.log(LOGTP_ERR, "WSASend error : %d", WSAGetLastError);
				bRet = FALSE;
			}
		}
		//printf("WSASend ok..................\n");
	}
	catch (...) {
		//TODO PrintMsg(0, "WSASend try catch error [CSession]");
		bRet = FALSE;
	}
	if (!bRet)
		delete pSend;

	return;
}


void  CClientInterface::RequestRecvIO(COMPLETION_KEY* pCKey)
{
	IO_CONTEXT* pRecv = NULL;
	DWORD dwNumberOfBytesRecvd = 0;
	DWORD dwFlags = 0;

	BOOL bRet = TRUE;
	try {
		pRecv = new IO_CONTEXT;
		ZeroMemory(pRecv, CONTEXT_SIZE);
		//ZeroMemory( &(pRecv->overLapped), sizeof(WSAOVERLAPPED));
		pRecv->wsaBuf.buf = pRecv->buf;
		pRecv->wsaBuf.len = BUF_LEN;
		pRecv->context = CTX_RQST_RECV;


		int nRet = WSARecv(pCKey->sock
			, &(pRecv->wsaBuf)
			, 1, &dwNumberOfBytesRecvd, &dwFlags
			, &(pRecv->overLapped)
			, NULL);
		if (nRet == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				g_log.log(LOGTP_ERR, "WSARecv error : %d", WSAGetLastError);
				bRet = FALSE;
			}
		}
	}
	catch (...) {
		//TODO PrintMsg(0, "RequestRecvIO : WSASend TRY CATCH [CSession]");
		bRet = FALSE;
	}

	if (!bRet)
		delete pRecv;

	return;
}
