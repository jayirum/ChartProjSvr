#pragma once
#pragma pack(push, 1)

enum	TCMDType { cmtIOCPEvent, cmtAccept, cmtDisconnect, cmtTR_CODE };
enum	TR_CODE { ctDestAdd, ctDestDel, ctDestPushSend, ctDestRRSend, ctDestRRSendResponse, ctUserID, ctDisconnect, ctUserConnectOk, ctUsersAlready, ctClientUniqKey };
enum	TDeliveryMode { Delivery_RR, Delivery_RR_Response, Delivery_Push };
enum	TMsgFieldType { mfInteger, mfDouble, mfString, mfBinary, mfFieldName };
enum	ThreadMode { thSingal, tbAutoMulti, tbCustom };

enum TColor { clRed, clBlack };

#define TAG_S_MSG  "S^02"
#define TAG_S_HANDLE  "S^03"
#define TAG_S_CLIENT_UNIQ  "S^04"
#define TAG_S_LOGIN  "S^05"
#define TAG_S_LOGOUT  "S^06"
#define TAG_S_LOGKILL  "S^07"


#define MAXLEN_KEY	32
#define MAXLEN_DEST	20
#define MAXLEN_DEST_MSG	20
#define MAXLEN_CLIENT_UID	64
#define MAXLEN_FIELD_STR	32
#define MAXLEN_DATA	128
#define MAXLEN_TCPBUFFER	1024

#define	SM_ADD_MSG		WM_USER + 1910;
#define	SM_REMOVE_MSG	WM_USER + 1911;
#define	SM_SEND_MSG		WM_USER + 1912;
#define	SM_RCV_MSG		WM_USER + 1913;
#define	SM_FILTER_MSG	WM_USER + 1914;

#define	SM_DATA_INTEGER 0x08
#define SM_DATA_DOUBLE 0x10
#define SM_DATA_STRING 0x20
#define SM_DATA_BINARY 0x40
#define SM_DATA_FIELD 0x80

#define SM_DATA_BYTE_1 0x01
#define SM_DATA_BYTE_2 0x02
#define SM_DATA_BYTE_4 0x04

typedef struct _TSMH_Head
{
	char	TR;
	int		DataLen;
}TSMH_Head, *PSMH_Head;

//////////////////////////////////////////////////////////////////////////////
// UserID Login
//////////////////////////////////////////////////////////////////////////////
typedef struct _TUserID_Login
{
	TSMH_Head Head;
	char*	DATA;	//: PChar; //User ID
}TUserID_Login, PTUserID_Login;

//////////////////////////////////////////////////////////////////////////////
// Msg Add, Msg Del
//////////////////////////////////////////////////////////////////////////////
typedef struct _TMsg_Data
{
	TSMH_Head Head;
	char* DATA;		//: PChar; // Msg String
}TMsg_Data, *PTMsg_Data;

#pragma pack(pop)