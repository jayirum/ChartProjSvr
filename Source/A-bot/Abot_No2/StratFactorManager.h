#pragma once

/*
	bitwise operation

	0000 0000	// there is no strategy
	0000 0001	// strat01 exist	STRATID_BUY_OPEN
	0000 0010	// strat02 exist	STRATID_SELL_OPEN
	
*/
#include "../../IRUM_INC/IRUM_Common.h"
#include "../../IRUM_UTIL/SymbolUtils.h"
#include "StratID.h"
#include <windows.h>
#include <string>

typedef enum _EN_STATUS_FLAG
{
	FLAG_NONE = 0,
	FLAG_OPEN_BUY,
	FLAG_OPEN_SELL
}EN_STATUS_FLAG;


typedef enum _EN_CNTR_FLAG
{
	FLAG_OPEN = 0,
	FLAG_CLR,
	FLAG_CLR_PART,
	FLAG_REVERSE
}EN_CNTR_FLAG;

typedef struct _ST_SYMBOL_INFO
{
	char zArtcCd[32];
	char zSymbol[32];
	char zName[128];
	double dTickSize;
	double dTickValue;
	int nDotCnt;
}ST_SYMBOL_INFO;

typedef struct _ST_STRAT_PARAM
{
	char zStartTM[6];
	char zEndTM[6];	//HH:MM
	int nOrdQty;
	//char zOpenPrc[32];
	int nAcmlLossTickCap;	//당일 전략을 해제할 누적손실 틱수
	int nAcmlPTTickCap;		//당일 전략을 해제할 누적이익 틱수
	
}ST_STRAT_PARAM;

class ST_POSITION
{
public:
	ST_POSITION() {};
	~ST_POSITION() {};

	void Init(BOOL bFirst);
	void IncSLCnt();
	void IncPTCnt();
	
	char zEntryPrc[32];
	char zClrPrc[32];
	char zPL[32];
	char cBsTp[1];
	double dQty;

	double dOrdSentQty;			//주문전송한 수량. 0 이 아니면 주문하지 않는다.


	EN_STATUS_FLAG	Status;
	//int				nTotSLCnt;	//당일 누적 sl 횟수
	//int				nTotPTCnt;	//당일 누적 pt 횟수
	int				nAcmlLossTickCap;	// 당일 전략을 해제할 누적손실 틱수
	int				nAcmlPTTickCap;
	//char			zMaxPLPrc[LEN_PRC + 1];	//진입 후 최고 수익가격
	//BOOL			bPT50Touched, bPT80Touched, bPT90Touched;	
	BOOL			bFinish;
};

#define PTRATE_90	0.9
#define PTRATE_80	0.8
#define PTRATE_50	0.5

class CStratFactorManager
{
public:
	CStratFactorManager(char* pzSymbol);
	~CStratFactorManager();

	//void	SetInitInfo(double dTickVal, double dTickSize, int nDotCnt, 
	//	char* pzOpenPrc, int nOrdQty, char* pzStartTM, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt,
	//	double dEntryTouchPoint, double dPT50_TouchPoint, double dProfitRealiezed);

	BOOL	LoadStratInfo();
	void	AcptCntrProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
	void	AcptEntryNewProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
	void	AcptEntryAddProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
	void	AcptCloseProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
public:

	BOOL	IsFinished() { return m_pos.bFinish; }
	void	SetMaxPLPrc(char* pzCurrPrc);
	BOOL	IsOpenSrategyExist() { return (m_pos.Status != FLAG_NONE); }
	VOID	SetOrderSent(double dSentQty);
	VOID	SetOpenPrc(char *pzOpenPrc);
	//VOID	SetConfigInfo(LPCSTR pzOpenPRc, LPCSTR pzOrdQty, LPCSTR pzEndTM, LPCSTR sMaxSL,
	//	LPCSTR sMaxPT, LPCSTR sEntrySpread, LPCSTR sClrSpread, LPCSTR sPtPoint);

	//BOOL	IsAlreadySLMaxCnt() { return (m_pos.nTotSLCnt >= m_param.nMaxCntSL); }
	//BOOL	IsProfitRealized() {return m_pos.bProfitRealized;}
	//BOOL	IsPTCondition(char* pzCurrPrc, _Out_ char* pMsg, _Out_ char* pDbLog );
	BOOL	IsProfitTakingCondition(char* pzCurrPrc, void* pOption, _Out_ char* pMsg,_Out_ char* pzStratID );
	double	GetPTLevelTouched();

	//char*	GtMaxPLPrc() { return m_pos.zMaxPLPrc; }
	//char*	GetEntryPrc() { return m_pos.zEntryPrc; }
	//BOOL	IsPT50Touched() { return m_pos.bPT50Touched; }
	//BOOL	IsPT80Touched() { return m_pos.bPT80Touched; }
	//BOOL	IsPT90Touched() { return m_pos.bPT90Touched; }
	BOOL	IsAlreadySent() { return (m_pos.dOrdSentQty > 0); }
	void	lock() { EnterCriticalSection(&m_cs); }
	void	unlock() { LeaveCriticalSection(&m_cs); }
public:
	//double	openprc_d() { return atof(m_param.zOpenPrc); }
	//char*	openprc() { return m_param.zOpenPrc; }
	char*	endtm() { return m_param.zEndTM; }
	char*	starttm() { return m_param.zStartTM; }
	EN_STATUS_FLAG	status() { return m_pos.Status; }
	BOOL	IsLong() { return (m_pos.Status == FLAG_OPEN_BUY); }
	BOOL	IsShort() { return (m_pos.Status == FLAG_OPEN_SELL); }
	//double	entryspread() { return m_param.dEntryTouchPoint; }
	int		ordqty() { return m_param.nOrdQty; }
	int		entryqty() { return (int)m_pos.dQty; }
	int		dotcnt() { return m_symbol.dotcnt(); }

private:

	CSymbolUtils	m_symbol;
	ST_STRAT_PARAM	m_param;
	ST_POSITION		m_pos;
	
	CRITICAL_SECTION	m_cs;
private:
};

