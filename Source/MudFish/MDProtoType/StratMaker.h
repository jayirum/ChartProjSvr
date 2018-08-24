#pragma once


//#include "../../IRUM_UTIL/ADOFunc.h"
#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/QueueShm.h"
#include "StratHistManager.h"

typedef struct _SYMBOL_INFO
{
	char	zTickSize[10];
	int		nDotCnt;
}SYMBOL_INFO;

enum { SLPT_NONE=0, SL_BUY, SL_SELL, PT_BUY, PT_SELL};
enum { MARKET_ON=0, MARKET_CLOSING, MARKET_CLOSED};

class CStratMaker : public CBaseThread
{ 
public:
	CStratMaker(char* pzSymbol, char* pzOpenPrc, CMemPool* pMemPool, 
		unsigned dwSaveThread, unsigned dwSendThread, char* pzCloseTM, char* pzMaxSLCnt);
	~CStratMaker();
	//unsigned int GetWorkerThreadId() { return m_dwStratThreadID; }
	unsigned int GetStratThreadId() { return m_dwStratThreadID; }
private:
	//BOOL	LoadSymbolSpec();		// 전략을 위한 공통정보

	VOID	MainFunc();
	VOID	ThreadFunc();

	//BOOL	BeginDB();
	//VOID	EndDB();

	//BOOL	BeginShm();
	//VOID	EndShm();

	//BOOL	BeginSubscribe();
	//VOID	EndSubScribe();

	VOID	StratProc(char* pMarketData);
	VOID	StratClose(char cCurrOpenSide, char* pzCurrPrc, char* pzApiDT, char* pzApiTm);
	VOID	MarketCloseClr();
	char*	GetCloseOrdType(char cCurrOpenSide, char* pzCurrPrc, 
					_Out_ char* pzBasePrc, _Out_ char* pzStratID, _Out_ char* pzClrMsg);
	VOID	StratOpen(char* pzCurrPrc, char* pzApiDT, char* pzApiTm);
	VOID	CheckMarketClosing();
	VOID	SendSaveSignal(_In_ const char* pSignalPacket, int nDataLen);
	

	// work thread
	static unsigned WINAPI StratThread(LPVOID lp);

private:
	CMemPool		*m_pMemPool;
	//CQueueShm		*m_pShmQ;
	//CDBPoolAdo		*m_pDBPool;
	CStratHistManager	*m_stratHist;

	//char			*m_pSignalResult;
	//char			*m_pClientBuff;

	//char			m_zGroupKey[128];
	char			m_zSymbol[128];
	char			m_zOpenPrc[32];
	char			m_zCloseTM[32];
	char			m_zMaxSLCnt[32];
	char			m_zLastCurrPrc[32];
	char			m_szMsg[1024];
	
	unsigned		m_dwSaveThread, m_dwSendThread;
	SYMBOL_INFO		m_SymbolInfo;

	int				m_nMarketStatus;

	//int				m_nChartTp;

	// worker thread
	HANDLE		m_hStratThread;
	HANDLE		m_hWorkDie;
	unsigned int		m_dwStratThreadID;
};


