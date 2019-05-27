// Test.cpp : Defines the entry point for the console application.
//


#include "../BAUtils/BAUtils.h"
#include "test.h"
#include <stdio.h>
#include <map>
#include <string>
#include <Windows.h>


#pragma warning(disable:4996)
//using namespace std;

//void BAUtils_OpenLog(char* pzEAName);

typedef void(*TBAOpenLog)(char*);
typedef void(*TBASetComplete)();
typedef void(*TBALog)(char*);
TBAOpenLog BAOpenLog;
TBASetComplete BASetComplete; 
TBALog BALog;
//BA_UTILS void BAUtils_Log(char* pzData);
//bool BAUtils_GetClosedOrd(int& ticket)

//struct _ST
//{
//	int         ticket;
//	char      symbol[32];
//	string      comment;
//};
//
//typedef map<int, _ST&> MAP;

//HMODULE g_hIns;
#include "../Common/BAProtocol.h"

int main()
{
	CProtoSet set;
	set.Begin();
	set.SetVal(FDS_CODE, "1001");
	set.SetVal(FDS_ORD_ACTION, "O");
	set.SetVal(FDD_CLOSED_LOTS, 0.05);
	set.SetVal(FDD_CLOSED_PRC, 0.05);
	printf("(%s)\n", set.m_sBuf.c_str());

	//set.SetVal(FDS_CODE, "1002");
	set.SetVal(FDD_CLOSED_LOTS, 0.55);
	printf("(%s)\n", set.m_sBuf.c_str());

	//CProtoGet get;
	//char buf[128];
	//sprintf(buf, "1=1001%xC%x3=%x100=120331655%x", DEF_DELI, DEF_DELI, DEF_DELI, DEF_DELI);
	//
	////sprintf(buf, "1=1001%x2=C%x", DEF_DELI, DEF_DELI);
	//get.Parsing(buf, strlen(buf));

	//

	//char aa[128], bb[128], cc[128];
	//char a[128], b[128], c[128];
	////sprintf(aa, "%d=%s", FDS_CODE, "1004");
	////int size = get.Parsing(aa, strlen(aa), mapResult);
	////printf("[1]%d\n", size);

	////sprintf(bb, "%d=%s%c", FDS_CODE, "1004", DEF_DELI);
	////INT size = get.Parsing(bb, strlen(bb));
	////printf("[2]%d\n", size);

	////sprintf(cc, "%d=%s%c%d=%d", FDS_CODE, "1004", DEF_DELI, FDN_ACCNO_MASTER, 1234555);
	////mapResult.erase(mapResult.begin(), mapResult.end());
	////size = get.Parsing(cc, strlen(cc), mapResult);
	////printf("[3]%d\n", size);

	//sprintf(a, "%d=%s%c%d=%d%c", FDS_CODE, "1004", DEF_DELI, FDN_ACCNO_MASTER, 1234555, DEF_DELI);
	////mapResult.erase(mapResult.begin(), mapResult.end());
	//int size = get.Parsing(a, strlen(a));
	//printf("[4]%d\n", size);

	//sprintf(b, "%d=%s%c%d=%d%c%d%f", FDS_CODE, "1004", DEF_DELI, FDN_ACCNO_MASTER, 1234555, DEF_DELI, FDD_CLOSED_LOTS, 0.05);
	//mapResult.erase(mapResult.begin(), mapResult.end());
	//size = get.Parsing(b, strlen(b), mapResult);
	//printf("[5]%d\n", size);

	//sprintf(c, "%d=%s%c%d=%d%c%d=%f%c", FDS_CODE, "1004", DEF_DELI, FDN_ACCNO_MASTER, 1234555, DEF_DELI, FDD_CLOSED_LOTS, 0.05, DEF_DELI);
	//mapResult.erase(mapResult.begin(), mapResult.end());
	//size = get.Parsing(c, strlen(c), mapResult);
	//printf("[6]%d\n", size);
	

//	printf("1\n");
//	g_hIns = GetModuleHandleA("BAUtils.dll");
//	if (!g_hIns)
//	{
//		g_hIns = LoadLibraryA("BAUtils.dll");
//		printf("g_hIns:%p\n", g_hIns);
//	}
//
//	
//	BASetComplete = (TBASetComplete)GetProcAddress(g_hIns, "BAUtils_SetComplete");
//	if (!BASetComplete) {
//		printf("BASetComplete  is null\n");
//	}
//	else
//		printf("BASetComplete  is ok\n");
//	BAOpenLog = (TBAOpenLog)GetProcAddress(g_hIns, "BAUtils_OpenLog");
//	if(!BAOpenLog)
//		printf("BAOpenLog  is null\n");
//	else
//		printf("BAOpenLog  is ok\n");
//
//	
//	BALog = (TBALog)GetProcAddress(g_hIns, "BAUtils_Log");
//	if (!BALog)
//		printf("BALog  is null\n");
//	else
//		printf("BALog  is ok\n");
//
//	char zfile[128]; strcpy(zfile, "TEST");
//	BAOpenLog(zfile);
//	printf("filename:%s\n", zfile);
//	BALog("This is Test");
	//MAP a, b;
	//char s[128];
	//for (int i = 0; i < 3; i++)
	//{
	//	_ST pp;// = new _ST;
	//	pp.ticket = i;
	//	sprintf(pp.symbol, "[SYMBOL:%d]", i);
	//	sprintf(s, "Comment==>%s", pp.symbol);
	//	pp.comment = s;

	//	a[i] = pp;
	//}

	//memcpy(&b, &a, sizeof(a));

	//map<int, _ST&>::iterator it;
	//int ticket;
	//_ST* p;
	//for(it=b.begin(); it!=b.end(); it++)
	//{ 
	//	ticket = (*it).first;
	//	p = (_ST*)&((*it).second);

	//	printf("b-[%d](%s)(%s)\n", ticket, p->symbol, p->comment.c_str());
	//	//delete p;
	//}

	//for (it = a.begin(); it != a.end(); it++)
	//{
	//	ticket = (*it).first;
	//	p = (_ST*)&((*it).second);

	//	printf("a-[%d](%s)(%s)\n", ticket, p->symbol, p->comment.c_str());
	//	//delete p;
	//}

	//a.clear();
	//b.clear();
	getchar();
    return 0;
}

