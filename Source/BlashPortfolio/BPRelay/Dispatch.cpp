#include "Dispatch.h"
#include <process.h>
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"

extern CLogMsg	g_log;
extern char		g_zConfig[_MAX_PATH];

CDispatch::CDispatch()
{
	m_hCompletionPort	= NULL;
	m_hListenEvent		= NULL;
	m_sockListen		= INVALID_SOCKET;
	m_hListenThread		= NULL;
	m_dListenThread		= 0;
	m_bRun				= TRUE;
}
CDispatch::~CDispatch()
{
	Finalize();
}

BOOL CDispatch::ReadIPPOrt()
{
	char zTemp[1024] = { 0, };
	CUtil::GetConfig(g_zConfig, "NETWORK", "LISTEN_IP", m_zListenIP);
	CUtil::GetConfig(g_zConfig, "NETWORK", "LISTEN_PORT", zTemp);
	m_nListenPort = atoi(zTemp);
	
	return TRUE;
}

BOOL CDispatch::Initialize( )
{
	ReadIPPOrt();

	InitializeCriticalSection(&m_csCK);
	InitializeCriticalSection(&m_csRecvData);
	

	if (!InitListen()) {
		g_log.log(NOTIFY, "Init Listen Failed");
		return FALSE;
	}
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, &ListenThread, this, 0, &m_dListenThread);



	//m_pack = new CPacketHandler;

	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (INVALID_HANDLE_VALUE == m_hCompletionPort)
	{
		g_log.log(LOGTP_ERR, "IOCP Create Error:%d", GetLastError());
		return FALSE;
	}

	// 실제로 recv와 send를 담당할 스레드를 생성한다.
	unsigned int dwID;
	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, &IocpWorkerThread, this, 0, &dwID);
	CloseHandle(h);
	return TRUE;
}

BOOL CDispatch::InitListen()
{
	g_log.log(INFO, "CDispatch::InitListen() starts..........");
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
		g_log.log(NOTIFY, "create socket error: %d", WSAGetLastError());
		return FALSE;
	}

	ReadIPPOrt();

	SOCKADDR_IN InternetAddr;
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = inet_addr(m_zListenIP);
	InternetAddr.sin_port = htons(m_nListenPort);

	BOOL opt = TRUE;
	int optlen = sizeof(opt);
	setsockopt(m_sockListen, SOL_SOCKET, SO_REUSEADDR, (const char far *)&opt, optlen);


	if (::bind(m_sockListen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		g_log.log(NOTIFY, "bind error (ip:%s) (port:%d) (err:%d)", m_zListenIP, m_nListenPort, WSAGetLastError());
		return FALSE;
	}
	// Prepare socket for listening 
	if (listen(m_sockListen, 5) == SOCKET_ERROR)
	{
		g_log.log(NOTIFY, "listen error: %d", WSAGetLastError());
		return FALSE;
	}

	m_hListenEvent = WSACreateEvent();
	if (WSAEventSelect(m_sockListen, m_hListenEvent, FD_ACCEPT)) {

		g_log.log(NOTIFY, "WSAEventSelect for accept error: %d", WSAGetLastError());
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


void CDispatch::CloseListenSock()
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

VOID CDispatch::SendMessageToIocpThread(int Message)
{
	for( int i=0; i<WORKTHREAD_CNT; i++)
	{
		PostQueuedCompletionStatus(
			m_hCompletionPort
			,0
			, (ULONG_PTR)Message
			, NULL
		);
	}
}

void CDispatch::Finalize()
{
	m_bRun = FALSE;

	for (UINT i = 0; i < WORKTHREAD_CNT; i++)
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
		COMPLETION_KEY* pCK = (*it).second;
		if (pCK->pData) {
			delete[] pCK->pData;
		}
		shutdown(pCK->sock, SD_BOTH);

		// TIME-WAIT 없도록
		struct linger structLinger;
		structLinger.l_onoff = TRUE;
		structLinger.l_linger = 0;
		setsockopt(pCK->sock, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
		closesocket(pCK->sock);
		delete pCK;
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
	//SAFE_DELETE(m_pack);
	WSACleanup();
}

void CDispatch::DeleteSocket(COMPLETION_KEY *pCompletionKey)
{
	lockCK();
	char zSock[32];
	sprintf(zSock, "%d", pCompletionKey->sock);

	std::map<std::string, COMPLETION_KEY*>::iterator it = m_mapCK.find(std::string(zSock));
	if (it == m_mapCK.end()) {
		unlockCK();
		return;
	}

	if (pCompletionKey->pData) {
		delete[] pCompletionKey->pData;
	}
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
unsigned WINAPI CDispatch::IocpWorkerThread(LPVOID lp)
{
	CDispatch* pThis = (CDispatch*)lp;

	COMPLETION_KEY	 *pCompletionKey = NULL;
	IO_CONTEXT		 *pIoContext = NULL;
	DWORD				 dwBytesTransferred = 0;
	DWORD                dwIoSize = 0;
	DWORD                dwRecvNumBytes = 0;
	DWORD                dwSendNumBytes = 0;
	DWORD                dwFlags = 0;
	LPOVERLAPPED		 pOverlap = NULL;
	BOOL bRet;
	
	g_log.log(LOGTP_SUCC, "[%d]IOCPThread Start.....", GetCurrentThreadId());

	while (pThis->m_bRun)
	{
		bRet = GetQueuedCompletionStatus(pThis->m_hCompletionPort,
			&dwIoSize,
			(LPDWORD)&pCompletionKey, 	//여기로는 실제 CK 는 던지지 않는다. 무조건 new, delete 하므로. 
			(LPOVERLAPPED *)&pOverlap,
			INFINITE);

		// Finalize 에서 PostQueuedCompletionStatus 에 NULL 입력
		if (pCompletionKey == NULL) // 종료
		{
			break;
		}

		if (FALSE == bRet)
		{
			pThis->DeleteSocket(pCompletionKey);
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
			if (pIoContext->context == CTX_RQST_SEND || pIoContext->context == CTX_RQST_RECV)
				pThis->DeleteSocket(pCompletionKey);
			continue;
		}
		
		if (pIoContext->context == CTX_DIE)
		{
			break;
		}

		// Master / Slave 로 부터 데이터 수신
		if (pIoContext->context == CTX_RQST_RECV)
		{
			pThis->RequestRecvIO(pCompletionKey);

			//pThis->m_pack->add(pIoContext->buf, dwIoSize);

			//TODO. Dispatch
			int nBufLen = 0;
			g_log.log(LOGTP_SUCC, "----------------------------------------");
			g_log.log(LOGTP_SUCC, "[FROM CLIENT][%d](%.*s)", GetCurrentThreadId(), nBufLen, pIoContext->buf);
			//TODO pThis->Deliever_Biz_ToMT4(mt4Helper, pIoContext);
		}

		if (pIoContext->context == CTX_RQST_SEND)
		{
			//printf("RequestSendIO returned\n");
		}
		delete pIoContext;

	} // while

	//mt4helper.disconnect();

	return 0;
}


unsigned WINAPI CDispatch::ListenThread(LPVOID lp)
{
	CDispatch *pThis = (CDispatch*)lp;

	SOCKADDR_IN			sinClient;
	int	sinSize = sizeof(sinClient);
	long nLoop = 0;
	//int nTimeout = 1000 * 60;	//1min
	int nHearbeatCnt = 0;
	while (pThis->m_bRun)
	{
		//DWORD dw = WSAWaitForMultipleEvents(1, &pThis->m_hListenEvent, TRUE, HEARTBEAT_TIMEOUT, FALSE);
		DWORD dw = WSAWaitForMultipleEvents(1, &pThis->m_hListenEvent, TRUE, 10, FALSE);
		if (dw != WSA_WAIT_EVENT_0) 
		{
			if (dw == WSA_WAIT_TIMEOUT)
			{
				//pThis->SendMessageToIocpThread(CTX_MT4PING);
				//if (++nHearbeatCnt > 30)
				//{
				//	pThis->api_hearbeat();
				//	printf("heartbeat.........\n");
				//	nHearbeatCnt = 0;
				//}
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
		COMPLETION_KEY* pCK = new COMPLETION_KEY;
		ZeroMemory(pCK, sizeof(COMPLETION_KEY));
		pCK->sock = sockClient;

		HANDLE h = CreateIoCompletionPort((HANDLE)pCK->sock,
			pThis->m_hCompletionPort,
			(DWORD)pCK,
			0);
		if (h == NULL)
		{
			delete pCK;
			closesocket(sockClient);
			continue;
		}

		char zSocket[32]; sprintf(zSocket, "%d", sockClient);
		pThis->lockCK();
		pThis->m_mapCK[std::string(zSocket)] = pCK;
		pThis->unlockCK();

		g_log.log(LOGTP_SUCC, "Accept & Add IOCP.socket:%d", sockClient);

		//	최초 RECV IO 요청
		pThis->RequestRecvIO(pCK);

		//pThis->SendMessageToIocpThread(CTX_MT4PING);

	}//while

	return 0;
}


/*
	클라이언트 시스템에 대한 구분이 없으니 (BIZ, etc...) 일단 
	모든 CLIENT 에게 다 보낸다.
*/
//VOID CDispatch::SendToBiz(char* pSendData, int nDataLen)
//{
//	lockCK();
//	std::map<std::string, COMPLETION_KEY*>::iterator it;
//	for (it = m_mapCK.begin(); it != m_mapCK.end(); it++)
//	{
//		RequestSendIO((*it).second, pSendData, nDataLen);
//	}
//	unlockCK();
//}

VOID CDispatch::RequestSendIO(COMPLETION_KEY* pCK, char* pSendBuf, int nSendLen)
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

		int nRet = WSASend(pCK->sock
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


void  CDispatch::RequestRecvIO(COMPLETION_KEY* pCK)
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


		int nRet = WSARecv(pCK->sock
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
