#ifndef		_ETK_T5511_H_20100802_
#define		_ETK_T5511_H_20100802_

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���������ֹ�(t5511) ( base21=SONBT001,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5511     "t5511"

// �⺻�Է�
typedef struct _t5511InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordmarketcode       [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 5, Length 2
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 7, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 27, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 35, Length 32
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 67, Length 1
    char    offergb             [   2];    // [string,    2] �����ɼ��ֹ������ڵ�           StartPos 68, Length 2
    char    hogagb              [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 70, Length 2
    char    tradegb             [   2];    // [string,    2] �����ɼǰŷ������ڵ�           StartPos 72, Length 2
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 74, Length 13
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 87, Length 16
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 103, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 105, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 111, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 131, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 141, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 151, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 161, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 171, Length 10
    char    operordnum          [  12];    // [string,   12] ������ù�ȣ                   StartPos 181, Length 12
    char    mgempno             [   9];    // [string,    9] ���������ȣ                   StartPos 193, Length 9
    char    fundid              [  12];    // [string,   12] �ݵ�ID                         StartPos 202, Length 12
    char    dummyfundorgordno   [   3];    // [long  ,    3] dummy�ݵ���ֹ���ȣ            StartPos 214, Length 3
    char    fundorgordno        [   7];    // [long  ,    7] �ݵ���ֹ���ȣ                 StartPos 217, Length 7
} t5511InBlock, *LPt5511InBlock;
#define NAME_t5511InBlock     "t5511InBlock"

// �⺻���
typedef struct _t5511OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordmarketcode       [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 5, Length 2
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 7, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 27, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 35, Length 32
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 67, Length 1
    char    offergb             [   2];    // [string,    2] �����ɼ��ֹ������ڵ�           StartPos 68, Length 2
    char    hogagb              [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 70, Length 2
    char    tradegb             [   2];    // [string,    2] �����ɼǰŷ������ڵ�           StartPos 72, Length 2
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 74, Length 13
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 87, Length 16
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 103, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 105, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 111, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 131, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 141, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 151, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 161, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 171, Length 10
    char    operordnum          [  12];    // [string,   12] ������ù�ȣ                   StartPos 181, Length 12
    char    mgempno             [   9];    // [string,    9] ���������ȣ                   StartPos 193, Length 9
    char    fundid              [  12];    // [string,   12] �ݵ�ID                         StartPos 202, Length 12
    char    dummyfundorgordno   [   3];    // [long  ,    3] dummy�ݵ���ֹ���ȣ            StartPos 214, Length 3
    char    fundorgordno        [   7];    // [long  ,    7] �ݵ���ֹ���ȣ                 StartPos 217, Length 7
} t5511OutBlock, *LPt5511OutBlock;
#define NAME_t5511OutBlock     "t5511OutBlock"

// �⺻���1
typedef struct _t5511OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    dummyordno          [   3];    // [long  ,    3] dummy�ֹ���ȣ                  StartPos 5, Length 3
    char    ordno               [   7];    // [long  ,    7] �ֹ���ȣ                       StartPos 8, Length 7
    char    brnnm               [  40];    // [string,   40] ������                         StartPos 15, Length 40
    char    accnm               [  40];    // [string,   40] ���¸�                         StartPos 55, Length 40
    char    isunm               [  40];    // [string,   40] �����                         StartPos 95, Length 40
    char    ordableamt          [  16];    // [long  ,   16] �ֹ����ɱݾ�                   StartPos 135, Length 16
    char    mnyordableamt       [  16];    // [long  ,   16] �����ֹ����ɱݾ�               StartPos 151, Length 16
    char    ordmgn              [  16];    // [long  ,   16] �ֹ����űݾ�                   StartPos 167, Length 16
    char    mnyordmgn           [  16];    // [long  ,   16] �����ֹ����űݾ�               StartPos 183, Length 16
    char    ordableqty          [  16];    // [long  ,   16] �ֹ����ɼ���                   StartPos 199, Length 16
} t5511OutBlock1, *LPt5511OutBlock1;
#define NAME_t5511OutBlock1     "t5511OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif