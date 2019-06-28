#pragma once

#include "MapClassSimpleBase.h"

class CSimpleMapLKey
{
public:
	CSimpleMapLKey();
	~CSimpleMapLKey();

	bool	Initialize(VAL_TYPE valType);
	void	DeInitialize();

	bool	AddValue(long key, char* val, int nValLen, bool bUpdate = true);
	bool	AddValue(long key, long* val, int nValLen, bool bUpdate = true);
	bool	AddValue(long key, double* val, int nValLen, bool bUpdate = true);
	bool	AddValue(long key, RECORD_VAL* val, int nValLen, bool bUpdate = true);
	
	bool	GetValue(long key, _Out_ char* pVal, int nValSize = 0);	// nValSize 값 불필요
	bool	GetValue(long key, _Out_ long* pVal, int nValSize = 0);	// nValSize 값 불필요
	bool	GetValue(long key, _Out_ double* pVal, int nValSize = 0);	// nValSize 값 불필요
	bool	GetValue(long key, _Out_ RECORD_VAL* pVal, int nValSize = 0);	// nValSize 값 반드시필요

	void	Del(long key);
	bool	IsExists(long key);
	int		GetLastErr() { return m_nLastErr; }

private:
	CSimpleMapBase<long, string>		*m_mapS;
	CSimpleMapBase<long, long>			*m_mapL;
	CSimpleMapBase<long, double>		*m_mapD;
	CSimpleMapBase<long, RECORD_VAL>	*m_mapR;

	VAL_TYPE m_tpVal;

	int		m_nLastErr;
};