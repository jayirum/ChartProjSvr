///////////////////////////////////////////////////////////////////////////////////////////////////
// ������ �����ں� �Ÿ���Ȳ(BM) ( attr,block )
#pragma pack( push, 1 )

#define NAME_BM_     "BM_"

// �Է�
typedef struct _BM__InBlock
{
    char    upcode              [   3];    // [string,    3] �����ڵ�                       StartPos 0, Length 3
} BM__InBlock, *LPBM__InBlock;
#define NAME_BM__InBlock     "InBlock"

// ���
typedef struct _BM__OutBlock
{
    char    tjjcode             [   4];    char    _tjjcode             ;    // [string,    4] �������ڵ�                     StartPos 0, Length 4
    char    tjjtime             [   8];    char    _tjjtime             ;    // [string,    8] ���Žð�                       StartPos 5, Length 8
    char    msvolume            [   8];    char    _msvolume            ;    // [long  ,    8] �ż� �ŷ���                    StartPos 14, Length 8
    char    mdvolume            [   8];    char    _mdvolume            ;    // [long  ,    8] �ŵ� �ŷ���                    StartPos 23, Length 8
    char    msvol               [   8];    char    _msvol               ;    // [long  ,    8] �ŷ��� ���ż�                  StartPos 32, Length 8
    char    p_msvol             [   8];    char    _p_msvol             ;    // [long  ,    8] �ŷ��� ���ż� �������         StartPos 41, Length 8
    char    msvalue             [   6];    char    _msvalue             ;    // [long  ,    6] �ż� �ŷ����                  StartPos 50, Length 6
    char    mdvalue             [   6];    char    _mdvalue             ;    // [long  ,    6] �ŵ� �ŷ����                  StartPos 57, Length 6
    char    msval               [   6];    char    _msval               ;    // [long  ,    6] �ŷ���� ���ż�                StartPos 64, Length 6
    char    p_msval             [   6];    char    _p_msval             ;    // [long  ,    6] �ŷ���� ���ż� �������       StartPos 71, Length 6
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 78, Length 3
} BM__OutBlock, *LPBM__OutBlock;
#define NAME_BM__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
