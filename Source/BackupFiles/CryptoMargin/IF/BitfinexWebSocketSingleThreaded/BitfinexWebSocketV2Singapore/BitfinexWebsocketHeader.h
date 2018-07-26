#include <string>
#include <ctime>
#include <time.h> 
#include <curses.h>			/* ncurses.h includes stdio.h */  
#include <string> 
#include <map>
using namespace std;


const int DATA_NOT_PROCESSED = -1;
const int DATA_PROCESSED_SUCCESSFULLY = 0;
const int DATA_PROCESSED_UNSUCCESSFULLY = 1;
const int DATA_USER_EXIT = 2;
//const int DATA_MAX_CRYPTOPAIR = 5;
const int DATA_NO_CHANNELID_YET = 0;
const int DATA_MAX_CANDLES = 241; //max candles for memory storage
const int DATA_MAX_MESSAGES = 10; //max messages for memory storage
const int DATA_MAX_TIME_TO_LIVE = 130; // unit in seconds , x1000 for milliseconds

const int DATA_WEBSOCKET_EVENT_RESTARTWEBSOCKETCONNECTION = 20051;
const int DATA_WEBSOCKET_EVENT_WEBSOCKETMAINTENANCESTART = 20060;
const int DATA_WEBSOCKET_EVENT_WEBSOCKETMAINTENANCEEND = 20061;

void Int64ToDate(__int64 n64Milliseconds, char szStringOut[]);
void Time_tToString(time_t timeRawTime, char szStringOut[]);

class stringsForDisplay
{
private:
	map<int, string> m_mapStrings;
public:
	stringsForDisplay::stringsForDisplay()
	{

	}

	map<int, string> StringsOutForDisplay()
	{
		return m_mapStrings;
	}

	int AddString(string szString)
	{
		m_mapStrings[m_mapStrings.size() + 1] = szString;
		return 0;
	}

	int DeleteAllStrings()
	{
		for (map<int, string>::iterator iterator = m_mapStrings.begin(); iterator != m_mapStrings.end(); iterator++)
		{
			m_mapStrings.erase(iterator->first);
		}
		return 0;
	}

	int DisplayEmptyStringsAtCoords(int x, int y, int nMaxRows)
	{
		int nCounter = 0;
		for (map<int, string>::iterator iterator = m_mapStrings.begin(); iterator != m_mapStrings.end(); iterator++)
		{
			nCounter++;
			if (nCounter == nMaxRows)
				return 0;
			mvprintw(y + iterator->first - 1, x, "%s", string(iterator->second.length(), ' ').c_str());
		}
		return 0;
	}

	int DisplayStringsAtCoords(int x, int y, int nMaxRows)
	{
		int nCounter = 0;
		for (map<int, string>::iterator iterator = m_mapStrings.begin(); iterator != m_mapStrings.end(); iterator++)
		{
			nCounter++;
			if (nCounter == nMaxRows)
				return 0;
			mvprintw(y + iterator->first - 1, x, m_mapStrings[iterator->first].c_str());
		}
		return 0;
	}
};


class tickerData
{
private:
	string szChannelString;
	int nChannelID;
	//double dFlashRateReturn; // only for currencies
	double dBid;
	//int		nBidPeriod;  // only for currencies
	double dBidSize;
	double dAsk;
	//int		nAskPeriod; // only for currencies
	double dAskSize;
	double dDailyChange;
	double dDailyChangePercentage;
	double dLastPrice;
	double dDailyVolume;
	double dDailyHigh;
	double dDailyLow;
public:
	tickerData()
	{
	}
	tickerData(	//double dFlashRateReturn, // only for currencies
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
		//this->dFlashRateReturn = dFlashRateReturn; // only for currencies
		this->dBid = dBid;
		//this->nBidPeriod = nBidPeriod; // only for currencies
		this->dBidSize = dBidSize;
		this->dAsk = dAsk;
		//this->nAskPeriod = nAskPeriod; // only for currencies
		this->dAskSize = dAskSize;
		this->dDailyChange = dDailyChange;
		this->dDailyChangePercentage = dDailyChangePercentage;
		this->dLastPrice = dLastPrice;
		this->dDailyVolume = dDailyVolume;
		this->dDailyHigh = dDailyHigh;
		this->dDailyLow = dDailyLow;
	}

	int SetTickerData(//double dFlashRateReturn, // only for currencies
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
		//this->dFlashRateReturn = dFlashRateReturn; // only for currencies
		this->dBid = dBid;
		//this->nBidPeriod = nBidPeriod; // only for currencies
		this->dBidSize = dBidSize;
		this->dAsk = dAsk;
		//this->nAskPeriod = nAskPeriod; // only for currencies
		this->dAskSize = dAskSize;
		this->dDailyChange = dDailyChange;
		this->dDailyChangePercentage = dDailyChangePercentage;
		this->dLastPrice = dLastPrice;
		this->dDailyVolume = dDailyVolume;
		this->dDailyHigh = dDailyHigh;
		this->dDailyLow = dDailyLow;
		return DATA_PROCESSED_SUCCESSFULLY;
	}
	int SetChannelString(string szChannelString)
	{
		this->szChannelString = szChannelString;
		return DATA_PROCESSED_SUCCESSFULLY;
	}
	string GetChannelString()
	{
		return szChannelString;
	}
	void SetChannelID(int nNewChannelID)
	{
		nChannelID = nNewChannelID;
	}
	__int64 GetChannelID()
	{
		return nChannelID;
	}

	int DisplayTickerAtCoords(int x, int y)
	{
		time_t timeRawTime;
		char szStringToDisplay[1024] = "\0";
		char szDateTime[15] = "\0";
		time(&timeRawTime);
		Time_tToString(timeRawTime, szDateTime);
		sprintf(szStringToDisplay, "Ticker Last Update:%s => %.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f",
			szDateTime,
			dBid,
			dBidSize,
			dAsk,
			dAskSize,
			dDailyChange,
			dDailyChangePercentage,
			dLastPrice,
			dDailyVolume,
			dDailyHigh,
			dDailyLow);
		mvprintw(y, x, szStringToDisplay);
		return DATA_PROCESSED_SUCCESSFULLY;
	}

};

class bookData
{
private:
	//	double	m_dRate; //only for currencies
	//	double	m_dPeriod; //only for currencies
	double	m_dPrice; //only for cryptocurrencies
	int		m_nCount;
	double	m_dAmount;
public:
	bookData()
	{
	}
	bookData::bookData(//double	dRate, //only for currencies
					   //double	dPeriod, //only for currencies
		double	dPrice, //only for cryptocurrencies
		int		nCount,
		double	dAmount)
	{
		//		m_dRate = dRate; //only for currencies
		//		m_dPeriod = dPeriod;//only for currencies
		m_dPrice = dPrice; //only for cryptocurrencies
		m_nCount = nCount;
		m_dAmount = dAmount;
	}

	char *BookDataToCharString()
	{
		char szData[1024];
		//sprintf(szData, "%.5f,%.5f,%d,%.5f", m_dRate, m_dPeriod, m_nCount, m_dAmount); //todo: precision depends on channel requested -> have to be manageable // rate only for currencies
		sprintf(szData, "%.5f,%d,%.5f", m_dPrice, m_nCount, m_dAmount); //todo: precision depends on channel requested -> have to be manageable

		return szData;
	}
};

class bookGroup
{
public:
	string		m_sChannelString;
	__int64		m_nChannelID;
	time_t		m_timeLastReceiveTime;
	map<double, bookData*>	m_bookBid;
	map<double, bookData*>	m_bookAsk;

	bookGroup()
	{
	}

	int AddOrUpdateBook(double dPrice,//only for cryptocurrencies
									  //double	dRate,//only for currencies
									  //double	dPeriod,//only for currencies
		int		nCount,
		double	dAmount)
	{
		//Algorithm from https://docs.bitfinex.com/v2/reference#ws-public-order-books

		if (nCount > 0)
		{
			if (dAmount > 0)
			{
				delete m_bookBid[dPrice];
				m_bookBid.erase(dPrice);
				m_bookBid[dPrice] = new bookData(dPrice, nCount, dAmount);//only for cryptocurrencies
																		  //m_bookBid[dPrice] = new bookData(dRate, dPeriod, nCount, dAmount);//only for currencies
			}
			else if (dAmount < 0)
			{
				delete m_bookAsk[dPrice];
				m_bookAsk.erase(dPrice);
				m_bookAsk[dPrice] = new bookData(dPrice, nCount, dAmount);
				//m_bookAsk[dPrice] = new bookData(dRate, dPeriod, nCount, dAmount); //only for currencies
			}
		}
		else if (nCount == 0)
		{
			if (dAmount == 1)
			{
				delete m_bookBid[dPrice];
				m_bookBid.erase(dPrice);
			}
			else if (dAmount == -1)
			{
				delete m_bookAsk[dPrice];
				m_bookAsk.erase(dPrice);
			}
		}

		std::time(&m_timeLastReceiveTime);

		//todo : if size more than max order rows

		return DATA_PROCESSED_SUCCESSFULLY;
	}

	time_t GetLastReceiveTime()
	{
		return m_timeLastReceiveTime;
	}

	void SetChannelID(__int64 nNewChannelID)
	{
		m_nChannelID = nNewChannelID;
	}

	__int64 GetChannelID()
	{
		return m_nChannelID;
	}

	string GetChannelString()
	{
		return m_sChannelString;
	}

	void SetChannelString(string sChannelString)
	{
		m_sChannelString = sChannelString;
	}

	void DeleteBooks()
	{
		for (auto aPrices : m_bookBid)
		{
			delete m_bookBid[aPrices.first];
			m_bookBid.erase(aPrices.first);
		}

		for (auto aPrices : m_bookAsk)
		{
			delete m_bookAsk[aPrices.first];
			m_bookAsk.erase(aPrices.first);
		}
	}

	void ShowBookBidData()
	{
		for (map<double, bookData*>::iterator bookIterator = m_bookBid.begin(); bookIterator != m_bookBid.end(); bookIterator++)
		{
			//todo:show mts in printf()
			char szDateTime[15] = "\0";
			Int64ToDate(bookIterator->first, szDateTime);
			printf("%s,%s\n", szDateTime, m_bookBid[bookIterator->first]->BookDataToCharString());
		}
	}

	int DisplayAskBookAtCoords(int x, int y, int nMaxRows)
	{
		int nCounter = 0;
		char szStringToDisplay[1024] = "\0";
		mvprintw(y, x, "Ask Order Book");
		for (map<double, bookData*>::iterator bookIterator = m_bookAsk.begin(); bookIterator != m_bookAsk.end(); bookIterator++)
		{
			nCounter++;
			//todo:show mts in printf()
			sprintf(szStringToDisplay, "%s\n", m_bookAsk[bookIterator->first]->BookDataToCharString());
			mvprintw(y + nCounter, x, szStringToDisplay);
			if (nCounter == nMaxRows)
				return DATA_PROCESSED_SUCCESSFULLY;
		}
		return DATA_PROCESSED_SUCCESSFULLY;
	}

	int DisplayBidBookAtCoords(int x, int y, int nMaxRows)
	{
		int nCounter = 0;
		char szStringToDisplay[1024] = "\0";
		mvprintw(y, x, "Bid Order Book");
		for (map<double, bookData*>::iterator bookIterator = m_bookBid.begin(); bookIterator != m_bookBid.end(); bookIterator++)
		{
			nCounter++;
			//todo:show mts in printf()
			sprintf(szStringToDisplay, "%s", m_bookBid[bookIterator->first]->BookDataToCharString());
			mvprintw(y + nCounter, x, szStringToDisplay);
			if (nCounter == nMaxRows)
				return DATA_PROCESSED_SUCCESSFULLY;
		}
		return DATA_PROCESSED_SUCCESSFULLY;
	}

};

class candleData
{
private:
	double		dOpen;
	double		dClose;
	double		dHigh;
	double		dLow;
	double		dVolume;
public:
	candleData::candleData(
		double dOpen,
		double dClose,
		double dHigh,
		double dLow,
		double dVolume)
	{
		this->dOpen = dOpen;
		this->dClose = dClose;
		this->dHigh = dHigh;
		this->dLow = dLow;
		this->dVolume = dVolume;
	}

	char *CandleDataToCharString()
	{
		char szData[1024];
		sprintf(szData, "%.5f,%.5f,%.5f,%.5f,%.5f", dOpen, dClose, dHigh, dLow, dVolume); //todo: precision depends on channel requested -> have to be manageable
		return szData;
	}

};

class candleGroup
{
public:
	string		m_sChannelString;
	__int64		m_nChannelID;
	time_t		m_timeLastReceiveTime;
	map<__int64, candleData*>	m_candles;

	candleGroup()
	{
	}

	int AddOrUpdateCandle(__int64 nMts,
		double dOpen,
		double dClose,
		double dHigh,
		double dLow,
		double dVolume)
	{
		delete m_candles[nMts];
		m_candles.erase(nMts);
		m_candles[nMts] = new candleData(dOpen, dClose, dHigh, dLow, dVolume);
		std::time(&m_timeLastReceiveTime);

		if (m_candles.size() >= DATA_MAX_CANDLES) //if size more than max candles
		{
			m_candles.erase(m_candles.begin()); //delete first
		}

		return DATA_PROCESSED_SUCCESSFULLY;
	}

	time_t GetLastReceiveTime()
	{
		return m_timeLastReceiveTime;
	}

	void SetChannelID(__int64 nNewChannelID)
	{
		m_nChannelID = nNewChannelID;
	}

	__int64 GetChannelID()
	{
		return m_nChannelID;
	}

	string GetChannelString()
	{
		return m_sChannelString;
	}

	void SetChannelString(string sChannelString)
	{
		m_sChannelString = sChannelString;
	}

	void DeleteCandleGroup()
	{
		for (auto aMts : m_candles)
		{
			delete m_candles[aMts.first];
			m_candles.erase(aMts.first);
		}
	}

	int DisplayCandlesAtCoords(int x, int y, int nMaxRows)
	{
		map<int, string> mapStringsReversedToDisplay;
		int nRowCounter = 0;
		char szStringToDisplay[1024] = "\0";
		char szDateTime[15] = "\0";

		mvprintw(y, x, "Candles");
		for (map<__int64, candleData*>::iterator candleIterator = m_candles.begin(); candleIterator != m_candles.end(); candleIterator++)
		{
			nRowCounter++;
			Int64ToDate(candleIterator->first, szDateTime);
			sprintf(szStringToDisplay, "%s,%s\n", szDateTime, m_candles[candleIterator->first]->CandleDataToCharString());
			mapStringsReversedToDisplay[m_candles.size() - nRowCounter] = szStringToDisplay;
		}

		for (map<int, string>::iterator stringIterator = mapStringsReversedToDisplay.begin(); stringIterator != mapStringsReversedToDisplay.end(); stringIterator++)
		{
			mvprintw(y + stringIterator->first + 1, x, stringIterator->second.c_str());
			if (stringIterator->first == nMaxRows)
				return DATA_PROCESSED_SUCCESSFULLY;
		}
		return DATA_PROCESSED_SUCCESSFULLY;

		/*
		int nCounter = 0;
		char szStringToDisplay[1024]="\0";
		char szDateTime[15] = "\0";
		for (map<__int64, candleData*>::iterator candleIterator = m_candles.begin(); candleIterator != m_candles.end(); candleIterator++)
		{
		nCounter++;
		Int64ToDate(candleIterator->first, szDateTime);
		sprintf(szStringToDisplay, "%s,%s\n", szDateTime, m_candles[candleIterator->first]->CandleDataToCharString());
		mvprintw(y + nCounter - 1, x, szStringToDisplay);
		if (nCounter == nMaxRows)
		return DATA_PROCESSED_SUCCESSFULLY;
		}
		return DATA_PROCESSED_SUCCESSFULLY;
		*/
	}

	void ShowCandleGroupData()
	{
		for (map<__int64, candleData*>::iterator candleIterator = m_candles.begin(); candleIterator != m_candles.end(); candleIterator++)
		{
			//todo:show mts in printf()
			char szDateTime[15] = "\0";
			Int64ToDate(candleIterator->first, szDateTime);
			printf("%s,%s\n", szDateTime, m_candles[candleIterator->first]->CandleDataToCharString());
		}
	}
};

class dataWebSocketData
{
private:

	time_t				m_timeTimeStamp;
	string				m_szMessage;
	time_t				m_timeLastUpdateTimeStamp;
	int					m_nStatus;
	dataWebSocketData	*m_pPreviousData, *m_pNextData;

public:

	dataWebSocketData(string szMessage)
	{
		std::time(&m_timeTimeStamp);
		std::time(&m_timeLastUpdateTimeStamp);
		m_szMessage = szMessage;
		m_nStatus = DATA_NOT_PROCESSED;
		m_pPreviousData = nullptr;
		m_pNextData = nullptr;
	}

	time_t GetLastUpdateTime()
	{
		return m_timeLastUpdateTimeStamp;
	}

	dataWebSocketData* InsertWebSocketData(dataWebSocketData* pWebSocketData, string szMessage)
	{
		/*----------------------------*/
		/* Purpose : insert new data

		create new data ptr
		if data is empty
		return new data ptr
		point new data ptr with head ptr as its previous
		point head ptr with new data ptr as its next
		delete last processed message
		if buffer is full
		todo:
		return new data ptr
		/*----------------------------*/
		dataWebSocketData* pNewData = new dataWebSocketData(szMessage);

		if (pWebSocketData == nullptr)
			return pNewData;
		pWebSocketData->m_pNextData = pNewData;
		pNewData->m_pPreviousData = pWebSocketData;

		if (pNewData->GetNumOfMessages(pNewData) >= DATA_MAX_MESSAGES)
		{
			pNewData = DeleteLastProcessedMessage(pNewData);
		}
		return pNewData;
	}

	int GetNumOfMessages(dataWebSocketData* pMessages)
	{
		while ((pMessages != nullptr) || (pMessages != NULL))
		{
			return GetNumOfMessages(pMessages->m_pPreviousData) + 1;
		}
		return 0;
	}

	void ShowAllMessages(int x, int y, dataWebSocketData * pMessages)
	{
		int nRow = 0;
		while (pMessages != nullptr)
		{
			nRow++;
			pMessages->ShowMessage(x, y, nRow + 1);
			pMessages = pMessages->m_pPreviousData;
		}
	}

	void ShowMessage(int x, int y, int nRow)
	{
		char sTimeStamp[15] = "\0", sLastUpdateTimeStamp[15] = "\0";
		Time_tToString(m_timeTimeStamp, sTimeStamp);
		Time_tToString(m_timeLastUpdateTimeStamp, sLastUpdateTimeStamp);
		if (m_szMessage.length()>256)
			mvprintw(y + nRow, x, "Time : %s,\tMessage : %s\tTimeLastUpdate : %s\tStatus : %d", sTimeStamp, m_szMessage.substr(0, 255), sLastUpdateTimeStamp, m_nStatus);
		else
			mvprintw(y + nRow, x, "Time : %s,\tMessage : %s\tTimeLastUpdate : %s\tStatus : %d", sTimeStamp, m_szMessage.c_str(), sLastUpdateTimeStamp, m_nStatus);
	}

	int GetStatus()
	{
		return m_nStatus;
	}

	void UpdateWebSocketData(int nStatus)
	{
		m_nStatus = nStatus;
		std::time(&m_timeLastUpdateTimeStamp);
	}

	string GetMessage()
	{
		return m_szMessage;
	}

	string GetUnprocessedMessage(dataWebSocketData * pMessages)
	{
		/*------------------------*/
		/* Purpose : Get earliest unprocessed message

		find to last message
		find earliest not processed message
		if found
		return message
		return blank
		/*------------------------*/
		dataWebSocketData *pMessagePtr = pMessages;
		if (pMessagePtr == nullptr)
			return "";
		while (pMessagePtr->m_pPreviousData != nullptr)
		{
			pMessagePtr = pMessagePtr->m_pPreviousData;
		}

		while ((pMessagePtr->GetStatus() != DATA_NOT_PROCESSED) && (pMessagePtr->m_pNextData != nullptr))
		{
			pMessagePtr = pMessagePtr->m_pNextData;
		}

		if ((pMessagePtr != nullptr) && (pMessagePtr->GetStatus() == DATA_NOT_PROCESSED))
		{
			pMessagePtr->UpdateWebSocketData(DATA_PROCESSED_SUCCESSFULLY);
			return pMessagePtr->GetMessage();
		}
		return "";
	}

	dataWebSocketData* DeleteLastProcessedMessage(dataWebSocketData * pMessages)
	{

		/*--------------------------*/
		/*Purpose: to delete last message and return pMessages linked list

		Go to next message in list until its previous is not pointing to anything (last message)
		if last message is null
		return head data //nothing in list
		if last message is processed
		if there is a nextmessage
		point nextmessage's previous to null
		delete lastmessage
		return head data
		/*--------------------------*/
		dataWebSocketData *pLastMessage, *pHead;

		pLastMessage = pMessages;
		pHead = pMessages;
		while (pLastMessage->m_pPreviousData != nullptr) //Go to next message in list until its previous is not pointing to anything (last message)
			pLastMessage = pLastMessage->m_pPreviousData;
		if (pLastMessage == nullptr) //if last message is null
			return pHead; //nothing in list
		if (pLastMessage->GetStatus() == DATA_PROCESSED_SUCCESSFULLY)//if last message is processed
		{
			if (pLastMessage->m_pNextData != nullptr) //if there is a nextmessage
				pLastMessage->m_pNextData->m_pPreviousData = nullptr; //point nextmessage's previous to null
			delete pLastMessage; //delete message
		}
		return pHead; //return head data
	}

	dataWebSocketData* DeleteAllMessages(dataWebSocketData * pMessages)
	{

		/*--------------------------*/
		/*Purpose: to delete all messages in linked list

		if already null
		return null
		go to previous message in list until its previous is not pointing to anything (last message)
		if last message is head
		delete head
		return nullptr
		if last message's next data exist
		point last message's next data's previous pointer to null
		delete last message
		call in deleteallmessages with the head

		/*--------------------------*/
		dataWebSocketData *pLastMessage, *pHead;
		pLastMessage = pMessages;
		pHead = pMessages;
		if (pMessages == nullptr)
			return nullptr;
		while (pLastMessage->m_pPreviousData != nullptr) //Go to next message in list until its previous is not pointing to anything (last message)
			pLastMessage = pLastMessage->m_pPreviousData;
		if (pLastMessage == pHead) //if last message is null
		{
			delete pHead;
			return nullptr; //nothing in list
		}
		if (pLastMessage->m_pNextData != nullptr)
			pLastMessage->m_pNextData->m_pPreviousData = nullptr;
		delete pLastMessage;
		DeleteAllMessages(pHead);
	}

	dataWebSocketData* getPreviousPtr()
	{
		return m_pPreviousData;
	}

};


//Todo : change the function name
void Int64ToDate(__int64 n64Milliseconds, char szStringOut[])
{
	char sTime[15]; //14 characters and 1 trailing endstring character
	time_t rawtime = n64Milliseconds / 1000; //remove the trailing 1000 
											 //http://www.cplusplus.com/reference/ctime/strftime/
	strftime(sTime, 15, "%Y%m%d%H%M%S", localtime(&rawtime));
	strcpy(szStringOut, sTime);
}

void Time_tToString(time_t timeRawTime, char szStringOut[])
{
	tm* ptm = std::localtime(&timeRawTime);
	char szBuffer[15];
	strftime(szBuffer, 15, "%Y%m%d%H%M%S", ptm);
	strcpy(szStringOut, szBuffer);
}
