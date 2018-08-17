#ifndef _CIDBQ02400_H_
#define _CIDBQ02400_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 주문체결내역 상세 조회 ( SERVICE=CIDBQ02400,ENCRYPT,HEADTYPE=B,CREATOR=이호섭,CREDATE=2015/06/18 18:34:41 )
#pragma pack( push, 1 )

#define NAME_CIDBQ02400     "CIDBQ02400"

// In(*EMPTY*)                    
typedef struct _CIDBQ02400InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 33, Length 18
    char    QrySrtDt            [   8];    // [string,    8] 조회시작일자                    StartPos 51, Length 8
    char    QryEndDt            [   8];    // [string,    8] 조회종료일자                    StartPos 59, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] 당일구분코드                    StartPos 67, Length 1
    char    OrdStatCode         [   1];    // [string,    1] 주문상태코드                    StartPos 68, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 69, Length 1
    char    QryTpCode           [   1];    // [string,    1] 조회구분코드                    StartPos 70, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] 주문유형코드                    StartPos 71, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] 해외파생선물옵션구분코드        StartPos 73, Length 1
} CIDBQ02400InBlock1, *LPCIDBQ02400InBlock1;
#define NAME_CIDBQ02400InBlock1     "CIDBQ02400InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ02400OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 33, Length 18
    char    QrySrtDt            [   8];    // [string,    8] 조회시작일자                    StartPos 51, Length 8
    char    QryEndDt            [   8];    // [string,    8] 조회종료일자                    StartPos 59, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] 당일구분코드                    StartPos 67, Length 1
    char    OrdStatCode         [   1];    // [string,    1] 주문상태코드                    StartPos 68, Length 1
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 69, Length 1
    char    QryTpCode           [   1];    // [string,    1] 조회구분코드                    StartPos 70, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] 주문유형코드                    StartPos 71, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] 해외파생선물옵션구분코드        StartPos 73, Length 1
} CIDBQ02400OutBlock1, *LPCIDBQ02400OutBlock1;
#define NAME_CIDBQ02400OutBlock1     "CIDBQ02400OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ02400OutBlock2
{
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 0, Length 8
    char    OvrsFutsOrdNo       [  10];    // [string,   10] 해외선물주문번호                StartPos 8, Length 10
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] 해외선물원주문번호              StartPos 18, Length 10
    char    FcmOrdNo            [  15];    // [string,   15] FCM주문번호                     StartPos 28, Length 15
    char    OvrsFutsExecNo      [  10];    // [string,   10] 해외선물체결번호                StartPos 43, Length 10
    char    FcmAcntNo           [  20];    // [string,   20] FCM계좌번호                     StartPos 53, Length 20
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 73, Length 18
    char    IsuNm               [  50];    // [string,   50] 종목명                          StartPos 91, Length 50
    char    AbrdFutsXrcPrc      [  29];    // [double,29.10] 해외선물행사가격                StartPos 141, Length 29
    char    BnsTpCode           [   1];    // [string,    1] 매매구분코드                    StartPos 170, Length 1
    char    BnsTpNm             [  10];    // [string,   10] 매매구분명                      StartPos 171, Length 10
    char    FutsOrdStatCode     [   1];    // [string,    1] 선물주문상태코드                StartPos 181, Length 1
    char    TpCodeNm            [  50];    // [string,   50] 구분코드명                      StartPos 182, Length 50
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 232, Length 1
    char    TrdTpNm             [  20];    // [string,   20] 거래구분명                      StartPos 233, Length 20
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] 해외선물주문유형코드            StartPos 253, Length 1
    char    OrdPtnNm            [  40];    // [string,   40] 주문유형명                      StartPos 254, Length 40
    char    OrdPtnTermTpCode    [   2];    // [string,    2] 주문유형기간구분코드            StartPos 294, Length 2
    char    CmnCodeNm           [ 100];    // [string,  100] 공통코드명                      StartPos 296, Length 100
    char    AppSrtDt            [   8];    // [string,    8] 적용시작일자                    StartPos 396, Length 8
    char    AppEndDt            [   8];    // [string,    8] 적용종료일자                    StartPos 404, Length 8
    char    OrdQty              [  16];    // [long  ,   16] 주문수량                        StartPos 412, Length 16
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] 해외파생주문가격                StartPos 428, Length 25
    char    ExecQty             [  16];    // [long  ,   16] 체결수량                        StartPos 453, Length 16
    char    AbrdFutsExecPrc     [  29];    // [double,29.10] 해외선물체결가격                StartPos 469, Length 29
    char    OrdCndiPrc          [  25];    // [double, 25.8] 주문조건가격                    StartPos 498, Length 25
    char    NowPrc              [  15];    // [double, 15.2] 현재가                          StartPos 523, Length 15
    char    TrxStatCode         [   2];    // [string,    2] 처리상태코드                    StartPos 538, Length 2
    char    TrxStatCodeNm       [  40];    // [string,   40] 처리상태코드명                  StartPos 540, Length 40
    char    CsgnCmsn            [  19];    // [double, 19.2] 위탁수수료                      StartPos 580, Length 19
    char    FcmCmsn             [  21];    // [double, 21.4] FCM수수료                       StartPos 599, Length 21
    char    ThcoCmsn            [  19];    // [double, 19.2] 당사수수료                      StartPos 620, Length 19
    char    MdaCode             [   2];    // [string,    2] 매체코드                        StartPos 639, Length 2
    char    MdaCodeNm           [  40];    // [string,   40] 매체코드명                      StartPos 641, Length 40
    char    RegTmnlNo           [   3];    // [string,    3] 등록단말번호                    StartPos 681, Length 3
    char    RegUserId           [  16];    // [string,   16] 등록사용자ID                    StartPos 684, Length 16
    char    OrdDttm             [  30];    // [string,   30] 주문일시                        StartPos 700, Length 30
    char    OrdTime             [   9];    // [string,    9] 주문시각                        StartPos 730, Length 9
    char    ExecDt              [   8];    // [string,    8] 체결일자                        StartPos 739, Length 8
    char    ExecTime            [   9];    // [string,    9] 체결시각                        StartPos 747, Length 9
    char    EufOneCmsnAmt       [  19];    // [double, 19.2] 거래소비용1수수료금액           StartPos 756, Length 19
    char    EufTwoCmsnAmt       [  19];    // [double, 19.2] 거래소비용2수수료금액           StartPos 775, Length 19
    char    LchOneCmsnAmt       [  19];    // [double, 19.2] 런던청산소1수수료금액           StartPos 794, Length 19
    char    LchTwoCmsnAmt       [  19];    // [double, 19.2] 런던청산소2수수료금액           StartPos 813, Length 19
    char    TrdOneCmsnAmt       [  19];    // [double, 19.2] 거래1수수료금액                 StartPos 832, Length 19
    char    TrdTwoCmsnAmt       [  19];    // [double, 19.2] 거래2수수료금액                 StartPos 851, Length 19
    char    TrdThreeCmsnAmt     [  19];    // [double, 19.2] 거래3수수료금액                 StartPos 870, Length 19
    char    StrmOneCmsnAmt      [  19];    // [double, 19.2] 단기1수수료금액                 StartPos 889, Length 19
    char    StrmTwoCmsnAmt      [  19];    // [double, 19.2] 단기2수수료금액                 StartPos 908, Length 19
    char    StrmThreeCmsnAmt    [  19];    // [double, 19.2] 단기3수수료금액                 StartPos 927, Length 19
    char    TransOneCmsnAmt     [  19];    // [double, 19.2] 전달1수수료금액                 StartPos 946, Length 19
    char    TransTwoCmsnAmt     [  19];    // [double, 19.2] 전달2수수료금액                 StartPos 965, Length 19
    char    TransThreeCmsnAmt   [  19];    // [double, 19.2] 전달3수수료금액                 StartPos 984, Length 19
    char    TransFourCmsnAmt    [  19];    // [double, 19.2] 전달4수수료금액                 StartPos 1003, Length 19
    char    OvrsOptXrcRsvTpCode [   1];    // [string,    1] 해외옵션행사예약구분코드        StartPos 1022, Length 1
} CIDBQ02400OutBlock2, *LPCIDBQ02400OutBlock2;
#define NAME_CIDBQ02400OutBlock2     "CIDBQ02400OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ02400_H_
