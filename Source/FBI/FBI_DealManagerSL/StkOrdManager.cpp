#include "StkOrdManager.h"
#include <Windows.h>
#include <process.h>
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/LogMsg.h"

extern CMemPool	g_memPool;
extern CLogMsg	g_log;

CStkOrdManager::CStkOrdManager(std::string sStkCd)
{
	m_sStkCd = sStkCd;
	m_hAsc = m_hDesc = NULL;
	m_unAsc = m_unDesc = 0;
	ZeroMemory(&m_stkInfo, sizeof(m_stkInfo));

	InitializeCriticalSectionAndSpinCount(&m_csAsc, 2000);
	InitializeCriticalSectionAndSpinCount(&m_csDesc, 2000);
}


CStkOrdManager::~CStkOrdManager()
{
	Finalize();
}


BOOL CStkOrdManager::Initialize(_FBI::ST_STK_INFO* p, unsigned int	nSaveThreadId)
{
	m_nSaveThreadId = nSaveThreadId;
	memcpy(&m_stkInfo, p, sizeof(_FBI::ST_STK_INFO));

	// create thread
	m_hAsc = (HANDLE)_beginthreadex(NULL, 0, &OrderHandlerThread, this, 0, &m_unAsc);
	m_hDesc = (HANDLE)_beginthreadex(NULL, 0, &OrderHandlerThread, this, 0, &m_unDesc);

	return TRUE;
}


VOID CStkOrdManager::Finalize()
{
	PostThreadMessage(m_unAsc, _FBI::WM_TERMINATE, 0, 0);
	PostThreadMessage(m_unDesc, _FBI::WM_TERMINATE, 0, 0);
	
	DeleteCriticalSection(&m_csAsc);
	DeleteCriticalSection(&m_csDesc);
}



VOID CStkOrdManager::RelayOrdAndPrc(int nMsg, void* pData, int nDataSize)
{
	char* p1 = g_memPool.get();
	memcpy(p1, pData, nDataSize);
	PostThreadMessage(m_unAsc, nMsg, 0, (LPARAM)p1);

	char* p2 = g_memPool.get();
	memcpy(p2, pData, nDataSize);
	PostThreadMessage(m_unDesc, nMsg, 0, (LPARAM)p2);
}
char* CStkOrdManager::GetThreadName(char* pzName) 
{ 
	if (IsThisAscThread())	strcpy(pzName, "ASC__Thread");
	else strcpy(pzName, "DESC_Thread");
	return pzName;
}

unsigned WINAPI CStkOrdManager::OrderHandlerThread(LPVOID lp)
{
	CStkOrdManager* pThis = (CStkOrdManager*)lp;
	char zName[32];
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case _FBI::WM_TERMINATE:
			return 0;
			break;
		case _FBI::WM_ORD_RECV:
			//g_log.log(INFO, "[%s]Hander Recv Ord(%s)", pThis->GetThreadName(zName), (char*)msg.lParam);
			pThis->OrderProc((void*)msg.lParam);
			g_memPool.release((char*)msg.lParam);
			break;
		case _FBI::WM_RECV_PRC:
			_FBI::SMPACK_FX_EXEC* pNowPrc = (_FBI::SMPACK_FX_EXEC*)(char*)msg.lParam;
			//g_log.log(INFO, "[%s]Hander Recv Prd(%s)", pThis->GetThreadName(zName), pNowPrc->issue);
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

- Only one Thread can Add / Cancel Order
*/
VOID CStkOrdManager::OrderProc(void* pOrdData)
{
	// Only one Thread can Add / Cancel Order
	if (IsThisAscThread())
		return;

	std::string sAscPrc, sDescPrc;
	_FBI::ST_SLORD* pOrd = (_FBI::ST_SLORD*)pOrdData;
	pOrd->sArtcCd = m_stkInfo.sArtcCd;
	pOrd->sStkCd = m_stkInfo.sStkCd;

	if (pOrd->cUpDn == 'U')
	{
		pOrd->dWinPrc = pOrd->dOrdPrc + (pOrd->nTickCnt * m_stkInfo.dTickSize);
		pOrd->dLosePrc = pOrd->dOrdPrc - (pOrd->nTickCnt * m_stkInfo.dTickSize);

		_FBI::Dbl2Str(pOrd->dWinPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sAscPrc);
		_FBI::Dbl2Str(pOrd->dLosePrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sDescPrc);
		pOrd->sWinPrc = sAscPrc;
		pOrd->sLosePrc = sDescPrc;
	}
	else
	{
		pOrd->dWinPrc = pOrd->dOrdPrc - (pOrd->nTickCnt * m_stkInfo.dTickSize);
		pOrd->dLosePrc = pOrd->dOrdPrc + (pOrd->nTickCnt * m_stkInfo.dTickSize);

		_FBI::Dbl2Str(pOrd->dWinPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sDescPrc);
		_FBI::Dbl2Str(pOrd->dLosePrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sAscPrc);
		pOrd->sWinPrc = sDescPrc;
		pOrd->sLosePrc = sAscPrc;
	}

	char zName[32];
	g_log.log(INFO, "[%s][RECV ORD](SYMBOL:%s)(OrdStatus:%d)(U/D:%c)(OrdPrc:%.5f)(WinPrc:%.5f)(LosePrc:%.5f)",
		GetThreadName(zName), pOrd->sStkCd.c_str(), pOrd->nOrdStatus, pOrd->cUpDn, pOrd->dOrdPrc, pOrd->dWinPrc, pOrd->dLosePrc);

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


VOID CStkOrdManager::OrderProcAdd(_FBI::ST_SLORD* pIn, std::string sAscPrc, std::string sDescPrc)
{
	//+-----------------------------------------
	//	Ascending Map
	//+-----------------------------------------

	_FBI::ST_SLORD* pAsc = new _FBI::ST_SLORD;
	CopyMemory(pAsc, pIn, sizeof(_FBI::ST_SLORD));
	std::list<_FBI::ST_SLORD*> lstOrdAsc;
	ITMAP_UP itAsc = m_mapAsc.find(sAscPrc);
	if (itAsc != m_mapAsc.end())
	{
		lstOrdAsc = (*itAsc).second;
		g_log.log(INFO, "[ADD Next ORD ASC ](OrdNo:%d)(OrdPrc:%f)(AscPrc:%s)(UpDn:%c)", 
			pIn->OrdNo, pIn->dOrdPrc, sAscPrc.c_str(), pIn->cUpDn);
	}
	else
		g_log.log(INFO, "[ADD 1st ORD ASC ](OrdNo:%d)(OrdPrc:%f)(AscPrc:%s)(UpDn:%c)",
			pIn->OrdNo, pIn->dOrdPrc, sAscPrc.c_str(), pIn->cUpDn);

	lstOrdAsc.push_back(pAsc);
	m_mapAsc[sAscPrc] = lstOrdAsc;



	//+-----------------------------------------
	//	Descending Map
	//+-----------------------------------------
	_FBI::ST_SLORD* pDesc = new _FBI::ST_SLORD;
	CopyMemory(pDesc, pIn, sizeof(_FBI::ST_SLORD));

	std::list<_FBI::ST_SLORD*> lstOrdDesc;

	ITMAP_DN itDesc = m_mapDesc.find(sDescPrc);
	if (itDesc != m_mapDesc.end())
	{
		lstOrdDesc = (*itDesc).second;
		g_log.log(INFO, "[ADD Next ORD DESC](OrdNo:%d)(OrdPrc:%f)(DescPrc:%s)(UpDn:%c)",
			pIn->OrdNo, pIn->dOrdPrc, sDescPrc.c_str(), pIn->cUpDn);
	}
	else
		g_log.log(INFO, "[ADD 1st ORD DESC](OrdNo:%d)(OrdPrc:%f)(DescPrc:%s)(UpDn:%c)",
			pIn->OrdNo, pIn->dOrdPrc, sDescPrc.c_str(), pIn->cUpDn);

	lstOrdDesc.push_back(pDesc);
	m_mapDesc[sDescPrc] = lstOrdDesc;
}

BOOL CStkOrdManager::DeleteOrder(EN_DIRECTION enDirection, std::string sOrdPrc, int nOrdNo)
{
	std::list<_FBI::ST_SLORD*> lstOrd;
	std::list<_FBI::ST_SLORD*>::iterator itList;

	char zName[32];
	BOOL bErased = FALSE;

	if (enDirection==EN_ASC)
	{
		g_log.log(INFO, "[In BEFORE DEL ORDER - ASC ](OrdNo:%d)(%s)", nOrdNo, GetThreadName(zName));
		ITMAP_UP itAsc;
		itAsc = m_mapAsc.find(sOrdPrc);
		if (itAsc == m_mapAsc.end())
		{
			g_log.log(INFO, "[In BEFORE DEL ORDER - ASC ](OrdNo:%d)(%s)(%s)ASC Map 에 없음.", nOrdNo, sOrdPrc.c_str(), GetThreadName(zName));
			return FALSE;
		}

		lstOrd = (*itAsc).second;
		if (lstOrd.size()==0) {
			g_log.log(INFO, "[In BEFORE DEL ORDER - ASC ](OrdNo:%d)(%s)List 가 비어 있음.", nOrdNo, GetThreadName(zName));
		}
		for (itList = lstOrd.begin(); itList != lstOrd.end(); )
		{
			if (nOrdNo == (*itList)->OrdNo)
			{
				delete (*itList);
				itList = lstOrd.erase(itList);

				if (lstOrd.size() == 0)
					m_mapAsc.erase(itAsc);
				else
					m_mapAsc[sOrdPrc] = lstOrd;
				bErased = TRUE;
				g_log.log(INFO, "[In DEL ORDER - ASC ](OrdNo:%d)", nOrdNo);
			}
		}
	}
	
	if(enDirection == EN_DESC)
	{
		g_log.log(INFO, "[In BEFORE DEL ORDER - DESC](OrdNo:%d)(%s)", nOrdNo, GetThreadName(zName));
		ITMAP_DN itDesc;
		itDesc = m_mapDesc.find(sOrdPrc);
		if (itDesc == m_mapDesc.end())
		{
			g_log.log(INFO, "[In BEFORE DEL ORDER - DESC ](OrdNo:%d)(%s)(%s)DESC Map 에 없음.", nOrdNo, sOrdPrc.c_str(), GetThreadName(zName));//LOG
			return FALSE;
		}

		lstOrd = (*itDesc).second; 
		if (lstOrd.size()==0) {
			g_log.log(INFO, "[In BEFORE DEL ORDER - DESC ](OrdNo:%d)(%s)List 가 비어 있음.", nOrdNo, GetThreadName(zName));
		}
		for (itList = lstOrd.begin(); itList != lstOrd.end(); )
		{
			if (nOrdNo == (*itList)->OrdNo)
			{
				delete (*itList);
				itList = lstOrd.erase(itList);
				if (lstOrd.size() == 0)
					m_mapDesc.erase(itDesc);
				else
					m_mapDesc[sOrdPrc] = lstOrd;
				bErased = TRUE;
				g_log.log(INFO, "[In DEL ORDER - DESC](OrdNo:%d)", nOrdNo);
			}
		}
	}
	return bErased;
}

VOID CStkOrdManager::OrderProcCncl(_FBI::ST_SLORD* pOrd, std::string sAscPrc, std::string sDescPrc)
{
	DeleteOrder(EN_ASC, sAscPrc, pOrd->OrdNo);

	DeleteOrder(EN_DESC, sDescPrc, pOrd->OrdNo);

}

/*
- receive tick data
- scan both map
- if one order is fired in one map, remove from that map and from another map as well.

Step-1) FIRE Condition
 [ASC  LIST] CurrPrc >= Map Prc (1/2/3/4/5/6)
 [DESC LIST] CurrPrc <= Map Prc (6/5/4/3/2/1)

Step-2) Win or Lose 
 [In ASC]	"U" order => WIN
			"D" order => Lose
 [In Desc]	"U" order => Lose
			"D" order => Win
*/
VOID CStkOrdManager::ScanOrdByPrc(void* pPrcData)
{
	std::string sOrdPrc, sCurrPrc;
	_FBI::SMPACK_FX_EXEC* pNowPrc = (_FBI::SMPACK_FX_EXEC*)pPrcData;
	double dCurrPrc = atof(pNowPrc->cup);

	char temp[32], zName[32];
	if (dCurrPrc == 0 || dCurrPrc > 100)
	{
		strcpy(temp, pNowPrc->issue);
	}

	ITMAP_UP itAsc;
	ITMAP_DN itDesc;
	std::list<_FBI::ST_SLORD*>::iterator itList;

	/*
	[ascending]

	if Curr Price >= Ord Prc : fired
	   350           100-200-300-[400]-500
	*/
	if (IsThisAscThread())
	{
		LockAsc();
		for (itAsc = m_mapAsc.begin(); itAsc != m_mapAsc.end(); )
		{
			sOrdPrc = (*itAsc).first;
			int nComp = _FBI::ComparePrices(dCurrPrc, sOrdPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sCurrPrc);
			if (nComp < 0) {
				//printf( "[ASC ][PASS ](CurrPrc:%s)<(OrdPrc:%s)\n", sCurrPrc.c_str(), sOrdPrc.c_str());
				itAsc++;
				continue;
			}

			for (itList = (*itAsc).second.begin(); itList != (*itAsc).second.end(); itList++)
			{
				_FBI::ST_SLORD* pOrd = (*itList);
				pOrd->sFiredPrc = sCurrPrc;
				pOrd->cWinLose = (pOrd->cUpDn == 'U') ? 'W' : 'L';
				g_log.log(INFO, "[ASC ][FIRED START](%s)(CurrPrc:%s)>=(OrdPrc:%s)(WinLose:%c)(OrdNo:%d)"
					,GetThreadName(zName) , sCurrPrc.c_str(), sOrdPrc.c_str(), pOrd->cWinLose, pOrd->OrdNo );
				FireOrder(pOrd, sCurrPrc);
				delete pOrd;
			}
			itAsc = m_mapAsc.erase(itAsc);
		}
		UnLockAsc();
	}
	/*
	[descending]

	if Curr Price <= Ord Prc : fired
	   350           500-400-[300]-200-100
	*/
	else
	{
		LockDesc();
		for (itDesc = m_mapDesc.begin(); itDesc != m_mapDesc.end(); )
		{
			sOrdPrc = (*itDesc).first;
			int nComp = _FBI::ComparePrices(dCurrPrc, sOrdPrc, _FBI::FBILEN_PRC, m_stkInfo.lDotCnt, &sCurrPrc);
			if (nComp > 0) {
				//printf( "[DESC][PASS ](CurrPrc:%s)>(OrdPrc:%s)\n", sCurrPrc.c_str(), sOrdPrc.c_str());
				itDesc++;
				continue;
			}

			for (itList = (*itDesc).second.begin(); itList != (*itDesc).second.end(); itList++)
			{
				_FBI::ST_SLORD* pOrd = (*itList);
				pOrd->sFiredPrc = sCurrPrc;
				pOrd->cWinLose = (pOrd->cUpDn == 'D') ? 'W' : 'L';
				g_log.log(INFO, "[DESC][FIRED START](%s)(CurrPrc:%s)<=(OrdPrc:%s)(WinLose:%c)(OrdNo:%d)"
					, GetThreadName(zName), sCurrPrc.c_str(), sOrdPrc.c_str(), pOrd->cWinLose, pOrd->OrdNo);
				FireOrder(pOrd, sCurrPrc);
				delete pOrd;
			}

			itDesc = m_mapDesc.erase(itDesc);

		} // for (itMap = m_mapDesc.begin(); itMap != m_mapDesc.end(); itMap++)
		UnLockDesc();
	}
}

/*
- Delete this order info from the opposite thread
- send to main thread to save into db and send signal to the client
  (
*/
VOID CStkOrdManager::FireOrder(_FBI::ST_SLORD* pOrd, std::string sFiredPrc)
{
	char* pData = g_memPool.get();
	memcpy(pData, pOrd, sizeof(_FBI::ST_SLORD));
	const int WM_ORD_FIRED = WM_USER + 519;
	PostThreadMessage(m_nSaveThreadId, _FBI::WM_ORD_FIRED, sizeof(_FBI::ST_SLORD), (LPARAM)pData);

	char zName[32];
	string sOppositePrc = (pOrd->cWinLose == 'W') ? pOrd->sLosePrc : pOrd->sWinPrc;

	// delete the other map
	if (IsThisAscThread())
	{
		g_log.log(INFO ,"[ASC BEFORE DeleteOrder](%s)", GetThreadName(zName));
		LockDesc();
		DeleteOrder(EN_DESC, sOppositePrc, pOrd->OrdNo);
		UnLockDesc();
	}
	else
	{
		g_log.log(INFO, "[DESC BEFORE DeleteOrder](%s)", GetThreadName(zName));
		LockAsc();
		DeleteOrder(EN_ASC, sOppositePrc, pOrd->OrdNo);
		UnLockAsc();
	}
}

unsigned int CStkOrdManager::GetOppositeThreadId()
{
	unsigned int id = (m_unDesc == GetCurrentThreadId()) ? m_unAsc : m_unDesc;
	return id;
}
