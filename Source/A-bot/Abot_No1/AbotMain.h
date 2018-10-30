#pragma once


#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/TcpClient.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "StratHistManager.h"
#include "StratMaker.h"
#include <map>
#include <string>
#include "../../IRUM_UTIL/adofunc.h"


enum {API_ORD=0, API_TICK};
typedef struct _ST_STRAT
{
	CStratHistManager*	h;
	CStratMaker*		m;
	BOOL				bFirstFeed;
}ST_STRAT;

typedef std::map<std::string, ST_STRAT*>			MAP_STRAT;
typedef std::map<std::string, ST_STRAT*>::iterator	ITMAP_STRAT;


// CAbotMain dialog
class CAbotMain : public CBaseThread
{
// Construction
public:
	CAbotMain();
	~CAbotMain();

	// Generated message map functions
	BOOL Initialize();
	VOID Finalize();

private:
	VOID ThreadFunc();
	static unsigned WINAPI Thread_Monitor(LPVOID lp);
	static unsigned WINAPI Thread_ApiTick(LPVOID lp);
	static unsigned WINAPI Thread_ApiOrd(LPVOID lp);
	static unsigned WINAPI Thread_SaveDBLog(LPVOID lp);


	BOOL	LoadSymbolInfo(BOOL bCreateStrat);
	void	ReSetSymbolInfo(char *pzSymbol, double dTickVal, double dTickSize, int nDotCnt
							, char* pzQty, char* pzStartTM, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt
							, double dEntrySpread, double dClrSpread, double dPtPoint);
	void	SetOpenPrc(char* pzSymbol, char* pzOpePrc);
	VOID	ClearStratMap();
	void	showMsg(BOOL bSucc, char* pMsg, ...);
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

	MAP_STRAT				m_mapStrat;
	
	BOOL	m_bContinue;
public:
	void CloseOpenPosition(char *pzSymbol);
	void StartStrategies();
	void StopStrategies();
	
};
