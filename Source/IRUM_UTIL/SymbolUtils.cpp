
#pragma warning(disable:4996)
#include "symbolutils.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSymbolUtils::CSymbolUtils()
{
	ZeroMemory(m_zArtcCd, sizeof(m_zArtcCd));
	ZeroMemory(m_zSymbol, sizeof(m_zSymbol));
	ZeroMemory(m_zSymbolName, sizeof(m_zSymbolName));
	m_dTickSize = 0;
	m_dTickValue = 0;
	m_nDotCnt = 0;
}

CSymbolUtils::~CSymbolUtils()
{
}

double CSymbolUtils::calc_tick(double dPrcGap)
{
	double dTick = dPrcGap / m_dTickSize;

	return dTick;
}

double CSymbolUtils::calc_prc_byTick(double dTickGap, double dBasePrc, bool bAdd)
{
	double dPrc = 0;
	if (bAdd)
		dPrc = dBasePrc + (dTickGap * ticksize());
	else
		dPrc = dBasePrc - (dTickGap * ticksize());

	return dPrc;
}