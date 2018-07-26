#pragma once

#include "FBStratBase.h"


class CFBStrat_OpenPrc : public CFBStratBase
{
public:
	CFBStrat_OpenPrc(DWORD	dwAcntThreadID, std::string	sSymbol, CQueueShm	*pShmQ, CManagePosition	*pPos);
	virtual ~CFBStrat_OpenPrc();

	virtual VOID DoStrategy(char* pData);
};