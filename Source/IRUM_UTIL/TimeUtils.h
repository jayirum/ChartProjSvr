#pragma once
#pragma warning(disable:4996)
#include <stdlib.h>  
#include <stdio.h>
#include <time.h>


class CTimeUtils
{
public:
	CTimeUtils();
	~CTimeUtils();

	static void AddSeconds(_In_ char* yyyymmdd, _In_ char* hhmmss, _In_ int addSec, _Out_ char* yyyymmddhhmmss);
	static void AddMins(_In_ char* yyyymmdd, _In_ char* hhmmss, _In_ int addMin, _Out_ char* yyyymmddhhmmss);

private:
	static void AddTime(_In_ char timeFrame, _In_ char* yyyymmdd, _In_ char* hhmmss, _In_ int addTime, _Out_ char* yyyymmddhhmmss);
};
