///////////////////////////////////////////////////////////////////////////////////////////////////
// �ð��뺰�����ڸŸ�����(t1602) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1602     "t1602"

// �⺻�Է�
typedef struct _t1602InBlock
{
    char    market              [   1];    char    _market              ;    // [string,    1] ���屸��                       StartPos 0, Length 1
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 2, Length 3
    char    gubun1              [   1];    char    _gubun1              ;    // [string,    1] ��������                       StartPos 6, Length 1
    char    gubun2              [   1];    char    _gubun2              ;    // [string,    1] ���Ϻб���                     StartPos 8, Length 1
    char    cts_time            [   8];    char    _cts_time            ;    // [string,    8] CTSTIME                        StartPos 10, Length 8
    char    cts_idx             [   4];    char    _cts_idx             ;    // [long  ,    4] CTSIDX                         StartPos 19, Length 4
    char    cnt                 [   4];    char    _cnt                 ;    // [string,    4] ��ȸ�Ǽ�                       StartPos 24, Length 4
} t1602InBlock, *LPt1602InBlock;
#define NAME_t1602InBlock     "t1602InBlock"

// �⺻���
typedef struct _t1602OutBlock
{
    char    cts_time            [   8];    char    _cts_time            ;    // [string,    8] CTSTIME                        StartPos 0, Length 8
    char    tjjcode_08          [   4];    char    _tjjcode_08          ;    // [string,    4] �����������ڵ�                 StartPos 9, Length 4
    char    ms_08               [  12];    char    _ms_08               ;    // [long  ,   12] ���θż�                       StartPos 14, Length 12
    char    md_08               [  12];    char    _md_08               ;    // [long  ,   12] ���θŵ�                       StartPos 27, Length 12
    char    rate_08             [  12];    char    _rate_08             ;    // [long  ,   12] ��������                       StartPos 40, Length 12
    char    svolume_08          [  12];    char    _svolume_08          ;    // [long  ,   12] ���μ��ż�                     StartPos 53, Length 12
    char    tjjcode_17          [   4];    char    _tjjcode_17          ;    // [string,    4] �ܱ����������ڵ�               StartPos 66, Length 4
    char    ms_17               [  12];    char    _ms_17               ;    // [long  ,   12] �ܱ��θż�                     StartPos 71, Length 12
    char    md_17               [  12];    char    _md_17               ;    // [long  ,   12] �ܱ��θŵ�                     StartPos 84, Length 12
    char    rate_17             [  12];    char    _rate_17             ;    // [long  ,   12] �ܱ�������                     StartPos 97, Length 12
    char    svolume_17          [  12];    char    _svolume_17          ;    // [long  ,   12] �ܱ��μ��ż�                   StartPos 110, Length 12
    char    tjjcode_18          [   4];    char    _tjjcode_18          ;    // [string,    4] ������������ڵ�               StartPos 123, Length 4
    char    ms_18               [  12];    char    _ms_18               ;    // [long  ,   12] �����ż�                     StartPos 128, Length 12
    char    md_18               [  12];    char    _md_18               ;    // [long  ,   12] �����ŵ�                     StartPos 141, Length 12
    char    rate_18             [  12];    char    _rate_18             ;    // [long  ,   12] ���������                     StartPos 154, Length 12
    char    svolume_18          [  12];    char    _svolume_18          ;    // [long  ,   12] �������ż�                   StartPos 167, Length 12
    char    tjjcode_01          [   4];    char    _tjjcode_01          ;    // [string,    4] �����������ڵ�                 StartPos 180, Length 4
    char    ms_01               [  12];    char    _ms_01               ;    // [long  ,   12] ���Ǹż�                       StartPos 185, Length 12
    char    md_01               [  12];    char    _md_01               ;    // [long  ,   12] ���Ǹŵ�                       StartPos 198, Length 12
    char    rate_01             [  12];    char    _rate_01             ;    // [long  ,   12] ��������                       StartPos 211, Length 12
    char    svolume_01          [  12];    char    _svolume_01          ;    // [long  ,   12] ���Ǽ��ż�                     StartPos 224, Length 12
    char    tjjcode_03          [   4];    char    _tjjcode_03          ;    // [string,    4] �����������ڵ�                 StartPos 237, Length 4
    char    ms_03               [  12];    char    _ms_03               ;    // [long  ,   12] ���Ÿż�                       StartPos 242, Length 12
    char    md_03               [  12];    char    _md_03               ;    // [long  ,   12] ���Ÿŵ�                       StartPos 255, Length 12
    char    rate_03             [  12];    char    _rate_03             ;    // [long  ,   12] ��������                       StartPos 268, Length 12
    char    svolume_03          [  12];    char    _svolume_03          ;    // [long  ,   12] ���ż��ż�                     StartPos 281, Length 12
    char    tjjcode_04          [   4];    char    _tjjcode_04          ;    // [string,    4] �����������ڵ�                 StartPos 294, Length 4
    char    ms_04               [  12];    char    _ms_04               ;    // [long  ,   12] ����ż�                       StartPos 299, Length 12
    char    md_04               [  12];    char    _md_04               ;    // [long  ,   12] ����ŵ�                       StartPos 312, Length 12
    char    rate_04             [  12];    char    _rate_04             ;    // [long  ,   12] ��������                       StartPos 325, Length 12
    char    svolume_04          [  12];    char    _svolume_04          ;    // [long  ,   12] ������ż�                     StartPos 338, Length 12
    char    tjjcode_02          [   4];    char    _tjjcode_02          ;    // [string,    4] �����������ڵ�                 StartPos 351, Length 4
    char    ms_02               [  12];    char    _ms_02               ;    // [long  ,   12] ����ż�                       StartPos 356, Length 12
    char    md_02               [  12];    char    _md_02               ;    // [long  ,   12] ����ŵ�                       StartPos 369, Length 12
    char    rate_02             [  12];    char    _rate_02             ;    // [long  ,   12] ��������                       StartPos 382, Length 12
    char    svolume_02          [  12];    char    _svolume_02          ;    // [long  ,   12] ������ż�                     StartPos 395, Length 12
    char    tjjcode_05          [   4];    char    _tjjcode_05          ;    // [string,    4] �����������ڵ�                 StartPos 408, Length 4
    char    ms_05               [  12];    char    _ms_05               ;    // [long  ,   12] ���ݸż�                       StartPos 413, Length 12
    char    md_05               [  12];    char    _md_05               ;    // [long  ,   12] ���ݸŵ�                       StartPos 426, Length 12
    char    rate_05             [  12];    char    _rate_05             ;    // [long  ,   12] ��������                       StartPos 439, Length 12
    char    svolume_05          [  12];    char    _svolume_05          ;    // [long  ,   12] ���ݼ��ż�                     StartPos 452, Length 12
    char    tjjcode_06          [   4];    char    _tjjcode_06          ;    // [string,    4] ����������ڵ�                 StartPos 465, Length 4
    char    ms_06               [  12];    char    _ms_06               ;    // [long  ,   12] ��ݸż�                       StartPos 470, Length 12
    char    md_06               [  12];    char    _md_06               ;    // [long  ,   12] ��ݸŵ�                       StartPos 483, Length 12
    char    rate_06             [  12];    char    _rate_06             ;    // [long  ,   12] �������                       StartPos 496, Length 12
    char    svolume_06          [  12];    char    _svolume_06          ;    // [long  ,   12] ��ݼ��ż�                     StartPos 509, Length 12
    char    tjjcode_07          [   4];    char    _tjjcode_07          ;    // [string,    4] ��Ÿ�������ڵ�                 StartPos 522, Length 4
    char    ms_07               [  12];    char    _ms_07               ;    // [long  ,   12] ��Ÿ�ż�                       StartPos 527, Length 12
    char    md_07               [  12];    char    _md_07               ;    // [long  ,   12] ��Ÿ�ŵ�                       StartPos 540, Length 12
    char    rate_07             [  12];    char    _rate_07             ;    // [long  ,   12] ��Ÿ����                       StartPos 553, Length 12
    char    svolume_07          [  12];    char    _svolume_07          ;    // [long  ,   12] ��Ÿ���ż�                     StartPos 566, Length 12
    char    tjjcode_11          [   4];    char    _tjjcode_11          ;    // [string,    4] �����������ڵ�                 StartPos 579, Length 4
    char    ms_11               [  12];    char    _ms_11               ;    // [long  ,   12] �����ż�                       StartPos 584, Length 12
    char    md_11               [  12];    char    _md_11               ;    // [long  ,   12] �����ŵ�                       StartPos 597, Length 12
    char    rate_11             [  12];    char    _rate_11             ;    // [long  ,   12] ��������                       StartPos 610, Length 12
    char    svolume_11          [  12];    char    _svolume_11          ;    // [long  ,   12] �������ż�                     StartPos 623, Length 12
    char    tjjcode_00          [   4];    char    _tjjcode_00          ;    // [string,    4] ����ݵ��ڵ�                   StartPos 636, Length 4
    char    ms_00               [  12];    char    _ms_00               ;    // [long  ,   12] ����ݵ�ż�                   StartPos 641, Length 12
    char    md_00               [  12];    char    _md_00               ;    // [long  ,   12] ����ݵ�ŵ�                   StartPos 654, Length 12
    char    rate_00             [  12];    char    _rate_00             ;    // [long  ,   12] ����ݵ�����                   StartPos 667, Length 12
    char    svolume_00          [  12];    char    _svolume_00          ;    // [long  ,   12] ����ݵ���ż�                 StartPos 680, Length 12
} t1602OutBlock, *LPt1602OutBlock;
#define NAME_t1602OutBlock     "t1602OutBlock"

// ���1, occurs
typedef struct _t1602OutBlock1
{
    char    time                [   8];    char    _time                ;    // [string,    8] �ð�                           StartPos 0, Length 8
    char    sv_08               [  12];    char    _sv_08               ;    // [long  ,   12] ���μ��ż�                     StartPos 9, Length 12
    char    sv_17               [  12];    char    _sv_17               ;    // [long  ,   12] �ܱ��μ��ż�                   StartPos 22, Length 12
    char    sv_18               [  12];    char    _sv_18               ;    // [long  ,   12] �������ż�                   StartPos 35, Length 12
    char    sv_01               [  12];    char    _sv_01               ;    // [long  ,   12] ���Ǽ��ż�                     StartPos 48, Length 12
    char    sv_03               [  12];    char    _sv_03               ;    // [long  ,   12] ���ż��ż�                     StartPos 61, Length 12
    char    sv_04               [  12];    char    _sv_04               ;    // [long  ,   12] ������ż�                     StartPos 74, Length 12
    char    sv_02               [  12];    char    _sv_02               ;    // [long  ,   12] ������ż�                     StartPos 87, Length 12
    char    sv_05               [  12];    char    _sv_05               ;    // [long  ,   12] ���ݼ��ż�                     StartPos 100, Length 12
    char    sv_06               [  12];    char    _sv_06               ;    // [long  ,   12] ��ݼ��ż�                     StartPos 113, Length 12
    char    sv_07               [  12];    char    _sv_07               ;    // [long  ,   12] ��Ÿ���ż�                     StartPos 126, Length 12
    char    sv_11               [  12];    char    _sv_11               ;    // [long  ,   12] �������ż�                     StartPos 139, Length 12
    char    sv_00               [  12];    char    _sv_00               ;    // [long  ,   12] ����ݵ���ż�                 StartPos 152, Length 12
} t1602OutBlock1, *LPt1602OutBlock1;
#define NAME_t1602OutBlock1     "t1602OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
