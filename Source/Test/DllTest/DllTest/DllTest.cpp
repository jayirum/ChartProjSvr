#include "../../../IRUM_UTIL/SMDataTypes.h"
#include <stdio.h>
#include <Windows.h>

int g_cnt = 0;

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
TSMMessageGetDeliveryType SMMessageGetDeliveryType;
TSMSendResponse SMSendResponse;
TSMGetDeliveryModeOfRecvMsg SMGetDeliveryModeOfRecvMsg;
TSMGetDestinationOfRecvMsg SMGetDestinationOfRecvMsg;
TSMGetMsgOfRecvMsg SMGetMsgOfRecvMsg;
TSMGetClientSessionOfRecvMsg SMGetClientSessionOfRecvMsg;
TSMSMessageToSMessageEx SMSMessageToSMessageEx;
TSetSMMessageHeader SetSMMessageHeader;

HINSTANCE	g_hIns;
long		g_lIdx;




void DeInitDll(long idx)
{
	SMClientDisconnect(idx);
}
int InitDLL(void)
{
	g_hIns = LoadLibraryA("SmartMessage.dll");
	if (g_hIns <= 0)
	{
		printf("DLL not found !\n");
		return -1;
	}

	//ShowMessage(IntToStr((int)sizeof(TFutExec)));

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
		printf( "SMSetMessageBinaryField function not found in the DLL !");
		return FALSE;
	}

	SMSetMessageStringField = NULL;
	SMSetMessageStringField = (TSMSetMessageStringField)GetProcAddress(g_hIns, "SMSetMessageStringField");
	if (SMSetMessageStringField == NULL)
	{
		printf( "SMSetMessageStringField function not found in the DLL !");
		return FALSE;
	}

	SMSetMessageIntegerField = NULL;
	SMSetMessageIntegerField = (TSMSetMessageIntegerField)GetProcAddress(g_hIns, "SMSetMessageIntegerField");
	if (SMSetMessageIntegerField == NULL)
	{
		printf( "SMSetMessageIntegerField function not found in the DLL !");
		return FALSE;
	}

	SMSendMessage = NULL;
	SMSendMessage = (TSMSendMessage)GetProcAddress(g_hIns, "SMSendMessage");
	if (SMSendMessage == NULL)
	{
		printf( "SMSendMessage function not found in the DLL !");
		return FALSE;
	}

	SMGetClientUniqKey = NULL;
	SMGetClientUniqKey = (TSMGetClientUniqKey)GetProcAddress(g_hIns, "SMGetClientUniqKey");
	if (SMGetClientUniqKey == NULL)
	{
		printf( "SMGetClientUniqKey function not found in the DLL !");
		return FALSE;
	}

	SMEventAddDestination = NULL;
	SMEventAddDestination = (TSMEventAddDestination)GetProcAddress(g_hIns, "SMEventAddDestination");
	if (SMEventAddDestination == NULL)
	{
		printf( "SMEventAddDestination function not found in the DLL !");
		return FALSE;
	}

	SMGetReceivedCnt = NULL;
	SMGetReceivedCnt = (TSMGetReceivedCnt)GetProcAddress(g_hIns, "SMGetReceivedCnt");
	if (SMGetReceivedCnt == NULL)
	{
		printf( "SMGetReceivedCnt function not found in the DLL !");
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
		printf( "SMMessageGetBinaryFieldValue function not found in the DLL !");
		return FALSE;
	}

	SMMessageGetStringFieldValue = NULL;
	SMMessageGetStringFieldValue = (TSMMessageGetStringFieldValue)GetProcAddress(g_hIns, "SMMessageGetStringFieldValue");
	if (SMMessageGetStringFieldValue == NULL)
	{
		printf( "SMMessageGetStringFieldValue function not found in the DLL !");
		return FALSE;
	}

	SMMessageGetIntegerFieldValue = NULL;
	SMMessageGetIntegerFieldValue = (TSMMessageGetIntegerFieldValue)GetProcAddress(g_hIns, "SMMessageGetIntegerFieldValue");
	if (SMMessageGetIntegerFieldValue == NULL)
	{
		printf( "SMMessageGetIntegerFieldValue function not found in the DLL !");
		return FALSE;
	}


	SMGetObjectsNumber = NULL;
	SMGetObjectsNumber = (TSMGetObjectsNumber)GetProcAddress(g_hIns, "SMGetObjectsNumber");
	if (SMGetObjectsNumber == NULL)
	{
		printf( "SMGetObjectsNumber function not found in the DLL !");
		return FALSE;
	}

	SMGetMaximumObjectsNumber = NULL;
	SMGetMaximumObjectsNumber = (TSMGetMaximumObjectsNumber)GetProcAddress(g_hIns, "SMGetMaximumObjectsNumber");
	if (SMGetMaximumObjectsNumber == NULL)
	{
		printf( "SMGetMaximumObjectsNumber function not found in the DLL !");
		return FALSE;
	}

	SMCreateInstance = NULL;
	SMCreateInstance = (TSMCreateInstance)GetProcAddress(g_hIns, "SMCreateInstance");
	if (SMCreateInstance == NULL)
	{
		printf( "SMCreateInstance function not found in the DLL !");
		return FALSE;
	}

	SMGetClientIP = NULL;
	SMGetClientIP = (TSMGetClientIP)GetProcAddress(g_hIns, "SMGetClientIP");
	if (SMGetClientIP == NULL)
	{
		printf( "SMGetClientIP function not found in the DLL !");
		return FALSE;
	}

	SMEventRemoveDestination = NULL;
	SMEventRemoveDestination = (TSMEventRemoveDestination)GetProcAddress(g_hIns, "SMEventRemoveDestination");
	if (SMEventRemoveDestination == NULL)
	{
		printf( "SMEventRemoveDestination function not found in the DLL !");
		return FALSE;
	}

	SMEventAllRemoveDestination = NULL;
	SMEventAllRemoveDestination = (TSMEventAllRemoveDestination)GetProcAddress(g_hIns, "SMEventAllRemoveDestination");
	if (SMEventAllRemoveDestination == NULL)
	{
		printf( "SMEventAllRemoveDestination function not found in the DLL !");
		return FALSE;
	}

	SMGetDeliveryModeOfRecvMsg = NULL;
	SMGetDeliveryModeOfRecvMsg = (TSMGetDeliveryModeOfRecvMsg)GetProcAddress(g_hIns, "SMGetDeliveryModeOfRecvMsg");
	if (SMGetDeliveryModeOfRecvMsg == NULL)
	{
		printf( "SMGetDeliveryModeOfRecvMsg function not found in the DLL !");
		return FALSE;
	}

	SMGetDestinationOfRecvMsg = NULL;
	SMGetDestinationOfRecvMsg = (TSMGetDestinationOfRecvMsg)GetProcAddress(g_hIns, "SMGetDestinationOfRecvMsg");
	if (SMGetDestinationOfRecvMsg == NULL)
	{
		printf( "SMGetDestinationOfRecvMsg function not found in the DLL !");
		return FALSE;
	}

	SMGetMsgOfRecvMsg = NULL;
	SMGetMsgOfRecvMsg = (TSMGetMsgOfRecvMsg)GetProcAddress(g_hIns, "SMGetMsgOfRecvMsg");
	if (SMGetMsgOfRecvMsg == NULL)
	{
		printf( "SMGetMsgOfRecvMsg function not found in the DLL !");
		return FALSE;
	}

	SMGetClientSessionOfRecvMsg = NULL;
	SMGetClientSessionOfRecvMsg = (TSMGetClientSessionOfRecvMsg)GetProcAddress(g_hIns, "SMGetClientSessionOfRecvMsg");
	if (SMGetClientSessionOfRecvMsg == NULL)
	{
		printf( "SMGetClientSessionOfRecvMsg function not found in the DLL !");
		return FALSE;
	}

	SMSMessageToSMessageEx = NULL;
	SMSMessageToSMessageEx = (TSMSMessageToSMessageEx)GetProcAddress(g_hIns, "SMSMessageToSMessageEx");
	if (SMSMessageToSMessageEx == NULL)
	{
		printf( "SMSMessageToSMessageEx function not found in the DLL !");
		return FALSE;
	}

	SetSMMessageHeader = NULL;
	SetSMMessageHeader = (TSetSMMessageHeader)GetProcAddress(g_hIns, "SetSMMessageHeader");
	if (SetSMMessageHeader == NULL)
	{
		printf( "SetSMMessageHeader function not found in the DLL !");
		return FALSE;
	}

	long int resi;
	resi = Initialize();
	//g_bInit = (resi == 0);
	return resi;

}



/*
procedure TRecvThread.FXExecReceived(Sender: TObject; RcvMsg: TSMessage);
var
FutExecBin: TBinaryData;
TTFutExec: TFutExec;
log : string;
begin
FutExecBin := RcvMsg.GetBinaryFieldValue(fldFXExec);

FillChar(TTFutExec, sizeof(TFutExec), 0);
move(FutExecBin.Buffer^, TTFutExec, FutExecBin.Len);

log := Format('Recv Prc (time:%d)', [TTFutExec.time]);
form1.memo.Lines.Insert(0, log);


end;
*/
long int __stdcall CallBackProc(int index, char* WorkThread, char* Message)
{
	char* fv;
	g_cnt++;
	//printf("[%d]Our CallBack func !\n", g_cnt);

	fv = SMMessageGetBinaryFieldValue(index, RECEIVED_MESSAGE, (char *)fldFXExec);
	TFutExec FutExec;
	memcpy(&FutExec, fv, sizeof(FutExec));
	//ShowMessage(FutExec.issue);

	//ShowMessage(FutExec.issue);
	printf("[%d][IDX:%d](ISSUE:%s)(TIME:%d) (%.5f)(%.5f)\n", 
		g_cnt, index, FutExec.issue, FutExec.time, FutExec.cup, FutExec.hip);

	return 0;
}




void recvData()
{
	//
	//
	//
	g_lIdx = SMCreateInstance() - 1;
	printf("SMCreateInstance ok\n");

	// 
	//
	//
	if (SMClientConnect(g_lIdx, "110.4.89.204", (long)7789) < 0) {
		printf("failed to connect\n");
		return;
	}
	printf("connect ok !\n");

	//
	//
	//
	//SMEventR.AddDest(SISE_GW, MSG_MKT_FX_HOGA);
	//   MSG_MKT_FX_EXEC   = '\MKT\FX\EXEC\';
	//SMEventAddDestination(g_lIdx, (char*)SISE_GW, (CHAR*)MSG_MKT_FX_EXEC);	// (char*)MSG_MKT_FX_HOGA);
	SMEventAddDestination(g_lIdx, (char*)SISE_GW, (CHAR*)MSG_MKT_CME_EXEC);	// (char*)MSG_MKT_FX_HOGA);
	printf("SMEventAddDestination ok !\n");

	//
	//
	//
	int resi;
	resi = SMSetWorkEventCallBack(g_lIdx, (char *)CallBackProc);



	//printf("SMClientDisconnect function ok !\n");
}





void main(int argc, char** argv)
{

	if (InitDLL() >= 0)
	{

		recvData();
	}
	getchar();


	DeInitDll(g_lIdx);
}