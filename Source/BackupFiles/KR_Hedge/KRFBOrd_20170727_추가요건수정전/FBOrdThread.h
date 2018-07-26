#pragma once

#include "stdafx.h"
#include "ESApi.h"
#include "../Common/KRCommon.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/sharedmem.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "SisePubSub.h"
#include "StopOrd.h"
#include "StratLog.h"
#include <map>

#define LOG1_ORD_SND	"[주문전송]"
#define LOG1_ORD_RECV	"[주문수신]"
#define LOG1_CNTR_RECV	"[체결수신]"
#define LOG1_STRAT_ORD	"[전략주문]"
#define LOG1_STRAT_RQMNT	"[전략요건]"
#define LOG1_STRAT_RELEASE	"[전략해제]"
#define LOG1_ADMIN			"[관리설정]"
#define LOG1_ETC			"[기타   ]"

#define LOG2_LIMIT_SEND		"[지정_전송]"
#define LOG2_STOP_SET		"[STOP설정]"
#define LOG2_STOP_FIRE		"[STOP발동]"
#define LOG2_STOP_CNCL		"[STOP취소]"
#define LOG2_STOP_RJCT		"[STOP거부]"
#define LOG2_CNCL_SEND		"[지정_취소]"
#define LOG2_CNCL_RJCT		"[취소_거부]"
#define LOG2_REFER			"[참조_정보]"
#define LOG2_STRAT			"[전략_설정]"
#define LOG2_STRAT_RELEASE	"[전략_해제]"
#define LOG2_ORD_API		"[API_전송]"
#define LOG2_ORD_API_R		"[API_수신]"
#define LOG2_ADMIN_STRAT	"[전략_셋팅]"
#define LOG2_ADMIN_SYMBOL	"[종목_셋팅]"
#define LOG2_ADMIN_CROSS		"[CROSS설정]"
#define LOG2_ADMIN_STRAT_TICK	"[전략틱설정]"
#define LOG2_ADMIN_CLEARALL	"[종목_청산]"
#define LOG2_ADMIN_CNCLALL	"[종목_취소]"
#define LOG2_ADMIN_NEWORD	"[신규_주문]"
#define LOG2_ERROR			"[오류_발생]"
#define LOG2_ETC			"[기타    ]"

#define MAX_ENTRY_STOP_TICK	15	// 진입즉시 손절을 걸때 MAX


/*
시가 일치 	
1) 새로 생성된 봉의 시가를 이전 9개봉의 시가와 비교를 한다.
2) 일치하는 시가가 양봉의 시가이면 매수, 음봉의 시가이면 매도"

손절 설정	
3) 매수 진입이 되자 마자 기존 9개 봉 중 최저가 - 2 TICK 가격에 지정가 주문 설정
4) 매도 진입이 되자 마자 기존 9개 봉 중 최고가 + 2 TICK 가격에 지정가 주문 설정"

진입가격 붕괴 청산	
1) 진입 이후 봉에서 진입가격 붕괴시 청산 주문 전송
- 지정가, 현재가격"

누적수익 익절 청산	
1) 당일 누적된 청산손익이 12 TICK 이 되면 청산한다.
- 진입을 하면 미리 계산해서 지정가주문을 걸어놓는다."

*/



#define GET_APIPTR	((CESApi*)m_api->api)
//#define GET_SHMPTR	((CSharedMem*)m_api->pShm)
#define GET_DBPTR	((CDBPool*)m_api->pDB)

//#define LIST_ORD	std::map<int, LIMIT_ORDER>	// 주문번호
///#define	LISTIT_ORD	std::map<int, LIMIT_ORDER>::iterator

#define MAP_ACNT_TRADE	std::map<CString, ACNT_TRADE_INFO*>		// symbol,
#define MAPIT_ACNT_TRADE	std::map<CString, ACNT_TRADE_INFO*>::iterator		// symbol,

//#define MAPDATA_GROUP_KEY(sSymbol)	m_mapTrade[sSymbol]->sGroupKey[m_mapTrade[sSymbol]->idxMyChart].GetBuffer()
//#define MAPDATA_CHART_IDX(sSymbol)	m_mapTrade[sSymbol]->idxMyChart



// 공통 전략 제약 사항
typedef struct _COMM_FACTOR
{
	//char	szSymbol[LEN_SYMBOL + 1];	// 한번에 한종목만 거래 가능하다.
	int		nOrdQTy;
	//int		nTotProfitCutTick;			// 수익청산 tick 수
	//int		nTotLossLimitTick;			// 당일 전략을 해제할 누적손실 틱수
	int		nLosscutTick;
	char	ChartYN[CHART_TP_CNT];		// 전략을 수행할 차트종류	
}COMM_FACTOR;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 주문정보
typedef struct _LIMIT_ORDER
{
	char zChartNm[LEN_CHART_NM + 1];	// 한번에 한종목만 거래 가능하다.
	int nOrdNo;
	int nOrigNo;
	int nRemainQty;
	char cSide[1];
	char zPrc[LEN_PRC + 1];
	BOOL bProfitOrder;					// 익절주문인가?
}LIMIT_ORDER;


// 포지션정보
typedef struct _POS_INFO
{
	//char szSymbol[LEN_SYMBOL + 1];	// 한번에 한종목만 거래 가능하다.
	int nOpenQty;
	char	cSide[1];
	char	zPosPrc[LEN_PRC + 1];
	char	zOpenTM[9];	//hh:mm:ss
	char	zChartNm[LEN_CHART_NM+1];
	int		nCntrNo;
}POS_INFO;


// 보조지표 가격
typedef struct _ASSIST_PRC
{
	char	zPrc[LEN_PRC + 1];
	DWORD	dwAssistClock;		// 보조지표 발생한 때 clock - GetTickCount
}ASSIST_PRC;

// 이전신호봉
typedef struct _AFTER_SIGNAL
{
	CString		SignalStopOrdDone;
	DWORD		dwStartClock;
}AFTER_SIGNAL;

typedef std::map<INT, LIMIT_ORDER*>				MAP_LIMITORD;				// ORDNO, LIMIT_ORDER
typedef std::map<INT, LIMIT_ORDER*>::iterator	MAPIT_LIMITORD;				// ORDNO, LIMIT_ORDER

// 한종목에는 한 차트만 있다.
//typedef std::map<CHART_TP, LIST_ORDER>				MAP_REMAIN_ORD;		// CHART_TP, ORDNO, CHART_NM
//typedef std::map<CHART_TP, LIST_ORDER>::iterator	MAPIT_REMAIN_ORD;



// 개별계좌 정보
class ACNT_TRADE_INFO
{
public:
	ACNT_TRADE_INFO();
	virtual ~ACNT_TRADE_INFO();

	BOOL	IsInitDone() {return bInitCheckDone;}
	VOID	SetIdx(CHART_IDX idx) { idxMyChart = idx; }
	CHART_IDX	Idx() { return idxMyChart; }
	CHART_TP	Tp() { return (CHART_TP)(idxMyChart + 1); }
	char*	GroupKeyEx() { return sGroupKey[Idx()].GetBuffer(); }
	CString	GroupKey() { return sGroupKey[Idx()]; }
	char* Symbol() { return sSymbol.GetBuffer(); }
	//VOID	SetSellSignal(const char* pzChartNm);
	//VOID	SetBuySignal(const char* pzChartNm);
	BOOL	IsPosOpened() { return (Pos.nOpenQty > 0); }
	BOOL	IsSameChartOfOpen(const char* psChartNm);
	BOOL	IsSameChartOfClr(const char* psChartNm);

	
	int		LimitOrd_Add(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char* pzPrc, int nTotProfitCutTick);
	INT		LimitOrd_Modify(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char* pzPrc);
	int		LimitOrd_Cancel(_In_ INT nOrigNo);
	int		LimitOrd_Cancel_SameSide(_In_ char cSide, _Out_ int *pnOrdNo);
	BOOL	LimitOrd_GetFirstOrd(_Out_ LIMIT_ORDER* pOrdInfo);
	VOID	LimitOrd_Clear();
	BOOL	LimitOrd_GetOrdTobeCanceled(_In_ char* pzNowChartNm, _In_ int nPosQty, _In_ char cPosSide, _Out_ LIMIT_ORDER* p);
	BOOL	IsExistSameSideLimitOrd(_In_ char cSide);
	BOOL	LimitOrd_FindBetterOrd(_In_ char cSide, _In_ char* pzPrc, _Out_ int* pnOrdNo, _Out_ char* pzOrdPrc);
	VOID	LimitOrd_Lock() {EnterCriticalSection(&m_csLimitOrd); }
	VOID	LimitOrd_UnLock() {LeaveCriticalSection(&m_csLimitOrd); }

	//VOID	RemainOrd_AllRemove();
	//INT		CheckRemainOrd(CHART_TP tp);	// 이전차트에 들어간 주문 갯수반환
	
public:
	double		dTickSize;
	int			nDotCnt;
	CString		sSymbol;
	CString		sGroupKey[CHART_TP_CNT];
	BOOL		bInitCheckDone;					//	시작하자마자는 처리하지 않도록
	POS_INFO	Pos;
	//LIST_ORD	lstOrd;

	INT			TotProfitCutTick;
	INT			TotLossLimitTick;
	CString		mStratOpenDone;	//	진입가전략 최종수행 차트 이름				Strat_Open
	CString		mStratCloseDone;	//	종가전략(보조지표) 최종수행 차트 이름			Strat_AssistSet
	BOOL		mClrOpenBreak;	//	진입가붕괴 청산주문 중복 실행방지를 위해.		Strat_Clr_OpenPrcBreak
	CString		mClrOpenHighLow;	// 진입후 진입봉 고/저 +/- 2틱 청산 stop 주문	Strat_Clr_OpenHiLo
	//CString		arrSignalStopOrdDone;	//	signal 조건으로 주문했는지 여부
	AFTER_SIGNAL	mAfterSignal;
	CString		mLastClrChartNm;		//	최종청산차트이름
	ASSIST_PRC	mAssist;				//	보조지표								Strat_AssistFire
	MAP_LIMITORD		mapLimitOrd;


	//CString		mStratOpenDone[CHART_TP_CNT];	//	진입가전략 최종수행 차트 이름				Strat_Open
	//CString		mStratCloseDone[CHART_TP_CNT];//	종가전략(보조지표) 최종수행 차트 이름			Strat_AssistSet
	//BOOL		mClrOpenBreak[CHART_TP_CNT];	//	진입가붕괴 청산주문 중복 실행방지를 위해.		Strat_Clr_OpenPrcBreak
	//CString		mClrOpenHighLow[CHART_TP_CNT];// 진입후 진입봉 고/저 +/- 2틱 청산 stop 주문	Strat_Clr_OpenHiLo
	////char		arrrSignalSide[CHART_TP_CNT];
	////CString		arrSignalChartNm[CHART_TP_CNT];		//	매수신호 차트이름
	//CString		arrSignalStopOrdDone[CHART_TP_CNT];	//	signal 조건으로 주문했는지 여부
	//CString		mLastClrChartNm[CHART_TP_CNT];	//	최종청산차트이름
	////CString		arrBuySignal[CHART_TP_CNT];		//	매수신호 차트이름
	////double		arrLastOpenPrcDick[CHART_TP_CNT];	//	최종 open price tick => CODE-6  
	//ASSIST_PRC	mAssist[CHART_TP_CNT];			//	보조지표								Strat_AssistFire
	////MAP_REMAIN_ORD	mapLimitOrd;					//	전략에 의한 주문이 체결이 안된 경우
	//MAP_LIMITORD		mapLimitOrd;

private:
	CHART_IDX	idxMyChart;
	CRITICAL_SECTION m_csLimitOrd;
};		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CFBOrdThread : public CBaseThread
{ 
public:
	CFBOrdThread(char* pAcnt, char* pPwd, API_OBJ* api, CStratLog* pLog);
	~CFBOrdThread();

	BOOL	Initialize();
	BOOL	LoadCommonFactor();		// 전략을 위한 공통정보
	BOOL	LoadAcntTradeInfo();	// 계좌관련정보

	VOID	MainFunc();
	VOID	ThreadFunc();
	BOOL	IsMySymbolData(_In_ BOOL bSise, _In_ char* pData, _Out_ CString& sSymbol, _Out_ CHART_IDX& idx);
	BOOL	IsMySymbol(_In_ CString& sSymbol);
	char*	GetMySymbol();
	//BOOL	IsMyChartData(_In_ CHART_IDX idx, _In_ CString& sSymbol);

	////////////////////////////////////////////////////////////////////////
	// 시세를 받은 경우
	VOID	DoSiseWrapper(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol);

	VOID	DoProc_RecvSise(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol);

	// CODE-1. 시가일치전략 점검
	VOID	Strat_Open(_In_ CString& sSymbol, const CHART_IDX idx, const char* pzGroupKey, const char* pzNowChartNm,
		_Out_ ACNT_TRADE_INFO* pTrade, const ST_SHM_CHART_UNIT& nowChart, const ST_SHM_CHART_UNIT& prevChart);

	// CODE-2. 종가일치전략 (보조지표) 점검
	VOID	Strat_AssistSet(_In_ CString& sSymbol, const CHART_IDX idx, const char* pzGroupKey, const char* pzNowChartNm, 
		_Out_ ACNT_TRADE_INFO* pTrade,const ST_SHM_CHART_UNIT& lastChart, const ST_SHM_CHART_UNIT& nowChart);

	// CODE-3. 보조지표 발동 점검Open
	//VOID	Strat_AssistFire(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol, _Out_ ACNT_TRADE_INFO* pTrade, const ST_SHM_CHART_UNIT& nowChart, const ST_SHM_CHART_UNIT& prevChart);		// 보조지표조건 일치해서 주문할지 판단.

	// CODE-4. 진입 후 진입가격붕괴 청산 점검
	VOID	Strat_Clr_OpenPrcBreak(const char* pSise, _In_ CString& sSymbol);

	// CODE-5. 진입 후 다음차트부터 진입차트 고가/저가 2틱 청산점검
	VOID	Strat_Clr_OpenHiLo(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol, const char* pzGroupKey,
									ACNT_TRADE_INFO* pTrade, const ST_SHM_CHART_UNIT& prevChart, const char* pzNowChartNm);

	//	CODE-6. 10개 봉 중 매도/매수신호 후 다음봉들 신호봉 고가/저가 1분30초 안에 돌파여부 점검 및 진입
	VOID	Strat_AfterSignal_HighLow(_In_ CString& sSymbol, ACNT_TRADE_INFO* pTrade, const char* pSise,
		const char* pzGroupKey, const char* pzNowChartNm, ST_SHM_CHART_UNIT& nowChart);

	//	CODE-7. 설정해 놓은 stop 주문 발동 체크
	VOID	Check_StopFire(const char* pSise, _In_ CString& sSymbol);

	//	CODE-8. 이전 차트에 들어간 미체결주문 취소
	VOID	Cancel_RemainOrd(const char* pSise, _In_ CString& sSymbol);


	// 시세를 받은 경우
	////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////
	// API 를 통해 주문체결 응답을 받은 경우 
	VOID	DoProc_RecvAPIReplyWrapper( At0985O_st* p, _In_ CString& sSymbol);
	VOID	DoProc_RecvAPIReply( At0985O_st* p, _In_ CString& sSymbol);

	// 손익 누적 및 전략해제 점검
	BOOL	AcmlPL_CheckRelease(const At0985O_st* p, const ACNT_TRADE_INFO* pTrade, _In_ CString& sSymbol);	

	// 청산용 stop 주문 add
	BOOL	AddNewStopOrder(CString& sSymbol, const double dPrc, const char cOrdSide, 
							const int nPosQty, const int nDotCnt, _Out_ char* pzPrc, 
							_In_ char* pzChartNm, STOPORD_TP stopOrdTp, ACNT_TRADE_INFO* pTrade,
							_Out_ int* pnTotCnt);

	// stop 주문 설정
	BOOL	SetStop_OnEntryPos(_In_ CString& sSymbol);	// 진입 후 즉시 손절 stop주문 

	// 익절주문 설정 - 지정가
	BOOL	SetProfit_OnEntryPos(_In_ CString& sSymbol);	// 진입 후 즉시 익절 주문 (12TICK)

	// 기존주문 취소처리
	VOID	CancelAllOrd_Symbol(_In_ CString& sSymbol);	// 청산이 되면 기존주문 취소 처리
	VOID	CancelSamesideLimitOrd(ACNT_TRADE_INFO* pTrade, char cSide);	// 같은 방향 지정가주문 취소
	VOID	ReleaseStrategy(const BOOL bDBSave, const char cNotifySaveYN, const char* pzReleaseNote);	// 당일 이익 12틱 또는 당일 손실 20틱 이면 전략해제

	// 당일 손익 누적
	VOID	UpdateAcmlPL(int nPL);
	VOID	ResetAcmlPL() { m_nAccmPLTick = 0; };

	// API 를 통해 주문체결 응답을 받은 경우 
	////////////////////////////////////////////////////////////////////////


	VOID	DoOrder(const char cOrdType, const BOOL bStop, const char* zPrc, const char cSide, 
							const int nQty, _In_ CString& sSymbol, const char* sOrigNo);
	ACNT_TRADE_INFO* GetTradeInfo(const CString& sSymbol);
	VOID	ReleaseSymbol(CString& sSymbol);
	VOID	ClearAllOrdPos(CString& sSymbol);

	// STOP주문과 지정가주문이 중복되는지 점검
	BOOL	IsOverlappedStopAndLimitOrd(_In_ char* pzStopPrc, _In_ char cStopSide, _In_ ACNT_TRADE_INFO* pTrade);

	// 현재 기준 현재 차트와 직전 차트 가져오기
	INT	GetLastChartSHM(char* pzGroupKey, char* pzChartNm, _Out_ ST_SHM_CHART_UNIT& shmLast, _Out_ ST_SHM_CHART_UNIT& shmNow);
	BOOL	SetSignal(const char cSide, const char* pzGroupKey, const char* pzChartNm );
	BOOL	IsChartUse(CHART_IDX tpIdx);
	BOOL	IsDoStrategy() { return m_bDoStrategy; };
	BOOL	IsPosOpened(CString& sSymbol){ return (m_mapTrade[sSymbol]->Pos.nOpenQty > 0); }
	BOOL	IsPosOpened(const ACNT_TRADE_INFO *pTrade) { return (pTrade->Pos.nOpenQty > 0); }
	BOOL	IsPassedAssistTime(DWORD dStartClock, DWORD* pdGap);	// 보조지표 점검 시간이 지났는지 여부
	BOOL	IsPassedSecs(const char *pzChartNm, const UINT nCheckSecs);
	BOOL	IsSamePrc(const char* pPrc1, const int nLen1, const char* pPrc2, const int nLen2, int nDotCnt, _Out_ char* pzPrc);
	//int		CompPrc(const char* pPrc1, const int nLen1, const char* pPrc2, const int nLen2, const int nDotCnt);
	CROSS_TP GetCross(const ST_SHM_CHART_UNIT& shmLast, _In_ ACNT_TRADE_INFO *pTrade);
	char	*AcntNo() { return m_sAcnt.GetBuffer(); }
	VOID	UpdateTradeMap(CString& sSymbol, ACNT_TRADE_INFO *p, BOOL bDelete);
	VOID	APILOCK() { EnterCriticalSection((CRITICAL_SECTION*)m_api->csApi); }
	VOID	APIUNLOCK() { LeaveCriticalSection((CRITICAL_SECTION*)m_api->csApi); }

	VOID	MAPLOCK() { EnterCriticalSection(&m_csMapTrade); };
	VOID	MAPUNLOCK() { EnterCriticalSection(&m_csMapTrade); };

	BOOL	InitChartSHM();
	VOID	CloseChartSHM();
private:
	CSharedMem		m_shm;
	CString			m_sAcnt,  m_sPwd;
	CSisePubSub		*m_siseSub;
	API_OBJ			*m_api;		//stdafx.h
	char			m_szLastDoTime[5];	//hh:mm
	char			m_zDate[9];
	BOOL			m_bDoStrategy;
	int				m_nAccmPLTick;				// 누적 손익 tick 수

	COMM_FACTOR		m_CommFactor;	// 공통 전략 factor
	MAP_ACNT_TRADE	m_mapTrade;
	CRITICAL_SECTION	m_csMapTrade;

	CStopOrd		*m_pStopOrd;	// 손절STOP주문설정
	CStratLog		*m_pStratLog;	//	전략수행내용을 db 저장

	char			m_szMsg[1024];
	CRITICAL_SECTION	m_csProtect;	// 다른 작업 수행 중 MANAGE_NOTIFY 실행 안되게.
	BOOL			bCrossCheck;			//	크로스 조건 점검 여부
};


