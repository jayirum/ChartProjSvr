#include "ChartUtils.h"
#include "Util.h"

CMA::CMA(int nShortCnt, int nLongCnt, BOOL bAsc)
{
	m_shortCnt = nShortCnt;
	m_longCnt = nLongCnt;
	m_arr = new double[m_longCnt];
	m_bAsc = bAsc;
	m_idx = 0;
	
}

CMA::~CMA()
{
	delete [] m_arr;
}

void	CMA::setClose(double dClose)
{
	if (m_idx >= m_longCnt)
		return;

	m_arr[m_idx++] = dClose;
}


// asc 이면 처음부터
BOOL CMA::getMA(_Out_ double* pdShortMA, _Out_ double *pdLongMA)
{
	if (m_idx < m_longCnt - 1)
		return FALSE;

	double sum = 0;
	if (m_bAsc)
	{
		for (int i = 0; i < m_longCnt; i++)
		{
			sum += m_arr[i];
			if (i == m_shortCnt - 1)
			{
				*pdShortMA = sum / (double)m_shortCnt;
			}

		}
		*pdLongMA = sum / (double)m_longCnt;
	}

	if (!m_bAsc)
	{
		int nLoop = 0;
		for (int i = m_longCnt-1; i >=0; i--, nLoop++)
		{
			sum += m_arr[i];
			if (nLoop == m_shortCnt - 1)
			{
				*pdShortMA = sum / (double)m_shortCnt;
			}
		}
		*pdLongMA = sum / (double)m_longCnt;
	}

	return TRUE;
}