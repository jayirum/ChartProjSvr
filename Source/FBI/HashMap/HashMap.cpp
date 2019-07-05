#include "HashMap.h"
#include "SimpleMapSKey.h"
#include "SimpleMapLKey.h"
#include "ListMapSKey.h"
#include "ListMapLKey.h"

void*	SimpMAP_CreateSS()
{
	CSimpleMapSKey* p = new CSimpleMapSKey();
	p->Initialize(VAL_TYPE::VAL_STRING);
	return (void*)p;
}


void*	SimpMAP_CreateLS()
{
	CSimpleMapLKey* p = new CSimpleMapLKey();
	p->Initialize(VAL_TYPE::VAL_STRING);
	return (void*)p;
}

void*	SimpMAP_CreateSL()
{
	CSimpleMapSKey* p = new CSimpleMapSKey();
	p->Initialize(VAL_TYPE::VAL_LONG);
	return (void*)p;
}


void*	SimpMAP_CreateLL()
{
	CSimpleMapLKey* p = new CSimpleMapLKey();
	p->Initialize(VAL_TYPE::VAL_LONG);
	return (void*)p;
}


void*	SimpMAP_CreateSD()
{
	CSimpleMapSKey* p = new CSimpleMapSKey();
	p->Initialize(VAL_TYPE::VAL_DOUBLE);
	return (void*)p;
}


void*	SimpMAP_CreateLD()
{
	CSimpleMapLKey* p = new CSimpleMapLKey();
	p->Initialize(VAL_TYPE::VAL_DOUBLE);
	return (void*)p;
}


void*	SimpMAP_CreateSR()
{
	CSimpleMapSKey* p = new CSimpleMapSKey();
	p->Initialize(VAL_TYPE::VAL_RECORD);
	return (void*)p;
}


void*	SimpMAP_CreateLR()
{
	CSimpleMapLKey* p = new CSimpleMapLKey();
	p->Initialize(VAL_TYPE::VAL_RECORD);
	return (void*)p;
}

int	SimpMAP_DestroyS(void* pMap)
{
	delete (CSimpleMapSKey*)pMap;
	return 0;
}

int SimpMAP_DestroyL(void* pMap)
{
	delete (CSimpleMapLKey*)pMap;
	return 0;
}

bool	SimpMAP_AddValueSS(void* pMap, char* key, char* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapSKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}

bool	SimpMAP_AddValueLS(void* pMap, long key, char* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapLKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}


bool	SimpMAP_AddValueSL(void* pMap, char* key, long* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapSKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}


bool	SimpMAP_AddValueLL(void* pMap, long key, long* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapLKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}


bool	SimpMAP_AddValueSD(void* pMap, char* key, double* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapSKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}

bool	SimpMAP_AddValueLD(void* pMap, long key, double* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapLKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}

bool	SimpMAP_AddValueSR(void* pMap, char* key, RECORD_VAL* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapSKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}


bool	SimpMAP_AddValueLR(void* pMap, long key, RECORD_VAL* val, int nValSize, bool bUpdate)
{
	return ((CSimpleMapLKey*)pMap)->AddValue(key, val, nValSize, bUpdate);
}

bool	SimpMAP_IsExistsS(void* pMap, char* key)
{
	return ((CSimpleMapSKey*)pMap)->IsExists(key);
}

bool	SimpMAP_IsExistsL(void* pMap, long key)
{
	return ((CSimpleMapLKey*)pMap)->IsExists(key);
}

bool SimpMAP_GetStringValueS(void* pMap, char* key, /*out*/char* pResult, int nStringSize)
{
	return ((CSimpleMapSKey*)pMap)->GetValue(key, pResult, nStringSize);
}


bool SimpMAP_GetLongValueS(void* pMap, char* key, /*out*/long* pResult)
{
	return ((CSimpleMapSKey*)pMap)->GetValue(key, pResult);
}

bool SimpMAP_GetDblValueS(void* pMap, char* key, /*out*/double* pResult)
{
	return ((CSimpleMapSKey*)pMap)->GetValue(key, pResult);
}

bool SimpMAP_GetRecordValueS(void* pMap, char* key, /*out*/RECORD_VAL* pResult)
{
	return ((CSimpleMapSKey*)pMap)->GetValue(key, pResult, sizeof(RECORD_VAL));
}


bool SimpMAP_GetStringValueL(void* pMap, long key, /*out*/char* pResult)
{
	return ((CSimpleMapLKey*)pMap)->GetValue(key, pResult);
}


bool SimpMAP_GetLongValueL(void* pMap, long key, /*out*/long* pResult)
{
	return ((CSimpleMapLKey*)pMap)->GetValue(key, pResult);
}

bool SimpMAP_GetDblValueL(void* pMap, long key, /*out*/double* pResult)
{
	return ((CSimpleMapLKey*)pMap)->GetValue(key, pResult);
}

bool SimpMAP_GetRecordValueL(void* pMap, long key, /*out*/RECORD_VAL* pResult)
{
	return ((CSimpleMapLKey*)pMap)->GetValue(key, pResult, sizeof(RECORD_VAL));
}


int SimpMAP_DelS(void* pMap, char* k)
{
	((CSimpleMapSKey*)pMap)->Del(k);
	return 0;
}

int SimpMAP_DelL(void* pMap, long k)
{
	((CSimpleMapLKey*)pMap)->Del(k);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//		List Map
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// create
void*	ListMAP_CreateS()
{
	CListMapSKey* p = new CListMapSKey();
	p->Initialize();
	return (void*)p;
}
void*	ListMAP_CreateL()
{
	CListMapLKey* p = new CListMapLKey();
	p->Initialize();
	return (void*)p;
}

// destroy
int	ListMAP_DestroyS(void* pMap)
{
	delete (CListMapSKey*)pMap;
	return 0;
}

int	ListMAP_DestroyL(void* pMap)
{
	delete (CListMapLKey*)pMap;
	return 0;
}


// add
bool	ListMAP_AddValueS(void* pMap, char* key, RECORD_VAL* val)
{
	return ((CListMapSKey*)pMap)->AddValue(key, val);
}

bool	ListMAP_AddValueL(void* pMap, long key, RECORD_VAL* val)
{
	return ((CListMapLKey*)pMap)->AddValue(key, val);
}

// get
RECORD_VAL* ListMAP_GetRecordValueS(void* pMap, char* key, /*out*/int* pnRecordCnt)
{
	return ((CListMapSKey*)pMap)->GetValue(key, pnRecordCnt);
}

RECORD_VAL* ListMAP_GetRecordValueL(void* pMap, long key, /*out*/int* pnRecordCnt)
{
	return ((CListMapLKey*)pMap)->GetValue(key, pnRecordCnt);
}


// is exists
bool ListMAP_IsExistsS(void* pMap, char* key)
{
	return ((CListMapSKey*)pMap)->IsExists(key);
}

bool ListMAP_IsExistsL(void* pMap, long key)
{
	return ((CListMapLKey*)pMap)->IsExists(key);
}

// delete value
int ListMAP_DelS(void* pMap, char* k)
{
	((CListMapSKey*)pMap)->Del(k);
	return 0;
}

int ListMAP_DelL(void* pMap, long k)
{
	((CListMapLKey*)pMap)->Del(k);
	return 0;
}


int ListMap_MemFreeS(void* pMap, void* ptr)
{
	((CListMapSKey*)pMap)->MemFree(ptr);
	return 0;
}

int ListMap_MemFreeL(void* pMap, void* ptr)
{
	((CListMapLKey*)pMap)->MemFree(ptr);
	return 0;
}