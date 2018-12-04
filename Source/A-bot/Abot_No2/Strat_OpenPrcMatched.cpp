#include "Strat_OpenPrcMatched.h"



CStrat_OpenPrcMatched::CStrat_OpenPrcMatched(CStratFactorManager* f)
{
	m_f = f;

	ZeroMemory(m_zStratApppliedTime, sizeof(m_zStratApppliedTime));
}


CStrat_OpenPrcMatched::~CStrat_OpenPrcMatched()
{
}


BOOL CStrat_OpenPrcMatched::Initialize()
{
	return TRUE;
}

BOOL CStrat_OpenPrcMatched::IsAlreadyApplied(char* pzChartNm)
{
	return (strcmp(m_zStratApppliedTime, pzChartNm) <= 0);
}

VOID CStrat_OpenPrcMatched::MainProc()
{}