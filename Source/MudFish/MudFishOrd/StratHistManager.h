#pragma once

/*
	bitwise operation

	0000 0000	// there is no strategy
	0000 0001	// strat01 exist	STRATID_BUY_OPEN
	0000 0010	// strat02 exist	STRATID_SELL_OPEN
	
*/
#include "../../IRUM_INC/IRUM_Common.h"
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
	char zOpenPrc[32];
	int nMaxCntSL;	//당일 누적 sl 제한횟수
	int nMaxCntPT;	//당일 누적 pt 제한횟수
	char zEndTM[6];	//HH:MM
	int nOrdQty;
	double dEntrySpread;	//0.1%
	double dClrSpread;		//0.5%
	double dPtPoint;
}ST_STRAT_PARAM;

class ST_POSITION
{
public:
	ST_POSITION() {};
	~ST_POSITION() {};

	void Init(BOOL bFirst);
	
	char zEntryPrc[32];
	char zClrPrc[32];
	char zPL[32];
	char cBsTp[1];
	double dQty;

	double dOrdSentQty;			//주문전송한 수량. 0 이 아니면 주문하지 않는다.

	EN_STATUS_FLAG	Status;
	int				nTotSLCnt;	//당일 누적 sl 횟수
	int				nTotPTCnt;	//당일 누적 pt 횟수
	//BOOL			bProfitRealized;
	char			zMaxPLPrc[LEN_PRC + 1];	//진입 후 최고 수익가격
	BOOL			bHitPTPrc;	// 0.5% 수익구간을 찍었나?
	
	BOOL			bFinish;
};

class CStratHistManager
{
public:
	CStratHistManager(char* pzSymbol);
	~CStratHistManager();

	void	SetInitInfo(double dTickVal, double dTickSize, int nDotCnt, 
		char* pzOpenPrc, int nOrdQty, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt,
		double dEntrySpread, double dClrSpread, double dProfitRealiezed);

	void	SetCntrInfo(LPCSTR lpBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
private:
	void	CalcClrCntr(LPCSTR lpBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty);
public:

	BOOL	IsFinished() { return m_pos.bFinish; }
	void	SetMaxPLPrc(char* pzCurrPrc);
	BOOL	IsOpenSrategyExist() { return (m_pos.Status != FLAG_NONE); }
	VOID	SetOrderSent(double dSentQty) { m_pos.dOrdSentQty += dSentQty; }
	VOID	SetOpenPrc(char *pzOpenPrc);


	//BOOL	IsAlreadySLMaxCnt() { return (m_pos.nTotSLCnt >= m_param.nMaxCntSL); }
	//BOOL	IsProfitRealized() {return m_pos.bProfitRealized;}
	BOOL	IsPTCondition(char* pzCurrPrc, _Out_ char* pMsg );

	char*	GtMaxPLPrc() { return m_pos.zMaxPLPrc; }
	char*	GetEntryPrc() { return m_pos.zEntryPrc; }
	BOOL	IsHitPTPrc() { return m_pos.bHitPTPrc; }
	BOOL	IsAlreadySent() { return (m_pos.dOrdSentQty == 0); }
	void	lock() { EnterCriticalSection(&m_cs); }
	void	unlock() { LeaveCriticalSection(&m_cs); }

public:
	double	openprc_d() { return atof(m_param.zOpenPrc); }
	char*	openprc() { return m_param.zOpenPrc; }
	char*	endtm() { return m_param.zEndTM; }
	EN_STATUS_FLAG	status() { return m_pos.Status; }
	BOOL	IsLong() { return (m_pos.Status == FLAG_OPEN_BUY); }
	BOOL	IsShort() { return (m_pos.Status == FLAG_OPEN_SELL); }
	double	entryspread() { return m_param.dEntrySpread; }
	int		ordqty() { return m_param.nOrdQty; }
	int		entryqty() { return (int)m_pos.dQty; }
	int		dotcnt() { return m_symbol.nDotCnt; }
private:

	ST_SYMBOL_INFO	m_symbol;
	ST_STRAT_PARAM	m_param;
	ST_POSITION		m_pos;
	
	CRITICAL_SECTION	m_cs;
private:
};

