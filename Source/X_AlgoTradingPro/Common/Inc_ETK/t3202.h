///////////////////////////////////////////////////////////////////////////////////////////////////
// ������������(t3202) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t3202     "t3202"

// �⺻�Է�
typedef struct _t3202InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 0, Length 6
    char    date                [   8];    char    _date                ;    // [string,    8] ��ȸ����                       StartPos 7, Length 8
} t3202InBlock, *LPt3202InBlock;
#define NAME_t3202InBlock     "t3202InBlock"

// ���, occurs
typedef struct _t3202OutBlock
{
    char    recdt               [   8];    char    _recdt               ;    // [string,    8] ������                         StartPos 0, Length 8
    char    tableid             [   6];    char    _tableid             ;    // [string,    6] ���̺���̵�                   StartPos 9, Length 6
    char    upgu                [   2];    char    _upgu                ;    // [string,    2] ��������                       StartPos 16, Length 2
    char    custno              [   5];    char    _custno              ;    // [string,    5] ����ü��ȣ                     StartPos 19, Length 5
    char    custnm              [  80];    char    _custnm              ;    // [string,   80] ����ȸ���                     StartPos 25, Length 80
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                       StartPos 106, Length 6
    char    upnm                [  20];    char    _upnm                ;    // [string,   20] ������                         StartPos 113, Length 20
} t3202OutBlock, *LPt3202OutBlock;
#define NAME_t3202OutBlock     "t3202OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
