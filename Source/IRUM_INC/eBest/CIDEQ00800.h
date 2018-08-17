#ifndef _CIDEQ00800_H_
#define _CIDEQ00800_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 일자별 미결제 잔고내역 ( SERVICE=CIDEQ00800,ENCRYPT,HEADTYPE=B,CREATOR=박시현,CREDATE=2012/05/03 23:15:35 )
#pragma pack( push, 1 )

#define NAME_CIDEQ00800     "CIDEQ00800"

// In(*EMPTY*)                    
typedef struct _CIDEQ00800InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    AcntPwd             [   8];    // [string,    8] 계좌비밀번호                    StartPos 25, Length 8
    char    TrdDt               [   8];    // [string,    8] 거래일자                        StartPos 33, Length 8
} CIDEQ00800InBlock1, *LPCIDEQ00800InBlock1;
#define NAME_CIDEQ00800InBlock1     "CIDEQ00800InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDEQ00800OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    AcntPwd             [   8];    // [string,    8] 계좌비밀번호                    StartPos 25, Length 8
    char    TrdDt               [   8];    // [string,    8] 거래일자                        StartPos 33, Length 8
} CIDEQ00800OutBlock1, *LPCIDEQ00800OutBlock1;
#define NAME_CIDEQ00800OutBlock1     "CIDEQ00800OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDEQ00800OutBlock2
{
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 0, Length 20
    char    TrdDt               [   8];    // [string,    8] 거래일자                        StartPos 20, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 28, Length 18
    char    BnsTpNm             [  10];    // [string,   10] 매매구분명                      StartPos 46, Length 10
    char    BalQty              [  16];    // [long  ,   16] 잔고수량                        StartPos 56, Length 16
    char    LqdtAbleQty         [  16];    // [long  ,   16] 청산가능수량                    StartPos 72, Length 16
    char    PchsPrc             [  25];    // [double, 25.8] 매입가격                        StartPos 88, Length 25
    char    OvrsDrvtNowPrc      [  25];    // [double, 25.8] 해외파생현재가                  StartPos 113, Length 25
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] 해외선물평가손익금액            StartPos 138, Length 19
    char    CustmBalAmt         [  19];    // [double, 19.2] 고객잔고금액                    StartPos 157, Length 19
    char    FcurrEvalAmt        [  21];    // [double, 21.4] 외화평가금액                    StartPos 176, Length 21
    char    IsuNm               [  50];    // [string,   50] 종목명                          StartPos 197, Length 50
    char    CrcyCodeVal         [   3];    // [string,    3] 통화코드값                      StartPos 247, Length 3
    char    OvrsDrvtPrdtCode    [  10];    // [string,   10] 해외파생상품코드                StartPos 250, Length 10
    char    DueDt               [   8];    // [string,    8] 만기일자                        StartPos 260, Length 8
    char    PrcntrAmt           [  19];    // [double, 19.2] 계약당금액                      StartPos 268, Length 19
    char    FcurrEvalPnlAmt     [  21];    // [double, 21.4] 외화평가손익금액                StartPos 287, Length 21
} CIDEQ00800OutBlock2, *LPCIDEQ00800OutBlock2;
#define NAME_CIDEQ00800OutBlock2     "CIDEQ00800OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDEQ00800_H_
