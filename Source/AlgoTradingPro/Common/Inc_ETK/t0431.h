///////////////////////////////////////////////////////////////////////////////////////////////////
// ����/�ɼ��ܰ���(�ֹ��˾�)(t0431) ( attr,tuxcode=t0431,headtype=D )
#pragma pack( push, 1 )

#define NAME_t0431     "t0431"

// �⺻�Է�
typedef struct _t0431InBlock
{
    char    accno               [  11];    char    _accno               ;    // [string,   11] ���¹�ȣ                       StartPos 0, Length 11
    char    passwd              [   8];    char    _passwd              ;    // [string,    8] ��й�ȣ                       StartPos 12, Length 8
    char    cts_expcode         [   8];    char    _cts_expcode         ;    // [string,    8] CTS_�����ȣ                   StartPos 21, Length 8
    char    cts_medocd          [   1];    char    _cts_medocd          ;    // [string,    1] CTS_�Ÿű���                   StartPos 30, Length 1
} t0431InBlock, *LPt0431InBlock;
#define NAME_t0431InBlock     "t0431InBlock"

// ���
typedef struct _t0431OutBlock
{
    char    tdtsunik            [  18];    char    _tdtsunik            ;    // [long  ,   18] �Ÿż����հ�                   StartPos 0, Length 18
    char    cts_expcode         [   8];    char    _cts_expcode         ;    // [string,    8] CTS_�����ȣ                   StartPos 19, Length 8
    char    cts_medocd          [   1];    char    _cts_medocd          ;    // [string,    1] CTS_�Ÿű���                   StartPos 28, Length 1
    char    tappamt             [  18];    char    _tappamt             ;    // [long  ,   18] �򰡱ݾ�                       StartPos 30, Length 18
    char    tsunik              [  18];    char    _tsunik              ;    // [long  ,   18] �򰡼���                       StartPos 49, Length 18
} t0431OutBlock, *LPt0431OutBlock;
#define NAME_t0431OutBlock     "t0431OutBlock"

// ���1, occurs
typedef struct _t0431OutBlock1
{
    char    expcode             [   8];    char    _expcode             ;    // [string,    8] �����ȣ                       StartPos 0, Length 8
    char    medosu              [   4];    char    _medosu              ;    // [string,    4] ����                           StartPos 9, Length 4
    char    jqty                [  10];    char    _jqty                ;    // [long  ,   10] �ܰ����                       StartPos 14, Length 10
    char    cqty                [  10];    char    _cqty                ;    // [long  ,   10] û�갡�ɼ���                   StartPos 25, Length 10
    char    pamt                [  10];    char    _pamt                ;    // [float , 10.2] ��մܰ�                       StartPos 36, Length 10
    char    mamt                [  18];    char    _mamt                ;    // [long  ,   18] �Ѹ��Աݾ�                     StartPos 47, Length 18
    char    medocd              [   1];    char    _medocd              ;    // [string,    1] �Ÿű���                       StartPos 66, Length 1
    char    dtsunik             [  18];    char    _dtsunik             ;    // [long  ,   18] �Ÿż���                       StartPos 68, Length 18
    char    sysprocseq          [  10];    char    _sysprocseq          ;    // [long  ,   10] ó������                       StartPos 87, Length 10
    char    price               [   9];    char    _price               ;    // [float ,  9.2] ���簡                         StartPos 98, Length 9
    char    appamt              [  18];    char    _appamt              ;    // [long  ,   18] �򰡱ݾ�                       StartPos 108, Length 18
    char    dtsunik1            [  18];    char    _dtsunik1            ;    // [long  ,   18] �򰡼���                       StartPos 127, Length 18
    char    sunikrt             [  10];    char    _sunikrt             ;    // [float , 10.2] ������                         StartPos 146, Length 10
} t0431OutBlock1, *LPt0431OutBlock1;
#define NAME_t0431OutBlock1     "t0431OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
