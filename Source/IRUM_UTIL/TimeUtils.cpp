#include "TimeUtils.h"

CTimeUtils::CTimeUtils()
{}

CTimeUtils::~CTimeUtils()
{


}


void CTimeUtils::AddSeconds(_In_ char* yyyymmdd, _In_ char* hhmmss, _In_ int addSec, _Out_ char* yyyymmddhhmmss)
{
	AddTime('s', yyyymmdd, hhmmss, addSec, yyyymmddhhmmss);
}
void CTimeUtils::AddMins(_In_ char* yyyymmdd, _In_ char* hhmmss, _In_ int addMin, _Out_ char* yyyymmddhhmmss)
{
	AddTime('m', yyyymmdd, hhmmss, addMin, yyyymmddhhmmss);
}

void CTimeUtils::AddTime(_In_ char timeFrame, _In_ char* yyyymmdd, _In_ char* hhmmss, _In_ int addTime, _Out_ char* yyyymmddhhmmss)
{
	// typedef __int64. get now time
	time_t now = time(&now);

	// breadown the current time into tm struct.
	struct tm * tmLocal = localtime(&now);

	char y[32], m[32], d[32], h[32], min[32], s[32];
	sprintf(y, "%.4s", yyyymmdd);
	sprintf(m, "%.2s", yyyymmdd+4);
	sprintf(d, "%.2s", yyyymmdd+6);

	sprintf(h, "%.2s", hhmmss);
	sprintf(min, "%.2s", hhmmss+2);
	sprintf(s, "%.2s", hhmmss+4);

	tmLocal->tm_year = atoi(y) - 1900;
	tmLocal->tm_mon = atoi(m) - 1;
	tmLocal->tm_mday = atoi(d);
	tmLocal->tm_hour = atoi(h);
	tmLocal->tm_min = atoi(min);
	tmLocal->tm_sec = atoi(s);

	switch (timeFrame)
	{
	case 'Y':	tmLocal->tm_year += addTime;	break;
	case 'M':	tmLocal->tm_mon += addTime;		break;
	case 'D':	tmLocal->tm_mday += addTime;	break;
	case 'h':	tmLocal->tm_hour += addTime;	break;
	case 'm':	tmLocal->tm_min += addTime;	break;
	case 's':	tmLocal->tm_sec += addTime;	break;
	}
	
	// make new time_t data using modified tm struct
	time_t newTimeT = mktime(tmLocal);
	struct tm* newTm = localtime(&newTimeT);

	sprintf(yyyymmddhhmmss, "%04d%02d%02d%02d%02d%02d",
		newTm->tm_year + 1900,
		newTm->tm_mon + 1,
		newTm->tm_mday,
		newTm->tm_hour,
		newTm->tm_min,
		newTm->tm_sec
	);
}