#include "FBStrat_OpenPrc.h"
#include "FBDefine.h"



CFBStrat_OpenPrc::CFBStrat_OpenPrc(DWORD	dwAcntThreadID, std::string	sSymbol, CQueueShm	*pShmQ, CManagePosition	*pPos) 
	:CFBStratBase(dwAcntThreadID, sSymbol, pShmQ, pPos)
{
}

CFBStrat_OpenPrc::~CFBStrat_OpenPrc()
{
}

VOID CFBStrat_OpenPrc::DoStrategy(char* pData)
{

}