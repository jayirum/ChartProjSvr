// BAUtils.cpp : Defines the exported functions for the DLL application.
//

#define _BA_UTILS_EXPORTS

#include "BAUtils.h"
#include "OrdManager.h"
#include "../../IRUM_UTIL/Log.h"
#include "SymbolPair.h"

CSymbolPair	g_symbolPair;
COrdManager g_oManager;
CLog g_log;

void  BAUtils_OMInitialize(
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
	g_oManager.AddNewOrder(
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
		, true
	);
}

void BAUtils_OMDeInitialize()
{
	//g_oManager.DeInitialize();
	g_log.Close();
}


CHANGED_RET BAUtils_OMCheckChange(
//int BAUtils_OMCheckChange(
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

int BAUtils_OMGetClosedOrd(int* arrTicket, double* arrLots)
{
	return g_oManager.GetClosedOrd(arrTicket, arrLots);
}

void	BAUtils_OMBeginCheck()
{
	g_oManager.BeginCheck();
}
int	BAUtils_OMDeletedOrderCnt()
{
	return g_oManager.DeletedOrderCnt();
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
void BAUtils_OpenLog(char* pzDir, char* pzEAName)
{
	//char path[]="C:\\BARelay";
	//GetCurrentDirectoryA(_MAX_PATH, path);
	g_log.OpenLog(pzDir, pzEAName);
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



BA_UTILS void BAUtils_SymbolPairAdd(char* pzMasterSymbol, char* pzSlaveSymbol)
{
	g_symbolPair.Add(pzMasterSymbol, pzSlaveSymbol);
}

BA_UTILS bool BAUtils_SymbolPairGet(_In_ char* pzMasterSymbol, char* _Out_ pzSlaveSymbol)
{
	return g_symbolPair.Get(pzMasterSymbol, pzSlaveSymbol);
}



//+------------------------------------------------------------+
//	Config File
//+------------------------------------------------------------+
//DWORD GetPrivateProfileString(
//	LPCTSTR lpAppName,
//	LPCTSTR lpKeyName,
//	LPCTSTR lpDefault,
//	LPTSTR  lpReturnedString,
//	DWORD   nSize,
//	LPCTSTR lpFileName
//);
bool BAUtils_GetConfig(char* i_psCnfgFileNm, char* i_psSectionNm, char* i_psKeyNm, char* o_psValue)
{
	*o_psValue = 0x00;
	DWORD dwRET = GetPrivateProfileString(i_psSectionNm, i_psKeyNm, NULL, o_psValue, 1024, (LPCSTR)i_psCnfgFileNm);
	// 주석은 제거
	char* pComment = strstr(o_psValue, "//");
	if (pComment)	*(pComment) = 0x00;

	pComment = strstr(o_psValue, "/*");
	if (pComment)	*(pComment) = 0x00;

	return (o_psValue!=NULL);
}

//BOOL WritePrivateProfileStringA(
//	LPCSTR lpAppName,
//	LPCSTR lpKeyName,
//	LPCSTR lpString,
//	LPCSTR lpFileName
//);
bool BAUtils_SetConfig(char* i_psCnfgFileNm, char* i_psSectionNm, char* i_psKeyNm, char* i_psValue)
{
	int ret = WritePrivateProfileStringA(i_psSectionNm, i_psKeyNm, i_psValue, i_psCnfgFileNm);

	return (ret != 0);
}
