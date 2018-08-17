///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ɼǽð��뺰ü����ȸ(t2201) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t2201     "t2201"

// �⺻�Է�
typedef struct _t2201InBlock
{
    char    focode              [   8];    char    _focode              ;    // [string,    8] �����ڵ�                       StartPos 0, Length 8
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] Ư�̰ŷ���                     StartPos 9, Length 12
    char    stime               [   4];    char    _stime               ;    // [string,    4] ���۽ð�                       StartPos 22, Length 4
    char    etime               [   4];    char    _etime               ;    // [string,    4] ����ð�                       StartPos 27, Length 4
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] �ð�CTS                        StartPos 32, Length 10
} t2201InBlock, *LPt2201InBlock;
#define NAME_t2201InBlock     "t2201InBlock"

// ���
typedef struct _t2201OutBlock
{
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] �ð�CTS                        StartPos 0, Length 10
} t2201OutBlock, *LPt2201OutBlock;
#define NAME_t2201OutBlock     "t2201OutBlock"

// ���1, occurs
typedef struct _t2201OutBlock1
{
    char    chetime             [  10];    char    _chetime             ;    // [string,   10] �ð�                           StartPos 0, Length 10
    char    price               [   6];    char    _price               ;    // [float ,  6.2] ���簡                         StartPos 11, Length 6
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 18, Length 1
    char    change              [   6];    char    _change              ;    // [float ,  6.2] ���ϴ��                       StartPos 20, Length 6
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ü�����                       StartPos 27, Length 8
    char    chdegree            [   8];    char    _chdegree            ;    // [float ,  8.2] ü�ᰭ��                       StartPos 36, Length 8
    char    offerho             [   6];    char    _offerho             ;    // [float ,  6.2] �ŵ�ȣ��                       StartPos 45, Length 6
    char    bidho               [   6];    char    _bidho               ;    // [float ,  6.2] �ż�ȣ��                       StartPos 52, Length 6
    char    volume              [  12];    char    _volume              ;    // [double,   12] �ŷ���                         StartPos 59, Length 12
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] �̰����                       StartPos 72, Length 8
    char    jnilopenupdn        [   8];    char    _jnilopenupdn        ;    // [long  ,    8] �̰���������                   StartPos 81, Length 8
    char    ibasis              [   6];    char    _ibasis              ;    // [float ,  6.2] �̷�BASIS                      StartPos 90, Length 6
    char    sbasis              [   6];    char    _sbasis              ;    // [float ,  6.2] ����BASIS                      StartPos 97, Length 6
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] ������                         StartPos 104, Length 6
    char    value               [  12];    char    _value               ;    // [double,   12] �ŷ����                       StartPos 111, Length 12
    char    j_openupdn          [   8];    char    _j_openupdn          ;    // [long  ,    8] �̰���������                   StartPos 124, Length 8
    char    n_msvolume          [  12];    char    _n_msvolume          ;    // [double,   12] �����ż�ü�ᷮ                 StartPos 133, Length 12
    char    n_mdvolume          [  12];    char    _n_mdvolume          ;    // [double,   12] �����ŵ�ü�ᷮ                 StartPos 146, Length 12
    char    s_msvolume          [  12];    char    _s_msvolume          ;    // [double,   12] �������ż�ü�ᷮ               StartPos 159, Length 12
    char    n_mschecnt          [   8];    char    _n_mschecnt          ;    // [long  ,    8] �����ż�ü��Ǽ�               StartPos 172, Length 8
    char    n_mdchecnt          [   8];    char    _n_mdchecnt          ;    // [long  ,    8] �����ŵ�ü��Ǽ�               StartPos 181, Length 8
    char    s_mschecnt          [   8];    char    _s_mschecnt          ;    // [long  ,    8] �������ż�ü��Ǽ�             StartPos 190, Length 8
} t2201OutBlock1, *LPt2201OutBlock1;
#define NAME_t2201OutBlock1     "t2201OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
