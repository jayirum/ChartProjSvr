///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽĺк��ְ���ȸ(t1302) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1302     "t1302"

// �⺻�Է�
typedef struct _t1302InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] �۾�����                       StartPos 7, Length 1
    char    time                [   6];    char    _time                ;    // [string,    6] �ð�                           StartPos 9, Length 6
    char    cnt                 [   3];    char    _cnt                 ;    // [string,    3] �Ǽ�                           StartPos 16, Length 3
} t1302InBlock, *LPt1302InBlock;
#define NAME_t1302InBlock     "t1302InBlock"

// ���
typedef struct _t1302OutBlock
{
    char    cts_time            [   6];    char    _cts_time            ;    // [string,    6] �ð�CTS                        StartPos 0, Length 6
} t1302OutBlock, *LPt1302OutBlock;
#define NAME_t1302OutBlock     "t1302OutBlock"

// ���1, occurs
typedef struct _t1302OutBlock1
{
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] �ð�                           StartPos 0, Length 6
    char    close               [   8];    char    _close               ;    // [long  ,    8] ����                           StartPos 7, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 16, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 18, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 27, Length 6
    char    chdegree            [   8];    char    _chdegree            ;    // [float ,  8.2] ü�ᰭ��                       StartPos 34, Length 8
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�ü�����                   StartPos 43, Length 12
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�ü�����                   StartPos 56, Length 12
    char    revolume            [  12];    char    _revolume            ;    // [long  ,   12] ���ż�ü�ᷮ                   StartPos 69, Length 12
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�ü��Ǽ�                   StartPos 82, Length 8
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�ü��Ǽ�                   StartPos 91, Length 8
    char    rechecnt            [   8];    char    _rechecnt            ;    // [long  ,    8] ��ü��Ǽ�                     StartPos 100, Length 8
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 109, Length 12
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�                           StartPos 122, Length 8
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��                           StartPos 131, Length 8
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����                           StartPos 140, Length 8
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] ü�ᷮ                         StartPos 149, Length 12
    char    mdchecnttm          [   8];    char    _mdchecnttm          ;    // [long  ,    8] �ŵ�ü��Ǽ�(�ð�)             StartPos 162, Length 8
    char    mschecnttm          [   8];    char    _mschecnttm          ;    // [long  ,    8] �ż�ü��Ǽ�(�ð�)             StartPos 171, Length 8
    char    totofferrem         [  12];    char    _totofferrem         ;    // [long  ,   12] �ŵ��ܷ�                       StartPos 180, Length 12
    char    totbidrem           [  12];    char    _totbidrem           ;    // [long  ,   12] �ż��ܷ�                       StartPos 193, Length 12
    char    mdvolumetm          [  12];    char    _mdvolumetm          ;    // [long  ,   12] �ð����ŵ�ü�ᷮ               StartPos 206, Length 12
    char    msvolumetm          [  12];    char    _msvolumetm          ;    // [long  ,   12] �ð����ż�ü�ᷮ               StartPos 219, Length 12
} t1302OutBlock1, *LPt1302OutBlock1;
#define NAME_t1302OutBlock1     "t1302OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
