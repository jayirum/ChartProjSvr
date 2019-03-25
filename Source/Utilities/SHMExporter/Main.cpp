#include "Main.h"
#include "../../IRUM_UTIL/ChartShmUtil.h"


int main(int argc, LPSTR *argv)
{
	if (argc == 4)
	{
		char sGroupKey[32], sTimeFrame[32],sDateTime[13], zStkCd[32];
		sprintf(zStkCd, "%s", argv[1]);
		sprintf(sTimeFrame, "%s", argv[2]);
		sprintf(sDateTime, "%s", argv[3]);
		QueueShmViewer v1;

		GET_GROUP_KEY(zStkCd, (int)atoi(sTimeFrame), sGroupKey);

		v1.GroupKeyViewer(sGroupKey, sTimeFrame, sDateTime);
		return 0;
	}
	printf("Usage SHMExporter.exe <ARTICLECODEXXXX> <TIMEFRAME99> <DATETIMEYYYYMMDDHHMM>\n");
	printf("e.g.  SHMExporter.exe YMZ7 01 201710251012\n");
	return 0;
	
	
}

