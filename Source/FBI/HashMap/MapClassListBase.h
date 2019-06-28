#pragma once

#include "HashMapInc.h"
#include <Windows.h>
#include <map>
#include <list>
#include <string>

using namespace std;


template <typename KEY> class CListMapBase
{
public:
	CListMapBase();
	~CListMapBase();

	bool	Initialize();
	void	DeInitialize();

	bool		Add		(KEY k, RECORD_VAL* v);
	void		Del		(KEY k);
	RECORD_VAL*	GetValue(KEY k, _Out_ int* pnRecordCnt);

	bool		IsExists(KEY k);
	int			GetLastErr()		{ return m_nLastErr; }

	void		MemFree(void* pPtr);
protected:
	void	Lock()			{ EnterCriticalSection(&m_cs); }
	void	Unlock()		{ LeaveCriticalSection(&m_cs); }

protected:
	map<KEY, list<RECORD_VAL>>		m_map;
	CRITICAL_SECTION				m_cs;
	MAP_STATUS						m_status;
	int								m_nLastErr;	
};



template <typename KEY>
CListMapBase<KEY>::CListMapBase()
{
	m_status = MAP_STATUS::READY;
}

template <typename KEY>
CListMapBase<KEY>::~CListMapBase()
{
	DeInitialize();
}


template <typename KEY>
bool CListMapBase<KEY>::Initialize()
{
	InitializeCriticalSection(&m_cs);

	m_nLastErr	= 0;
	m_status	= MAP_STATUS::CREATED;
	m_nLastErr	= MAP_ERRNO::ERR_OK;
	
	return true;
}

template <typename KEY>
void CListMapBase<KEY>::DeInitialize()
{
	if (m_status == MAP_STATUS::REMOVED)
	{
		return;
	}

	Lock();
	//for (auto it = m_map.begin(); it != m_map.end(); it++)
	//{
	//	list<RECORD_VAL> lstData = (*it).second;
	//	for (auto itList = lstData.begin(); itList != lstData.end(); itList++)
	//	{
	//		RECORD_VAL* p = *itList;
	//		delete p;
	//	}
	//}
	m_map.clear();
	Unlock();
	DeleteCriticalSection(&m_cs);

	m_status	= MAP_STATUS::REMOVED;
	m_nLastErr	= MAP_ERRNO::ERR_OK;
}



template <typename KEY>
bool CListMapBase<KEY>::Add(KEY k, RECORD_VAL* v)
{
	Lock();

	list<RECORD_VAL> lstData;

	map<KEY, list<RECORD_VAL> >::iterator it = m_map.find(k);
	if (it != m_map.end())
	{
		lstData = (*it).second;
	}

	//v->pNext = nullptr;
	lstData.push_back(*v);

	m_map[k] = lstData;

	Unlock();

	m_nLastErr = MAP_ERRNO::ERR_OK;
	return true;
}



template <typename KEY>
void CListMapBase<KEY>::Del(KEY k)
{
	Lock();

	map<KEY, list<RECORD_VAL> >::iterator it = m_map.find(k);
	if(it!=m_map.end())
	{
		list<RECORD_VAL> lstData = (*it).second;
		for (auto itList = lstData.begin(); itList != lstData.end(); itList++)
		{
			//RECORD_VAL* p = *itList;
			//delete p;
		}
		m_map.erase(it);
	}

	m_nLastErr = MAP_ERRNO::ERR_OK;
}


/*
	pnRecordCnt : 반환되는 데이터의 갯수
*/
//template <typename KEY>
//RECORD_VAL* CListMapBase<KEY>::GetValue(KEY k, _Out_ int* pnRecordCnt)
//{
//	RECORD_VAL* pResult = nullptr;
//	*pnRecordCnt = 0;
//
//	Lock();
//
//	map<KEY, list<RECORD_VAL> >::iterator it = m_map.find(k);
//	if (it == m_map.end()) 
//	{
//		Unlock();
//		m_nLastErr = MAP_ERRNO::ERR_KEY_ABSENCE;
//		return pResult;
//	}
//
//	list<RECORD_VAL> lstData = (*it).second;
//	if(lstData.size()==0)
//	{
//		Unlock();
//		m_nLastErr = MAP_ERRNO::ERR_KEY_ABSENCE;
//		return pResult;
//	}
//	
//	int nRecord				= 0;
//	RECORD_VAL *pCurr = nullptr;
//	RECORD_VAL *pNext = nullptr;
//
//	for (auto itList = lstData.begin(); itList != lstData.end(); itList++)
//	{
//		if (nRecord==0)
//		{
//			pCurr	= new RECORD_VAL;
//			pResult = pCurr;
//
//			ZeroMemory(pCurr, sizeof(RECORD_VAL));
//			memcpy(pCurr, &(*itList), sizeof(RECORD_VAL));
//
//			pCurr->pNext = nullptr;
//		}
//		else 
//		{
//			pCurr->pNext = new RECORD_VAL;
//			ZeroMemory(pCurr->pNext, sizeof(RECORD_VAL));
//			memcpy(pCurr->pNext, &(*itList), sizeof(RECORD_VAL));
//			
//			RECORD_VAL* temp	= pCurr->pNext;
//			pCurr				= temp;
//			pCurr->pNext		= nullptr;
//		}
//
//		nRecord++;
//	}
//
//	Unlock();
//	
//	*pnRecordCnt = nRecord;
//	return pResult;
//}


template <typename KEY>
RECORD_VAL* CListMapBase<KEY>::GetValue(KEY k, _Out_ int* pnRecordCnt)
{
	RECORD_VAL* pResult = nullptr;
	*pnRecordCnt = 0;

	Lock();

	map<KEY, list<RECORD_VAL> >::iterator it = m_map.find(k);
	if (it == m_map.end())
	{
		Unlock();
		m_nLastErr = MAP_ERRNO::ERR_KEY_ABSENCE;
		return pResult;
	}

	list<RECORD_VAL> lstData = (*it).second;
	if (lstData.size() == 0)
	{
		Unlock();
		m_nLastErr = MAP_ERRNO::ERR_KEY_ABSENCE;
		return pResult;
	}

	int nRecord = lstData.size();
	pResult = new RECORD_VAL[nRecord];

	int nCnt = 0;
	for (auto itList = lstData.begin(); itList != lstData.end(); itList++)
	{
		memcpy(&pResult[nCnt++], &(*itList), sizeof(RECORD_VAL));
	}

	Unlock();

	*pnRecordCnt = nRecord;
	return pResult;
}

template <typename KEY>
bool CListMapBase<KEY>::IsExists(KEY k)
{
	bool bExists = false;

	Lock();
	map<KEY, list<RECORD_VAL> >::iterator it = m_map.find(k);
	if (it != m_map.end())
	{
		if (((*it).second).size() > 0)
		{
			bExists = true;
		}
	}

	Unlock();

	return bExists;
}

//template <typename KEY>
//void CListMapBase<KEY>::MemFree(void* pPtr)
//{
//	if (pPtr == nullptr)	return;
//
//	RECORD_VAL* pCurr, *pNext;
//	pCurr = (RECORD_VAL*)pPtr;
//	while (true)
//	{
//		pNext = pCurr->pNext;
//		delete pCurr;
//
//		if (pNext == nullptr)
//			break;
//
//		pCurr = pNext;
//	}
//}

template <typename KEY>
void CListMapBase<KEY>::MemFree(void* pPtr)
{
	if (pPtr == nullptr)	return;

	delete[] pPtr;
}