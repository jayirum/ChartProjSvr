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
	int				m_nTotSLCnt;	//���� ���� sl Ƚ��
	int				m_nMaxSLCnt;	//���� ���� sl ����Ƚ��
	double			m_dMaxPLPrc;	//���� �� �ְ� ���Ͱ���
	double			m_dEntryPrc;	//���԰���
};

