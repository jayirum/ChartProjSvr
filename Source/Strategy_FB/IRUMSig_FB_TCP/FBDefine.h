#pragma once


#define LOG1_ORD_SND	"[주문전송]"
#define LOG1_ORD_RECV	"[주문수신]"
#define LOG1_CNTR_RECV	"[체결수신]"
#define LOG1_STRAT_ORD	"[전략주문]"
#define LOG1_STRAT_RQMNT	"[전략요건]"
#define LOG1_STRAT_RELEASE	"[전략해제]"
#define LOG1_ADMIN			"[관리설정]"
#define LOG1_ETC			"[기타   ]"

#define LOG2_LIMIT_SEND		"[지정_전송]"
#define LOG2_STOP_SET		"[STOP설정]"
#define LOG2_STOP_FIRE		"[STOP발동]"
#define LOG2_STOP_CNCL		"[STOP취소]"
#define LOG2_STOP_RJCT		"[STOP거부]"
#define LOG2_CNCL_SEND		"[지정_취소]"
#define LOG2_CNCL_RJCT		"[취소_거부]"
#define LOG2_REFER			"[참조_정보]"
#define LOG2_STRAT			"[전략_설정]"
#define LOG2_STRAT_RELEASE	"[전략_해제]"
#define LOG2_ORD_API		"[API_전송]"
#define LOG2_ORD_API_R		"[API_수신]"
#define LOG2_ADMIN_STRAT	"[전략_셋팅]"
#define LOG2_ADMIN_SYMBOL	"[종목_셋팅]"
#define LOG2_ADMIN_CROSS		"[CROSS설정]"
#define LOG2_ADMIN_STRAT_TICK	"[전략틱설정]"
#define LOG2_ADMIN_CLEARALL	"[종목_청산]"
#define LOG2_ADMIN_CNCLALL	"[종목_취소]"
#define LOG2_ADMIN_NEWORD	"[신규_주문]"
#define LOG2_ERROR			"[오류_발생]"
#define LOG2_ETC			"[기타    ]"


#define INNER_CHANNEL_PUBSUB	"IRUMSTRAT_FB"

#define FB_STRAT_CNT	6
enum {FBSTRAT_OPEN, FBSTRAT_ASSIST, FBSTRAT_MACROSS, FBSTRAT_SIDEWALKCROSS, FBSTRAT_COMPOSE_1, FBSTRAT_COMPOSE_2};

// STRATEGY RESULT STRUTURE
typedef struct _STRAT_RESULT
{
	char zCompCd[10];	//	company code. IRUM_Common.h
	char zStratCd[10];	
	char cDoSomethingYN[1];
	char SellOrdActCd[1];	//	신규, 정정, 취소
	char SellOrdTp[2];		//	지정가, 시장가, stop...
	char zSellQty[10];
	char zSellPrc[20];
	char BuyOrdActCd[1];	//	신규, 정정, 취소
	char BuyOrdTp[2];		//	지정가, 시장가, stop...
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