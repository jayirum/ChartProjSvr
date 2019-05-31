#pragma once

#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "../common/FBIInc.h"
#include "../../IRUM_UTIL/ChartShmUtilFixed.h"
#include <map>
#include <string>

using namespace std;

class CDealManagerTenOp : public CBaseThread
{
public:
	CDealManagerTenOp(char* pzStkCd, char* pzArtcCd, int nIdx, unsigned int	nToClientThread);
	~CDealManagerTenOp();

	BOOL Initialize();
	VOID Finalize();

	BOOL InitChartShm();
	BOOL LoadDealInfo();

	VOID ThreadFunc			();
	VOID DealManage			();
	BOOL DealManageInner	();
	BOOL DealOrd			(char* pzNowFullTm, _FBI::ST_DEAL_INFO_TENOP* pInfo);
	int GetOrdTmRange		(char* pzNowFullTm, _FBI::ST_DEAL_INFO_TENOP* pInfo);
	BOOL DealWait			(char* pzNowFull, _FBI::ST_DEAL_INFO_TENOP* pInfo);
	BOOL DealChartWait		(char* pzNowFull, _FBI::ST_DEAL_INFO_TENOP* pInfo);
	BOOL DealResulting		(char* pzNowFull, _FBI::ST_DEAL_INFO_TENOP* pInfo);
	VOID DealErase			(int nDealSeq);
	void UpdateDealStatus	(_FBI::ST_DEAL_INFO_TENOP* pInfo);

	static unsigned WINAPI Thread_ResultProcByChart(LPVOID lp);
	VOID SaveAndSendResult	(_FBI::ST_DEAL_INFO_TENOP *pDeal, char UpDown, char* pzOpen, char* pzClose, char* pzChartNm);
	BOOL ReadChartShm		(CHART_TP ChartTp, char* pzChartNm, char* pzTmEnd, _Out_ ST_SHM_CHART_UNIT& chart, _Out_ int& refRetryCnt);


	static unsigned WINAPI Thread_TimeSave(LPVOID lp);
	BOOL IsTimesaveClass() { return (m_nIdx == 0); }

private:
	char	m_zStkCd[32];
	char	m_zArtcCd[32];
	CDBPoolAdo		*m_pDBPool;
	
	HANDLE			m_hRsltProc, m_hTimeSave;
	unsigned int	m_unRsltProc, m_unTimeSave;

	CChartShmUtilFixed	*m_pChartShm;
	int				m_nIdx;
	BOOL			m_bTimeSaveRun;

	int				m_nImAlive;
	int				m_nResultWaitSec;

	unsigned int	m_unToClientThread;

	map<int, _FBI::ST_DEAL_INFO_TENOP*>	m_mapDeal;	// deal seq, deal info
	CRITICAL_SECTION					m_csDeal;

};

