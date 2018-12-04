#pragma once
#include "StratFactorManager.h"

class CStrat_OpenPrcMatched
{
public:
	CStrat_OpenPrcMatched(CStratFactorManager* f);
	~CStrat_OpenPrcMatched();

	BOOL Initialize();

	VOID MainProc();
	BOOL IsAlreadyApplied(char* pzChartNm);
private:
	CStratFactorManager*	m_f;

private:
	char	m_zStratApppliedTime[32];;
};

