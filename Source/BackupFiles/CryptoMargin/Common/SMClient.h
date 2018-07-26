#pragma once
#pragma pack(1)
#include <Windows.h>

typedef enum { ctDestAdd, ctDestDel, ctDestPushSend, ctDestRRSend, ctDestRRSendResponse, ctUserID, ctDisconnect, ctUserConnectOk, ctUsersAlready, ctClientUniqKey }
TR_CODE;

typedef enum { Delivery_RR, Delivery_RR_Response, Delivery_Push } TDeliveryMode;
typedef enum { TYPE_INT, TYPE_DOUBLE, TYPE_STRING, TYPE_BINARY } FIELD_TYPE;

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


class CSMClient
{
public:
	CSMClient();
	virtual ~CSMClient();

	VOID	SetDestination(char* psDest);
	VOID	SetDelieveryMode(TDeliveryMode mode);
	VOID	SetMsg(char* psMsgKey, char* psMsg);
	VOID	SetData(char* psDataKey, char* pData, FIELD_TYPE DataType, int nDataSize);

	char*	GetSendData(_Out_ int *pnSendSize);
	char*	GetErrMsg() { return m_zErrMsg; }
private:
	char*	GetSendDataFn(_Out_ int *pnSendSize);
	int GetByteSize(int nLen);
	int CalcDataSize(char* pKey, FIELD_TYPE type, char* pData, int nDataSize);
	int SetFieldData_Msg(int iPos, char* Buffer, char* pKey, char* pData);
	int SetFieldData_Data(int iPos, char* Buffer, char* pKey, char* pData, int nDataSize);

private:
	char*	m_pSendBuff;
	char*	m_pData;
	char	m_zDestination[128];
	char	m_zMsgKey[128], m_zMsgData[128];
	char	m_zDataKey[128];
	FIELD_TYPE	m_DataType;
	int			m_nDataSize;
	int			m_nSendSize;
	TR_CODE		m_Tr;

	char	m_zErrMsg[1024];
	char Buffer[1024];
};