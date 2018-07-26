#pragma once

#include "../../IRUM_UTIL/QueueShm.h"

class CStratAppliedUser
{
public:
	CStratAppliedUser(char *pzUserID, CQueueShm *pShm, void* pDBPool);
	~CStratAppliedUser();


protected:
	// Get strategy data of user at first
	BOOL	GetStratInfo();


protected:
	char		m_zUserID[64];
	CQueueShm	*m_pShm;
	void		*m_pDBPool;
};

