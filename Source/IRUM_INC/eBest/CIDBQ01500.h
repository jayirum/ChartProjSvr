#ifndef _CIDBQ01500_H_
#define _CIDBQ01500_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 미결제 잔고내역 조회 ( SERVICE=CIDBQ01500,ENCRYPT,HEADTYPE=B,CREATOR=이호섭,CREDATE=2013/04/11 18:36:15 )
#pragma pack( push, 1 )

#define NAME_CIDBQ01500     "CIDBQ01500"

// In(*EMPTY*)                    
typedef struct _CIDBQ01500InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] 계좌구분코드                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 6, Length 20
    char    FcmAcntNo           [  20];    // [string,   20] FCM계좌번호                     StartPos 26, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 46, Length 8
    char    QryDt               [   8];    // [string,    8] 조회일자                        StartPos 54, Length 8
    char    BalTpCode           [   1];    // [string,    1] 잔고구분코드                    StartPos 62, Length 1
} CIDBQ01500InBlock1, *LPCIDBQ01500InBlock1;
#define NAME_CIDBQ01500InBlock1     "CIDBQ01500InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ01500OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] 계좌구분코드                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 6, Length 20
    char    FcmAcntNo           [  20];    // [string,   20] FCM계좌번호                     StartPos 26, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 46, Length 8
    char    QryDt               [   8];    // [string,    8] 조회일자                        StartPos 54, Length 8
    char    BalTpCode           [   1];    // [string,    1] 잔고구분코드                    StartPos 62, Length 1
} CIDBQ01500OutBlock1, *LPCIDBQ01500OutBlock1;
#define NAME_CIDBQ01500OutBlock1     "CIDBQ01500OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ01500OutBlock2
{
    char    BaseDt              [   8];    // [string,    8] 기준일자                        StartPos 0, Length 8
    char    Dps                 [  16];    // [long  ,   16] 예수금                          StartPos 8, Length 16
    char    LpnlAmt             [  19];    // [double, 19.2] 청산손익금액                    StartPos 24, Length 19
    char    FutsDueBfLpnlAmt    [  23];    // [double, 23.2] 선물만기전청산손익금액          StartPos 43, Length 23
    char    FutsDueBfCmsn       [  23];    // [double, 23.2] 선물만기전수수료                StartPos 66, Length 23
    char    CsgnMgn             [  16];    // [long  ,   16] 위탁증거금액                    StartPos 89, Length 16
    char    MaintMgn            [  16];    // [long  ,   16] 유지증거금                      StartPos 105, Length 16
    char    CtlmtAmt            [  23];    // [double, 23.2] 신용한도금액                    StartPos 121, Length 23
    char    AddMgn              [  16];    // [long  ,   16] 추가증거금액                    StartPos 144, Length 16
    char    MgnclRat            [  27];    // [double,27.10] 마진콜율                        StartPos 160, Length 27
    char    OrdAbleAmt          [  16];    // [long  ,   16] 주문가능금액                    StartPos 187, Length 16
    char    WthdwAbleAmt        [  16];    // [long  ,   16] 인출가능금액                    StartPos 203, Length 16
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 219, Length 20
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 239, Length 18
    char    IsuNm               [  50];    // [string,   50] 종목명                          StartPos 257, Length 50
    char    CrcyCodeVal         [   3];    // [string,    3] 통화코드값                      StartPos 307, Length 3
    char    OvrsDrvtPrdtCode    [  10];    // [string,   10] 해외파생상품코드                StartPos 310, Length 10
    char    OvrsDrvtOptTpCode   [   1];    // [string,    1] 해외파생옵션구분코드            StartPos 320, Length 1
    char    DueDt               [   8];    // [string,    8] 만기일자                        StartPos 321, Length 8
    char    OvrsDrvtXrcPrc      [  25];    // [double, 25.8] 해외파생행사가격                StartPos 329, Length 25
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 354, Length 1
    char    CmnCodeNm           [ 100];    // [string,  100] 공통코드명                      StartPos 355, Length 100
    char    TpCodeNm            [  50];    // [string,   50] 구분코드명                      StartPos 455, Length 50
    char    BalQty              [  16];    // [long  ,   16] 잔고수량                        StartPos 505, Length 16
    char    PchsPrc             [  25];    // [double, 25.8] 매입가격                        StartPos 521, Length 25
    char    OvrsDrvtNowPrc      [  25];    // [double, 25.8] 해외파생현재가                  StartPos 546, Length 25
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] 해외선물평가손익금액            StartPos 571, Length 19
    char    CsgnCmsn            [  19];    // [double, 19.2] 위탁수수료                      StartPos 590, Length 19
    char    PosNo               [  13];    // [string,   13] 포지션번호                      StartPos 609, Length 13
    char    EufOneCmsnAmt       [  19];    // [double, 19.2] 거래소비용1수수료금액           StartPos 622, Length 19
    char    EufTwoCmsnAmt       [  19];    // [double, 19.2] 거래소비용2수수료금액           StartPos 641, Length 19
} CIDBQ01500OutBlock2, *LPCIDBQ01500OutBlock2;
#define NAME_CIDBQ01500OutBlock2     "CIDBQ01500OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ01500_H_
