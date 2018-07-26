#pragma once


#include <stdio.h>
#include "../IRUM_INC/IRUM_Common.h"
#include <string>
#include <Windows.h>


class CManagePosition
{
public:
	CManagePosition();
	virtual ~CManagePosition();

	bool	IsPosOpened() { return (m_nOpenQty > 0); }
	void	Reset();
	bool	IsSameChartWithOpenChart(const char* psChartNm);
	bool	IsSameChartWithClrChart(const char* psChartNm);

	void	OpenPosInfo(std::string sSymbol, int nOpenQty, char cSide, char* pzPosPrc, char* pzOpenTM, char* pzOpenChartNm, int nCntrNo);
	void	ClrPosInfo(std::string sSymbol, char* pzClrChartNm);

private:
	std::string m_sSymbol;
	int		m_nOpenQty;
	char	m_cSide;
	char	m_zPosPrc[LEN_PRC + 1];
	char	m_zOpenTM[9];	//hh:mm:ss
	char	m_zOpenChartNm[LEN_CHART_NM + 1];
	char	m_zClrChartNm[LEN_CHART_NM + 1];
	int		m_nCntrNo;
	CRITICAL_SECTION	m_cs;
};