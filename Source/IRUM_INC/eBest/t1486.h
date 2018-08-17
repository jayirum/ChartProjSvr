///////////////////////////////////////////////////////////////////////////////////////////////////
// �ð�������ü�ᰡ(t1486) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1486     "t1486"

// �⺻�Է�
typedef struct _t1486InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] �ð�CTS                        StartPos 7, Length 10
    char    cnt                 [   4];    char    _cnt                 ;    // [string,    4] ��ȸ�Ǽ�                       StartPos 18, Length 4
} t1486InBlock, *LPt1486InBlock;
#define NAME_t1486InBlock     "t1486InBlock"

// ���
typedef struct _t1486OutBlock
{
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] �ð�CTS                        StartPos 0, Length 10
} t1486OutBlock, *LPt1486OutBlock;
#define NAME_t1486OutBlock     "t1486OutBlock"

// ���1, occurs
typedef struct _t1486OutBlock1
{
    char    chetime             [   8];    char    _chetime             ;    // [string,    8] �ð�                           StartPos 0, Length 8
    char    price               [   8];    char    _price               ;    // [long  ,    8] ����ü�ᰡ                     StartPos 9, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 18, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                       StartPos 20, Length 8
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 29, Length 6
    char    cvolume             [  12];    char    _cvolume             ;    // [long  ,   12] ����ü�ᷮ                     StartPos 36, Length 12
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] �ŵ�ȣ��                       StartPos 49, Length 8
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] �ż�ȣ��                       StartPos 58, Length 8
    char    offerrem1           [  12];    char    _offerrem1           ;    // [long  ,   12] �ŵ��ܷ�                       StartPos 67, Length 12
    char    bidrem1             [  12];    char    _bidrem1             ;    // [long  ,   12] �ż��ܷ�                       StartPos 80, Length 12
} t1486OutBlock1, *LPt1486OutBlock1;
#define NAME_t1486OutBlock1     "t1486OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
