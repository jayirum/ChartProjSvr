///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ü�ᰡ�����������ȸ(t1488) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1488     "t1488"

// �⺻�Է�
typedef struct _t1488InBlock
{
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] �ŷ��ұ���                     StartPos 0, Length 1
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���϶�����                     StartPos 2, Length 1
    char    jgubun              [   1];    char    _jgubun              ;    // [string,    1] �屸��                         StartPos 4, Length 1
    char    jongchk             [  12];    char    _jongchk             ;    // [string,   12] ����üũ                       StartPos 6, Length 12
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 19, Length 4
    char    volume              [   1];    char    _volume              ;    // [string,    1] �ŷ���                         StartPos 24, Length 1
    char    yesprice            [   8];    char    _yesprice            ;    // [long  ,    8] ����ü����۰���               StartPos 26, Length 8
    char    yeeprice            [   8];    char    _yeeprice            ;    // [long  ,    8] ����ü�����ᰡ��               StartPos 35, Length 8
    char    yevolume            [  12];    char    _yevolume            ;    // [long  ,   12] ����ü�ᷮ                     StartPos 44, Length 12
} t1488InBlock, *LPt1488InBlock;
#define NAME_t1488InBlock     "t1488InBlock"

// ���
typedef struct _t1488OutBlock
{
    char    idx                 [   4];    char    _idx                 ;    // [long  ,    4] IDX                            StartPos 0, Length 4
} t1488OutBlock, *LPt1488OutBlock;
#define NAME_t1488OutBlock     "t1488OutBlock"

// ���1, occurs
typedef struct _t1488OutBlock1
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �ѱ۸�                         StartPos 0, Length 20
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                         StartPos 21, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 30, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 32, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 41, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                     StartPos 48, Length 12
    char    offerrem            [   8];    char    _offerrem            ;    // [long  ,    8] �ŵ��ܷ�                       StartPos 61, Length 8
    char    offerho             [   8];    char    _offerho             ;    // [long  ,    8] �ŵ�ȣ��                       StartPos 70, Length 8
    char    bidho               [   8];    char    _bidho               ;    // [long  ,    8] �ż�ȣ��                       StartPos 79, Length 8
    char    bidrem              [   8];    char    _bidrem              ;    // [long  ,    8] �ż��ܷ�                       StartPos 88, Length 8
    char    cnt                 [   4];    char    _cnt                 ;    // [long  ,    4] �����ϼ�                       StartPos 97, Length 4
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 102, Length 6
    char    jkrate              [   3];    char    _jkrate              ;    // [string,    3] ���ű���                       StartPos 109, Length 3
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϰŷ���                     StartPos 113, Length 12
} t1488OutBlock1, *LPt1488OutBlock1;
#define NAME_t1488OutBlock1     "t1488OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
