///////////////////////////////////////////////////////////////////////////////////////////////////
// KOSPI���α׷��Ÿ���ü����(PM) ( attr,block )
#pragma pack( push, 1 )

#define NAME_PM_     "PM_"

// �Է�
typedef struct _PM__InBlock
{
    char    gubun               [   1];    // [string,    1] ���а�                         StartPos 0, Length 1
} PM__InBlock, *LPPM__InBlock;
#define NAME_PM__InBlock     "InBlock"

// ���
typedef struct _PM__OutBlock
{
    char    time                [   6];    char    _time                ;    // [string,    6] ���Žð�                       StartPos 0, Length 6
    char    cdhrem              [   6];    char    _cdhrem              ;    // [long  ,    6] ���͸ŵ�ȣ�� �ܷ�              StartPos 7, Length 6
    char    cshrem              [   6];    char    _cshrem              ;    // [long  ,    6] ���͸ż�ȣ�� �ܷ�              StartPos 14, Length 6
    char    bdhrem              [   6];    char    _bdhrem              ;    // [long  ,    6] �����͸ŵ�ȣ�� �ܷ�            StartPos 21, Length 6
    char    bshrem              [   6];    char    _bshrem              ;    // [long  ,    6] �����͸ż�ȣ�� �ܷ�            StartPos 28, Length 6
    char    cdhvolume           [   6];    char    _cdhvolume           ;    // [long  ,    6] ���͸ŵ�ȣ�� ����              StartPos 35, Length 6
    char    cshvolume           [   6];    char    _cshvolume           ;    // [long  ,    6] ���͸ż�ȣ�� ����              StartPos 42, Length 6
    char    bdhvolume           [   6];    char    _bdhvolume           ;    // [long  ,    6] �����͸ŵ�ȣ�� ����            StartPos 49, Length 6
    char    bshvolume           [   6];    char    _bshvolume           ;    // [long  ,    6] �����͸ż�ȣ�� ����            StartPos 56, Length 6
    char    cdwvolume           [   6];    char    _cdwvolume           ;    // [long  ,    6] ���͸ŵ���Źü�����           StartPos 63, Length 6
    char    cdjvolume           [   6];    char    _cdjvolume           ;    // [long  ,    6] ���͸ŵ��ڱ�ü�����           StartPos 70, Length 6
    char    cswvolume           [   6];    char    _cswvolume           ;    // [long  ,    6] ���͸ż���Źü�����           StartPos 77, Length 6
    char    csjvolume           [   6];    char    _csjvolume           ;    // [long  ,    6] ���͸ż��ڱ�ü�����           StartPos 84, Length 6
    char    cwvol               [   6];    char    _cwvol               ;    // [long  ,    6] ������Ź���ż� ����            StartPos 91, Length 6
    char    cjvol               [   6];    char    _cjvol               ;    // [long  ,    6] �����ڱ���ż� ����            StartPos 98, Length 6
    char    bdwvolume           [   6];    char    _bdwvolume           ;    // [long  ,    6] �����͸ŵ���Źü�����         StartPos 105, Length 6
    char    bdjvolume           [   6];    char    _bdjvolume           ;    // [long  ,    6] �����͸ŵ��ڱ�ü�����         StartPos 112, Length 6
    char    bswvolume           [   6];    char    _bswvolume           ;    // [long  ,    6] �����͸ż���Źü�����         StartPos 119, Length 6
    char    bsjvolume           [   6];    char    _bsjvolume           ;    // [long  ,    6] �����͸ż��ڱ�ü�����         StartPos 126, Length 6
    char    bwvol               [   6];    char    _bwvol               ;    // [long  ,    6] ��������Ź���ż� ����          StartPos 133, Length 6
    char    bjvol               [   6];    char    _bjvol               ;    // [long  ,    6] �������ڱ���ż� ����          StartPos 140, Length 6
    char    dwvolume            [   6];    char    _dwvolume            ;    // [long  ,    6] ��ü�ŵ���Źü�����           StartPos 147, Length 6
    char    swvolume            [   6];    char    _swvolume            ;    // [long  ,    6] ��ü�ż���Źü�����           StartPos 154, Length 6
    char    wvol                [   6];    char    _wvol                ;    // [long  ,    6] ��ü��Ź���ż� ����            StartPos 161, Length 6
    char    djvolume            [   6];    char    _djvolume            ;    // [long  ,    6] ��ü�ŵ��ڱ�ü�����           StartPos 168, Length 6
    char    sjvolume            [   6];    char    _sjvolume            ;    // [long  ,    6] ��ü�ż��ڱ�ü�����           StartPos 175, Length 6
    char    jvol                [   6];    char    _jvol                ;    // [long  ,    6] ��ü�ڱ���ż� ����            StartPos 182, Length 6
    char    cdwvalue            [   8];    char    _cdwvalue            ;    // [long  ,    8] ���͸ŵ���Źü��ݾ�           StartPos 189, Length 8
    char    cdjvalue            [   8];    char    _cdjvalue            ;    // [long  ,    8] ���͸ŵ��ڱ�ü��ݾ�           StartPos 198, Length 8
    char    cswvalue            [   8];    char    _cswvalue            ;    // [long  ,    8] ���͸ż���Źü��ݾ�           StartPos 207, Length 8
    char    csjvalue            [   8];    char    _csjvalue            ;    // [long  ,    8] ���͸ż��ڱ�ü��ݾ�           StartPos 216, Length 8
    char    cwval               [   8];    char    _cwval               ;    // [long  ,    8] ������Ź���ż� �ݾ�            StartPos 225, Length 8
    char    cjval               [   8];    char    _cjval               ;    // [long  ,    8] �����ڱ���ż� �ݾ�            StartPos 234, Length 8
    char    bdwvalue            [   8];    char    _bdwvalue            ;    // [long  ,    8] �����͸ŵ���Źü��ݾ�         StartPos 243, Length 8
    char    bdjvalue            [   8];    char    _bdjvalue            ;    // [long  ,    8] �����͸ŵ��ڱ�ü��ݾ�         StartPos 252, Length 8
    char    bswvalue            [   8];    char    _bswvalue            ;    // [long  ,    8] �����͸ż���Źü��ݾ�         StartPos 261, Length 8
    char    bsjvalue            [   8];    char    _bsjvalue            ;    // [long  ,    8] �����͸ż��ڱ�ü��ݾ�         StartPos 270, Length 8
    char    bwval               [   8];    char    _bwval               ;    // [long  ,    8] ��������Ź���ż� �ݾ�          StartPos 279, Length 8
    char    bjval               [   8];    char    _bjval               ;    // [long  ,    8] �������ڱ���ż� �ݾ�          StartPos 288, Length 8
    char    dwvalue             [   8];    char    _dwvalue             ;    // [long  ,    8] ��ü�ŵ���Źü��ݾ�           StartPos 297, Length 8
    char    swvalue             [   8];    char    _swvalue             ;    // [long  ,    8] ��ü�ż���Źü��ݾ�           StartPos 306, Length 8
    char    wval                [   8];    char    _wval                ;    // [long  ,    8] ��ü��Ź���ż� �ݾ�            StartPos 315, Length 8
    char    djvalue             [   8];    char    _djvalue             ;    // [long  ,    8] ��ü�ŵ��ڱ�ü��ݾ�           StartPos 324, Length 8
    char    sjvalue             [   8];    char    _sjvalue             ;    // [long  ,    8] ��ü�ż��ڱ�ü��ݾ�           StartPos 333, Length 8
    char    jval                [   8];    char    _jval                ;    // [long  ,    8] ��ü�ڱ���ż� �ݾ�            StartPos 342, Length 8
    char    k200jisu            [   6];    char    _k200jisu            ;    // [float ,  6.2] KOSPI200 ����                  StartPos 351, Length 6
    char    k200sign            [   1];    char    _k200sign            ;    // [string,    1] KOSPI200 ���ϴ�񱸺�          StartPos 358, Length 1
    char    change              [   6];    char    _change              ;    // [float ,  6.2] KOSPI200 ���ϴ��              StartPos 360, Length 6
    char    k200basis           [   4];    char    _k200basis           ;    // [float ,  4.2] KOSPI200 ���̽ý�              StartPos 367, Length 4
    char    cdvolume            [   6];    char    _cdvolume            ;    // [long  ,    6] ���͸ŵ�ü������հ�           StartPos 372, Length 6
    char    csvolume            [   6];    char    _csvolume            ;    // [long  ,    6] ���͸ż�ü������հ�           StartPos 379, Length 6
    char    cvol                [   6];    char    _cvol                ;    // [long  ,    6] ���ͼ��ż� �����հ�            StartPos 386, Length 6
    char    bdvolume            [   6];    char    _bdvolume            ;    // [long  ,    6] �����͸ŵ�ü������հ�         StartPos 393, Length 6
    char    bsvolume            [   6];    char    _bsvolume            ;    // [long  ,    6] �����͸ż�ü������հ�         StartPos 400, Length 6
    char    bvol                [   6];    char    _bvol                ;    // [long  ,    6] �����ͼ��ż� �����հ�          StartPos 407, Length 6
    char    tdvolume            [   6];    char    _tdvolume            ;    // [long  ,    6] ��ü�ŵ�ü������հ�           StartPos 414, Length 6
    char    tsvolume            [   6];    char    _tsvolume            ;    // [long  ,    6] ��ü�ż�ü������հ�           StartPos 421, Length 6
    char    tvol                [   6];    char    _tvol                ;    // [long  ,    6] ��ü���ż� �����հ�            StartPos 428, Length 6
    char    cdvalue             [   8];    char    _cdvalue             ;    // [long  ,    8] ���͸ŵ�ü��ݾ��հ�           StartPos 435, Length 8
    char    csvalue             [   8];    char    _csvalue             ;    // [long  ,    8] ���͸ż�ü��ݾ��հ�           StartPos 444, Length 8
    char    cval                [   8];    char    _cval                ;    // [long  ,    8] ���ͼ��ż� �ݾ��հ�            StartPos 453, Length 8
    char    bdvalue             [   8];    char    _bdvalue             ;    // [long  ,    8] �����͸ŵ�ü��ݾ��հ�         StartPos 462, Length 8
    char    bsvalue             [   8];    char    _bsvalue             ;    // [long  ,    8] �����͸ż�ü��ݾ��հ�         StartPos 471, Length 8
    char    bval                [   8];    char    _bval                ;    // [long  ,    8] �����ͼ��ż� �ݾ��հ�          StartPos 480, Length 8
    char    tdvalue             [   8];    char    _tdvalue             ;    // [long  ,    8] ��ü�ŵ�ü��ݾ��հ�           StartPos 489, Length 8
    char    tsvalue             [   8];    char    _tsvalue             ;    // [long  ,    8] ��ü�ż�ü��ݾ��հ�           StartPos 498, Length 8
    char    tval                [   8];    char    _tval                ;    // [long  ,    8] ��ü���ż� �ݾ��հ�            StartPos 507, Length 8
    char    p_cdvolcha          [   6];    char    _p_cdvolcha          ;    // [long  ,    6] ���͸ŵ�ü������������       StartPos 516, Length 6
    char    p_csvolcha          [   6];    char    _p_csvolcha          ;    // [long  ,    6] ���͸ż�ü������������       StartPos 523, Length 6
    char    p_cvolcha           [   6];    char    _p_cvolcha           ;    // [long  ,    6] ���ͼ��ż� �����������        StartPos 530, Length 6
    char    p_bdvolcha          [   6];    char    _p_bdvolcha          ;    // [long  ,    6] �����͸ŵ�ü������������     StartPos 537, Length 6
    char    p_bsvolcha          [   6];    char    _p_bsvolcha          ;    // [long  ,    6] �����͸ż�ü������������     StartPos 544, Length 6
    char    p_bvolcha           [   6];    char    _p_bvolcha           ;    // [long  ,    6] �����ͼ��ż� �����������      StartPos 551, Length 6
    char    p_tdvolcha          [   6];    char    _p_tdvolcha          ;    // [long  ,    6] ��ü�ŵ�ü������������       StartPos 558, Length 6
    char    p_tsvolcha          [   6];    char    _p_tsvolcha          ;    // [long  ,    6] ��ü�ż�ü������������       StartPos 565, Length 6
    char    p_tvolcha           [   6];    char    _p_tvolcha           ;    // [long  ,    6] ��ü���ż� �����������        StartPos 572, Length 6
    char    p_cdvalcha          [   8];    char    _p_cdvalcha          ;    // [long  ,    8] ���͸ŵ�ü��ݾ��������       StartPos 579, Length 8
    char    p_csvalcha          [   8];    char    _p_csvalcha          ;    // [long  ,    8] ���͸ż�ü��ݾ��������       StartPos 588, Length 8
    char    p_cvalcha           [   8];    char    _p_cvalcha           ;    // [long  ,    8] ���ͼ��ż� �ݾ��������        StartPos 597, Length 8
    char    p_bdvalcha          [   8];    char    _p_bdvalcha          ;    // [long  ,    8] �����͸ŵ�ü��ݾ��������     StartPos 606, Length 8
    char    p_bsvalcha          [   8];    char    _p_bsvalcha          ;    // [long  ,    8] �����͸ż�ü��ݾ��������     StartPos 615, Length 8
    char    p_bvalcha           [   8];    char    _p_bvalcha           ;    // [long  ,    8] �����ͼ��ż� �ݾ��������      StartPos 624, Length 8
    char    p_tdvalcha          [   8];    char    _p_tdvalcha          ;    // [long  ,    8] ��ü�ŵ�ü��ݾ��������       StartPos 633, Length 8
    char    p_tsvalcha          [   8];    char    _p_tsvalcha          ;    // [long  ,    8] ��ü�ż�ü��ݾ��������       StartPos 642, Length 8
    char    p_tvalcha           [   8];    char    _p_tvalcha           ;    // [long  ,    8] ��ü���ż� �ݾ��������        StartPos 651, Length 8
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ���а�                         StartPos 660, Length 1
} PM__OutBlock, *LPPM__OutBlock;
#define NAME_PM__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
