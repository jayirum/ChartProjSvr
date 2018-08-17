#ifndef _CIDBQ01500_H_
#define _CIDBQ01500_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� �̰��� �ܰ��� ��ȸ ( SERVICE=CIDBQ01500,ENCRYPT,HEADTYPE=B,CREATOR=��ȣ��,CREDATE=2013/04/11 18:36:15 )
#pragma pack( push, 1 )

#define NAME_CIDBQ01500     "CIDBQ01500"

// In(*EMPTY*)                    
typedef struct _CIDBQ01500InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] ���±����ڵ�                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 6, Length 20
    char    FcmAcntNo           [  20];    // [string,   20] FCM���¹�ȣ                     StartPos 26, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 46, Length 8
    char    QryDt               [   8];    // [string,    8] ��ȸ����                        StartPos 54, Length 8
    char    BalTpCode           [   1];    // [string,    1] �ܰ����ڵ�                    StartPos 62, Length 1
} CIDBQ01500InBlock1, *LPCIDBQ01500InBlock1;
#define NAME_CIDBQ01500InBlock1     "CIDBQ01500InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ01500OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] ���±����ڵ�                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 6, Length 20
    char    FcmAcntNo           [  20];    // [string,   20] FCM���¹�ȣ                     StartPos 26, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 46, Length 8
    char    QryDt               [   8];    // [string,    8] ��ȸ����                        StartPos 54, Length 8
    char    BalTpCode           [   1];    // [string,    1] �ܰ����ڵ�                    StartPos 62, Length 1
} CIDBQ01500OutBlock1, *LPCIDBQ01500OutBlock1;
#define NAME_CIDBQ01500OutBlock1     "CIDBQ01500OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ01500OutBlock2
{
    char    BaseDt              [   8];    // [string,    8] ��������                        StartPos 0, Length 8
    char    Dps                 [  16];    // [long  ,   16] ������                          StartPos 8, Length 16
    char    LpnlAmt             [  19];    // [double, 19.2] û����ͱݾ�                    StartPos 24, Length 19
    char    FutsDueBfLpnlAmt    [  23];    // [double, 23.2] ����������û����ͱݾ�          StartPos 43, Length 23
    char    FutsDueBfCmsn       [  23];    // [double, 23.2] ����������������                StartPos 66, Length 23
    char    CsgnMgn             [  16];    // [long  ,   16] ��Ź���űݾ�                    StartPos 89, Length 16
    char    MaintMgn            [  16];    // [long  ,   16] �������ű�                      StartPos 105, Length 16
    char    CtlmtAmt            [  23];    // [double, 23.2] �ſ��ѵ��ݾ�                    StartPos 121, Length 23
    char    AddMgn              [  16];    // [long  ,   16] �߰����űݾ�                    StartPos 144, Length 16
    char    MgnclRat            [  27];    // [double,27.10] ��������                        StartPos 160, Length 27
    char    OrdAbleAmt          [  16];    // [long  ,   16] �ֹ����ɱݾ�                    StartPos 187, Length 16
    char    WthdwAbleAmt        [  16];    // [long  ,   16] ���Ⱑ�ɱݾ�                    StartPos 203, Length 16
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 219, Length 20
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 239, Length 18
    char    IsuNm               [  50];    // [string,   50] �����                          StartPos 257, Length 50
    char    CrcyCodeVal         [   3];    // [string,    3] ��ȭ�ڵ尪                      StartPos 307, Length 3
    char    OvrsDrvtPrdtCode    [  10];    // [string,   10] �ؿ��Ļ���ǰ�ڵ�                StartPos 310, Length 10
    char    OvrsDrvtOptTpCode   [   1];    // [string,    1] �ؿ��Ļ��ɼǱ����ڵ�            StartPos 320, Length 1
    char    DueDt               [   8];    // [string,    8] ��������                        StartPos 321, Length 8
    char    OvrsDrvtXrcPrc      [  25];    // [double, 25.8] �ؿ��Ļ���簡��                StartPos 329, Length 25
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 354, Length 1
    char    CmnCodeNm           [ 100];    // [string,  100] �����ڵ��                      StartPos 355, Length 100
    char    TpCodeNm            [  50];    // [string,   50] �����ڵ��                      StartPos 455, Length 50
    char    BalQty              [  16];    // [long  ,   16] �ܰ����                        StartPos 505, Length 16
    char    PchsPrc             [  25];    // [double, 25.8] ���԰���                        StartPos 521, Length 25
    char    OvrsDrvtNowPrc      [  25];    // [double, 25.8] �ؿ��Ļ����簡                  StartPos 546, Length 25
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] �ؿܼ����򰡼��ͱݾ�            StartPos 571, Length 19
    char    CsgnCmsn            [  19];    // [double, 19.2] ��Ź������                      StartPos 590, Length 19
    char    PosNo               [  13];    // [string,   13] �����ǹ�ȣ                      StartPos 609, Length 13
    char    EufOneCmsnAmt       [  19];    // [double, 19.2] �ŷ��Һ��1������ݾ�           StartPos 622, Length 19
    char    EufTwoCmsnAmt       [  19];    // [double, 19.2] �ŷ��Һ��2������ݾ�           StartPos 641, Length 19
} CIDBQ01500OutBlock2, *LPCIDBQ01500OutBlock2;
#define NAME_CIDBQ01500OutBlock2     "CIDBQ01500OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ01500_H_
