#pragma once

/*
	bitwise operation

	0000 0000	// there is no strategy
	0000 0001	// strat01 exist	STRATID_BUY_OPEN
	0000 0010	// strat02 exist	STRATID_SELL_OPEN
	0000 0100	// strat03 exist	STRATID_SELL_CLOSE
	0000 1000	// strat04 exist	STRATID_BUY_CLOSE
*/
#include "../../IRUM_INC/IRUM_Common.h"
#include "StratID.h"
#include <windows.h>
#include <map>
#include <string>
#include <list>

//typedef struct _STRAT_INFO
//{	
//	//char	zTradeDT[8];
//	char	zSysDT[8];
//	char	zStratPrc[LEN_PRC];
//	char	zStraTM[12];
//}STRAT_INFO;

typedef std::map<std::string, unsigned char>	MAP_STATUS;		// CHART_NM, BIT FLAG

typedef std::map<std::string, std::list<ST_STRAT_SAVE*> >	MAP_STRAT_ID;	//	STRAT_ID
typedef std::map<std::string, MAP_STRAT_ID>				MAP_STRAT_HIST;	//	CHART_NM

typedef std::map<std::string, unsigned char>::iterator	itMAP_STATUS;
typedef std::map<std::string, std::list<ST_STRAT_SAVE*> >::iterator	itMAP_STRAT_ID;	//	STRAT_ID
typedef std::map<std::string, MAP_STRAT_ID>::iterator			itMAP_STRAT_HIST;	//	CHART_NM

class CStratHistManager
{
public:
	CStratHistManager();
	~CStratHistManager();

	void	SaveHist(ST_STRAT_SAVE* p);
	//int		StratExistCntSameCandle(char* pzChartNm, char* pzStratID);
	void	Lock(CRITICAL_SECTION* cs) { EnterCriticalSection(cs); }
	void	Unlock(CRITICAL_SECTION* cs) { LeaveCriticalSection(cs); }

	char*	WhichStratShouldbeChecked(std::string sChartNm, char* pOut);
	BOOL	IsSrategyExist(std::string sChartNm, std::string sStratID);
	BOOL	IsOpenSrategyExist(std::string sChartNm);
	void	SetStrategyExist(std::string sChartNm, std::string sStratID);
	void	SetStrategyNonExist(std::string sChartNm, std::string sStratID);
private:
	MAP_STRAT_HIST	m_mapStratHist;
	MAP_STATUS		m_mapStatus;

	CRITICAL_SECTION	m_csHist;
	CRITICAL_SECTION	m_csStatus;
};

