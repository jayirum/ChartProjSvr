///////////////////////////////////////////////////////////////////////////////////////////////////
// ����/�ɼǹ̰����ܰ�(t0433) ( tuxcode=t0433,headtype=D )
#pragma pack( push, 1 )

#define NAME_t0433     "t0433"

// �⺻�Է�
typedef struct _t0433InBlock
{
    char    accno               [  11];                                      // [string,   11] ���¹�ȣ                       StartPos 0, Length 11
    char    passwd              [   8];                                      // [string,    8] ��й�ȣ                       StartPos 11, Length 8
    char    cts_expcode         [   8];                                      // [string,    8] CTS_�����ȣ                   StartPos 19, Length 8
    char    cts_medocd          [   1];                                      // [string,    1] CTS_�Ÿű���                   StartPos 27, Length 1
} t0433InBlock, *LPt0433InBlock;
#define NAME_t0433InBlock     "t0433InBlock"

// ���
typedef struct _t0433OutBlock
{
    char    tdtsunik            [  18];                                      // [long  ,   18] �Ÿż����հ�                   StartPos 0, Length 18
    char    tmamt               [  18];                                      // [long  ,   18] �̰��������հ�                 StartPos 18, Length 18
    char    cts_expcode         [   8];                                      // [string,    8] CTS_�����ȣ                   StartPos 36, Length 8
    char    cts_medocd          [   1];                                      // [string,    1] CTS_�Ÿű���                   StartPos 44, Length 1
} t0433OutBlock, *LPt0433OutBlock;
#define NAME_t0433OutBlock     "t0433OutBlock"

// ���1, occurs
typedef struct _t0433OutBlock1
{
    char    expcode             [   8];                                      // [string,    8] �����ȣ                       StartPos 0, Length 8
    char    medosu              [   4];                                      // [string,    4] ����                           StartPos 8, Length 4
    char    jqty                [  10];                                      // [long  ,   10] �ܰ����                       StartPos 12, Length 10
    char    pamt                [  10];                                      // [float , 10.2] ��մܰ�                       StartPos 22, Length 10
    char    medocd              [   1];                                      // [string,    1] �Ÿű���                       StartPos 33, Length 1
    char    mamt                [  18];                                      // [long  ,   18] �Ѹ��Աݾ�                     StartPos 34, Length 18
    char    sysprocseq          [  10];                                      // [long  ,   10] ó������                       StartPos 52, Length 10
} t0433OutBlock1, *LPt0433OutBlock1;
#define NAME_t0433OutBlock1     "t0433OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
