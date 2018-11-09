#include "Monitor.h"
#include "../../IRUM_UTIL/BaseThread.h"


//CMonitor::CMonitor()
//{
//}
//
//
//CMonitor::~CMonitor()
//{
//}

void CMonitor::ThreadFunc()
{
	while (1)
	{
		printf("this is thread class...");
		Sleep(100);
	}
}