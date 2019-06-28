#pragma once

#include "HashMapInc.h"
#include <Windows.h>
#include <map>
#include <string>

using namespace std;


template <typename KEY, typename VAL> class CSimpleMapBase
{
public:
	CSimpleMapBase();
	~CSimpleMapBase();

	bool	Initialize();
	void	DeInitialize();

	bool	IsExists		(KEY k){ return (m_map.find(k) != m_map.end()); }

	bool	Add				(KEY k, VAL* v, int nValSize, VAL_TYPE tpVal, bool bUpdate=true);
	void	Del				(KEY k);
	bool	GetValue		(KEY k, VAL_TYPE tpVal, int nValSize, _Out_ VAL* pVal);

	int		GetLastErr() { return m_nLastErr; }

protected:
	void	Lock()			{ EnterCriticalSection(&m_cs); }
	void	Unlock()		{ LeaveCriticalSection(&m_cs); }
	bool	IsStringKey()	{ return (m_keyType == KEY_TYPE::KEY_STRING); }

protected:
	map<KEY, VAL>		m_map;
	CRITICAL_SECTION	m_cs;
	MAP_STATUS			m_status;
	int					m_nLastErr;	
};



template <typename KEY, typename VAL>
CSimpleMapBase<typename KEY, typename VAL>::CSimpleMapBase()
{
	m_status = MAP_STATUS::READY;
}

template <typename KEY, typename VAL>
CSimpleMapBase<KEY, VAL>::~CSimpleMapBase()
{
	DeInitialize();
}


template <typename KEY, typename VAL>
bool CSimpleMapBase<KEY, VAL>::Initialize()
{
	InitializeCriticalSection(&m_cs);

	m_nLastErr	= 0;
	m_status	= MAP_STATUS::CREATED;
	m_nLastErr	= MAP_ERRNO::ERR_OK;
	
	return true;
}

template <typename KEY, typename VAL>
void CSimpleMapBase<KEY, VAL>::DeInitialize()
{
	if (m_status == MAP_STATUS::REMOVED)
	{
		return;
	}

	Lock();
	m_map.clear();
	Unlock();
	DeleteCriticalSection(&m_cs);

	m_status	= MAP_STATUS::REMOVED;
	m_nLastErr	= MAP_ERRNO::ERR_OK;
}



template <typename KEY, typename VAL>
bool CSimpleMapBase<KEY, VAL>::Add(KEY k, VAL* v, int nValSize, VAL_TYPE tpVal, bool bUpdate)
{
	Lock();

	// find whether KEY exists already
	map<KEY, VAL>::iterator it = m_map.find(k);
	if (it != m_map.end())
	{
		if (!bUpdate)
		{
			m_nLastErr = MAP_ERRNO::ERR_ALREADY_EXISTS;

			Unlock();
			return false;
		}

		// delete previous value
		m_map.erase(it);
	}
	m_map[k] = *v;

	Unlock();

	m_nLastErr = MAP_ERRNO::ERR_OK;
	return true;
}



template <typename KEY, typename VAL>
void CSimpleMapBase<KEY, VAL>::Del(KEY k)
{
	Lock();

	// find whether KEY exists already
	map<KEY, VAL>::iterator it = m_map.find(k);
	if (it != m_map.end())
	{
		m_map.erase(it);
	}
	m_nLastErr = MAP_ERRNO::ERR_OK;
}

template <typename KEY, typename VAL>
bool CSimpleMapBase<KEY, VAL>::GetValue(KEY k, VAL_TYPE tpVal, int nValSize, _Out_ VAL* pVal)
{
	Lock();

	// find whether KEY exists already
	map<KEY, VAL>::iterator it = m_map.find(k);

	if (it == m_map.end()) {
		Unlock();
		m_nLastErr = MAP_ERRNO::ERR_KEY_ABSENCE;
		return false;
	}

	*pVal = (*it).second;

	Unlock();
	
	return true;
}

