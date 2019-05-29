#pragma once
#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include <Windows.h>
#include <string>

namespace _BA_RELAY
{
	enum QUEUE_TP { TP_INPROC, TP_INTERPROC, TP_TCP };
	enum NANOMSG_TP { TP_PIPE_READER, TP_PIPE_WRITER, TP_PUB, TP_SUB };

	enum { Q_ERROR = -1, Q_SUCCESS, Q_TIMEOUT };

	const char RELAY_CHANNEL_NM[] = "BA_RELAY";
	//const int MAX_PUB_CHNNEL_CNT = 10;

	const int LEN_CHANNEL_NM	= 30;
	const int LEN_ACC_NO		= 20;
	const int LEN_BROKER = 20;
	const int LEN_ORD_NO		= 10;
	const int LEN_SYMBOL		= 10;
	const int LEN_PRC			= 10;
	const int LEN_LOTS			= 7;
	const int LEN_ORD_TP		= 2;
	const int LEN_MONEY			= 9;
	const int VOL_DECIMAL_CNT	= 2;

	//const char SYS_MT4[]	= "MT4";
	//const char SYS_NINJA[]	= "NINJA";
	//const char SYS_ZORRO[]	= "ZORRO";

	//
	//const char CODE_REG_MASTER[]	= "1001";
	//const char CODE_REG_SLAVE[]		= "1002";
	//const char CODE_MASTER_ORDER[]	= "1003";
	//const char CODE_PUBLISH_ORDER[]	= "1004";

	//const char TP_COMMAND = 'C';
	//const char TP_ORDER = 'O';
	//const char TP_REG = 'R';
	//const char TP_UNREG = 'U';

	char* Now(char* pBuf);

	//struct Header
	//{
	//	char Code[4];
	//	char Type[1];
	//	char System[5];
	//	char AccNo[LEN_ACC_NO];
	//	char Tm[18];	//yyyymmdd_hhmmssmmm
	//	char Broker[LEN_BROKER];
	//};

	//struct PT_REG_MASTER
	//{
	//	Header header;
	//	char Action[1];		// R, U
	//	char MasterAccNo[LEN_ACC_NO];
	//	char Reserved[10];
	//};

	//struct PT_REG_SLAVE
	//{
	//	Header header;
	//	char Action[1];		// R, U
	//	char MasterAccNo[LEN_ACC_NO];
	//	char Reserved[10];
	//};

	//struct PT_TEST
	//{
	//	Header header;
	//	char Data[100];
	//};

	//struct PT_MASTER_ORD
	//{
	//	Header header;
	//	char MasterAccNo	[20];
	//	char Ticket			[10];
	//	char symbol			[10];
	//	char Action			[1];	// O:Open, C:Close, P:Partial
	//	char Type			[1];	// OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP
	//	char OrdPrc			[10];
	//	char SLPrc			[10];
	//	char PTPrc			[10];
	//	char Lots			[7];
	//	char OrdTp			[2];	// MT, PD, SL, TP
	//	char OrdTm			[6];
	//	char OpenedTicket	[10];
	//	char OpenedPrc		[10];
	//	char OpenedLots		[7];
	//	char OpenedTm		[6];
	//	char Profit			[9];
	//	char Swap			[9];
	//	char Comments		[30];
	//};
}