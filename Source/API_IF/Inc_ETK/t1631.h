///////////////////////////////////////////////////////////////////////////////////////////////////
// ���α׷��Ÿ�������ȸ(t1631) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1631     "t1631"

// �⺻�Է�
typedef struct _t1631InBlock
{
    char    gubun               [   1];    char    _gubun               ;    // [string,    1] ����                           StartPos 0, Length 1
    char    dgubun              [   1];    char    _dgubun              ;    // [string,    1] ���ڱ���                       StartPos 2, Length 1
    char    sdate               [   8];    char    _sdate               ;    // [string,    8] ��������                       StartPos 4, Length 8
    char    edate               [   8];    char    _edate               ;    // [string,    8] ��������                       StartPos 13, Length 8
} t1631InBlock, *LPt1631InBlock;
#define NAME_t1631InBlock     "t1631InBlock"

// ���
typedef struct _t1631OutBlock
{
    char    cdhrem              [   8];    char    _cdhrem              ;    // [long  ,    8] �ŵ����͹�ü���ܷ�             StartPos 0, Length 8
    char    bdhrem              [   8];    char    _bdhrem              ;    // [long  ,    8] �ŵ������͹�ü���ܷ�           StartPos 9, Length 8
    char    tcdrem              [   8];    char    _tcdrem              ;    // [long  ,    8] �ŵ������ֹ�����               StartPos 18, Length 8
    char    tbdrem              [   8];    char    _tbdrem              ;    // [long  ,    8] �ŵ��������ֹ�����             StartPos 27, Length 8
    char    cshrem              [   8];    char    _cshrem              ;    // [long  ,    8] �ż����͹�ü���ܷ�             StartPos 36, Length 8
    char    bshrem              [   8];    char    _bshrem              ;    // [long  ,    8] �ż������͹�ü���ܷ�           StartPos 45, Length 8
    char    tcsrem              [   8];    char    _tcsrem              ;    // [long  ,    8] �ż������ֹ�����               StartPos 54, Length 8
    char    tbsrem              [   8];    char    _tbsrem              ;    // [long  ,    8] �ż��������ֹ�����             StartPos 63, Length 8
} t1631OutBlock, *LPt1631OutBlock;
#define NAME_t1631OutBlock     "t1631OutBlock"

// ���1, occurs
typedef struct _t1631OutBlock1
{
    char    offervolume         [   8];    char    _offervolume         ;    // [long  ,    8] �ŵ�����                       StartPos 0, Length 8
    char    offervalue          [  12];    char    _offervalue          ;    // [long  ,   12] �ŵ��ݾ�                       StartPos 9, Length 12
    char    bidvolume           [   8];    char    _bidvolume           ;    // [long  ,    8] �ż�����                       StartPos 22, Length 8
    char    bidvalue            [  12];    char    _bidvalue            ;    // [long  ,   12] �ż��ݾ�                       StartPos 31, Length 12
    char    volume              [   8];    char    _volume              ;    // [long  ,    8] ���ż�����                     StartPos 44, Length 8
    char    value               [  12];    char    _value               ;    // [long  ,   12] ���ż��ݾ�                     StartPos 53, Length 12
} t1631OutBlock1, *LPt1631OutBlock1;
#define NAME_t1631OutBlock1     "t1631OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
