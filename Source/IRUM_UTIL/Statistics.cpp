#include "Statistics.h"



CStatistics::CStatistics()
{
}


CStatistics::~CStatistics()
{
}


// 평균
double CStatistics::Average(std::list<double>* pList, int num, BOOL bAsc)
{
	double average = 0.0;
	int i = 0;
	int nLoop = 0;
	if (bAsc)
	{
		std::list<double>::iterator it;
		for (it = pList->begin(); it != pList->end(); it++)
		{
			average += (*it);
			if (++nLoop == num)
				break;
		}
	}
	else
	{
		std::list<double>::reverse_iterator rIt;
		for (rIt = pList->rbegin(); rIt != pList->rend(); rIt++)
		{
			average += (*rIt);
			if (++nLoop == num)
				break;
		}
	}

	
	//평균 (average) : 수들의 총합을 개수로 나눈 수 
	average = float(average / num);
	return average;
}

//분산(Variance) 과 표준편차(Standard Deviation)
void CStatistics::Var_StandardDev(std::list<double>* pList, int num, double dAvg, BOOL bAsc
									,_Out_ double* pVar, _Out_ double *pStandDev)
{
	double deviation = 0.0;
	double variance = 0.0;
	double standDev = 0.0;
	int i = 0;

	int nLoop = 0;
	if (bAsc)
	{
		std::list<double>::iterator it;
		for (it = pList->begin(); it != pList->end(); it++)
		{
			deviation = (*it) - dAvg;
			variance += deviation * deviation;
			if (++nLoop == num)
				break;
		}
	}
	else
	{
		// 편차(deviation) : 평균에서 각각의 수가 떨어진 정도
		std::list<double>::reverse_iterator rIt;
		for (rIt = pList->rbegin(); rIt != pList->rend(); rIt++)
		{
			deviation = (*rIt) - dAvg;
			variance += deviation * deviation;
			if (++nLoop == num)
				break;
		}
	}


	// 분산(variance) : 편차의 제곱들의 합을 개수로 나눈 수
	*pVar = double(variance / num);
	*pStandDev = sqrt(*pVar);
	return ;
}


double CStatistics::GetStandardDeviation(double variance)
{
	double sd = 0.0;
	// 표준편차(standard deviation) : 분산의 제곱근
	sd = sqrt(variance);
	return sd;
}

// 공분산
double CStatistics::GetCoVariance(std::list<double>* pListX, std::list<double>* pListY, int num,
	double averageX, double averageY, BOOL bAsc)
{
	double *setX = new double[pListX->size()];
	double *setY = new double[pListY->size()];

	std::copy(pListX->begin(), pListX->end(), setX);
	std::copy(pListY->begin(), pListY->end(), setY);

	double deviation = 0.0;
	double covariance = 0.0;
	int i = 0;
	for (i = 0; i<num; i++)
	{
		covariance += (setX[i] - averageX) * (setY[i] - averageY);
	}
	// 공분산(covariance) : setX와 setY의 편차곱들의 합을 개수로 나눈 수
	covariance = double(covariance / num);

	delete[] setX;
	delete[] setY;
	return covariance;
}


// PCC (Pearson Correlation Coefficient) 피어슨상관계수
// -1 <= PCC <= 1 사이의 값으로 상관 관계를 나타낸다.
// 1 에 가까울 수록 같은 방향의 상관성을 나타낸다.
// -1 에 가까울 수록 반대 방향의 상관성을 나타낸다.
// 0 에 가까울 수록 상관성이 없음을 나타낸다.
double CStatistics::GetPCC(double covariance, double sdX, double sdY)
{
	double pcc = 0.0;
	// 피어슨 상관계수(Pearson Correlation Coefficient) : 공분산을 setX와 setY의 표준편차의 곱으로 나눈 수
	pcc = covariance / (sdX*sdY);
	return pcc;
}