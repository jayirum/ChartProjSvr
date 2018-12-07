///////////////////////////////////////////////////////////////////////////////////////////////////
// ����/�ɼ�ü��/��ü��(t0434) ( tuxcode=t0434,headtype=D )
#pragma pack( push, 1 )

#define NAME_t0434     "t0434"

// �⺻�Է�
typedef struct _t0434InBlock
{
    char    accno               [  11];    // [string,   11] ���¹�ȣ                       StartPos 0, Length 11
    char    passwd              [   8];    // [string,    8] ��й�ȣ                       StartPos 11, Length 8
    char    expcode             [   8];    // [string,    8] �����ȣ                       StartPos 19, Length 8
    char    chegb               [   1];    // [string,    1] ü�ᱸ��                       StartPos 27, Length 1
    char    sortgb              [   1];    // [string,    1] ���ļ���                       StartPos 28, Length 1
    char    cts_ordno           [   7];    // [string,    7] CTS_�ֹ���ȣ                   StartPos 29, Length 7
} t0434InBlock, *LPt0434InBlock;
#define NAME_t0434InBlock     "t0434InBlock"

// ���
typedef struct _t0434OutBlock
{
    char    cts_ordno           [   7];    // [string,    7] CTS_�ֹ���ȣ                   StartPos 0, Length 7
} t0434OutBlock, *LPt0434OutBlock;
#define NAME_t0434OutBlock     "t0434OutBlock"

// ���1, occurs
typedef struct _t0434OutBlock1
{
    char    ordno               [   7];    // [long  ,    7] �ֹ���ȣ                       StartPos 0, Length 7
    char    orgordno            [   7];    // [long  ,    7] ���ֹ���ȣ                     StartPos 7, Length 7
    char    medosu              [  10];    // [string,   10] ����                           StartPos 14, Length 10
    char    ordgb               [  20];    // [string,   20] ����                           StartPos 24, Length 20
    char    qty                 [   9];    // [long  ,    9] �ֹ�����                       StartPos 44, Length 9
    char    price               [   9];    // [float ,  9.2] �ֹ�����                       StartPos 53, Length 9
    char    cheqty              [   9];    // [long  ,    9] ü�����                       StartPos 62, Length 9
    char    cheprice            [   9];    // [float ,  9.2] ü�ᰡ��                       StartPos 71, Length 9
    char    ordrem              [   9];    // [long  ,    9] ��ü���ܷ�                     StartPos 80, Length 9
    char    status              [  10];    // [string,   10] ����                           StartPos 89, Length 10
    char    ordtime             [   8];    // [string,    8] �ֹ��ð�                       StartPos 99, Length 8
    char    ordermtd            [  10];    // [string,   10] �ֹ���ü                       StartPos 107, Length 10
    char    expcode             [   8];    // [string,    8] �����ȣ                       StartPos 117, Length 8
    char    rtcode              [   3];    // [string,    3] �����ڵ�                       StartPos 125, Length 3
    char    sysprocseq          [  10];    // [long  ,   10] ó������                       StartPos 128, Length 10
    char    hogatype            [   1];    // [string,    1] ȣ��Ÿ��                       StartPos 138, Length 1
} t0434OutBlock1, *LPt0434OutBlock1;
#define NAME_t0434OutBlock1     "t0434OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
