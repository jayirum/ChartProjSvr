///////////////////////////////////////////////////////////////////////////////////////////////////
// ����������ü�(t1516) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1516     "t1516"

// �⺻�Է�
typedef struct _t1516InBlock
{
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 0, Length 3
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ����                           StartPos 4, Length 1
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 6, Length 6
} t1516InBlock, *LPt1516InBlock;
#define NAME_t1516InBlock     "t1516InBlock"

// ���
typedef struct _t1516OutBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    pricejisu           [  12];    char    _pricejisu           ;    // [float , 12.2] ����                           StartPos 7, Length 12
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 20, Length 1
    char    change              [   7];    char    _change              ;    // [float ,  7.2] ���ϴ��                       StartPos 22, Length 7
    char    jdiff               [   6];    char    _jdiff               ;    // [float ,  6.2] �����                         StartPos 30, Length 6
} t1516OutBlock, *LPt1516OutBlock;
#define NAME_t1516OutBlock     "t1516OutBlock"

// ���1, occurs
typedef struct _t1516OutBlock1
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �����                         StartPos 0, Length 20
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 21, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 30, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 32, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 41, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 48, Length 12
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�                           StartPos 61, Length 8
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��                           StartPos 70, Length 8
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����                           StartPos 79, Length 8
    char    sojinrate           [   6];    char    _sojinrate           ;    // [float ,  6.2] ������                         StartPos 88, Length 6
    char    beta                [   6];    char    _beta                ;    // [float ,  6.5] ��Ÿ���                       StartPos 95, Length 6
    char    perx                [   8];    char    _perx                ;    // [float ,  8.2] PER                            StartPos 102, Length 8
    char    frgsvolume          [  12];    char    _frgsvolume          ;    // [long  ,   12] ���μ��ż�                     StartPos 111, Length 12
    char    orgsvolume          [  12];    char    _orgsvolume          ;    // [long  ,   12] ������ż�                     StartPos 124, Length 12
    char    diff_vol            [  10];    char    _diff_vol            ;    // [float , 10.2] �ŷ�������                     StartPos 137, Length 10
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 148, Length 6
    char    total               [  12];    char    _total               ;    // [long  ,   12] �ð��Ѿ�                       StartPos 155, Length 12
    char    value               [  12];    char    _value               ;    // [long  ,   12] �ŷ����                       StartPos 168, Length 12
} t1516OutBlock1, *LPt1516OutBlock1;
#define NAME_t1516OutBlock1     "t1516OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
