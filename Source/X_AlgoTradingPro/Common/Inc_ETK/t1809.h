///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȣ��ȸ(t1809) ( block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1809     "t1809"

// �⺻�Է�
typedef struct _t1809InBlock
{
    char    gubun               [   1];    // [string,    1] ��ȣ����                       StartPos 0, Length 1
    char    jmGb                [   1];    // [string,    1] ���񱸺�                       StartPos 1, Length 1
    char    jmcode              [   6];    // [string,    6] �����ڵ�                       StartPos 2, Length 6
    char    cts                 [  30];    // [string,   30] NEXTKEY                        StartPos 8, Length 30
} t1809InBlock, *LPt1809InBlock;
#define NAME_t1809InBlock     "t1809InBlock"

// �⺻���
typedef struct _t1809OutBlock
{
    char    cts                 [  30];    // [string,   30] NEXTKEY                        StartPos 0, Length 30
} t1809OutBlock, *LPt1809OutBlock;
#define NAME_t1809OutBlock     "t1809OutBlock"

// �⺻���1, occurs
typedef struct _t1809OutBlock1
{
    char    date                [   8];    // [string,    8] ����                           StartPos 0, Length 8
    char    time                [   6];    // [string,    6] �ð�                           StartPos 8, Length 6
    char    signal_id           [   4];    // [string,    4] ��ȣID                         StartPos 14, Length 4
    char    signal_desc         [ 300];    // [string,  300] ��ȣ��                         StartPos 18, Length 300
    char    point               [   3];    // [string,    3] ��ȣ����                       StartPos 318, Length 3
    char    keyword             [  40];    // [string,   40] ������ȣŰ����                 StartPos 321, Length 40
    char    seq                 [  24];    // [string,   24] ��ȣ������                     StartPos 361, Length 24
    char    gubun               [   2];    // [string,    2] ��ȣ����                       StartPos 385, Length 2
    char    jmcode              [   6];    // [string,    6] ��ȣ����                       StartPos 387, Length 6
    char    price               [   7];    // [long  ,    7] ���񰡰�                       StartPos 393, Length 7
    char    sign                [   1];    // [string,    1] ��������ȣ                   StartPos 400, Length 1
    char    chgrate             [   6];    // [float ,  6.2] �������                     StartPos 401, Length 6
    char    volume              [   9];    // [long  ,    9] �ŷ���                         StartPos 407, Length 9
    char    datetime            [  16];    // [string,   16] ��ȣ�Ͻ�                       StartPos 416, Length 16
} t1809OutBlock1, *LPt1809OutBlock1;
#define NAME_t1809OutBlock1     "t1809OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
