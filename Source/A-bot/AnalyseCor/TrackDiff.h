#pragma once
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/Statistics.h"
#include <list>
#include <string>


/*
	# (EUR - GBP) 의 차(Diff) 를 대상으로 한다.
	# 계산을 시작하기 위한 타겟수량이 모이기 전까지는 Tracking 하지 않는다. ==> 일단 하자.
	# 한 종목의 tick 이 들어오면 다른 종목의 마지막 tick 과 비교한다.
	# Diff 가 벌어진 경우, 어느쪽 종목이 어떻게 변한건지 확인한다.
	  즉, 각 종목의 표준편차와 마지막 tick 의 편차도 확인을 한다. 
*/

enum {EN_EUR=0, EN_GBP};

struct VALUE
{
	double dLast;	// 가장 마지막 값
	double dSum;	// 평균계산을 위한 합
	double dAvg;	// 평균
	double dVar;	// 분산
	double dSD;		// 표준편차
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
	int				m_nTargetNum;	// 계산 기준 수량
	BOOL			m_bWaitUntilTargetNum;

};

