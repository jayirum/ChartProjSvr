#include "ClientInterface.h"
#include <process.h>
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/Irum_Common.h"
#include "../Common/Algo_PacketHandler.h"
#include "../../IRUM_UTIL/logmsg.h"
#include "../Common/Inc_ETK/ETK_Common.h"
#include "../Common/Inc_ETK/CVT_ETK.h"
#include "../../IRUM_UTIL/mempool.h"

extern char	g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;
extern CMemPool	g_memPool;
extern BOOL		g_bContinue;



CClientInterface::CClientInterface(DWORD dwRealThreadId)
{
	m_dwRealThreadId	= dwRealThreadId;

	m_hCompletionPort	= NULL;
	m_bRun				= TRUE;
	m_hListenEvent		= NULL;;

	m_dwThreadCount = 0;
	m_sockListen	= INVALID_SOCKET;
	m_hListenThread = NULL;
	m_dListenThread = 0;

	m_lbInfo = NULL;
}
CClientInterface::~CClientInterface()
{
	Finalize();
}



BOOL CClientInterface::Initialize( )
{
	g_log.log(INFO, "CClientInterface::Initialize() starts........");

	InitializeCriticalSection(&m_csUser);
	InitializeCriticalSection(&m_csRequest);
	InitializeCriticalSection(&m_csRecvData);

	// Create IOCP
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (INVALID_HANDLE_VALUE == m_hCompletionPort)
	{
		g_log.log(LOGTP_ERR, "IOCP Create Error:%d", GetLastError());
		return FALSE;
	}

	// connect AlgoLB
	m_lbInfo = new CLBInfo;
	if (!m_lbInfo->Initialize(&CClientInterface::SendAllUserInfo))
		return FALSE;

	// link LBInfo with IOCP
	HANDLE h = CreateIoCompletionPort((HANDLE)m_lbInfo->m_sock,
		m_hCompletionPort,
		(DWORD)m_lbInfo,
		0);
	if (h == NULL)
	{
		g_log.log(LOGTP_ERR, "Link LBInfo with IOCP Error:%d", GetLastError());
		closesocket(m_lbInfo->m_sock);
		delete m_lbInfo;
		return FALSE;
	}
	
	m_lbInfo->RequestRecvIO();



	// Init listen socket
	char port[32];
	CUtil::GetConfig(g_zConfig, "CLIENT_IF", "LISTEN_IP", m_zServerIP);
	CUtil::GetConfig(g_zConfig, "CLIENT_IF", "LISTEN_PORT", port);
	m_nListenPort = atoi(port);

	if (!InitListen()) {
		g_log.log(ERR, "Failed to Init Listen");
		return FALSE;
	}

	// packet handler
	m_pack = new CPacketHandler;

	// create iocp worker threads

	SYSTEM_INFO         systemInfo;
	GetSystemInfo(&systemInfo);
	m_dwThreadCount = systemInfo.dwNumberOfProcessors;
	if (m_dwThreadCount > IOCP_THREAD_CNT)
		m_dwThreadCount = IOCP_THREAD_CNT;

	unsigned int dwID;
	for (DWORD n = 0; n<m_dwThreadCount; n++)
	{
		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, &IocpWorkerThread, this, 0, &dwID);
		CloseHandle(h);
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
	for( unsigned int i=0; i<m_dwThreadCount; i++)
	{
		PostQueuedCompletionStatus(
			m_hCompletionPort
			,0
			, (ULONG_PTR)Message
			, NULL
		);
	}
}


void CClientInterface::Finalize()
{
	m_bRun = FALSE;

	// kill all iocp worker threads
	for (UINT i = 0; i < m_dwThreadCount; i++)
	{
		PostQueuedCompletionStatus(
			m_hCompletionPort
			, 0
			, NULL
			, NULL
		);
	}


	// destroy user map
	lock(&m_csUser);
	std::map<std::string, CUserInfo*>::iterator it;
	for (it = m_mapUser.begin(); it != m_mapUser.end(); ++it)
	{
		CUserInfo* pCKey = ((*it).second);
		shutdown(pCKey->sockUser, SD_BOTH);

		// TIME-WAIT 없도록
		struct linger structLinger;
		structLinger.l_onoff = TRUE;
		structLinger.l_linger = 0;
		setsockopt(pCKey->sockUser, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
		closesocket(pCKey->sockUser);
		delete pCKey;
	}
	m_mapUser.clear();
	unlock(&m_csUser);


	// destroy request map
	lock(&m_csRequest);
	std::map<std::string, CUserInfo*>::iterator it2;
	for (it2 = m_mapRequest.begin(); it2 != m_mapRequest.end(); ++it2)
	{
		CUserInfo* pCKey = ((*it2).second);
		shutdown(pCKey->sockUser, SD_BOTH);

		// TIME-WAIT 없도록
		struct linger structLinger;
		structLinger.l_onoff = TRUE;
		structLinger.l_linger = 0;
		setsockopt(pCKey->sockUser, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
		closesocket(pCKey->sockUser);
		delete pCKey;
	}
	m_mapRequest.clear();
	unlock(&m_csRequest);


	// close listen socket
	CloseListenSock();

	// close LBInfo 
	delete m_lbInfo;


	SAFE_CLOSEHANDLE(m_hCompletionPort);

	DeleteCriticalSection(&m_csUser);
	DeleteCriticalSection(&m_csRequest);
	DeleteCriticalSection(&m_csRecvData);
	SAFE_DELETE(m_pack);
	WSACleanup();
}

/*
1) request map 을 찾아서 없으면 ref 도 없으므로 삭제 처리. 유저맵에서 제거
2) request map 에 있으면 ref 확인. 0 이면 삭제. ==> DeleteUser()
3) request map 에 있고 ref >0 이면 status 만 변경
*/
void CClientInterface::OnClose(void *pCompletionKey)
{
	CUserInfo* p = (CUserInfo*)pCompletionKey;
	lock(&m_csRequest);
	std::map<std::string, CUserInfo*>::iterator it2 = m_mapRequest.find(p->userID);
	if (it2 == m_mapRequest.end())
	{
		DeleteUser(p, TRUE, FALSE);
	}
	else
	{
		// ref check
		CUserInfo* p = (*it2).second;
		if (p->hasRefCnt())
		{
			p->markCloseRequested();
			m_mapRequest[p->userID] = p;
		}
		else
		{
			DeleteUser(p, TRUE, FALSE);
		}
	}
	unlock(&m_csRequest);
}

VOID CClientInterface::DeleteUser(CUserInfo* pCK, BOOL bNeedLockUser, BOOL bNeedLockRqst)
{
	// Tell CReal Thread
	UnsubscribeSymbol(pCK->sockUser);


	// USER MAP
	if(bNeedLockUser)	lock(&m_csUser);
	std::map<std::string, CUserInfo*>::iterator it = m_mapUser.find(pCK->userID);
	if (it != m_mapUser.end())
	{
		m_mapUser.erase(it);
	}
	if (bNeedLockUser)	unlock(&m_csUser);


	// REQUEST MAP
	if (bNeedLockRqst)	lock(&m_csRequest);
	std::map<std::string, CUserInfo*>::iterator it2 = m_mapRequest.find(pCK->userID);
	if (it2 != m_mapRequest.end())
	{
		m_mapRequest.erase(it2);
	}
	if (bNeedLockRqst)	unlock(&m_csRequest);

	std::string sUserId = pCK->userID;

	// CLOSE SOCKET
	shutdown(pCK->sockUser, SD_BOTH);

	struct linger structLinger;
	structLinger.l_onoff = TRUE;
	structLinger.l_linger = 0;
	setsockopt(pCK->sockUser, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
	closesocket(pCK->sockUser);
	delete pCK;

	// Tell AlgoLB
	m_lbInfo->TellUserClose((LPSTR)sUserId.c_str());

}

VOID CClientInterface::SubscribeSymbol(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx)
{
	_ALGOINC::ST_1003* p = (_ALGOINC::ST_1003*)pCtx->buf;

	char *pSymbol = g_memPool.get();
	sprintf(pSymbol, "%.*s", sizeof(p->symbolCode), p->symbolCode);

	PostThreadMessage(m_dwRealThreadId, _ALGOINC::WM_RELEASE_CLIENT, (WPARAM)pCK->sockUser, (LPARAM)pSymbol);
}


/*
	try to connect AlgoLB again
*/
void CClientInterface::ReConnLB()
{
	m_lbInfo->ReConnLB();
}


void CClientInterface::SendAllUserInfo()
{
	lock(&m_csRequest);
	std::map<std::string, CUserInfo*>::iterator it2;
	for(it2 = m_mapRequest.begin(); it2!= m_mapRequest.end();it2++)
	{
		m_lbInfo->TellUserLogon((char*)(*it2).first.c_str());
	}
	unlock(&m_csRequest);

	lock(&m_csUser);
	std::map<std::string, CUserInfo*>::iterator it;
	for (it = m_mapRequest.begin(); it != m_mapRequest.end(); it++)
	{
		m_lbInfo->TellUserLogon((char*)(*it).first.c_str());
	}
	unlock(&m_csUser);
}


VOID CClientInterface::UnsubscribeSymbol(SOCKET sockUser)
{
	PostThreadMessage(m_dwRealThreadId, _ALGOINC::WM_RELEASE_CLIENT, (WPARAM)sockUser, (LPARAM)0);
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

	VOID	 *pCKey = NULL;
	_ALGOINC::IO_CONTEXT		 *pIoContext = NULL;
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
			pIoContext = (_ALGOINC::IO_CONTEXT*)pOverlap;

			if (pIoContext->sysTp == _ALGOINC::SYSTP_LB)
			{
				pThis->ReConnLB();
			}

			if (pIoContext->sysTp == _ALGOINC::SYSTP_CLIENT)
			{
				pThis->OnClose(pCKey);
			}

			g_log.log(LOGTP_ERR, "GetQueuedCompletionStatus error:%d", GetLastError());
			Sleep(3000);
			continue;
		}

		// Finalize 에서 PostQueuedCompletionStatus 에 NULL 입력
		if (pOverlap == NULL)
		{
			break;
		}

		pIoContext = (_ALGOINC::IO_CONTEXT*)pOverlap;

		if (dwIoSize == 0)
		{
			if (pIoContext->context == _ALGOINC::CTX_RQST_SEND ||
				pIoContext->context == _ALGOINC::CTX_RQST_RECV
				)
			{
				pIoContext = (_ALGOINC::IO_CONTEXT*)pOverlap;

				if (pIoContext->sysTp == _ALGOINC::SYSTP_LB && pIoContext->context == _ALGOINC::CTX_RQST_RECV)
				{
					pThis->ReConnLB();
				}

				if (pIoContext->sysTp == _ALGOINC::SYSTP_CLIENT && pIoContext->context == _ALGOINC::CTX_RQST_RECV)
				{
					pThis->OnClose(pCKey);
				}
			}
			continue;
		}

		if (pIoContext->context == _ALGOINC::CTX_DIE)
		{
			break;
		}

		// receive market data from inner thread
		if (pIoContext->context == _ALGOINC::CTX_RQST_RECV)
		{
			pThis->m_pack->add(pIoContext->buf, dwIoSize);

			ZeroMemory(pIoContext->buf, sizeof(pIoContext->buf));
			int nBufLen = 0;
			while ((nBufLen = pThis->m_pack->getonepacket(pIoContext->buf)) > 0)
			{
				if (pIoContext->sysTp == _ALGOINC::SYSTP_CLIENT)
				{
					pThis->OnClientRequest((CUserInfo*)pCKey, pIoContext);

					pThis->AfterClientRequest((CUserInfo*)pCKey, pIoContext);

					char* pIoBuffer = g_memPool.get();
					if (!_ALGOINC::RequestRecvIO(((CUserInfo*)pCKey)->sockUser, pIoContext->sysTp, pIoBuffer))
						g_memPool.release(pIoBuffer);
				}
				if (pIoContext->sysTp == _ALGOINC::SYSTP_LB)
				{
					//TODO. 

					((CLBInfo*)pCKey)->RequestRecvIO();
				}
			}			
		}
		
		if (pIoContext->context == _ALGOINC::CTX_RQST_SEND)
		{
			//printf("RequestSendIO returned\n");
		}
		g_memPool.release((char*) pIoContext);

	} // while


	return 0;
}


/*
# 일반 요청을 받으면 => OnRequest
 1) request map 에 있고, closing status 이면 처리하지 않는다.
 2) request map 에 있고, closing status 가 아니면 ref 만 증가
 3) request map 에 없으면, 유저맵에서 제거, request map 에 input. ref 증가
*/
void CClientInterface::OnClientRequest(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx)
{
	BOOL bExist = FALSE;

	lock(&m_csRequest);
	std::map<std::string, CUserInfo*>::iterator it2 = m_mapRequest.find(pCK->userID);
	if (it2 != m_mapRequest.end())
	{
		CUserInfo* p = (*it2).second;
		if (!p->hasMarkedClose())
		{
			p->addRef();
			m_mapRequest[p->userID] = p;
			BOOL bExist = TRUE;
		}
		else
		{
			//do nothing
			g_log.log(INFO, "[OnClientRequest][%s]The user is being closed. Do not proceed this request", pCK->userID.c_str());
		}
	}
	unlock(&m_csRequest);
	if (bExist) return;

	lock(&m_csUser);
	std::map<std::string, CUserInfo*>::iterator it = m_mapUser.find(pCK->userID);
	if (it != m_mapUser.end())
	{
		m_mapUser.erase(it);
		unlock(&m_csUser);

		lock(&m_csRequest);
		pCK->refCnt = 1;
		m_mapRequest[pCK->userID] = pCK;
		unlock(&m_csRequest);
	}
	else
	{
		g_log.log(INFO, "[OnClientRequest][%s]The user is not in the User Map.This is maybe login request", pCK->userID.c_str());
	}
	unlock(&m_csUser);


	//
	ExecuteByRequest(pCK, pCtx);
	//
}



VOID CClientInterface::ExecuteByRequest(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx)
{
	_ALGOINC::HEADER* p = (_ALGOINC::HEADER*)&pCtx->buf[1];
	if (strncmp(p->code, "1001", 4) == 0)
		LoginProc(pCK, pCtx);
	if (strncmp(p->code, "1003", 4) == 0)
		SubscribeSymbol(pCK, pCtx);
}

BOOL CClientInterface::LoginProc(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx)
{
	_ALGOINC::ST_1001* p = (_ALGOINC::ST_1001*)pCtx->buf;

	char zUserID[32], zPwd[32], zMac[32];
	sprintf(zUserID, "%.*s", sizeof(p->userId), p->userId);
	sprintf(zPwd, "%.*s", sizeof(p->pwd), p->pwd);
	sprintf(zMac, "%.*s", sizeof(p->mac), p->mac);

	//DB Process
	char zExtrMsg[128] = { 0, }, tmp[128];
	BOOL bRet = LoginDB(zUserID, zPwd, pCK->clientIp.c_str(), zMac, zExtrMsg);

	// return to client
	_ALGOINC::ST_2003 t;
	memset(&t, 0x20, sizeof(_ALGOINC::ST_2003));
	t.stx[0] = _ALGOINC::STX;
	t.etx[0] = _ALGOINC::ETX;

	memcpy(t.header.len, _ALGOINC::packlen(sizeof(_ALGOINC::ST_2003), tmp), sizeof(t.header.len));
	memcpy(t.header.code, "2003", sizeof(t.header.code));
	
	if (bRet)
	{
		memcpy(t.header.rsltCode, _ALGOINC::rsltCode(_ALGOINC::RT_SUCCESS, tmp), sizeof(t.header.rsltCode));
		memcpy(t.extraMsg, zExtrMsg, min(sizeof(t.extraMsg),strlen(zExtrMsg)));
	}
	else
		memcpy(t.header.rsltCode, "9999", sizeof(t.header.rsltCode));

	memcpy(t.header.tm, _ALGOINC::now(tmp), sizeof(t.header.tm));
	t.header.sysTp[0] = _ALGOINC::SYSTP_FRONT;

	char* pIoBuffer = g_memPool.get();
	if (!_ALGOINC::RequestSendIO(pCK->sockUser, _ALGOINC::SYSTP_FRONT, (char*)&t, sizeof(t), pIoBuffer))
	{
		g_memPool.release(pIoBuffer);
		return FALSE;
	}

	// send to AlgoLB
	m_lbInfo->TellUserLogon(zUserID);

	return TRUE;
}

BOOL CClientInterface::LoginDB(char* pzUserId, char* pzPwd, const char* pzIpAddress, const char* pzMacAddress, char* pzExtraMsg)
{
	//TODO. DB.

	// error. 

	return TRUE;
}


/*
	1. request map 에 없으면 에러
	2. request map 에 있고
		1) closing status 이고, ref 가 0 이 되면 삭제
		2) closing status 이고 ref>0 이면 놔둔다.
		3) 일반 status 이고 ref==0 이면 request map 에서 제거하고 일반map 에 넣는다.
		4) 일반 status 이고 ref>0 이면 놔둔다.
*/
VOID CClientInterface::AfterClientRequest(CUserInfo* pCK, _ALGOINC::IO_CONTEXT* pCtx)
{
	BOOL bExist = FALSE;

	lock(&m_csRequest);

	std::map<std::string, CUserInfo*>::iterator it2 = m_mapRequest.find(pCK->userID);
	if (it2 != m_mapRequest.end())
	{
		CUserInfo* p = (*it2).second;
		p->releaseRef();

		if (p->refCnt == 0)
		{
			if (p->hasMarkedClose()) {
				DeleteUser(p, TRUE, FALSE);
			}
			else
			{
				m_mapRequest.erase(it2);

				lock(&m_csUser);
				m_mapUser[pCK->userID] = p;
				unlock(&m_csUser);
			}
		}
		else
		{
			m_mapRequest[p->userID] = p;
		}

	}//if (it2 != m_mapRequest.end())

	unlock(&m_csRequest);
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

		char zClientIp[32];
		strcpy(zClientIp,inet_ntoa(sinClient.sin_addr));

		//	CK 와 IOCP 연결
		CUserInfo* pCKey = new CUserInfo();
		pCKey->SetInitData(sockClient, "", zClientIp);

		// Device(sock) 과 CP 를 연결시키고, 이때 Key 에는 유저가 원하는 데이터를 넘겨서 i/o 통지때 같이 받을 수 있다.
		HANDLE h = CreateIoCompletionPort((HANDLE)pCKey->sockUser,
			pThis->m_hCompletionPort,
			(DWORD)pCKey,
			0);
		if (h == NULL)
		{
			delete pCKey;
			closesocket(sockClient);
			continue;
		}

		g_log.log(LOGTP_SUCC, "Accept & Add IOCP.socket:%d", sockClient);

		//	최초 RECV IO 요청
		char* pIoBuffer = g_memPool.get();
		if (!_ALGOINC::RequestRecvIO(sockClient, _ALGOINC::SYSTP_CLIENT, pIoBuffer))
		{
			//TODO. LOG
			g_memPool.release(pIoBuffer);
		}

	}//while

	return 0;
}

/*
	모든 CLIENT 에게 다 보낸다.
*/
VOID CClientInterface::SendToClients(char* pSendData, int nDataLen)
{
	lock(&m_csUser);
	std::map<std::string, CUserInfo*>::iterator it;
	for (it = m_mapUser.begin(); it != m_mapUser.end(); it++)
	{
		char* p = g_memPool.get();
		if (!_ALGOINC::RequestSendIO((*it).second->sockUser, _ALGOINC::SYSTP_FRONT, pSendData, nDataLen, p))
			g_memPool.release(p);
	}
	unlock(&m_csUser);
}