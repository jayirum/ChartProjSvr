#include "stdafx.h"
#include "StopOrd.h"


char STOP_TP_S[5][20] = { "�ð���ġ","�����ż�","�����ŵ�","SIGNAL����","���԰���" };

CStopOrd::CStopOrd()
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
}


CStopOrd::~CStopOrd()
{
	Clear();
	DeleteCriticalSection(&m_cs);
}


VOID CStopOrd::Clear()
{
	MAPIT_ORD it;
	LOCK();
	for (it = m_mapOrd.begin(); it != m_mapOrd.end(); it++)
	{
		delete (*it).second;
	}
	m_mapOrd.clear();
	UNLOCK();
}

INT CStopOrd::AddNewOrd(_In_ CString* sSymbol, 
	_In_ char* pzPrc, _In_ char cSide, _In_ int nQty,
	_In_ char* psChartNm, _In_ STOPORD_TP stopOrdTp, _Out_ int *pnTotCnt)
{
	// �̹� ���� ����, ���� ���� �ִ��� CHECK
	MAPIT_ORD it;
	int nResult = -1;
	LOCK();
	STOPORD_TP tp;

	__try
	{
		for (it = m_mapOrd.begin(); it != m_mapOrd.end(); it++)
		{
			STOPORD_INFO* pStop = (STOPORD_INFO*)(*it).second;

			tp = (*it).first;
			
			if (pStop->cSide[0] == cSide)
			{
				if (strcmp(pzPrc, pStop->zPrc) == 0)
				{
					sprintf(m_zMsg, "(������,CHART:%s,Side:%c,Prc:%10.5f,STOP tp:%s)(�ű�,Side:%c,Prc:%10.5f,STOP tp:%s)",
						pStop->zChartNm, pStop->cSide[0], atof(pStop->zPrc), STOP_TP_S[(int)tp-1],
						cSide, atof(pzPrc), STOP_TP_S[(int)stopOrdTp-1]);

					nResult = 0;
					__leave;
				}
			}
		} // for (it = m_mapOrd.begin(); it != m_mapOrd.end(); it++)

		STOPORD_INFO *pStopInfo = new STOPORD_INFO;
		ZeroMemory(pStopInfo, sizeof(STOPORD_INFO));
		strcpy(pStopInfo->zPrc, pzPrc);
		pStopInfo->cSide[0] = cSide;
		pStopInfo->nQty = nQty;
		sprintf(pStopInfo->zChartNm, "%.*s", LEN_CHART_NM, psChartNm);
		pStopInfo->stopOrdTp = stopOrdTp;
		strcpy(pStopInfo->zSymbol, sSymbol->GetBuffer());
		pStopInfo->dSetClock = GetTickCount();

		m_mapOrd[stopOrdTp] = pStopInfo;
		*pnTotCnt = m_mapOrd.size();
		nResult = *pnTotCnt;
	}
	__finally
	{
		UNLOCK();
	}

	return nResult;
}

BOOL CStopOrd::IsStopOrdExists(_In_ STOPORD_TP stopOrdTp)
{
	MAPIT_ORD itOrd = m_mapOrd.find(stopOrdTp);
	return (itOrd != m_mapOrd.end());
}



INT CStopOrd::RemoveOrder(_In_ STOPORD_TP stopOrdTp)
{
	INT nRemoveCnt = 0;
	LOCK();
	MAPIT_ORD itOrd = m_mapOrd.find(stopOrdTp);
	if (itOrd != m_mapOrd.end())
	{
		STOPORD_INFO* p = (*itOrd).second;
		delete p;
		m_mapOrd.erase(stopOrdTp);
		nRemoveCnt++;
	}

	// ������ǥ�� ���� �ݴ���⵵ ���ÿ� �����Ѵ�.
	if (stopOrdTp == STOPORD_ASSIST_BUY || stopOrdTp == STOPORD_ASSIST_SELL)
	{
		STOPORD_TP tp = (stopOrdTp == STOPORD_ASSIST_BUY) ? STOPORD_ASSIST_SELL : STOPORD_ASSIST_BUY;
		itOrd = m_mapOrd.find(tp);
		if (itOrd != m_mapOrd.end())
		{
			delete (*itOrd).second;
			m_mapOrd.erase(tp);
			nRemoveCnt++;
		}
	}
	UNLOCK();
	return nRemoveCnt;
}

int CStopOrd::RemoveAllOrder()
{
	INT nRemoveCnt = 0;
	LOCK();
	MAPIT_ORD itOrd;
	for (itOrd=m_mapOrd.begin(); itOrd != m_mapOrd.end(); itOrd++)
	{
		STOPORD_INFO* p = (*itOrd).second;
		delete p;
		nRemoveCnt++;
	}
	m_mapOrd.clear();
	UNLOCK();

	return nRemoveCnt;
}

int CStopOrd::RemoveOrdWhenOtherFired(_In_ STOPORD_TP stopOrdTpFired, _In_ char cSideFired)
{
	INT nRemoveCnt = 0;
	MAPIT_ORD itOrd;
	
	LOCK();
	for (itOrd = m_mapOrd.begin(); itOrd != m_mapOrd.end(); )
	{
		STOPORD_INFO* p = (*itOrd).second;
		if (p->cSide[0] == cSideFired)
		{
			delete p;
			itOrd = m_mapOrd.erase(itOrd);
			nRemoveCnt++;
		}
		else
		{
			if(itOrd != m_mapOrd.end())
				itOrd++;
		}
	}
	UNLOCK();

	return nRemoveCnt;
}

/*
return
	-1 : ����
	0 : �ִµ� ������ǥ�̰� ���Ժ��� ���� ��
	1 : ����
*/
BOOL CStopOrd::CheckFire(
	_In_ CString& sSymbol, _In_ char* pzCurrPrc, const char* pzOpenChartNm, const char* pzNowChartNm,
	_Out_ char* pzPrc, _Out_ char* pzSide, _Out_ int* pnQty, _Out_ STOPORD_TP* pStopOrd)
{
	CString sPrc = pzCurrPrc;

	LOCK();
	//MAPIT_ORD itOrd = m_mapOrd.find(sSymbol);
	//if (itOrd == m_mapOrd.end())
	//{
	//	UNLOCK();
	//	return FALSE;
	//}
	//STOPORD_INFO* p = (*itOrd).second;

	if (m_mapOrd.size() == 0)
	{
		UNLOCK();
		return FALSE;
	}

	BOOL bFireResult = FALSE;
	MAPIT_ORD itOrd;
	STOPORD_TP	tp;
	for (itOrd = m_mapOrd.begin(); itOrd != m_mapOrd.end(); itOrd++)
	{
		tp = (*itOrd).first;
		STOPORD_INFO* p = (*itOrd).second;

		// �ֹ��� <= ���簡 �̸� �ߵ�
		if( (p->cSide[0] == DEF_BUY)&& (strcmp(p->zPrc, pzCurrPrc) <= 0))
		{
			bFireResult = TRUE;
		}

		// �ֹ��� >= ���簡 �̸� �ߵ�
		if ((p->cSide[0] == DEF_SELL)&& (strcmp(p->zPrc, pzCurrPrc) >= 0))
		{
			bFireResult = TRUE;
		}

		if (bFireResult )
		{
			strcpy(pzPrc, p->zPrc);
			*pzSide = p->cSide[0];
			*pnQty = p->nQty;
			*pStopOrd = p->stopOrdTp;
			delete p;
			m_mapOrd.erase(itOrd);

			// ������ǥ�� ���� �ݴ���⵵ ����ó��
			if (tp == STOPORD_ASSIST_BUY || tp == STOPORD_ASSIST_SELL)
			{
				STOPORD_TP tp2 = (tp == STOPORD_ASSIST_BUY) ? STOPORD_ASSIST_SELL : STOPORD_ASSIST_BUY;
				itOrd = m_mapOrd.find(tp2);
				if (itOrd != m_mapOrd.end())
				{
					delete (*itOrd).second;
					m_mapOrd.erase(tp2);
				}
			}
			break;
		}

	} // for (itOrd = m_mapOrd.begin(); itOrd != m_mapOrd.end(); itOrd++)

	
	UNLOCK();

	return bFireResult;
}
