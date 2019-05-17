#pragma once
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/Statistics.h"
#include <list>
#include <string>


/*
	# (EUR - GBP) �� ��(Diff) �� ������� �Ѵ�.
	# ����� �����ϱ� ���� Ÿ�ټ����� ���̱� �������� Tracking ���� �ʴ´�. ==> �ϴ� ����.
	# �� ������ tick �� ������ �ٸ� ������ ������ tick �� ���Ѵ�.
	# Diff �� ������ ���, ����� ������ ��� ���Ѱ��� Ȯ���Ѵ�.
	  ��, �� ������ ǥ�������� ������ tick �� ������ Ȯ���� �Ѵ�. 
*/

enum {EN_EUR=0, EN_GBP};

struct VALUE
{
	double dLast;	// ���� ������ ��
	double dSum;	// ��հ���� ���� ��
	double dAvg;	// ���
	double dVar;	// �л�
	double dSD;		// ǥ������
	std::list<double>	listData;
};


class CTrackDiff : public CBaseThread
{
public:
	CTrackDiff();
	~CTrackDiff();

	BOOL Initialize();
	VOID Finalize();

	VOID ThreadFunc();
	VOID MainProcess(char* pApiTick);

	VOID TrackLogging(char* pzStkCd, double dTick, VALUE* pMyVal);
	BOOL IsTrackLog(VALUE* pVal);
	VOID AddNewTick(double dTick, VALUE* pVal);

private:

	CStatistics		m_stat;
	
	VALUE			m_eur, m_gbp, m_diff;

	// Config
	int				m_nTargetNum;	// ��� ���� ����
	BOOL			m_bWaitUntilTargetNum;

};

