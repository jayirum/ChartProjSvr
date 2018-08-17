#ifndef _CIDBT01000_H_
#define _CIDBT01000_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 해외선물 취소주문 ( SERVICE=CIDBT01000,ENCRYPT,SIGNATURE,HEADTYPE=B,CREATOR=최영호,CREDATE=2012/04/26 14:52:30 )
#pragma pack( push, 1 )

#define NAME_CIDBT01000     "CIDBT01000"

// In(*EMPTY*)                    
typedef struct _CIDBT01000InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 5, Length 8
    char    BrnNo               [   3];    // [string,    3] 지점번호                        StartPos 13, Length 3
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 16, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 36, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 44, Length 18
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] 해외선물원주문번호              StartPos 62, Length 10
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 72, Length 1
    char    PrdtTpCode          [   2];    // [string,    2] 상품구분코드                    StartPos 73, Length 2
    char    ExchCode            [  10];    // [string,   10] 거래소코드                      StartPos 75, Length 10
} CIDBT01000InBlock1, *LPCIDBT01000InBlock1;
#define NAME_CIDBT01000InBlock1     "CIDBT01000InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBT01000OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] 주문일자                        StartPos 5, Length 8
    char    BrnNo               [   3];    // [string,    3] 지점번호                        StartPos 13, Length 3
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 16, Length 20
    char    Pwd                 [   8];    // [string,    8] 비밀번호                        StartPos 36, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] 종목코드값                      StartPos 44, Length 18
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] 해외선물원주문번호              StartPos 62, Length 10
    char    FutsOrdTpCode       [   1];    // [string,    1] 선물주문구분코드                StartPos 72, Length 1
    char    PrdtTpCode          [   2];    // [string,    2] 상품구분코드                    StartPos 73, Length 2
    char    ExchCode            [  10];    // [string,   10] 거래소코드                      StartPos 75, Length 10
} CIDBT01000OutBlock1, *LPCIDBT01000OutBlock1;
#define NAME_CIDBT01000OutBlock1     "CIDBT01000OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CIDBT01000OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] 레코드갯수                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] 계좌번호                        StartPos 5, Length 20
    char    OvrsFutsOrdNo       [  10];    // [string,   10] 해외선물주문번호                StartPos 25, Length 10
    char    InnerMsgCnts        [  80];    // [string,   80] 내부메시지내용                  StartPos 35, Length 80
} CIDBT01000OutBlock2, *LPCIDBT01000OutBlock2;
#define NAME_CIDBT01000OutBlock2     "CIDBT01000OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBT01000_H_
