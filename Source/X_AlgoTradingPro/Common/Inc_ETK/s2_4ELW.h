///////////////////////////////////////////////////////////////////////////////////////////////////
// ELW�켱ȣ��(s2) ( attr,block )
#pragma pack( push, 1 )

#define NAME_s2_     "s2_"

// �Է�
typedef struct _s2__InBlock
{
    char    shcode              [   6];    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} s2__InBlock, *LPs2__InBlock;
#define NAME_s2__InBlock     "InBlock"

// ���
typedef struct _s2__OutBlock
{
    char    offerho             [   8];    char    _offerho             ;    // [long  ,    8] �ŵ�ȣ��                       StartPos 0, Length 8
    char    bidho               [   8];    char    _bidho               ;    // [long  ,    8] �ż�ȣ��                       StartPos 9, Length 8
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 18, Length 6
} s2__OutBlock, *LPs2__OutBlock;
#define NAME_s2__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
