
#pragma warning(disable:4786)
#pragma warning(disable:4503)


#define BA_RELAY_EXPORTS

#include "main.h"
#include <Windows.h>
#include <stdio.h>
#include "../BARelayEA/BARelayEA.h"
#include <stdlib.h>
#include "../../IRUM_UTIL/LogMsg.h"

using namespace _BA_RELAY;

#pragma warning(disable:4996)

TBASender_InitChannel BASender_InitChannel;
TBASender_DeInitChannel BASender_DeInitChannel;
TBASender_RegisterAsMaster BASender_RegisterAsMaster;
TBASender_UnRegisterAsMaster BASender_UnRegisterAsMaster;
TBASender_SendOrder BASender_SendOrder;
TBAReceiver_InitSlaveChannel BAReceiver_InitSlaveChannel;
TBAReceiver_DeInitSlaveChannel BAReceiver_DeInitSlaveChannel;
TBASender_RegisterAsSlave BASender_RegisterAsSlave;
TBASender_UnRegisterAsSlave BASender_UnRegisterAsSlave;
TBASender_SendData BASender_SendData;
TBAReceiver_RecvData BAReceiver_RecvData;
TBASender_GetMsg BASender_GetMsg;
TBAReceiver_GetMsg BAReceiver_GetMsg;
TBASender_ChannelNm BASender_ChannelNm;
TBAReceiver_ChannelNm BAReceiver_ChannelNm;


BOOL WINAPI ControlHandler(DWORD dwCtrlType);

char g_zMsg[1024];
int	g_idxSender;
int g_ordNo = 0;
CLogMsg		g_log;
HMODULE		g_hIns = NULL;

struct _ACC_NO
{
	char Mine[32];
	char Master[32];
};

_ACC_NO g_accNo;

char whoami[32];


void DoSlave()
{
	int ret = BAReceiver_InitSlaveChannel(g_accNo.Master, DEF_RECV_TIMEOUT);
	if (ret != Q_SUCCESS)
	{
		g_log.log(ERR, "[%s]DoSlave BAReceiver_InitSlaveChannel error:%s\n",whoami, BAReceiver_GetMsg());
		return;
	}
	g_log.log(INFO, "[%s]DoSlave OK(%s)\n", whoami, BAReceiver_ChannelNm());
	
	int nRet = 0, nErrCode = 0;
	char zRecvBuff[1024];
	int nBuffSize = sizeof(zRecvBuff);
	while (1)
	{
		int nRet = BAReceiver_RecvData(zRecvBuff, nBuffSize);
		if (nRet == Q_ERROR)
		{
			g_log.log(ERR, "[%s]DoSlave BAReceiver_RecvData err(%s)", whoami, BAReceiver_GetMsg());
		}
		else if (nRet == Q_TIMEOUT)
		{
			continue;
		}
		else
		{
			g_log.log(INFO, "[%s]DoSlave BAReceiver_RecvData OK (%.*s)", whoami,nRet, zRecvBuff);

		}
	}

	BAReceiver_DeInitSlaveChannel();
}


void RegisterAsSlave()
{
	int ret = BASender_InitChannel(DEF_SEND_TIMEOUT);
	if (ret != Q_SUCCESS)
	{
		printf("[%s]RegisterAsSlave InitSendChannel error(%s)\n", whoami, BASender_GetMsg());
		return;
	}

	printf("[%s]RegisterAsSlave BASender_InitChannel Ok(%s)\n", whoami, BASender_ChannelNm());

	char zSendBuffer[1024] = { 0, };
	int nBufLen = sizeof(zSendBuffer);
	int nRet = BASender_RegisterAsSlave(g_accNo.Mine, g_accNo.Master, zSendBuffer, nBufLen);
	g_log.log(INFO, "[%s][RegisterAsSlave PACKET](%s)\n", whoami, zSendBuffer);

	if (nRet == Q_ERROR) g_log.log(ERR, "[%s]RegisterAsSlave ERR(%s)\n", whoami, BASender_GetMsg());
	else if (nRet == Q_TIMEOUT) g_log.log(INFO,"[%s]RegisterAsSlave Time out.....\n", whoami);
	else g_log.log(INFO, "[%s]RegisterAsSlave Ok!\n", whoami);

}

void DoMaster()
{
	int ret = BASender_InitChannel(DEF_SEND_TIMEOUT);
	if (ret != Q_SUCCESS)
	{
		printf("[%s]DoMaster InitSendChannel error:%s\n", whoami, BASender_GetMsg());
		return;
	}

	g_log.log(INFO, "[%s]DoMaster InitSendChannel(%s) ok\n", whoami, BASender_ChannelNm());
	
	Sleep(100);	//for logging

	char zSendBuffer[1024];
	char zCommand[128];
	int nBufLen = sizeof(zSendBuffer);
	int nRet;
	SYSTEMTIME st;
	char zTm[32];
	while (1)
	{
		ZeroMemory(zSendBuffer, sizeof(zSendBuffer));
		ZeroMemory(zCommand, sizeof(zCommand));

		printf("\nRegister Master   : type 'R'\n");
		printf("UnRegister Master : type 'U'\n");
		printf("Send Order        : type 'O'\n");

		scanf("%1s", zCommand);
		if(!isupper(zCommand[0]))
			zCommand[0] = _toupper(zCommand[0]);
		if (zCommand[0] != 'R' && zCommand[0] != 'U' && zCommand[0] != 'O')
			continue;

		if (zCommand[0] == TP_REG)
		{
			nRet = BASender_RegisterAsMaster(g_accNo.Mine, zSendBuffer, nBufLen);
			g_log.log(INFO, "[%s][RegisterAsMaster PACKET](%s)\n",whoami, zSendBuffer);
		}
		else if (zCommand[0] == TP_UNREG)
		{
			nRet = BASender_UnRegisterAsMaster(g_accNo.Mine, zSendBuffer, nBufLen);
			g_log.log(INFO, "[%s][UnRegisterAsMaster PACKET](%s)\n",whoami, zSendBuffer);
		}
		else if (zCommand[0] == TP_ORDER)
		{
			while (1) {
				GetLocalTime(&st); sprintf(zTm, "%02d%02d%02d", st.wHour, st.wMinute, st.wSecond);

				int randNum = rand();
				double dPrc = 1.1 + ((double)randNum / 100000);
				nRet = BASender_SendOrder(
					g_accNo.Mine
					, g_accNo.Master
					, ++g_ordNo
					, "EURUSD"
					, "O"		// O:Open, C:Close
					, "B"		// B:Buy, S:Sell
					, 5
					, dPrc
					, 0
					, 0
					, 0.5
					, "MK"
					, zTm
					, 0
					, 0
					, ""
					, 0
					, 0
					, zSendBuffer
					, nBufLen
				);
				g_log.log(INFO, "[%s][Sender_SendOrder PACKET](%s)\n", whoami, zSendBuffer);
				Sleep(3000);
			}
		} // if (zCommand[0] == 'O')
		else 
		{
			g_log.log(ERR, "Non defined.....");
		}
		if (nRet == Q_ERROR) 
			g_log.log(ERR, "[%s]DoMaster ERR(%s)\n", whoami, BASender_GetMsg());
		else if (nRet == Q_TIMEOUT) 
			g_log.log(INFO, "[%s]DoMaster Time out.....\n", whoami);
		else if (nRet == Q_SUCCESS)
			g_log.log(INFO, "[%s]DoMaster Ok!\n", whoami);

		Sleep(1000);

	}// while (1)
	BASender_DeInitChannel();
}

BOOL InitDll()
{
	g_hIns = GetModuleHandleA("BARelayEA.dll");

	if (g_hIns)
	{
		return TRUE;
	}
	g_hIns = LoadLibraryA("BARelayEA.dll");
	if (!g_hIns)
	{
		g_log.log(ERR,"[%s]DLL not found !\n", whoami);
		return TRUE;
	}

	BASender_InitChannel = (TBASender_InitChannel)GetProcAddress(g_hIns, "Sender_InitChannel");
	if (BASender_InitChannel == NULL) {
		g_log.log(ERR, "[%s]Sender_InitChannel is not found !\n", whoami);
		return TRUE;
	}

	BASender_DeInitChannel = (TBASender_DeInitChannel)GetProcAddress(g_hIns, "Sender_DeInitChannel");
	if (BASender_DeInitChannel == NULL) {
		g_log.log(ERR, "[%s]Sender_DeInitChannel is not found !\n", whoami);
		return TRUE;
	}

	BASender_RegisterAsMaster = (TBASender_RegisterAsMaster)GetProcAddress(g_hIns, "Sender_RegisterAsMaster");
	if (BASender_RegisterAsMaster == NULL) {
		g_log.log(ERR, "[%s]Sender_RegisterAsMaster is not found !\n", whoami);
		return TRUE;
	}

	BASender_UnRegisterAsMaster = (TBASender_UnRegisterAsMaster)GetProcAddress(g_hIns, "Sender_UnRegisterAsMaster");
	if (BASender_UnRegisterAsMaster == NULL) {
		g_log.log(ERR, "[%s]Sender_UnRegisterAsMaster is not found !\n", whoami);
		return TRUE;
	}

	BASender_SendOrder = (TBASender_SendOrder)GetProcAddress(g_hIns, "Sender_SendOrder");
	if (BASender_SendOrder == NULL) {
		g_log.log(ERR, "[%s]Sender_SendOrder is not found !\n", whoami);
		return TRUE;
	}

	BAReceiver_InitSlaveChannel = (TBAReceiver_InitSlaveChannel)GetProcAddress(g_hIns, "Receiver_InitSlaveChannel");
	if (BAReceiver_InitSlaveChannel == NULL) {
		g_log.log(ERR, "[%s]Receiver_InitSlaveChannel is not found !\n", whoami);
		return TRUE;
	}

	BAReceiver_DeInitSlaveChannel = (TBAReceiver_DeInitSlaveChannel)GetProcAddress(g_hIns, "Receiver_DeInitSlaveChannel");
	if (BAReceiver_DeInitSlaveChannel == NULL) {
		g_log.log(ERR, "[%s]Receiver_DeInitSlaveChannel is not found !\n", whoami);
		return TRUE;
	}


	BASender_RegisterAsSlave = (TBASender_RegisterAsSlave)GetProcAddress(g_hIns, "Sender_RegisterAsSlave");
	if (BASender_RegisterAsSlave == NULL) {
		g_log.log(ERR, "[%s]Sender_RegisterAsSlave is not found !\n", whoami);
		return TRUE;
	}

	BASender_UnRegisterAsSlave = (TBASender_UnRegisterAsSlave)GetProcAddress(g_hIns, "Sender_UnRegisterAsSlave");
	if (BASender_UnRegisterAsSlave == NULL) {
		g_log.log(ERR, "[%s]Sender_UnRegisterAsSlave is not found !\n", whoami);
		return TRUE;
	}
	
	BASender_SendData = (TBASender_SendData)GetProcAddress(g_hIns, "Sender_SendData");
	if (BASender_SendData == NULL) {
		g_log.log(ERR, "[%s]Sender_SendData is not found !\n", whoami);
		return TRUE;
	}

	BAReceiver_RecvData = (TBAReceiver_RecvData)GetProcAddress(g_hIns, "Receiver_RecvData");
	if (BAReceiver_RecvData == NULL) {
		g_log.log(ERR, "[%s]Receiver_RecvData is not found !\n", whoami);
		return TRUE;
	}


	BASender_GetMsg = (TBASender_GetMsg)GetProcAddress(g_hIns, "Sender_GetMsg");
	if (BASender_GetMsg == NULL) {
		g_log.log(ERR, "[%s]Sender_GetMsg is not found !\n", whoami);
		return TRUE;
	}

	BAReceiver_GetMsg = (TBAReceiver_GetMsg)GetProcAddress(g_hIns, "Receiver_GetMsg");
	if (BAReceiver_GetMsg == NULL) {
		g_log.log(ERR, "[%s]Receiver_GetMsg is not found !\n", whoami);
		return TRUE;
	}

	BASender_ChannelNm = (TBASender_ChannelNm)GetProcAddress(g_hIns, "Sender_ChannelNm");
	if (BASender_ChannelNm == NULL) {
		g_log.log(ERR, "[%s]Sender_ChannelNm is not found !\n", whoami);
		return TRUE;
	}

	BAReceiver_ChannelNm = (TBAReceiver_ChannelNm)GetProcAddress(g_hIns, "Receiver_ChannelNm");
	if (BAReceiver_ChannelNm == NULL) {
		g_log.log(ERR, "[%s]Receiver_ChannelNm is not found !\n", whoami);
		return TRUE;
	}

	return TRUE;
}

void DeInitDll()
{
	FreeLibrary(g_hIns);
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("Usage => S : Slave [idx]\n");
		printf("Usage => M : Master 0\n");
		return 0;
	}

	g_log.OpenLog("./Log", "BA_DemoEADll"); 
	
	InitDll();

	SetConsoleCtrlHandler(ControlHandler, TRUE);

	
	if (*argv[1] == 'M' || *argv[1] == 'm')
	{
		strcpy(whoami, "MASTER");
		strcpy(g_accNo.Mine, "MASTER_DEMOACC");
		strcpy(g_accNo.Master, "MASTER_DEMOACC");
		g_log.log(INFO, "[%s]Mine(%s) Master(%s)",
			whoami, g_accNo.Mine, g_accNo.Master);
		DoMaster();
	}
	if (*argv[1] == 'S' || *argv[1] == 's')
	{
		g_idxSender = atoi(argv[2]);
		strcpy(whoami, "SLAVE");
		sprintf(g_accNo.Mine, "SLAVE_DEMOACC_%d", g_idxSender);
		strcpy(g_accNo.Master, "MASTER_DEMOACC");

		g_log.log(INFO, "[%s]Mine(%s) Master(%s)",
			whoami, g_accNo.Mine, g_accNo.Master);

		RegisterAsSlave();
		DoSlave();
	}

	DeInitDll();

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
		DeInitDll();
		break;

	}
	return FALSE;
}

