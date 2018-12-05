#ifndef _COMMON_H_20100610
#define _COMMON_H_20100610

#include <stdio.h>


/************************************************************************
	CONFIG FILE
************************************************************************/
#define	CONFIG_NETWORK	"network.cnfg"
#define	CONFIG_MQ		"MQ.cnfg"
#define	CONFIG_COMMON	"CONFIG.cnfg"

/************************************************************************
	SISE RELAY CONFIG FILE
************************************************************************/
#define	SISE_RELAY_CNFG	"D:\\MGN_LEND\\SiseRelay"		//SiseRelay_1.ini 의 형태이다.


/************************************************************************
	COMPONENT 명명을 위한 DEFINE
************************************************************************/
#define PROGID_HEADER		"X1"
#define PROGID_HEADER_MN	"MN"	//	관리자화면

/************************************************************************
	TR 연속조회(이전/다음 관련) 구분자
************************************************************************/
#define NEXTGB_CLIENT_FIRST	'0'	//	CLIENT 가 전송할 때 처음버튼 누른 경우. 
#define NEXTGB_CLIENT_PREV	'1'	//	CLIENT 가 전송할 때 이전버튼 누른 경우. 
#define NEXTGB_CLIENT_NEXT	'2'	//	CLIENT 가 전송할 때 다음버튼 누른 경우. 

#define NEXTGB_SVR_EXIST_PREVNEXT	'2'	//	다음/이전 레코드 있음
#define NEXTGB_SVR_NOEXIST_PREVNEXT	'3'	//	다음/이전 레코드 없음


/************************************************************************
	API KEY DEFINE
************************************************************************/
#define APIKEY_KS_DONGBU		"KS_DONGBU"
#define APIKEY_KS_MINI			"KS_MINI"
#define APIKEY_KS_HANADAETOO	"KS_HANADT"
#define APIKEY_KS_ETK			"KS_ETK"
#define APIKEY_FU_HANADAETOO	"FU_HANADT"
#define APIKEY_FU_DONGYANG		"FU_DONGYANG"
#define APIKEY_FU_MINI			"FU_MINI"
#define APIKEY_FX_MINI			"FX_MINI"
#define APIKEY_FX_DAEWOO		"FX_DAEWOO"

/************************************************************************
	BUY_SELL_TP
************************************************************************/
#define BUY_SELL_TP_BUY		'2'
#define BUY_SELL_TP_SELL	'1'
#define BS_TP_BUY	'2'
#define BS_TP_SELL	'1'

/************************************************************************
	REGISTRY DEFINE
************************************************************************/
#define PROJ_NAME			"MGN_LEND_PCK"
#define	REG_ROOT			"Software\\MGN_LEND_PCK"
#define REG_ROOT_DB_INFO	"Software\\MGN_LEND_PCK\\DB_INFO"
#define REG_EVENTMSG_DLL	"Software\\MGN_LEND_PCK\\EventMsgDll"
#define REG_SMS				"Software\\MGN_LEND_PCK\\SMS"


/************************************************************************
	COMPONENT PROGID
************************************************************************/
#define PROGID_TR_LOGIN_REQ			"X1TR00013.TR00013.1"
#define PROGID_TR_LOGIN_COMMIT		"X1TR00014.TR00014.1"
#define PROGID_TR_LOGOFF			"X1TR00017.TR00017.1"
#define PROGID_OR_TRANS_KS			"X1OR21001.OR21001.1"
#define PROGID_OR_TRANS_FU			"X1OR22001.OR22001.1"
#define PROGID_OR_TRANS_FX			"X1OR23001.OR21001.1"

#define PROGID_TR_LOGIN_REQ_MNG		"X1MN90001.MN90001.1"
#define PROGID_TR_LOGOFF_MNG		"X1MN90002.MN90002.1"


/************************************************************************
	ERROR CODE
************************************************************************/
#define		SET_RET_STR( i_nCode, o_sCode )	sprintf_s( o_sCode, "%04d", i_nCode )

#define		RET_SUCCESS					0L	//	정상처리되었습니다.
#define		RET_COMPONENT_NOTEXIST		2L	//	컴포넌트가 설치되지 않았습니다. 관리자에게 문의하세요
#define		RET_NO_DATA					1401L	//	조회내역이 없습니다.
#define		RET_EXCEPTION				9998L	//	예외가 발생하였습니다.
#define		RET_UNDEFINED				9999L	//	정의되지 않은 오류입니다.

#define		RET_INVALID_PACKET			3L	//	유효하지 않은 패킷입니다.
#define		RET_FRONT_NOUSERINFO		4L	
#define		RET_DATADELAY				5L	//	처리가 지연되고 있습니다.
#define		RET_DBOPEN_ERR				6L	//	데이터베이스 연결 오류입니다.
#define		RET_DBEXEC_ERR				7L	//	데이터베이스 실행 중 에러
#define		RET_USER_NOTFOUND			8L	//	등록되지 않은 ID입니다.
#define		RET_CONN_PWD_ERR			9L	//	접속비밀번호 오류
#define		RET_CONN_ERR				10L	//	접속 중 오류
#define		RET_SVR_COMM_ERR			11L	//	서버간 통신 오류입니다.
#define		RET_SVR_INNER_ERR			12L	//	서버 내부 오류입니다.
#define		RET_WRONG_GDS_CODE			13L	//	상품코드 오류입니다.
#define		RET_COND_EXIST				14L	//	조건주문을 먼저 취소 해야 합니다.
#define		RET_API_DISCONNED			15L	//	증권사API 와 연결이 되어 있지 않습니다.
#define		RET_PASSED_ORD_VALID_TIME	16L	//	주문처리가 지연되어 거부합니다.
#define		RET_WRONG_ORD_ST			17L	//	잘못된 주문유형 입니다.
#define		RET_WRONG_BUY_SELL_TP		18L	//	잘못된 매도매수구분입니다.
#define		RET_WRONG_PRC				19L	//	잘못된 가격입니다.
#define		RET_MKT_RNG_OVER			20L	//	시장가범위가 최대값을 초과했습니다.
#define		RET_MKT_NOT_OPEN			21L	//	장개시가 되지 않았습니다.
#define		RET_NOT_ABLE_STK			22L	//	해당종목은 주문가능하지 않습니다.
#define		RET_HOGA_UNIT_ERR			23L	//	호가단위(Tick size)가 맞지 않습니다.
#define		RET_NOT_ABLE_USER			24L	//	해당 계좌는 주문불가 상태입니다.
#define		RET_ORG_ORD_NOEXIST			25L	//	원주문이 존재하지 않습니다.
#define		RET_ORD_NOACPT				26L	//	주문이 미접수상태 입니다.
#define		RET_ORD_RJCT				27L	//	주문이 거부되었습니다.
#define		RET_CNCLQTY_OVER			28L	//	일부청산시 원주문의 잔량을 초과할 수 없습니다.
#define		RET_NO_ORD_RMNQ				29L	//	미체결주문 잔량이 없습니다.
#define		RET_UNABLE_CNCL_MARKET		30L	//	시장가주문은 취소할 수 없습니다.
#define		RET_FAIL_CREATE_ORD_NO		31L	//	주문번호 생성에 실패했습니다.
#define		RET_MDFY_SAME_PRC			32L	//	같은 가격으로의 정정은 불가합니다.
#define		RET_WRONG_ORD_QTY			33L	//	주문수량이 잘못되었습니다.
#define		RET_MUST_CNCL_COND			34L	//	기존 Stop Loss 또는 Profit Taking 주문을 취소해야 합니다.
#define		RET_UNABLE_NEW_ORD			35L	//	신규주문 불가 계좌입니다.
#define		RET_BEYOND_LIMIT			36L	//	주문가능 최대수량 초과 입니다.
#define		RET_ORDPROC_ERROR			37L	//	주문 처리 중 오류입니다.
#define		RET_NO_ORG_NO				38L	//	원주문이 존재하지 않습니다.
#define		RET_MT_ORDPROC_ERROR		39L	//	체결서버에서 주문처리 중 오류가 발생
#define		RET_NO_REMNQ				40L	//	잔량이 존재하지 않습니다.
#define		RET_NO_REMN_DAY_1			41L	//	사용가능일이 없습니다. 이용료를 추가 납부 하세요.
#define		RET_NO_SISE					42L	//	시세가 없습니다.
#define		RET_CAN_ORD_QTY				43L	//	주문가능수량 초과 입니다.
#define		RET_LOSS_CUT_ACNT			44L	//	LOSS CUT 계좌입니다.
#define		RET_WRONG_ACNT_PWD			45L	//	계좌비밀번호 오류입니다.
#define		RET_API_SEND_ERROR			46L	//	증권사 전송 중 오류입니다.
#define		RET_MINI_SEND_ERROR			47L	//	모의 체결서버 전송 중 오류입니다.
#define		RET_WRONG_CONN_ORD			48L	//	잘못된 접속으로 접수된 주문입니다.
#define		RET_WRONG_ACNT_NO			49L	//	잘못된 계좌번호 입니다.
#define		RET_OPT_SELL_OVERNGT		50L	//	옵션매도는 오버나잇 불가합니다.
#define		RET_UNABLE_OVERNGT			51L	//	오버나잇 불가합니다.
#define		RET_CLTL_SHORT_OVERNGT		56L	//	오버나잇을 위한 담보금이 부족합니다.
#define		RET_NO_REMN_DAY				57L	//	잔여일이 남아 있지 않습니다.
#define		RET_ALREADY_TRADE_APPLY		58L	//	이미 사용신청 한 계좌입니다.
#define		RET_NO_TRADE_APPLY			59L	//	사용신청 하지 않은 계좌 입니다.
#define		RET_TRADE_APPLY_CNCL_TIMEOVER	60L	//	사용신청취소 가능한 시간이 지났습니다.
#define		RET_ALREADY_DONE			61L	//	이미 처리된 자료 입니다.
#define		RET_ALREADY_ORD				62L	//	이미 주문을 한 경우 사용취소 불가합니다.
#define		RET_NOT_REQ_TRADE			63L	//	사용신청을 먼저 해주세요
#define		RET_CHK_LIMIT_ERROR			64L	//	한도 체크 오류입니다.
#define		RET_ORD_INSERT_ERROR		65L	//	주문 접수 오류입니다.
#define		RET_SHORT_CLTL_4GUJA		66L	//	구좌 변경을 위한 담보금이 부족합니다.
#define		RET_NCNTR_NCLR_EXISTS		67L	//	구좌변경/출금신청을 위해서는 미체결주문 및 미청산잔고가 없어야 합니다.
#define		RET_OPT_PRC_RANGE_OUT		68L	//	0.5미만, 8이상 가격의 옵션은 거래 불가합니다.
#define		RET_ORD_Q_ERROR				69L	//	주문 큐처리 오류입니다.
#define		RET_CRNC_CODE_ERR			70L	//	통화코드 오류입니다.
#define		RET_OVER_CAN_OUT			71L	//	출금가능금액 초과 입니다.
#define		RET_MAX_GUJA_CNT_OVER		72L	//	최대 구좌수 초과 입니다.
#define		RET_API_MARGIN_SHORT		101L	//	증권사 증거금 부족
#define		RET_API_REJECT				102L	//	증권사가 거부하였습니다. 관리자에게 문의하세요
#define		RET_EXEC_LOGIC_ERR			103L	//	처리중 오류발생. 데이터를 확인해주세요

#define		RET_OPT_SELL_CLR_ONLY		108L	//	옵션매도는 청산용만 가능합니다.
#define		RET_EXEC_OPT_SELL_NCLR		109L	//	매도미체결과의 합이 매수잔고수량을 초과합니다.
#define		RET_MIT_SL_TRACE			120L	//	MIT, SL 주문 기록

/************************************************************************
	하나대투 증권사 에러코드
************************************************************************/
/*
	FF014	위탁증거금이 부족합니다
*/

/************************************************************************
	TR / REAL CODE
************************************************************************/
#define		SET_CODE_STR(i_code, o_code)	sprintf_s(o_code, "%05d", i_code)

#define	CODE_TR_UPGRADE					0L
#define	CODE_TR_POLLING					1L
#define	CODE_TR_LOGIN_REQUEST			13L
#define	CODE_TR_LOGIN_COMMIT			14L
#define	CODE_TR_ACNT_NO					15L
#define	CODE_TR_ACNT_PWD				16L
#define	CODE_TR_LOG_OFF					17L
#define	CODE_TR_RELOAD					18L
#define	CODE_TR_ORD						20000L
#define CODE_TR_REALTIME_REQUES			20090L
#define	CODE_TR_LOGIN_REQUEST_MNG		90001L
#define	CODE_TR_LOG_OFF_MNG				90002L

#define	CODE_REAL_NOTIFY				84901L
#define	CODE_REAL_SISE_KS				81003L	//	원래 국내 시세 패킷
#define	CODE_REAL_SISE_KS2				84003L	//	CLIENT 에게 전달하기 위해 길이를 줄인 국내 시세 패킷
#define	CODE_REAL_HOGA_KS				81103L
#define	CODE_REAL_SISE_FU				82003L
#define	CODE_REAL_HOGA_FU				82103L
#define	CODE_REAL_SISE_FX				83003L
#define	CODE_REAL_HOGA_FX				83103L
#define CODE_REAL_CNTR					82004L
#define CODE_REAL_JANGO					82005L

#define CODE_SVR_RSLT_NEW				9000L
#define CODE_SVR_RSLT_MDFY				9001L
#define CODE_SVR_RSLT_CNCL				9002L
#define CODE_SVR_RSLT_RJCT				9003L
#define CODE_SVR_RSLT_CNTR				9004L
#define CODE_SVR_RSLT_TRORD				9005L

#define	CODE_REAL_NOTIFY_S				"84901"
#define	CODE_REAL_SISE_KS_S				"81003"
#define	CODE_REAL_NOW_PRC_KS_S			"81004"
#define	CODE_REAL_HOGA_KS_S				"81103"
#define	CODE_REAL_SISE_FU_S				"82003"
#define	CODE_REAL_HOGA_FU_S				"82103"
#define	CODE_REAL_SISE_FX_S				"83003"
#define	CODE_REAL_HOGA_FX_S				"83103"
#define CODE_REAL_CNTR_S				"82004"
#define CODE_REAL_JANGO_S				"82005"


#define CODE_SVR_RSLT_NEW_S				"9000"
#define CODE_SVR_RSLT_MDFY_S			"9001"
#define CODE_SVR_RSLT_CNCL_S			"9002"
#define CODE_SVR_RSLT_RJCT_S			"9003"
#define CODE_SVR_RSLT_CNTR_S			"9004"
#define CODE_SVR_RSLT_TRORD_S			"9005"

// skeo 2011-01-25 [[
#define	CODE_TR_UPGRADE_RES_1				(1)
#define	CODE_TR_UPGRADE_RES_0				(0)
#define	CODE_TR_UPGRADE_RES_N1				(-1)
#define	CODE_TR_UPGRADE_RES_N2				(-2)
// ]]

/************************************************************************
	시세 데이터 CODE
************************************************************************/
#define SISEIDX_SISE_KS200		0
#define SISEIDX_SISE_FU			1		//	해외선물 시세

// #define SISE_KS200_F	1
// #define SISE_KS200_O	2
// #define SISE_FU			3		//	해외선물 시세
// #define SISE_FX			4


/************************************************************************
	MASTER FILE NAME
************************************************************************/
#define DEF_MASTER_KSF	"STK_KSF.MST"
#define DEF_MASTER_KSO	"STK_KSO.MST"
#define DEF_MASTER_FU	"STK_FU.MST"
#define DEF_MASTER_FX	"STK_FX.MST"
#define DEF_MASTER_MSG	"MsgCode.kor"
#define DEF_API_MASTER_KSF	"fmaster.dat"
#define DEF_API_MASTER_KSO	"pmaster.dat"


/************************************************************************
	KS200 선물, 옵션에 사용하는 연도, 월 기호
************************************************************************/
#define DEF_KS200_MON_10	"A"		//	10월
#define DEF_KS200_MON_11	"B"		//	11월
#define DEF_KS200_MON_12	"C"		//	12월

#define DEF_KS200_YEAR_10	"E"		//	2010년
#define DEF_KS200_YEAR_11	"F"		//	2011년
#define DEF_KS200_YEAR_12	"G"		//	2012년
#define DEF_KS200_YEAR_13	"H"		//	2013년
#define DEF_KS200_YEAR_14	"I"		//	2014년
#define DEF_KS200_YEAR_15	"J"		//	2015년
#define DEF_KS200_YEAR_16	"K"		//	2016년
#define DEF_KS200_YEAR_17	"L"		//	2017년
#define DEF_KS200_YEAR_18	"M"		//	2018년
#define DEF_KS200_YEAR_19	"N"		//	2019년
#define DEF_KS200_YEAR_20	"O"		//	2020년
#define DEF_KS200_YEAR_21	"P"		//	2021년
#define DEF_KS200_YEAR_22	"Q"		//	2022년
#define DEF_KS200_YEAR_23	"R"		//	2023년
#define DEF_KS200_YEAR_24	"S"		//	2024년
#define DEF_KS200_YEAR_25	"T"		//	2025년



/************************************************************************
	MAKE MQ NAME
************************************************************************/
#define SET_MQNAME(in,out)	sprintf_s(out, ".\\private$\\%s", in );


/************************************************************************
	GDS CODE (상품코드)
************************************************************************/
#define GDS_CODE_KSF	"00"
#define GDS_CODE_KSO	"01"
#define GDS_CODE_FU		"02"
#define GDS_CODE_FX		"03"


/************************************************************************
	MQ MSG TYPE
************************************************************************/
#define MQ_MSGTYPE_TR	"TR"
#define MQ_MSGTYPE_REAL	"RL"


/************************************************************************
	API KEY DEFINE
************************************************************************/
#define NTTN_CODE_10	"D0"
#define NTTN_CODE_02	"02"
#define NTTN_CODE_04	"04"
#define NTTN_CODE_08	"08"
#define NTTN_CODE_16	"16"
#define NTTN_CODE_32	"EH"
#define NTTN_CODE_64	"EQ"
#define NTTN_CODE_128	"FH"

/************************************************************************
	주문관련
************************************************************************/
#define MAX_MKT_RNG_TICK	100.

#define ORD_PROC_NEW	'1'	//	신규주문
#define ORD_PROC_MDFY	'2'	//	정정주문
#define ORD_PROC_CNCL	'3'	//	취소주문

#define ORD_SELL_SIDE		'1'	//	매도
#define ORD_BUY_SIDE		'2'	//	매수

#define ORD_ST_MARKET		'1'	//	MARKET ORDER
#define ORD_ST_LIMIT		'2'	//	지정가
#define ORD_ST_STOP			'3'	//	역지정가
#define ORD_ST_COND			'4'	//	조건주문
#define ORD_ST_BEST_LIMIT	'5'	//	최유리지정가
#define ORD_ST_COND_LIMIT	'6'	//	조건부지정가

#define COND_ST_STOP		'1'	//	STOP LOSS
#define COND_ST_LIMIT		'2'	//	PROFIT TAKING
#define COND_ST_BOTH		'3'	//	STOP + PROFIT
#define COND_ST_TS			'4'	//	TS
#define COND_ST_ALL			'5'	//	ALL

#define	ACPT_TP_NON			'0'	//	미접수
#define	ACPT_TP_ORD			'1'	//	주문접수
#define	ACPT_TP_MDFY		'2'	//	정정확인
#define	ACPT_TP_CNCL		'3'	//	취소확인
#define	ACPT_TP_CNTR		'4'	//	체결
#define	ACPT_TP_PCNT		'5'	//	부분체결확인
#define	ACPT_TP_CNCL_ACPT	'A'	//	취소접수
#define	ACPT_TP_MDFY_ACPT	'B'	//	정정접수
#define	ACPT_TP_RJCT		'X'	//	거부


#define FORCE_ORD_YN_MKT_ORD	'1'
#define FORCE_ORD_YN_LIMIT_ORD		'2'	//'	지정가
#define FORCE_ORD_YN_CNCL_STK		'3'	//'	종목취소
#define FORCE_ORD_YN_CNCL_ALL		'4'	//'	전체취소
#define FORCE_ORD_YN_CLR_STK		'5'	//'	종목청산
#define FORCE_ORD_YN_CLR_ALL		'6'	//'	전체청산
#define FORCE_ORD_YN_MIT			'7'	//'	MIT
#define FORCE_ORD_YN_SL				'8'	//SL
#define FORCE_ORD_YN_CNCL_MTKEND	'9'	//'	장마감취소
#define FORCE_ORD_YN_CLR_MKTEND		'A'	//'	장마감청산
#define FORCE_ORD_YN_LOSSCUT		'B'	//'	로스컷
#define FORCE_ORD_YN_CNCL_LOSSCUT	'C'	//'	로스컷취소
#define FORCE_ORD_YN_CLR_OMNI	'D'	//	옴니계좌청산주문
#define FORCE_ORD_YN_CLR_MNG	'E'	//'	관리자청산

/************************************************************************
	호가범위를 위한 %
************************************************************************/
#define MAX_F_HOGA_RNG_RATE	0.15
#define MAX_O_HOGA_RNG_RATE	0.20


/************************************************************************
	시세 SHM 을 위한 종목 MAX 
************************************************************************/
#define MAX_SHM_STK_KS	200
#define MAX_SHM_STK_FU	10
#define MAX_SHM_STK_FX	50


/************************************************************************
	옵션 tick size 기준 가격
************************************************************************/
#define OPTION_TICKSIZE_BASE_PRC	3.0

/************************************************************************/
/*  SAFE FREE FUNCTION                                                  */
/************************************************************************/
#define SafeCloseHandle(h) \
	if (NULL != h){ \
		CloseHandle(h); \
		h = NULL;	\
	}

#define SafeDelete(p)	\
	if (NULL != p){		\
		delete p;		\
		p = NULL;		\
	}
#define SafeWSACloseEvent(h)	\
	if (NULL != h){ \
		WSACloseEvent(h); \
		h = NULL;	\
	}


#endif

