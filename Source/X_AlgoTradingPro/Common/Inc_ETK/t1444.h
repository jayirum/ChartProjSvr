///////////////////////////////////////////////////////////////////////////////////////////////////
// �ð��Ѿ׻���(t1444) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1444     "t1444"

// �⺻�Է�
typedef struct _t1444InBlock
{
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 0, Length 3
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 4, Length 4
} t1444InBlock, *LPt1444InBlock;
#define NAME_t1444InBlock     "t1444InBlock"

// ���
typedef struct _t1444OutBlock
{
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 0, Length 4
} t1444OutBlock, *LPt1444OutBlock;
#define NAME_t1444OutBlock     "t1444OutBlock"

// ���1, occurs
typedef struct _t1444OutBlock1
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    hname               [  20];    char    _hname               ;    // [string,   20] �����                         StartPos 7, Length 20
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 28, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 37, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 39, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 48, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 55, Length 12
    char    vol_rate            [   6];    char    _vol_rate            ;    // [float ,  6.2] �ŷ�����                       StartPos 68, Length 6
    char    total               [  12];    char    _total               ;    // [long  ,   12] �ð��Ѿ�                       StartPos 75, Length 12
    char    rate                [   6];    char    _rate                ;    // [float ,  6.2] ����                           StartPos 88, Length 6
    char    for_rate            [   6];    char    _for_rate            ;    // [float ,  6.2] ���κ���                       StartPos 95, Length 6
} t1444OutBlock1, *LPt1444OutBlock1;
#define NAME_t1444OutBlock1     "t1444OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
