///////////////////////////////////////////////////////////////////////////////////////////////////
// ����/�ɼ����簡�ü��޸�(t2106) ( block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t2106     "t2106"

// �⺻�Է�
typedef struct _t2106InBlock
{
    char    code                [   8];    // [string,    8] �����ڵ�                       StartPos 0, Length 8
    char    nrec                [   2];    // [string,    2] �Ǽ�                           StartPos 8, Length 2
} t2106InBlock, *LPt2106InBlock;
#define NAME_t2106InBlock     "t2106InBlock"

// �⺻�Է�1, occurs
typedef struct _t2106InBlock1
{
    char    indx                [   1];    // [string,    1] �ε���                         StartPos 0, Length 1
    char    gubn                [   1];    // [string,    1] ���Ǳ���                       StartPos 1, Length 1
    char    dat1                [   1];    // [string,    1] ����Ÿ1                        StartPos 2, Length 1
    char    dat2                [   8];    // [string,    8] ����Ÿ2                        StartPos 3, Length 8
} t2106InBlock1, *LPt2106InBlock1;
#define NAME_t2106InBlock1     "t2106InBlock1"

// ���
typedef struct _t2106OutBlock
{
    char    nrec                [   2];    // [string,    2] ��°Ǽ�                       StartPos 0, Length 2
} t2106OutBlock, *LPt2106OutBlock;
#define NAME_t2106OutBlock     "t2106OutBlock"

// ���1, occurs
typedef struct _t2106OutBlock1
{
    char    indx                [   1];    // [string,    1] �ε���                         StartPos 0, Length 1
    char    gubn                [   1];    // [string,    1] ���Ǳ���                       StartPos 1, Length 1
    char    vals                [   8];    // [string,    8] ��°�                         StartPos 2, Length 8
} t2106OutBlock1, *LPt2106OutBlock1;
#define NAME_t2106OutBlock1     "t2106OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
