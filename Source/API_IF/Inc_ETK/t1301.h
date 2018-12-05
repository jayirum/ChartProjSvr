///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽĽð��뺰ü����ȸ(t1301) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1301     "t1301"

// �⺻�Է�
typedef struct _t1301InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] Ư�̰ŷ���                     StartPos 7, Length 12
    char    starttime           [   4];    char    _starttime           ;    // [string,    4] ���۽ð�                       StartPos 20, Length 4
    char    endtime             [   4];    char    _endtime             ;    // [string,    4] ����ð�                       StartPos 25, Length 4
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] �ð�CTS                        StartPos 30, Length 10
} t1301InBlock, *LPt1301InBlock;
#define NAME_t1301InBlock     "t1301InBlock"

// ���
typedef struct _t1301OutBlock
{
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] �ð�CTS                        StartPos 0, Length 10
} t1301OutBlock, *LPt1301OutBlock;
#define NAME_t1301OutBlock     "t1301OutBlock"

// ���1, occurs
typedef struct _t1301OutBlock1
{
    char    chetime             [  10];    char    _chetime             ;    // [string,   10] �ð�                           StartPos 0, Length 10
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 11, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 20, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 22, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 31, Length 6
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] ü�����                       StartPos 38, Length 12
    char    chdegree            [   8];    char    _chdegree            ;    // [float ,  8.2] ü�ᰭ��                       StartPos 51, Length 8
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 60, Length 12
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�ü�����                   StartPos 73, Length 12
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�ü��Ǽ�                   StartPos 86, Length 8
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�ü�����                   StartPos 95, Length 12
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�ü��Ǽ�                   StartPos 108, Length 8
    char    revolume            [  12];    char    _revolume            ;    // [long  ,   12] ��ü�ᷮ                       StartPos 117, Length 12
    char    rechecnt            [   8];    char    _rechecnt            ;    // [long  ,    8] ��ü��Ǽ�                     StartPos 130, Length 8
} t1301OutBlock1, *LPt1301OutBlock1;
#define NAME_t1301OutBlock1     "t1301OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
