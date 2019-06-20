#include "SimpleMapSKey.h" 


CSimpleMapSKey::CSimpleMapSKey()
{
	m_mapS = nullptr;
	m_mapL = nullptr;
	m_mapD = nullptr;
	m_mapR = nullptr;
}

CSimpleMapSKey::~CSimpleMapSKey()
{
	DeInitialize();
}

bool CSimpleMapSKey::Initialize(VAL_TYPE valType)
{
	m_tpVal = valType;

	if (m_tpVal == VAL_TYPE::VAL_STRING) {
		m_mapS = new CSimpleMapBase<string, string>();
		return m_mapS->Initialize();
	}

	if (m_tpVal == VAL_TYPE::VAL_LONG) {
		m_mapL = new CSimpleMapBase<string, long>();
		return m_mapL->Initialize();
	}

	if (m_tpVal == VAL_TYPE::VAL_DOUBLE) {
		m_mapD = new CSimpleMapBase<string, double>();
		return m_mapD->Initialize();
	}
	
	if (m_tpVal == VAL_TYPE::VAL_RECORD) {
		m_mapR = new CSimpleMapBase<string, RECORD_VAL>();
		return m_mapR->Initialize();
	}

	return false;
}


void CSimpleMapSKey::DeInitialize()
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


bool CSimpleMapSKey::AddValue(char* key, char* val, int nValLen, bool bUpdate)
{
	if (m_mapS == nullptr)	return false;

	string sKey = string(key);
	string sVal = string(val, nValLen);
	return m_mapS->Add(sKey, &sVal, nValLen, VAL_TYPE::VAL_STRING, bUpdate);
}

bool CSimpleMapSKey::AddValue(char* key, long* val, int nValLen, bool bUpdate)
{
	if (m_mapL == nullptr)	return false;

	string sKey = string(key);
	return m_mapL->Add(sKey, val, nValLen, VAL_TYPE::VAL_LONG, bUpdate);
}

bool CSimpleMapSKey::AddValue(char* key, double* val, int nValLen, bool bUpdate)
{
	if (m_mapD == nullptr)	return false;

	string sKey = string(key);
	return m_mapD->Add(sKey, val, nValLen, VAL_TYPE::VAL_DOUBLE, bUpdate);
}

bool CSimpleMapSKey::AddValue(char* key, RECORD_VAL* val, int nValLen, bool bUpdate)
{
	if (m_mapR == nullptr)	return false;

	string sKey = string(key);
	return m_mapR->Add(sKey, val, nValLen, VAL_TYPE::VAL_RECORD, bUpdate);
}


bool CSimpleMapSKey::IsExists(char* key)
{
	bool bReturn;
	string sKey = string(key);
	switch (m_tpVal)
	{
	case VAL_TYPE::VAL_STRING:
		if (m_mapS == nullptr)	return false;
		bReturn = m_mapS->IsExists(sKey);
		break;
	case VAL_TYPE::VAL_LONG:
		if (m_mapL == nullptr)	return false;
		bReturn = m_mapL->IsExists(sKey);
		break;
	case VAL_TYPE::VAL_DOUBLE:
		if (m_mapD == nullptr)	return false;
		bReturn = m_mapD->IsExists(sKey);
		break;
	case VAL_TYPE::VAL_RECORD:
		if (m_mapR == nullptr)	return false;
		bReturn = m_mapR->IsExists(sKey);
		break;
	}

	return bReturn;
}



bool CSimpleMapSKey::GetValue(char* key, _Out_ char* pVal, int nValSize)
{
	if (m_mapS == nullptr)	return false;

	string sKey = string(key);
	string sVal;
	if (!m_mapS->GetValue(sKey, m_tpVal, nValSize, &sVal))
	{
		m_nLastErr = m_mapS->GetLastErr();
		return false;
	}
	strcpy(pVal, sVal.c_str());
	return true;
}

bool CSimpleMapSKey::GetValue(char* key, _Out_ long* pVal, int nValSize)
{
	if (m_mapL == nullptr)	return false;

	string sKey = string(key);

	if (!m_mapL->GetValue(sKey, m_tpVal, sizeof(long), pVal))
	{
		m_nLastErr = m_mapL->GetLastErr();
		return false;
	}

	return true;
}

bool CSimpleMapSKey::GetValue(char* key, _Out_ double* pVal, int nValSize)
{
	if (m_mapD == nullptr)	return false;

	string sKey = string(key);

	if (!m_mapD->GetValue(sKey, m_tpVal, sizeof(double), pVal))
	{
		m_nLastErr = m_mapD->GetLastErr();
		return false;
	}

	return true;
}

bool CSimpleMapSKey::GetValue(char* key, _Out_ RECORD_VAL* pVal, int nValSize)
{
	if (m_mapR == nullptr)	return false;

	string sKey = string(key);

	if (!m_mapR->GetValue(sKey, m_tpVal, nValSize, pVal))
	{
		m_nLastErr = m_mapR->GetLastErr();
		return false;
	}

	return true;
}