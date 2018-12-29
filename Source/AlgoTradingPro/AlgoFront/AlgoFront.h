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

	BOOL	LoadSymbolInfo(BOOL bCreateStrat);
	//void	ReSetSymbolInfo(char *pzSymbol, double dTickVal, double dTickSize, int nDotCnt
	//						, char* pzQty, char* pzStartTM, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt
	//						, double dEntrySpread, double dClrSpread, double dPtPoint);
	//VOID	ResetSymbolInfo(char *pzSymbol);
	//void	SetOpenPrc(char* pzSymbol, char* pzOpePrc);
	VOID	ClearStratMap();
	void	InitApiSocket(int Idx);
	void	CloseApiSocket(int Idx);
	void	InitMonitorSocket();
	void	CloseMonitorSocket();

	//void	CalcEstmPL();
	VOID	ApiOrd_Err(char* pPacket);
	VOID	ApiOrd_Acpt(char* pPacket);
	VOID	ApiOrd_RealOrd(char* pPacket);
	VOID	ApiOrd_RealCntr(char* pPacket);


private:
	void	lock() { EnterCriticalSection(&m_csMap); }
	void	unlock() { LeaveCriticalSection(&m_csMap); }
private:
	CMemPool		*m_pMemPool;
	CTcpClient		*m_pApiClient[2];
	CTcpClient		*m_pMonitorClient;
	CDBPoolAdo		*m_pDBPool;

	std::string		m_sApiIP[2];
	int				m_nApiPort[2];

	std::string		m_sMonitorIP;
	int				m_nMonitorPort;

	HANDLE			m_hApiTick, m_hApiOrd, m_hSaveData, m_hMonitor;
	unsigned int	m_unApiTick, m_unApiOrd, m_unSaveData, m_unMonitor;

	MAP_STRAT		m_mapStrat;
	CRITICAL_SECTION	m_csMap;
	BOOL	m_bContinue;
	char	m_zServerID[32];
public:
	void CloseOpenPosition(char *pzSymbol);
	void StartStrategies();
	void StopStrategies();
};

