
#include "../../IRUM_UTIL/ADOFunc.h"
#include "SignalCrossMA.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"

extern CLogMsg g_log;



CSignalCrossMA::CSignalCrossMA(CQueueShm *pShm, void* pDBPool) : CSignalBase(pShm, pDBPool)
{
}


CSignalCrossMA::~CSignalCrossMA()
{
}


BOOL CSignalCrossMA::Begin()
{
	CDBHandlerAdo db(DBPoolPtr(m_pDBPool)->Get());

	char sQ[1024];
	sprintf(sQ, "SELECT STRAT_ID,STRAT_NM FROM STRAT_MST WHERE STRAT_USE_YN='Y' AND STRAT_TP='S' AND (STRAT_NM LIKE '%%ÀÌÆò¼±%%' or STRAT_NM LIKE '%%MA%%') ");
	if (FALSE == db->ExecQuery(sQ))
	{
		g_log.log(LOGTP_ERR, "Load Cross MA Strat Info failed(%s)(%s)", sQ, db->GetError());
		return FALSE;
	}

	if (db->IsNextRow())
	{
		db->GetStr("STRAT_ID", m_zStratID);
		db->GetStr("STRAT_NM", m_zStratNm);

	} // if (db->NextRow())
	else
	{
		g_log.log(LOGTP_ERR, "There is no Cross MA Strategy info (%s)", sQ);
		//m_bDoStrategy = FALSE;
		return FALSE;
	}
	return TRUE;
}


VOID CSignalCrossMA::MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm,
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


	CHECKSIG_RESULT_CROSSMA* pRslt = (CHECKSIG_RESULT_CROSSMA*)m_p.pResult;
	ZeroMemory(pRslt, sizeof(CHECKSIG_RESULT_CROSSMA));
	pRslt->SigRet = SIGNAL_NONE;
	sprintf(pRslt->zGroupKey, "%.*s", LEN_GROUP_KEY, m_p.zGroupKey);
	sprintf(pRslt->zChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);
	sprintf(pRslt->zStratID, "%.*s", min(sizeof(pRslt->zStratID), strlen(m_zStratID)), m_zStratID);
	sprintf(pRslt->zStratNm, "%.*s", min(sizeof(pRslt->zStratNm), strlen(m_zStratNm)), m_zStratNm);


	// return once this is already checked in this chart name
	if (IsAlreadyCheckChart())
		return;

	// if there is no sma, just return
	if( atof(pChartNow->sma_short)<=0 ){
		SetCheckLastChart();
		return;
	}

	// µ¹ÆÄ(ÇöÀç°¡°¡ ¹Ø¿¡¼­ À§·Î ma ¸¦ ¶Õ´Â´Ù. close price breakthrough the ma from below to above)
	// ºØ±«(ÇöÀç°¡°¡ À§¿¡¼­ ¹ØÀ¸·Î ma ¸¦ ¶Õ´Â´Ù. close price breakthrough the ma from above to below)
	char zPrevClose[LEN_PRC + 1], zMA[LEN_PRC+1];
	sprintf(zPrevClose, "%.*s", LEN_PRC, pChartNow->close);
	sprintf(zMA, "%.*s", LEN_PRC, pChartNow->sma_short);

	sprintf(pRslt->zMAPrice, zMA, min(sizeof(pRslt->zMAPrice),strlen(zMA)));
	sprintf(pRslt->zNowPrice, pzCurrPrc, min(sizeof(pRslt->zNowPrice), strlen(pzCurrPrc)));
	//sprintf(pRslt->zPrevPrice, zPrevClose, min(sizeof(pRslt->zPrevPrice), strlen(zPrevClose)));

	int nCompPrev = CUtil::CompPrc(zPrevClose, LEN_PRC, zMA, LEN_PRC, m_p.nDotCnt, LEN_PRC);
	int nCompNow = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zMA, LEN_PRC, m_p.nDotCnt, LEN_PRC);
	
	pRslt->SigRet = SIGNAL_NONE;

	// µ¹ÆÄ upward breakthrough
	if (nCompPrev <= 0 && nCompNow>0 )
	{
		pRslt->SigRet = SIGNAL_EXIST;
		pRslt->Tp[0] = 'U';
	}

	// ºØ±« downward breakthrough
	if (nCompPrev >= 0 && nCompNow<0)
	{
		pRslt->SigRet = SIGNAL_EXIST;
		pRslt->Tp[0] = 'D';
	}

	// check just one time in same chart
	if(pRslt->SigRet == SIGNAL_EXIST)
		SetCheckLastChart();
}


BOOL CSignalCrossMA::IsAlreadyCheckChart()
{
	int nRet = strncmp(m_zLastCheckChartNm, m_p.zChartNm, LEN_CHART_NM);
	return (nRet >= 0);
}

VOID CSignalCrossMA::SetCheckLastChart()
{
	sprintf(m_zLastCheckChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);
}
