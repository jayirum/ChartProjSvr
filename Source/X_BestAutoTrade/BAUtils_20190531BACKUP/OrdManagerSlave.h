#pragma once

#pragma warning(disable:4996)

#include "BAUtils.h"
#include <map>
using namespace std;




class COrdManagerSlave
{
public:
	COrdManagerSlave();
	~COrdManagerSlave();

	
	void LoadOrdInfo(
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
	void UnLoadInfo();

	bool GetOrdInfo(
		_In_ int ticket
		, _Out_ int&		type
		, _Out_ double&	lots
		, _Out_ int&		open_time
		, _Out_ double&	open_price
		, _Out_ double&	stoploss
		, _Out_ double&	takeprofit
		, _Out_ int&		close_time
		, _Out_ double&	close_price
		, _Out_ double&	commission
		, _Out_ double&	swap
		, _Out_ double&	profit
		, _Out_ char*		comment
		, _Out_ int&		magic
	);

	bool DelOrdInfo(int ticket);

private:
	void	Clear();
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
	);
private:

	map<int, MT4_ORD*>		m_mapOrdLast;
	//map<int, MT4_ORD*>		m_mapOrdNew;
	bool					m_bDeInit;
};

