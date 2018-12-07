#pragma once

#pragma warning(disable:4996)
#include "Inc_ETK\OVC.h"
#include "Inc_ETK\OVH.h"

	///////////////////////////////////////////////////////////
	//DEF-LEN
#define	SIZE_SYMBOL	10			//FDAXU17
#define SIZE_PRC		10
#define SIZE_SEQNO	10
#define SIZE_VOLUME			7
#define SIZE_BUFF_SIZE		1024
#define SIZE_USERID			16
#define SIZE_PACKET_CODE		8
#define SIZE_PACKET_LEN		4
#define SIZE_MONEY			12

#define	CD_STX	0x02
#define	CD_ETX	0x03

//DEF-BUY, DEF-SELL
#define CD_BUY	'B'
#define CD_SELL	'S'

#define DEF_PASSWORD "AlGoTrAdInG!@#"
#define CD_PACKET_LOGIN	"LOGIN001"
	// Distinguish the usage between In and Out
#define	CD_PACKET_DATAFEED_CME_TICK_IN	"DF001_01"	// DATAFEED+SEQ+IN
#define	CD_PACKET_DATAFEED_CME_TICK_OUT	"DF001_02"	// DATAFEED+SEQ+OUT
#define	CD_PACKET_DATAFEED_CME_QOUTE_IN	"DF002_01"	// DATAFEED+SEQ+IN
#define	CD_PACKET_DATAFEED_CME_QOUTE_OUT	"DF002_02"	// DATAFEED+SEQ+OUT

#define	CD_PACKET_DATAFEED_ETK_CME_TICK		"DFETK_01"
#define	CD_PACKET_DATAFEED_ETK_CME_QOUTE	"DFETK_02"

#define SET_LEN(len,out){ sprintf(out, "%0*d", SIZE_PACKET_LEN, len);}

	int getDotCnt(char* psSymbol);
	void change_etk_to_normal(char* etk_symbol, char* pOut);

	typedef struct _ALGO_HEADER
	{
		char	Len[SIZE_PACKET_LEN];
		char	Code[SIZE_PACKET_CODE];
		char	GMTTime[15];	//yyyymmdd-hhmmss	GMT+0
	}ALGO_HEADER;


	typedef struct _ALGO_LOGIN
	{
		char	STX[1];
		ALGO_HEADER Header;
		char	Type[2];
		char	UserID[SIZE_USERID];
		char	Pwd[20];
		char	ETX[1];
	}ALGO_LOGIN;
	
	typedef struct _DF_CME_TICK_OUT
	{
		char STX[1];
		ALGO_HEADER Header;
		char Symbol[SIZE_SYMBOL];

		char Tick[SIZE_PRC];
		char Open[SIZE_PRC];
		char High[SIZE_PRC];
		char Low[SIZE_PRC];
		char Vol[SIZE_VOLUME];
		char daebi_sign[1];
		char chg[SIZE_PRC];
		char chg_rate[SIZE_PRC];
		char TradeTM[6];	//hhmmss
		char ETX[1];
	}DF_CME_TICK_OUT;

	typedef struct _QUOTE_SET
	{
		char p1[SIZE_PRC];
		char p2[SIZE_PRC];
		char p3[SIZE_PRC];
		char p4[SIZE_PRC];
		char p5[SIZE_PRC];
		char v1[SIZE_VOLUME];
		char v2[SIZE_VOLUME];
		char v3[SIZE_VOLUME];
		char v4[SIZE_VOLUME];
		char v5[SIZE_VOLUME];
		char v_sum[SIZE_VOLUME];
		char c1[5];
		char c2[5];
		char c3[5];
		char c4[5];
		char c5[5];
		char c_sum[5];
	}QUOTE_SET;
	typedef struct _DF_CME_QUOTE_OUT
	{
		char STX[1];
		ALGO_HEADER Header;
		char Symbol[SIZE_SYMBOL];
		QUOTE_SET	bid, ask;
		char TradeTM[6];	//hhmmss
		char ETX[1];
	}DF_CME_QUOTE_OUT;

	typedef struct _ETK_CME_QUOTE
	{
		char STX[1];
		ALGO_HEADER Header;
		OVH_OutBlock etkQuote;
		char ETX[1];
	}ETK_CME_QUOTE;