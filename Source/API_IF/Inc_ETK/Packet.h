#ifndef  __G7_COMMON_PACKET_20050220__
#define  __G7_COMMON_PACKET_20050220__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COMMON.h"

#define MAX_BUFSIZE		1024L
#define MAX_FILESIZE	4096L
#define	MAX_TRSIZE		2048L
#define	MAX_SENDSIZE	4096L
#define	LEN_SIZE		4L
#define CORRECT_LEN		6L
#define MAX_UPGRADE		6144L	//	6k
#define MAX_LOGGING		1024

#define MAX_ACNT_NO		11
#define MAX_USERID		12
#define MAX_STK_CODE	32
#define MAX_GDS_CODE	2
#define MAX_ARTC_CODE	10
#define MAX_ORD_NO		9
#define MAX_ORD_QTY		9
#define MAX_ORD_PRC		20
#define MAX_ORD_PRC_DOT	8

#define MAX_ORD_NO_EX		(20)	// ++ skeo 2011-04-29	

#define SET_USERID(id)	{							\
	char temp[MAX_USERID+1];						\
	int	len = min( MAX_USERID, strlen(id) );		\
	sprintf( temp, "%-*.*s", MAX_USERID, len, id);	\
	strcpy( id, temp);								\
}

#define	SET_LEN( i_len, o_len ) sprintf( o_len, "%0*d", LEN_SIZE, i_len-CORRECT_LEN )

//#define USE_USERCCYPL	1

//########################################################
//##	//HEADER, C_HEADER 가 붙은 패킷은
//##	Stx, Etx 를 붙여서 통신해야 한다.
//#########################################################


/**	\brief	CLIENT - SERVER 간 헤더
*/
typedef struct  _ST_CLIENT_HEADER
{
	char	Len		[LEN_SIZE];		// 길이 
	char	Code	[5];		// 코드 
}C_HEADER, * LPCLIENT_HEADER;	//	9
#define C_HEADER_LEN	sizeof(C_HEADER)

/**	\brief	CLIENT - SERVER 간 TR 패킷에 사용하는 TR //HEADER

	*	//HEADER + TRHEADER

	*	MsgGb	: 결과코드 (N, E)
		EndGb	: 패킷이 여러개 전송될 때 마지막 패킷인지 여부
		DataHeaderSize : TR_HEADER 다음 정보가 있을 때 해당 정보를 저장하는 SIZE
*/
typedef struct _TR_HEADER
{
	char	MsgGb	[1];		//	'N' OK, 'E' ERROR
	char	MsgCode	[4];
	char	FormNo	[4];
	char	EndGb	[1];		//	1: end, 0:not end
	char	DataHeaderSize[4];	//	14
}TR_HEADER, * LPTR_HEADER;

#define TRHEADER_ENDGB_END	'1'
#define TRHEADER_ENDGB_NEXT	'0'
#define TRHEADER_MSGGB_OK	'N'
#define TRHEADER_MSGGB_ERR	'E'

/*==================================================================================
/////////////		UPGRADE PACKET		/////////////
==================================================================================*/
/**	\brief	업그레이드용 패킷 중 CLIENT 가 SERVER 로 전송하는 패킷

	*	Complete	:	직전에 다운로드가 완료되었는지 여부 (0-미완, 1-완료)
	*	LastDate	:	최종 다운로드 일자
	*	FileName	:	직전 다운로드가 완료되지 않았을 때(Complete 가 0 일때)
						최종적으로 다운로드한 파일명
*/
typedef struct _ST_UPGRADE_TR
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		Complete	[1];	// 직전 다운로드완료여부 (0-미완  1-완료)
	char		LastDate	[14];	// YYYYMMDDHHMMSS
	char		FileName	[32];	// 파일 이름. client 에서 직전 다운로드가 완전하지 않으면 
									// 올릴때 최종 파일명을 넘긴다.	
	char		FilePath	[32];	// 최종 디렉토리 (Root 인 경우는 space)
	char		ETX			[1];
} ST_UPGRADE_TR, *PTR_UPGRADE_TR;
#define SIZE_UPGRADE_TR	sizeof(ST_UPGRADE_TR)

/**	\brief	업그레이드 파일 중 SERVER 가 CLIENT 에게 전송하는 패킷

	*	한 파일이 여러번 전송되어야 할 때 최초 전송하는 패킷

	*	FileFirstCheck	:	1 - 파일 최초, 0 - 최초 아니다, 2 - 파일이 없다.
		FileLastCheck	:	1 - 마지막, 0 - 마지막 아니다.
		Binary			:	실제 파일의 데이터
*/
typedef struct _PKT_UPGRADE 
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		FileFirstCheck[1];	// 1 - 파일 최초, 0 - not, 2 - 파일 없다.
	char		FileLastCheck[1];	// 1 - 파일 마지막, 0 - not
	char		FileCnt		[4];		// 업그레이드 할 파일 갯수
	char		FileDate	[14];	// YYYYMMDDHHMMSS
	char		FileName	[32];	// 파일 이름
	char		FilePath	[32];	// 경로
	char		FileSize	[8];	// 파일 크기	:: 이상 Data Header

	char		Binary[MAX_FILESIZE];

	char		ETX[1];
} PKT_UPGRADE, *PTR_PKT_UPGRADE;
#define SIZE_PKT_UPGRADE sizeof(PKT_UPGRADE)


/**	\brief	업그레이드 파일 중 SERVER 가 CLIENT 에게 전송하는 패킷

	*	한 파일이 여러번 전송되어야 할때 최초 다음에 전송하는 패킷
*/

typedef struct _PKT_UPGRADE_NEXT
{
	char				STX[1];
	C_HEADER			Header;
	TR_HEADER			TRHeader;
	char				FileFirstCheck[1];
	char				FileLastCheck[1];
	char				Binary[MAX_FILESIZE];
	char				ETX[1];
} PKT_UPGRADE_NEXT, *PTR_PKT_UPGRADE_NEXT;
#define SIZE_PKT_UPGRADE_NEXT	sizeof(PKT_UPGRADE_NEXT)



typedef struct _PKT_TR
{
	char			STX[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			Data[MAX_TRSIZE];
	char			ETX[1];
}PKT_TR, *PTR_PKT_TR;



//////////////////////////////////////////////////////////////////////////
//	LOGIN PACKET
typedef struct _ST_TR_00013_C
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID				[MAX_USERID];
	char			Conn_Pwd			[10];
	char			CLIENT_IP_LOCAL		[15];
	char			CLIENT_IP_PUBLIC	[15];
	char			CLIENT_MAC			[12];
	char			Reserved			[100];
	char			ETX					[1];
}ST_TR_00013_C;

typedef struct _ST_TR_00013_S
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID		[MAX_USERID]; 	
	char			SERVER_IP	[15];
	char			SERVER_PORT	[5]; 	
	char			Reserved	[100];
	char			ETX			[1];
}ST_TR_00013_S;


//////////////////////////////////////////////////////////////////////////
//	LOGIN COMMIT PACKET
typedef struct _ST_TR_00014_C
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID				[MAX_USERID];
	char			CLIENT_IP_LOCAL		[15];
	char			CLIENT_IP_PUBLIC	[15];
	char			CLIENT_MAC			[12];
	char			Reserved			[100];
	char			ETX					[1];
}ST_TR_00014_C;

typedef struct _ST_TR_00014_S
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID		[MAX_USERID]; 	
	char			Reserved	[100];
	char			ETX			[1];
}ST_TR_00014_S;

//	FRONT RELOAD
typedef struct _ST_TR_00018
{
	char		STX			[ 1];
	C_HEADER	Header			;
	TR_HEADER	TRHeader		;
	char		ETX			[ 1];
}ST_TR_00018;

//////////////////////////////////////////////////////////////////////////
//	주문
typedef struct _ST_TR_20000
{
	char		STX	[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;

	char		GDS_CODE	[2];
	char		UserID		[MAX_USERID];
	char		AcntPwd		[4];
	char		ACNT_NO		[MAX_ACNT_NO];
	char		STK_CODE	[MAX_STK_CODE];
	char		ORD_ST		[1];			//	시장가, 지정가, ..
	char		COND_ORD_ST	[1];
	char		BUY_SELL_TP	[1];			//	1:SELL, 2:BUY
	char		ORD_PROC_TP	[1];			//	신규, 정정, 취소
	char		ORG_ORD_NO	[MAX_ORD_NO];	//	
	char		ORG_ORD_PRC	[MAX_ORD_PRC];	//	20,8 (소수이하 8)	//	취소/정정 주문에만 사용

	char		ORD_PRC		[MAX_ORD_PRC];	//	신규주문가격, 정정인 경우 정정가격 ONLY, 20,8 (소수이하 8)
	char		ORD_QTY		[MAX_ORD_QTY];	//	주문수량
// 	char		MDFY_ORD_PRC[20];			//	정정주문 ONLY 20,8 (소수이하 8)
// 	char		MDFY_ORD_QTY[9];			//	정정주문인 경우 ONLY
// 	char		CNCL_ORD_QTY[9];			//	취소주문인 경우 ONLY
	char		ALL_YN		[1];			//	ALL Cancel/Modify? or Partially cancel/modify?
	char		MKT_RNG		[3];			//	Slippage 범위
	char		SL_PRC		[MAX_ORD_PRC];	//	20,8 (소수이하 8)//	조건주문(STOP/LIMIT) 주문환율
	char		TS_RNG		[3];			//	TS 범위 (TICK)
	char		IFD_YN		[1];
	char		ORD_TERM_TP	[1];
	char		FCM_CODE	[5];
	char		FORCE_ORD_YN[1];			//	 
	char		MNG_USERID	[MAX_USERID];
	
	//char		ORG_ORD_NO_FU[MAX_ORD_NO_EX];//	하나대투 해외선물 주문번호. ++ skeo 2011-04-29		

	char		ETX	[1];
}ST_TR_20000;


// 동양선물(FU) 주문 패킷
typedef struct _DONGYANG_FU_TR_20000_I
{
	char		API_USERID		[MAX_USERID];
	char		API_ACNT_NO		[MAX_ACNT_NO];
	char		API_ACNT_PWD	[4];
	char		STK_CODE		[MAX_STK_CODE];
	char		ORD_PRC			[MAX_ORD_PRC];	//	신규주문가격, 정정인 경우 정정가격 ONLY, 20,8 (소수이하 8)
	char		ORD_QTY			[MAX_ORD_QTY];	//	주문수량
	char		BUY_SELL_TP		[1];			//	1:매수, 2:매도
	char		ORD_ST			[1];			//	1: 지정가, 2: 시장가
	char		ORD_TERM_TP		[1];			//	1: FAS, 3: FAS
	char		ORD_ST2			[1];			//	1: 일반, 2: STOP
	char		STOP_PRC		[MAX_ORD_PRC];	//	STOP ORDER 지정가격
	char		CLORD_NO		[MAX_ORD_NO];
	char		PROC_TP			[1];			// 1:NEW, 2:MODIFY, 3:CANCEL
	char		ORG_ORD_NO		[MAX_ORD_NO];	// Modify and Cancel Order
}DONGYANG_FU_TR_20000_I;


// 하나대투(KS) 주문 패킷
typedef struct _HANADT_FU_TR_20000_I
{
	char		Rcnt		[4];	// 처리 건수
	char		Odgb		[1];	// 1:일반, 2:선물옵션
	char		Mkgb		[1];	// 1:거래소, 2:코스닥, 3:프리보드, 5:장외
	char		Mmgb		[1];	// 1:매도, 2:매수, 3:정정, 4:취소
	char		Acno		[10];
	char		Pswd		[8];
#if _NO_NINE	// 주문번호자릿수 9 자리 (SAEHAN, BLASH 실전만))
	char		Ogno		[12];	// 정정/취소시 원주문번호
#else
	// ++ skeo 2011-04-29	
	char		Ogno		[MAX_ORD_NO_EX];	// 정정/취소시 원주문번호	
#endif
	char		Code		[12];	// 단축코드
	char		Jqty		[8];	// 주문수량
	char		Jprc		[10];	// 주문가격(선물옵션의 경우 100배수 처리 [ex] 112.13 -> 11213
	char		Hogb		[2];	// 00:지정가, 03:시장가, 05:조건부지정가, 06:최유리지정가, 07:최우선지정가
	char		Jmgb		[2];	// 01:IOC, 02:FOK
	char		Mdgb		[1];	// 1:일부, 2:전부
	char		Prgb		[1];	// 'X'
}HANADT_FU_TR_20000_I;


//	OrdChk class 에서 반환할 정보
typedef struct _ST_SV_ORD_CHK_RSLT
{
	char			MINI_YN		[1];	//	MINI 고객 여부
	char			API_CODE	[11];
	char			API_ACNT_NO	[20];
	char			API_PWD		[10];
	char			ClORD_NO	[MAX_ORD_NO];	//	우리가 생성한 주문번호
	char			ORD_PRC		[MAX_ORD_PRC];
	char			ORD_QTY		[MAX_ORD_QTY];
//	char			SERVER_IP	[15];
	char			ORD_TM		[9];	//	HHMMSSmmm
	char			API_USERID	[MAX_USERID];
	char			NTTN_CODE	[2];
	char			Reserved	[6];	//오토봇. 2byte 를 체결율에 사용한다.(%값)
	ST_TR_20000		stC;
	
}ST_SV_ORD_CHK_RSLT;




/************************************************************************
	API 와의 관계
************************************************************************/

//	IF 프로그램이 MQ 에 넣을 때 채워야 할 정보
typedef struct	_ST_MQLABEL_API_INFO
{
	char	CODE		[2];			//	TR, RL
	char	GDS_CODE	[2];
	char	API_CODE	[11];
	char	NTTN_CODE	[2];	//	해당 종목의 진법코드
}ST_MQLABEL_API_INFO;

// 하나대투 해외선물 주문번호 길이. skeo 2011-04-29		
#define HANADT_FU_ORDER_NO_LEN		14	

//	각 API에서 받은 주문TR 응답데이터를 변환할 공통 포맷
typedef struct _ST_IFRECV_TR_ORD
{
	char	GDS_CODE	[2];
	char	CLORD_NO	[MAX_ORD_NO];	//	CLIENT ORDER NO
#if _NO_NINE	// 주문번호자릿수 9 자리 (SAEHAN, BLASH 실전만))
	char	ORD_NO		[MAX_ORD_NO];	//	API ORD NO
#else
	// ++ skeo 2011-04-29	
	char	ORD_NO		[MAX_ORD_NO_EX];	//	API ORD NO
#endif
	char	RSLT_CODE	[4];			//	성공여부를 우리의 RET_CODE 값으로 표현(RET_SUCCESS or RET_ORD_RJCT)
	char	API_CODE	[11];
	char	API_MSGCODE	[6];
	char	API_MSG		[100];
}ST_IFRECV_TR_ORD;

//	각 API에서 받은 체결REAL 응답데이터를 변환할 공통 포맷
typedef struct _ST_IFRECV_REAL_CNTR
{
	char	ORD_RSLT_CODE	[4];	//	0~3	9000(신규주문)/9001(정정)/9002(취소)/9003(거부)/9004(체결)/9005(주문TR)
	char	GDS_CODE		[2];	//	4~5
	char	API_CODE		[11];	//	6~16
	char	STK_CODE		[MAX_STK_CODE];	//	17~48
	char	API_USERID		[MAX_USERID];	//	49~60
	char	API_ACNT_NO		[20];			//	61~80
#if _NO_NINE	// 주문번호자릿수 9 자리 (SAEHAN, BLASH 실전만))
	char	API_ORD_NO		[MAX_ORD_NO];	//	81~89
	char	ClORD_NO		[MAX_ORD_NO];	//	82~98
	char	API_ORD_ORG_NO	[MAX_ORD_NO];	//	99~107
	char	API_CNTR_NO		[MAX_ORD_NO];	//	108~116
#else
	// ++ skeo 2011-04-29	
	char	API_ORD_NO		[MAX_ORD_NO_EX];	//	
	char	ClORD_NO		[MAX_ORD_NO];		//	
	char	API_ORD_ORG_NO	[MAX_ORD_NO_EX];	//	
	char	API_CNTR_NO		[MAX_ORD_NO_EX];	//	
#endif
	char	API_MKT_TP		[1];		//	시장구분(1/5/6/7)
	char	ORD_ST			[1];
	char	PROC_TP			[1];
	//char	ACPT_TP			[1];
	char	BUY_SELL_TP		[1];
	char	ORD_QTY			[MAX_ORD_QTY];
	char	ORD_PRC			[MAX_ORD_PRC];
	char	CNTR_QTY		[MAX_ORD_NO];
	char	CNTR_PRC		[MAX_ORD_PRC];
	char	REMN_QTY		[MAX_ORD_NO];		//	미체결잔량
	char	API_RJCT_CODE	[4];		//	거부시만,
	char	API_RJCT_MSG	[40];		//	거부시만
	char	DT				[8];
	char	TM				[9];		//	hhmmssmmm
}ST_IFRECV_REAL_CNTR;


//////////////////////////////////////////////////////////////////////////
//	MINI 를 위한 패킷
// typedef struct _ST_MINI_TR_ORD_I
// {
// 	char	incnt		[4];	//	주문카운트 (zero padding)
// 	char	mapno		[5];	//	TR 사용하는 MAP 번호
// 	char	clikey		[9];	//	주문key(zero padding)
// 	char	gb			[1];	//	1:매도, 2:매수, 3:정정, 4:취소
// 	char	zAcntNo		[20];	//	'-'는 모두 제거. (space trailing)
// 	char	zInptPwd	[8];	//	계좌비밀번호
// 	char	zFnoIsuNo	[15];	//	종목코드(space trailing)
// 	char	lOrgOrdNo	[9];	//	zero padding
// 	char	Ord_ST		[2];	//	00(지정가), 03(시장가), 05(조건부지정가), 06(최유리지정가)
// 	char	dOrdPrc		[12];	//	zero padding
// 	char	lOrdQty		[15];	//	zero padding
// 	char	ip			[12];	//	199.99.9.90 -> 199099009090
// 	//	이상 동부 API 그대로
// 	
// 	char	ord_term_tp	[1];	//	DAILY, GTC, FOK, IOC
// }ST_MINI_TR_ORD_I;
// 
// typedef struct _ST_MINI_TR_ORD_O
// {
// 	char	outcnt		[4];	//	주문카운트 (zero padding)
// 	char	clikey		[9];	//	주문key(zero padding)
// 	char	lOrdNo		[9];	//	0 이하일 경우 주문장애
// 	char	msgcd		[6];	//	
// 	char	msgcm		[100];	
// 	//	이상 동부 API 그대로
// 	
// 	char	ord_term_tp	[1];	//	DAILY, GTC, FOK, IOC
// 	
// }ST_MINI_TR_ORD_O;

//	주문패킷
typedef ST_SV_ORD_CHK_RSLT	ST_MINI_TR_ORD_I;
//	주문/체결 통보
typedef ST_IFRECV_REAL_CNTR	ST_MINI_REAL_CNTR;


//////////////////////////////////////////////////////////////////////////
//	NOTIFY REAL PACKET
typedef struct _ST_REAL_84901
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		ItemCd		[2];	//	11:장마감전통보, 12:장마감통보
	char		Title		[80];	
	char		Data		[MAX_BUFSIZE];
	char		ETX			[1];
}ST_REAL_84901;




/*
//입출금실시간
  - 입출금컴포넌트(70302)에서 입출금확인 후 이 패킷을 클라이언트에 보내면
    클라이언트는 TR을 다시 날려 정보 갱신
*/
typedef struct _ST_REAL_84902
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		RESERVED	[20];
	char		ETX			[1];
}ST_REAL_84902;


//	CLIENT 에게 전달할 주문/체결 결과 패킷
typedef struct _ST_REAL_82004
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;

	char	ORD_RSLT_CODE	[4];
	char	GDS_CODE		[2];
	char	STK_CODE		[MAX_STK_CODE];
	char	USERID			[MAX_USERID];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	ORD_NO			[MAX_ORD_NO];
	char	ORD_ORG_NO		[MAX_ORD_NO];
	char	CNTR_NO			[MAX_ORD_NO];
	char	ORD_ST			[1];
	char	PROC_TP			[1];
	char	ACPT_TP			[1];
	char	BUY_SELL_TP		[1];
	char	ORD_QTY			[MAX_ORD_QTY];
	char	ORD_PRC			[MAX_ORD_PRC];
	char	CNTR_QTY		[MAX_ORD_NO];
	char	CNTR_PRC		[MAX_ORD_PRC];
	char	REMN_QTY		[MAX_ORD_NO];		//	미체결잔량
	char	PRC_REMN_QTY	[MAX_ORD_QTY];
	char	DT				[8];
	char	TM				[9];				//	hhmmssmmm

	char	AFTER_CLR_PL_SUM[15];
	char	AFTER_POS_SUM	[MAX_ORD_QTY];		//	체결 후 포지션 수량 SUM
	char	AFTER_AVG_PRC	[MAX_ORD_PRC];
	char	AFTER_BUY_SELL_TP[1];

	char	AFTER_CAN_ORD_QTY_S	[4];	//	체결 후 매도 주문가능수량
	char	AFTER_CAN_ORD_QTY_B	[4];	//	체결 후 매수 주문가능수량

	char	NOW_PRC				[15];	//	현재가를 넣는다.
	char	CMSN				[5];
	char	STK_REMNQ_SELL		[4];	//	해당 종목의 미체결잔량 전체
	char	STK_REMNQ_BUY		[4];
	char	TOT_EVLT_AMT		[15];
	char	EVLT_AMT			[15];
	
	char	ETX			[1];
}ST_REAL_82004;

//	CLIENT 에게 전달할 실시간 잔고
typedef struct _ST_REAL_82005
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	
	char	ACNT_NO			[MAX_ACNT_NO];
	char	USERID			[MAX_USERID];
	char	ACPT_TP			[1];
	char	STK_CODE		[MAX_STK_CODE];
	char	AFTER_CAN_ORD_QTY_S	[4];	//	체결 후 매도 주문가능수량
	char	AFTER_CAN_ORD_QTY_B	[4];	//	체결 후 매수 주문가능수량
	char	NOW_PRC				[20];	//	현재가를 넣는다.
	char	STK_REMNQ_SELL		[4];	//	해당 종목의 미체결잔량 전체
	char	STK_REMNQ_BUY		[4];
	char	AFTER_CLR_PL_SUM	[15];	//	해당종목 손익
	char	AFTER_POS_SUM		[MAX_ORD_QTY];		//	체결 후 포지션 수량 SUM
	char	AFTER_AVG_PRC		[MAX_ORD_PRC];
	char	AFTER_BUY_SELL_TP	[1];
	char	CMSN				[5];
	char	TOT_EVLT_AMT		[15];
	char	EVLT_AMT			[15];
	
	//	이하 20101124 김배완 추가
	char	AFTER_TOT_PL_SUM	[15];	//	전종목 손익
	char	AFTER_REMNQ_SELL	[4];	//	전종목 미체결잔량
	char	AFTER_REMNQ_BUY		[4];	//	전종목 미체결잔량	

	//	20101216 김배완 추가
	char	LossCutAmt			[15];	//	현포지션의 로스컷 금액
	char	ETX			[1];
}ST_REAL_82005;


//////////////////////////////////////////////////////////////////////////
//	시스템신호 송출 1 (특정계좌 수익정보)
typedef struct _ST_REAL_83005
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;

	char	GROUP_IDX	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	POS_QTY		[15];
	char	AVG_PRC		[MAX_ORD_PRC];
	char	EVLT_PL		[15];
	char	CLR_PL		[15];
	char	TOT_PL		[15];
	char	ETX			[1];
}ST_REAL_83005;


//////////////////////////////////////////////////////////////////////////
//	MASTER FILE
typedef	struct _MASTER_KSF
{
	char	STK_CODE	[MAX_STK_CODE];	
	char	STK_NM		[30];	
	char	ARTC_CODE	[MAX_ARTC_CODE];	//	품목코드
	char	ARTC_NM		[30];	//	품목명
	char	NTTN_CODE	[2]; 	//	진법코드
	char	LSTNG_DT	[8];	//	상장일
	char	MTR_DT		[8];	//	만기일
	char	TICK_SIZE	[7];	//	xxxx.xx
	char	TICK_VALUE	[7];
} MASTER_KSF;


typedef	struct _MASTER_KSO
{
	char	STK_CODE		[MAX_STK_CODE];	
	char	STK_NM			[30];	
	char	ARTC_CODE		[MAX_ARTC_CODE];	//	품목코드
	char	ARTC_NM			[30];	//	품목명
	char	NTTN_CODE		[2]; 	//	진법코드
	char	LSTNG_DT		[8];	//	상장일
	char	MTR_DT			[8];	//	만기일
	char	STRK_PRC		[MAX_ORD_PRC];	//	STRIKE PRICE
	char	TICK_CHG_STND	[7];
	char	TICK_SIZE_1		[7];	//	xxxx.xx
	char	TICK_VALUE_1	[7];
	char	TICK_SIZE_2		[7];	//	xxxx.xx
	char	TICK_VALUE_2	[7];
}MASTER_KSO;


typedef	struct _MASTER_FU
{
	char	STK_CODE	[MAX_STK_CODE];	
	char	STK_NM		[30];	
	char	ARTC_CODE	[MAX_ARTC_CODE];	//	품목코드
	char	ARTC_NM		[30];	//	품목명
	char	NTTN_CODE	[2]; 	//	진법코드
	char	LSTNG_DT	[8];	//	상장일
	char	MTR_DT		[8];	//	만기일
	char	FND_DT		[8];	//	최종인도일
	char	TICK_SIZE	[7];	//	xxxx.xx
	char	TICK_VALUE	[7];
} MASTER_FU;


typedef	struct _MASTER_FX
{
	char	STK_CODE			[MAX_STK_CODE];	
	char	STK_NM				[30];	
	char	FCM_CODE			[12];
	char	FCM_NM				[30];
	char	DEAL_UNIT			[15];
	char	PIP_UNIT			[11];	//	XXXXX.XXXXX
	char	HOGA_UNIT			[11];	//	TICK SIZE
	char	PIP_VALUE_TP		[1];	//	1(미국식),2(유럽식),3(교차미국식),4(교차유럽식)
	char	PIP_VALUE_STK_CODE	[32];
} MASTER_FX;

typedef	struct _MASTER_MSG
{
	char	MSGCODE				[4];	
	char	MSG_KO				[200];	
	char	MSG_ENG				[200]; // ERROR MESSAGE 영문 버전
	char	MSG_CHINA			[200]; // ERROR MESSAGE 중국어 버전

} MASTER_MSG;


//	API 가 보내주는 MASTER FILE
typedef struct _API_MASTER_KSF
{
	char	STK_GB				[1];	//	무조건 'F'
	char	Filler_1			[1];	//	SPACE
	char	YearMon				[4];	//	2010년 3월이면 "1003 "
	char	Filler_2			[24];	//	SPACE
	char	STK_CODE			[8];
}API_MASTER_KSF;

typedef struct _API_MASTER_KSO
{
	char	CALL_PUT			[1];	//	C:CALL, P:PUT
	char	Filler_1			[1];	//	SPACE
	char	YearMon				[4];	//	2010년 3월이면 "1003 "
	char	Filler_2			[1];	//	SPACE
	char	STRK_PRC			[6];	//	행사가
	char	Filler_3			[1];	//	space
	char	STK_CODE			[8];
}API_MASTER_KSO;


//////////////////////////////////////////////////////////////////////////
//	KOSPI200 선물/옵션 현재가 공통 포맷 (81003)
typedef struct _REAL_SISE_KS
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;

	char	stk_code			[MAX_STK_CODE];		//32
	char	gds_code			[2];	// 2	KSF, KSO 구분 2010/07/26 추가	//34
	char	tm					[6];											//40
	char	now_prc				[7];	//	xxxx.xx								//47
	char	open				[7];	//	2									//54
	char	high				[7];	//	2									//61
	char	low					[7];	//	2									//68
	char	acml_cntr_qty		[7];	//	0	누적체결수량					//75
	char	acml_buy_cntr_qty	[6];	//	0	누적매수체결량	//	사용하지 않음	//81
	char	buy_sell_tp			[1];												//82
	char	acml_amt			[15];	//	0	누적거래대금						//97
	char	daebi_sign			[1];	//	0	전일대비부호						//98
	char	chg					[7];	//	2	전일대비							//105
	char	chg_rate			[7];	//	2	등락율
	char	cntr_qty			[6];	//	0	체결수량
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	매도잔량
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	총매도호가잔량
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	총매도건수
	char	o_hoga_cnt_chg_1	[4];	//	0	매도호가건수대비
	char	o_hoga_cnt_chg_2	[4];	//	0	
	char	o_hoga_cnt_chg_3	[4];	//	0	
	char	o_hoga_cnt_chg_4	[4];	//	0	
	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	매수잔량
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		총매수호가잔량
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	총매수건수
	char	b_hoga_cnt_chg_1	[4];	//	0	
	char	b_hoga_cnt_chg_2	[4];	//	0	
	char	b_hoga_cnt_chg_3	[4];	//	0	
	char	b_hoga_cnt_chg_4	[4];	//	0	
	char	b_hoga_cnt_chg_5	[4];	//	0	
	char	ks200				[7];	//	2	
	char	hoga_trmnq_chg		[7];	//	0	호가총잔량차이
	char	qty_chg				[9];	//		거래량증감
	char	amt_chg				[9];	//		거래대금증감
	char	theory_prc			[7];	//	2	이론가
	char	open_yak			[7];	//	0	미결제약정수량(부호)-옵션만부호있음
	char	open_yak_chg		[6];	//		미결제약정수량대비
	char	gyuri_rate			[7];	//	2	괴리율
	char	basis				[7];	//	2	시장베이시스
	char	open_chg			[7];	//	2	시가대비
	char	high_chg			[7];	//	2	고가대비
	char	low_chg				[7];	//	2	저가대비
	char	theory_basis		[7];	//	2	이론베이시스
	char	imp_v				[7];	//		내재변동성 - OPTION ONLY
	char	delta				[7];	//		델타 - OPTION ONLY
	char	gmma				[7];	//		감마 - OPTION ONLY
	char	veta				[7];	//		베타 - OPTION ONLY
	char	theta				[7];	//		세타 - OPTION ONLY
	char	rho					[7];	//		로 - OPTION ONLY
	char	in_val				[6];	//		내재가치 - OPTION ONLY
	char	tm_value			[6];	//		시간가치 - OPTION ONLY
	char	cntr_net_buy		[9];	//		체결순매수 - OPTION ONLY
	char	rmnq_gap			[7];	//		잔량차(매수잔량-매도잔량) - OPTION ONLY
	char	cnt_gap				[5];	//		건수차(매수건수-매도건수) - OPTION ONLY
	
	char	ETX					[1];

}REAL_SISE_KS;		// 508


//////////////////////////////////////////////////////////////////////////
//	KOSPI200 선물옵션 현재가 (TR:21004, REAL:81004)
typedef struct _REAL_NOW_PRC
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_PRC		[7];
	char	ETX					[1];

}REAL_NOW_PRC;

//////////////////////////////////////////////////////////////////////////
//	KOSPI200 선물/옵션 호가 공통 포맷 (81103)
typedef struct _REAL_HOGA_KS
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	//패킷크기 때문에 'MAX_STK_CODE'(32) 아닌 '8'Byte
	char	gds_code			[2];	// 2	KSF, KSO 구분 2010/07/26 추가
	char	tm					[6];
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	매도잔량
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	총매도호가잔량
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	총매도건수
// 	char	o_hoga_cnt_chg_1	[4];	//	0	매도호가건수대비
// 	char	o_hoga_cnt_chg_2	[4];	//	0	
// 	char	o_hoga_cnt_chg_3	[4];	//	0	
// 	char	o_hoga_cnt_chg_4	[4];	//	0	
// 	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	매수잔량
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		총매수호가잔량
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	총매수건수
// 	char	b_hoga_cnt_chg_1	[4];	//	0	
// 	char	b_hoga_cnt_chg_2	[4];	//	0	
// 	char	b_hoga_cnt_chg_3	[4];	//	0	
// 	char	b_hoga_cnt_chg_4	[4];	//	0	
// 	char	b_hoga_cnt_chg_5	[4];	//	0	
// 	char	hoga_trmnq_chg		[7];	//	0	호가총잔량차이
	
	char	ETX					[1];
	
}REAL_HOGA_KS;		// 281 (234)


//////////////////////////////////////////////////////////////////////////
//	KOSPI200 선물/옵션 현재가 공통 포맷 두번째
typedef struct _REAL_SISE_KS_84003
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	
	char	gds_code			[2];	// 2	KSF, KSO 구분 2010/07/26 추가
	char	tm					[6];
	char	now_prc				[7];	//	xxxx.xx
	char	open				[7];	//	2	
	char	high				[7];	//	2	
	char	low					[7];	//	2	
	char	acml_cntr_qty		[7];	//	0	누적체결수량
	char	acml_buy_cntr_qty	[6];	//	0	누적매수체결량	//2012.08.22. basis 사용으로 대체.
	char	buy_sell_tp			[1];
	char	acml_amt			[15];	//	0	누적거래대금
	char	daebi_sign			[1];	//	0	전일대비부호
	char	chg					[7];	//	2	전일대비
	char	chg_rate			[7];	//	2	등락율
	char	cntr_qty			[6];	//	0	체결수량
	char	ks200				[7];	//	2	
	char	qty_chg				[9];	//		거래량증감
	char	amt_chg				[9];	//		거래대금증감
	char	open_yak			[7];	//	0	미결제약정수량(부호)-옵션만부호있음
	char	open_yak_chg		[6];	//		미결제약정수량대비 
	char	ETX					[1];
	
}REAL_SISE_KS_84003;		// 220


//////////////////////////////////////////////////////////////////////////
//	해외선물 현재가, 호가 공통 포맷
typedef struct _REAL_SISE_FU
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	
	char	tm					[6];
	char	nttn_code			[2];
	char	now_prc				[MAX_ORD_PRC];	//	20,8
	char	open				[MAX_ORD_PRC];	//	20,8
	char	high				[MAX_ORD_PRC];	//	20,8
	char	low					[MAX_ORD_PRC];	//	20,8
	char	last_close			[MAX_ORD_PRC];	//	20,8
	char	setl_price			[MAX_ORD_PRC];	//	20,8	
	char	acml_cntr_qty		[MAX_ORD_QTY];	//	0	누적체결수량
	char	acml_buy_cntr_qty	[MAX_ORD_QTY];	//	0	누적매수체결량
	char	acml_amt			[15];	//	0	누적거래대금
	char	daebi_sign			[1];	//	0	전일대비부호
	char	chg					[7];	//	2	전일대비
	char	chg_rate			[7];	//	2	등락율
	char	cntr_qty			[9];	//	0	체결수량
	char	o_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	o_rmnq_1			[6];	//	0	매도잔량
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	총매도호가잔량
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	총매도건수
	char	o_hoga_cnt_chg_1	[4];	//	0	매도호가건수대비
	char	o_hoga_cnt_chg_2	[4];	//	0	
	char	o_hoga_cnt_chg_3	[4];	//	0	
	char	o_hoga_cnt_chg_4	[4];	//	0	
	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	b_rmnq_1			[6];	//	0	매수잔량
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		총매수호가잔량
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	총매수건수
	char	gl_str				[MAX_STK_CODE];
	char	dspl_now_prc		[MAX_ORD_PRC];
	char	ETX					[1];
}REAL_SISE_FU;


//////////////////////////////////////////////////////////////////////////
//	해외선물 현재가, 호가 공통 포맷
typedef struct _REAL_HOGA_FU
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	
	char	tm					[6];
	char	o_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	o_rmnq_1			[6];	//	0	매도잔량
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	총매도호가잔량
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	총매도건수
	char	b_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	b_rmnq_1			[6];	//	0	매수잔량
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		총매수호가잔량
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	총매수건수
	
	char	ETX					[1];
	
}REAL_HOGA_FU;


//////////////////////////////////////////////////////////////////////////
//	FX마 현재가, 호가 공통 포맷
typedef struct _REAL_SISE_FX
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;

	char	stk_code			[MAX_STK_CODE];	
	char	tm					[6];
	char	nttn_code			[2];
	char	o_now_prc			[11];	//	20,8
	char	b_now_prc			[11];	//	20,8
	char	spread_sign			[1];
	char	spread				[11];
	char	o_open				[11];
	char	b_open				[11];
	char	open				[11];
	char	o_high				[11];
	char	b_high				[11];
	char	high				[11];
	char	o_low				[11];
	char	b_low				[11];
	char	low					[11];
	char	o_intr				[11];	//	11,2
	char	b_intr				[11];	//	11,2
	char	pip_value			[11];	//	11,2
	
	char	ETX					[1];
	
}REAL_SISE_FX;

/***********************************************************************

	COMPONENT(컴포넌트) 용 패킷 정의 (정렬순서 : TR번호순)

***********************************************************************/

///////////////////////////////////////////////////////////////////////////
//	계좌조회 화면: X1TR00015 :  화면번호 (????)

typedef struct _ST_TR_00015_I
{
	char	UserID		[MAX_USERID];	//	사용자ID
}ST_TR_00015_I;

typedef struct _ST_TR_00015_O
{
	char	UserID		[MAX_USERID];	//	사용자ID
	char	Acnt_No_Ks	[MAX_ACNT_NO];	//	계좌번호 (KS) 예: 00-00-00000 (11자리)
	char	Acnt_Nm_Ks	[20];			//	계좌명 (KS)
	char	Acnt_Pwd_Ks	[4];			//	계좌비밀빈호 (KS)
	char	Acnt_No_Fu	[MAX_ACNT_NO];	//	계좌번호 (FU)
	char	Acnt_Nm_Fu	[20];			//	계좌명 (FU)
	char	Acnt_Pwd_Fu	[4];			//	계좌비밀빈호 (FU)
	char	Acnt_No_Fx	[MAX_ACNT_NO];	//	계좌번호 (FX)
	char	Acnt_Nm_Fx	[20];			//	계좌명 (FX)
	char	Acnt_Pwd_Fx	[4];			//	계좌비밀빈호 (FX)
}ST_TR_00015_O;

///////////////////////////////////////////////////////////////////////////
//	포지션(잔고) 내역 조회 화면: X1TR20001 :  화면번호 (2101)

typedef struct _ST_TR_20001_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd		[ 4];			//	계좌 비밀번호
	char	StkCode		[MAX_STK_CODE];	//	종목코드
}ST_TR_20001_I;

typedef struct _ST_TR_20001_O
{
	char	StkCode		[MAX_STK_CODE];	//	종목코드
	char	GdsCode		[ 2];			//	상품코드 (00:KS선물, 01:KS옵션, 02:해외선물, 03:FX마진)
	char	BuySell_Tp	[ 1];			//	주문구분 (1.매도 , 2.매수)
	char	Qty			[10];			//	체결수량
	char	AvgPrc		[MAX_ORD_PRC];	//	평균단가
	char	EvalPnlAmt	[15];			//	평가손익
	char	AvgRate		[10];			//	수익률
	char	CLTL		[10];			//	담보금
}ST_TR_20001_O;


//////////////////////////////////////////////////////////////////////////
//	미체결주문조회 화면: X1TR20005  , 화면번호 (1102)

typedef struct _ST_TR_20005_I 
{
	char		ACNT_NO		[MAX_ACNT_NO];	//	계좌번호
	char		ACNT_PWD	[4];			//	계좌 비밀번호
	char		GDS_CODE	[2];
	char		STK_CODE	[MAX_STK_CODE];
}ST_TR_20005_I;

typedef struct _ST_TR_20005_O
{
	char	ACNT_NO		[MAX_ACNT_NO];
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	ORD_NO		[MAX_ORD_NO];
	char	ORG_NO		[MAX_ORD_NO];
	char	BUY_SELL_TP	[1];
	char	ORD_PRC		[MAX_ORD_PRC];
	char	ORD_QTY		[MAX_ORD_QTY];
	char	REMN_QTY	[MAX_ORD_QTY];
	char	ORD_ST		[1];
	char	MNG_USERID	[MAX_USERID];
	
}ST_TR_20005_O;


//////////////////////////////////////////////////////////////////////////
//	일자별 입출금
typedef struct _ST_TR_20011_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	ACNT_PWD	[4];
	char	DT_FROM		[8];
	char	DT_TO		[8];
	char	NEXT_KEY	[10];
}ST_TR_20011_I;

typedef struct _ST_TR_20011_O
{
	char	ACNT_NO		[MAX_ACNT_NO];
	char	USERID		[MAX_USERID];
	char	USER_NM		[20];
	char	REQ_DT		[8];
	char	REQ_TM		[8];
	char	REQ_CODE	[4];		//	입금, 출금
	char	REQ_MONEY	[10];
	char	RESULT		[4];
	char	RSLT_DT		[8];
	char	RSLT_TM		[8];
	char	NEXT_KEY	[10];
}ST_TR_20011_O;


///////////////////////////////////////////////////////////////////////////
//	담보금 입금 화면: X1TR20012 :  화면번호 (3001) , 입금

typedef struct _ST_TR_20012_I
{
	char	Proc_Tp		[1];			//	Q : 조회, U : 신청
	char	UserID		[MAX_USERID];	//	사용자ID
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd		[ 4];			//	계좌 비밀번호
	char	Req_Money	[15];			//	신청금액 ( Proc_TP 가 Q 일때는 값이 없어도 됨 )
	char	GujaCnt		[2];			//	신청 구좌수
	char	DayCnt		[3];			//	신청 사용일 수 
	char	Dpst_Nm		[20];			//	입금자명 ( 입금 신청시 입력)
}ST_TR_20012_I;

typedef struct _ST_TR_20012_O
{
	char	Cpny_Bank_Nm		[20];	//	입금할 회사 계좌은행
	char	Cpny_Acnt_No		[20];	//	입금할 회사 계좌번호
	char	Cpny_Acnt_Nm		[20];	//	입금할 회사 계좌법인(사용자)명
	char	NOW_CLTL			[11];
	char	Success_Yn			[ 1];	//	입금성공 구분 (0:성공 , 1:실패)
}ST_TR_20012_O;



///////////////////////////////////////////////////////////////////////////
//	담보금 입금 화면: X1TR30012 :  화면번호 (3003) , 해외선물 입금

typedef struct _ST_TR_30012_I
{
	char	Proc_Tp		[1];			//	Q : 조회, U : 신청
	char	UserID		[MAX_USERID];	//	사용자ID
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd		[ 4];			//	계좌 비밀번호
	char	Req_Money	[15];			//	신청금액 ( Proc_TP 가 Q 일때는 값이 없어도 됨 )
	char	Req_Money_KRW	[15];			//	신청금액 ( Proc_TP 가 Q 일때는 값이 없어도 됨 )
	char	Forex		[7];			//	환율
	char	Dpst_Nm		[20];			//	입금자명 ( 입금 신청시 입력)
}ST_TR_30012_I;

typedef struct _ST_TR_30012_O
{
	char	Cpny_Bank_Nm		[20];	//	입금할 회사 계좌은행
	char	Cpny_Acnt_No		[20];	//	입금할 회사 계좌번호
	char	Cpny_Acnt_Nm		[20];	//	입금할 회사 계좌법인(사용자)명
	char	NOW_CLTL			[11];
	char	Forex_Buy			[7];	//	외화매입환율(외화입금)
	char	Forex_Sell			[7];	//	외화매도환율(외화출금)
	char	Success_Yn			[ 1];	//	입금성공 구분 (0:성공 , 1:실패)

}ST_TR_30012_O;



///////////////////////////////////////////////////////////////////////////
//	담보금 출금 화면: X1TR20013 :  화면번호 (3001) , 출금

typedef struct _ST_TR_20013_I
{
	char	Proc_Tp				[1];			//	Q : 조회, U : 신청
	char	UserID				[MAX_USERID];	//	사용자ID
	char	AcntNo				[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd				[ 4];			//	계좌 비밀번호
	char	Req_Money			[15];			//	신청금액. 신청시 입력
}ST_TR_20013_I;

typedef struct _ST_TR_20013_O
{
	char	User_Bank_Nm		[20];	//	고객이 받을 은행 명
	char	User_Bank_Acnt_No	[20];	//	고객이 받을 은행 계좌
	char	User_Bank_User_Nm	[20];	//	고객 계좌 예금주
	char	User_CanOut_Entr	[15];	//	출금가능금액	
	char	Success_Yn			[ 1];	//	출금신청 성공 구분 (0:성공 , 1:실패)
}ST_TR_20013_O;


// 1-016-007-005-003. client logging
typedef struct _ST_TR_88888_C
{
	char			Code				[2];	// 00 : client logging
	//char			SeqNo				[9];
	char			UserID				[MAX_USERID];
	char			UserNm				[20];
	char			DT					[8];
	char			TM					[12];
	char			Data				[128];
	char			Reserved			[50];
	
}ST_TR_88888_C;

typedef struct _ST_TR_88888_S 
{
	char			Code		[2];
	char			Result		[1];
	char			Reserved	[50];
}ST_TR_88888_S;

///////////////////////////////////////////////////////////////////////////
//	담보금 출금 화면: X1TR20013 :  화면번호 (3001) , 출금

typedef struct _ST_TR_30013_I
{
	char	Proc_Tp				[1];			//	Q : 조회, U : 신청
	char	UserID				[MAX_USERID];	//	사용자ID
	char	AcntNo				[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd				[ 4];			//	계좌 비밀번호
	char	Req_Money			[15];			//	신청금액. 신청시 입력
	char	Req_Money_KRW		[15];			//	신청금액. 신청시 입력
	char	Forex				[7];			//	환율
}ST_TR_30013_I;

typedef struct _ST_TR_30013_O
{
	char	User_Bank_Nm		[20];	//	고객이 받을 은행 명
	char	User_Bank_Acnt_No	[20];	//	고객이 받을 은행 계좌
	char	User_Bank_User_Nm	[20];	//	고객 계좌 예금주
	char	User_CanOut_Entr	[15];	//	출금가능금액	
	char	Forex_Buy			[7];	//	외화매입환율(외화입금)
	char	Forex_Sell			[7];	//	외화매도환율(외화출금)
	char	Success_Yn			[ 1];	//	출금신청 성공 구분 (0:성공 , 1:실패)

}ST_TR_30013_O;


///////////////////////////////////////////////////////////////////////////
//	구좌수 조회 및 변경 화면: X1TR20014 :  화면번호 (3001) , 출금

typedef struct _ST_TR_20014_I
{
	char	Proc_Tp				[1];			//	Q : 조회, U : 신청
	char	GDS_CODE			[2];
	char	AcntNo				[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd				[ 4];			//	계좌 비밀번호
	char	GUJA_CNT			[4];			//	신청구좌수. 신청시 입력
	char	SITE				[5];			//	X1 OR BLASH
}ST_TR_20014_I;

typedef struct _ST_TR_20014_O
{
	char	GUJA_CNT			[ 4];	//	현재 구좌수
	char	CLTL				[ 11];	//	현재 담보금
	char	MAX_GUJA_CNT		[4];	//	가능 구좌수
	char	CAN_GUJA_CNT		[4];	//	잔액대비 가능 구좌수
}ST_TR_20014_O;

///////////////////////////////////////////////////////////////////////////
//	체결내역 조회 화면: X1TR20017 :  화면번호 (1201)

typedef struct _ST_TR_20017_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd		[ 4];			//	계좌 비밀번호
	char	StkCode		[32];			//	종목코드
	char	StartDt		[ 8];			//	조회 시작일
	char	EndDt		[ 8];			//	조회 끝일
	char	NextKey		[17];			//	다음 조회키
}ST_TR_20017_I;

typedef struct _ST_TR_20017_O
{
	char	CntrNo		[MAX_ORD_NO];	//	체결번호
	char	CntrDt		[ 8];			//	체결일자
	char	StkCode		[MAX_STK_CODE];	//	종목코드
	char	BuySell_Tp	[ 1];			//	주문구분 (1.매도, 2.매수)
	char	CntrQty		[10];			//	체결수량
	char	OrdPrice	[MAX_ORD_PRC];	//	주문가격
	char	CntrPrice	[MAX_ORD_PRC];	//	체결가격
	char	CntrTime	[ 8];			//	체결시간 (hh:mm:ss)
	char	ClrPl		[15];			//	손익
	char	OrdSt		[ 1];			//	주문구분 (1. 시장가 , 2. 지정가)
	char	OrdNO		[MAX_ORD_NO];	//	체결된 주문의 주문번호
	char	CMSN		[5];			//	수수료
}ST_TR_20017_O;


///////////////////////////////////////////////////////////////////////////
//	예탁금 조회: X1TR20018 :  화면번호 (2001)  , X1TR20019 : 화면번호 (2002)

// 국내(KOSPI200) 예탁금 조회 I/O
typedef struct _ST_TR_20018_I
{
	char	AcntNo			[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd			[ 4];			//	계좌 비밀번호
}ST_TR_20018_I;

typedef struct _ST_TR_20018_O
{
	char	OpenTotAmt		[15];	//	개장시 예탁총액
	char	RlzPnlAmt		[15];	//	실현손익
	char	CurrentTotAmt	[15];	//	현재 예탁총액
	char	EvalPnlAmt		[15];	//	평가손익
	char	EvalTotAmt		[15];	//	평가 예탁총액
	char	LossCutAmt		[15];	//	로스컷 설정금액
	char	AcntStatCode	[ 8];	//	계좌상태
	char	OrdAbleAmt		[15];	//	주문가능금액
	char	OpenPldgAmt		[15];	//	개장시 담보금액
	char	Cmsn			[15];	//	수수료
	char	Utlfree			[15];	//	이용료
	char	CurrentPldgAmt	[15];	//	현재 담보금액
	char	EvalPldgAmt		[15];	//	평가 담보금액
	char	LossCutFreeAmt	[15];	//	로스컷 여유금액
	char	FcurrMgn		[15];	//	위탁증거금
	char	OutAbleAmt		[15];	//	출금가능금액
}ST_TR_20018_O;

// 해외(FUFX) 예탁금 조회 I/O
typedef struct _ST_TR_20019_I
{
	char	AcntNo			[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd			[ 4];			//	계좌 비밀번호
}ST_TR_20019_I;

typedef struct _ST_TR_20019_O
{
	char	OpenTotAmt_USD		[15];	//	개장시 예탁총액 (USD)
	char	RlzPnlAmt_USD		[15];	//	실현손익 (USD)
	char	CurrentTotAmt_USD	[15];	//	현재 예탁총액 (USD)
//	char	RlzPnlAmt_KRW		[15];	//	실현손익 (KRW)
	char	Xchrat				[ 8];	//	환율
	char	EvalPnlAmt_USD		[15];	//	평가손익 (USD)
	char	EvalTotAmt_USD		[15];	//	평가 예탁총액 (USD)
	char	LossCutAmt			[15];	//	로스컷 설정금액
	char	AcntStatCode		[ 2];	//	계좌상태
	char	OrdAbleAmt_USD		[15];	//	주문가능금액 (USD)
	char	OpenPldgAmt_USD		[15];	//	개장시 담보금액 (USD)
//	char	OpenPldgAmt_KRW		[15];	//	개장시 담보금액 (KRW)
	char	Cmsn_USD			[15];	//	수수료 (USD)
	char	Utlfree				[15];	//	이용료
//	char	Cmsn_KRW			[15];	//	수수료 (KRW)
	char	CurrentPldgAmt		[15];	//	현재 담보금액
//	char	EvalPldgAmt_KRW		[15];	//	평가 손익 (KRW)
	char	EvalPldgAmt			[15];	//	평가 담보금액
	char	LossCutFreeAmt		[15];	//	로스컷 여유금액
	char	FcurrMgn_USD		[15];	//	위탁증거금 (USD)
//	char	OutAbleAmt_KRW		[15];	//	출금가능금액 (KRW)
}ST_TR_20019_O;


///////////////////////////////////////////////////////////////////////////
//	정산 손익분석 조회 화면: X1TR20020 :  화면번호 (2201)

typedef struct _ST_TR_20020_I
{
	char	GDS_CODE	[2];
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	AcntPwd		[ 4];			//	계좌 비밀번호
	char	StartDt		[ 8];			//	조회 시작일
	char	EndDt		[ 8];			//	조회 끝일
	char	NEXT_KEY	[85];
}ST_TR_20020_I;

// typedef struct _ST_TR_20020_O
// {
// 	char	OpenTotAmt		[15];	//	예탁총액
// 	char	RlzPnlSumAmt	[15];	//	실현손익 누계
// 	char	FutsRlzPnlAmt	[15];	//	선물 실현손익
// 	char	OptRlzPnlAmt	[15];	//	옵션 실현손익
// 	char	EvalPlnSum		[15];	//	평가손익 합계
// 	char	FutsEvalPnlAmt	[15];	//	선물 평가손익
// 	char	OptEvalPnlAmt	[15];	//	옵션 평가손익
// 	char	TotSumErn		[15];	//	총수익 합계
// 	char	FutsCtrctAmt	[15];	//	선물 약정금액 누계
// 	char	OptCtrctAmt		[15];	//	옵션 약정금액 누계
// 	char	CmsnSumAmt		[10];	//	수수료 누계
// 	char	FutsCmsn		[10];	//	선물 수수료
// 	char	OptCmsn			[10];	//	옵션 수수료
// 	char	NetPnlSumAmt	[15];	//	순 손익합계
// }ST_TR_20020_O;
typedef struct _ST_TR_20020_O
{	
	char	DT		[8];
	char	CLTL	[15];
	char	IN_AMT	[15];	//	입금액
	char	OUT_AMT	[15];	//	출금액
	char	CLR_PL	[15];
	char	CMSN	[15];
	char	ACML_PL	[15];	//	누적손익
	char	SEQ_NO	[8];
}ST_TR_20020_O;



//////////////////////////////////////////////////////////////////////////
//	수익률 조회
typedef struct _ST_TR_20030_I
{

	char	GDS_CODE	[2];
	char	ACNT_NO		[11];
	char	DT_FROM		[8];
	char	DT_TO		[8];
}ST_TR_20030_I;


#ifdef _WISEMULTI
typedef struct _ST_TR_20030_O
{
	char	DT			[8];
	char	IN_AMT		[15];
	char	OUT_AMT		[15];
	char	CLR_PL_KS	[15];	// 	국내손익
	char	CMSN_KS		[15];	//	국내수수료
	char	CLR_PL_EUR	[15];	//	유로손익
	char	CMSN_EUR	[15];	//	해외수수료
	char	CLR_PL		[15];	// 	실현손익
	char	ACCM_PL		[15];	//	누적손익
	char	CMSN_NQ		[15];	//	NQ수수료
	char	CLR_PL_NQ	[15];	//	NQ손익
	char	PL_RATE		[7];	//	XXX.XXX %
	//char	ACCM_RATE	[7];	//	XXX.XXX %
	char	ENTR_RT		[3];
	char	CNTR_QTY	[5];
	char	ESTM_CLTL	[15];	//	전일담보금 + 실현손익
	
}ST_TR_20030_O;
#else
typedef struct _ST_TR_20030_O
{
	char	DT			[8];
	char	IN_AMT		[15];
	char	OUT_AMT		[15];
	char	CLR_PL_F	[15];	// 	국내손익
	char	CMSN_F		[15];	//	국내수수료
	char	CLR_PL_O	[15];	//	유로손익
	char	CMSN_O		[15];	//	해외수수료
	char	CLR_PL		[15];	// 	실현손익
	char	ACCM_PL		[15];	//	나스닥손익
	char	START_CLTL	[15];	//	전일자 담보금
	char	ESTM_CLTL	[15];	//	전일담보금 + 실현손익
	char	PL_RATE		[7];	//	XXX.XXX %
	//char	ACCM_RATE	[7];	//	XXX.XXX %
	char	ENTR_RT		[3];
	char	CNTR_QTY	[5];
	
}ST_TR_20030_O;
#endif

//////////////////////////////////////////////////////////////////////////
//	추천인 수수료 조회
typedef struct _ST_TR_20040_I
{	
	char	GDS_CODE	[2];
	char	ACNT_NO		[11];
	char	DT_FROM		[8];
	char	DT_TO		[8];
	char	Gb			[1];
	char	SEL_IDX		[1];  	//  0:전체, 1:이름, 2:필명
    char	SEL_VAL		[10];
}ST_TR_20040_I;

typedef struct _ST_TR_20040_O
{
	char	Gb		[1];
	char	DT		[8];
	char	UserNM	[20];
	char	NickNM	[20];
	char	CMSN_F	[15];
	char	CMSN_O	[15];
	char	CMSN_SUM[15];
	char	CMSN_Back[15];
	char	RSLT	[1]; // Y:SUCCESS, N:FAIL
    char	MSG     [50]; // 에러시 메세지
	
}ST_TR_20040_O;

//////////////////////////////////////////////////////////////////////////
//	추천인 수수료 조회 2
typedef struct _ST_TR_20041_I
{
	char	ACNT_NO		[11];
	char	DT_FROM		[8];
	char	DT_TO		[8];
	char	Gb			[1];
	char	SEL_IDX		[1];  	//  0:전체, 1:이름, 2:필명
    char	SEL_VAL		[10];
}ST_TR_20041_I;

typedef struct _ST_TR_20041_O
{
	char	Gb			[1];
	char	DT			[8];
	char	UserNM		[20];
	char	NickNM		[20];
	char	TrdeDTCnt	[5];
	char	CondYN		[1];
	char	RSLT		[1]; // Y:SUCCESS, N:FAIL
    //char	MSG			[50]; // 에러시 메세지
	
}ST_TR_20041_O;


//////////////////////////////////////////////////////////////////////////
//	HI-PRO 추천인 수수료 조회
typedef struct _ST_TR_20042_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[11];	//	추천인 계좌번호
	char	DT_FROM		[8];	//	조회일
	char	DT_TO		[8];	
	char	Gb			[1];
	char	SEL_IDX		[1];  	
    char	SEL_VAL		[10];
}ST_TR_20042_I;

typedef struct _ST_TR_20042_O
{
	char	Gb		[1];
	char	DT		[8];
	char	UserNM	[20];
	char	NickNM	[20];
	char	CMSN_F1	[15];
	char	CMSN_O1	[15];
	char	CMSN_F2	[15];
	char	CMSN_O2	[15];
	char	CMSN_SUM[15];
	char	CMSN_Back[15];
	char	RSLT	[1]; // Y:SUCCESS, N:FAIL
    char	MSG     [50]; // 에러시 메세지
	
}ST_TR_20042_O;

///////////////////////////////////////////////////////////////////////////
//	KOSPI 200 선물/옵션 현재가 조회: X1TR21003 :  화면번호 (Real Packet TR)

typedef struct _ST_TR_21003_I			// 국내 KS
{
	char	GdsCode		[ 2];			//	상품코드 (00:KS선물, 01:KS옵션, 02:해외선물, 03:FX마진)
	char	StkCode		[12];			//	종목코드 (패킷의 크기 문제로 MAX_STK_CODE = 32 사용하지 않는다.)
}ST_TR_21003_I;

typedef struct _ST_TR_21003_O
{
	char	StkCode				[12];	//	종목코드
	char	GdsCode				[2];	// 	상품코드 (00:KS선물, 01:KS옵션, 02:해외선물, 03:FX마진)
	char	tm					[6];
	char	now_prc				[7];	//	xxxx.xx
	char	open				[7];	//	2	
	char	high				[7];	//	2	
	char	low					[7];	//	2	
	char	acml_cntr_qty		[7];	//	0	누적체결수량
	char	acml_buy_cntr_qty	[7];	//	0	누적매수체결량
	char	acml_amt			[15];	//	0	누적거래대금
	char	daebi_sign			[1];	//	0	전일대비부호
	char	chg					[7];	//	2	전일대비
	char	chg_rate			[7];	//	2	등락율
	char	cntr_qty			[6];	//	0	체결수량
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	매도잔량
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	총매도호가잔량
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	총매도건수
	char	o_hoga_cnt_chg_1	[4];	//	0	매도호가건수대비
	char	o_hoga_cnt_chg_2	[4];	//	0	
	char	o_hoga_cnt_chg_3	[4];	//	0	
	char	o_hoga_cnt_chg_4	[4];	//	0	
	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	매수잔량
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		총매수호가잔량
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	총매수건수
	char	b_hoga_cnt_chg_1	[4];	//	0	
	char	b_hoga_cnt_chg_2	[4];	//	0	
	char	b_hoga_cnt_chg_3	[4];	//	0	
	char	b_hoga_cnt_chg_4	[4];	//	0	
	char	b_hoga_cnt_chg_5	[4];	//	0	
	char	ks200				[7];	//	2	
	char	hoga_trmnq_chg		[7];	//	0	호가총잔량차이
	char	qty_chg				[9];	//		거래량증감
	char	amt_chg				[9];	//		거래대금증감
	char	theory_prc			[7];	//	2	이론가
	char	open_yak			[7];	//	0	미결제약정수량(부호)-옵션만부호있음
	char	open_yak_chg		[6];	//		미결제약정수량대비
	char	gyuri_rate			[7];	//	2	괴리율
	char	basis				[7];	//	2	시장베이시스
	char	open_chg			[7];	//	2	시가대비
	char	high_chg			[7];	//	2	고가대비
	char	low_chg				[7];	//	2	저가대비
	char	theory_basis		[7];	//	2	이론베이시스
	char	imp_v				[7];	//		내재변동성 - OPTION ONLY
	char	delta				[7];	//		델타 - OPTION ONLY
	char	gmma				[7];	//		감마 - OPTION ONLY
	char	veta				[7];	//		베타 - OPTION ONLY
	char	theta				[7];	//		세타 - OPTION ONLY
	char	rho					[7];	//		로 - OPTION ONLY
	char	in_val				[6];	//		내재가치 - OPTION ONLY
	char	tm_value			[6];	//		시간가치 - OPTION ONLY
	char	cntr_net_buy		[9];	//		체결순매수 - OPTION ONLY
	char	rmnq_gap			[7];	//		잔량차(매수잔량-매도잔량) - OPTION ONLY
	char	cnt_gap				[5];	//		건수차(매수건수-매도건수) - OPTION ONLY
}ST_TR_21003_O;


///////////////////////////////////////////////////////////////////////////
//	KOSPI 200 선물/옵션 호가 조회: X1TR21103 :  화면번호 (Real Packet TR)

typedef struct _ST_TR_21103_I
{
	char	GdsCode		[ 2];			//	상품코드 (00:KS선물, 01:KS옵션, 02:해외선물, 03:FX마진)
	char	StkCode		[12];			//	종목코드
}ST_TR_21103_I;

typedef struct _ST_TR_21103_O
{
	char	StkCode				[32];	//	종목코드
	char	GdsCode				[2];	// 	상품코드 (00:KS선물, 01:KS옵션, 02:해외선물, 03:FX마진)
	char	tm					[6];
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	매도잔량
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	총매도호가잔량
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	총매도건수
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	매수잔량
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		총매수호가잔량
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	총매수건수
}ST_TR_21103_O;


///////////////////////////////////////////////////////////////////////////
//	 차트조회 화면: X1TR35001 :  화면번호 (5001) (1분,2분,3분,5분,10분,30분,60분,일,주,월 DATA)

typedef struct _ST_TR_35001_I
{
	char	DataGb		[ 4];			//	차트데이터 구분 (1분,2분,3분,5분,10분,30분,60분,일,주,월)
	char	GdsCode		[ 2];			//	상품구분
	char	StkCode		[MAX_STK_CODE];	//	종목코드
	char	StartDt		[ 8];			//	조회 시작일
	char	EndDt		[ 8];			//	조회 끝일
}ST_TR_35001_I;

typedef struct _ST_TR_35001_O
{
	char	Cnt			[ 4];			//	데이터 카운트
	char	DT			[ 8];			//	일자
	char	TM			[ 6];			//	시간
	char	Open		[15];			//	시가
	char	High		[15];			//	고가
	char	Low			[15];			//	저가
	char	Close		[15];			//	종가
}ST_TR_35001_O;


///////////////////////////////////////////////////////////////////////////
//	 TICK 차트조회 화면: X1TR35003 :  화면번호 (????)

typedef struct _ST_TR_35003_I
{
	char	SeqNo		[ 4];			//	Sequence 번호
	char	GdsCode		[ 2];			//	상품구분
	char	StkCode		[MAX_STK_CODE];	//	종목코드
}ST_TR_35003_I;

typedef struct _ST_TR_35003_O
{
	char	Cnt			[ 4];			//	데이터 카운트
	char	TM			[ 6];			//	시간
	char	Close		[15];			//	종가
}ST_TR_35003_O;


///////////////////////////////////////////////////////////////////////////
//	공지사항 화면: X1TR41001 , X1TR41002 :  화면번호 (6201)

// X1TR41001 : 제목내용 I/O
typedef struct _ST_TR_41001_I
{
	char		StanDate[8];	//	공지일자 (YYYYMMDD)
	char		UrgentTp[1];	//	긴급구분 (0.전체조회, 1.공지, 2.시황, 3.긴급)
	char		Seq		[20];	//	시퀀스번호 (0. 전체, 그외 Number 로 조회)
	char		BtnGb	[1];	//	다음버튼 구분 (0. 없음 , 1. 있음)
}ST_TR_41001_I;


typedef struct _ST_TR_41001_O
{
	char		StanDate[8];	//	공지일자 (YYYYMMDD)
	char		StanTime[8];	//	공지시간 (hh:mm:ss)
	char		Seq		[20];	//	요청시퀀스번호
	char		Title	[80];	//	공지제목
}ST_TR_41001_O;

// X1TR41002 : 본문내용 I/O 
typedef struct _ST_TR_41002_I
{
	char		Seq		[20];	//	요청시퀀스번호
	char		StanDate[8];	//	공지일자 (YYYYMMDD)
}ST_TR_41002_I;


typedef struct _ST_TR_41002_O
{
	//char		Body	[800]; //	공지내용
	char		Body	[1500]; //	공지내용
}ST_TR_41002_O;


//////////////////////////////////////////////////////////////////////////
//	고객정보 조회 화면: X1TR51001 : 조회 / 변경  , 화면번호 (5001)

typedef struct _ST_TR_51001_I 
{
	char		TrxTp		[ 1];			//	고객정보 조회(Q) / 변경(U) 구분코드
	char		UserID		[MAX_USERID];	//	고객ID
	char		UserNm		[20];			//	고객명
//	char		UserPNm		[20];			//	고객필명
	char		Email		[50];			//	E-mail 주소
	char		HpNo		[20];			//	핸드폰 번호
//	char		TelNo		[20];			//	전화번호
	char		BankNM		[20];
	char		BankAcntNo	[20];
	char		BankUserNM	[20];
}ST_TR_51001_I;

typedef struct _ST_TR_51001_O
{
	char		UserID		[MAX_USERID];	//	고객ID
	char		UserNm		[20];			//	고객명
//	char		UserPNm		[20];			//	고객필명
	char		Email		[50];			//	E-mail 주소
	char		HpNo		[20];			//	핸드폰 번호
//	char		TelNo		[20];			//	전화번호
	char		BankNM		[20];
	char		BankAcntNo	[20];
	char		BankUserNM	[20];
	char		UserNO		[5];			//	고객번호
}ST_TR_51001_O;

//////////////////////////////////////////////////////////////////////////
//	지수맞추기 이벤트 (SMILOR. INDEXEVENT)
typedef struct _ST_TR_55001_I
{
	char	TR			[5];
	char	ACNT_NO		[11];
}ST_TR_55001_I;

typedef struct _ST_TR_55001_O
{
	char	TR			[5];
	char	OPEN		[10];
	char	HIGH		[10];
	char	LOW			[10];
	char	CLOSE		[10];
	char	OPEN_MY		[10];
	char	HIGH_MY		[10];
	char	LOW_MY		[10];
	char	CLOSE_MY	[10];
	char	OPEN_YN		[1];
	char	HIGH_YN		[1];
	char	LOW_YN		[1];
	char	CLOSE_YN	[1];
	char	OPEN_PRIZE	[8];
	char	HIGH_PRIZE	[8];
	char	LOW_PRIZE	[8];
	char	CLOSE_PRIZE	[8];	
}ST_TR_55001_O;

typedef struct _ST_TR_55002_I
{
	char	TR			[5];
	char	ACNT_NO		[11];
	char	OPEN		[10];
	char	HIGH		[10];
	char	LOW			[10];
	char	CLOSE		[10];
}ST_TR_55002_I;

typedef struct _ST_TR_55002_O
{
	char	TR			[5];
	char	SUCC_YN		[1];
	char	MSG			[100];
}ST_TR_55002_O;





//////////////////////////////////////////////////////////////////////////
//	비밀번호 변경 화면: X1TR51002 : 변경  , 화면번호 (5101)

typedef struct _ST_TR_51002_I 
{
	char		PwdGb		[ 2];			//	패스워드 변경 구분코드 (CU:접속비번 , AU:계좌비번)
	char		UserID		[MAX_USERID];	//	고객ID
	char		AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char		BeforePwd	[10];			//	이전 비밀번호
	char		AfterPwd	[10];			//	변경 후 비밀번호
	char		CheckPwd	[10];			//	변경 비밀번호 확인
}ST_TR_51002_I;

typedef struct _ST_TR_51002_O
{
	char		Success_YN	[ 1];			//	비밀번호 변경 성공 구분 (0:성공 , 1:실패)
}ST_TR_51002_O;


//////////////////////////////////////////////////////////////////////////
//	오버나잇 변경 화면: X1TR53001 : 조회(Q),신청(U),취소(D)  , 화면번호 (5301)

typedef struct _ST_TR_53001_I 
{
	char		TrxTp		[ 1];			//	오버나잇 변경구분 코드 (Q:조회, U:신청, D:취소)
	char		UserID		[MAX_USERID];	//	고객ID
	char		AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char		AcntPwd		[4];			//	계좌 비밀번호
}ST_TR_53001_I;

typedef struct _ST_TR_53001_O
{
//	char		Success_Yn	[ 1];			//	변경 성공여부 코드 (Y:성공 , N:실패)
	char		OverStat_Yn	[ 1];			//	오버나잇 상태코드 (Y:오버나잇 가능 , N:오버나잇 불가)
}ST_TR_53001_O;


//////////////////////////////////////////////////////////////////////////
//	미체결주문조회  , 화면번호 (1102)

typedef struct _ST_TR_11001_I 
{
	char		ACNT_NO		[MAX_ACNT_NO];	//	계좌번호
	char		ACNT_PWD	[4];			//	계좌 비밀번호
	char		GDS_CODE	[2];
	char		STK_CODE	[MAX_STK_CODE];
}ST_TR_11001_I;

typedef struct _ST_TR_11001_O
{
	char	ACNT_NO		[MAX_ACNT_NO];
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	ORD_NO		[MAX_ORD_NO];
	char	ORG_NO		[MAX_ORD_NO];
	char	BUY_SELL_TP	[1];
	char	ORD_PRC		[MAX_ORD_PRC];
	char	ORD_QTY		[MAX_ORD_QTY];
	char	REMN_QTY	[MAX_ORD_QTY];
	char	ORD_ST		[1];
	char	MNG_USERID	[MAX_USERID];

}ST_TR_11001_O;


//////////////////////////////////////////////////////////////////////////
//	잔고조회(스피드주문창)
typedef struct _ST_TR_20021_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	STK_CODE	[MAX_STK_CODE];
	char	CRNC_CODE	[3];
}ST_TR_20021_I;

typedef struct _ST_TR_20021_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	CLTL		[15];
	char	AVG_PRC		[MAX_ORD_PRC];	
	char	ESTM_PL		[15];	//	평가손익
	char	CLR_PL		[15];	//	실현손익
	char	TOT_PL		[15];
	char	POS_QTY		[9];
	char	BUY_SELL_TP	[1];
	char	TICK_SIZE	[15];
	char	TICK_VALUE	[15];
	char	TOT_ESTM_ENTR	[15];
	char	ESTM_ENTR		[15];
	char	TOT_REMNQ_SELL	[4];
	char	TOT_REMNQ_BUY	[4];
	char	LOSS_CUT_AMT	[15];
	char	Leverage		[3];
}ST_TR_20021_O;



//////////////////////////////////////////////////////////////////////////
//	미체결잔량조회(스피드주문창)
typedef struct _ST_TR_20022_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20022_I;

typedef struct _ST_TR_20022_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	PRC			[7];
	char	SELL_REMNQ	[4];
	char	BUY_REMNQ	[4];
}ST_TR_20022_O;



//////////////////////////////////////////////////////////////////////////
//	주문가능수량 조회 (스피드주문창)
typedef struct _ST_TR_20031_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20031_I;

typedef struct _ST_TR_20031_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_PRC		[MAX_ORD_PRC];
	char	ABLE_SELL	[4];
	char	ABLE_BUY	[4];
}ST_TR_20031_O;


typedef struct _ST_TR_20031_O_FX
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_PRC_S	[MAX_ORD_PRC];
	char	NOW_PRC_B	[MAX_ORD_PRC];
	char	ABLE_SELL	[4];
	char	ABLE_BUY	[4];
}ST_TR_20031_O_FX;


//////////////////////////////////////////////////////////////////////////
//	시고저종 등 조회 (스피드주문창)
typedef struct _ST_TR_20032_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20032_I;

typedef struct _ST_TR_20032_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW			[MAX_ORD_PRC];
	char	CHG			[7];	//	7,2
	char	CHG_RATE	[7];	//	7,2
	char	KS200		[MAX_ORD_PRC];
	//char	BS			[MAX_ORD_PRC];
	char	LAST_END_PRC[MAX_ORD_PRC];
	char	OPEN		[MAX_ORD_PRC];
	char	HIGH		[MAX_ORD_PRC];
	char	LOW			[MAX_ORD_PRC];
	char	OPEN_YAK	[9];	//	미결
	char	OPEN_YAK_CHG[9];	//	미결증감
}ST_TR_20032_O;


typedef struct _ST_TR_20032_O_FX
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_S		[MAX_ORD_PRC];
	char	NOW_B		[MAX_ORD_PRC];
	char	CHG			[7];	//	7,2
	char	CHG_RATE	[7];	//	7,2
	char	KS200		[MAX_ORD_PRC];
	//char	BS			[MAX_ORD_PRC];
	char	LAST_END_PRC[MAX_ORD_PRC];
	char	OPEN		[MAX_ORD_PRC];
	char	HIGH		[MAX_ORD_PRC];
	char	LOW			[MAX_ORD_PRC];
	char	OPEN_YAK	[9];	//	미결
	char	OPEN_YAK_CHG[9];	//	미결증감
}ST_TR_20032_O_FX;


//////////////////////////////////////////////////////////////////////////
//	시세 조회 (스피드주문창-30건)
typedef struct _ST_TR_20033_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20033_I;

typedef struct _ST_TR_20033_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	PRC			[20];
	char	QTY			[4];
}ST_TR_20033_O;


//////////////////////////////////////////////////////////////////////////
//	KS200 FUTURES 호가범위 (스피드주문창)
typedef struct _ST_TR_20034_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20034_I;

typedef struct _ST_TR_20034_O
{
	char	PRC			[7];
}ST_TR_20034_O;

//////////////////////////////////////////////////////////////////////////
//	KS200 FUTURES 호가범위 (스피드주문창)
typedef struct _ST_TR_20037_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20037_I;

typedef struct _ST_TR_20037_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	MAX			[7];
	char	MIN			[7];
	char	CNT			[4];
}ST_TR_20037_O;

//////////////////////////////////////////////////////////////////////////
//	KS200 OPTIONS 호가범위 (스피드주문창)
typedef struct _ST_TR_20035_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20035_I;

typedef struct _ST_TR_20035_O
{
	char	PRC			[7];
}ST_TR_20035_O;

//////////////////////////////////////////////////////////////////////////
//	KS200 OPTIONS 호가범위 (스피드주문창)
typedef struct _ST_TR_20038_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20038_I;

typedef struct _ST_TR_20038_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	MAX			[7];
	char	MIN			[7];
	char	CNT			[4];
}ST_TR_20038_O;


//////////////////////////////////////////////////////////////////////////
//	KS200 전종목 잔고/미체결수량
typedef struct _ST_TR_20039_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
}ST_TR_20039_I;

typedef struct _ST_TR_20039_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	BUY_SELL_TP	[1];
	char	CNT			[4];
	char	AVG_PRC		[7];
	char	BUY_NCNTR	[4];
	char	SELL_NCNTR	[4];
	char	NOW_PRC		[15];
	char	CLR_PL		[15];
}ST_TR_20039_O;



//////////////////////////////////////////////////////////////////////////
//	FU 호가범위 (스피드주문창)
typedef struct _ST_TR_20036_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20036_I;

typedef struct _ST_TR_20036_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	MAX			[20];
	char	MIN			[20];
	char	CNT			[4];
	//char	PRC			[10];
}ST_TR_20036_O;



//////////////////////////////////////////////////////////////////////////
//	사용신청
typedef struct _ST_TR_52001_I
{
	char	GDS_CODE	[2];
	char	ACTION		[1];			//	'Q':조회, 'A':신청, 'C':취소
	char	ACNT_NO		[MAX_ACNT_NO];
	char	ACNT_PWD	[4];
}ST_TR_52001_I;

typedef struct _ST_TR_52001_O
{
	char	GDS_CODE	[2];
	char	ACTION		[1];			//	'Q':조회, 'A':신청, 'C':취소
	char	CLTL		[15];			//	담보금액
	char	DAY_CNT		[3];			//	잔여일수
	char	ACNT_STAT	[10];			//	거래가능 OR 거래불가능
}ST_TR_52001_O;



//////////////////////////////////////////////////////////////////////////
//	특정계좌 수익률 조회(시스템 신호 연동1호)
typedef struct _ST_TR_32001_I
{
	char	GROUP_IDX	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_32001_I;

typedef struct _ST_TR_32001_O
{
	char	GROUP_IDX	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	POS_QTY		[15];
	char	AVG_PRC		[MAX_ORD_PRC];
	char	EVLT_PL		[15];
	char	CLR_PL		[15];
	char	TOT_PL		[15];
}ST_TR_32001_O;

/*
	CLIENT 가 시세를 받을 종목 등록
*/
typedef struct _ST_TR_20090_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	RegFlag		[1];			//	0:수신, 1:해제
}ST_TR_20090_I;


/*
	레버리지 조회 및 변경
*/
typedef struct _ST_TR_60014_I
{
	char	PROC_TP		[1];	//	Q:조회, U:수정
	char	GDS_CODE	[2];
	char	ACNT_NO		[11];
	char	ACNT_PWD	[4];
    char	ENTR_RT		[3];	//	1,5,10,20,100
}ST_TR_60014_I;

typedef struct _ST_TR_60014_O
{
	char	Leverage	[3];	//	1,5,10,20,100
}ST_TR_60014_O;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/
//																											//
//					관리자용 COMPONENT(컴포넌트) 패킷 정의 (정렬순서 : MN 번호순)							//
//																											//
/************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	관리자 로그인 PACKET

// X1MN90001 : 로그인 PACKET
typedef struct _ST_MN_90001_I
{
	char	STX				[ 1];
			C_HEADER		Header;
			TR_HEADER		TRHeader;
	char	ID				[MAX_USERID];	//	접속 관리자 ID
	char	Conn_Pwd		[10];			//	접속 관리자 비밀번호
	char	MN_IP_Local		[15];			//	접속 관리자 IP (Local)
	char	MN_IP_Public	[15];			//	접속 관리자 IP (Public)
	char	MN_Mac			[12];			//	접속 관리자 MAC 주소
	char	ETX				[ 1];
}ST_MN_90001_I;

typedef struct _ST_MN_90001_O
{
	char	STX				[ 1];
			C_HEADER		Header;
			TR_HEADER		TRHeader;
	char	ManagerCd		[ 5];			//	접속 관리자 코드 (형식 : XX-XX , 예: 00-00, 01-01)
	char	SuccYn			[ 1];			//	성공 여부 (Y:성공, N:실패)
	char	ETX				[ 1];
}ST_MN_90001_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	일자별 수수료 전체 조회 관리자 화면: X1MN92001

// X1MN92001 : 제목내용 I/O
typedef struct _ST_MN_92001_I
{
	char	ManagerCd	[ 5];	//	관리자코드 (형식 : 00-00)
	char	StartDt		[ 8];	//	조회 시작일자 (YYYYMMDD)
	char	EndDt		[ 8];	//	조회 끝 일자
	char	NextKey		[ 8];	//	다음 조회 (NextKey='0' 은 금일조회, 과거일자 NextKey='YYYYMMDD' 의 형식)
}ST_MN_92001_I;


typedef struct _ST_MN_92001_O
{
	char	Dt			[ 8];	//	일별 조회일자 (YYYYMMDD)
	char	SumEntr		[15];	//	일별 잔고 합계
	char	SumCltl		[15];	//	일별 입출금 합계
	char	SumClrPl	[15];	//	일별 실현손익 합계
	char	SumCmsn		[15];	//	일별 수수료 합계
	char	SumFee		[15];	//	일별 이용료 합계
}ST_MN_92001_O;


// X1MN92002 : 개인별 수수료 조회 (관리자용) I/O 
typedef struct _ST_MN_92002_I
{
	char	ManagerCd	[ 5];			//	관리자코드	(형식: 00-00)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	StartDt		[ 8];			//	조회 시작일자 (YYYYMMDD)
	char	EndDt		[ 8];			//	조회 끝 일자
	char	NextKey		[ 24];			//	다음조회: 회원번호+계좌번호+날짜 ("T000101-01-0000020100831")
}ST_MN_92002_I;


typedef struct _ST_MN_92002_O
{
	char	ActiveYn	[ 1];			//	로그인 상태 (Y:접속 , N:비접속)
	char	UserNo		[ 5];			//	회원번호
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	UserID		[MAX_USERID];	//	회원 ID
	char	UserNm		[20];			//	회원이름
	char	Dt			[ 8];			//	날짜
	char	Entr		[15];			//	잔고
	char	ClrPl		[15];			//	실현손익
	char	Cmsn		[15];			//	수수료
	char	Fee			[15];			//	이용료
}ST_MN_92002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 입금 요청 정보 조회 

// X1MN93000 : 회원 입/출금 승인 일자별 내역 정보 조회 I/O
typedef struct _ST_MN_93000_I
{
	char	ManagerCd	[ 5];			//	관리자코드	(형식: 00-00)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	StartDt		[ 8];			//	조회 시작일 (형식: 20100822)
	char	EndDt		[ 8];			//	조회 끝일	(형식: 20100828)
	char	NextKey		[20];			//	다음조회키	(형식: 2010082315:23:20:145 , 20자리) '0' 일때는 전체조회
}ST_MN_93000_I;

typedef struct _ST_MN_93000_O
{
	char	Dt			[ 8];			//	승인처리 날짜 (형식: 20100826)
	char	UserNo		[ 5];			//	회원 번호 (형식: A0001)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX)
	char	UserID		[MAX_USERID];	//	회원 ID
	char	UserNm		[20];			//	회원 이름
	char	ReqCode		[ 2];			//	요청 코드 (입금: 01 , 출금: 02)
	char	ReqDt		[ 8];			//	요청 날짜
	char	ReqTm		[12];			//	요청 시간
	char	ReqMoney	[15];			//	요청 입/출액
	char	ProcDt		[ 8];			//	승인 날짜
	char	ProcTm		[12];			//	승인 시간
	char	ProcMoney	[15];			//	승인 입/출금액
}ST_MN_93000_O;

// X1MN93001 : 회원 입금 요청 정보 조회 I/O
typedef struct _ST_MN_93001_I
{
	char	ManagerCd	[ 5];			//	관리자코드 (형식 : 00-00)
}ST_MN_93001_I;

typedef struct _ST_MN_93001_O
{
	char	UserNo		[ 5];			//	회원 번호 (A0001)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호
	char	UserNm		[20];			//	회원 이름
	char	Dt			[ 8];			//	요청 날짜
	char	Tm			[12];			//	요청 시간
	char	ReqMoney	[15];			//	요청 입금액
}ST_MN_93001_O;

// X1MN93002 : 회원 입금 요청 ==> 관리자 입금 승인 처리 I/O (관리자용) 
typedef struct _ST_MN_93002_I
{
	char	ManagerCd	[ 5];			//	관리자코드 (형식 : '00-00' , 5자리 )
	char	ProcTp		[ 1];			//	처리구분 ( 승인: 'U' , 취소: 'C' )
	char	DtTm		[20];			//	입금 요청날짜 + 입금 요청시간 (형식: 'YYYYMMDD15:26:25.124' , 20자리)
	char	AcntNo		[MAX_ACNT_NO];	//	입금될 계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	ReqMoney	[15];			//	입금 요청 금액
	char	ReqGuJaCnt	[15];			//	요청 구좌수
}ST_MN_93002_I;

typedef struct _ST_MN_93002_O
{
	char	SuccYn		[ 1];			//	성공여부 반환 (Y:성공 , N:실패)
}ST_MN_93002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 출금 요청 정보 조회 

// X1MN93003 : 회원 출금 요청 정보 조회 I/O
typedef struct _ST_MN_93003_I
{
	char		ManagerCd		[ 5];	//	관리자코드 (형식 : 00-00)
}ST_MN_93003_I;

typedef struct _ST_MN_93003_O
{
	char	UserNo			[ 5];			//	회원 번호 (A0001)
	char	AcntNo			[MAX_ACNT_NO];	//	계좌번호
	char	UserNm			[20];			//	회원 이름
	char	Dt				[ 8];			//	요청 날짜
	char	Tm				[12];			//	요청 시간
	char	ReqMoney		[15];			//	요청 출금액
	char	UserBankNm		[20];			//	은행명
	char	UserBankAcntNo	[20];			//	은행 계좌번호
	char	UserBankDpstNm	[20];			//	예금주
}ST_MN_93003_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 신상정보 조회 / 변경 (수정) 화면: X1MN94001(조회) , X1MN94002(변경) :  화면번호 (9401)

// X1MN94001 : 회원 신상정보 정보 조회 I/O (관리자용)
typedef struct _ST_MN_94001_I
{
	char		ManagerCd	[ 5];			//	관리자코드 (형식 : 00-00)
	char		NextKey		[16];			//	다음조회 키값 (회원번호+계좌번호)
}ST_MN_94001_I;

typedef struct _ST_MN_94001_O
{
	char	UserNo		[ 5];			//	회원번호
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	UserID		[MAX_USERID];	//	회원 ID
	char	ConnPwd		[10];			//	접속 비밀번호
	char	AcntPwd		[ 4];			//	계좌 비밀전호
	char	HpNo		[20];			//	핸드폰 번호
	char	TelNo		[20];			//	전화번호
	char	UserNm		[20];			//	회원 이름
	char	UserBankNm	[20];			//	사용자 은행명
	char	UserAcntNo	[20];			//	사용자 계좌번호
	char	Email		[50];			//	사용자 Email
}ST_MN_94001_O;


// X1MN94002 : 회원 신상정보 정보 변경 I/O (관리자용) 
typedef struct _ST_MN_94002_I
{
	char	UserNo		[ 5];			//	회원번호
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	UserID		[MAX_USERID];	//	회원 ID
	char	ConnPwd		[10];			//	접속 비밀번호
	char	AcntPwd		[ 4];			//	계좌 비밀전호
	char	HpNo		[20];			//	핸드폰 번호
	char	TelNo		[20];			//	전화번호
	char	UserNm		[20];			//	회원 이름
	char	UserBankNm	[20];			//	사용자 은행명
	char	UserAcntNo	[20];			//	사용자 계좌번호
	char	Email		[50];			//	사용자 Email
}ST_MN_94002_I;

typedef struct _ST_MN_94002_O
{
	char	SuccYn		[ 1];			//성공여부 반환 (Y:성공 , N:실패)
}ST_MN_94002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 거래 설정 정보 조회 / 변경 화면: X1MN94003(설정 내역 조회) , X1MN94004(설정변경) :  화면번호 (9401)

// X1MN94003 : 회원 거래 설정 정보 조회 I/O (관리자용)
typedef struct _ST_MN_94003_I
{
	char	ManagerCd	[ 5];			//	관리자코드 (형식 : 00-00)
}ST_MN_94003_I;

typedef struct _ST_MN_94003_O
{
	char	UserNo		[ 5];			//	회원 번호
	char	AcntNo		[MAX_ACNT_NO];	//	계좌 번호 (형식: XX-XX-XXXXX , 11자리)
	char	UserNm		[20];			//	회원 이름
	char	ConnSysNm	[20];			//	접속 시스템 (실전, 모의)
	char	ApplyRt		[10];			//	비율 적용
	char	MaxQty		[15];			//	최대 가능 수량
	char	CmsnRtF		[15];			//	선물 수수료
	char	CmsnRtO		[15];			//	옵션 수수료
	char	CmsnAmt		[15];			//	유로 선물 수수료 금액
	char	GujaCnt		[15];			//	구좌수
	char	UserStat	[ 1];			//	사용자 상태코드 (로그인 가능: 4, 제한: 5)
}ST_MN_94003_O;


// X1MN94004 : 회원 거래 설정 정보 변경 I/O (관리자용) 
typedef struct _ST_MN_94004_I
{
	char	ManageCd	[ 5];			//	관리자 코드 (형식: 00-00, 5자리)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌 번호 (형식: XX-XX-XXXXX , 11자리)
	char	ConnSysNm	[20];			//	접속 시스템 (실전, 모의)
	char	ApplyRt		[10];			//	비율 적용
	char	MaxQty		[15];			//	최대 가능 수량
	char	CmsnRtF		[15];			//	선물 수수료
	char	CmsnRtO		[15];			//	옵션 수수료
	char	CmsnAmt		[15];			//	유로 선물 수수료 금액
	char	GujaCnt		[15];			//	구좌수
	char	UserStat	[ 1];			//	사용자 상태코드 (로그인 가능: 4, 제한: 5)
}ST_MN_94004_I;

typedef struct _ST_MN_94004_O
{
	char		SuccYn		[ 1];			//성공여부 반환 (Y:성공 , N:실패)
}ST_MN_94004_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	공지사항 화면: X1MN96001(제목) , X1MN96011(본문), X1MN96002(저장/발송) :  화면번호 (9601)

// X1MN96001 : 제목내용 I/O
typedef struct _ST_MN_96001_I
{
	char	StanDate	[ 8];	//	공지일자 (YYYYMMDD)
	char	UrgentTp	[ 1];	//	긴급구분 (0.전체조회, 1.공지, 2.시황, 3.긴급)
	char	Seq			[20];	//	시퀀스번호 (0. 전체, 그외 Number 로 조회)
	char	BtnGb		[ 1];	//	다음버튼 구분 (0. 없음 , 1. 있음)
}ST_MN_96001_I;

typedef struct _ST_MN_96001_O
{
	char	StanDate	[ 8];	//	공지일자 (YYYYMMDD)
	char	StanTime	[ 8];	//	공지시간 (hh:mm:ss)
	char	Seq			[20];	//	요청시퀀스번호
	char	Title		[80];	//	공지제목
	char	SendYn		[ 1];	//	발송완료 여부 (Y:발송완료, N:발송대기) --2010.08.16 추가
}ST_MN_96001_O;


// X1MN96002 : 관리자 공지 저장/발송 I/O 
typedef struct _ST_MN_96002_I
{
	char	UrgentTp	[  1];	//	긴급구분 (1.공지, 2.시황, 3.긴급)
	//char	UserID		[MAX_USERID];	//SMILOR. 개별공지전송
	char	Title		[ 80];	//	공지제목
	//char	Body		[800]; //	공지내용
	char	Body		[1500]; //	공지내용
}ST_MN_96002_I;

typedef struct _ST_MN_96002_O
{
	char	SuccYn	[  1];	//성공여부 반환 (Y:성공 , N:실패)
}ST_MN_96002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	관리자정보 조회 / 변경 (추가/수정/삭제) 화면: X1MN97001(조회) , X1MN97002(변경) :  화면번호 (9701)

// X1MN97001 : 관리자 정보 조회 I/O
typedef struct _ST_MN_97001_I
{
	char		ManagerCd	[ 5];	//	관리자코드 (형식 : 00-00)
}ST_MN_97001_I;

typedef struct _ST_MN_97001_O
{
	char	ManagerCd	[ 5];	//	관리자코드 (형식 : XX-XX , 예: 00-00)
	char	ID			[12];	//	관리자 ID
	char	ConnPwd		[10];	//	관리자 접속 비밀번호
	char	BankNm		[50];	//	관리자 은행명
	char	AcntDpNm	[20];	//	관리자 예금주
	char	AcntNo		[20];	//	관리자 계좌번호
	char	AcntPwd		[ 4];	//	관리자 계좌 비밀번호
	char	IP			[15];	//	관리자 아이피
	char	Phone		[20];	//	관리자 전화
	char	Email		[50];	//	관리자 Email
}ST_MN_97001_O;


// X1MN97002 : 관리자 정보 변경 I/O 
typedef struct _ST_MN_97002_I
{
	char	ProcGb			[ 1];	//	처리 구분코드 (N:추가, C:수정, D:삭제)
	char	ConnManagerCd	[ 5];	//	접속 관리자코드 (형식 : XX-XX 예: 00-00)
	char	ManagerCd		[ 5];	//	처리대상 관리자코드 (형식 : XX-XX 예: 00-00)
	char	ID				[12];	//	관리자 ID
	char	ConnPwd			[10];	//	관리자 접속 비밀번호
	char	BankNm			[50];	//	관리자 은행명
	char	AcntDpNm		[20];	//	관리자 예금주
	char	AcntNo			[20];	//	관리자 계좌번호
	char	AcntPwd			[ 4];	//	관리자 계좌 비밀번호
	char	IP				[15];	//	관리자 아이피
	char	Phone			[20];	//	관리자 전화
	char	Email			[50];	//	관리자 Email
}ST_MN_97002_I;

typedef struct _ST_MN_97002_O
{
	char		SuccYn		[ 1];	//성공여부 반환 (Y:성공 , N:실패)
}ST_MN_97002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	장운영 관련 설정 관리자 조회 / 변경 (수정) 화면: X1MN99001(조회) , X1MN99002(변경) :  화면번호 (9901)

// X1MN99001 : 관리자 정보 조회 I/O
typedef struct _ST_MN_99001_I
{
	char		ManagerCd	[ 5];	//	관리자코드 (형식 : 00-00)
}ST_MN_99001_I;

typedef struct _ST_MN_99001_O
{
	char	TrdeDt_Ks		[8];	//	KS 현재 영업일
	char	BatchStatus_Ks	[1];	//	KS 현재 일배치 상태
	char	ManageGb_Ks		[1];	//	KS 현재 장상태 (0:장개시전, 1:장개시, 2:장마감, 3:장중지)
	char	StartTm_Ks		[5];	//	KS 현재 장개시 시간
	char	EndTm_Mtr_KsF	[5];	//	KS 현재 선물 만기일의 장마감 시간
	char	EndTm_Mtr_KsO	[5];	//	KS 현재 옵션 만기일의 장마감 시간
	char	EndTm_KsF		[5];	//	KS 현재 선물 일반 장마감 시간
	char	EndTm_KsO		[5];	//	KS 현재 옵션 일반 장마감 시간
	char	TrdeDt_FuFx		[8];	//	FUFX 현재 영업일
	char	BatchStatus_FuFx[1];	//	FUFX 현재 일배치 상태
	char	ManageGb_FuFx	[1];	//	FUFX 현재 장상태 (0:장개시전, 1:장개시, 2:장마감, 3:장중지)
	char	StartTm_FuFx	[5];	//	FUFX 현재 장개시 시간
	char	EndTm_FuFx		[5];	//	FUFX 현재 장마감 시간
}ST_MN_99001_O;


// X1MN99002 : 관리자 정보 변경 I/O 
typedef struct _ST_MN_99002_I
{
	char	ManagerCd		[5];	//	관리자코드 (형식 : 00-00)
	char	ManageGb_Ks		[1];	//	KS 현재 장상태 (0:장개시전, 1:장개시, 2:장마감, 3:장중지)
	char	StartTm_Ks		[5];	//	KS 현재 장개시 시간
	char	EndTm_Mtr_KsF	[5];	//	KS 현재 선물 만기일의 장마감 시간
	char	EndTm_Mtr_KsO	[5];	//	KS 현재 옵션 만기일의 장마감 시간
	char	EndTm_KsF		[5];	//	KS 현재 선물 일반 장마감 시간
	char	EndTm_KsO		[5];	//	KS 현재 옵션 일반 장마감 시간
	char	ManageGb_FuFx	[1];	//	FUFX 현재 장상태 (0:장개시전, 1:장개시, 2:장마감, 3:장중지)
	char	StartTm_FuFx	[5];	//	FUFX 현재 장개시 시간
	char	EndTm_FuFx		[5];	//	FUFX 현재 장마감 시간
}ST_MN_99002_I;

typedef struct _ST_MN_99002_O
{
	char		SuccYn		[ 1];	//성공여부 반환 (Y:성공 , N:실패)
}ST_MN_99002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원정보 검색 화면: X1MN70100 (공통으로 사용)

// X1MN70100 : 회원 검색하여 해당회원의 계좌번호 출력 조회 I/O (관리자용)
typedef struct _ST_MN_70100_I
{
	char		ProcTp	[1];			//	처리구분 ('0':ALL , '1': ID검색 , '2': 이름검색)
	char		UserID	[MAX_USERID];	//	회원 ID
	char		UserNm	[20];			//	회원이름
}ST_MN_70100_I;

typedef struct _ST_MN_70100_O
{
	char		AcntNo	[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
}ST_MN_70100_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 신상정보 조회 / 변경 (수정) 화면: X1MN70101(조회) , X1MN70102(변경)

// X1MN70101 : 회원 신상정보 정보 조회 I/O (관리자용)
typedef struct _ST_MN_70101_I
{
	char	AdminID		[MAX_USERID];	//	관리자ID
//	char	GdsCode		[ 2];			//	GDS_CODE
	char	UserNm		[20];			//	회원이름
	char	NextKey		[11];			//	다음조회 키값 (회원번호+계좌번호)
}ST_MN_70101_I;

typedef struct _ST_MN_70101_O
{
	char	UserID		[MAX_USERID];	//	회원 ID
	char	UserNm		[20];			//	회원이름
	char	ConnPwd		[10];			//	접속 비밀번호
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	AcntPwd		[ 4];			//	계좌 비밀전호
	char	UserBankNm	[20];			//	사용자 은행명
	char	UserAcntNo	[20];			//	사용자 계좌번호
	char	HpNo		[20];			//	핸드폰 번호
	char	Email		[50];			//	사용자 Email
	char	AdminID		[MAX_USERID];	//	관리자ID
	char	AdminNm		[20];			//	관리자명
}ST_MN_70101_O;


// X1MN70102 : 회원 신상정보 정보 변경 I/O (관리자용) 
typedef struct _ST_MN_70102_I
{
	char	ProcTp		[ 1];			//	처리구분 (I: 회원추가, U: 회원정보 수정, D: 회원삭제)
	char	UserID		[MAX_USERID];	//	회원 ID
	char	UserNm		[20];			//	회원이름
	char	ConnPwd		[10];			//	접속 비밀번호
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	AcntPwd		[ 4];			//	계좌 비밀전호
	char	UserBankNm	[20];			//	사용자 은행명
	char	UserAcntNo	[20];			//	사용자 계좌번호
	char	HpNo		[20];			//	핸드폰 번호
	char	Email		[50];			//	사용자 Email
	char	AdminID		[MAX_USERID];	//	관리자ID
}ST_MN_70102_I;

typedef struct _ST_MN_70102_O
{
	char		SuccYn	[ 1];			//성공여부 반환 (Y:성공 , N:실패)
}ST_MN_70102_O;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 신상정보 조회 / 변경 (수정) 화면: X1MN90101(조회) , X1MN90102(변경)

// X1MN70101 : 회원 신상정보 정보 조회 I/O (관리자용)
typedef struct _ST_MN_90101_I
{
	char	AdminID		[MAX_USERID];	//	관리자ID
	char	GdsCode		[ 2];			//	GDS_CODE
	char	SelCond		[1];			//	조회 조건 (0:전체, 1:고객명, 2:고객ID, 3:계좌번호, 4:고객번호)
	char	SelVal		[20];			//	
	char	NextKey		[11];
}ST_MN_90101_I;

typedef struct _ST_MN_90101_O
{
	char	UserID[MAX_USERID];
	char	UserNm[20];
	char	ConnPwd[10];
	char	AcntNo[MAX_ACNT_NO];
	char	AcntPwd[4];
	char	UserBankNm[20];
	char	UserAcntNo[20];
	char	HpNo[20];
	char	TelNo[20];
	char	Email[50];
	char	AdminID[12];
	char	AdminNm[20];
	char	USER_NO[5];
	char	REG_NO[13];
	char	FIRST_TRDE_DT[8];
	char	PARTNER_ID[MAX_USERID];
	char	MAX_GUJA[4];
	char	CHG_RT_F[10];
	char	CHG_RT_O[10];
	char	USER_STAT[1];
	char	API_CODE[12];
	char	LEVERAGE[3];
	char	GRADE[1];
	char	Nick_NM[20];
}ST_MN_90101_O;

typedef struct _ST_MN_90102_I
{
	char	UserID[MAX_USERID];
	char	UserNm[20];
	char	ConnPwd[10];
	char	AcntNo[MAX_ACNT_NO];
	char	AcntPwd[4];
	char	UserBankNm[20];
	char	UserAcntNo[20];
	char	HpNo[20];
	char	TelNo[20];
	char	Email[50];
	char	AdminID[12];
	char	AdminNm[20];
	char	USER_NO[5];
	char	REG_NO[13];
	char	FIRST_TRDE_DT[8];
	char	PARTNER_ID[MAX_USERID];
	char	MAX_GUJA[4];
	char	CHG_RT_F[10];
	char	CHG_RT_O[10];
	char	USER_STAT[1];
	char	API_CODE[12];
	char	LEVERAGE[3];
	char	ACTION[1];	//	U:UPD, I:INS
	char	GRADE [1];
	char	NICK_NM	[20];
	char	BLACK_REASON[100];
}ST_MN_90102_I;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 계좌정보 조회 / 변경 (수정) 화면: X1MN70103(조회) , X1MN70104(변경)

// X1MN70103 : 회원 계좌정보 조회 I/O (관리자용)
typedef struct _ST_MN_70103_I
{
	char	AdminID		[MAX_USERID];	//	관리자ID
//	char	GdsCode		[ 2];			//	GDS_CODE
	char	UserID		[MAX_USERID];	//	회원ID
	char	NextKey		[11];			//	다음조회 키값 (계좌번호)
}ST_MN_70103_I;

typedef struct _ST_MN_70103_O
{
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	AcntNm		[20];			//	계좌명
	char	UserID		[MAX_USERID];	//	회원 ID
	char	Entr		[15];			//	금일잔고
	char	Cltl		[15];			//	담보금
	char	GujaCnt		[15];			//	구좌수
	char	RemnDay		[15];			//	잔여일 (남은일수)
	char	AcntStat	[ 1];			//	계좌상태 (1:거래가능, 2:마진콜, 3:거래불가, 9:사용 미신청)
	char	ConnSysNm	[20];			//	접속서버
	char	CmsnRt_F	[15];			//	선물 수수료율
	char	CmsnRt_O	[15];			//	옵션 수수료율
	char	RegDate		[ 8];			//	등록일 (형식: YYYYMMDD , 예: 20100928)
}ST_MN_70103_O;


// X1MN70104 : 회원 계좌정보 변경 I/O (관리자용) 
typedef struct _ST_MN_70104_I
{
	char	ProcTp		[ 1];			//	처리구분 (I: 회원추가, U: 회원정보 수정, D: 회원삭제)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)	--변경불가
	char	AcntNm		[20];			//	계좌명									--변경불가
	char	UserID		[MAX_USERID];	//	회원 ID									--변경불가
	char	Entr		[15];			//	금일잔고								--변경불가
	char	Cltl		[15];			//	담보금									--변경불가
	char	GujaCnt		[15];			//	구좌수
	char	RemnDay		[15];			//	잔여일 (남은일수)
	char	AcntStat	[ 1];			//	계좌상태 (1:거래가능, 2:마진콜, 3:거래불가, 9:사용 미신청)
	char	ConnSysNm	[20];			//	접속서버
	char	CmsnRt_F	[15];			//	선물 수수료율
	char	CmsnRt_O	[15];			//	옵션 수수료율
	char	RegDate		[ 8];			//	등록일 (형식: YYYYMMDD , 예: 20100928)
}ST_MN_70104_I;

typedef struct _ST_MN_70104_O
{
	char	SuccYn		[ 1];			//성공여부 반환 (Y:성공 , N:실패)
}ST_MN_70104_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	회원 계좌 잔고/매매 내역 정보 조회: X1MN70105(잔고 조회) , X1MN70107(매매내역 조회)

// X1MN70105 : 계좌 일자별 잔고내역 (관리자용)
typedef struct _ST_MN_70105_I
{
	char	GdsCode		[MAX_GDS_CODE];	//	국내/해외 구분코드 (국내:00 or 01 , 해외: 02)
	char	StartDt		[8];			//	조회 시작일자 (형식: YYYYMMDD, 예: 20100928)
	char	EndDt		[8];			//	조회 끝 일자 (형식: YYYYMMDD, 예: 20100928)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	NextKey		[8];			//	다음조회 - 일자 (형식: YYYYMMDD, 예: 20100928)
}ST_MN_70105_I;

typedef struct _ST_MN_70105_O
{
	char	Dt			[ 8];			//	날짜 (형식: YYYYMMDD, 예: 20100928)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	PrevCltl	[15];			//	전일잔고
	char	InCltl		[15];			//	금일입금 담보금
	char	OutCltl		[15];			//	금일출금 담보금
	char	ClrPl		[15];			//	거래손익
	char	Cmsn		[15];			//	수수료
	char	Fee			[15];			//	이용료
	char	Cltl		[15];			//	당일 잔고
	char	CrncCode	[ 3];			//	통화코드
}ST_MN_70105_O;

// X1MN70106 : 회원전체 잔고내역 조회 (관리자용)
typedef struct _ST_MN_70106_I
{
	char	ProcTp		[ 1];			//	조회구분 (0: 전체조회, 1: 체크잔고, 2: 체크손익)
	char	GdsCode		[MAX_GDS_CODE];	//	국내/해외 구분코드 (국내선물:00 , 국내옵션: 01 , 해외: 02)
	char	StanDate	[ 8];			//	조회 일자 (형식: YYYYMMDD, 예: 20100928)
	char	NextKey		[MAX_ACNT_NO];	//	다음조회 키 (키: 계좌번호 , 형식: XX-XX-XXXXX , 11자리)
	char	MiniYN		[1];
}ST_MN_70106_I;

typedef struct _ST_MN_70106_O
{
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	UserNm		[20];			//	회원이름
	char	PrevCltl	[15];			//	전일잔고
	char	Cltl		[15];			//	금일잔고 (당일잔고)
	char	InCltl		[15];			//	금일입금 담보금
	char	OutCltl		[15];			//	금일출금 담보금
	char	SumClrPl	[15];			//	총손익
	char	ClrPl		[15];			//	거래손익
	char	Cmsn		[15];			//	수수료
	char	Fee			[15];			//	이용료
	char	PL_F		[15];
	char	PL_O		[15];
	char	CMSN_F		[15];
	char	CMSN_O		[15];

}ST_MN_70106_O;

// X1MN70107 : 일자별 매매내역 조회 조회 I/O (관리자용)
typedef struct _ST_MN_70107_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	GdsCode		[MAX_GDS_CODE];	//	국내/해외 구분코드 (국내:00 or 01 , 해외: 02)
	char	StkCode		[MAX_STK_CODE];	//	회원ID
	char	StartDt		[ 8];			//	조회 시작일자 (형식: YYYYMMDD, 예: 20100928)
	char	EndDt		[ 8];			//	조회 끝 일자 (형식: YYYYMMDD, 예: 20100928)
	char	NextKey		[20];			//	다음조회 키값 (체결일자+ 체결번호?) : TODO : ?????
}ST_MN_70107_I;

typedef struct _ST_MN_70107_O
{
	char	CntrDt		[ 8];			//	체결일자 (형식: YYYYMMDD, 예: 20100928)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	StkCode		[MAX_STK_CODE];	//	종목정보
	char	BuySellTp	[ 1];			//	매매구분 (1:매도 , 2:매수)
	char	OrdPrice	[15];			//	주문가격
	char	OrdQty		[15];			//	주문수량
	char	OrdTp		[ 1];			//	주문형태 (1:시장가 , 2:지정가)
	char	OrdTime		[12];			//	주문시간 (형식: hh:mm:ss:xxx, 예: 15:23:45:211)
	char	CntrPrice	[15];			//	체결가격
	char	CntrQty		[15];			//	체결수량
	char	CntrTime	[12];			//	체결시간 (형식: hh:mm:ss:xxx, 예: 15:23:45:211)
	char	ClrPl		[15];			//	손익
	char	Cmsn		[15];			//	수수료
}ST_MN_70107_O;

// X1MN70108 : 회원전체 체결내역 조회 (관리자용)
typedef struct _ST_MN_70108_I
{
	char	GdsCode		[MAX_GDS_CODE];	//	국내/해외 구분코드 (국내선물:00 , 국내옵션: 01 , 해외: 02)
	char	StanDate	[ 8];			//	조회 일자 (형식: YYYYMMDD, 예: 20100928)
	char	NextKey		[MAX_ACNT_NO];	//	다음조회 키 (키: 계좌번호 , 형식: XX-XX-XXXXX , 11자리)
	char	MiniYN		[1];
}ST_MN_70108_I;

typedef struct _ST_MN_70108_O
{
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	UserNm		[20];			//	회원이름
	char	Apply_Rt	[15];			//	적용비율
	char	CntrQty_F	[15];			//	선물 체결 수량
 	char	AVG_PRC_F	[20];			//	평균환율
// 	char	ESTM_PL_F	[15];			//	평가손익
	char	UnCntrQty_F	[15];			//	선물 미체결 수량
	char	CntrQty_C	[15];			//	콜 체결 수량
	char	UnCntrQty_C	[15];			//	콜 미체결 수량
	char	CntrQty_P	[15];			//	풋 체결 수량
	char	UnCntrQty_P	[15];			//	풋 미체결 수량
}ST_MN_70108_O;

// X1MN70118 : 개인회원 상세 체결내역 조회 (관리자용)
typedef struct _ST_MN_70118_I
{
	char	GDS_CODE	[2];
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX , 11자리)
	char	NextKey		[MAX_STK_CODE];	//	다음조회 키 (키: 종목코드)
}ST_MN_70118_I;

typedef struct _ST_MN_70118_O
{
	char	StkCode		[MAX_STK_CODE];	//	종목코드
	char	Apply_Rt	[15];			//	적용비율
	char	UnCntrQty	[15];			//	미체결 수량
	char	CntrQty		[15];			//	체결 수량
	char	AvgCntrPrc	[15];			//	체결 평균가
	char	NowPrc		[15];			//	현재가격
	char	EvalPnlAmt	[15];			//	평가손익
}ST_MN_70118_O;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	X1 회원입금 / 출금 요청 조회

// X1MN70301 : 회원 입금 / 출금 요청 조회 (관리자용)
typedef struct _ST_MN_70301_I
{
	char	ACTION		[ 1];	//	I:입금, O:출금
	char	GDS_CODE	[ 2];
	char	MNG_ID		[MAX_USERID];
}ST_MN_70301_I;

typedef struct _ST_MN_70301_O
{
	char	SEQNO			[10];
	char	ACTION			[ 1];		//	I:입금, O:출금
	char	GDS_CODE		[ 2];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	NAME			[20];
	char	APPL_DT			[ 8];		//	신청일자
	char	APPL_TM			[ 8];		//	신청시각
	char	REQ_AMT			[10];		//	신청금액
	char	REQ_AMT_2		[10];		//	해외선물의 경우 원화금액
	char	FOREX			[7];
	char	APPL_DAY_CNT	[ 3];		//	(입금)사용신청일수
	char	APPL_GUJA_CNT	[ 3];		//	(입금)사용신청 구좌수
	char	OUT_BANK_NM		[10];		//	(출금)출금 은행명
	char	OUT_ACNT_NO		[20];		//	(출금)출금계좌번호
	char	ACNT_OWNER		[10];		//	(출금)예금주
}ST_MN_70301_O;


// X1MN70302 : 회원 입금 / 출금 요청 처리 (관리자용)
typedef struct _ST_MN_70302_I
{
	char	ACTION			[ 1];		//	I:입금, O:출금
	char	SEQNO			[10];
	char	GDS_CODE		[ 2];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	APPL_DAY_CNT	[ 3];		//	(입금)사용신청일수
	char	APPL_GUJA_CNT	[ 3];		//	(입금)사용신청 구좌수
	char	REQ_AMT			[10];		//	신청금액
	char	RESULT			[ 1];		//	A:승인, R:거절
	char	MNG_ID			[MAX_USERID];
}ST_MN_70302_I;

typedef struct _ST_MN_70302_O
{
	char	ACTION			[ 1];		//	I:입금, O:출금
	char	GDS_CODE		[ 2];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	RESULT			[ 1];		//	A:승인, R:거절
}ST_MN_70302_O;

// X1MN70303 : 회원 입/출금 승인 일자별 내역 정보 조회 I/O (관리자용)
typedef struct _ST_MN_70303_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 ('0' 일때는 전체 조회) 
	char	StartDt		[ 8];			//	조회 시작일 (형식: 20100822)
	char	EndDt		[ 8];			//	조회 끝일	(형식: 20100828)
	char	NextKey		[20];			//	다음조회키	(형식: 2010082315:23:20:145 , 20자리) '0' 일때는 전체조회
}ST_MN_70303_I;

typedef struct _ST_MN_70303_O
{
	char	Dt			[ 8];			//	승인처리 날짜 (형식: 20100826)
	char	AcntNo		[MAX_ACNT_NO];	//	계좌번호 (형식: XX-XX-XXXXX)
	char	UserID		[MAX_USERID];	//	회원 ID
	char	UserNm		[20];			//	회원 이름
	char	ReqCode		[ 2];			//	요청 코드 (입금: 01 , 출금: 02)
	char	ReqDt		[ 8];			//	요청 날짜
	char	ReqTm		[12];			//	요청 시간
	char	ReqMoney	[15];			//	요청 입/출액
	char	ResultYn	[ 1];			//	처리결과 (Y:처리완료 , N:미처리)
	char	ProcMoney	[15];			//	승인 입/출금액
	char	ProcDt		[ 8];			//	승인 날짜
	char	ProcTm		[12];			//	승인 시간
}ST_MN_70303_O;

//////////////////////////////////////////////////////////////////////////
//	실시간통보를 위해 X1MN84901 호출 (SMILOR.개별공지전송)
typedef struct _ST_MN_84901_I
{
	char	UserID		[MAX_USERID];
	char	AllYN		[1];
	char	Title		[128];
	char	Msg			[MAX_BUFSIZE];
}ST_MN_84901_I;

typedef struct _ST_MN_84901_O
{
	char Succ_YN	[1];
	char Reserved	[32];
}ST_MN_84901_O;


//////////////////////////////////////////////////////////////////////////
//	프리스타일의 관리자화면
/*
	Code
		- 10001 : 시세컨트롤
	
	SubCode	
		-	0011:국내ETK시작, 0012:국내ETK종료, 
			0021:해외하나대투시작, 0022:해외하나대투종료, 
			0031:해외솔로몬시작, 0032:해외솔로몬종료
*/
typedef struct _ST_MN_99999_I
{
	char	Code	[4];		//	1001 : 시세컨트롤, 1002 : 강제로그오프, 1003 : 새로고침
	char	SubCode	[4];		//	0011:국내시작, 0012:국내종료, 0021:해외하나대투시작,
	char	Body	[512];
}ST_MN_99999_I;


typedef struct _ST_MN_99999_O
{
	char	Code	[4];
	char	SubCode	[4];
	char	SuccYN	[1];
	char	Body	[512];
}ST_MN_99999_O;


// X1MN70303 : 회원 입금 / 출금 일자별 조회 (관리자용)
// typedef struct _ST_MN_70303_I
// {
// 	char	ACTION			[1];	//	A: ALL, I:입금, O:출금
// 	char	GDS_CODE		[2];
// 	char	STRT_DT			[ 8];	//	조회 시작일 (형식: 20100822)
// 	char	END_DT			[ 8];	//	조회 끝일	(형식: 20100828)
// 	char	NEXT_KEY		[10];	//	다음조회키	(SEQNO)
// 	char	MNG_ID			[MAX_USERID];
// }ST_MN_70303_I;
// 
// typedef struct _ST_MN_70303_O
// {
// 	char	SEQNO			[10];
// 	char	ACTION			[1];	//	I:입금, O:출금
// 	char	GDS_CODE		[2];
// 	char	ACNT_NO			[MAX_ACNT_NO];
// 	char	NAME			[20];
// 	char	APPL_DT			[8];	//	신청일자
// 	char	APPL_TM			[12];	//	신청시각
// 	char	REQ_AMT			[10];	//	신청금액
// //	char	APPL_DAY_CNT	[3];	//	사용신청일수
// //	char	APPL_GUJA_CNT	[3];	//	사용신청 구좌수
// //	char	CAN_OUT_AMT		[10];	//	출금가능금액 
// //	char	OUT_BANK_NM		[10];	//	출금 은행명
// //	char	OUT_ACNT_NO		[20];	//	출금계좌번호
// //	char	ACNT_OWNER		[10];	//	예금주
// 	char	RESULT			[1];	//	A:승인, R:거절
// }ST_MN_70303_O;


#endif
