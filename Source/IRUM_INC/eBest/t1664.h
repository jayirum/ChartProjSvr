///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ڸŸ�����(íƮ) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1664     "t1664"

// �⺻�Է�
typedef struct _t1664InBlock
{
    char    mgubun              [   1];    char    _mgubun              ;    // [string,    1] ���屸��                       StartPos 0, Length 1
    char    vagubun             [   1];    char    _vagubun             ;    // [string,    1] �ݾ׼�������                   StartPos 2, Length 1
    char    bdgubun             [   1];    char    _bdgubun             ;    // [string,    1] �ð��Ϻ�����                   StartPos 4, Length 1
    char    cnt                 [   3];    char    _cnt                 ;    // [string,    3] ��ȸ�Ǽ�                       StartPos 6, Length 3
} t1664InBlock, *LPt1664InBlock;
#define NAME_t1664InBlock     "t1664InBlock"

// ���1, occurs
typedef struct _t1664OutBlock1
{
    char    dt                  [   8];    char    _dt                  ;    // [string,    8] ���ڽð�                       StartPos 0, Length 8
    char    tjj01               [  12];    char    _tjj01               ;    // [double,   12] ���Ǽ��ż�                     StartPos 9, Length 12
    char    tjj02               [  12];    char    _tjj02               ;    // [double,   12] ������ż�                     StartPos 22, Length 12
    char    tjj03               [  12];    char    _tjj03               ;    // [double,   12] ���ż��ż�                     StartPos 35, Length 12
    char    tjj04               [  12];    char    _tjj04               ;    // [double,   12] ������ż�                     StartPos 48, Length 12
    char    tjj05               [  12];    char    _tjj05               ;    // [double,   12] ���ݼ��ż�                     StartPos 61, Length 12
    char    tjj06               [  12];    char    _tjj06               ;    // [double,   12] ��ݼ��ż�                     StartPos 74, Length 12
    char    tjj07               [  12];    char    _tjj07               ;    // [double,   12] ��Ÿ���ż�                     StartPos 87, Length 12
    char    tjj08               [  12];    char    _tjj08               ;    // [double,   12] ���μ��ż�                     StartPos 100, Length 12
    char    tjj17               [  12];    char    _tjj17               ;    // [double,   12] �ܱ��μ��ż�                   StartPos 113, Length 12
    char    tjj18               [  12];    char    _tjj18               ;    // [double,   12] ������ż�                     StartPos 126, Length 12
    char    cha                 [  12];    char    _cha                 ;    // [double,   12] ���ͼ��ż�                     StartPos 139, Length 12
    char    bicha               [  12];    char    _bicha               ;    // [double,   12] �����ͼ��ż�                   StartPos 152, Length 12
    char    totcha              [  12];    char    _totcha              ;    // [double,   12] ���ռ��ż�                     StartPos 165, Length 12
    char    basis               [   6];    char    _basis               ;    // [float ,  6.2] ���̽ý�                       StartPos 178, Length 6
} t1664OutBlock1, *LPt1664OutBlock1;
#define NAME_t1664OutBlock1     "t1664OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
