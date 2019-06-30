#include "LBInfo.h"

#include "../../IRUM_UTIL/logmsg.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../Common/AlgoInc.h"


extern char		g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;
extern BOOL		g_bContinue;
extern CMemPool	g_memPool;
extern char		g_zMyName[128];


CLBInfo::CLBInfo()
{
	m_bConn = FALSE;
}


CLBInfo::~CLBInfo()
{
	Finalize();
}




BOOL CLBInfo::Initialize(RECONNCALLBACK pReconnCallback)
{
	// reconnect 후 CClientInterface 의 콜백함수 호출
	m_ReconnCallBack = pReconnCallback;


	char port[32];
	CUtil::GetConfig(g_zConfig, "LB_INFO", "CONNECT_IP", m_ip);
	CUtil::GetConfig(g_zConfig, "LB_INFO", "CONNECT_PORT", port);
	m_port = atoi(port);

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
	

	return Connect();
}

BOOL	 CLBInfo::Connect()
{
	////	remote address
	SOCKADDR_IN		sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_port);
	sin.sin_addr.s_addr = inet_addr(m_ip);

	// Create a client socket 
	if ((m_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		g_log.log(LOGTP_ERR, "create socket error: %d", WSAGetLastError());
		return FALSE;
	}

	while (g_bContinue)
	{
		if (connect(m_sock, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			m_bConn = FALSE;
			g_log.log(LOGTP_ERR, "failed to connect LB (%d)(IP:%s)(Port:%d)", WSAGetLastError(), m_ip, m_port);
			Sleep(1000);
		}
		else
		{
			m_bConn = TRUE;
			break;
		}
	}

	return m_bConn;
}


void CLBInfo::Finalize()
{
	// TIME-WAIT 없도록
	struct linger structLinger;
	structLinger.l_onoff = TRUE;
	structLinger.l_linger = 0;
	setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (LPSTR)&structLinger, sizeof(structLinger));
	closesocket(m_sock);
}

void CLBInfo::TellUserClose(char* pzUserId)
{
	_ALGOINC::ST_2002 p;

	char t[128];
	memset(&p, 0x20, sizeof(_ALGOINC::ST_2002));
	p.stx[0] = _ALGOINC::STX;
	p.etx[0] = _ALGOINC::ETX;
	
	memcpy(p.header.len,		_ALGOINC::packlen(sizeof(_ALGOINC::ST_2002), t),	sizeof(p.header.len));
	memcpy(p.header.code,		"2002",												sizeof(p.header.code));
	memcpy(p.header.rsltCode,	_ALGOINC::rsltCode(_ALGOINC::RT_SUCCESS, t), sizeof(p.header.rsltCode));
	memcpy(p.header.tm,			_ALGOINC::now(t),									sizeof(p.header.tm));
	p.header.sysTp[0] =			_ALGOINC::SYSTP_FRONT;

	memcpy(p.frontName, g_zMyName,	min(sizeof(p.frontName),strlen(g_zMyName)));
	memcpy(p.userID,	pzUserId,	min(sizeof(p.userID), strlen(pzUserId)));
	p.logOnOff[0] = '0';
	
	char* pIoBuffer = g_memPool.get();
	if (!_ALGOINC::RequestSendIO(m_sock, _ALGOINC::SYSTP_FRONT, (char*)&p, sizeof(p), pIoBuffer))
	{
		g_memPool.release(pIoBuffer);
	}
}


void CLBInfo::TellUserLogon(char* pzUserId)
{
	_ALGOINC::ST_2002 p;

	char t[128];
	memset(&p, 0x20, sizeof(_ALGOINC::ST_2002));
	p.stx[0] = _ALGOINC::STX;
	p.etx[0] = _ALGOINC::ETX;

	memcpy(p.header.len, _ALGOINC::packlen(sizeof(_ALGOINC::ST_2002), t), sizeof(p.header.len));
	memcpy(p.header.code, "2002", sizeof(p.header.code));
	memcpy(p.header.rsltCode, _ALGOINC::rsltCode(_ALGOINC::RT_SUCCESS, t), sizeof(p.header.rsltCode));
	memcpy(p.header.tm, _ALGOINC::now(t), sizeof(p.header.tm));
	p.header.sysTp[0] = _ALGOINC::SYSTP_FRONT;

	memcpy(p.frontName, g_zMyName, min(sizeof(p.frontName), strlen(g_zMyName)));
	memcpy(p.userID, pzUserId, min(sizeof(p.userID), strlen(pzUserId)));
	p.logOnOff[0] = '1';

	char* pIoBuffer = g_memPool.get();
	if (!_ALGOINC::RequestSendIO(m_sock, _ALGOINC::SYSTP_FRONT, (char*)&p, sizeof(p), pIoBuffer))
	{
		g_memPool.release(pIoBuffer);
	}
}


void CLBInfo::RequestRecvIO()
{
	char* pIoBuffer = g_memPool.get();
	if (!_ALGOINC::RequestRecvIO(m_sock, _ALGOINC::SYSTP_LB, pIoBuffer))
		g_memPool.release(pIoBuffer);
}

VOID CLBInfo::ReConnLB()
{
	DWORD dwID;
	CreateThread(NULL, 0, ReconnThread, this, 0, &dwID);
}

DWORD WINAPI CLBInfo::ReconnThread(LPVOID lp)
{
	CLBInfo* p = (CLBInfo*)lp;

	p->MarkDisconnected();

	p->Connect();

	p->m_ReconnCallBack;

	return 0;
}
