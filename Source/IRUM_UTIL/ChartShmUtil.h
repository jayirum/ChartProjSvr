#pragma once

#include "QueueShm.h"
#include "../IRUM_Util/IRUM_Common.h"
#include "LogMsg.h"



//DEF-SHART SHM, DEF-CHARTSHM
#define CHART_TP_CNT		20
#define MAX_CHART_TP_CNT	CHART_TP_CNT
#define MAX_SYMBOL_PER_ARTC	2		//	품목당 2개의 월물(종목)에 대해서만 지원한다.
#define MAX_GROUPKEY_CNT	MAX_SYMBOL_PER_ARTC * CHART_TP_CNT
#define MAX_CHART_UNIT_CNT	1000	//	한 GROUP당 (한종목, 한차트당 'CLU7 00')
#define MAX_ARTC_CNT		10		//	10개의 품목만 
#define GET_TICKCHART_NM(seq,out){ sprintf(out, "%0*d", LEN_CHART_NM, seq);}
#define GET_GROUP_KEY(symbol, CHART_TP, out) { sprintf(out, "%-*.*s%02d", LEN_SYMBOL, LEN_SYMBOL, symbol, CHART_TP) ; }

enum CHARTNAME_TYPE { CHARTNAME_TP_NEAR, CHARTNAME_TP_ };


enum CHART_TP {
	TP_1MIN = 0,
	TP_3MIN,
	TP_5MIN,
	TP_10MIN,
	TP_15MIN,
	TP_20MIN,
	//TP_30MIN, 
	TP_60MIN,
	TP_120MIN,
	TP_DAY,
	TP_WEEK,
	TP_MON,
	TP_TICK1,
	TP_TICK2,
	TP_TICK5,
	TP_TICK10,
	TP_TICK20,
	TP_TICK30,
	TP_TICK40,
	TP_TICK60
};
#define CHART_TP_S(tp,out){\
	if(tp==TP_1MIN) strcpy(out,"1분");		\
	if (tp == TP_3MIN) strcpy(out, "3분");	\
	if (tp == TP_5MIN) strcpy(out, "5분");	\
	if (tp == TP_10MIN) strcpy(out, "10분");	\
	if (tp == TP_15MIN) strcpy(out, "15분");	\
	if (tp == TP_20MIN) strcpy(out, "20분");	\
	if (tp == TP_60MIN) strcpy(out, "60분");	\
	if (tp == TP_120MIN) strcpy(out, "120분");	\
	if (tp == TP_DAY) strcpy(out, "일간");	\
	if (tp == TP_WEEK) strcpy(out, "주간");	\
}

class CChartShmUtil
{
public:
	CChartShmUtil(int nChartNameTp);
	virtual ~CChartShmUtil();

	BOOL	OpenChart(char* pzArtc,  CLogMsg* log=NULL/*, char* pzConfig=NULL*/);
	VOID	ReleaseChart();

	char*	ComposeChartName(char* date, char* time, int tp, char* out);

	BOOL	GetChartData(char *pzSymbol, CHART_TP ChartTp, char* pzChartNm, _Out_ ST_SHM_CHART_UNIT& chart);
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
	int			m_nChartNameTp;
	CQueueShm	*m_pShmQ;
};