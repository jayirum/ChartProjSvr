///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ڸŸ�����2(t1617) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1617     "t1617"

// �⺻�Է�
typedef struct _t1617InBlock
{
    char    gubun1              [   1];    char    _gubun1              ;    // [string,    1] ���屸��(1:�ڽ���2:�ڽ���3:���� StartPos 0, Length 1
    char    gubun2              [   1];    char    _gubun2              ;    // [string,    1] �����ݾױ���(1:����2:�ݾ�)     StartPos 2, Length 1
    char    gubun3              [   1];    char    _gubun3              ;    // [string,    1] ���ڱ���(1:�ð��뺰2:�Ϻ�)     StartPos 4, Length 1
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] CTSDATE(����Ű��-����)         StartPos 6, Length 8
    char    cts_time            [   8];    char    _cts_time            ;    // [string,    8] CTSTIME(����Ű��-�ð�)         StartPos 15, Length 8
} t1617InBlock, *LPt1617InBlock;
#define NAME_t1617InBlock     "t1617InBlock"

// �⺻���
typedef struct _t1617OutBlock
{
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] CTSDATE                        StartPos 0, Length 8
    char    cts_time            [   8];    char    _cts_time            ;    // [string,    8] CTSTIME                        StartPos 9, Length 8
    char    ms_08               [  12];    char    _ms_08               ;    // [long  ,   12] ���θż�                       StartPos 18, Length 12
    char    md_08               [  12];    char    _md_08               ;    // [long  ,   12] ���θŵ�                       StartPos 31, Length 12
    char    sv_08               [  12];    char    _sv_08               ;    // [long  ,   12] ���μ��ż�                     StartPos 44, Length 12
    char    ms_17               [  12];    char    _ms_17               ;    // [long  ,   12] �ܱ��θż�                     StartPos 57, Length 12
    char    md_17               [  12];    char    _md_17               ;    // [long  ,   12] �ܱ��θŵ�                     StartPos 70, Length 12
    char    sv_17               [  12];    char    _sv_17               ;    // [long  ,   12] �ܱ��μ��ż�                   StartPos 83, Length 12
    char    ms_18               [  12];    char    _ms_18               ;    // [long  ,   12] �����ż�                     StartPos 96, Length 12
    char    md_18               [  12];    char    _md_18               ;    // [long  ,   12] �����ŵ�                     StartPos 109, Length 12
    char    sv_18               [  12];    char    _sv_18               ;    // [long  ,   12] �������ż�                   StartPos 122, Length 12
    char    ms_01               [  12];    char    _ms_01               ;    // [long  ,   12] ���Ǹż�                       StartPos 135, Length 12
    char    md_01               [  12];    char    _md_01               ;    // [long  ,   12] ���Ǹŵ�                       StartPos 148, Length 12
    char    sv_01               [  12];    char    _sv_01               ;    // [long  ,   12] ���Ǽ��ż�                     StartPos 161, Length 12
} t1617OutBlock, *LPt1617OutBlock;
#define NAME_t1617OutBlock     "t1617OutBlock"

// ���1, occurs
typedef struct _t1617OutBlock1
{
    char    date                [   8];    char    _date                ;    // [string,    8] ��¥                           StartPos 0, Length 8
    char    time                [   8];    char    _time                ;    // [string,    8] �ð�                           StartPos 9, Length 8
    char    sv_08               [  12];    char    _sv_08               ;    // [long  ,   12] ����                           StartPos 18, Length 12
    char    sv_17               [  12];    char    _sv_17               ;    // [long  ,   12] �ܱ���                         StartPos 31, Length 12
    char    sv_18               [  12];    char    _sv_18               ;    // [long  ,   12] �����                         StartPos 44, Length 12
    char    sv_01               [  12];    char    _sv_01               ;    // [long  ,   12] ����                           StartPos 57, Length 12
} t1617OutBlock1, *LPt1617OutBlock1;
#define NAME_t1617OutBlock1     "t1617OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
