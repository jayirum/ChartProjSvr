#pragma once

#include "MapClassSimpleBase.h"

class CSimpleMapSKey
{
public:
	CSimpleMapSKey();
	~CSimpleMapSKey();

	bool	Initialize(VAL_TYPE valType);
	void	DeInitialize();

	bool	AddValue(char* key, char* val,			int nValLen, bool bUpdate = true);
	bool	AddValue(char* key, long* val,			int nValLen, bool bUpdate = true);
	bool	AddValue(char* key, double* val,		int nValLen, bool bUpdate = true);
	bool	AddValue(char* key, RECORD_VAL* val,	int nValLen, bool bUpdate = true);
	bool	IsExists(char* key );
	bool	GetValue(char* key, _Out_ char* pVal,		int nValSize=0);	// nValSize 값 불필요
	bool	GetValue(char* key, _Out_ long* pVal,		int nValSize=0);	// nValSize 값 불필요
	bool	GetValue(char* key, _Out_ double* pVal,		int nValSize=0);	// nValSize 값 불필요
	bool	GetValue(char* key, _Out_ RECORD_VAL* pVal,	int nValSize=0);	// nValSize 값 반드시필요

	int		GetLastErr() { return m_nLastErr; }
private:
	CSimpleMapBase<string, string>		*m_mapS;
	CSimpleMapBase<string, long>		*m_mapL;
	CSimpleMapBase<string, double>		*m_mapD;
	CSimpleMapBase<string, RECORD_VAL>	*m_mapR;
	VAL_TYPE m_tpVal;

	int		m_nLastErr;
};

