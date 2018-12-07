#pragma once

#include "../IRUM_UTIL/IRUM_Common.h"
#include <Windows.h>

class CMA
{
public:
	CMA(int nShortCnt, int nLongCnt, BOOL bAsc);
	CMA(int nShortestCnt, int nShortCnt, int nLongCnt, BOOL bAsc);
	virtual ~CMA();
	
	void	setClose(double dClose);
	BOOL	getMA(_Out_ double* pdShortMA, _Out_ double *pdLongMA);
	BOOL	getMAEx(_Out_ double* pdShortestMA, _Out_ double* pdShortMA, _Out_ double *pdLongMA);

private:
	int m_shortestCnt, m_shortCnt, m_longCnt;
	int m_idx;
	double *m_arr;
	BOOL m_bAsc;
};