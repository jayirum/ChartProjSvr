///////////////////////////////////////////////////////////////////////////////////////////////////
// �������к������ڸŸŵ���(íƮ��) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1621     "t1621"

// �⺻�Է�
typedef struct _t1621InBlock
{
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 0, Length 3
    char    nmin                [   2];    char    _nmin                ;    // [string,    2] N��                            StartPos 4, Length 2
    char    cnt                 [   3];    char    _cnt                 ;    // [string,    3] ��ȸ�Ǽ�                       StartPos 7, Length 3
    char    bgubun              [   1];    char    _bgubun              ;    // [string,    1] ���Ϻ�                         StartPos 11, Length 1
} t1621InBlock, *LPt1621InBlock;
#define NAME_t1621InBlock     "t1621InBlock"

// �⺻���
typedef struct _t1621OutBlock
{
    char    indcode             [   4];    char    _indcode             ;    // [string,    4] �����������ڵ�                 StartPos 0, Length 4
    char    forcode             [   4];    char    _forcode             ;    // [string,    4] �ܱ����������ڵ�               StartPos 5, Length 4
    char    syscode             [   4];    char    _syscode             ;    // [string,    4] ������������ڵ�               StartPos 10, Length 4
    char    stocode             [   4];    char    _stocode             ;    // [string,    4] �����������ڵ�                 StartPos 15, Length 4
    char    invcode             [   4];    char    _invcode             ;    // [string,    4] �����������ڵ�                 StartPos 20, Length 4
    char    bancode             [   4];    char    _bancode             ;    // [string,    4] �����������ڵ�                 StartPos 25, Length 4
    char    inscode             [   4];    char    _inscode             ;    // [string,    4] �����������ڵ�                 StartPos 30, Length 4
    char    fincode             [   4];    char    _fincode             ;    // [string,    4] �����������ڵ�                 StartPos 35, Length 4
    char    moncode             [   4];    char    _moncode             ;    // [string,    4] ����������ڵ�                 StartPos 40, Length 4
    char    etccode             [   4];    char    _etccode             ;    // [string,    4] ��Ÿ�������ڵ�                 StartPos 45, Length 4
    char    natcode             [   4];    char    _natcode             ;    // [string,    4] �����������ڵ�                 StartPos 50, Length 4
    char    pefcode             [   4];    char    _pefcode             ;    // [string,    4] ����ݵ��������ڵ�             StartPos 55, Length 4
    char    jisucd              [   8];    char    _jisucd              ;    // [string,    8] ���������ڵ�                   StartPos 60, Length 8
    char    jisunm              [  20];    char    _jisunm              ;    // [string,   20] ����������                     StartPos 69, Length 20
} t1621OutBlock, *LPt1621OutBlock;
#define NAME_t1621OutBlock     "t1621OutBlock"

// ���1, occurs
typedef struct _t1621OutBlock1
{
    char    date                [   8];    char    _date                ;    // [string,    8] ����                           StartPos 0, Length 8
    char    time                [   6];    char    _time                ;    // [string,    6] �ð�                           StartPos 9, Length 6
    char    datetime            [  14];    char    _datetime            ;    // [string,   14] ���ڽð�                       StartPos 16, Length 14
    char    indmsvol            [   8];    char    _indmsvol            ;    // [long  ,    8] ���μ��ż��ŷ���               StartPos 31, Length 8
    char    indmsamt            [  12];    char    _indmsamt            ;    // [double,   12] ���μ��ż��ŷ����             StartPos 40, Length 12
    char    formsvol            [   8];    char    _formsvol            ;    // [long  ,    8] �ܱ��μ��ż��ŷ���             StartPos 53, Length 8
    char    formsamt            [  12];    char    _formsamt            ;    // [double,   12] �ܱ��μ��ż��ŷ����           StartPos 62, Length 12
    char    sysmsvol            [   8];    char    _sysmsvol            ;    // [long  ,    8] �������ż��ŷ���             StartPos 75, Length 8
    char    sysmsamt            [  12];    char    _sysmsamt            ;    // [double,   12] �������ż��ŷ����           StartPos 84, Length 12
    char    stomsvol            [   8];    char    _stomsvol            ;    // [long  ,    8] ���Ǽ��ż��ŷ���               StartPos 97, Length 8
    char    stomsamt            [  12];    char    _stomsamt            ;    // [double,   12] ���Ǽ��ż��ŷ����             StartPos 106, Length 12
    char    invmsvol            [   8];    char    _invmsvol            ;    // [long  ,    8] ���ż��ż��ŷ���               StartPos 119, Length 8
    char    invmsamt            [  12];    char    _invmsamt            ;    // [double,   12] ���ż��ż��ŷ����             StartPos 128, Length 12
    char    banmsvol            [   8];    char    _banmsvol            ;    // [long  ,    8] ������ż��ŷ���               StartPos 141, Length 8
    char    banmsamt            [  12];    char    _banmsamt            ;    // [double,   12] ������ż��ŷ����             StartPos 150, Length 12
    char    insmsvol            [   8];    char    _insmsvol            ;    // [long  ,    8] ������ż��ŷ���               StartPos 163, Length 8
    char    insmsamt            [  12];    char    _insmsamt            ;    // [double,   12] ������ż��ŷ����             StartPos 172, Length 12
    char    finmsvol            [   8];    char    _finmsvol            ;    // [long  ,    8] ���ݼ��ż��ŷ���               StartPos 185, Length 8
    char    finmsamt            [  12];    char    _finmsamt            ;    // [double,   12] ���ݼ��ż��ŷ����             StartPos 194, Length 12
    char    monmsvol            [   8];    char    _monmsvol            ;    // [long  ,    8] ��ݼ��ż��ŷ���               StartPos 207, Length 8
    char    monmsamt            [  12];    char    _monmsamt            ;    // [double,   12] ��ݼ��ż��ŷ����             StartPos 216, Length 12
    char    etcmsvol            [   8];    char    _etcmsvol            ;    // [long  ,    8] ��Ÿ���ż��ŷ���               StartPos 229, Length 8
    char    etcmsamt            [  12];    char    _etcmsamt            ;    // [double,   12] ��Ÿ���ż��ŷ����             StartPos 238, Length 12
    char    natmsvol            [   8];    char    _natmsvol            ;    // [long  ,    8] �������ż��ŷ���               StartPos 251, Length 8
    char    natmsamt            [  12];    char    _natmsamt            ;    // [double,   12] �������ż��ŷ����             StartPos 260, Length 12
    char    pefmsvol            [   8];    char    _pefmsvol            ;    // [long  ,    8] ����ݵ���ż��ŷ���           StartPos 273, Length 8
    char    pefmsamt            [  12];    char    _pefmsamt            ;    // [double,   12] ����ݵ���ż��ŷ����         StartPos 282, Length 12
    char    upclose             [   6];    char    _upclose             ;    // [float ,  6.2] ��������                       StartPos 295, Length 6
    char    upcvolume           [   8];    char    _upcvolume           ;    // [long  ,    8] ����ü��ŷ���                 StartPos 302, Length 8
    char    upvolume            [  12];    char    _upvolume            ;    // [double,   12] ���ش����ŷ���                 StartPos 311, Length 12
    char    upvalue             [  12];    char    _upvalue             ;    // [double,   12] ���ذŷ����                   StartPos 324, Length 12
} t1621OutBlock1, *LPt1621OutBlock1;
#define NAME_t1621OutBlock1     "t1621OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
