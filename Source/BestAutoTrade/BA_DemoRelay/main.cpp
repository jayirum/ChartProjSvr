
#pragma warning(disable:4786)
#pragma warning(disable:4503)


//#include "AlgoFront.h"

#include "main.h"
#include <Windows.h>
#include <stdio.h>

#include "../Common/Demo.h"
#include "./utils/err.c"	//important
#include "Relay.h"
#include "../../IRUM_UTIL/LogMsg.h"

#pragma warning(disable:4996)

BOOL WINAPI ControlHandler(DWORD dwCtrlType);

CLogMsg		g_log;


int main(int argc, char** argv)
{
	SetConsoleCtrlHandler(ControlHandler, TRUE);

	g_log.OpenLog("./Log", "BA_DemoRelay");

	CRelay relay;
	if (!relay.Begin())
		return 0;


	getchar();


	return 0;
}



BOOL WINAPI ControlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate  
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode  
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		printf("\t\tStopping...\n");
		break;

	}
	return FALSE;
}

