#include "Statistics.h"



CStatistics::CStatistics()
{
}


CStatistics::~CStatistics()
{
}


// ���
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

	
	//��� (average) : ������ ������ ������ ���� �� 
	average = float(average / num);
	return average;
}

//�л�(Variance) �� ǥ������(Standard Deviation)
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
		// ����(deviation) : ��տ��� ������ ���� ������ ����
		std::list<double>::reverse_iterator rIt;
		for (rIt = pList->rbegin(); rIt != pList->rend(); rIt++)
		{
			deviation = (*rIt) - dAvg;
			variance += deviation * deviation;
			if (++nLoop == num)
				break;
		}
	}


	// �л�(variance) : ������ �������� ���� ������ ���� ��
	*pVar = double(variance / num);
	*pStandDev = sqrt(*pVar);
	return ;
}


double CStatistics::GetStandardDeviation(double variance)
{
	double sd = 0.0;
	// ǥ������(standard deviation) : �л��� ������
	sd = sqrt(variance);
	return sd;
}

// ���л�
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
	// ���л�(covariance) : setX�� setY�� ���������� ���� ������ ���� ��
	covariance = double(covariance / num);

	delete[] setX;
	delete[] setY;
	return covariance;
}


// PCC (Pearson Correlation Coefficient) �Ǿ������
// -1 <= PCC <= 1 ������ ������ ��� ���踦 ��Ÿ����.
// 1 �� ����� ���� ���� ������ ������� ��Ÿ����.
// -1 �� ����� ���� �ݴ� ������ ������� ��Ÿ����.
// 0 �� ����� ���� ������� ������ ��Ÿ����.
double CStatistics::GetPCC(double covariance, double sdX, double sdY)
{
	double pcc = 0.0;
	// �Ǿ ������(Pearson Correlation Coefficient) : ���л��� setX�� setY�� ǥ�������� ������ ���� ��
	pcc = covariance / (sdX*sdY);
	return pcc;
}