///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ڸŸ�����1(t1615) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1615     "t1615"

// �⺻�Է�
typedef struct _t1615InBlock
{
    char    gubun1              [   1];    char    _gubun1              ;    // [string,    1] �ֽı���                       StartPos 0, Length 1
    char    gubun2              [   1];    char    _gubun2              ;    // [string,    1] �ɼǱ���                       StartPos 2, Length 1
} t1615InBlock, *LPt1615InBlock;
#define NAME_t1615InBlock     "t1615InBlock"

// ���
typedef struct _t1615OutBlock
{
    char    dwvolume            [  12];    char    _dwvolume            ;    // [long  ,   12] ��Ź�ŵ�����                   StartPos 0, Length 12
    char    dwvalue             [  12];    char    _dwvalue             ;    // [long  ,   12] ��Ź�ŵ��ݾ�                   StartPos 13, Length 12
    char    djvolume            [  12];    char    _djvolume            ;    // [long  ,   12] �ڱ�ŵ�����                   StartPos 26, Length 12
    char    djvalue             [  12];    char    _djvalue             ;    // [long  ,   12] �ڱ�ŵ��ݾ�                   StartPos 39, Length 12
    char    sum_volume          [  12];    char    _sum_volume          ;    // [long  ,   12] �հ����                       StartPos 52, Length 12
    char    sum_value           [  12];    char    _sum_value           ;    // [long  ,   12] �հ�ݾ�                       StartPos 65, Length 12
} t1615OutBlock, *LPt1615OutBlock;
#define NAME_t1615OutBlock     "t1615OutBlock"

// ���1, occurs
typedef struct _t1615OutBlock1
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �����                         StartPos 0, Length 20
    char    sv_08               [  12];    char    _sv_08               ;    // [long  ,   12] ����                           StartPos 21, Length 12
    char    sv_17               [  12];    char    _sv_17               ;    // [long  ,   12] �ܱ���                         StartPos 34, Length 12
    char    sv_18               [  12];    char    _sv_18               ;    // [long  ,   12] �����                         StartPos 47, Length 12
    char    sv_07               [  12];    char    _sv_07               ;    // [long  ,   12] ����                           StartPos 60, Length 12
} t1615OutBlock1, *LPt1615OutBlock1;
#define NAME_t1615OutBlock1     "t1615OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
