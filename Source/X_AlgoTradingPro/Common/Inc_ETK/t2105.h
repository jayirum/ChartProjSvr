///////////////////////////////////////////////////////////////////////////////////////////////////
// ����/�ɼ����簡ȣ����ȸ(t2105) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t2105     "t2105"

// �⺻�Է�
typedef struct _t2105InBlock
{
    char    shcode              [   8];    char    _shcode              ;    // [string,    8] �����ڵ�                       StartPos 0, Length 8
} t2105InBlock, *LPt2105InBlock;
#define NAME_t2105InBlock     "t2105InBlock"

// ���
typedef struct _t2105OutBlock
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �����                         StartPos 0, Length 20
    char    price               [   6];    char    _price               ;    // [float ,  6.2] ���簡                         StartPos 21, Length 6
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                   StartPos 28, Length 1
    char    change              [   6];    char    _change              ;    // [float ,  6.2] ���ϴ��                       StartPos 30, Length 6
    char    diff                [   6];    char    _diff                ;    // [float ,  6.2] �����                         StartPos 37, Length 6
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �ŷ���                         StartPos 44, Length 12
    char    stimeqrt            [   6];    char    _stimeqrt            ;    // [float ,  6.2] �ŷ������ϵ��ð�����           StartPos 57, Length 6
    char    jnilclose           [   6];    char    _jnilclose           ;    // [float ,  6.2] ��������                       StartPos 64, Length 6
    char    offerho1            [   6];    char    _offerho1            ;    // [float ,  6.2] �ŵ�ȣ��1                      StartPos 71, Length 6
    char    bidho1              [   6];    char    _bidho1              ;    // [float ,  6.2] �ż�ȣ��1                      StartPos 78, Length 6
    char    offerrem1           [   8];    char    _offerrem1           ;    // [long  ,    8] �ŵ�ȣ������1                  StartPos 85, Length 8
    char    bidrem1             [   8];    char    _bidrem1             ;    // [long  ,    8] �ż�ȣ������1                  StartPos 94, Length 8
    char    dcnt1               [   8];    char    _dcnt1               ;    // [long  ,    8] �ŵ�ȣ���Ǽ�1                  StartPos 103, Length 8
    char    scnt1               [   8];    char    _scnt1               ;    // [long  ,    8] �ż�ȣ���Ǽ�1                  StartPos 112, Length 8
    char    offerho2            [   6];    char    _offerho2            ;    // [float ,  6.2] �ŵ�ȣ��2                      StartPos 121, Length 6
    char    bidho2              [   6];    char    _bidho2              ;    // [float ,  6.2] �ż�ȣ��2                      StartPos 128, Length 6
    char    offerrem2           [   8];    char    _offerrem2           ;    // [long  ,    8] �ŵ�ȣ������2                  StartPos 135, Length 8
    char    bidrem2             [   8];    char    _bidrem2             ;    // [long  ,    8] �ż�ȣ������2                  StartPos 144, Length 8
    char    dcnt2               [   8];    char    _dcnt2               ;    // [long  ,    8] �ŵ�ȣ���Ǽ�2                  StartPos 153, Length 8
    char    scnt2               [   8];    char    _scnt2               ;    // [long  ,    8] �ż�ȣ���Ǽ�2                  StartPos 162, Length 8
    char    offerho3            [   6];    char    _offerho3            ;    // [float ,  6.2] �ŵ�ȣ��3                      StartPos 171, Length 6
    char    bidho3              [   6];    char    _bidho3              ;    // [float ,  6.2] �ż�ȣ��3                      StartPos 178, Length 6
    char    offerrem3           [   8];    char    _offerrem3           ;    // [long  ,    8] �ŵ�ȣ������3                  StartPos 185, Length 8
    char    bidrem3             [   8];    char    _bidrem3             ;    // [long  ,    8] �ż�ȣ������3                  StartPos 194, Length 8
    char    dcnt3               [   8];    char    _dcnt3               ;    // [long  ,    8] �ŵ�ȣ���Ǽ�3                  StartPos 203, Length 8
    char    scnt3               [   8];    char    _scnt3               ;    // [long  ,    8] �ż�ȣ���Ǽ�3                  StartPos 212, Length 8
    char    offerho4            [   6];    char    _offerho4            ;    // [float ,  6.2] �ŵ�ȣ��4                      StartPos 221, Length 6
    char    bidho4              [   6];    char    _bidho4              ;    // [float ,  6.2] �ż�ȣ��4                      StartPos 228, Length 6
    char    offerrem4           [   8];    char    _offerrem4           ;    // [long  ,    8] �ŵ�ȣ������4                  StartPos 235, Length 8
    char    bidrem4             [   8];    char    _bidrem4             ;    // [long  ,    8] �ż�ȣ������4                  StartPos 244, Length 8
    char    dcnt4               [   8];    char    _dcnt4               ;    // [long  ,    8] �ŵ�ȣ���Ǽ�4                  StartPos 253, Length 8
    char    scnt4               [   8];    char    _scnt4               ;    // [long  ,    8] �ż�ȣ���Ǽ�4                  StartPos 262, Length 8
    char    offerho5            [   6];    char    _offerho5            ;    // [float ,  6.2] �ŵ�ȣ��5                      StartPos 271, Length 6
    char    bidho5              [   6];    char    _bidho5              ;    // [float ,  6.2] �ż�ȣ��5                      StartPos 278, Length 6
    char    offerrem5           [   8];    char    _offerrem5           ;    // [long  ,    8] �ŵ�ȣ������5                  StartPos 285, Length 8
    char    bidrem5             [   8];    char    _bidrem5             ;    // [long  ,    8] �ż�ȣ������5                  StartPos 294, Length 8
    char    dcnt5               [   8];    char    _dcnt5               ;    // [long  ,    8] �ŵ�ȣ���Ǽ�5                  StartPos 303, Length 8
    char    scnt5               [   8];    char    _scnt5               ;    // [long  ,    8] �ż�ȣ���Ǽ�5                  StartPos 312, Length 8
    char    dvol                [   8];    char    _dvol                ;    // [long  ,    8] �ŵ�ȣ���Ѽ���                 StartPos 321, Length 8
    char    svol                [   8];    char    _svol                ;    // [long  ,    8] �ż�ȣ���Ѽ���                 StartPos 330, Length 8
    char    toffernum           [   8];    char    _toffernum           ;    // [long  ,    8] �Ѹŵ�ȣ���Ǽ�                 StartPos 339, Length 8
    char    tbidnum             [   8];    char    _tbidnum             ;    // [long  ,    8] �Ѹż�ȣ���Ǽ�                 StartPos 348, Length 8
    char    time                [   6];    char    _time                ;    // [string,    6] ���Žð�                       StartPos 357, Length 6
    char    shcode              [   8];    char    _shcode              ;    // [string,    8] �����ڵ�                       StartPos 364, Length 8
} t2105OutBlock, *LPt2105OutBlock;
#define NAME_t2105OutBlock     "t2105OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
