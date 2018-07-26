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

#define LOG1_ORD_SND	"[�ֹ�����]"
#define LOG1_ORD_RECV	"[�ֹ�����]"
#define LOG1_CNTR_RECV	"[ü�����]"
#define LOG1_STRAT_ORD	"[�����ֹ�]"
#define LOG1_STRAT_RQMNT	"[�������]"
#define LOG1_STRAT_RELEASE	"[��������]"
#define LOG1_ADMIN			"[��������]"
#define LOG1_ETC			"[��Ÿ   ]"

#define LOG2_LIMIT_SEND		"[����_����]"
#define LOG2_STOP_SET		"[STOP����]"
#define LOG2_STOP_FIRE		"[STOP�ߵ�]"
#define LOG2_STOP_CNCL		"[STOP���]"
#define LOG2_STOP_RJCT		"[STOP�ź�]"
#define LOG2_CNCL_SEND		"[����_���]"
#define LOG2_CNCL_RJCT		"[���_�ź�]"
#define LOG2_REFER			"[����_����]"
#define LOG2_STRAT			"[����_����]"
#define LOG2_STRAT_RELEASE	"[����_����]"
#define LOG2_ORD_API		"[API_����]"
#define LOG2_ORD_API_R		"[API_����]"
#define LOG2_ADMIN_STRAT	"[����_����]"
#define LOG2_ADMIN_SYMBOL	"[����_����]"
#define LOG2_ADMIN_CROSS		"[CROSS����]"
#define LOG2_ADMIN_STRAT_TICK	"[����ƽ����]"
#define LOG2_ADMIN_CLEARALL	"[����_û��]"
#define LOG2_ADMIN_CNCLALL	"[����_���]"
#define LOG2_ADMIN_NEWORD	"[�ű�_�ֹ�]"
#define LOG2_ERROR			"[����_�߻�]"
#define LOG2_ETC			"[��Ÿ    ]"

#define MAX_ENTRY_STOP_TICK	15	// ������� ������ �ɶ� MAX


/*
�ð� ��ġ 	
1) ���� ������ ���� �ð��� ���� 9������ �ð��� �񱳸� �Ѵ�.
2) ��ġ�ϴ� �ð��� ����� �ð��̸� �ż�, ������ �ð��̸� �ŵ�"

���� ����	
3) �ż� ������ ���� ���� ���� 9�� �� �� ������ - 2 TICK ���ݿ� ������ �ֹ� ����
4) �ŵ� ������ ���� ���� ���� 9�� �� �� �ְ� + 2 TICK ���ݿ� ������ �ֹ� ����"

���԰��� �ر� û��	
1) ���� ���� ������ ���԰��� �ر��� û�� �ֹ� ����
- ������, ���簡��"

�������� ���� û��	
1) ���� ������ û������� 12 TICK �� �Ǹ� û���Ѵ�.
- ������ �ϸ� �̸� ����ؼ� �������ֹ��� �ɾ���´�."

*/



#define GET_APIPTR	((CESApi*)m_api->api)
//#define GET_SHMPTR	((CSharedMem*)m_api->pShm)
#define GET_DBPTR	((CDBPool*)m_api->pDB)

//#define LIST_ORD	std::map<int, LIMIT_ORDER>	// �ֹ���ȣ
///#define	LISTIT_ORD	std::map<int, LIMIT_ORDER>::iterator

#define MAP_ACNT_TRADE	std::map<CString, ACNT_TRADE_INFO*>		// symbol,
#define MAPIT_ACNT_TRADE	std::map<CString, ACNT_TRADE_INFO*>::iterator		// symbol,

//#define MAPDATA_GROUP_KEY(sSymbol)	m_mapTrade[sSymbol]->sGroupKey[m_mapTrade[sSymbol]->idxMyChart].GetBuffer()
//#define MAPDATA_CHART_IDX(sSymbol)	m_mapTrade[sSymbol]->idxMyChart



// ���� ���� ���� ����
typedef struct _COMM_FACTOR
{
	//char	szSymbol[LEN_SYMBOL + 1];	// �ѹ��� ������ �ŷ� �����ϴ�.
	int		nOrdQTy;
	//int		nTotProfitCutTick;			// ����û�� tick ��
	//int		nTotLossLimitTick;			// ���� ������ ������ �����ս� ƽ��
	int		nLosscutTick;
	char	ChartYN[CHART_TP_CNT];		// ������ ������ ��Ʈ����	
}COMM_FACTOR;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �ֹ�����
typedef struct _LIMIT_ORDER
{
	char zChartNm[LEN_CHART_NM + 1];	// �ѹ��� ������ �ŷ� �����ϴ�.
	int nOrdNo;
	int nOrigNo;
	int nRemainQty;
	char cSide[1];
	char zPrc[LEN_PRC + 1];
	BOOL bProfitOrder;					// �����ֹ��ΰ�?
}LIMIT_ORDER;


// ����������
typedef struct _POS_INFO
{
	//char szSymbol[LEN_SYMBOL + 1];	// �ѹ��� ������ �ŷ� �����ϴ�.
	int nOpenQty;
	char	cSide[1];
	char	zPosPrc[LEN_PRC + 1];
	char	zOpenTM[9];	//hh:mm:ss
	char	zChartNm[LEN_CHART_NM+1];
	int		nCntrNo;
}POS_INFO;


// ������ǥ ����
typedef struct _ASSIST_PRC
{
	char	zPrc[LEN_PRC + 1];
	DWORD	dwAssistClock;		// ������ǥ �߻��� �� clock - GetTickCount
}ASSIST_PRC;

// ������ȣ��
typedef struct _AFTER_SIGNAL
{
	CString		SignalStopOrdDone;
	DWORD		dwStartClock;
}AFTER_SIGNAL;

typedef std::map<INT, LIMIT_ORDER*>				MAP_LIMITORD;				// ORDNO, LIMIT_ORDER
typedef std::map<INT, LIMIT_ORDER*>::iterator	MAPIT_LIMITORD;				// ORDNO, LIMIT_ORDER

// �����񿡴� �� ��Ʈ�� �ִ�.
//typedef std::map<CHART_TP, LIST_ORDER>				MAP_REMAIN_ORD;		// CHART_TP, ORDNO, CHART_NM
//typedef std::map<CHART_TP, LIST_ORDER>::iterator	MAPIT_REMAIN_ORD;



// �������� ����
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
	//INT		CheckRemainOrd(CHART_TP tp);	// ������Ʈ�� �� �ֹ� ������ȯ
	
public:
	double		dTickSize;
	int			nDotCnt;
	CString		sSymbol;
	CString		sGroupKey[CHART_TP_CNT];
	BOOL		bInitCheckDone;					//	�������ڸ��ڴ� ó������ �ʵ���
	POS_INFO	Pos;
	//LIST_ORD	lstOrd;

	INT			TotProfitCutTick;
	INT			TotLossLimitTick;
	CString		mStratOpenDone;	//	���԰����� �������� ��Ʈ �̸�				Strat_Open
	CString		mStratCloseDone;	//	��������(������ǥ) �������� ��Ʈ �̸�			Strat_AssistSet
	BOOL		mClrOpenBreak;	//	���԰��ر� û���ֹ� �ߺ� ��������� ����.		Strat_Clr_OpenPrcBreak
	CString		mClrOpenHighLow;	// ������ ���Ժ� ��/�� +/- 2ƽ û�� stop �ֹ�	Strat_Clr_OpenHiLo
	//CString		arrSignalStopOrdDone;	//	signal �������� �ֹ��ߴ��� ����
	AFTER_SIGNAL	mAfterSignal;
	CString		mLastClrChartNm;		//	����û����Ʈ�̸�
	ASSIST_PRC	mAssist;				//	������ǥ								Strat_AssistFire
	MAP_LIMITORD		mapLimitOrd;


	//CString		mStratOpenDone[CHART_TP_CNT];	//	���԰����� �������� ��Ʈ �̸�				Strat_Open
	//CString		mStratCloseDone[CHART_TP_CNT];//	��������(������ǥ) �������� ��Ʈ �̸�			Strat_AssistSet
	//BOOL		mClrOpenBreak[CHART_TP_CNT];	//	���԰��ر� û���ֹ� �ߺ� ��������� ����.		Strat_Clr_OpenPrcBreak
	//CString		mClrOpenHighLow[CHART_TP_CNT];// ������ ���Ժ� ��/�� +/- 2ƽ û�� stop �ֹ�	Strat_Clr_OpenHiLo
	////char		arrrSignalSide[CHART_TP_CNT];
	////CString		arrSignalChartNm[CHART_TP_CNT];		//	�ż���ȣ ��Ʈ�̸�
	//CString		arrSignalStopOrdDone[CHART_TP_CNT];	//	signal �������� �ֹ��ߴ��� ����
	//CString		mLastClrChartNm[CHART_TP_CNT];	//	����û����Ʈ�̸�
	////CString		arrBuySignal[CHART_TP_CNT];		//	�ż���ȣ ��Ʈ�̸�
	////double		arrLastOpenPrcDick[CHART_TP_CNT];	//	���� open price tick => CODE-6  
	//ASSIST_PRC	mAssist[CHART_TP_CNT];			//	������ǥ								Strat_AssistFire
	////MAP_REMAIN_ORD	mapLimitOrd;					//	������ ���� �ֹ��� ü���� �ȵ� ���
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
	BOOL	LoadCommonFactor();		// ������ ���� ��������
	BOOL	LoadAcntTradeInfo();	// ���°�������

	VOID	MainFunc();
	VOID	ThreadFunc();
	BOOL	IsMySymbolData(_In_ BOOL bSise, _In_ char* pData, _Out_ CString& sSymbol, _Out_ CHART_IDX& idx);
	BOOL	IsMySymbol(_In_ CString& sSymbol);
	char*	GetMySymbol();
	//BOOL	IsMyChartData(_In_ CHART_IDX idx, _In_ CString& sSymbol);

	////////////////////////////////////////////////////////////////////////
	// �ü��� ���� ���
	VOID	DoSiseWrapper(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol);

	VOID	DoProc_RecvSise(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol);

	// CODE-1. �ð���ġ���� ����
	VOID	Strat_Open(_In_ CString& sSymbol, const CHART_IDX idx, const char* pzGroupKey, const char* pzNowChartNm,
		_Out_ ACNT_TRADE_INFO* pTrade, const ST_SHM_CHART_UNIT& nowChart, const ST_SHM_CHART_UNIT& prevChart);

	// CODE-2. ������ġ���� (������ǥ) ����
	VOID	Strat_AssistSet(_In_ CString& sSymbol, const CHART_IDX idx, const char* pzGroupKey, const char* pzNowChartNm, 
		_Out_ ACNT_TRADE_INFO* pTrade,const ST_SHM_CHART_UNIT& lastChart, const ST_SHM_CHART_UNIT& nowChart);

	// CODE-3. ������ǥ �ߵ� ����Open
	//VOID	Strat_AssistFire(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol, _Out_ ACNT_TRADE_INFO* pTrade, const ST_SHM_CHART_UNIT& nowChart, const ST_SHM_CHART_UNIT& prevChart);		// ������ǥ���� ��ġ�ؼ� �ֹ����� �Ǵ�.

	// CODE-4. ���� �� ���԰��ݺر� û�� ����
	VOID	Strat_Clr_OpenPrcBreak(const char* pSise, _In_ CString& sSymbol);

	// CODE-5. ���� �� ������Ʈ���� ������Ʈ ��/���� 2ƽ û������
	VOID	Strat_Clr_OpenHiLo(const CHART_IDX idx, const char* pSise, _In_ CString& sSymbol, const char* pzGroupKey,
									ACNT_TRADE_INFO* pTrade, const ST_SHM_CHART_UNIT& prevChart, const char* pzNowChartNm);

	//	CODE-6. 10�� �� �� �ŵ�/�ż���ȣ �� �������� ��ȣ�� ��/���� 1��30�� �ȿ� ���Ŀ��� ���� �� ����
	VOID	Strat_AfterSignal_HighLow(_In_ CString& sSymbol, ACNT_TRADE_INFO* pTrade, const char* pSise,
		const char* pzGroupKey, const char* pzNowChartNm, ST_SHM_CHART_UNIT& nowChart);

	//	CODE-7. ������ ���� stop �ֹ� �ߵ� üũ
	VOID	Check_StopFire(const char* pSise, _In_ CString& sSymbol);

	//	CODE-8. ���� ��Ʈ�� �� ��ü���ֹ� ���
	VOID	Cancel_RemainOrd(const char* pSise, _In_ CString& sSymbol);


	// �ü��� ���� ���
	////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////
	// API �� ���� �ֹ�ü�� ������ ���� ��� 
	VOID	DoProc_RecvAPIReplyWrapper( At0985O_st* p, _In_ CString& sSymbol);
	VOID	DoProc_RecvAPIReply( At0985O_st* p, _In_ CString& sSymbol);

	// ���� ���� �� �������� ����
	BOOL	AcmlPL_CheckRelease(const At0985O_st* p, const ACNT_TRADE_INFO* pTrade, _In_ CString& sSymbol);	

	// û��� stop �ֹ� add
	BOOL	AddNewStopOrder(CString& sSymbol, const double dPrc, const char cOrdSide, 
							const int nPosQty, const int nDotCnt, _Out_ char* pzPrc, 
							_In_ char* pzChartNm, STOPORD_TP stopOrdTp, ACNT_TRADE_INFO* pTrade,
							_Out_ int* pnTotCnt);

	// stop �ֹ� ����
	BOOL	SetStop_OnEntryPos(_In_ CString& sSymbol);	// ���� �� ��� ���� stop�ֹ� 

	// �����ֹ� ���� - ������
	BOOL	SetProfit_OnEntryPos(_In_ CString& sSymbol);	// ���� �� ��� ���� �ֹ� (12TICK)

	// �����ֹ� ���ó��
	VOID	CancelAllOrd_Symbol(_In_ CString& sSymbol);	// û���� �Ǹ� �����ֹ� ��� ó��
	VOID	CancelSamesideLimitOrd(ACNT_TRADE_INFO* pTrade, char cSide);	// ���� ���� �������ֹ� ���
	VOID	ReleaseStrategy(const BOOL bDBSave, const char cNotifySaveYN, const char* pzReleaseNote);	// ���� ���� 12ƽ �Ǵ� ���� �ս� 20ƽ �̸� ��������

	// ���� ���� ����
	VOID	UpdateAcmlPL(int nPL);
	VOID	ResetAcmlPL() { m_nAccmPLTick = 0; };

	// API �� ���� �ֹ�ü�� ������ ���� ��� 
	////////////////////////////////////////////////////////////////////////


	VOID	DoOrder(const char cOrdType, const BOOL bStop, const char* zPrc, const char cSide, 
							const int nQty, _In_ CString& sSymbol, const char* sOrigNo);
	ACNT_TRADE_INFO* GetTradeInfo(const CString& sSymbol);
	VOID	ReleaseSymbol(CString& sSymbol);
	VOID	ClearAllOrdPos(CString& sSymbol);

	// STOP�ֹ��� �������ֹ��� �ߺ��Ǵ��� ����
	BOOL	IsOverlappedStopAndLimitOrd(_In_ char* pzStopPrc, _In_ char cStopSide, _In_ ACNT_TRADE_INFO* pTrade);

	// ���� ���� ���� ��Ʈ�� ���� ��Ʈ ��������
	INT	GetLastChartSHM(char* pzGroupKey, char* pzChartNm, _Out_ ST_SHM_CHART_UNIT& shmLast, _Out_ ST_SHM_CHART_UNIT& shmNow);
	BOOL	SetSignal(const char cSide, const char* pzGroupKey, const char* pzChartNm );
	BOOL	IsChartUse(CHART_IDX tpIdx);
	BOOL	IsDoStrategy() { return m_bDoStrategy; };
	BOOL	IsPosOpened(CString& sSymbol){ return (m_mapTrade[sSymbol]->Pos.nOpenQty > 0); }
	BOOL	IsPosOpened(const ACNT_TRADE_INFO *pTrade) { return (pTrade->Pos.nOpenQty > 0); }
	BOOL	IsPassedAssistTime(DWORD dStartClock, DWORD* pdGap);	// ������ǥ ���� �ð��� �������� ����
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
	int				m_nAccmPLTick;				// ���� ���� tick ��

	COMM_FACTOR		m_CommFactor;	// ���� ���� factor
	MAP_ACNT_TRADE	m_mapTrade;
	CRITICAL_SECTION	m_csMapTrade;

	CStopOrd		*m_pStopOrd;	// ����STOP�ֹ�����
	CStratLog		*m_pStratLog;	//	�������೻���� db ����

	char			m_szMsg[1024];
	CRITICAL_SECTION	m_csProtect;	// �ٸ� �۾� ���� �� MANAGE_NOTIFY ���� �ȵǰ�.
	BOOL			bCrossCheck;			//	ũ�ν� ���� ���� ����
};


