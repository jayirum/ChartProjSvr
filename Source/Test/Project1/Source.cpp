#include <stdio.h>
//#include "../../IRUM_UTIL/TimeInteval.h"

#include <winsock2.h>
#include <stdio.h>
#include <list>

#pragma comment(lib, "ws2_32.lib")
void main()
{
	WORD	wVersionRequired;
	WSADATA	wsaData;

	//// WSAStartup
	wVersionRequired = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequired, &wsaData))
	{

		return;
	}

	//DumpWsaData(&wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		return;
	}

	SOCKET m_sockListen = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockListen == INVALID_SOCKET)
	{

		return;
	}

	SOCKADDR_IN m_sock_addr;
	m_sock_addr.sin_family = AF_INET;
	m_sock_addr.sin_port = htons(8081);
	m_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_sockListen, (struct sockaddr*)&m_sock_addr, sizeof(m_sock_addr)) == SOCKET_ERROR)
	{
		printf("bind error\n");
		return;
	}

	if (listen(m_sockListen, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen error\n");
		return;
	}

	SOCKADDR_IN s;
	int len = sizeof(s);
	SOCKET client = accept(m_sockListen, (sockaddr*)&s, &len);
	if (client != INVALID_SOCKET)
		printf("accept:%d\n", client);
	char buffer[1024] = { 0, };

	int ret = recv(client, buffer, 1024, 0);
	if (ret == SOCKET_ERROR)
	{
		printf("recv error:%d\n", GetLastError());
	}
	else
		printf("recv(%d)(%s)\n", ret, buffer);

	getchar();
}

