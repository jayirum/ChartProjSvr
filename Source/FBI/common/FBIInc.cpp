#include "FBIInc.h"
#include <stdio.h>


void _FBI::Dbl2Str(double in, int nTotLen, int nDotLen, std::string* out)
{
	char z[128];
	sprintf(z, "%0*.*f", nTotLen, nDotLen, in);
	*out = z;
}


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
	case DEAL_STATUS_CHARTWAIT:
		strcpy(pzStatus, "��Ʈ");
		break;
	case DEAL_STATUS_RESULTING:
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

char* _FBI::dealstatusTenOp(const int status, char* pzStatus)
{
	switch (status)
	{
	case DEAL_STATUS_NONSTART:
		strcpy(pzStatus, "�̽���");
		break;
	case DEAL_STATUS_ORD_1:
		strcpy(pzStatus, "�ֹ�_1");
		break;
	case DEAL_STATUS_ORD_2:
		strcpy(pzStatus, "�ֹ�_2");
		break;
	case DEAL_STATUS_ORD_3:
		strcpy(pzStatus, "�ֹ�_3");
		break;
	case DEAL_STATUS_ORD_4:
		strcpy(pzStatus, "�ֹ�_4");
		break;
	case DEAL_STATUS_ORD_5:
		strcpy(pzStatus, "�ֹ�_5");
		break;
	case DEAL_STATUS_ORD_6:
		strcpy(pzStatus, "�ֹ�_6");
		break;
	case DEAL_STATUS_ORD_7:
		strcpy(pzStatus, "�ֹ�_7");
		break;
	case DEAL_STATUS_ORD_8:
		strcpy(pzStatus, "�ֹ�_8");
		break;
	case DEAL_STATUS_WAIT_TENOP:
		strcpy(pzStatus, "���");
		break;
	case DEAL_STATUS_CHARTWAIT_TENOP:
		strcpy(pzStatus, "��Ʈ");
		break;
	case DEAL_STATUS_RESULTING_TENOP:
		strcpy(pzStatus, "���");
		break;
	case DEAL_STATUS_DONE_TENOP:
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

_FBI::EN_DATETP   _FBI::dateTp(char* pzNow)
{
	_FBI::EN_DATETP tp;

	if (strncmp(_FBI::TM_DEAL_START, pzNow, strlen(pzNow)) <= 0 &&
		strncmp(pzNow, "23:59:59", strlen(pzNow)) <= 0)
	{
		tp = _FBI::DATETP_TRADE;
	}

	if (strncmp("00:00:00", pzNow, strlen(pzNow)) <= 0 &&
		strncmp(pzNow, _FBI::TM_DEAL_START, strlen(pzNow)) < 0)
	{
		tp = _FBI::DATETP_NEXT;

	}
	return tp;
}

// date->yyyymmdd, time -> hh:mm:ss
char* _FBI::chartName(char* psDate, char* psTime, _Out_ char* pzChartNm)
{

	sprintf(pzChartNm, "%.8s%.2s%.2s", psDate, psTime, psTime + 3);
	return pzChartNm;
}