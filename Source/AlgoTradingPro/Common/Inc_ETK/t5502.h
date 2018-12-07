#ifndef _t5502_H_
#define _t5502_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���������ֹ�(t5502) ( base21=SONAT001,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5502     "t5502"

// �⺻�Է�
typedef struct _t5502InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    orgordno            [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 5, Length 10
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 15, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 35, Length 8
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 43, Length 12
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 55, Length 16
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 71, Length 2
    char    memegb              [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 73, Length 1
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 74, Length 13
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 87, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 89, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 95, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 115, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 125, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 135, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 145, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 155, Length 10
} t5502InBlock, *LPt5502InBlock;
#define NAME_t5502InBlock     "t5502InBlock"

// �⺻���
typedef struct _t5502OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    orgordno            [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 5, Length 10
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 15, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 35, Length 8
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 43, Length 12
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 55, Length 16
    char    hogagb              [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 71, Length 2
    char    memegb              [   1];    // [string,    1] �ֹ����Ǳ���                   StartPos 73, Length 1
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 74, Length 13
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 87, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 89, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 95, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 115, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 125, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 135, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 145, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 155, Length 10
} t5502OutBlock, *LPt5502OutBlock;
#define NAME_t5502OutBlock     "t5502OutBlock"

// �⺻���1
typedef struct _t5502OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordno               [  10];    // [long  ,   10] �ֹ���ȣ                       StartPos 5, Length 10
    char    mordno              [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 15, Length 10
    char    ordtime             [   9];    // [string,    9] �ֹ��ð�                       StartPos 25, Length 9
    char    ordmktcode          [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 34, Length 2
    char    ordcode             [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 36, Length 2
    char    expcode             [   9];    // [string,    9] ���������ȣ                   StartPos 38, Length 9
    char    pgmtype             [   2];    // [string,    2] ���α׷�ȣ�������ڵ�           StartPos 47, Length 2
    char    gongtype            [   1];    // [string,    1] ���ŵ�ȣ������                 StartPos 49, Length 1
    char    gonghoga            [   1];    // [string,    1] ���ŵ����ɿ���                 StartPos 50, Length 1
    char    sinmemecode         [   3];    // [string,    3] �ſ�ŷ��ڵ�                   StartPos 51, Length 3
    char    loandt              [   8];    // [string,    8] ������                         StartPos 54, Length 8
    char    oppbuygb            [   1];    // [string,    1] �ݴ�Ÿ��ֹ�����               StartPos 62, Length 1
    char    fowsupgb            [   1];    // [string,    1] �����������ڿ���               StartPos 63, Length 1
    char    operno              [   9];    // [string,    9] ���������ȣ                   StartPos 64, Length 9
    char    ordamt              [  16];    // [long  ,   16] �ֹ��ݾ�                       StartPos 73, Length 16
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 89, Length 1
    char    fillermemegb        [  10];    // [long  ,   10] �����ֹ���ȣ                   StartPos 90, Length 10
    char    oppbuyseqno         [  10];    // [long  ,   10] �ݴ�Ÿ��Ϸù�ȣ               StartPos 100, Length 10
    char    preeordno           [  10];    // [long  ,   10] �����ֹ���ȣ                   StartPos 110, Length 10
    char    cashordamt          [  16];    // [long  ,   16] �����ֹ��ݾ�                   StartPos 120, Length 16
    char    daeordamt           [  16];    // [long  ,   16] ����ֹ��ݾ�                   StartPos 136, Length 16
    char    recordamt           [  16];    // [long  ,   16] �����ֹ��ݾ�                 StartPos 152, Length 16
    char    accno               [  40];    // [string,   40] ���¸�                         StartPos 168, Length 40
    char    hname               [  40];    // [string,   40] �����                         StartPos 208, Length 40
} t5502OutBlock1, *LPt5502OutBlock1;
#define NAME_t5502OutBlock1     "t5502OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t5502_H_
