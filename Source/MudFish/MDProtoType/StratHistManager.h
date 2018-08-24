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
	CStratHistManager(char* pzMaxSLCnt, int nDotLen);
	~CStratHistManager();

	void	SetStrategyExist( std::string sStratID, char* pzCurrPrc, char* pzOpenPrc);
	char	IsOpenSrategyExist();
	BOOL	IsAlreadySLMaxCnt() { return (m_nTotSLCnt >= m_nMaxSLCnt); }
	BOOL	IsProfitRealized() {return m_bProfitRealized;}
	BOOL	IsPTCondition(char* pzCurrPrc, _Out_ char* pMsg );
	void	SetMaxPLPrc(char* pzCurrPrc);

	char*	GtMaxPLPrc() { return m_zMaxPLPrc; }
	char*	GetEntryPrc() { return m_zEntryPrc; }
	void	SetOpenPrc(char* pzOpenPrc) { strcpy(m_zOpenPrc, pzOpenPrc); }
	BOOL	IsHitPTPrc() { return m_bHitPTPrc; }
private:
	unsigned char	m_nStatus;
	int				m_nDotLen;
	int				m_nTotSLCnt;	//당일 누적 sl 횟수
	int				m_nMaxSLCnt;	//당일 누적 sl 제한횟수
	BOOL			m_bProfitRealized;
	char			m_zMaxPLPrc[LEN_PRC+1];	//진입 후 최고 수익가격
	char			m_zEntryPrc[LEN_PRC + 1];	//진입가격
	char			m_zOpenPrc[LEN_PRC + 1];
	BOOL			m_bHitPTPrc;	// 0.5% 수익구간을 찍었나?
};

