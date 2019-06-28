#include "ListMapLKey.h" 


CListMapLKey::CListMapLKey()
{
	m_map = nullptr;
}

CListMapLKey::~CListMapLKey()
{
	DeInitialize();
}

bool CListMapLKey::Initialize()
{
	m_map = new CListMapBase<long>();
	return m_map->Initialize();
}


void CListMapLKey::DeInitialize()
{
	if (m_map != nullptr)	delete m_map;
	m_map = nullptr;
}


bool CListMapLKey::AddValue(long key, RECORD_VAL* val)
{
	return m_map->Add(key, val);
}

bool CListMapLKey::IsExists(long key)
{
	bool bReturn = false;
	if (m_map != nullptr) {
		bReturn = m_map->IsExists(key);
	}
	return bReturn;
}

RECORD_VAL* CListMapLKey::GetValue(long key, _Out_ int* pnRecordCnt)
{
	if (m_map == nullptr)	return false;

	RECORD_VAL* pResult = m_map->GetValue(key, pnRecordCnt);
	if (!pResult)
	{
		m_nLastErr = m_map->GetLastErr();
	}
	return pResult;
}

void CListMapLKey::Del(long key)
{
	if (m_map != nullptr)	m_map->Del(key);
}


void CListMapLKey::MemFree(void* pPtr)
{
	if (m_map != nullptr)	m_map->MemFree(pPtr);
}