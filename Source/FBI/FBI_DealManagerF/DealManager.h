#pragma once

#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "../common/FBIInc.h"
#include "../../IRUM_UTIL/ChartShmUtilFixed.h"
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

