#include "UserInfoManager.h"
#include "../../IRUM_UTIL/Util.h"

CIndividual::CIndividual()
{
	ZeroMemory(m_arrStrategy, sizeof(m_arrStrategy));
}


CIndividual::~CIndividual()
{
	Clear();
}

VOID CIndividual::Clear()
{
	std::map<CString/*�ֹ���ȣ*/, ORDER_INFO*>::iterator itOrd;
	std::map<CString/*����*/, POS_INFO*>::iterator itPos;

	// �ֹ����� ����
	for (itOrd = m_mapOrd.begin(); itOrd != m_mapOrd.end(); itOrd++)
	{
		delete (*itOrd).second;
	}

	// ���������� ����
	for (itPos = m_mapPos.begin(); itPos != m_mapPos.end(); itPos++)
	{
		delete (*itPos).second;
	}
	m_mapPos.clear();

}




VOID CUserInfoManager::Clear()
{
	std::map<CString/*����*/, CIndividual*>::iterator it;

	LOCK();
	__try
	{
		for (it = m_mapUser.begin(); it != m_mapUser.end(); it++)
		{
			delete (*it).second;
		}
		m_mapUser.clear();
	}
	__finally
	{
		UNLOCK();
	}
}


CUserInfoManager::CUserInfoManager()
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
}

CUserInfoManager::~CUserInfoManager()
{
	Clear();
	DeleteCriticalSection(&m_cs);
}


BOOL CUserInfoManager::ProcOrderApiData(char* pzAcntNo, CString& sSymbol, char* psApiBuff)
{
	lpAt0985O_st p = (lpAt0985O_st)psApiBuff;
	char cReplType = p->ReplyType[0];

	BOOL result;

	LOCK();

	__try
	{
		if (cReplType == REPL_TYPE_ACPT_ORD ||
			cReplType == REPL_TYPE_ACPT_MDFY ||
			cReplType == REPL_TYPE_ACPT_CANCEL)
		{
			result = ProcOrderInfo(pzAcntNo, sSymbol, psApiBuff);
		}

		if (cReplType == REPL_TYPE_MATCH)
		{
			result = ProcPosInfo(pzAcntNo, sSymbol, psApiBuff);
		}
	}
	__finally
	{
		UNLOCK();
	}
	return result;
}


/*

CString m_sSymbol;
CString m_sOrderNo;
char	m_cSide;	// 1:�ż�, 2:�ŵ�
CString m_sPrc;
UINT	m_nRmainQty;
CString m_sTM;
*/
BOOL CUserInfoManager::ProcOrderInfo(char* pzAcntNo, CString& sSymbol, char* psApiBuff)
{
	lpAt0985O_st p = (lpAt0985O_st)psApiBuff;
	ORDER_INFO* pOrd = NULL;
	CString sOrgNo;
	ORDER_INFO* info;
	BOOL bResult;

	char cReplType = p->ReplyType[0];

	// ����ֹ�'
	if (cReplType == REPL_TYPE_ACPT_CANCEL)
	{
		sOrgNo.Format("%.*s", sizeof(p->Orig_ord_no), p->Orig_ord_no);
		pOrd = GetOrderPtr(pzAcntNo, sOrgNo);
		if (pOrd == NULL)
		{
			return FALSE;
		}
		if (!DelOrder(pzAcntNo, sOrgNo))
			return FALSE;

		return TRUE;
	}

	info = new ORDER_INFO;
	info->sSymbol = sSymbol;
	info->sOrderNo.Format("%.*s", sizeof(p->Ord_no), p->Ord_no);
	info->cSide = p->Side[0];
	info->sPrc.Format("%.*s", sizeof(p->Price), p->Price);
	info->nRmainQty = S2N(p->RemainQty, sizeof(p->RemainQty));
	info->sTM.Format("%.*s", sizeof(p->TradeTime), p->TradeTime);

	// �����ֹ�
	if (cReplType == REPL_TYPE_ACPT_MDFY)
	{
		CString sOrgNo; sOrgNo.Format("%.*s", sizeof(p->Orig_ord_no), p->Orig_ord_no);
		pOrd = GetOrderPtr(pzAcntNo, sOrgNo);
		if (pOrd == NULL)
		{
			return FALSE;
		}

		if (!DelOrder(pzAcntNo, sOrgNo))
			return FALSE;
	}

	// save
	bResult = SaveOrder(pzAcntNo, info->sOrderNo, info);
}

BOOL CUserInfoManager::SaveOrder(CString sAcntNo, CString& sOrdNo, ORDER_INFO* pOrd)
{
	CIndividual* p;
	std::map<CString, CIndividual*>::iterator it;
	std::map<CString, ORDER_INFO*>::iterator itOrd;
	it = m_mapUser.find(sAcntNo);
	if (it == m_mapUser.end())
	{
		m_sMsg.Format("[�ֹ������������] (%s)���������� ����", (LPCSTR)sAcntNo);
		return FALSE;
	}

	p = (*it).second;
	p->m_mapOrd[sOrdNo] = pOrd;
	m_mapUser[sAcntNo] = p;
	return TRUE;
}


ORDER_INFO* CUserInfoManager::GetOrderPtr(char* pzAcntNo, CString& sOrdNo)
{
	CString sAcntNo(pzAcntNo);
	CIndividual* p;
	std::map<CString, CIndividual*>::iterator it;
	std::map<CString, ORDER_INFO*>::iterator itOrd;

	it = m_mapUser.find(sAcntNo);
	if (it == m_mapUser.end())
	{
		m_sMsg.Format("[�ֹ�������ȸ����] (%s)���������� ����", pzAcntNo);
		return NULL;
	}

	p = (*it).second;
	itOrd = p->m_mapOrd.find(sOrdNo);
	if (itOrd == p->m_mapOrd.end())
	{
		m_sMsg.Format("[����� �ֹ����� ����](����:%s)(�ֹ���ȣ:%s)", pzAcntNo, (LPCSTR)sOrdNo);
		return NULL;
	}
	return (*itOrd).second;
}


BOOL CUserInfoManager::DelOrder(CString sAcntNo, CString& sOrdNo)
{
	std::map<CString, CIndividual*>::iterator it;
	std::map<CString/*�ֹ���ȣ*/, ORDER_INFO*>::iterator itOrd;
	CIndividual* p;

	it = m_mapUser.find(sAcntNo);
	if (it == m_mapUser.end())
	{
		m_sMsg.Format("[�ֹ����� ��ȸ����] (%s)���������� ����", (LPCSTR)sAcntNo);
		return FALSE;
	}

	p = (*it).second;
	itOrd = p->m_mapOrd.find(sOrdNo);
	if (itOrd == p->m_mapOrd.end())
	{
		m_sMsg.Format("[�ֹ����� ����](����:%s)(�ֹ���ȣ:%s)", (LPCSTR)sAcntNo, (LPCSTR)sOrdNo);
		return FALSE;
	}
	delete (*itOrd).second;
	p->m_mapOrd.erase(itOrd);
	return TRUE;
}




/*

CString sSymbol;
char	cSide;	// 1:�ż�, 2:�ŵ�
CString sPrc;	// ��մܰ�
UINT	nRmainQty;
*/
BOOL CUserInfoManager::ProcPosInfo(char* pzAcntNo, CString& sSymbol, char* psApiBuff)
{
	lpAt0985O_st p = (lpAt0985O_st)psApiBuff;
	POS_INFO* pPos = GetPosPtr(pzAcntNo, sSymbol);
	if (pPos == NULL)
		return FALSE;

	pPos->sSymbol = sSymbol;
	pPos->cSide = p->Open_tp[0];
	pPos->sAvgPrc.Format("%.*s", sizeof(p->AvgExecAmt), p->AvgExecAmt);
	pPos->nPosQty = S2N(p->Open_q, sizeof(p->Open_q));
	
	return SavePos(pzAcntNo, sSymbol, pPos);
}

POS_INFO* CUserInfoManager::GetPosPtr(char* pzAcntNo, CString& sSymbol)
{
	CString sAcntNo(pzAcntNo);
	CIndividual* p;
	std::map<CString, CIndividual*>::iterator it;
	std::map<CString, POS_INFO*>::iterator itPos;

	it = m_mapUser.find(sAcntNo);
	if (it == m_mapUser.end())
	{
		m_sMsg.Format("[������������ȸ����] (%s)���������� ����", pzAcntNo);
		return NULL;
	}

	p = (*it).second;
	itPos = p->m_mapPos.find(sSymbol);
	if (itPos == p->m_mapPos.end())
	{
		m_sMsg.Format("[����� ���������� ����](����:%s)(����:%s)", pzAcntNo, (LPCSTR)sSymbol);
		return NULL;
	}
	return (*itPos).second;

}

BOOL CUserInfoManager::DelPosPtr(CString& sAcntNo, CString& sSymbol)
{
	CIndividual* p;
	std::map<CString, CIndividual*>::iterator it;
	std::map<CString, POS_INFO*>::iterator itPos;

	it = m_mapUser.find(sAcntNo);
	if (it == m_mapUser.end())
	{
		m_sMsg.Format("[������������������] (%s)���������� ����", (LPCSTR)sAcntNo);
		return FALSE;
	}

	p = (*it).second;
	itPos = p->m_mapPos.find(sSymbol);
	if (itPos == p->m_mapPos.end())
	{
		m_sMsg.Format("[������������������](����:%s)(����:%s)", (LPCSTR)sAcntNo, (LPCSTR)sSymbol);
		return FALSE;
	}
	return TRUE;
}



BOOL CUserInfoManager::SavePos(CString sAcntNo, CString& sSymbol, POS_INFO* pPos)
{
	CIndividual* p;
	std::map<CString, CIndividual*>::iterator it;
	std::map<CString/*����*/, POS_INFO*>::iterator itPos;

	it = m_mapUser.find(sAcntNo);
	if (it == m_mapUser.end())
	{
		m_sMsg.Format("[�����������������] (%s)���������� ����", (LPCSTR)sAcntNo);
		return FALSE;
	}

	p = (*it).second;
	p->m_mapPos[sSymbol] = pPos;
	m_mapUser[sAcntNo] = p;
	return TRUE;
}

/*
CString m_sAcntNo;
CString m_sConnPwd;
CString m_sAcntPwd;
CString m_sOrdSymbol;		// �ڵ��ֹ��� ����

UINT	m_arrStrategy[STRATEGY_CNT];	
*/
BOOL CUserInfoManager::SetAcntInfo(char* pzAcntNo, char* pzConnPwd, char* pzAcntPwd)
{
	LOCK();
	CString sAcntNo(pzAcntNo);
	CIndividual* pIndiv;
	std::map<CString/*����*/, CIndividual*>::iterator it = m_mapUser.find(sAcntNo);

	if (it != m_mapUser.end())
	{
		pIndiv = new CIndividual;
	}
	else
	{
		pIndiv = (*it).second;
	}
	pIndiv->m_sAcntNo = sAcntNo;
	pIndiv->m_sConnPwd = pzConnPwd;
	pIndiv->m_sAcntPwd = pzAcntPwd;

	m_mapUser[sAcntNo] = pIndiv;

	UNLOCK();
}

BOOL CUserInfoManager::SetStrategy(char* pzAcntNo, STRATEGY_TP tp)
{
	BOOL result = TRUE;
	CString sAcntNo(pzAcntNo);
	CIndividual* pIndiv;
	std::map<CString/*����*/, CIndividual*>::iterator it = m_mapUser.find(sAcntNo);

	LOCK();
	if (it != m_mapUser.end())
	{
		m_sMsg.Format("������������ �������� ��ȸ ����(%s)", pzAcntNo);
		result = FALSE;
	}
	else
	{
		pIndiv = (*it).second;
		pIndiv->m_arrStrategy[tp] = TRUE;
	}

	return result;
}

BOOL CUserInfoManager::SetOrderSymbol(char* pzAcntNo, char* pzSymbol)
{
	BOOL result = TRUE;
	CString sAcntNo(pzAcntNo);
	CIndividual* pIndiv;
	std::map<CString/*����*/, CIndividual*>::iterator it = m_mapUser.find(sAcntNo);

	LOCK();
	if (it != m_mapUser.end())
	{
		m_sMsg.Format("������������ �������� ��ȸ ����(%s)", pzAcntNo);
		result = FALSE;
	}
	else
	{
		pIndiv = (*it).second;
		pIndiv->m_sOrdSymbol = CString(pzSymbol);
	}

	return result;
}