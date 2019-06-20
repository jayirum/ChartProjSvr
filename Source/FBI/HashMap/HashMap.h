#pragma once

/*

###
	DLL �������̽� �Լ��� ���� �� ����
	- template �� ����ϹǷ� ������Ͽ� �Լ� ����� ���Ǹ� ���� �Ѵ�.

###
����

1) SMAP_Create ȣ���Ͽ� ��ȯ�Ǵ� �����͸� ����.
2) IsExists, Add, Del, Get �Լ� ȣ���� �� �ݵ�� �� �����͸� �Բ� �Ķ���ͷ� �־�� ��.
3) �����ϴ� Value �� Ÿ����
A. int, double �� �״�� ���
B. string �� ���� ST_VALUE �� �����Ͽ� �Ķ���ͷ� ����
C. STRUCTURE �� ���� ��� �ʵ�� �ݵ�� CHAR �迭 �̾�� ��

### ���ǻ���

1) key �� string �� ��� space ���� ����� �ؾ� �Ѵ�.
*/


#ifndef _HASH_MAP_EXPORS
#define _HASH_MAP_EXPORS extern "C" __declspec(dllexport)
#else
#define _HASH_MAP_EXPORS extern "C" __declspec(dllimport)
#endif

#include "HashMapInc.h"

//+------------------------------------------------------------
//	+ dll �� ����ϱ����� �ݵ�� ȣ���ؾ� �Ѵ�.
//	+ return value
//		- �ݵ�� ������ �ϰ� �־�� �Ѵ�.
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
//	dll �� ��� ����� �ݵ�� ȣ���ؾ� �Ѵ�. Otherwise, memory leak
//+------------------------------------------------------------
_HASH_MAP_EXPORS void	SimpMAP_Delete(void* pMap);

//+----------------------------------------------------------------
//	+ long value �� �����Ѵ�.
//	+ bUpdate �� false �ε� �̹� �ش� key �� ����Ǿ� ������ return false
//		- SMAP_GetLastErr �� ȣ���ؼ� ���� ERR_EXISTS
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
//	�ش� Key �� map �� �ִ��� ����
//+------------------------------------------------------------
_HASH_MAP_EXPORS bool	SimpMAP_IsExistsS(void* pMap, char* key);
_HASH_MAP_EXPORS bool	SimpMAP_IsExistsL(void* pMap, long key);


//+------------------------------------------------------------
//	+ key �� string value �� ��ȯ
//	+ ��ȯ�Ǵ� string value(pResullt) �� ���̵� ���� ��ȯ(pnResultLen)
//	+ �ش� key �� ������ return false
//+------------------------------------------------------------
_HASH_MAP_EXPORS bool SimpMAP_GetStringValueS	(void* pMap, char* key, /*out*/char* pResult, int nStringSize);
_HASH_MAP_EXPORS bool SimpMAP_GetLongValueS		(void* pMap, char* key, /*out*/long* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetDblValueS		(void* pMap, char* key, /*out*/double* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetRecordValueS	(void* pMap, char* key, /*out*/RECORD_VAL* pResult);


_HASH_MAP_EXPORS bool SimpMAP_GetStringValueL	(void* pMap, long key, /*out*/char* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetLongValueL		(void* pMap, long key, /*out*/long* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetDblValueL		(void* pMap, long key, /*out*/double* pResult);
_HASH_MAP_EXPORS bool SimpMAP_GetRecordValueL	(void* pMap, long key, /*out*/RECORD_VAL* pResult);

//
////+------------------------------------------------------------
////	+ key �� long value �� ��ȯ
////	+ �ش� key �� ������ return false
////+------------------------------------------------------------
//template <class KEY, class VAL>
//_HASH_MAP_EXPORS bool SMAP_GetLongValue(char* pMap, KEY k, /*out*/long* pResult)
//{
//	bool bRet = (CSimpleMap*)pMap)->GetValue(k, pResult, sizeof(long));
//	return bRet;
//}
//
////+------------------------------------------------------------
////	+ key �� double value �� ��ȯ
////	+ �ش� key �� ������ return false
////+------------------------------------------------------------
//template <class KEY, class VAL>
//_HASH_MAP_EXPORS bool SMAP_GetDblValue(char* pMap, KEY k, double* pResult)
//{
//	bool bRet = (CSimpleMap*)pMap)->GetValue(k, pResult, sizeof(double));
//	return bRet;
//}
//

//
//
////+------------------------------------------------------------
////	+ key �� structure(record) value �� ��ȯ
////	+ nRecordSize ��ŭ�� copy �Ǿ ��ȯ�ȴ�.
////	+ �ش� key �� ������ return false
////+------------------------------------------------------------
//template <class KEY, class VAL>
//_HASH_MAP_EXPORS bool SMAP_GetRecordValue(char* pMap, KEY k, /*out*/char* pResult, int nRecordSize)
//{
//	bool bRet = (CSimpleMap*)pMap)->GetValue(k, (VAL*)pResult, nRecordSize);
//	return bRet;
//}
//
////+------------------------------------------------------------
////	+ �ش� key �� value �� map ���� ����
////+------------------------------------------------------------
//template <class KEY, class VAL>
//_HASH_MAP_EXPORS void SMAP_Del(char* pMap, KEY k)
//{
//	return ((CSimpleMap*)pMap)->Del(k);
//}
//
//template <class KEY, class VAL>
//_HASH_MAP_EXPORS int SMAP_GetLastErr(char* pMap, )
//{ 
//	return ((CSimpleMap*)pMap)->GetLastErr();
//}
