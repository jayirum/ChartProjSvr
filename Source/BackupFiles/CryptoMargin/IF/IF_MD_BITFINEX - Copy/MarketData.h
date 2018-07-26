#pragma once
#include <string>
#include <map>
#include <functional>
#include "Precompiled.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////
class CBaseData
{
public:
	CBaseData() {};
	virtual ~CBaseData() {};

	void SetChannelString(string sChannelString) { m_sChannelString = sChannelString; }
	void SetChannelID(__int64 nNewChannelID) { m_nChannelID = nNewChannelID; }

	string GetChannelString(){	return m_sChannelString;}
	__int64 GetChannelID(){	return m_nChannelID;}

protected:

	string	m_sChannelString;
	__int64	m_nChannelID;
};


////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _bookData
{
	double	m_dPrice; //only for cryptocurrencies
	int		m_nCount;
	double	m_dAmount;
}bookData;


class bookGroup : public CBaseData
{
public:

	bookGroup();
	virtual ~bookGroup();

	time_t GetLastReceiveTime();
	int AddOrUpdateBook(double dPrice, int nCount, double dAmount);
	void DeleteBooks();

	INT GetBidBooks(_In_ int nDepth, _Out_ double *pPrcArray, _Out_ double *pAmtArray, _Out_ int *pCntArray, _Out_ double *pTotAmt, _Out_ int *pTotCnt);
	INT GetAskBooks(_In_ int nDepth, _Out_ double *pPrcArray, _Out_ double *pAmtArray, _Out_ int *pCntArray, _Out_ double *pTotAmt, _Out_ int *pTotCnt);

	//void bookGroup::ShowBookBidData();
	//int bookGroup::DisplayAskBookAtCoords(int x, int y, int nMaxRows);
	//int DisplayBidBookAtCoords(int x, int y, int nMaxRows);

private:
	void Lock(CRITICAL_SECTION* cs) { EnterCriticalSection(cs);	}
	void Unlock(CRITICAL_SECTION* cs) { LeaveCriticalSection(cs); }

private:

	time_t		m_timeLastReceiveTime;
	map<double, bookData*, greater<double> >	m_bookBid;
	map<double, bookData*, less<double> >	m_bookAsk;
	CRITICAL_SECTION		m_csBid, m_csAsk;

	double		m_dTotAmtBid, m_dTotAmtAsk;
	int			m_nTotCntBid, m_nTotCntAsk;
};



class tickerData : public CBaseData
{
private:
	double m_dBid;
	double m_dBidSize;
	double m_dAsk;
	double m_dAskSize;
	double m_dDailyChange;
	double m_dDailyChangePercentage;
	double m_dLastPrice;
	double m_dDailyVolume;
	double m_dDailyHigh;
	double m_dDailyLow;
public:
	tickerData();
	tickerData(
		double dBid,
		double dBidSize,
		double dAsk,
		double dAskSize,
		double dDailyChange,
		double dDailyChangePercentage,
		double dLastPrice,
		double dDailyVolume,
		double dDailyHigh,
		double dDailyLow);

	int DisplayTickerAtCoords(int x, int y);
	int SetTickerData(
		double dBid,
		double dBidSize,
		double dAsk,
		double dAskSize,
		double dDailyChange,
		double dDailyChangePercentage,
		double dLastPrice,
		double dDailyVolume,
		double dDailyHigh,
		double dDailyLow);

};