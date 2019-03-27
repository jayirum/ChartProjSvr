#pragma once

#include "../../IRUM_UTIL/TcpClient.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "../common/FBIInc.h"
#include "../../IRUM_UTIL/ChartShmUtil.h"
#include <map>
#include <string>

/*
[DEAL ����]
DEAL_MST ���� [�ֹ�����] [������] [�������] [����] �� �ϳ��� DEAL �� �ð��̴�.
DEAL_MANAGER �� �ε��� �� DEAL_SEQ�� �� ���۽ð��� DEAL_STATUS, DURATION �� �����´�.
"�ֹ����۽ð��� KEY �� �ϴ� MAP �� ����
- ����ð���� DURATION*2 ������ �����ʹ� �����Ѵ�."
Ÿ�̸� 0.1�� ���� ����.
"1) ����ð��� �ֹ����� ~ ������ �̸� �ֹ�����
2) ����ð��� ������ ~ ������� �̸� ������
3) ����ð��� ������� ~ ���� �̸� �������
4) ���� + 5�� : TR ��û"
��Ʈ TR ���� - MAP ���� TR ��û���� �� ã�Ƽ� ó��
��� DB ó�� �� �������� Ŭ���̾�Ʈ���� ����
*/


//class CChartMap
//{
//public:
//	CChartMap();
//	~CChartMap();
//
//	VOID Save(char* pChartData);
//	_FBI::PT_API_CHART* GetFirstChart(_Out_ std::string *psChartNm);
//	_FBI::PT_API_CHART* GetNextChart(_In_ std::string sPrevChartNm, _Out_ std::string *psChartNm);
//	VOID DeleteAfterOrerProc(_In_ std::string sChartNm);
//	//std::string GetFirstChartNm() { return m_sFirstChartNm; }
//private:
//	std::map<std::string, _FBI::PT_API_CHART*>	m_mapChart;
//	CRITICAL_SECTION							m_csChart;
//	//std::string m_sFirstChartNm;
//};

//#define INTERVAL_IMALIVE	10

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
	CChartShmUtil	*m_chart;
	int				m_nIdx;
	BOOL			m_bTimeSaveRun;
	//char			m_zNextCandleTm[32];

	int				m_nImAlive;
};

