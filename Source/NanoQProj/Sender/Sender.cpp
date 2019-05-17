

// Sender.cpp : Defines the entry point for the console application.
//


// Core.cpp : Defines the entry point for the console application.
//

#include "nanoqc.h"
//#include "nanoqs.h"
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
			Sleep(3000);
		}

	}
}

#if 0
void pull()
{
	CNanoQS srv;
	if (!srv.Begin("RELAY", 3000))
	{
		printf("%s\n", srv.GetMsg());
		return;
	}


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
#endif 

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage : S => SERVER, C => CLIENT");
		return 0;
	}

	if (*argv[1] == 'S' || *argv[1] == 's')
	{
		//pull();
	}
	if (*argv[1] == 'C' || *argv[1] == 'c')
	{
		push();
	}

	return 0;
}



//#include "nn.h"
//#include "pipeline.h"
//#include "../src/utils/attr.h"
//#include "../src/utils/err.c"
//#include "../src/utils/sleep.c"
//#include <stdio.h>
//#include <Windows.h>
//
//#define SOCKET_ADDRESS "ipc://REALY"
//
//
//int main()
//{
//	int sockClient = nn_socket(AF_SP, NN_PUSH);
//	if (sockClient == -1) {
//		printf("Failed create socket: %s [%d]\n", nn_err_strerror(errno), (int)errno);
//		return -1;	// nn_err_abort();
//	}
//
//	int rc = nn_connect(sockClient, SOCKET_ADDRESS);
//	if (rc < 0) {
//		printf("Failed bind to \"%s\": %s [%d]\n",
//			SOCKET_ADDRESS,
//			nn_err_strerror(errno),
//			(int)errno);
//		return -1;
//	}
//	printf("connect ok...\n");
//
//	for (;;)
//	{
//		getchar();
//
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		char data[1024];
//		sprintf(data,"[%02d:%02d:%02d.%03d]send data......\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
//		size_t data_len;
//		int rc;
//
//		data_len = strlen(data);
//
//		rc = nn_send(sockClient, data, data_len, 0);
//		if (rc < 0) {
//			fprintf(stderr, "Failed to send: %s [%d]\n",
//				nn_err_strerror(errno),
//				(int)errno);
//			nn_err_abort();
//		}
//		if (rc != (int)data_len) {
//			fprintf(stderr, "Data to send is truncated: %d != %d \n",
//				rc, (int)data_len);
//			nn_err_abort();
//		}
//		printf("send data(%d)(%s)\n", rc, data);
//	}
//	return 0;
//}
//



//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
////#include "unistd.h"
//#include "nn.h"
//#include "pipeline.h"
//#include "../src/utils/attr.h"
//#include "../src/utils/err.c"
//#include "../src/utils/sleep.c"
//#define NODE0 "node0"
//#define NODE1 "node1"
//
//#define ADDR "ipc://Relay"
//
//void
//fatal(const char *func)
//{
//	fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
//	exit(1);
//}
//
//int
//node0(const char *url)
//{
//	int sock;
//	int rv;
//
//	if ((sock = nn_socket(AF_SP, NN_PULL)) < 0) {
//		fatal("nn_socket");
//	}
//	if ((rv = nn_bind(sock, url)) < 0) {
//		fatal("nn_bind");
//	}
//	for (;;) {
//		char *buf = NULL;
//		int bytes;
//		if ((bytes = nn_recv(sock, &buf, NN_MSG, 0)) < 0) {
//			fatal("nn_recv");
//		}
//		printf("NODE0: RECEIVED \"%s\"\n", buf);
//		nn_freemsg(buf);
//	}
//}
//
//int
//node1(const char *url, const char *msg)
//{
//	int sz_msg = strlen(msg) + 1; // '\0' too
//	int sock;
//	int rv;
//	int bytes;
//
//	if ((sock = nn_socket(AF_SP, NN_PUSH)) < 0) {
//		fatal("nn_socket");
//	}
//	if ((rv = nn_connect(sock, url)) < 0) {
//		fatal("nn_connect");
//	}
//	printf("NODE1: SENDING \"%s\"\n", msg);
//	if ((bytes = nn_send(sock, msg, sz_msg, 0)) < 0) {
//		fatal("nn_send");
//	}
//	Sleep(1); // wait for messages to flush before shutting down
//	return (nn_shutdown(sock, 0));
//}
//
//int
//main(const int argc, const char **argv)
//{
//	if ((argc > 1) && (strcmp(NODE0, argv[1]) == 0))
//		return (node0(ADDR));
//
//	if ((argc > 2) && (strcmp(NODE1, argv[1]) == 0))
//		return (node1(ADDR, argv[3]));
//
//	fprintf(stderr, "Usage: pipeline %s|%s <URL> <ARG> ...'\n",
//		NODE0, NODE1);
//	return (1);
//}
