#pragma once

/*
*/
#include "../Common/BARelayInc.h"


#ifdef BA_RELAY_EXPORTS
#define BA_RELAY extern "C" __declspec(dllexport)
#else 
#define BA_RELAY extern "C" __declspec(dllimport)
enum { Q_ERROR = -1, Q_SUCCESS, Q_TIMEOUT };
#endif

BA_RELAY int Sender_InitChannel(int nSendTimeout);
BA_RELAY int Sender_DeInitChannel();

BA_RELAY int Sender_RegisterAsMaster(char* pzMyAccNo, /*out*/char* pSendBuf, int nBufLen);
BA_RELAY int Sender_UnRegisterAsMaster(char* pzMyAccNo, /*out*/char* pSendBuf, int nBufLen);
BA_RELAY int Sender_SendOrder(
	char* pzMyAccNo
	, char* pzMasterAccNo
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
	, /*out*/char* pSendBuf
	, int nBufLen
);


BA_RELAY int Receiver_InitSlaveChannel(char* pzMasterAccNo, int nSendTimeout);
BA_RELAY int Receiver_DeInitSlaveChannel();

BA_RELAY int Sender_RegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo, /*out*/char* pSendBuf, int nBufLen);
BA_RELAY int Sender_UnRegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo, /*out*/char* pSendBuf, int nBufLen);

BA_RELAY int Sender_SendData(char* pData, int nSendLen);
BA_RELAY int Receiver_RecvData(/*out*/ char* pData, int nBuffLen);

BA_RELAY char* Sender_GetMsg();
BA_RELAY char* Receiver_GetMsg();

BA_RELAY char* Sender_ChannelNm();
BA_RELAY char* Receiver_ChannelNm();
