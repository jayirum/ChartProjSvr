#pragma once
#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include <Windows.h>
#include <string>

namespace _BLASHPF
{
	//enum QUEUE_TP	{ TP_INPROC, TP_INTERPROC, TP_TCP };
	//enum NANOMSG_TP { TP_PIPE_READER, TP_PIPE_WRITER, TP_PUB, TP_SUB };

	//enum RESULT	{ Q_ERROR = -1, Q_SUCCESS, Q_TIMEOUT };

	enum CLIENT_TP	{ MASTR=0, SLAVE};
	//enum PUBSCOPE_TP {
	//	ALLSLAVES_UNDER_ONEMASTER,	//	All slaves under one master id
	//	ONESLAVE_WITH_ID,			//	one Specific Slave
	//	ALLMASTERS,					//	All Masters
	//	ONEMASTER_WITH_ID,			//	one specific master
	//	ALL
	//};

	const char SYS_MT4[] = "MT4";
	const char SYS_NINJA[] = "NINJA";
	const char SYS_ZORRO[] = "ZORRO";


	const char CODE_REG_MASTER[]	= "1001";
	const char CODE_REG_SLAVE[]		= "1002";
	const char CODE_MASTER_ORDER[]	= "1003";
	const char CODE_PUBLISH_ORDER[]	= "1004";
	const char CODE_PING[]			= "1005";
	const char CODE_LOGOFF[]		= "1006";
	const char CODE_LOGON[]			= "1007";
	const char CODE_PUBLISH_TEST[]	= "9001";

	const char TP_COMMAND	= 'C';
	const char TP_ORDER		= 'O';
	const char TP_REG		= 'R';
	const char TP_UNREG		= 'U';
	
	//#define LEN_CHANNEL_NM	128
	#define BUF_SIZE		512
	//#define LOCAL_RELAY_CHANNEL	"BA_LOCALRELAY"

	//+--------------------------------------------------------------------
	//+	Field
	//+--------------------------------------------------------------------

	#define DEF_DELI    0x01

	#define FDS_CODE			1
	#define FDS_COMMAND			2   //  ORDER, COMMAND
	#define FDS_SYS				3   //  MT4, 
	#define FDS_TM_HEADER		4   //  yyyymmdd_hhmmssmmm
	#define FDS_BROKER			5
	#define FDS_REGUNREG		6   //  R, U
	#define FDS_SYMBOL			7
	#define FDS_ORD_ACTION		8   // O:Open, C:Close, P:Partial
	#define FDS_OPENED_TM		10
	#define FDS_CLOSED_TM		11
	#define FDS_COMMENTS		12
	#define FDS_LIVEDEMO		13
	#define FDS_USERID_MINE		14
	#define FDS_USERID_MASTER	15
	#define FDS_LOGONOFF_MSG    16


	#define FDN_ACCNO_MY        100
	#define FDN_ACCNO_MASTER    101
	#define FDN_OPENED_TICKET   103
	#define FDN_CLOSED_TICKET   104
	#define FDN_ORD_TYPE        105 // OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP
	#define FDN_PUBSCOPE_TP		106	
	#define FDN_OPENED_TM       107
	#define FDN_CLOSED_TM       108	

	#define FDD_OPENED_PRC   200
	#define FDD_CLOSED_PRC   201  
	#define FDD_OPENED_LOTS  202
	#define FDD_CLOSED_LOTS  203
	#define FDD_SLPRC        204
	#define FDD_PTPRC        205
	#define FDD_PROFIT       206
	#define FDD_SWAP         207
	#define FDD_COMMISSION   208

	#define MAX_BUF 1024
	#define DOT_CNT 5
	#define MAX_BUF	1024

	
	char* Now(char* pBuf);

}