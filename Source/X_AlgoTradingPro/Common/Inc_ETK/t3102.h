///////////////////////////////////////////////////////////////////////////////////////////////////
// ��������(t3102) ( block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t3102     "t3102"

// �⺻�Է�
typedef struct _t3102InBlock
{
    char    sNewsno             [  24];    // [string,   24] ������ȣ                       StartPos 0, Length 24
} t3102InBlock, *LPt3102InBlock;
#define NAME_t3102InBlock     "t3102InBlock"

// �⺻���, occurs
typedef struct _t3102OutBlock
{
    char    sJongcode           [   6];    // [string,    6] ��������                       StartPos 0, Length 6
} t3102OutBlock, *LPt3102OutBlock;
#define NAME_t3102OutBlock     "t3102OutBlock"

// �⺻���1, occurs
typedef struct _t3102OutBlock1
{
    char    sBody               [ 100];    // [string,  100] ��������                       StartPos 0, Length 100
} t3102OutBlock1, *LPt3102OutBlock1;
#define NAME_t3102OutBlock1     "t3102OutBlock1"

// �⺻���2
typedef struct _t3102OutBlock2
{
    char    sTitle              [ 300];    // [string,  300] ����Ÿ��Ʋ                     StartPos 0, Length 300
} t3102OutBlock2, *LPt3102OutBlock2;
#define NAME_t3102OutBlock2     "t3102OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
