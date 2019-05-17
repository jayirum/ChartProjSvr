#pragma once


/*
	하나의 주문에 대해 두개의 MAP 에 관리 - WIN, LOSE
	- UP 주문에 대해 WIN-UP, LOSE-DOWN
	- DOWN 주문에 대해 WIN-DOWN, LOSE-UP

	각 MAP (UP, DOWN) 은 개별 스레드에서 관리
*/
#include "../common/FBIInc.h"
#include <map>
#include <list>
#include <string>
#include <functional>

/*
	string : fire price
*/
typedef std::map<std::string, std::list<_FBI::ST_SLORD*>, std::less<std::string> >		MAP_UP;	//ascending
typedef std::map<std::string, std::list<_FBI::ST_SLORD*>, std::greater<std::string> >	MAP_DN;	//descending

typedef std::map<std::string, std::list<_FBI::ST_SLORD*>, std::less<std::string> >::iterator	ITMAP_UP;	//ascending
typedef std::map<std::string, std::list<_FBI::ST_SLORD*>, std::greater<std::string> >::iterator	ITMAP_DN;	//descending


class CStkOrd
{
public:
	CStkOrd(std::string sStkCd);
	~CStkOrd();

	BOOL Initialize(_FBI::ST_STK_INFO* p, unsigned int	nSaveThreadId);
	VOID Finalize();

	// FBIMainProc calls this function when it gets order / tick
	VOID RelayOrdAndPrc(int nMsg, void* pData);

private:

	////////////////////////////////////////////////////////////////////////////
	// Thread Function. Be created 2 - one is for Up, the other is for Down
	static unsigned WINAPI OrderHandlerThread(LPVOID lp);

	// add order / delete order
	VOID OrderProc(void* pOrdData);

	// check fire against every tick
	VOID ScanOrdByPrc(void* pPrcData);
	
	VOID OrderProcAdd(_FBI::ST_SLORD* pOrd, std::string sAscPrc, std::string sDescPrc);
	VOID OrderProcCncl(_FBI::ST_SLORD* pOrd, std::string sAscPrc, std::string sDescPrc);
	
	VOID FireOrder(_FBI::ST_SLORD* pOrd, std::string sFiredPrc);
	BOOL DeleteOrder(BOOL bAsc, std::string sOrdPrc, int nOrdNo);
	////////////////////////////////////////////////////////////////////////////


	unsigned int GetOppositeThreadId();
	inline BOOL IsThisAscThread() { return (GetCurrentThreadId() == m_unAsc); }

	inline VOID LockAsc() { EnterCriticalSection(&m_csAsc); }
	inline VOID LockDesc() { EnterCriticalSection(&m_csDesc); }
	inline VOID UnLockAsc() { LeaveCriticalSection(&m_csAsc); }
	inline VOID UnLockDesc() { LeaveCriticalSection(&m_csDesc); }

private:
	std::string			m_sStkCd;
	HANDLE				m_hAsc, m_hDesc;
	unsigned int		m_unAsc, m_unDesc;
	unsigned int		m_nSaveThreadId;

	_FBI::ST_STK_INFO	m_stkInfo;

	MAP_UP	m_mapAsc;	//ascending
	MAP_DN	m_mapDesc;	//descending

	CRITICAL_SECTION	m_csAsc, m_csDesc;

};

