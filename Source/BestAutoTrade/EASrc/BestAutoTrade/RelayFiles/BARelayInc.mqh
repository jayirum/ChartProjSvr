//+------------------------------------------------------------------+
//|                                                   BARelayInc.mqh |
//|                              Copyright 2019, BestAutoTrade Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, BestAutoTrade Corp."
#property link      "https://www.mql5.com"
#property strict


#ifndef _BARELAY_INC
#define _BARELAY_INC



#include "NanoLibDef.mqh"


#import "nanomsg.dll"
    int nn_socket (int domain, int protocol);
    int nn_shutdown (int s, int how);
    int nn_close(int s);
    int nn_setsockopt (int s, int level, int option, int &optval, int optvallen);
    int nn_bind (int s, char & addr[] );
    int nn_connect (int s, char & addr[]);
    int nn_send (int s, char & buf[], int len, int flags);
    int nn_recv (int s, char & buf[], int len, int flags);
    int nn_errno ();
#import

// MQL4 CONSTANTS
//enum {OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP}


enum QUEUE_TP   { TP_INPROC,    TP_INTERPROC };
enum NANOMSG_TP { TP_PIPE,      READER,     TP_PIPE_WRITER, TP_PUB, TP_SUB };
enum COPY_TP    { TP_NONE,      TP_MASTER,  TP_SLAVE};
enum BA_RESULT  { Q_ERROR = -1, Q_TIMEOUT,  Q_SUCCESS  };
enum PUBSCOPE_TP {
		ALLSLAVES_UNDER_ONEMASTER,	//	All slaves under one master id
		ONESLAVE_WITH_ID,			//	one Specific Slave
		ALLMASTERS,					//	All Masters
		ONEMASTER_WITH_ID,			//	one specific master
		ALL
	};


const string    RELAY_CHANNEL_NM    = "ipc://BA_RELAY";   //local
const int       SEND_TIMEOUT_MS     = 100;

#define BUF_LEN 512





#endif //_BARELAY_INC