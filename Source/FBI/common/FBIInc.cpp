#include "FBIInc.h"
#include <stdio.h>


char* _FBI::dealstatus(const int status, char* pzStatus)
{
	switch (status)
	{
	case DEAL_STATUS_NONSTART:
		strcpy(pzStatus, "�̽���");
		break;
	case DEAL_STATUS_ORD:
		strcpy(pzStatus, "�ֹ�");
		break;
	case DEAL_STATUS_WAIT:
		strcpy(pzStatus, "���");
		break;
	case DEAL_STATUS_RESULT:
		strcpy(pzStatus, "���");
		break;
	case DEAL_STATUS_DONE:
		strcpy(pzStatus, "�Ϸ�");
		break;
	default:
		strcpy(pzStatus, "UNKNOWN");
		break;
	}
	return pzStatus;
}


char* _FBI::packlen(int len, char* out)
{
	sprintf(out, "%0*d", FBILEN_PACKET_SIZE, len);
	return out;
}


char* _FBI::rsltCode(int code, char* out)
{
	sprintf(out, "%0*d", _FBI::FBILEN_RSLT_CODE, code);
	return out;
}

char* _FBI::now(char* out)
{
	SYSTEMTIME st; GetLocalTime(&st);
	sprintf(out, "%04d%02d%02d"
		"%02d%02d%02d%03d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return out;
}

