///////////////////////////////////////////////////////////////////////////////////////////////////
// �������ȸ(t9907) ( block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t9907     "t9907"

// �⺻�Է�
typedef struct _t9907InBlock
{
    char    dummy               [   1];    // [string,    1] DUMMY                          StartPos 0, Length 1
} t9907InBlock, *LPt9907InBlock;
#define NAME_t9907InBlock     "t9907InBlock"

// ���1, occurs
typedef struct _t9907OutBlock1
{
    char    lastym              [   6];    // [string,    6] �����                         StartPos 0, Length 6
    char    lastnm              [  10];    // [string,   10] �������                       StartPos 6, Length 10
} t9907OutBlock1, *LPt9907OutBlock1;
#define NAME_t9907OutBlock1     "t9907OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
