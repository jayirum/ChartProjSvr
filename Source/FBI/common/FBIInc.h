#pragma once
#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include <Windows.h>

namespace _FBI
{
	const char TM_DEAL_START[] = "08:00:00";
	const char TM_DEAL_END[] = "06:00:00";

	enum EN_DEAL_SATTUS {
		DEAL_STATUS_NONSTART = 0//	미시작
		, DEAL_STATUS_ORD		//	주문가능 1
		, DEAL_STATUS_WAIT		//	대기 2
		, DEAL_STATUS_CHARTWAIT	//	차트대기 3
		, DEAL_STATUS_RESULTING	//	결과산출 4
		, DEAL_STATUS_DONE		//	완료 5
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

	
	const int WM_TERMINATE = WM_USER + 513;
	const int WM_RECV_API_CHART = WM_USER + 514;
	const int WM_DEAL_STATUS = WM_USER + 515;
	const int WM_RESULT_START = WM_USER + 516;
	const int WM_ORD_RECV = WM_USER + 517;
	
	const int RT_SUCCESS = 0;

	const int TIMEOUT_CHECK_DEAL = 100;	// miliseconds


	enum EN_DATETP { DATETP_TRADE = 0, DATETP_NEXT } ;

	char* dealstatus(const int status, char* pzStatus);
	char* packlen(int len, char* out);
	char* rsltCode(int code, char* out);
	char* now(char* out);
	EN_DATETP   dateTp(char* pzNow);

	// date->yyyymmdd, time -> hh:mm:ss
	char* chartName(char* psDate, char* psTime, _Out_ char* pzChartNm);


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

	struct PT_DEAL_STATUS
	{
		char STX[1];
		char Len[4];
		char ArtcCd[FBILEN_SYMBOL];
		char StkCd[FBILEN_SYMBOL];
		char DealSeq[FBILEN_DEAL_SEQ];
		char DealStatus[1];			// 1:주문가능/2:대기/3:차트/4:결과중/5:완료
		char OrdResult[1];			// 1:DOWN / 2:UP / 3:EVEN
		char Time[8];				// 각 status 시작시간
		char CandleTime[5];			// hh:mm
		char ETX[1];
	};


	struct ST_ORD_INFO
	{
		char StkCd[FBILEN_SYMBOL];
		char DealSeq[FBILEN_DEAL_SEQ];
	};
}