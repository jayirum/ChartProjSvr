#pragma once
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "../common/FBIInc.h"
#include "../../IRUM_UTIL/ChartShmUtil.h"
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


class CChartMap
{
public:
	CChartMap();
	~CChartMap();

	VOID Save(char* pChartData);
	_FBI::PT_API_CHART* GetFirstChart(_Out_ std::string *psChartNm);
	_FBI::PT_API_CHART* GetNextChart(_In_ std::string sPrevChartNm, _Out_ std::string *psChartNm);
	VOID DeleteAfterOrerProc(_In_ std::string sChartNm);
	//std::string GetFirstChartNm() { return m_sFirstChartNm; }
private:
	std::map<std::string, _FBI::PT_API_CHART*>	m_mapChart;
	CRITICAL_SECTION							m_csChart;
	//std::string m_sFirstChartNm;
};

class CDealManager : public CBaseThread
{
public:
	CDealManager(char* pzStkCd, char* pzArtcCd);
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
	void UpdateDeal(_FBI::ST_DEAL_INFO* pInfo);

	static unsigned WINAPI Thread_ResultProcByChart(LPVOID lp);
	
private:
	char	m_zStkCd[32], m_zArtcCd[32];
	CDBPoolAdo		*m_pDBPool;
	
	HANDLE			m_hRsltProc;
	unsigned int	m_unRsltProc;

	std::map<int, _FBI::ST_DEAL_INFO*>	m_mapDeal;	// deal seq, deal info
	CRITICAL_SECTION					m_csDeal;

	//CChartMap							m_chartMap;
	CChartShmUtil						*m_chart;
};

