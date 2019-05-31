#include "SlaveTicketList.h"



CSlaveTicketList::CSlaveTicketList()
{
}


CSlaveTicketList::~CSlaveTicketList()
{
}




void	CSlaveTicketList::SaveTicket(int nMasterAcc, int nSaveTicket, int nMasterTicket)
{
	ITOR_MAP it = m_mapTicket.find(nMasterAcc);
	if (it != m_mapTicket.end())
	{
		SUB_TICKET itSub = (*it).second;
	}
}