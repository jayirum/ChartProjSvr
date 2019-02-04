#pragma once
#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include <WinSock2.h>
#include <Windows.h>

namespace _ALGOINC
{
	const int LEN_SYMBOL		= 10;
	const int LEN_PRC			= 10;
	const int LEN_VOLUME		= 10;
	const int LEN_BUFFER		= 1024;
	const int LEN_USERID		= 20;
	const int LEN_USER_NM		= 20;
	const int LEN_PACKET_CODE	= 4;
	const int LEN_PACKET_SIZE	= 4;
	const int LEN_FRONT_NAME	= 10;
	const int LEN_RSLT_CODE		= 4;

	const char	STX = 0x02;
	const char	ETX = 0x03;

	// RequestRecv : request receive from Whom
	// RequestSend : be sent by whom
	const char SYSTP_CLIENT	= '0';
	const char SYSTP_LB		= '1';
	const char SYSTP_FRONT	= '2';

	typedef enum { STATUS_NON_LOGON, STATUS_LOGGEDON, STATUS_CLOSING } EN_CLIENT_STATUS;
	enum { CTX_DIE = 990, CTX_RQST_INNER_TICK, CTX_RQST_INNER_QUOTE, CTX_RQST_SEND, CTX_RQST_RECV };

	const int MEM_BLOCK_SIZE	= 1024;
	const int MEM_PRE_ALLOC		= 100;
	const int MEM_MAX_ALLOC		= 1000;

	const int WM_ADD_CLIENT		= WM_USER + 510;
	const int WM_RELEASE_CLIENT = WM_USER + 511;
	const int WM_DATAFEED		= WM_USER + 512;

	
	const int RT_SUCCESS = 0;

	BOOL RequestSendIO(SOCKET sock, char sysTp,char* pSendBuf, int nSendLen, char* pIoBuffer);
	BOOL RequestRecvIO(SOCKET sock, char sysTp, char* pIoBuffer);

	char* packlen(int len, char* out);
	char* rsltCode(int code, char* out);
	char* now(char* out);


	typedef struct _IO_CONTEXT
	{
		WSAOVERLAPPED	overLapped;
		WSABUF			wsaBuf;
		char			buf[LEN_BUFFER];
		char			sysTp;		//_ALGOINC::enum {SYSTP_CIENT=0, SYSTP_LB, SYSTP_FRONT};
		int				context;
		//BOOL			bLoginComplete;
	}IO_CONTEXT;



	struct HEADER
	{
		char len[4];
		char code[4];
		char sysTp[1];
		char rsltCode[4];
		char tm[17];
	};

	struct ST_1001
	{
		char	stx[1];
		HEADER	header;
		char	userId[LEN_USERID];
		char	pwd[10];
		char	mac[20];
		char	etx[1];
	};

	struct ST_1003
	{
		char	stx[1];
		HEADER	header;
		char	symbolCode[LEN_SYMBOL];
		char	etx[1];
	};

	struct ST_2001
	{
		char stx[1];
		HEADER header;
		char etx[1];
	};
	struct ST_2002
	{
		char	stx[1];
		HEADER	header;
		char	frontName[LEN_FRONT_NAME];
		char	userID[LEN_USERID];
		char	logOnOff[1];	// 0:logoff, 1:logon
		char	etx[1];
	};
	struct ST_2003
	{
		char	stx[1];
		HEADER	header;
		char	extraMsg[100];
		char	userID[LEN_USERID];
		char	etx[1];
	};


	// API TICK DATA PACKET
	struct ST_TICK_API
	{
		char	STX[1];
		char	Len[4];	// 전체 길이
		char	Date[8];
		char	Time[8];
		char	ShortCode[7];
		char	Close[10];
		char	CntrQty[10];
		char	DecLen[5];				// 소숫점 자릿수
		char	ETX[1];
	};
}