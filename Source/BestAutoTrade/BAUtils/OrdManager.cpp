#include "OrdManager.h"
#include "../../IRUM_UTIL/Log.h"

extern CLog g_log;

COrdManager::COrdManager()
{
	m_bDeInit = false;
}


COrdManager::~COrdManager()
{
	DeInitialize();
}

void COrdManager::Clear(bool bMapLast)
{
	map<int, MT4_ORD*>::iterator it;
	if (bMapLast)
	{
		if (m_mapOrdLast.size() > 0) {
			for (it = m_mapOrdLast.begin(); it != m_mapOrdLast.end(); it++)
			{
				delete (*it).second;
			}
			m_mapOrdLast.clear();
		}
	}
	else
	{
		if (m_mapOrdNew.size() > 0) {
			for (it = m_mapOrdNew.begin(); it != m_mapOrdNew.end(); it++)
			{
				delete (*it).second;
			}
			m_mapOrdNew.clear();
		}
	}
}

void COrdManager::SetComplete()
{
	bool bMapLast = true;
	Clear(bMapLast);
	map<int, MT4_ORD*>::iterator it;
	for (it = m_mapOrdNew.begin(); it != m_mapOrdNew.end(); it++)
	{
		MT4_ORD* p = new MT4_ORD;
		memcpy(p, (*it).second, sizeof(MT4_ORD));
		m_mapOrdLast[(*it).first] = p;
	}

	//m_mapOrdLast.insert(m_mapOrdNew.begin(), m_mapOrdNew.end());
	Clear(!bMapLast);
	//g_log.Log("[SetComplete]Last size:%d, New Size:%d", m_mapOrdLast.size(), m_mapOrdNew.size());
}

void COrdManager::Initialize(
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
		, true
	);
}


/*
return
	
*/
CHANGED_RET COrdManager::CheckChange(
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
		, false
	);

	// this order already exists in the MapLast?
	map<int, MT4_ORD*>::iterator it = m_mapOrdLast.find(ticket);
	if (it == m_mapOrdLast.end())
	{
		g_log.Log("[BAUTILS]New Order is Opened(Ticket:%d)(Type:%d)(Prc:%f)(Lots:%f)(TM:%d)",
			ticket, type, open_price, lots, open_time);
		return CHANGED_POS_OPEN;
	}
	
	MT4_ORD stLast;
	memcpy(&stLast, (*it).second, sizeof(stLast));

	// yes, it exists. Remove from MapLast
	delete (*it).second;
	m_mapOrdLast.erase(ticket);

	// Compare Order
	oOpenedTicket	= stLast.ticket;
	oOpenedPrc		= stLast.open_price;
	oOpenedLots		= stLast.lots;
	oOpenedTm		= stLast.open_time;
	return CompareOrd(ticket, &stLast);

}

CHANGED_RET COrdManager::CompareOrd(int ticket, MT4_ORD* pLast)
{
	map<int, MT4_ORD*>::iterator it = m_mapOrdNew.find(ticket);
	if (it == m_mapOrdNew.end())
	{
		g_log.Log("[BAUTILS][CompareOrd] ticket is not in New Map!!!!!(%d)", ticket);
	}
	MT4_ORD* pNew = (MT4_ORD*)(*it).second;

	// lots
	if (pLast->lots > pNew->lots) {
		g_log.Log("[BAUTILS][Partial](OpenedTicket:%d)(OpendedPrc:%f)(OpenedLots:%f)(OpendedTM:%d)(CloseTicket:%d)(ClosePrc:%f)(CloseLots:%f)(CloseTM:%d)",
			pLast->ticket, pLast->open_price, pLast->lots, pLast->open_time,
			pNew->ticket, pNew->open_price, pNew->lots, pNew->open_time);
		return CHANGED_POS_CLOSE_PARTIAL;
	}
	else if (pLast->lots < pNew->lots) {
		g_log.Log("[BAUTILS][Open Add](OpenedTicket:%d)(OpendedPrc:%f)(OpenedLots:%f)(OpendedTM:%d)",
			pLast->ticket, pLast->open_price, pLast->lots, pLast->open_time);
		return CHANGED_POS_OPEN_ADD;
	}
	if (pLast->type != pNew->type)
	{
		g_log.Log("[BAUTILS][Changed:Type](OpenedTicket:%d)(OldType:%d)(NewType:%d)",
			pLast->ticket, pLast->type, pNew->type);
		return CHANGED_ORD_TYPE;
	}
	if (pLast->open_price != pNew->open_price)
	{
		g_log.Log("[BAUTILS][Changed:Open Price](OpenedTicket:%d)(OldPrc:%f)(NewPrc:%f)",
			pLast->ticket, pLast->open_price, pNew->open_price);
		return CHANGED_OPEN_PRC;
	}
	if (pLast->stoploss != pNew->stoploss)
	{
		g_log.Log("[BAUTILS][Changed:SL Price](OpenedTicket:%d)(OldSLPrc:%f)(NewSLPrc:%f)",
			pLast->ticket, pLast->stoploss, pNew->stoploss);
		return CHANGED_SL_PRC;
	}
	if (pLast->takeprofit != pNew->takeprofit)
	{
		g_log.Log("[BAUTILS][Changed:TP Price](OpenedTicket:%d)(OldTPPrc:%f)(NewTPPrc:%f)",
			pLast->ticket, pLast->takeprofit, pNew->takeprofit);
		return CHANGED_PT_PRC;
	}
	
	return CHANGED_NONE;
}



/*
return
	true : next data exists
*/
bool COrdManager::GetClosedOrd(
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
	ticket = -1;
	
	if (m_mapOrdLast.size() == 0)
		return false;

	map<int, MT4_ORD*>::iterator it = m_mapOrdLast.begin();
	ticket = (*it).first;
	MT4_ORD* p = (MT4_ORD*)(*it).second;
	type		= p->type;
	lots		= p->lots;
	open_time	= p->open_time;
	open_price	= p->open_price;
	stoploss	= p->stoploss;
	takeprofit	= p->takeprofit;
	close_time	= p->close_time;
	close_price = p->close_price;
	commission	= p->commission;
	swap		= p->swap;
	profit		= p->profit;
	magic		= p->magic;
	strcpy(comment, p->comment.c_str());
	
	delete p;
	m_mapOrdLast.erase(ticket);

	g_log.Log("[BAUTILS][DELETED](OpenedTicket:%d)(Type:%d)(OpenPrc:%f)(OpenLots:%f)(OpenTM:%d)",
		ticket, type, open_price, lots, open_time);


	return (m_mapOrdLast.size() > 0);
}

void COrdManager::DeInitialize()
{
	if (!m_bDeInit) {
		Clear(true);
		Clear(false);
	}
	m_bDeInit = true;
}



/*
	add order into the New Map
*/
void  COrdManager::AddNewList(
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
	, bool		bInit	// true when is called in Initialize
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

	if (bInit) {
		g_log.Log("[BAUTILS][Init]ticket;%d, type:%d, lots:%f, open_time:%d, open_prc:%f, sl:%f, tp:%f, close_time:%d, close_prc:%f, comment:%s"
			, ticket
			, type
			, lots
			, open_time
			, open_price
			, stoploss
			, takeprofit
			, close_time
			, close_price
			, comment
		);
		m_mapOrdLast[ticket] = p;
	}
	else {
		m_mapOrdNew[ticket] = p;
	}

}

//bool COrdManager::FindOrdInfo(
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
//void COrdManager::GetOrdInfo(
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

//int  COrdManager::CheckOrderChanged(
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