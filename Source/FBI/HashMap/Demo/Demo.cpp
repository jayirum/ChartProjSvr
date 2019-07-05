// Demo.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "../HashMapIF.h"
#include <string>
#include <Windows.h>


#pragma warning(disable:4996)

TSimpMAP_CreateSS	SimpMAP_CreateSS;
TSimpMAP_CreateSL	SimpMAP_CreateSL;
TSimpMAP_CreateSD	SimpMAP_CreateSD;
TSimpMAP_CreateSR	SimpMAP_CreateSR;

TSimpMAP_CreateLS	SimpMAP_CreateLS;
//TSimpMAP_Delete	SimpMAP_Delete;

TSimpMAP_AddValueSS SimpMAP_AddValueSS;
TSimpMAP_AddValueSL SimpMAP_AddValueSL;
TSimpMAP_AddValueSD SimpMAP_AddValueSD;
TSimpMAP_AddValueSR SimpMAP_AddValueSR;

TSimpMAP_GetStringValueS	SimpMAP_GetStringValueS;
TSimpMAP_GetLongValueS		SimpMAP_GetLongValueS;
TSimpMAP_GetDoubleValueS	SimpMAP_GetDoubleValueS;
TSimpMAP_GetRecordValueS	SimpMAP_GetRecordValueS;

TListMAP_CreateS ListMAP_CreateS;
TListMAP_CreateL ListMAP_CreateL;

TListMAP_DestroyS ListMAP_DestroyS;
TListMAP_DestroyL ListMAP_DestroyL;

TListMAP_AddValueS ListMAP_AddValueS;
TListMAP_AddValueL ListMAP_AddValueL;

TListMAP_GetRecordValueS ListMAP_GetRecordValueS;
TListMAP_GetRecordValueL ListMAP_GetRecordValueL;

TListMAP_DelS	ListMAP_DelS;

TListMap_MemFreeS	ListMap_MemFreeS;
TListMap_MemFreeL	ListMap_MemFreeL;

HMODULE g_hIns = nullptr;

void Initialize()
{
	g_hIns = GetModuleHandleA("HashMap.dll");

	if (g_hIns)
	{
		return ;
	}
	g_hIns = LoadLibraryA("HashMap.dll");
	if (!g_hIns)
	{
		printf("DLL not found !\n");
		return;
	}

	SimpMAP_CreateSS = (TSimpMAP_CreateSS)GetProcAddress(g_hIns, "SimpMAP_CreateSS");
	if (SimpMAP_CreateSS == NULL) {
		printf("SimpMAP_CreateSS is not found !\n");
		return ;
	}
	SimpMAP_CreateSL = (TSimpMAP_CreateSL)GetProcAddress(g_hIns, "SimpMAP_CreateSL");
	if (SimpMAP_CreateSS == NULL) {
		printf("SimpMAP_CreateSL is not found !\n");
		return;
	}
	SimpMAP_CreateSD = (TSimpMAP_CreateSD)GetProcAddress(g_hIns, "SimpMAP_CreateSD");
	if (SimpMAP_CreateSD == NULL) {
		printf("SimpMAP_CreateSD is not found !\n");
		return;
	}
	SimpMAP_CreateSR = (TSimpMAP_CreateSR)GetProcAddress(g_hIns, "SimpMAP_CreateSR");
	if (SimpMAP_CreateSR == NULL) {
		printf("SimpMAP_CreateSR is not found !\n");
		return;
	}

	SimpMAP_CreateLS = (TSimpMAP_CreateLS)GetProcAddress(g_hIns, "SimpMAP_CreateLS");
	if (SimpMAP_CreateLS == NULL) {
		printf("SimpMAP_CreateLS is not found !\n");
		return;
	}

	//SimpMAP_Delete = (TSimpMAP_Delete)GetProcAddress(g_hIns, "SimpMAP_Delete");
	//if (SimpMAP_Delete == NULL) {
	//	printf("SimpMAP_Delete is not found !\n");
	//	return;
	//}


	SimpMAP_AddValueSS = (TSimpMAP_AddValueSS)GetProcAddress(g_hIns, "SimpMAP_AddValueSS");
	if (SimpMAP_AddValueSS == NULL) {
		printf("SimpMAP_AddValueSS is not found !\n");
		return;
	}
	SimpMAP_AddValueSL = (TSimpMAP_AddValueSL)GetProcAddress(g_hIns, "SimpMAP_AddValueSL");
	if (SimpMAP_AddValueSL == NULL) {
		printf("SimpMAP_AddValueSL is not found !\n");
		return;
	}
	SimpMAP_AddValueSD = (TSimpMAP_AddValueSD)GetProcAddress(g_hIns, "SimpMAP_AddValueSD");
	if (SimpMAP_AddValueSD == NULL) {
		printf("SimpMAP_AddValueSD is not found !\n");
		return;
	}
	SimpMAP_AddValueSR = (TSimpMAP_AddValueSR)GetProcAddress(g_hIns, "SimpMAP_AddValueSR");
	if (SimpMAP_AddValueSR == NULL) {
		printf("SimpMAP_AddValueSR is not found !\n");
		return;
	}

	SimpMAP_GetStringValueS = (TSimpMAP_GetStringValueS)GetProcAddress(g_hIns, "SimpMAP_GetStringValueS");
	if (SimpMAP_GetStringValueS == NULL) {
		printf("SimpMAP_GetStringValueS is not found !\n");
		return;
	}

	SimpMAP_GetLongValueS = (TSimpMAP_GetLongValueS)GetProcAddress(g_hIns, "SimpMAP_GetLongValueS");
	if (SimpMAP_GetLongValueS == NULL) {
		printf("SimpMAP_GetLongValueS is not found !\n");
		return;
	}

	
	SimpMAP_GetRecordValueS = (TSimpMAP_GetRecordValueS)GetProcAddress(g_hIns, "SimpMAP_GetRecordValueS");
	if (SimpMAP_GetRecordValueS == NULL) {
		printf("SimpMAP_GetRecordValueS is not found !\n");
		return;
	}


	//+---------------------------------------------------------------------------------+//
	//+---------------------------------------------------------------------------------+//
	//+---------------------------------------------------------------------------------+//

	ListMAP_CreateS = (TListMAP_CreateS)GetProcAddress(g_hIns, "ListMAP_CreateS");
	if (ListMAP_CreateS == NULL) {
		printf("ListMAP_CreateS is not found !\n");
		return;
	}

	ListMAP_CreateL = (TListMAP_CreateL)GetProcAddress(g_hIns, "ListMAP_CreateL");
	if (ListMAP_CreateL == NULL) {
		printf("ListMAP_CreateL is not found !\n");
		return;
	}

	ListMAP_DestroyS = (TListMAP_DestroyS)GetProcAddress(g_hIns, "ListMAP_DestroyS");
	if (ListMAP_DestroyS == NULL) {
		printf("ListMAP_DestroyS is not found !\n");
		return;
	}

	ListMAP_DestroyS = (TListMAP_DestroyS)GetProcAddress(g_hIns, "ListMAP_DestroyS");
	if (ListMAP_DestroyS == NULL) {
		printf("ListMAP_DestroyS is not found !\n");
		return;
	}

	ListMAP_AddValueS = (TListMAP_AddValueS)GetProcAddress(g_hIns, "ListMAP_AddValueS");
	if (ListMAP_AddValueS == NULL) {
		printf("ListMAP_AddValueS is not found !\n");
		return;
	}

	ListMAP_AddValueL = (TListMAP_AddValueL)GetProcAddress(g_hIns, "ListMAP_AddValueL");
	if (ListMAP_AddValueL == NULL) {
		printf("ListMAP_AddValueL is not found !\n");
		return;
	}

	ListMAP_GetRecordValueS = (TListMAP_GetRecordValueS)GetProcAddress(g_hIns, "ListMAP_GetRecordValueS");
	if (ListMAP_GetRecordValueS == NULL) {
		printf("ListMAP_GetRecordValueS is not found !\n");
		return;
	}

	ListMAP_GetRecordValueL = (TListMAP_GetRecordValueL)GetProcAddress(g_hIns, "ListMAP_GetRecordValueL");
	if (ListMAP_GetRecordValueL == NULL) {
		printf("ListMAP_GetRecordValueL is not found !\n");
		return;
	}

	ListMap_MemFreeS = (TListMap_MemFreeS)GetProcAddress(g_hIns, "ListMap_MemFreeS");
	if (ListMap_MemFreeS == NULL) {
		printf("ListMap_MemFreeS is not found !\n");
		return;
	}

	ListMap_MemFreeL = (TListMap_MemFreeL)GetProcAddress(g_hIns, "ListMap_MemFreeL");
	if (ListMap_MemFreeL == NULL) {
		printf("ListMap_MemFreeL is not found !\n");
		return;
	}

}



int main()
{
	Initialize();

	void* pMapS = ListMAP_CreateS();
	RECORD_VAL rec;

	strcpy(rec.val, "1111111111111111");
	ListMAP_AddValueS(pMapS, "KEY_1", &rec); printf("ADD[KEY_1][%s]\n", rec.val);

	strcpy(rec.val, "2222222222222222");
	ListMAP_AddValueS(pMapS, "KEY_1", &rec); printf("ADD[KEY_1][%s]\n", rec.val);

	strcpy(rec.val, "3333333333333333");
	ListMAP_AddValueS(pMapS, "KEY_1", &rec); printf("ADD[KEY_1][%s]\n", rec.val);

	strcpy(rec.val, "aaaaaaaaaaa");
	ListMAP_AddValueS(pMapS, "KEY_2", &rec); printf("ADD[KEY_2][%s]\n", rec.val);

	strcpy(rec.val, "bbbbbbbbbbb");
	ListMAP_AddValueS(pMapS, "KEY_2", &rec); printf("ADD[KEY_2][%s]\n", rec.val);


	int nRecordCnt = 0;
	RECORD_VAL* result = ListMAP_GetRecordValueS(pMapS, "KEY_1", &nRecordCnt);
	for (int i = 0; i < nRecordCnt; i++)
	{
		printf("A[%d](%s)\n", i, (result+i)->val);
	}
	ListMap_MemFreeS(pMapS, result);


	result = nullptr;
	nRecordCnt = 0;
	result = ListMAP_GetRecordValueS(pMapS, "KEY_2", &nRecordCnt);
	for (int i = 0; i < nRecordCnt; i++)
	{
		printf("B[%d](%s)\n", i, result[i].val);
	}
	ListMap_MemFreeS(pMapS, result);

	ListMAP_DestroyS(pMapS);

	getchar();
	return 0;
}




int main_simle()
{
	Initialize();

	void* pMapSL = SimpMAP_CreateSL();
	long val = 1;
	SimpMAP_AddValueSL(pMapSL, "KEY_1", &val, sizeof(long), true); printf("ADD[KEY_1][%d]\n", val);

	val = 2;
	SimpMAP_AddValueSL(pMapSL, "KEY_2", &val, sizeof(long), true); printf("ADD[KEY_2][%d]\n", val);

	val = 3;
	SimpMAP_AddValueSL(pMapSL, "KEY_3", &val, sizeof(long), true); printf("ADD[KEY_3][%d]\n", val);

	long newVal;
	SimpMAP_GetLongValueS(pMapSL, "KEY_2", &newVal);	printf("GET[KEY_2](%d)\n", newVal);
	SimpMAP_GetLongValueS(pMapSL, "KEY_1", &newVal);	printf("GET[KEY_1](%d)\n", newVal);
	SimpMAP_GetLongValueS(pMapSL, "KEY_3", &newVal);	printf("GET[KEY_3](%d)\n", newVal);

	//SimpMAP_Delete(pMapSL);


	void* pMapSS = SimpMAP_CreateSS();
	SimpMAP_AddValueSS(pMapSS, "KEY_1", "VAL_1", 5, true); printf("ADD[KEY_1][VAL_1]\n");
	SimpMAP_AddValueSS(pMapSS, "KEY_2", "VAL_2", 5, true); printf("ADD[KEY_2][VAL_2]\n");
	SimpMAP_AddValueSS(pMapSS, "KEY_3", "VAL_3", 5, true); printf("ADD[KEY_3][VAL_3]\n");

	char zKey[128], zVal[128];
	SimpMAP_GetStringValueS(pMapSS, "KEY_2", zVal, sizeof(zVal));	printf("GET[KEY_2](%s)\n", zVal);
	SimpMAP_GetStringValueS(pMapSS, "KEY_1", zVal, sizeof(zVal));	printf("GET[KEY_1](%s)\n", zVal);
	SimpMAP_GetStringValueS(pMapSS, "KEY_3", zVal, sizeof(zVal));	printf("GET[KEY_3](%s)\n", zVal);

	//SimpMAP_Delete(pMapSS);


	void* pMapSR = SimpMAP_CreateSR();
	RECORD_VAL rec;

	strcpy(rec.val, "1111111111111111");
	SimpMAP_AddValueSR(pMapSS, "KEY_1", &rec, sizeof(rec), true); printf("ADD[KEY_1][%s]\n", rec.val);

	strcpy(rec.val, "2222222222222222");
	SimpMAP_AddValueSR(pMapSS, "KEY_2", &rec, sizeof(rec), true); printf("ADD[KEY_2][%s]\n", rec.val);
	
	strcpy(rec.val, "3333333333333333");
	SimpMAP_AddValueSR(pMapSS, "KEY_3", &rec, sizeof(rec), true); printf("ADD[KEY_3][%s]\n", rec.val);

	ZeroMemory(rec.val, sizeof(rec));
	SimpMAP_GetRecordValueS(pMapSR, "KEY_2", &rec);	printf("GET[KEY_2](%s)\n", rec.val);
	SimpMAP_GetRecordValueS(pMapSR, "KEY_1", &rec);	printf("GET[KEY_1](%s)\n", rec.val);
	SimpMAP_GetRecordValueS(pMapSR, "KEY_3", &rec);	printf("GET[KEY_3](%s)\n", rec.val);

	//SimpMAP_Delete(pMapSR);

	getchar();
    return 0;
}

