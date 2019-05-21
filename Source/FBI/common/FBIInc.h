#pragma once
#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include <Windows.h>
#include <string>

namespace _FBI
{
	const char TM_DEAL_START[] = "08:00:00";
	const char TM_DEAL_END[] = "06:00:00";


	enum EN_ORD_STATUS {
		ORD_STATUS_ORD = 0
		,ORD_STATUS_LOSE
		,ORD_STATUS_WIN
		,ORD_STATUS_EVEN
		,ORD_STATUS_CNCL
	};

	enum EN_DEAL_TP {
		DEAL_TP_ONE_OP = 0
		,DEAL_TP_TEN_OP
	};
	const char DEAL_TP_ONE_OP_C = '0';
	const char DEAL_TP_TEN_OP_C = '1';

	enum EN_DEAL_SATTUS {
		DEAL_STATUS_NONSTART = 0//	미시작
		, DEAL_STATUS_ORD		//	주문가능 1
		, DEAL_STATUS_WAIT		//	대기 2
		, DEAL_STATUS_CHARTWAIT	//	차트대기 3
		, DEAL_STATUS_RESULTING	//	결과산출 4
		, DEAL_STATUS_DONE		//	완료 5
	};	
	enum EN_DEAL_SATTUS_TENOP {
		DEAL_STATUS_NONSTART_TENOP = 0//	미시작
		, DEAL_STATUS_ORD_1		//	주문가능 1
		, DEAL_STATUS_ORD_2		//	주문가능 2
		, DEAL_STATUS_ORD_3		//	주문가능 3
		, DEAL_STATUS_ORD_4		//	주문가능 4
		, DEAL_STATUS_ORD_5		//	주문가능 5
		, DEAL_STATUS_ORD_6		//	주문가능 6
		, DEAL_STATUS_ORD_7		//	주문가능 7
		, DEAL_STATUS_ORD_8		//	주문가능 8
		, DEAL_STATUS_WAIT_TENOP		//	대기 9
		, DEAL_STATUS_CHARTWAIT_TENOP	//	차트대기 10
		, DEAL_STATUS_RESULTING_TENOP	//	결과산출 11
		, DEAL_STATUS_DONE_TENOP		//	완료 12
	};

	const char DEAL_RSLT_UP = 'U';
	const char DEAL_RSLT_DOWN = 'D';
	const char DEAL_RSLT_EVEN = 'E';

	enum EN_DEAL_SETTLE {
		SETTLE_LOSE = 0
		, SETTLE_WIN = 1
		, SETTLE_EVEN = 2

	};

	const int FBILEN_SYMBOL		= 10;
	const int FBILEN_PRC		= 10;
	const int FBIDOT_CNT		= 5;
	const int FBILEN_VOLUME		= 10;
	const int FBILEN_BUFFER		= 1024;
	const int FBILEN_USERID		= 20;
	const int FBILEN_USER_NM		= 20;
	const int FBILEN_PACKET_CODE	= 4;
	const int FBILEN_PACKET_SIZE	= 4;
	const int FBILEN_FRONT_NAME		= 10;
	const int FBILEN_RSLT_CODE		= 4;
	const int FBILEN_DEAL_SEQ		= 9;

	const char	STX = 0x02;
	const char	ETX = 0x03;

	const int MEM_BLOCK_SIZE	= 1024;
	const int MEM_PRE_ALLOC		= 100;
	const int MEM_MAX_ALLOC		= 1000;

	
	const int WM_TERMINATE		= WM_USER + 513;
	const int WM_RECV_API_CHART	= WM_USER + 514;
	const int WM_DEAL_STATUS	= WM_USER + 515;
	const int WM_RESULT_START	= WM_USER + 516;
	const int WM_ORD_RECV		= WM_USER + 517;
	const int WM_RECV_ORD		= WM_ORD_RECV;
	const int WM_RECV_PRC		= WM_USER + 518;
	const int WM_ORD_FIRED		= WM_USER + 519;
	
	const int RT_SUCCESS = 0;

	const int TIMEOUT_CHECK_DEAL = 100;	// miliseconds


	enum EN_DATETP { DATETP_TRADE = 0, DATETP_NEXT } ;

	char* dealstatus(const int status, char* pzStatus);
	char* dealstatusTenOp(const int status, char* pzStatus);
	char* packlen(int len, char* out);
	char* rsltCode(int code, char* out);
	char* now(char* out);
	EN_DATETP   dateTp(char* pzNow);

	// date->yyyymmdd, time -> hh:mm:ss
	char* chartName(char* psDate, char* psTime, _Out_ char* pzChartNm);

	void Dbl2Str(double in, int nTotLen, int nDotLen, std::string* out);
	int ComparePrices(std::string sPrc1, double dPrc2, int nTotLen, int nDotLen, std::string* sPrc2);

	struct HEADER
	{
		char len[4];
		char code[4];
		char sysTp[1];
		char rsltCode[4];
		char tm[17];
	};

	// API CHART DATA
	struct PT_API_CHART
	{
		char	STX[1];
		char	Len[4];
		char	StkCd[FBILEN_SYMBOL];
		char	Date[8];
		char	ChartType[1];	// m:minute, h:hour, d:day
		char	TimeFrame[3];	// 001: 1min, 005
		char	ChartTime[5];	// hh:mm
		char	Open[FBILEN_PRC];
		char	High[FBILEN_PRC];
		char	Low[FBILEN_PRC];
		char	Close[FBILEN_PRC];
		char	Volume[FBILEN_VOLUME];
		char	ETX[1];
	};
	// API TICK DATA PACKET
	struct PT_TICK_API
	{
		char	STX[1];
		char	Len[4];	// 전체 길이
		char	Date[8];
		char	Time[8];
		char	ShortCode[7];
		char	Close[10];
		char	CntrQty[10];
		char	DecLen[5];				// 소숫점 자릿수
		char	ETX[1];
	};

	struct ST_DEAL_INFO
	{
		char	ArtcCd[FBILEN_SYMBOL];
		int		DealSeq;
		int		DateTp;
		char	Date[8+1];
		char	tm_order[8+1];	//hh:mm:ss
		char	tm_wait[8+1];		//hh:mm:ss
		char	tm_chartwait[8+1];	//hh:mm:ss
		char	tm_end[8 + 1];
		char	tm_chart[5 + 1];
		EN_DEAL_SATTUS		DealStatus;
		int		DurationMin;
	};

	struct ST_DEAL_INFO_TENOP
	{
		char	ArtcCd[FBILEN_SYMBOL];
		int		DealSeq;
		int		DateTp;
		char	Date[8 + 1];
		char	tm_order1[8 + 1];	//hh:mm:ss
		char	tm_order2[8 + 1];	//hh:mm:ss
		char	tm_order3[8 + 1];	//hh:mm:ss
		char	tm_order4[8 + 1];	//hh:mm:ss
		char	tm_order5[8 + 1];	//hh:mm:ss
		char	tm_order6[8 + 1];	//hh:mm:ss
		char	tm_order7[8 + 1];	//hh:mm:ss
		char	tm_order8[8 + 1];	//hh:mm:ss
		char	tm_wait[8 + 1];		//hh:mm:ss
		char	tm_chartwait[8 + 1];	//hh:mm:ss
		char	tm_end[8 + 1];
		char	tm_chart[5 + 1];
		EN_DEAL_SATTUS_TENOP		DealStatus;
		int		DurationMin;
	};

	struct PT_DEAL_STATUS
	{
		char STX[1];
		char Len[4];
		char DealTp[1];				// 0:3M, 1:10M
		char ArtcCd[FBILEN_SYMBOL];
		char StkCd[FBILEN_SYMBOL];
		char DealSeq[FBILEN_DEAL_SEQ];
		char DealStatus[2];			// 1:주문가능/2:대기/3:차트/4:결과중/5:완료
									// DEALSTATUS_TENOP 도 포함
		char OrdResult[1];			// 1:DOWN / 2:UP / 3:EVEN
		char Time[8];				// 각 status 시작시간
		char CandleTime[5];			// hh:mm
		char ClosePrc[10];			// chart 의 close price
		char ETX[1];
	};


	struct ST_ORD_INFO
	{
		char StkCd[FBILEN_SYMBOL];
		char DealSeq[FBILEN_DEAL_SEQ];
	};

	// FBI_Order ==> FBI_DealManagerSL
	struct ST_SLORD
	{
		int		OrdNo;
		int		nOrdStatus;
		double	dOrdPrc;
		char	cUpDn;
		int		nTickCnt;

		char	cWinLose;	// W, L
		std::string sFiredPrc;
		std::string sArtcCd;
		std::string sStkCd;
		BOOL	bMain;
		double	dWinPrc;
		double	dLosePrc;
	};

	struct ST_STK_INFO
	{
		std::string	sArtcCd;
		std::string	sStkCd;
		double		dTickSize;
		long		lDotCnt;
	};


	struct ST_PRC_INFO
	{
		std::string	sStkCd;
		double		dPrc;
	};


	// Exec Price packet coming through SM server
#pragma pack(push, 1)
	struct TFutExec2
	{
		char issue[8 + 1];
		double gap;
		double cup;
		double sip;
		double hip;
		double lip;
		int vol;
		double amt;
		int time;
		char side[1 + 1];
		char ydiffSign[1 + 1];
		char chgrate[6 + 1];
	};

#pragma pack(pop)
}