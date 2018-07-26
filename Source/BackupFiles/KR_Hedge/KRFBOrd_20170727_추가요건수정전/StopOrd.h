#pragma once

#include "stdafx.h"
#include "../Common/KRCommon.h"
#include <map>


#define MAP_ORD		std::map<STOPORD_TP, STOPORD_INFO*>		// symbol,
#define MAPIT_ORD	std::map<STOPORD_TP, STOPORD_INFO*>::iterator		// symbol,

typedef struct _STOPORD_INFO
{
	char zPrc[LEN_PRC + 1];
	char cSide[1];
	int nQty;
	char zChartNm[LEN_CHART_NM + 1];
	STOPORD_TP stopOrdTp;
	char zSymbol[LEN_SYMBOL+1];
	double dSetClock;	//1분30초 점검위한 clock
	//BOOL bFired;
}STOPORD_INFO;

class CStopOrd
{
public:
	CStopOrd();
	~CStopOrd();

	INT AddNewOrd(_In_ CString* sSymbol, _In_ char* pzPrc, _In_ char cSide, _In_ int nQty,
						_In_ char* psChartNm, _In_ STOPORD_TP stopOrdTp, _Out_ int *pnTotCnt);
	BOOL CheckFire(_In_ CString& sSymbol, _In_ char* pzCurrPrc, const char* pzOpenChartNm, const char* pzNowChartNm, _Out_ char* pzPrc, 
						_Out_ char* pzSide, _Out_ int* pnQty, _Out_ STOPORD_TP* pStopOrd);
	INT RemoveOrder(_In_ STOPORD_TP stopOrdTp);
	INT RemoveAllOrder();
	BOOL IsStopOrdExists(_In_ STOPORD_TP stopOrdTp);
	int RemoveOrdWhenOtherFired(_In_ STOPORD_TP stopOrdTpFired, _In_ char cSideFired);
	char* GetMsg() { return m_zMsg; }
private:
	VOID Clear();

	VOID LOCK() { EnterCriticalSection(&m_cs); };
	VOID UNLOCK() { LeaveCriticalSection(&m_cs); };

private:
	CRITICAL_SECTION	m_cs;
	MAP_ORD		m_mapOrd;
	char		m_zMsg[1024];
};

