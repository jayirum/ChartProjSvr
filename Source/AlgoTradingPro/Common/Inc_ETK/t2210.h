///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ɼǽð��뺰ü����ȸ(������¿�) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t2210     "t2210"

// �⺻�Է�
typedef struct _t2210InBlock
{
    char    focode              [   8];    char    _focode              ;    // [string,    8] �����ڵ�                       StartPos 0, Length 8
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] Ư�̰ŷ���                     StartPos 9, Length 12
    char    stime               [   4];    char    _stime               ;    // [string,    4] ���۽ð�                       StartPos 22, Length 4
    char    etime               [   4];    char    _etime               ;    // [string,    4] ����ð�                       StartPos 27, Length 4
} t2210InBlock, *LPt2210InBlock;
#define NAME_t2210InBlock     "t2210InBlock"

// ���
typedef struct _t2210OutBlock
{
    char    mdvolume            [   8];    char    _mdvolume            ;    // [long  ,    8] �ŵ�ü�����                   StartPos 0, Length 8
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�ü��Ǽ�                   StartPos 9, Length 8
    char    msvolume            [   8];    char    _msvolume            ;    // [long  ,    8] �ż�ü�����                   StartPos 18, Length 8
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�ü��Ǽ�                   StartPos 27, Length 8
} t2210OutBlock, *LPt2210OutBlock;
#define NAME_t2210OutBlock     "t2210OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
