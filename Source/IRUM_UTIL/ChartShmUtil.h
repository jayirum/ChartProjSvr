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

	CROSS_TP GetCross(ST_SHM_CHART_UNIT* chart, int nDotCnt, _Out_ char* pzCross=NULL);

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