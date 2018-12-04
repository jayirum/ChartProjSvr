#pragma once


#include "TradeOption.h"
#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/ChartShmUtil.h"
#include "StratFactorManager.h"
#include "Strat_OpenPrcMatched.h"


enum { SLPT_NONE=0, SL_BUY, SL_SELL, PT_BUY, PT_SELL};
enum { MARKET_NONE=0, MARKET_ON, MARKET_CLOSING, MARKET_CLOSED};

class CStratMaker : public CBaseThread
{ 
public:
	CStratMaker(char* pzSymbol, char* pzArtc, CStratFactorManager* f);
	BOOL Initialize();
	void SetInitInfo(CMemPool* pMemPool,unsigned dwSaveThread, unsigned dwSendThread);	// , char* pzCloseTM, char* pzMaxSLCnt);
	~CStratMaker();
	//unsigned int GetMainThreadId() { return m_dwStratThreadID; }
	unsigned int GetStratThreadId() { return m_dwStratThreadID; }
private:
	//BOOL	LoadSymbolSpec();		// 전략을 위한 공통정보

	VOID	MainFunc();
	VOID	ThreadFunc();
	static unsigned WINAPI StratThread(LPVOID lp);	// work thread
	VOID	StratProc(char* pMarketData);

	//BOOL	BeginDB();
	//VOID	EndDB();

	VOID	APIOrdProc(char* pData);
	VOID	StratClose(char* pzCurrPrc, char* pzApiDT, char* pzApiTm);
	VOID	MarketCloseClr();
	VOID	ClosePosition();
	char*	GetCloseOrdType( char* pzCurrPrc, 
					_Out_ char* pzBasePrc, _Out_ char* pzStratID, _Out_ char* pzClrMsg
					,_Out_ ABOTLOG_NO1 *dblog);
	VOID	StratOpen(char* pzCurrPrc, char* pzApiDT, char* pzApiTm);
	INT		CheckMarketTime();
	VOID	SaveDBLog(_In_ ABOTLOG_NO1* p);
	
	BOOL	InitChartShm();
	VOID	CloseChartShm();

	

	BOOL	TradeOption();

private:
	CMemPool			*m_pMemPool;
	CStratFactorManager	*m_f;
	CChartShmUtil		*m_chart;
	CStrat_OpenPrcMatched	*m_stratOpenMatched;

	char			m_zSymbol[128];
	char			m_zArtc[128];
	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_zLastCurrPrc[32];
	char			m_szMsg[1024];
	
	unsigned		m_dwSaveThread, m_dwSendThread;
	//ST_SYMBOL_INFO	m_SymbolInfo;

	int				m_nMarketStatus;

	// worker thread
	HANDLE			m_hStratThread;
	HANDLE			m_hWorkDie;
	unsigned int	m_dwStratThreadID;

	CTradeOption	*m_option;
};


