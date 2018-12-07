#ifndef		_ETK_T5513_H_20100802_
#define		_ETK_T5513_H_20100802_


///////////////////////////////////////////////////////////////////////////////////////////////////
// ��������ֹ�(t5513) ( base21=SONBT003,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5513     "t5513"

// �⺻�Է�
typedef struct _t5513InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordmarketcode       [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 5, Length 2
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 7, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 27, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 35, Length 32
    char    fnoordptncode       [   2];    // [string,    2] �����ɼ��ֹ������ڵ�           StartPos 67, Length 2
    char    dummyorgordno       [   3];    // [long  ,    3] dummy���ֹ���ȣ                StartPos 69, Length 3
    char    orgordno            [   7];    // [long  ,    7] ���ֹ���ȣ                     StartPos 72, Length 7
    char    qty                 [  16];    // [long  ,   16] ��Ҽ���                       StartPos 79, Length 16
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 95, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 97, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 103, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 123, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 133, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 143, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 153, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 163, Length 10
    char    mgempno             [   9];    // [string,    9] �������                       StartPos 173, Length 9
    char    fundid              [  12];    // [string,   12] �ݵ�ID                         StartPos 182, Length 12
    char    dummyfundorgordno   [   3];    // [long  ,    3] dummy�ݵ���ֹ���ȣ            StartPos 194, Length 3
    char    fundorgordno        [   7];    // [long  ,    7] �ݵ���ֹ���ȣ                 StartPos 197, Length 7
    char    dummyfundordno      [   3];    // [long  ,    3] dummy�ݵ��ֹ���ȣ              StartPos 204, Length 3
    char    fundordno           [   7];    // [long  ,    7] �ݵ��ֹ���ȣ                   StartPos 207, Length 7
} t5513InBlock, *LPt5513InBlock;
#define NAME_t5513InBlock     "t5513InBlock"

// �⺻���
typedef struct _t5513OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    ordmarketcode       [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 5, Length 2
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 7, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 27, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 35, Length 32
    char    fnoordptncode       [   2];    // [string,    2] �����ɼ��ֹ������ڵ�           StartPos 67, Length 2
    char    dummyorgordno       [   3];    // [long  ,    3] dummy���ֹ���ȣ                StartPos 69, Length 3
    char    orgordno            [   7];    // [long  ,    7] ���ֹ���ȣ                     StartPos 72, Length 7
    char    qty                 [  16];    // [long  ,   16] ��Ҽ���                       StartPos 79, Length 16
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 95, Length 2
    char    stragb              [   6];    // [string,    6] �����ڵ�                       StartPos 97, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 103, Length 20
    char    ordernum            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 123, Length 10
    char    portnum             [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 133, Length 10
    char    basketnum           [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 143, Length 10
    char    tranchnum           [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 153, Length 10
    char    itemnum             [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 163, Length 10
    char    mgempno             [   9];    // [string,    9] �������                       StartPos 173, Length 9
    char    fundid              [  12];    // [string,   12] �ݵ�ID                         StartPos 182, Length 12
    char    dummyfundorgordno   [   3];    // [long  ,    3] dummy�ݵ���ֹ���ȣ            StartPos 194, Length 3
    char    fundorgordno        [   7];    // [long  ,    7] �ݵ���ֹ���ȣ                 StartPos 197, Length 7
    char    dummyfundordno      [   3];    // [long  ,    3] dummy�ݵ��ֹ���ȣ              StartPos 204, Length 3
    char    fundordno           [   7];    // [long  ,    7] �ݵ��ֹ���ȣ                   StartPos 207, Length 7
} t5513OutBlock, *LPt5513OutBlock;
#define NAME_t5513OutBlock     "t5513OutBlock"

// �⺻���1
typedef struct _t5513OutBlock1
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
} t5513OutBlock1, *LPt5513OutBlock1;
#define NAME_t5513OutBlock1     "t5513OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif