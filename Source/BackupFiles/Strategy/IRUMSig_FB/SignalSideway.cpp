
#include "../../IRUM_UTIL/ADOFunc.h"
#include "SignalSideWay.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include <stdio.h>
#include <map>
#include <string>

extern CLogMsg g_log;

CSignalSideWay::CSignalSideWay(CQueueShm *pShm, void* pDBPool) : CSignalBase(pShm, pDBPool)
{
}


CSignalSideWay::~CSignalSideWay()
{
}


BOOL CSignalSideWay::Begin()
{
	CDBHandlerAdo db(DBPoolPtr(m_pDBPool)->Get());

	char sQ[1024];
	sprintf(sQ, "SELECT STRAT_ID,STRAT_NM FROM STRAT_MST WHERE STRAT_USE_YN='Y' AND STRAT_TP='S' AND (STRAT_NM LIKE '%%횡보%%' or STRAT_NM LIKE '%%SideWay%%')  ");
	if (FALSE == db->ExecQuery(sQ))
	{
		g_log.log(LOGTP_ERR, "Load Cross Sideway Strat Info failed(%s)(%s)", sQ, db->GetError());
		return FALSE;
	}

	if (db->IsNextRow())
	{
		db->GetStr("STRAT_ID", m_zStratID);
		db->GetStr("STRAT_NM", m_zStratNm);

	} // if (db->NextRow())
	else
	{
		g_log.log(LOGTP_ERR, "There is no Cross Sideway Strategy info (%s)", sQ);
		//m_bDoStrategy = FALSE;
		return FALSE;
	}
	return TRUE;
}


VOID CSignalSideWay::MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm,
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


	CHECKSIG_RESULT_SIDEWAY* pRslt = (CHECKSIG_RESULT_SIDEWAY*)m_p.pResult;
	ZeroMemory(pRslt, sizeof(CHECKSIG_RESULT_SIDEWAY));
	pRslt->SigRet = SIGNAL_NONE;
	sprintf(pRslt->zGroupKey, "%.*s", LEN_GROUP_KEY, m_p.zGroupKey);
	sprintf(pRslt->zChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);
	sprintf(pRslt->zStratID, "%.*s", min(sizeof(pRslt->zStratID), strlen(m_zStratID)), m_zStratID);
	sprintf(pRslt->zStratNm, "%.*s", min(sizeof(pRslt->zStratNm), strlen(m_zStratNm)), m_zStratNm);
	pRslt->Tp[0] = '0';

	// return once this is already checked in this chart name
	if (IsAlreadyCheckChart())
		return;

	// 과거 차트가 10개가 안되면 pass (chart count should be 10 at least)
	int nChartCnt = m_pShmQ->GetCurrDataCnt((char*)m_p.zGroupKey);
	if (nChartCnt < CHECKCNT_SIDEWAY) {
		//g_log.log(LOGTP_SUCC, "차트가 %d개가 안된다.(%.5s)(%d)", DEF_CHECK_CHART_CNT, pzGroupKey, nChartCnt);
		return;
	}

	// last close
	char zLastPrc[LEN_PRC + 1];
	sprintf(zLastPrc, "%.*s", LEN_PRC, m_p.pChartNow->close);

	// 과거 10개 차트 에서 고가가 2개 이상 일치하는 차트와 저가가 2개 이상 일치하는 차트를 찾는다.
	// find same high price and same low price in previous 10 charts	
	//std::map <string> mHigh, mLow;
	std::map<std::string, int, std::greater<std::string> > mH;
	std::map<std::string, int, std::less<std::string> > mL;
	std::map<std::string, int, std::greater<std::string> >::iterator iH;
	std::map<std::string, int, std::less<std::string> >::iterator iL;

	char zHighChartNm[LEN_CHART_NM + 1] = { 0, } , zHighPrice[LEN_PRC + 1] = { 0, };
	char zLowChartNm[LEN_CHART_NM + 1] = { 0, }, zLowPrice[LEN_PRC + 1] = { 0, };

	ST_SHM_CHART_UNIT chartUnit;
	char zPrice[128];
	std::string sPrice;
	char prevChartNm[LEN_CHART_NM + 1];
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, m_p.pChartNow->prevNm);
	
	for (int i = 0; i < CHECKCNT_SIDEWAY; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_pShmQ->DataGet((char*)m_p.zGroupKey, (char*)prevChartNm, (char*)&chartUnit))
		{
			g_log.log(LOGTP_ERR, "[Sideway::MakeSignal][%s][%s]차트가 SHM 에 없다", prevChartNm, m_p.zGroupKey);
			continue;
		}

		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, chartUnit.prevNm);

		// high price
		sprintf(zPrice, "%.*s", sizeof(chartUnit.high), chartUnit.high);
		sPrice = zPrice;
		iH = mH.find(sPrice);
		
		if (iH != mH.end())
		{// same price
			int nCnt = (*iH).second + 1;
			mH[sPrice] = nCnt;
		}
		else
		{// different price
			mH[sPrice] = 1;
		}

		// low price
		sprintf(zPrice, "%.*s", sizeof(chartUnit.low), chartUnit.low);
		sPrice = zPrice;
		iL = mL.find(sPrice);
		if (iL != mL.end())
		{// same price
			int nCnt = (*iL).second + 1;
			mL[sPrice] = nCnt;
		}
		else
		{// different price
			mL[sPrice] = 1;
		}
	} // for (int i = 0; i < CHECKCNT_SIDEWAY; i++)

	// descending . 같은 가격 있으면 check cross upward.
	for (iH = mH.begin(); iH != mH.end(); iH++)
	{
		int nCnt = (*iH).second;
		if (nCnt > 1)
		{
			sprintf(zHighChartNm, "%.*s", sizeof(chartUnit.Nm), chartUnit.Nm);
			sprintf(zHighPrice, "%.*s", sizeof(chartUnit.high), chartUnit.high);

			// 돌파(현재가가 밑에서 위로 high 를 뚫는다. close price breakthrough the high from below to above)
			int nCompLast = CUtil::CompPrc(zLastPrc, LEN_PRC, zHighPrice, LEN_PRC, m_p.nDotCnt, LEN_PRC);
			int nCompNow = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zHighPrice, LEN_PRC, m_p.nDotCnt, LEN_PRC);

			// 돌파 upward breakthrough
			if (nCompLast <= 0 && nCompNow>0)
			{
				pRslt->SigRet = SIGNAL_EXIST;
				pRslt->Tp[0] = 'U';
				memcpy(pRslt->zChartNm, zHighChartNm, min(sizeof(pRslt->zChartNm), strlen(zHighChartNm)));
				memcpy(pRslt->zHihgLowPrice, zHighPrice, min(sizeof(pRslt->zHihgLowPrice), strlen(zHighPrice)));
				SetCheckLastChart();
			}
			return;
		}
	}

	// ascending . 같은 가격 있으면 바로 check cross downward
	for (iL = mL.begin(); iL != mL.end(); iL++)
	{
		int nCnt = (*iL).second;
		if (nCnt > 1)
		{
			sprintf(zLowChartNm, "%.*s", sizeof(chartUnit.Nm), chartUnit.Nm);
			sprintf(zLowPrice, "%.*s", sizeof(chartUnit.low), chartUnit.low);

			int nCompLast = CUtil::CompPrc(zLastPrc, LEN_PRC, zLowPrice, LEN_PRC, m_p.nDotCnt, LEN_PRC);
			int nCompNow = CUtil::CompPrc(pzCurrPrc, LEN_PRC, zLowPrice, LEN_PRC, m_p.nDotCnt, LEN_PRC);

			// 붕괴 downward breakthrough
			if (nCompLast >= 0 && nCompNow<0)
			{
				pRslt->SigRet = SIGNAL_EXIST;
				memcpy(pRslt->zChartNm, zLowChartNm, min(sizeof(pRslt->zChartNm),strlen(zLowChartNm)));
				memcpy(pRslt->zHihgLowPrice, zLowPrice, min(sizeof(pRslt->zHihgLowPrice), strlen(zLowPrice)));
				pRslt->Tp[0] = 'D';
				SetCheckLastChart();
			}

			return;
		}
	}
}


BOOL CSignalSideWay::IsAlreadyCheckChart()
{
	int nRet = strncmp(m_zLastCheckChartNm, m_p.zChartNm, LEN_CHART_NM);
	return (nRet >= 0);
}

VOID CSignalSideWay::SetCheckLastChart()
{
	sprintf(m_zLastCheckChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);
}
