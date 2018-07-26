#include "SignalBase.h"
#include "../../IRUM_UTIL/Util.h"

CSignalBase::CSignalBase(CQueueShm *pShm, void* pDBPool)
{
	m_pShmQ = pShm;
	m_pDBPool = pDBPool;
}


CSignalBase::~CSignalBase()
{
}


CROSS_TP CSignalBase::GetCross(ST_SHM_CHART_UNIT* shmLast)
{
	int nRet = CUtil::CompPrc(shmLast->sma_short, sizeof(shmLast->sma_short),
								shmLast->sma_long, sizeof(shmLast->sma_long), 
								m_p.nDotCnt, LEN_PRC);
	
	CROSS_TP tp;

	double dShort = S2D((char*)shmLast->sma_short, sizeof(shmLast->sma_short));
	double dLong = S2D((char*)shmLast->sma_long, sizeof(shmLast->sma_long));
	if (dShort <= 0 || dLong <= 0)
	{
		sprintf(m_zMsg, "[GetCross]<NONE_CROSS-데이터이상> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
		tp = NONE_CROSS;
	}

	if (nRet == 0) {
		tp = NONE_CROSS;
		sprintf(m_zMsg, "[GetCross]<NONE_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}
	else if (nRet > 0) {
		tp = GOLDEN_CROSS;
		sprintf(m_zMsg, "[GetCross]<GOLDEN_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}
	else {
		tp = DEAD_CROSS;
		sprintf(m_zMsg, "[GetCross]<DEAD_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}

	//g_log.log(LOGTP_SUCC,"%s(%s)", msg, AcntNo());
	return tp;
}

//VOID CSignalBase::SetLastCheckChartNm(char* pzChartNm)
//{
//	strcpy(m_zLastCheckChart, pzChartNm);
//}

//BOOL CSignalBase::IsAlreadyCheck(char* pzChartNm)
//{
//	int nRet = strncmp(m_zLastCheckChart, pzChartNm, LEN_CHART_NM);
//	return (nRet >= 0);
//}
