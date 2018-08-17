///////////////////////////////////////////////////////////////////////////////////////////////////
// KOSPI�ð���ȣ���ܷ�(H2) ( attr,block )
#pragma pack( push, 1 )

#define NAME_H2_     "H2_"

// �Է�
typedef struct _H2__InBlock
{
    char    shcode              [   6];    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} H2__InBlock, *LPH2__InBlock;
#define NAME_H2__InBlock     "InBlock"

// ���
typedef struct _H2__OutBlock
{
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] ȣ���ð�                       StartPos 0, Length 6
    char    tmofferrem          [  12];    char    _tmofferrem          ;    // [long  ,   12] �ð��ܸŵ��ܷ�                 StartPos 7, Length 12
    char    tmbidrem            [  12];    char    _tmbidrem            ;    // [long  ,   12] �ð��ܸż��ܷ�                 StartPos 20, Length 12
    char    pretmoffercha       [  12];    char    _pretmoffercha       ;    // [long  ,   12] �ð��ܸŵ������������         StartPos 33, Length 12
    char    pretmbidcha         [  12];    char    _pretmbidcha         ;    // [long  ,   12] �ð��ܸż������������         StartPos 46, Length 12
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 59, Length 6
} H2__OutBlock, *LPH2__OutBlock;
#define NAME_H2__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
