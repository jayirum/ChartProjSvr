#ifndef _CIDBQ01800_H_
#define _CIDBQ01800_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 주문체결내역 조회 ( SERVICE=CIDBQ01800,HEADTYPE=B,CREATOR=이호섭,CREDATE=2015/06/22 19:06:11 )
#pragma pack( push, 1 )

#define NAME_CIDBQ01800     "CIDBQ01800"

// In(*EMPTY*)                    
typedef struct _CIDBQ01800InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 33, Length 18
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 51, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] 당일구분코드                    StartPos 59, Length 1
    char    OrdStatCode         [   1];    // [string,    1] 주문상태코드                    StartPos 60, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 61, Length 1
    char    QryTpCode           [   1];    // [string,    1] 조회구분코드                    StartPos 62, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] 주문유형코드                    StartPos 63, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] 해외파생선물옵션구분코드        StartPos 65, Length 1
} CIDBQ01800InBlock1, *LPCIDBQ01800InBlock1;
#define NAME_CIDBQ01800InBlock1     "CIDBQ01800InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ01800OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 33, Length 18
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 51, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] 당일구분코드                    StartPos 59, Length 1
    char    OrdStatCode         [   1];    // [string,    1] 주문상태코드                    StartPos 60, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 61, Length 1
    char    QryTpCode           [   1];    // [string,    1] 조회구분코드                    StartPos 62, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] 주문유형코드                    StartPos 63, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] 해외파생선물옵션구분코드        StartPos 65, Length 1
} CIDBQ01800OutBlock1, *LPCIDBQ01800OutBlock1;
#define NAME_CIDBQ01800OutBlock1     "CIDBQ01800OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ01800OutBlock2
{
    char    OvrsFutsOrdNo       [  10];    // [string,   10] 해외선물주문번호                StartPos 0, Length 10
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] 해외선물원주문번호              StartPos 10, Length 10
    char    FcmOrdNo            [  15];    // [string,   15] FCM주문번호                     StartPos 20, Length 15
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 35, Length 18
    char    IsuNm               [  50];    // [string,   50] 종목명                          StartPos 53, Length 50
    char    AbrdFutsXrcPrc      [  29];    // [double,29.10] 해외선물행사가격                StartPos 103, Length 29
    char    FcmAcntNo           [  20];    // [string,   20] FCM계좌번호                     StartPos 132, Length 20
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 152, Length 1
    char    BnsTpNm             [  10];    // [string,   10] 매매구분명                      StartPos 153, Length 10
    char    FutsOrdStatCode     [   1];    // [string,    1] 선물주문상태코드                StartPos 163, Length 1
    char    TpCodeNm            [  50];    // [string,   50] 구분코드명                      StartPos 164, Length 50
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 214, Length 1
    char    TrdTpNm             [  20];    // [string,   20] 거래구분명                      StartPos 215, Length 20
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] 해외선물주문유형코드            StartPos 235, Length 1
    char    OrdPtnNm            [  40];    // [string,   40] 주문유형명                      StartPos 236, Length 40
    char    OrdPtnTermTpCode    [   2];    // [string,    2] 주문유형기간구분코드            StartPos 276, Length 2
    char    CmnCodeNm           [ 100];    // [string,  100] 공통코드명                      StartPos 278, Length 100
    char    AppSrtDt            [   8];    // [string,    8] 적용시작일자                    StartPos 378, Length 8
    char    AppEndDt            [   8];    // [string,    8] 적용종료일자                    StartPos 386, Length 8
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] 해외파생주문가격                StartPos 394, Length 25
    char    OrdQty              [  16];    // [long  ,   16] 주문수량                        StartPos 419, Length 16
    char    AbrdFutsExecPrc     [  29];    // [double,29.10] 해외선물체결가격                StartPos 435, Length 29
    char    ExecQty             [  16];    // [long  ,   16] 체결수량                        StartPos 464, Length 16
    char    OrdCndiPrc          [  25];    // [double, 25.8] 주문조건가격                    StartPos 480, Length 25
    char    OvrsDrvtNowPrc      [  25];    // [double, 25.8] 해외파생현재가                  StartPos 505, Length 25
    char    MdfyQty             [  16];    // [long  ,   16] 정정수량                        StartPos 530, Length 16
    char    CancQty             [  16];    // [long  ,   16] 취소수량                        StartPos 546, Length 16
    char    RjtQty              [  13];    // [long  ,   13] 거부수량                        StartPos 562, Length 13
    char    CnfQty              [  16];    // [long  ,   16] 확인수량                        StartPos 575, Length 16
    char    CvrgYn              [   1];    // [string,    1] 반대매매여부                    StartPos 591, Length 1
    char    RegTmnlNo           [   3];    // [string,    3] 등록단말번호                    StartPos 592, Length 3
    char    RegBrnNo            [   3];    // [string,    3] 등록지점번호                    StartPos 595, Length 3
    char    RegUserId           [  16];    // [string,   16] 등록사용자ID                    StartPos 598, Length 16
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 614, Length 8
    char    OrdTime             [   9];    // [string,    9] 주문시각                        StartPos 622, Length 9
    char    OvrsOptXrcRsvTpCode [   1];    // [string,    1] 해외옵션행사예약구분코드        StartPos 631, Length 1
} CIDBQ01800OutBlock2, *LPCIDBQ01800OutBlock2;
#define NAME_CIDBQ01800OutBlock2     "CIDBQ01800OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ01800_H_
