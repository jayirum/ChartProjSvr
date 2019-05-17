#include "ManageLimitOrd.h"
#include "Util.h"


CManageLimitOrd::CManageLimitOrd()
{
	InitializeCriticalSection(&m_cs);
}


CManageLimitOrd::~CManageLimitOrd()
{
	DeleteCriticalSection(&m_cs);

}

VOID CManageLimitOrd::SetSymbolInfo(std::string sSymbol, double dTickSize, int nDotCnt)
{
	m_sSymbol = sSymbol;
	m_dTickSize = dTickSize;
	m_nDotCnt = nDotCnt;
}

int CManageLimitOrd::Add(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, 
						char cSide, char* pzPrc, int nTotProfitCutTick,
						BOOL bIsPosOpened, char cPosSide, char* pzPosPrc)
{
	ST_LIMIT_ORDER* p = new ST_LIMIT_ORDER;

	sprintf(p->zChartNm, "%.*s", LEN_CHART_NM, pzChartNm);
	p->nOrdNo = nOrdNo;
	p->nOrigNo = nOrigNo;
	p->nRemainQty = nRemainQty;
	p->cSide[0] = cSide;
	strcpy(p->zPrc, pzPrc);
	p->bProfitOrder = FALSE;

	// �������� �ִ� ��� �� �ֹ��� ���� �ֹ����� Ȯ���Ѵ�. 
	// �����ֹ��� ��Ҵ���� �ƴϹǷ� ���⿡ �߰����� �ʴ´�.
	if (bIsPosOpened)
	{
		if (cPosSide != cSide)
		{
			double dOpenPrc = atof(pzPosPrc);
			double dOrdPrc = atof(pzPrc);
			int nTickGap = 0;

			if (cSide == DEF_BUY)
				nTickGap = CUtil::GetTickGap(dOrdPrc, dOpenPrc, m_nDotCnt, m_dTickSize);
			else
				nTickGap = CUtil::GetTickGap(dOpenPrc, dOrdPrc, m_nDotCnt, m_dTickSize);

			if (nTickGap == nTotProfitCutTick)
				p->bProfitOrder = TRUE;
		}
	}
	LOCK_CS(m_cs);
	m_mapLimit[nOrdNo] = p;
	UNLOCK_CS(m_cs);

	return m_mapLimit.size();
}

INT CManageLimitOrd::Modify(char* pzChartNm, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char* pzPrc)
{
	LOCK_CS(m_cs);
	MAPIT_LIMITORD it = m_mapLimit.find(nOrigNo);
	if (it == m_mapLimit.end()) {
		UNLOCK_CS(m_cs);
		return -1;
	}

	// �����ֹ���ȣ ����� ���ֹ���ȣ�� ����
	delete (*it).second;
	m_mapLimit.erase(nOrigNo);

	ST_LIMIT_ORDER* p = new ST_LIMIT_ORDER;

	sprintf(p->zChartNm, pzChartNm, LEN_CHART_NM);
	p->nOrdNo = nOrdNo;
	p->nOrigNo = nOrigNo;
	p->nRemainQty = nRemainQty;
	p->cSide[0] = cSide;
	strcpy(p->zPrc, pzPrc);

	m_mapLimit[nOrdNo] = p;

	UNLOCK_CS(m_cs);

	return m_mapLimit.size();
}


int CManageLimitOrd::Cancel(_In_ INT nOrigNo)
{
	LOCK_CS(m_cs);
	MAPIT_LIMITORD it = m_mapLimit.find(nOrigNo);
	if (it == m_mapLimit.end()) {
		UNLOCK_CS(m_cs);
		return -1;
	}

	// �����ֹ���ȣ ����� ���ֹ���ȣ�� ����
	delete (*it).second;
	m_mapLimit.erase(nOrigNo);

	UNLOCK_CS(m_cs);

	return m_mapLimit.size();
}


//BOOL CManageLimitOrd::LimitOrd_GetFirstOrd(_Out_ ST_LIMIT_ORDER* pOrdInfo)
//{
//	LimitOrd_Lock();
//	if (mapLimitOrd.size() == 0) {
//		LimitOrd_UnLock();
//		return FALSE;
//	}
//
//	MAPIT_LIMITORD it = mapLimitOrd.begin();
//	CopyMemory(pOrdInfo, (*it).second, sizeof(ST_LIMIT_ORDER));
//	delete (*it).second;
//	mapLimitOrd.erase(it);
//
//	LimitOrd_UnLock();
//	return TRUE;
//}
//
//
//// ���纸�� ������Ʈ���� �� �ֹ� ����ϱ� ���� ���� ��ȯ
//BOOL CManageLimitOrd::LimitOrd_GetOrdTobeCanceled(_In_ char* pzNowChartNm, _In_ int nPosQty, _In_ char cPosSide, _Out_ ST_LIMIT_ORDER* p)
//{
//	LimitOrd_Lock();
//	if (mapLimitOrd.size() == 0) {
//		LimitOrd_UnLock();
//		return FALSE;
//	}
//
//	MAPIT_LIMITORD it;
//	for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
//	{
//		ST_LIMIT_ORDER* pInfo = (*it).second;
//
//		// �ش�Ǹ� ��� ��ȯ
//		if (strncmp(pInfo->zChartNm, pzNowChartNm, LEN_CHART_NM) < 0)
//		{
//			if (pInfo->bProfitOrder)
//				continue;
//
//			// û���ֹ��� ���ܳ��´�.
//			if (nPosQty > 0)
//			{
//				if (cPosSide != pInfo->cSide[0])
//					continue;
//			}
//
//			CopyMemory(p, pInfo, sizeof(ST_LIMIT_ORDER));
//			delete pInfo;
//			mapLimitOrd.erase(it);
//			return TRUE;
//		}
//	}
//
//	LimitOrd_UnLock();
//
//	return FALSE;
//}



// �������� �ֹ� ���
//int CManageLimitOrd::Cancel_SameSide(_In_ char cSide, _Out_ int *pnOrdNo)
//{
//	int nCnt = 0;
//	LOCK_CS(m_cs);
//	if (m_mapLimit.size() == 0) {
//		UNLOCK_CS(m_cs);
//		return 0;
//	}
//
//	MAPIT_LIMITORD it;
//	for (it = m_mapLimit.begin(); it != m_mapLimit.end(); it++)
//	{
//		ST_LIMIT_ORDER* pInfo = (*it).second;
//
//		// �ش�Ǹ� �ֹ���ȣ ����
//		if (pInfo->cSide[0] == cSide)
//		{
//			pnOrdNo[nCnt++] = pInfo->nOrdNo;
//		}
//	}
//
//	UNLOCK_CS(m_cs);
//
//	return FALSE;
//}


//BOOL CManageLimitOrd::IsExistSameSideLimitOrd(_In_ char cSide)
//{
//	LimitOrd_Lock();
//	MAPIT_LIMITORD it;
//	for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
//	{
//		ST_LIMIT_ORDER* p = (*it).second;
//		if (p->cSide[0] == cSide) {
//			LimitOrd_UnLock();
//			return TRUE;
//		}
//	}
//	LimitOrd_UnLock();
//	return FALSE;
//}
//
//
///*
//���� ������ �� ���� ������ �ֹ��� �ִ��� ����
//*/
//BOOL CManageLimitOrd::LimitOrd_FindBetterOrd(_In_ char cSide, char* pzPrc, _Out_ int* pnOrdNo, _Out_ char* pzOrdPrc)
//{
//	LimitOrd_Lock();
//	MAPIT_LIMITORD it;
//
//	__try
//	{
//		for (it = mapLimitOrd.begin(); it != mapLimitOrd.end(); it++)
//		{
//			ST_LIMIT_ORDER* p = (*it).second;
//			if (p->cSide[0] != cSide)
//				continue;
//
//			int nComp = CUtil::CompPrc(p->zPrc, strlen(p->zPrc), pzPrc, strlen(pzPrc), nDotCnt, LEN_PRC);
//
//			// �ż�, ������ ���� >= �񱳴��
//			if (cSide == DEF_BUY && nComp >= 0)
//			{
//				*pnOrdNo = p->nOrdNo;
//				strcpy(pzOrdPrc, p->zPrc);
//				return TRUE;
//			}
//
//			// �ŵ�, ������ ���� <= �񱳴��
//			if (cSide == DEF_SELL && nComp <= 0)
//			{
//				*pnOrdNo = p->nOrdNo;
//				strcpy(pzOrdPrc, p->zPrc);
//				return TRUE;
//			}
//		}
//	}
//	__finally {
//		LimitOrd_UnLock();
//	}
//
//
//	return FALSE;
//}


VOID CManageLimitOrd::Clear()
{
	MAPIT_LIMITORD it;

	LOCK_CS(m_cs);
	for (it = m_mapLimit.begin(); it != m_mapLimit.end(); it++)
	{
		delete (*it).second;
	}

	m_mapLimit.clear();
	UNLOCK_CS(m_cs);
}