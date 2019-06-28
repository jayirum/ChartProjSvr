#pragma once

#include "MapClassListBase.h"

class CListMapSKey
{
public:
	CListMapSKey();
	~CListMapSKey();

	bool	Initialize();
	void	DeInitialize();

	bool		AddValue(char* key, RECORD_VAL* val);
	RECORD_VAL*	GetValue(char* key, _Out_ int* pnRecordCnt);
	void		Del(char* key);
	bool		IsExists(char* key);
	void		MemFree(void* pPtr);
	int			GetLastErr() { return m_nLastErr; }
private:
	CListMapBase<string>	*m_map;

	int		m_nLastErr;
};

