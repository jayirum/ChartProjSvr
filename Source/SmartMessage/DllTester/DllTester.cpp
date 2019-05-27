// DllTester.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "SMDataTypes.h"
#include <Windows.h>
#include <stdio.h>

#pragma warning (disable:4996)

TInitialize Initialize;
TSMClientConnect SMClientConnect;
TSMClientDisconnect SMClientDisconnect;
TSMClientIsConnected SMClientIsConnected;
TSMSetMessageParameters SMSetMessageParameters;
TSMSetMessageBinaryField SMSetMessageBinaryField;
TSMSetMessageStringField SMSetMessageStringField;
TSMSetMessageIntegerField SMSetMessageIntegerField;
TSMSendMessage SMSendMessage;
TSMGetClientUniqKey SMGetClientUniqKey;
TSMGetClientIP SMGetClientIP;
TSMEventAddDestination SMEventAddDestination;
TSMEventRemoveDestination SMEventRemoveDestination;
TSMEventAllRemoveDestination SMEventAllRemoveDestination;
TSMGetReceivedCnt SMGetReceivedCnt;
TSMSetWorkEventCallBack SMSetWorkEventCallBack;
TSMMessageGetBinaryFieldValue SMMessageGetBinaryFieldValue;
TSMMessageGetStringFieldValue SMMessageGetStringFieldValue;
TSMMessageGetIntegerFieldValue SMMessageGetIntegerFieldValue;
TSMGetObjectsNumber SMGetObjectsNumber;
TSMGetMaximumObjectsNumber SMGetMaximumObjectsNumber;
TSMCreateInstance SMCreateInstance;
TSMMessageGetDeliveryMode SMMessageGetDeliveryMode;
TSMSendResponse SMSendResponse;
TSMMessageGetDestination SMMessageGetDestination;
TSMMessageGetMsg SMMessageGetMsg;
TSMMessageGetClientSession SMMessageGetClientSession;
TSMSMessageToSMessageEx SMSMessageToSMessageEx;
TSMGetMsgOfRecvMsg SMGetMsgOfRecvMsg;
TSetSMMessageHeader SetSMMessageHeader;

HMODULE	g_hIns = NULL;
long	g_lIdx = 0;
int		g_cnt = 0;

BOOL InitDll();
VOID DeInitDll(int idx);


int __stdcall CallBackProc(int index, char* WorkThread, char* Message)
{
	char* fv;
	g_cnt++;
	//printf("[%d]Our CallBack func !\n", g_cnt);

	fv = SMMessageGetBinaryFieldValue(index, RECEIVED_MESSAGE, (char *)fldFXExec);
	TFutExec FutExec;
	memcpy(&FutExec, fv, sizeof(FutExec));
	//ShowMessage(FutExec.issue);

	char deststr[255];
	char msgstr[255];
	char* destination = SMMessageGetDestination(index, RECEIVED_MESSAGE);
	strcpy(&deststr[0], destination);

	char* msg = SMMessageGetMsg(index, RECEIVED_MESSAGE);
	strcpy(&msgstr[0], msg);

	printf("[dest](%s)__[msg](%s)\n", deststr, msgstr);

	printf("[%d][IDX:%d](ISSUE:%s)(TIME:%d) (%.5f)(%.5f)\n",
		g_cnt, index, FutExec.issue, FutExec.time, FutExec.cup, FutExec.hip);

	return 0;
}

void main(int argc, char** argv)
{

	if (InitDll())
	{
		g_lIdx = SMCreateInstance() - 1;
		//g_lIdx = SMCreateInstance() - 1;
		//g_lIdx = 0;
		printf("SMCreateInstance ok\n");

		if (SMClientConnect(g_lIdx, "110.4.89.204", (long)7789) < 0) {
			printf("failed to connect\n");
			return;
		}
		printf("connect ok !\n");

		SMEventAddDestination(g_lIdx, (char*)SISE_GW, (CHAR*)"");	// MSG_MKT_FX_EXEC);	// (char*)MSG_MKT_FX_HOGA);
		//SMEventAddDestination(g_lIdx, (char*)SISE_GW, (CHAR*)MSG_MKT_CME_EXEC);	// (char*)MSG_MKT_FX_HOGA);
		printf("SMEventAddDestination ok !\n");

		int resi;
		resi = SMSetWorkEventCallBack(g_lIdx, (char *)CallBackProc, NULL);
	}
	getchar();


	DeInitDll(g_lIdx);
}

BOOL InitDll()
{
	g_hIns = GetModuleHandleA("SmartMessage.dll");
	if (g_hIns)
	{
		return TRUE;
	}

	g_hIns = LoadLibraryA("SmartMessage.dll");
	if (g_hIns <= 0)
	{
		printf("DLL not found !\n");
		return -1;
	}

	Initialize = NULL;
	Initialize = (TInitialize)GetProcAddress(g_hIns, "Initialize");
	if (Initialize == NULL)
	{
		printf("Initialize function not found in the DLL !\n");
		return -1;
	}

	SMCreateInstance = (TSMCreateInstance)GetProcAddress(g_hIns, "SMCreateInstance");
	if (SMCreateInstance == NULL)
	{
		//ShowMessage("Initialize function not found in the DLL !");
		printf("SMCreateInstance error\n");
		return -1;

	}



	SMClientConnect = NULL;
	SMClientConnect = (TSMClientConnect)GetProcAddress(g_hIns, "SMClientConnect");
	if (SMClientConnect == NULL)
	{
		printf("SMClientConnect function not found in the DLL !");
		return -1;
	}

	SMClientDisconnect = NULL;
	SMClientDisconnect = (TSMClientDisconnect)GetProcAddress(g_hIns, "SMClientDisconnect");
	if (SMClientDisconnect == NULL)
	{
		printf("SMClientDisconnect function not found in the DLL !");
		return -1;
	}

	SMClientIsConnected = NULL;
	SMClientIsConnected = (TSMClientIsConnected)GetProcAddress(g_hIns, "SMClientIsConnected");
	if (SMClientIsConnected == NULL)
	{
		printf("SMClientIsConnected function not found in the DLL !");
		return -1;
	}

	SMSetMessageParameters = NULL;
	SMSetMessageParameters = (TSMSetMessageParameters)GetProcAddress(g_hIns, "SMSetMessageParameters");
	if (SMSetMessageParameters == NULL)
	{
		printf("SMSetMessageParameters function not found in the DLL !");
		return -1;
	}

	SMSetMessageBinaryField = NULL;
	SMSetMessageBinaryField = (TSMSetMessageBinaryField)GetProcAddress(g_hIns, "SMSetMessageBinaryField");
	if (SMSetMessageBinaryField == NULL)
	{
		printf("SMSetMessageBinaryField function not found in the DLL !");
		return FALSE;
	}

	SMSetMessageStringField = NULL;
	SMSetMessageStringField = (TSMSetMessageStringField)GetProcAddress(g_hIns, "SMSetMessageStringField");
	if (SMSetMessageStringField == NULL)
	{
		printf("SMSetMessageStringField function not found in the DLL !");
		return FALSE;
	}

	SMSetMessageIntegerField = NULL;
	SMSetMessageIntegerField = (TSMSetMessageIntegerField)GetProcAddress(g_hIns, "SMSetMessageIntegerField");
	if (SMSetMessageIntegerField == NULL)
	{
		printf("SMSetMessageIntegerField function not found in the DLL !");
		return FALSE;
	}

	SMSendMessage = NULL;
	SMSendMessage = (TSMSendMessage)GetProcAddress(g_hIns, "SMSendMessage");
	if (SMSendMessage == NULL)
	{
		printf("SMSendMessage function not found in the DLL !");
		return FALSE;
	}

	SMGetClientUniqKey = NULL;
	SMGetClientUniqKey = (TSMGetClientUniqKey)GetProcAddress(g_hIns, "SMGetClientUniqKey");
	if (SMGetClientUniqKey == NULL)
	{
		printf("SMGetClientUniqKey function not found in the DLL !");
		return FALSE;
	}

	SMEventAddDestination = NULL;
	SMEventAddDestination = (TSMEventAddDestination)GetProcAddress(g_hIns, "SMEventAddDestination");
	if (SMEventAddDestination == NULL)
	{
		printf("SMEventAddDestination function not found in the DLL !");
		return FALSE;
	}

	SMGetReceivedCnt = NULL;
	SMGetReceivedCnt = (TSMGetReceivedCnt)GetProcAddress(g_hIns, "SMGetReceivedCnt");
	if (SMGetReceivedCnt == NULL)
	{
		printf("SMGetReceivedCnt function not found in the DLL !");
		return FALSE;
	}

	SMSetWorkEventCallBack = NULL;
	SMSetWorkEventCallBack = (TSMSetWorkEventCallBack)GetProcAddress(g_hIns, "SMSetWorkEventCallBack");
	if (SMSetWorkEventCallBack == NULL)
	{
		printf("SMSetWorkEventCallBack function not found in the DLL !");
		return FALSE;
	}

	SMMessageGetBinaryFieldValue = NULL;
	SMMessageGetBinaryFieldValue = (TSMMessageGetBinaryFieldValue)GetProcAddress(g_hIns, "SMMessageGetBinaryFieldValue");
	if (SMMessageGetBinaryFieldValue == NULL)
	{
		printf("SMMessageGetBinaryFieldValue function not found in the DLL !");
		return FALSE;
	}

	SMMessageGetStringFieldValue = NULL;
	SMMessageGetStringFieldValue = (TSMMessageGetStringFieldValue)GetProcAddress(g_hIns, "SMMessageGetStringFieldValue");
	if (SMMessageGetStringFieldValue == NULL)
	{
		printf("SMMessageGetStringFieldValue function not found in the DLL !");
		return FALSE;
	}

	SMMessageGetIntegerFieldValue = NULL;
	SMMessageGetIntegerFieldValue = (TSMMessageGetIntegerFieldValue)GetProcAddress(g_hIns, "SMMessageGetIntegerFieldValue");
	if (SMMessageGetIntegerFieldValue == NULL)
	{
		printf("SMMessageGetIntegerFieldValue function not found in the DLL !");
		return FALSE;
	}


	SMGetObjectsNumber = NULL;
	SMGetObjectsNumber = (TSMGetObjectsNumber)GetProcAddress(g_hIns, "SMGetObjectsNumber");
	if (SMGetObjectsNumber == NULL)
	{
		printf("SMGetObjectsNumber function not found in the DLL !");
		return FALSE;
	}

	SMGetMaximumObjectsNumber = NULL;
	SMGetMaximumObjectsNumber = (TSMGetMaximumObjectsNumber)GetProcAddress(g_hIns, "SMGetMaximumObjectsNumber");
	if (SMGetMaximumObjectsNumber == NULL)
	{
		printf("SMGetMaximumObjectsNumber function not found in the DLL !");
		return FALSE;
	}

	SMCreateInstance = NULL;
	SMCreateInstance = (TSMCreateInstance)GetProcAddress(g_hIns, "SMCreateInstance");
	if (SMCreateInstance == NULL)
	{
		printf("SMCreateInstance function not found in the DLL !");
		return FALSE;
	}

	SMGetClientIP = NULL;
	SMGetClientIP = (TSMGetClientIP)GetProcAddress(g_hIns, "SMGetClientIP");
	if (SMGetClientIP == NULL)
	{
		printf("SMGetClientIP function not found in the DLL !");
		return FALSE;
	}

	SMEventRemoveDestination = NULL;
	SMEventRemoveDestination = (TSMEventRemoveDestination)GetProcAddress(g_hIns, "SMEventRemoveDestination");
	if (SMEventRemoveDestination == NULL)
	{
		printf("SMEventRemoveDestination function not found in the DLL !");
		return FALSE;
	}

	SMEventAllRemoveDestination = NULL;
	SMEventAllRemoveDestination = (TSMEventAllRemoveDestination)GetProcAddress(g_hIns, "SMEventAllRemoveDestination");
	if (SMEventAllRemoveDestination == NULL)
	{
		printf("SMEventAllRemoveDestination function not found in the DLL !");
		return FALSE;
	}

	SMMessageGetDeliveryMode = NULL;
	SMMessageGetDeliveryMode = (TSMMessageGetDeliveryMode)GetProcAddress(g_hIns, "SMMessageGetDeliveryMode");
	if (SMMessageGetDeliveryMode == NULL)
	{
		printf("SMMessageGetDeliveryMode function not found in the DLL !");
		return FALSE;
	}


	SMSendResponse = NULL;
	SMSendResponse = (TSMSendResponse)GetProcAddress(g_hIns, "SMSendResponse");
	if (SMSendResponse == NULL)
	{
		printf("SMSendResponse function not found in the DLL !");
		return FALSE;
	}


	SMMessageGetDestination = NULL;
	SMMessageGetDestination = (TSMMessageGetDestination)GetProcAddress(g_hIns, "SMMessageGetDestination");
	if (SMMessageGetDestination == NULL)
	{
		printf("SMMessageGetDestination function not found in the DLL !");
		return FALSE;
	}


	SMMessageGetMsg = NULL;
	SMMessageGetMsg = (TSMMessageGetMsg)GetProcAddress(g_hIns, "SMMessageGetMsg");
	if (SMMessageGetDestination == NULL)
	{
		printf("SMMessageGetMsg function not found in the DLL !");
		return FALSE;
	}


	SMMessageGetClientSession = NULL;
	SMMessageGetClientSession = (TSMMessageGetClientSession)GetProcAddress(g_hIns, "SMMessageGetClientSession");
	if (SMMessageGetDestination == NULL)
	{
		printf("SMMessageGetClientSession function not found in the DLL !");
		return FALSE;
	}



	SMGetMsgOfRecvMsg = NULL;
	SMGetMsgOfRecvMsg = (TSMGetMsgOfRecvMsg)GetProcAddress(g_hIns, "SMGetMsgOfRecvMsg");
	if (SMGetMsgOfRecvMsg == NULL)
	{
		printf("SMGetMsgOfRecvMsg function not found in the DLL !");
		return FALSE;
	}


	SMSMessageToSMessageEx = NULL;
	SMSMessageToSMessageEx = (TSMSMessageToSMessageEx)GetProcAddress(g_hIns, "SMSMessageToSMessageEx");
	if (SMSMessageToSMessageEx == NULL)
	{
		printf("SMSMessageToSMessageEx function not found in the DLL !");
		return FALSE;
	}

	SetSMMessageHeader = NULL;
	SetSMMessageHeader = (TSetSMMessageHeader)GetProcAddress(g_hIns, "SetSMMessageHeader");
	if (SetSMMessageHeader == NULL)
	{
		printf("SetSMMessageHeader function not found in the DLL !");
		return FALSE;
	}

	long int resi;
	resi = Initialize();
	return (resi==0);

}
VOID DeInitDll(int idx)
{
	SMClientDisconnect(idx);
	FreeLibrary(g_hIns);
}