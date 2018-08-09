/*
	subscribing
	chart 생성
	shm 저장
*/
#pragma once

#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/QueueShm.h"
#include "../../IRUM_UTIL/MemPool.h"

#define MAX_CHARTTP_CNT	20

#define MEMPOOL_BLOCK_SIZE	512
#define MEMPOOL_PRE_ALLOC	500
#define MEMPOOL_MAX_ALLOC	10000

class CChartMaker : public CBaseThread
{
public:
	CChartMaker(char* pzSymbol, char* pzArtcCode, /*CMemPool* pMemPool,*/ unsigned dwMainThreadId);
	virtual ~CChartMaker();

	VOID	ChartProc(VOID* pIn, int tp);
	VOID	Chart_SMA(int tp, char* pzGroupKey, char* pzNowChartNm, _Out_ char* o_pNowChart);


	BOOL	InitChartSHM();
	VOID	CloseChartShm();

	BOOL	InitMemPool();
	VOID	CloseMemPool();
	unsigned int GetChartThreadId() {return m_dwWorkThreadID;}
private:
	BOOL	IsMySymbol(char* pSise);
	VOID	ThreadFunc();


	// work thread
	static unsigned WINAPI WorkThread(LPVOID lp);

private:
	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_zSymbol[128];
	char			m_zArtc[128];
	CMemPool*		m_pMemPool;
	CQueueShm		m_shmQ;
	
	unsigned		m_dwMainThreadId;
	std::string		m_sLastChartNm[MAX_CHARTTP_CNT], m_sPrevChartNm[MAX_CHARTTP_CNT];

	// worker thread
	HANDLE			m_hWorkThread;
	HANDLE			m_hWorkDie;
	unsigned int	m_dwWorkThreadID;
};