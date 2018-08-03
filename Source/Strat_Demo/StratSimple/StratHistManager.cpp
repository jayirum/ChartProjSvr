#include "StratHistManager.h"
#include "../../IRUM_UTIL/Util.h"


CStratHistManager::CStratHistManager()
{
	InitializeCriticalSectionAndSpinCount(&m_csHist, 2000);
	InitializeCriticalSectionAndSpinCount(&m_csStatus, 2000);
}


CStratHistManager::~CStratHistManager()
{
	itMAP_STRAT_HIST itChart;
	Lock(&m_csHist);
	for (itChart = m_mapStratHist.begin(); itChart != m_mapStratHist.end();++itChart)
	{
		MAP_STRAT_ID mapStrat = (*itChart).second;
		itMAP_STRAT_ID itStrat;
		for (itStrat = mapStrat.begin(); itStrat != mapStrat.end();++itStrat)
		{
			std::list<ST_STRAT_SAVE*> lst = (*itStrat).second;
			std::list<ST_STRAT_SAVE*>::iterator itList;
			for (itList = lst.begin(); itList != lst.end();++itList)
			{
				ST_STRAT_SAVE* p = *itList;
				delete p;
			}
			lst.clear();
		}
		mapStrat.clear();
	}
	m_mapStratHist.clear();
	Unlock(&m_csHist);

	DeleteCriticalSection(&m_csHist);
	DeleteCriticalSection(&m_csStatus);
}

/*
typedef std::map<std::string, std::list<STRAT_INFO> >	MAP_STRAT_ID;	//	STRAT_ID
typedef std::map<std::string, MAP_STRAT_ID>				MAP_STRAT_HIST;	//	CHART_NM
*/
void CStratHistManager::SaveHist(ST_STRAT_SAVE* p)
{
	ST_STRAT_SAVE*	st = new ST_STRAT_SAVE;
	CopyMemory(st, p, sizeof(ST_STRAT_SAVE));

	std::string sStratID = st->zStratID;
	std::string sChartNm = st->zChartNm;


	Lock(&m_csHist);

	// find Chart Name
	itMAP_STRAT_HIST itChart = m_mapStratHist.find(sChartNm);

	std::list<ST_STRAT_SAVE*> ListInfo;

	if (itChart != m_mapStratHist.end())
	{
		MAP_STRAT_ID mapID = (*itChart).second;

		// find STRAT_ID
		itMAP_STRAT_ID itID = mapID.find(sStratID);
		
		if (itID != mapID.end())
		{
			ListInfo = (*itID).second;
		}
		

		// PUSH to List
		ListInfo.push_back(st);

		// add to MAP_STRATID
		mapID[sStratID] = ListInfo;

		// add to MAP_CHART_NM
		m_mapStratHist[sChartNm] = mapID;
	}
	else
	{
		// PUSH to List
		ListInfo.push_back(st);

		// add to MAP_STRATID
		MAP_STRAT_ID mapID;
		mapID[sStratID] = ListInfo;

		// add to MAP_CHART_NM
		m_mapStratHist[sChartNm] = mapID;
	}

	Unlock(&m_csHist);


	//bitwise operation
	SetStrategyExist(sChartNm, sStratID);
}



/*
	
*/
BOOL CStratHistManager::IsSrategyExist(std::string sChartNm, std::string sStratID)
{
	BOOL bExist = FALSE;
	Lock(&m_csStatus);

	if (m_mapStatus.size() >= 0)
	{
		itMAP_STATUS it = m_mapStatus.find(sChartNm);
		if (it != m_mapStatus.end())
		{
			unsigned char flag = (*it).second;

			if (sStratID.compare(STRATID_BUY_OPEN) == 0)
			{
				if (flag & 1)
					bExist = TRUE;
			}
			if (sStratID.compare(STRATID_SELL_OPEN) == 0)
			{
				if (flag & 2)
					bExist = TRUE;
			}
			if (sStratID.compare(STRATID_SELL_CLOSE) == 0)
			{
				if (flag & 4)
					bExist = TRUE;
			}
			if (sStratID.compare(STRATID_BUY_CLOSE) == 0)
			{
				if (flag & 8)
					bExist = TRUE;
			}
		}
	}
	Unlock(&m_csStatus);

	return bExist;
}




/*
	현재 아무 전략이 없으면, 
		OPEN 점검
	현재 BUY OPEN있으면 
		SELL CLOSE 점검
	현재 SELL OPEN있으면 
		BUY CLOSE 점검
		
*/
char*	CStratHistManager::WhichStratShouldbeChecked(std::string sChartNm, char* pOut)
{
	Lock(&m_csStatus);
	if (m_mapStatus.size() == 0) 
	{
		strcpy(pOut, STRAT_OPEN);
	}
	else
	{
		itMAP_STATUS it = m_mapStatus.find(sChartNm);
		if (it == m_mapStatus.end())
		{
			strcpy(pOut, STRAT_OPEN);
		}
		else
		{
			unsigned char flag = (*it).second;

			if (flag == 0) {
				strcpy(pOut, STRAT_OPEN);
			}
			if (flag & 1){
				strcpy(pOut, STRATID_SELL_CLOSE);
			}
			if (flag & 2) {
				strcpy(pOut, STRATID_BUY_CLOSE);
			}	
		}
	}
	Unlock(&m_csStatus);

	return pOut;
}




BOOL CStratHistManager::IsOpenSrategyExist(std::string sChartNm)
{
	BOOL bExist = FALSE;
	Lock(&m_csStatus);

	if (m_mapStatus.size() >= 0)
	{
		itMAP_STATUS it = m_mapStatus.find(sChartNm);
		if (it != m_mapStatus.end())
		{
			unsigned char flag = (*it).second;

			if (flag & 1)
				bExist = TRUE;
			if (flag & 2)
				bExist = TRUE;
			
		}
	}
	Unlock(&m_csStatus);

	return bExist;
}


/*
	if close strategy happens, off the corresponding open strategy
*/
void CStratHistManager::SetStrategyExist(std::string sChartNm, std::string sStratID)
{
	unsigned char flag;

	Lock(&m_csStatus);

	itMAP_STATUS it = m_mapStatus.find(sChartNm);
	if (it != m_mapStatus.end())
		flag = (*it).second;
	else
		flag = 0;

	if (sStratID.compare(STRATID_BUY_OPEN) == 0)
	{
		flag ^= 1;
	}
	if (sStratID.compare(STRATID_SELL_OPEN) == 0)
	{
		flag ^= 2;
	}
	if (sStratID.compare(STRATID_SELL_CLOSE) == 0)
	{
		flag ^= 4;

		// off STRATID_BUY_OPEN
		flag &= ~1;
	}
	if (sStratID.compare(STRATID_BUY_CLOSE) == 0)
	{
		flag ^= 8;

		// off STRATID_BUY_OPEN
		flag &= ~2;
	}

	m_mapStatus[sChartNm] = flag;
	Unlock(&m_csStatus);
}

void CStratHistManager::SetStrategyNonExist(std::string sChartNm, std::string sStratID)
{

}


//int CStratHistManager::StratExistCntSameCandle(char* pzChartNm, char* pzStratID)
//{
//	int nCnt = 0;
//	std::string sStratID = pzStratID;
//	std::string sChartNm = pzChartNm;
//
//	Lock(&m_csHist);
//
//	// find Chart Name
//	itMAP_STRAT_HIST itChart = m_mapStratHist.find(sChartNm);
//	if (itChart != m_mapStratHist.end())
//	{
//		MAP_STRAT_ID mapID = (*itChart).second;
//
//		// find STRAT_ID
//		itMAP_STRAT_ID itID = mapID.find(sStratID);
//
//		if (itID != mapID.end())
//		{
//			std::list<STRAT_INFO*>* pListInfo = (*itID).second;
//			nCnt = pListInfo->size();
//		}
//	}
//	Unlock(&m_csHist);
//
//	return nCnt;
//}
