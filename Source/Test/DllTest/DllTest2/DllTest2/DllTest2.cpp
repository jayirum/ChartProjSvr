// DllTest2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "../../../../IRUM_UTIL/SmartMessage.h"


#pragma pack(push, 1)
struct TFutExec
{
	//TFutExec = packed record
	char issue[8 + 1];
	//issue : array[0..8] of char;
	double gap;
	//gap : real;
	double cup;
	//cup : real;
	double sip;
	//sip : real;
	double hip;
	//hip : real;
	double lip;
	//lip : real;
	int vol;
	//vol : integer;
	double amt;
	//amt : real;
	int time;
	//time: integer;
	char side[1 + 1];
	//side: array[0..1] of char;
	char ydiffSign[1 + 1];
	//ydiffSign : array[0..1] of char;
	char chgrate[6 + 1];
	//chgrate   : array[0..6]  of char
	//end;
};

#pragma pack(pop)

CRITICAL_SECTION	m_cs;
HANDLE	m_hMutex;

CSmartMessage a, b;
long int __stdcall CallBackProc(int index, char* WorkThread, char* Message)
{
	TFutExec st;
	a.GetBinaryFieldValue((char *)fldFXExec, sizeof(TFutExec), (char*)&st);
	

	char msg[128];
	a.GetMsgOfRecvMsg(msg);
	printf("MSG===>%s\n", msg);
	if(strcmp(msg, (char*)MSG_MKT_CME_HOGA)==0)
	{
		printf("[HOGA][%s](%.5f)(%.5f)(%.5f)(%.5f)\n",
			st.issue, st.cup, st.sip, st.hip, st.lip);
	}
	if (strcmp(msg, (char*)MSG_MKT_FX_EXEC) == 0)
	{
		printf("[EXEC][%s](%.5f)(%.5f)(%.5f)(%.5f)\n",
			st.issue, st.cup, st.sip, st.hip, st.lip);
	}

	
	

	return 0;
}


long int __stdcall CallBackProc2(int index, char* WorkThread, char* Message)
{
	//WaitForSingleObject(m_hMutex, INFINITE);
	TFutExec st;
	a.GetBinaryFieldValue((char *)fldFXExec, sizeof(TFutExec), (char*)&st);
	//ReleaseMutex(m_hMutex);
	printf("\t\t[B][%s](%.5f)(%.5f)(%.5f)(%.5f)\n",
		st.issue, st.cup, st.sip, st.hip, st.lip);

	return 0;
}
int main()
{

	if (!a.Begin())
		return 0;
	if (!a.ConnectSMSrv("101.79.1.45", (long)7789) ) {
		printf("failed to connect\n");
		return 0;
	}
	printf("connect ok !\n");
	a.SetRecvCallBackFn(CallBackProc);
	//if (!a.AddDest((char*)SISE_GW, (char*)MSG_MKT_CME_HOGA))
	//	return 0;
	if (!a.AddDest((char*)SISE_GW, (char*)MSG_MKT_FX_EXEC))
	{
		printf("error\n");
		return 0;
	}

	//if (!b.Begin())
	//	return 0;

	//if (!b.ConnectSMSrv("110.4.89.204", (long)7789)) {
	//	printf("failed to connect\n");
	//	return 0;
	//}
	//printf("connect ok !\n");
	//SMEventR.AddDest(SISE_GW, MSG_MKT_FX_HOGA);
	//if (!b.AddDest((char*)SISE_GW, (char*)MSG_MKT_FX_HOGA, CallBackProc2))
		//return 0;

	getchar();
    return 0;
}

