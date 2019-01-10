#include "ApiClient.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/logmsg.h"

extern BOOL	g_bContinue;
extern char	g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;
extern CMemPool	g_memPool;;



CApiClient::CApiClient(DWORD dwRealThreadId)
{
	m_dwRealThreadId = dwRealThreadId;
	m_apiClient = NULL;
	m_pack = NULL;
}


CApiClient::~CApiClient()
{
	Finalize();
}


BOOL CApiClient::Initialize()
{
	char t[128];
	CUtil::GetConfig(g_zConfig, "API_INFO", "SERVER_ID", m_zApiIp);
	CUtil::GetConfig(g_zConfig, "API_INFO", "SERVER_PORT", t);
	m_nApiPort = atoi(t);

	m_pack = new CPacketHandler;

	CreateThread(NULL, 0, ApiThread, this, 0, &m_dwApiThreadId);
	CreateThread(NULL, 0, TransferThread, this, 0, &m_dwTransferThreadId);

	return FALSE;
}


BOOL CApiClient::InitConnectApi()
{
	if (m_apiClient)
		delete m_apiClient;

	m_apiClient = new CTcpClient("AlgoAPI");

	if (!m_apiClient->Begin(m_zApiIp, m_nApiPort, 10))
	{
		g_log.log(NOTIFY, "API DataFeed Socket Error[%s][%d]", m_zApiIp, m_nApiPort);
		return FALSE;
	}
	g_log.log(INFO, "API DataFeed Socket OK(%s)(%d)", m_zApiIp, m_nApiPort);
	return TRUE;
}
	
VOID CApiClient::Finalize()
{
	SAFE_DELETE(m_apiClient);
	SAFE_DELETE(m_pack);
}

DWORD WINAPI CApiClient::ApiThread(LPVOID lp)
{
	CApiClient* p = (CApiClient*)lp;
	while(g_bContinue)
	{ 
		if (!p->m_apiClient->IsConnected())
		{
			if (!p->InitConnectApi())
			{
				Sleep(3000);
				continue;
			}
			else
			{
				p->m_apiClient->StartRecvData();
			}
		}
		if (p->m_apiClient->HappenedRecvError())
		{
			g_log.log(ERR, "API_ORD DATA RECV ERROR:%s", p->m_apiClient->GetMsg());
			continue;
		}
		char* pBuf = NULL;;
		if (!g_memPool.get(&pBuf))
		{
			g_log.log(ERR, "memory pool get error");
			continue;
		}
		int nLen = p->m_apiClient->GetOneRecvedPacket(pBuf);
		if (nLen == 0) {
			g_memPool.release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			g_log.log(ERR, "API_ORD DATA PAKCET ÀÌ»ף(%s)(%s)", pBuf, p->m_apiClient->GetMsg());
			g_memPool.release(pBuf);
			continue;
		}
		
		///////////////////////////////////////
		// send to CReal
		///////////////////////////////////////
		PostThreadMessage(p->m_dwRealThreadId, _ALGOINC::WM_DATAFEED, (WPARAM)nLen, (LPARAM)pBuf);

	}//while(g_bContinue)

	return 0;
}
	
DWORD WINAPI CApiClient::TransferThread(LPVOID lp)
{
	CApiClient* p = (CApiClient*)lp;
	while (g_bContinue)
	{
	}

	return 0;
}