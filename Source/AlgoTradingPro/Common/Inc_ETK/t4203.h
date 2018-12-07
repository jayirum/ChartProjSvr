///////////////////////////////////////////////////////////////////////////////////////////////////
// ����íƮ(����)(t4203) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t4203     "t4203"

// �⺻�Է�
typedef struct _t4203InBlock
{
    char    shcode              [   3];    char    _shcode              ;    // [string,    3] �����ڵ�                       StartPos 0, Length 3
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] �ֱⱸ��(0:ƽ1:��2:��3:��4:��) StartPos 4, Length 1
    char    ncnt                [   4];    char    _ncnt                ;    // [long  ,    4] ƽ����                         StartPos 6, Length 4
    char    qrycnt              [   4];    char    _qrycnt              ;    // [long  ,    4] �Ǽ�                           StartPos 11, Length 4
    char    tdgb                [   1];    char    _tdgb                ;    // [string,    1] ���ϱ���(0:��ü1:���ϸ�)       StartPos 16, Length 1
    char    sdate               [   8];    char    _sdate               ;    // [string,    8] ��������                       StartPos 18, Length 8
    char    edate               [   8];    char    _edate               ;    // [string,    8] ��������                       StartPos 27, Length 8
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] ��������                       StartPos 36, Length 8
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] ���ӽð�                       StartPos 45, Length 10
    char    cts_daygb           [   1];    char    _cts_daygb           ;    // [string,    1] ���Ӵ��ϱ���(0:������ü1:���Ӵ� StartPos 56, Length 1
} t4203InBlock, *LPt4203InBlock;
#define NAME_t4203InBlock     "t4203InBlock"

// ���
typedef struct _t4203OutBlock
{
    char    shcode              [   3];    char    _shcode              ;    // [string,    3] �����ڵ�                       StartPos 0, Length 3
    char    jisiga              [   7];    char    _jisiga              ;    // [float ,  7.2] ���Ͻð�                       StartPos 4, Length 7
    char    jihigh              [   7];    char    _jihigh              ;    // [float ,  7.2] ���ϰ�                       StartPos 12, Length 7
    char    jilow               [   7];    char    _jilow               ;    // [float ,  7.2] ��������                       StartPos 20, Length 7
    char    jiclose             [   7];    char    _jiclose             ;    // [float ,  7.2] ��������                       StartPos 28, Length 7
    char    jivolume            [  12];    char    _jivolume            ;    // [long  ,   12] ���ϰŷ���                     StartPos 36, Length 12
    char    disiga              [   7];    char    _disiga              ;    // [float ,  7.2] ���Ͻð�                       StartPos 49, Length 7
    char    dihigh              [   7];    char    _dihigh              ;    // [float ,  7.2] ���ϰ�                       StartPos 57, Length 7
    char    dilow               [   7];    char    _dilow               ;    // [float ,  7.2] ��������                       StartPos 65, Length 7
    char    diclose             [   7];    char    _diclose             ;    // [float ,  7.2] ��������                       StartPos 73, Length 7
    char    disvalue            [  12];    char    _disvalue            ;    // [long  ,   12] ���ϰŷ����                   StartPos 81, Length 12
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] ��������                       StartPos 94, Length 8
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] ���ӽð�                       StartPos 103, Length 10
    char    cts_daygb           [   1];    char    _cts_daygb           ;    // [string,    1] ���Ӵ��ϱ���                   StartPos 114, Length 1
} t4203OutBlock, *LPt4203OutBlock;
#define NAME_t4203OutBlock     "t4203OutBlock"

// ���1, occurs
typedef struct _t4203OutBlock1
{
    char    date                [   8];    char    _date                ;    // [string,    8] ��¥                           StartPos 0, Length 8
    char    time                [   6];    char    _time                ;    // [string,    6] �ð�                           StartPos 9, Length 6
    char    open                [   7];    char    _open                ;    // [float ,  7.2] �ð�                           StartPos 16, Length 7
    char    high                [   7];    char    _high                ;    // [float ,  7.2] ��                           StartPos 24, Length 7
    char    low                 [   7];    char    _low                 ;    // [float ,  7.2] ����                           StartPos 32, Length 7
    char    close               [   7];    char    _close               ;    // [float ,  7.2] ����                           StartPos 40, Length 7
    char    jdiff_vol           [  12];    char    _jdiff_vol           ;    // [long  ,   12] �ŷ���                         StartPos 48, Length 12
    char    value               [  12];    char    _value               ;    // [long  ,   12] �ŷ����                       StartPos 61, Length 12
} t4203OutBlock1, *LPt4203OutBlock1;
#define NAME_t4203OutBlock1     "t4203OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
