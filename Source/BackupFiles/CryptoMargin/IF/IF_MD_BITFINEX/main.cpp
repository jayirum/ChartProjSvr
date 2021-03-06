#include "../../../IRUM_UTIL/TcpClient.h"
#include <string>
#include <curses.h>			/* curses.h includes stdio.h */  
#include "Precompiled.h"
#include "../../../IRUM_UTIL/LogMsg.h"
#include "../../Common/Struct.h"

//JSON parser https://github.com/Tencent/rapidjson/
#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "MessageQueue.h"
#include "marketdata.h"
#include "../../Common/SMClient.h"

//for time conversion from __int64 of milliseconds
#include <chrono>

//for multithreading
#include <thread>

//for the __int64 formatting from bitfinex int 64 bit
#include <inttypes.h>

#include "BitfinexWebsocketHeader.h"
using namespace std;
using namespace KennyKerr;

unsigned int __stdcall ProcessDataThread(LPVOID lp);

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

int createWebSocketConnection();
int createChannels();
int sendAllData();
int sendData(char sJson[]);
int receiveAllData();
int closeWebSocketConnection();
int processEventMessages(rapidjson::Document &jsonData);
int processDataMessages(rapidjson::Document &jsonData);
int processDataTicker(rapidjson::Document &jsonData);
//int processDataCandles(rapidjson::Document &jsonData, candleGroup *candleGroupData);
int processDataBook(rapidjson::Document &jsonData);
int processMessages();
void unsubscribeChannel(int nChanId);
void subscribeChannel(string szChannel);

//WINHTTP_WEB_SOCKET_BUFFER_TYPE winhttpType = WINHTTP_WEB_SOCKET_BUFFER_TYPE{}; //used for checking the data type of data received from websocket
winhttp gWebSocket = unique_handle<winhttp_traits>(); //used as a global variable for the websocket handle

//dataWebSocketData *g_pWsdSendMsgQueue=nullptr, *g_pWsdRecvMsgQueue=nullptr;
CMessageQueue g_WsdSendMsgQueue, g_WsdRecvMsgQueue;


bookGroup g_bookGroups; // 1 orderbookgroup for 1 cryptocurrencyfiat pair
tickerData g_ticker; // 1 ticker for 1 cryptocurrencyfiat pair

char szBuffer[65536]; //used to store data received from websocket
char g_msg[512];
CLogMsg g_log;
CTcpClient	g_tcpClient;

enum {IDX_BTC, IDX_ETH};
double g_dOpen[2] = { -1, -1};


int createWebSocketConnection()
{
	/*----------------------------*/
	/*Purpose : To create HTTP connection,
	then to upgrade to websocket connection
	if version is different or no response
	return 1 - unsuccessful
	return 0
	/*----------------------------*/

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Initializes the use of WinHttp functions and returns a WinHttp-session handle.
	HINTERNET httpSess = WinHttpOpen(nullptr,
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		nullptr,
		nullptr,
		0);
	if (!httpSess)
		throw windows_exception{};

	auto session = winhttp(httpSess);
	if (!session) throw windows_exception{};


	//////////////////////////////////////////////////////////////////////////////////////////////
	// specifies the initial target server of an HTTP request and 
	// returns an HINTERNET connection handle to an HTTP session for that initial target.
	HINTERNET httpConn = WinHttpConnect(session.get(),
		L"api.bitfinex.com",
		80,
		0);
	if (!httpConn)
		throw windows_exception{};

	auto connection = winhttp(httpConn);
	if (!connection) throw windows_exception{};


	//////////////////////////////////////////////////////////////////////////////////////////////
	// creates an HTTP request handle.
	HINTERNET httpReq = WinHttpOpenRequest(connection.get(),
		nullptr,
		L"/ws/2",
		nullptr,
		nullptr,
		nullptr,
		0);
	if (!httpReq)	throw windows_exception{};
	auto request = winhttp(httpReq);
	if (!request) throw windows_exception{};


	/////////////////////////////////////////////////////////
	// sets an Internet option
	if (!WinHttpSetOption(request.get(),
		WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET,
		nullptr,
		0))
	{
		throw windows_exception{};
	}

	/////////////////////////////////////////////////////////
	// sends the specified request to the HTTP server
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

	////////////////////////////////////////////////////
	// completes a WebSocket handshake started by WinHttpSendRequest
	HINTERNET hWebSock = WinHttpWebSocketCompleteUpgrade(request.get(), 0);

	//winhttp gWebSocket = unique_handle<winhttp_traits>();
	gWebSocket = winhttp(hWebSock);
	if (!gWebSocket) throw windows_exception{};

	WINHTTP_WEB_SOCKET_BUFFER_TYPE winhttpType;
	DWORD dwBytesRead;
	long result = WinHttpWebSocketReceive(gWebSocket.get(),
		szBuffer,
		sizeof(szBuffer),
		&dwBytesRead,
		&winhttpType);

	if (result != ERROR_SUCCESS)
		return DATA_PROCESSED_UNSUCCESSFULLY;


	// The server sent a close frame.
	if (WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE == winhttpType)
	{
		WinHttpWebSocketClose(gWebSocket.get(),
			WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS,
			nullptr,
			0);
		return DATA_PROCESSED_UNSUCCESSFULLY;
	}

	auto message = string{ szBuffer, dwBytesRead };

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
				return DATA_PROCESSED_SUCCESSFULLY;	/////// return success!!!
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

	return DATA_PROCESSED_UNSUCCESSFULLY;
}
//
int main()
{
	/*----------------------------*/
	/*Purpose : To create connection
	assign candlegroup

	do following till the process is interrupted by user to stop
	take timeNow
	maintain connection (websocket/channels/process "event" json messages)
	receive all data
	send all data
	update candles (process data json messmainages)*/
	/*----------------------------*/

	g_log.OpenLog(".", "IF_MD_BITFINEX.exe");

	//initscr();		//start the curses mode
	//resize_term(60, 240);
	//curs_set(0);	//remove cursor
	int nUserInterrupt = 0;
	time_t timeNow;
	//g_pWsdSendMsgQueue = new dataWebSocketData("");
	//g_pWsdRecvMsgQueue = new dataWebSocketData("");

	int nDeleteMe = 0; // <== temporary, delete me

	// connect & gWebSock 
	while (createWebSocketConnection() != ERROR_SUCCESS)
	{
		Sleep(5000);
	}

	// create channel and send subscription
	createChannels();


	unsigned int dwThreadID;
	_beginthreadex(NULL, 0, &ProcessDataThread, 0, 0, &dwThreadID);

	// connect to SM
	char zMarketSMIP[] = "110.4.89.208";
	int nMarketSMPort = 2244;
	if (!g_tcpClient.Begin(zMarketSMIP, nMarketSMPort, 10))
	{
		printf("connect to Market SM server failed (%s)(%d)\n", zMarketSMIP, nMarketSMPort);
		exit(0);
	}



	while (nUserInterrupt != DATA_USER_EXIT)
	{
		std::time(&timeNow);
		receiveAllData();
		//processMessages();

		/*
		printf("\nReceivedMessageQueue (-1:NotProcessedYet  0:Processed)\n");
		printf("======================================================\n");
		g_pWsdRecvMsgQueue->ShowAllMessages(g_pWsdRecvMsgQueue);
		printf("\nSendMessageQueue (-1:NotProcessedYet  0:Processed)\n");
		printf("==================================================\n");
		g_pWsdSendMsgQueue->ShowAllMessages(g_pWsdSendMsgQueue);
		*/
		//g_ticker.DisplayTickerAtCoords(0, 0);
		//g_bookGroups.DisplayBidBookAtCoords(0, 2, -1);
		//g_bookGroups.DisplayAskBookAtCoords(30, 2, -1);

		//g_WsdRecvMsgQueue.ShowAllMessages(0, 35);

		//refresh();
		//clear();

		/*---temporary maintain connection code for test---*/
		mvprintw(1, 0, "Time to live : %d , now %d", DATA_MAX_TIME_TO_LIVE * 1000, timeNow - g_WsdRecvMsgQueue.GetLastUpdateTime());
		if (timeNow - g_WsdRecvMsgQueue.GetLastUpdateTime() > (DATA_MAX_TIME_TO_LIVE * 1000))
			createChannels();
		/*---temporary maintain connection code for test---*/
	}
	return 0;
}


unsigned int __stdcall ProcessDataThread(LPVOID lp)
{
	while (TRUE)
	{
		processMessages();
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
	g_bookGroups.SetChannelString("{\r\n  \"event\": \"subscribe\",\r\n  \"channel\": \"book\",\r\n  \"symbol\": \"tBTCUSD\",\r\n  \"prec\": \"P0\",\r\n  \"freq\": \"F0\",\r\n  \"len\": 25\r\n}");
	g_ticker.SetChannelString("{\r\n  \"event\": \"subscribe\",\r\n  \"channel\": \"ticker\",\r\n  \"symbol\": \"tBTCUSD\"\r\n}");

	//TODO g_WsdRecvMsgQueue.DeleteAllData();
	//TODO g_WsdSendMsgQueue.DeleteAllData();

	subscribeChannel(g_bookGroups.GetChannelString()); // queue the message to send out to create channel 
	subscribeChannel(g_ticker.GetChannelString()); // queue the message to send out to create channel 
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

	g_log.log(LOGTP_SUCC, "[SEND](%.*s", strlen(sJson), sJson);
	return result; //return 0 if successful 
}

int sendAllData()
{
	/*---------------------------------*/
	/*Purpose : To send all data through the websocket by going through SendMessageQueue with the status -1
	then update to 0*/
	/*---------------------------------*/
	char szOutBuffer[1024];
	string sMessageOut;

	while (TRUE)
	{
		//szMessageOut = g_pWsdSendMsgQueue->GetUnprocessedMessage(g_pWsdSendMsgQueue);
		sMessageOut = g_WsdSendMsgQueue.GetData();
		if (sMessageOut == "")	break;

		sprintf(szOutBuffer, "%s", sMessageOut.c_str());
		sendData(szOutBuffer);		
	}

	return DATA_PROCESSED_SUCCESSFULLY;
}


int receiveAllData()
{
	/*---------------------------------*/
	/*Purpose : To receive all data from the websocket and store in the ReceivedMessageQueue*/
	/*---------------------------------*/

	DWORD dwBytesRead;
	DWORD dwReturn;
	WINHTTP_WEB_SOCKET_BUFFER_TYPE winhttpType;
	while (TRUE)
	{
		//receive until WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE is received.
		dwReturn = WinHttpWebSocketReceive(gWebSocket.get(),
			szBuffer,
			sizeof(szBuffer),
			&dwBytesRead,
			&winhttpType);
		if (ERROR_SUCCESS == dwReturn)
		{
			//add the received message into the ReceivedMessageQueue
			g_log.log(LOGTP_SUCC, "[RECV](%.*s)", dwBytesRead, szBuffer);
			g_WsdRecvMsgQueue.InsertData(string(szBuffer, dwBytesRead));
		}
		else
		{
			switch (dwReturn)
			{
			case ERROR_INVALID_OPERATION:sprintf(g_msg, "WinHttpWebSocketReceive error(ERROR_INVALID_OPERATION)"); break;
			case ERROR_INVALID_PARAMETER:sprintf(g_msg, "WinHttpWebSocketReceive error(ERROR_INVALID_PARAMETER)"); break;
			//case ERROR_INVALID_SERVER_RESPONSE:sprintf(g_msg, "WinHttpWebSocketReceive error(ERROR_INVALID_SERVER_RESPONSE)"); break;
			case ERROR_WINHTTP_OPERATION_CANCELLED:sprintf(g_msg, "WinHttpWebSocketReceive error(ERROR_WINHTTP_OPERATION_CANCELLED)"); break;
			default: sprintf(g_msg, "WinHttpWebSocketReceive error. unknown. return value(%d)", dwReturn); break;
			}
			g_log.log(LOGTP_ERR, "(%s)", g_msg);
		}
		if (winhttpType != WINHTTP_WEB_SOCKET_UTF8_FRAGMENT_BUFFER_TYPE)
			break;
	}
	
	return DATA_PROCESSED_SUCCESSFULLY;
}

int processMessages()
{
	rapidjson::Document jsonData;
	string sMessageIn;

	//dataWebSocketData *pdwsdPtr = g_pWsdRecvMsgQueue;
	while (TRUE)
	{
		//szMessageIn = g_pWsdRecvMsgQueue->GetUnprocessedMessage(g_pWsdRecvMsgQueue);
		sMessageIn = g_WsdRecvMsgQueue.GetData();
		if (sMessageIn == "")	break;

		jsonData.Parse(sMessageIn.c_str());
		if (jsonData.IsObject())
		{
			processEventMessages(jsonData);
		}
		else
		{
			processDataMessages(jsonData);
		}
		
	}
	return DATA_PROCESSED_SUCCESSFULLY;
}


/*
receiving message example
{"event":"subscribed","channel":"ticker","chanId":2,"symbol":"tBTCUSD","pair":"BTCUSD"}

*/
int processEventMessages(rapidjson::Document &jsonData)
{
	//take reference of functions (ping,subscribe,unsubscribe,errorcodes,etc) from https://docs.bitfinex.com/v2/docs/ws-general
	/*---------------------------------*/
	/*Purpose:		Go through Received Message Queue
	Get Event Messages and Error Codes
	Update Message Processed Status
	*/
	/*---------------------------------*/


	if (!jsonData.HasMember("event"))
		return DATA_PROCESSED_UNSUCCESSFULLY;


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
		jsonBookChannel.Parse(g_bookGroups.GetChannelString().c_str());
		jsonTickerChannel.Parse(g_ticker.GetChannelString().c_str());	// ChannelString of g_ticker is written manually.

		//if channel is book
		if (strcmp("book", jsonData["channel"].GetString()) == 0)
		{
			//if candlegroup contains the key subscribed
			if (strcmp(jsonBookChannel["symbol"].GetString(), jsonData["symbol"].GetString()) == 0)
			{
				//assign id to the candlegroup
				g_bookGroups.SetChannelID(jsonData["chanId"].GetInt());
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
				g_ticker.SetChannelID(jsonData["chanId"].GetInt());
				return DATA_PROCESSED_SUCCESSFULLY;
			}
		}
	}
	else if (strcmp(jsonData["event"].GetString(), "error") == 0) //if the event has the value of error
	{
		mvprintw(0, 0, "\nError Code : %" PRIu64 "\nError Message : %s\n", jsonData["code"].GetInt64(), jsonData["msg"].GetString()); //log error
	}

	return DATA_PROCESSED_UNSUCCESSFULLY;
}


int processDataBook(rapidjson::Document &jsonData)
{
	BOOL bSend = FALSE;

	if (jsonData[1].IsArray()) // if it is data ...
	{
		//if the 2nd element is an array of arrays i.e. 3d array 
		if (jsonData[1][0].IsArray())
		{
			for (unsigned int nJsonCounter = 0; nJsonCounter < jsonData[1].Size(); nJsonCounter++)
			{
				//add item into book
				/*only for cryptocurrencies*/
				g_bookGroups.AddOrUpdateBook(
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
			bSend = TRUE;
		}
		else // if the 2nd element is not an array of data i.e. 2d array
		{
			/*only for cryptocurrencies*/
			g_bookGroups.AddOrUpdateBook(
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
			bSend = TRUE;
		}

		if (bSend)
		{
			/*			
			typedef struct _BOOK_DATA
			{
				char	symbol[9];
				double	sellprc[5];
				double	sellqty[5];
				int		sellcnt[5];
				double	selltotqty;
				int		selltotcnt;
				double	buyprc[5];
				double	buyqty[5];
				int		buycnt[5];
				double	buytotqty;
				int		buytotcnt;
				int		time;
			}BOOK_DATA;
			*/
			SYSTEMTIME st; char temp[32];

			BOOK_DATA bookSend;
			ZeroMemory(&bookSend, sizeof(bookSend));
			strcpy(bookSend.symbol, SYMBOL_BTCKRW);

			g_bookGroups.GetAskBooks(5, bookSend.sellprc, bookSend.sellqty, bookSend.sellcnt, &bookSend.selltotqty, &bookSend.selltotcnt);
			g_bookGroups.GetBidBooks(5, bookSend.buyprc, bookSend.buyqty, bookSend.buycnt, &bookSend.buytotqty, &bookSend.buytotcnt);
			
			GetLocalTime(&st);
			sprintf(temp, "%02d%02d%02d", st.wHour, st.wMinute, st.wSecond);
			bookSend.time = atoi(temp);

			char *pSendBuffer = NULL;
			CSMClient smClient;
			smClient.SetDelieveryMode(Delivery_Push);
			smClient.SetDestination(SISE_GW);
			smClient.SetMsg(TAG_S_MSG, MSG_MKT_FUT_HOGA);
			smClient.SetData(fldFutHoga, (char*)&bookSend, TYPE_BINARY, sizeof(bookSend));

			int nSendSize = 0;
			pSendBuffer = smClient.GetSendData(&nSendSize);
			if (!pSendBuffer)
			{
				printf("Set SM PACKET error:%s\n", smClient.GetErrMsg());
				return DATA_PROCESSED_UNSUCCESSFULLY;
			}
			int nErrCode = 0;
			int nSendLen = g_tcpClient.SendData(pSendBuffer, nSendSize, &nErrCode);
			if (nSendLen < 0) {
				printf("SEND BOOK ERROR(%s)\n", g_tcpClient.GetMsg());
			}
			
			sprintf(g_msg, "[BOOK SEND](%s)/ASK(%f)(%f)(%f)(%f)(%f)/BID(%f)(%f)(%f)(%f)(%f)",
				bookSend.symbol, bookSend.sellprc[0], bookSend.sellprc[1], bookSend.sellprc[2], bookSend.sellprc[3], bookSend.sellprc[4],
				bookSend.buyprc[0], bookSend.buyprc[1], bookSend.buyprc[2], bookSend.buyprc[3], bookSend.buyprc[4]);			
			g_log.log(LOGTP_SUCC, g_msg);
			printf("%s\n", g_msg);

		}

		return DATA_PROCESSED_SUCCESSFULLY;
	}
	else if (strcmp(jsonData[1].GetString(), "hb") == 0)
	{
		//maintain connection as it is a heartbeat message
		//printf("\nBook Heartbeat Message\n");
		return DATA_PROCESSED_SUCCESSFULLY;
	}

	return DATA_PROCESSED_UNSUCCESSFULLY;
}



/*
DATA EXAMPLE
[4,[10346,116.10253822,10347,62.75196956,-1246,-0.1075,10346,70008.40892856,11900,10009]]

4 : CHANNEL NO

*/
int processDataTicker(rapidjson::Document &jsonData)
{
	if (jsonData[1].IsArray()) // if it is data ...
	{
		/*only for cryptocurrencies*/
		/*
		tickerGroupData->SetTickerData(
		jsonData[1][0].GetFloat(),		// double dBid,
			jsonData[1][1].GetFloat(),	// double dBidSize,
			jsonData[1][2].GetFloat(),	// double dAsk,
			jsonData[1][3].GetFloat(),	// double dAskSize,
			jsonData[1][4].GetFloat(),	// double dDailyChange,
			jsonData[1][5].GetFloat(),	// double dDailyChangePercentage,
			jsonData[1][6].GetFloat(),	// double dLastPrice,
			jsonData[1][7].GetFloat(),	// double dDailyVolume,
			jsonData[1][8].GetFloat(),	// double dDailyHigh,
			jsonData[1][9].GetFloat());	// double dDailyLow
		*/

		SYSTEMTIME st; char temp[32]; 

		TICK_DATA tick;
		ZeroMemory(&tick, sizeof(tick));
		strcpy(tick.symbol, SYMBOL_BTCKRW);
		tick.gap = jsonData[1][4].GetFloat();
		tick.tick = jsonData[1][6].GetFloat();

		if (g_dOpen[IDX_BTC] < 0)
			g_dOpen[IDX_BTC] = tick.tick;
		
		tick.open = g_dOpen[IDX_BTC];
		tick.high = jsonData[1][8].GetFloat();
		tick.low = jsonData[1][9].GetFloat();
		tick.accmlqty = jsonData[1][7].GetFloat();
		tick.accmlamt = 0;

		GetLocalTime(&st);
		sprintf(temp, "%02d%02d%02d", st.wHour, st.wMinute, st.wSecond);
		tick.time = atoi(temp);
		tick.side[0] = 0x00;	//TODO. HOW?

		tick.diffsign[0] = (tick.gap < 0) ? '5' : '2';	// 5 : 하락 2 : 상승

		sprintf(temp, "%.2f", jsonData[1][5].GetFloat());
		strcpy(tick.chgrate, temp);

		/////////////////////////////////////////////////////////////////////////
		// SM 서버에게 전송
		char *pSendBuffer = NULL;
		CSMClient smClient;
		smClient.SetDelieveryMode(Delivery_Push);
		smClient.SetDestination(SISE_GW);
		smClient.SetMsg(TAG_S_MSG, MSG_MKT_FUT_EXEC);
		smClient.SetData(fldFutExec, (char*)&tick, TYPE_BINARY, sizeof(tick));
		
		int nSendSize = 0;
		pSendBuffer = smClient.GetSendData(&nSendSize);
		if (!pSendBuffer)
		{
			printf("Set SM PACKET error:%s\n", smClient.GetErrMsg());
			return DATA_PROCESSED_UNSUCCESSFULLY;
		}

		int nErrCode = 0;
		int nSendLen = g_tcpClient.SendData(pSendBuffer, nSendSize, &nErrCode);
		if (nSendLen < 0) {
			printf("SEND ERROR(%s)\n", g_tcpClient.GetMsg());
		}
		
		SHM_HEAD *pH = (SHM_HEAD*)pSendBuffer;
		int nHead = sizeof(SHM_HEAD);
		int nDestPos = sizeof(WORD);
		int nDestLen = strlen(SISE_GW);
		int nDestStart = nHead + nDestPos;
		int nMsgKeyStart = nDestStart + nDestLen + 2;
		int nMsgDataStart = nMsgKeyStart + strlen(TAG_S_MSG) + 2;
		/*sprintf(g_msg, "[TotalLen:%d][SentLen:%d]TR(%c)DataLen(%d), DEST(%.*s), MSGKEY(%.*s) MSG(%.*s)", 
				nSendSize, nSendLen,
				pH->TR, pH->DataLen, 
				nDestLen, pSendBuffer + nDestStart, strlen(TAG_S_MSG), pSendBuffer + nMsgKeyStart, strlen(MSG_MKT_FX_EXEC), pSendBuffer + nMsgDataStart);*/
		sprintf(g_msg, "[TICK](SYMBOL:%s)(전일대비:%f)(시가:%f)(고가:%f)(저가:%f)(누적거래량:%f)",
			tick.symbol, tick.gap, tick.open,tick.high, tick.low, tick.accmlqty
		);
		g_log.log(LOGTP_SUCC, g_msg);
		printf("%s\n", g_msg);

		/*for currencies , there would be bid period (days,integer) and ask period (days,integer)*/
		return DATA_PROCESSED_SUCCESSFULLY;
	}
	else if (strcmp(jsonData[1].GetString(), "hb") == 0)
	{
		//maintain connection as it is a heartbeat message
		//printf("\nTicker Heartbeat Message\n");
		return DATA_PROCESSED_SUCCESSFULLY;
	}

	return DATA_PROCESSED_UNSUCCESSFULLY;
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

	__int64 nChannel = jsonData[0].GetInt64();
	__int64 nBookChannel = g_bookGroups.GetChannelID();
	__int64 nTickerChannel = g_ticker.GetChannelID();

	if (nChannel == nBookChannel) //if the first element of the data received has the candles channelId
		return processDataBook(jsonData);
	else if (nChannel == nTickerChannel) //if the first element of the data received has the candles channelId
		//return processDataTickerOld(jsonData, &g_ticker);
		return processDataTicker(jsonData);
	
	return DATA_PROCESSED_UNSUCCESSFULLY;
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

	return DATA_PROCESSED_SUCCESSFULLY;
}

void subscribeChannel(string szChannel)
{
	char sChannelSubscriptionJson[1024] = "";
	sprintf(sChannelSubscriptionJson, "%s", szChannel.c_str());
	//g_pWsdSendMsgQueue = g_pWsdSendMsgQueue->InsertWebSocketData(g_pWsdSendMsgQueue, sChannelSubscriptionJson);
	//g_WsdSendMsgQueue.InsertData(sChannelSubscriptionJson);

	sendData(sChannelSubscriptionJson);
}

void unsubscribeChannel(int nChanId)
{
	char sChannelSubscriptionJson[1024] = "";
	sprintf(sChannelSubscriptionJson, "{\r\n   \"event\": \"unsubscribe\",\r\n   \"chanId\":%d\r\n}\0", nChanId);
	//g_pWsdSendMsgQueue = g_pWsdSendMsgQueue->InsertWebSocketData(g_pWsdSendMsgQueue, sChannelSubscriptionJson);
	//g_WsdSendMsgQueue.InsertData(sChannelSubscriptionJson);

	sendData(sChannelSubscriptionJson);
}

