#pragma once

//MCAST
#include "../../IRUM_UTIL/MCastRecv.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/QueueShm.h"
//MCAST #include "../../IRUM_UTIL/NanoPubSub.h"

#include "SignalOpenPrc.h"
#include "SignalAssist.h"
#include "SignalCrossMA.h"
#include "SignalSideway.h"

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
	BOOL	LoadSymbolInfo();		// 전략을 위한 공통정보

	VOID	MainFunc();
	VOID	ThreadFunc();

	BOOL	BeginDB();
	VOID	EndDB();

	BOOL	BeginShm();
	VOID	EndShm();

	BOOL	BeginSubscribe();
	VOID	EndSubScribe();

	BOOL	BeginSignals();
	VOID	EndSignals();

	VOID	SignalProcWrapper(char* pMarketData);
	VOID	SignalProc(char* pMarketData);
	
	INT		GetLastChartShm(char* pzGroupKey, char* pzChartNm, ST_SHM_CHART_UNIT& shmLast, ST_SHM_CHART_UNIT& shmNow);

	VOID	SetSigPack_OpenPrc(_In_ BOOL bFilled,_In_ void* pSignal, _Out_ char* pRsltPack);
	VOID	SetSigPack_AssistPrc(_In_ void* pSignal, BOOL bClr, _Out_ char* pRsltPack);
	VOID	SetSigPack_CrossMA(_In_ void* pSignal, _Out_ char* pRsltPack);
	VOID	SetSigPack_SideWay(_In_ void* pSignal, _Out_ char* pRsltPack);

	VOID	SendSaveSignal(_In_ const char* pSignalPacket);
	///VOID	SaveSignalToDB(_In_ const char* pSignalPacket);

	BOOL	IsInitCheckDone() {	return m_bInitCheck;}
	VOID	SetInitCheckDone() { m_bInitCheck = TRUE; }

private:
	CMemPool		*m_pMemPool;
	CQueueShm		*m_pShmQ;
	//CNanoPubSub		*m_pMDSub;
	CDBPoolAdo		*m_pDBPool;
	//MCAST
	CMCastRecv		*m_pMcastRecv;

	CSignalOpenPrc	*m_pSigOpenPrc;
	CSignalAssist	*m_pSigAssist;
	CSignalCrossMA	*m_pSigCrossMA;
	CSignalSideWay	*m_pSigSideWay;

	char			*m_pSignalResult;
	char			*m_pClientBuff;

	char			m_zGroupKey[128], m_zSymbol[128], m_zArtc[32];
	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_szMsg[1024];
	
	unsigned		m_dwSaveThread, m_dwSendThread;
	SYMBOL_INFO		m_SymbolInfo;

	BOOL			m_bInitCheck;

	int				m_nChartTp;
};


