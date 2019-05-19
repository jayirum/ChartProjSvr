#pragma once

/*
*/
#include "../Common/BARelayInc.h"


#ifdef BARELAYEZ_EXPORTS
#define BARELAYEZ extern "C" __declspec(dllexport)
#else 
#define BARELAYEZ extern "C" __declspec(dllimport)
enum { Q_ERROR = -1, Q_SUCCESS, Q_TIMEOUT };
#endif

BARELAYEZ int InitSendChannel(int nSendTimeout);
BARELAYEZ int DeInitSendChannel();

BARELAYEZ int RegisterAsMaster(char* pzMyAccNo);
BARELAYEZ int UnRegisterAsMaster(char* pzMyAccNo);
BARELAYEZ int SendOrder(
	char pzMyAccNo
	, char pzMasterAccNo
	, int nOrdNo
	, char* pzSymbol
	, char* pAction		// O:Open, C:Close
	, char* pSide		// B:Buy, S:Sell
	, int nDecimalCnt
	, double dOrdPrc
	, double dSLPrc
	, double dPTPrc
	, double dLots
	, char* psOrdTp		// MK(Market), PD(Pending), SL, TP
	, char* psOrdTm
	, double dOpendedPrc
	, double dOpenedLots
	, char* psOpenedTm
	, double dProfit
	, double dSwap
);


BARELAYEZ int InitSlaveChannel(char* pzMasterAccNo, int nSendTimeout);
BARELAYEZ int DeInitSlaveChannel();
BARELAYEZ int RegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo);
BARELAYEZ int UnRegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo);

BARELAYEZ void GetMsg(/*out*/char* pMsg);
BARELAYEZ int SendData(char* pData, int nSendLen);
