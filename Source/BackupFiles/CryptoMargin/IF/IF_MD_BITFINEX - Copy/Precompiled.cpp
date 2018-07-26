#include "Precompiled.h"
#include <time.h> 



//Todo : change the function name
void Int64ToDate(__int64 n64Milliseconds, char szStringOut[])
{
	char sTime[15]; //14 characters and 1 trailing endstring character
	time_t rawtime = n64Milliseconds / 1000; //remove the trailing 1000 
											 //http://www.cplusplus.com/reference/ctime/strftime/
	strftime(sTime, 15, "%Y%m%d%H%M%S", localtime(&rawtime));
	strcpy(szStringOut, sTime);
}

void Time_tToString(time_t timeRawTime, char szStringOut[])
{
	tm* ptm = localtime(&timeRawTime);
	char szBuffer[15];
	strftime(szBuffer, 15, "%Y%m%d%H%M%S", ptm);
	strcpy(szStringOut, szBuffer);
}
