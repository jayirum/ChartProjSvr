#ifndef _CIDBQ03000_H_
#define _CIDBQ03000_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 예수금/잔고현황 ( SERVICE=CIDBQ03000,HEADTYPE=B,CREATOR=이호섭,CREDATE=2015/06/25 09:12:31 )
#pragma pack( push, 1 )

#define NAME_CIDBQ03000     "CIDBQ03000"

// In(*EMPTY*)                    
typedef struct _CIDBQ03000InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] 계좌구분코드                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 6, Length 20
    char    AcntPwd             [   8];    // [string,    8] 계좌비밀번호                    StartPos 26, Length 8
    char    TrdDt               [   8];    // [string,    8] 거래일자                        StartPos 34, Length 8
} CIDBQ03000InBlock1, *LPCIDBQ03000InBlock1;
#define NAME_CIDBQ03000InBlock1     "CIDBQ03000InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ03000OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] 계좌구분코드                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 6, Length 20
    char    AcntPwd             [   8];    // [string,    8] 계좌비밀번호                    StartPos 26, Length 8
    char    TrdDt               [   8];    // [string,    8] 거래일자                        StartPos 34, Length 8
} CIDBQ03000OutBlock1, *LPCIDBQ03000OutBlock1;
#define NAME_CIDBQ03000OutBlock1     "CIDBQ03000OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ03000OutBlock2
{
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 0, Length 20
    char    TrdDt               [   8];    // [string,    8] 거래일자                        StartPos 20, Length 8
    char    CrcyObjCode         [  12];    // [string,   12] 통화대상코드                    StartPos 28, Length 12
    char    OvrsFutsDps         [  23];    // [double, 23.2] 해외선물예수금                  StartPos 40, Length 23
    char    CustmMnyioAmt       [  19];    // [double, 19.2] 고객입출금금액                  StartPos 63, Length 19
    char    AbrdFutsLqdtPnlAmt  [  19];    // [double, 19.2] 해외선물청산손익금액            StartPos 82, Length 19
    char    AbrdFutsCmsnAmt     [  19];    // [double, 19.2] 해외선물수수료금액              StartPos 101, Length 19
    char    PrexchDps           [  19];    // [double, 19.2] 가환전예수금                    StartPos 120, Length 19
    char    EvalAssetAmt        [  19];    // [double, 19.2] 평가자산금액                    StartPos 139, Length 19
    char    AbrdFutsCsgnMgn     [  19];    // [double, 19.2] 해외선물위탁증거금액            StartPos 158, Length 19
    char    AbrdFutsAddMgn      [  19];    // [double, 19.2] 해외선물추가증거금액            StartPos 177, Length 19
    char    AbrdFutsWthdwAbleAmt[  19];    // [double, 19.2] 해외선물인출가능금액            StartPos 196, Length 19
    char    AbrdFutsOrdAbleAmt  [  19];    // [double, 19.2] 해외선물주문가능금액            StartPos 215, Length 19
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] 해외선물평가손익금액            StartPos 234, Length 19
    char    LastSettPnlAmt      [  19];    // [double, 19.2] 최종결제손익금액                StartPos 253, Length 19
    char    OvrsOptSettAmt      [  19];    // [double, 19.2] 해외옵션결제금액                StartPos 272, Length 19
    char    OvrsOptBalEvalAmt   [  19];    // [double, 19.2] 해외옵션잔고평가금액            StartPos 291, Length 19
} CIDBQ03000OutBlock2, *LPCIDBQ03000OutBlock2;
#define NAME_CIDBQ03000OutBlock2     "CIDBQ03000OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ03000_H_
