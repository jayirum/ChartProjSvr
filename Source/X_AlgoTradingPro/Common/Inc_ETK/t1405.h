///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ڰ��/�Ÿ�����/�����Ÿ���ȸ(t1405) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1405     "t1405"

// �⺻�Է�
typedef struct _t1405InBlock
{
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ����                           StartPos 0, Length 1
    char    jongchk             [   1];    char    _jongchk             ;    // [string,    1] ����üũ                       StartPos 2, Length 1
    char    cts_shcode          [   6];    char    _cts_shcode          ;    // [string,    6] �����ڵ�_CTS                   StartPos 4, Length 6
} t1405InBlock, *LPt1405InBlock;
#define NAME_t1405InBlock     "t1405InBlock"

// ���
typedef struct _t1405OutBlock
{
    char    cts_shcode          [   6];    char    _cts_shcode          ;    // [string,    6] �����ڵ�_CTS                   StartPos 0, Length 6
} t1405OutBlock, *LPt1405OutBlock;
#define NAME_t1405OutBlock     "t1405OutBlock"

// ���1, occurs
typedef struct _t1405OutBlock1
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �ѱ۸�                         StartPos 0, Length 20
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 21, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 30, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 32, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 41, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 48, Length 12
    char    date                [   8];    char    _date                ;    // [string,    8] ������                         StartPos 61, Length 8
    char    edate               [   8];    char    _edate               ;    // [string,    8] ������                         StartPos 70, Length 8
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 79, Length 6
} t1405OutBlock1, *LPt1405OutBlock1;
#define NAME_t1405OutBlock1     "t1405OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
