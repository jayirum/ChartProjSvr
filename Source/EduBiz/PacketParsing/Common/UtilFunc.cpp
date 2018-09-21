#include "UtilFunc.h"
#include <stdio.h>



DWORD	ReportException(DWORD dExitCode, const char* psPos, _Out_ char* pzMsgBuff) // 20120510
{
	switch (dExitCode)
	{
	case EXCEPTION_ACCESS_VIOLATION: strcpy(pzMsgBuff, "EXCEPTION_ACCESS_VIOLATION");		break;
	case EXCEPTION_BREAKPOINT: strcpy(pzMsgBuff, "EXCEPTION_BREAKPOINT");			break;
	case EXCEPTION_DATATYPE_MISALIGNMENT: strcpy(pzMsgBuff, "EXCEPTION_DATATYPE_MISALIGNMENT"); break;
	case EXCEPTION_SINGLE_STEP: strcpy(pzMsgBuff, "EXCEPTION_SINGLE_STEP");			break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: strcpy(pzMsgBuff, "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"); break;
	case EXCEPTION_FLT_DENORMAL_OPERAND: strcpy(pzMsgBuff, "EXCEPTION_FLT_DENORMAL_OPERAND");	break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: strcpy(pzMsgBuff, "EXCEPTION_FLT_DIVIDE_BY_ZERO");	break;
	case EXCEPTION_FLT_INEXACT_RESULT: strcpy(pzMsgBuff, "EXCEPTION_FLT_INEXACT_RESULT");	break;
	case EXCEPTION_FLT_INVALID_OPERATION: strcpy(pzMsgBuff, "EXCEPTION_FLT_INVALID_OPERATION"); break;
	case EXCEPTION_FLT_OVERFLOW: strcpy(pzMsgBuff, "EXCEPTION_FLT_OVERFLOW");			break;
	case EXCEPTION_FLT_STACK_CHECK: strcpy(pzMsgBuff, "EXCEPTION_FLT_STACK_CHECK");		break;
	case EXCEPTION_FLT_UNDERFLOW: strcpy(pzMsgBuff, "EXCEPTION_FLT_UNDERFLOW");		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO: strcpy(pzMsgBuff, "EXCEPTION_INT_DIVIDE_BY_ZERO");	break;
	case EXCEPTION_INT_OVERFLOW: strcpy(pzMsgBuff, "EXCEPTION_INT_OVERFLOW");			break;
	case EXCEPTION_PRIV_INSTRUCTION: strcpy(pzMsgBuff, "EXCEPTION_PRIV_INSTRUCTION");		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: strcpy(pzMsgBuff, "EXCEPTION_NONCONTINUABLE_EXCEPTION"); break;
	default:sprintf(pzMsgBuff, "[except code:%d]undefined error", dExitCode); break;
	}
	return EXCEPTION_EXECUTE_HANDLER;
}



VOID	DumpErr(char* pzSrc, int nErr)
{
	LPVOID lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	printf("[%s:%d]%s", pzSrc, nErr, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

