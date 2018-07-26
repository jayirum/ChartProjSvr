#pragma once
#include <Windows.h>

enum TR_CODE { ctDestAdd, ctDestDel, ctDestPushSend, ctDestRRSend, ctDestRRSendResponse, ctUserID, ctDisconnect, ctUserConnectOk, ctUsersAlready, ctClientUniqKey };
enum TDeliveryMode { Delivery_RR, Delivery_RR_Response, Delivery_Push };

#define SISE_GW  "\\SISE\\GW\\"

#define MSG_MKT_FX_EXEC "\\MKT\\FX\\EXEC\\"
#define MSG_MKT_FX_HOGA "\\MKT\\FX\\HOGA\\"
#define MSG_MKT_FUT_HOGA "\\MKT\\FUT\\HOGA\\"
#define MSG_MKT_FUT_EXEC "\\MKT\\FUT\\EXEC\\"

#define fldFutHoga  "S002"
#define fldFutExec  "S003"
#define fldFXExec "S009"
#define fldFXHoga "S008"


#define TAG_S_MSG "S^02"
#define TAG_S_HANDLE "S^03"
#define TAG_S_CLIENT_UNIQ "S^04"
#define TAG_S_LOGIN "S^05"
#define TAG_S_LOGOUT "S^06"
#define TAG_S_LOGKILL "S^07"

#define SM_DATA_FIELD 0x80
#define SM_DATA_BYTE_1 0x01
#define SM_DATA_BYTE_2 0x02
#define SM_DATA_BYTE_4 0x04

#define SM_DATA_INTEGER 0x08
#define SM_DATA_DOUBLE 0x10
#define SM_DATA_STRING 0x20
#define SM_DATA_BINARY 0x40
#define SM_DATA_FIELD 0x80


/*

*/

typedef struct _SHM_HEAD
{
	BYTE TR;
	int DataLen;
}SHM_HEAD;


	int GetByteSize(int nLen);
	int CalcDataSize(char* pKey, char cFieldType, char* pData, int nDataSize);
	int SetFieldData_Msg(int iPos, char* Buffer, char* pKey, char* pData);
	int SetFieldData_Data(int iPos, char* Buffer, char* pKey, char* pData, int nDataSize);
