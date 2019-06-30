///////////////////////////////////////////////////////////////////////////////////////////////////
// �������簡(t1511) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1511     "t1511"

// �⺻�Է�
typedef struct _t1511InBlock
{
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 0, Length 3
} t1511InBlock, *LPt1511InBlock;
#define NAME_t1511InBlock     "t1511InBlock"

// �⺻���
typedef struct _t1511OutBlock
{
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ��������                       StartPos 0, Length 1
    char    hname               [  20];    char    _hname               ;    // [string,   20] ������                         StartPos 2, Length 20
    char    pricejisu           [   7];    char    _pricejisu           ;    // [float ,  7.2] ��������                       StartPos 23, Length 7
    char    jniljisu            [   7];    char    _jniljisu            ;    // [float ,  7.2] ��������                       StartPos 31, Length 7
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 39, Length 1
    char    change              [   7];    char    _change              ;    // [float ,  7.2] ���ϴ��                       StartPos 41, Length 7
    char    diffjisu            [   6];    char    _diffjisu            ;    // [float ,  6.2] ���������                     StartPos 49, Length 6
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϰŷ���                     StartPos 56, Length 12
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] ���ϰŷ���                     StartPos 69, Length 12
    char    volumechange        [  12];    char    _volumechange        ;    // [long  ,   12] �ŷ������ϴ��                 StartPos 82, Length 12
    char    volumerate          [   6];    char    _volumerate          ;    // [float ,  6.2] �ŷ�������                     StartPos 95, Length 6
    char    jnilvalue           [  12];    char    _jnilvalue           ;    // [long  ,   12] ���ϰŷ����                   StartPos 102, Length 12
    char    value               [  12];    char    _value               ;    // [long  ,   12] ���ϰŷ����                   StartPos 115, Length 12
    char    valuechange         [  12];    char    _valuechange         ;    // [long  ,   12] �ŷ�������ϴ��               StartPos 128, Length 12
    char    valuerate           [   6];    char    _valuerate           ;    // [float ,  6.2] �ŷ���ݺ���                   StartPos 141, Length 6
    char    openjisu            [   7];    char    _openjisu            ;    // [float ,  7.2] �ð�����                       StartPos 148, Length 7
    char    opendiff            [   6];    char    _opendiff            ;    // [float ,  6.2] �ð������                     StartPos 156, Length 6
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] �ð��ð�                       StartPos 163, Length 6
    char    highjisu            [   7];    char    _highjisu            ;    // [float ,  7.2] ������                       StartPos 170, Length 7
    char    highdiff            [   6];    char    _highdiff            ;    // [float ,  6.2] �������                     StartPos 178, Length 6
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] ���ð�                       StartPos 185, Length 6
    char    lowjisu             [   7];    char    _lowjisu             ;    // [float ,  7.2] ��������                       StartPos 192, Length 7
    char    lowdiff             [   6];    char    _lowdiff             ;    // [float ,  6.2] ���������                     StartPos 200, Length 6
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] �����ð�                       StartPos 207, Length 6
    char    whjisu              [   7];    char    _whjisu              ;    // [float ,  7.2] 52���ְ�����                   StartPos 214, Length 7
    char    whchange            [   7];    char    _whchange            ;    // [float ,  7.2] 52���ְ����簡���             StartPos 222, Length 7
    char    whjday              [   8];    char    _whjday              ;    // [string,    8] 52���ְ���������               StartPos 230, Length 8
    char    wljisu              [   7];    char    _wljisu              ;    // [float ,  7.2] 52����������                   StartPos 239, Length 7
    char    wlchange            [   7];    char    _wlchange            ;    // [float ,  7.2] 52���������簡���             StartPos 247, Length 7
    char    wljday              [   8];    char    _wljday              ;    // [string,    8] 52��������������               StartPos 255, Length 8
    char    yhjisu              [   7];    char    _yhjisu              ;    // [float ,  7.2] �����ְ�����                   StartPos 264, Length 7
    char    yhchange            [   7];    char    _yhchange            ;    // [float ,  7.2] �����ְ����簡���             StartPos 272, Length 7
    char    yhjday              [   8];    char    _yhjday              ;    // [string,    8] �����ְ���������               StartPos 280, Length 8
    char    yljisu              [   7];    char    _yljisu              ;    // [float ,  7.2] ������������                   StartPos 289, Length 7
    char    ylchange            [   7];    char    _ylchange            ;    // [float ,  7.2] �����������簡���             StartPos 297, Length 7
    char    yljday              [   8];    char    _yljday              ;    // [string,    8] ����������������               StartPos 305, Length 8
    char    firstjcode          [   3];    char    _firstjcode          ;    // [string,    3] ù��°�����ڵ�                 StartPos 314, Length 3
    char    firstjname          [  20];    char    _firstjname          ;    // [string,   20] ù��°������                   StartPos 318, Length 20
    char    firstjisu           [   7];    char    _firstjisu           ;    // [float ,  7.2] ù��°����                     StartPos 339, Length 7
    char    firsign             [   1];    char    _firsign             ;    // [string,    1] ù��°��񱸺�                 StartPos 347, Length 1
    char    firchange           [   7];    char    _firchange           ;    // [float ,  7.2] ù��°���ϴ��                 StartPos 349, Length 7
    char    firdiff             [   6];    char    _firdiff             ;    // [float ,  6.2] ù��°�����                   StartPos 357, Length 6
    char    secondjcode         [   3];    char    _secondjcode         ;    // [string,    3] �ι�°�����ڵ�                 StartPos 364, Length 3
    char    secondjname         [  20];    char    _secondjname         ;    // [string,   20] �ι�°������                   StartPos 368, Length 20
    char    secondjisu          [   7];    char    _secondjisu          ;    // [float ,  7.2] �ι�°����                     StartPos 389, Length 7
    char    secsign             [   1];    char    _secsign             ;    // [string,    1] �ι�°��񱸺�                 StartPos 397, Length 1
    char    secchange           [   7];    char    _secchange           ;    // [float ,  7.2] �ι�°���ϴ��                 StartPos 399, Length 7
    char    secdiff             [   6];    char    _secdiff             ;    // [float ,  6.2] �ι�°�����                   StartPos 407, Length 6
    char    thirdjcode          [   3];    char    _thirdjcode          ;    // [string,    3] ����°�����ڵ�                 StartPos 414, Length 3
    char    thirdjname          [  20];    char    _thirdjname          ;    // [string,   20] ����°������                   StartPos 418, Length 20
    char    thirdjisu           [   7];    char    _thirdjisu           ;    // [float ,  7.2] ����°����                     StartPos 439, Length 7
    char    thrsign             [   1];    char    _thrsign             ;    // [string,    1] ����°��񱸺�                 StartPos 447, Length 1
    char    thrchange           [   7];    char    _thrchange           ;    // [float ,  7.2] ����°���ϴ��                 StartPos 449, Length 7
    char    thrdiff             [   6];    char    _thrdiff             ;    // [float ,  6.2] ����°�����                   StartPos 457, Length 6
    char    fourthjcode         [   3];    char    _fourthjcode         ;    // [string,    3] �׹�°�����ڵ�                 StartPos 464, Length 3
    char    fourthjname         [  20];    char    _fourthjname         ;    // [string,   20] �׹�°������                   StartPos 468, Length 20
    char    fourthjisu          [   7];    char    _fourthjisu          ;    // [float ,  7.2] �׹�°����                     StartPos 489, Length 7
    char    forsign             [   1];    char    _forsign             ;    // [string,    1] �׹�°��񱸺�                 StartPos 497, Length 1
    char    forchange           [   7];    char    _forchange           ;    // [float ,  7.2] �׹�°���ϴ��                 StartPos 499, Length 7
    char    fordiff             [   6];    char    _fordiff             ;    // [float ,  6.2] �׹�°�����                   StartPos 507, Length 6
    char    highjo              [   4];    char    _highjo              ;    // [long  ,    4] ��������                     StartPos 514, Length 4
    char    upjo                [   4];    char    _upjo                ;    // [long  ,    4] ���������                     StartPos 519, Length 4
    char    unchgjo             [   4];    char    _unchgjo             ;    // [long  ,    4] ���������                     StartPos 524, Length 4
    char    lowjo               [   4];    char    _lowjo               ;    // [long  ,    4] �϶������                     StartPos 529, Length 4
    char    downjo              [   4];    char    _downjo              ;    // [long  ,    4] ���������                     StartPos 534, Length 4
} t1511OutBlock, *LPt1511OutBlock;
#define NAME_t1511OutBlock     "t1511OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
