#pragma once

#pragma warning(disable:4996)
#include <Windows.h>
#include <list>

class CStatistics //: public CBaseThread
{
public:
	CStatistics();
	~CStatistics();

	double Average(std::list<double>* pList, int num, BOOL bAsc);
	void Var_StandardDev(std::list<double>* pList, int num, double dAvg, BOOL bAsc, 
							_Out_ double* pVar, _Out_ double *pStandDev);
	double GetStandardDeviation(double variance);
	double GetCoVariance(std::list<double>* pListX, std::list<double>* pListY, int num, double averageX, double averageY, BOOL bAsc);
	double GetPCC(double covariance, double sdX, double sdY);

private:
};
