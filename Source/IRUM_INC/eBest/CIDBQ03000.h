#ifndef _CIDBQ03000_H_
#define _CIDBQ03000_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� ������/�ܰ���Ȳ ( SERVICE=CIDBQ03000,HEADTYPE=B,CREATOR=��ȣ��,CREDATE=2015/06/25 09:12:31 )
#pragma pack( push, 1 )

#define NAME_CIDBQ03000     "CIDBQ03000"

// In(*EMPTY*)                    
typedef struct _CIDBQ03000InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] ���±����ڵ�                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 6, Length 20
    char    AcntPwd             [   8];    // [string,    8] ���º�й�ȣ                    StartPos 26, Length 8
    char    TrdDt               [   8];    // [string,    8] �ŷ�����                        StartPos 34, Length 8
} CIDBQ03000InBlock1, *LPCIDBQ03000InBlock1;
#define NAME_CIDBQ03000InBlock1     "CIDBQ03000InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ03000OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntTpCode          [   1];    // [string,    1] ���±����ڵ�                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 6, Length 20
    char    AcntPwd             [   8];    // [string,    8] ���º�й�ȣ                    StartPos 26, Length 8
    char    TrdDt               [   8];    // [string,    8] �ŷ�����                        StartPos 34, Length 8
} CIDBQ03000OutBlock1, *LPCIDBQ03000OutBlock1;
#define NAME_CIDBQ03000OutBlock1     "CIDBQ03000OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ03000OutBlock2
{
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 0, Length 20
    char    TrdDt               [   8];    // [string,    8] �ŷ�����                        StartPos 20, Length 8
    char    CrcyObjCode         [  12];    // [string,   12] ��ȭ����ڵ�                    StartPos 28, Length 12
    char    OvrsFutsDps         [  23];    // [double, 23.2] �ؿܼ���������                  StartPos 40, Length 23
    char    CustmMnyioAmt       [  19];    // [double, 19.2] ������ݱݾ�                  StartPos 63, Length 19
    char    AbrdFutsLqdtPnlAmt  [  19];    // [double, 19.2] �ؿܼ���û����ͱݾ�            StartPos 82, Length 19
    char    AbrdFutsCmsnAmt     [  19];    // [double, 19.2] �ؿܼ���������ݾ�              StartPos 101, Length 19
    char    PrexchDps           [  19];    // [double, 19.2] ��ȯ��������                    StartPos 120, Length 19
    char    EvalAssetAmt        [  19];    // [double, 19.2] ���ڻ�ݾ�                    StartPos 139, Length 19
    char    AbrdFutsCsgnMgn     [  19];    // [double, 19.2] �ؿܼ�����Ź���űݾ�            StartPos 158, Length 19
    char    AbrdFutsAddMgn      [  19];    // [double, 19.2] �ؿܼ����߰����űݾ�            StartPos 177, Length 19
    char    AbrdFutsWthdwAbleAmt[  19];    // [double, 19.2] �ؿܼ������Ⱑ�ɱݾ�            StartPos 196, Length 19
    char    AbrdFutsOrdAbleAmt  [  19];    // [double, 19.2] �ؿܼ����ֹ����ɱݾ�            StartPos 215, Length 19
    char    AbrdFutsEvalPnlAmt  [  19];    // [double, 19.2] �ؿܼ����򰡼��ͱݾ�            StartPos 234, Length 19
    char    LastSettPnlAmt      [  19];    // [double, 19.2] �����������ͱݾ�                StartPos 253, Length 19
    char    OvrsOptSettAmt      [  19];    // [double, 19.2] �ؿܿɼǰ����ݾ�                StartPos 272, Length 19
    char    OvrsOptBalEvalAmt   [  19];    // [double, 19.2] �ؿܿɼ��ܰ��򰡱ݾ�            StartPos 291, Length 19
} CIDBQ03000OutBlock2, *LPCIDBQ03000OutBlock2;
#define NAME_CIDBQ03000OutBlock2     "CIDBQ03000OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ03000_H_
