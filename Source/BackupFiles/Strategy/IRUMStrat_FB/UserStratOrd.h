#pragma once

#include "../../IRUM_UTIL/ADOFunc.h"
#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/QueueShm.h"
#include "../../IRUM_UTIL/NanoPubSub.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "StratLog.h"
#include <map>

using namespace std;


// 주문정보
typedef struct _ST_ORD
{
	char zSymbol[LEN_SYMBOL + 1];
	char zChartNm[LEN_CHART_NM + 1];	// 한번에 한종목만 거래 가능하다.
	char cOrdTp[1];	
	int nOrdNo;
	int nOrigNo;
	int nRemainQty;
	char cSide[1];
	char zPrc[LEN_PRC + 1];
	char zStratID[LEN_STRAT_ID+1];
	char zApiDT[32];
	char zApiTM[32];
	char zSysDT[8+1];		//yyyymmdd
	char zSysTM[9+1];	//hhmmssmmm
}ST_ORD;

typedef std::map<INT, ST_ORD*>::iterator			IT_LIMIT;
typedef std::map<std::string, ST_ORD*>::iterator	IT_MIT_INNER;
typedef std::map<char, std::map<std::string, ST_ORD*>>::iterator IT_MIT;
class COrdInfo
{
public:
	COrdInfo();
	virtual ~COrdInfo();

	VOID SetOrdInfo(char* pzSymbol, char* pzStratID, char cOrdTp, char* pzOrdNo, char* pzOrigNo, 
					char* pzRemainQty, char cSide, char* pzPrc, char* pzApiDT, char* pzApiTM);
	
	BOOL GetLimitOrdInfoByOrdNo(int nOrdNo, ST_ORD* o_pInfo);
	int  GetOrdInfoBySide(char* pzSymbol, char cSide, char cOrdTp, char* o_pArrOrdInfo);

	INT DelLimitOrdInfoByOrdNo(char* pzOrdNo);
	INT DelMITOrdInfoByPrc(char cSide, char* pzFormatPrc);
	INT DelOrdInfoBySymbol(char* pzSymbol, char cOrdTp, char cSide, BOOL bAllSide, BOOL bAllTp);

public:
	std::map<INT, ST_ORD*>			m_mLimit;	// ORD_NO, 
	std::map< char, std::map<std::string, ST_ORD*> >	m_mMIT;	// SIDE, PRC, ST_ORD
};


//typedef std::map<INT, COrdInfo*>				MAP_LIMITORD;	// ORDNO, LIMIT_ORDER
//typedef std::map<INT, COrdInfo*>::iterator	MAPIT_LIMITORD;		// ORDNO, LIMIT_ORDER



// 포지션정보
typedef struct _ST_POS
{
	char	zSymbol[LEN_SYMBOL + 1];
	int		nOpenQty;
	char	cSide[1];
	char	zPosPrc[LEN_PRC + 1];
	char	zOpenTM[9];	//hh:mm:ss
	char	zChartNm[LEN_CHART_NM + 1];
	//int		m_nCntrNo;
}ST_POS;

class CPosInfo
{
public:
	CPosInfo();
	virtual ~CPosInfo();

	BOOL IsOpened(char *pzSymbol);
	BOOL IsSameChart(char *pzSymbol, char* psChartNm);
	VOID SetPosInfoTR(char* pzSymbol, char* pzOpenQty, char cSide, char* pzPosPrc, char* pzOpenTM, CHART_TP tp);
	VOID SetPosInfoReal(char* pzSymbol, int nPosQty, char cSide, char* pzPosPrc, char* pzDT, char* pzTM, CHART_TP tp);
	VOID DelPosInfo(char* pzSymbol);
	char PosSide(char* pzSymbol);
public:
	std::map<std::string, ST_POS*>	m_map;	// symbol, ST_POS
};



typedef struct _SYMBOL_INFO
{
	char	zSymbolInfo[LEN_SYMBOL + 1];
	double	dTickSize;
	int		nDotCnt;
}SYMBOL_INFO;

typedef struct _USER_STRAT_INFO
{
	char	zCompID[LEN_STRAT_COMP_ID+1];
	char	zUserID[LEN_USERID + 1];
	BOOL	bStratApplied;
	char	zSymbol[LEN_SYMBOL + 1];			// 전략심볼
	char	zAppliedStratID[32];
	CHART_TP ChartTp;
	BOOL	bCrossApplied;
	char	zLastClrChartNm[LEN_CHART_NM + 1];		// 최종청산봉
	char	zLastEntryChartNm[LEN_CHART_NM + 1];	// 최종진입봉
	int		nEntryOrdQty;	// 진입기본수량
	char	zLastStratPrc[LEN_PRC + 1];				// 전략최종가격
	CPosInfo	PosInfo;
	COrdInfo	OrdInfo;
	SYMBOL_INFO	SymbolInfo;
}USER_STRAT_INFO;

typedef struct _FLOW_INFO
{
	//BOOL	bRsltSend;
	char*	pSigData;
	//char	zClientBuf[LEN_BUFF_SIZE];
	char	zSymbol[LEN_SYMBOL + 1];
	char	zGroupKey[LEN_GROUP_KEY + 1];
	char	zChartNm[LEN_CHART_NM + 1];
	ST_SHM_CHART_UNIT	ChartNow, ChartPrev;

}FLOW_INFO;

class CUserStratOrd : public CBaseThread
{ 
public:
	CUserStratOrd(char* pzUserID, CMemPool* pMemPool, unsigned dwSaveThread, unsigned dwSendThread);
	~CUserStratOrd();

private:
	// 고객정보를 db 에서 조회. 또는, 전략해제
	BOOL	LoadUserInfo(BOOL bReleaseStrat);	// STRAT_ID 도

	VOID	MainFunc();
	VOID	ThreadFunc();

	BOOL	BeginDB();
	VOID	EndDB();

	BOOL	BeginShm();
	VOID	EndShm();

	BOOL	BeginSubscribe();
	VOID	EndSubScribe();

	BOOL	BeginStratLog();
	VOID	EndStratLog();

	//BOOL	BeginSignals();
	//VOID	EndSignals();

	VOID	StratOrdWrapper(char* pSignalData);
	VOID	StratOrd(char* pSignalData);

	VOID	Strat_OpenPrc();
	VOID	Strat_AssistPrc();
	VOID	Strat_AssistPrcSet();
	VOID	Strat_AssistPrcClr();
	VOID	Strat_CrossMA();
	VOID	Strat_SideWay();
	VOID	Strat_Multi_OpenAssist();
	VOID	Strat_Multi_OpenAssistClrOrd();

	VOID	ClientDataProc(char* pRecvData);
	VOID	ClientDataTrPos(char* pRecvData);
	VOID	ClientDataTrOrd(char* pRecvData);
	VOID	ClientDataRealOrd(char* pRecvData);
	VOID	ClientDataRealExec(char* pRecvData);

	CROSS_TP GetCross(const ST_SHM_CHART_UNIT& shmPrev);

	// 현재 기준 현재 차트와 직전 차트 가져오기
	INT		GetLastChartShm(char* pzGroupKey, char* pzChartNm, ST_SHM_CHART_UNIT& shmPrev, ST_SHM_CHART_UNIT& shmNow);
	
	VOID	Cncl_LimitOrd(char* pzSymbol, char cSide, int nOrdNo, char cSideOption='N');
	INT		Cncl_MITOrd(char* pzSymbol, char cSide, BOOL bBothSideAll);
	//VOID	New_LimitOrd();
	//VOID	New_MITOrd();

	VOID	SendSaveStratOrd(char cOrdTp, char cOrdProcTp, char* pzPrc, char* pzOrigPrc, char cSide, int nQty, 
								char cClrYN, int nOrigNo,char cApiYN, BOOL bCnclAllQty, char* pChartNm);
	
	// Logic for manager notification
	VOID	DoManagerNotify(void* pNotify);

	BOOL	IsInitCheckDone() {	return m_bInitCheck;}
	VOID	SetInitCheckDone() { m_bInitCheck = TRUE; }

	BOOL	IsStratApplied() { return m_UserInfo.bStratApplied; }
	VOID	SetStratApplied() { m_UserInfo.bStratApplied = !m_UserInfo.bStratApplied; }
	BOOL	IsCrossApplied() { return m_UserInfo.bCrossApplied; }
	BOOL	IsSameUserID(char* psUserID, int nIDLen);
	BOOL	IsMyStratSymbol(char* pzSymbol);

private:
	CMemPool		*m_pMemPool;
	CQueueShm		*m_pShmQ;
	CNanoPubSub		*m_pMDSub;
	CDBPoolAdo		*m_pDBPool;
	
	char			*m_pSignalResult;
	char			*m_pClientBuff;

	USER_STRAT_INFO	m_UserInfo;
	//SYMBOL_INFO		m_SymbolInfo;
	FLOW_INFO		m_FlowInfo;	// packet 받을 때마다 reset

	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_szMsg[1024];
	
	unsigned		m_dwSaveThread, m_dwSendThread;
	
	BOOL			m_bInitCheck;
	CStratLog		*m_pStratLog;
	
};


