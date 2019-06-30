///////////////////////////////////////////////////////////////////////////////////////////////////
// ����(IJ) ( attr,block )
#pragma pack( push, 1 )

#define NAME_IJ_     "IJ_"

// �Է�
typedef struct _IJ__InBlock
{
    char    upcode              [   3];    // [string,    3] �����ڵ�                       StartPos 0, Length 3
} IJ__InBlock, *LPIJ__InBlock;
#define NAME_IJ__InBlock     "InBlock"

// ���
typedef struct _IJ__OutBlock
{
    char    time                [   6];    char    _time                ;    // [string,    6] �ð�                           StartPos 0, Length 6
    char    jisu                [   8];    char    _jisu                ;    // [float ,  8.2] ����                           StartPos 7, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 16, Length 1
    char    change              [   8];    char    _change              ;    // [float ,  8.2] ���Ϻ�                         StartPos 18, Length 8
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����                         StartPos 27, Length 6
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ü�ᷮ                         StartPos 34, Length 8
    char    volume              [   8];    char    _volume              ;    // [long  ,    8] �ŷ���                         StartPos 43, Length 8
    char    value               [   8];    char    _value               ;    // [long  ,    8] �ŷ����                       StartPos 52, Length 8
    char    upjo                [   4];    char    _upjo                ;    // [long  ,    4] ���������                     StartPos 61, Length 4
    char    highjo              [   4];    char    _highjo              ;    // [long  ,    4] ��������                     StartPos 66, Length 4
    char    unchgjo             [   4];    char    _unchgjo             ;    // [long  ,    4] ���������                     StartPos 71, Length 4
    char    lowjo               [   4];    char    _lowjo               ;    // [long  ,    4] �϶������                     StartPos 76, Length 4
    char    downjo              [   4];    char    _downjo              ;    // [long  ,    4] ���������                     StartPos 81, Length 4
    char    upjrate             [   6];    char    _upjrate             ;    // [float ,  6.2] ����������                   StartPos 86, Length 6
    char    openjisu            [   8];    char    _openjisu            ;    // [float ,  8.2] �ð�����                       StartPos 93, Length 8
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] �ð��ð�                       StartPos 102, Length 6
    char    highjisu            [   8];    char    _highjisu            ;    // [float ,  8.2] ������                       StartPos 109, Length 8
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] ���ð�                       StartPos 118, Length 6
    char    lowjisu             [   8];    char    _lowjisu             ;    // [float ,  8.2] ��������                       StartPos 125, Length 8
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] �����ð�                       StartPos 134, Length 6
    char    frgsvolume          [   8];    char    _frgsvolume          ;    // [long  ,    8] ���μ��ż�����                 StartPos 141, Length 8
    char    orgsvolume          [   8];    char    _orgsvolume          ;    // [long  ,    8] ������ż�����                 StartPos 150, Length 8
    char    frgsvalue           [  10];    char    _frgsvalue           ;    // [long  ,   10] ���μ��ż��ݾ�                 StartPos 159, Length 10
    char    orgsvalue           [  10];    char    _orgsvalue           ;    // [long  ,   10] ������ż��ݾ�                 StartPos 170, Length 10
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 181, Length 3
} IJ__OutBlock, *LPIJ__OutBlock;
#define NAME_IJ__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
