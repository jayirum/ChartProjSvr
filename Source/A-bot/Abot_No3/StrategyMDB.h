#pragma once

#include "../../IRUM_UTIL/StrategyMDBBase.h"
#include "../../IRUM_UTIL/util.h"
#include <string>

enum EN_MKT_STATUS { MARKET_NONE = 0, MARKET_ON, MARKET_CLOSING, MARKET_CLOSED };
enum EN_POS_STATUS { POS_NONE=0, POS_OPEN};

typedef struct _PARAM
{
	int MaxCntSL;	//당일 누적 sl 제한횟수
	int MaxCntPT;	//당일 누적 pt 제한횟수
	char StartTM[5+1];
	char EndTM[5+1];	//HH:MM
	int OrdQty; 
	double dSLFirePoint;		// 0.1% loss of the entry
	double dPT_TouchPoint_50;	//0.5%
	double dPT_TouchPoint_80;	//0.8%
	double dPT_TouchPoint_90;	//0.9%
}PARAM;

typedef struct _OPTION
{
	int HiLoThroughFireCnt;	// 
	int ThroughCompareCandleCnt;
	char CrossUseYN[1];
	int CrossCandle;	//	cross 에 사용할 캔들 1,3,5,10,20,30,60,120 분
	int CrossSMATp;		//	SMA 를 5개 짜리? 10개짜리?	
	char CrossStrongYN[1];	//	진성, 가성
}OPTION;

typedef struct _TX_INFO
{
	//char EntryPrc[LEN_PRC+1];	//진입가
	char ClrPrc[LEN_PRC+1];
	char PL[LEN_PL+1];
	
	EN_POS_STATUS PosStatus;	// DEF_POS_STATUS_NONE, DEF_POS_STATUS_OPEN
	char PosBsTp[1];
	int PosQty;
	char PosPrc[LEN_PRC+1];
	
	int	OrdSentQty;			//주문전송한 수량. 0 이 아니면 주문하지 않는다.
	int TotSLCnt;	//당일 누적 sl 횟수
	int TotPTCnt;	//당일 누적 pt 횟수
	EN_MKT_STATUS MktStatus;	//EN_MKT_STATUS

	char zMaxPLPrc[LEN_PRC + 1];	//진입 후 최고 수익가격
	BOOL bPT_Touched_50, bPT_Touched_80, bPT_Touched_90;

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
	CStrategyMDB(char *pzSymbol);
	~CStrategyMDB();

	virtual BOOL	CreateMDB();
	//virtual VOID	Finalize();
	virtual BOOL	LoadSymbolAndStrategy();

	//BOOL Read(_In_ char* pzSymbol);
	//BOOL Save(_In_ char* pzSymbol, _In_ ST_MDB* pMDB);

	double	TickSize() { return m_p->symbol.TickSize; }
	double	TickValue() { return m_p->symbol.TickValue; }
	int		dotcnt() { return m_p->symbol.DotCnt; }

	BOOL IsFinished() {	return (m_p->tx.FinishedYN[0] == 'Y') ? TRUE : FALSE;}
	void SetFinished() { m_p->tx.FinishedYN[0] = 'Y'; }
	EN_MKT_STATUS	MktStatus() { return m_p->tx.MktStatus; }
	
	BOOL	Cross_IsOn() { return (m_p->option.CrossUseYN[0] == 'Y'); }
	int		Cross_GetSMATp() { return m_p->option.CrossSMATp; }
	BOOL	Cross_IsStrong() { return (m_p->option.CrossStrongYN[0] == 'Y'); }
	BOOL	Cross_Is_1MinCandle() { return (m_p->option.CrossCandle== 1); }
	BOOL	Cross_Is_3MinCandle() { return (m_p->option.CrossCandle == 3); }
	BOOL	Cross_Is_5MinCandle() { return (m_p->option.CrossCandle == 5); }
	
	int		OrdQty() { return m_p->param.OrdQty; }
	void	AddSentOrdQty(int nOrdQty) { m_p->tx.OrdSentQty += nOrdQty; }
	void	ReleaseSentOrdQty(int nReleaseQty) { m_p->tx.OrdSentQty -= nReleaseQty; }
	BOOL	OrderAlreadySent() { return m_p->tx.OrdSentQty; }

	BOOL	IsOpenSrategyExist() { return (m_p->tx.PosStatus == POS_OPEN); }
	BOOL	IsLong() { return (m_p->tx.PosBsTp[0] == 'B'); }
	BOOL	IsShort() { return (m_p->tx.PosBsTp[0] == 'S'); }
	int		PosQty() { return m_p->tx.PosQty; }
	char*	PosPrc() { return m_p->tx.PosPrc; }
	double	PosPrcD() { return atof(m_p->tx.PosPrc); }

	void	IncPTCnt() { m_p->tx.TotPTCnt++; }
	void	IncSLCnt() { m_p->tx.TotSLCnt++; }
	int		PTCnt() { return m_p->tx.TotPTCnt; }
	int		SLCnt() { return m_p->tx.TotPTCnt; }
	int		MaxPTCnt() { return m_p->param.MaxCntPT; }
	int		MaxSLCnt() { return m_p->param.MaxCntSL; }

	char*	StartTM() { return m_p->param.StartTM; }
	char*	EndTM() { return m_p->param.EndTM; }

	int		ThroughCompareCandleCnt() { return m_p->option.ThroughCompareCandleCnt; }
	int		HiLoThroughFireCnt() { return m_p->option.HiLoThroughFireCnt; }

	BOOL	SaveMktStatus(EN_MKT_STATUS status) { m_p->tx.MktStatus = status; }
	void	SetMaxPLPrc(char* pzCurrPrc);
	double	MaxPLPrc() { atof(m_p->tx.zMaxPLPrc); }
	BOOL	IsPT_Touched_50() { return m_p->tx.bPT_Touched_50; }
	BOOL	IsPT_Touched_80() { return m_p->tx.bPT_Touched_80; }
	BOOL	IsPT_Touched_90() { return m_p->tx.bPT_Touched_90; }
	VOID	SetPT_Touched_50() { m_p->tx.bPT_Touched_50 = TRUE; }
	VOID	SetPT_Touched_80() { m_p->tx.bPT_Touched_80 = TRUE; }
	VOID	SetPT_Touched_90() { m_p->tx.bPT_Touched_90 = TRUE; }

	void	AcptCntrProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
	void	AcptEntryAddProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
	VOID	AcptEntryNewProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
	void	AcptCloseProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);

	ST_MDB* CurrMdb() { return m_p; }
	
public:

	ST_MDB	*m_p;
private:
	char	t[512];
};

