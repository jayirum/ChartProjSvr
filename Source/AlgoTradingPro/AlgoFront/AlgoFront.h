#pragma once

#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/TcpClient.h"
#include "../../IRUM_UTIL/BaseThread.h"

class CAlgoFront : public CBaseThread
{
public:
	CAlgoFront();
	~CAlgoFront();

	BOOL Initialize();
	void Finalize();

private:
	VOID ThreadFunc();
	static unsigned WINAPI Thread_ApiTick(LPVOID lp);

	//BOOL	LoadSymbolInfo(BOOL bCreateStrat);
	//void	ReSetSymbolInfo(char *pzSymbol, double dTickVal, double dTickSize, int nDotCnt
	//						, char* pzQty, char* pzStartTM, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt
	//						, double dEntrySpread, double dClrSpread, double dPtPoint);
	//VOID	ResetSymbolInfo(char *pzSymbol);
	//void	SetOpenPrc(char* pzSymbol, char* pzOpePrc);
	void	InitApiSocket(int Idx);
	void	CloseApiSocket(int Idx);
	void	InitMonitorSocket();
	void	CloseMonitorSocket();

private:
	CMemPool		*m_pMemPool;
	CTcpClient		*m_pApiClient;
	//CTcpClient		*m_pMonitorClient;
	//CDBPoolAdo		*m_pDBPool;

	std::string		m_sApiIP;
	int				m_nApiPort;

	//std::string		m_sMonitorIP;
	//int				m_nMonitorPort;

	HANDLE			m_hApiTick, m_hApiOrd, m_hSaveData, m_hMonitor;
	unsigned int	m_unApiTick, m_unApiOrd, m_unSaveData, m_unMonitor;

	BOOL	m_bContinue;
	char	m_zServerID[32];
};

