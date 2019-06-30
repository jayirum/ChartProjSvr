///////////////////////////////////////////////////////////////////////////////////////////////////
// ���α�����񺰵���(t1701) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1701     "t1701"

// �⺻�Է�
typedef struct _t1701InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ����                           StartPos 7, Length 1
    char    fromdt              [   8];    char    _fromdt              ;    // [string,    8] ��������                       StartPos 9, Length 8
    char    todt                [   8];    char    _todt                ;    // [string,    8] ��������                       StartPos 18, Length 8
    char    prapp               [   3];    char    _prapp               ;    // [long  ,    3] PR����                         StartPos 27, Length 3
    char    prgubun             [   1];    char    _prgubun             ;    // [string,    1] PR���뱸��                     StartPos 31, Length 1
    char    orggubun            [   1];    char    _orggubun            ;    // [string,    1] �������                       StartPos 33, Length 1
    char    frggubun            [   1];    char    _frggubun            ;    // [string,    1] ��������                       StartPos 35, Length 1
} t1701InBlock, *LPt1701InBlock;
#define NAME_t1701InBlock     "t1701InBlock"

// ���, occurs
typedef struct _t1701OutBlock
{
    char    date                [   8];    char    _date                ;    // [string,    8] ����                           StartPos 0, Length 8
    char    close               [   8];    char    _close               ;    // [long  ,    8] ����                           StartPos 9, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 18, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 20, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 29, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 36, Length 12
    char    psnvolume           [  12];    char    _psnvolume           ;    // [long  ,   12] ����                           StartPos 49, Length 12
    char    orgvolume           [  12];    char    _orgvolume           ;    // [long  ,   12] ���                           StartPos 62, Length 12
    char    frgvolume           [  12];    char    _frgvolume           ;    // [long  ,   12] �ܱ���                         StartPos 75, Length 12
    char    frgvolumesum        [  12];    char    _frgvolumesum        ;    // [long  ,   12] �ܱ���                         StartPos 88, Length 12
    char    pgmvolume           [  12];    char    _pgmvolume           ;    // [long  ,   12] ���α׷�                       StartPos 101, Length 12
    char    listing             [  12];    char    _listing             ;    // [long  ,   12] �����ֽļ�                     StartPos 114, Length 12
    char    listupdn            [  12];    char    _listupdn            ;    // [long  ,   12] ��������                       StartPos 127, Length 12
    char    sjrate              [   6];    char    _sjrate              ;    // [float ,  6.2] ������                         StartPos 140, Length 6
} t1701OutBlock, *LPt1701OutBlock;
#define NAME_t1701OutBlock     "t1701OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
