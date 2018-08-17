#ifndef _TC1_H_
#define _TC1_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 주문접수 ( BLOCK, KEY=7, GROUP=1 )
#pragma pack( push, 1 )

#define NAME_TC1     "TC1"

// 입력                           
typedef struct _TC1_InBlock
{
} TC1_InBlock, *LPTC1_InBlock;
#define NAME_TC1_InBlock     "InBlock"

// 출력                           
typedef struct _TC1_OutBlock
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
    char    ordr_typ_cd         [   1];    // [string,    1] 주문유형코드                    StartPos 117, Length 1
    char    ordr_typ_prd_ccd    [   2];    // [string,    2] 주문기간코드                    StartPos 118, Length 2
    char    ordr_aplc_strt_dt   [   8];    // [string,    8] 주문적용시작일자                StartPos 120, Length 8
    char    ordr_aplc_end_dt    [   8];    // [string,    8] 주문적용종료일자                StartPos 128, Length 8
    char    ordr_prc            [  15];    // [double, 15.8] 주문가격                        StartPos 136, Length 15
    char    cndt_ordr_prc       [  15];    // [double, 15.8] 주문조건가격                    StartPos 151, Length 15
    char    ordr_q              [  12];    // [long  ,   12] 주문수량                        StartPos 166, Length 12
    char    ordr_tm             [   9];    // [string,    9] 주문시간                        StartPos 178, Length 9
    char    userid              [   8];    // [string,    8] 사용자ID                        StartPos 187, Length 8
} TC1_OutBlock, *LPTC1_OutBlock;
#define NAME_TC1_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _TC1_H_
