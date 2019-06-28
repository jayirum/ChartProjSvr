#pragma once

#include "MapClassListBase.h"

class CListMapLKey
{
public:
	CListMapLKey();
	~CListMapLKey();

	bool		Initialize();
	void		DeInitialize();

	bool		AddValue(long key, RECORD_VAL* val);
	RECORD_VAL*	GetValue(long key, _Out_ int* pnRecordCnt);
	void		Del(long key);
	bool		IsExists(long key);
	void		MemFree(void* pPtr);

	int			GetLastErr() { return m_nLastErr; }
private:
	CListMapBase<long>	*m_map;
	int					m_nLastErr;
};

