// Prepare.cpp : Defines the entry point for the console application.
//


#include "nn.h"
#include "pipeline.h"
#include "../src/utils/attr.h"
#include "../src/utils/err.c"
#include "../src/utils/sleep.c"


#define SOCKET_ADDRESS "ipc://REALY"


int main()
{
	int sockSvr = nn_socket(AF_SP, NN_PULL);
	if (sockSvr == -1) {
		printf("Failed create socket: %s [%d]\n", nn_err_strerror(errno), (int)errno);
		return -1;	// nn_err_abort();
	}

	int rc = nn_bind(sockSvr, SOCKET_ADDRESS);
	if (rc < 0) {
		printf("Failed bind to \"%s\": %s [%d]\n", 
			SOCKET_ADDRESS, 
			nn_err_strerror(errno),
			(int)errno);
		return -1;
	}

	for (;;) {
		char buf[1024] = { 0, };
		int bytes;
		if ((bytes = nn_recv(sockSvr, buf, 1024, NN_DONTWAIT)) < 0) {
			if (nn_errno() != EAGAIN) {
				printf("nn_recv(%s)(%d)\n", nn_err_strerror(errno), (int)errno);
			}
			else {
				nn_sleep(1);
				continue;
			}
		}
		printf("RECEIVED \"%s\"\n", buf);

		//nn_freemsg(buf);
		//delete[] buf;

	}

    return 0;
}

