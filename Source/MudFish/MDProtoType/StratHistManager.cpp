#include "StratHistManager.h"
#include "../../IRUM_UTIL/Util.h"


CStratHistManager::CStratHistManager(char* pzMaxSLCnt)
{
	m_nStatus = 0;
	m_nTotSLCnt = 0;
	m_dMaxPLPrc = 0.;
	m_dEntryPrc = 0.;
	m_nMaxSLCnt = atoi(pzMaxSLCnt);
}


CStratHistManager::~CStratHistManager()
{
	m_nStatus = 0;
	m_nTotSLCnt = 0;
}



char CStratHistManager::IsOpenSrategyExist()
{
	char cCurrStra = 'N';

	if (m_nStatus & 1)
		cCurrStra = CD_BUY;
	if (m_nStatus & 2)
		cCurrStra = CD_SELL;
			
	return cCurrStra;
}


/*
	진입 후 최고수익의 50% 이하로 떨어져서 익절 할 조건인가?

ex)
	매수진입 100일때 (최고가 - 진입가)
		최고가 120  ==> gap : 20
		현재가 115  ==> gap : 15 -> 익절 안한다.
		현재가 109  ==> gap : 9  -> 익절한다.
		현재가 99   ==> gap : -1 -> 익절한다.

	매도진입 100일때 (진입가 - 최고가)
		최고가 80  ==> gap : 20
		현재가 85  ==> gap : 15 -> 익절 안한다.
		현재가 95  ==> gap : 5  -> 익절한다.
		현재가 101   ==> gap : -1 -> 익절한다.
*/
BOOL CStratHistManager::IsPTCondition(double dCurrPrc, _Out_ char* pMsg)
{
	BOOL bResult = FALSE;
	double dGapMax, dGapCurr = 0;

	// 현재 LONG
	if (m_nStatus & 1)
	{
		dGapMax		= m_dMaxPLPrc - m_dEntryPrc;
		dGapCurr = dCurrPrc - m_dEntryPrc;
		if (dGapCurr < dGapMax*0.5) {
			bResult = TRUE;
			sprintf(pMsg, " LONG익절조건.(진입가:%f) (최고가:%f) (현재가:%f)",
				m_dEntryPrc, m_dMaxPLPrc, dCurrPrc);
		}
	}

	// 현재 Short
	if (m_nStatus & 2)
	{
		dGapMax = m_dEntryPrc - m_dMaxPLPrc;
		dGapCurr = m_dEntryPrc - dCurrPrc;
		if (dGapCurr < dGapMax*0.5) {
			bResult = TRUE;
			sprintf(pMsg, "SHORT익절조건.(진입가:%f) (최고가:%f) (현재가:%f)",
				m_dEntryPrc, m_dMaxPLPrc, dCurrPrc);
		}
	}

	return bResult;
}


// 진입 후 최고 수익가격을 저장하기 위해
void CStratHistManager::SetMaxPLPrc(char* pzCurrPrc)
{
	if (m_nStatus & 1) {
		if (m_dMaxPLPrc == 0)
			m_dMaxPLPrc = atof(pzCurrPrc);
		else
		{
			// BUY POS. MAXPL -> HIGHER
			if (m_dMaxPLPrc > atof(pzCurrPrc))
				m_dMaxPLPrc = atof(pzCurrPrc);
		}
	}
	else if (m_nStatus & 2)
	{
		if (m_dMaxPLPrc == 0)
			m_dMaxPLPrc = atof(pzCurrPrc);
		else
		{
			// SELL POS. MAXPL -> LOWER
			if (m_dMaxPLPrc < atof(pzCurrPrc))
				m_dMaxPLPrc = atof(pzCurrPrc);
		}
	}
	//else {
	//	m_dEntryPrc = 0;
	//	m_dMaxPLPrc = 0;
	//}
}

/*
	if close strategy happens, off the corresponding open strategy
*/
void CStratHistManager::SetStrategyExist(std::string sStratID, char* pzCurrPrc)
{

	if (sStratID.compare(STRATID_BUY_OPEN) == 0)
	{
		m_nStatus ^= 1;
		m_dEntryPrc = atof(pzCurrPrc);
	}
	if (sStratID.compare(STRATID_SELL_OPEN) == 0)
	{
		m_nStatus ^= 2;
		m_dEntryPrc = atof(pzCurrPrc);
	}
	if (sStratID.compare(STRATID_SELL_SL) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~1;
		m_nTotSLCnt++;
		m_dEntryPrc = 0;
		m_dMaxPLPrc = 0;
	}
	if (sStratID.compare(STRATID_SELL_PT) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~1;
		m_dEntryPrc = 0;
		m_dMaxPLPrc = 0;
	}
	if (sStratID.compare(STRATID_BUY_SL) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~2;
		m_nTotSLCnt++;
		m_dEntryPrc = 0;
		m_dMaxPLPrc = 0;
	}
	if (sStratID.compare(STRATID_BUY_PT) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~2;
		m_dEntryPrc = 0;
		m_dMaxPLPrc = 0;
	}
}
