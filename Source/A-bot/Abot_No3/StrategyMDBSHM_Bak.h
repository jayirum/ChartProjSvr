#pragma once

#include "../../IRUM_UTIL/StrategyMDBBase.h"
#include "../../IRUM_UTIL/util.h"
#include <string>

enum EN_MKT_STATUS { MARKET_NONE = 0, MARKET_ON, MARKET_CLOSING, MARKET_CLOSED };


typedef struct _PARAM
{
	char MaxCntSL[3];	//당일 누적 sl 제한횟수
	char MaxCntPT[3];	//당일 누적 pt 제한횟수
	char StartTM[6];
	char EndTM[6];	//HH:MM
	char OrdQty[LEN_QTY];
}PARAM;

typedef struct _OPTION
{
	char CrossUseYN[1];
	char CrossCandle[3];	//	cross 에 사용할 캔들 1,3,5,10,20,30,60,120 분
	char CrossSMATp[2];		//	SMA 를 5개 짜리? 10개짜리?	
	char CrossStrongYN[1];	//	진성, 가성
}OPTION;

typedef struct _TX_INFO
{
	char EntryPrc[LEN_PRC];	//진입가
	char ClrPrc[LEN_PRC];
	char PL[LEN_PL];
	
	char PosStatus[1];	// DEF_POS_STATUS_NONE, DEF_POS_STATUS_OPEN
	char PosBsTp[1];
	char PosQty[LEN_QTY];
	char PosPrc[LEN_PRC];
	
	char OrdSentQty[LEN_QTY];			//주문전송한 수량. 0 이 아니면 주문하지 않는다.
	char TotSLCnt[3];	//당일 누적 sl 횟수
	char TotPTCnt[3];	//당일 누적 pt 횟수
	char MktStatus[1];	//EN_MKT_STATUS
	char FinishedYN[1];
}TX_INFO;

typedef struct _ST_MDB
{
	char		Symbol[LEN_SYMBOL];
	SYMBOL_INFO	symbol;	// from parent class
	PARAM		param;
	OPTION		option;
	TX_INFO		tx;
}ST_MDB;

class CStrategyMDB : public CStrategyMDBBase
{
public:
	CStrategyMDB(char *pzMDBName);
	~CStrategyMDB();

	virtual BOOL	CreateMDB();
	//virtual VOID	Finalize();
	virtual BOOL	LoadSymbolAndStrategy();

	BOOL Read(_In_ char* pzSymbol);
	BOOL Save(_In_ char* pzSymbol, _In_ ST_MDB* pMDB);

	BOOL IsFinished();
	EN_MKT_STATUS	MktStatus();
	BOOL	OrderAlreadySent();
	BOOL	IsOpenSrategyExist();
	int		dotcnt() { return m_nDotCnt; }
	BOOL	Cross_IsOn() { return (m_stCurrMdb.option.CrossUseYN[0] == 'Y'); }
	int		Cross_GetSMATp() { return S2N(m_stCurrMdb.option.CrossSMATp, sizeof(m_stCurrMdb.option.CrossSMATp)); }
	BOOL	Cross_IsStrong() { return (m_stCurrMdb.option.CrossStrongYN[0] == 'Y'); }
	BOOL	Cross_Is_1MinCandle() { return (S2N(m_stCurrMdb.option.CrossCandle, sizeof(m_stCurrMdb.option.CrossCandle)) == 1); }
	BOOL	Cross_Is_3MinCandle() { return (S2N(m_stCurrMdb.option.CrossCandle, sizeof(m_stCurrMdb.option.CrossCandle)) == 3); }
	BOOL	Cross_Is_5MinCandle() { return (S2N(m_stCurrMdb.option.CrossCandle, sizeof(m_stCurrMdb.option.CrossCandle)) == 5); }
	
	int		OrdQty() { return S2N(m_stCurrMdb.param.OrdQty, LEN_QTY); }
	BOOL	AddSentOrdQty();
	BOOL	ReleaseSentOrdQty(int nReleaseQty);

	BOOL	IsLong() { return (m_stCurrMdb.tx.PosBsTp[0] == 'B'); }
	BOOL	IsShort() { return (m_stCurrMdb.tx.PosBsTp[0] == 'S'); }
	int		PosQty() { return S2N(m_stCurrMdb.tx.PosQty, LEN_QTY); }
	char*	PosPrc();

	char*	StartTM();
	char*	EndTM();

	BOOL	SaveMktStatus(EN_MKT_STATUS status);

	ST_MDB* CurrMdb() { return &m_stCurrMdb; }
	ST_MDB	m_stCurrMdb;

private:
	BOOL	m_bAlreadRead;
	int		m_nDotCnt;
	char	t[512];
};

