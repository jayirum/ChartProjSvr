///////////////////////////////////////////////////////////////////////////////////////////////////
// �繫��������(t3341) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t3341     "t3341"

// �⺻�Է�
typedef struct _t3341InBlock
{
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ���屸��                       StartPos 0, Length 1
    char    gubun1              [   1];    char    _gubun1              ;    // [string,    1] ��������(1:�����������2:������ StartPos 2, Length 1
    char    gubun2              [   1];    char    _gubun2              ;    // [string,    1] ��񱸺�                       StartPos 4, Length 1
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 6, Length 4
} t3341InBlock, *LPt3341InBlock;
#define NAME_t3341InBlock     "t3341InBlock"

// ���
typedef struct _t3341OutBlock
{
    char    cnt                 [   4];    char    _cnt                 ;    // [long  ,    4] CNT                            StartPos 0, Length 4
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 5, Length 4
} t3341OutBlock, *LPt3341OutBlock;
#define NAME_t3341OutBlock     "t3341OutBlock"

// ���1, occurs
typedef struct _t3341OutBlock1
{
    char    rank                [   4];    char    _rank                ;    // [long  ,    4] ����                           StartPos 0, Length 4
    char    hname               [  20];    char    _hname               ;    // [string,   20] �����                         StartPos 5, Length 20
    char    salesgrowth         [  12];    char    _salesgrowth         ;    // [long  ,   12] �����������                   StartPos 26, Length 12
    char    operatingincomegrowt[  12];    char    _operatingincomegrowt;    // [long  ,   12] ��������������                 StartPos 39, Length 12
    char    ordinaryincomegrowth[  12];    char    _ordinaryincomegrowth;    // [long  ,   12] �������������                 StartPos 52, Length 12
    char    liabilitytoequity   [  12];    char    _liabilitytoequity   ;    // [long  ,   12] ��ä����                       StartPos 65, Length 12
    char    enterpriseratio     [  12];    char    _enterpriseratio     ;    // [long  ,   12] ������                         StartPos 78, Length 12
    char    eps                 [  12];    char    _eps                 ;    // [long  ,   12] EPS                            StartPos 91, Length 12
    char    bps                 [  12];    char    _bps                 ;    // [long  ,   12] BPS                            StartPos 104, Length 12
    char    roe                 [  12];    char    _roe                 ;    // [long  ,   12] ROE                            StartPos 117, Length 12
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 130, Length 6
    char    per                 [  13];    char    _per                 ;    // [float , 13.2] PER                            StartPos 137, Length 13
    char    pbr                 [  13];    char    _pbr                 ;    // [float , 13.2] PBR                            StartPos 151, Length 13
    char    peg                 [  13];    char    _peg                 ;    // [float , 13.2] PEG                            StartPos 165, Length 13
} t3341OutBlock1, *LPt3341OutBlock1;
#define NAME_t3341OutBlock1     "t3341OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
