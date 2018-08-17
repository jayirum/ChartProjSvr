#ifndef _TC3_H_
#define _TC3_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 주문체결 ( BLOCK, KEY=7, GROUP=1 )
#pragma pack( push, 1 )

#define NAME_TC3     "TC3"

// 입력                           
typedef struct _TC3_InBlock
{
} TC3_InBlock, *LPTC3_InBlock;
#define NAME_TC3_InBlock     "InBlock"

// 출력                           
typedef struct _TC3_OutBlock
{
    char    lineseq             [  10];    // [long  ,   10] 라인일련번호                    StartPos 0, Length 10
    char    key                 [  11];    // [string,   11] KEY                             StartPos 10, Length 11
    char    user                [   8];    // [string,    8] 조작자ID                        StartPos 21, Length 8
    char    svc_id              [   4];    // [string,    4] 서비스ID                        StartPos 29, Length 4
    char    ordr_dt             [   8];    // [string,    8] 주문일자                        StartPos 33, Length 8
    char    brn_cd              [   3];    // [string,    3] 지점번호                        StartPos 41, Length 3
    char    ordr_no             [  10];    // [long  ,   10] 주문번호                        StartPos 44, Length 10
    char    orgn_ordr_no        [  10];    // [long  ,   10] 원주문번호                      StartPos 54, Length 10
    char    mthr_ordr_no        [  10];    // [long  ,   10] 모주문번호                      StartPos 64, Length 10
    char    ac_no               [  11];    // [string,   11] 계좌번호                        StartPos 74, Length 11
    char    is_cd               [  30];    // [string,   30] 종목코드                        StartPos 85, Length 30
    char    s_b_ccd             [   1];    // [string,    1] 매도매수유형                    StartPos 115, Length 1
    char    ordr_ccd            [   1];    // [string,    1] 정정취소유형                    StartPos 116, Length 1
    char    ccls_q              [  15];    // [long  ,   15] 체결수량                        StartPos 117, Length 15
    char    ccls_prc            [  15];    // [double, 15.8] 체결가격                        StartPos 132, Length 15
    char    ccls_no             [  10];    // [string,   10] 체결번호                        StartPos 147, Length 10
    char    ccls_tm             [   9];    // [string,    9] 체결시간                        StartPos 157, Length 9
    char    avg_byng_uprc       [  12];    // [double, 12.6] 매입평균단가                    StartPos 166, Length 12
    char    byug_amt            [  25];    // [double, 25.8] 매입금액                        StartPos 178, Length 25
    char    clr_pl_amt          [  19];    // [double, 19.2] 청산손익                        StartPos 203, Length 19
    char    ent_fee             [  19];    // [double, 19.2] 위탁수수료                      StartPos 222, Length 19
    char    fcm_fee             [  19];    // [double, 19.2] FCM수수료                       StartPos 241, Length 19
    char    userid              [   8];    // [string,    8] 사용자ID                        StartPos 260, Length 8
    char    now_prc             [  15];    // [double, 15.8] 현재가격                        StartPos 268, Length 15
    char    crncy_cd            [   3];    // [string,    3] 통화코드                        StartPos 283, Length 3
    char    mtrt_dt             [   8];    // [string,    8] 만기일자                        StartPos 286, Length 8
} TC3_OutBlock, *LPTC3_OutBlock;
#define NAME_TC3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _TC3_H_
