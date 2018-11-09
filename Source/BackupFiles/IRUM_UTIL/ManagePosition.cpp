#include "ManagePosition.h"
#include "Util.h"

CManagePosition::CManagePosition()
{
	InitializeCriticalSection(&m_cs);
	Reset();
}


CManagePosition::~CManagePosition()
{
	DeleteCriticalSection(&m_cs);
}

void CManagePosition::Reset()
{
	m_sSymbol = "";
	m_nOpenQty = 0;;
	m_cSide = '0';
	ZeroMemory(m_zPosPrc, sizeof(m_zPosPrc));
	ZeroMemory(m_zOpenTM, sizeof(m_zOpenTM));
	ZeroMemory(m_zOpenChartNm, sizeof(m_zOpenChartNm));
	ZeroMemory(m_zClrChartNm, sizeof(m_zClrChartNm));
	m_nCntrNo = 0;
}

bool CManagePosition::IsSameChartWithOpenChart(const char* psChartNm)
{
	if (m_zOpenChartNm[0] == 0x00)	return FALSE;

	int nComp = strncmp(m_zOpenChartNm, psChartNm, LEN_CHART_NM);
	return (nComp >= 0);
}


bool CManagePosition::IsSameChartWithClrChart(const char* psChartNm)
{
	if (m_zClrChartNm[0] == 0x00)	return FALSE;

	int nComp = strncmp(m_zClrChartNm, psChartNm, LEN_CHART_NM);
	return (nComp >= 0);
}

void CManagePosition::OpenPosInfo(std::string sSymbol, int nOpenQty, char cSide, char* pzPosPrc, char* pzOpenTM, char* pzOpenChartNm, int nCntrNo)
{
	LOCK_CS(m_cs);
	m_sSymbol = sSymbol;
	m_nOpenQty = nOpenQty;
	m_cSide = cSide;
	strcpy(m_zPosPrc, pzPosPrc);
	sprintf(m_zOpenTM, "%.*s", sizeof(m_zOpenTM) - 1, pzOpenTM);
	strcpy(m_zOpenChartNm, pzOpenChartNm);
	m_nCntrNo = nCntrNo;
	UNLOCK_CS(m_cs);
}

void CManagePosition::ClrPosInfo(std::string sSymbol, char* pzClrChartNm)
{
	if (m_sSymbol.compare(sSymbol) != 0)
		return;

	LOCK_CS(m_cs);
	Reset();

	m_sSymbol = sSymbol;
	strcpy(m_zClrChartNm, pzClrChartNm);
	UNLOCK_CS(m_cs);
}
