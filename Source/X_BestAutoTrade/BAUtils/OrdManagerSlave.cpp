#include "OrdManagerSlave.h"
#include "../../IRUM_UTIL/Log.h"

extern CLog g_log;

COrdManagerSlave::COrdManagerSlave()
{
	m_bDeInit = false;
}


COrdManagerSlave::~COrdManagerSlave()
{
	UnLoadInfo();
}

void COrdManagerSlave::Clear()
{
	map<int, MT4_ORD*>::iterator it;
	for (it = m_mapOrdLast.begin(); it != m_mapOrdLast.end(); it++)
	{
		delete (*it).second;
	}
	m_mapOrdLast.clear();
}


void COrdManagerSlave::LoadOrdInfo(
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

	AddNewList(
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

bool COrdManagerSlave::DelOrdInfo(int ticket)
{
	if (m_mapOrdLast.size() == 0)
		return false;

	map<int, MT4_ORD*>::iterator it = m_mapOrdLast.find(ticket);
	if (it == m_mapOrdLast.end())
		return false;

	delete (*it).second;
	m_mapOrdLast.erase(ticket);
}

bool COrdManagerSlave::GetOrdInfo(
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
)
{
	if (m_mapOrdLast.size() == 0)
		return false;

	map<int, MT4_ORD*>::iterator it = m_mapOrdLast.find(ticket);
	if (it == m_mapOrdLast.end())
		return false;

	MT4_ORD* p = (MT4_ORD*)(*it).second;
	type = p->type;
	lots = p->lots;
	open_time = p->open_time;
	open_price = p->open_price;
	stoploss = p->stoploss;
	takeprofit = p->takeprofit;
	close_time = p->close_time;
	close_price = p->close_price;
	commission = p->commission;
	swap = p->swap;
	profit = p->profit;
	magic = p->magic;
	strcpy(comment, p->comment.c_str());

	g_log.Log("[BAUTILS][GetOrdInfo](OpenedTicket:%d)(Type:%d)(OpenPrc:%f)(OpenLots:%f)(OpenTM:%d)",
		ticket, type, open_price, lots, open_time);

	return true;
}


void COrdManagerSlave::UnLoadInfo()
{
	if (!m_bDeInit) {
		Clear();
	}
	m_bDeInit = true;
}



/*
	add order into the New Map
*/
void  COrdManagerSlave::AddNewList(
	int			ticket
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
	MT4_ORD* p = new MT4_ORD;
	ZeroMemory(p, sizeof(MT4_ORD));
	p->ticket = ticket;
	p->type = type;
	p->lots = lots;
	p->comment = comment;
	p->open_time = open_time;
	p->open_price = open_price;
	p->stoploss = stoploss;
	p->takeprofit = takeprofit;
	p->close_time = close_time;
	p->close_price = close_price;
	p->swap = swap;
	p->profit = profit;
	p->magic = magic;

	m_mapOrdLast[ticket] = p;
}

//bool COrdManagerSlave::FindOrdInfo(
//	int		ticket
//	, int&		type
//	, double&	lots
//	, char*		comment
//)
//{
//	bool find = false;
//	map<int, MT4_ORD*>::iterator it = m_mapOrd.find(ticket);
//
//	if (it != m_mapOrd.end())
//	{
//		find = true;
//
//		MT4_ORD* p = (MT4_ORD*)(*it).second;
//		type = p->type;
//		lots = p->lots;
//		strcpy(comment, p->comment.c_str());
//	}
//	return find;
//}
//
//
//
//void COrdManagerSlave::GetOrdInfo(
//	int&		ticket
//	, int&		type
//	, double&	lots
//	, int&		ticket2
//	, int&		type2
//	, double&	lots2)
//{
//	map<int, MT4_ORD*>::iterator it = m_mapOrd.begin();
//	MT4_ORD* p = (MT4_ORD*)(*it).second;
//
//	ticket = p->ticket;
//	type = p->type;
//	lots = p->lots;
//	
//	char zMsg[512];
//	sprintf(zMsg, "ticket[%d], type[%d], lots[%f]\n",
//		p->ticket, p->type, p->lots);
//	g_log.log(zMsg);
//
//	it++;
//	MT4_ORD* p2 = (MT4_ORD*)(*it).second;
//
//	ticket2 = p2->ticket;
//	type2 = p2->type;
//	lots2 = p2->lots;
//	sprintf(zMsg, "ticket[%d], type[%d], lots[%f]\n",
//		p2->ticket, p2->type, p2->lots);
//	g_log.log(zMsg);
//
//}

//int  COrdManagerSlave::CheckOrderChanged(
//	int			totalCnt
//	, int		ticket
//	, int		type
//	, double	lots
//	, int		open_time
//	, double	open_price
//	, double	stoploss
//	, double	takeprofit
//	, int		close_time
//	, double	close_price
//	, double	commission
//	, double	swap
//	, double	profit
//	, char*		comment
//	, int		magic
//)
//{
//
//}