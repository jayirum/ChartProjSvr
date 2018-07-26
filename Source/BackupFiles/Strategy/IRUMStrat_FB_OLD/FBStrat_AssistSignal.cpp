#include "FBStrat_AssistSignal.h"
#include "FBDefine.h"



CFBStrat_AssistSignal::CFBStrat_AssistSignal(DWORD	dwAcntThreadID, std::string	sSymbol, CQueueShm	*pShmQ, CManagePosition	*pPos)
	:CFBStratBase(dwAcntThreadID, sSymbol, pShmQ, pPos)
{
}

CFBStrat_AssistSignal::~CFBStrat_AssistSignal()
{
}


VOID CFBStrat_AssistSignal::DoStrategy(char* pData)
{

}