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


CROSS_TP CChartShmUtil::GetCross(ST_SHM_CHART_UNIT* chart, int nDotCnt, char* pzCross)
{
	int nRet = CUtil::CompPrc(
		chart->sma_short, 
		sizeof(chart->sma_short),
		chart->sma_long, 
		sizeof(chart->sma_long),
		nDotCnt, LEN_PRC);

	CROSS_TP tp;

	double dShort = S2D((char*)chart->sma_short, sizeof(chart->sma_short));
	double dLong = S2D((char*)chart->sma_long, sizeof(chart->sma_long));
	if (dShort <= 0 || dLong <= 0)
	{
		sprintf(m_zMsg, "[GetCross]<NONE_CROSS-데이터이상> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
		tp = NONE_CROSS;
		if (pzCross)	sprintf(pzCross, "NONE(SHORT:%.5f)(LONG:%.5f)", dShort, dLong);
	}

	if (nRet == 0) {
		tp = NONE_CROSS;
		sprintf(m_zMsg, "[GetCross]<NONE_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
		if (pzCross)	sprintf(pzCross, "NONE(SHORT:%.5f)(LONG:%.5f)", dShort, dLong);
	}
	else if (nRet > 0) {
		tp = GOLDEN_CROSS;
		sprintf(m_zMsg, "[GetCross]<GOLDEN_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
		if (pzCross)	sprintf(pzCross, "GOLDEN(SHORT:%.5f)(LONG:%.5f)", dShort, dLong);
	}
	else {
		tp = DEAD_CROSS;
		sprintf(m_zMsg, "[GetCross]<DEAD_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
		if (pzCross)	sprintf(pzCross, "DEAD(SHORT:%.5f)(LONG:%.5f)", dShort, dLong);
	}

	return tp;
}
