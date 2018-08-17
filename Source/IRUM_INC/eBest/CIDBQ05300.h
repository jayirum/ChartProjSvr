#ifndef _CIDBQ05300_H_
#define _CIDBQ05300_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� ���¿�Ź�ڻ� ��ȸ ( SERVICE=CIDBQ05300,HEADTYPE=B,CREATOR=��ȣ��,CREDATE=2015/06/22 19:42:57 )
#pragma pack( push, 1 )

#define NAME_CIDBQ05300     "CIDBQ05300"

// In(*EMPTY*)                    
typedef struct _CIDBQ05300InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OvrsAcntTpCode      [   1];    // [string,    1] �ؿܰ��±����ڵ�                StartPos 5, Length 1
    char    FcmAcntNo           [  20];    // [string,   20] FCM���¹�ȣ                     StartPos 6, Length 20
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 26, Length 20
    char    AcntPwd             [   8];    // [string,    8] ���º�й�ȣ                    StartPos 46, Length 8
    char    CrcyCode            [   3];    // [string,    3] ��ȭ�ڵ�                        StartPos 54, Length 3
} CIDBQ05300InBlock1, *LPCIDBQ05300InBlock1;
#define NAME_CIDBQ05300InBlock1     "CIDBQ05300InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ05300OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OvrsAcntTpCode      [   1];    // [string,    1] �ؿܰ��±����ڵ�                StartPos 5, Length 1
    char    FcmAcntNo           [  20];    // [string,   20] FCM���¹�ȣ                     StartPos 6, Length 20
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 26, Length 20
    char    AcntPwd             [   8];    // [string,    8] ���º�й�ȣ                    StartPos 46, Length 8
    char    CrcyCode            [   3];    // [string,    3] ��ȭ�ڵ�                        StartPos 54, Length 3
} CIDBQ05300OutBlock1, *LPCIDBQ05300OutBlock1;
#define NAME_CIDBQ05300OutBlock1     "CIDBQ05300OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ05300OutBlock2
{
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 0, Length 20
    char    CrcyCode            [   3];    // [string,    3] ��ȭ�ڵ�                        StartPos 20, Length 3
    char    OvrsFutsDps         [  23];    // [double, 23.2] �ؿܼ���������                  StartPos 23, Length 23
    char    AbrdFutsCsgnMgn     [  19];    // [double, 19.2] �ؿܼ�����Ź���űݾ�            StartPos 46, Length 19
    char    OvrsFutsSplmMgn     [  23];    // [double, 23.2] �ؿܼ����߰����ű�              StartPos 65, Length 23
    char    CustmLpnlAmt        [  19];    // [double, 19.2] ��û����ͱݾ�                StartPos 88, Length 19
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] �ؿܼ����򰡼��ͱݾ�            StartPos 107, Length 19
    char    AbrdFutsCmsnAmt     [  19];    // [double, 19.2] �ؿܼ���������ݾ�              StartPos 126, Length 19
    char    AbrdFutsEvalDpstgTotAmt[  19];    // [double, 19.2] �ؿܼ����򰡿�Ź�ѱݾ�          StartPos 145, Length 19
    char    Xchrat              [  15];    // [double, 15.4] ȯ��                            StartPos 164, Length 15
    char    FcurrRealMxchgAmt   [  19];    // [double, 19.2] ��ȭ��ȯ���ݾ�                  StartPos 179, Length 19
    char    AbrdFutsWthdwAbleAmt[  19];    // [double, 19.2] �ؿܼ������Ⱑ�ɱݾ�            StartPos 198, Length 19
    char    AbrdFutsOrdAbleAmt  [  19];    // [double, 19.2] �ؿܼ����ֹ����ɱݾ�            StartPos 217, Length 19
    char    FutsDueNarrvLqdtPnlAmt[  19];    // [double, 19.2] ��������̵���û����ͱݾ�      StartPos 236, Length 19
    char    FutsDueNarrvCmsn    [  19];    // [double, 19.2] ��������̵���������            StartPos 255, Length 19
    char    AbrdFutsLqdtPnlAmt  [  19];    // [double, 19.2] �ؿܼ���û����ͱݾ�            StartPos 274, Length 19
    char    OvrsFutsDueCmsn     [  19];    // [double, 19.2] �ؿܼ������������              StartPos 293, Length 19
    char    OvrsFutsOptBuyAmt   [  23];    // [double, 23.2] �ؿܼ����ɼǸż��ݾ�            StartPos 312, Length 23
    char    OvrsFutsOptSellAmt  [  23];    // [double, 23.2] �ؿܼ����ɼǸŵ��ݾ�            StartPos 335, Length 23
    char    OptBuyMktWrthAmt    [  19];    // [double, 19.2] �ɼǸż����尡ġ�ݾ�            StartPos 358, Length 19
    char    OptSellMktWrthAmt   [  19];    // [double, 19.2] �ɼǸŵ����尡ġ�ݾ�            StartPos 377, Length 19
} CIDBQ05300OutBlock2, *LPCIDBQ05300OutBlock2;
#define NAME_CIDBQ05300OutBlock2     "CIDBQ05300OutBlock2"

// SelOut(*EMPTY*)                
typedef struct _CIDBQ05300OutBlock3
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OvrsFutsDps         [  23];    // [double, 23.2] �ؿܼ���������                  StartPos 5, Length 23
    char    AbrdFutsLqdtPnlAmt  [  19];    // [double, 19.2] �ؿܼ���û����ͱݾ�            StartPos 28, Length 19
    char    FutsDueNarrvLqdtPnlAmt[  19];    // [double, 19.2] ��������̵���û����ͱݾ�      StartPos 47, Length 19
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] �ؿܼ����򰡼��ͱݾ�            StartPos 66, Length 19
    char    AbrdFutsEvalDpstgTotAmt[  19];    // [double, 19.2] �ؿܼ����򰡿�Ź�ѱݾ�          StartPos 85, Length 19
    char    CustmLpnlAmt        [  19];    // [double, 19.2] ��û����ͱݾ�                StartPos 104, Length 19
    char    OvrsFutsDueCmsn     [  19];    // [double, 19.2] �ؿܼ������������              StartPos 123, Length 19
    char    FcurrRealMxchgAmt   [  19];    // [double, 19.2] ��ȭ��ȯ���ݾ�                  StartPos 142, Length 19
    char    AbrdFutsCmsnAmt     [  19];    // [double, 19.2] �ؿܼ���������ݾ�              StartPos 161, Length 19
    char    FutsDueNarrvCmsn    [  19];    // [double, 19.2] ��������̵���������            StartPos 180, Length 19
    char    AbrdFutsCsgnMgn     [  19];    // [double, 19.2] �ؿܼ�����Ź���űݾ�            StartPos 199, Length 19
    char    OvrsFutsMaintMgn    [  19];    // [double, 19.2] �ؿܼ����������ű�              StartPos 218, Length 19
    char    OvrsFutsOptBuyAmt   [  23];    // [double, 23.2] �ؿܼ����ɼǸż��ݾ�            StartPos 237, Length 23
    char    OvrsFutsOptSellAmt  [  23];    // [double, 23.2] �ؿܼ����ɼǸŵ��ݾ�            StartPos 260, Length 23
    char    CtlmtAmt            [  23];    // [double, 23.2] �ſ��ѵ��ݾ�                    StartPos 283, Length 23
    char    OvrsFutsSplmMgn     [  23];    // [double, 23.2] �ؿܼ����߰����ű�              StartPos 306, Length 23
    char    MgnclRat            [  27];    // [double,27.10] ��������                        StartPos 329, Length 27
    char    AbrdFutsOrdAbleAmt  [  19];    // [double, 19.2] �ؿܼ����ֹ����ɱݾ�            StartPos 356, Length 19
    char    AbrdFutsWthdwAbleAmt[  19];    // [double, 19.2] �ؿܼ������Ⱑ�ɱݾ�            StartPos 375, Length 19
    char    OptBuyMktWrthAmt    [  19];    // [double, 19.2] �ɼǸż����尡ġ�ݾ�            StartPos 394, Length 19
    char    OptSellMktWrthAmt   [  19];    // [double, 19.2] �ɼǸŵ����尡ġ�ݾ�            StartPos 413, Length 19
    char    OvrsOptSettAmt      [  19];    // [double, 19.2] �ؿܿɼǰ����ݾ�                StartPos 432, Length 19
    char    OvrsOptBalEvalAmt   [  19];    // [double, 19.2] �ؿܿɼ��ܰ��򰡱ݾ�            StartPos 451, Length 19
} CIDBQ05300OutBlock3, *LPCIDBQ05300OutBlock3;
#define NAME_CIDBQ05300OutBlock3     "CIDBQ05300OutBlock3"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ05300_H_
