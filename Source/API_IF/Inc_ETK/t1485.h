///////////////////////////////////////////////////////////////////////////////////////////////////
// ��������(t1485) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1485     "t1485"

// �⺻�Է�
typedef struct _t1485InBlock
{
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 0, Length 3
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ��ȸ����                       StartPos 4, Length 1
} t1485InBlock, *LPt1485InBlock;
#define NAME_t1485InBlock     "t1485InBlock"

// ���
typedef struct _t1485OutBlock
{
    char    pricejisu           [  10];    char    _pricejisu           ;    // [float , 10.2] ��������                       StartPos 0, Length 10
    char    sign                [   1];    char    _sign                ;    // [string,    1] �������ϴ�񱸺�               StartPos 11, Length 1
    char    change              [  10];    char    _change              ;    // [float , 10.2] ���ϴ��                       StartPos 13, Length 10
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 24, Length 12
    char    yhighjo             [   4];    char    _yhighjo             ;    // [long  ,    4] ��������                     StartPos 37, Length 4
    char    yupjo               [   4];    char    _yupjo               ;    // [long  ,    4] ���������                     StartPos 42, Length 4
    char    yunchgjo            [   4];    char    _yunchgjo            ;    // [long  ,    4] ���������                     StartPos 47, Length 4
    char    ylowjo              [   4];    char    _ylowjo              ;    // [long  ,    4] �϶������                     StartPos 52, Length 4
    char    ydownjo             [   4];    char    _ydownjo             ;    // [long  ,    4] ���������                     StartPos 57, Length 4
    char    ytrajo              [   4];    char    _ytrajo              ;    // [long  ,    4] �ŷ�������                     StartPos 62, Length 4
} t1485OutBlock, *LPt1485OutBlock;
#define NAME_t1485OutBlock     "t1485OutBlock"

// ���1, occurs
typedef struct _t1485OutBlock1
{
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] �ð�                           StartPos 0, Length 6
    char    jisu                [  10];    char    _jisu                ;    // [float , 10.2] ��������                       StartPos 7, Length 10
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 18, Length 1
    char    change              [  10];    char    _change              ;    // [float , 10.2] ���ϴ��                       StartPos 20, Length 10
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] ����ü�ᷮ                     StartPos 31, Length 12
    char    volcha              [  12];    char    _volcha              ;    // [long  ,   12] ����ü�ᷮ�������             StartPos 44, Length 12
} t1485OutBlock1, *LPt1485OutBlock1;
#define NAME_t1485OutBlock1     "t1485OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
