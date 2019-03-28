#pragma once

#include "../../IRUM_UTIL/TcpClient.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "../common/FBIInc.h"
#include "../../IRUM_UTIL/ChartShmUtilFixed.h"
#include <map>
#include <string>


typedef std::map<int, 
class CDealManager : public CBaseThread
{
public:
	CDealManager(char* pzStkCd, char* pzArtcCd, int nIdx);
	~CDealManager();

	BOOL Initialize();
	VOID Finalize();

	BOOL InitClientConnect();
	BOOL InitChartShm();
	//BOOL LoadChartInfo();
	BOOL LoadDealInfo();

	VOID ThreadFunc();
	VOID DealManage();
	BOOL DealManageInner();
	BOOL DealOrd(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	BOOL DealWait(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	BOOL DealChartWait(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	BOOL DealResulting(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	VOID DealErase(int nDealSeq);
	void UpdateDealStatus(_FBI::ST_DEAL_INFO* pInfo);

	BOOL SendToClient(_FBI::PT_DEAL_STATUS* pPacket, int nRecurCnt);

	static unsigned WINAPI Thread_ResultProcByChart(LPVOID lp);
	static unsigned WINAPI Thread_TimeSave(LPVOID lp);
	BOOL IsTimesaveClass() { return (m_nIdx == 0); }

private:
	char	m_zStkCd[32], m_zArtcCd[32];
	CDBPoolAdo		*m_pDBPool;
	CTcpClient		*m_pClient;
	
	HANDLE			m_hRsltProc, m_hTimeSave;
	unsigned int	m_unRsltProc, m_unTimeSave;

	std::map<int, _FBI::ST_DEAL_INFO*>	m_mapDeal;	// deal seq, deal info
	CRITICAL_SECTION					m_csDeal;

	//CChartMap							m_chartMap;
	CChartShmUtilFixed	*m_chartShm;
	int				m_nIdx;
	BOOL			m_bTimeSaveRun;
	//char			m_zNextCandleTm[32];

	int				m_nImAlive;
	int				m_nResultWaitSec;
};

