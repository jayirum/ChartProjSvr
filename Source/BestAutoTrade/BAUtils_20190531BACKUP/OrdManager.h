#pragma once

#pragma warning(disable:4996)

#include "BAUtils.h"
#include <map>
using namespace std;


class COrdManager
{
public:
	COrdManager();
	~COrdManager();

	
	void Initialize(
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
		, int		magic);
	void DeInitialize();
	void SetComplete();

	CHANGED_RET CheckChange(
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

	bool	GetClosedOrd(
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


private:
	void	Clear(bool bMapLast);
	CHANGED_RET CompareOrd(int ticket, MT4_ORD* pLast);

	void  AddNewList(
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
		, bool		bInit = false
	);
private:

	map<int, MT4_ORD*>		m_mapOrdLast;
	map<int, MT4_ORD*>		m_mapOrdNew;
	bool					m_bDeInit;
};

