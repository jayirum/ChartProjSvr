#pragma once


#pragma warning(disable:4996)

#include "BAUtils.h"
#include <map>
using namespace std;

//map<int, map<int, int> >	masterAcc - MasterTicket - SlaveTicket


typedef map<int, int>					SUB_TICKET;
typedef map<int, int>::iterator			ITOR_SUB_TICKET;
typedef map<int, SUB_TICKET >			MAP;
typedef map<int, SUB_TICKET >::iterator	ITOR_MAP;
class CSlaveTicketList
{
public:
	CSlaveTicketList();
	~CSlaveTicketList();

	void	SaveTicket( int nMasterAcc, int nMasterTicket, int nSaveTicket);

private:
	MAP	m_mapTicket;
};

