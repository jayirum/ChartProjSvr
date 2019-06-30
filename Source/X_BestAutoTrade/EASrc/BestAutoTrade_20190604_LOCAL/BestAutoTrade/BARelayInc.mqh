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


enum QUEUE_TP { TP_INPROC, TP_INTERPROC };
enum NANOMSG_TP { TP_PIPE_READER, TP_PIPE_WRITER, TP_PUB, TP_SUB };
enum COPY_TP { TP_NONE, TP_MASTER, TP_SLAVE};

enum { Q_ERROR = -1, Q_TIMEOUT, Q_SUCCESS  };

const string RELAY_CHANNEL_NM = "ipc://BA_RELAY";
const int SEND_TIMEOUT_MS   = 100;

#define BUF_LEN 512


const int LEN_CHANNEL_NM	= 30;
const int LEN_ACC_NO		= 20;
const int LEN_BROKER        = 20;
const int LEN_ORD_NO		= 10;
const int LEN_SYMBOL		= 10;
const int LEN_PRC			= 10;
const int LEN_LOTS			= 7;
const int LEN_ORD_TP		= 2;
const int LEN_MONEY			= 9;
const int VOL_DECIMAL_CNT	= 2;
//
//const string SYS_MT4	= "MT4";
//const string SYS_NINJA	= "NINJA";
//const string SYS_ZORRO	= "ZORRO";
//
//
//const string CODE_REG_MASTER	= "1001";
//const string CODE_REG_SLAVE		= "1002";
//const string CODE_MASTER_ORDER	= "1003";
//const string CODE_PUBLISH_ORDER	= "1004";
//
//const string TP_COMMAND = "C";
//const string TP_ORDER = "O";
//const string TP_REG = "R";
//const string TP_UNREG = "U";

//char* Now(char* pBuf);


struct BAHeader
{
	char Code   [4];    
	char Type   [1];    
	char System [5];  
	char AccNo  [20];
	char Tm     [18];
	char Broker [20];
};//69

struct PT_REG_MASTER
{
	BAHeader header;
	char Action     [1];		// R, U
	char MasterAccNo[20];
	char Reserved   [10];
};

struct PT_REG_SLAVE
{
	BAHeader header;
	char Action     [1];		// R, U
	char MasterAccNo[20];
	char Reserved   [10];
};

struct PT_TEST
{
	BAHeader header;
	char Data[100];
};

struct PT_MASTER_ORD
{
	BAHeader header;
	char MasterAccNo    [20];
	char Ticket         [10];
	char symbol         [10];
	char Action         [1];	// O:Open, C:Close, P:Partial
	char Type           [1];	// OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP
	char OrdPrc         [10];
	char SLPrc          [10];
	char PTPrc          [10];
	char Lots           [7];
	char OrdTp          [2];	// MT, PD, SL, TP
	char OrdTm          [6];
	char OpenedTicket  [10];
	char OpenedPrc     [10];
	char OpenedLots     [7];
	char OpenedTm       [6];
	char Profit         [9];
	char Swap           [9];
	char Comments       [30];
};



/*
PACKET PARSING
struct BAHeader
{
	char Code   [4];    
	char Type   [1];    
	char System [5];  
	char AccNo  [20];
	char Tm     [19];
	char Broker [20];
};//69
struct PT_MASTER_ORD
{
	BAHeader header;
	char MasterAccNo    [20];
	char Ticket         [10];
	char symbol         [10];
	char Action         [1];	// O:Open, C:Close, P:Partial
	char Type           [1];	// OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP
	char OrdPrc         [10];
	char SLPrc          [10];
	char PTPrc          [10];
	char Lots           [7];
	char OrdTp          [2];	// MT, PD, SL, TP
	char OrdTm          [6];
	char OpendedTicket  [10];
	char OpendedPrc     [10];
	char OpenedLots     [10];
	char OpenedTm       [6];
	char Profit         [9];
	char Swap           [9];
};
*/
//bool ParseMasterOrd(bool bHeader, /*in*/string& sPacket, string sField, string& result)
//{
//    if(bHeader)
//        return ParseHeader(sPacket, sField, result);
//    
//    BAHeader        h;
//    PT_MASTER_ORD   p;
//    
//    StringToUpper(sField);
//    if( StringCompare("MASTERACCNO", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                    ,sizeof(h)
//                    ,sizeof(p.MasterAccNo)
//                    );
//    }
//    else if( StringCompare("TICKET", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)
//                  ,sizeof(p.Ticket));
//    }
//    else if( StringCompare("SYMBOL", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)
//                  ,sizeof(p.symbol));
//    }
//    else if( StringCompare("ACTION", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)
//                  ,sizeof(p.Action));
//    }
//    else if( StringCompare("TYPE", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)
//                  ,sizeof(p.Type));
//    }
//    else if( StringCompare("ORDPRC", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)
//                  ,sizeof(p.OrdPrc));
//    }
//    else if( StringCompare("SLPRC", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                  ,sizeof(p.SLPrc));
//    }
//    else if( StringCompare("PTPRC", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                  +sizeof(p.SLPrc)
//                  ,sizeof(p.PTPrc));
//    }
//    else if( StringCompare("LOTS", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                  +sizeof(p.SLPrc)+sizeof(p.PTPrc)
//                  ,sizeof(p.Lots));
//    }
//    else if( StringCompare("ORDTP", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)
//                  ,sizeof(p.OrdTp));
//    }
//    else if( StringCompare("ORDTM", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)
//                  ,sizeof(p.OrdTm));
//    }
//    else if( StringCompare("OPENEDTICKET", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)+sizeof(p.OrdTm)
//                  ,sizeof(p.OpenedTicket));
//    }
//    else if( StringCompare("OPENEDPRC", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)+sizeof(p.OrdTm)+sizeof(p.OpenedTicket)
//                  ,sizeof(p.OpenedPrc));
//    }
//    else if( StringCompare("OPENEDLOTS", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)+sizeof(p.OrdTm)+sizeof(p.OpenedTicket)
//                   +sizeof(p.OpenedPrc)
//                  ,sizeof(p.OpenedLots));
//    }
//    else if( StringCompare("OPENEDTM", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)+sizeof(p.OrdTm)+sizeof(p.OpenedTicket)
//                   +sizeof(p.OpenedPrc)+sizeof(p.OpenedLots)
//                  ,sizeof(p.OpenedTm));
//    }
//    else if( StringCompare("PROFIT", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)+sizeof(p.OrdTm)+sizeof(p.OpenedTicket)
//                   +sizeof(p.OpenedPrc)+sizeof(p.OpenedLots)+sizeof(p.OpenedTm)
//                  ,sizeof(p.Profit));
//    }
//    else if( StringCompare("SWAP", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)+sizeof(p.OrdTm)+sizeof(p.OpenedTicket)
//                   +sizeof(p.OpenedPrc)+sizeof(p.OpenedLots)+sizeof(p.OpenedTm)+sizeof(p.Profit)
//                  ,sizeof(p.Swap));
//    }
//    else if( StringCompare("COMMENTS", sField)==0)
//    {
//        result = StringSubstr(sPacket
//                  ,sizeof(h)+sizeof(p.MasterAccNo)+sizeof(p.Ticket)+sizeof(p.symbol)+sizeof(p.Action)+sizeof(p.Type)+sizeof(p.OrdPrc)
//                   +sizeof(p.SLPrc)+sizeof(p.PTPrc)+sizeof(p.Lots)+sizeof(p.OrdTp)+sizeof(p.OrdTm)+sizeof(p.OpenedTicket)
//                   +sizeof(p.OpenedPrc)+sizeof(p.OpenedLots)+sizeof(p.OpenedTm)+sizeof(p.Profit)+sizeof(p.Swap)
//                  ,sizeof(sizeof(p.Comments)));
//    }
//    
//    string msg;
//    msg = StringFormat("[%s][%s]", sField, result);
//    Alert(msg);
//    
//    return (StringBufferLen(result)>0);
//}
//
//bool ParseHeader(/*in*/string& sPacket, string sField, string& result)
//{
//    result = "";
//    BAHeader h;
//    StringToUpper(sField);
//    if( StringCompare("CODE", sField)==0)
//    {
//        result = StringSubstr(sPacket,0,sizeof(h.Code));
//    }
//    else if( StringCompare("TYPE", sField)==0)
//    {
//        result = StringSubstr(sPacket,sizeof(h.Code),sizeof(h.Type));
//    }
//    else if( StringCompare("SYSTEM", sField)==0)
//    {
//        result = StringSubstr(sPacket,sizeof(h.Code)+sizeof(h.Type), sizeof(h.System));
//    }
//    else if( StringCompare("ACCNO", sField)==0)
//    {
//        result = StringSubstr(sPacket,sizeof(h.Code)+sizeof(h.Type)+sizeof(h.System), sizeof(h.AccNo));
//    }
//    else if( StringCompare("TM", sField)==0)
//    {
//        result = StringSubstr(sPacket,sizeof(h.Code)+sizeof(h.Type)+sizeof(h.System)+sizeof(h.AccNo), sizeof(h.Tm));
//    }
//    else if( StringCompare("BROKER", sField)==0)
//    {
//        result = StringSubstr(sPacket,sizeof(h.Code)+sizeof(h.Type)+sizeof(h.System)+sizeof(h.AccNo)+sizeof(h.Tm), sizeof(h.Broker));
//    }
//    
//    string msg;
//    msg = StringFormat("[%s][%s]", sField, result);
//    Alert(msg);
//    return (StringBufferLen(result)>0);
//}




#endif //_BARELAY_INC