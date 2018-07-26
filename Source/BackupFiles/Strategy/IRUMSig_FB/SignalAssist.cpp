#include "../../IRUM_UTIL/ADOFunc.h"
#include "SignalAssist.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"

extern CLogMsg g_log;




CSignalAssist::CSignalAssist(CQueueShm *pShm, void* pDBPool) : CSignalBase(pShm, pDBPool)
{
}


CSignalAssist::~CSignalAssist()
{
}

VOID CSignalAssist::MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm,
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
	strcpy(m_p.zCurrPrc, pzCurrPrc);

	if (nOption == 1)
		AssistSet();
	else
		AssistClr();
}

VOID CSignalAssist::AssistSet()
{
	CHECKSIG_RESULT_ASSIST* pRslt = (CHECKSIG_RESULT_ASSIST*)m_p.pResult;
	ZeroMemory(pRslt, sizeof(CHECKSIG_RESULT_ASSIST));
	pRslt->SigRet = SIGNAL_NONE;
	sprintf(pRslt->zGroupKey, "%.*s", LEN_GROUP_KEY, m_p.zGroupKey);
	sprintf(pRslt->zChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);
	sprintf(pRslt->zStratID, "%.*s", min(sizeof(pRslt->zStratID), strlen(m_zStratID)), m_zStratID);
	sprintf(pRslt->zStratNm, "%.*s", min(sizeof(pRslt->zStratNm), strlen(m_zStratNm)), m_zStratNm);
	pRslt->cClrYN[0] = 'N';

	// �̹� CHECK ������ ������.
	// sDone(1201) ���� zNowChartNm(1201 or 1200) �� ���ų� ����.
	if( IsAlreadyCheckChart())
		return;

	//// ���� ��Ʈ�� 10���� �ȵǸ� pass (���� �������� 9�� �̹Ƿ�)
	int nChartCnt = m_pShmQ->GetCurrDataCnt((char*)m_p.zGroupKey);
	if (nChartCnt < CHECKCNT_CHART_ASSIST + 1) {
		//g_log.log(LOGTP_SUCC, "��Ʈ�� %d���� �ȵȴ�.(%.5s)(%d)", DEF_CHECK_CHART_CNT + 1, pzGroupKey, nChartCnt);
		SetCheckLastChart();
		return;
	}

	/*
	1. ���� ĵ���� ������ �����´�.
	2. �� ���� 9���� ĵ���� ������ ���Ѵ�.
	3. ����� ���, ������ ����
	*/
	ST_SHM_CHART_UNIT	baseChart, checkChart;
	char prevChartNm[LEN_CHART_NM + 1], zMatchedChartNm[LEN_CHART_NM+1];
	INT nFound = -1;
	BOOL bSame;

	// if previous candle is not minus candle nor plus candle, skip this candle
	if (m_p.pChartPrev->gb[0] != DEF_CANDLE_PLUS && m_p.pChartPrev->gb[0] != DEF_CANDLE_MINUS)
	{
		//g_log.log(LOGTP_SUCC, "[STRAT_CLOSE] ���, ���� ��� �ƴϹǷ� skip(%s)(%s)(%.*s)(%10.5f)", 
		//			AcntNo(), pzGroupKey, LEN_CHART_NM, lastChart.Nm, atof(lastChart.close) );
		SetCheckLastChart();
		return;
	}
	memcpy(&baseChart, m_p.pChartPrev, sizeof(baseChart));
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, baseChart.prevNm);
	//g_log.log(LOGTP_SUCC, "STRAT_CLOSE[%s][�񱳱�����Ʈ:%.*s](%c)(%10.5f)",
	//	 pzGroupKey, LEN_CHART_NM, lastChart.Nm, lastChart.gb[0], atof(lastChart.close));

	// ���� ĵ�� ���� 9��
	int nLoop = CHECKCNT_CHART_ASSIST;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_pShmQ->DataGet((char*)m_p.zGroupKey, (char*)prevChartNm, (char*)&checkChart))
		{
			g_log.log(LOGTP_ERR, "[STRAT_ASSIST][%s][%s]��Ʈ�� SHM �� ����", prevChartNm, m_p.zGroupKey);
			continue;
		}
		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, checkChart.prevNm);

		//g_log.log(LOGTP_SUCC, "start_close[%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, checkChart.Nm, checkChart.gb[0], atof(checkChart.close));

		// ��ȣ�� �ٸ��� PASS
		if (baseChart.gb[0] != checkChart.gb[0])
			continue;

		// ���� ��ġ�ϴ°�?
		bSame = CUtil::IsSamePrice(
			checkChart.close, sizeof(checkChart.close),
			baseChart.close, sizeof(baseChart.close),
			m_p.nDotCnt, LEN_PRC);
		if (bSame)
		{
			strcpy(zMatchedChartNm, prevChartNm);
			nFound = i;
			break;
		}
	}
	//g_log.log(LOGTP_SUCC, "--------------------------------------------------------");

	// �ߺ����� ����	
	SetCheckLastChart();

	if (nFound == -1)
		return;
	
	pRslt->SigRet = SIGNAL_EXIST;
	sprintf(pRslt->zAssistPrc, "%.*s", LEN_PRC, baseChart.close);
	strcpy(pRslt->zMatchedChartNm, zMatchedChartNm);
	pRslt->ChartGb[0] = baseChart.gb[0];


	// ������ǥ���� ���� �ð� 
	SetAssistPrc(pRslt->zAssistPrc);
}

/*
���簡�� ������ǥ�� ���Ͽ� ������ �´� ��� û��
1. �ر� : ������ǥ > ���簡 => �ż������� �ŵ��ֹ� û�길
2. ���� : ������ǥ < ���簡 => �ŵ������� �ż��ֹ� û�길
*/
VOID	CSignalAssist::AssistClr()
{
	CHECKSIG_RESULT_ASSIST* pRslt = (CHECKSIG_RESULT_ASSIST*)m_p.pResult;
	ZeroMemory(pRslt, sizeof(CHECKSIG_RESULT_ASSIST));
	pRslt->SigRet = SIGNAL_NONE;
	sprintf(pRslt->zGroupKey, "%.*s", LEN_GROUP_KEY, m_p.zGroupKey);
	sprintf(pRslt->zChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);
	sprintf(pRslt->zStratID, "%.*s", min(sizeof(pRslt->zStratID), strlen(m_zStratID)), m_zStratID);
	sprintf(pRslt->zStratNm, "%.*s", min(sizeof(pRslt->zStratNm), strlen(m_zStratNm)), m_zStratNm);

	// ������ǥ�� ������ ���°� �ƴϸ� ������.
	if (!IsAissistSet())
		return;

	// ������ǥ�� ���簡 ��
	int nComp = CUtil::CompPrc(m_zAssistPrc, strlen(m_zAssistPrc), m_p.zCurrPrc, strlen(m_p.zCurrPrc), m_p.nDotCnt, LEN_PRC);

	// ������ PASS
	if (nComp == 0)
		return;

	// ������ǥ����
	sprintf(pRslt->zAssistPrc, "%.*s", LEN_PRC, m_zAssistPrc);

	// �ر� : ���簡 < ������ǥ
	if (nComp > 0)
	{
		//TODO. LOG
		pRslt->SigRet = SIGNAL_EXIST;
		pRslt->cClrYN[0] = 'Y';
		pRslt->cClrCrossTp[0] = 'D';
		sprintf(pRslt->zClrAssistPrc, "%.*s", LEN_PRC, m_zAssistPrc);
		sprintf(pRslt->zClrNowPrc, "%.*s", LEN_PRC, m_p.zCurrPrc);
	}

	// ���� : ���簡 > ������ǥ
	if (nComp < 0)
	{
		//TODO. LOG
		pRslt->SigRet = SIGNAL_EXIST;
		pRslt->cClrYN[0] = 'Y';
		pRslt->cClrCrossTp[0] = 'U';
		sprintf(pRslt->zClrAssistPrc, "%.*s", LEN_PRC, m_zAssistPrc);
		sprintf(pRslt->zClrNowPrc, "%.*s", LEN_PRC, m_p.zCurrPrc);
	}

	// ������ǥ���� ������ ������ clear 
	ClearAssistPrc(m_zAssistPrc);

}

BOOL CSignalAssist::IsAlreadyCheckChart()
{
	int nRet = strncmp(m_zLastCheckChartNm, m_p.zChartNm, LEN_CHART_NM);
	return (nRet >= 0);
}

VOID CSignalAssist::SetCheckLastChart()
{
	sprintf(m_zLastCheckChartNm, "%.*s", LEN_CHART_NM, m_p.zChartNm);	
}


BOOL CSignalAssist::Begin()
{
	CDBHandlerAdo db(DBPoolPtr(m_pDBPool)->Get());

	char sQ[1024];
	sprintf(sQ, "SELECT STRAT_ID,STRAT_NM FROM STRAT_MST WHERE STRAT_USE_YN='Y' AND STRAT_TP='S' AND (STRAT_NM LIKE '%%����%%' OR STRAT_NM LIKE '%%����%%' or STRAT_NM LIKE '%%Assist%%')");
	if (FALSE == db->ExecQuery(sQ))
	{
		g_log.log(LOGTP_ERR, "Load Assist Strat Info failed(%s)(%s)", sQ, db->GetError());
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
