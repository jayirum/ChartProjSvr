///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ��Ǹ�/��ũ��ȸ(t1105) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1105     "t1105"

// �⺻�Է�
typedef struct _t1105InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} t1105InBlock, *LPt1105InBlock;
#define NAME_t1105InBlock     "t1105InBlock"

// ���
typedef struct _t1105OutBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    pbot                [   8];    char    _pbot                ;    // [long  ,    8] �Ǻ�                           StartPos 7, Length 8
    char    offer1              [   8];    char    _offer1              ;    // [long  ,    8] 1������                        StartPos 16, Length 8
    char    supp1               [   8];    char    _supp1               ;    // [long  ,    8] 1������                        StartPos 25, Length 8
    char    offer2              [   8];    char    _offer2              ;    // [long  ,    8] 2������                        StartPos 34, Length 8
    char    supp2               [   8];    char    _supp2               ;    // [long  ,    8] 2������                        StartPos 43, Length 8
    char    stdprc              [   8];    char    _stdprc              ;    // [long  ,    8] ���ذ���                       StartPos 52, Length 8
    char    offerd              [   8];    char    _offerd              ;    // [long  ,    8] D����                          StartPos 61, Length 8
    char    suppd               [   8];    char    _suppd               ;    // [long  ,    8] D����                          StartPos 70, Length 8
} t1105OutBlock, *LPt1105OutBlock;
#define NAME_t1105OutBlock     "t1105OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
