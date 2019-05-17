#ifndef		_ETK_T5512_H_20100802_
#define		_ETK_T5512_H_20100802_


///////////////////////////////////////////////////////////////////////////////////////////////////
// ���������ֹ�(t5512) ( base21=SONBT002,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5512     "t5512"

// �⺻�Է�
typedef struct _t5512InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordmarketcode       [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 5, Length 2
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 7, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 27, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 35, Length 32
    char    fnoordptncode       [   2];    // [string,    2] �����ɼ��ֹ������ڵ�           StartPos 67, Length 2
    char    dummyorgordno       [   3];    // [long  ,    3] dummy���ֹ���ȣ                StartPos 69, Length 3
    char    orgordno            [   7];    // [long  ,    7] ���ֹ���ȣ                     StartPos 72, Length 7
    char    offergb             [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 79, Length 2
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 81, Length 13
    char    qty                 [  16];    // [long  ,   16] ��������                       StartPos 94, Length 16
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 110, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 112, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 118, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 138, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 148, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 158, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 168, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 178, Length 10
    char    mgempno             [   9];    // [string,    9] �������                       StartPos 188, Length 9
    char    fundid              [  12];    // [string,   12] �ݵ�ID                         StartPos 197, Length 12
    char    dummyfundorgordno   [   3];    // [long  ,    3] dummy�ݵ���ֹ���ȣ            StartPos 209, Length 3
    char    fundorgordno        [   7];    // [long  ,    7] �ݵ���ֹ���ȣ                 StartPos 212, Length 7
    char    dummyfundordno      [   3];    // [long  ,    3] dummy�ݵ��ֹ���ȣ              StartPos 219, Length 3
    char    fundordno           [   7];    // [long  ,    7] �ݵ��ֹ���ȣ                   StartPos 222, Length 7
} t5512InBlock, *LPt5512InBlock;
#define NAME_t5512InBlock     "t5512InBlock"

// �⺻���
typedef struct _t5512OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordmarketcode       [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 5, Length 2
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 7, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 27, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 35, Length 32
    char    fnoordptncode       [   2];    // [string,    2] �����ɼ��ֹ������ڵ�           StartPos 67, Length 2
    char    dummyorgordno       [   3];    // [long  ,    3] dummy���ֹ���ȣ                StartPos 69, Length 3
    char    orgordno            [   7];    // [long  ,    7] ���ֹ���ȣ                     StartPos 72, Length 7
    char    offergb             [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 79, Length 2
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 81, Length 13
    char    qty                 [  16];    // [long  ,   16] ��������                       StartPos 94, Length 16
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 110, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 112, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 118, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 138, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 148, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 158, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 168, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 178, Length 10
    char    mgempno             [   9];    // [string,    9] �������                       StartPos 188, Length 9
    char    fundid              [  12];    // [string,   12] �ݵ�ID                         StartPos 197, Length 12
    char    dummyfundorgordno   [   3];    // [long  ,    3] dummy�ݵ���ֹ���ȣ            StartPos 209, Length 3
    char    fundorgordno        [   7];    // [long  ,    7] �ݵ���ֹ���ȣ                 StartPos 212, Length 7
    char    dummyfundordno      [   3];    // [long  ,    3] dummy�ݵ��ֹ���ȣ              StartPos 219, Length 3
    char    fundordno           [   7];    // [long  ,    7] �ݵ��ֹ���ȣ                   StartPos 222, Length 7
} t5512OutBlock, *LPt5512OutBlock;
#define NAME_t5512OutBlock     "t5512OutBlock"

// �⺻���1
typedef struct _t5512OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    dummyordno          [   3];    // [long  ,    3] dummy�ֹ���ȣ                  StartPos 5, Length 3
    char    ordno               [   7];    // [long  ,    7] �ֹ���ȣ                       StartPos 8, Length 7
    char    brnnm               [  40];    // [string,   40] ������                         StartPos 15, Length 40
    char    accno               [  40];    // [string,   40] ���¸�                         StartPos 55, Length 40
    char    isunm               [  40];    // [string,   40] �����                         StartPos 95, Length 40
    char    ordableamt          [  16];    // [long  ,   16] �ֹ����ɱݾ�                   StartPos 135, Length 16
    char    mnyordableamt       [  16];    // [long  ,   16] �����ֹ����ɱݾ�               StartPos 151, Length 16
    char    ordmgn              [  16];    // [long  ,   16] �ֹ����űݾ�                   StartPos 167, Length 16
    char    mnyordmgn           [  16];    // [long  ,   16] �����ֹ����űݾ�               StartPos 183, Length 16
    char    ordableqty          [  16];    // [long  ,   16] �ֹ����ɼ���                   StartPos 199, Length 16
} t5512OutBlock1, *LPt5512OutBlock1;
#define NAME_t5512OutBlock1     "t5512OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif