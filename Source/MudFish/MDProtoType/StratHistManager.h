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
	CStratHistManager(char* pzMaxSLCnt);
	~CStratHistManager();

	void	SetStrategyExist( std::string sStratID, char* pzCurrPrc);
	char	IsOpenSrategyExist();
	BOOL	IsAlreadySLMaxCnt() { return (m_nTotSLCnt >= m_nMaxSLCnt); }
	BOOL	IsPTCondition(double dCurrPrc, _Out_ char* pMsg );
	void	SetMaxPLPrc(char* pzCurrPrc);
private:
	unsigned char	m_nStatus;
	int				m_nTotSLCnt;	//당일 누적 sl 횟수
	int				m_nMaxSLCnt;	//당일 누적 sl 제한횟수
	double			m_dMaxPLPrc;	//진입 후 최고 수익가격
	double			m_dEntryPrc;	//진입가격
};

