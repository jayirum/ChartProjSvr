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

	void BeginCheck();
	int	DeletedOrderCnt();


	int	AddNewOrder(
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
		, bool		bChecked
	);

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

	int GetClosedOrd(int* arrTicket, double* arrLots);


private:
	void	Clear();

private:
	map<int, MT4_ORD*>		m_mapOrdLast;
	bool					m_bDeInit;

	int						m_nUnCheckedCnt;
};

