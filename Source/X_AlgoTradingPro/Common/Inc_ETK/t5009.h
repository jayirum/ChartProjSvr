///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ɺ������º������������ֹ� ( base21=SONBT712,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5009     "t5009"

// �⺻�Է�
typedef struct _t5009InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 5, Length 2
    char    reccnt1             [   5];    // [long  ,    5] ���ڵ尹��1                    StartPos 7, Length 5
} t5009InBlock, *LPt5009InBlock;
#define NAME_t5009InBlock     "t5009InBlock"

// �⺻�Է�1, occurs
typedef struct _t5009InBlock1
{
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 0, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 20, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 28, Length 32
    char    mmgubun             [   1];    // [string,    1] �Ÿű���                       StartPos 60, Length 1
    char    hogagb              [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 61, Length 2
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 63, Length 13
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 76, Length 16
    char    opjisino            [  12];    // [string,   12] ������ù�ȣ                   StartPos 92, Length 12
} t5009InBlock1, *LPt5009InBlock1;
#define NAME_t5009InBlock1     "t5009InBlock1"

// ���
typedef struct _t5009OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 5, Length 2
} t5009OutBlock, *LPt5009OutBlock;
#define NAME_t5009OutBlock     "t5009OutBlock"

// ���1, occurs
typedef struct _t5009OutBlock1
{
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 0, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 20, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 28, Length 32
    char    mmgubun             [   1];    // [string,    1] �Ÿű���                       StartPos 60, Length 1
    char    hogagb              [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 61, Length 2
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 63, Length 13
    char    qty                 [  16];    // [long  ,   16] �ֹ�����                       StartPos 76, Length 16
    char    opjisino            [  12];    // [string,   12] ������ù�ȣ                   StartPos 92, Length 12
} t5009OutBlock1, *LPt5009OutBlock1;
#define NAME_t5009OutBlock1     "t5009OutBlock1"

// ���2, occurs
typedef struct _t5009OutBlock2
{
    char    dummyordno          [   3];    // [long  ,    3] dummy�ֹ���ȣ                  StartPos 0, Length 3
    char    ordno               [   7];    // [long  ,    7] �ֹ���ȣ                       StartPos 3, Length 7
    char    msgcode             [   4];    // [string,    4] �޽����ڵ�                     StartPos 10, Length 4
    char    msg                 [ 100];    // [string,  100] �޽�������                     StartPos 14, Length 100
    char    proctime            [   9];    // [string,    9] ó���ð�                       StartPos 114, Length 9
} t5009OutBlock2, *LPt5009OutBlock2;
#define NAME_t5009OutBlock2     "t5009OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
