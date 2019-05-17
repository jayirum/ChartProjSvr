#include "TrackDiff.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/IRUM_Common.h"
#include "../../IRUM_UTIL/MemPool.h"

extern CLogMsg	g_log;
extern CLogMsg	g_logTrack;
extern char		g_zConfig[_MAX_PATH];
extern BOOL		g_bContinue;
extern CMemPool	g_memPool;



CTrackDiff::CTrackDiff()
{
}


CTrackDiff::~CTrackDiff()
{
}



BOOL CTrackDiff::Initialize()
{
	//InitializeCriticalSection(&m_csDiff[0]);
	//InitializeCriticalSection(&m_csDiff[1]);

	char zConfig[128];
	CUtil::GetConfig(g_zConfig, "TRACKING", "TARGET_NUM", zConfig);
	m_nTargetNum = atoi(zConfig);


	CUtil::GetConfig(g_zConfig, "TRACKING", "WAIT_UNTIL_TARGETNUM", zConfig);
	m_bWaitUntilTargetNum = (zConfig[0] == 'Y') ? TRUE : FALSE;
	

	return TRUE;
}

VOID CTrackDiff::Finalize()
{
	//DeleteCriticalSection(&m_csDiff[0]);
	//DeleteCriticalSection(&m_csDiff[1]);
}

VOID CTrackDiff::ThreadFunc()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_DIE)
			return;
		if (msg.message != WM_RECV_API_MD)
		{
			g_log.log(ERR, "CTracDiff �����忡 �� �� ���� �޼��� ����");
			continue;
		}

		char* pData = (char*)msg.lParam;
		int nLen = (int)msg.wParam;
		MainProcess(pData);
		g_memPool.release(pData);

	}
}

VOID CTrackDiff::MainProcess(char* pApiTick)
{
	char zCurrPrc[LEN_PRC + 1] = { 0, };

	ST_PACK2CHART_EX	*pPack = (ST_PACK2CHART_EX*)pApiTick;
	sprintf(zCurrPrc, "%.*s", sizeof(pPack->Close), pPack->Close);

	double dTick = atof(zCurrPrc);
	double dDiff;
	VALUE* pVal;
	if (strncmp(pPack->ShortCode, "6E", 2) == 0)
	{
		pVal = &m_eur;
		if (IsTrackLog(&m_eur))
		{
			TrackLogging("EUR", dTick, &m_eur);
			dDiff = dTick - m_gbp.dLast;
			TrackLogging("DIFF", dDiff, &m_diff);
		}
	}
	else
	{
		pVal = &m_gbp;
		if (IsTrackLog(&m_eur))
		{
			TrackLogging("GBP", dTick, &m_gbp);
			dDiff = m_eur.dLast - dTick;
			TrackLogging("DIFF", dDiff, &m_diff);
		}
	}

	AddNewTick(dTick, pVal);
	AddNewTick(dDiff, &m_diff);
}


BOOL CTrackDiff::IsTrackLog(VALUE* pVal)
{
	BOOL bTrack = TRUE;

	int nCurrSize = pVal->listData.size();
	if (nCurrSize < 2)
		return FALSE;

	if (nCurrSize < m_nTargetNum)
		return FALSE;

	return TRUE;
}

VOID CTrackDiff::AddNewTick(double dTick, VALUE* pVal)
{
	pVal->dLast = dTick;
	pVal->dSum += dTick;

	// ���ο� ������ Add
	pVal->listData.push_back(dTick);

	int nCurrSize = pVal->listData.size();

	// �ּ� 2���� �𿩾� �Ѵ�.
	if (nCurrSize < 2)
		return;

	if (nCurrSize > m_nTargetNum)
	{
		// ���� �տ��� ����
		pVal->dSum -= *(pVal->listData.begin());

		pVal->listData.pop_front();

	}
	else if (nCurrSize < m_nTargetNum)
	{
		// Target ���ڰ� �� ä������ �������� �׳� Add �� �Ѵ�.
		if (m_bWaitUntilTargetNum)
		{
			return;
		}
	}

	// ���
	pVal->dAvg = pVal->dSum / nCurrSize;

	// �л�� ǥ������
	m_stat.Var_StandardDev(&pVal->listData, nCurrSize, pVal->dAvg, FALSE, &pVal->dVar, &pVal->dSD);

}


// ������ TICK �� �ڱ� ���/���� �� ��
VOID CTrackDiff::TrackLogging(char* pzStkCd, double dTick, VALUE* pMyVal)
{
	double dDeviation, dDevPercent;

	// �ڱ�Ͱ� ��
	dDeviation = dTick - pMyVal->dAvg;
	dDevPercent = (dDeviation / pMyVal->dAvg) * 100.;

	// Diff �� ��
	//dDeviation[1] = dDiff - pDiffVal->dAvg;
	//dDevPercent[1] = (dDeviation[1] / pDiffVal->dAvg) * 100.;

	g_logTrack.log(INFO, "<%s> "
		"(TICK:%.5f)(AVG:%.5f)(DEV:%.5f)(PER:%.5f)"
		,
		pzStkCd,
		dTick, pMyVal->dAvg, dDeviation, dDevPercent
	);

}