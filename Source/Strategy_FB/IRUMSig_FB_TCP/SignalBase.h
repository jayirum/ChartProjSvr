#pragma once

#include "../../IRUM_UTIL/QueueShm.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "SignalResultStruct.h"


typedef struct _ST_PARAMS
{
	CHART_TP	chartTp;
	char		zGroupKey[128];
	char		zChartNm[128];
	ST_SHM_CHART_UNIT	*pChartNow, *pChartPrev;
	char		zTickSize[128];
	int			nDotCnt;
	char		zCurrPrc[LEN_PRC + 1];
	void		*pResult;
}ST_PARAMS;


class CSignalBase
{
public:
	CSignalBase(CQueueShm *pShm, void* pDBPool);
	~CSignalBase();

	virtual BOOL Begin() = 0;
	virtual VOID  MakeSignal(CHART_TP tp, char* pzCurrPrc, char* pzGroupKey, char* pzChartNm, 
		ST_SHM_CHART_UNIT *pChartNow, ST_SHM_CHART_UNIT *pChartPrev, char* pzTickSize, int nDotCnt,
		void* pResult, int nOption=0) = 0;
	
	//VOID SetLastCheckChartNm(char* pzChartNm);
	//BOOL IsAlreadyCheck(char* pzChartNm);

	char* GetMsg() { return m_zMsg; }

protected:
	CROSS_TP GetCross(ST_SHM_CHART_UNIT* shmLast);
protected:
	CQueueShm	*m_pShmQ;
	void		*m_pDBPool;
	char		m_zStratID[32];
	char		m_zStratNm[128];
	char		m_zMsg[1024];

	ST_PARAMS	m_p;
};

