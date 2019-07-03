#include "BPInc.h"
#include <stdio.h>

char* _BLASHPF::Now(char* pBuf)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(pBuf, "%04d%02d%02d_%02d%02d%02d%03d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds
	);
	return pBuf;
}