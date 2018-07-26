
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
	������Ʈ�� �ð��� ���� 9�� ��Ʈ �ð� ��
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

	// ���� ��Ʈ�� 9���� �ȵǸ� pass
	int nChartCnt = m_pShmQ->GetCurrDataCnt((char*)m_p.zGroupKey);
	if (nChartCnt < CHECKCNT_CHART_OPENPRC) {
		//g_log.log(LOGTP_SUCC, "��Ʈ�� %d���� �ȵȴ�.(%.5s)(%d)", DEF_CHECK_CHART_CNT, pzGroupKey, nChartCnt);
		SetLastCheckEmptyArrow();
		return;
	}

	// �̹� üũ������ pass
	if (IsAlreadyCheckEmptyArrow())
		return;

	/*
		���簡 ���� �� ����
		���� ��Ʈ �ð��� ���� 9�� ��
	*/

	ST_SHM_CHART_UNIT	chartUnit[CHECKCNT_CHART_OPENPRC];	//	�Ϸ�� ��Ʈ 9��

	INT nFoundPlus = -1, nFoundMinus = -1;
	char zBaseOpenPrc[LEN_PRC + 1];
	char prevChartNm[LEN_CHART_NM + 1];

	sprintf(zBaseOpenPrc, "%.*s", LEN_PRC, m_p.pChartNow->open);	// ������Ʈ�� �ð��� ����. (CheckFilledArrow ���� ������)
	
	//g_log.log(LOGTP_SUCC, "[STRAT_OPEN.%s][%s][NOW:%.*s](%c)(%10.5f)",
	//	AcntNo(), pzGroupKey, LEN_CHART_NM, nowChart.Nm, nowChart.gb[0], atof(nowChart.open));

	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, m_p.pChartNow->prevNm);
	int nLoop = CHECKCNT_CHART_OPENPRC;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_pShmQ->DataGet((char*)m_p.zGroupKey, (char*)prevChartNm, (char*)&chartUnit[i]))
		{
			g_log.log(LOGTP_ERR, "[CheckEmptyArrow][%s][%s]��Ʈ�� SHM �� ����", prevChartNm, m_p.zGroupKey);
			continue;
		}

		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, chartUnit[i].prevNm);

		//g_log.log(LOGTP_SUCC, "[strat_open:%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, chart[i].Nm, chart[i].gb[0], atof(chart[i].open));

		// ������ ����� �ƴ� ���
		if (chartUnit[i].gb[0] != DEF_CANDLE_PLUS && chartUnit[i].gb[0] != DEF_CANDLE_MINUS) {
			continue;
		}

		// �ð��� ��ġ�ϴ°�?
		BOOL bSame = CUtil::IsSamePrice(zBaseOpenPrc, LEN_PRC,chartUnit[i].open, sizeof(chartUnit[i].open), m_p.nDotCnt, LEN_PRC);
		if (!bSame)
			continue;

		// ��� �Ǵ� �����ΰ�?
		if (chartUnit[i].gb[0] == DEF_CANDLE_PLUS)	nFoundPlus = i;
		if (chartUnit[i].gb[0] == DEF_CANDLE_MINUS)	nFoundMinus = i;

	}
	//g_log.log(LOGTP_SUCC, "--------------------------------------------------------");

	if ((nFoundPlus == -1) && (nFoundMinus == -1))
	{
		// ������Ʈ �ð��� ��ġ�ϴ� �ð��� ���� ��Ʈ�� �����Ƿ� ���̻� �̹� ��Ʈ�� üũ���� �ʴ´�.		
		SetLastCheckEmptyArrow();
		return;
	}


	// �Ѵ� ��ġ. ��ġ�ϴ� ��Ʈ�� �־ ����, ��� ��� ��ġ�ϸ� �ȵȴ�.
	if (nFoundPlus > -1 && nFoundMinus > -1)
	{
		SetLastCheckEmptyArrow();
		return ;
	}


	// ��ġ�ϴ� ������ ����
	pResult->SigRet = SIGNAL_EXIST;
	memcpy(pResult->zSigPrc, zBaseOpenPrc, MIN(sizeof(pResult->zSigPrc), strlen(zBaseOpenPrc)));

	// �����ġ
	if (nFoundPlus > -1)
	{
		// GET CROSS CHECK
		CROSS_TP crossTp = GetCross(m_p.pChartNow);
		pResult->nCrossTp = (int)crossTp;
		pResult->ChartGb[0] = DEF_CANDLE_PLUS;		
		SetLastCheckEmptyArrow();

		//// �ż�Signal ǥ�� (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_BUY, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]�ż���ȣ�߻�(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
		//	g_log.log(LOGTP_SUCC, msg);
		//	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		//}
		return;
	}

	// ������ġ
	if (nFoundMinus > -1)
	{
		// GET CROSS CHECK
		CROSS_TP crossTp = GetCross(m_p.pChartNow);
		pResult->nCrossTp = (int)crossTp;
		pResult->ChartGb[0] = DEF_CANDLE_MINUS;
		SetLastCheckEmptyArrow();

		//// �ŵ�Signal ǥ�� (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_SELL, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]�ŵ���ȣ�߻�(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
		//	g_log.log(LOGTP_SUCC, msg);
		//	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		//}

		return ;
	}

}


// ������ �ϼ��� ��Ʈ�� �ð��� �� ���� 9�� ��Ʈ �ð� ��
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

	// ���� ��Ʈ�� 9���� �ȵǸ� pass
	int nChartCnt = m_pShmQ->GetCurrDataCnt((char*)m_p.zGroupKey);
	if (nChartCnt < CHECKCNT_CHART_OPENPRC) {
		//g_log.log(LOGTP_SUCC, "��Ʈ�� %d���� �ȵȴ�.(%.5s)(%d)", DEF_CHECK_CHART_CNT, pzGroupKey, nChartCnt);
		return;
	}

	/*
		������ �ϼ��� ��Ʈ�� �ð��� �� ���� 9�� ��Ʈ �ð� ��
	*/

	ST_SHM_CHART_UNIT	chartUnit[CHECKCNT_CHART_OPENPRC];	//	�Ϸ�� ��Ʈ 9��

	INT nFoundPlus = -1, nFoundMinus = -1;
	char zBaseOpenPrc[LEN_PRC + 1];
	char prevChartNm[LEN_CHART_NM + 1];

	sprintf(zBaseOpenPrc, "%.*s", LEN_PRC, m_p.pChartPrev->open);		// CheckEmptyArrow �� ������(difference from CheckEmptyArrow)
	sprintf(prevChartNm, "%.*s", LEN_CHART_NM, m_p.pChartPrev->prevNm);

	int nLoop = CHECKCNT_CHART_OPENPRC;	//(nChartCnt < m_nMaxChartCnt) ? nChartCnt : m_nMaxChartCnt;
	for (int i = 0; i < nLoop; i++)
	{
		if (strcmp(DEFINE_NO_CHART, prevChartNm) == 0)	break;

		if (FALSE == m_pShmQ->DataGet((char*)m_p.zGroupKey, (char*)prevChartNm, (char*)&chartUnit[i]))
		{
			g_log.log(LOGTP_ERR, "[CheckFilledArrow][%s][%s]��Ʈ�� SHM �� ����", prevChartNm, m_p.zGroupKey);
			continue;
		}

		sprintf(prevChartNm, "%.*s", LEN_CHART_NM, chartUnit[i].prevNm);

		//g_log.log(LOGTP_SUCC, "[strat_open:%d][%s][NOW:%.*s](%c)(%10.5f)",
		//	i, pzGroupKey, LEN_CHART_NM, chart[i].Nm, chart[i].gb[0], atof(chart[i].open));

		// ������ ����� �ƴ� ���
		if (chartUnit[i].gb[0] != DEF_CANDLE_PLUS && chartUnit[i].gb[0] != DEF_CANDLE_MINUS) {
			continue;
		}

		// �ð��� ��ġ�ϴ°�?
		BOOL bSame = CUtil::IsSamePrice(zBaseOpenPrc, LEN_PRC,
											chartUnit[i].open, sizeof(chartUnit[i].open), m_p.nDotCnt, LEN_PRC);
		if (!bSame)
			continue;

		// ��� �Ǵ� �����ΰ�?
		if (chartUnit[i].gb[0] == DEF_CANDLE_PLUS)	nFoundPlus = i;
		if (chartUnit[i].gb[0] == DEF_CANDLE_MINUS)	nFoundMinus = i;

	}


	//	�ϼ��� ��Ʈ ������ ���̹Ƿ� ���� �к������� �ѹ��� �����ϸ� �ȴ�.
	//	Just check one time because compares already passed candle charts
	SetLastCheckFilledArrow();

	if ((nFoundPlus == -1) && (nFoundMinus == -1))
	{
		return ;
	}


	// �Ѵ� ��ġ. ��ġ�ϴ� ��Ʈ�� �־ ����, ��� ��� ��ġ�ϸ� �ȵȴ�.
	if (nFoundPlus > -1 && nFoundMinus > -1)
	{
		return ;
	}

	// ��ġ�ϴ� ������ ����
	pResult->SigRet = SIGNAL_EXIST;
	memcpy(pResult->zSigPrc, zBaseOpenPrc, MIN(sizeof(pResult->zSigPrc), strlen(zBaseOpenPrc)));

	// �����ġ
	if (nFoundPlus > -1)
	{
		// TODO. CROSS CHECK?
		// TODO. CROSS_TP crossTp = GetCross(prevChart, pTrade);
		// TODO. if ((crossTp == DEAD_CROSS) && (side == DEF_SELL) || (crossTp == GOLDEN_CROSS) && (side == DEF_BUY))

		pResult->ChartGb[0] = DEF_CANDLE_PLUS;
		
		//// �ż�Signal ǥ�� (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_BUY, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]�ż���ȣ�߻�(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
		//	g_log.log(LOGTP_SUCC, msg);
		//	m_pStratLog->LoggingEx(AcntNo(), sSymbol.GetBuffer(), (CHART_TP)(pTrade->Tp()), LOG1_STRAT_RQMNT, LOG2_STRAT, msg);
		//}
		return;
	}

	// ������ġ
	if (nFoundMinus > -1)
	{
		//? CROSS_TP crossTp = GetCross(prevChart, pTrade);
		//? TODO. if ((crossTp == DEAD_CROSS) && (side == DEF_SELL) || (crossTp == GOLDEN_CROSS) && (side == DEF_BUY))

		pResult->ChartGb[0] = DEF_CANDLE_MINUS;
		
		//// �ŵ�Signal ǥ�� (0606)
		//if ((nowChart.signalGb[0] != DEF_SELL) && (nowChart.signalGb[0] != DEF_BUY)) {
		//	SetSignal(DEF_SELL, pzGroupKey, pzNowChartNm);
		//	sprintf(msg, "%s%s[Strat_Open][SetSignal]�ŵ���ȣ�߻�(%s)(%s)", LOG1_STRAT_RQMNT, LOG2_STRAT, AcntNo(), pzNowChartNm);
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
	sprintf(sQ, "SELECT STRAT_ID, STRAT_NM FROM STRAT_MST WHERE STRAT_USE_YN='Y' AND STRAT_TP='S' AND (STRAT_NM LIKE '%%�ð�%%' or STRAT_NM LIKE '%%OpenPrice%%') ");
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