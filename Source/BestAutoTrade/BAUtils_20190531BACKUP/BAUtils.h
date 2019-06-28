#pragma once

#ifdef _BA_UTILS_EXPORTS
#define BA_UTILS extern "C" __declspec(dllexport)
#else
#define BA_UTILS extern "C" __declspec(dllimport)
#endif

#include <string>
using namespace std;



enum CHANGED_RET {
	CHANGED_NONE = 0,
	CHANGED_POS_OPEN,
	CHANGED_POS_OPEN_ADD,
	CHANGED_POS_CLOSE_FULL,
	CHANGED_POS_CLOSE_PARTIAL,
	CHANGED_POS_OPENPRC,
	CHANGED_ORD_TYPE,
	CHANGED_OPEN_PRC,
	CHANGED_SL_PRC,
	CHANGED_PT_PRC,
	CHANGED_PENDING_NEW,
	CHANGED_PENDING_DEL,
	CHNAGED_PENDING_MOIDIFY
};




struct MT4_ORD
{
	int         ticket;
	string      symbol;
	int         type;
	double      lots;
	int			open_time;
	double      open_price;
	double      stoploss;
	double      takeprofit;
	int		    close_time;
	double      close_price;
	int		    expiration;
	double      commission;
	double      swap;
	double      profit;
	string      comment;
	int         magic;
	CHANGED_RET	changedRet;
};

//+------------------------------------------------------------+
//	COrdManager
//+------------------------------------------------------------+
BA_UTILS void  BAUtils_OMInitialize(
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
);

BA_UTILS void BAUtils_OMDeInitialize();
BA_UTILS void BAUtils_OMSetComplete();

BA_UTILS CHANGED_RET BAUtils_OMCheckChange(
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
);

BA_UTILS bool	BAUtils_OMGetClosedOrd(
	int& ticket
	//, int&		type
	, double&	lots
	//, int&		open_time
	//, double&	open_price
	//, double&	stoploss
	//, double&	takeprofit
	//, int&		close_time
	//, double&	close_price
	//, double&	commission
	//, double&	swap
	//, double&	profit
	//, char*		comment
	//, int&		magic
);


//+------------------------------------------------------------+
//	Utility
//+------------------------------------------------------------+

//yyyymmdd_hhmmssmmm	/18bytes
BA_UTILS void BAUtils_HeaderTime(_Out_ char* zTime);

//+------------------------------------------------------------+
//	CLog
//+------------------------------------------------------------+

BA_UTILS void BAUtils_OpenLog(char* pzEAName);
BA_UTILS void BAUtils_Log(char* pzData);
