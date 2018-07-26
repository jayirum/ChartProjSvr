#pragma once
#include "SignalBase.h"
class CSignalCrossMA :	public CSignalBase
{
public:
	CSignalCrossMA(CQueueShm *pShm, void* pDBPool);
	~CSignalCrossMA();


	virtual BOOL Begin();
	virtual VOID  MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm,
		ST_SHM_CHART_UNIT *pChartNow, ST_SHM_CHART_UNIT *pChartPrev, char* pzTickSize, int nDotCnt,
		void* pResult, int nOption);

private:
	BOOL		IsAlreadyCheckChart();
	VOID		SetCheckLastChart();

private:

	char		m_zLastCheckChartNm[128];
};

