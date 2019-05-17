///////////////////////////////////////////////////////////////////////////////////////////////////
// KOSPI �⼼(S4) ( attr,block )
#pragma pack( push, 1 )

#define NAME_S4_     "S4_"

// �Է�
typedef struct _S4__InBlock
{
    char    shcode              [   6];    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} S4__InBlock, *LPS4__InBlock;
#define NAME_S4__InBlock     "InBlock"

// ���
typedef struct _S4__OutBlock
{
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 0, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 2, Length 8
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����                         StartPos 11, Length 6
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 18, Length 8
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] �ð��ð�                       StartPos 27, Length 6
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�                           StartPos 34, Length 8
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] ���ð�                       StartPos 43, Length 6
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��                           StartPos 50, Length 8
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] �����ð�                       StartPos 59, Length 6
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����                           StartPos 66, Length 8
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 75, Length 6
} S4__OutBlock, *LPS4__OutBlock;
#define NAME_S4__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
