#ifndef _CIDBT00100_H_
#define _CIDBT00100_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 신규주문 ( SERVICE=CIDBT00100,ENCRYPT,SIGNATURE,HEADTYPE=B,CREATOR=최영호,CREDATE=2012/04/26 14:50:17 )
#pragma pack( push, 1 )

#define NAME_CIDBT00100     "CIDBT00100"

// In(*EMPTY*)                    
typedef struct _CIDBT00100InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 5, Length 8
    char    BrnCode             [   7];    // [string,    7] 지점코드                        StartPos 13, Length 7
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 20, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 40, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 48, Length 18
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 66, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드(1:매도, 2:매수)    StartPos 67, Length 1
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] 해외선물주문유형코드            StartPos 68, Length 1
    char    CrcyCode            [   3];    // [string,    3] 통화코드                        StartPos 69, Length 3
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] 해외파생주문가격                StartPos 72, Length 25
    char    CndiOrdPrc          [  25];    // [double, 25.8] 조건주문가격                    StartPos 97, Length 25
    char    OrdQty              [  16];    // [long  ,   16] 주문수량                        StartPos 122, Length 16
    char    PrdtCode            [   6];    // [string,    6] 상품코드                        StartPos 138, Length 6
    char    DueYymm             [   6];    // [string,    6] 만기년월                        StartPos 144, Length 6
    char    ExchCode            [  10];    // [string,   10] 거래소코드                      StartPos 150, Length 10
} CIDBT00100InBlock1, *LPCIDBT00100InBlock1;
#define NAME_CIDBT00100InBlock1     "CIDBT00100InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBT00100OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 5, Length 8
    char    BrnCode             [   7];    // [string,    7] 지점코드                        StartPos 13, Length 7
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 20, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 40, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 48, Length 18
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 66, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 67, Length 1
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] 해외선물주문유형코드            StartPos 68, Length 1
    char    CrcyCode            [   3];    // [string,    3] 통화코드                        StartPos 69, Length 3
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] 해외파생주문가격                StartPos 72, Length 25
    char    CndiOrdPrc          [  25];    // [double, 25.8] 조건주문가격                    StartPos 97, Length 25
    char    OrdQty              [  16];    // [long  ,   16] 주문수량                        StartPos 122, Length 16
    char    PrdtCode            [   6];    // [string,    6] 상품코드                        StartPos 138, Length 6
    char    DueYymm             [   6];    // [string,    6] 만기년월                        StartPos 144, Length 6
    char    ExchCode            [  10];    // [string,   10] 거래소코드                      StartPos 150, Length 10
} CIDBT00100OutBlock1, *LPCIDBT00100OutBlock1;
#define NAME_CIDBT00100OutBlock1     "CIDBT00100OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CIDBT00100OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    OvrsFutsOrdNo       [  10];    // [string,   10] 해외선물주문번호                StartPos 25, Length 10
} CIDBT00100OutBlock2, *LPCIDBT00100OutBlock2;
#define NAME_CIDBT00100OutBlock2     "CIDBT00100OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBT00100_H_
