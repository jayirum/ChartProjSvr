#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>


#include "nn.h"
#include "pair.h"

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "nanomsg.lib")
#define NODE0 "node0"
#define NODE1 "node1"


#define CHANNEL  "ipc://pairtest"

int sock;
char now[32];
char* getCurrTime() {
	SYSTEMTIME st; GetLocalTime(&st);
	sprintf(now, "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return now;
}

int Send(char* type)
{
	char sendBuf[128];
	sprintf(sendBuf, "[%s][%s]This is Send function...\n", getCurrTime(), type);

	int sz_n = strlen(sendBuf); // '\0' too
	int ret = (nn_send(sock, sendBuf, sz_n, 0));
	if (ret > 0) {
		printf("(%s) ok\n", sendBuf);
	}
	else
		printf("(%s) failed!!!\n", sendBuf);

	return 0;
}

void RecvSend()
{
	//int to = 100;
	//if (nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to,
	//	sizeof(to)) < 0) {
	//	fatal("nn_setsockopt");
	//}
	char buffer[128] = { 0, };
	int result = nn_recv(sock, buffer, sizeof(buffer), 0);
	if (result > 0) {
		printf("[RecvSend]Recv(%s)\n", buffer);

		//if (strstr(buffer, "JustSend") > 0)
		//	return;
		Send("RecvSend");
	}
}

int main(const int argc, const char **argv)
{
	
	if ((sock = nn_socket(AF_SP, NN_PAIR)) < 0) {
		return 0;
	}
	if (nn_bind(sock, CHANNEL) < 0) {
		return 0;
	}

	printf("server socket create and bind ok\n");

	while (1)
	{
		printf("input 1 for recvsend, input 2 for send \n");
		int input;
		//scanf("%d", &input);

		RecvSend();

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