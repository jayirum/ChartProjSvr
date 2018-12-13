#include "ChartShmUtil.h"
#include "Util.h"

CChartShmUtil::CChartShmUtil()
{
	m_log = NULL;
	m_pShmQ = NULL;
	m_bOpen = FALSE;
}


CChartShmUtil::~CChartShmUtil()
{
	m_bOpen = FALSE;
}

BOOL CChartShmUtil::OpenChart(char* pzArtc, CLogMsg* log /*= NULL*/)
{
	strcpy(m_zArtc, pzArtc);
	if (log)	m_log = log;
	//if (pzConfig[0] != NULL)	strcpy(m_zConfig, pzConfig);

	GET_SHM_NM(m_zArtc, m_zShmNm);
	GET_SHM_LOCK_NM(m_zArtc, m_zMutexNm);

	m_pShmQ = new CQueueShm();
	if (!m_pShmQ->Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
	{
		sprintf(m_zMsg, "CHART SHM OPEN 에러(%s)(artc:%s)(%s)",m_zShmNm, pzArtc, m_pShmQ->GetErr());
		return FALSE;
	}
	m_bOpen = TRUE;
	return TRUE;
}


VOID CChartShmUtil::ReleaseChart()
{
	if (m_pShmQ)
	{
		m_pShmQ->Close();
		m_bOpen = FALSE;
	}
}


/*
	date : yyyymmdd
	time : hh:mm:ss
*/
BOOL CChartShmUtil::CurrChart(char *pzSymbol, CHART_TP ChartTp, char* date, char *time, 
	_Out_ ST_SHM_CHART_UNIT& chart)
{
	if (!m_bOpen)
	{
		sprintf(m_zMsg, "Please call [OpenChart] function first");
		return FALSE;
	}
	char zGroupKey[128] = { 0, };
	char zChartNm[128] = { 0, };

	GET_GROUP_KEY(pzSymbol, (int)ChartTp, zGroupKey);
	GET_CHART_NM_EX(date, time, (int)ChartTp, zChartNm);

	int nLoop = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the current chart
	BOOL bExist = m_pShmQ->DataGet(zGroupKey, zChartNm, (char*)&chart);

	// retry 2 times
	while (FALSE == bExist)
	{
		Sleep(10);
		if (++nLoop > 2) {
			// There is no chart of the current time. Something is wrong.
			sprintf(m_zMsg, "[%s][%s] No Chart even if receive curr price.",zGroupKey, zChartNm);
			return FALSE;
		}
		bExist = m_pShmQ->DataGet(zGroupKey, zChartNm, (char*)&chart);

	} // if(!bExist)

	return bExist;
}

BOOL	CChartShmUtil::CompareHiLo(char *pzSymbol, CHART_TP ChartTp, char* pzCurrPrc, int nCompCnt, 
	char* date, char *time, int nDotCnt,
	_Out_ int* pnHiCnt, 
	//_Out_ ST_SHM_CHART_UNIT* arrHi, 
	_Out_ int* pnLoCnt, 
	//_Out_ ST_SHM_CHART_UNIT* arrLo,
	_Out_ ST_SHM_CHART_UNIT* currChart)
{
	if (!m_bOpen)
	{
		sprintf(m_zMsg, "Please call [OpenChart] function first");
		return FALSE;
	}
	char zGroupKey[128] = { 0, };
	char zChartNm[128] = { 0, };

	GET_GROUP_KEY(pzSymbol, (int)ChartTp, zGroupKey);
	GET_CHART_NM_EX(date, time, (int)ChartTp, zChartNm);

	int nLoop = 0;
	while (FALSE == m_pShmQ->DataGet(zGroupKey, zChartNm, (char*)currChart))
	{
		Sleep(10);
		if (++nLoop > 10) {
			// There is no chart of the current time. Something is wrong.
			sprintf(m_zMsg, "[%s][%s] No Chart even if receive curr price.", zGroupKey, zChartNm);
			return FALSE;
		}
	} // if(!bExist)

	ST_SHM_CHART_UNIT chart;
	memcpy(&chart, currChart, sizeof(chart));
	//ST_SHM_CHART_UNIT* hiChart = new ST_SHM_CHART_UNIT[nCompCnt];
	//ST_SHM_CHART_UNIT* loChart = new ST_SHM_CHART_UNIT[nCompCnt];

	*pnHiCnt = *pnLoCnt = 0;
	for(int i=0;i<nCompCnt-1;i++)
	{
		int nComp = CUtil::CompPrc(pzCurrPrc, strlen(pzCurrPrc), chart.high, sizeof(chart.high), nDotCnt, LEN_PRC);
		if (nComp > 0)
		{
			//memcpy(&hiChart[*pnHiCnt++], &chart, sizeof(chart));
			*pnHiCnt++;
		}
		nComp = CUtil::CompPrc(pzCurrPrc, strlen(pzCurrPrc), chart.low, sizeof(chart.low), nDotCnt, LEN_PRC);
		if (nComp < 0)
		{
			//memcpy(&hiChart[*pnLoCnt++], &chart, sizeof(chart));
			*pnLoCnt++;
		}

		sprintf(zChartNm, "%.*s", LEN_CHART_NM, chart.prevNm);
		if (strcmp(DEFINE_NO_CHART, zChartNm) == 0)	break;

		if (!m_pShmQ->DataGet(zGroupKey, zChartNm, (char*)&chart))
			break;
	}
	return TRUE;
}

BOOL CChartShmUtil::PrevChart(char *pzSymbol, CHART_TP ChartTp,
	_In_ ST_SHM_CHART_UNIT& currChart, _Out_ ST_SHM_CHART_UNIT& prevChart)
{
	if (!m_bOpen)
	{
		sprintf(m_zMsg, "Please call [OpenChart] function first");
		return FALSE;
	}

	char zGroupKey[128] = { 0, };
	GET_GROUP_KEY(pzSymbol, (int)ChartTp, zGroupKey);

	BOOL bExist = m_pShmQ->DataGet(zGroupKey, currChart.prevNm, (char*)&prevChart);

	// retry 2 times
	int nLoop = 0;
	while (FALSE == bExist)
	{
		Sleep(1000);
		if (++nLoop > 2) {
			// There is no chart of the current time. Something is wrong.
			sprintf(m_zMsg, "[%s][%s] No Chart even if receive curr price.", zGroupKey, currChart.prevNm);
			return FALSE;
		}
		bExist = m_pShmQ->DataGet(zGroupKey, currChart.prevNm, (char*)&prevChart);

	} // if(!bExist)

	return bExist;

}


CROSS_TP CChartShmUtil::GetCross(
	char* pzSymbol, 
	CHART_TP chartTp, 
	ST_SHM_CHART_UNIT* chart, 
	int nDotCnt, 
	BOOL bShortest,
	BOOL bStrongCross,
	_Out_ char* pzCross, 
	_Out_ char* pzLMAFirstPrc, 
	_Out_ char* pzLMALastPrc
)
{
	int nRet = 0;
	double dShort = 0;
	CROSS_TP tp;

	*pzCross = 0x00;
	*pzLMAFirstPrc = 0x00;
	*pzLMALastPrc = 0x00;

	char zChartTp[128];
	CHART_TP_S(chartTp, zChartTp);
	char zStrong[32];
	if (bStrongCross) strcpy(zStrong, "진성");
	else strcpy(zStrong, "가성");

	// 5-20 CROSS
	if(bShortest)
	{
		nRet = CUtil::CompPrc(
			chart->sma_shortest,
			sizeof(chart->sma_shortest),
			chart->sma_long,
			sizeof(chart->sma_long),
			nDotCnt, LEN_PRC);

		dShort = S2D((char*)chart->sma_shortest, sizeof(chart->sma_shortest));
	}
	else
	{
		// 10-20 CROSS
		nRet = CUtil::CompPrc(
			chart->sma_short,
			sizeof(chart->sma_short),
			chart->sma_long,
			sizeof(chart->sma_long),
			nDotCnt, LEN_PRC);
		
		dShort = S2D((char*)chart->sma_short, sizeof(chart->sma_short));
	}

	double dLong = S2D((char*)chart->sma_long, sizeof(chart->sma_long));

	if (dShort <= 0 || dLong <= 0)
	{
		sprintf(m_zMsg, "[%s][%s]<NONE_CROSS-There is no cross data> (short:%10.5f)(long:%10.5f) ", 
			zChartTp,zStrong, dShort, dLong);
		tp = NONE_CROSS;
		if (pzCross)	sprintf(pzCross, m_zMsg);
		return NONE_CROSS;
	}

	if (nRet == 0) {
		tp = NONE_CROSS;
		sprintf(m_zMsg, "[%s][%s]<NONE_CROSS> (short:%10.5f)(long:%10.5f) ",
			zChartTp, zStrong, dShort, dLong);
		if (pzCross)	sprintf(pzCross, m_zMsg);
		return NONE_CROSS;
	}

	if (nRet > 0) 
	{
		tp = GOLDEN_CROSS;
		sprintf(m_zMsg, "[%s][%s]<GOLDEN_CROSS> (short:%10.5f)(long:%10.5f) ",
			zChartTp, zStrong, dShort, dLong);
		if (pzCross)	sprintf(pzCross, m_zMsg);
	}
	if (nRet < 0)
	{
		tp = DEAD_CROSS;
		sprintf(m_zMsg, "[%s][%s]<DEAD_CROSS> (short:%10.5f)(long:%10.5f) ",
			zChartTp, zStrong, dShort, dLong);
		if (pzCross)	sprintf(pzCross, m_zMsg);
	}

	// Get the close price of the first chart of LMA
	char zStartChartNm[32];
	GetPreviousChartNm(chart->Nm, chartTp, 20, zStartChartNm);
	ST_SHM_CHART_UNIT firstChart;
	char zGroupKey[128] = { 0, };
	GET_GROUP_KEY(pzSymbol, (int)chartTp, zGroupKey);

	BOOL bExist = m_pShmQ->DataGet(zGroupKey, zStartChartNm, (char*)&firstChart);

	if (bExist)
	{
		sprintf(pzLMAFirstPrc, "%.*s", sizeof(firstChart.close), firstChart.close);
		sprintf(pzLMALastPrc, "%.*s", sizeof(chart->close), chart->close);
	}

	if (bStrongCross)
	{
		if (!bExist)
			return NONE_CROSS;

		int nComp = CUtil::CompPrc(
			firstChart.close,
			sizeof(firstChart.close),
			chart->close,
			sizeof(chart->close),
			nDotCnt, LEN_PRC);

		if (tp == GOLDEN_CROSS && nComp < 0) {
			sprintf(m_zMsg, "[%s][%s]<GOLDEN_CROSS> (short:%10.5f)(long:%10.5f)(First:%s)(Last:%s)",
				zChartTp, zStrong, dShort, dLong, pzLMAFirstPrc, pzLMALastPrc);
			if (pzCross)	sprintf(pzCross, m_zMsg);
			return GOLDEN_CROSS;
		}
		if (tp == DEAD_CROSS && nComp > 0) {
			sprintf(m_zMsg, "[%s][%s]<DEAD_CROSS> (short:%10.5f)(long:%10.5f)(First:%s)(Last:%s)",
				zChartTp, zStrong, dShort, dLong, pzLMAFirstPrc, pzLMALastPrc);
			if (pzCross)	sprintf(pzCross, m_zMsg);
			return DEAD_CROSS;
		}

		sprintf(m_zMsg, "[%s][%s]<NONE_CROSS> (short:%10.5f)(long:%10.5f)(First:%s)(Last:%s)",
			zChartTp, zStrong, dShort, dLong, pzLMAFirstPrc, pzLMALastPrc);
		if (pzCross)	sprintf(pzCross, m_zMsg);
		return NONE_CROSS;
	}
	return tp;
}




/*
TP_1MIN = 0,
TP_3MIN,
TP_5MIN,
TP_10MIN,
TP_15MIN,
TP_20MIN,
//TP_30MIN,
TP_60MIN,
TP_120MIN,

pzOrgChartNm : yyyymmddhhmmss
*/
BOOL CChartShmUtil::GetPreviousChartNm(_In_ char* pzOrgChartNm, _In_ CHART_TP ChartTp, _In_ int nPreviousCnt, _Out_ char* pResult)
{
	int nValCnt;
	switch (ChartTp)
	{
	case TP_1MIN:
		nValCnt = nPreviousCnt;
		break;
	case TP_3MIN:
		nValCnt = 3 * nPreviousCnt;
		break;
	case TP_5MIN:
		nValCnt = 5 * nPreviousCnt;
		break;
	case TP_10MIN:
		nValCnt = 10 * nPreviousCnt;
		break;
	case TP_15MIN:
		nValCnt = 15 * nPreviousCnt;
		break;
	case TP_20MIN:
		nValCnt = 20 * nPreviousCnt;
		break;
	default:
		return FALSE;
	}

	char y[32]; sprintf(y, "%.4s", pzOrgChartNm);
	char m[32]; sprintf(m, "%.2s", pzOrgChartNm + 4);
	char d[32]; sprintf(d, "%.2s", pzOrgChartNm + 6);
	char hh[32]; sprintf(hh, "%.2s", pzOrgChartNm + 8);
	char mm[32]; sprintf(mm, "%.2s", pzOrgChartNm + 10);
	char ss[32]; sprintf(ss, "%.2s", pzOrgChartNm + 12);

	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = atoi(y) - 1900;
	timeinfo->tm_mon = atoi(m) - 1;
	timeinfo->tm_mday = atoi(d);
	timeinfo->tm_hour = atoi(hh);
	timeinfo->tm_min = atoi(mm);
	timeinfo->tm_sec = atoi(ss);

	time_t timer = mktime(timeinfo) - (60 * nValCnt);
	struct tm *ltime = localtime(&timer);

	char zDT[32], zTM[32];
	sprintf(zDT, "%04d%02d%02d", 1900 + ltime->tm_year, ltime->tm_mon + 1, ltime->tm_mday);
	sprintf(zTM, "%02d:%02d:%02d", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	GET_CHART_NM_EX(zDT, zTM, ChartTp, pResult);

	return TRUE;
}