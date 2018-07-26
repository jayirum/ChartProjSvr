#include "stdafx.h"
#include "HedgeTarget.h"


CHedgeTarget::CHedgeTarget()
{
	InitializeCriticalSection(&m_cs);
}


CHedgeTarget::~CHedgeTarget()
{
	DeleteCriticalSection(&m_cs);
}


// Data 에는 무엇을 저장할지 미정
VOID	CHedgeTarget::Add(CString sAcnt, CString sData, BOOL bLock)
{
	if(bLock)	LOCK();
	m_map[sAcnt] = sData;
	if(bLock)	UNLOCK();
}

VOID CHedgeTarget::Delete(CString sAcnt, BOOL bLock )
{
	if (bLock)	LOCK();
	m_map.erase(sAcnt);
	if (bLock)	UNLOCK();
}



VOID	CHedgeTarget::Reset()
{
	LOCK();
	__try {
		m_map.clear();
	}
	__finally {
		UNLOCK();
	}
}
BOOL	CHedgeTarget::IsTarget(CString& i_acno)
{
	BOOL bFind = FALSE;
	std::map<CString, CString>::iterator it;
	LOCK();
	__try{
		it = m_map.find(i_acno);
		bFind = (it != m_map.end());
	}
	__finally {
		UNLOCK();
	}
	return bFind;
}

INT		CHedgeTarget::GetAllData(CString *pOut)
{
	std::map<CString, CString>::iterator it;
	int i = 0;
	LOCK();
	__try {
		for (it = m_map.begin(), i=0; it != m_map.end();i++, ++it)
		{
			pOut[i] = (*it).first;
		}
	}
	__finally {
		UNLOCK();
	}

	return m_map.size();
}