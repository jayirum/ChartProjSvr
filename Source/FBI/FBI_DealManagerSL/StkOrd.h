#pragma once

#include "../common/FBIInc.h"
#include <map>
#include <list>
#include <string>


class CStkOrd
{
public:
	CStkOrd();
	~CStkOrd();

	BOOL Initialize(_FBI::ST_STK_INFO* p);
	VOID Finalize();

	BOOL RecvOrder();

private:
	VOID OrderProc();
	VOID ScanOrdByPrc();
	unsigned int GetOppositeThreadId();
	BOOL AmIUp();

	static unsigned WINAPI Thread_OrderList(LPVOID lp);

private:
	HANDLE				m_hUp, m_hDn;
	unsigned int		m_unUp, m_unDn;

	_FBI::ST_STK_INFO	m_stkInfo;

	std::map<std::string, std::list<_FBI::ST_SLORD*> >		m_mapUp, m_mapDn;

};

