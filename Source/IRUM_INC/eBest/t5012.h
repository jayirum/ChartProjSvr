#ifndef _t5012_H_
#define _t5012_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ŷ��� �������ŵ�(t5012) ( base21=SONAT013,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5012     "t5012"

// �⺻�Է�
typedef struct _t5012InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    futaccno            [  20];    // [string,   20] �������¹�ȣ                   StartPos 33, Length 20
    char    futmarketgb         [   1];    // [string,    1] �������屸��                   StartPos 53, Length 1
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 54, Length 12
    char    futdaegb            [   1];    // [string,    1] ������뱸��                   StartPos 66, Length 1
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 67, Length 16
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 83, Length 13
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 96, Length 2
    char    ordcondgb           [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 98, Length 1
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 99, Length 2
} t5012InBlock, *LPt5012InBlock;
#define NAME_t5012InBlock     "t5012InBlock"

// �⺻���
typedef struct _t5012OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    futaccno            [  20];    // [string,   20] �������¹�ȣ                   StartPos 33, Length 20
    char    futmarketgb         [   1];    // [string,    1] �������屸��                   StartPos 53, Length 1
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 54, Length 12
    char    futdaegb            [   1];    // [string,    1] ������뱸��                   StartPos 66, Length 1
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 67, Length 16
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 83, Length 13
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 96, Length 2
    char    ordcondgb           [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 98, Length 1
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 99, Length 2
} t5012OutBlock, *LPt5012OutBlock;
#define NAME_t5012OutBlock     "t5012OutBlock"

// �⺻���1
typedef struct _t5012OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordno               [  10];    // [long  ,   10] �ֹ���ȣ                       StartPos 5, Length 10
} t5012OutBlock1, *LPt5012OutBlock1;
#define NAME_t5012OutBlock1     "t5012OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t5012_H_
