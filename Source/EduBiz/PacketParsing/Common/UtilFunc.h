
#pragma once 
#include <Windows.h>

#pragma warning(disable:4996)


DWORD	ReportException(DWORD dExitCode, const char* psPos, _Out_ char* pzMsgBuff);
VOID	DumpErr(char* pzSrc, int nErr);