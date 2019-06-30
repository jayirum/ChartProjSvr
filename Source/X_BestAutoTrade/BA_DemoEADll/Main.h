#pragma once


#define DEF_SEND_TIMEOUT	100
#define DEF_RECV_TIMEOUT	100


typedef int(*TBASender_InitChannel)(int);
typedef int(*TBASender_DeInitChannel)();
typedef int(*TBASender_RegisterAsMaster)(char*, /*out*/char*, int);
typedef int(*TBASender_UnRegisterAsMaster)(char*, /*out*/char*, int);
typedef int(*TBASender_SendOrder)(
	char*	//pzMyAccNo
	, char*		//pzMasterAccNo
	, int		//nOrdNo
	, char*		//pzSymbol
	, char*		//pAction		// O:Open, C:Close
	, char*		//pSide		// B:Buy, S:Sell
	, int		//nDecimalCnt
	, double	//dOrdPrc
	, double	//dSLPrc
	, double	//dPTPrc
	, double	//dLots
	, char*		//psOrdTp		// MK(Market), PD(Pending), SL, TP
	, char*		//psOrdTm
	, double	//dOpendedPrc
	, double	//dOpenedLots
	, char*		//psOpenedTm
	, double	//dProfit
	, double	//dSwap
	, char*		//pSendData/*out*/
	, int
	);

typedef int(*TBAReceiver_InitSlaveChannel)(char*, int);// (char* pzMasterAccNo, int nSendTimeout);
typedef int(*TBAReceiver_DeInitSlaveChannel)();
typedef int(*TBASender_RegisterAsSlave)(char*, char*, char*, int); //(char* pzMyAccNo, char* pzMasterAccNo);
typedef int(*TBASender_UnRegisterAsSlave)(char*, char*, char*, int); //(char* pzMyAccNo, char* pzMasterAccNo);
typedef int(*TBASender_SendData)(char*, int);//(char* pData, int nSendLen);
typedef int(*TBAReceiver_RecvData)(char*, int);//(/*out*/ char* pData, int nBuffLen);
typedef char* (*TBASender_GetMsg)();
typedef char* (*TBAReceiver_GetMsg)();
typedef char* (*TBASender_ChannelNm)();
typedef char* (*TBAReceiver_ChannelNm)();

