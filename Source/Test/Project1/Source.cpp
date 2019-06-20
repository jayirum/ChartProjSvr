
#include "Source.h"


int AddVal(string k, string v)
{
	printf("AddVal(string k, string v)\n");
	return 0;
}
int AddVal(string k, long l)
{
	printf("AddVal(string k, long l)\n");
	return 0;
}
int AddVal(long k, string v)
{
	printf("AddVal(long k, string v)\n");
	return 0;
}
int AddVal(long k, long l)
{
	printf("AddVal(long k, long l)\n");
	return 0;
}


void main()
{
	AddVal("a", "a");
	AddVal("b", 1);
	AddVal(1, "A");
	AddVal(2, 2);

	getchar();
}



//#include <stdio.h>
////#include "../../IRUM_UTIL/TimeInteval.h"
//
//#include <winsock2.h>
//#include <stdio.h>
//#include <list>
//#include "../../IRUM_UTIL/PocoODBC.h"
//
//#pragma comment(lib, "ws2_32.lib")
//void main()
//{
//
//	CPocoODBC *odbc = new CPocoODBC;
//	if (!odbc->Connect("127.0.0.1,10606", "FBI", "sa", "irumsa123!@#"))
//		printf("error\n");
//	//delete odbc;
//
//	//std::stringstream SQL;
//	//SQL << "SELECT * FROM AA_STK_MST";
//	//odbc->ExecQueryWithRs(SQL.str());
//
//	char zStkCd[128], zDate[128];
//	//while (odbc->MoveNext())
//	//{
//	//	if (odbc->GetValue("STK_CD", zStkCd))
//	//		printf("STK_CD:%s\t", zStkCd);
//	//	if (odbc->GetValue("CLOSE_DT", zDate))
//	//		printf("CLOSE_DT:%s\n", zDate);
//	//}
//
//	std::stringstream sql;
//	char cl[] = "CL";
//	CHAR zQ[1024];
//	sql << "EXEC AA_TEST '"<<cl<<"', "<<1;
//	//sprintf(zQ, "EXEC AA_GET_DEAL_INFO '%s', %d", cl,1);
//	sprintf(zQ, "EXEC AA_TEST '%s', %d", cl, 1);
//	odbc->ExecQueryWithRs(zQ);
//
//	double ticksize;
//	long dotcnt;
//	while (odbc->MoveNext())
//	{
//		if (odbc->GetValue("TM_ORD_START", zStkCd))
//			printf("TM_ORD_START:%s\t", zStkCd);
//		//if (odbc->GetValue("TICK_SIZE", &ticksize))
//		//	printf("TICK_SIZE:%.5f\t", ticksize);
//		//if (odbc->GetValue("DEAL_SEQ", &dotcnt))
//			//printf("DEAL_SEQ:%d\n", dotcnt);
//	}
//
//	delete odbc;
//
//
//	//WORD	wVersionRequired;
//	//WSADATA	wsaData;
//
//	////// WSAStartup
//	//wVersionRequired = MAKEWORD(2, 2);
//	//if (WSAStartup(wVersionRequired, &wsaData))
//	//{
//
//	//	return;
//	//}
//
//	////DumpWsaData(&wsaData);
//	//if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
//	//{
//	//	return;
//	//}
//
//	//SOCKET m_sockListen = socket(AF_INET, SOCK_STREAM, 0);
//	//if (m_sockListen == INVALID_SOCKET)
//	//{
//
//	//	return;
//	//}
//
//	//SOCKADDR_IN m_sock_addr;
//	//m_sock_addr.sin_family = AF_INET;
//	//m_sock_addr.sin_port = htons(8081);
//	//m_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//
//	//if (bind(m_sockListen, (struct sockaddr*)&m_sock_addr, sizeof(m_sock_addr)) == SOCKET_ERROR)
//	//{
//	//	printf("bind error\n");
//	//	return;
//	//}
//
//	//if (listen(m_sockListen, SOMAXCONN) == SOCKET_ERROR)
//	//{
//	//	printf("listen error\n");
//	//	return;
//	//}
//
//	//SOCKADDR_IN s;
//	//int len = sizeof(s);
//	//SOCKET client = accept(m_sockListen, (sockaddr*)&s, &len);
//	//if (client != INVALID_SOCKET)
//	//	printf("accept:%d\n", client);
//	//char buffer[1024] = { 0, };
//
//	//int ret = recv(client, buffer, 1024, 0);
//	//if (ret == SOCKET_ERROR)
//	//{
//	//	printf("recv error:%d\n", GetLastError());
//	//}
//	//else
//	//	printf("recv(%d)(%s)\n", ret, buffer);
//
//	getchar();
//}
//
