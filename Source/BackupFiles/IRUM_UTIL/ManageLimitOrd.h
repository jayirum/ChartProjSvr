#pragma once


#include "../IRUM_INC/IRUM_Common.h"
#include <stdio.h>
#include <string>
#include <map>
#include <Windows.h>


typedef struct _ST_LIMIT_ORDER
{
	char zChartNm[LEN_CHART_NM + 1];	// 한번에 한종목만 거래 가능하다.
	int nOrdNo;
	int nOrigNo;
	int nRemainQty;
	char cSide[1];
	char zPrc[LEN_PRC + 1];
	BOOL bProfitOrder;					// 익절주문인가?
}ST_LIMIT_ORDER;

typedef std::map<INT, ST_LIMIT_ORDER*>				MAP_LIMITORD;				// ORDNO, LIMIT_ORDER
typedef std::map<INT, ST_LIMIT_ORDER*>::iterator	MAPIT_LIMITORD;				// ORDNO, LIMIT_ORDER


class CManageLimitOrd
{
public:
	CManageLimitOrd();
	virtual ~CManageLimitOrd();

	VOID	SetSymbolInfo(std::string sSymbol, double dTickSize, int nDotCnt);
	int		Add(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, 
				char* pzPrc, int nTotProfitCutTick,	BOOL bIsPosOpened, char cPosSide, char* pzPosPrc);
	INT		Modify(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char* pzPrc);
	int		Cancel(_In_ INT nOrigNo);
	VOID	Clear();

	//int		Cancel_SameSide(_In_ char cSide, _Out_ int *pnOrdNo);
	//BOOL	GetFirstOrd(_Out_ ST_LIMIT_ORDER* pOrdInfo);
	//BOOL	GetOrdTobeCanceled(_In_ char* pzNowChartNm, _In_ int nPosQty, _In_ char cPosSide, _Out_ ST_LIMIT_ORDER* p);
	//BOOL	IsExistSameSideLimitOrd(_In_ char cSide);
	//BOOL	FindBetterOrd(_In_ char cSide, _In_ char* pzPrc, _Out_ int* pnOrdNo, _Out_ char* pzOrdPrc);


private:
	MAP_LIMITORD		m_mapLimit;
	CRITICAL_SECTION	m_cs;

	std::string			m_sSymbol;
	double				m_dTickSize;
	int					m_nDotCnt;
};