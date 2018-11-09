#pragma once

#include "BaseThread.h"
#include "../IRUM_INC/IRUM_COMMON.h"


//1:�ü�����, 2:�ű��ֹ�����,3:����ֹ�����,4:����ü�����,5:û��ü�����, 6:�ֹ�API����, 7:�׿�
enum EVENT_TP {EVENT_SISE=1, EVENT_NEWORD, EVENT_CNCL, EVENT_ENTRY, EVENT_CLR,EVENT_SEND_ORDER, EVENT_ETC=99};	

// 1:�ð���ġ,2:������ǥ����,3:������ǥ�ߵ�,4:���԰��ر�,5.STOP����,6:STOP�ߵ�,7:���Լ���,8:û�����, 9:STOP���, 10:���������
enum CHECK_TP { CHK_OPENPRC = 1, CHK_ASSIST_SET, CHK_ASSIST_FIRE, CHK_ENTRY_DOWN, CHK_STOP_SET, CHK_STOP_FIRE, 
	CHK_RECV_OPEN, CHK_RECV_CLR, CHK_STOP_CNCL, CHK_LIMIT_CNCL, CHK_NONE=99 };

// 1:����������, 2:û��������, 3:���������, 4:STOP����, 5:STOP�ߵ�, 6:STOP���, 
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
	1) �α�
	2) ���� ����
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

