///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ݴ뺰�Ÿź�����ȸ(t1449) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1449     "t1449"

// �⺻�Է�
typedef struct _t1449InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    dategb              [   1];    char    _dategb              ;    // [string,    1] ���ڱ���                       StartPos 7, Length 1
} t1449InBlock, *LPt1449InBlock;
#define NAME_t1449InBlock     "t1449InBlock"

// ���
typedef struct _t1449OutBlock
{
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 0, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 9, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 11, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 20, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 27, Length 12
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�ü�ᷮ                     StartPos 40, Length 12
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�ü�ᷮ                     StartPos 53, Length 12
} t1449OutBlock, *LPt1449OutBlock;
#define NAME_t1449OutBlock     "t1449OutBlock"

// ���1, occurs
typedef struct _t1449OutBlock1
{
    char    price               [   8];    char    _price               ;    // [long  ,    8] ü�ᰡ                         StartPos 0, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 9, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 11, Length 8
    char    tickdiff            [   6];    char    _tickdiff            ;    // [float ,  6.2] �����                         StartPos 20, Length 6
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] ü�����                       StartPos 27, Length 12
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] ����                           StartPos 40, Length 6
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�ü�ᷮ                     StartPos 47, Length 12
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�ü�ᷮ                     StartPos 60, Length 12
    char    msdiff              [   6];    char    _msdiff              ;    // [float ,  6.2] �ż�����                       StartPos 73, Length 6
} t1449OutBlock1, *LPt1449OutBlock1;
#define NAME_t1449OutBlock1     "t1449OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
