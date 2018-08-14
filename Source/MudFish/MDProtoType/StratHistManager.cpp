#include "StratHistManager.h"
#include "../../IRUM_UTIL/Util.h"


CStratHistManager::CStratHistManager()
{
	m_nStatus = 0;
	m_nTotSLCnt = 0;
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
	if close strategy happens, off the corresponding open strategy
*/
void CStratHistManager::SetStrategyExist(std::string sStratID)
{

	if (sStratID.compare(STRATID_BUY_OPEN) == 0)
	{
		m_nStatus ^= 1;
	}
	if (sStratID.compare(STRATID_SELL_OPEN) == 0)
	{
		m_nStatus ^= 2;
	}
	if (sStratID.compare(STRATID_SELL_SL) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~1;
		m_nTotSLCnt++;
	}
	if (sStratID.compare(STRATID_SELL_PT) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~1;
	}
	if (sStratID.compare(STRATID_BUY_SL) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~2;
		m_nTotSLCnt++;
	}
	if (sStratID.compare(STRATID_BUY_PT) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~2;
	}
}
