#pragma once

#include "FBStratBase.h"


class CFBStrat_AssistSignal : public CFBStratBase
{
public:
	CFBStrat_AssistSignal(DWORD	dwAcntThreadID, std::string	sSymbol, CQueueShm	*pShmQ, CManagePosition	*pPos);
	virtual ~CFBStrat_AssistSignal();

	virtual VOID DoStrategy(char* pData);

};