#ifndef _t5501_H_
#define _t5501_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���������ֹ�(t5501) ( base21=SONAT000,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5501     "t5501"

// �Է�
typedef struct _t5501InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 33, Length 12
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 45, Length 16
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 61, Length 13
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 74, Length 1
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 75, Length 2
    char    pgmtype             [   2];    // [string,    2] ���α׷�ȣ�������ڵ�           StartPos 77, Length 2
    char    gongtype            [   1];    // [string,    1] ���ŵ����ɿ���                 StartPos 79, Length 1
    char    gonghoga            [   1];    // [string,    1] ���ŵ�ȣ������                 StartPos 80, Length 1
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 81, Length 2
    char    sinmemecode         [   3];    // [string,    3] �ſ�ŷ��ڵ�                   StartPos 83, Length 3
    char    loandt              [   8];    // [string,    8] ������                         StartPos 86, Length 8
    char    memnumber           [   3];    // [string,    3] ȸ����ȣ                       StartPos 94, Length 3
    char    ordcondgb           [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 97, Length 1
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 98, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 104, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 124, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 134, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 144, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 154, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 164, Length 10
    char    operordnum          [  12];    // [string,   12] ������ù�ȣ                   StartPos 174, Length 12
    char    flowsupgb           [   1];    // [string,    1] �����������ڿ���               StartPos 186, Length 1
    char    oppbuygb            [   1];    // [string,    1] �ݴ�Ÿű���                   StartPos 187, Length 1
} t5501InBlock, *LPt5501InBlock;
#define NAME_t5501InBlock     "t5501InBlock"

// ���
typedef struct _t5501OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 33, Length 12
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 45, Length 16
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 61, Length 13
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 74, Length 1
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 75, Length 2
    char    pgmtype             [   2];    // [string,    2] ���α׷�ȣ�������ڵ�           StartPos 77, Length 2
    char    gongtype            [   1];    // [string,    1] ���ŵ����ɿ���                 StartPos 79, Length 1
    char    gonghoga            [   1];    // [string,    1] ���ŵ�ȣ������                 StartPos 80, Length 1
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 81, Length 2
    char    sinmemecode         [   3];    // [string,    3] �ſ�ŷ��ڵ�                   StartPos 83, Length 3
    char    loandt              [   8];    // [string,    8] ������                         StartPos 86, Length 8
    char    memnumber           [   3];    // [string,    3] ȸ����ȣ                       StartPos 94, Length 3
    char    ordcondgb           [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 97, Length 1
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 98, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 104, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 124, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 134, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 144, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 154, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 164, Length 10
    char    operordnum          [  12];    // [string,   12] ������ù�ȣ                   StartPos 174, Length 12
    char    flowsupgb           [   1];    // [string,    1] �����������ڿ���               StartPos 186, Length 1
    char    oppbuygb            [   1];    // [string,    1] �ݴ�Ÿű���                   StartPos 187, Length 1
} t5501OutBlock, *LPt5501OutBlock;
#define NAME_t5501OutBlock     "t5501OutBlock"

// ���1
typedef struct _t5501OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordno               [  10];    // [long  ,   10] �ֹ���ȣ                       StartPos 5, Length 10
    char    ordtime             [   9];    // [string,    9] �ֹ��ð�                       StartPos 15, Length 9
    char    ordmktcode          [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 24, Length 2
    char    ordcode             [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 26, Length 2
    char    expcode             [   9];    // [string,    9] ���������ȣ                   StartPos 28, Length 9
    char    operno              [   9];    // [string,    9] ���������ȣ                   StartPos 37, Length 9
    char    ordamt              [  16];    // [long  ,   16] �ֹ��ݾ�                       StartPos 46, Length 16
    char    fillermemegb        [  10];    // [long  ,   10] �����ֹ���ȣ                   StartPos 62, Length 10
    char    oppbuyseqno         [  10];    // [long  ,   10] �ݴ�Ÿ��Ϸù�ȣ               StartPos 72, Length 10
    char    preeordno           [  10];    // [long  ,   10] �����ֹ���ȣ                   StartPos 82, Length 10
    char    realordcnt          [  16];    // [long  ,   16] �ǹ��ֹ�����                   StartPos 92, Length 16
    char    recoordcnt          [  16];    // [long  ,   16] �����ֹ�����                 StartPos 108, Length 16
    char    cashordamt          [  16];    // [long  ,   16] �����ֹ��ݾ�                   StartPos 124, Length 16
    char    daeordamt           [  16];    // [long  ,   16] ����ֹ��ݾ�                   StartPos 140, Length 16
    char    recordamt           [  16];    // [long  ,   16] �����ֹ��ݾ�                 StartPos 156, Length 16
    char    accno               [  40];    // [string,   40] ���¸�                         StartPos 172, Length 40
    char    hname               [  40];    // [string,   40] �����                         StartPos 212, Length 40
} t5501OutBlock1, *LPt5501OutBlock1;
#define NAME_t5501OutBlock1     "t5501OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t5501_H_
