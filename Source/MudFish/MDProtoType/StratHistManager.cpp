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
	���� �� �ְ������ 50% ���Ϸ� �������� ���� �� �����ΰ�?

ex)
	�ż����� 100�϶� (�ְ� - ���԰�)
		�ְ� 120  ==> gap : 20
		���簡 115  ==> gap : 15 -> ���� ���Ѵ�.
		���簡 109  ==> gap : 9  -> �����Ѵ�.
		���簡 99   ==> gap : -1 -> �����Ѵ�.

	�ŵ����� 100�϶� (���԰� - �ְ�)
		�ְ� 80  ==> gap : 20
		���簡 85  ==> gap : 15 -> ���� ���Ѵ�.
		���簡 95  ==> gap : 5  -> �����Ѵ�.
		���簡 101   ==> gap : -1 -> �����Ѵ�.
*/
BOOL CStratHistManager::IsPTCondition(double dCurrPrc, _Out_ char* pMsg)
{
	BOOL bResult = FALSE;
	double dGapMax, dGapCurr = 0;

	// ���� LONG
	if (m_nStatus & 1)
	{
		dGapMax		= m_dMaxPLPrc - m_dEntryPrc;
		dGapCurr = dCurrPrc - m_dEntryPrc;
		if (dGapCurr < dGapMax*0.5) {
			bResult = TRUE;
			sprintf(pMsg, " LONG��������.(���԰�:%f) (�ְ�:%f) (���簡:%f)",
				m_dEntryPrc, m_dMaxPLPrc, dCurrPrc);
		}
	}

	// ���� Short
	if (m_nStatus & 2)
	{
		dGapMax = m_dEntryPrc - m_dMaxPLPrc;
		dGapCurr = m_dEntryPrc - dCurrPrc;
		if (dGapCurr < dGapMax*0.5) {
			bResult = TRUE;
			sprintf(pMsg, "SHORT��������.(���԰�:%f) (�ְ�:%f) (���簡:%f)",
				m_dEntryPrc, m_dMaxPLPrc, dCurrPrc);
		}
	}

	return bResult;
}


// ���� �� �ְ� ���Ͱ����� �����ϱ� ����
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
