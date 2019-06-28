#include "ListMapSKey.h" 


CListMapSKey::CListMapSKey()
{
	m_map = nullptr;
}

CListMapSKey::~CListMapSKey()
{
	DeInitialize();
}

bool CListMapSKey::Initialize()
{
	m_map = new CListMapBase<string>();
	return m_map->Initialize();
}


void CListMapSKey::DeInitialize()
{
	if (m_map != nullptr)	delete m_map;
	m_map = nullptr;
}


bool CListMapSKey::AddValue(char* key, RECORD_VAL* val)
{	
	string sKey = string(key);
	return m_map->Add(sKey, val);
}

bool CListMapSKey::IsExists(char* key)
{
	bool bReturn = false;
	string sKey = string(key);

	if (m_map != nullptr) {
		bReturn = m_map->IsExists(sKey);
	}
	return bReturn;
}



RECORD_VAL* CListMapSKey::GetValue(char* key, _Out_ int* pnRecordCnt)
{
	if (m_map == nullptr)	return false;

	string sKey = string(key);
	RECORD_VAL* pResult = m_map->GetValue(sKey, pnRecordCnt);
	if (!pResult)
	{
		m_nLastErr = m_map->GetLastErr();
	}
	return pResult;
}

void CListMapSKey::Del(char* key)
{
	string sKey = key;
	if (m_map != nullptr)	m_map->Del(sKey);
}


void CListMapSKey::MemFree(void* pPtr)
{
	if (m_map != nullptr)	m_map->MemFree(pPtr);
}