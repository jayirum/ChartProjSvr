///////////////////////////////////////////////////////////////////////////////////////////////////
// ETF����������ȸ(t1904) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1904     "t1904"

// �⺻�Է�
typedef struct _t1904InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    cts_jcode           [  12];    char    _cts_jcode           ;    // [string,   12] ����CTS                        StartPos 7, Length 12
} t1904InBlock, *LPt1904InBlock;
#define NAME_t1904InBlock     "t1904InBlock"

// ���
typedef struct _t1904OutBlock
{
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 0, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 9, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 11, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 20, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 27, Length 12
    char    etftotcap           [  12];    char    _etftotcap           ;    // [long  ,   12] ���ڻ��Ѿ�                     StartPos 40, Length 12
    char    etfnum              [   4];    char    _etfnum              ;    // [long  ,    4] �����                         StartPos 53, Length 4
    char    cash                [  12];    char    _cash                ;    // [long  ,   12] ����                           StartPos 58, Length 12
    char    cts_jcode           [  12];    char    _cts_jcode           ;    // [string,   12] ����CTS                        StartPos 71, Length 12
} t1904OutBlock, *LPt1904OutBlock;
#define NAME_t1904OutBlock     "t1904OutBlock"

// ���1, occurs
typedef struct _t1904OutBlock1
{
    char    shcode              [  12];    char    _shcode              ;    // [string,   12] �����ڵ�                       StartPos 0, Length 12
    char    hname               [  20];    char    _hname               ;    // [string,   20] �ѱ۸�                         StartPos 13, Length 20
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 34, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 43, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 45, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 54, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 61, Length 12
    char    icux                [   6];    char    _icux                ;    // [long  ,    6] �������Ǽ�                     StartPos 74, Length 6
    char    profitdate          [   8];    char    _profitdate          ;    // [string,    8] ���ͺй������                 StartPos 81, Length 8
    char    parprice            [   8];    char    _parprice            ;    // [long  ,    8] �׸�ݾ�                       StartPos 90, Length 8
} t1904OutBlock1, *LPt1904OutBlock1;
#define NAME_t1904OutBlock1     "t1904OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
