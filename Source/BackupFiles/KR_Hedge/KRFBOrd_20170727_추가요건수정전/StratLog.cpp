#include "stdafx.h"
#include "StratLog.h"
#include "../../IRUM_UTIL/Util.h"


CStratLog::CStratLog(_In_ CDBPool	*pDBPool)
{
	m_pDBPool = pDBPool;
}


CStratLog::~CStratLog()
{
}


BOOL	CStratLog::Initialize()
{
	ResumeThread();
	return TRUE;
}

VOID CStratLog::Logging(
	char* psAcntNo, char* psSymbol, CHART_TP chartTp, EVENT_TP eventTp, CHECK_TP chkTp,
	char* pzFrstValNm, char* pzFrstVal1, char* pzFrstVal2,
	char* pzSndValNm, char* pzSndVal1, char* pzSndVal2,
	char cBFPosSide, char* psBFPosPrc, int nBFPosQty,
	ORD_TP ordTp, char cOrdSide, char* psOrdPrc, char* psOrigNo, char* pzMemo)
{
	LOGGING_PARAM* p = new LOGGING_PARAM;

	__try
	{
		sprintf(p->zAcntNo, "%.*s", LEN_ACNT_NO, psAcntNo);
		sprintf(p->zSymbol, "%.*s", LEN_SYMBOL, psSymbol);
		p->nChartTp = (int)chartTp;
		p->nEventTp = (int)eventTp;
		p->nChkTp = (int)chkTp;
		sprintf(p->zFrstValNm, "%.*s", sizeof(p->zFrstValNm), pzFrstValNm);
		sprintf(p->zFrstVal1, "%.*s", sizeof(p->zFrstVal1), pzFrstVal1);
		sprintf(p->zFrstVal2, "%.*s", sizeof(p->zFrstVal2), pzFrstVal2);
		sprintf(p->zSndValNm, "%.*s", sizeof(p->zSndValNm), pzSndValNm);
		sprintf(p->zSndVal1, "%.*s", sizeof(p->zSndVal1), pzSndVal1);
		sprintf(p->zSndVal2, "%.*s", sizeof(p->zSndVal2), pzSndVal2);
		p->cBFPosSide[0] = cBFPosSide;
		sprintf(p->zBFPosPrc, "%.*s", LEN_PRC, psBFPosPrc);
		p->nBFPosQty = nBFPosQty;
		p->nOrdTp = (int)ordTp;
		p->cOrdSide[0] = cOrdSide;
		sprintf(p->zOrdPrc, "%.*s", LEN_PRC, psOrdPrc);
		sprintf(p->zOrigNo, "%.*s", LEN_ORD_NO, psOrigNo);
		sprintf(p->zMemo, "%.*s", sizeof(p->zMemo), pzMemo);

		PostThreadMessage((DWORD)m_dwThreadID, WM_STRAT_LOGGING, (WPARAM)0, (LPARAM)p);
	}
	__except (ReportException(GetExceptionCode(), "CStratLog::Logging", m_szMsg))
	{

	}
}


VOID CStratLog::LoggingEx(
	char* psAcntNo, char* psSymbol, CHART_TP chartTp, char* pzCate1, char* pzCate2, char* pzLogMsg)
{
	LOGGING_PARAM_EX* p = new LOGGING_PARAM_EX;

	__try
	{
		sprintf(p->zAcntNo, "%.*s", LEN_ACNT_NO, psAcntNo);
		sprintf(p->zSymbol, "%.*s", LEN_SYMBOL, psSymbol);
		p->nChartTp = (int)chartTp;
		sprintf(p->zCate1, "%.*s", strlen(pzCate1) - 2, pzCate1 + 1);
		sprintf(p->zCate2, "%.*s", strlen(pzCate2) - 2, pzCate2 + 1);
		sprintf(p->zMemo, pzLogMsg);

		PostThreadMessage((DWORD)m_dwThreadID, WM_STRAT_LOGGING, (WPARAM)0, (LPARAM)p);
	}
	__except (ReportException(GetExceptionCode(), "CStratLog::Logging", m_szMsg))
	{

	}
}



VOID CStratLog::SaveCntrLog(_In_ char* pApiPacket)
{
	At0985O_st* p = new At0985O_st;
	CopyMemory(p, pApiPacket, sizeof(At0985O_st));


	__try
	{
		//sprintf(p->zAcntNo, "%.*s", LEN_ACNT_NO, psAcntNo);
		//sprintf(p->zSymbol, "%.*s", LEN_SYMBOL, psSymbol);
		//p->nChartTp = (int)chartTp;
		//sprintf(p->zMemo, pzLogMsg);

		PostThreadMessage((DWORD)m_dwThreadID, WM_SAVE_MATCH, (WPARAM)0, (LPARAM)p);
	}
	__except (ReportException(GetExceptionCode(), "CStratLog::SaveMatchLog", m_szMsg))
	{

	}
}



VOID CStratLog::ThreadFunc()
{
	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			Sleep(1000);
			continue;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_STRAT_LOGGING:
				DBLogging((LOGGING_PARAM_EX*)msg.lParam);
				delete (LOGGING_PARAM_EX*)msg.lParam;
				break;
			case WM_SAVE_MATCH:
				DBSaveCntr((char*)msg.lParam);
				delete (char*)msg.lParam;
				break;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
}


VOID CStratLog::DBSaveCntr(_In_ char* pApiPacket)
{
	CDBHandler db(m_pDBPool->Get());
	char sQ[1024];
	lpAt0985O_st p = (lpAt0985O_st)pApiPacket;

	sprintf(sQ, "INSERT INTO FB_TRADE_INFO ("
		"SYMBOL "
		",TRADE_QTY"
		",ACNT_NO"
		",TRADE_NO"
		",ORD_NO"
		",TRADE_SIDE"
		",ORD_TP"
		",TRADE_PRICE"
		",ORD_PRICE"
		",OPEN_QTY"
		",TRADE_DT"
		",SYS_DT"
		",TRADE_TM ) VALUES ("
		"'%.*s'"
		",%d"
		",'%.*s'"
		",%d"
		",%d"
		",'%.1s'" //		<TRADE_SIDE, char(1), >
		",'%.1s'"	//			, <ORD_TP, char(1), >
		",%10.5f"	//, <TRADE_PRICE, decimal(15, 5), >
		",%10.5f"	//, <ORD_PRICE, decimal(15, 5), >
		",%d"
		",'%.*s'"	//, <TRADE_DT, char(8), >
		",CONVERT(CHAR(8),GETDATE(),112)"	//, <SYS_DT, char(8), >
		",'%.*s'"	//,	, <TRADE_TM, char(8), >)
		")"
		,LEN_SYMBOL, p->FullCode
		, S2N(p->ExecQty, sizeof(p->ExecQty))
		, sizeof(p->Account), p->Account
		, S2N(p->Trd_no, sizeof(p->Trd_no))
		, S2N(p->Ord_no, sizeof(p->Ord_no))
		, p->Side
		, p->Open_tp
		, S2D(p->ExecPrice, sizeof(p->ExecPrice))
		, S2D(p->Price, sizeof(p->Price))
		, S2N(p->Open_q, sizeof(p->Open_q))
		, sizeof(p->Trd_date), p->Trd_date
		, sizeof(p->TradeTime), p->TradeTime
	);

	if (!db->Cmd(sQ) || !db->Exec())
	{
		sprintf(m_szMsg, "DB INSERT ERROR(%s)(%s)", sQ, db->GetLastError());
		return;
	}
}



/*

@I_ACNT_NO varchar(11)
,@I_SYMBOL varchar(5)
,@I_CHART_TP int
,@I_EVENT int
,@I_CHECK_TP int
,@I_FIRST_VAL_NM varchar(20)
,@I_FIRST_VAL_1 varchar(20)
,@I_FIRST_VAL_2 varchar(20)
,@I_SND_VAL_NM varchar(20)
,@I_SND_VAL_1 varchar(20)
,@I_SND_VAL_2 varchar(20)
,@I_BF_POS_SIDE char(1)
,@I_BF_POS_PRC varchar(20)
,@I_BF_POS_QTY int
,@I_ORD_TP int
,@I_ORD_SIDE char(1)
,@I_ORD_PRC varchar(20)
,@I_ORIG_NO varchar(20)
,@I_MEMO varchar(200)

*/
VOID CStratLog::DBLoggingWrapper(LOGGING_PARAM_EX* p)
{
	__try
	{
		DBLogging(p);
	}
	__except (ReportException(GetExceptionCode(), "CStratLog::DBLogging", m_szMsg))
	{
	}
}


VOID CStratLog::DBLogging(LOGGING_PARAM_EX* p)
{
	CDBHandler db(m_pDBPool->Get());
	char sQ[1024];

	sprintf(sQ, "EXEC FB_STRAT_LOGGINGEX "
		"'%.11s', "	//@I_ACNT_NO varchar(11)
		"'%.5s', "	//, @I_SYMBOL varchar(5)
		"%d, "		//, @I_CHART_TP int
		"'%s',"
		"'%s',"
		"'%.1024s' "	//, @I_MEMO varchar(200)
		,
		p->zAcntNo,	//@I_ACNT_NO varchar(11)
		p->zSymbol, //@I_SYMBOL varchar(5)
		p->nChartTp,	//, @I_CHART_TP int
		p->zCate1,
		p->zCate2,
		p->zMemo	// @I_MEMO varchar(200)
	);

	if (!db->Cmd(sQ) || !db->Exec())
	{
		//PrintMsg(FALSE, TRUE, "계좌정보 저장 오류(%s)", sQ.GetBuffer());
		return;
	}
}


#if 0
VOID CStratLog::DBLogging_old
(LOGGING_PARAM* p)
{
	CDBHandler db(m_pDBPool->Get());
	char sQ[1024];

	sprintf(sQ, "EXEC FB_STRAT_LOGGING "
		"'%.11s', "	//@I_ACNT_NO varchar(11)
		"'%.5s', "	//, @I_SYMBOL varchar(5)
		"%d, "		//, @I_CHART_TP int
		"%d, "		//, @I_EVENT int
		"%d, "		//, @I_CHECK_TP int
		"'%.20s', "	//, @I_FIRST_VAL_NM varchar(20)
		"'%.20s', "	//, @I_FIRST_VAL_1 varchar(20)
		"'%.20s', "	//, @I_FIRST_VAL_2 varchar(20)
		"'%.20s', "	//, @I_SND_VAL_NM varchar(20)
		"'%.20s', "	//, @I_SND_VAL_1 varchar(20)
		"'%.20s', "	//, @I_SND_VAL_2 varchar(20)
		"'%.1s', "	//, @I_BF_POS_SIDE char(1)
		"'%.1s', "	//, @I_BF_POS_PRC varchar(20)
		"%d, "		//, @I_BF_POS_QTY int
		"%d, "		//, @I_ORD_TP int
		"'%.1s', "	//,, @I_ORD_SIDE char(1)
		"'%.20s', "	//, @I_ORD_PRC varchar(20)
		"'%.20s', "	//, @I_ORIG_NO varchar(20)
		"'%.200s' "	//, @I_MEMO varchar(200)
		,
		p->zAcntNo,	//@I_ACNT_NO varchar(11)
		p->zSymbol, //@I_SYMBOL varchar(5)
		p->nChartTp,	//, @I_CHART_TP int
		p->nEventTp,	//, @I_EVENT int
		p->nChkTp,		//, @I_CHECK_TP int
		p->zFrstValNm,	// @I_FIRST_VAL_NM varchar(20)
		p->zFrstVal1,	// @I_FIRST_VAL_1 varchar(20)
		p->zFrstVal2,	// @I_FIRST_VAL_2 varchar(20)
		p->zSndValNm,	// @I_SND_VAL_NM varchar(20)
		p->zSndVal1,	// @I_SND_VAL_1 varchar(20)
		p->zSndVal2,	// @I_SND_VAL_2 varchar(20)
		p->cBFPosSide,	// @I_B,	F_POS_SIDE char(1)
		p->zBFPosPrc,	// @I_BF_POS_PRC varchar(20)
		p->nBFPosQty,	// @I_BF_POS_QTY int
		p->nOrdTp,		// @I_ORD_TP int
		p->cOrdSide,	// @I_ORD_SIDE char(1)
		p->zOrdPrc,	// @I_ORD_PRC varchar(20)
		p->zOrigNo,	// @I_ORIG_NO varchar(20)
		p->zMemo	// @I_MEMO varchar(200)
	);
		
	if (!db->Cmd(sQ) || !db->Exec())
	{
		//PrintMsg(FALSE, TRUE, "계좌정보 저장 오류(%s)", sQ.GetBuffer());
		return ;
	}
}

#endif