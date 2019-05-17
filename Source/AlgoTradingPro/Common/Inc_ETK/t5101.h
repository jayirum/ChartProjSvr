#ifndef _t5101_H_
#define _t5101_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �������������ֹ� ( base21=SONAT016,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5101     "t5101"

// �Է�
typedef struct _t5101InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 5, Length 2
    char    ordercnt            [   6];    // [long  ,    6] �����ֹ�Ƚ��                   StartPos 7, Length 6
    char    reccnt1             [   5];    // [long  ,    5] ���ڵ尹��1                    StartPos 13, Length 5
} t5101InBlock, *LPt5101InBlock;
#define NAME_t5101InBlock     "t5101InBlock"

// �Է�, occurs
typedef struct _t5101InBlock1
{
    char    ordergb             [   1];    // [string,    1] �ֹ�ó������                   StartPos 0, Length 1
    char    orgordno            [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 1, Length 10
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 11, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 31, Length 8
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 39, Length 1
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 40, Length 12
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 52, Length 16
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 68, Length 13
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 81, Length 2
    char    gongtype            [   1];    // [string,    1] ���ŵ����ɿ���                 StartPos 83, Length 1
    char    ghogagb             [   1];    // [string,    1] ���ŵ�ȣ������                 StartPos 84, Length 1
    char    ordcondgb           [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 85, Length 1
    char    pgmtype             [   2];    // [string,    2] ���α׷�ȣ�������ڵ�           StartPos 86, Length 2
    char    operordnum          [  12];    // [string,   12] ������ù�ȣ                   StartPos 88, Length 12
    char    sincd               [   3];    // [string,    3] �ſ�ŷ��ڵ�                   StartPos 100, Length 3
    char    loandt              [   8];    // [string,    8] ������                         StartPos 103, Length 8
    char    flowsupgb           [   1];    // [string,    1] �����������ڿ���               StartPos 111, Length 1
    char    multiordno          [  10];    // [long  ,   10] �����ֹ��Ϸù�ȣ               StartPos 112, Length 10
    char    ordno               [  10];    // [long  ,   10] �ֹ���ȣ                       StartPos 122, Length 10
    char    msgcode             [   4];    // [string,    4] �޼����ڵ�                     StartPos 132, Length 4
    char    msg                 [ 100];    // [string,  100] �޼�������                     StartPos 136, Length 100
    char    proctm              [   9];    // [string,    9] ó���ð�                       StartPos 236, Length 9
} t5101InBlock1, *LPt5101InBlock1;
#define NAME_t5101InBlock1     "t5101InBlock1"

// �Է�
typedef struct _t5101OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 5, Length 2
    char    ordercnt            [   6];    // [long  ,    6] �����ֹ�Ƚ��                   StartPos 7, Length 6
} t5101OutBlock, *LPt5101OutBlock;
#define NAME_t5101OutBlock     "t5101OutBlock"

// �Է�, occurs
typedef struct _t5101OutBlock1
{
    char    ordergb             [   1];    // [string,    1] �ֹ�ó������                   StartPos 0, Length 1
    char    orgordno            [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 1, Length 10
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 11, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 31, Length 8
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 39, Length 1
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 40, Length 12
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 52, Length 16
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 68, Length 13
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 81, Length 2
    char    gongtype            [   1];    // [string,    1] ���ŵ����ɿ���                 StartPos 83, Length 1
    char    ghogagb             [   1];    // [string,    1] ���ŵ�ȣ������                 StartPos 84, Length 1
    char    ordcondgb           [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 85, Length 1
    char    pgmtype             [   2];    // [string,    2] ���α׷�ȣ�������ڵ�           StartPos 86, Length 2
    char    operordnum          [  12];    // [string,   12] ������ù�ȣ                   StartPos 88, Length 12
    char    sincd               [   3];    // [string,    3] �ſ�ŷ��ڵ�                   StartPos 100, Length 3
    char    loandt              [   8];    // [string,    8] ������                         StartPos 103, Length 8
    char    flowsupgb           [   1];    // [string,    1] �����������ڿ���               StartPos 111, Length 1
    char    multiordno          [  10];    // [long  ,   10] �����ֹ��Ϸù�ȣ               StartPos 112, Length 10
    char    ordno               [  10];    // [long  ,   10] �ֹ���ȣ                       StartPos 122, Length 10
    char    msgcode             [   4];    // [string,    4] �޼����ڵ�                     StartPos 132, Length 4
    char    msg                 [ 100];    // [string,  100] �޼�������                     StartPos 136, Length 100
    char    proctm              [   9];    // [string,    9] ó���ð�                       StartPos 236, Length 9
} t5101OutBlock1, *LPt5101OutBlock1;
#define NAME_t5101OutBlock1     "t5101OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t5101_H_
