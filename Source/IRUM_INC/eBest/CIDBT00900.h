#ifndef _CIDBT00900_H_
#define _CIDBT00900_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 정정주문 ( SERVICE=CIDBT00900,ENCRYPT,SIGNATURE,HEADTYPE=B,CREATOR=김재홍,CREDATE=2012/02/23 15:00:10 )
#pragma pack( push, 1 )

#define NAME_CIDBT00900     "CIDBT00900"

// In(*EMPTY*)                    
typedef struct _CIDBT00900InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 5, Length 8
    char    RegBrnNo            [   3];    // [string,    3] 등록지점번호                    StartPos 13, Length 3
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 16, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 36, Length 8
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] 해외선물원주문번호              StartPos 44, Length 10
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 54, Length 18
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 72, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 73, Length 1
    char    FutsOrdPtnCode      [   1];    // [string,    1] 선물주문유형코드                StartPos 74, Length 1
    char    CrcyCodeVal         [   3];    // [string,    3] 통화코드값                      StartPos 75, Length 3
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] 해외파생주문가격                StartPos 78, Length 25
    char    CndiOrdPrc          [  25];    // [double, 25.8] 조건주문가격                    StartPos 103, Length 25
    char    OrdQty              [  16];    // [long  ,   16] 주문수량                        StartPos 128, Length 16
    char    OvrsDrvtPrdtCode    [  10];    // [string,   10] 해외파생상품코드                StartPos 144, Length 10
    char    DueYymm             [   6];    // [string,    6] 만기년월                        StartPos 154, Length 6
    char    ExchCode            [  10];    // [string,   10] 거래소코드                      StartPos 160, Length 10
} CIDBT00900InBlock1, *LPCIDBT00900InBlock1;
#define NAME_CIDBT00900InBlock1     "CIDBT00900InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBT00900OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 5, Length 8
    char    RegBrnNo            [   3];    // [string,    3] 등록지점번호                    StartPos 13, Length 3
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 16, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 36, Length 8
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] 해외선물원주문번호              StartPos 44, Length 10
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 54, Length 18
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 72, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 73, Length 1
    char    FutsOrdPtnCode      [   1];    // [string,    1] 선물주문유형코드                StartPos 74, Length 1
    char    CrcyCodeVal         [   3];    // [string,    3] 통화코드값                      StartPos 75, Length 3
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] 해외파생주문가격                StartPos 78, Length 25
    char    CndiOrdPrc          [  25];    // [double, 25.8] 조건주문가격                    StartPos 103, Length 25
    char    OrdQty              [  16];    // [long  ,   16] 주문수량                        StartPos 128, Length 16
    char    OvrsDrvtPrdtCode    [  10];    // [string,   10] 해외파생상품코드                StartPos 144, Length 10
    char    DueYymm             [   6];    // [string,    6] 만기년월                        StartPos 154, Length 6
    char    ExchCode            [  10];    // [string,   10] 거래소코드                      StartPos 160, Length 10
} CIDBT00900OutBlock1, *LPCIDBT00900OutBlock1;
#define NAME_CIDBT00900OutBlock1     "CIDBT00900OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CIDBT00900OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    OvrsFutsOrdNo       [  10];    // [string,   10] 해외선물주문번호                StartPos 25, Length 10
    char    InnerMsgCnts        [  80];    // [string,   80] 내부메시지내용                  StartPos 35, Length 80
} CIDBT00900OutBlock2, *LPCIDBT00900OutBlock2;
#define NAME_CIDBT00900OutBlock2     "CIDBT00900OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBT00900_H_
