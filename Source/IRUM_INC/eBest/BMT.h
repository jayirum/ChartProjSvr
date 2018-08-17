///////////////////////////////////////////////////////////////////////////////////////////////////
// �ð��뺰 �����ڸŸ�����(BMT) ( attr,block )
#pragma pack( push, 1 )

#define NAME_BMT     "BMT"

// �Է�
typedef struct _BMT_InBlock
{
    char    upcode              [   3];    // [string,    3] �����ڵ�                       StartPos 0, Length 3
} BMT_InBlock, *LPBMT_InBlock;
#define NAME_BMT_InBlock     "InBlock"

// ���
typedef struct _BMT_OutBlock
{
    char    tjjtime             [   8];    char    _tjjtime             ;    // [string,    8] ���Žð�                       StartPos 0, Length 8
    char    tjjcode1            [   4];    char    _tjjcode1            ;    // [string,    4] �������ڵ�1(����)              StartPos 9, Length 4
    char    msvolume1           [   8];    char    _msvolume1           ;    // [long  ,    8] �ż� �ŷ���1                   StartPos 14, Length 8
    char    mdvolume1           [   8];    char    _mdvolume1           ;    // [long  ,    8] �ŵ� �ŷ���1                   StartPos 23, Length 8
    char    msvol1              [   8];    char    _msvol1              ;    // [long  ,    8] �ŷ��� ���ż�1                 StartPos 32, Length 8
    char    msvalue1            [   6];    char    _msvalue1            ;    // [long  ,    6] �ż� �ŷ����1                 StartPos 41, Length 6
    char    mdvalue1            [   6];    char    _mdvalue1            ;    // [long  ,    6] �ŵ� �ŷ����1                 StartPos 48, Length 6
    char    msval1              [   6];    char    _msval1              ;    // [long  ,    6] �ŷ���� ���ż�1               StartPos 55, Length 6
    char    tjjcode2            [   4];    char    _tjjcode2            ;    // [string,    4] �������ڵ�2(�ܱ���)            StartPos 62, Length 4
    char    msvolume2           [   8];    char    _msvolume2           ;    // [long  ,    8] �ż� �ŷ���2                   StartPos 67, Length 8
    char    mdvolume2           [   8];    char    _mdvolume2           ;    // [long  ,    8] �ŵ� �ŷ���2                   StartPos 76, Length 8
    char    msvol2              [   8];    char    _msvol2              ;    // [long  ,    8] �ŷ��� ���ż�2                 StartPos 85, Length 8
    char    msvalue2            [   6];    char    _msvalue2            ;    // [long  ,    6] �ż� �ŷ����2                 StartPos 94, Length 6
    char    mdvalue2            [   6];    char    _mdvalue2            ;    // [long  ,    6] �ŵ� �ŷ����2                 StartPos 101, Length 6
    char    msval2              [   6];    char    _msval2              ;    // [long  ,    6] �ŷ���� ���ż�2               StartPos 108, Length 6
    char    tjjcode3            [   4];    char    _tjjcode3            ;    // [string,    4] �������ڵ�3(�����)            StartPos 115, Length 4
    char    msvolume3           [   8];    char    _msvolume3           ;    // [long  ,    8] �ż� �ŷ���3                   StartPos 120, Length 8
    char    mdvolume3           [   8];    char    _mdvolume3           ;    // [long  ,    8] �ŵ� �ŷ���3                   StartPos 129, Length 8
    char    msvol3              [   8];    char    _msvol3              ;    // [long  ,    8] �ŷ��� ���ż�3                 StartPos 138, Length 8
    char    msvalue3            [   6];    char    _msvalue3            ;    // [long  ,    6] �ż� �ŷ����3                 StartPos 147, Length 6
    char    mdvalue3            [   6];    char    _mdvalue3            ;    // [long  ,    6] �ŵ� �ŷ����3                 StartPos 154, Length 6
    char    msval3              [   6];    char    _msval3              ;    // [long  ,    6] �ŷ���� ���ż�3               StartPos 161, Length 6
    char    tjjcode4            [   4];    char    _tjjcode4            ;    // [string,    4] �������ڵ�4(����)              StartPos 168, Length 4
    char    msvolume4           [   8];    char    _msvolume4           ;    // [long  ,    8] �ż� �ŷ���4                   StartPos 173, Length 8
    char    mdvolume4           [   8];    char    _mdvolume4           ;    // [long  ,    8] �ŵ� �ŷ���4                   StartPos 182, Length 8
    char    msvol4              [   8];    char    _msvol4              ;    // [long  ,    8] �ŷ��� ���ż�4                 StartPos 191, Length 8
    char    msvalue4            [   6];    char    _msvalue4            ;    // [long  ,    6] �ż� �ŷ����4                 StartPos 200, Length 6
    char    mdvalue4            [   6];    char    _mdvalue4            ;    // [long  ,    6] �ŵ� �ŷ����4                 StartPos 207, Length 6
    char    msval4              [   6];    char    _msval4              ;    // [long  ,    6] �ŷ���� ���ż�4               StartPos 214, Length 6
    char    tjjcode5            [   4];    char    _tjjcode5            ;    // [string,    4] �������ڵ�5(����)              StartPos 221, Length 4
    char    msvolume5           [   8];    char    _msvolume5           ;    // [long  ,    8] �ż� �ŷ���5                   StartPos 226, Length 8
    char    mdvolume5           [   8];    char    _mdvolume5           ;    // [long  ,    8] �ŵ� �ŷ���5                   StartPos 235, Length 8
    char    msvol5              [   8];    char    _msvol5              ;    // [long  ,    8] �ŷ��� ���ż�5                 StartPos 244, Length 8
    char    msvalue5            [   6];    char    _msvalue5            ;    // [long  ,    6] �ż� �ŷ����5                 StartPos 253, Length 6
    char    mdvalue5            [   6];    char    _mdvalue5            ;    // [long  ,    6] �ŵ� �ŷ����5                 StartPos 260, Length 6
    char    msval5              [   6];    char    _msval5              ;    // [long  ,    6] �ŷ���� ���ż�5               StartPos 267, Length 6
    char    tjjcode6            [   4];    char    _tjjcode6            ;    // [string,    4] �������ڵ�6(����)              StartPos 274, Length 4
    char    msvolume6           [   8];    char    _msvolume6           ;    // [long  ,    8] �ż� �ŷ���6                   StartPos 279, Length 8
    char    mdvolume6           [   8];    char    _mdvolume6           ;    // [long  ,    8] �ŵ� �ŷ���6                   StartPos 288, Length 8
    char    msvol6              [   8];    char    _msvol6              ;    // [long  ,    8] �ŷ��� ���ż�6                 StartPos 297, Length 8
    char    msvalue6            [   6];    char    _msvalue6            ;    // [long  ,    6] �ż� �ŷ����6                 StartPos 306, Length 6
    char    mdvalue6            [   6];    char    _mdvalue6            ;    // [long  ,    6] �ŵ� �ŷ����6                 StartPos 313, Length 6
    char    msval6              [   6];    char    _msval6              ;    // [long  ,    6] �ŷ���� ���ż�6               StartPos 320, Length 6
    char    tjjcode7            [   4];    char    _tjjcode7            ;    // [string,    4] �������ڵ�7(����)              StartPos 327, Length 4
    char    msvolume7           [   8];    char    _msvolume7           ;    // [long  ,    8] �ż� �ŷ���7                   StartPos 332, Length 8
    char    mdvolume7           [   8];    char    _mdvolume7           ;    // [long  ,    8] �ŵ� �ŷ���7                   StartPos 341, Length 8
    char    msvol7              [   8];    char    _msvol7              ;    // [long  ,    8] �ŷ��� ���ż�7                 StartPos 350, Length 8
    char    msvalue7            [   6];    char    _msvalue7            ;    // [long  ,    6] �ż� �ŷ����7                 StartPos 359, Length 6
    char    mdvalue7            [   6];    char    _mdvalue7            ;    // [long  ,    6] �ŵ� �ŷ����7                 StartPos 366, Length 6
    char    msval7              [   6];    char    _msval7              ;    // [long  ,    6] �ŷ���� ���ż�7               StartPos 373, Length 6
    char    tjjcode8            [   4];    char    _tjjcode8            ;    // [string,    4] �������ڵ�8(����)              StartPos 380, Length 4
    char    msvolume8           [   8];    char    _msvolume8           ;    // [long  ,    8] �ż� �ŷ���8                   StartPos 385, Length 8
    char    mdvolume8           [   8];    char    _mdvolume8           ;    // [long  ,    8] �ŵ� �ŷ���8                   StartPos 394, Length 8
    char    msvol8              [   8];    char    _msvol8              ;    // [long  ,    8] �ŷ��� ���ż�8                 StartPos 403, Length 8
    char    msvalue8            [   6];    char    _msvalue8            ;    // [long  ,    6] �ż� �ŷ����8                 StartPos 412, Length 6
    char    mdvalue8            [   6];    char    _mdvalue8            ;    // [long  ,    6] �ŵ� �ŷ����8                 StartPos 419, Length 6
    char    msval8              [   6];    char    _msval8              ;    // [long  ,    6] �ŷ���� ���ż�8               StartPos 426, Length 6
    char    tjjcode9            [   4];    char    _tjjcode9            ;    // [string,    4] �������ڵ�9(���)              StartPos 433, Length 4
    char    msvolume9           [   8];    char    _msvolume9           ;    // [long  ,    8] �ż� �ŷ���9                   StartPos 438, Length 8
    char    mdvolume9           [   8];    char    _mdvolume9           ;    // [long  ,    8] �ŵ� �ŷ���9                   StartPos 447, Length 8
    char    msvol9              [   8];    char    _msvol9              ;    // [long  ,    8] �ŷ��� ���ż�9                 StartPos 456, Length 8
    char    msvalue9            [   6];    char    _msvalue9            ;    // [long  ,    6] �ż� �ŷ����9                 StartPos 465, Length 6
    char    mdvalue9            [   6];    char    _mdvalue9            ;    // [long  ,    6] �ŵ� �ŷ����9                 StartPos 472, Length 6
    char    msval9              [   6];    char    _msval9              ;    // [long  ,    6] �ŷ���� ���ż�9               StartPos 479, Length 6
    char    tjjcode10           [   4];    char    _tjjcode10           ;    // [string,    4] �������ڵ�10(��������)         StartPos 486, Length 4
    char    msvolume10          [   8];    char    _msvolume10          ;    // [long  ,    8] �ż� �ŷ���10                  StartPos 491, Length 8
    char    mdvolume10          [   8];    char    _mdvolume10          ;    // [long  ,    8] �ŵ� �ŷ���10                  StartPos 500, Length 8
    char    msvol10             [   8];    char    _msvol10             ;    // [long  ,    8] �ŷ��� ���ż�10                StartPos 509, Length 8
    char    msvalue10           [   6];    char    _msvalue10           ;    // [long  ,    6] �ż� �ŷ����10                StartPos 518, Length 6
    char    mdvalue10           [   6];    char    _mdvalue10           ;    // [long  ,    6] �ŵ� �ŷ����10                StartPos 525, Length 6
    char    msval10             [   6];    char    _msval10             ;    // [long  ,    6] �ŷ���� ���ż�10              StartPos 532, Length 6
    char    tjjcode11           [   4];    char    _tjjcode11           ;    // [string,    4] �������ڵ�11(��Ÿ)             StartPos 539, Length 4
    char    msvolume11          [   8];    char    _msvolume11          ;    // [long  ,    8] �ż� �ŷ���11                  StartPos 544, Length 8
    char    mdvolume11          [   8];    char    _mdvolume11          ;    // [long  ,    8] �ŵ� �ŷ���11                  StartPos 553, Length 8
    char    msvol11             [   8];    char    _msvol11             ;    // [long  ,    8] �ŷ��� ���ż�11                StartPos 562, Length 8
    char    msvalue11           [   6];    char    _msvalue11           ;    // [long  ,    6] �ż� �ŷ����11                StartPos 571, Length 6
    char    mdvalue11           [   6];    char    _mdvalue11           ;    // [long  ,    6] �ŵ� �ŷ����11                StartPos 578, Length 6
    char    msval11             [   6];    char    _msval11             ;    // [long  ,    6] �ŷ���� ���ż�11              StartPos 585, Length 6
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 592, Length 3
    char    tjjcode0            [   4];    char    _tjjcode0            ;    // [string,    4] �������ڵ�0(����ݵ�)          StartPos 596, Length 4
    char    msvolume0           [   8];    char    _msvolume0           ;    // [long  ,    8] �ż� �ŷ���0                   StartPos 601, Length 8
    char    mdvolume0           [   8];    char    _mdvolume0           ;    // [long  ,    8] �ŵ� �ŷ���0                   StartPos 610, Length 8
    char    msvol0              [   8];    char    _msvol0              ;    // [long  ,    8] �ŷ��� ���ż�0                 StartPos 619, Length 8
    char    msvalue0            [   6];    char    _msvalue0            ;    // [long  ,    6] �ż� �ŷ����0                 StartPos 628, Length 6
    char    mdvalue0            [   6];    char    _mdvalue0            ;    // [long  ,    6] �ŵ� �ŷ����0                 StartPos 635, Length 6
    char    msval0              [   6];    char    _msval0              ;    // [long  ,    6] �ŷ���� ���ż�0               StartPos 642, Length 6
} BMT_OutBlock, *LPBMT_OutBlock;
#define NAME_BMT_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
