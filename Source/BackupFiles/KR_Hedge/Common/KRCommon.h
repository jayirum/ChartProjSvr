#pragma once
#include "../../IRUM_INC/IRUM_Common.h"
/*
	종목 : MAX 50개
	차트종류 : 5개 (1분,5분,10분,30분,60분)
	==> 종목당 차트 5개 = 50* 5 = 250	==> GROUP_CNT

	차트당 MAX : 1440 개 (1분차트 고려)	==> STRUCT_CNT
	==> 250 * 1440 * 45(sizeof(struct)) 
	GROUP KEY : 
	종목 - GROUP KEY : 50개
		차트 - STRUCT KEY : 

	GROUP KEY : CLK71
	STRUCT KEY : 1분, 5분, 10분, 30분, 60분 (01MIN, 05MIN, 10MIN, 30MIN, 60MIN_

*/


#include "Define\ESApiLayout.h"

//#define	LEN_SYMBOL	4
#define LEN_PRC		20
#define LEN_SEQNO	10
//#define	LEN_SHM_GROUP_KEY	5	//CLK7+1  // GROUP KEY (종목+charttp)
#define	LEN_GROUP_KEY	LEN_SHM_GROUP_KEY	//CLK7+1  // GROUP KEY (종목+charttp)
//#define	LEN_CHART_NM		8	// mmddhhmm
#define	LEN_SHM_STRUCT_KEY	LEN_CHART_NM	// // STRUCT KEY (차트이름) 0000, 0005, 0010
#define LEN_ACNT_NO	11
#define LEN_ORD_NO	5
#define LEN_ORD_QTY	10
#define LEN_INDICHART_NM	12	//yyyymmddhhmm


//enum CHART_TP { TP_1MIN = 1, TP_5MIN, TP_10MIN, TP_30MIN, TP_60MIN };
enum CHART_IDX { IDX_1MIN = 0, IDX_5MIN, IDX_10MIN, IDX_30MIN, IDX_60MIN };
//#define CHART_TP_CNT 5

#define DEF_CHECK_CHART_CNT	9

//#define DEF_ASSIST_CHECK_SEC	90	// 90초. 1분 30초
#define DEF_CHECK_STRAT_SEC		90	// 90초. 1분 30초



//#define DEF_CANDLE_PLUS		'+'
//#define DEF_CANDLE_MINUS	'-'

#define DEF_RSLT_NEW	'1'	//신규주문접수
#define DEF_RSLT_MATCH	'2'	//체결
#define DEF_RSLT_MDFY	'3'	//정정
#define DEF_RSLT_CNCL	'4'	//취소
#define DEF_RSLT_RJCT_NEW	'5'
#define DEF_RSLT_RJCT_MDFY	'6'
#define DEF_RSLT_RJCT_CNCL	'5'

#define	ORD_TYPE_NEW	'1'
#define	ORD_TYPE_MDFY	'2'
#define	ORD_TYPE_CNCL	'3'

//enum STOPORD_TP {STOPORD_OPEN =1, STOPORD_ASSIST_BUY, STOPORD_ASSIST_SELL, STOPORD_AFTERSIGNAL,STOPORD_OPENHILO };	// 1=진입되는 경우 STOP, 2=보조지표 설정 STOP

#define NOTIFY_ORD_TP_NEW		'1'
#define NOTIFY_ORD_TP_ALLCNCL	'2'
#define NOTIFY_ORD_TP_ALLCLR	'3'
#define NOTIFY_PRC_TP_LIMIT		'1'
#define NOTIFY_PRC_TP_MARKET	'2'




#define DEFINE_NO_CHART	"NONE"
/*
0=10개봉중 시가 일치
1=진입 후 진입가격 붕괴시 청산
2=10개봉 중 최저가 1틱 아래 손절
3=10틱 수익시 수칙 청산
*/
enum STRATEGY_TP { STRATEGY_OPEN = 0, /*STRATEGY_UNDEROPEN, STRATEGY_LOW,  STRATEGY_PF*/ };
#define STRATEGY_CNT	1	//4
#define STRAEGY_DONE	'Y'	// 해당 전략을 수행했는지 여부


// 골든크로스 (단기>장기), 데드크로스(단기<장기)
//enum CROSS_TP { NONE_CROSS=0, GOLDEN_CROSS, DEAD_CROSS};

//#define MAX_SYMBOL_CNT		50
//#define MAX_CHART_KIND		5	// 1,5,10,30,60
//#define MAX_CHART_UNIT_CNT	1440	// 1분차트 고려

enum SHM_TP { CHART = 0, LASTNM };

//#define GET_GROUP_KEY(symbol, CHART_TP, out) { sprintf(out, "%-*.*s%d", LEN_SYMBOL, LEN_SYMBOL, symbol, CHART_TP) ; }
#define	FORMAT_PRC(prc,dotcnt,out) { sprintf(out, "%0*.*f", LEN_PRC, dotcnt, prc); }
#define FORMAT_SEQNO(seqno,out) { sprintf(out, "%0*ld",LEN_SEQNO, seqno);}


#define WM_CHART_ALL_KILL	WM_USER + 800
#define WM_CHART_DATA		WM_USER + 801
#define WM_MANAGER_NOTIFY	WM_USER + 802
#define WM_MANAGER_NOTIFY_SOCK	WM_USER + 803
#define WM_NOTI_MATCH		WM_USER + 804
#define WM_SAVE_CHART		WM_USER + 805
#define WM_STRAT_LOGGING	WM_USER + 806
#define WM_SAVE_MATCH		WM_USER + 807
#define WM_NOTI_REMAINORD	WM_USER + 808
#define WM_NOTI_POSITION	WM_USER + 809
//
//// 관리자 작업
///*
//"1)로쎄스 계좌 추가
//2)로쎄스 헷지대상 설정
//3)로쎄스 헷지대상 해제
//4)로쎄스 헷지계좌 설정
//5)로쎄스 계좌 삭제
//6)FB 계좌 추가
//7)FB 자동주문종목 설정
//8)FB 자동주문종목 해제
//9)FB 계좌 삭제
//10) FB특정계좌 전략설정
//11) FB특정계좌 전략해제
//*/
//enum MANAGER_JOB {
//	MNG_ACNT_ADD = 1
//	, MNG_ACNT_AUTOORD_SET		//2
//	, MNG_ACNT_AUTOORD_RELEASE	//3
//	, MNG_HEDGE_ACNT_ADD		//4
//	, MNG_ACNT_DEL				//5
//	, MNG_FB_ACNT_ADD			//6
//	, MNG_FB_ORDSYMBOL_ADD		//7
//	, MNG_FB_ORDSYMBOL_RELEASE	//8
//	, MNG_FB_ACNT_DEL			//9
//	, MGN_FB_STRAT_SET			//10
//	, MGN_FB_STRAT_RELEASE		//11
//	, MGN_FB_CROSS_CHECK = 17
//	, MNG_FB_CROSS_UNCHECK = 18
//	, MNG_FB_CLEAR_ALL = 19
//	, MNG_FB_CNCL_ALL = 20
//	, MNG_FB_PROFIT_TICK = 21
//	, MNG_FB_LOSS_TICK = 22
//};


#if 0  //////////////// 기존 로직 ///////////////////////////
/*
	10:25	=> 25 / 5 = 5. (5+1)*5 = 30분봉
	10:29   => 29 / 5 = 5. (5+1)*5 = 30분봉
	10:32	=> 32 / 5 = 6. (6+1)*5 = 35분봉

	10:20:00 => 20 / 1 = 20 (20+1)* 1 = 21분봉
	10:20:59 => 20 / 1 = 20 (20+1)* 1 = 21분봉
	10:21:05 => 21 / 1 = 21 (21+1)* 1 = 22분봉
*/
#define GET_CHART_NM(time,CHART_TP, out){	\
	int divider; char tmp[32];	char zTm[32];\
	sprintf(zTm, "%.8s", time);						\
	if(CHART_TP==TP_1MIN) divider=1;		\
	if(CHART_TP==TP_5MIN) divider=5;		\
	if(CHART_TP==TP_10MIN) divider=10;		\
	if(CHART_TP==TP_30MIN) divider=30;		\
	if(CHART_TP==TP_60MIN) divider=60;		\
	sprintf(tmp, "%.2s", (zTm+3));		\
	int ret = (atoi(tmp) / divider); 		\
	int min = (ret+1)*divider;				\
	if(min!=60) sprintf(out, "%.2s%02d", zTm, min);\
	else{ int h = S2N(zTm,2)+1; if(h==24) h=0; sprintf(out, "%02d00", h);}	\
}
#endif 
///////////////////////////////////////////////////////////////////////////////////////////


/*
10:25	=> 25 / 5 = 5. (5)*5 = 25 분봉
10:29   => 29 / 5 = 5. (5)*5 = 25 분봉
10:32	=> 32 / 5 = 6. (6)*5 = 30 분봉

10:20:00 => 20 / 1 = 20 (20)* 1 = 20분봉
10:20:59 => 20 / 1 = 20 (20)* 1 = 20분봉
10:21:05 => 21 / 1 = 21 (21)* 1 = 21분봉
*/
//#define GET_CHART_NM(time,CHART_TP, out){	\
//	int divider; char tmp[32];	char zTm[32]; SYSTEMTIME systime;\
//	sprintf(zTm, "%.8s", time);						\
//	if(CHART_TP==TP_1MIN) divider=1;		\
//	if(CHART_TP==TP_5MIN) divider=5;		\
//	if(CHART_TP==TP_10MIN) divider=10;		\
//	if(CHART_TP==TP_30MIN) divider=30;		\
//	if(CHART_TP==TP_60MIN) divider=60;		\
//	sprintf(tmp, "%.2s", (zTm+3));		\
//	int ret = (atoi(tmp) / divider); 		\
//	int min = (ret)*divider;				\
//	GetLocalTime(&systime);					\
//	if(min!=60) sprintf(out, "%02d%02d%.2s%02d", systime.wMonth, systime.wDay, zTm, min);\
//	else{ int h = S2N(zTm,2)+1; if(h==24) h=0; sprintf(out, "%02d%02d%02d00", systime.wMonth, systime.wDay,h);}	\
//}






//#define COMP_RCS	"ROCCESS"
//#define COMP_FB		"FB"

#define	SMA_SHORT_CNT	10	// 단기 SMA 갯수
#define	SMA_LONG_CNT	20	// 장기 SMA 갯수
//
//typedef struct _ST_SHM_CHART_UNIT
//{
//	char	Nm[LEN_CHART_NM];		//8,8 KEY. hhmm		0000, 0005, 0010
//	char	prevNm[LEN_CHART_NM];	//8,16 직전KEY
//	char	gb[1];					//4,17 +:양봉, -:음봉
//	char	open[LEN_PRC];			//20,35
//	char	high[LEN_PRC];			//20,55
//	char	low[LEN_PRC];			//20,75
//	char	close[LEN_PRC];			//20,95
//	char	dotcnt[5];				//5,100
//	char	seq[LEN_SEQNO];			//10,110 시간저장한다.
//	char	sma_short[LEN_PRC];		//20,130 단기 SMA(Simple Moving Average). 10
//	char	sma_long[LEN_PRC];		//20,150 장기 SMA(Simple Moving Average). 20
//	char	signalGb[1];			//0:신호없음, 2:매도신호, 1:매수신호
//	char	Reserved[50];			//100,250
//	
//}ST_SHM_CHART_UNIT;	// 45
//#define LEN_CHART_WITHOUT_STRAT	sizeof(ST_SHM_CHART_UNIT) - STRATEGY_CNT

typedef struct _ST_SHM_LASTCHART
{
	char	GroupKey[LEN_SHM_GROUP_KEY];
	char	LastChartNm[LEN_CHART_NM];
}ST_SHM_LASTCHART;


/*
	지표를 위한 차트
*/
//typedef struct _ST_SHM_INDICATOR
//{
//	char	IndiNm[LEN_INDICHART_NM];	// KEY. yyyymmddhhmm		0000, 0005, 0010
//	char	prevNm[LEN_INDICHART_NM];	// 직전KEY
//	char	sma_short[LEN_PRC];		// 단기 SMA(Simple Moving Average). 10
//	char	sma_long[LEN_PRC];		// 장기 SMA(Simple Moving Average). 20
//	char	dotcnt[5];
//}ST_SHM_INDICATOR;	



// kr 시세 패킷에 short code 추가
//typedef struct _At0933O_st_Ex
//{
//	At0933O_st	org;
//	char		shortCode[LEN_SYMBOL];	// 6EM7 형식
//	long		seq;
//}At0933O_st_Ex;

// 주문서버에게 전송할 패킷
//typedef struct _ST_PACK2ORD
//{
//	char	Date[8];
//	char	Time[8];
//	char	shortCode[LEN_SYMBOL];
//	char	close[LEN_PRC];
//	char	seq[LEN_SEQNO];
//}ST_PACK2ORD;

//// 관리자 작업 통보
//typedef struct _ST_MANAGER_NOTIFY
//{
//	int nSeqNo;
//	char zAcntNo[LEN_ACNT_NO+1];
//	int nWorkTp;
//	char zWorkVal[64];
//	char zWorkNote[64];
//	char zAdminID[64];
//}ST_MANAGER_NOTIFY;
//
//// 관리자 작업통보 소켓 버퍼
//typedef struct _ST_MANAGER_SOCKNOTIFY
//{
//	char AcntNo[11];
//	char Symbol[5];
//	char OrdTp[1];	// 1:NEW, 2:전체취소, 3:전체청산
//	char PrcTp[1];	// 1:지정가, 2:시장가
//	char Side[1];	// 1:BUY, 2:SELL
//	char Prc[10];
//}ST_MANAGER_SOCKNOTIFY;
//typedef struct _ST_ORD_RESULT
//{
//	char	zSymbol[LEN_SYMBOL + 1];
//	char	cRsltType[1];
//	char	cSide[1];
//	char	zPosPrc[LEN_PRC + 1];
//	char	zOpenTM[9];	//hh:mm:ss
//}ST_ORD_RESULT;

typedef struct _API_OBJ
{
	//CString *omniAcnt;
	//CString *omniPwd;
	void	*api;
	HWND	*hWnd;
	void	*pShm;	// *shm[2];
	void	*pDB;
	void	*csApi;
}API_OBJ;