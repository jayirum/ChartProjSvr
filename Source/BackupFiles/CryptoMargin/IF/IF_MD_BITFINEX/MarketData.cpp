#include "MarketData.h"
#include <time.h> 
#include <curses.h>

bookGroup::bookGroup()
{
	m_dTotAmtBid = m_dTotAmtAsk = 0;
	m_nTotCntBid = m_nTotCntAsk = 0;

	InitializeCriticalSection(&m_csBid);
	InitializeCriticalSection(&m_csAsk);
}

bookGroup::~bookGroup()
{
	DeleteCriticalSection(&m_csBid);
	DeleteCriticalSection(&m_csAsk);
}

/*
PRICE	float	Price level
RATE	float	Rate level
PERIOD	float	Period level
COUNT	int	Number of orders at that price level
¡¾AMOUNT	float	Total amount available at that price level. 
				Trading: if AMOUNT > 0 then bid else ask; Funding: if AMOUNT < 0 then bid else ask;
*/
int bookGroup::AddOrUpdateBook(double dPrice, int nCount, double	dAmount)
{
	//Algorithm from https://docs.bitfinex.com/v2/reference#ws-public-order-books

	/*
	when count > 0 then you have to add or update the price level
		if amount > 0 then add/update bids
		if amount < 0 then add/update asks
	*/
	if (nCount > 0)
	{
		bookData *p = new bookData;
		p->m_dPrice = dPrice; p->m_dAmount = dAmount; p->m_nCount = nCount;

		if (dAmount > 0)
		{
			Lock(&m_csBid);
			delete m_bookBid[dPrice];
			
			m_bookBid[dPrice] = p;
			
			m_dTotAmtBid += dAmount;
			m_nTotCntBid += nCount;

			Unlock(&m_csBid);
		}
		else if (dAmount < 0)
		{
			Lock(&m_csAsk);
			delete m_bookAsk[dPrice];

			m_bookAsk[dPrice] = p;

			m_dTotAmtAsk += dAmount;
			m_nTotCntAsk += nCount;

			Unlock(&m_csAsk);
		}
	}

	/*
	when count = 0 then you have to delete the price level.
		if amount = 1 then remove from bids
		if amount = -1 then remove from asks
	*/
	else if (nCount == 0)
	{
		if (dAmount == 1)
		{
			Lock(&m_csBid);

			bookData *p = m_bookBid[dPrice];

			m_dTotAmtBid -= p->m_dAmount;
			m_nTotCntBid -= p->m_nCount;
			delete p;

			m_bookBid.erase(dPrice);
			Unlock(&m_csBid);
		}
		else if (dAmount == -1)
		{
			Lock(&m_csAsk);

			bookData *p = m_bookAsk[dPrice];

			m_dTotAmtAsk -= p->m_dAmount;
			m_nTotCntAsk -= p->m_nCount;
			delete p;

			m_bookAsk.erase(dPrice);
			Unlock(&m_csAsk);
		}
	}

	time(&m_timeLastReceiveTime);

	//todo : if size more than max order rows

	return DATA_PROCESSED_SUCCESSFULLY;
}

/*
int nDepth : book depth to be wanted. 1,2,3,4,5
return value : depth of returned.

*/
INT bookGroup::GetBidBooks(_In_ int nDepth, 
							_Out_ double *pPrcArray, _Out_ double *pAmtArray, _Out_ int *pCntArray,
							_Out_ double *pTotAmt, _Out_ int *pTotCnt)
{
	int nReturned = 0;

	map<double, bookData*, greater<double> >::iterator it;
	for (it = m_bookBid.begin(); it != m_bookBid.end(); it++)
	{
		pPrcArray[nReturned] = (*it).first;
		pAmtArray[nReturned] = ((*it).second)->m_dAmount;
		pCntArray[nReturned] = ((*it).second)->m_nCount;

		if(++nReturned>=nDepth)
			break;
	}

	*pTotAmt = m_dTotAmtBid;
	*pTotCnt = m_nTotCntBid;
	return nReturned;
}


/*
int nDepth : book depth to be wanted. 1,2,3,4,5
return value : depth of returned.

*/
INT bookGroup::GetAskBooks(_In_ int nDepth, _Out_ double *pPrcArray, _Out_ double *pAmtArray, _Out_ int *pCntArray, _Out_ double *pTotAmt, _Out_ int *pTotCnt)
{
	int nReturned = 0;

	map<double, bookData*, less<double> >::iterator it;
	for (it = m_bookAsk.begin(); it != m_bookAsk.end(); it++)
	{
		pPrcArray[nReturned] = (*it).first;
		pAmtArray[nReturned] = ((*it).second)->m_dAmount;
		pCntArray[nReturned] = ((*it).second)->m_nCount;

		if (++nReturned >= nDepth)
			break;
	}
	*pTotAmt = m_dTotAmtAsk;
	*pTotCnt = m_nTotCntAsk;

	return nReturned;
}



time_t bookGroup::GetLastReceiveTime()
{
	return m_timeLastReceiveTime;
}


void bookGroup::DeleteBooks()
{
	Lock(&m_csBid);
	for (auto aPrices : m_bookBid)
	{
		delete m_bookBid[aPrices.first];
	}
	m_bookBid.clear();
	Unlock(&m_csBid);

	Lock(&m_csAsk);
	for (auto aPrices : m_bookAsk)
	{
		delete m_bookAsk[aPrices.first];
	}
	m_bookAsk.clear();
	Lock(&m_csAsk);
}
//
//void bookGroup::ShowBookBidData()
//{
//	Lock(&m_csBid);
//	for (map<double, bookData*>::iterator bookIterator = m_bookBid.begin(); bookIterator != m_bookBid.end(); bookIterator++)
//	{
//		//todo:show mts in printf()
//		char szDateTime[15] = "\0";
//		Int64ToDate(bookIterator->first, szDateTime);
//		printf("%s,%s\n", szDateTime, m_bookBid[bookIterator->first]->BookDataToCharString());
//	}
//	Unlock(&m_csBid);
//}
//
//int bookGroup::DisplayAskBookAtCoords(int x, int y, int nMaxRows)
//{
//	int nCounter = 0;
//	char szStringToDisplay[1024] = "\0";
//	mvprintw(y, x, "Ask Order Book");
//	Lock(&m_csAsk);
//	for (map<double, bookData*>::iterator bookIterator = m_bookAsk.begin(); bookIterator != m_bookAsk.end(); bookIterator++)
//	{
//		nCounter++;
//		//todo:show mts in printf()
//		sprintf(szStringToDisplay, "%s\n", m_bookAsk[bookIterator->first]->BookDataToCharString());
//		mvprintw(y + nCounter, x, szStringToDisplay);
//		if (nCounter == nMaxRows)
//			break;
//	}
//	Unlock(&m_csAsk);
//	return DATA_PROCESSED_SUCCESSFULLY;
//}
//
//int bookGroup::DisplayBidBookAtCoords(int x, int y, int nMaxRows)
//{
//	int nCounter = 0;
//	char szStringToDisplay[1024] = "\0";
//	mvprintw(y, x, "Bid Order Book");
//	Lock(&m_csBid);
//	for (map<double, bookData*>::iterator bookIterator = m_bookBid.begin(); bookIterator != m_bookBid.end(); bookIterator++)
//	{
//		nCounter++;
//		//todo:show mts in printf()
//		sprintf(szStringToDisplay, "%s", m_bookBid[bookIterator->first]->BookDataToCharString());
//		mvprintw(y + nCounter, x, szStringToDisplay);
//		if (nCounter == nMaxRows)
//			break;
//	}
//	Unlock(&m_csBid);
//	return DATA_PROCESSED_SUCCESSFULLY;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tickerData::tickerData()
{
}
tickerData::tickerData(
	double dBid,
	//int		nBidPeriod, // only for currencies
	double dBidSize,
	double dAsk,
	//int		nAskPeriod, // only for currencies
	double dAskSize,
	double dDailyChange,
	double dDailyChangePercentage,
	double dLastPrice,
	double dDailyVolume,
	double dDailyHigh,
	double dDailyLow)
{
	m_dBid = dBid;
	m_dBidSize = dBidSize;
	m_dAsk = dAsk;
	m_dAskSize = dAskSize;
	m_dDailyChange = dDailyChange;
	m_dDailyChangePercentage = dDailyChangePercentage;
	m_dLastPrice = dLastPrice;
	m_dDailyVolume = dDailyVolume;
	m_dDailyHigh = dDailyHigh;
	m_dDailyLow = dDailyLow;
}




int tickerData::DisplayTickerAtCoords(int x, int y)
{
	time_t timeRawTime;
	char szStringToDisplay[1024] = "\0";
	char szDateTime[15] = "\0";
	time(&timeRawTime);
	Time_tToString(timeRawTime, szDateTime);
	sprintf(szStringToDisplay, "Ticker Last Update:%s => %.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f",
		szDateTime,
		m_dBid,
		m_dBidSize,
		m_dAsk,
		m_dAskSize,
		m_dDailyChange,
		m_dDailyChangePercentage,
		m_dLastPrice,
		m_dDailyVolume,
		m_dDailyHigh,
		m_dDailyLow);
	mvprintw(y, x, szStringToDisplay);
	return DATA_PROCESSED_SUCCESSFULLY;
}

int tickerData::SetTickerData(
	double dBid,
	double dBidSize,
	double dAsk,
	double dAskSize,
	double dDailyChange,
	double dDailyChangePercentage,
	double dLastPrice,
	double dDailyVolume,
	double dDailyHigh,
	double dDailyLow)
{
	m_dBid = dBid;
	m_dBidSize = dBidSize;
	m_dAsk = dAsk;
	m_dAskSize = dAskSize;
	m_dDailyChange = dDailyChange;
	m_dDailyChangePercentage = dDailyChangePercentage;
	m_dLastPrice = dLastPrice;
	m_dDailyVolume = dDailyVolume;
	m_dDailyHigh = dDailyHigh;
	m_dDailyLow = dDailyLow;
	return DATA_PROCESSED_SUCCESSFULLY;
}