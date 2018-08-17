#ifndef _CIDBQ05300_H_
#define _CIDBQ05300_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 계좌예탁자산 조회 ( SERVICE=CIDBQ05300,HEADTYPE=B,CREATOR=이호섭,CREDATE=2015/06/22 19:42:57 )
#pragma pack( push, 1 )

#define NAME_CIDBQ05300     "CIDBQ05300"

// In(*EMPTY*)                    
typedef struct _CIDBQ05300InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OvrsAcntTpCode      [   1];    // [string,    1] 해외계좌구분코드                StartPos 5, Length 1
    char    FcmAcntNo           [  20];    // [string,   20] FCM계좌번호                     StartPos 6, Length 20
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 26, Length 20
    char    AcntPwd             [   8];    // [string,    8] 계좌비밀번호                    StartPos 46, Length 8
    char    CrcyCode            [   3];    // [string,    3] 통화코드                        StartPos 54, Length 3
} CIDBQ05300InBlock1, *LPCIDBQ05300InBlock1;
#define NAME_CIDBQ05300InBlock1     "CIDBQ05300InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ05300OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OvrsAcntTpCode      [   1];    // [string,    1] 해외계좌구분코드                StartPos 5, Length 1
    char    FcmAcntNo           [  20];    // [string,   20] FCM계좌번호                     StartPos 6, Length 20
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 26, Length 20
    char    AcntPwd             [   8];    // [string,    8] 계좌비밀번호                    StartPos 46, Length 8
    char    CrcyCode            [   3];    // [string,    3] 통화코드                        StartPos 54, Length 3
} CIDBQ05300OutBlock1, *LPCIDBQ05300OutBlock1;
#define NAME_CIDBQ05300OutBlock1     "CIDBQ05300OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ05300OutBlock2
{
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 0, Length 20
    char    CrcyCode            [   3];    // [string,    3] 통화코드                        StartPos 20, Length 3
    char    OvrsFutsDps         [  23];    // [double, 23.2] 해외선물예수금                  StartPos 23, Length 23
    char    AbrdFutsCsgnMgn     [  19];    // [double, 19.2] 해외선물위탁증거금액            StartPos 46, Length 19
    char    OvrsFutsSplmMgn     [  23];    // [double, 23.2] 해외선물추가증거금              StartPos 65, Length 23
    char    CustmLpnlAmt        [  19];    // [double, 19.2] 고객청산손익금액                StartPos 88, Length 19
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] 해외선물평가손익금액            StartPos 107, Length 19
    char    AbrdFutsCmsnAmt     [  19];    // [double, 19.2] 해외선물수수료금액              StartPos 126, Length 19
    char    AbrdFutsEvalDpstgTotAmt[  19];    // [double, 19.2] 해외선물평가예탁총금액          StartPos 145, Length 19
    char    Xchrat              [  15];    // [double, 15.4] 환율                            StartPos 164, Length 15
    char    FcurrRealMxchgAmt   [  19];    // [double, 19.2] 외화실환전금액                  StartPos 179, Length 19
    char    AbrdFutsWthdwAbleAmt[  19];    // [double, 19.2] 해외선물인출가능금액            StartPos 198, Length 19
    char    AbrdFutsOrdAbleAmt  [  19];    // [double, 19.2] 해외선물주문가능금액            StartPos 217, Length 19
    char    FutsDueNarrvLqdtPnlAmt[  19];    // [double, 19.2] 선물만기미도래청산손익금액      StartPos 236, Length 19
    char    FutsDueNarrvCmsn    [  19];    // [double, 19.2] 선물만기미도래수수료            StartPos 255, Length 19
    char    AbrdFutsLqdtPnlAmt  [  19];    // [double, 19.2] 해외선물청산손익금액            StartPos 274, Length 19
    char    OvrsFutsDueCmsn     [  19];    // [double, 19.2] 해외선물만기수수료              StartPos 293, Length 19
    char    OvrsFutsOptBuyAmt   [  23];    // [double, 23.2] 해외선물옵션매수금액            StartPos 312, Length 23
    char    OvrsFutsOptSellAmt  [  23];    // [double, 23.2] 해외선물옵션매도금액            StartPos 335, Length 23
    char    OptBuyMktWrthAmt    [  19];    // [double, 19.2] 옵션매수시장가치금액            StartPos 358, Length 19
    char    OptSellMktWrthAmt   [  19];    // [double, 19.2] 옵션매도시장가치금액            StartPos 377, Length 19
} CIDBQ05300OutBlock2, *LPCIDBQ05300OutBlock2;
#define NAME_CIDBQ05300OutBlock2     "CIDBQ05300OutBlock2"

// SelOut(*EMPTY*)                
typedef struct _CIDBQ05300OutBlock3
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OvrsFutsDps         [  23];    // [double, 23.2] 해외선물예수금                  StartPos 5, Length 23
    char    AbrdFutsLqdtPnlAmt  [  19];    // [double, 19.2] 해외선물청산손익금액            StartPos 28, Length 19
    char    FutsDueNarrvLqdtPnlAmt[  19];    // [double, 19.2] 선물만기미도래청산손익금액      StartPos 47, Length 19
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] 해외선물평가손익금액            StartPos 66, Length 19
    char    AbrdFutsEvalDpstgTotAmt[  19];    // [double, 19.2] 해외선물평가예탁총금액          StartPos 85, Length 19
    char    CustmLpnlAmt        [  19];    // [double, 19.2] 고객청산손익금액                StartPos 104, Length 19
    char    OvrsFutsDueCmsn     [  19];    // [double, 19.2] 해외선물만기수수료              StartPos 123, Length 19
    char    FcurrRealMxchgAmt   [  19];    // [double, 19.2] 외화실환전금액                  StartPos 142, Length 19
    char    AbrdFutsCmsnAmt     [  19];    // [double, 19.2] 해외선물수수료금액              StartPos 161, Length 19
    char    FutsDueNarrvCmsn    [  19];    // [double, 19.2] 선물만기미도래수수료            StartPos 180, Length 19
    char    AbrdFutsCsgnMgn     [  19];    // [double, 19.2] 해외선물위탁증거금액            StartPos 199, Length 19
    char    OvrsFutsMaintMgn    [  19];    // [double, 19.2] 해외선물유지증거금              StartPos 218, Length 19
    char    OvrsFutsOptBuyAmt   [  23];    // [double, 23.2] 해외선물옵션매수금액            StartPos 237, Length 23
    char    OvrsFutsOptSellAmt  [  23];    // [double, 23.2] 해외선물옵션매도금액            StartPos 260, Length 23
    char    CtlmtAmt            [  23];    // [double, 23.2] 신용한도금액                    StartPos 283, Length 23
    char    OvrsFutsSplmMgn     [  23];    // [double, 23.2] 해외선물추가증거금              StartPos 306, Length 23
    char    MgnclRat            [  27];    // [double,27.10] 마진콜율                        StartPos 329, Length 27
    char    AbrdFutsOrdAbleAmt  [  19];    // [double, 19.2] 해외선물주문가능금액            StartPos 356, Length 19
    char    AbrdFutsWthdwAbleAmt[  19];    // [double, 19.2] 해외선물인출가능금액            StartPos 375, Length 19
    char    OptBuyMktWrthAmt    [  19];    // [double, 19.2] 옵션매수시장가치금액            StartPos 394, Length 19
    char    OptSellMktWrthAmt   [  19];    // [double, 19.2] 옵션매도시장가치금액            StartPos 413, Length 19
    char    OvrsOptSettAmt      [  19];    // [double, 19.2] 해외옵션결제금액                StartPos 432, Length 19
    char    OvrsOptBalEvalAmt   [  19];    // [double, 19.2] 해외옵션잔고평가금액            StartPos 451, Length 19
} CIDBQ05300OutBlock3, *LPCIDBQ05300OutBlock3;
#define NAME_CIDBQ05300OutBlock3     "CIDBQ05300OutBlock3"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ05300_H_
