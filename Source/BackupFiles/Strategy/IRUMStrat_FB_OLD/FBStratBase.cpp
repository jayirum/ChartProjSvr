#include "FBStratBase.h"
#include "FBDefine.h"



CFBStratBase::CFBStratBase(DWORD	dwAcntThreadID, std::string	sSymbol, CQueueShm	*pShmQ, CManagePosition	*pPos)
{
	m_dwAcntThreadID = dwAcntThreadID;
	m_sSymbol = sSymbol;
	m_pShmQ = pShmQ;
	m_pPos = pPos;
}

CFBStratBase::~CFBStratBase()
{
}