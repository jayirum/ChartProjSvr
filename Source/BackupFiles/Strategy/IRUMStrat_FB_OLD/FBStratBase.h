#pragma once

#include <stdio.h>
#include <string>
#include "../../IRUM_UTIL/QueueShm.h"
#include "../../IRUM_UTIL/ManagePosition.h"


class CFBStratBase
{
public:
	CFBStratBase(DWORD	dwAcntThreadID, std::string	sSymbol, CQueueShm	*pShmQ, CManagePosition	*pPos);
	virtual ~CFBStratBase();

	virtual VOID DoStrategy(char* pData) = 0;

protected:
	DWORD			m_dwAcntThreadID;
	std::string		m_sSymbol;
	CQueueShm		*m_pShmQ;
	CManagePosition	*m_pPos;
};