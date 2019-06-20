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

TSimpMAP_Delete	SimpMAP_Delete;

TSimpMAP_AddValueSS SimpMAP_AddValueSS;
TSimpMAP_AddValueSL SimpMAP_AddValueSL;
TSimpMAP_AddValueSD SimpMAP_AddValueSD;
TSimpMAP_AddValueSR SimpMAP_AddValueSR;

TSimpMAP_GetStringValueS	SimpMAP_GetStringValueS;
TSimpMAP_GetLongValueS		SimpMAP_GetLongValueS;
TSimpMAP_GetDoubleValueS	SimpMAP_GetDoubleValueS;
TSimpMAP_GetRecordValueS	SimpMAP_GetRecordValueS;



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

	SimpMAP_Delete = (TSimpMAP_Delete)GetProcAddress(g_hIns, "SimpMAP_Delete");
	if (SimpMAP_Delete == NULL) {
		printf("SimpMAP_Delete is not found !\n");
		return;
	}


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

}

int main()
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

	SimpMAP_Delete(pMapSL);


	void* pMapSS = SimpMAP_CreateSS();
	SimpMAP_AddValueSS(pMapSS, "KEY_1", "VAL_1", 5, true); printf("ADD[KEY_1][VAL_1]\n");
	SimpMAP_AddValueSS(pMapSS, "KEY_2", "VAL_2", 5, true); printf("ADD[KEY_2][VAL_2]\n");
	SimpMAP_AddValueSS(pMapSS, "KEY_3", "VAL_3", 5, true); printf("ADD[KEY_3][VAL_3]\n");

	char zKey[128], zVal[128];
	SimpMAP_GetStringValueS(pMapSS, "KEY_2", zVal, sizeof(zVal));	printf("GET[KEY_2](%s)\n", zVal);
	SimpMAP_GetStringValueS(pMapSS, "KEY_1", zVal, sizeof(zVal));	printf("GET[KEY_1](%s)\n", zVal);
	SimpMAP_GetStringValueS(pMapSS, "KEY_3", zVal, sizeof(zVal));	printf("GET[KEY_3](%s)\n", zVal);

	SimpMAP_Delete(pMapSS);


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

	SimpMAP_Delete(pMapSR);

	getchar();
    return 0;
}

