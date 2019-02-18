#include "ChartShmUtil.h"
#include "Util.h"
#include "TimeUtils.h"

CChartShmUtil::CChartShmUtil(int nChartNameTp)
{
	m_log = NULL;
	m_pShmQ = NULL;
	m_bOpen = FALSE;
	m_nChartNameTp = nChartNameTp;
}


CChartShmUtil::~CChartShmUtil()
{
	m_bOpen = FALSE;
}



// 00:01:00 ~ 00:01:59 ==> 01분 차트
// date : yyyymmdd, time:hh:mm:ss
// 120분 차트는 6시 부터 시작한다
char* CChartShmUtil::ComposeChartName(char* date, char* time, int tp, char* out)
{
	if (tp == TP_1MIN)
		return ComposeChartName_1min(date, time, out);


	int divider, ret;
	char zHour[32], zMin[32], zSec[32];
	char zTm[32];
	sprintf(zTm, "%.8s", time);
	sprintf(zHour, "%.2s", zTm);
	sprintf(zMin, "%.2s", zTm + 3);
	sprintf(zSec, "%.2s", zTm + 6);

	if (tp == TP_1MIN) divider = 1;
	else if (tp == TP_3MIN) divider = 3;
	else if (tp == TP_5MIN) divider = 5;
	else if (tp == TP_10MIN) divider = 10;
	else if (tp == TP_15MIN) divider = 15;
	else if (tp == TP_20MIN) divider = 20;
	else if (tp == TP_60MIN) divider = 60;
	else if (tp == TP_120MIN) divider = 120;

	// 00:01:00 ~ 00:01:59 ==> 01분 차트
	if (m_nChartNameTp == CHARTNAME_TP_NEAR)
	{
		ret = (atoi(zMin) / divider);
	}
	// 00:01:01 ~ 00:02:00 ==> 02분 차트
	else
	{
		if (strncmp(time + 6, "00", 2) == 0)
			ret = ((atoi(zMin)) / divider);
		else {
			int nMin = atoi(zMin) + 1;

			// 시간 변경
			if (nMin == 60)
			{
				ret = 0;
				int nHour = atoi(zHour) + 1;
				if (nHour == 24)
					strcpy(zHour, "00");
				else
					sprintf(zHour, "%02d", nHour);
			}
			else
				ret = nMin / divider;
		}
	}
	int min = (ret)*divider;

	if (tp == TP_60MIN)
	{
		int h = atoi(zHour);
		if (h == 24)
			h = 0;
		sprintf(out, "%.8s%02d00", date, h);
	}
	else if (tp == TP_120MIN) {
		int h = atoi(zHour);
		int hRemain = h % 2;
		if (hRemain == 1)
			h -= 1;
		sprintf(out, "%.8s%02d00", date, h);
	}
	else {
		sprintf(out, "%.8s%.2s%02d", date, zHour, min);
	}

	return out;
}




// date : yyyymmdd
// time : hh:mm:ss
char* CChartShmUtil::ComposeChartName_1min(char* date, char* time, char* out)
{
	char zHour[32], zMin[32], zSec[32];
	char zTm[32];
	sprintf(zTm, "%.8s", time);
	sprintf(zHour, "%.2s", zTm);
	sprintf(zMin, "%.2s", zTm + 3);
	sprintf(zSec, "%.2s", zTm + 6);

	// case 1
	// 00:01:00 ~ 00:01:59 ==> candle is 01 min
	if (m_nChartNameTp == CHARTNAME_TP_NEAR)
	{
		sprintf(out, "%.8s%.2s%02s", date, zHour, zMin);
		return out;
	}

	// case 2
	// 00:01:01 ~ 00:02:00 ==> candle is 02 min
	if (strncmp(zSec, "00", 2) == 0)
	{
		sprintf(out, "%.8s%.2s%02s", date, zHour, zMin);
		return out;
	}


	/*
		23:58:50	==> 59 min candle
		23:59:01	==> +1 min. It may change hour/day/month/year
	*/
	char hhmmss[32];
	sprintf(hhmmss, "%s%s%s", zHour, zMin, zSec);
	CTimeUtils::AddMins(date, hhmmss, 1, zTm);
	sprintf(out, "%.12s", zTm);
	return out;
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



BOOL CChartShmUtil::GetChartData(char *pzSymbol, CHART_TP ChartTp, char* pzChartNm,
	_Out_ ST_SHM_CHART_UNIT& chart)
{
	if (!m_bOpen)
	{
		sprintf(m_zMsg, "Please call [OpenChart] function first");
		return FALSE;
	}
	char zGroupKey[128] = { 0, };

	GET_GROUP_KEY(pzSymbol, (int)ChartTp, zGroupKey);

	int nLoop = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the current chart
	BOOL bExist = m_pShmQ->DataGet(zGroupKey, pzChartNm, (char*)&chart);

	// retry 2 times
	while (FALSE == bExist)
	{
		Sleep(10);
		if (++nLoop > 2) {
			// There is no chart of the current time. Something is wrong.
			sprintf(m_zMsg, "[%s][%s] No Chart even if receive curr price.", zGroupKey, pzChartNm);
			return FALSE;
		}
		bExist = m_pShmQ->DataGet(zGroupKey, pzChartNm, (char*)&chart);

	} // if(!bExist)

	return bExist;
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
	//GET_CHART_NM_EX(date, time, (int)ChartTp, zChartNm);
	ComposeChartName(date, time, (int)ChartTp, zChartNm);

	return GetChartData(pzSymbol, ChartTp, zChartNm, chart);
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
	//GET_CHART_NM_EX(date, time, (int)ChartTp, zChartNm);
	ComposeChartName(date, time, (int)ChartTp, zChartNm);

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

	//GET_CHART_NM_EX(zDT, zTM, ChartTp, pResult);
	ComposeChartName(zDT, zTM, (int)ChartTp, pResult);

	return TRUE;
}