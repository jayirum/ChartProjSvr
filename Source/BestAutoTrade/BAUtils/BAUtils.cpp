// BAUtils.cpp : Defines the exported functions for the DLL application.
//

#define _BA_UTILS_EXPORTS

#include "BAUtils.h"
#include "OrdManager.h"
#include "../../IRUM_UTIL/Log.h"

COrdManager g_oManager;
CLog g_log;

void  BAUtils_Initialize(
	int		ticket
	, int		type
	, double	lots
	, int		open_time
	, double	open_price
	, double	stoploss
	, double	takeprofit
	, int		close_time
	, double	close_price
	, double	commission
	, double	swap
	, double	profit
	, char*		comment
	, int		magic
)
{
	g_oManager.Initialize(
		ticket
		, type
		, lots
		, open_time
		, open_price
		, stoploss
		, takeprofit
		, close_time
		, close_price
		, commission
		, swap
		, profit
		, comment
		, magic
	);
}

void BAUtils_DeInitialize()
{
	g_oManager.DeInitialize();
	g_log.Close();
}

void BAUtils_SetComplete()
{
	g_oManager.SetComplete();
}

CHANGED_RET BAUtils_CheckChange(
	int		ticket
	, int		type
	, double	lots
	, int		open_time
	, double	open_price
	, double	stoploss
	, double	takeprofit
	, int		close_time
	, double	close_price
	, double	commission
	, double	swap
	, double	profit
	, char*		comment
	, int		magic
	, /*out*/int&		oOpenedTicket
	, /*out*/double&	oOpenedPrc
	, /*out*/double&	oOpenedLots
	, /*out*/int&		oOpenedTm
)
{
	return g_oManager.CheckChange(
		ticket
		, type
		, lots
		, open_time
		, open_price
		, stoploss
		, takeprofit
		, close_time
		, close_price
		, commission
		, swap
		, profit
		, comment
		, magic
		, oOpenedTicket
		, oOpenedPrc
		, oOpenedLots
		, oOpenedTm
	);
}

bool BAUtils_GetClosedOrd(
	int& ticket
	, int&		type
	, double&	lots
	, int&		open_time
	, double&	open_price
	, double&	stoploss
	, double&	takeprofit
	, int&		close_time
	, double&	close_price
	, double&	commission
	, double&	swap
	, double&	profit
	, char*		comment
	, int&		magic
)
{
	return g_oManager.GetClosedOrd(
		ticket
		, type
		, lots
		, open_time
		, open_price
		, stoploss
		, takeprofit
		, close_time
		, close_price
		, commission
		, swap
		, profit
		, comment
		, magic
	);
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//void BAUtils_OpenLog(char* pzEAName);
void BAUtils_OpenLog(char* pzEAName)
{
	char path[]="C:\\BARelay";
	//GetCurrentDirectoryA(_MAX_PATH, path);
	g_log.OpenLog(path, pzEAName);
	strcpy(pzEAName, g_log.GetFileName());
}

void BAUtils_Log(char* pzData)
{
	g_log.Log(pzData);
}



//yyyymmdd_hhmmssmmm	/18bytes
BA_UTILS void BAUtils_HeaderTime(_Out_ char* zTime)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(zTime, "%04d%02d%02d_%02d%02d%02d%03d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}