#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>


#include "nn.h"
#include "reqrep.h"

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "nanomsg.lib")
#define NODE0 "node0"
#define NODE1 "node1"


#define CHANNEL  "ipc://pairtest"

int sock;
char now[32];
char seq[32];

char* getCurrTime() {
	SYSTEMTIME st; GetLocalTime(&st);
	sprintf(now, "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return now;
}

int Send()
{
	char sendBuf[128];
	sprintf(sendBuf, "[CLIENT][%s]",  getCurrTime());

	int sz_n = strlen(sendBuf); // '\0' too
	int ret = (nn_send(sock, sendBuf, sz_n, 0));
	if (ret > 0) {
		printf("<SEND>%s\n", sendBuf);
	}
	else
		printf("(%s) failed!!!\n", sendBuf);

	return 0;
}

void Recv()
{
	//Send();

	char buffer[128] = { 0, };
	int result = nn_recv(sock, buffer, sizeof(buffer), 0);
	if (result > 0) {
		printf("<RECV>%s\n", buffer);

		//if (strstr(buffer, "JustSend") > 0)
		//	return;
		
	}
}

int main(const int argc, const char **argv)
{
	
	if ((sock = nn_socket(AF_SP, NN_REQ)) < 0) {
		return 0;
	}
	if (nn_connect(sock, CHANNEL) < 0) {
		return 0;
	}

	//strcpy(seq, argv[1]);

	printf("server socket create and connect ok\n");

	while (1)
	{
		printf("1 for send, 2 for recv\n");
		int type;
		scanf("%d", &type);

		if (type == 1)
			Send();
		else
			Recv();

		//switch (input)
		//{
		//case 1:
		//	RecvSend();
		//	break;
		//case 2:
		//	Send("JustSend");
		//}
	}

	return 1;
}