#ifndef		_HO1_H_20100809_
#define		_HO1_H_20100809_


///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ֹ�������� ( block )
#pragma pack( push, 1 )

#define NAME_H01     "H01"

// �Է�
typedef struct _H01_InBlock
{
} H01_InBlock, *LPH01_InBlock;
#define NAME_H01_InBlock     "InBlock"

// ���
typedef struct _H01_OutBlock
{
    char    lineseq             [  10];    // [long  ,   10] �����Ϸù�ȣ                   StartPos 0, Length 10
    char    accno               [  11];    // [string,   11] ���¹�ȣ                       StartPos 10, Length 11
    char    user                [   8];    // [string,    8] ������ID                       StartPos 21, Length 8
    char    seq                 [  11];    // [long  ,   11] �Ϸù�ȣ                       StartPos 29, Length 11
    char    trcode              [  11];    // [string,   11] trcode                         StartPos 40, Length 11
    char    typecode            [   1];    // [string,    1] ���Խð��ܱ���                 StartPos 51, Length 1
    char    memberno            [   5];    // [string,    5] ȸ����ȣ                       StartPos 52, Length 5
    char    bpno                [   5];    // [string,    5] ������ȣ                       StartPos 57, Length 5
    char    ordno               [  10];    // [string,   10] �ֹ���ȣ                       StartPos 62, Length 10
    char    orgordno            [  10];    // [string,   10] ���ֹ���ȣ                     StartPos 72, Length 10
    char    expcode             [  12];    // [string,   12] �����ڵ�                       StartPos 82, Length 12
    char    dosugb              [   1];    // [string,    1] �ŵ�������                     StartPos 94, Length 1
    char    mocagb              [   1];    // [string,    1] ������ұ���                   StartPos 95, Length 1
    char    accno1              [  12];    // [string,   12] ���¹�ȣ1                      StartPos 96, Length 12
    char    qty2                [  10];    // [long  ,   10] ȣ������                       StartPos 108, Length 10
    char    price               [  11];    // [float , 11.2] ȣ������                       StartPos 118, Length 11
    char    ordgb               [   1];    // [string,    1] �ֹ�����                       StartPos 129, Length 1
    char    hogagb              [   1];    // [string,    1] ȣ������                       StartPos 130, Length 1
    char    sihogagb            [  11];    // [string,   11] ��������ȣ������               StartPos 131, Length 11
    char    tradid              [   5];    // [string,    5] �ڻ��ֽŰ�ID                 StartPos 142, Length 5
    char    treacode            [   1];    // [string,    1] �ڻ��ָŸŹ��                 StartPos 147, Length 1
    char    askcode             [   2];    // [string,    2] �ŵ������ڵ�                   StartPos 148, Length 2
    char    creditcode          [   2];    // [string,    2] �ſ뱸���ڵ�                   StartPos 150, Length 2
    char    jakigb              [   2];    // [string,    2] ��Ź�ڱⱸ��                   StartPos 152, Length 2
    char    trustnum            [   5];    // [string,    5] ��Ź���ȣ                     StartPos 154, Length 5
    char    ptgb                [   2];    // [string,    2] ���α׷�����                   StartPos 159, Length 2
    char    substonum           [  12];    // [string,   12] ����ֱǰ��¹�ȣ               StartPos 161, Length 12
    char    accgb               [   2];    // [string,    2] ���±����ڵ�                   StartPos 173, Length 2
    char    accmarggb           [   2];    // [string,    2] �������ű��ڵ�                 StartPos 175, Length 2
    char    nationcode          [   3];    // [string,    3] �����ڵ�                       StartPos 177, Length 3
    char    investgb            [   4];    // [string,    4] �����ڱ���                     StartPos 180, Length 4
    char    forecode            [   2];    // [string,    2] �ܱ����ڵ�                     StartPos 184, Length 2
    char    medcode             [   1];    // [string,    1] �ֹ���ü����                   StartPos 186, Length 1
    char    ordid               [  12];    // [string,   12] �ֹ��ĺ��ڹ�ȣ                 StartPos 187, Length 12
    char    orddate             [   8];    // [string,    8] ȣ������                       StartPos 199, Length 8
    char    rcvtime             [   9];    // [string,    9] ȸ�����ֹ��ð�                 StartPos 207, Length 9
    char    mem_filler          [   6];    // [string,    6] mem_filler                     StartPos 216, Length 6
    char    mem_accno           [  11];    // [string,   11] mem_accno                      StartPos 222, Length 11
    char    mem_filler1         [  23];    // [string,   23] mem_filler1                    StartPos 233, Length 23
    char    qty                 [  10];    // [long  ,   10] ��������Ҽ���                 StartPos 256, Length 10
    char    autogb              [   1];    // [string,    1] �ڵ���ұ���                   StartPos 266, Length 1
    char    rejcode             [   4];    // [string,    4] �źλ���                       StartPos 267, Length 4
    char    filler              [  58];    // [string,   58] �ʷ�                           StartPos 271, Length 58
} H01_OutBlock, *LPH01_OutBlock;
#define NAME_H01_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif