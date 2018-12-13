#pragma once

#include "QueueShm.h"
#include "../IRUM_Util/IRUM_Common.h"
#include "LogMsg.h"


class CChartShmUtil
{
public:
	CChartShmUtil();
	virtual ~CChartShmUtil();

	BOOL	OpenChart(char* pzArtc, CLogMsg* log=NULL/*, char* pzConfig=NULL*/);
	VOID	ReleaseChart();

	BOOL	CurrChart(char *pzSymbol, CHART_TP ChartTp, char* date, char *time, _Out_ ST_SHM_CHART_UNIT& chart);
	BOOL	PrevChart(char *pzSymbol, CHART_TP ChartTp, _In_ ST_SHM_CHART_UNIT& currChart, _Out_ ST_SHM_CHART_UNIT& prevChart);

	CROSS_TP GetCross(char* pzSymbol, CHART_TP chartTp, ST_SHM_CHART_UNIT* chart, int nDotCnt, BOOL bShortest, BOOL bStrongCross,
		_Out_ char* pzCross, _Out_ char* pzLMAFirstPrc, _Out_ char* pzLMALastPrc);

	BOOL	CompareHiLo(char *pzSymbol, CHART_TP ChartTp, char* pzCurrPrc, int nCompCnt, char* date, char *time,int nDotCnt,
		_Out_ int* pnHiCnt, 
		//_Out_ ST_SHM_CHART_UNIT* arrHi, 
		_Out_ int* pnLoCnt, 
		//_Out_ ST_SHM_CHART_UNIT* arrLo,
		_Out_ ST_SHM_CHART_UNIT* currChart);


	BOOL	GetPreviousChartNm(_In_ char* pzOrgChartNm,_In_ CHART_TP ChartTp, _In_ int nPreviousCnt, _Out_ char* pResult);
	char*	getmsg() { return m_zMsg; }
private:
	char	m_zArtc[128];
	char	m_zShmNm[128], m_zMutexNm[128];
	char	m_zMsg[512];

	CLogMsg		*m_log;
	char		m_zConfig[_MAX_PATH];

	BOOL		m_bOpen;

	CQueueShm		*m_pShmQ;
};