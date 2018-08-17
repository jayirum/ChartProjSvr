#ifndef _CIDEQ00800_H_
#define _CIDEQ00800_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ں� �̰��� �ܰ��� ( SERVICE=CIDEQ00800,ENCRYPT,HEADTYPE=B,CREATOR=�ڽ���,CREDATE=2012/05/03 23:15:35 )
#pragma pack( push, 1 )

#define NAME_CIDEQ00800     "CIDEQ00800"

// In(*EMPTY*)                    
typedef struct _CIDEQ00800InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    AcntPwd             [   8];    // [string,    8] ���º�й�ȣ                    StartPos 25, Length 8
    char    TrdDt               [   8];    // [string,    8] �ŷ�����                        StartPos 33, Length 8
} CIDEQ00800InBlock1, *LPCIDEQ00800InBlock1;
#define NAME_CIDEQ00800InBlock1     "CIDEQ00800InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDEQ00800OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    AcntPwd             [   8];    // [string,    8] ���º�й�ȣ                    StartPos 25, Length 8
    char    TrdDt               [   8];    // [string,    8] �ŷ�����                        StartPos 33, Length 8
} CIDEQ00800OutBlock1, *LPCIDEQ00800OutBlock1;
#define NAME_CIDEQ00800OutBlock1     "CIDEQ00800OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDEQ00800OutBlock2
{
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 0, Length 20
    char    TrdDt               [   8];    // [string,    8] �ŷ�����                        StartPos 20, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 28, Length 18
    char    BnsTpNm             [  10];    // [string,   10] �Ÿű��и�                      StartPos 46, Length 10
    char    BalQty              [  16];    // [long  ,   16] �ܰ����                        StartPos 56, Length 16
    char    LqdtAbleQty         [  16];    // [long  ,   16] û�갡�ɼ���                    StartPos 72, Length 16
    char    PchsPrc             [  25];    // [double, 25.8] ���԰���                        StartPos 88, Length 25
    char    OvrsDrvtNowPrc      [  25];    // [double, 25.8] �ؿ��Ļ����簡                  StartPos 113, Length 25
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] �ؿܼ����򰡼��ͱݾ�            StartPos 138, Length 19
    char    CustmBalAmt         [  19];    // [double, 19.2] ���ܰ�ݾ�                    StartPos 157, Length 19
    char    FcurrEvalAmt        [  21];    // [double, 21.4] ��ȭ�򰡱ݾ�                    StartPos 176, Length 21
    char    IsuNm               [  50];    // [string,   50] �����                          StartPos 197, Length 50
    char    CrcyCodeVal         [   3];    // [string,    3] ��ȭ�ڵ尪                      StartPos 247, Length 3
    char    OvrsDrvtPrdtCode    [  10];    // [string,   10] �ؿ��Ļ���ǰ�ڵ�                StartPos 250, Length 10
    char    DueDt               [   8];    // [string,    8] ��������                        StartPos 260, Length 8
    char    PrcntrAmt           [  19];    // [double, 19.2] ����ݾ�                      StartPos 268, Length 19
    char    FcurrEvalPnlAmt     [  21];    // [double, 21.4] ��ȭ�򰡼��ͱݾ�                StartPos 287, Length 21
} CIDEQ00800OutBlock2, *LPCIDEQ00800OutBlock2;
#define NAME_CIDEQ00800OutBlock2     "CIDEQ00800OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDEQ00800_H_
