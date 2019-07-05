#pragma once

/*

###
	DLL 인터페이스 함수들 선언 및 정의
	- template 을 사용하므로 헤더파일에 함수 선언과 정의를 같이 한다.

###
사용법

1) SMAP_Create 호출하여 반환되는 포인터를 저장.
2) IsExists, Add, Del, Get 함수 호출할 때 반드시 이 포인터를 함께 파라미터로 넣어야 함.
3) 저장하는 Value 의 타입은
A. int, double 은 그대로 사용
B. string 의 경우는 ST_VALUE 에 저장하여 파라미터로 전달
C. STRUCTURE 의 경우는 모든 필드는 반드시 CHAR 배열 이어야 함

### 주의사항

1) key 가 string 인 경우 space 까지 고려를 해야 한다.
*/


#ifndef _HASH_MAP_EXPORS
#define _HASH_MAP_EXPORS extern "C" __declspec(dllexport)
#else
#define _HASH_MAP_EXPORS extern "C" __declspec(dllimport)
#endif

#include "HashMapInc.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//		Simple Map
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//+------------------------------------------------------------
//	+ dll 을 사용하기전에 반드시 호출해야 한다.
//	+ return value
//		- 반드시 저장을 하고 있어야 한다.
//+------------------------------------------------------------
_HASH_MAP_EXPORS void*	SimpMAP_CreateSS();
_HASH_MAP_EXPORS void*	SimpMAP_CreateSL();
_HASH_MAP_EXPORS void*	SimpMAP_CreateSD();
_HASH_MAP_EXPORS void*	SimpMAP_CreateSR();

_HASH_MAP_EXPORS void*	SimpMAP_CreateLS();
_HASH_MAP_EXPORS void*	SimpMAP_CreateLL();
_HASH_MAP_EXPORS void*	SimpMAP_CreateLD();
_HASH_MAP_EXPORS void*	SimpMAP_CreateLR();

//+------------------------------------------------------------
//	dll 을 사용 종료시 반드시 호출해야 한다. Otherwise, memory leak
//+------------------------------------------------------------
_HASH_MAP_EXPORS int	SimpMAP_DestroyS(void* pMap);
_HASH_MAP_EXPORS int	SimpMAP_DestroyL(void* pMap);

//+----------------------------------------------------------------
//	+ long value 를 저장한다.
//	+ bUpdate 가 false 인데 이미 해당 key 가 저장되어 있으면 return false
//		- SMAP_GetLastErr 를 호출해서 값이 ERR_EXISTS
//+----------------------------------------------------------------
_HASH_MAP_EXPORS bool	SimpMAP_AddValueSS(void* pMap, char* key, char* val,		int nValSize, bool bUpdate = true);
_HASH_MAP_EXPORS bool	SimpMAP_AddValueSL(void* pMap, char* key, long* val,		int nValSize, bool bUpdate = true);
_HASH_MAP_EXPORS bool	SimpMAP_AddValueSD(void* pMap, char* key, double* val,		int nValSize, bool bUpdate = true);
_HASH_MAP_EXPORS bool	SimpMAP_AddValueSR(void* pMap, char* key, RECORD_VAL* val,	int nValSize, bool bUpdate = true);

_HASH_MAP_EXPORS bool	SimpMAP_AddValueLS(void* pMap, long key, char* val,			int nValLen, bool bUpdate = true);
_HASH_MAP_EXPORS bool	SimpMAP_AddValueLL(void* pMap, long key, long* val,			int nValLen, bool bUpdate = true);
_HASH_MAP_EXPORS bool	SimpMAP_AddValueLD(void* pMap, long key, double* val,		int nValLen, bool bUpdate = true);
_HASH_MAP_EXPORS bool	SimpMAP_AddValueLR(void* pMap, long key, RECORD_VAL* val,	int nValLen, bool bUpdate = true);
_HASH_MAP_EXPORS bool	SimpMAP_IsExistsL(void* pMap, long key);

//+------------------------------------------------------------
//	해당 Key 가 map 에 있는지 여부
//+------------------------------------------------------------
_HASH_MAP_EXPORS bool	SimpMAP_IsExistsS(void* pMap, char* key);
_HASH_MAP_EXPORS bool	SimpMAP_IsExistsL(void* pMap, long key);


//+------------------------------------------------------------
//	+ key 의 string value 를 반환
//	+ 반환되는 string value(pResullt) 의 길이도 같이 반환(pnResultLen)
//	+ 해당 key 가 없으면 return false
//+------------------------------------------------------------
_HASH_MAP_EXPORS bool SimpMAP_GetStringValueS	(void* pMap, char* key, /*out*/char* pResult, int nStringSize);
_HASH_MAP_EXPORS bool SimpMAP_GetLongValueS		(void* pMap, char* key, /*out*/long* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetDblValueS		(void* pMap, char* key, /*out*/double* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetRecordValueS	(void* pMap, char* key, /*out*/RECORD_VAL* pResult);


_HASH_MAP_EXPORS bool SimpMAP_GetStringValueL	(void* pMap, long key, /*out*/char* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetLongValueL		(void* pMap, long key, /*out*/long* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetDblValueL		(void* pMap, long key, /*out*/double* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetRecordValueL	(void* pMap, long key, /*out*/RECORD_VAL* pResult);


//+------------------------------------------------------------
//	+ 해당 key 와 value 를 map 에서 제거
//+------------------------------------------------------------
_HASH_MAP_EXPORS int SimpMAP_DelS(void* pMap, char* k);
_HASH_MAP_EXPORS int SimpMAP_DelL(void* pMap, long k);


//_HASH_MAP_EXPORS int SMAP_GetLastErr(char* pMap, )
//{ 
//	return ((CSimpleMap*)pMap)->GetLastErr();
//}


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
_HASH_MAP_EXPORS void*	ListMAP_CreateS();
_HASH_MAP_EXPORS void*	ListMAP_CreateL();

// destroy
_HASH_MAP_EXPORS int	ListMAP_DestroyS(void* pMap);
_HASH_MAP_EXPORS int	ListMAP_DestroyL(void* pMap);

// add
_HASH_MAP_EXPORS bool	ListMAP_AddValueS(void* pMap, char* key, RECORD_VAL* val);
_HASH_MAP_EXPORS bool	ListMAP_AddValueL(void* pMap, long key, RECORD_VAL* val);

// get
_HASH_MAP_EXPORS RECORD_VAL* ListMAP_GetRecordValueS(void* pMap, char* key, /*out*/int* pnRecordCnt);
_HASH_MAP_EXPORS RECORD_VAL* ListMAP_GetRecordValueL(void* pMap, long key, /*out*/int* pnRecordCnt);

// is exists
_HASH_MAP_EXPORS bool ListMAP_IsExistsS(void* pMap, char* key);
_HASH_MAP_EXPORS bool ListMAP_IsExistsL(void* pMap, long key);

// delete value
_HASH_MAP_EXPORS int ListMAP_DelS(void* pMap, char* k);
_HASH_MAP_EXPORS int ListMAP_DelL(void* pMap, long k);

// free memory
_HASH_MAP_EXPORS int ListMap_MemFreeS(void* pMap, void* ptr);
_HASH_MAP_EXPORS int ListMap_MemFreeL(void* pMap, void* ptr);