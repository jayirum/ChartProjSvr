#pragma once
#include "../../IRUM_INC/IRUM_Common.h"
/*
	���� : MAX 50��
	��Ʈ���� : 5�� (1��,5��,10��,30��,60��)
	==> ����� ��Ʈ 5�� = 50* 5 = 250	==> GROUP_CNT

	��Ʈ�� MAX : 1440 �� (1����Ʈ ���)	==> STRUCT_CNT
	==> 250 * 1440 * 45(sizeof(struct)) 
	GROUP KEY : 
	���� - GROUP KEY : 50��
		��Ʈ - STRUCT KEY : 

	GROUP KEY : CLK71
	STRUCT KEY : 1��, 5��, 10��, 30��, 60�� (01MIN, 05MIN, 10MIN, 30MIN, 60MIN_

*/


#include "Define\ESApiLayout.h"

//#define	LEN_SYMBOL	4
#define LEN_PRC		20
#define LEN_SEQNO	10
//#define	LEN_SHM_GROUP_KEY	5	//CLK7+1  // GROUP KEY (����+charttp)
#define	LEN_GROUP_KEY	LEN_SHM_GROUP_KEY	//CLK7+1  // GROUP KEY (����+charttp)
//#define	LEN_CHART_NM		8	// mmddhhmm
#define	LEN_SHM_STRUCT_KEY	LEN_CHART_NM	// // STRUCT KEY (��Ʈ�̸�) 0000, 0005, 0010
#define LEN_ACNT_NO	11
#define LEN_ORD_NO	5
#define LEN_ORD_QTY	10
#define LEN_INDICHART_NM	12	//yyyymmddhhmm


//enum CHART_TP { TP_1MIN = 1, TP_5MIN, TP_10MIN, TP_30MIN, TP_60MIN };
enum CHART_IDX { IDX_1MIN = 0, IDX_5MIN, IDX_10MIN, IDX_30MIN, IDX_60MIN };
//#define CHART_TP_CNT 5

#define DEF_CHECK_CHART_CNT	9

//#define DEF_ASSIST_CHECK_SEC	90	// 90��. 1�� 30��
#define DEF_CHECK_STRAT_SEC		90	// 90��. 1�� 30��



//#define DEF_CANDLE_PLUS		'+'
//#define DEF_CANDLE_MINUS	'-'

#define DEF_RSLT_NEW	'1'	//�ű��ֹ�����
#define DEF_RSLT_MATCH	'2'	//ü��
#define DEF_RSLT_MDFY	'3'	//����
#define DEF_RSLT_CNCL	'4'	//���
#define DEF_RSLT_RJCT_NEW	'5'
#define DEF_RSLT_RJCT_MDFY	'6'
#define DEF_RSLT_RJCT_CNCL	'5'

#define	ORD_TYPE_NEW	'1'
#define	ORD_TYPE_MDFY	'2'
#define	ORD_TYPE_CNCL	'3'

//enum STOPORD_TP {STOPORD_OPEN =1, STOPORD_ASSIST_BUY, STOPORD_ASSIST_SELL, STOPORD_AFTERSIGNAL,STOPORD_OPENHILO };	// 1=���ԵǴ� ��� STOP, 2=������ǥ ���� STOP

#define NOTIFY_ORD_TP_NEW		'1'
#define NOTIFY_ORD_TP_ALLCNCL	'2'
#define NOTIFY_ORD_TP_ALLCLR	'3'
#define NOTIFY_PRC_TP_LIMIT		'1'
#define NOTIFY_PRC_TP_MARKET	'2'




#define DEFINE_NO_CHART	"NONE"
/*
0=10������ �ð� ��ġ
1=���� �� ���԰��� �ر��� û��
2=10���� �� ������ 1ƽ �Ʒ� ����
3=10ƽ ���ͽ� ��Ģ û��
*/
enum STRATEGY_TP { STRATEGY_OPEN = 0, /*STRATEGY_UNDEROPEN, STRATEGY_LOW,  STRATEGY_PF*/ };
#define STRATEGY_CNT	1	//4
#define STRAEGY_DONE	'Y'	// �ش� ������ �����ߴ��� ����


// ���ũ�ν� (�ܱ�>���), ����ũ�ν�(�ܱ�<���)
//enum CROSS_TP { NONE_CROSS=0, GOLDEN_CROSS, DEAD_CROSS};

//#define MAX_SYMBOL_CNT		50
//#define MAX_CHART_KIND		5	// 1,5,10,30,60
//#define MAX_CHART_UNIT_CNT	1440	// 1����Ʈ ���

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
//// ������ �۾�
///*
//"1)�ν꽺 ���� �߰�
//2)�ν꽺 ������� ����
//3)�ν꽺 ������� ����
//4)�ν꽺 �������� ����
//5)�ν꽺 ���� ����
//6)FB ���� �߰�
//7)FB �ڵ��ֹ����� ����
//8)FB �ڵ��ֹ����� ����
//9)FB ���� ����
//10) FBƯ������ ��������
//11) FBƯ������ ��������
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


#if 0  //////////////// ���� ���� ///////////////////////////
/*
	10:25	=> 25 / 5 = 5. (5+1)*5 = 30�к�
	10:29   => 29 / 5 = 5. (5+1)*5 = 30�к�
	10:32	=> 32 / 5 = 6. (6+1)*5 = 35�к�

	10:20:00 => 20 / 1 = 20 (20+1)* 1 = 21�к�
	10:20:59 => 20 / 1 = 20 (20+1)* 1 = 21�к�
	10:21:05 => 21 / 1 = 21 (21+1)* 1 = 22�к�
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
10:25	=> 25 / 5 = 5. (5)*5 = 25 �к�
10:29   => 29 / 5 = 5. (5)*5 = 25 �к�
10:32	=> 32 / 5 = 6. (6)*5 = 30 �к�

10:20:00 => 20 / 1 = 20 (20)* 1 = 20�к�
10:20:59 => 20 / 1 = 20 (20)* 1 = 20�к�
10:21:05 => 21 / 1 = 21 (21)* 1 = 21�к�
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

#define	SMA_SHORT_CNT	10	// �ܱ� SMA ����
#define	SMA_LONG_CNT	20	// ��� SMA ����
//
//typedef struct _ST_SHM_CHART_UNIT
//{
//	char	Nm[LEN_CHART_NM];		//8,8 KEY. hhmm		0000, 0005, 0010
//	char	prevNm[LEN_CHART_NM];	//8,16 ����KEY
//	char	gb[1];					//4,17 +:���, -:����
//	char	open[LEN_PRC];			//20,35
//	char	high[LEN_PRC];			//20,55
//	char	low[LEN_PRC];			//20,75
//	char	close[LEN_PRC];			//20,95
//	char	dotcnt[5];				//5,100
//	char	seq[LEN_SEQNO];			//10,110 �ð������Ѵ�.
//	char	sma_short[LEN_PRC];		//20,130 �ܱ� SMA(Simple Moving Average). 10
//	char	sma_long[LEN_PRC];		//20,150 ��� SMA(Simple Moving Average). 20
//	char	signalGb[1];			//0:��ȣ����, 2:�ŵ���ȣ, 1:�ż���ȣ
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
	��ǥ�� ���� ��Ʈ
*/
//typedef struct _ST_SHM_INDICATOR
//{
//	char	IndiNm[LEN_INDICHART_NM];	// KEY. yyyymmddhhmm		0000, 0005, 0010
//	char	prevNm[LEN_INDICHART_NM];	// ����KEY
//	char	sma_short[LEN_PRC];		// �ܱ� SMA(Simple Moving Average). 10
//	char	sma_long[LEN_PRC];		// ��� SMA(Simple Moving Average). 20
//	char	dotcnt[5];
//}ST_SHM_INDICATOR;	



// kr �ü� ��Ŷ�� short code �߰�
//typedef struct _At0933O_st_Ex
//{
//	At0933O_st	org;
//	char		shortCode[LEN_SYMBOL];	// 6EM7 ����
//	long		seq;
//}At0933O_st_Ex;

// �ֹ��������� ������ ��Ŷ
//typedef struct _ST_PACK2ORD
//{
//	char	Date[8];
//	char	Time[8];
//	char	shortCode[LEN_SYMBOL];
//	char	close[LEN_PRC];
//	char	seq[LEN_SEQNO];
//}ST_PACK2ORD;

//// ������ �۾� �뺸
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
//// ������ �۾��뺸 ���� ����
//typedef struct _ST_MANAGER_SOCKNOTIFY
//{
//	char AcntNo[11];
//	char Symbol[5];
//	char OrdTp[1];	// 1:NEW, 2:��ü���, 3:��üû��
//	char PrcTp[1];	// 1:������, 2:���尡
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