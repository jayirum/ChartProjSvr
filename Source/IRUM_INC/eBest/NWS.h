///////////////////////////////////////////////////////////////////////////////////////////////////
// �ǽð�����������Ŷ ( block )
#pragma pack( push, 1 )

#define NAME_NWS     "NWS"

// �Է�
typedef struct _NWS_InBlock
{
    char    nwcode              [   6];    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} NWS_InBlock, *LPNWS_InBlock;
#define NAME_NWS_InBlock     "InBlock"

// ���
typedef struct _NWS_OutBlock
{
    char    date                [   8];    // [string,    8] ��¥                           StartPos 0, Length 8
    char    time                [   6];    // [string,    6] �ð�                           StartPos 8, Length 6
    char    id                  [   2];    // [string,    2] ����������                     StartPos 14, Length 2
    char    realkey             [  24];    // [string,   24] Ű��                           StartPos 16, Length 24
    char    title               [ 300];    // [string,  300] ����                           StartPos 40, Length 300
    char    code                [ 240];    // [string,  240] ���������ڵ�                   StartPos 340, Length 240
    char    bodysize            [   8];    // [long  ,    8] BODY����                       StartPos 580, Length 8
} NWS_OutBlock, *LPNWS_OutBlock;
#define NAME_NWS_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
