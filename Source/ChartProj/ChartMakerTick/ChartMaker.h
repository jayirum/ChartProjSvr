/*
	subscribing
	chart 생성
	shm 저장
*/
#pragma once
#include "../../IRUM_UTIL/MCastRecv.h"
#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/QueueShm.h"

/*
TP_TICK2,
TP_TICK5,
TP_TICK10,
TP_TICK20,
TP_TICK30,
TP_TICK40,
TP_TICK60
*/
#define MAX_CHARTTP_CNT	7

class CChartMaker : public CBaseThread
{
public:
	CChartMaker(char* pzSymbol, char* pzArtcCode, CMemPool* pMemPool, unsigned dwMainThreadId);
	virtual ~CChartMaker();

	VOID	ChartProc(VOID* pIn, int tp);
	VOID	Chart_SMA(int tp, char* pzGroupKey, char* pzNowChartNm, _Out_ char* i_pNowChart);


	BOOL	InitChartSHM();
	VOID	CloseChartShm();
	
	BOOL	BeginMcastRecv();
	VOID	EndMcastRecv();

private:
	BOOL	IsMySymbol(char* pSise);
	VOID	ThreadFunc();
	int		ChartIdx(int chartTp);
	BOOL	IsMaxChartMakeCnt(int chartTp,int lCnt);
	// work thread
	static unsigned WINAPI WorkThread(LPVOID lp);
private:
	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_zSymbol[128];
	char			m_zArtc[128];
	
	CMemPool*		m_pMemPool;
	CQueueShm		m_shmQ;
	CMCastRecv		*m_pMcastRecv;

	unsigned		m_dwMainThreadId;
	std::string		m_sLastChartNm[MAX_CHARTTP_CNT], m_sPrevChartNm[MAX_CHARTTP_CNT];
	long			m_ChartNmSeq[MAX_CHARTTP_CNT];
	long			m_ChartMakeCnt[MAX_CHARTTP_CNT];


	// worker thread
	HANDLE		m_hWorkThread;
	HANDLE		m_hWorkDie;
	unsigned int		m_dwWorkThreadID;
	
};