#pragma once
#include "SignalBase.h"
#define CHECKCNT_CHART_ASSIST	9



class CSignalAssist :public CSignalBase
{
public:
	CSignalAssist(CQueueShm *pShm, void* pDBPool);
	~CSignalAssist();

	virtual BOOL Begin();
	virtual VOID  MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm,
		ST_SHM_CHART_UNIT *pChartNow, ST_SHM_CHART_UNIT *pChartPrev, char* pzTickSize, int nDotCnt,
		void* pResult, int nOption);

	VOID	AssistSet();
	VOID	AssistClr();

	BOOL	IsAlreadyCheckChart();
	VOID	SetCheckLastChart();

	VOID	SetAssistPrc(char* pzAssistPrc) { sprintf(m_zAssistPrc, "%.*s", LEN_PRC, pzAssistPrc); }
	VOID	ClearAssistPrc(char* pzAssistPrc) { ZeroMemory(m_zAssistPrc, sizeof(m_zAssistPrc)); }
	BOOL	IsAissistSet() { return (m_zAssistPrc[0] != 0); }

private:
	char	m_zAssistPrc[LEN_PRC + 1];
	char	m_zLastCheckChartNm[128];
};

