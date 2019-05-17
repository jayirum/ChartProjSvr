///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ǰ�(t3401) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t3401     "t3401"

// �⺻�Է�
typedef struct _t3401InBlock
{
    char    shcode              [   9];    char    _shcode              ;    // [string,    9] �����ڵ�                       StartPos 0, Length 9
    char    gubun1              [   1];    char    _gubun1              ;    // [string,    1] ����                           StartPos 10, Length 1
    char    tradno              [   3];    char    _tradno              ;    // [string,    3] ȸ�����ڵ�                     StartPos 12, Length 3
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] IDXDATE                        StartPos 16, Length 8
} t3401InBlock, *LPt3401InBlock;
#define NAME_t3401InBlock     "t3401InBlock"

// ���
typedef struct _t3401OutBlock
{
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] IDXDATE                        StartPos 0, Length 8
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 9, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���Ӽ�                       StartPos 18, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���                           StartPos 20, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 29, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 36, Length 12
    char    value               [  12];    char    _value               ;    // [long  ,   12] �ŷ����                       StartPos 49, Length 12
} t3401OutBlock, *LPt3401OutBlock;
#define NAME_t3401OutBlock     "t3401OutBlock"

// ���1, occurs
typedef struct _t3401OutBlock1
{
    char    shcode              [   9];    char    _shcode              ;    // [string,    9] �����ڵ�                       StartPos 0, Length 9
    char    tradno              [   3];    char    _tradno              ;    // [string,    3] ȸ�����ڵ�                     StartPos 10, Length 3
    char    date                [   8];    char    _date                ;    // [string,    8] �ǰ�����                       StartPos 14, Length 8
    char    tradname            [  30];    char    _tradname            ;    // [string,   30] ȸ�����                       StartPos 23, Length 30
    char    bopn                [  30];    char    _bopn                ;    // [string,   30] �����ǰߺ�����                 StartPos 54, Length 30
    char    nopn                [  30];    char    _nopn                ;    // [string,   30] �����ǰߺ�����                 StartPos 85, Length 30
    char    boga                [  12];    char    _boga                ;    // [long  ,   12] ��ǥ��������                   StartPos 116, Length 12
    char    noga                [  12];    char    _noga                ;    // [long  ,   12] ��ǥ��������                   StartPos 129, Length 12
    char    close               [   8];    char    _close               ;    // [long  ,    8] �ǰ�������                     StartPos 142, Length 8
} t3401OutBlock1, *LPt3401OutBlock1;
#define NAME_t3401OutBlock1     "t3401OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
