///////////////////////////////////////////////////////////////////////////////////////////////////
// KOSDAQ����ü��(YK3) ( attr,block )
#pragma pack( push, 1 )

#define NAME_YK3     "YK3"

// �Է�
typedef struct _YK3_InBlock
{
    char    shcode              [   6];    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} YK3_InBlock, *LPYK3_InBlock;
#define NAME_YK3_InBlock     "InBlock"

// ���
typedef struct _YK3_OutBlock
{
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] ȣ���ð�                       StartPos 0, Length 6
    char    yeprice             [   8];    char    _yeprice             ;    // [long  ,    8] ����ü�ᰡ��                   StartPos 7, Length 8
    char    yevolume            [  12];    char    _yevolume            ;    // [long  ,   12] ����ü�����                   StartPos 16, Length 12
    char    jnilysign           [   1];    char    _jnilysign           ;    // [string,    1] ����ü�ᰡ����������񱸺�     StartPos 29, Length 1
    char    preychange          [   8];    char    _preychange          ;    // [long  ,    8] ����ü�ᰡ�����������         StartPos 31, Length 8
    char    jnilydrate          [   6];    char    _jnilydrate          ;    // [float ,  6.2] ����ü�ᰡ�������������       StartPos 40, Length 6
    char    yofferho0           [   8];    char    _yofferho0           ;    // [long  ,    8] ����ŵ�ȣ��                   StartPos 47, Length 8
    char    ybidho0             [   8];    char    _ybidho0             ;    // [long  ,    8] ����ż�ȣ��                   StartPos 56, Length 8
    char    yofferrem0          [  12];    char    _yofferrem0          ;    // [long  ,   12] ����ŵ�ȣ������               StartPos 65, Length 12
    char    ybidrem0            [  12];    char    _ybidrem0            ;    // [long  ,   12] ����ż�ȣ������               StartPos 78, Length 12
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 91, Length 6
} YK3_OutBlock, *LPYK3_OutBlock;
#define NAME_YK3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
