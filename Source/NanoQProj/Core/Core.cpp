// Core.cpp : Defines the entry point for the console application.
//

#include "./utils/err.c"	//important
#include "../../IRUM_UTIL/NanoQ/NanoQWriter.h"
#include "../../IRUM_UTIL/NanoQ/NanoQReader.h"
#include <stdio.h>

BOOL g_bContinue = TRUE;
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
		g_bContinue = FALSE;
		return TRUE;
		break;

	}
	return FALSE;
}




void Write()
{
	CNanoQWriter writer;
	if (!writer.Begin("RELAY_1", 1000))
	{
		printf("%s\n", writer.GetMsg());
		return;
	}

	if (!writer.Connect())
	{
		printf("%s\n", writer.GetMsg());
		return;
	}

	char buf[1024] = { 0, };
	SYSTEMTIME st;
	int nErrCode;
	while (g_bContinue)
	{
		GetLocalTime(&st);
		sprintf(buf, "[%02d:%02d:%02d.%03d]send data......\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		size_t data_len = strlen(buf);

		int nSendLen = writer.SendData(buf, data_len, &nErrCode);
		if (nErrCode == Q_TIMEOUT)
		{
			printf("send time out.....\n");
		}
		else if (nErrCode == Q_ERROR)
		{
			printf("send err(%s)\n", writer.GetMsg());
		}
		else
		{
			printf("send success[%d](%s)\n", nSendLen, buf);
			Sleep(10);
		}
	}

	printf("before..............\n");
	writer.End();
	printf("after..............\n");
}


void Read()
{
	CNanoQReader srv;
	if (!srv.Begin("RELAY", 3000))
	{
		printf("%s\n", srv.GetMsg());
		return;
	}
	printf("begin ok...\n");

	char buf[1024] = { 0, };
	int nErrCode;
	while (1)
	{
		int nRecvLen = srv.RecvData(buf, sizeof(buf), &nErrCode);
		if (nRecvLen == Q_ERROR)
		{
			if (nErrCode == Q_TIMEOUT)
			{
				printf("recv time out.....\n");
			}
			else if (nErrCode == Q_ERROR)
			{
				printf("recv error(%s)\n", srv.GetMsg());
			}
		}
		else
		{
			printf("recv success[%d](%s)\n", nRecvLen, buf);
		}

	}
}


int main(int argc, char** argv )
{
	if (argc != 2)
	{
		printf("Usage : W => Writer, R => Reader");
		return 0;
	}

	SetConsoleCtrlHandler(ControlHandler, TRUE);

	if (*argv[1] == 'W' || *argv[1] == 'w')
	{
		Write();
	}
	if (*argv[1] == 'R' || *argv[1] == 'r')
	{
		Read();
	}

    return 0;
}

