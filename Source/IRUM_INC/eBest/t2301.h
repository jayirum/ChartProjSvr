///////////////////////////////////////////////////////////////////////////////////////////////////
// �ɼ�������(t2301) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t2301     "t2301"

// �⺻�Է�
typedef struct _t2301InBlock
{
    char    yyyymm              [   6];    char    _yyyymm              ;    // [string,    6] ����                           StartPos 0, Length 6
} t2301InBlock, *LPt2301InBlock;
#define NAME_t2301InBlock     "t2301InBlock"

// ���
typedef struct _t2301OutBlock
{
    char    histimpv            [   4];    char    _histimpv            ;    // [long  ,    4] ������������                   StartPos 0, Length 4
    char    jandatecnt          [   4];    char    _jandatecnt          ;    // [long  ,    4] �ɼ�������                     StartPos 5, Length 4
    char    cimpv               [   6];    char    _cimpv               ;    // [float ,  6.3] �ݿɼǴ�ǥIV                   StartPos 10, Length 6
    char    pimpv               [   6];    char    _pimpv               ;    // [float ,  6.3] ǲ�ɼǴ�ǥIV                   StartPos 17, Length 6
    char    gmprice             [   6];    char    _gmprice             ;    // [float ,  6.2] �ٿ������簡                   StartPos 24, Length 6
    char    gmsign              [   1];    char    _gmsign              ;    // [string,    1] �ٿ������ϴ�񱸺�             StartPos 31, Length 1
    char    gmchange            [   6];    char    _gmchange            ;    // [float ,  6.2] �ٿ������ϴ��                 StartPos 33, Length 6
    char    gmdiff              [   6];    char    _gmdiff              ;    // [float ,  6.2] �ٿ��������                   StartPos 40, Length 6
    char    gmvolume            [  12];    char    _gmvolume            ;    // [long  ,   12] �ٿ����ŷ���                   StartPos 47, Length 12
    char    gmshcode            [   8];    char    _gmshcode            ;    // [string,    8] �ٿ��������ڵ�                 StartPos 60, Length 8
} t2301OutBlock, *LPt2301OutBlock;
#define NAME_t2301OutBlock     "t2301OutBlock"

// ���1, occurs
typedef struct _t2301OutBlock1
{
    char    actprice            [   6];    char    _actprice            ;    // [float ,  6.2] ��簡                         StartPos 0, Length 6
    char    optcode             [   8];    char    _optcode             ;    // [string,    8] �ݿɼ��ڵ�                     StartPos 7, Length 8
    char    price               [   6];    char    _price               ;    // [float ,  6.2] ���簡                         StartPos 16, Length 6
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 23, Length 1
    char    change              [   6];    char    _change              ;    // [float ,  6.2] ���ϴ��                       StartPos 25, Length 6
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 32, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 39, Length 12
    char    iv                  [   6];    char    _iv                  ;    // [float ,  6.2] IV                             StartPos 52, Length 6
    char    mgjv                [  12];    char    _mgjv                ;    // [long  ,   12] �̰�������                     StartPos 59, Length 12
    char    mgjvupdn            [  12];    char    _mgjvupdn            ;    // [long  ,   12] �̰�����������                 StartPos 72, Length 12
    char    offerho1            [   6];    char    _offerho1            ;    // [float ,  6.2] �ŵ�ȣ��                       StartPos 85, Length 6
    char    bidho1              [   6];    char    _bidho1              ;    // [float ,  6.2] �ż�ȣ��                       StartPos 92, Length 6
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] ü�ᷮ                         StartPos 99, Length 12
    char    delt                [   6];    char    _delt                ;    // [float ,  6.4] ��Ÿ                           StartPos 112, Length 6
    char    gama                [   6];    char    _gama                ;    // [float ,  6.4] ����                           StartPos 119, Length 6
    char    vega                [   6];    char    _vega                ;    // [float ,  6.4] ����                           StartPos 126, Length 6
    char    ceta                [   6];    char    _ceta                ;    // [float ,  6.4] ��Ÿ                           StartPos 133, Length 6
    char    rhox                [   6];    char    _rhox                ;    // [float ,  6.4] �ο�                           StartPos 140, Length 6
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] �̷а�                         StartPos 147, Length 6
    char    impv                [   6];    char    _impv                ;    // [float ,  6.2] ���簡ġ                       StartPos 154, Length 6
    char    timevl              [   6];    char    _timevl              ;    // [float ,  6.2] �ð���ġ                       StartPos 161, Length 6
    char    jvolume             [  12];    char    _jvolume             ;    // [long  ,   12] �ܰ����                       StartPos 168, Length 12
    char    parpl               [  12];    char    _parpl               ;    // [long  ,   12] �򰡼���                       StartPos 181, Length 12
    char    jngo                [   6];    char    _jngo                ;    // [long  ,    6] û�갡�ɼ���                   StartPos 194, Length 6
    char    offerrem1           [  12];    char    _offerrem1           ;    // [long  ,   12] �ŵ��ܷ�                       StartPos 201, Length 12
    char    bidrem1             [  12];    char    _bidrem1             ;    // [long  ,   12] �ż��ܷ�                       StartPos 214, Length 12
    char    open                [   6];    char    _open                ;    // [float ,  6.2] �ð�                           StartPos 227, Length 6
    char    high                [   6];    char    _high                ;    // [float ,  6.2] ��                           StartPos 234, Length 6
    char    low                 [   6];    char    _low                 ;    // [float ,  6.2] ����                           StartPos 241, Length 6
    char    atmgubun            [   1];    char    _atmgubun            ;    // [string,    1] ATM����                        StartPos 248, Length 1
    char    jisuconv            [   6];    char    _jisuconv            ;    // [float ,  6.2] ����ȯ��                       StartPos 250, Length 6
    char    value               [  12];    char    _value               ;    // [float ,   12] �ŷ����                       StartPos 257, Length 12
} t2301OutBlock1, *LPt2301OutBlock1;
#define NAME_t2301OutBlock1     "t2301OutBlock1"

// ���2, occurs
typedef struct _t2301OutBlock2
{
    char    actprice            [   6];    char    _actprice            ;    // [float ,  6.2] ��簡                         StartPos 0, Length 6
    char    optcode             [   8];    char    _optcode             ;    // [string,    8] ǲ�ɼ��ڵ�                     StartPos 7, Length 8
    char    price               [   6];    char    _price               ;    // [float ,  6.2] ���簡                         StartPos 16, Length 6
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 23, Length 1
    char    change              [   6];    char    _change              ;    // [float ,  6.2] ���ϴ��                       StartPos 25, Length 6
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 32, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 39, Length 12
    char    iv                  [   6];    char    _iv                  ;    // [float ,  6.2] IV                             StartPos 52, Length 6
    char    mgjv                [  12];    char    _mgjv                ;    // [long  ,   12] �̰�������                     StartPos 59, Length 12
    char    mgjvupdn            [  12];    char    _mgjvupdn            ;    // [long  ,   12] �̰�����������                 StartPos 72, Length 12
    char    offerho1            [   6];    char    _offerho1            ;    // [float ,  6.2] �ŵ�ȣ��                       StartPos 85, Length 6
    char    bidho1              [   6];    char    _bidho1              ;    // [float ,  6.2] �ż�ȣ��                       StartPos 92, Length 6
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] ü�ᷮ                         StartPos 99, Length 12
    char    delt                [   6];    char    _delt                ;    // [float ,  6.4] ��Ÿ                           StartPos 112, Length 6
    char    gama                [   6];    char    _gama                ;    // [float ,  6.4] ����                           StartPos 119, Length 6
    char    vega                [   6];    char    _vega                ;    // [float ,  6.4] ����                           StartPos 126, Length 6
    char    ceta                [   6];    char    _ceta                ;    // [float ,  6.4] ��Ÿ                           StartPos 133, Length 6
    char    rhox                [   6];    char    _rhox                ;    // [float ,  6.4] �ο�                           StartPos 140, Length 6
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] �̷а�                         StartPos 147, Length 6
    char    impv                [   6];    char    _impv                ;    // [float ,  6.2] ���簡ġ                       StartPos 154, Length 6
    char    timevl              [   6];    char    _timevl              ;    // [float ,  6.2] �ð���ġ                       StartPos 161, Length 6
    char    jvolume             [  12];    char    _jvolume             ;    // [long  ,   12] �ܰ����                       StartPos 168, Length 12
    char    parpl               [  12];    char    _parpl               ;    // [long  ,   12] �򰡼���                       StartPos 181, Length 12
    char    jngo                [   6];    char    _jngo                ;    // [long  ,    6] û�갡�ɼ���                   StartPos 194, Length 6
    char    offerrem1           [  12];    char    _offerrem1           ;    // [long  ,   12] �ŵ��ܷ�                       StartPos 201, Length 12
    char    bidrem1             [  12];    char    _bidrem1             ;    // [long  ,   12] �ż��ܷ�                       StartPos 214, Length 12
    char    open                [   6];    char    _open                ;    // [float ,  6.2] �ð�                           StartPos 227, Length 6
    char    high                [   6];    char    _high                ;    // [float ,  6.2] ��                           StartPos 234, Length 6
    char    low                 [   6];    char    _low                 ;    // [float ,  6.2] ����                           StartPos 241, Length 6
    char    atmgubun            [   1];    char    _atmgubun            ;    // [string,    1] ATM����                        StartPos 248, Length 1
    char    jisuconv            [   6];    char    _jisuconv            ;    // [float ,  6.2] ����ȯ��                       StartPos 250, Length 6
    char    value               [  12];    char    _value               ;    // [float ,   12] �ŷ����                       StartPos 257, Length 12
} t2301OutBlock2, *LPt2301OutBlock2;
#define NAME_t2301OutBlock2     "t2301OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
