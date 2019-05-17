///////////////////////////////////////////////////////////////////////////////////////////////////
// KOSDAQ�켱ȣ��(KS) ( attr,block )
#pragma pack( push, 1 )

#define NAME_KS_     "KS_"

// �Է�
typedef struct _KS__InBlock
{
    char    shcode              [   6];    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} KS__InBlock, *LPKS__InBlock;
#define NAME_KS__InBlock     "InBlock"

// ���
typedef struct _KS__OutBlock
{
    char    offerho             [   8];    char    _offerho             ;    // [long  ,    8] �ŵ�ȣ��                       StartPos 0, Length 8
    char    bidho               [   8];    char    _bidho               ;    // [long  ,    8] �ż�ȣ��                       StartPos 9, Length 8
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 18, Length 6
} KS__OutBlock, *LPKS__OutBlock;
#define NAME_KS__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
