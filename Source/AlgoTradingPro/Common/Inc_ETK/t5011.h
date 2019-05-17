///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ɺ������º�����������ֹ� ( base21=SONBT714,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5011     "t5011"

// �⺻�Է�
typedef struct _t5011InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 5, Length 2
    char    reccnt1             [   5];    // [long  ,    5] ���ڵ尹��1                    StartPos 7, Length 5
} t5011InBlock, *LPt5011InBlock;
#define NAME_t5011InBlock     "t5011InBlock"

// �⺻�Է�1, occurs
typedef struct _t5011InBlock1
{
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 0, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 20, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 28, Length 32
    char    dummyorgordno       [   3];    // [long  ,    3] dummy���ֹ���ȣ                StartPos 60, Length 3
    char    orgordno            [   7];    // [long  ,    7] ���ֹ���ȣ                     StartPos 63, Length 7
    char    qty                 [  16];    // [long  ,   16] ��Ҽ���                       StartPos 70, Length 16
} t5011InBlock1, *LPt5011InBlock1;
#define NAME_t5011InBlock1     "t5011InBlock1"

// �⺻���
typedef struct _t5011OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 5, Length 2
} t5011OutBlock, *LPt5011OutBlock;
#define NAME_t5011OutBlock     "t5011OutBlock"

// �⺻���1, occurs
typedef struct _t5011OutBlock1
{
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 0, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 20, Length 8
    char    expcode             [  32];    // [string,   32] �����ɼ������ȣ               StartPos 28, Length 32
    char    dummyorgordno       [   3];    // [long  ,    3] dummy���ֹ���ȣ                StartPos 60, Length 3
    char    orgordno            [   7];    // [long  ,    7] ���ֹ���ȣ                     StartPos 63, Length 7
    char    qty                 [  16];    // [long  ,   16] ��Ҽ���                       StartPos 70, Length 16
} t5011OutBlock1, *LPt5011OutBlock1;
#define NAME_t5011OutBlock1     "t5011OutBlock1"

// �⺻���2, occurs
typedef struct _t5011OutBlock2
{
    char    dummyordno          [   3];    // [long  ,    3] dummy�ֹ���ȣ                  StartPos 0, Length 3
    char    ordno               [   7];    // [long  ,    7] �ֹ���ȣ                       StartPos 3, Length 7
    char    msgcode             [   4];    // [string,    4] �޽����ڵ�                     StartPos 10, Length 4
    char    msg                 [ 100];    // [string,  100] �޽�������                     StartPos 14, Length 100
    char    proctime            [   9];    // [string,    9] ó���ð�                       StartPos 114, Length 9
} t5011OutBlock2, *LPt5011OutBlock2;
#define NAME_t5011OutBlock2     "t5011OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
