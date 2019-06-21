#include "SimpleMapLKey.h"


CSimpleMapLKey::CSimpleMapLKey()
{
	m_mapS = nullptr;
	m_mapL = nullptr;
	m_mapD = nullptr;
	m_mapR = nullptr;
}

CSimpleMapLKey::~CSimpleMapLKey()
{
	DeInitialize();
}

bool CSimpleMapLKey::Initialize(VAL_TYPE valType)
{
	m_tpVal = valType;

	if (m_tpVal == VAL_TYPE::VAL_STRING) {
		m_mapS = new CSimpleMapBase<long, string>();
		return m_mapS->Initialize();
	}

	if (m_tpVal == VAL_TYPE::VAL_LONG) {
		m_mapL = new CSimpleMapBase<long, long>();
		return m_mapL->Initialize();
	}

	if (m_tpVal == VAL_TYPE::VAL_DOUBLE) {
		m_mapD = new CSimpleMapBase<long, double>();
		return m_mapD->Initialize();
	}

	if (m_tpVal == VAL_TYPE::VAL_RECORD) {
		m_mapR = new CSimpleMapBase<long, RECORD_VAL>();
		return m_mapR->Initialize();
	}
	
	return false;
}


void CSimpleMapLKey::DeInitialize()
{
	if (m_mapS != nullptr)	delete m_mapS;
	if (m_mapL != nullptr)	delete m_mapL;
	if (m_mapD != nullptr)	delete m_mapD;
	if (m_mapR != nullptr)	delete m_mapR;

	m_mapS = nullptr;
	m_mapL = nullptr;
	m_mapD = nullptr;
	m_mapR = nullptr;
}

bool CSimpleMapLKey::AddValue(long key, char* val, int nValLen, bool bUpdate)
{
	if (m_mapS == nullptr)	return false;

	string sVal = string(val);
	return m_mapS->Add(key, &sVal, nValLen, VAL_TYPE::VAL_STRING, bUpdate);
}

bool CSimpleMapLKey::AddValue(long key, long* val, int nValLen, bool bUpdate)
{
	if (m_mapL == nullptr)	return false;

	return m_mapL->Add(key, val, nValLen, VAL_TYPE::VAL_LONG, bUpdate);
}

bool CSimpleMapLKey::AddValue(long key, double* val, int nValLen, bool bUpdate)
{
	if (m_mapD == nullptr)	return false;

	return m_mapD->Add(key, val, nValLen, VAL_TYPE::VAL_DOUBLE, bUpdate);
}

bool CSimpleMapLKey::AddValue(long key, RECORD_VAL* val, int nValLen, bool bUpdate)
{
	if (m_mapR == nullptr)	return false;

	return m_mapR->Add(key, val, nValLen, VAL_TYPE::VAL_RECORD, bUpdate);
}

bool CSimpleMapLKey::IsExists(long key)
{
	bool bReturn;
	switch (m_tpVal)
	{
	case VAL_TYPE::VAL_STRING:
		if (m_mapS == nullptr)	return false;
		bReturn = m_mapS->IsExists(key);
		break;
	case VAL_TYPE::VAL_LONG:
		if (m_mapL == nullptr)	return false;
		bReturn = m_mapL->IsExists(key);
		break;
	case VAL_TYPE::VAL_DOUBLE:
		if (m_mapD == nullptr)	return false;
		bReturn = m_mapD->IsExists(key);
		break;
	case VAL_TYPE::VAL_RECORD:
		if (m_mapR == nullptr)	return false;
		bReturn = m_mapR->IsExists(key);
		break;
	}

	return bReturn;
}



bool CSimpleMapLKey::GetValue(long key, _Out_ char* pVal, int nValSize)
{
	if (m_mapS == nullptr)	return false;

	string sVal;

	if (!m_mapS->GetValue(key, m_tpVal, 0, &sVal))
	{
		m_nLastErr = m_mapS->GetLastErr();
		return false;
	}

	strcpy(pVal, sVal.c_str());
	return true;
}

bool CSimpleMapLKey::GetValue(long key, _Out_ long* pVal, int nValSize)
{
	if (m_mapL == nullptr)	return false;

	if (!m_mapL->GetValue(key, m_tpVal, 0, pVal))
	{
		m_nLastErr = m_mapL->GetLastErr();
		return false;
	}

	return true;
}

bool CSimpleMapLKey::GetValue(long key, _Out_ double* pVal, int nValSize)
{
	if (m_mapD == nullptr)	return false;

	if (!m_mapD->GetValue(key, m_tpVal, 0, pVal))
	{
		m_nLastErr = m_mapD->GetLastErr();
		return false;
	}

	return true;
}

bool CSimpleMapLKey::GetValue(long key, _Out_ RECORD_VAL* pVal, int nValSize)
{
	if (m_mapR == nullptr)	return false;

	if (!m_mapR->GetValue(key, m_tpVal, nValSize, pVal))
	{
		m_nLastErr = m_mapR->GetLastErr();
		return false;
	}

	return true;
}

void CSimpleMapLKey::Del(long key)
{
	if (m_mapS != nullptr)	m_mapS->Del(key);
	if (m_mapL != nullptr)	m_mapL->Del(key);
	if (m_mapD != nullptr)	m_mapD->Del(key);
	if (m_mapR != nullptr)	m_mapR->Del(key);
}