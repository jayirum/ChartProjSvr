#pragma once


#define LOG1_ORD_SND	"[�ֹ�����]"
#define LOG1_ORD_RECV	"[�ֹ�����]"
#define LOG1_CNTR_RECV	"[ü�����]"
#define LOG1_STRAT_ORD	"[�����ֹ�]"
#define LOG1_STRAT_RQMNT	"[�������]"
#define LOG1_STRAT_RELEASE	"[��������]"
#define LOG1_ADMIN			"[��������]"
#define LOG1_ETC			"[��Ÿ   ]"

#define LOG2_LIMIT_SEND		"[����_����]"
#define LOG2_STOP_SET		"[STOP����]"
#define LOG2_STOP_FIRE		"[STOP�ߵ�]"
#define LOG2_STOP_CNCL		"[STOP���]"
#define LOG2_STOP_RJCT		"[STOP�ź�]"
#define LOG2_CNCL_SEND		"[����_���]"
#define LOG2_CNCL_RJCT		"[���_�ź�]"
#define LOG2_REFER			"[����_����]"
#define LOG2_STRAT			"[����_����]"
#define LOG2_STRAT_RELEASE	"[����_����]"
#define LOG2_ORD_API		"[API_����]"
#define LOG2_ORD_API_R		"[API_����]"
#define LOG2_ADMIN_STRAT	"[����_����]"
#define LOG2_ADMIN_SYMBOL	"[����_����]"
#define LOG2_ADMIN_CROSS		"[CROSS����]"
#define LOG2_ADMIN_STRAT_TICK	"[����ƽ����]"
#define LOG2_ADMIN_CLEARALL	"[����_û��]"
#define LOG2_ADMIN_CNCLALL	"[����_���]"
#define LOG2_ADMIN_NEWORD	"[�ű�_�ֹ�]"
#define LOG2_ERROR			"[����_�߻�]"
#define LOG2_ETC			"[��Ÿ    ]"


#define INNER_CHANNEL_PUBSUB	"IRUMSTRAT_FB"

#define FB_STRAT_CNT	6
enum {FBSTRAT_OPEN, FBSTRAT_ASSIST, FBSTRAT_MACROSS, FBSTRAT_SIDEWALKCROSS, FBSTRAT_COMPOSE_1, FBSTRAT_COMPOSE_2};

// STRATEGY RESULT STRUTURE
typedef struct _STRAT_RESULT
{
	char zCompCd[10];	//	company code. IRUM_Common.h
	char zStratCd[10];	
	char cDoSomethingYN[1];
	char SellOrdActCd[1];	//	�ű�, ����, ���
	char SellOrdTp[2];		//	������, ���尡, stop...
	char zSellQty[10];
	char zSellPrc[20];
	char BuyOrdActCd[1];	//	�ű�, ����, ���
	char BuyOrdTp[2];		//	������, ���尡, stop...
	char zBuyQty[10];
	char zBuyPrc[20];

}STRAT_RESULT;


// FB STRATEGY COMMON FACTOR
typedef struct _STRAT_COMMON
{
	char zCompCode[20];
	int	nProfitCutTick;
	int nLossCutTick;
	int nTotProfitLimit;
	int nTotLossLimit;
}STRAT_COMMON;