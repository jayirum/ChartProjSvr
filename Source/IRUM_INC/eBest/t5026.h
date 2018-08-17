///////////////////////////////////////////////////////////////////////////////////////////////////
// �ϰ�û���ֹ�/���ɿ�����ȸ(t5026) ( base21=SONBT701,headtype=B )
#pragma pack( push, 1 )

#define NAME_t5026     "t5026"

// �⺻�Է�
typedef struct _t5026InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    qrygb               [   1];    // [string,    1] ��ȸó������                   StartPos 33, Length 1
    char    fotradgb            [   2];    // [string,    2] �����ɼǰŷ������ڵ�           StartPos 34, Length 2
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 36, Length 2
    char    gb                  [   1];    // [string,    1] ó������                       StartPos 38, Length 1
} t5026InBlock, *LPt5026InBlock;
#define NAME_t5026InBlock     "t5026InBlock"

// �⺻���
typedef struct _t5026OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 25, Length 8
    char    qrygb               [   1];    // [string,    1] ��ȸó������                   StartPos 33, Length 1
    char    fotradgb            [   2];    // [string,    2] �����ɼǰŷ������ڵ�           StartPos 34, Length 2
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 36, Length 2
    char    gb                  [   1];    // [string,    1] ó������                       StartPos 38, Length 1
} t5026OutBlock, *LPt5026OutBlock;
#define NAME_t5026OutBlock     "t5026OutBlock"

// �⺻���1
typedef struct _t5026OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  40];    // [string,   40] ���¸�                         StartPos 5, Length 40
    char    ordgb               [   1];    // [string,    1] �ֹ�ó������                   StartPos 45, Length 1
    char    ordgbnm             [  20];    // [string,   20] �ֹ�ó�����и�                 StartPos 46, Length 20
} t5026OutBlock1, *LPt5026OutBlock1;
#define NAME_t5026OutBlock1     "t5026OutBlock1"

// �⺻���2, occurs
typedef struct _t5026OutBlock2
{
    char    seq                 [   4];    // [long  ,    4] ����                           StartPos 0, Length 4
    char    offergb             [   1];    // [string,    1] �����ɼ�������������           StartPos 4, Length 1
    char    focode              [  32];    // [string,   32] �����ɼ������ȣ               StartPos 5, Length 32
    char    mmgb                [   1];    // [string,    1] �Ÿű���                       StartPos 37, Length 1
    char    mmgbnm              [  10];    // [string,   10] �Ÿű��и�                     StartPos 38, Length 10
    char    openyak             [  16];    // [long  ,   16] �̰�������                     StartPos 48, Length 16
    char    micheqty            [  16];    // [long  ,   16] ��ü�����                     StartPos 64, Length 16
    char    price               [  13];    // [double, 13.2] ���簡                         StartPos 80, Length 13
    char    banhoqty            [  16];    // [long  ,   16] �ݴ�ȣ������                   StartPos 93, Length 16
    char    ordgb               [   1];    // [string,    1] �ֹ�ó������                   StartPos 109, Length 1
    char    ordgbnm             [  20];    // [string,   20] �ֹ�ó�����и�                 StartPos 110, Length 20
} t5026OutBlock2, *LPt5026OutBlock2;
#define NAME_t5026OutBlock2     "t5026OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
