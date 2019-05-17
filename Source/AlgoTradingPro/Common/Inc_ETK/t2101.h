///////////////////////////////////////////////////////////////////////////////////////////////////
// ����/�ɼ����簡(�ü�)��ȸ(t2101) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t2101     "t2101"

// �⺻�Է�
typedef struct _t2101InBlock
{
    char    focode              [   8];    char    _focode              ;    // [string,    8] �����ڵ�                       StartPos 0, Length 8
} t2101InBlock, *LPt2101InBlock;
#define NAME_t2101InBlock     "t2101InBlock"

// ���
typedef struct _t2101OutBlock
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �ѱ۸�                         StartPos 0, Length 20
    char    price               [   6];    char    _price               ;    // [float ,  6.2] ���簡                         StartPos 21, Length 6
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 28, Length 1
    char    change              [   6];    char    _change              ;    // [float ,  6.2] ���ϴ��                       StartPos 30, Length 6
    char    jnilclose           [   6];    char    _jnilclose           ;    // [float ,  6.2] ��������                       StartPos 37, Length 6
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 44, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 51, Length 12
    char    value               [  12];    char    _value               ;    // [long  ,   12] �ŷ����                       StartPos 64, Length 12
    char    mgjv                [   8];    char    _mgjv                ;    // [long  ,    8] �̰�����                       StartPos 77, Length 8
    char    mgjvdiff            [   8];    char    _mgjvdiff            ;    // [long  ,    8] �̰�������                     StartPos 86, Length 8
    char    open                [   6];    char    _open                ;    // [float ,  6.2] �ð�                           StartPos 95, Length 6
    char    high                [   6];    char    _high                ;    // [float ,  6.2] ��                           StartPos 102, Length 6
    char    low                 [   6];    char    _low                 ;    // [float ,  6.2] ����                           StartPos 109, Length 6
    char    uplmtprice          [   6];    char    _uplmtprice          ;    // [float ,  6.2] ���Ѱ�                         StartPos 116, Length 6
    char    dnlmtprice          [   6];    char    _dnlmtprice          ;    // [float ,  6.2] ���Ѱ�                         StartPos 123, Length 6
    char    high52w             [   6];    char    _high52w             ;    // [float ,  6.2] 52�ְ�                       StartPos 130, Length 6
    char    low52w              [   6];    char    _low52w              ;    // [float ,  6.2] 52������                       StartPos 137, Length 6
    char    basis               [   6];    char    _basis               ;    // [float ,  6.2] ���̽ý�                       StartPos 144, Length 6
    char    recprice            [   6];    char    _recprice            ;    // [float ,  6.2] ���ذ�                         StartPos 151, Length 6
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] �̷а�                         StartPos 158, Length 6
    char    glyl                [   6];    char    _glyl                ;    // [float ,  6.3] ������                         StartPos 165, Length 6
    char    cbhprice            [   6];    char    _cbhprice            ;    // [float ,  6.2] CB���Ѱ�                       StartPos 172, Length 6
    char    cblprice            [   6];    char    _cblprice            ;    // [float ,  6.2] CB���Ѱ�                       StartPos 179, Length 6
    char    lastmonth           [   8];    char    _lastmonth           ;    // [string,    8] ������                         StartPos 186, Length 8
    char    jandatecnt          [   8];    char    _jandatecnt          ;    // [long  ,    8] �ܿ���                         StartPos 195, Length 8
    char    pricejisu           [   6];    char    _pricejisu           ;    // [float ,  6.2] ��������                       StartPos 204, Length 6
    char    jisusign            [   1];    char    _jisusign            ;    // [string,    1] �����������ϴ�񱸺�           StartPos 211, Length 1
    char    jisuchange          [   6];    char    _jisuchange          ;    // [float ,  6.2] �����������ϴ��               StartPos 213, Length 6
    char    jisudiff            [   6];    char    _jisudiff            ;    // [float ,  6.2] �������������                 StartPos 220, Length 6
    char    kospijisu           [   6];    char    _kospijisu           ;    // [float ,  6.2] KOSPI200����                   StartPos 227, Length 6
    char    kospisign           [   1];    char    _kospisign           ;    // [string,    1] KOSPI200���ϴ�񱸺�           StartPos 234, Length 1
    char    kospichange         [   6];    char    _kospichange         ;    // [float ,  6.2] KOSPI200���ϴ��               StartPos 236, Length 6
    char    kospidiff           [   6];    char    _kospidiff           ;    // [float ,  6.2] KOSPI200�����                 StartPos 243, Length 6
    char    listhprice          [   6];    char    _listhprice          ;    // [float ,  6.2] �����ְ�                     StartPos 250, Length 6
    char    listlprice          [   6];    char    _listlprice          ;    // [float ,  6.2] ����������                     StartPos 257, Length 6
    char    delt                [   6];    char    _delt                ;    // [float ,  6.4] ��Ÿ                           StartPos 264, Length 6
    char    gama                [   6];    char    _gama                ;    // [float ,  6.4] ����                           StartPos 271, Length 6
    char    ceta                [   6];    char    _ceta                ;    // [float ,  6.4] ��Ÿ                           StartPos 278, Length 6
    char    vega                [   6];    char    _vega                ;    // [float ,  6.4] ����                           StartPos 285, Length 6
    char    rhox                [   6];    char    _rhox                ;    // [float ,  6.4] �ο�                           StartPos 292, Length 6
    char    gmprice             [   6];    char    _gmprice             ;    // [float ,  6.2] �ٿ������簡                   StartPos 299, Length 6
    char    gmsign              [   1];    char    _gmsign              ;    // [string,    1] �ٿ������ϴ�񱸺�             StartPos 306, Length 1
    char    gmchange            [   6];    char    _gmchange            ;    // [float ,  6.2] �ٿ������ϴ��                 StartPos 308, Length 6
    char    gmdiff              [   6];    char    _gmdiff              ;    // [float ,  6.2] �ٿ��������                   StartPos 315, Length 6
    char    theorypriceg        [   6];    char    _theorypriceg        ;    // [float ,  6.2] �̷а�                         StartPos 322, Length 6
    char    histimpv            [   6];    char    _histimpv            ;    // [float ,  6.2] ������������                   StartPos 329, Length 6
    char    impv                [   6];    char    _impv                ;    // [float ,  6.2] ���纯����                     StartPos 336, Length 6
    char    sbasis              [   6];    char    _sbasis              ;    // [float ,  6.2] ����BASIS                      StartPos 343, Length 6
    char    ibasis              [   6];    char    _ibasis              ;    // [float ,  6.2] �̷�BASIS                      StartPos 350, Length 6
    char    gmfutcode           [   8];    char    _gmfutcode           ;    // [string,    8] �ٿ��������ڵ�                 StartPos 357, Length 8
    char    actprice            [   6];    char    _actprice            ;    // [float ,  6.2] ��簡                         StartPos 366, Length 6
} t2101OutBlock, *LPt2101OutBlock;
#define NAME_t2101OutBlock     "t2101OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
