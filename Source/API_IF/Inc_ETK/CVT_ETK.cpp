
#include <afxwin.h>         // MFC core and standard components
#include "CVT_ETK.h"
//#include "../include/UtilLibInc.h"

#include "../Inc_ETK/FC0.h"
#include "../Inc_ETK/FH0.h"
#include "../Inc_ETK/OC0.h"
#include "../Inc_ETK/OH0.h"


//CME
#include "../Inc_ETK/NC0.h"
#include "../Inc_ETK/NH0.h"

//해외선물
#include "../Inc_ETK/OVH.h"
#include "../Inc_ETK/OVC.h"

#include "../../IRUM_UTIL/util.h"

//	DEFINE IN StdAfx.h
enum
{
	DATA_TYPE_STRING = 0,		// 문자열
		DATA_TYPE_LONG,				// 정수
		DATA_TYPE_FLOAT,			// 실수
		DATA_TYPE_FLOAT_DOT,		// 실수( 소숫점을 가지고 있음 )
};


CCVT_ETK::CCVT_ETK()
{

}
CCVT_ETK::~CCVT_ETK()
{

}


/************************************************************************
	주문을 ETK API 로 ENCODING
************************************************************************/

VOID CCVT_ETK::en_KS_TR_Order( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia, char* pOut)
{
	if( pInPack->stC.ORD_PROC_TP[0]==ORD_PROC_NEW )
	{
		en_KS_TR_Order_New(pInPack, i_psCommMedia,pOut);
	}
	if( pInPack->stC.ORD_PROC_TP[0]==ORD_PROC_MDFY )
	{
		en_KS_TR_Order_Mdfy(pInPack, i_psCommMedia,pOut);
	}
	if( pInPack->stC.ORD_PROC_TP[0]==ORD_PROC_CNCL )
	{
		en_KS_TR_Order_Cncl(pInPack, i_psCommMedia,pOut);
	}
}

VOID CCVT_ETK::en_KS_TR_Order_Mdfy( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia,  char* pOut)
{
	char t[128];
	
	t5512InBlock* p = (t5512InBlock*)pOut;
	memset( p, 0x20, sizeof(t5511InBlock) );
	
	//	레코드 갯수
	ETK_SetPacketData( p->reccnt , sizeof( p->reccnt ), "1"	, DATA_TYPE_LONG );
	
	//	주문시장코드
	ETK_SetPacketData( p->ordmarketcode , sizeof( p->ordmarketcode ), "40"	, DATA_TYPE_STRING );
	
	
	/*--------------------------------------------
	계좌번호
	- 중간에 '-' 가 있으면 떼어네야 한다.
	--------------------------------------------*/
	sprintf(t, "%-.*s", sizeof(pInPack->API_ACNT_NO), pInPack->API_ACNT_NO);
	CUtil::Remove_char( t, strlen(t), '-' );
	sprintf(t, "%.*s", min(sizeof(p->accno), strlen(t) ), t); 
	ETK_SetPacketData( p->accno , sizeof( p->accno ),	t	, DATA_TYPE_STRING );
	
	//	비밀번호
	sprintf(t, "%-.*s", sizeof(pInPack->API_PWD), pInPack->API_PWD);
	sprintf(t, "%.*s", min(sizeof(p->passwd), strlen(t) ), t); 
	ETK_SetPacketData( p->passwd , sizeof( p->passwd ),	t	, DATA_TYPE_STRING );
	
	//	종목번호
	sprintf(t, "%-.*s", sizeof(pInPack->stC.STK_CODE), pInPack->stC.STK_CODE);
	sprintf(t, "%.*s", min(sizeof(p->expcode), strlen(t) ), t); 
	ETK_SetPacketData( p->expcode , sizeof( p->expcode ),	t	, DATA_TYPE_STRING );
	
	//	주문유형코드
	ETK_SetPacketData( p->fnoordptncode , sizeof( p->fnoordptncode ), "00"	, DATA_TYPE_STRING );

	//	DUMMY 원주문번호
	ETK_SetPacketData( p->dummyorgordno , sizeof( p->dummyorgordno ), "000"	, DATA_TYPE_LONG );

	//////////////////////////////////////////////////////////////////////////
	//	원주문번호
	//	자체 API_ORD_NO 는 9 BYTE 인데, ETK전문 주문번호크기는 7 
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORG_ORD_NO)-2, pInPack->stC.ORG_ORD_NO+2 );
	CUtil::TrimAll(t, strlen(t));
	ETK_SetPacketData( p->orgordno , sizeof( p->orgordno ), t	, DATA_TYPE_LONG );

	//	호가유형코드
	sprintf(t, "%.*s", sizeof(pInPack->stC.ORD_ST), pInPack->stC.ORD_ST );
	if( t[0] == ORD_ST_MARKET )
		ETK_SetPacketData( p->offergb  , sizeof( p->offergb  ), "03"	, DATA_TYPE_STRING );
	if( t[0] == ORD_ST_LIMIT )
		ETK_SetPacketData( p->offergb  , sizeof( p->offergb  ), "00"	, DATA_TYPE_STRING );

	//	주문가
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORD_PRC), pInPack->stC.ORD_PRC );
	CUtil::TrimAll(t, strlen(t));
	ETK_SetPacketData( p->price      , sizeof( p->price), t      , DATA_TYPE_FLOAT_DOT, 2 );	// 주문가, Header Type이 B 인 경우 Data Type이 실수면 소수점을 포함해야 한다.
	
	//	정정수량
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORD_QTY), pInPack->stC.ORD_QTY );
	CUtil::TrimAll(t,strlen(t));
	ETK_SetPacketData( p->qty , sizeof( p->qty ), t	, DATA_TYPE_LONG );
	
	//	통신매체코드
	ETK_SetPacketData( p->tongsingb , sizeof( p->tongsingb ), i_psCommMedia	, DATA_TYPE_STRING );
	
	//	전략코드	
	ETK_SetPacketData( p->stragb , sizeof( p->reccnt ), ""	, DATA_TYPE_STRING );
	
	//	그룹ID
	ETK_SetPacketData( p->groupid , sizeof( p->groupid ), ""	, DATA_TYPE_STRING );
	
	//	주문회차
	ETK_SetPacketData( p->ordernum , sizeof( p->ordernum ), "0"	, DATA_TYPE_LONG );
	
	//	포트폴리오번호
	ETK_SetPacketData( p->portnum , sizeof( p->portnum ), "0"	, DATA_TYPE_LONG );
	
	//	바스켓번호
	ETK_SetPacketData( p->basketnum , sizeof( p->basketnum ), "0"	, DATA_TYPE_LONG );
	
	//	트렌치번호
	ETK_SetPacketData( p->tranchnum , sizeof( p->tranchnum ), "0"	, DATA_TYPE_LONG );
	
	//	아이템번호
	ETK_SetPacketData( p->itemnum , sizeof( p->itemnum ), "1"	, DATA_TYPE_LONG );
	
	//	관리사번
	ETK_SetPacketData( p->mgempno , sizeof( p->mgempno ), ""	, DATA_TYPE_STRING );
	
	//	펀드id
	ETK_SetPacketData( p->fundid , sizeof( p->fundid ), ""	, DATA_TYPE_STRING );

	//	dummy 펀드원주문번호
	ETK_SetPacketData( p->dummyfundorgordno , sizeof( p->dummyfundorgordno ), "0"	, DATA_TYPE_LONG );

	//	펀드원주문번호
	ETK_SetPacketData( p->fundorgordno , sizeof( p->fundorgordno ), "0"	, DATA_TYPE_LONG );

	//	dummy 펀드주문번호
	ETK_SetPacketData( p->dummyfundordno  , sizeof( p->dummyfundordno ), "0"	, DATA_TYPE_LONG );
	
	//	펀드원주문번호
	ETK_SetPacketData( p->fundordno  , sizeof( p->fundordno  ), "0"	, DATA_TYPE_LONG );
}


VOID CCVT_ETK::en_KS_TR_Order_Cncl( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia, char* pOut)
{
	char t[128];
	
	t5513InBlock * p = (t5513InBlock*)pOut;
	memset( p, 0x20, sizeof(t5511InBlock) );
	
	//	레코드 갯수
	ETK_SetPacketData( p->reccnt , sizeof( p->reccnt ), "1"	, DATA_TYPE_LONG );
	
	//	주문시장코드
	ETK_SetPacketData( p->ordmarketcode , sizeof( p->ordmarketcode ), "40"	, DATA_TYPE_STRING );
	
	
	/*--------------------------------------------
	계좌번호
	- 중간에 '-' 가 있으면 떼어네야 한다.
	--------------------------------------------*/
	sprintf(t, "%-.*s", sizeof(pInPack->API_ACNT_NO), pInPack->API_ACNT_NO);
	CUtil::Remove_char( t, strlen(t), '-' );
	sprintf(t, "%.*s", min(sizeof(p->accno), strlen(t) ), t); 
	ETK_SetPacketData( p->accno , sizeof( p->accno ),	t	, DATA_TYPE_STRING );
	
	//	비밀번호
	sprintf(t, "%-.*s", sizeof(pInPack->API_PWD), pInPack->API_PWD);
	sprintf(t, "%.*s", min(sizeof(p->passwd), strlen(t) ), t); 
	ETK_SetPacketData( p->passwd , sizeof( p->passwd ),	t	, DATA_TYPE_STRING );
	
	//	종목번호
	sprintf(t, "%-.*s", sizeof(pInPack->stC.STK_CODE), pInPack->stC.STK_CODE);
	sprintf(t, "%.*s", min(sizeof(p->expcode), strlen(t) ), t); 
	ETK_SetPacketData( p->expcode , sizeof( p->expcode ),	t	, DATA_TYPE_STRING );
	
	//	주문유형코드
	ETK_SetPacketData( p->fnoordptncode , sizeof( p->fnoordptncode ), "00"	, DATA_TYPE_STRING );

	//	DUMMY 원주문번호
	ETK_SetPacketData( p->dummyorgordno , sizeof( p->dummyorgordno ), "000"	, DATA_TYPE_LONG );

	//////////////////////////////////////////////////////////////////////////
	//	원주문번호
	//	자체 API_ORD_NO 는 9 BYTE 인데, ETK전문 주문번호크기는 7 
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORG_ORD_NO)-2, pInPack->stC.ORG_ORD_NO+2 );
	CUtil::TrimAll(t, strlen(t));
	ETK_SetPacketData( p->orgordno , sizeof( p->orgordno ), t	, DATA_TYPE_LONG );

	//	취소수량
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORD_QTY), pInPack->stC.ORD_QTY );
	CUtil::TrimAll(t, strlen(t));
	ETK_SetPacketData( p->qty , sizeof( p->qty ), t	, DATA_TYPE_LONG );

	//	정정수량
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORD_QTY), pInPack->stC.ORD_QTY );
	CUtil::TrimAll(t,strlen(t));
	ETK_SetPacketData( p->qty , sizeof( p->qty ), t	, DATA_TYPE_LONG );
	
	//	통신매체코드
	ETK_SetPacketData( p->tongsingb , sizeof( p->tongsingb ), i_psCommMedia	, DATA_TYPE_STRING );
	
	//	전략코드	
	ETK_SetPacketData( p->stragb , sizeof( p->reccnt ), ""	, DATA_TYPE_STRING );
	
	//	그룹ID
	ETK_SetPacketData( p->groupid , sizeof( p->groupid ), ""	, DATA_TYPE_STRING );
	
	//	주문회차
	ETK_SetPacketData( p->ordernum , sizeof( p->ordernum ), "0"	, DATA_TYPE_LONG );
	
	//	포트폴리오번호
	ETK_SetPacketData( p->portnum , sizeof( p->portnum ), "0"	, DATA_TYPE_LONG );
	
	//	바스켓번호
	ETK_SetPacketData( p->basketnum , sizeof( p->basketnum ), "0"	, DATA_TYPE_LONG );
	
	//	트렌치번호
	ETK_SetPacketData( p->tranchnum , sizeof( p->tranchnum ), "0"	, DATA_TYPE_LONG );
	
	//	아이템번호
	ETK_SetPacketData( p->itemnum , sizeof( p->itemnum ), "1"	, DATA_TYPE_LONG );
	
	//	관리사번
	ETK_SetPacketData( p->mgempno , sizeof( p->mgempno ), ""	, DATA_TYPE_STRING );
	
	//	펀드id
	ETK_SetPacketData( p->fundid , sizeof( p->fundid ), ""	, DATA_TYPE_STRING );

	//	dummy 펀드원주문번호
	ETK_SetPacketData( p->dummyfundorgordno , sizeof( p->dummyfundorgordno ), "0"	, DATA_TYPE_LONG );

	//	펀드원주문번호
	ETK_SetPacketData( p->fundorgordno , sizeof( p->fundorgordno ), "0"	, DATA_TYPE_LONG );

	//	dummy 펀드주문번호
	ETK_SetPacketData( p->dummyfundordno  , sizeof( p->dummyfundordno ), "0"	, DATA_TYPE_LONG );
	
	//	펀드원주문번호
	ETK_SetPacketData( p->fundordno  , sizeof( p->fundordno  ), "0"	, DATA_TYPE_LONG );


}

VOID CCVT_ETK::en_KS_TR_Order_New( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia,  char* pOut)
{
	char t[128];
	ST_TR_20000* pIn = &pInPack->stC;

	t5511InBlock* p = (t5511InBlock*)pOut;
	memset( p, 0x20, sizeof(t5511InBlock) );

	//	레코드갯수
	ETK_SetPacketData( p->reccnt , sizeof( p->reccnt ), "1"	, DATA_TYPE_LONG );

	//	주문시장코드
	ETK_SetPacketData( p->ordmarketcode , sizeof( p->ordmarketcode ), "40"	, DATA_TYPE_STRING );

	/*--------------------------------------------
	계좌번호
	- 중간에 '-' 가 있으면 떼어네야 한다.
	--------------------------------------------*/
	sprintf(t, "%-.*s", sizeof(pInPack->API_ACNT_NO), pInPack->API_ACNT_NO);
	CUtil::Remove_char( t, strlen(t), '-' );
	sprintf(t, "%.*s", min(sizeof(p->accno), strlen(t) ), t); 
	ETK_SetPacketData( p->accno , sizeof( p->accno ),	t	, DATA_TYPE_STRING );

	//	계좌비밀번호
	sprintf(t, "%-.*s", sizeof(pInPack->API_PWD), pInPack->API_PWD);
	sprintf(t, "%.*s", min(sizeof(p->passwd), strlen(t) ), t); 
	ETK_SetPacketData( p->passwd , sizeof( p->passwd ),	t	, DATA_TYPE_STRING );

	//	종목번호
	sprintf(t, "%-.*s", sizeof(pInPack->stC.STK_CODE), pInPack->stC.STK_CODE);
	sprintf(t, "%.*s", min(sizeof(p->expcode), strlen(t) ), t); 
	ETK_SetPacketData( p->expcode , sizeof( p->expcode ),	t	, DATA_TYPE_STRING );

	//	매매구분(매도,매수)
	sprintf(t, "%.*s", sizeof(pInPack->stC.BUY_SELL_TP), pInPack->stC.BUY_SELL_TP);
	ETK_SetPacketData( p->memegb , sizeof( p->memegb), t	, DATA_TYPE_STRING );

	//	주문유형코드
	ETK_SetPacketData( p->offergb , sizeof( p->offergb ), "00"	, DATA_TYPE_STRING );

	//	호가유형코드
	sprintf(t, "%.*s", sizeof(pInPack->stC.ORD_ST), pInPack->stC.ORD_ST );
	if( t[0] == ORD_ST_MARKET )
		ETK_SetPacketData( p->hogagb , sizeof( p->hogagb ), "03"	, DATA_TYPE_STRING );
	if( t[0] == ORD_ST_LIMIT )
		ETK_SetPacketData( p->hogagb , sizeof( p->hogagb ), "00"	, DATA_TYPE_STRING );

	//	거래코드
	ETK_SetPacketData( p->tradegb , sizeof( p->tradegb ), "03"	, DATA_TYPE_STRING );	//	기타거래(프로그램매매)
	
	//	주문가. 시장가인 경우는 채우지 않는다.
	
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORD_PRC), pInPack->stC.ORD_PRC );
	if( atof(t)==0. )
		sprintf( t, "%.2f", atof(t) );
	CUtil::TrimAll(t,strlen(t));
	ETK_SetPacketData( p->price      , sizeof( p->price), t      , DATA_TYPE_FLOAT_DOT, 2 );	// 주문가, Header Type이 B 인 경우 Data Type이 실수면 소수점을 포함해야 한다.

	
	//	주문수량
	sprintf( t, "%.*s", sizeof(pInPack->stC.ORD_QTY), pInPack->stC.ORD_QTY );
	CUtil::TrimAll(t,strlen(t));
	ETK_SetPacketData( p->qty , sizeof( p->qty ), t	, DATA_TYPE_LONG );

	//	통신매체코드
	ETK_SetPacketData( p->tongsingb , sizeof( p->tongsingb ), i_psCommMedia	, DATA_TYPE_STRING );

	//	전략코드
	ETK_SetPacketData( p->stragb , sizeof( p->reccnt ), ""	, DATA_TYPE_STRING );

	//	그룹ID
	ETK_SetPacketData( p->groupid , sizeof( p->groupid ), ""	, DATA_TYPE_STRING );

	//	주문회차
	ETK_SetPacketData( p->ordernum , sizeof( p->ordernum ), "0"	, DATA_TYPE_LONG );

	//	포트폴리오번호
	ETK_SetPacketData( p->portnum , sizeof( p->portnum ), "0"	, DATA_TYPE_LONG );

	//	바스켓번호
	ETK_SetPacketData( p->basketnum , sizeof( p->basketnum ), "0"	, DATA_TYPE_LONG );

	//	트렌치번호
	ETK_SetPacketData( p->tranchnum , sizeof( p->tranchnum ), "0"	, DATA_TYPE_LONG );

	//	아이템번호
	ETK_SetPacketData( p->itemnum , sizeof( p->itemnum ), "1"	, DATA_TYPE_LONG );

	//	운용지시번호
	ETK_SetPacketData( p->operordnum , sizeof( p->operordnum ), "0"	, DATA_TYPE_STRING );

	//	관리사원번호
	ETK_SetPacketData( p->mgempno , sizeof( p->mgempno ), ""	, DATA_TYPE_STRING );
	
	//	펀드ID
	ETK_SetPacketData( p->fundid , sizeof( p->fundid ), ""	, DATA_TYPE_STRING );

	//	dummy펀드원주문번호
	ETK_SetPacketData( p->dummyfundorgordno , sizeof( p->dummyfundorgordno ), "0"	, DATA_TYPE_LONG );

	//	펀드원주문번호
	ETK_SetPacketData( p->fundorgordno , sizeof( p->fundorgordno ), "0"	, DATA_TYPE_LONG );
}




// 
// VOID CCVT_ETK::de_KS_RL_Cntr(	ST_MQLABEL_API_INFO		*i_pMQLabel
// 									,DRDS_XF2_DATA			*pInPack
// 									,ST_IFRECV_REAL_CNTR	*pOut
// 								)
// {
// 	char	b_temp[128];
// 	int		nNo;
// 	int		nDecimalCnt;	//	해당 종목 소숫점 이하 갯수
// 	double	dNo;

	//	소수이하 갯수를 가져온다.
// 	sprintf( b_temp, "%.2s", i_pMQLabel->NTTN_CODE );
// 	nDecimalCnt = CUtil::GetDecimalCnt( b_temp );
// 
// 	//	init buffer
// 	memset( pOut, 0x20, sizeof(ST_IFRECV_REAL_CNTR) );
// 	*( ((char*)pOut) + sizeof(ST_IFRECV_REAL_CNTR) ) = 0x00;
// 
// 	memcpy( pOut->GDS_CODE, i_pMQLabel->GDS_CODE, sizeof(pOut->GDS_CODE) );
// 	memcpy( pOut->API_CODE, i_pMQLabel->API_CODE, sizeof(pOut->API_CODE) );
// 
// 	//	종목코드
// 	lstrcpy( b_temp, CUtil::TrimAllEx(pInPack->jgcode, sizeof(pInPack->jgcode)) );
// 	memcpy( pOut->STK_CODE, b_temp, strlen(b_temp) );
// 	
// 	//	API USERID
// 	lstrcpy( b_temp, CUtil::TrimAllEx(pInPack->userid, sizeof(pInPack->userid)) );
// 	memcpy( pOut->API_USERID, b_temp, strlen(b_temp) );
// 	
// 	//	API 계좌번호
// 	lstrcpy( b_temp, CUtil::TrimAllEx(pInPack->geja, sizeof(pInPack->geja)) );
// 	memcpy( pOut->API_ACNT_NO, b_temp, strlen(b_temp) );
// 	
// 	//	API 주문번호
// 	nNo = S2N( pInPack->juno, sizeof(pInPack->juno) );
// 	sprintf( b_temp, "%0*d", pOut->API_ORD_NO, nNo );
// 	memcpy( pOut->API_ORD_NO, b_temp, strlen(b_temp) );
// 
// 	//	API 원주문번호
// 	nNo = S2N( pInPack->wonjuno, sizeof(pInPack->wonjuno) );
// 	sprintf( b_temp, "%0*d", pOut->API_ORD_ORG_NO, nNo );
// 	memcpy( pOut->API_ORD_ORG_NO, b_temp, strlen(b_temp) );
// 
// 	//	동부는 체결번호가 없다.
// // 	int nAPI_CntrNo = S2N( pInPack->juno, sizeof(pInPack->juno) );
// // 	sprintf( b_temp, "%0*d", pOut->API_ORD_NO, nAPI_OrdNo );
// // 	memcpy( pOut->API_ORD_NO, b_temp, strlen(b_temp) );
// 
// 	/*
// 		동부의 주문구분(3)을 이용하여 MKT_TP, ORD_ST, PROC_TP, ACPT_TP 를 만든다.
// 		- 주문구분(1) : 시장구분 (1/5/6/7)
// 		  주문구분(2) : 0(매수)/1(매도)/2(정정)/3(취소)
// 		  주문구분(3) : 0(접수확인)/1(체결확인)/2(정정확인)/3(취소확인)/9(거부)
// 	*/
// 	pOut->API_MKT_TP[0] = pInPack->jugb[0];
// 
// 	//	ORD_ST	: 비운다.
// 
// 	//	PROC_TP : 
// 	switch(pInPack->jugb[1])
// 	{
// 	case '0':
// 	case '1':
// 		pOut->ORD_ST[0] = ORD_PROC_NEW;
// 		break;
// 	case '2':
// 		pOut->ORD_ST[0] = ORD_PROC_MDFY;
// 		break;
// 	case '3':
// 		pOut->ORD_ST[0] = ORD_PROC_CNCL;
// 		break;
// 	}

	//	ACPT_TP
// 	switch(pInPack->jugb[2])
// 	{
// 	case '0':
// 		pOut->ACPT_TP[0] = ACPT_TP_ORD;
// 		break;
// 	case '1':
// 		pOut->ACPT_TP[0] = ACPT_TP_CNTR;
// 		break;
// 	case '2':
// 		pOut->ACPT_TP[0] = ACPT_TP_MDFY;
// 		break;
// 	case '3':
// 		pOut->ACPT_TP[0] = ACPT_TP_CNCL;
// 		break;
// 	case '9':
// 		pOut->ACPT_TP[0] = ACPT_TP_RJCT;
// 		break;
// 	}

	//	주문수량
// 	nNo = S2N( pInPack->juqty, sizeof(pInPack->juqty) );
// 	sprintf( b_temp, "%0*d", pOut->ORD_QTY, nNo );
// 	memcpy( pOut->ORD_QTY, b_temp, strlen(b_temp) );
// 
// 	//	주문가격
// 	dNo = S2D( pInPack->juprc, sizeof(pInPack->juprc) );
// 	sprintf( b_temp, "%0*.*f", sizeof(pOut->ORD_PRC), nDecimalCnt, nNo );
// 	memcpy( pOut->ORD_PRC, b_temp, strlen(b_temp) );
// 
// 	//	체결수량
// 	nNo = S2N( pInPack->cheqty, sizeof(pInPack->cheqty) );
// 	sprintf( b_temp, "%0*d", pOut->CNTR_QTY, nNo );
// 	memcpy( pOut->CNTR_QTY, b_temp, strlen(b_temp) );
// 
// 	//	체결 가격
// 	dNo = S2D( pInPack->cheprc, sizeof(pInPack->cheprc) );
// 	sprintf( b_temp, "%0*.*f", sizeof(pOut->CNTR_PRC), nDecimalCnt, nNo );
// 	memcpy( pOut->CNTR_PRC, b_temp, strlen(b_temp) );
// 
// 	//	잔량
// 	nNo = S2N( pInPack->nonqty, sizeof(pInPack->nonqty) );
// 	sprintf( b_temp, "%0*d", pOut->REMN_QTY, nNo );
// 	memcpy( pOut->REMN_QTY, b_temp, strlen(b_temp) );
// 
// 	//	API 거부코드
// 	lstrcpy( b_temp, CUtil::TrimAllEx(pInPack->denycode, sizeof(pInPack->denycode)) );
// 	memcpy( pOut->API_RJCT_CODE, b_temp, strlen(b_temp) );
// 
// 	//	API 거부메세지
// 	lstrcpy( b_temp, CUtil::TrimAllEx(pInPack->denymsg, sizeof(pInPack->denymsg)) );
// 	memcpy( pOut->API_RJCT_MSG, b_temp, strlen(b_temp) );	
/*}*/

VOID CCVT_ETK::de_KS_TR_Order( char* i_etkData, char* i_szGDS_CODE, char* i_szCLORD_NO, char* o_szLabel, char* o_szBody)
{
	t5511OutBlock1* pIn			= (t5511OutBlock1*)i_etkData;
	ST_MQLABEL_API_INFO* pLabel	= (ST_MQLABEL_API_INFO*)o_szLabel;
	ST_IFRECV_TR_ORD* pBody		= (ST_IFRECV_TR_ORD*)o_szBody;
	
	//	init buffer
	memset( pBody, 0x20, sizeof(ST_IFRECV_TR_ORD) );
	*( ((char*)pBody) + sizeof(ST_IFRECV_TR_ORD) ) = 0x00;

	memset( pLabel, 0x20, sizeof(ST_MQLABEL_API_INFO) );
	*( o_szLabel+sizeof(ST_MQLABEL_API_INFO) ) = 0x00;

	// LABEL
	memcpy( pLabel->API_CODE,	APIKEY_KS_ETK,	min(strlen(APIKEY_KS_ETK),sizeof(pLabel->API_CODE) ) );
	memcpy( pLabel->CODE,		MQ_MSGTYPE_TR,	sizeof(pLabel->CODE) );
	memcpy( pLabel->GDS_CODE,	i_szGDS_CODE,	min(strlen(i_szGDS_CODE),sizeof(pLabel->GDS_CODE) ) );
	memcpy( pLabel->NTTN_CODE,	NTTN_CODE_10,	sizeof(pLabel->NTTN_CODE) );
	
	memcpy( pBody->GDS_CODE,	pLabel->GDS_CODE,		sizeof(pBody->GDS_CODE) );
	sprintf( pBody->CLORD_NO,	i_szCLORD_NO,	min(strlen(i_szCLORD_NO), sizeof(pBody->CLORD_NO)) );

	//////////////////////////////////////////////////////////////////////////
	//	ETK 주문번호 : 총 10자리, 우리는 9 자리, 따라서 제일 앞 1자리 빼고 처리한다.
	char szOrdNo[32];
	sprintf( szOrdNo, "%.2s%.*s", pIn->dummyordno+1,  sizeof(pIn->ordno), pIn->ordno );	
	memcpy( pBody->ORD_NO, szOrdNo, min(strlen(szOrdNo),sizeof(pBody->ORD_NO) ) );
	
	sprintf( pBody->RSLT_CODE,	"%0*d",		sizeof(pBody->RSLT_CODE),							RET_SUCCESS );
	sprintf( pBody->API_CODE,	"%-*.*s",	sizeof(pLabel->API_CODE), sizeof(pLabel->API_CODE),	pLabel->API_CODE );
	sprintf( pBody->API_MSGCODE,"%-*.*s",	sizeof(pBody->API_MSGCODE),sizeof(pBody->API_MSGCODE),	"000000" );
	memset( pBody->API_MSG, 0x20,	sizeof(pBody->API_MSG) );
	
}


VOID CCVT_ETK::de_KS_TR_Order_Mdfy( char* i_etkData, char* i_szGDS_CODE, char* i_szCLORD_NO, char* o_szLabel, char* o_szBody)
{
	t5512OutBlock1 * pIn		= (t5512OutBlock1 *)i_etkData;
	ST_MQLABEL_API_INFO* pLabel	= (ST_MQLABEL_API_INFO*)o_szLabel;
	ST_IFRECV_TR_ORD* pBody		= (ST_IFRECV_TR_ORD*)o_szBody;
	
	//	init buffer
	memset( pBody, 0x20, sizeof(ST_IFRECV_TR_ORD) );
	*( ((char*)pBody) + sizeof(ST_IFRECV_TR_ORD) ) = 0x00;
	
	memset( pLabel, 0x20, sizeof(ST_MQLABEL_API_INFO) );
	*( o_szLabel+sizeof(ST_MQLABEL_API_INFO) ) = 0x00;
	
	// LABEL
	memcpy( pLabel->API_CODE,	APIKEY_KS_ETK,	min(strlen(APIKEY_KS_ETK),sizeof(pLabel->API_CODE) ) );
	memcpy( pLabel->CODE,		MQ_MSGTYPE_TR,	sizeof(pLabel->CODE) );
	memcpy( pLabel->GDS_CODE,	i_szGDS_CODE,	min(strlen(i_szGDS_CODE),sizeof(pLabel->GDS_CODE) ) );
	memcpy( pLabel->NTTN_CODE,	NTTN_CODE_10,	sizeof(pLabel->NTTN_CODE) );
	
	memcpy( pBody->GDS_CODE,	pLabel->GDS_CODE,		sizeof(pBody->GDS_CODE) );
	sprintf( pBody->CLORD_NO,	i_szCLORD_NO,	min(strlen(i_szCLORD_NO), sizeof(pBody->CLORD_NO)) );
	
	//////////////////////////////////////////////////////////////////////////
	//	ETK 주문번호 : 총 10자리, 우리는 9 자리, 따라서 제일 앞 1자리 빼고 처리한다.
	char szOrdNo[32];
	sprintf( szOrdNo, "%.2s%.*s", pIn->dummyordno+1,  sizeof(pIn->ordno), pIn->ordno );	
	memcpy( pBody->ORD_NO, szOrdNo, min(strlen(szOrdNo),sizeof(pBody->ORD_NO) ) );
	
	sprintf( pBody->RSLT_CODE,	"%0*d",		sizeof(pBody->RSLT_CODE),							RET_SUCCESS );
	sprintf( pBody->API_CODE,	"%-*.*s",	sizeof(pLabel->API_CODE), sizeof(pLabel->API_CODE),	pLabel->API_CODE );
	sprintf( pBody->API_MSGCODE,"%-*.*s",	sizeof(pBody->API_MSGCODE),sizeof(pBody->API_MSGCODE),	"000000" );
	memset( pBody->API_MSG, 0x20,	sizeof(pBody->API_MSG) );
	
}

VOID CCVT_ETK::de_KS_TR_Order_Cncl( char* i_etkData, char* i_szGDS_CODE, char* i_szCLORD_NO, char* o_szLabel, char* o_szBody)
{
	t5513OutBlock1* pIn			= (t5513OutBlock1 *)i_etkData;
	ST_MQLABEL_API_INFO* pLabel	= (ST_MQLABEL_API_INFO*)o_szLabel;
	ST_IFRECV_TR_ORD* pBody		= (ST_IFRECV_TR_ORD*)o_szBody;
	
	//	init buffer
	memset( pBody, 0x20, sizeof(ST_IFRECV_TR_ORD) );
	*( ((char*)pBody) + sizeof(ST_IFRECV_TR_ORD) ) = 0x00;
	
	memset( pLabel, 0x20, sizeof(ST_MQLABEL_API_INFO) );
	*( o_szLabel+sizeof(ST_MQLABEL_API_INFO) ) = 0x00;
	
	// LABEL
	memcpy( pLabel->API_CODE,	APIKEY_KS_ETK,	min(strlen(APIKEY_KS_ETK),sizeof(pLabel->API_CODE) ) );
	memcpy( pLabel->CODE,		MQ_MSGTYPE_TR,	sizeof(pLabel->CODE) );
	memcpy( pLabel->GDS_CODE,	i_szGDS_CODE,	min(strlen(i_szGDS_CODE),sizeof(pLabel->GDS_CODE) ) );
	memcpy( pLabel->NTTN_CODE,	NTTN_CODE_10,	sizeof(pLabel->NTTN_CODE) );
	
	memcpy( pBody->GDS_CODE,	pLabel->GDS_CODE,		sizeof(pBody->GDS_CODE) );
	sprintf( pBody->CLORD_NO,	i_szCLORD_NO,	min(strlen(i_szCLORD_NO), sizeof(pBody->CLORD_NO)) );
	
	//////////////////////////////////////////////////////////////////////////
	//	ETK 주문번호 : 총 10자리, 우리는 9 자리, 따라서 제일 앞 1자리 빼고 처
	char szOrdNo[32];
	sprintf( szOrdNo, "%.2s%.*s", pIn->dummyordno+1,  sizeof(pIn->ordno), pIn->ordno );	
	memcpy( pBody->ORD_NO, szOrdNo, min(strlen(szOrdNo),sizeof(pBody->ORD_NO) ) );
	
	sprintf( pBody->RSLT_CODE,	"%0*d",		sizeof(pBody->RSLT_CODE),							RET_SUCCESS );
	sprintf( pBody->API_CODE,	"%-*.*s",	sizeof(pLabel->API_CODE), sizeof(pLabel->API_CODE),	pLabel->API_CODE );
	sprintf( pBody->API_MSGCODE,"%-*.*s",	sizeof(pBody->API_MSGCODE),sizeof(pBody->API_MSGCODE),	"000000" );
	memset( pBody->API_MSG, 0x20,	sizeof(pBody->API_MSG) );
	
}

VOID CCVT_ETK::de_KS_F_Sise(char* psOrgData, char* pOut)
{
	char	szLen[32], szCode[128];
	char	t[128];

	FC0_OutBlock*	org	= (FC0_OutBlock*) psOrgData;		//	DrdsSC0.h
	REAL_SISE_KS*	p	= (REAL_SISE_KS*)pOut;			//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_SISE_KS) );

	SET_LEN( sizeof(REAL_SISE_KS), szLen );
	SET_CODE_STR( CODE_REAL_SISE_KS, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );

 	
 	memcpy(p->stk_code,			org->futcode,		min(sizeof(p->stk_code), sizeof(org->futcode)) );
 	memcpy( p->gds_code, GDS_CODE_KSF, sizeof(p->gds_code));
 	memcpy(p->tm,				org->chetime,			min(sizeof(p->tm), 	sizeof(org->chetime)) );
 	
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(p->now_prc), t );
	memcpy(p->now_prc, t, sizeof(p->now_prc) );
	
	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(p->open), t );
	memcpy(p->open, t, sizeof(p->open) );
	
	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(p->high), t );
	memcpy(p->high, t, sizeof(p->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(p->low), t );
	memcpy(p->low, t, sizeof(p->low) );
	
	//	매도매수구분 추가
	if( org->cgubun[0]=='-' )
		p->buy_sell_tp[0] = '1';
	else
		p->buy_sell_tp[0] = '2';

	//	누적체결수량
	sprintf( t, "%*d", sizeof(p->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(p->acml_cntr_qty,	t,		sizeof(p->acml_cntr_qty) );
	//	memcpy(p->acml_buy_cntr_qty,org->bidvol,		min(sizeof(p->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(p->acml_amt), t );
	memcpy(p->acml_amt,			t,  sizeof(p->acml_amt) );
	
	switch(org->sign[0])
	{
	case '2':	p->daebi_sign[0] = '+';	break;
	case '5':	p->daebi_sign[0] = '-';	break;
	default:	p->daebi_sign[0] = ' ';	break;
	}

	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(p->chg), t );
	memcpy(p->chg,				t, sizeof(p->chg) );
	
	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(p->chg_rate), t );
	memcpy(p->chg_rate,			t, sizeof(p->chg_rate) );
	
	sprintf( t, "%*d", sizeof(p->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(p->cntr_qty, 		t, sizeof(p->cntr_qty) );
	
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	memcpy(p->o_hoga_1, 		t, sizeof(p->o_hoga_1) );
	
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );
 	memcpy(p->b_hoga_1, 		t, sizeof(p->b_hoga_1) );
	
	get_double_withdot( org->theoryprice, sizeof(org->theoryprice), 2, sizeof(p->theory_prc), t );
	memcpy(p->theory_prc, t, sizeof(p->theory_prc) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(p->open_yak), t );
	memcpy(p->open_yak, 		t, 	sizeof(p->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(p->open_yak_chg), t );
	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

 	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

	get_double_withdot( org->kasis, sizeof(org->kasis), 2, sizeof(p->gyuri_rate), t );
 	memcpy(p->gyuri_rate,		t,	sizeof(p->gyuri_rate) );

	get_double_withdot( org->sbasis, sizeof(org->sbasis), 2, sizeof(p->basis), t );
	memcpy(p->basis, 			t, 	sizeof(p->basis) );
	
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(p->ks200), t );
	memcpy(p->ks200,			t,	sizeof(p->ks200) );

	get_double_withdot( org->ibasis, sizeof(org->ibasis), 2, sizeof(p->theory_basis), t );
 	memcpy(p->theory_basis,		t, sizeof(p->theory_basis) );

 	p->ETX[0] = 0x03;
 	p->ETX[1] = 0x00;


}


//	CLIENT 에 내려보내기 위한 축소화된 패킷
/*시세오류고려
VOID CCVT_ETK::de_KS_F_SiseEx(	char* psOrgData
								, char* pOut				//	SISE_SHM 을 위한 패킷
								, char* pOutForClient		//	FRONT 를 위한 패킷
								)
*/

#define SISE_MIN 100
#define SISE_MAX 300
BOOL CCVT_ETK::de_KS_F_SiseEx(	char* psOrgData
							  , char* pOut				//	SISE_SHM 을 위한 패킷
							  , char* pOutForClient		//	FRONT 를 위한 패킷
							  , double* pErrSise		//	시세오류인경우 해당 시세 반환
							  )
{
	char	szLen[32], szCode[128];
	char	t[128];

	FC0_OutBlock*	org		= (FC0_OutBlock*) psOrgData;	//	DrdsSC0.h
	REAL_SISE_KS*	p		= (REAL_SISE_KS*)pOut;			//	PacketDef.h
	REAL_SISE_KS_84003*	pFront	= (REAL_SISE_KS_84003*)pOutForClient;	//	PacketDef.h

	
	//////////////////////////////////////////////////////////////////////////
	//	SHM 용 패킷 구성
	memset( p, 0x20, sizeof(REAL_SISE_KS) );
	
	SET_LEN( sizeof(REAL_SISE_KS), szLen );
	SET_CODE_STR( CODE_REAL_SISE_KS, szCode );

	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );

 	
 	memcpy(p->stk_code,			org->futcode,		min(sizeof(p->stk_code), sizeof(org->futcode)) );
 	memcpy( p->gds_code, GDS_CODE_KSF, sizeof(p->gds_code));
 	memcpy(p->tm,				org->chetime,			min(sizeof(p->tm), 	sizeof(org->chetime)) );
 	
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(p->now_prc), t );
	memcpy(p->now_prc, t, sizeof(p->now_prc) );

	//시세오류고려
	double dNow = atof(t);
	if( dNow>=SISE_MAX || dNow<=SISE_MIN ){
		*pErrSise = dNow;
		return FALSE;
	}

	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(p->open), t );
	memcpy(p->open, t, sizeof(p->open) );
	
	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(p->high), t );
	memcpy(p->high, t, sizeof(p->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(p->low), t );
	memcpy(p->low, t, sizeof(p->low) );
	
	//	매도매수구분 추가
	if( org->cgubun[0]=='-' )
		p->buy_sell_tp[0] = '1';
	else
		p->buy_sell_tp[0] = '2';

	//	누적체결수량
	sprintf( t, "%*d", sizeof(p->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(p->acml_cntr_qty,	t,		sizeof(p->acml_cntr_qty) );
	//	memcpy(p->acml_buy_cntr_qty,org->bidvol,		min(sizeof(p->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(p->acml_amt), t );
	memcpy(p->acml_amt,			t,  sizeof(p->acml_amt) );
	
	switch(org->sign[0])
	{
	case '2':	p->daebi_sign[0] = '+';	break;
	case '5':	p->daebi_sign[0] = '-';	break;
	default:	p->daebi_sign[0] = ' ';	break;
	}

	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(p->chg), t );
	memcpy(p->chg,				t, sizeof(p->chg) );
	
	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(p->chg_rate), t );
	memcpy(p->chg_rate,			t, sizeof(p->chg_rate) );
	
	sprintf( t, "%*d", sizeof(p->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(p->cntr_qty, 		t, sizeof(p->cntr_qty) );
	
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	memcpy(p->o_hoga_1, 		t, sizeof(p->o_hoga_1) );
	
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );
 	memcpy(p->b_hoga_1, 		t, sizeof(p->b_hoga_1) );
	
	get_double_withdot( org->theoryprice, sizeof(org->theoryprice), 2, sizeof(p->theory_prc), t );
	memcpy(p->theory_prc, t, sizeof(p->theory_prc) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(p->open_yak), t );
	memcpy(p->open_yak, 		t, 	sizeof(p->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(p->open_yak_chg), t );
	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

 	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

	get_double_withdot( org->kasis, sizeof(org->kasis), 2, sizeof(p->gyuri_rate), t );
 	memcpy(p->gyuri_rate,		t,	sizeof(p->gyuri_rate) );

	get_double_withdot( org->sbasis, sizeof(org->sbasis), 2, sizeof(p->basis), t );
	memcpy(p->basis, 			t, 	sizeof(p->basis) );
	
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(p->ks200), t );
	memcpy(p->ks200,			t,	sizeof(p->ks200) );

	get_double_withdot( org->ibasis, sizeof(org->ibasis), 2, sizeof(p->theory_basis), t );
 	memcpy(p->theory_basis,		t, sizeof(p->theory_basis) );

 	p->ETX[0] = 0x03;
 	p->ETX[1] = 0x00;


	//////////////////////////////////////////////////////////////////////////
	//	FRONT 용 패킷 구성
	memset( pFront, 0x20, sizeof(REAL_SISE_KS_84003) );
	
	SET_LEN( sizeof(REAL_SISE_KS_84003), szLen );
	SET_CODE_STR( 84003, szCode );

	pFront->STX[0]	=	0x02;
	memcpy( pFront->Header.Len, szLen, sizeof(pFront->Header.Len) );	
	memcpy( pFront->Header.Code, szCode, sizeof(pFront->Header.Code) );
	
	
	memcpy(pFront->stk_code,	org->futcode,		min(sizeof(pFront->stk_code), sizeof(org->futcode)) );
	memcpy(pFront->gds_code,	GDS_CODE_KSF, sizeof(pFront->gds_code));
	memcpy(pFront->tm,			org->chetime,		min(sizeof(pFront->tm), 	sizeof(org->chetime)) );
	
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(pFront->now_prc), t );
	memcpy(pFront->now_prc, t, sizeof(pFront->now_prc) );
	
	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(pFront->open), t );
	memcpy(pFront->open, t, sizeof(pFront->open) );
	
	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(pFront->high), t );
	memcpy(pFront->high, t, sizeof(pFront->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(pFront->low), t );
	memcpy(pFront->low, t, sizeof(pFront->low) );
	
	//	매도매수구분 추가
	if( org->cgubun[0]=='-' )
		pFront->buy_sell_tp[0] = '1';
	else
		pFront->buy_sell_tp[0] = '2';
	
	//	누적체결수량
	sprintf( t, "%*d", sizeof(pFront->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(pFront->acml_cntr_qty,	t,		sizeof(pFront->acml_cntr_qty) );
	//	memcpy(pFront->acml_buy_cntr_qty,org->bidvol,		min(sizeof(pFront->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	// 20120822 basis로 대체
	get_double_withdot( org->sbasis, sizeof(org->sbasis), 2, sizeof(p->basis), t );
	sprintf( t, "%*.2f", sizeof(pFront->acml_buy_cntr_qty), atof(t));
	memcpy(pFront->acml_buy_cntr_qty,	t,		strlen(t) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(pFront->acml_amt), t );
	memcpy(pFront->acml_amt,			t,  sizeof(pFront->acml_amt) );
	
	switch(org->sign[0])
	{
	case '2':	pFront->daebi_sign[0] = '+';	break;
	case '5':	pFront->daebi_sign[0] = '-';	break;
	default:	pFront->daebi_sign[0] = ' ';	break;
	}
	
	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(pFront->chg), t );
	memcpy(pFront->chg,				t, sizeof(pFront->chg) );
	
	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(pFront->chg_rate), t );
	memcpy(pFront->chg_rate,			t, sizeof(pFront->chg_rate) );
	
	sprintf( t, "%*d", sizeof(pFront->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(pFront->cntr_qty, 		t, min( strlen(t),sizeof(pFront->cntr_qty)) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(pFront->open_yak), t );
	memcpy(pFront->open_yak, 		t, 	sizeof(pFront->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(pFront->open_yak_chg), t );
	memcpy(pFront->open_yak_chg, 	t, 	sizeof(pFront->open_yak_chg) );
		
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(pFront->ks200), t );
	memcpy(pFront->ks200,			t,	sizeof(pFront->ks200) );
	
	pFront->ETX[0] = 0x03;
 	pFront->ETX[1] = 0x00;

	return TRUE;
}



//CME
VOID CCVT_ETK::de_KS_F_SiseEx_CME(	char* psOrgData
								, char* pOut				//	SISE_SHM 을 위한 패킷
								, char* pOutForClient		//	FRONT 를 위한 패킷
								)
{
	char	szLen[32], szCode[128];
	char	t[128];

	NC0_OutBlock*	org		= (NC0_OutBlock*) psOrgData;	//	DrdsSC0.h
	REAL_SISE_KS*	p		= (REAL_SISE_KS*)pOut;			//	PacketDef.h
	REAL_SISE_KS_84003*	pFront	= (REAL_SISE_KS_84003*)pOutForClient;	//	PacketDef.h

	
	//////////////////////////////////////////////////////////////////////////
	//	SHM 용 패킷 구성
	memset( p, 0x20, sizeof(REAL_SISE_KS) );
	
	SET_LEN( sizeof(REAL_SISE_KS), szLen );
	SET_CODE_STR( CODE_REAL_SISE_KS, szCode );

	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );

 	
 	memcpy(p->stk_code,			org->futcode,		min(sizeof(p->stk_code), sizeof(org->futcode)) );
 	memcpy( p->gds_code, GDS_CODE_KSF, sizeof(p->gds_code));
 	memcpy(p->tm,				org->chetime,			min(sizeof(p->tm), 	sizeof(org->chetime)) );
 	
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(p->now_prc), t );
	memcpy(p->now_prc, t, sizeof(p->now_prc) );
	
	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(p->open), t );
	memcpy(p->open, t, sizeof(p->open) );
	
	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(p->high), t );
	memcpy(p->high, t, sizeof(p->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(p->low), t );
	memcpy(p->low, t, sizeof(p->low) );
	
	//	매도매수구분 추가
	if( org->cgubun[0]=='-' )
		p->buy_sell_tp[0] = '1';
	else
		p->buy_sell_tp[0] = '2';

	//	누적체결수량
	sprintf( t, "%*d", sizeof(p->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(p->acml_cntr_qty,	t,		sizeof(p->acml_cntr_qty) );
	//	memcpy(p->acml_buy_cntr_qty,org->bidvol,		min(sizeof(p->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(p->acml_amt), t );
	memcpy(p->acml_amt,			t,  sizeof(p->acml_amt) );
	
	switch(org->sign[0])
	{
	case '2':	p->daebi_sign[0] = '+';	break;
	case '5':	p->daebi_sign[0] = '-';	break;
	default:	p->daebi_sign[0] = ' ';	break;
	}

	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(p->chg), t );
	memcpy(p->chg,				t, sizeof(p->chg) );
	
	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(p->chg_rate), t );
	memcpy(p->chg_rate,			t, sizeof(p->chg_rate) );
	
	sprintf( t, "%*d", sizeof(p->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(p->cntr_qty, 		t, sizeof(p->cntr_qty) );
	
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	memcpy(p->o_hoga_1, 		t, sizeof(p->o_hoga_1) );
	
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );
 	memcpy(p->b_hoga_1, 		t, sizeof(p->b_hoga_1) );
	
	get_double_withdot( org->theoryprice, sizeof(org->theoryprice), 2, sizeof(p->theory_prc), t );
	memcpy(p->theory_prc, t, sizeof(p->theory_prc) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(p->open_yak), t );
	memcpy(p->open_yak, 		t, 	sizeof(p->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(p->open_yak_chg), t );
	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

 	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

	get_double_withdot( org->kasis, sizeof(org->kasis), 2, sizeof(p->gyuri_rate), t );
 	memcpy(p->gyuri_rate,		t,	sizeof(p->gyuri_rate) );

	get_double_withdot( org->sbasis, sizeof(org->sbasis), 2, sizeof(p->basis), t );
	memcpy(p->basis, 			t, 	sizeof(p->basis) );
	
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(p->ks200), t );
	memcpy(p->ks200,			t,	sizeof(p->ks200) );

	get_double_withdot( org->ibasis, sizeof(org->ibasis), 2, sizeof(p->theory_basis), t );
 	memcpy(p->theory_basis,		t, sizeof(p->theory_basis) );

 	p->ETX[0] = 0x03;
 	p->ETX[1] = 0x00;


	//////////////////////////////////////////////////////////////////////////
	//	FRONT 용 패킷 구성
	memset( pFront, 0x20, sizeof(REAL_SISE_KS_84003) );
	
	SET_LEN( sizeof(REAL_SISE_KS_84003), szLen );
	SET_CODE_STR( 84003, szCode );

	pFront->STX[0]	=	0x02;
	memcpy( pFront->Header.Len, szLen, sizeof(pFront->Header.Len) );	
	memcpy( pFront->Header.Code, szCode, sizeof(pFront->Header.Code) );
	
	
	memcpy(pFront->stk_code,	org->futcode,		min(sizeof(pFront->stk_code), sizeof(org->futcode)) );
	memcpy(pFront->gds_code,	GDS_CODE_KSF, sizeof(pFront->gds_code));
	memcpy(pFront->tm,			org->chetime,		min(sizeof(pFront->tm), 	sizeof(org->chetime)) );
	
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(pFront->now_prc), t );
	memcpy(pFront->now_prc, t, sizeof(pFront->now_prc) );
	
	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(pFront->open), t );
	memcpy(pFront->open, t, sizeof(pFront->open) );
	
	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(pFront->high), t );
	memcpy(pFront->high, t, sizeof(pFront->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(pFront->low), t );
	memcpy(pFront->low, t, sizeof(pFront->low) );
	
	//	매도매수구분 추가
	if( org->cgubun[0]=='-' )
		pFront->buy_sell_tp[0] = '1';
	else
		pFront->buy_sell_tp[0] = '2';
	
	//	누적체결수량
	sprintf( t, "%*d", sizeof(pFront->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(pFront->acml_cntr_qty,	t,		sizeof(pFront->acml_cntr_qty) );
	//	memcpy(pFront->acml_buy_cntr_qty,org->bidvol,		min(sizeof(pFront->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(pFront->acml_amt), t );
	memcpy(pFront->acml_amt,			t,  sizeof(pFront->acml_amt) );
	
	switch(org->sign[0])
	{
	case '2':	pFront->daebi_sign[0] = '+';	break;
	case '5':	pFront->daebi_sign[0] = '-';	break;
	default:	pFront->daebi_sign[0] = ' ';	break;
	}
	
	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(pFront->chg), t );
	memcpy(pFront->chg,				t, sizeof(pFront->chg) );
	
	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(pFront->chg_rate), t );
	memcpy(pFront->chg_rate,			t, sizeof(pFront->chg_rate) );
	
	sprintf( t, "%*d", sizeof(pFront->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(pFront->cntr_qty, 		t, min( strlen(t),sizeof(pFront->cntr_qty)) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(pFront->open_yak), t );
	memcpy(pFront->open_yak, 		t, 	sizeof(pFront->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(pFront->open_yak_chg), t );
	memcpy(pFront->open_yak_chg, 	t, 	sizeof(pFront->open_yak_chg) );
		
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(pFront->ks200), t );
	memcpy(pFront->ks200,			t,	sizeof(pFront->ks200) );
	
	pFront->ETX[0] = 0x03;
 	pFront->ETX[1] = 0x00;
}




//SMILOR.VOID CCVT_ETK::de_KS_F_Hoga(char* psOrgData, char* pOut)
BOOL CCVT_ETK::de_KS_F_Hoga(char* psOrgData, char* pOut)
{
	char	szLen[32], szCode[128], t[128];

	FH0_OutBlock*	org	= (FH0_OutBlock*) psOrgData;	//	FH0.h
	REAL_HOGA_KS*	p	= (REAL_HOGA_KS*)pOut;			//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_HOGA_KS) );
	*(pOut+sizeof(REAL_HOGA_KS)) = 0x00;
	
	SET_LEN( sizeof(REAL_HOGA_KS), szLen );
	SET_CODE_STR( CODE_REAL_HOGA_KS, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );
	
	//	종목코드
	memcpy(p->stk_code,			org->futcode,		min(sizeof(p->stk_code), sizeof(org->futcode)) );

	//	GDS CODE
	memcpy( p->gds_code, GDS_CODE_KSF, sizeof(p->gds_code));

	//	시각
	memcpy(p->tm,				org->hotime,		min(sizeof(p->tm), 		sizeof(org->hotime)) );
	
	//	매도호가 1
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	
	//SMILOR0803
	if( atof(t)<100) return FALSE;

	memcpy(p->o_hoga_1, t, sizeof(p->o_hoga_1) );

	//	매도호가 2
	get_double_withdot( org->offerho2, sizeof(org->offerho2), 2, sizeof(p->o_hoga_2), t );
	memcpy(p->o_hoga_2, t, sizeof(p->o_hoga_2) );

	//	매도호가 3
	get_double_withdot( org->offerho3, sizeof(org->offerho3), 2, sizeof(p->o_hoga_3), t );
	memcpy(p->o_hoga_3, t, sizeof(p->o_hoga_3) );

	//	매도호가 4
	get_double_withdot( org->offerho4, sizeof(org->offerho4), 2, sizeof(p->o_hoga_4), t );
	memcpy(p->o_hoga_4, t, sizeof(p->o_hoga_4) );

	//	매도호가 5
	get_double_withdot( org->offerho5, sizeof(org->offerho5), 2, sizeof(p->o_hoga_5), t );
	memcpy(p->o_hoga_5, t, sizeof(p->o_hoga_5) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_1), S2N( org->offerrem1,sizeof(org->offerrem1) ) );
	memcpy(p->o_rmnq_1, 		t, sizeof(p->o_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_2), S2N( org->offerrem2,sizeof(org->offerrem2) ) );
	memcpy(p->o_rmnq_2, 		t, sizeof(p->o_rmnq_2) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_3), S2N( org->offerrem3,sizeof(org->offerrem3) ) );
	memcpy(p->o_rmnq_3, 		t, sizeof(p->o_rmnq_3) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_4), S2N( org->offerrem4,sizeof(org->offerrem4) ) );
	memcpy(p->o_rmnq_4, 		t, sizeof(p->o_rmnq_4) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_5), S2N( org->offerrem5,sizeof(org->offerrem5) ) );
	memcpy(p->o_rmnq_5, 		t, sizeof(p->o_rmnq_5) );

	sprintf( t, "%*d", sizeof(p->o_trmnq), S2N( org->totofferrem,sizeof(org->totofferrem) ) );
	memcpy(p->o_trmnq, 		t, sizeof(p->o_trmnq) );

	sprintf( t, "%*d", sizeof(p->o_cnt_1), S2N( org->offercnt1,sizeof(org->offercnt1) ) );
	memcpy(p->o_cnt_1, 			t, sizeof(p->o_cnt_1) );

	sprintf( t, "%*d", sizeof(p->o_cnt_2), S2N( org->offercnt2,sizeof(org->offercnt2) ) );
	memcpy(p->o_cnt_2, 			t, sizeof(p->o_cnt_2) );

	sprintf( t, "%*d", sizeof(p->o_cnt_3), S2N( org->offercnt3,sizeof(org->offercnt3) ) );
	memcpy(p->o_cnt_3, 			t, sizeof(p->o_cnt_3) );

	sprintf( t, "%*d", sizeof(p->o_cnt_4), S2N( org->offercnt4,sizeof(org->offercnt4) ) );
	memcpy(p->o_cnt_4, 			t, sizeof(p->o_cnt_4) );

	sprintf( t, "%*d", sizeof(p->o_cnt_5), S2N( org->offercnt5,sizeof(org->offercnt5) ) );
	memcpy(p->o_cnt_5, 			t, sizeof(p->o_cnt_5) );

	sprintf( t, "%*d", sizeof(p->o_tot_cnt), S2N( org->totoffercnt,sizeof(org->totoffercnt) ) );
	memcpy(p->o_tot_cnt, 		t, sizeof(p->o_tot_cnt) );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	매수호가 1
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );

	//SMILOR0803
	if( atof(t)<100) return FALSE;

	memcpy(p->b_hoga_1, t, sizeof(p->b_hoga_1) );
	
	//	매수호가 2
	get_double_withdot( org->bidho2, sizeof(org->bidho2), 2, sizeof(p->b_hoga_2), t );
	memcpy(p->b_hoga_2, t, sizeof(p->b_hoga_2) );
	
	//	매수호가 3
	get_double_withdot( org->bidho3, sizeof(org->bidho3), 2, sizeof(p->b_hoga_3), t );
	memcpy(p->b_hoga_3, t, sizeof(p->b_hoga_3) );
	
	//	매수호가 4
	get_double_withdot( org->bidho4, sizeof(org->bidho4), 2, sizeof(p->b_hoga_4), t );
	memcpy(p->b_hoga_4, t, sizeof(p->b_hoga_4) );
	
	//	매수호가 5
	get_double_withdot( org->bidho5, sizeof(org->bidho5), 2, sizeof(p->b_hoga_5), t );
	memcpy(p->b_hoga_5, t, sizeof(p->b_hoga_5) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_1), S2N( org->bidrem1,sizeof(org->bidrem1) ) );
	memcpy(p->b_rmnq_1, 		t, sizeof(p->b_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->b_rmnq_2), S2N( org->bidrem2,sizeof(org->bidrem2) ) );
	memcpy(p->b_rmnq_2, 		t, sizeof(p->b_rmnq_2) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_3), S2N( org->bidrem3,sizeof(org->bidrem3) ) );
	memcpy(p->b_rmnq_3, 		t, sizeof(p->b_rmnq_3) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_4), S2N( org->bidrem4,sizeof(org->bidrem4) ) );
	memcpy(p->b_rmnq_4, 		t, sizeof(p->b_rmnq_4) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_5), S2N( org->bidrem5,sizeof(org->bidrem5) ) );
	memcpy(p->b_rmnq_5, 		t, sizeof(p->b_rmnq_5) );
	
	sprintf( t, "%*d", sizeof(p->b_trmnq), S2N( org->totbidrem,sizeof(org->totbidrem) ) );
	memcpy(p->b_trmnq, 			t, sizeof(p->b_trmnq) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_1), S2N( org->bidcnt1,sizeof(org->bidcnt1) ) );
	memcpy(p->b_cnt_1, 			t, sizeof(p->b_cnt_1) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_2), S2N( org->bidcnt2,sizeof(org->bidcnt2) ) );
	memcpy(p->b_cnt_2, 			t, sizeof(p->b_cnt_2) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_3), S2N( org->bidcnt3,sizeof(org->bidcnt3) ) );
	memcpy(p->b_cnt_3, 			t, sizeof(p->b_cnt_3) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_4), S2N( org->bidcnt4,sizeof(org->bidcnt4) ) );
	memcpy(p->b_cnt_4, 			t, sizeof(p->b_cnt_4) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_5), S2N( org->bidcnt5,sizeof(org->bidcnt5) ) );
	memcpy(p->b_cnt_5, 			t, sizeof(p->b_cnt_5) );
	
	sprintf( t, "%*d", sizeof(p->b_tot_cnt), S2N( org->totbidcnt,sizeof(org->totbidcnt) ) );
	memcpy(p->b_tot_cnt, 		t, sizeof(p->b_tot_cnt) );
	
	p->ETX[0] = 0x03;
	p->ETX[1] = 0x00;

	//SMILOR0803
	return TRUE;

}


/*
	- 동시호가 잔량 변화도 front 에 보여준다.
	- SHM 에 저장하지는 않는다.	
	- RETURN : 동시호가이면 FALSE, 일반이면 TRUE
*/
BOOL CCVT_ETK::de_KS_F_HogaDongsi(char* psOrgData, char* pOut, BOOL *o_bDongsi/*동시호가여부*/)
{
	char	szLen[32], szCode[128], t[128];

	*o_bDongsi = FALSE;

	FH0_OutBlock*	org	= (FH0_OutBlock*) psOrgData;	//	FH0.h
	REAL_HOGA_KS*	p	= (REAL_HOGA_KS*)pOut;			//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_HOGA_KS) );
	*(pOut+sizeof(REAL_HOGA_KS)) = 0x00;
	
	SET_LEN( sizeof(REAL_HOGA_KS), szLen );
	SET_CODE_STR( CODE_REAL_HOGA_KS, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );
	
	//	종목코드
	memcpy(p->stk_code,			org->futcode,		min(sizeof(p->stk_code), sizeof(org->futcode)) );

	//	GDS CODE
	memcpy( p->gds_code, GDS_CODE_KSF, sizeof(p->gds_code));

	//	시각
	memcpy(p->tm,				org->hotime,		min(sizeof(p->tm), 		sizeof(org->hotime)) );
	
	//	매도호가 1
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	
	//SMILOR0803
	if( atof(t)<100)
	{
		SYSTEMTIME st; GetLocalTime(&st);
		char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
	
		if( strcmp(szTime, "10:00")<0 )
			*o_bDongsi = TRUE;
		else
			return FALSE;
	}

	memcpy(p->o_hoga_1, t, sizeof(p->o_hoga_1) );

	//	매도호가 2
	get_double_withdot( org->offerho2, sizeof(org->offerho2), 2, sizeof(p->o_hoga_2), t );
	memcpy(p->o_hoga_2, t, sizeof(p->o_hoga_2) );

	//	매도호가 3
	get_double_withdot( org->offerho3, sizeof(org->offerho3), 2, sizeof(p->o_hoga_3), t );
	memcpy(p->o_hoga_3, t, sizeof(p->o_hoga_3) );

	//	매도호가 4
	get_double_withdot( org->offerho4, sizeof(org->offerho4), 2, sizeof(p->o_hoga_4), t );
	memcpy(p->o_hoga_4, t, sizeof(p->o_hoga_4) );

	//	매도호가 5
	get_double_withdot( org->offerho5, sizeof(org->offerho5), 2, sizeof(p->o_hoga_5), t );
	memcpy(p->o_hoga_5, t, sizeof(p->o_hoga_5) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_1), S2N( org->offerrem1,sizeof(org->offerrem1) ) );
	memcpy(p->o_rmnq_1, 		t, sizeof(p->o_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_2), S2N( org->offerrem2,sizeof(org->offerrem2) ) );
	memcpy(p->o_rmnq_2, 		t, sizeof(p->o_rmnq_2) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_3), S2N( org->offerrem3,sizeof(org->offerrem3) ) );
	memcpy(p->o_rmnq_3, 		t, sizeof(p->o_rmnq_3) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_4), S2N( org->offerrem4,sizeof(org->offerrem4) ) );
	memcpy(p->o_rmnq_4, 		t, sizeof(p->o_rmnq_4) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_5), S2N( org->offerrem5,sizeof(org->offerrem5) ) );
	memcpy(p->o_rmnq_5, 		t, sizeof(p->o_rmnq_5) );

	sprintf( t, "%*d", sizeof(p->o_trmnq), S2N( org->totofferrem,sizeof(org->totofferrem) ) );
	memcpy(p->o_trmnq, 		t, sizeof(p->o_trmnq) );

	sprintf( t, "%*d", sizeof(p->o_cnt_1), S2N( org->offercnt1,sizeof(org->offercnt1) ) );
	memcpy(p->o_cnt_1, 			t, sizeof(p->o_cnt_1) );

	sprintf( t, "%*d", sizeof(p->o_cnt_2), S2N( org->offercnt2,sizeof(org->offercnt2) ) );
	memcpy(p->o_cnt_2, 			t, sizeof(p->o_cnt_2) );

	sprintf( t, "%*d", sizeof(p->o_cnt_3), S2N( org->offercnt3,sizeof(org->offercnt3) ) );
	memcpy(p->o_cnt_3, 			t, sizeof(p->o_cnt_3) );

	sprintf( t, "%*d", sizeof(p->o_cnt_4), S2N( org->offercnt4,sizeof(org->offercnt4) ) );
	memcpy(p->o_cnt_4, 			t, sizeof(p->o_cnt_4) );

	sprintf( t, "%*d", sizeof(p->o_cnt_5), S2N( org->offercnt5,sizeof(org->offercnt5) ) );
	memcpy(p->o_cnt_5, 			t, sizeof(p->o_cnt_5) );

	sprintf( t, "%*d", sizeof(p->o_tot_cnt), S2N( org->totoffercnt,sizeof(org->totoffercnt) ) );
	memcpy(p->o_tot_cnt, 		t, sizeof(p->o_tot_cnt) );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	매수호가 1
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );

	//SMILOR0803
	if( atof(t)<100)
	{
		SYSTEMTIME st; GetLocalTime(&st);
		char szTime[32]; sprintf(szTime, "%02d:%02d", st.wHour, st.wMinute);
		
		if( strcmp(szTime, "10:00")<0 )
			*o_bDongsi = TRUE;
		else
			return FALSE;
	}


	memcpy(p->b_hoga_1, t, sizeof(p->b_hoga_1) );
	
	//	매수호가 2
	get_double_withdot( org->bidho2, sizeof(org->bidho2), 2, sizeof(p->b_hoga_2), t );
	memcpy(p->b_hoga_2, t, sizeof(p->b_hoga_2) );
	
	//	매수호가 3
	get_double_withdot( org->bidho3, sizeof(org->bidho3), 2, sizeof(p->b_hoga_3), t );
	memcpy(p->b_hoga_3, t, sizeof(p->b_hoga_3) );
	
	//	매수호가 4
	get_double_withdot( org->bidho4, sizeof(org->bidho4), 2, sizeof(p->b_hoga_4), t );
	memcpy(p->b_hoga_4, t, sizeof(p->b_hoga_4) );
	
	//	매수호가 5
	get_double_withdot( org->bidho5, sizeof(org->bidho5), 2, sizeof(p->b_hoga_5), t );
	memcpy(p->b_hoga_5, t, sizeof(p->b_hoga_5) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_1), S2N( org->bidrem1,sizeof(org->bidrem1) ) );
	memcpy(p->b_rmnq_1, 		t, sizeof(p->b_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->b_rmnq_2), S2N( org->bidrem2,sizeof(org->bidrem2) ) );
	memcpy(p->b_rmnq_2, 		t, sizeof(p->b_rmnq_2) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_3), S2N( org->bidrem3,sizeof(org->bidrem3) ) );
	memcpy(p->b_rmnq_3, 		t, sizeof(p->b_rmnq_3) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_4), S2N( org->bidrem4,sizeof(org->bidrem4) ) );
	memcpy(p->b_rmnq_4, 		t, sizeof(p->b_rmnq_4) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_5), S2N( org->bidrem5,sizeof(org->bidrem5) ) );
	memcpy(p->b_rmnq_5, 		t, sizeof(p->b_rmnq_5) );
	
	sprintf( t, "%*d", sizeof(p->b_trmnq), S2N( org->totbidrem,sizeof(org->totbidrem) ) );
	memcpy(p->b_trmnq, 			t, sizeof(p->b_trmnq) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_1), S2N( org->bidcnt1,sizeof(org->bidcnt1) ) );
	memcpy(p->b_cnt_1, 			t, sizeof(p->b_cnt_1) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_2), S2N( org->bidcnt2,sizeof(org->bidcnt2) ) );
	memcpy(p->b_cnt_2, 			t, sizeof(p->b_cnt_2) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_3), S2N( org->bidcnt3,sizeof(org->bidcnt3) ) );
	memcpy(p->b_cnt_3, 			t, sizeof(p->b_cnt_3) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_4), S2N( org->bidcnt4,sizeof(org->bidcnt4) ) );
	memcpy(p->b_cnt_4, 			t, sizeof(p->b_cnt_4) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_5), S2N( org->bidcnt5,sizeof(org->bidcnt5) ) );
	memcpy(p->b_cnt_5, 			t, sizeof(p->b_cnt_5) );
	
	sprintf( t, "%*d", sizeof(p->b_tot_cnt), S2N( org->totbidcnt,sizeof(org->totbidcnt) ) );
	memcpy(p->b_tot_cnt, 		t, sizeof(p->b_tot_cnt) );
	
	p->ETX[0] = 0x03;
	p->ETX[1] = 0x00;

	//SMILOR0803
	return TRUE;

}




//CME

BOOL CCVT_ETK::de_KS_F_Hoga_CME(char* psOrgData, char* pOut)
{
	char	szLen[32], szCode[128], t[128];

	NH0_OutBlock*	org	= (NH0_OutBlock*) psOrgData;	//	FH0.h
	REAL_HOGA_KS*	p	= (REAL_HOGA_KS*)pOut;			//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_HOGA_KS) );
	*(pOut+sizeof(REAL_HOGA_KS)) = 0x00;
	
	SET_LEN( sizeof(REAL_HOGA_KS), szLen );
	SET_CODE_STR( CODE_REAL_HOGA_KS, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );
	
	//	종목코드
	memcpy(p->stk_code,			org->futcode,		min(sizeof(p->stk_code), sizeof(org->futcode)) );

	//	GDS CODE
	memcpy( p->gds_code, GDS_CODE_KSF, sizeof(p->gds_code));

	//	시각
	memcpy(p->tm,				org->hotime,		min(sizeof(p->tm), 		sizeof(org->hotime)) );
	
	//	매도호가 1
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	
	//SMILOR0803
	if( atof(t)<100) return FALSE;

	memcpy(p->o_hoga_1, t, sizeof(p->o_hoga_1) );

	//	매도호가 2
	get_double_withdot( org->offerho2, sizeof(org->offerho2), 2, sizeof(p->o_hoga_2), t );
	memcpy(p->o_hoga_2, t, sizeof(p->o_hoga_2) );

	//	매도호가 3
	get_double_withdot( org->offerho3, sizeof(org->offerho3), 2, sizeof(p->o_hoga_3), t );
	memcpy(p->o_hoga_3, t, sizeof(p->o_hoga_3) );

	//	매도호가 4
	get_double_withdot( org->offerho4, sizeof(org->offerho4), 2, sizeof(p->o_hoga_4), t );
	memcpy(p->o_hoga_4, t, sizeof(p->o_hoga_4) );

	//	매도호가 5
	get_double_withdot( org->offerho5, sizeof(org->offerho5), 2, sizeof(p->o_hoga_5), t );
	memcpy(p->o_hoga_5, t, sizeof(p->o_hoga_5) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_1), S2N( org->offerrem1,sizeof(org->offerrem1) ) );
	memcpy(p->o_rmnq_1, 		t, sizeof(p->o_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_2), S2N( org->offerrem2,sizeof(org->offerrem2) ) );
	memcpy(p->o_rmnq_2, 		t, sizeof(p->o_rmnq_2) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_3), S2N( org->offerrem3,sizeof(org->offerrem3) ) );
	memcpy(p->o_rmnq_3, 		t, sizeof(p->o_rmnq_3) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_4), S2N( org->offerrem4,sizeof(org->offerrem4) ) );
	memcpy(p->o_rmnq_4, 		t, sizeof(p->o_rmnq_4) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_5), S2N( org->offerrem5,sizeof(org->offerrem5) ) );
	memcpy(p->o_rmnq_5, 		t, sizeof(p->o_rmnq_5) );

	sprintf( t, "%*d", sizeof(p->o_trmnq), S2N( org->totofferrem,sizeof(org->totofferrem) ) );
	memcpy(p->o_trmnq, 		t, sizeof(p->o_trmnq) );

	sprintf( t, "%*d", sizeof(p->o_cnt_1), S2N( org->offercnt1,sizeof(org->offercnt1) ) );
	memcpy(p->o_cnt_1, 			t, sizeof(p->o_cnt_1) );

	sprintf( t, "%*d", sizeof(p->o_cnt_2), S2N( org->offercnt2,sizeof(org->offercnt2) ) );
	memcpy(p->o_cnt_2, 			t, sizeof(p->o_cnt_2) );

	sprintf( t, "%*d", sizeof(p->o_cnt_3), S2N( org->offercnt3,sizeof(org->offercnt3) ) );
	memcpy(p->o_cnt_3, 			t, sizeof(p->o_cnt_3) );

	sprintf( t, "%*d", sizeof(p->o_cnt_4), S2N( org->offercnt4,sizeof(org->offercnt4) ) );
	memcpy(p->o_cnt_4, 			t, sizeof(p->o_cnt_4) );

	sprintf( t, "%*d", sizeof(p->o_cnt_5), S2N( org->offercnt5,sizeof(org->offercnt5) ) );
	memcpy(p->o_cnt_5, 			t, sizeof(p->o_cnt_5) );

	sprintf( t, "%*d", sizeof(p->o_tot_cnt), S2N( org->totoffercnt,sizeof(org->totoffercnt) ) );
	memcpy(p->o_tot_cnt, 		t, sizeof(p->o_tot_cnt) );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	매수호가 1
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );

	//SMILOR0803
	if( atof(t)<100) return FALSE;

	memcpy(p->b_hoga_1, t, sizeof(p->b_hoga_1) );
	
	//	매수호가 2
	get_double_withdot( org->bidho2, sizeof(org->bidho2), 2, sizeof(p->b_hoga_2), t );
	memcpy(p->b_hoga_2, t, sizeof(p->b_hoga_2) );
	
	//	매수호가 3
	get_double_withdot( org->bidho3, sizeof(org->bidho3), 2, sizeof(p->b_hoga_3), t );
	memcpy(p->b_hoga_3, t, sizeof(p->b_hoga_3) );
	
	//	매수호가 4
	get_double_withdot( org->bidho4, sizeof(org->bidho4), 2, sizeof(p->b_hoga_4), t );
	memcpy(p->b_hoga_4, t, sizeof(p->b_hoga_4) );
	
	//	매수호가 5
	get_double_withdot( org->bidho5, sizeof(org->bidho5), 2, sizeof(p->b_hoga_5), t );
	memcpy(p->b_hoga_5, t, sizeof(p->b_hoga_5) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_1), S2N( org->bidrem1,sizeof(org->bidrem1) ) );
	memcpy(p->b_rmnq_1, 		t, sizeof(p->b_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->b_rmnq_2), S2N( org->bidrem2,sizeof(org->bidrem2) ) );
	memcpy(p->b_rmnq_2, 		t, sizeof(p->b_rmnq_2) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_3), S2N( org->bidrem3,sizeof(org->bidrem3) ) );
	memcpy(p->b_rmnq_3, 		t, sizeof(p->b_rmnq_3) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_4), S2N( org->bidrem4,sizeof(org->bidrem4) ) );
	memcpy(p->b_rmnq_4, 		t, sizeof(p->b_rmnq_4) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_5), S2N( org->bidrem5,sizeof(org->bidrem5) ) );
	memcpy(p->b_rmnq_5, 		t, sizeof(p->b_rmnq_5) );
	
	sprintf( t, "%*d", sizeof(p->b_trmnq), S2N( org->totbidrem,sizeof(org->totbidrem) ) );
	memcpy(p->b_trmnq, 			t, sizeof(p->b_trmnq) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_1), S2N( org->bidcnt1,sizeof(org->bidcnt1) ) );
	memcpy(p->b_cnt_1, 			t, sizeof(p->b_cnt_1) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_2), S2N( org->bidcnt2,sizeof(org->bidcnt2) ) );
	memcpy(p->b_cnt_2, 			t, sizeof(p->b_cnt_2) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_3), S2N( org->bidcnt3,sizeof(org->bidcnt3) ) );
	memcpy(p->b_cnt_3, 			t, sizeof(p->b_cnt_3) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_4), S2N( org->bidcnt4,sizeof(org->bidcnt4) ) );
	memcpy(p->b_cnt_4, 			t, sizeof(p->b_cnt_4) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_5), S2N( org->bidcnt5,sizeof(org->bidcnt5) ) );
	memcpy(p->b_cnt_5, 			t, sizeof(p->b_cnt_5) );
	
	sprintf( t, "%*d", sizeof(p->b_tot_cnt), S2N( org->totbidcnt,sizeof(org->totbidcnt) ) );
	memcpy(p->b_tot_cnt, 		t, sizeof(p->b_tot_cnt) );
	
	p->ETX[0] = 0x03;
	p->ETX[1] = 0x00;

	//SMILOR0803
	return TRUE;

}

VOID CCVT_ETK::de_KS_O_Sise(char* psOrgData, char* pOut)
{	
	char	szLen[32], szCode[128];
	char	t[128];

	OC0_OutBlock*	org	= (OC0_OutBlock*) psOrgData;	//	FC0.h
	REAL_SISE_KS*	p	= (REAL_SISE_KS*)pOut;			//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_SISE_KS) );

	SET_LEN( sizeof(REAL_SISE_KS), szLen );
	SET_CODE_STR( CODE_REAL_SISE_KS, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );

	memcpy(p->stk_code,			org->optcode,		min(sizeof(p->stk_code), sizeof(org->optcode)) );
	memcpy( p->gds_code, GDS_CODE_KSO, sizeof(p->gds_code));
	memcpy(p->tm,				org->chetime,	min(sizeof(p->tm), 	sizeof(org->chetime)) );

	//	현재가
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(p->now_prc), t );
	memcpy(p->now_prc, t, sizeof(p->now_prc) );

	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(p->open), t );
	memcpy(p->open, t, sizeof(p->open) );

	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(p->high), t );
	memcpy(p->high, t, sizeof(p->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(p->low), t );
	memcpy(p->low, t, sizeof(p->low) );

	if( org->cgubun[0]=='-' )
		p->buy_sell_tp[0] = '1';
	else
		p->buy_sell_tp[0] = '2';
	
	//	누적체결수량
	sprintf( t, "%*d", sizeof(p->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(p->acml_cntr_qty,	t,		sizeof(p->acml_cntr_qty) );
//	memcpy(p->acml_buy_cntr_qty,org->bidvol,		min(sizeof(p->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(p->acml_amt), t );
	memcpy(p->acml_amt,			t,  sizeof(p->acml_amt) );

	switch(org->sign[0])
	{
	case '2':	p->daebi_sign[0] = '+';	break;
	case '5':	p->daebi_sign[0] = '-';	break;
	default:	p->daebi_sign[0] = ' ';	break;
	}
	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(p->chg), t );
	memcpy(p->chg,				t, sizeof(p->chg) );

	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(p->chg_rate), t );
	memcpy(p->chg_rate,			t, sizeof(p->chg_rate) );

	sprintf( t, "%*d", sizeof(p->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(p->cntr_qty, 		t, 	sizeof(p->cntr_qty) );
 	
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	memcpy(p->o_hoga_1, 		t, sizeof(p->o_hoga_1) );

	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );
 	memcpy(p->b_hoga_1, 		t, sizeof(p->b_hoga_1) );

	get_double_withdot( org->theoryprice, sizeof(org->theoryprice), 2, sizeof(p->theory_prc), t );
	memcpy(p->theory_prc, t, sizeof(p->theory_prc) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(p->open_yak), t );
	memcpy(p->open_yak, 		t, 	sizeof(p->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(p->open_yak_chg), t );
 	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(p->ks200), t );
 	memcpy(p->ks200,			t, sizeof(p->ks200) );
 	
	get_double_withdot( org->impv, sizeof(org->impv), 2, sizeof(p->imp_v), t );
	memcpy(p->imp_v,			t, sizeof(p->imp_v) );

	get_double_withdot( org->timevalue, sizeof(org->timevalue), 2, sizeof(p->tm_value), t );
 	memcpy(p->tm_value,			t, sizeof(p->tm_value) );
	
	p->ETX[0] = 0x03;
	p->ETX[1] = 0x00;

}


VOID CCVT_ETK::de_KS_O_SiseEx(char* psOrgData, char* pOut, char* pOutForClient)
{	
	char	szLen[32], szCode[128];
	char	t[128];
	
	OC0_OutBlock*	org	= (OC0_OutBlock*) psOrgData;	//	FC0.h
	REAL_SISE_KS*	p	= (REAL_SISE_KS*)pOut;			//	PacketDef.h

	
	memset( p, 0x20, sizeof(REAL_SISE_KS) );
	

	//////////////////////////////////////////////////////////////////////////
	//	SHM 을 위한 패킷
	SET_LEN( sizeof(REAL_SISE_KS), szLen );
	SET_CODE_STR( CODE_REAL_SISE_KS, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );
	
	memcpy(p->stk_code,			org->optcode,		min(sizeof(p->stk_code), sizeof(org->optcode)) );
	memcpy( p->gds_code, GDS_CODE_KSO, sizeof(p->gds_code));
	memcpy(p->tm,				org->chetime,	min(sizeof(p->tm), 	sizeof(org->chetime)) );
	
	//	현재가
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(p->now_prc), t );
	memcpy(p->now_prc, t, sizeof(p->now_prc) );
	
	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(p->open), t );
	memcpy(p->open, t, sizeof(p->open) );
	
	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(p->high), t );
	memcpy(p->high, t, sizeof(p->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(p->low), t );
	memcpy(p->low, t, sizeof(p->low) );
	
	if( org->cgubun[0]=='-' )
		p->buy_sell_tp[0] = '1';
	else
		p->buy_sell_tp[0] = '2';
	
	//	누적체결수량
	sprintf( t, "%*d", sizeof(p->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(p->acml_cntr_qty,	t,		sizeof(p->acml_cntr_qty) );
	//	memcpy(p->acml_buy_cntr_qty,org->bidvol,		min(sizeof(p->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(p->acml_amt), t );
	memcpy(p->acml_amt,			t,  sizeof(p->acml_amt) );
	
	switch(org->sign[0])
	{
	case '2':	p->daebi_sign[0] = '+';	break;
	case '5':	p->daebi_sign[0] = '-';	break;
	default:	p->daebi_sign[0] = ' ';	break;
	}
	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(p->chg), t );
	memcpy(p->chg,				t, sizeof(p->chg) );
	
	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(p->chg_rate), t );
	memcpy(p->chg_rate,			t, sizeof(p->chg_rate) );
	
	sprintf( t, "%*d", sizeof(p->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(p->cntr_qty, 		t, 	sizeof(p->cntr_qty) );
	
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	memcpy(p->o_hoga_1, 		t, sizeof(p->o_hoga_1) );
	
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );
	memcpy(p->b_hoga_1, 		t, sizeof(p->b_hoga_1) );
	
	get_double_withdot( org->theoryprice, sizeof(org->theoryprice), 2, sizeof(p->theory_prc), t );
	memcpy(p->theory_prc, t, sizeof(p->theory_prc) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(p->open_yak), t );
	memcpy(p->open_yak, 		t, 	sizeof(p->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(p->open_yak_chg), t );
	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );
	
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(p->ks200), t );
	memcpy(p->ks200,			t, sizeof(p->ks200) );
	
	get_double_withdot( org->impv, sizeof(org->impv), 2, sizeof(p->imp_v), t );
	memcpy(p->imp_v,			t, sizeof(p->imp_v) );
	
	get_double_withdot( org->timevalue, sizeof(org->timevalue), 2, sizeof(p->tm_value), t );
	memcpy(p->tm_value,			t, sizeof(p->tm_value) );
	
	p->ETX[0] = 0x03;
	p->ETX[1] = 0x00;
	
	//////////////////////////////////////////////////////////////////////////
	//	FRONT 을 위한 패킷
	REAL_SISE_KS_84003*	pFront	= (REAL_SISE_KS_84003*)pOutForClient;	//	PacketDef.h
	memset( pFront, 0x20, sizeof(REAL_SISE_KS_84003) );
	SET_LEN( sizeof(REAL_SISE_KS_84003), szLen );
	SET_CODE_STR( 84003, szCode );
	
	pFront->STX[0]	=	0x02;
	memcpy( pFront->Header.Len, szLen, sizeof(pFront->Header.Len) );	
	memcpy( pFront->Header.Code, szCode, sizeof(pFront->Header.Code) );
	
	memcpy(pFront->stk_code,	org->optcode,		min(sizeof(pFront->stk_code), sizeof(org->optcode)) );
	memcpy( pFront->gds_code,	GDS_CODE_KSO, sizeof(pFront->gds_code));
	memcpy(pFront->tm,			org->chetime,	min(sizeof(pFront->tm), 	sizeof(org->chetime)) );
	
	//	현재가
	get_double_withdot( org->price, sizeof(org->price), 2, sizeof(pFront->now_prc), t );
	memcpy(pFront->now_prc, t, sizeof(pFront->now_prc) );
	
	//	시가
	get_double_withdot( org->open, sizeof(org->open), 2, sizeof(pFront->open), t );
	memcpy(pFront->open, t, sizeof(pFront->open) );
	
	//	고가
	get_double_withdot( org->high, sizeof(org->high), 2, sizeof(pFront->high), t );
	memcpy(pFront->high, t, sizeof(pFront->high) );
	
	//	저가
	get_double_withdot( org->low, sizeof(org->low), 2, sizeof(pFront->low), t );
	memcpy(pFront->low, t, sizeof(pFront->low) );
	
	if( org->cgubun[0]=='-' )
		pFront->buy_sell_tp[0] = '1';
	else
		pFront->buy_sell_tp[0] = '2';
	
	//	누적체결수량
	sprintf( t, "%*d", sizeof(pFront->acml_cntr_qty), S2N(org->volume,sizeof(org->volume)) );
	memcpy(pFront->acml_cntr_qty,	t,		sizeof(pFront->acml_cntr_qty) );
	//	memcpy(pFront->acml_buy_cntr_qty,org->bidvol,		min(sizeof(pFront->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	get_double_withdot( org->value, sizeof(org->value), 0, sizeof(pFront->acml_amt), t );
	memcpy(pFront->acml_amt,			t,  sizeof(pFront->acml_amt) );
	
	switch(org->sign[0])
	{
	case '2':	pFront->daebi_sign[0] = '+';	break;
	case '5':	pFront->daebi_sign[0] = '-';	break;
	default:	pFront->daebi_sign[0] = ' ';	break;
	}
	get_double_withdot( org->change, sizeof(org->change), 2, sizeof(pFront->chg), t );
	memcpy(pFront->chg,				t, sizeof(pFront->chg) );
	
	get_double_withdot( org->drate, sizeof(org->drate), 2, sizeof(pFront->chg_rate), t );
	memcpy(pFront->chg_rate,			t, sizeof(pFront->chg_rate) );
	
	sprintf( t, "%*d", sizeof(pFront->cntr_qty), S2N(org->cvolume,sizeof(org->cvolume)) );
	memcpy(pFront->cntr_qty, 		t, 	sizeof(pFront->cntr_qty) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(pFront->open_yak), t );
	memcpy(pFront->open_yak, 		t, 	sizeof(pFront->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(pFront->open_yak_chg), t );
	memcpy(pFront->open_yak_chg, 	t, 	sizeof(pFront->open_yak_chg) );
	
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(pFront->ks200), t );
	memcpy(pFront->ks200,			t, sizeof(pFront->ks200) );
	
	
	pFront->ETX[0] = 0x03;
	pFront->ETX[1] = 0x00;
	
}

VOID CCVT_ETK::de_KS_O_Hoga(char* psOrgData, char* pOut)
{
	char	szLen[32], szCode[128], t[128];
	
	OH0_OutBlock*	org	= (OH0_OutBlock*) psOrgData;		//	DrdsSC0.h
	REAL_HOGA_KS*	p	= (REAL_HOGA_KS*)pOut;			//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_HOGA_KS) );

	SET_LEN( sizeof(REAL_HOGA_KS), szLen );
	SET_CODE_STR( CODE_REAL_HOGA_KS, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );
 	
 	memcpy(p->stk_code,			org->optcode,		min(sizeof(p->stk_code), sizeof(org->optcode)) );
 	memcpy(p->gds_code, GDS_CODE_KSO, sizeof(p->gds_code) );
 	memcpy(p->tm,				org->hotime,		min(sizeof(p->tm), 		sizeof(org->hotime)) );
// 
	//	매도호가 1
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	memcpy(p->o_hoga_1, t, sizeof(p->o_hoga_1) );
	
	//	매도호가 2
	get_double_withdot( org->offerho2, sizeof(org->offerho2), 2, sizeof(p->o_hoga_2), t );
	memcpy(p->o_hoga_2, t, sizeof(p->o_hoga_2) );
	
	//	매도호가 3
	get_double_withdot( org->offerho3, sizeof(org->offerho3), 2, sizeof(p->o_hoga_3), t );
	memcpy(p->o_hoga_3, t, sizeof(p->o_hoga_3) );
	
	//	매도호가 4
	get_double_withdot( org->offerho4, sizeof(org->offerho4), 2, sizeof(p->o_hoga_4), t );
	memcpy(p->o_hoga_4, t, sizeof(p->o_hoga_4) );
	
	//	매도호가 5
	get_double_withdot( org->offerho5, sizeof(org->offerho5), 2, sizeof(p->o_hoga_5), t );
	memcpy(p->o_hoga_5, t, sizeof(p->o_hoga_5) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_1), S2N(org->offerrem1, sizeof(org->offerrem1)) );
	memcpy(p->o_rmnq_1, 		t, sizeof(p->o_rmnq_1) );
	
	sprintf( t, "%*d", sizeof(p->o_rmnq_2), S2N(org->offerrem2, sizeof(org->offerrem2)) );
	memcpy(p->o_rmnq_2, 		t, sizeof(p->o_rmnq_2) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_3), S2N(org->offerrem3, sizeof(org->offerrem3)) );
	memcpy(p->o_rmnq_3, 		t, sizeof(p->o_rmnq_3) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_4), S2N(org->offerrem4, sizeof(org->offerrem4)) );
	memcpy(p->o_rmnq_4, 		t, sizeof(p->o_rmnq_4) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_5), S2N(org->offerrem5, sizeof(org->offerrem5)) );
	memcpy(p->o_rmnq_5, 		t, sizeof(p->o_rmnq_5) );

	sprintf( t, "%*d", sizeof(p->o_trmnq), S2N(org->totofferrem, sizeof(org->totofferrem)) );
	memcpy(p->o_trmnq, 		t, sizeof(p->o_trmnq) );

	sprintf( t, "%*d", sizeof(p->o_cnt_1), S2N( org->offercnt1,sizeof(org->offercnt1) ) );
	memcpy(p->o_cnt_1, 			t, sizeof(p->o_cnt_1) );
	
	sprintf( t, "%*d", sizeof(p->o_cnt_2), S2N( org->offercnt2,sizeof(org->offercnt2) ) );
	memcpy(p->o_cnt_2, 			t, sizeof(p->o_cnt_2) );
	
	sprintf( t, "%*d", sizeof(p->o_cnt_3), S2N( org->offercnt3,sizeof(org->offercnt3) ) );
	memcpy(p->o_cnt_3, 			t, sizeof(p->o_cnt_3) );
	
	sprintf( t, "%*d", sizeof(p->o_cnt_4), S2N( org->offercnt4,sizeof(org->offercnt4) ) );
	memcpy(p->o_cnt_4, 			t, sizeof(p->o_cnt_4) );
	
	sprintf( t, "%*d", sizeof(p->o_cnt_5), S2N( org->offercnt5,sizeof(org->offercnt5) ) );
	memcpy(p->o_cnt_5, 			t, sizeof(p->o_cnt_5) );

	sprintf( t, "%*d", sizeof(p->o_tot_cnt), S2N( org->totoffercnt,sizeof(org->totoffercnt) ) );
	memcpy(p->o_tot_cnt, 			t, sizeof(p->o_tot_cnt) );

	//	매수호가 1
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );
	memcpy(p->b_hoga_1, t, sizeof(p->b_hoga_1) );
	
	//	매수호가 2
	get_double_withdot( org->bidho2, sizeof(org->bidho2), 2, sizeof(p->b_hoga_2), t );
	memcpy(p->b_hoga_2, t, sizeof(p->b_hoga_2) );
	
	//	매수호가 3
	get_double_withdot( org->bidho3, sizeof(org->bidho3), 2, sizeof(p->b_hoga_3), t );
	memcpy(p->b_hoga_3, t, sizeof(p->b_hoga_3) );
	
	//	매수호가 4
	get_double_withdot( org->bidho4, sizeof(org->bidho4), 2, sizeof(p->b_hoga_4), t );
	memcpy(p->b_hoga_4, t, sizeof(p->b_hoga_4) );
	
	//	매수호가 5
	get_double_withdot( org->bidho5, sizeof(org->bidho5), 2, sizeof(p->b_hoga_5), t );
	memcpy(p->b_hoga_5, t, sizeof(p->b_hoga_5) );

	sprintf( t, "%*d", sizeof(p->b_rmnq_1), S2N( org->bidrem1,sizeof(org->bidrem1) ) );
	memcpy(p->b_rmnq_1, 		t, sizeof(p->b_rmnq_1) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_2), S2N( org->bidrem2,sizeof(org->bidrem2) ) );
	memcpy(p->b_rmnq_2, 		t, sizeof(p->b_rmnq_2) );

	sprintf( t, "%*d", sizeof(p->b_rmnq_3), S2N( org->bidrem3,sizeof(org->bidrem3) ) );
	memcpy(p->b_rmnq_3, 		t, sizeof(p->b_rmnq_3) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_4), S2N( org->bidrem4,sizeof(org->bidrem4) ) );
	memcpy(p->b_rmnq_4, 		t, sizeof(p->b_rmnq_4) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_5), S2N( org->bidrem5,sizeof(org->bidrem5) ) );
	memcpy(p->b_rmnq_5, 		t, sizeof(p->b_rmnq_5) );
	
	sprintf( t, "%*d", sizeof(p->b_trmnq), S2N( org->totbidrem,sizeof(org->totbidrem) ) );
	memcpy(p->b_trmnq, 		t, sizeof(p->b_trmnq) );

	sprintf( t, "%*d", sizeof(p->b_cnt_1), S2N( org->bidcnt1,sizeof(org->bidcnt1) ) );
	memcpy(p->b_cnt_1, 			t, sizeof(p->b_cnt_1) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_2), S2N( org->bidcnt2,sizeof(org->bidcnt2) ) );
	memcpy(p->b_cnt_2, 			t, sizeof(p->b_cnt_2) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_3), S2N( org->bidcnt3,sizeof(org->bidcnt3) ) );
	memcpy(p->b_cnt_3, 			t, sizeof(p->b_cnt_3) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_4), S2N( org->bidcnt4,sizeof(org->bidcnt4) ) );
	memcpy(p->b_cnt_4, 			t, sizeof(p->b_cnt_4) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_5), S2N( org->bidcnt5,sizeof(org->bidcnt5) ) );
	memcpy(p->b_cnt_5, 			t, sizeof(p->b_cnt_5) );

	sprintf( t, "%*d", sizeof(p->b_tot_cnt), S2N( org->totbidcnt,sizeof(org->totbidcnt) ) );
	memcpy(p->b_tot_cnt, 			t, sizeof(p->b_tot_cnt) );
// 	
// // 	memcpy(p->o_hoga_cnt_chg_1, org->offcnt1_chg, 	min(sizeof(p->o_hoga_cnt_chg_1),	sizeof(org->offcnt1_chg) ) );
// // 	memcpy(p->o_hoga_cnt_chg_2, org->offcnt2_chg, 	min(sizeof(p->o_hoga_cnt_chg_2),	sizeof(org->offcnt2_chg) ) );
// // 	memcpy(p->o_hoga_cnt_chg_3, org->offcnt3_chg, 	min(sizeof(p->o_hoga_cnt_chg_3),	sizeof(org->offcnt3_chg) ) );
// // 	memcpy(p->o_hoga_cnt_chg_4, org->offcnt4_chg, 	min(sizeof(p->o_hoga_cnt_chg_4),	sizeof(org->offcnt4_chg) ) );
// // 	memcpy(p->o_hoga_cnt_chg_5, org->offcnt5_chg, 	min(sizeof(p->o_hoga_cnt_chg_5),	sizeof(org->offcnt5_chg) ) );
// // 	memcpy(p->b_hoga_cnt_chg_1, org->bidcnt1_chg, 	min(sizeof(p->b_hoga_cnt_chg_1),	sizeof(org->bidcnt1_chg) ) );
// // 	memcpy(p->b_hoga_cnt_chg_2, org->bidcnt2_chg, 	min(sizeof(p->b_hoga_cnt_chg_2),	sizeof(org->bidcnt2_chg) ) );
// // 	memcpy(p->b_hoga_cnt_chg_3, org->bidcnt3_chg, 	min(sizeof(p->b_hoga_cnt_chg_3),	sizeof(org->bidcnt3_chg) ) );
// // 	memcpy(p->b_hoga_cnt_chg_4, org->bidcnt4_chg, 	min(sizeof(p->b_hoga_cnt_chg_4),	sizeof(org->bidcnt4_chg) ) );
// // 	memcpy(p->b_hoga_cnt_chg_5, org->bidcnt5_chg, 	min(sizeof(p->b_hoga_cnt_chg_5),	sizeof(org->bidcnt5_chg) ) );
// // 	
 	p->ETX[0] = 0x03;
 	p->ETX[1] = 0x00;
	

}



void CCVT_ETK::ETK_SetPacketData( char* psData, int nSize, LPCTSTR pszSrc, int nType, int nDotPos )
{
	//-----------------------------------------------------------------------
	// 문자열
	if( nType == DATA_TYPE_STRING )
	{
		// 왼쪽 정렬
		// 뒤의 빈자리는 ' ' 로 채움

		// 버퍼크기가 원 데이터보다 작다면 데이터는 짤려야 하므로 에러발생
		int nSrcLen = strlen( pszSrc );
		ASSERT( nSize >= nSrcLen );

		// 먼저 Space를 채우고
		FillMemory( psData, nSize, ' ' );

		// 앞에부터 데이터를 넣는다.
		// 원데이터가 크다면 뒷부분을 버린다.
		CopyMemory( psData, pszSrc, min( nSize, nSrcLen ) );
	}

	//-----------------------------------------------------------------------
	// 정수
	else if( nType == DATA_TYPE_LONG )
	{
		// 오른쪽 정렬
		// 앞의 빈자리는 '0' 으로 채움

		// 버퍼크기가 원 데이터보다 작다면 데이터는 짤려야 하므로 에러발생
		int nSrcLen = strlen( pszSrc );
		ASSERT( nSize >= nSrcLen );

		// 먼저 0 으로 채우고
		FillMemory( psData, nSize, '0' );

		// 뒤에서부터 데이터를 넣는다.
		if( nSize >= nSrcLen )
		{
			CopyMemory( psData+nSize-nSrcLen, pszSrc, nSrcLen );
		}
		// 원데이터가 크다면 원데이터의 뒷부분을 버린다.
		else
		{
			CopyMemory( psData, pszSrc, nSize );
		}
	}

	//-----------------------------------------------------------------------
	// 실수 : 소숫점을 찍지 않는다.
	else if( nType == DATA_TYPE_FLOAT )
	{
		// 소숫점 위치를 기준으로 정렬
		// 소숫점을 찍지 않으며 정수부의 빈자리와 소수부의 빈자리는 0으로 채움

		int nSrcLen = strlen( pszSrc );

		// 먼저 0 으로 채우고
		FillMemory( psData, nSize, '0' );

		// 원데이터에서 소숫점의 위치를 찾아서
		// 원데이터의 정수부의 길이와 소수부의 길이를 구한다.
		int nSrcIntLen;
		int nSrcDotLen;
		LPCSTR psz = _tcschr( pszSrc, '.' );
		if( psz == NULL )		// 소수부가 없다.
		{
			nSrcIntLen = strlen( pszSrc );
			nSrcDotLen = 0;
		}
		else					// 소수부가 있다.
		{
			nSrcIntLen = psz - pszSrc;
			nSrcDotLen = strlen( pszSrc ) - nSrcIntLen - 1;
		}

		// 정수부를 넣는다.
		if( nSize-nDotPos >= nSrcIntLen )
		{
			CopyMemory( psData+nSize-nDotPos-nSrcIntLen, pszSrc, nSrcIntLen );
		}
		else
		{
			// 원데이터의 정수부 길이가 더 긴 경우 정수부의 뒷자리는 삭제된다.
			ASSERT( FALSE );
			CopyMemory( psData, pszSrc, nSize-nDotPos );
		}

		// 소수부를 넣는데 원데이터의 소수부 길이가 더 긴 경우 소수부의 뒷자리는 삭제된다.
		ASSERT( nDotPos >= nSrcDotLen );
		CopyMemory( psData+nSize-nDotPos, pszSrc + strlen( pszSrc ) - nSrcDotLen, min( nDotPos, nSrcDotLen ) );
	}

	//-----------------------------------------------------------------------
	// 실수 : 소숫점을 포함
	else if( nType == DATA_TYPE_FLOAT_DOT )
	{
		// 소숫점 위치를 기준으로 정렬
		// 소숫점을 찍지 않으며 정수부의 빈자리와 소수부의 빈자리는 0으로 채움
		
		int nSrcLen = strlen( pszSrc );
		
		// 먼저 0 으로 채우고
		FillMemory( psData, nSize, '0' );
		
		// 원데이터에서 소숫점의 위치를 찾아서
		// 원데이터의 정수부의 길이와 소수부의 길이를 구한다.
		int nSrcIntLen;
		int nSrcDotLen;
		LPCSTR psz = _tcschr( pszSrc, '.' );
		if( psz == NULL )		// 소수부가 없다.
		{
			nSrcIntLen = strlen( pszSrc );
			nSrcDotLen = 0;
		}
		else					// 소수부가 있다.
		{
			nSrcIntLen = psz - pszSrc;
			nSrcDotLen = strlen( pszSrc ) - nSrcIntLen - 1;
		}
		
		// 정수부를 넣는다.
		if( nSize-nDotPos-1 >= nSrcIntLen )
		{
			CopyMemory( psData+nSize-nDotPos-nSrcIntLen-1, pszSrc, nSrcIntLen );
		}
		else
		{
			// 원데이터의 정수부 길이가 더 긴 경우 정수부의 뒷자리는 삭제된다.
			ASSERT( FALSE );
			CopyMemory( psData, pszSrc, nSize-nDotPos-1 );
		}

		// 소숫점을 찍는다.
		psData[nSize-nDotPos-1] = '.';
		
		// 소수부를 넣는데 원데이터의 소수부 길이가 더 긴 경우 소수부의 뒷자리는 삭제된다.
		ASSERT( nDotPos >= nSrcDotLen );
		CopyMemory( psData+nSize-nDotPos, pszSrc + strlen( pszSrc ) - nSrcDotLen, min( nDotPos, nSrcDotLen ) );
	}
}


//	ETK 패킷은 소숫점이 없다. 이를 소수점이 있는 숫자로 만든다.
//	소숫점이 있는 경우와 없는 경우가 있다.
char* CCVT_ETK::get_double_withdot( char* i_psOrg, int i_nOrgLen, int i_nDotCnt, int i_nOutLen, char* o_psOut )
{
	char	t[128], t2[128];
	double	dTemp;
	
	sprintf( t, "%.*s", i_nOrgLen, i_psOrg );

	//	소숫점 있는 경우
	if( strchr( t, '.' )>0 )
	{
		sprintf( t, "%.*s", i_nOrgLen, i_psOrg );
		dTemp = atof(t);
		sprintf( o_psOut, "%*.*f",i_nOutLen, i_nDotCnt, dTemp );
		return o_psOut;
	}

	//	소숫점이 없는 경우
	int nPos = i_nOrgLen - i_nDotCnt;
	sprintf( t, "%.*s", i_nDotCnt, i_psOrg+nPos );
	sprintf( t2, "%.*s.%s", nPos, i_psOrg, t );
	dTemp = atof(t2);
	sprintf( o_psOut, "%*.*f", i_nOutLen, i_nDotCnt, dTemp );
	return o_psOut;
}


//	ETK 패킷은 소숫점이 없다. 이를 소수점이 있는 숫자로 만든다.
//	소숫점이 있는 경우와 없는 경우가 있다.
char* CCVT_ETK::get_double_withdot2( char* i_psStkCode, char* i_psOrg, int i_nOrgLen, int i_nOutLen, char* o_psOut )
{
	char	t[128], t2[128];
	double	dTemp;
	int nDotCnt;

	switch(i_psStkCode[1]) 
	{
	case 'E':	//6E
		nDotCnt = 5;
		break;
	case 'A':	//6A	
	case 'B':	//6B
    case 'D':	
		if( i_psStkCode[0]=='F')
			nDotCnt = 1;	//FDAX
		if( i_psStkCode[0]=='A')
			nDotCnt = 4;	//AD
		if( i_psStkCode[0]=='C')
			nDotCnt = 4;	//CD
		if( i_psStkCode[0]=='6')
			nDotCnt = 4;	//CD
		break;
	case 'J':	//6J
		nDotCnt = 1;
		break;
	case 'L':	//CL
		nDotCnt = 2;	//CL
		break;
	case 'S':
		if( i_psStkCode[0]=='E' )	nDotCnt = 2;	//ES
		if( i_psStkCode[0]=='H' )	nDotCnt = 0;	//HSI(항생지수)
		break;
	case 'C':
		if( i_psStkCode[0]=='G')
			nDotCnt = 1;	//GC
		if( i_psStkCode[0]=='S')
			nDotCnt = 1;	//SCN (CHINA A50 )
		break;
	case 'Q':	
		nDotCnt = 2;	//NASDAQ
		break;
	case 'G':	
		nDotCnt = 3;	//NG
		break;
	case 'I':	
		nDotCnt = 3;	//SIU (SILVER)
		break;
	case 'M':
		nDotCnt	= 0;	//YM(DOW)
		break;
	case 'Y':
		nDotCnt	= 1;	//JY
		break;
	}
	
	sprintf( t, "%.*s", i_nOrgLen, i_psOrg );
	
	//	소숫점 있는 경우
	if( strchr( t, '.' )>0 )
	{
		sprintf( t, "%.*s", i_nOrgLen, i_psOrg );
		dTemp = atof(t);
		sprintf( o_psOut, "%*.*f",i_nOutLen, nDotCnt, dTemp );
		return o_psOut;
	}
	
	//	소숫점이 없는 경우
	int nPos = i_nOrgLen - nDotCnt;
	sprintf( t, "%.*s", nDotCnt, i_psOrg+nPos );
	sprintf( t2, "%.*s.%s", nPos, i_psOrg, t );
	dTemp = atof(t2);
	sprintf( o_psOut, "%*.*f", i_nOutLen, nDotCnt, dTemp );
	return o_psOut;
}

/*******************************************************
	주문REAL 데이터 CONVERT
*******************************************************/
VOID CCVT_ETK::de_KS_RL_ORD(char* psOrgData, char* pOut)
{	
	char	t[128];

	O01_OutBlock*			org	= (O01_OutBlock*) psOrgData;		//	O01.h
	ST_IFRECV_REAL_CNTR*	p	= (ST_IFRECV_REAL_CNTR*)pOut;		//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_SISE_KS) );

	memcpy( p->ORD_RSLT_CODE,	CODE_SVR_RSLT_NEW_S,	sizeof(p->ORD_RSLT_CODE) );
	memcpy( p->API_CODE,		APIKEY_KS_ETK,			min( strlen(APIKEY_KS_ETK), sizeof(p->API_CODE)) );
	memcpy( p->STK_CODE,		org->fnoIsuno,			sizeof(org->fnoIsuno) );
	memcpy( p->API_USERID,		org->userid,			sizeof(org->userid) );
	memcpy( p->API_ACNT_NO,		org->accno,				sizeof(org->accno) );
	
	//////////////////////////////////////////////////////////////////////////
	//	ETK 주문번호 : 총 10자리, 우리는 9 자리, 따라서 제일 앞 1자리 빼고 처리
	sprintf( t, "%.2s%.*s", org->ordno1+1, sizeof(org->ordno), org->ordno );
	memcpy( p->API_ORD_NO,		t,						strlen(t) );
	
	//	***	패킷에 이에 해당하는 필드가 없다.	***
	//	memcpy( p->ClORD_NO		[MAX_ORD_NO];

	//	***	패킷에 시장가, 지정가 여부가 없다.	***
	//	char	ORD_ST			[1];
	
	p->PROC_TP[0]	= ORD_PROC_NEW;

	memcpy( p->BUY_SELL_TP,		org->bnstp,				sizeof(org->bnstp) );

	sprintf( t, "%*d",			sizeof(p->ORD_QTY),		S2N(org->ordqty,sizeof(org->ordqty)) );
	memcpy( p->ORD_QTY,			t,						sizeof(p->ORD_QTY) );

	get_double_withdot( org->ordprc, sizeof(org->ordprc), 2, sizeof(p->ORD_PRC), t );
	memcpy( p->ORD_PRC,			t,						sizeof(p->ORD_PRC)			);

	sprintf( t, "%*d",			sizeof(p->REMN_QTY),	S2N(org->unercqty, sizeof(org->unercqty)) );
	memcpy(p->REMN_QTY,			t,						sizeof(p->REMN_QTY) );

	SYSTEMTIME st; GetLocalTime(&st);
	sprintf( t, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay );
	memcpy( p->DT,				t,						sizeof(p->DT) );
	memcpy( p->TM,				org->trxtime,			sizeof(p->TM) );

}



/*******************************************************
	정정, 취소 REAL 데이터 CONVERT
*******************************************************/
VOID CCVT_ETK::de_KS_RL_MDFYCNCL(char* psOrgData, char* pOut)
{
	char	t[128];
	
	H01_OutBlock*			org	= (H01_OutBlock*) psOrgData;		//	O01.h
	ST_IFRECV_REAL_CNTR*	p	= (ST_IFRECV_REAL_CNTR*)pOut;		//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_SISE_KS) );
	
	memcpy( p->API_CODE,		APIKEY_KS_ETK,			min(strlen(APIKEY_KS_ETK),sizeof(p->API_CODE)) );

	//	KR4301EC2520 의 형태이다.
	memcpy( p->STK_CODE,		org->expcode+3,			sizeof(org->expcode)-4 );

	//	***	패킷에 USERID 가 없다.	***	//
	//	memcpy( p->API_USERID,		org->userid,			sizeof(org->userid) );
	memcpy( p->API_ACNT_NO,		org->accno,				sizeof(org->accno) );
	
	//////////////////////////////////////////////////////////////////////////
	//	ETK 주문번호 : 총 10자리, 우리는 9 자리, 따라서 제일 앞 1자리 빼고 처리
	sprintf( t, "%.*s", sizeof(org->ordno)-1, org->ordno+1 );
	memcpy( p->API_ORD_NO,		t,						sizeof(p->API_ORD_NO) );

	//	***	패킷에 없다.	***	//
	//	memcpy( p->ClORD_NO		[MAX_ORD_NO];
	
	//////////////////////////////////////////////////////////////////////////
	//	ETK 주문번호 : 총 10자리, 우리는 9 자리, 따라서 제일 앞 1자리 빼고 처리
	sprintf( t, "%.*s", sizeof(org->orgordno)-1, org->orgordno+1 );
	memcpy( p->API_ORD_ORG_NO,	t,						sizeof(p->API_ORD_ORG_NO) );

	switch (org->mocagb[0])
	{
	case '1':
		p->PROC_TP[0] = ORD_PROC_NEW;
		memcpy( p->ORD_RSLT_CODE,	CODE_SVR_RSLT_NEW_S,	sizeof(p->ORD_RSLT_CODE) );
		break;
	case '2':
		p->PROC_TP[0] = ORD_PROC_MDFY;
		memcpy( p->ORD_RSLT_CODE,	CODE_SVR_RSLT_MDFY_S,	sizeof(p->ORD_RSLT_CODE) );
		break;
	case '3':
		p->PROC_TP[0] = ORD_PROC_CNCL;
		memcpy( p->ORD_RSLT_CODE,	CODE_SVR_RSLT_CNCL_S,	sizeof(p->ORD_RSLT_CODE) );
		break;
	}

	memcpy( p->BUY_SELL_TP,		org->dosugb,				sizeof(org->dosugb) );
	
	sprintf( t, "%*d",			sizeof(p->ORD_QTY),		S2N(org->qty, sizeof(org->qty)) );
	memcpy( p->ORD_QTY,			t,						sizeof(p->ORD_QTY) );
	
	//	*** 패킷에서 어떤게 정정가격인지 모르겠다. ***
	//	memcpy( p->ORD_PRC			t,						sizeof(p->ORD_PRC))			;
	
	//	*** 패킷에서 어떤게 잔량인지 모르겠다. ***
	//	sprintf( t, "%*d",			sizeof(p->REMN_QTY),	S2N(org->unercqty, sizeof(org->unercqty)) );
	//	memcpy(p->REMN_QTY			t,						sizeof(p->REMN_QTY) );
	
	memcpy( p->DT,				org->orddate,			sizeof(p->DT) );
	memcpy( p->TM,				org->rcvtime,			sizeof(p->TM) );
}

VOID CCVT_ETK::de_KS_RL_CNTR(char* psOrgData, char* pOut)
{
	char	t[128];
	
	C01_OutBlock*			org	= (C01_OutBlock*) psOrgData;		//	C01.h
	ST_IFRECV_REAL_CNTR*	p	= (ST_IFRECV_REAL_CNTR*)pOut;		//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_SISE_KS) );
	
	memcpy( p->ORD_RSLT_CODE,	CODE_SVR_RSLT_CNTR_S,	sizeof(p->ORD_RSLT_CODE) );
	memcpy( p->API_CODE,		APIKEY_KS_ETK,			min(strlen(APIKEY_KS_ETK),sizeof(p->API_CODE)) );

	//	KR4301EC2520 의 형태이다.
	memcpy( p->STK_CODE,		org->expcode+3,			sizeof(org->expcode)-4 );
	
	switch(p->STK_CODE[0])
	{
	case '1':
		memcpy(p->GDS_CODE, "00", 2);
		break;
	case '2':
	case '3':
		memcpy(p->GDS_CODE, "01", 2);
		break;
	}
	
	//	***	패킷에 USERID 가 없다.	***	//
	//	memcpy( p->API_USERID,		org->userid,			sizeof(org->userid) );
	memcpy( p->API_ACNT_NO,		org->accno,				sizeof(org->accno) );
	
	//////////////////////////////////////////////////////////////////////////
	//	ETK 주문번호 : 총 10자리, 우리는 9 자리, 따라서 제일 앞 1자리 빼고 처리
	sprintf( t, "%.*s", sizeof(org->ordno)-1, org->ordno+1 );
	memcpy( p->API_ORD_NO,		t,						sizeof(p->API_ORD_NO) );
	
	//	***	패킷에 없다.	***	//
	//	memcpy( p->ClORD_NO		[MAX_ORD_NO];
	
	//	*** ETK 약정번호는 11자리, 우리 체결번호는 9자리 ***
	sprintf( t, "%.*s", sizeof(org->yakseq)-2, org->yakseq+2 );
	memcpy( p->API_CNTR_NO,		t,						sizeof(p->API_CNTR_NO) );

	memcpy( p->BUY_SELL_TP,		org->dosugb,				sizeof(org->dosugb) );
	
	//	***	패킷에 주문수량 없다.	***
	//	sprintf( t, "%*d",			sizeof(p->ORD_QTY),		S2N(org->qty, sizeof(org->qty)) );
	//	memcpy( p->ORD_QTY,			t,						sizeof(p->ORD_QTY) );
	
	//	***	패킷에 주문가격 없다.	***
	//	memcpy( p->ORD_PRC			t,						sizeof(p->ORD_PRC))			;
	

	sprintf( t, "%*d", sizeof(p->CNTR_QTY), S2N(org->chevol, sizeof(org->chevol)));
	memcpy( p->CNTR_QTY,			t,						sizeof(p->CNTR_QTY));

	get_double_withdot( org->cheprice, sizeof(org->cheprice), 2, sizeof(p->CNTR_PRC), t );
	memcpy( p->CNTR_PRC,			t,						sizeof(p->CNTR_PRC));


	//	*** 패킷에서 잔량 없다. ***
	//	sprintf( t, "%*d",			sizeof(p->REMN_QTY),	S2N(org->unercqty, sizeof(org->unercqty)) );
	//	memcpy(p->REMN_QTY			t,						sizeof(p->REMN_QTY) );
	
	memcpy( p->DT,				org->chedate,			sizeof(p->DT) );
	memcpy( p->TM,				org->chetime,			sizeof(p->TM) );

}






//	CLIENT 에 내려보내기 위한 축소화된 패킷
//	시세오류고려. 해외선물시세

#define SISE_MIN 100
#define SISE_MAX 300
BOOL CCVT_ETK::de_FU_SiseEx(	char* psOrgData
							  , char* pOut				//	SISE_SHM 을 위한 패킷
							  , double* pErrSise		//	시세오류인경우 해당 시세 반환
							  )
{
	char	szLen[32], szCode[128];
	char	t[128];
	char	szStkCode[128];

	OVC_OutBlock*	org		= (OVC_OutBlock*)psOrgData;	//	DrdsSC0.h
	REAL_SISE_FU*	p		= (REAL_SISE_FU*)pOut;			//	PacketDef.h
	//REAL_SISE_KS_84003*	pFront	= (REAL_SISE_KS_84003*)pOutForClient;	//	PacketDef.h

	
	
	//////////////////////////////////////////////////////////////////////////
	//	SHM 용 패킷 구성
	memset( p, 0x20, sizeof(REAL_SISE_FU) );
	
	SET_LEN( sizeof(REAL_SISE_FU), szLen );
	SET_CODE_STR( CODE_REAL_SISE_FU, szCode );

	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );

 	sprintf( t, "%-*.*s", sizeof(p->stk_code), sizeof(p->stk_code), org->symbol);
	// URO==>6E
	if( strncmp(t,"URO",3)==0)
		sprintf( szStkCode, "6E%.3s", t+3);
	else if( strncmp(t,"BP",2)==0)
		sprintf( szStkCode, "6B%.3s", t+2);
	else if( strncmp(t,"JY",2)==0)
		sprintf( szStkCode, "6J%.3s", t+2);
	else
		sprintf( szStkCode, "%s", t);

#ifdef _DEF_NA	// 나래는 엔화종목코드가 6J 가 아니라 JY 이다.
	if( strncmp(t,"JY",2)==0)
		sprintf( szStkCode, "%s", t);
#endif

 	memcpy(p->stk_code,		szStkCode,		min(strlen(szStkCode), sizeof(org->symbol)) );
 	memcpy(p->tm,			org->kortm,		min(sizeof(p->tm), 	sizeof(org->kortm)) );
 	
	get_double_withdot2( szStkCode, org->curpr, sizeof(org->curpr), sizeof(p->now_prc), t );
	memcpy(p->now_prc, t, sizeof(p->now_prc) );

	//	시가
	get_double_withdot2( szStkCode, org->open, sizeof(org->open), sizeof(p->open), t );
	memcpy(p->open, t, sizeof(p->open) );
	
	//	고가
	get_double_withdot2( szStkCode, org->high, sizeof(org->high), sizeof(p->high), t );
	memcpy(p->high, t, sizeof(p->high) );
	
	//	저가
	get_double_withdot2( szStkCode, org->low, sizeof(org->low), sizeof(p->low), t );
	memcpy(p->low, t, sizeof(p->low) );
	
	//	매도매수구분 추가
	//if( org->cgubun[0]=='-' )
	//	p->buy_sell_tp[0] = '1';
	//else
	//	p->buy_sell_tp[0] = '2';

	//	누적체결수량
	sprintf( t, "%*d", sizeof(p->acml_cntr_qty), S2N(org->totq,sizeof(org->totq)) );
	memcpy(p->acml_cntr_qty,	t,		sizeof(p->acml_cntr_qty) );
	//	memcpy(p->acml_buy_cntr_qty,org->bidvol,		min(sizeof(p->acml_buy_cntr_qty), 	sizeof(org->bidvol)) );
	
	//	누적체결금액
	//get_double_withdot( org->value, sizeof(org->value), 0, sizeof(p->acml_amt), t );
	//memcpy(p->acml_amt,			t,  sizeof(p->acml_amt) );
	
	switch(org->ydiffSign[0])
	{
	case '2':	p->daebi_sign[0] = '+';	break;
	case '5':	p->daebi_sign[0] = '-';	break;
	default:	p->daebi_sign[0] = ' ';	break;
	}

	get_double_withdot2( szStkCode, org->ydiffpr, sizeof(org->ydiffpr), sizeof(p->chg), t );
	memcpy(p->chg,				t, sizeof(p->chg) );
	
	get_double_withdot2( szStkCode, org->chgrate, sizeof(org->chgrate), sizeof(p->chg_rate), t );
	memcpy(p->chg_rate,			t, sizeof(p->chg_rate) );
	
	//1-018-009-007-004.
	if (org->cgubun[0]=='-')
		sprintf( t, "%*d", sizeof(p->cntr_qty), -1*S2N(org->trdq,sizeof(org->trdq)) );
	else
		sprintf( t, "%*d", sizeof(p->cntr_qty), S2N(org->trdq,sizeof(org->trdq)) );
	memcpy(p->cntr_qty, 		t, sizeof(p->cntr_qty) );
	
	/*
	get_double_withdot( org->offerho1, sizeof(org->offerho1), 2, sizeof(p->o_hoga_1), t );
	memcpy(p->o_hoga_1, 		t, sizeof(p->o_hoga_1) );
	
	get_double_withdot( org->bidho1, sizeof(org->bidho1), 2, sizeof(p->b_hoga_1), t );
 	memcpy(p->b_hoga_1, 		t, sizeof(p->b_hoga_1) );
	
	get_double_withdot( org->theoryprice, sizeof(org->theoryprice), 2, sizeof(p->theory_prc), t );
	memcpy(p->theory_prc, t, sizeof(p->theory_prc) );
	
	get_double_withdot( org->openyak, sizeof(org->openyak), 0, sizeof(p->open_yak), t );
	memcpy(p->open_yak, 		t, 	sizeof(p->open_yak) );
	
	get_double_withdot( org->openyakcha, sizeof(org->openyakcha), 0, sizeof(p->open_yak_chg), t );
	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

 	memcpy(p->open_yak_chg, 	t, 	sizeof(p->open_yak_chg) );

	get_double_withdot( org->kasis, sizeof(org->kasis), 2, sizeof(p->gyuri_rate), t );
 	memcpy(p->gyuri_rate,		t,	sizeof(p->gyuri_rate) );

	get_double_withdot( org->sbasis, sizeof(org->sbasis), 2, sizeof(p->basis), t );
	memcpy(p->basis, 			t, 	sizeof(p->basis) );
	
	get_double_withdot( org->k200jisu, sizeof(org->k200jisu), 2, sizeof(p->ks200), t );
	memcpy(p->ks200,			t,	sizeof(p->ks200) );

	get_double_withdot( org->ibasis, sizeof(org->ibasis), 2, sizeof(p->theory_basis), t );
 	memcpy(p->theory_basis,		t, sizeof(p->theory_basis) );
	*/
 	p->ETX[0] = 0x03;
 	p->ETX[1] = 0x00;


	return TRUE;
}

BOOL CCVT_ETK::de_FU_Hoga(char* psOrgData, char* pOut)
{
	char	szLen[32], szCode[128], t[128];
	char	szStkCode[128];

	OVH_OutBlock*	org	= (OVH_OutBlock*) psOrgData;	//	FH0.h
	REAL_HOGA_FU*	p	= (REAL_HOGA_FU*)pOut;			//	PacketDef.h
	memset( p, 0x20, sizeof(REAL_HOGA_FU) );
	*(pOut+sizeof(REAL_HOGA_FU)) = 0x00;
	
	SET_LEN( sizeof(REAL_HOGA_FU), szLen );
	SET_CODE_STR( CODE_REAL_HOGA_FU, szCode );
	
	p->STX[0]	=	0x02;
	memcpy( p->Header.Len, szLen, sizeof(p->Header.Len) );	
	memcpy( p->Header.Code, szCode, sizeof(p->Header.Code) );
	
	//	종목코드	
	sprintf( t, "%-*.*s", sizeof(p->stk_code), sizeof(p->stk_code), org->symbol);
	// URO==>6E
	if( strncmp(t,"URO",3)==0)
		sprintf( szStkCode, "6E%.3s", t+3);
	// BP==>6B
	else if( strncmp(t,"BP",2)==0)
		sprintf( szStkCode, "6B%.3s", t+2);
	// JY==>6J
	else if( strncmp(t,"JY",2)==0)
		sprintf( szStkCode, "6J%.3s", t+2);
	else
		sprintf( szStkCode, "%s", t);

#ifdef _DEF_NA
	if( strncmp(t,"JY",2)==0)
		sprintf( szStkCode, "%s", t);
#endif

 	memcpy(p->stk_code,		szStkCode,		min(strlen(szStkCode), sizeof(org->symbol)) );

	//	시각
	memcpy(p->tm,				org->hotime,		min(sizeof(p->tm), 		sizeof(org->hotime)) );
	
	//	매도호가 1
	get_double_withdot2( szStkCode, org->offerho1, sizeof(org->offerho1), sizeof(p->o_hoga_1), t );


	memcpy(p->o_hoga_1, t, sizeof(p->o_hoga_1) );

	//	매도호가 2
	get_double_withdot2( szStkCode, org->offerho2, sizeof(org->offerho2), sizeof(p->o_hoga_2), t );
	memcpy(p->o_hoga_2, t, sizeof(p->o_hoga_2) );

	//	매도호가 3
	get_double_withdot2( szStkCode, org->offerho3, sizeof(org->offerho3),  sizeof(p->o_hoga_3), t );
	memcpy(p->o_hoga_3, t, sizeof(p->o_hoga_3) );

	//	매도호가 4
	get_double_withdot2( szStkCode, org->offerho4, sizeof(org->offerho4), sizeof(p->o_hoga_4), t );
	memcpy(p->o_hoga_4, t, sizeof(p->o_hoga_4) );

	//	매도호가 5
	get_double_withdot2( szStkCode, org->offerho5, sizeof(org->offerho5), sizeof(p->o_hoga_5), t );
	memcpy(p->o_hoga_5, t, sizeof(p->o_hoga_5) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_1), S2N( org->offerrem1,sizeof(org->offerrem1) ) );
	memcpy(p->o_rmnq_1, 		t, sizeof(p->o_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_2), S2N( org->offerrem2,sizeof(org->offerrem2) ) );
	memcpy(p->o_rmnq_2, 		t, sizeof(p->o_rmnq_2) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_3), S2N( org->offerrem3,sizeof(org->offerrem3) ) );
	memcpy(p->o_rmnq_3, 		t, sizeof(p->o_rmnq_3) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_4), S2N( org->offerrem4,sizeof(org->offerrem4) ) );
	memcpy(p->o_rmnq_4, 		t, sizeof(p->o_rmnq_4) );

	sprintf( t, "%*d", sizeof(p->o_rmnq_5), S2N( org->offerrem5,sizeof(org->offerrem5) ) );
	memcpy(p->o_rmnq_5, 		t, sizeof(p->o_rmnq_5) );

	sprintf( t, "%*d", sizeof(p->o_trmnq), S2N( org->totofferrem,sizeof(org->totofferrem) ) );
	memcpy(p->o_trmnq, 		t, sizeof(p->o_trmnq) );

	sprintf( t, "%*d", sizeof(p->o_cnt_1), S2N( org->offerno1,sizeof(org->offerno1) ) );
	memcpy(p->o_cnt_1, 			t, sizeof(p->o_cnt_1) );

	sprintf( t, "%*d", sizeof(p->o_cnt_2), S2N( org->offerno2,sizeof(org->offerno2) ) );
	memcpy(p->o_cnt_2, 			t, sizeof(p->o_cnt_2) );

	sprintf( t, "%*d", sizeof(p->o_cnt_3), S2N( org->offerno3,sizeof(org->offerno3) ) );
	memcpy(p->o_cnt_3, 			t, sizeof(p->o_cnt_3) );

	sprintf( t, "%*d", sizeof(p->o_cnt_4), S2N( org->offerno4,sizeof(org->offerno4) ) );
	memcpy(p->o_cnt_4, 			t, sizeof(p->o_cnt_4) );

	sprintf( t, "%*d", sizeof(p->o_cnt_5), S2N( org->offerno5,sizeof(org->offerno5) ) );
	memcpy(p->o_cnt_5, 			t, sizeof(p->o_cnt_5) );

	sprintf( t, "%*d", sizeof(p->o_tot_cnt), S2N( org->totoffercnt,sizeof(org->totoffercnt) ) );
	memcpy(p->o_tot_cnt, 		t, sizeof(p->o_tot_cnt) );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	매수호가 1
	get_double_withdot2( szStkCode, org->bidho1, sizeof(org->bidho1), sizeof(p->b_hoga_1), t );

	memcpy(p->b_hoga_1, t, sizeof(p->b_hoga_1) );
	
	//	매수호가 2
	get_double_withdot2( szStkCode, org->bidho2, sizeof(org->bidho2), sizeof(p->b_hoga_2), t );
	memcpy(p->b_hoga_2, t, sizeof(p->b_hoga_2) );
	
	//	매수호가 3
	get_double_withdot2( szStkCode, org->bidho3, sizeof(org->bidho3), sizeof(p->b_hoga_3), t );
	memcpy(p->b_hoga_3, t, sizeof(p->b_hoga_3) );
	
	//	매수호가 4
	get_double_withdot2( szStkCode, org->bidho4, sizeof(org->bidho4), sizeof(p->b_hoga_4), t );
	memcpy(p->b_hoga_4, t, sizeof(p->b_hoga_4) );
	
	//	매수호가 5
	get_double_withdot2( szStkCode, org->bidho5, sizeof(org->bidho5), sizeof(p->b_hoga_5), t );
	memcpy(p->b_hoga_5, t, sizeof(p->b_hoga_5) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_1), S2N( org->bidrem1,sizeof(org->bidrem1) ) );
	memcpy(p->b_rmnq_1, 		t, sizeof(p->b_rmnq_1) );

	sprintf( t, "%*d", sizeof(p->b_rmnq_2), S2N( org->bidrem2,sizeof(org->bidrem2) ) );
	memcpy(p->b_rmnq_2, 		t, sizeof(p->b_rmnq_2) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_3), S2N( org->bidrem3,sizeof(org->bidrem3) ) );
	memcpy(p->b_rmnq_3, 		t, sizeof(p->b_rmnq_3) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_4), S2N( org->bidrem4,sizeof(org->bidrem4) ) );
	memcpy(p->b_rmnq_4, 		t, sizeof(p->b_rmnq_4) );
	
	sprintf( t, "%*d", sizeof(p->b_rmnq_5), S2N( org->bidrem5,sizeof(org->bidrem5) ) );
	memcpy(p->b_rmnq_5, 		t, sizeof(p->b_rmnq_5) );
	
	sprintf( t, "%*d", sizeof(p->b_trmnq), S2N( org->totbidrem,sizeof(org->totbidrem) ) );
	memcpy(p->b_trmnq, 			t, sizeof(p->b_trmnq) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_1), S2N( org->bidno1,sizeof(org->bidno1) ) );
	memcpy(p->b_cnt_1, 			t, sizeof(p->b_cnt_1) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_2), S2N( org->bidno2,sizeof(org->bidno2) ) );
	memcpy(p->b_cnt_2, 			t, sizeof(p->b_cnt_2) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_3), S2N( org->bidno3,sizeof(org->bidno3) ) );
	memcpy(p->b_cnt_3, 			t, sizeof(p->b_cnt_3) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_4), S2N( org->bidno4,sizeof(org->bidno4) ) );
	memcpy(p->b_cnt_4, 			t, sizeof(p->b_cnt_4) );
	
	sprintf( t, "%*d", sizeof(p->b_cnt_5), S2N( org->bidno5,sizeof(org->bidno5) ) );
	memcpy(p->b_cnt_5, 			t, sizeof(p->b_cnt_5) );
	
	sprintf( t, "%*d", sizeof(p->b_tot_cnt), S2N( org->totbidcnt,sizeof(org->totbidcnt) ) );
	memcpy(p->b_tot_cnt, 		t, sizeof(p->b_tot_cnt) );
	
	p->ETX[0] = 0x03;
	p->ETX[1] = 0x00;
	
	//SMILOR0803
	return TRUE;

}