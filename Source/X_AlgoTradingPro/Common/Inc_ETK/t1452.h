///////////////////////////////////////////////////////////////////////////////////////////////////
// �ŷ�������(t1452) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1452     "t1452"

// �⺻�Է�
typedef struct _t1452InBlock
{
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ����                           StartPos 0, Length 1
    char    jnilgubun           [   1];    char    _jnilgubun           ;    // [string,    1] ���ϱ���                       StartPos 2, Length 1
    char    sdiff               [   3];    char    _sdiff               ;    // [long  ,    3] ���۵����                     StartPos 4, Length 3
    char    ediff               [   3];    char    _ediff               ;    // [long  ,    3] ��������                     StartPos 8, Length 3
    char    jc_num              [  12];    char    _jc_num              ;    // [long  ,   12] �������                       StartPos 12, Length 12
    char    sprice              [   8];    char    _sprice              ;    // [long  ,    8] ���۰���                       StartPos 25, Length 8
    char    eprice              [   8];    char    _eprice              ;    // [long  ,    8] ���ᰡ��                       StartPos 34, Length 8
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 43, Length 12
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 56, Length 4
} t1452InBlock, *LPt1452InBlock;
#define NAME_t1452InBlock     "t1452InBlock"

// ���
typedef struct _t1452OutBlock
{
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 0, Length 4
} t1452OutBlock, *LPt1452OutBlock;
#define NAME_t1452OutBlock     "t1452OutBlock"

// ���1, occurs
typedef struct _t1452OutBlock1
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �����                         StartPos 0, Length 20
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 21, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 30, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 32, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 41, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 48, Length 12
    char    vol                 [   6];    char    _vol                 ;    // [float ,  6.2] ȸ����                         StartPos 61, Length 6
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϰŷ���                     StartPos 68, Length 12
    char    bef_diff            [  10];    char    _bef_diff            ;    // [float , 10.2] ���Ϻ�                         StartPos 81, Length 10
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 92, Length 6
} t1452OutBlock1, *LPt1452OutBlock1;
#define NAME_t1452OutBlock1     "t1452OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
