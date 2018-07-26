#pragma once
#include "SignalBase.h"

#define CHECKCNT_SIDEWAY	10

class CSignalSideWay :	public CSignalBase
{
public:
	CSignalSideWay(CQueueShm *pShm, void* pDBPool);
	~CSignalSideWay();


	virtual BOOL Begin();
	virtual VOID  MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm,
		ST_SHM_CHART_UNIT *pChartNow, ST_SHM_CHART_UNIT *pChartPrev, char* pzTickSize, int nDotCnt,
		void* pResult, int nOption=0);

private:
	BOOL		IsAlreadyCheckChart();
	VOID		SetCheckLastChart();

private:

	char		m_zLastCheckChartNm[128];
};

