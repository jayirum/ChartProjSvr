#include "Main.h"


int main(int argc, LPSTR *argv)
{
	if (argc == 4)
	{
		char sGroupKey[5], sTimeFrame[3],sDateTime[13];
		sprintf(sGroupKey, "%s", argv[1]);
		sprintf(sTimeFrame, "%s", argv[2]);
		sprintf(sDateTime, "%s", argv[3]);
		QueueShmViewer v1;
		v1.GroupKeyViewer(sGroupKey, sTimeFrame, sDateTime);
		return 0;
	}
	printf("Usage SHMExporter.exe <ARTICLECODEXXXX> <TIMEFRAME99> <DATETIMEYYYYMMDDHHMM>\n");
	printf("e.g.  SHMExporter.exe YMZ7 01 201710251012\n");
	return 0;
	
	
}

