/*
	subscribing
	chart ����
	shm ����
*/
#pragma once

#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/ChartShmUtilFixed.h"

#define MAX_CHARTTP_CNT	20

#define MEMPOOL_BLOCK_SIZE	512
#define MEMPOOL_PRE_ALLOC	500
#define MEMPOOL_MAX_ALLOC	10000

class CChartMaker : public CBaseThread
{
public:
	CChartMaker(char* pzSymbol, char* pzArtcCode, int nDotCnt, unsigned dwMainThreadId, BOOL bSaveChart);
	virtual ~CChartMaker();

	VOID	ChartProc(VOID* pIn, int tp);
	VOID	Chart_SMA(int tp, char* pzGroupKey, char* pzNowChartNm, _Out_ char* o_pNowChart);


	BOOL	InitChartSHM();
	VOID	CloseChartShm();

	unsigned int GetChartThreadId() {return m_dwWorkThreadID;}
private:
	BOOL	IsMySymbol(char* pSise);
	VOID	ThreadFunc();


	// work thread
	static unsigned WINAPI WorkThread(LPVOID lp);

private:
	char			m_zSymbol[128];
	char			m_zArtc[128];
	
	unsigned		m_dwMainThreadId;
	std::string		m_sLastChartNm[MAX_CHARTTP_CNT], m_sPrevChartNm[MAX_CHARTTP_CNT];

	// worker thread
	HANDLE			m_hWorkThread;
	HANDLE			m_hWorkDie;
	unsigned int	m_dwWorkThreadID;
	int				m_nDotCnt;
	BOOL			m_bSaveChart;

	CHARTNAME_TYPE	m_chartNmType;
	CChartShmUtilFixed	*m_chartUtil;

	BOOL			m_bLogData;
};