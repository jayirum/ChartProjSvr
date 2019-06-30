///////////////////////////////////////////////////////////////////////////////////////////////////
// �ð��뺰�����ڸŸ����̻�(t1603) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1603     "t1603"

// �⺻�Է�
typedef struct _t1603InBlock
{
    char    market              [   1];    char    _market              ;    // [string,    1] ���屸��                       StartPos 0, Length 1
    char    gubun1              [   1];    char    _gubun1              ;    // [string,    1] �����ڱ���                     StartPos 2, Length 1
    char    gubun2              [   1];    char    _gubun2              ;    // [string,    1] ���Ϻб���                     StartPos 4, Length 1
    char    cts_time            [   8];    char    _cts_time            ;    // [string,    8] CTSTIME                        StartPos 6, Length 8
    char    cts_idx             [   4];    char    _cts_idx             ;    // [long  ,    4] CTSIDX                         StartPos 15, Length 4
    char    cnt                 [   3];    char    _cnt                 ;    // [string,    3] ��ȸ�Ǽ�                       StartPos 20, Length 3
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 24, Length 3
} t1603InBlock, *LPt1603InBlock;
#define NAME_t1603InBlock     "t1603InBlock"

// ���
typedef struct _t1603OutBlock
{
    char    cts_idx             [   4];    char    _cts_idx             ;    // [long  ,    4] CTSIDX                         StartPos 0, Length 4
    char    cts_time            [   8];    char    _cts_time            ;    // [string,    8] CTSTIME                        StartPos 5, Length 8
} t1603OutBlock, *LPt1603OutBlock;
#define NAME_t1603OutBlock     "t1603OutBlock"

// ���1, occurs
typedef struct _t1603OutBlock1
{
    char    time                [   8];    char    _time                ;    // [string,    8] �ð�                           StartPos 0, Length 8
    char    tjjcode             [   4];    char    _tjjcode             ;    // [string,    4] �����ڱ���                     StartPos 9, Length 4
    char    msvolume            [   8];    char    _msvolume            ;    // [long  ,    8] �ż�����                       StartPos 14, Length 8
    char    mdvolume            [   8];    char    _mdvolume            ;    // [long  ,    8] �ŵ�����                       StartPos 23, Length 8
    char    msvalue             [  12];    char    _msvalue             ;    // [long  ,   12] �ż��ݾ�                       StartPos 32, Length 12
    char    mdvalue             [  12];    char    _mdvalue             ;    // [long  ,   12] �ŵ��ݾ�                       StartPos 45, Length 12
    char    svolume             [   8];    char    _svolume             ;    // [long  ,    8] ���ż�����                     StartPos 58, Length 8
    char    svalue              [  12];    char    _svalue              ;    // [long  ,   12] ���ż��ݾ�                     StartPos 67, Length 12
} t1603OutBlock1, *LPt1603OutBlock1;
#define NAME_t1603OutBlock1     "t1603OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
