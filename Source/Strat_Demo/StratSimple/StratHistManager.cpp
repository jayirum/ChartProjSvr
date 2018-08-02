#include "StratHistManager.h"
#include "../../IRUM_UTIL/Util.h"


CStratHistManager::CStratHistManager()
{
	InitializeCriticalSectionAndSpinCount(&m_csHist, 2000);
	InitializeCriticalSectionAndSpinCount(&m_csStatus, 2000);
}


CStratHistManager::~CStratHistManager()
{
	DeleteCriticalSection(&m_csHist);
	DeleteCriticalSection(&m_csStatus);
}

/*
typedef std::map<std::string, std::list<STRAT_INFO> >	MAP_STRAT_ID;	//	STRAT_ID
typedef std::map<std::string, MAP_STRAT_ID>				MAP_STRAT_HIST;	//	CHART_NM
*/
void CStratHistManager::SaveHist(char* pzChartNm, char* pzStratID, char* pzTradeDT, char* pzStratPrc)
{
	STRAT_INFO*	st = new STRAT_INFO;;
	memcpy(st->zTradeDT, pzTradeDT, sizeof(st->zTradeDT));
	memcpy(st->zStratPrc, pzStratPrc, sizeof(st->zStratPrc));

	char zSysDT[32] = { 0, };
	CUtil::GetToday(zSysDT);
	memcpy(st->zSysDT, zSysDT, sizeof(st->zSysDT));

	char zTM[32] = { 0, };
	strcpy(zTM, CUtil::Get_NowTime(FALSE));
	memcpy(st->zStraTM, zTM, sizeof(st->zStraTM));

	std::string sStratID = pzStratID;
	std::string sChartNm = pzChartNm;


	Lock(&m_csHist);

	// find Chart Name
	itMAP_STRAT_HIST itChart = m_mapStratHist.find(sChartNm);

	std::list<STRAT_INFO*>* pListInfo;

	if (itChart != m_mapStratHist.end())
	{
		MAP_STRAT_ID mapID = (*itChart).second;

		// find STRAT_ID
		itMAP_STRAT_ID itID = mapID.find(sStratID);
		
		if (itID != mapID.end())
		{
			pListInfo = (*itID).second;
		}
		else
		{
			pListInfo = new std::list<STRAT_INFO*>;
		}

		// PUSH to List
		pListInfo->push_back(st);

		// add to MAP_STRATID
		mapID[sStratID] = pListInfo;

		// add to MAP_CHART_NM
		m_mapStratHist[sChartNm] = mapID;
	}
	else
	{
		// PUSH to List
		pListInfo = new std::list<STRAT_INFO*>;
		pListInfo->push_back(st);

		// add to MAP_STRATID
		std::map<std::string, std::list<STRAT_INFO*>* > mapID;
		mapID[sStratID] = pListInfo;

		// add to MAP_CHART_NM
		m_mapStratHist[sChartNm] = mapID;
	}

	Unlock(&m_csHist);

	SetStrategyExist(sChartNm, sStratID);
}


int CStratHistManager::StratExistCntSameCandle(char* pzChartNm, char* pzStratID)
{
	int nCnt = 0;
	std::string sStratID = pzStratID;
	std::string sChartNm = pzChartNm;

	Lock(&m_csHist);

	// find Chart Name
	itMAP_STRAT_HIST itChart = m_mapStratHist.find(sChartNm);
	if (itChart != m_mapStratHist.end())
	{
		MAP_STRAT_ID mapID = (*itChart).second;

		// find STRAT_ID
		itMAP_STRAT_ID itID = mapID.find(sStratID);

		if (itID != mapID.end())
		{
			std::list<STRAT_INFO*>* pListInfo = (*itID).second;
			nCnt = pListInfo->size();
		}
	}
	Unlock(&m_csHist);

	return nCnt;
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



void CStratHistManager::SetStrategyExist(std::string sChartNm, std::string sStratID)
{
	unsigned char flag;

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
	}
	if (sStratID.compare(STRATID_BUY_CLOSE) == 0)
	{
		flag ^= 8;
	}

	Lock(&m_csStatus);
	m_mapStatus[sChartNm] = flag;
	Unlock(&m_csStatus);
}

void CStratHistManager::SetStrategyNonExist(std::string sChartNm, std::string sStratID)
{

}