#include "IRExcept.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>


CIRExcept::CIRExcept(int nError)
{
	strcpy(m_zMsg, GetCodeMsg(nError));
}

CIRExcept::CIRExcept(char* pzMsg)
{
	strcpy(m_zMsg, pzMsg);
}


CIRExcept::~CIRExcept()
{

}

const char* CIRExcept::GetCodeMsg(int nErr)
{
	static char msg[1024];

	switch (nErr)
	{
	case ERR_COMM_INIT:	sprintf(msg, "통신연결 오류"); break;
	default:sprintf(msg, "exception 발생"); break;
	}

	return msg;
}