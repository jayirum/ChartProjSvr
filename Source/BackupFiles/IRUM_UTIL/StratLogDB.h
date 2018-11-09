#pragma once

#include "BaseThread.h"
#include "../IRUM_INC/IRUM_COMMON.h"


//1:시세수신, 2:신규주문수신,3:취소주문수신,4:진입체결수신,5:청산체결수신, 6:주문API전송, 7:그외
enum EVENT_TP {EVENT_SISE=1, EVENT_NEWORD, EVENT_CNCL, EVENT_ENTRY, EVENT_CLR,EVENT_SEND_ORDER, EVENT_ETC=99};	

// 1:시가일치,2:보조지표설정,3:보조지표발동,4:진입가붕괴,5.STOP설정,6:STOP발동,7:진입수신,8:청산수신, 9:STOP취소, 10:지정가취소
enum CHECK_TP { CHK_OPENPRC = 1, CHK_ASSIST_SET, CHK_ASSIST_FIRE, CHK_ENTRY_DOWN, CHK_STOP_SET, CHK_STOP_FIRE, 
	CHK_RECV_OPEN, CHK_RECV_CLR, CHK_STOP_CNCL, CHK_LIMIT_CNCL, CHK_NONE=99 };

// 1:진입지정가, 2:청산지정가, 3:지정가취소, 4:STOP설정, 5:STOP발동, 6:STOP취소, 
enum ORD_TP { ORD_LIMIT_ENTRY = 1, ORD_LIMIT_CLR, ORD_LIMIT_CNCL,  ORD_STOP_SET, ORD_STOP_FIRE, ORD_STOP_CNCL, ORD_TP_NONE=99  };

typedef struct _LOGGING_PARAM
{
	char zAcntNo[LEN_ACNT_NO+1];
	char zSymbol[LEN_SYMBOL+1];
	int		nChartTp;
	int		nEventTp;
	int		nChkTp;
	char	zFrstValNm[20+1];
	char	zFrstVal1[20 + 1];
	char	zFrstVal2[20 + 1];
	char	zSndValNm[20 + 1];
	char	zSndVal1[20 + 1];
	char	zSndVal2[20 + 1];
	char	cBFPosSide[1];
	char	zBFPosPrc[LEN_PRC + 1];
	int		nBFPosQty;
	int		nOrdTp;
	char	cOrdSide[1];
	char	zOrdPrc[LEN_PRC + 1];
	char	zOrigNo[LEN_ORD_NO + 1];
	char	zMemo[100];
}LOGGING_PARAM;


typedef struct _LOGGING_PARAM_EX
{
	char zAcntNo[LEN_ACNT_NO + 1];
	char zSymbol[LEN_SYMBOL + 1];
	int		nChartTp;
	char	zCate1[20];
	char	zCate2[20];
	char	zMemo[1024];
}LOGGING_PARAM_EX;

/*
	1) 로깅
	2) 전략 저장
*/
class CStratLogDB : public CBaseThread
{
public:
	CStratLogDB(_In_ void *pDBPool);
	~CStratLogDB();

	BOOL Begin();
	VOID End();

	VOID	Logging(char* psAcntNo, char* psSymbol, CHART_TP chartTp, EVENT_TP eventTp, CHECK_TP chkTp,
		char* pzFrstValNm, char* pzFrstVal1, char* pzFrstVal2, 
		char* pzSndValNm, char* pzSndVal1, char* pzSndVal2,
		char cBFPosSide, char* psBFPosPrc, int nBFPosQty,
		ORD_TP ordTp, char cOrdSide, char* psOrdPrc, char* psOrigNo, char* pzMemo);
	VOID	LoggingEx(char* psAcntNo, char* psSymbol, CHART_TP chartTp, char* pzCate1, char* pzCate2, char* pzLogMsg);
	//VOID	SaveCntrLog(_In_ char* pApiPacket);
	VOID	ThreadFunc(); 
	
	VOID	DBLogging(LOGGING_PARAM_EX* p);
	VOID	DBSaveCntr_KR(_In_ char* pApiPacket);
	VOID	DBLoggingWrapper(LOGGING_PARAM_EX* p);
private:
	void					*m_pDBPool;
	char					m_szMsg[1024];
};

