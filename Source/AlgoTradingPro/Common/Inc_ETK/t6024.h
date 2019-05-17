///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֹ����ɼ���/���ű�(t6024) ( base21=SONBQ103,headtype=B )
#pragma pack( push, 1 )

#define NAME_t6024     "t6024"

// �⺻�Է�
typedef struct _t6024InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    gubun               [   1];    // [string,    1] �ݾ׼�������                   StartPos 33, Length 1
    char    fohogagb            [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 34, Length 2
    char    ordamt              [  16];    // [long  ,   16] �ֹ��ݾ�                       StartPos 36, Length 16
} t6024InBlock, *LPt6024InBlock;
#define NAME_t6024InBlock     "t6024InBlock"

// �⺻�Է�1
typedef struct _t6024InBlock1
{
    char    reccnt2             [   5];    // [long  ,    5] ���ڵ尹��2                    StartPos 0, Length 5
} t6024InBlock1, *LPt6024InBlock1;
#define NAME_t6024InBlock1     "t6024InBlock1"

// �⺻�Է�2, occurs
typedef struct _t6024InBlock2
{
    char    focodeno            [  32];    // [string,   32] �����ɼ������ȣ               StartPos 0, Length 32
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 32, Length 1
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 33, Length 13
    char    ordqty              [  16];    // [long  ,   16] �ֹ�����                       StartPos 46, Length 16
} t6024InBlock2, *LPt6024InBlock2;
#define NAME_t6024InBlock2     "t6024InBlock2"

// �⺻���
typedef struct _t6024OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    gubun               [   1];    // [string,    1] �ݾ׼�������                   StartPos 33, Length 1
    char    fohogagb            [   2];    // [string,    2] �����ɼ�ȣ�������ڵ�           StartPos 34, Length 2
    char    ordamt              [  16];    // [long  ,   16] �ֹ��ݾ�                       StartPos 36, Length 16
} t6024OutBlock, *LPt6024OutBlock;
#define NAME_t6024OutBlock     "t6024OutBlock"

// �⺻���1, occurs
typedef struct _t6024OutBlock1
{
    char    focodeno            [  32];    // [string,   32] �����ɼ������ȣ               StartPos 0, Length 32
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 32, Length 1
    char    price               [  13];    // [double, 13.2] �ֹ���                         StartPos 33, Length 13
    char    ordqty              [  16];    // [long  ,   16] �ֹ�����                       StartPos 46, Length 16
} t6024OutBlock1, *LPt6024OutBlock1;
#define NAME_t6024OutBlock1     "t6024OutBlock1"

// �⺻���2
typedef struct _t6024OutBlock2
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accname             [  40];    // [string,   40] ���¸�                         StartPos 5, Length 40
    char    date                [   8];    // [string,    8] �ֹ���                         StartPos 45, Length 8
    char    ordnamtqty          [  16];    // [long  ,   16] �ֹ����ɱݾ�                   StartPos 53, Length 16
    char    ordcashamt          [  16];    // [long  ,   16] �����ֹ����ɱݾ�               StartPos 69, Length 16
    char    orddyamt            [  16];    // [long  ,   16] ����ֹ����ɱݾ�               StartPos 85, Length 16
    char    syfpriceamt         [  16];    // [long  ,   16] �ҿ伱�����űݾ�               StartPos 101, Length 16
    char    syfcashamt          [  16];    // [long  ,   16] �ҿ伱���������űݾ�           StartPos 117, Length 16
    char    syfdyamt            [  16];    // [long  ,   16] �ҿ伱��������űݾ�           StartPos 133, Length 16
    char    syoptpriceamt       [  16];    // [long  ,   16] �ҿ�ɼ����űݾ�               StartPos 149, Length 16
    char    syoptchashamt       [  16];    // [long  ,   16] �ҿ�ɼ��������űݾ�           StartPos 165, Length 16
    char    syoptdyamt          [  16];    // [long  ,   16] �ҿ�ɼǴ�����űݾ�           StartPos 181, Length 16
    char    syspdpriceamt       [  16];    // [long  ,   16] �ҿ佺���������űݾ�           StartPos 197, Length 16
    char    syspdcashamt        [  16];    // [long  ,   16] �ҿ佺�������������űݾ�       StartPos 213, Length 16
    char    syspddyamt          [  16];    // [long  ,   16] �ҿ佺�����������űݾ�       StartPos 229, Length 16
    char    sypriceamt          [  16];    // [long  ,   16] �ҿ����űݾ�                   StartPos 245, Length 16
    char    sychashamt          [  16];    // [long  ,   16] �ҿ��������űݾ�               StartPos 261, Length 16
    char    sydyamt             [  16];    // [long  ,   16] �ҿ������űݾ�               StartPos 277, Length 16
    char    namamt              [  16];    // [long  ,   16] ���ű��ܾ�                     StartPos 293, Length 16
    char    cashnamamt          [  16];    // [long  ,   16] �������ű��ܾ�                 StartPos 309, Length 16
    char    dynamamt            [  16];    // [long  ,   16] ������ű��ܾ�                 StartPos 325, Length 16
} t6024OutBlock2, *LPt6024OutBlock2;
#define NAME_t6024OutBlock2     "t6024OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
