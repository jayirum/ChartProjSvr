#pragma once

#include "../Common/AlgoInc.h"
#include "../../IRUM_UTIL/TcpClient.h"
#include "../Common/Algo_PacketHandler.h"

class CApiClient
{
private:
	CTcpClient		*m_apiClient;
	CPacketHandler	*m_pack;

	char	m_zApiIp[32];
	int		m_nApiPort;

	DWORD	m_dwRealThreadId;		//CReal Thread ID
	DWORD	m_dwApiThreadId;
	DWORD	m_dwTransferThreadId;
public:
	CApiClient(DWORD dwRealThreadId);
	~CApiClient();

	BOOL	Initialize();
	BOOL	InitConnectApi();
	VOID	Finalize();

	static DWORD WINAPI ApiThread(LPVOID lp);
	static DWORD WINAPI TransferThread(LPVOID lp);
};

