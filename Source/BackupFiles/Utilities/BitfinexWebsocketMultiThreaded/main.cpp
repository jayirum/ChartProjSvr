#include <string>
#include <curses.h>			/* curses.h includes stdio.h */  
#include "Precompiled.h"
#include "StringsforDisplay.h"

//JSON parser https://github.com/Tencent/rapidjson/


#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

//for time conversion from __int64 of milliseconds
#include <chrono>

//for multithreading
#include <thread>

//for the __int64 formatting from bitfinex int 64 bit
#include <inttypes.h>

#include "BitfinexWebsocketHeader.h"
using namespace std;
using namespace KennyKerr; //todo: replace with a proper msdn winhttp websocket solution


struct windows_exception
{
	DWORD error;

	windows_exception(DWORD const value = GetLastError()) :
		error{ value }
	{}
};

struct winhttp_traits
{
	using pointer = HINTERNET;

	static auto invalid() throw() -> pointer
	{
		return nullptr;
	}

	static auto close(pointer value) throw() -> void
	{
		VERIFY(WinHttpCloseHandle(value));
	}
};

using winhttp = unique_handle<winhttp_traits>;

int LogicThread();
int ReceiveThread();
int createWebSocketConnection();
int createChannels();
int sendAllData();
int sendData(char sJson[]);
int receiveAllData();
int closeWebSocketConnection();
int processEventMessages(rapidjson::Document &jsonData);
int processDataMessages(rapidjson::Document &jsonData);
int processDataTicker(rapidjson::Document &jsonData, tickerData *tickerGroupData);
int processDataCandles(rapidjson::Document &jsonData, candleGroup *candleGroupData);
int processDataBook(rapidjson::Document &jsonData, bookGroup *bookGroupData);
int processMessages();
void unsubscribeChannel(int nChanId);
void subscribeChannel(string szChannel);

WINHTTP_WEB_SOCKET_BUFFER_TYPE winhttpType = WINHTTP_WEB_SOCKET_BUFFER_TYPE{}; //used for checking the data type of data received from websocket
winhttp gWebSocket = unique_handle<winhttp_traits>(); //used as a global variable for the websocket handle

dataWebSocketData *dwsdSendMessageQueue, *dwsdReceivedMessageQueue;

class cryptoGroup
{
public:
	candleGroup candleGroups;	// 1 candlegroup for 1 cryptocurrencyfiat pair
	bookGroup bookGroups; // 1 orderbookgroup for 1 cryptocurrencyfiat pair
	tickerData ticker; // 1 ticker for 1 cryptocurrencyfiat pair
	cryptoGroup::cryptoGroup()
	{
	}
};

cryptoGroup cryptoGroup1;

DWORD g_nCount; //used for checking the data length received from websocket
char g_szBuffer[65536]; //used to store data received from websocket
string g_szMessage; //used for appending fragments of data received from websocket
int g_nConnected = DATA_NOT_PROCESSED; //used for checking connection
time_t g_timeNow;

int createWebSocketConnection()
{
	/*----------------------------*/
	/*Purpose : To create HTTP connection,
	then to upgrade to websocket connection
	if version is different or no response
	return 1 - unsuccessful
	return 0
	/*----------------------------*/

	auto session = winhttp
	{
		WinHttpOpen(nullptr,
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		nullptr,
		nullptr,
		0)
	};

	if (!session) throw windows_exception{};

	auto connection = winhttp
	{
		WinHttpConnect(session.get(),
		L"api.bitfinex.com",
		80,
		0)
	};

	if (!connection) throw windows_exception{};

	auto request = winhttp
	{
		WinHttpOpenRequest(connection.get(),
		nullptr,
		L"/ws/2",
		nullptr,
		nullptr,
		nullptr,
		0)
	};

	if (!request) throw windows_exception{};

	if (!WinHttpSetOption(request.get(),
		WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET,
		nullptr,
		0))
	{
		throw windows_exception{};
	}

	if (!WinHttpSendRequest(request.get(),
		nullptr,
		0,
		nullptr,
		0,
		0,
		0))
	{
		throw windows_exception{};
	}

	if (!WinHttpReceiveResponse(request.get(),
		nullptr))
	{
		throw windows_exception{};
	}

	auto status = DWORD{};
	auto size = DWORD{ sizeof(status) };

	if (!WinHttpQueryHeaders(request.get(),
		WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
		WINHTTP_HEADER_NAME_BY_INDEX,
		&status,
		&size,
		nullptr))
	{
		throw windows_exception{};
	}

	if (HTTP_STATUS_SWITCH_PROTOCOLS != status)
	{
		mvprintw(0, 0, "Not websocket server\n");
		return 0;
	}

	gWebSocket = winhttp
	{
		WinHttpWebSocketCompleteUpgrade(request.get(),
		0)
	};

	if (!gWebSocket) throw windows_exception{};

	long result = WinHttpWebSocketReceive(gWebSocket.get(),
		g_szBuffer,
		sizeof(g_szBuffer),
		&g_nCount,
		&winhttpType);
	if (ERROR_SUCCESS == result)
	{
		if (WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE == winhttpType)
		{
			VERIFY_(ERROR_SUCCESS, WinHttpWebSocketClose(gWebSocket.get(),
				WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS,
				nullptr,
				0));

		}

		auto message = string{ g_szBuffer, g_nCount };

		rapidjson::Document rjResponse;
		rjResponse.Parse(message.c_str()); //convert message string to json
		mvprintw(0, 0, "\nString Received : %s\n", message.c_str());
		if (rjResponse.HasMember("event"))
		{
			if (strcmp(rjResponse["event"].GetString(), "info") == 0) //check whether the first response is event->info
			{
				if (rjResponse["version"].GetInt64() == 2) //check whether the first response is version->2
				{
					mvprintw(0, 0, "\nSuccessfully connected to Bitfinex Websockets Version 2\n");
					return DATA_PROCESSED_SUCCESSFULLY;
				}
				else
				{
					mvprintw(0, 0, "\nConnection Error to Bitfinex Websockets Version 2, Different Version is reported.\n");
					return DATA_PROCESSED_UNSUCCESSFULLY;
				}
			}
			else
			{
				mvprintw(0, 0, "\nConnection Error to Bitfinex Websockets Version 2, Different Data Format(1) is received : %s\n", rjResponse["event"].GetString());
				return DATA_PROCESSED_UNSUCCESSFULLY;
			}
		}
		else
		{
			mvprintw(0, 0, "\nConnection Error to Bitfinex Websockets Version 2, Different Data Format(2) is received : %s\n", rjResponse["event"].GetString());
			return DATA_PROCESSED_UNSUCCESSFULLY;
		}
	}
	return DATA_PROCESSED_UNSUCCESSFULLY;
}

int main()
{
	thread r1(ReceiveThread);
	thread l1(LogicThread);
	r1.join();
	return 0;
}

int ReceiveThread()
{
	/*----------------------------*/
	/*Purpose : To create connection
	assign candlegroup

	do following till the process is interrupted by user to stop
	take timeNow
	maintain connection (websocket/channels/process "event" json messages)
	receive all data
	send all data
	update candles (process data json messages)*/
	/*----------------------------*/

	initscr(); //start the curses mode for x y display
	resize_term(SCREEN_TERMINAL_Y_SIZE, SCREEN_TERMINAL_X_SIZE); // resize the screen
	curs_set(0);	//remove text cursor
	int nUserInterrupt = 0; //todo: for ctrl c in future

	do {} while (createWebSocketConnection() != ERROR_SUCCESS);
	g_nConnected = DATA_PROCESSED_SUCCESSFULLY;
	createChannels(); //set the data to send to create channels
	while (nUserInterrupt != DATA_USER_EXIT)
	{
		std::time(&g_timeNow);
		receiveAllData();
	}
	return 0;
}

int LogicThread()
{
	while (1)
		if (g_nConnected == DATA_PROCESSED_SUCCESSFULLY)
		{
			sendAllData();
			processMessages();

			cryptoGroup1.ticker.DisplayTickerAtCoords(0, 0);
			cryptoGroup1.bookGroups.DisplayBidBookAtCoords(0, 2, -1);
			cryptoGroup1.bookGroups.DisplayAskBookAtCoords(30, 2, -1);
			cryptoGroup1.candleGroups.DisplayCandlesAtCoords(70, 2, 30);
			dwsdReceivedMessageQueue->ShowAllMessages(0, 35, dwsdReceivedMessageQueue);
			refresh();
			clear();

			/*---todo: temporary maintain connection code for test 
			if (g_nConnected == DATA_PROCESSED_SUCCESSFULLY)
			{
				mvprintw(1, 0, "Time to live : %d , now %d", DATA_MAX_TIME_TO_LIVE, timeNow - dwsdReceivedMessageQueue->GetLastUpdateTime());
				if (timeNow - dwsdReceivedMessageQueue->GetLastUpdateTime() > (DATA_MAX_TIME_TO_LIVE))
					createChannels();
			}
			/*---temporary maintain connection code for test---*/

		}
	return 0;
}

int createChannels()
{
	//symbols refer to https://api.bitfinex.com/v1/symbols
	// following subject to change
	//["btcusd","ltcusd","ltcbtc","ethusd","ethbtc","etcbtc","etcusd","rrtusd","rrtbtc","zecusd","zecbtc","xmrusd","xmrbtc","dshusd","dshbtc","btceur","xrpusd","xrpbtc","iotusd","iotbtc","ioteth","eosusd","eosbtc","eoseth","sanusd","sanbtc","saneth","omgusd","omgbtc","omgeth","bchusd","bchbtc","bcheth","neousd","neobtc","neoeth","etpusd","etpbtc","etpeth","qtmusd","qtmbtc","qtmeth","avtusd","avtbtc","avteth","edousd","edobtc","edoeth","btgusd","btgbtc","datusd","datbtc","dateth","qshusd","qshbtc","qsheth","yywusd","yywbtc","yyweth","gntusd","gntbtc","gnteth","sntusd","sntbtc","snteth","ioteur"]
	//timeframe  https://bitfinex.readme.io/v2/reference#ws-public-candle

	/* JSON FORMAT

	{
	"event": "subscribe",
	"channel": "candles",
	"key": "trade:1m:tBTCUSD"
	}

	*/
	cryptoGroup1.candleGroups.SetChannelString("{\r\n\"event\": \"subscribe\",\r\n\"channel\":\"candles\",\r\n\"key\":\"trade:1m:tBTCUSD\"\r\n}\0");
	cryptoGroup1.bookGroups.SetChannelString("{\r\n  \"event\": \"subscribe\",\r\n  \"channel\": \"book\",\r\n  \"symbol\": \"tBTCUSD\",\r\n  \"prec\": \"P0\",\r\n  \"freq\": \"F0\",\r\n  \"len\": 25\r\n}");
	cryptoGroup1.ticker.SetChannelString("{\r\n  \"event\": \"subscribe\",\r\n  \"channel\": \"ticker\",\r\n  \"symbol\": \"tBTCUSD\"\r\n}");

	dwsdReceivedMessageQueue = dwsdReceivedMessageQueue->DeleteAllMessages(dwsdReceivedMessageQueue);
	dwsdSendMessageQueue = dwsdSendMessageQueue->DeleteAllMessages(dwsdSendMessageQueue);
	cryptoGroup1.candleGroups.DeleteCandleGroup();
	subscribeChannel(cryptoGroup1.candleGroups.GetChannelString()); // queue the message to send out to create channel 
	subscribeChannel(cryptoGroup1.bookGroups.GetChannelString()); // queue the message to send out to create channel 
	subscribeChannel(cryptoGroup1.ticker.GetChannelString()); // queue the message to send out to create channel 
	return DATA_PROCESSED_SUCCESSFULLY;
}



int sendData(char sJson[])
{
	/*---------------------------------*/
	/*Purpose : The method to send data*/
	/*---------------------------------*/
	auto result = WinHttpWebSocketSend(gWebSocket.get(),
		WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE,
		sJson,
		strlen(sJson));

	return result; //return 0 if successful 
}

int sendAllData()
{
	/*---------------------------------*/
	/*Purpose : To send all data through the websocket by going through SendMessageQueue with the status -1
	then update to 0*/
	/*---------------------------------*/
	char szOutBuffer[1024];
	string szMessageOut;

	szMessageOut = dwsdSendMessageQueue->GetUnprocessedMessage(dwsdSendMessageQueue);
	while (szMessageOut != "")
	{
		sprintf(szOutBuffer, "%s", szMessageOut.c_str());
		sendData(szOutBuffer);
		szMessageOut = dwsdSendMessageQueue->GetUnprocessedMessage(dwsdSendMessageQueue);
	}

	return DATA_PROCESSED_SUCCESSFULLY;
}


int receiveAllData()
{
	/*---------------------------------*/
	/*Purpose : To receive all data from the websocket and store in the ReceivedMessageQueue*/
	/*---------------------------------*/
	if (ERROR_SUCCESS == WinHttpWebSocketReceive(gWebSocket.get(),
		g_szBuffer,
		sizeof(g_szBuffer),
		&g_nCount,
		&winhttpType))
	{
		// Messages might be fragmented up. use the winhttpType to determine whether the message is broken. if so, combine the messages if needed.

		g_szMessage.clear();
		g_szMessage.append(g_szBuffer, g_nCount); //append to message whatever was received

		if (winhttpType == WINHTTP_WEB_SOCKET_UTF8_FRAGMENT_BUFFER_TYPE)
		{
			while (winhttpType == WINHTTP_WEB_SOCKET_UTF8_FRAGMENT_BUFFER_TYPE)
			{
				//receive until WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE is received.
				if (ERROR_SUCCESS == WinHttpWebSocketReceive(gWebSocket.get(),
					g_szBuffer,
					sizeof(g_szBuffer),
					&g_nCount,
					&winhttpType))
				{
					g_szMessage.append(g_szBuffer, g_nCount); //append till the last fragment of message
				}
			}
		}

		//add the received message into the ReceivedMessageQueue
		dwsdReceivedMessageQueue = dwsdReceivedMessageQueue->InsertWebSocketData(dwsdReceivedMessageQueue, g_szMessage);
		return DATA_PROCESSED_SUCCESSFULLY;
	}
	return DATA_PROCESSED_UNSUCCESSFULLY; //error if receive is not successful
}

int processMessages()
{
	rapidjson::Document jsonData;
	string szMessageIn;

	//dataWebSocketData *pdwsdPtr = dwsdReceivedMessageQueue;
	szMessageIn = dwsdReceivedMessageQueue->GetUnprocessedMessage(dwsdReceivedMessageQueue);
	while (szMessageIn != "")
	{
		jsonData.Parse(szMessageIn.c_str());
		if (jsonData.IsObject())
		{
			processEventMessages(jsonData);
		}
		else
		{
			processDataMessages(jsonData);
		}
		szMessageIn = dwsdReceivedMessageQueue->GetUnprocessedMessage(dwsdReceivedMessageQueue);
	}
	return DATA_PROCESSED_SUCCESSFULLY;
}


int processEventMessages(rapidjson::Document &jsonData)
{
	//take reference of functions (ping,subscribe,unsubscribe,errorcodes,etc) from https://docs.bitfinex.com/v2/docs/ws-general
	/*---------------------------------*/
	/*Purpose:		Go through Received Message Queue
	Get Event Messages and Error Codes
	Update Message Processed Status
	*/
	/*---------------------------------*/


	if (jsonData.HasMember("event"))
	{
		if (strcmp(jsonData["event"].GetString(), "info") == 0) //if the event has the value of info
		{
			if (jsonData.HasMember("code")) //check the code if it exists
			{
				switch (jsonData["code"].GetInt64())
				{
				case DATA_WEBSOCKET_EVENT_RESTARTWEBSOCKETCONNECTION:
					closeWebSocketConnection();
					createWebSocketConnection();
					createChannels();
					return DATA_PROCESSED_SUCCESSFULLY;
				case DATA_WEBSOCKET_EVENT_WEBSOCKETMAINTENANCESTART:
					// do necessary to wait for DATA_MAX_TIME_TO_LIVE seconds max
					return DATA_PROCESSED_SUCCESSFULLY;
				case DATA_WEBSOCKET_EVENT_WEBSOCKETMAINTENANCEEND:
					// resume application
					return DATA_PROCESSED_SUCCESSFULLY;
				}
			}
		}
		else if (strcmp(jsonData["event"].GetString(), "subscribed") == 0) //if the event has the value of subscribed
		{
			rapidjson::Document jsonBookChannel;
			rapidjson::Document jsonCandlesChannel;
			rapidjson::Document jsonTickerChannel;
			jsonBookChannel.Parse(cryptoGroup1.bookGroups.GetChannelString().c_str());
			jsonCandlesChannel.Parse(cryptoGroup1.candleGroups.GetChannelString().c_str());
			jsonTickerChannel.Parse(cryptoGroup1.ticker.GetChannelString().c_str());

			//if channel is book
			if (strcmp("book", jsonData["channel"].GetString()) == 0)
			{
				//if candlegroup contains the key subscribed
				if (strcmp(jsonBookChannel["symbol"].GetString(), jsonData["symbol"].GetString()) == 0)
				{
					//assign id to the candlegroup
					cryptoGroup1.bookGroups.SetChannelID(jsonData["chanId"].GetInt());
					return DATA_PROCESSED_SUCCESSFULLY;
				}
			}
			//if channel is ticker
			else if (strcmp("ticker", jsonData["channel"].GetString()) == 0)
			{
				//if candlegroup contains the key subscribed
				if (strcmp(jsonTickerChannel["symbol"].GetString(), jsonData["symbol"].GetString()) == 0)
				{
					//assign id to the candlegroup
					cryptoGroup1.ticker.SetChannelID(jsonData["chanId"].GetInt());
					return DATA_PROCESSED_SUCCESSFULLY;
				}
			}
			//if channel is candle
			else if (strcmp("candles", jsonData["channel"].GetString()) == 0)
			{
				//if candlegroup contains the key subscribed
				if (strcmp(jsonCandlesChannel["key"].GetString(), jsonData["key"].GetString()) == 0)
				{
					//assign id to the candlegroup
					cryptoGroup1.candleGroups.SetChannelID(jsonData["chanId"].GetInt());
					return DATA_PROCESSED_SUCCESSFULLY;
				}
			}
		}
		else if (strcmp(jsonData["event"].GetString(), "error") == 0) //if the event has the value of error
		{
			mvprintw(0, 0, "\nError Code : %" PRIu64 "\nError Message : %s\n", jsonData["code"].GetInt64(), jsonData["msg"].GetString()); //log error
		}
	}
	return DATA_PROCESSED_UNSUCCESSFULLY;
}

int processDataCandles(rapidjson::Document &jsonData, candleGroup *candleGroupData)
{
	if (jsonData[1].IsArray()) // if it is data ...
	{
		//if the 2nd element is an array of arrays i.e. 3d array 
		if (jsonData[1][0].IsArray())
		{
			for (int nJsonCounter = 0; nJsonCounter < jsonData[1].Size(); nJsonCounter++)
			{
				//add new candle
				candleGroupData->AddOrUpdateCandle(
					jsonData[1][nJsonCounter][0].GetInt64(),
					jsonData[1][nJsonCounter][1].GetFloat(),
					jsonData[1][nJsonCounter][2].GetFloat(),
					jsonData[1][nJsonCounter][3].GetFloat(),
					jsonData[1][nJsonCounter][4].GetFloat(),
					jsonData[1][nJsonCounter][5].GetFloat());
			}
			return DATA_PROCESSED_SUCCESSFULLY;
		}
		else // if the 2nd element is an array of data i.e. 2d array
		{
			candleGroupData->AddOrUpdateCandle(
				jsonData[1][0].GetInt64(),
				jsonData[1][1].GetFloat(),
				jsonData[1][2].GetFloat(),
				jsonData[1][3].GetFloat(),
				jsonData[1][4].GetFloat(),
				jsonData[1][5].GetFloat());
			return DATA_PROCESSED_SUCCESSFULLY;
		}
	}
	else if (strcmp(jsonData[1].GetString(), "hb") == 0)
	{
		//maintain connection as it is a heartbeat message
		//printf("\nCandles Heartbeat Message\n");
		return DATA_PROCESSED_SUCCESSFULLY;
	}
}

int processDataBook(rapidjson::Document &jsonData, bookGroup *bookGroupData)
{
	if (jsonData[1].IsArray()) // if it is data ...
	{
		//if the 2nd element is an array of arrays i.e. 3d array 
		if (jsonData[1][0].IsArray())
		{
			for (int nJsonCounter = 0; nJsonCounter < jsonData[1].Size(); nJsonCounter++)
			{
				//add item into book
				/*only for cryptocurrencies*/
				bookGroupData->AddOrUpdateBook(
					jsonData[1][nJsonCounter][0].GetFloat(),
					jsonData[1][nJsonCounter][1].GetInt(),
					jsonData[1][nJsonCounter][2].GetFloat());
				/* only for currencies
				cryptoGroupData->bookGroups.AddOrUpdateBook(
				jsonData[1][nJsonCounter][0].GetFloat(),
				jsonData[1][nJsonCounter][1].GetFloat(),
				jsonData[1][nJsonCounter][2].GetFloat(),
				jsonData[1][nJsonCounter][3].GetInt(),
				jsonData[1][nJsonCounter][4].GetFloat());				*/
			}
			return DATA_PROCESSED_SUCCESSFULLY;
		}
		else // if the 2nd element is an array of data i.e. 2d array
		{
			/*only for cryptocurrencies*/
			bookGroupData->AddOrUpdateBook(
				jsonData[1][0].GetFloat(),
				jsonData[1][1].GetInt(),
				jsonData[1][2].GetFloat());
			/*only for currencies
			cryptoGroupData->candleGroups.AddOrUpdateBook(
			jsonData[1][0].GetInt64(),
			jsonData[1][1].GetFloat(),
			jsonData[1][2].GetFloat(),
			jsonData[1][3].GetFloat(),
			jsonData[1][4].GetFloat(),
			jsonData[1][5].GetFloat());
			*/
			return DATA_PROCESSED_SUCCESSFULLY;
		}
	}
	else if (strcmp(jsonData[1].GetString(), "hb") == 0)
	{
		//maintain connection as it is a heartbeat message
		//printf("\nBook Heartbeat Message\n");
		return DATA_PROCESSED_SUCCESSFULLY;
	}
}

int processDataTicker(rapidjson::Document &jsonData, tickerData *tickerGroupData)
{
	if (jsonData[1].IsArray()) // if it is data ...
	{
		/*only for cryptocurrencies*/
		tickerGroupData->SetTickerData(jsonData[1][0].GetFloat(),
			jsonData[1][1].GetFloat(),
			jsonData[1][2].GetFloat(),
			jsonData[1][3].GetFloat(),
			jsonData[1][4].GetFloat(),
			jsonData[1][5].GetFloat(),
			jsonData[1][6].GetFloat(),
			jsonData[1][7].GetFloat(),
			jsonData[1][8].GetFloat(),
			jsonData[1][9].GetFloat());
		/*
		cryptoGroupData->ticker.dBid =						jsonData[1][0].GetFloat();
		cryptoGroupData->ticker.dBidSize =					jsonData[1][1].GetFloat();
		cryptoGroupData->ticker.dAsk =						jsonData[1][2].GetFloat();
		cryptoGroupData->ticker.dAskSize =					jsonData[1][3].GetFloat();
		cryptoGroupData->ticker.dDailyChange =				jsonData[1][4].GetFloat();
		cryptoGroupData->ticker.dDailyChangePercentage =	jsonData[1][5].GetFloat();
		cryptoGroupData->ticker.dLastPrice =				jsonData[1][6].GetFloat();
		cryptoGroupData->ticker.dDailyVolume =				jsonData[1][7].GetFloat();
		cryptoGroupData->ticker.dDailyHigh =				jsonData[1][8].GetFloat();
		cryptoGroupData->ticker.dDailyLow =					jsonData[1][9].GetFloat();
		*/
		/*for currencies , there would be bid period (days,integer) and ask period (days,integer)*/
		return DATA_PROCESSED_SUCCESSFULLY;
	}
	else if (strcmp(jsonData[1].GetString(), "hb") == 0)
	{
		//maintain connection as it is a heartbeat message
		//printf("\nTicker Heartbeat Message\n");
		return DATA_PROCESSED_SUCCESSFULLY;
	}
}

int processDataMessages(rapidjson::Document &jsonData)
{
	/*---------------------------------*/
	/*Purpose:
	Get Data Messages ,
	update orderbook
	update ticker
	update candles
	*/
	/*---------------------------------*/

	//TODO: check if the channelid is not 0

	int nChannel = jsonData[0].GetInt64();//<== temporary delete me
	int nBookChannel = cryptoGroup1.bookGroups.GetChannelID();
	int nTickerChannel = cryptoGroup1.ticker.GetChannelID();
	int nCandleChannel = cryptoGroup1.candleGroups.GetChannelID();
	if (jsonData[0].GetInt64() == cryptoGroup1.bookGroups.GetChannelID()) //if the first element of the data received has the candles channelId
		return processDataBook(jsonData, &cryptoGroup1.bookGroups);
	else if (jsonData[0].GetInt64() == cryptoGroup1.ticker.GetChannelID()) //if the first element of the data received has the candles channelId
		return processDataTicker(jsonData, &cryptoGroup1.ticker);
	else if (jsonData[0].GetInt64() == cryptoGroup1.candleGroups.GetChannelID()) //if the first element of the data received has the candles channelId
		return processDataCandles(jsonData, &cryptoGroup1.candleGroups);
}

int closeWebSocketConnection()
{
	int result = WinHttpWebSocketClose(gWebSocket.get(),
		WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS,
		NULL,
		0);
	if (result != ERROR_SUCCESS)
	{
		//log error
		return 1;
	}
}

void subscribeChannel(string szChannel)
{
	char sChannelSubscriptionJson[1024] = "";
	sprintf(sChannelSubscriptionJson, "%s", szChannel.c_str());
	dwsdSendMessageQueue = dwsdSendMessageQueue->InsertWebSocketData(dwsdSendMessageQueue, sChannelSubscriptionJson);
}

void unsubscribeChannel(int nChanId)
{
	char sChannelSubscriptionJson[1024] = "";
	sprintf(sChannelSubscriptionJson, "{\r\n   \"event\": \"unsubscribe\",\r\n   \"chanId\":%d\r\n}\0", nChanId);
	dwsdSendMessageQueue = dwsdSendMessageQueue->InsertWebSocketData(dwsdSendMessageQueue, sChannelSubscriptionJson);
}

