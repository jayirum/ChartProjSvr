#pragma once
#include "SignalBase.h"


#define CHECKCNT_CHART_OPENPRC	9



class CSignalOpenPrc :	public CSignalBase
{
public:
	CSignalOpenPrc(CQueueShm *pShm, void* pDBPool);
	~CSignalOpenPrc();

	virtual BOOL Begin();
	virtual VOID  MakeSignal(_In_ CHART_TP tp, _In_ char* pzCurrPrc, _In_ char* pzGroupKey, _In_ char* pzChartNm,
		_In_ ST_SHM_CHART_UNIT *pChartNow, _In_ ST_SHM_CHART_UNIT *pChartPrev, _In_ char* pzTickSize, _In_ int nDotCnt,
		_Out_ void* pResult, int nOption);

	BOOL		IsAlreadyCheckFilledArrow();
	VOID		SetLastCheckFilledArrow();

	BOOL		IsAlreadyCheckEmptyArrow();
	VOID		SetLastCheckEmptyArrow();
private:
	char		m_zLastCheckFilledArrow[128];
	char		m_zLastCheckEmptyArrow[128];

	VOID	CheckEmptyArrow();
	VOID	CheckFilledArrow();

};

