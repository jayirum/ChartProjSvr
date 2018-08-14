#pragma once

/*
	bitwise operation

	0000 0000	// there is no strategy
	0000 0001	// strat01 exist	STRATID_BUY_OPEN
	0000 0010	// strat02 exist	STRATID_SELL_OPEN
	
*/
#include "../../IRUM_INC/IRUM_Common.h"
#include "StratID.h"
#include <windows.h>
#include <string>
class CStratHistManager
{
public:
	CStratHistManager();
	~CStratHistManager();

	void	SetStrategyExist( std::string sStratID);
	char	IsOpenSrategyExist();

private:
	unsigned char	m_nStatus;
	int				m_nTotSLCnt;
	
};

