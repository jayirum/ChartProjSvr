#pragma once

#pragma warning(disable:4996)

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

//DEF-REGISTRY
#define	IRUM_ROOT			"Software\\IRUMSYSTEMS"
#define	IRUM_DIRECTORY		"Software\\IRUMSYSTEMS\\DIRECTORY"


////////////////////////////////////////////////////////////////////
#define IR_BROKER_HD	"HD"
#define IR_BROKER_KR	"KR"

//DEF-COMPANY CODE
#define COMP_FB	"FB"
#define COMP_DEMO "DEMO"

///////////////////////////////////////////////////////////
//DEF-LEN
#define IRLEN_KR_FULLCODE	32
#define	LEN_SYMBOL	7			//FDAXU17
#define LEN_PRC		10
#define LEN_SEQNO	10
#define	LEN_SHM_GROUP_KEY	9	//CLK17+01  // GROUP KEY (����+charttp)
#define	LEN_GROUP_KEY		LEN_SHM_GROUP_KEY	//CLK7+1  // GROUP KEY (����+charttp)
#define	LEN_CHART_NM		12	// yyyymmddhhmm
#define	LEN_SHM_STRUCT_KEY	LEN_CHART_NM	// // STRUCT KEY (��Ʈ�̸�) 0000, 0005, 0010
#define LEN_INDICHART_NM	12	//yyyymmddhhmm
#define LEN_QTY				10
#define LEN_PL				10
#define LEN_ACNT_NO	11
#define LEN_ORD_NO	5
//#define LEN_ORD_QTY	10
#define LEN_INDICHART_NM	12	//yyyymmddhhmm
#define	LEN_STRAT_COMP_ID	10
#define LEN_STRAT_ID		10
#define LEN_BUFF_SIZE		1024
#define LEN_USERID			16
#define LEN_PACKET_CODE		6
#define SIZE_PACKET_LEN		4



//DEF-BUY, DEF-SELL
#define CD_BUY	'B'
#define CD_SELL	'S'

//DEF-OPEN, DEF-CLOSE
#define CD_OPEN		'O'
#define CD_CLOSE	'C'


//DEF-ORD_TP
#define CD_ORD_TP_LIMIT		'1' // ������
#define CD_ORD_TP_MARKET	'2' // ���尡
#define CD_ORD_TP_SL		'3' // ����
#define CD_ORD_TP_PT		'4' // ����
#define CD_ORD_TP_SLPT		'5'	// ����+����
#define CD_ORD_TP_MIT		'6'

#define ORDTP2NAME(cd,out){\
	if(cd==CD_ORD_TP_LIMIT)	sprintf(out,"����");	\
	if(cd==CD_ORD_TP_MARKET)sprintf(out,"����");	\
	if(cd==CD_ORD_TP_SL)	sprintf(out,"����");	\
	if(cd==CD_ORD_TP_PT)	sprintf(out,"����");	\
	if(cd==CD_ORD_TP_SLPT)	sprintf(out,"����");	\
	if(cd==CD_ORD_TP_MIT)	sprintf(out,"����");	\
}


//DEF-ORD_PROC
#define CD_ORD_PROC_NEW		'1'	//�ű�
#define CD_ORD_PROC_MDFY	'2'	//����
#define CD_ORD_PROC_CNCL	'3'	//���
#define CD_ORD_PROC_RJCT	'4'	//�ź�
#define	CD_ORD_PROC_SYMBOL_CNCL	'A'	//�������, 
#define CD_ORD_PROC_SYMBOL_CLR	'B'	//����û��
#define	CD_ORD_PROC_ALL_CNCL	'C'	//���������, 
#define CD_ORD_PROC_ALL_CLR		'D'	//������û��

#define ORDPROC2NAME(cd,out){\
	if(cd==CD_ORD_PROC_NEW)			sprintf(out,"�ű�");	\
	if(cd==CD_ORD_PROC_MDFY)		sprintf(out,"����");	\
	if(cd==CD_ORD_PROC_CNCL)		sprintf(out,"���");	\
	if(cd==CD_ORD_PROC_RJCT)		sprintf(out,"�ź�");	\
	if(cd==CD_ORD_PROC_SYMBOL_CNCL)	sprintf(out,"����");	\
	if(cd==CD_ORD_PROC_SYMBOL_CLR)	sprintf(out,"��û");	\
	if(cd==CD_ORD_PROC_ALL_CNCL)	sprintf(out,"����");	\
	if(cd==CD_ORD_PROC_ALL_CLR)		sprintf(out,"��û");	\
}

//DEF-CANDLE
//#define DEF_CANDLE_PLUS		'1'		//LEGACY
//#define DEF_CANDLE_MINUS	'2'		//LEGACY
#define DEFINE_NO_CHART		"NONE"	//LEGACY
#define CD_PLUS_CANDLE		'+'
#define CD_MINUS_CANDLE		'-'
#define CD_OPENSIG_MINUS	'1'
#define CD_OPENSIG_PLUS		'2'



//DEF-PACKET CODES
#define CD_SIG_OPENPRC		"SS0001"
#define CD_SIG_ASSIST		"SS0002"
#define CD_SIG_MACROSS		"SS0003"
#define CD_SIG_SIDEWAY		"SS0004"
#define CD_SIG_OPEN_ASSIST	"SS0005"
#define CD_SIG_OPEN_ASSIST2	"SS0006"
#define CD_STRAT_ORD		"SO0001"
#define CD_C_ORDER			"SO0001"
#define CD_C_NCLR			"ST0001"
#define CD_C_REMAIN_ORD		"ST0002"

#define DEF_EOL				0x0a	// delphi ipworks �� ����ϱ� ����


enum CHART_TP {
	TP_1MIN = 0,
	TP_3MIN,
	TP_5MIN,
	TP_10MIN,
	TP_15MIN,
	TP_20MIN,
	//TP_30MIN, 
	TP_60MIN,
	TP_120MIN,
	TP_DAY,
	TP_WEEK,
	TP_MON,
	TP_TICK1,
	TP_TICK2,
	TP_TICK5,
	TP_TICK10,
	TP_TICK20,
	TP_TICK30,
	TP_TICK40,
	TP_TICK60
};


//DEF-SHART SHM, DEF-CHARTSHM
#define CHART_TP_CNT		20
#define MAX_CHART_TP_CNT	CHART_TP_CNT
#define MAX_SYMBOL_PER_ARTC	2		//	ǰ��� 2���� ����(����)�� ���ؼ��� �����Ѵ�.
#define MAX_GROUPKEY_CNT	MAX_SYMBOL_PER_ARTC * CHART_TP_CNT
#define MAX_CHART_UNIT_CNT	1000	//	�� GROUP�� (������, ����Ʈ�� 'CLU7 00')
#define MAX_ARTC_CNT		10		//	10���� ǰ�� 


//DEF-CROSS
// ���ũ�ν� (�ܱ�>���), ����ũ�ν�(�ܱ�<���)
enum CROSS_TP { NONE_CROSS = 0, GOLDEN_CROSS, DEAD_CROSS };
#define CROSS2NAME(cross,out){\
	if(cross==GOLDEN_CROSS) strcpy(out,"���");		\
	if (cross == DEAD_CROSS) strcpy(out, "����");	\
	if (cross == NONE_CROSS) strcpy(out, "��ũ");	\
}

//DEF-SMA
#define	SMA_SHORT_CNT	10	// �ܱ� SMA ����
#define	SMA_LONG_CNT	20	// ��� SMA ����

#define GET_GROUP_KEY(symbol, CHART_TP, out) { sprintf(out, "%-*.*s%02d", LEN_SYMBOL, LEN_SYMBOL, symbol, CHART_TP) ; }
#define	FORMAT_PRC(prc,dotcnt,out) { sprintf(out, "%0*.*f", LEN_PRC, dotcnt, prc); } // 000000012.12
#define FORMAT_PRC_STR(prc,len, out){ sprintf(out, "%*.*s", len, len, prc);} // "       12.12"	
#define FORMAT_SEQNO(seqno,out) { sprintf(out, "%0*ld",LEN_SEQNO, seqno);}
#define GET_SHM_NM(artc,out){ sprintf(out, "Global\\%s_SHM", artc);}
#define GET_SHM_LOCK_NM(artc,out){ sprintf(out, "Global\\%s_MUTEX", artc);}
#define GET_ARTC_CODE(symbol,out){ sprintf(out,"%.*s", strlen(symbol) -2 , symbol);}// CLQ7,6EQ7
#define FORMAT_USERID(id,out) { sprintf(out, "%*.*s", LEN_USERID, LEN_USERID, id);}
#define FORMAT_SYMBOL(symbol) { CUtil::TrimAll(symbol, strlen(symbol));}

//DEF_WM CODE
#define WM_CHART_ALL_KILL	WM_USER + 800
#define WM_CHART_DATA		WM_USER + 801
#define WM_MANAGER_NOTIFY	WM_USER + 802
#define WM_MANAGER_NOTIFY_SOCK	WM_USER + 803
#define WM_NOTI_MATCH		WM_USER + 804
#define WM_SAVE_CHART		WM_USER + 805
#define WM_STRAT_LOGGING	WM_USER + 806
#define WM_SAVE_MATCH		WM_USER + 807
#define WM_SEND_STRATEGY	WM_USER + 808	// Send signal to client
#define WM_RECV_CLIENT		WM_USER + 809	// recv data from client - TR, real
#define WM_RECV_API_MD		WM_USER + 810	// recv data from client - TR, real
#define WM_SENDORD_API		WM_USER + 811
#define WM_DIE				WM_USER + 999




/*
10:25	=> 25 / 5 = 5. (5)*5 = 25 �к�
10:29   => 29 / 5 = 5. (5)*5 = 25 �к�
10:32	=> 32 / 5 = 6. (6)*5 = 30 �к�

10:20:00 => 20 / 1 = 20 (20)* 1 = 20�к�
10:20:59 => 20 / 1 = 20 (20)* 1 = 20�к�
10:21:05 => 21 / 1 = 21 (21)* 1 = 21�к�

time -> hh:mm:ss
*/
#define GET_CHART_NM(time,CHART_TP, out){	\
	int divider; char tmp[32];	char zTm[32]; SYSTEMTIME systime;\
	sprintf(zTm, "%.8s", time);						\
	if(CHART_TP==TP_1MIN) divider=1;		\
	if(CHART_TP==TP_3MIN) divider=3;		\
	if(CHART_TP==TP_5MIN) divider=5;		\
	if(CHART_TP==TP_10MIN) divider=10;		\
	if(CHART_TP==TP_15MIN) divider=15;		\
	if(CHART_TP==TP_20MIN) divider=20;		\
	if(CHART_TP==TP_60MIN) divider=60;		\
	if(CHART_TP==TP_120MIN) divider=120;		\
	sprintf(tmp, "%.2s", (zTm+3));		\
	int ret = (atoi(tmp) / divider); 		\
	int min = (ret)*divider;				\
	GetLocalTime(&systime);					\
	if(min==60){ int h = S2N(zTm,2)+1; if(h==24) h=0; sprintf(out, "%04d%02d%02d%02d00",systime.wYear, systime.wMonth, systime.wDay,h);}	\
	else if(min==120){ int h = S2N(zTm,2)+2; if(h==24) h=0; sprintf(out, "%04d%02d%02d%02d00",systime.wYear, systime.wMonth, systime.wDay,h);}	\
	else sprintf(out, "%04d%02d%02d%.2s%02d",systime.wYear, systime.wMonth, systime.wDay, zTm, min);\
}

// date : yyyymmdd, time:hh:mm:ss
// 120�� ��Ʈ�� 6�� ���� �����Ѵ�
#define GET_CHART_NM_EX(date,time, tp, out){	\
	int divider; char tmp[32];	char zTm[32]; \
	sprintf(zTm, "%.8s", time);					\
	if(tp==TP_1MIN) divider=1;		\
	if(tp==TP_3MIN) divider=3;		\
	if(tp==TP_5MIN) divider=5;		\
	if(tp==TP_10MIN) divider=10;		\
	if(tp==TP_15MIN) divider=15;		\
	if(tp==TP_20MIN) divider=20;		\
	if(tp==TP_60MIN) divider=60;		\
	if(tp==TP_120MIN) divider=120;		\
	sprintf(tmp, "%.2s", (zTm+3));		\
	int ret = (atoi(tmp) / divider); 		\
	int min = (ret)*divider;				\
	if (tp == TP_60MIN) {int h = S2N(zTm, 2); if (h == 24) h = 0; sprintf(out, "%.8s%02d00", date, h);	}	\
	else if (tp == TP_120MIN) {	int h = S2N(zTm, 2); int hRemain = h % 2; if (hRemain == 1)	h -= 1; sprintf(out, "%.8s%02d00", date, h);}\
	else {sprintf(out, "%.8s%.2s%02d", date, zTm, min);}\
}

#define GET_TICKCHART_NM(seq,out){ sprintf(out, "%0*d", LEN_CHART_NM, seq);}


/*
SHM - ǰ���ڵ�� (6E)
GROUP_KEY - �����ڵ�+CHART_TP
*/
typedef struct _ST_SHM_CHART_HEADER
{
	char LastChartNm[LEN_CHART_NM];
	char Reserved2[10];
	char Reserved3[10];
	char Reserved4[10];
	char Reserved5[10];
}ST_SHM_CHART_HEADER;

typedef struct _ST_SHM_CHART_UNIT
{
	char	Nm[LEN_CHART_NM];		//8,8 KEY. chart name. hhmm		0000, 0005, 0010
	char	prevNm[LEN_CHART_NM];	//8,16 previous EY
	char	gb[1];					//4,17 +:���(plus candel), -:����(minus candle)
	char	open[LEN_PRC];			//
	char	high[LEN_PRC];			//
	char	low[LEN_PRC];			//
	char	close[LEN_PRC];			//
	char	cntr_qty[LEN_QTY];
	char	dotcnt[5];				//5,100
									//char	seq[LEN_SEQNO];			//10,110 �ð������Ѵ�.
	char	sma_short[LEN_PRC];		//20,130 �ܱ�short term SMA(Simple Moving Average). 10
	char	sma_long[LEN_PRC];		//20,150 ���long term SMA(Simple Moving Average). 20

	char	Reserved[50];
	
}ST_SHM_CHART_UNIT;	// 
					//#define LEN_CHART_WITHOUT_STRAT	sizeof(ST_SHM_CHART_UNIT) - STRATEGY_CNT

typedef struct _ST_SHM_LAST_CHART
{
	char	Nm[LEN_CHART_NM];
	char	Reserved[50];
}ST_SHM_LAST_CHART;

/*
��ǥ�� ���� ��Ʈ
*/
typedef struct _ST_SHM_INDICATOR
{
	char	IndiNm[LEN_INDICHART_NM];	// KEY. yyyymmddhhmm		0000, 0005, 0010
	char	prevNm[LEN_INDICHART_NM];	// ����KEY
	char	sma_short[LEN_PRC];		// �ܱ� SMA(Simple Moving Average). 10
	char	sma_long[LEN_PRC];		// ��� SMA(Simple Moving Average). 20
	char	dotcnt[5];
}ST_SHM_INDICATOR;




// ChartMaker ���� ������ ��Ŷ
//typedef struct _ST_PACK2CHART
//{
//	char	Date[8];
//	char	Time[8];
//	char	ShortCode[LEN_SYMBOL];
//	char	Close[LEN_PRC];
//	char	CntrQty[LEN_QTY];
//	char	DecLen[5];				// �Ҽ��� �ڸ���
//}ST_PACK2CHART;

// ChartMaker ���� ������ ��Ŷ
typedef struct _ST_PACK2CHART_EX
{
	char	STX[1];
	char	Len[SIZE_PACKET_LEN];	// ��ü ����
	char	Date[8];
	char	Time[8];
	char	ShortCode[LEN_SYMBOL];
	char	Close[LEN_PRC];
	char	CntrQty[LEN_QTY];	
	char	DecLen[5];				// �Ҽ��� �ڸ���
	char	ETX[1];
}ST_PACK2CHART_EX;


   
// real-time strategy packet from SERVER to CLIENT
typedef struct _ST_STRAT_REAL_CLIENT
{
	char	Len[SIZE_PACKET_LEN];	// ��ü ����
	char	Symbol[LEN_SYMBOL];
	char	GroupKey[LEN_GROUP_KEY];
	char	ChartNm[LEN_CHART_NM];
	char	StratID[32];
	char	StratPrc[LEN_PRC];
	char	ApiDT[8];
	char	ApiTM[8];	//hh:mm:ss
	char	Note[100];
	char	EOL[1];
	
}ST_STRAT_REAL_CLIENT;


/*
// MUBFISH �����ߵ�. �ֹ���Ŷ

//DEF-BUY, DEF-SELL
#define CD_BUY	'B'
#define CD_SELL	'S'


//DEF-ORD_TP
#define CD_ORD_TP_LIMIT		'1' // ������
#define CD_ORD_TP_MARKET	'2' // ���尡
#define CD_ORD_TP_SL		'3' // ����
#define CD_ORD_TP_PT		'4' // ����
#define CD_ORD_TP_SLPT		'5'	// ����+����
#define CD_ORD_TP_MIT		'6'
*/
typedef struct _ST_MF_STRAT_ORD
{
	char	Symbol[LEN_SYMBOL];
	char	StratID[32];
	char	ClrTp[1];				// CD_OPEN,CD_CLOSE
	char	Side[1];				// CD_BUY, CD_SELL
	char	OpenPrc[LEN_PRC];
	char	BasePrc[LEN_PRC];
	char	CurrPrc[LEN_PRC];
	char	BestPrc[LEN_PRC];		// ���� �� �ְ����� ����
	char	BestPL[LEN_PL];			// �ְ����� ����
	char	CurrPL[LEN_PL];			// ���簡���� ����
	char	OrdProtTp[1];			// CD_ORD_TP_MARKET, CD_ORD_TP_LIMIT
	char	OrdTM[12];				// hh:mm:ss.mmm
	char	ApiDT[8];
	char	ApiTM[8];	//hh:mm:ss
	char	Note[100];
}ST_MF_STRAT_ORD;


// ������ �۾��뺸 ���� ����
typedef struct _ST_MANAGER_SOCKNOTIFY
{
	char AcntNo[11];
	char Symbol[5];
	char OrdTp[1];	// 1:NEW, 2:��ü���, 3:��üû��
	char PrcTp[1];	// 1:������, 2:���尡
	char Side[1];	// 1:BUY, 2:SELL
	char Prc[10];
}ST_MANAGER_SOCKNOTIFY;

// ������ �۾� �뺸
typedef struct _ST_MANAGER_NOTIFY
{
	int nSeqNo;
	char zAcntNo[LEN_ACNT_NO + 1];
	int nWorkTp;
	char zWorkVal[64];
	char zWorkNote[64];
	char zAdminID[64];
}ST_MANAGER_NOTIFY;


/////////////////////////////////////////////////////////////////////
// LEGACY

//#define STRATID_FB_SIG_OPENPRC		"01"
//#define STRATID_FB_SIG_ASSISTPRC	"02"
//#define STRATID_FB_SIG_CROSSMA		"03"
//#define STRATID_FB_SIG_SIDEWAY		"04"
//#define STRATID_FB_ORD_OPENPRC		"11"
//#define STRATID_FB_ORD_ASSISTPRC	"12"
//#define STRATID_FB_ORD_CROSSMA		"13"
//#define STRATID_FB_ORD_SIDEWAY		"14"
//#define STRATID_FB_ORD_OPEN_ASSIST	"15"
//#define STRATID_FB_ORD_OPEN_ASSIST_CLRORD	"16"
//
//#define STRATID2NAME(cd,out){\
//	if(!strcmp(cd,STRATID_FB_SIG_OPENPRC))		sprintf(out,"SIG�ð���ġ");	\
//	if(!strcmp(cd,STRATID_FB_SIG_ASSISTPRC))	sprintf(out,"SIG������ǥ");	\
//	if(!strcmp(cd,STRATID_FB_SIG_CROSSMA))		sprintf(out,"SIG������");	\
//	if(!strcmp(cd,STRATID_FB_SIG_SIDEWAY))		sprintf(out,"SIGȾ������");	\
//	if(!strcmp(cd,STRATID_FB_ORD_OPENPRC))		sprintf(out,"ORD�ð���ġ");	\
//	if(!strcmp(cd,STRATID_FB_ORD_ASSISTPRC))	sprintf(out,"ORD������ǥ");	\
//	if(!strcmp(cd,STRATID_FB_ORD_CROSSMA))		sprintf(out,"ORD������");	\
//	if(!strcmp(cd,STRATID_FB_ORD_SIDEWAY))		sprintf(out,"ORDȾ������");	\
//	if(!strcmp(cd,STRATID_FB_ORD_OPEN_ASSIST))			sprintf(out,"ORD�ð�����");	\
//	if(!strcmp(cd,STRATID_FB_ORD_OPEN_ASSIST_CLRORD))	sprintf(out,"ORD�ú�û��");	\
//}



/****************************************************************************************
SIGNAL PACKET
*****************************************************************************************/
//typedef struct _PACK_STRAT_HEADER
//{
//	char	StratCompID[LEN_STRAT_COMP_ID];// FB
//	char	StratTp[1];						// S:SIGNAL, O:ORDER, B:BOTH
//	char	StratID[LEN_STRAT_ID];
//	char	Symbol[LEN_SYMBOL];
//	char	ChartTp[2];						// chart kind. 00, 01, 02...
//	char	ChartNm[LEN_CHART_NM];
//	char	StratPrc[LEN_PRC];
//	char	TM[9];// HHMMSSmmm
//	char	Reserved[20];
//}PACK_STRAT_HEADER;


//typedef struct _PACK_HEADER_S2C
//{
//	char	Code[LEN_PACKET_CODE];
//	char	UserID[LEN_USERID];
//	char	StratCompID[LEN_STRAT_COMP_ID];// FB
//	char	StratTp[1];						// S:SIGNAL, O:ORDER, B:BOTH
//	char	Symbol[LEN_SYMBOL];
//	char	GroupKey[LEN_GROUP_KEY];
//	char	API_YN[1];
//	char	TM[9];// HHMMSSmmm
//	char	Reserved[20];
//}PACK_HEADER_S2C;
//
//typedef struct _PACK_HEADER_C2S
//{
//	char	Code[LEN_PACKET_CODE];
//	char	UserID[LEN_USERID];
//	char	StratCompID[LEN_STRAT_COMP_ID];// FB
//	char	RsltTp[1];						// S:SUCCESS, R:REJECT, N:NO RESPONSE
//	char	Symbol[LEN_SYMBOL];
//	char	ArrayCnt[2];
//	char	Msg[100];
//	char	TM[9];// HHMMSSmmm
//}PACK_HEADER_C2S;
//
//typedef struct _PACK_STRAT_FB_OPENPRC
//{
//	PACK_HEADER_S2C	Header;
//	char	StratID[LEN_STRAT_ID];
//	char	ChartTp[2];						// chart kind. 00, 01, 02...
//	char	ChartNm[LEN_CHART_NM];
//	char	StratPrc[LEN_PRC];
//	char	ChartGb[1];					// 1:����(-), 2:���(+)
//	char	ArrowType[1];				// E:empty, F:filled
//	char	CrossTp[1];					// 1:���, 2:����, 0:��
//	char	EOL[1];						// 0x10
//}PACK_STRAT_FB_OPENPRC;
//
//
//typedef struct _PACK_STRAT_FB_ASSIST
//{
//	PACK_HEADER_S2C	Header;
//	char	StratID[LEN_STRAT_ID];
//	char	ChartTp[2];						// chart kind. 00, 01, 02...
//	char	ChartNm[LEN_CHART_NM];
//	char	StratPrc[LEN_PRC];
//	char	ChartGb[1];						// 1:����, 2:���
//	char	MatchedChartNm[LEN_CHART_NM];	// chart which makes assist signal with base chart
//	char	ClrYN[1];
//	char	ClrCrossTp[1];
//	char	ClrAssistPrc[LEN_PRC];
//	char	ClrNowPrc[LEN_PRC];
//	char	EOL[1];						// 0x10
//}PACK_STRAT_FB_ASSIST;


//typedef struct _PACK_STRAT_FB_CROSSMA
//{
//	PACK_HEADER_S2C	Header;
//	char	StratID[LEN_STRAT_ID];
//	char	ChartTp[2];						// chart kind. 00, 01, 02...
//	char	ChartNm[LEN_CHART_NM];
//	char	StratPrc[LEN_PRC];
//	char	Tp[1];		// U : ���⵹��.upward breakthrough  D:����ر� downward breakthrough
//	char	MAPrice[LEN_PRC];
//	char	NowPrice[LEN_PRC];
//	char	EOL[1];						// 0x10
//}PACK_STRAT_FB_CROSSMA;
//
//
//typedef struct _PACK_STRAT_FB_SIDEWAY
//{
//	PACK_HEADER_S2C	Header;
//	char	StratID[LEN_STRAT_ID];
//	char	ChartTp[2];				// chart kind. 00, 01, 02...
//	char	ChartNm[LEN_CHART_NM];
//	char	StratPrc[LEN_PRC];		// when Tp=U High, when Tp=D, Low
//	char	Tp[1];					// U : ���⵹��.upward breakthrough  D:����ر� downward breakthrough
//	//char	HihgLowPrice[LEN_PRC];	
//	char	NowPrice[LEN_PRC];
//	char	EOL[1];						// 0x10
//}PACK_STRAT_FB_SIDEWAY;
//
//typedef struct _PACK_STRAT_ORD
//{
//	PACK_HEADER_S2C	Header;
//	char	Side[1];	// B, S
//	char	OrdTp[1];	// 1-������, 2-���尡, 3-����, 4-����
//	char	OrdProcTp[1];	// 1-�ű�, 2-����, 3-���, 4-�ź�, A - �������, B - ���������, C - ����û��, D - ������û��
//	char	ClrYN[1];		// û���ֹ�����
//	char	OrdPrc[LEN_PRC];
//	char	OrdQty[LEN_QTY];
//	char	CnclAllQtyYN[1];
//	char	OrigOrdNo[LEN_ORD_NO];
//	char	OrigPrcCncl[LEN_PRC];	//����,���� ��ҽ� ���ֹ� ����
//	char	SLTick[3];
//	char	PTTick[3];
//	char	EOL[1];						// 0x10
//
//}PACK_STRAT_ORD;
//
//typedef struct _PACK_C_INNERNCLR
//{
//	char Symbol[LEN_SYMBOL];
//	char Side[1];
//	char AvgPrc[LEN_PRC];
//	char Qty[LEN_QTY];
//}PACK_C_INNERNCLR;
//
//typedef struct _PACK_C_INNERORD
//{
//	char Symbol[LEN_SYMBOL];
//	char OrdTp[1];
//	char OrdNo[LEN_ORD_NO];
//	char OrigNo[LEN_ORD_NO];
//	char Side[1];
//	char OrdPrc[LEN_PRC];
//	char Qty[LEN_QTY];
//	char OrdDT[8];	//yyyymmss
//	char OrdTM[9];	//hhmmssmmm
//}PACK_C_INNERORD;
//
//typedef struct _PACK_C_MULTIRECORDS
//{
//	PACK_HEADER_C2S Header;
//
//	char	Body[LEN_BUFF_SIZE];	// header �� count ��ŭ PACK_C_INNERORD,PACK_C_INNERNCLR  �����Ͱ� �ݺ��ȴ�.
//}PACK_C_MULTIRECORDS;
//
//
////�ֹ�ü�� �ǽð�
//typedef struct _PACK_C_ORDCNTR
//{
//	PACK_HEADER_C2S	Header;
//	char	Symbol[LEN_SYMBOL];
//	char	ReplyTp[1];		// O:ORD, E:EXECUTE, R:REJECT
//	char	Side[1];
//	char	OrdPrc[LEN_PRC];
//	char	OrdQty[LEN_QTY];
//	char	ExecPrc[LEN_PRC];	//ü�ᰡ��
//	char	RemainQty[LEN_QTY];	//	�ܷ�
//	char	OrdNo[LEN_ORD_NO];
//	char	OrigNo[LEN_ORD_NO];
//	char	DT[8];				// �ֹ�ü�� ����
//	char	TM[8];				// �ֹ�ü�� �ð�
//	char	OrdTp[1];			//	������,���尡
//	char	OrdProcTp[1];		//	�ű�,����
//	char	ExecNo[LEN_ORD_NO];	//	ü���ȣ
//	char	OpenQty[LEN_QTY];	//	�ܰ����
//	char	OpenSide[1];		//	�ܰ����
//	char	AvgPrc[LEN_PRC];
//	char	Msg[50];
//}PACK_C_ORDCNTR;


//typedef struct _CHECKSIG_RESULT_SIDEWAY
//{
//	RET_SIGNAL	SigRet;
//	char		zStratID[LEN_STRAT_ID + 1];
//	char		zStratNm[64];
//	char		zGroupKey[LEN_GROUP_KEY + 1];
//	char		zChartNm[LEN_CHART_NM + 1];
//	char		zHihgLowPrice[LEN_PRC + 1];	// when Tp=U High, when Tp=D, Low
//	char		zNowPrice[LEN_PRC + 1];
//	char		Tp[1];						// U : ���⵹��.upward breakthrough  D:����ر� downward breakthrough
//
//}CHECKSIG_RESULT_SIDEWAY;


// kr �ü� ��Ŷ�� short code �߰�
//typedef	struct	_At0933O_REP
//{
//	char	WinId[10];	/* Client WINDOWS ID										 	*/
//	char	PCKey[1];	/* Request key													*/
//	char	ErrCode[4];	/* Error Code
//						�����̳� 0000�̸� ����, �׿� ����, _ESErr_st �� ����)		*/
//	char	NextKind[1];	/* ���� ���� (0.��������, 1.���� ����)							*/
//	char	filler[15];	/* filler														*/
//
//	char	FullCode[32];	/* ����ǥ���ڵ� 												*/
//	char	JinbubGb[1];	/* ���� (0.10���� 1.32���� 2.64���� 3.128���� 4.8����)			*/
//	char	Time[8];	/* �ð�(HH:MM:SS)												*/
//	char	CmpSign[1];	/* ����ȣ
//						0.���� 1.���� 2.���� 3.��� 4.�϶� 5.�⼼����
//						6.�⼼���� 7.�⼼��� 8.�⼼�϶�							*/
//	char	CmpPrice[20];	/* ���ϴ��														*/
//	char	ClosePrice[20];	/* ���簡														*/
//	char	CmpRate[8];	/* �����9(5)V9(2)												*/
//	char	TotQty[20];	/* �ŷ��� 														*/
//	char	ContQty[20];	/* ü�ᷮ 														*/
//	char	MatchKind[1];	/* ���簡�� ȣ������ (+.�ż� -.�ŵ�)							*/
//	char	Date[8];	/* ����(YYYYMMDD) 												*/
//	char	OpenPrice[20];	/* �ð� 														*/
//	char	HighPrice[20];	/* �� 														*/
//	char	LowPrice[20];	/* ���� 														*/
//	char	BuyPrice[20];	/* �ż�ȣ��														*/
//	char	SellPrice[20];	/* �ŵ�ȣ��														*/
//	char	MarketFlag[1];	/* �屸�� 0.���� 1.������										*/
//	char	DecLen[5];	/* ���� �Ҽ��� ����												*/
//}	At0933O_REP, *lpAt0933O_REP;
//
//typedef struct _At0933O_st_Ex
//{
//	At0933O_REP	org;
//	char		shortCode[LEN_SYMBOL];	// 6EM7 ����
//	long		seq;
//}At0933O_st_Ex;


//// �ֹ��������� ������ ��Ŷ
//typedef struct _ST_PACK2ORD
//{
//	char	Date[8];
//	char	Time[8];
//	char	shortCode[LEN_SYMBOL];
//	char	close[LEN_PRC];
//	char	seq[LEN_SEQNO];
//}ST_PACK2ORD;


//
//// HEDGE ����� ü�� ����
//typedef struct _COPYORD_TARGET_CNTR
//{
//	char	TradeTp[1];	//	DEF_TRADEGB_COPY, DEF_TRADEGB_HEDGE
//	char	Broker[2];	//	IR_BROKER_HD, IR_BROKER_KR
//	char	symbol[IRLEN_KR_FULLCODE];
//	char	side[1];		// 1: �ż� , 2:�ŵ�
//	char	prc[10];
//	char	qty[10];
//	char	multiple[2];	// ���� �¼�. ���?
//	char	TargetAcntNo[20];
//	char	TargetOrdNo[20];
//	char	TargetOrdTp[10]; // 1:���尡, 2:������, 3:STOP, 4:STOP������
//	char	TargetCmsn[10];
//	char	TargetDT[8];
//	char	TargetTM[8];
//}COPYORD_TARGET_CNTR;
//

//#define IR_GET_SHORTCODE(broker,code_in,code_out)

bool ir_isbrokerKR(char* psBroker);
bool ir_isbrokerHD(char* psBroker);
char* ir_cvtcode_HD_KR(char *pzhdCode, char* pzOut);	// CLQ17 -> CLQ7
char* ir_cvtcode_uro_6e(char* pzIn, char* pzOut);
char* ir_cvtcode_6e_uro(char* pzIn, char* pzOut);
//char* pack_header_fields(char* pData, char* pzName, char* pzOut);


// LEGACY
//#define	DEF_BUY	'1'
//#define DEF_SELL '2'

//#define DEF_LIMIT_ORD '1'
//#define DEF_MARKET_ORD '2'
//
//
//#define DEF_RSLT_NEW	'1'	//�ű��ֹ�����
//#define DEF_RSLT_MATCH	'2'	//ü��
//#define DEF_RSLT_MDFY	'3'	//����
//#define DEF_RSLT_CNCL	'4'	//���
//#define DEF_RSLT_RJCT_NEW	'5'
//#define DEF_RSLT_RJCT_MDFY	'6'
//#define DEF_RSLT_RJCT_CNCL	'5'
//
//#define	ORD_TYPE_NEW	'1'
//#define	ORD_TYPE_MDFY	'2'
//#define	ORD_TYPE_CNCL	'3'
//
//enum STOPORD_TP { STOPORD_OPEN = 1, STOPORD_ASSIST_BUY, STOPORD_ASSIST_SELL, STOPORD_AFTERSIGNAL, STOPORD_OPENHILO };	// 1=���ԵǴ� ��� STOP, 2=������ǥ ���� STOP

//#define DEF_TRADEGB_COPY	'1'
//#define DEF_TRADEGB_HEDGE	'1'
//
//
//
//#define STRAT_TP_SIGNAL		'S'
//#define STRAT_TP_ORDER		'O'
//#define STRAT_TP_BOTH		'B'

//#define NOTIFY_ORD_TP_NEW		'1'
//#define NOTIFY_ORD_TP_ALLCNCL	'2'
//#define NOTIFY_ORD_TP_ALLCLR	'3'
//#define NOTIFY_PRC_TP_LIMIT		'1'
//#define NOTIFY_PRC_TP_MARKET	'2'




// ������ �۾�
/*
"1)�ν꽺 ���� �߰�
2)�ν꽺 ������� ����
3)�ν꽺 ������� ����
4)�ν꽺 �������� ����
5)�ν꽺 ���� ����
6)FB ���� �߰�
7)FB �ڵ��ֹ����� ����
8)FB �ڵ��ֹ����� ����
9)FB ���� ����
10) FBƯ������ ��������
11) FBƯ������ ��������
*/
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

// LEGACY
