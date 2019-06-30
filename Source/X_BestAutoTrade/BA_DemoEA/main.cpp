
#pragma warning(disable:4786)
#pragma warning(disable:4503)


//#include "AlgoFront.h"

#include "main.h"
#include <Windows.h>
#include <stdio.h>
#include "../common/NanoQPipe.h"
#include "../common/NanoQPubSub.h"
#include "../Common/Demo.h"
#include "./utils/err.c"	//important

#pragma warning(disable:4996)

BOOL WINAPI ControlHandler(DWORD dwCtrlType);

CNanoQSub		subscriber;
CNanoQWriter	pipeWriter;
char			g_zMsg[1024];


void Subscriber()
{
	if (!subscriber.Begin(DEF_SUB_CHANNEL, DEF_RECV_TIMEOUT, 1, FALSE))
	{
		strcpy(g_zMsg, subscriber.GetMsg());
		printf("subscriber.Begin error(%s)\n", g_zMsg);
		return ;
	}
	printf("[%s]subscriber.Begin Ok\n", DEF_SUB_CHANNEL);
	if (!subscriber.Connect())
	{
		strcpy(g_zMsg, subscriber.GetMsg());
		printf("subscriber.Connect error(%s)\n", g_zMsg);
		return ;
	}
	printf("[%s]subscriber.Connect Ok\n", DEF_RELAY_CHANNEL);

	int nRet = 0, nErrCode = 0;
	char zRecvBuff[1024];
	int nBuffSize = sizeof(zRecvBuff);
	while (1)
	{
		int nRet = subscriber.RecvData(zRecvBuff, nBuffSize, &nErrCode);
		if (nRet == Q_ERROR)
		{
			if (nErrCode == Q_ERROR)
			{
				printf("subscriber.RecvData error (%s)\n", subscriber.GetMsg());
			}
			else
			{
				printf("subscriber.RecvData timeout.....\n");
			}
			continue;
		}
		printf("[RECV]%.*s\n", nRet, zRecvBuff);
	}
}

void PipeWriter()
{
	//if (!pipeWriter.Begin("ipc://BA_RELAY", DEF_SEND_TIMEOUT, 1, FALSE))
	if (!pipeWriter.Begin(NULL, DEF_SEND_TIMEOUT, 1, FALSE))
	{
		strcpy(g_zMsg, pipeWriter.GetMsg());
		printf("pipeWriter.Begin error(%s)\n", g_zMsg);
		return ;
	}
	printf("pipeWriter.Begin Ok(%s)\n", DEF_RELAY_CHANNEL);

	if (!pipeWriter.Connect())
	{
		strcpy(g_zMsg, pipeWriter.GetMsg());
		printf("pipeWriter.Connect error(%s)\n", g_zMsg);
		return ;
	}
	printf("pipeWriter.Connect Ok(%s)\n", DEF_RELAY_CHANNEL);

	int idx;
	printf("type idx:");
	scanf("%d", &idx);
	char buf[1024] = { 0, };
	int nBufLen;
	int nErrCode;
	SYSTEMTIME st;
	while (1)
	{
		GetLocalTime(&st);
		sprintf(buf, "[%02d:%02d:%02d.%03d][%d]send data......\n",
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, idx);

		nBufLen = strlen(buf);
		int nRet = pipeWriter.SendData(buf, nBufLen, &nErrCode);
		if (nRet == Q_ERROR)
		{
			if (nErrCode == Q_ERROR)
			{
				printf("pipeWriter.SendData error (%s)\n", pipeWriter.GetMsg());
			}
			else
			{
				printf("pipeWriter.SendData timeout.....\n");
			}
			continue;
		}

		printf("[RECV]%.*s\n", nBufLen, buf);
		Sleep(500);
	}
}



int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage => W : Writer, S : Subscriber");
		return 0;
	}

	SetConsoleCtrlHandler(ControlHandler, TRUE);

	if (*argv[1] == 'W' || *argv[1] == 'w')
	{
		PipeWriter();
	}
	if (*argv[1] == 'S' || *argv[1] == 's')
	{
		Subscriber();
	}


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

