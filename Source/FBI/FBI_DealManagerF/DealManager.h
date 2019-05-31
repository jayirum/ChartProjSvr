#pragma once

#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "../common/FBIInc.h"
#include "../../IRUM_UTIL/ChartShmUtilFixed.h"
#include <map>
#include <string>

/*
[DEAL 관리]
DEAL_MST 에는 [주문시작] [대기시작] [결과시작] [종료] 가 하나의 DEAL 의 시간이다.
DEAL_MANAGER 가 로딩할 때 DEAL_SEQ와 각 시작시간과 DEAL_STATUS, DURATION 를 가져온다.
"주문시작시간을 KEY 로 하는 MAP 을 구성
- 현재시간대비 DURATION*2 이전의 데이터는 삭제한다."
타이머 0.1초 마다 돈다.
"1) 현재시간이 주문시작 ~ 대기시작 이면 주문시작
2) 현재시간이 대기시작 ~ 결과시작 이면 대기시작
3) 현재시간이 결과시작 ~ 종료 이면 결과시작
4) 종료 + 5초 : TR 요청"
차트 TR 수신 - MAP 에서 TR 요청중인 것 찾아서 처리
결과 DB 처리 후 소켓으로 클라이언트에게 전달
*/


class CDealManager : public CBaseThread
{
public:
	CDealManager(char* pzStkCd, char* pzArtcCd, int nIdx, unsigned int	nToClientThread);
	~CDealManager();

	BOOL Initialize();
	VOID Finalize();

	BOOL InitChartShm();
	BOOL LoadDealInfo();

	VOID ThreadFunc			();
	VOID DealManage			();
	BOOL DealManageInner	();
	BOOL DealOrd			(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	BOOL DealWait			(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	BOOL DealChartWait		(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	BOOL DealResulting		(char* pzNowFull, _FBI::ST_DEAL_INFO* pInfo);
	VOID DealErase			(int nDealSeq);
	void UpdateDealStatus	(_FBI::ST_DEAL_INFO* pInfo);

	
	static unsigned WINAPI Thread_ResultProcByChart(LPVOID lp);	
	VOID SaveAndSendResult	(_FBI::ST_DEAL_INFO *pDeal, char UpDown, char* pzOpen, char* pzClose, char* pzChartNm);
	BOOL ReadChartShm		(CHART_TP ChartTp, char* pzChartNm, char* pzTmEnd, _Out_ ST_SHM_CHART_UNIT& chart, _Out_ int& refRetryCnt);
	

	static unsigned WINAPI Thread_TimeSave(LPVOID lp);
	BOOL IsTimesaveClass() { return (m_nIdx == 0); }

private:
	char			m_zStkCd[32];
	char			m_zArtcCd[32];
	CDBPoolAdo		*m_pDBPool;
	
	HANDLE			m_hRsltProc, m_hTimeSave;
	unsigned int	m_unRsltProc, m_unTimeSave;


	CChartShmUtilFixed	*m_chartShm;
	int				m_nIdx;
	BOOL			m_bTimeSaveRun;

	int				m_nImAlive;
	int				m_nResultWaitSec;

	unsigned int	m_unToClientThread;

	std::map<int, _FBI::ST_DEAL_INFO*>	m_mapDeal;	// deal seq, deal info
	CRITICAL_SECTION					m_csDeal;

};

