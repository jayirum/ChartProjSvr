#pragma once

#include "../../IRUM_UTIL/BaseThread.h"

class CMonitor : public CBaseThread
{
public:
	CMonitor();
	~CMonitor();


	virtual VOID	ThreadFunc();

};

