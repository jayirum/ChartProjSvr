#pragma once

#include "../../IRUM_INC/KR/ESApiLayout.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/StratLogDB.h"
#include "../../IRUM_UTIL/QueueShm.h"
#include "../../IRUM_UTIL/NanoPubSub.h"
#include "../../IRUM_UTIL/ManageLimitOrd.h"
#include "../../IRUM_UTIL/ManageStopOrd.h"
#include "../../IRUM_UTIL/ManagePosition.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "FBStratBase.h"

#include <string>

class CAcntThread : public CBaseThread
{
public:

	CAcntThread(char* pAcnt, char* pPwd);
	virtual ~CAcntThread();

	BOOL Begin();
	VOID End();

private:
	BOOL Begin_StratLog(); BOOL Begin_Shm(); BOOL Begin_NanoSub(); BOOL	Begin_DB();
	BOOL LoadStrat();
	BOOL LoadCommonFactor();	// 전략을 위한 공통정보
	BOOL LoadAcntTradeInfo();	// 계좌관련정보
	VOID ReleaseStrat();		// 전략해제

	// Logic after receive market data
	VOID DoProc_RecvMarketData(const CHART_TP idx, const char* pSise);
	VOID CallStratWithMarketData(const CHART_TP idx, const char* pSise);

	// Logic after receive API response
	VOID DoProc_APIRespose(At0985O_st* p);
	VOID APIResponseProc(At0985O_st* p);

	// Logic after check every strategy
	VOID DoProc_StratResult(char* pResult);

	// Send order to broker
	VOID SendOrderToAPI();

	// Logic for manager notification
	VOID DoProc_ManagerNotify(ST_MANAGER_SOCKNOTIFY* p);

	////////////////////////////////////////////////////////////////////
	// STRATEGY
	VOID StrateryProc();
	VOID Strat1_OpenPrc();
	VOID Strat2_Assist();
	VOID Strat3_MACross();
	VOID Strat4_SidewalkCross();
	VOID Strat5_Compose1();
	VOID Strat6_Compose2();

	VOID ThreadFunc();
	VOID MainFunc();

	// UTIL 성 함수들
private:
	char* AcntNo() { return (char*)m_sAcnt.c_str(); }
	BOOL IsMySymbol(std::string sSymbol);

private:
	CStratLogDB		*m_pStratLog;
	CQueueShm		*m_pShmQ;
	CNanoPubSub		*m_pNanoSub;
	CManageLimitOrd	*m_pLimitOrd;
	CManageStopOrd	*m_pStopOrd;
	CManagePosition	*m_pPos;
	VOID			*m_pDBPool;

	std::string		m_sSymbol;
	std::string		m_sAcnt, m_sPwd;
	UINT			m_unStratNo;	// {FBSTRAT_OPEN, FBSTRAT_ASSIST, FBSTRAT_MACROSS, FBSTRAT_SIDEWALKCROSS, FBSTRAT_COMPOSE_1, FBSTRAT_COMPOSE_2};
	CFBStratBase	*m_pStrat1, *m_pStrat2; // 합성전략이 있으므로 두개 선언


	//API_OBJ			*m_api;		//stdafx.h
	BOOL			m_bDoStrategy;
	int				m_nAccmPLTick;			// 누적 손익 tick 수
	std::string		m_sCompCd, m_sCompNm;	// strategy company code, name
	char			m_zShmNm[256], m_zMutexNm[256];

	STRAT_COMMON	m_CommFactor;	// 공통 전략 factor
	//MAP_ACNT_TRADE	m_mapTrade;
	//CRITICAL_SECTION	m_csMapTrade;

	//CStopOrd		*m_pStopOrd;	// 손절STOP주문설정

	char			m_szMsg[1024];
	//CRITICAL_SECTION	m_csProtect;	// 다른 작업 수행 중 MANAGE_NOTIFY 실행 안되게.

};
