#pragma once
#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include <Windows.h>
#include <string>

namespace _BARELAYEA
{
	enum QUEUE_TP { TP_INPROC, TP_INTERPROC };
	enum NANOMSG_TP { TP_PIPE_READER, TP_PIPE_WRITER, TP_PUB, TP_SUB };

	enum { Q_ERROR = -1, Q_SUCCESS, Q_TIMEOUT };

	
	const int LEN_CHANNEL_NM	= 30;
	const int LEN_ACC_NO		= 20;
	const int LEN_ORD_NO		= 10;
	const int LEN_SYMBOL		= 10;
	const int LEN_PRC			= 10;
	const int LEN_LOTS			= 7;
	const int LEN_ORD_TP		= 2;
	const int LEN_MONEY			= 9;
	const int VOL_DECIMAL_CNT	= 2;

	const char SYS_MT4[]	= "MT4";
	const char SYS_NINJA[]	= "NINJA";
	const char SYS_ZORRO[]	= "ZORRO";

	
	const char CODE_REG_MASTER[]	= "1001";
	const char CODE_UNREG_MASTER[]	= "1002";
	const char CODE_REG_SLAVE[]		= "1003";
	const char CODE_UNREG_SLAVE[]	= "1004";
	const char CODE_MASTER_ORDER[]	= "1005";
	const char CODE_PUBLISH_ORDER[]	= "1006";

	const char TP_COMMAND = 'C';
	const char TP_ORDER = 'O';


	struct Header
	{
		char Code[4];
		char Type[1];
		char System[5];
		char Broker[5];
		char AccNo[LEN_ACC_NO];
	};

	struct PACK_REG_MASTER
	{
		Header header;
		char Reserved[10];
	};

	struct PACK_UNREG_MASTER
	{
		Header header;
		char Reserved[10];
	};

	struct PACK_REG_SLAVE
	{
		Header header;
		char MasterAccNo[LEN_ACC_NO];
		char Reserved[10];
	};

	struct PACK_UNREG_SLAVE
	{
		Header header;
		char MasterAccNo[LEN_ACC_NO];
		char Reserved[10];
	};
	
	struct PACK_TEST
	{
		Header header;
		char Data[100];
	};

	struct PACK_MASTER_ORD
	{
		Header header;
		char MasterAccNo[LEN_ACC_NO];
		char OrdNo[LEN_ORD_NO];
		char Symbol[LEN_SYMBOL];
		char Action[1];
		char Side[1];
		char OrdPrc[LEN_PRC];
		char SLPrc[LEN_PRC];
		char PTPrc[LEN_PRC];
		char Lots[LEN_LOTS];
		char OrdTp[LEN_ORD_TP];
		char OrdTm[6];
		char OpendedPrc[LEN_PRC];
		char OpenedLots[LEN_PRC];
		char OpenedTm[6];
		char Profit[LEN_MONEY];
		char Swap[LEN_MONEY];
	};
}