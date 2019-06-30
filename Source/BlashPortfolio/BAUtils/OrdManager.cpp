#include "OrdManager.h"
#include "../../IRUM_UTIL/Log.h"

extern CLog g_log;

COrdManager::COrdManager()
{
	m_bDeInit = false;
}


COrdManager::~COrdManager()
{
}

void COrdManager::Clear()
{
	map<int, MT4_ORD*>::iterator it;
	if (m_mapOrdLast.size() > 0)
	{
		for (it = m_mapOrdLast.begin(); it != m_mapOrdLast.end(); it++)
		{
			delete (*it).second;
		}
		m_mapOrdLast.clear();
		//g_log.Log("[BAUTILS][Clear]Last Map");
	}
}


int COrdManager::AddNewOrder(
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
)
{
	MT4_ORD* p = new MT4_ORD;
	ZeroMemory(p, sizeof(MT4_ORD));
	p->ticket		= ticket;
	p->type			= type;
	p->lots			= lots;
	p->comment		= comment;
	p->open_time	= open_time;
	p->open_price	= open_price;
	p->stoploss		= stoploss;
	p->takeprofit	= takeprofit;
	p->close_time	= close_time;
	p->close_price	= close_price;
	p->swap			= swap;
	p->profit		= profit;
	p->magic		= magic;
	p->bChecked		= bChecked;

	
	m_mapOrdLast[ticket] = p;
	g_log.Log("[BAUTILS][AddNewOrder][Total:%d]ticket;%d, type:%d, lots:%f, open_time:%d, open_prc:%f, sl:%f, tp:%f, close_time:%d, close_prc:%f, comment:%s"
		, m_mapOrdLast.size()
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
	return m_mapOrdLast.size();
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
	// this order already exists in the Map Last?
	map<int, MT4_ORD*>::iterator it = m_mapOrdLast.find(ticket);
	if (it == m_mapOrdLast.end())
	{
		AddNewOrder(ticket, type, lots, open_time, open_price, stoploss, takeprofit, 
			close_time, close_price, commission,swap, profit, comment, magic, 
			true  // mark as checked in order not to recognize as deleted order
		);
		return CHANGED_POS_OPEN;
	}
	
	CHANGED_RET	ret;
	MT4_ORD *pLast = (*it).second;
	//memcpy(&stLast, (*it).second, sizeof(stLast));

	// Compare Order
	oOpenedTicket	= pLast->ticket;
	oOpenedPrc		= pLast->open_price;
	oOpenedLots		= pLast->lots;
	oOpenedTm		= pLast->open_time;
	
	// lots
	if (pLast->lots > lots && lots>0) {
		g_log.Log("[BAUTILS][Partial](OpenedTicket:%d)(OpendedPrc:%f)(OpenedLots:%f)(OpendedTM:%d)(CloseTicket:%d)(ClosePrc:%f)(CloseLots:%f)(CloseTM:%d)",
			pLast->ticket, pLast->open_price, pLast->lots, pLast->open_time,
			ticket, open_price, lots, open_time);
		pLast->lots = lots;
		ret = CHANGED_POS_CLOSE_PARTIAL;
	}
	else if (pLast->lots < lots) {
		g_log.Log("[BAUTILS][Open Add](OpenedTicket:%d)(OpendedPrc:%f)(OpenedLots:%f)(OpendedTM:%d)",
			pLast->ticket, pLast->open_price, pLast->lots, pLast->open_time);
		pLast->lots = lots;
		ret = CHANGED_POS_OPEN_ADD;
	}
	if (pLast->type != type)
	{
		g_log.Log("[BAUTILS][Changed:Type](OpenedTicket:%d)(OldType:%d)(NewType:%d)",pLast->ticket, pLast->type, type);
		pLast->type = type;
		ret = CHANGED_ORD_TYPE;
	}
	if (pLast->open_price != open_price)
	{
		g_log.Log("[BAUTILS][Changed:Open Price](OpenedTicket:%d)(OldPrc:%f)(NewPrc:%f)", pLast->ticket, pLast->open_price, open_price);
		pLast->open_price = open_price;
		ret = CHANGED_OPEN_PRC;
	}
	if (pLast->stoploss != stoploss)
	{
		g_log.Log("[BAUTILS][Changed:SL Price](OpenedTicket:%d)(OldSLPrc:%f)(NewSLPrc:%f)", pLast->ticket, pLast->stoploss, stoploss);
		pLast->stoploss = stoploss;
		ret = CHANGED_SL_PRC;
	}
	if (pLast->takeprofit != takeprofit)
	{
		g_log.Log("[BAUTILS][Changed:TP Price](OpenedTicket:%d)(OldTPPrc:%f)(NewTPPrc:%f)",pLast->ticket, pLast->takeprofit, takeprofit);
		pLast->takeprofit = takeprofit;
		ret = CHANGED_PT_PRC;
	}
	
	m_nUnCheckedCnt--;
	//g_log.Log("[BAUTILS]Checked! (tiket:%d)(UnCheckedCnt:%d)", pLast->ticket, m_nUnCheckedCnt); 
	pLast->bChecked = true;
	m_mapOrdLast[ticket] = pLast;
	return ret;
}


int	COrdManager::DeletedOrderCnt() 
{ 
	return m_nUnCheckedCnt; 
}
/*
return
	if the ticket is in Last map & is not in new  ==> close
*/
int COrdManager::GetClosedOrd(int* arrTicket, double* arrLots)
{
	if (m_mapOrdLast.size() == 0) {
		//g_log.log("[BAUTILS]GetClosedOrd  mapsize == 0 ");
		return 0;
	}

	int nDeletedCnt = 0;
	map<int, MT4_ORD*>::iterator itLast;
	for (itLast = m_mapOrdLast.begin(); itLast != m_mapOrdLast.end();)
	{	
		MT4_ORD* p = (*itLast).second;
		if (p->bChecked == true) 
		{
			p->bChecked = false;
			m_mapOrdLast[(*itLast).first] = p;
			++itLast;
			continue;
		}

		arrTicket[nDeletedCnt]	= p->ticket;
		arrLots[nDeletedCnt]	= p->lots;
		
		g_log.Log("[BAUTILS][DELETED][%d](OpenedTicket:%d)(Type:%d)(OpenPrc:%f)(OpenLots:%f)(OpenTM:%d)",
			nDeletedCnt, p->ticket, p->type, p->open_price, p->lots, p->open_time);

		nDeletedCnt++;
		delete p;
		itLast = m_mapOrdLast.erase(itLast);
		m_nUnCheckedCnt--;
	}

	return nDeletedCnt;
}

void COrdManager::BeginCheck()
{
	map<int, MT4_ORD*>::iterator itLast;
	for (itLast = m_mapOrdLast.begin(); itLast != m_mapOrdLast.end(); itLast++)
	{
		((*itLast).second)->bChecked = false;
		m_mapOrdLast[(*itLast).first] = (*itLast).second;
	}
	m_nUnCheckedCnt = m_mapOrdLast.size();
	
}
