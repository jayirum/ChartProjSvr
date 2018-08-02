#pragma once


#include "../../IRUM_UTIL/ADOFunc.h"
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

class CSignalMaker : public CBaseThread
{ 
public:
	CSignalMaker(char* pzSymbol, char* pzArtcCode, CMemPool* pMemPool, /*CQueueShm* pShm,*/ unsigned dwSaveThread, unsigned dwSendThread);
	~CSignalMaker();

private:
	BOOL	LoadSymbolSpec();		// 전략을 위한 공통정보

	VOID	MainFunc();
	VOID	ThreadFunc();

	BOOL	BeginDB();
	VOID	EndDB();

	BOOL	BeginShm();
	VOID	EndShm();

	//BOOL	BeginSubscribe();
	//VOID	EndSubScribe();

	VOID	SignalProc(char* pMarketData);
	
	INT		GetCurrChartShm(char* pzGroupKey, char* pzChartNm, _Out_ ST_SHM_CHART_UNIT& shmPrev, _Out_ ST_SHM_CHART_UNIT& shmNow);

	VOID	StratClose(char* pzGroupKey, char* pzChartNm, _In_ ST_SHM_CHART_UNIT& shmPrev, _In_ ST_SHM_CHART_UNIT& shmNow, char* pzCurrPrc);
	VOID	StratOpen(char* pzGroupKey, char* pzChartNm, _In_ ST_SHM_CHART_UNIT& shmPrev, _In_ ST_SHM_CHART_UNIT& shmNow, char* pzCurrPrc);

	VOID	SendSaveSignal(_In_ const char* pSignalPacket);
	///VOID	SaveSignalToDB(_In_ const char* pSignalPacket);


	// work thread
	static unsigned WINAPI StratThread(LPVOID lp);

private:
	CMemPool		*m_pMemPool;
	CQueueShm		*m_pShmQ;
	CDBPoolAdo		*m_pDBPool;
	CStratHistManager	m_stratHist;

	char			*m_pSignalResult;
	char			*m_pClientBuff;

	char			m_zGroupKey[128], m_zSymbol[128], m_zArtc[32];
	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_szMsg[1024];
	
	unsigned		m_dwSaveThread, m_dwSendThread;
	SYMBOL_INFO		m_SymbolInfo;

	BOOL			m_bIsFirstRunning;

	int				m_nChartTp;

	// worker thread
	HANDLE		m_hStratThread;
	HANDLE		m_hWorkDie;
	unsigned int		m_dwStratThreadID;
};


