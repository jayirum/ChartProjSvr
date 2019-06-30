///////////////////////////////////////////////////////////////////////////////////////////////////
// ELW�ð���ȣ���ܷ�(h2) ( attr,block )
#pragma pack( push, 1 )

#define NAME_h2_     "h2_"

// �Է�
typedef struct _h2__InBlock
{
    char    shcode              [   6];    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} h2__InBlock, *LPh2__InBlock;
#define NAME_h2__InBlock     "InBlock"

// ���
typedef struct _h2__OutBlock
{
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] ȣ���ð�                       StartPos 0, Length 6
    char    tmofferrem          [  12];    char    _tmofferrem          ;    // [long  ,   12] �ð��ܸŵ��ܷ�                 StartPos 7, Length 12
    char    tmbidrem            [  12];    char    _tmbidrem            ;    // [long  ,   12] �ð��ܸż��ܷ�                 StartPos 20, Length 12
    char    pretmoffercha       [  12];    char    _pretmoffercha       ;    // [long  ,   12] �ð��ܸŵ������������         StartPos 33, Length 12
    char    pretmbidcha         [  12];    char    _pretmbidcha         ;    // [long  ,   12] �ð��ܸż������������         StartPos 46, Length 12
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 59, Length 6
} h2__OutBlock, *LPh2__OutBlock;
#define NAME_h2__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
