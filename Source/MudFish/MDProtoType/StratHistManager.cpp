#include "StratHistManager.h"
#include "../../IRUM_UTIL/Util.h"


CStratHistManager::CStratHistManager(char* pzMaxSLCnt, int nDotLen)
{
	m_nDotLen = nDotLen;
	m_nStatus = 0;
	m_nTotSLCnt = 0;
	ZeroMemory(m_zMaxPLPrc, sizeof(m_zMaxPLPrc));
	ZeroMemory(m_zEntryPrc, sizeof(m_zEntryPrc));
	m_nMaxSLCnt = atoi(pzMaxSLCnt);
	m_bHitPTPrc = FALSE;
	m_bProfitRealized = FALSE;
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
BOOL CStratHistManager::IsPTCondition(char* pzCurrPrc,_Out_ char* pMsg)
{
	BOOL bResult = FALSE;
	char zGapMax[32], zGapCurr[32];

	int nComp = CUtil::CompPrc(m_zMaxPLPrc, LEN_PRC, m_zEntryPrc, LEN_PRC, 2, LEN_PRC);
	if (nComp == 0)
		return FALSE;
	
	// ���� LONG
	if (m_nStatus & 1)
	{
		sprintf(zGapMax, "%.2f", (atof(m_zMaxPLPrc) - atof(m_zEntryPrc))*0.5);
		sprintf(zGapCurr, "%.2f", atof(pzCurrPrc) - atof(m_zEntryPrc));
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, 2, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "LONG��������.(���԰�:%s, �ְ�:%s, ���簡:%s) (�ְ�GAP/2:%s, ���簡Gap:%s)",
				m_zEntryPrc, m_zMaxPLPrc, pzCurrPrc, zGapMax, zGapCurr);
		}
	}

	// ���� Short
	if (m_nStatus & 2)
	{

		sprintf(zGapMax, "%.2f", (atof(m_zEntryPrc) - atof(m_zMaxPLPrc))*0.5);
		sprintf(zGapCurr, "%.2f", atof(m_zEntryPrc) - atof(pzCurrPrc));
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, 2, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "SHORT��������.(���԰�:%s, �ְ�:%s, ���簡:%s) (�ְ�GAP/2:%s, ���簡Gap:%s)",
				m_zEntryPrc, m_zMaxPLPrc, pzCurrPrc, zGapMax, zGapCurr);
		}
	}

	return bResult;
}


// ���� �� �ְ� ���Ͱ����� �����ϱ� ����
void CStratHistManager::SetMaxPLPrc(char* pzCurrPrc)
{
	int nComp = 0;
	if (m_nStatus & 1) 
	{
		if (m_zMaxPLPrc[0] == 0x00)
		{
			strcpy(m_zMaxPLPrc, pzCurrPrc);
		}
		else
		{
			// BUY POS. MAXPL -> HIGHER
			nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_zMaxPLPrc, LEN_PRC, m_nDotLen, LEN_PRC);
			if (nComp > 0)
			{
				strcpy(m_zMaxPLPrc, pzCurrPrc);

				int nComp = CUtil::CompPrc(atof(pzCurrPrc), atof(m_zOpenPrc)*(1.0 + DEF_CLR_SPREAD)
										, m_nDotLen, LEN_PRC);
				if (nComp >= 0)
					m_bHitPTPrc = TRUE;
			}
		}
	}
	else if (m_nStatus & 2)
	{
		if (m_zMaxPLPrc[0] == 0x00)
			strcpy(m_zMaxPLPrc, pzCurrPrc);
		else
		{
			// SELL POS. MAXPL -> LOWER
			nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_zMaxPLPrc, LEN_PRC, m_nDotLen, LEN_PRC);
			if (nComp < 0)
			{
				strcpy(m_zMaxPLPrc, pzCurrPrc);

				int nComp = CUtil::CompPrc(atof(pzCurrPrc), atof(m_zOpenPrc)*(1 - DEF_CLR_SPREAD)
					, m_nDotLen, LEN_PRC);
				if (nComp <= 0)
					m_bHitPTPrc = TRUE;
			}
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
void CStratHistManager::SetStrategyExist(std::string sStratID, char* pzCurrPrc, char* pzOpenPrc)
{
	bool bClr = false;
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
		bClr = true;
	}
	if (sStratID.compare(STRATID_SELL_PT) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~1;
		bClr = true;
		m_bProfitRealized = TRUE;
	}
	if (sStratID.compare(STRATID_BUY_SL) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~2;
		m_nTotSLCnt++;
		bClr = true;
	}
	if (sStratID.compare(STRATID_BUY_PT) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~2;
		bClr = true;
		m_bProfitRealized = TRUE;
	}
	if (sStratID.compare(STRATID_MARKET_CLR) == 0)
	{
		// off STRATID_BUY_OPEN
		m_nStatus &= ~1;
		m_nStatus &= ~2;
		bClr = true;
	}

	if (bClr)
	{
		ZeroMemory(m_zEntryPrc, sizeof(m_zEntryPrc));
		ZeroMemory(m_zMaxPLPrc, sizeof(m_zMaxPLPrc));
		m_bHitPTPrc = FALSE;
	}
	else
	{
		strcpy(m_zEntryPrc, pzCurrPrc);
		strcpy(m_zMaxPLPrc, pzCurrPrc);
		strcpy(m_zOpenPrc, pzOpenPrc);
	}
}
