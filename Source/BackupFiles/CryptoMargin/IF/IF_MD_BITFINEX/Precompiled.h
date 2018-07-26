#pragma once
#pragma pack(1)

#include "handle.h"
#include <winhttp.h>
#pragma comment(lib, "winhttp")
#include <string>



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



#define SYMBOL_BTCKRW	"BTCKRW"
#define USDKRW	1063.8


//Todo : change the function name
void Int64ToDate(__int64 n64Milliseconds, char szStringOut[]);
void Time_tToString(time_t timeRawTime, char szStringOut[]);
