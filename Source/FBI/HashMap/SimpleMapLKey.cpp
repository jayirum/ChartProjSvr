#include "SimpleMapLKey.h"


CSimpleMapLKey::CSimpleMapLKey()
{}

CSimpleMapLKey::~CSimpleMapLKey()
{}

bool CSimpleMapLKey::Initialize(VAL_TYPE valType)
{
	bool bReturn;
	m_tpVal = valType;
	bReturn = m_mapS.Initialize();
	bReturn = m_mapL.Initialize();
	bReturn = m_mapD.Initialize();
	bReturn = m_mapR.Initialize();
	
	return bReturn;
}


void CSimpleMapLKey::DeInitialize()
{
	if (m_tpVal == VAL_TYPE::VAL_STRING)	m_mapS.DeInitialize();
	if (m_tpVal == VAL_TYPE::VAL_LONG)		m_mapL.DeInitialize();
	if (m_tpVal == VAL_TYPE::VAL_DOUBLE)	m_mapD.DeInitialize();
	if (m_tpVal == VAL_TYPE::VAL_RECORD)	m_mapR.DeInitialize();
}

bool CSimpleMapLKey::AddValue(long key, char* val, int nValLen, bool bUpdate)
{
	string sVal = string(val);
	return m_mapS.Add(key, &sVal, nValLen, VAL_TYPE::VAL_STRING, bUpdate);
}

bool CSimpleMapLKey::AddValue(long key, long* val, int nValLen, bool bUpdate)
{
	return m_mapL.Add(key, val, nValLen, VAL_TYPE::VAL_LONG, bUpdate);
}

bool CSimpleMapLKey::AddValue(long key, double* val, int nValLen, bool bUpdate)
{
	return m_mapD.Add(key, val, nValLen, VAL_TYPE::VAL_DOUBLE, bUpdate);
}

bool CSimpleMapLKey::AddValue(long key, RECORD_VAL* val, int nValLen, bool bUpdate)
{
	return m_mapR.Add(key, val, nValLen, VAL_TYPE::VAL_RECORD, bUpdate);
}

bool CSimpleMapLKey::IsExists(long key)
{
	bool bReturn;
	switch (m_tpVal)
	{
	case VAL_TYPE::VAL_STRING:
		bReturn = m_mapS.IsExists(key);
		break;
	case VAL_TYPE::VAL_LONG:
		bReturn = m_mapL.IsExists(key);
		break;
	case VAL_TYPE::VAL_DOUBLE:
		bReturn = m_mapD.IsExists(key);
		break;
	case VAL_TYPE::VAL_RECORD:
		bReturn = m_mapR.IsExists(key);
		break;
	}

	return bReturn;
}



bool CSimpleMapLKey::GetValue(long key, _Out_ char* pVal, int nValSize)
{
	string sVal;

	if (!m_mapS.GetValue(key, m_tpVal, 0, &sVal))
	{
		m_nLastErr = m_mapS.GetLastErr();
		return false;
	}

	strcpy(pVal, sVal.c_str());
	return true;
}

bool CSimpleMapLKey::GetValue(long key, _Out_ long* pVal, int nValSize)
{
	if (!m_mapL.GetValue(key, m_tpVal, 0, pVal))
	{
		m_nLastErr = m_mapL.GetLastErr();
		return false;
	}

	return true;
}

bool CSimpleMapLKey::GetValue(long key, _Out_ double* pVal, int nValSize)
{
	if (!m_mapD.GetValue(key, m_tpVal, 0, pVal))
	{
		m_nLastErr = m_mapD.GetLastErr();
		return false;
	}

	return true;
}

bool CSimpleMapLKey::GetValue(long key, _Out_ RECORD_VAL* pVal, int nValSize)
{
	if (!m_mapR.GetValue(key, m_tpVal, nValSize, pVal))
	{
		m_nLastErr = m_mapR.GetLastErr();
		return false;
	}

	return true;
}