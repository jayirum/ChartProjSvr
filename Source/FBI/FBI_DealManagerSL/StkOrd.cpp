#include "StkOrd.h"
#include <Windows.h>
#include <process.h>
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/LogMsg.h"

extern CMemPool	g_memPool;
extern CLogMsg	g_log;

CStkOrd::CStkOrd(std::string sStkCd)
{
	m_sStkCd = sStkCd;
	m_hAsc = m_hDesc = NULL;
	m_unAsc = m_unDesc = 0;
	ZeroMemory(&m_stkInfo, sizeof(m_stkInfo));

	InitializeCriticalSectionAndSpinCount(&m_csAsc, 2000);
	InitializeCriticalSectionAndSpinCount(&m_csDesc, 2000);
}


CStkOrd::~CStkOrd()
{
	Finalize();
}


BOOL CStkOrd::Initialize(_FBI::ST_STK_INFO* p, unsigned int	nSaveThreadId)
{
	m_nSaveThreadId = nSaveThreadId;
	memcpy(&m_stkInfo, p, sizeof(_FBI::ST_STK_INFO));

	// create thread
	m_hAsc = (HANDLE)_beginthreadex(NULL, 0, &Thread_OrderList, NULL, 0, &m_unAsc);
	m_hDesc = (HANDLE)_beginthreadex(NULL, 0, &Thread_OrderList, NULL, 0, &m_unDesc);

	return TRUE;
}


VOID CStkOrd::Finalize()
{
	PostThreadMessage(m_unAsc, _FBI::WM_TERMINATE, 0, 0);
	PostThreadMessage(m_unDesc, _FBI::WM_TERMINATE, 0, 0);
	
	DeleteCriticalSection(&m_csAsc);
	DeleteCriticalSection(&m_csDesc);
}

VOID CStkOrd::RelayOrdAndPrc(int nMsg, void* pData)
{
	PostThreadMessage(m_unAsc, nMsg, 0, (LPARAM)pData);
	PostThreadMessage(m_unDesc, nMsg, 0, (LPARAM)pData);
}

unsigned WINAPI CStkOrd::Thread_OrderList(LPVOID lp)
{
	CStkOrd* pThis = (CStkOrd*)lp;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case _FBI::WM_TERMINATE:
			return 0;
			break;
		case _FBI::WM_ORD_RECV:
			pThis->OrderProc((void*)msg.lParam);
			g_memPool.release((char*)msg.lParam);
			break;
		case _FBI::WM_RECV_PRC:
			pThis->ScanOrdByPrc((void*)msg.lParam);
			g_memPool.release((char*)msg.lParam);
			break;
		}
	} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))

	return 0;
}

/*
- New / Cancel Order
- Add New Order Data to both maps 
- Remove Cncl Order from both maps
	// 일반주문완료 from FBI_Order
	// 취소주문완료 from FBI_Order
*/
VOID CStkOrd::OrderProc(void* pOrdData)
{

	std::string sAscPrc, sDescPrc;
	_FBI::ST_SLORD* pOrd = (_FBI::ST_SLORD*)pOrdData;

	if (pOrd->cUpDn == 'U')
	{
		pOrd->dWinPrc = pOrd->dOrdPrc += (pOrd->nTickCnt * m_stkInfo.dTickSize);
		pOrd->dLosePrc = pOrd->dOrdPrc -= (pOrd->nTickCnt * m_stkInfo.dTickSize);

		_FBI::Dbl2Str(pOrd->dWinPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sAscPrc);
		_FBI::Dbl2Str(pOrd->dLosePrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sDescPrc);
	}
	else
	{
		pOrd->dWinPrc = pOrd->dOrdPrc -= (pOrd->nTickCnt * m_stkInfo.dTickSize);
		pOrd->dLosePrc = pOrd->dOrdPrc += (pOrd->nTickCnt * m_stkInfo.dTickSize);

		_FBI::Dbl2Str(pOrd->dWinPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sDescPrc);
		_FBI::Dbl2Str(pOrd->dLosePrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sAscPrc);
	}

	std::list<_FBI::ST_SLORD*> lstOrd;
	std::string sFirePrc;

	LockAsc();
	LockDesc();

	//일반주문이면 if() ==> 둘다 넣는다.
	if (pOrd->nOrdStatus == _FBI::ORD_STATUS_ORD)
	{
		OrderProcAdd(pOrd, sAscPrc, sDescPrc);
	}
	else if (pOrd->nOrdStatus == _FBI::ORD_STATUS_CNCL)
	{
		OrderProcCncl(pOrd, sAscPrc, sDescPrc);
	}

	UnLockAsc();
	UnLockDesc();
}

VOID CStkOrd::OrderProcAdd(_FBI::ST_SLORD* pOrd, std::string sAscPrc, std::string sDescPrc)
{
	ITMAP_UP itAsc;
	ITMAP_DN itDesc;

	std::list<_FBI::ST_SLORD*> lstOrd;

	itAsc = m_mapAsc.find(sAscPrc);
	if (itAsc != m_mapAsc.end())
	{
		lstOrd = (*itAsc).second;
	}
	lstOrd.push_back(pOrd);
	m_mapAsc[sAscPrc] = lstOrd;

	lstOrd.clear();
	itDesc = m_mapDesc.find(sDescPrc);
	if (itDesc != m_mapDesc.end())
	{
		lstOrd = (*itDesc).second;
	}
	lstOrd.push_back(pOrd);
	m_mapDesc[sDescPrc] = lstOrd;

}


BOOL CStkOrd::DeleteOrder(BOOL bAsc, std::string sOrdPrc, int nOrdNo)
{
	ITMAP_UP itAsc;
	ITMAP_DN itDesc;

	std::list<_FBI::ST_SLORD*> lstOrd;
	std::list<_FBI::ST_SLORD*>::iterator itList;

	BOOL bErased = FALSE;

	if (bAsc)
	{
		itAsc = m_mapAsc.find(sOrdPrc);
		if (itAsc == m_mapAsc.end())
		{
			//LOG
			return FALSE;
		}

		lstOrd = (*itAsc).second;
		for (itList = lstOrd.begin(); itList != lstOrd.end(); itList++)
		{
			if (nOrdNo == (*itList)->OrdNo)
			{
				delete (*itList);
				lstOrd.erase(itList);
				m_mapAsc[sOrdPrc] = lstOrd;
				bErased = TRUE;
			}
		}
	}
	else
	{
		itDesc = m_mapDesc.find(sOrdPrc);
		if (itDesc == m_mapDesc.end())
		{
			//LOG
			return FALSE;
		}

		lstOrd = (*itAsc).second;
		for (itList = lstOrd.begin(); itList != lstOrd.end(); itList++)
		{
			if (nOrdNo == (*itList)->OrdNo)
			{
				delete (*itList);
				lstOrd.erase(itList);
				m_mapDesc[sOrdPrc] = lstOrd;
				bErased = TRUE;
			}
		}
	}
	return bErased;
}

VOID CStkOrd::OrderProcCncl(_FBI::ST_SLORD* pOrd, std::string sAscPrc, std::string sDescPrc)
{
	DeleteOrder(TRUE, sAscPrc, pOrd->OrdNo);

	DeleteOrder(FALSE, sDescPrc, pOrd->OrdNo);

}

/*
- receive tick data
- scan both map
- if one order is fired in one map, remove from that map and from another map as well.
*/
VOID CStkOrd::ScanOrdByPrc(void* pPrcData)
{
	double dNowPrc = 0.;
	std::string sOrdPrc;
	
	ITMAP_UP itAsc;
	ITMAP_DN itDesc;
	std::list<_FBI::ST_SLORD*>::iterator itList;

	/*
	[ascending]

	if order price <= new price ==> fired
	*/
	if (IsThisAscThread())
	{
		for (itAsc = m_mapAsc.begin(); itAsc != m_mapAsc.end(); )
		{
			sOrdPrc = (*itAsc).first;
			int nComp = _FBI::ComparePrices(sOrdPrc, dNowPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt);
			if (nComp > 0) {
				itAsc++;
				continue;
			}

			for (itList = (*itAsc).second.begin(); itList != (*itAsc).second.end(); itList++)
			{
				_FBI::ST_SLORD* pOrd = (*itList);
				FireOrder(pOrd, sOrdPrc);
				delete pOrd;
			}
			itAsc = m_mapAsc.erase(itAsc);
		}
	}
	/*
	[descending]

	if order price >= new price ==> fired
	*/
	else
	{
		for (itDesc = m_mapDesc.begin(); itDesc != m_mapDesc.end(); )
		{
			sOrdPrc = (*itDesc).first;
			int nComp = _FBI::ComparePrices(sOrdPrc, dNowPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt);

			if (nComp < 0) {
				itDesc++;
				continue;
			}

			for (itList = (*itDesc).second.begin(); itList != (*itDesc).second.end(); itList++)
			{
				_FBI::ST_SLORD* pOrd = (*itList);
				FireOrder(pOrd, sOrdPrc);
				delete pOrd;
			}

			itDesc = m_mapDesc.erase(itDesc);

		} // for (itMap = m_mapDesc.begin(); itMap != m_mapDesc.end(); itMap++)
	}
}

/*
- Delete this order info from the opposite thread
- send to main thread to save into db and send signal to the client
  (
*/
VOID CStkOrd::FireOrder(_FBI::ST_SLORD* pOrd, std::string sFiredPrc)
{
	char* pData = g_memPool.get();
	memcpy(pData, pOrd, sizeof(_FBI::ST_SLORD));
	const int WM_ORD_FIRED = WM_USER + 519;
	PostThreadMessage(m_nSaveThreadId, WM_ORD_FIRED, sizeof(_FBI::ST_SLORD), (LPARAM)pData);

	// delete the other map
	if(IsThisAscThread())
	{
		LockDesc();
		DeleteOrder(FALSE, sFiredPrc, pOrd->OrdNo);
		UnLockDesc();
	}
	else
	{
		LockAsc();
		DeleteOrder(TRUE, sFiredPrc, pOrd->OrdNo);
		UnLockAsc();
	}
}

unsigned int CStkOrd::GetOppositeThreadId()
{
	unsigned int id = (m_unDesc == GetCurrentThreadId()) ? m_unAsc : m_unDesc;
	return id;
}
