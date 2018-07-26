
#include "../../IRUM_UTIL/ADOFunc.h"
#include "SignalOpenPrc.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"

extern CLogMsg g_log;

CSignalOpenPrc::CSignalOpenPrc(CQueueShm *pShm, void* pDBPool) : CSignalBase(pShm, pDBPool)
{
}


CSignalOpenPrc::~CSignalOpenPrc()
{
}



/*
	현재차트의 시가와 이전 9개 차트 시가 비교
*/
VOID CSignalOpenPrc::CheckEmptyArrow()
{
	CHECKSIG_RESULT_OPENPRC* p = (CHECKSIG_RESULT_OPENPRC*)m_p.pResult;
	CHECKSIG_RESULT_OPENPRC_ONE* pResult = &p->sigEmpty;


	ZeroMemory(pResult, sizeof(CHECKSIG_RESULT_OPENPRC_ONE));
	pResult->SigRet = SIGNAL_NONE;
	sprintf(pResult->zStratID, "%.*s", min(sizeof(pResult->zStratID), strlen(m_zStratID)), m_zStratID);
	sprintf(pResult->zStratNm, "%.*s", min(sizeof(pResult->zStratNm), strlen(m_zStratNm)), m_zStratNm);
	strcpy(pResult->zGroupKey, m_p.zGroupKey);
	sprintf(pResult->zChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);

	// 과거 차트가 9개가 안되면 pass
	int nChartCnt = m_pShmQ->GetCurrDataCnt((char*)m_p.zGroupKey);
	if (nChartCnt < CHECKCNT_CHART_OPENPRC) {
		//g_log.log(LOGTP_SUCC, "차트가 %d개가 안된다.(%.5s)(%d)", DEF_CHECK_CHART_CNT, pzGroupKey, nChartCnt);
		SetLastCheckEmptyArrow();
		return;
	}

	// 이미 체크했으면 pass
	if (IsAlreadyCheckEmptyArrow())
		return;

	/*
		현재가 받을 때 마다
		현재 차트 시가와 이전 9개 비교
	*/

	ST_SHM_CHART_UNIT	chartUnit[CHECKCNT_CHART_OPENPRC];	//	완료된 차트 9개

	INT nFoundPlus = -1, nFoundMinus = -1;
	char zBaseOpenPrc[LEN_PRC + 1];
	char prevChartNm[LEN_CHART_NM + 1];

	sprintf(zBaseOpenPrc, "%.*s", LEN_PRC, m_p.pChartNow->open);	// 현재차트의 시가를 본다. (CheckFilledArrow 와의 차이점)
	
	//g_log.log(LOGTP_SUCC, "[STRAT_OPEN.%s][%s][NOW:%.*s](%c)(%10.5f)",
	//	AcntNo(), pzGroupKey, LEN_CHART_NM, nowChart.Nm, nowChart.gb[0], atof(nowChart.open));

	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, m_p.pChartNow->prevNm);
	int nLoop = CHECKCNT_CHART_OPENPRC;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_pShmQ->DataGet((char*)m_p.zGroupKey, (char*)prevChartNm, (char*)&chartUnit[i]))
		{
			g_log.log(LOGTP_ERR, "[CheckEmptyArrow][%s][%s]차트가 SHM 에 없다", prevChartNm, m_p.zGroupKey);
			continue;
		}

		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, chartUnit[i].prevNm);

		//g_log.log(LOGTP_SUCC, "[strat_open:%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, chart[i].Nm, chart[i].gb[0], atof(chart[i].open));

		// 음봉도 양봉도 아닌 경우
		if (chartUnit[i].gb[0] != DEF_CANDLE_PLUS && chartUnit[i].gb[0] != DEF_CANDLE_MINUS) {
			continue;
		}

		// 시가가 일치하는가?
		BOOL bSame = CUtil::IsSamePrice(zBaseOpenPrc, LEN_PRC,chartUnit[i].open, sizeof(chartUnit[i].open), m_p.nDotCnt, LEN_PRC);
		if (!bSame)
			continue;

		// 양봉 또는 음봉인가?
		if (chartUnit[i].gb[0] == DEF_CANDLE_PLUS)	nFoundPlus = i;
		if (chartUnit[i].gb[0] == DEF_CANDLE_MINUS)	nFoundMinus = i;

	}
	//g_log.log(LOGTP_SUCC, "--------------------------------------------------------");

	if ((nFoundPlus == -1) && (nFoundMinus == -1))
	{
		// 현재차트 시가와 일치하는 시가가 이전 차트에 없으므로 더이상 이번 차트는 체크하지 않는다.		
		SetLastCheckEmptyArrow();
		return;
	}


	// 둘다 일치. 일치하는 차트가 있어도 음봉, 양봉 모두 일치하면 안된다.
	if (nFoundPlus > -1 && nFoundMinus > -1)
	{
		SetLastCheckEmptyArrow();
		return ;
	}


	// 일치하는 데이터 있음
	pResult->SigRet = SIGNAL_EXIST;
	memcpy(pResult->zSigPrc, zBaseOpenPrc, MIN(sizeof(pResult->zSigPrc), strlen(zBaseOpenPrc)));

	// 양봉일치
	if (nFoundPlus > -1)
	{
		// GET CROSS CHECK
		CROSS_TP crossTp = GetCross(m_p.pChartNow);
		pResult->nCrossTp = (int)crossTp;
		pResult->ChartGb[0] = DEF_CANDLE_PLUS;		
		SetLastCheckEmptyArrow();

		//// 매수Signal 표시 (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_BUY, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]매수신호발생(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
		//	g_log.log(LOGTP_SUCC, msg);
		//	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		//}
		return;
	}

	// 음봉일치
	if (nFoundMinus > -1)
	{
		// GET CROSS CHECK
		CROSS_TP crossTp = GetCross(m_p.pChartNow);
		pResult->nCrossTp = (int)crossTp;
		pResult->ChartGb[0] = DEF_CANDLE_MINUS;
		SetLastCheckEmptyArrow();

		//// 매도Signal 표시 (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_SELL, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]매도신호발생(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
		//	g_log.log(LOGTP_SUCC, msg);
		//	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		//}

		return ;
	}

}


// 마지막 완성된 차트의 시가와 그 이전 9개 차트 시가 비교
VOID CSignalOpenPrc::CheckFilledArrow()
{
	CHECKSIG_RESULT_OPENPRC* p = (CHECKSIG_RESULT_OPENPRC*)m_p.pResult;
	CHECKSIG_RESULT_OPENPRC_ONE* pResult = &p->sigFilled;

	ZeroMemory(pResult, sizeof(CHECKSIG_RESULT_OPENPRC_ONE));
	pResult->SigRet = SIGNAL_NONE;
	sprintf(pResult->zStratID, "%.*s", min(sizeof(pResult->zStratID), strlen(m_zStratID)), m_zStratID);
	sprintf(pResult->zStratNm, "%.*s", min(sizeof(pResult->zStratNm), strlen(m_zStratNm)), m_zStratNm);
	strcpy(pResult->zGroupKey, m_p.zGroupKey);
	sprintf(pResult->zChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);

	// 과거 차트가 9개가 안되면 pass
	int nChartCnt = m_pShmQ->GetCurrDataCnt((char*)m_p.zGroupKey);
	if (nChartCnt < CHECKCNT_CHART_OPENPRC) {
		//g_log.log(LOGTP_SUCC, "차트가 %d개가 안된다.(%.5s)(%d)", DEF_CHECK_CHART_CNT, pzGroupKey, nChartCnt);
		return;
	}

	/*
		마지막 완성된 차트의 시가와 그 이전 9개 차트 시가 비교
	*/

	ST_SHM_CHART_UNIT	chartUnit[CHECKCNT_CHART_OPENPRC];	//	완료된 차트 9개

	INT nFoundPlus = -1, nFoundMinus = -1;
	char zBaseOpenPrc[LEN_PRC + 1];
	char prevChartNm[LEN_CHART_NM + 1];

	sprintf(zBaseOpenPrc, "%.*s", LEN_PRC, m_p.pChartPrev->open);		// CheckEmptyArrow 와 차이점(difference from CheckEmptyArrow)
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, m_p.pChartPrev->prevNm);

	int nLoop = CHECKCNT_CHART_OPENPRC;	//(nChartCnt < m_nMaxChartCnt) ? nChartCnt : m_nMaxChartCnt;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_pShmQ->DataGet((char*)m_p.zGroupKey, (char*)prevChartNm, (char*)&chartUnit[i]))
		{
			g_log.log(LOGTP_ERR, "[CheckFilledArrow][%s][%s]차트가 SHM 에 없다", prevChartNm, m_p.zGroupKey);
			continue;
		}

		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, chartUnit[i].prevNm);

		//g_log.log(LOGTP_SUCC, "[strat_open:%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, chart[i].Nm, chart[i].gb[0], atof(chart[i].open));

		// 음봉도 양봉도 아닌 경우
		if (chartUnit[i].gb[0] != DEF_CANDLE_PLUS && chartUnit[i].gb[0] != DEF_CANDLE_MINUS) {
			continue;
		}

		// 시가가 일치하는가?
		BOOL bSame = CUtil::IsSamePrice(zBaseOpenPrc, LEN_PRC,
											chartUnit[i].open, sizeof(chartUnit[i].open), m_p.nDotCnt, LEN_PRC);
		if (!bSame)
			continue;

		// 양봉 또는 음봉인가?
		if (chartUnit[i].gb[0] == DEF_CANDLE_PLUS)	nFoundPlus = i;
		if (chartUnit[i].gb[0] == DEF_CANDLE_MINUS)	nFoundMinus = i;

	}


	//	완성된 차트 끼리의 비교이므로 같은 분봉에서는 한번만 수행하면 된다.
	//	Just check one time because compares already passed candle charts
	SetLastCheckFilledArrow();

	if ((nFoundPlus == -1) && (nFoundMinus == -1))
	{
		return ;
	}


	// 둘다 일치. 일치하는 차트가 있어도 음봉, 양봉 모두 일치하면 안된다.
	if (nFoundPlus > -1 && nFoundMinus > -1)
	{
		return ;
	}

	// 일치하는 데이터 있음
	pResult->SigRet = SIGNAL_EXIST;
	memcpy(pResult->zSigPrc, zBaseOpenPrc, MIN(sizeof(pResult->zSigPrc), strlen(zBaseOpenPrc)));

	// 양봉일치
	if (nFoundPlus > -1)
	{
		// TODO. CROSS CHECK?
		// TODO. CROSS_TP crossTp = GetCross(prevChart, pTrade);
		// TODO. if ((crossTp == DEAD_CROSS) && (side == DEF_SELL) || (crossTp == GOLDEN_CROSS) && (side == DEF_BUY))

		pResult->ChartGb[0] = DEF_CANDLE_PLUS;
		
		//// 매수Signal 표시 (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_BUY, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]매수신호발생(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
		//	g_log.log(LOGTP_SUCC, msg);
		//	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		//}
		return;
	}

	// 음봉일치
	if (nFoundMinus > -1)
	{
		//? CROSS_TP crossTp = GetCross(prevChart, pTrade);
		//? TODO. if ((crossTp == DEAD_CROSS) && (side == DEF_SELL) || (crossTp == GOLDEN_CROSS) && (side == DEF_BUY))

		pResult->ChartGb[0] = DEF_CANDLE_MINUS;
		
		//// 매도Signal 표시 (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_SELL, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]매도신호발생(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
		//	g_log.log(LOGTP_SUCC, msg);
		//	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		//}

		return ;
	}
}

BOOL CSignalOpenPrc::Begin()
{
	CDBHandlerAdo db(DBPoolPtr(m_pDBPool)->Get());

	char sQ[1024];
	sprintf(sQ, "SELECT STRAT_ID, STRAT_NM FROM STRAT_MST WHERE STRAT_USE_YN='Y' AND STRAT_TP='S' AND (STRAT_NM LIKE '%%시가%%' or STRAT_NM LIKE '%%OpenPrice%%') ");
	if (FALSE == db->ExecQuery(sQ))
	{
		g_log.log(LOGTP_ERR, "Load OpenPrice Strat Info failed(%s)(%s)", sQ, db->GetError());
		return FALSE;
	}

	if (db->IsNextRow())
	{
		db->GetStr("STRAT_ID", m_zStratID);
		db->GetStr("STRAT_NM", m_zStratNm);

	} // if (db->NextRow())
	else
	{
		g_log.log(LOGTP_ERR, "There is no Strategy info (%s)", sQ);
		//m_bDoStrategy = FALSE;
		return FALSE;
	}
	return TRUE;
}


VOID CSignalOpenPrc::MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm,
	ST_SHM_CHART_UNIT *pChartNow, ST_SHM_CHART_UNIT *pChartPrev, char* pzTickSize, int nDotCnt,
	void* pResult, int nOption)
{
	m_p.chartTp = tp;
	strcpy(m_p.zGroupKey, pzGroupKey);
	strcpy(m_p.zChartNm, pzChartNm);
	m_p.pChartNow = pChartNow;
	m_p.pChartPrev = pChartPrev;
	strcpy(m_p.zTickSize, pzTickSize);
	m_p.nDotCnt = nDotCnt;
	m_p.pResult = pResult;

	// 
	// Do logic for empty arrow
	CheckEmptyArrow();

	if (!IsAlreadyCheckFilledArrow()) {
		// Do logic for filled arrow
		CheckFilledArrow();
	}
	SetLastCheckFilledArrow();
		

}

VOID CSignalOpenPrc::SetLastCheckFilledArrow()
{
	sprintf(m_zLastCheckFilledArrow, "%.*s", LEN_CHART_NM, m_p.zChartNm);
}

BOOL CSignalOpenPrc::IsAlreadyCheckFilledArrow()
{
	int nRet = strncmp(m_zLastCheckFilledArrow, m_p.zChartNm, LEN_CHART_NM);
	return (nRet >= 0);
}



VOID CSignalOpenPrc::SetLastCheckEmptyArrow()
{
	sprintf(m_zLastCheckEmptyArrow, "%.*s", LEN_CHART_NM, m_p.zChartNm);
}

BOOL CSignalOpenPrc::IsAlreadyCheckEmptyArrow()
{
	int nRet = strncmp(m_zLastCheckEmptyArrow, m_p.zChartNm, LEN_CHART_NM);
	return (nRet >= 0);
}