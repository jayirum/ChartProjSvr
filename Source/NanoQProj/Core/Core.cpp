// Core.cpp : Defines the entry point for the console application.
//

#include "./utils/err.c"	//important
#include "../../IRUM_UTIL/NanoQ/NanoQC.h"
#include "../../IRUM_UTIL/NanoQ/NanoQS.h"
#include <stdio.h>

void push()
{
	CNanoQC client;
	if (!client.Begin("RELAY", 3000))
	{
		printf("%s\n", client.GetMsg());
		return;
	}

	if (!client.Connect())
	{
		printf("%s\n", client.GetMsg());
		return;
	}

	char buf[1024] = { 0, };
	SYSTEMTIME st;
	int nErrCode;
	while (1)
	{
		GetLocalTime(&st);
		sprintf(buf, "[%02d:%02d:%02d.%03d]send data......\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		size_t data_len = strlen(buf);

		int nSendLen = client.SendData(buf, data_len, &nErrCode);
		if (nSendLen == Q_ERROR)
		{
			if (nErrCode == Q_TIMEOUT)
			{
				printf("send time out.....\n");
			}
			else if (nErrCode == Q_ERROR)
			{
				printf("send err(%s)\n", client.GetMsg());
			}
		}
		else
		{
			printf("send success[%d](%s)\n", nSendLen, buf);
			Sleep(10);
		}

	}
}


void pull()
{
	CNanoQS srv;
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
		printf("Usage : S => SERVER, C => CLIENT");
		return 0;
	}

	if (*argv[1] == 'S' || *argv[1] == 's')
	{
		pull();
	}
	if (*argv[1] == 'C' || *argv[1] == 'c')
	{
		push();
	}

    return 0;
}

