///////////////////////////////////////////////////////////////////////////////////////////////////
// �Ⱓ�����α׷��Ÿ�����(t1633) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1633     "t1633"

// �⺻�Է�
typedef struct _t1633InBlock
{
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ���屸��                       StartPos 0, Length 1
    char    gubun1              [   1];    char    _gubun1              ;    // [string,    1] �ݾ׼�������                   StartPos 2, Length 1
    char    gubun2              [   1];    char    _gubun2              ;    // [string,    1] ��ġ��������                   StartPos 4, Length 1
    char    gubun3              [   1];    char    _gubun3              ;    // [string,    1] ���ֿ�����                     StartPos 6, Length 1
    char    fdate               [   8];    char    _fdate               ;    // [string,    8] from����                       StartPos 8, Length 8
    char    tdate               [   8];    char    _tdate               ;    // [string,    8] to����                         StartPos 17, Length 8
    char    gubun4              [   1];    char    _gubun4              ;    // [string,    1] ���������������               StartPos 26, Length 1
    char    date                [   8];    char    _date                ;    // [string,    8] ��¥                           StartPos 28, Length 8
} t1633InBlock, *LPt1633InBlock;
#define NAME_t1633InBlock     "t1633InBlock"

// ���
typedef struct _t1633OutBlock
{
    char    date                [   8];    char    _date                ;    // [string,    8] ��¥                           StartPos 0, Length 8
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 9, Length 4
} t1633OutBlock, *LPt1633OutBlock;
#define NAME_t1633OutBlock     "t1633OutBlock"

// ���1, occurs
typedef struct _t1633OutBlock1
{
    char    date                [   8];    char    _date                ;    // [string,    8] ����                           StartPos 0, Length 8
    char    jisu                [   6];    char    _jisu                ;    // [float ,  6.2] KP200                          StartPos 9, Length 6
    char    sign                [   1];    char    _sign                ;    // [string,    1] ��񱸺�                       StartPos 16, Length 1
    char    change              [   6];    char    _change              ;    // [float ,  6.2] ���                           StartPos 18, Length 6
    char    tot3                [  12];    char    _tot3                ;    // [long  ,   12] ��ü���ż�                     StartPos 25, Length 12
    char    tot1                [  12];    char    _tot1                ;    // [long  ,   12] ��ü�ż�                       StartPos 38, Length 12
    char    tot2                [  12];    char    _tot2                ;    // [long  ,   12] ��ü�ŵ�                       StartPos 51, Length 12
    char    cha3                [  12];    char    _cha3                ;    // [long  ,   12] ���ͼ��ż�                     StartPos 64, Length 12
    char    cha1                [  12];    char    _cha1                ;    // [long  ,   12] ���͸ż�                       StartPos 77, Length 12
    char    cha2                [  12];    char    _cha2                ;    // [long  ,   12] ���͸ŵ�                       StartPos 90, Length 12
    char    bcha3               [  12];    char    _bcha3               ;    // [long  ,   12] �����ͼ��ż�                   StartPos 103, Length 12
    char    bcha1               [  12];    char    _bcha1               ;    // [long  ,   12] �����͸ż�                     StartPos 116, Length 12
    char    bcha2               [  12];    char    _bcha2               ;    // [long  ,   12] �����͸ŵ�                     StartPos 129, Length 12
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 142, Length 12
} t1633OutBlock1, *LPt1633OutBlock1;
#define NAME_t1633OutBlock1     "t1633OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
