#include "StratAppliedUser.h"



CStratAppliedUser::CStratAppliedUser(char *pzUserID, CQueueShm *pShm, void* pDBPool)
{
	strcpy(m_zUserID, pzUserID);
	m_pShm = pShm;
	m_pDBPool = pDBPool;
}


CStratAppliedUser::~CStratAppliedUser()
{
}
