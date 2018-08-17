///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽĽð��뺰ü����ȸíƮ(t1308) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1308     "t1308"

// �⺻�Է�
typedef struct _t1308InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    starttime           [   4];    char    _starttime           ;    // [string,    4] ���۽ð�                       StartPos 7, Length 4
    char    endtime             [   4];    char    _endtime             ;    // [string,    4] ����ð�                       StartPos 12, Length 4
    char    bun_term            [   2];    char    _bun_term            ;    // [string,    2] �а���                         StartPos 17, Length 2
} t1308InBlock, *LPt1308InBlock;
#define NAME_t1308InBlock     "t1308InBlock"

// ���1, occurs
typedef struct _t1308OutBlock1
{
    char    chetime             [   8];    char    _chetime             ;    // [string,    8] �ð�                           StartPos 0, Length 8
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 9, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 18, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 20, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 29, Length 6
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ü�����                       StartPos 36, Length 8
    char    chdegvol            [   8];    char    _chdegvol            ;    // [float ,  8.2] ü�ᰭ��(�ŷ���)               StartPos 45, Length 8
    char    chdegcnt            [   8];    char    _chdegcnt            ;    // [float ,  8.2] ü�ᰭ��(�Ǽ�)                 StartPos 54, Length 8
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 63, Length 12
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�ü�����                   StartPos 76, Length 12
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�ü��Ǽ�                   StartPos 89, Length 8
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�ü�����                   StartPos 98, Length 12
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�ü��Ǽ�                   StartPos 111, Length 8
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�                           StartPos 120, Length 8
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��                           StartPos 129, Length 8
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����                           StartPos 138, Length 8
} t1308OutBlock1, *LPt1308OutBlock1;
#define NAME_t1308OutBlock1     "t1308OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
