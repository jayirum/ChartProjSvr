
#include "SmartMessage.h"
#include "Util.h"

TSMInitialize					SMInitialize;
TSMDeInitialize					SMDeInitialize;
TSMClientConnect				SMClientConnect;
TSMClientDisconnect				SMClientDisconnect;
TSMClientIsConnected			SMClientIsConnected;
TSMSetMessageParameters			SMSetMessageParameters;
TSMSetMessageBinaryField		SMSetMessageBinaryField;
TSMSetMessageStringField		SMSetMessageStringField;
TSMSetMessageIntegerField		SMSetMessageIntegerField;
TSMSendMessage					SMSendMessage;
TSMGetClientUniqKey				SMGetClientUniqKey;
TSMGetClientIP					SMGetClientIP;
TSMEventAddDestination			SMEventAddDestination;
TSMEventRemoveDestination		SMEventRemoveDestination;
TSMEventAllRemoveDestination	SMEventAllRemoveDestination;
TSMGetReceivedCnt				SMGetReceivedCnt;
TSMSetWorkEventCallBack			SMSetWorkEventCallBack;
TSMMessageGetBinaryFieldValue	SMMessageGetBinaryFieldValue;
TSMMessageGetBinaryFieldValueEx	SMMessageGetBinaryFieldValueEx;
TSMMessageGetStringFieldValue	SMMessageGetStringFieldValue;
TSMMessageGetIntegerFieldValue	SMMessageGetIntegerFieldValue;
TSMMessageGetDoubleFieldValue	SMMessageGetDoubleFieldValue;
TSMGetObjectsNumber				SMGetObjectsNumber;
TSMGetMaximumObjectsNumber		SMGetMaximumObjectsNumber;
TSMCreateInstance				SMCreateInstance;
TSMMessageGetDeliveryMode		SMMessageGetDeliveryMode;
TSMSendResponse					SMSendResponse;
TSMMessageGetDestination		SMMessageGetDestination;
TSMMessageGetMsg				SMMessageGetMsg;
TSMMessageGetClientSession		SMMessageGetClientSession;
TSMSMessageToSMessageEx			SMSMessageToSMessageEx;
TSMGetMsgOfRecvMsg				SMGetMsgOfRecvMsg;
TSetSMMessageHeader				SetSMMessageHeader;


CSmartMessage::CSmartMessage()
{
	m_hIns	= NULL;
}


CSmartMessage::~CSmartMessage()
{
	End();
}



VOID CSmartMessage::End()
{
//	if (m_lIdx == 0) {
//		SMDeInitialize();
//	}
	Sleep(100);
	if(m_hIns)
		FreeLibrary(m_hIns);
	m_hIns = NULL;
}


BOOL CSmartMessage::CreateInstance()
{
	m_lIdx = SMCreateInstance();
	m_lIdx  -= 1;
	return (m_lIdx > -1);
}

long CSmartMessage::GetCurrInstanceCnt()
{
	return SMGetObjectsNumber();
}

BOOL CSmartMessage::ConnectSMSrv(char* pzIp, long lPort)
{
	if (SMClientConnect(m_lIdx, pzIp, lPort) != 0)
	{
		sprintf(m_zMsg, "Connect to SM error(IP:%s)(Port:%d)", pzIp, lPort);
		return FALSE;
	}
	return TRUE;
}

VOID CSmartMessage::DisConnectSMSrv()
{
	SMClientDisconnect(m_lIdx);
}

//
//#pragma pack(push, 1)
//struct TFutExec2
//{
//	//TFutExec = packed record
//	char issue[8 + 1];
//	//issue : array[0..8] of char;
//	double gap;
//	//gap : real;
//	double cup;
//	//cup : real;
//	double sip;
//	//sip : real;
//	double hip;
//	//hip : real;
//	double lip;
//	//lip : real;
//	int vol;
//	//vol : integer;
//	double amt;
//	//amt : real;
//	int time;
//	//time: integer;
//	char side[1 + 1];
//	//side: array[0..1] of char;
//	char ydiffSign[1 + 1];
//	//ydiffSign : array[0..1] of char;
//	char chgrate[6 + 1];
//	//chgrate   : array[0..6]  of char
//	//end;
//};
//
//#pragma pack(pop)
//long int __stdcall MyCallBack(int index, char* WorkThread, char* Message)
//{
//	index = 0;
//	char* fv = SMMessageGetBinaryFieldValue(index, RECEIVED_MESSAGE, (char *)fldFXExec);
//	TFutExec2 FutExec;
//	memcpy(&FutExec, fv, sizeof(FutExec));
//	//ShowMessage(FutExec.issue);
//
//	char deststr[255];
//	char msgstr[255];
//	char* destination = SMMessageGetDestination(index, RECEIVED_MESSAGE);
//	strcpy(deststr, destination);
//
//	char* msg = SMMessageGetMsg(index, RECEIVED_MESSAGE);
//	strcpy(&msgstr[0], msg);
//
//	printf("[dest](%s)__[msg](%s)\n", deststr, msgstr);
//	//printf("[dest](%s)\n", deststr);
//
//	printf("[IDX:%d](ISSUE:%s)(TIME:%d) (%.5f)(%.5f)\n",
//		index, FutExec.issue, FutExec.time, FutExec.cup, FutExec.hip);
//
//	//TFutExec2 st;
//	//GetBinaryFieldValue((char *)fldFXExec, sizeof(TFutExec2), (char*)&st);
//
//
//	//char msg[128];
//	//GetMsgOfRecvMsg(msg);
//	//printf("MSG===>%s\n", msg);
//	//if (strcmp(msg, (char*)MSG_MKT_CME_HOGA) == 0)
//	//{
//	//	printf("[HOGA][%s](%.5f)(%.5f)(%.5f)(%.5f)\n",
//	//		st.issue, st.cup, st.sip, st.hip, st.lip);
//	//}
//	//if (strcmp(msg, (char*)MSG_MKT_FX_EXEC) == 0)
//	//{
//	//	printf("[EXEC][%s](%.5f)(%.5f)(%.5f)(%.5f)\n",
//	//		st.issue, st.cup, st.sip, st.hip, st.lip);
//	//}
//	
//	return 0;
//}
//
//long __stdcall CSmartMessage::callbackTest(int index, char* WorkThread, char* pMsg)
//{
//	CSmartMessage* pThis = (CSmartMessage*)WorkThread;
//
//	printf("%s\n", pThis->m_msg);
//
//	index = 0;
//	char* fv = SMMessageGetBinaryFieldValue(index, RECEIVED_MESSAGE, (char *)fldFXExec);
//	TFutExec2 FutExec;
//	memcpy(&FutExec, fv, sizeof(FutExec));
//	//ShowMessage(FutExec.issue);
//
//	char deststr[255];
//	char msgstr[255];
//	char* destination = SMMessageGetDestination(index, RECEIVED_MESSAGE);
//	strcpy(deststr, destination);
//
//	char* msg = SMMessageGetMsg(index, RECEIVED_MESSAGE);
//	strcpy(&msgstr[0], msg);
//
//	printf("[dest2](%s)__[msg](%s)\n", deststr, msgstr);
//	//printf("[dest](%s)\n", deststr);
//
//	printf("[IDX2:%d](ISSUE:%s)(TIME:%d) (%.5f)(%.5f)\n",
//		index, FutExec.issue, FutExec.time, FutExec.cup, FutExec.hip);
//	return 1;
//}

//void CSmartMessage::SetRecvCallBackFn()// (RECV_CALLBACK RecvCallBack)
void CSmartMessage::SetRecvCallBackFn(LPVOID lpCustomPtr, RECV_CALLBACK fnCallBack)
{
	SMSetWorkEventCallBack(m_lIdx, (char*)(RECV_CALLBACK)fnCallBack, (char*)lpCustomPtr);
}

BOOL CSmartMessage::AddDest( char* pDest, char* pMsg)
{
	__try
	{
		SMEventAddDestination(m_lIdx, pDest, pMsg);
	}
	__except(ReportException(GetExceptionCode(), "AddDest", m_zMsg))
	{
		return FALSE;
	}
	return TRUE;
}


BOOL CSmartMessage::GetSMDestination(_Out_ char* pzDest)
{
	char* pDest = SMMessageGetDestination(m_lIdx, RECEIVED_MESSAGE);
	if (!pDest)
		return FALSE;

	strcpy(pzDest, pDest);
	return TRUE;
}

BOOL CSmartMessage::GetSMMessage(_Out_ char* pzMessage)
{
	char* pMsg = SMGetMsgOfRecvMsg(m_lIdx);
	//char* pMsg = SMMessageGetMsg(m_lIdx, RECEIVED_MESSAGE);
	if (!pMsg)
		return FALSE;
	strcpy(pzMessage, pMsg);
	return TRUE;
}

#pragma pack(push, 1)
struct TFutExec3
{
	char issue[9 + 1];
	double gap;
	double cup;
	double sip;
	double hip;
	double lip;
	int vol;
	double amt;
	int time;
	char side[1 + 1];
	char ydiffSign[1 + 1];
	char chgrate[6 + 1];
};

#pragma pack(pop)
BOOL CSmartMessage::GetBinaryFieldValue(char* pFieldName, int nValSize, void* pValue)
{
	TFutExec3 pack;
	ZeroMemory(&pack, sizeof(TFutExec3));

	char* pResult = SMMessageGetBinaryFieldValue(m_lIdx, RECEIVED_MESSAGE, pFieldName);
	if (!pResult)
	{
		sprintf(m_zMsg, "SMMessageGetBinaryFieldValue Error(%s)", pFieldName);
		return FALSE;
	}

	memcpy(pValue, pResult, nValSize);
	return TRUE;
}

int CSmartMessage::GetBinaryFieldValueEx(char* pFieldName, int nValSize, void* pValue)
{
	//TFutExec3 pack;
	//ZeroMemory(&pack, sizeof(TFutExec3));

	int ret = SMMessageGetBinaryFieldValueEx(m_lIdx, pFieldName, (void*)pValue);
	//printf("(%s)(cup:%f)(sip:%f)(hip:%f)(lip:%f)\n",
	//	pack.issue, pack.cup, pack.sip, pack.hip, pack.lip);

	//memcpy(pValue, &pack, nValSize);
	return (ret);
}

BOOL CSmartMessage::GetStringFieldValue(char* pFieldName, int nValSize, char* pValue)
{
	char* pResult = SMMessageGetStringFieldValue(m_lIdx, RECEIVED_MESSAGE, pFieldName);
	if (!pResult)
	{
		sprintf(m_zMsg, "SMMessageGetStringFieldValue Error(%s)", pFieldName);
		return FALSE;
	}

	if (nValSize == 0)
		strcpy(pValue, pResult);
	else
		memcpy(pValue, pResult, nValSize);
	return TRUE;
}


BOOL CSmartMessage::GetIntegerFieldValue(char* pFieldName, int nValSize, int* pValue)
{
	int nResult = SMMessageGetIntegerFieldValue(m_lIdx, RECEIVED_MESSAGE, pFieldName);
	if (nResult== -99999)
	{
		sprintf(m_zMsg, "SMMessageGetIntegerFieldValue Error(%s)", pFieldName);
		return FALSE;
	}

	*pValue = nResult;
	return TRUE;
}


BOOL CSmartMessage::GetDoubleFieldValue(char* pFieldName, int nValSize, double* pValue)
{
	double dResult = SMMessageGetDoubleFieldValue(m_lIdx, RECEIVED_MESSAGE, pFieldName);
	if (dResult == -99999)
	{
		sprintf(m_zMsg, "SMMessageGetDoubleFieldValue Error(%s)", pFieldName);
		return FALSE;
	}

	*pValue = dResult;
	return TRUE;
}


BOOL CSmartMessage::SMessageToSMessage()
{
	return (SMSMessageToSMessageEx(m_lIdx) == 0);
}

BOOL CSmartMessage::SetHeaderOfSMessage(int nDeliveryMode, char* pDest, char* pMsg, int ClientSession)
{
	return (SetSMMessageHeader(m_lIdx, nDeliveryMode, pDest, pMsg, ClientSession) == 0);
}


/*
function SMSetMessageBinaryField(index, messtype : integer; FieldName : PChar; Data : Pointer; DataSize : integer) : integer; stdcall;

*/
BOOL CSmartMessage::SetBinaryFieldValue(char* pFieldName, char* pData, int nDataSize)
{
	return(SMSetMessageBinaryField(m_lIdx, MESSAGE_TO_SEND, pFieldName, pData, nDataSize) == 0);
}

/*
function SMSetMessageStringField(index, messtype : integer; FieldName : PChar; Str : Pointer) : integer; stdcall;
*/
BOOL CSmartMessage::SetStringFieldValue(char* pFieldName, char* pzData)
{
	return(SMSetMessageStringField(m_lIdx, MESSAGE_TO_SEND, pFieldName, pzData) == 0);
}

/*
function SMSetMessageIntegerField(index, messtype : integer; FieldName : PChar; Val : integer) : integer; stdcall;
*/
BOOL CSmartMessage::SetIntegerFieldValue(char* pFieldName, int nData)
{
	return (SMSetMessageIntegerField(m_lIdx, MESSAGE_TO_SEND, pFieldName, nData) == 0);
}

/*
function SMSendMessage(index, messtype : integer) : integer; stdcall;
*/
BOOL CSmartMessage::SendData()
{
	return (SMSendMessage(m_lIdx, MESSAGE_TO_SEND) == 0);
}


BOOL CSmartMessage::Begin()
{
	m_hIns = GetModuleHandle("SmartMessage.dll");
	if (!m_hIns)
	{
		m_hIns = LoadLibraryA("SmartMessage.dll");
		if (m_hIns <= 0)
		{
			sprintf(m_zMsg, "SmartMessage.dll is not found !");
			return FALSE;
		}

		//ShowMessage(IntToStr((int)sizeof(TFutExec)));

		SMInitialize = NULL;
		SMInitialize = (TSMInitialize)GetProcAddress(m_hIns, "Initialize");
		if (SMInitialize == NULL)
		{
			sprintf(m_zMsg, "Initialize function not found in the DLL !\n");
			return FALSE;
		}

		SMDeInitialize = NULL;
		SMDeInitialize = (TSMDeInitialize)GetProcAddress(m_hIns, "DeInitialize");
		if (SMDeInitialize == NULL)
		{
			sprintf(m_zMsg, "DeInitialize function not found in the DLL !\n");
			return FALSE;
		}

		SMCreateInstance = (TSMCreateInstance)GetProcAddress(m_hIns, "SMCreateInstance");
		if (SMCreateInstance == NULL)
		{
			//ShowMessage("Initialize function not found in the DLL !");
			sprintf(m_zMsg, "SMCreateInstance error\n");
			return FALSE;

		}



		SMClientConnect = NULL;
		SMClientConnect = (TSMClientConnect)GetProcAddress(m_hIns, "SMClientConnect");
		if (SMClientConnect == NULL)
		{
			sprintf(m_zMsg, "SMClientConnect function not found in the DLL !");
			return FALSE;
		}

		SMClientDisconnect = NULL;
		SMClientDisconnect = (TSMClientDisconnect)GetProcAddress(m_hIns, "SMClientDisconnect");
		if (SMClientDisconnect == NULL)
		{
			sprintf(m_zMsg, "SMClientDisconnect function not found in the DLL !");
			return FALSE;
		}

		SMClientIsConnected = NULL;
		SMClientIsConnected = (TSMClientIsConnected)GetProcAddress(m_hIns, "SMClientIsConnected");
		if (SMClientIsConnected == NULL)
		{
			sprintf(m_zMsg, "SMClientIsConnected function not found in the DLL !");
			return FALSE;
		}

		SMSetMessageParameters = NULL;
		SMSetMessageParameters = (TSMSetMessageParameters)GetProcAddress(m_hIns, "SMSetMessageParameters");
		if (SMSetMessageParameters == NULL)
		{
			sprintf(m_zMsg, "SMSetMessageParameters function not found in the DLL !");
			return FALSE;
		}

		SMSetMessageBinaryField = NULL;
		SMSetMessageBinaryField = (TSMSetMessageBinaryField)GetProcAddress(m_hIns, "SMSetMessageBinaryField");
		if (SMSetMessageBinaryField == NULL)
		{
			sprintf(m_zMsg, "SMSetMessageBinaryField function not found in the DLL !");
			return FALSE;
		}

		SMSetMessageStringField = NULL;
		SMSetMessageStringField = (TSMSetMessageStringField)GetProcAddress(m_hIns, "SMSetMessageStringField");
		if (SMSetMessageStringField == NULL)
		{
			sprintf(m_zMsg, "SMSetMessageStringField function not found in the DLL !");
			return FALSE;
		}

		SMSetMessageIntegerField = NULL;
		SMSetMessageIntegerField = (TSMSetMessageIntegerField)GetProcAddress(m_hIns, "SMSetMessageIntegerField");
		if (SMSetMessageIntegerField == NULL)
		{
			sprintf(m_zMsg, "SMSetMessageIntegerField function not found in the DLL !");
			return FALSE;
		}

		SMSendMessage = NULL;
		SMSendMessage = (TSMSendMessage)GetProcAddress(m_hIns, "SMSendMessage");
		if (SMSendMessage == NULL)
		{
			sprintf(m_zMsg, "SMSendMessage function not found in the DLL !");
			return FALSE;
		}

		SMGetClientUniqKey = NULL;
		SMGetClientUniqKey = (TSMGetClientUniqKey)GetProcAddress(m_hIns, "SMGetClientUniqKey");
		if (SMGetClientUniqKey == NULL)
		{
			sprintf(m_zMsg, "SMGetClientUniqKey function not found in the DLL !");
			return FALSE;
		}

		SMEventAddDestination = NULL;
		SMEventAddDestination = (TSMEventAddDestination)GetProcAddress(m_hIns, "SMEventAddDestination");
		if (SMEventAddDestination == NULL)
		{
			sprintf(m_zMsg, "SMEventAddDestination function not found in the DLL !");
			return FALSE;
		}

		SMGetReceivedCnt = NULL;
		SMGetReceivedCnt = (TSMGetReceivedCnt)GetProcAddress(m_hIns, "SMGetReceivedCnt");
		if (SMGetReceivedCnt == NULL)
		{
			sprintf(m_zMsg, "SMGetReceivedCnt function not found in the DLL !");
			return FALSE;
		}

		SMSetWorkEventCallBack = NULL;
		SMSetWorkEventCallBack = (TSMSetWorkEventCallBack)GetProcAddress(m_hIns, "SMSetWorkEventCallBack");
		if (SMSetWorkEventCallBack == NULL)
		{
			printf("SMSetWorkEventCallBack function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetBinaryFieldValue = NULL;
		SMMessageGetBinaryFieldValue = (TSMMessageGetBinaryFieldValue)GetProcAddress(m_hIns, "SMMessageGetBinaryFieldValue");
		if (SMMessageGetBinaryFieldValue == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetBinaryFieldValue function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetBinaryFieldValueEx = NULL;
		SMMessageGetBinaryFieldValueEx = (TSMMessageGetBinaryFieldValueEx)GetProcAddress(m_hIns, "SMMessageGetBinaryFieldValueEx");
		if (SMMessageGetBinaryFieldValue == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetBinaryFieldValueEx function not found in the DLL !");
			return FALSE;
		}
		
		SMMessageGetStringFieldValue = NULL;
		SMMessageGetStringFieldValue = (TSMMessageGetStringFieldValue)GetProcAddress(m_hIns, "SMMessageGetStringFieldValue");
		if (SMMessageGetStringFieldValue == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetStringFieldValue function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetIntegerFieldValue = NULL;
		SMMessageGetIntegerFieldValue = (TSMMessageGetIntegerFieldValue)GetProcAddress(m_hIns, "SMMessageGetIntegerFieldValue");
		if (SMMessageGetIntegerFieldValue == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetIntegerFieldValue function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetDoubleFieldValue = NULL;
		SMMessageGetDoubleFieldValue = (TSMMessageGetDoubleFieldValue)GetProcAddress(m_hIns, "SMMessageGetDoubleFieldValue");
		if (SMMessageGetDoubleFieldValue == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetDoubleFieldValue function not found in the DLL !");
			return FALSE;
		}


		SMGetObjectsNumber = NULL;
		SMGetObjectsNumber = (TSMGetObjectsNumber)GetProcAddress(m_hIns, "SMGetObjectsNumber");
		if (SMGetObjectsNumber == NULL)
		{
			sprintf(m_zMsg, "SMGetObjectsNumber function not found in the DLL !");
			return FALSE;
		}

		SMGetMaximumObjectsNumber = NULL;
		SMGetMaximumObjectsNumber = (TSMGetMaximumObjectsNumber)GetProcAddress(m_hIns, "SMGetMaximumObjectsNumber");
		if (SMGetMaximumObjectsNumber == NULL)
		{
			sprintf(m_zMsg, "SMGetMaximumObjectsNumber function not found in the DLL !");
			return FALSE;
		}

		SMCreateInstance = NULL;
		SMCreateInstance = (TSMCreateInstance)GetProcAddress(m_hIns, "SMCreateInstance");
		if (SMCreateInstance == NULL)
		{
			sprintf(m_zMsg, "SMCreateInstance function not found in the DLL !");
			return FALSE;
		}

		SMGetClientIP = NULL;
		SMGetClientIP = (TSMGetClientIP)GetProcAddress(m_hIns, "SMGetClientIP");
		if (SMGetClientIP == NULL)
		{
			sprintf(m_zMsg, "SMGetClientIP function not found in the DLL !");
			return FALSE;
		}

		SMEventRemoveDestination = NULL;
		SMEventRemoveDestination = (TSMEventRemoveDestination)GetProcAddress(m_hIns, "SMEventRemoveDestination");
		if (SMEventRemoveDestination == NULL)
		{
			sprintf(m_zMsg, "SMEventRemoveDestination function not found in the DLL !");
			return FALSE;
		}

		SMEventAllRemoveDestination = NULL;
		SMEventAllRemoveDestination = (TSMEventAllRemoveDestination)GetProcAddress(m_hIns, "SMEventAllRemoveDestination");
		if (SMEventAllRemoveDestination == NULL)
		{
			sprintf(m_zMsg, "SMEventAllRemoveDestination function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetDeliveryMode = NULL;
		SMMessageGetDeliveryMode = (TSMMessageGetDeliveryMode)GetProcAddress(m_hIns, "SMMessageGetDeliveryMode");
		if (SMMessageGetDeliveryMode == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetDeliveryMode function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetDestination = NULL;
		SMMessageGetDestination = (TSMMessageGetDestination)GetProcAddress(m_hIns, "SMMessageGetDestination");
		if (SMMessageGetDestination == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetDestination function not found in the DLL !");
			return FALSE;
		}

		SMGetMsgOfRecvMsg = NULL;
		SMGetMsgOfRecvMsg = (TSMGetMsgOfRecvMsg)GetProcAddress(m_hIns, "SMGetMsgOfRecvMsg");
		if (SMGetMsgOfRecvMsg == NULL)
		{
			sprintf(m_zMsg, "SMGetMsgOfRecvMsg function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetClientSession = NULL;
		SMMessageGetClientSession = (TSMMessageGetClientSession)GetProcAddress(m_hIns, "SMMessageGetClientSession");
		if (SMMessageGetDestination == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetClientSession function not found in the DLL !");
			return FALSE;
		}

		SMSMessageToSMessageEx = NULL;
		SMSMessageToSMessageEx = (TSMSMessageToSMessageEx)GetProcAddress(m_hIns, "SMSMessageToSMessageEx");
		if (SMSMessageToSMessageEx == NULL)
		{
			sprintf(m_zMsg, "SMSMessageToSMessageEx function not found in the DLL !");
			return FALSE;
		}

		SetSMMessageHeader = NULL;
		SetSMMessageHeader = (TSetSMMessageHeader)GetProcAddress(m_hIns, "SetSMMessageHeader");
		if (SetSMMessageHeader == NULL)
		{
			sprintf(m_zMsg, "SetSMMessageHeader function not found in the DLL !");
			return FALSE;
		}

		SMMessageGetMsg = NULL;
		SMMessageGetMsg = (TSMMessageGetMsg)GetProcAddress(m_hIns, "SMMessageGetMsg");
		if (SMMessageGetMsg == NULL)
		{
			sprintf(m_zMsg, "SMMessageGetMsg function not found in the DLL !");
			return FALSE;
		}
		

		if (SMInitialize() != 0)
			return FALSE;
	}
	return CreateInstance();
}
