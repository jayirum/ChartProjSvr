///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ����簡(�ü�)��ȸ(t1102) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1102     "t1102"

// �⺻�Է�
typedef struct _t1102InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
} t1102InBlock, *LPt1102InBlock;
#define NAME_t1102InBlock     "t1102InBlock"

// ���
typedef struct _t1102OutBlock
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �ѱ۸�                         StartPos 0, Length 20
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 21, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 30, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 32, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 41, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 48, Length 12
    char    recprice            [   8];    char    _recprice            ;    // [long  ,    8] ���ذ�                         StartPos 61, Length 8
    char    avg                 [   8];    char    _avg                 ;    // [long  ,    8] �������                       StartPos 70, Length 8
    char    uplmtprice          [   8];    char    _uplmtprice          ;    // [long  ,    8] ���Ѱ�                         StartPos 79, Length 8
    char    dnlmtprice          [   8];    char    _dnlmtprice          ;    // [long  ,    8] ���Ѱ�                         StartPos 88, Length 8
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϰŷ���                     StartPos 97, Length 12
    char    volumediff          [  12];    char    _volumediff          ;    // [long  ,   12] �ŷ�����                       StartPos 110, Length 12
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�                           StartPos 123, Length 8
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] �ð��ð�                       StartPos 132, Length 6
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��                           StartPos 139, Length 8
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] ���ð�                       StartPos 148, Length 6
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����                           StartPos 155, Length 8
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] �����ð�                       StartPos 164, Length 6
    char    high52w             [   8];    char    _high52w             ;    // [long  ,    8] 52�ְ�                       StartPos 171, Length 8
    char    high52wdate         [   8];    char    _high52wdate         ;    // [string,    8] 52�ְ���                     StartPos 180, Length 8
    char    low52w              [   8];    char    _low52w              ;    // [long  ,    8] 52������                       StartPos 189, Length 8
    char    low52wdate          [   8];    char    _low52wdate          ;    // [string,    8] 52��������                     StartPos 198, Length 8
    char    exhratio            [   6];    char    _exhratio            ;    // [float ,  6.2] ������                         StartPos 207, Length 6
    char    per                 [   6];    char    _per                 ;    // [float ,  6.2] PER                            StartPos 214, Length 6
    char    pbrx                [   6];    char    _pbrx                ;    // [float ,  6.2] PBRX                           StartPos 221, Length 6
    char    listing             [  12];    char    _listing             ;    // [long  ,   12] �����ֽļ�(õ)                 StartPos 228, Length 12
    char    jkrate              [   8];    char    _jkrate              ;    // [long  ,    8] ���ű���                       StartPos 241, Length 8
    char    memedan             [   5];    char    _memedan             ;    // [string,    5] ��������                       StartPos 250, Length 5
    char    offernocd1          [   3];    char    _offernocd1          ;    // [string,    3] �ŵ����ǻ��ڵ�1                StartPos 256, Length 3
    char    bidnocd1            [   3];    char    _bidnocd1            ;    // [string,    3] �ż����ǻ��ڵ�1                StartPos 260, Length 3
    char    offerno1            [   6];    char    _offerno1            ;    // [string,    6] �ŵ����ǻ��1                  StartPos 264, Length 6
    char    bidno1              [   6];    char    _bidno1              ;    // [string,    6] �ż����ǻ��1                  StartPos 271, Length 6
    char    dvol1               [   8];    char    _dvol1               ;    // [long  ,    8] �Ѹŵ�����1                    StartPos 278, Length 8
    char    svol1               [   8];    char    _svol1               ;    // [long  ,    8] �Ѹż�����1                    StartPos 287, Length 8
    char    dcha1               [   8];    char    _dcha1               ;    // [long  ,    8] �ŵ�����1                      StartPos 296, Length 8
    char    scha1               [   8];    char    _scha1               ;    // [long  ,    8] �ż�����1                      StartPos 305, Length 8
    char    ddiff1              [   6];    char    _ddiff1              ;    // [float ,  6.2] �ŵ�����1                      StartPos 314, Length 6
    char    sdiff1              [   6];    char    _sdiff1              ;    // [float ,  6.2] �ż�����1                      StartPos 321, Length 6
    char    offernocd2          [   3];    char    _offernocd2          ;    // [string,    3] �ŵ����ǻ��ڵ�2                StartPos 328, Length 3
    char    bidnocd2            [   3];    char    _bidnocd2            ;    // [string,    3] �ż����ǻ��ڵ�2                StartPos 332, Length 3
    char    offerno2            [   6];    char    _offerno2            ;    // [string,    6] �ŵ����ǻ��2                  StartPos 336, Length 6
    char    bidno2              [   6];    char    _bidno2              ;    // [string,    6] �ż����ǻ��2                  StartPos 343, Length 6
    char    dvol2               [   8];    char    _dvol2               ;    // [long  ,    8] �Ѹŵ�����2                    StartPos 350, Length 8
    char    svol2               [   8];    char    _svol2               ;    // [long  ,    8] �Ѹż�����2                    StartPos 359, Length 8
    char    dcha2               [   8];    char    _dcha2               ;    // [long  ,    8] �ŵ�����2                      StartPos 368, Length 8
    char    scha2               [   8];    char    _scha2               ;    // [long  ,    8] �ż�����2                      StartPos 377, Length 8
    char    ddiff2              [   6];    char    _ddiff2              ;    // [float ,  6.2] �ŵ�����2                      StartPos 386, Length 6
    char    sdiff2              [   6];    char    _sdiff2              ;    // [float ,  6.2] �ż�����2                      StartPos 393, Length 6
    char    offernocd3          [   3];    char    _offernocd3          ;    // [string,    3] �ŵ����ǻ��ڵ�3                StartPos 400, Length 3
    char    bidnocd3            [   3];    char    _bidnocd3            ;    // [string,    3] �ż����ǻ��ڵ�3                StartPos 404, Length 3
    char    offerno3            [   6];    char    _offerno3            ;    // [string,    6] �ŵ����ǻ��3                  StartPos 408, Length 6
    char    bidno3              [   6];    char    _bidno3              ;    // [string,    6] �ż����ǻ��3                  StartPos 415, Length 6
    char    dvol3               [   8];    char    _dvol3               ;    // [long  ,    8] �Ѹŵ�����3                    StartPos 422, Length 8
    char    svol3               [   8];    char    _svol3               ;    // [long  ,    8] �Ѹż�����3                    StartPos 431, Length 8
    char    dcha3               [   8];    char    _dcha3               ;    // [long  ,    8] �ŵ�����3                      StartPos 440, Length 8
    char    scha3               [   8];    char    _scha3               ;    // [long  ,    8] �ż�����3                      StartPos 449, Length 8
    char    ddiff3              [   6];    char    _ddiff3              ;    // [float ,  6.2] �ŵ�����3                      StartPos 458, Length 6
    char    sdiff3              [   6];    char    _sdiff3              ;    // [float ,  6.2] �ż�����3                      StartPos 465, Length 6
    char    offernocd4          [   3];    char    _offernocd4          ;    // [string,    3] �ŵ����ǻ��ڵ�4                StartPos 472, Length 3
    char    bidnocd4            [   3];    char    _bidnocd4            ;    // [string,    3] �ż����ǻ��ڵ�4                StartPos 476, Length 3
    char    offerno4            [   6];    char    _offerno4            ;    // [string,    6] �ŵ����ǻ��4                  StartPos 480, Length 6
    char    bidno4              [   6];    char    _bidno4              ;    // [string,    6] �ż����ǻ��4                  StartPos 487, Length 6
    char    dvol4               [   8];    char    _dvol4               ;    // [long  ,    8] �Ѹŵ�����4                    StartPos 494, Length 8
    char    svol4               [   8];    char    _svol4               ;    // [long  ,    8] �Ѹż�����4                    StartPos 503, Length 8
    char    dcha4               [   8];    char    _dcha4               ;    // [long  ,    8] �ŵ�����4                      StartPos 512, Length 8
    char    scha4               [   8];    char    _scha4               ;    // [long  ,    8] �ż�����4                      StartPos 521, Length 8
    char    ddiff4              [   6];    char    _ddiff4              ;    // [float ,  6.2] �ŵ�����4                      StartPos 530, Length 6
    char    sdiff4              [   6];    char    _sdiff4              ;    // [float ,  6.2] �ż�����4                      StartPos 537, Length 6
    char    offernocd5          [   3];    char    _offernocd5          ;    // [string,    3] �ŵ����ǻ��ڵ�5                StartPos 544, Length 3
    char    bidnocd5            [   3];    char    _bidnocd5            ;    // [string,    3] �ż����ǻ��ڵ�5                StartPos 548, Length 3
    char    offerno5            [   6];    char    _offerno5            ;    // [string,    6] �ŵ����ǻ��5                  StartPos 552, Length 6
    char    bidno5              [   6];    char    _bidno5              ;    // [string,    6] �ż����ǻ��5                  StartPos 559, Length 6
    char    dvol5               [   8];    char    _dvol5               ;    // [long  ,    8] �Ѹŵ�����5                    StartPos 566, Length 8
    char    svol5               [   8];    char    _svol5               ;    // [long  ,    8] �Ѹż�����5                    StartPos 575, Length 8
    char    dcha5               [   8];    char    _dcha5               ;    // [long  ,    8] �ŵ�����5                      StartPos 584, Length 8
    char    scha5               [   8];    char    _scha5               ;    // [long  ,    8] �ż�����5                      StartPos 593, Length 8
    char    ddiff5              [   6];    char    _ddiff5              ;    // [float ,  6.2] �ŵ�����5                      StartPos 602, Length 6
    char    sdiff5              [   6];    char    _sdiff5              ;    // [float ,  6.2] �ż�����5                      StartPos 609, Length 6
    char    fwdvl               [  12];    char    _fwdvl               ;    // [long  ,   12] �ܱ���ŵ��հ����             StartPos 616, Length 12
    char    ftradmdcha          [  12];    char    _ftradmdcha          ;    // [long  ,   12] �ܱ���ŵ��������             StartPos 629, Length 12
    char    ftradmddiff         [   6];    char    _ftradmddiff         ;    // [float ,  6.2] �ܱ���ŵ�����                 StartPos 642, Length 6
    char    fwsvl               [  12];    char    _fwsvl               ;    // [long  ,   12] �ܱ���ż��հ����             StartPos 649, Length 12
    char    ftradmscha          [  12];    char    _ftradmscha          ;    // [long  ,   12] �ܱ���ż��������             StartPos 662, Length 12
    char    ftradmsdiff         [   6];    char    _ftradmsdiff         ;    // [float ,  6.2] �ܱ���ż�����                 StartPos 675, Length 6
    char    vol                 [   6];    char    _vol                 ;    // [float ,  6.2] ȸ����                         StartPos 682, Length 6
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 689, Length 6
    char    value               [  12];    char    _value               ;    // [long  ,   12] �����ŷ����                   StartPos 696, Length 12
    char    jvolume             [  12];    char    _jvolume             ;    // [long  ,   12] ���ϵ��ð��ŷ���               StartPos 709, Length 12
    char    highyear            [   8];    char    _highyear            ;    // [long  ,    8] �����ְ�                     StartPos 722, Length 8
    char    highyeardate        [   8];    char    _highyeardate        ;    // [string,    8] �����ְ�����                   StartPos 731, Length 8
    char    lowyear             [   8];    char    _lowyear             ;    // [long  ,    8] ����������                     StartPos 740, Length 8
    char    lowyeardate         [   8];    char    _lowyeardate         ;    // [string,    8] ������������                   StartPos 749, Length 8
    char    target              [   8];    char    _target              ;    // [long  ,    8] ��ǥ��                         StartPos 758, Length 8
    char    capital             [  12];    char    _capital             ;    // [long  ,   12] �ں���                         StartPos 767, Length 12
    char    abscnt              [  12];    char    _abscnt              ;    // [long  ,   12] �����ֽļ�                     StartPos 780, Length 12
    char    parprice            [   8];    char    _parprice            ;    // [long  ,    8] �׸鰡                         StartPos 793, Length 8
    char    gsmm                [   2];    char    _gsmm                ;    // [string,    2] ����                         StartPos 802, Length 2
    char    subprice            [   8];    char    _subprice            ;    // [long  ,    8] ��밡                         StartPos 805, Length 8
    char    total               [  12];    char    _total               ;    // [long  ,   12] �ð��Ѿ�                       StartPos 814, Length 12
    char    listdate            [   8];    char    _listdate            ;    // [string,    8] ������                         StartPos 827, Length 8
    char    name                [  10];    char    _name                ;    // [string,   10] ���б��                       StartPos 836, Length 10
    char    bfsales             [  12];    char    _bfsales             ;    // [long  ,   12] ���б�����                   StartPos 847, Length 12
    char    bfoperatingincome   [  12];    char    _bfoperatingincome   ;    // [long  ,   12] ���б⿵������                 StartPos 860, Length 12
    char    bfordinaryincome    [  12];    char    _bfordinaryincome    ;    // [long  ,   12] ���б�������                 StartPos 873, Length 12
    char    bfnetincome         [  12];    char    _bfnetincome         ;    // [long  ,   12] ���б������                   StartPos 886, Length 12
    char    bfeps               [  13];    char    _bfeps               ;    // [float , 13.2] ���б�EPS                      StartPos 899, Length 13
    char    name2               [  10];    char    _name2               ;    // [string,   10] �����б��                     StartPos 913, Length 10
    char    bfsales2            [  12];    char    _bfsales2            ;    // [long  ,   12] �����б�����                 StartPos 924, Length 12
    char    bfoperatingincome2  [  12];    char    _bfoperatingincome2  ;    // [long  ,   12] �����б⿵������               StartPos 937, Length 12
    char    bfordinaryincome2   [  12];    char    _bfordinaryincome2   ;    // [long  ,   12] �����б�������               StartPos 950, Length 12
    char    bfnetincome2        [  12];    char    _bfnetincome2        ;    // [long  ,   12] �����б������                 StartPos 963, Length 12
    char    bfeps2              [  13];    char    _bfeps2              ;    // [float , 13.2] �����б�EPS                    StartPos 976, Length 13
    char    salert              [   7];    char    _salert              ;    // [float ,  7.2] ����������                 StartPos 990, Length 7
    char    opert               [   7];    char    _opert               ;    // [float ,  7.2] �����񿵾�����               StartPos 998, Length 7
    char    ordrt               [   7];    char    _ordrt               ;    // [float ,  7.2] ������������               StartPos 1006, Length 7
    char    netrt               [   7];    char    _netrt               ;    // [float ,  7.2] �����������                 StartPos 1014, Length 7
    char    epsrt               [   7];    char    _epsrt               ;    // [float ,  7.2] ������EPS                    StartPos 1022, Length 7
    char    info1               [  10];    char    _info1               ;    // [string,   10] ������                         StartPos 1030, Length 10
    char    info2               [  10];    char    _info2               ;    // [string,   10] ����/�޵��                  StartPos 1041, Length 10
    char    info3               [  10];    char    _info3               ;    // [string,   10] ����/���屸��                  StartPos 1052, Length 10
    char    info4               [  12];    char    _info4               ;    // [string,   12] ����/�Ҽ��Ǳ���                StartPos 1063, Length 12
    char    janginfo            [  10];    char    _janginfo            ;    // [string,   10] �屸��                         StartPos 1076, Length 10
    char    t_per               [   6];    char    _t_per               ;    // [float ,  6.2] T.PER                          StartPos 1087, Length 6
} t1102OutBlock, *LPt1102OutBlock;
#define NAME_t1102OutBlock     "t1102OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
