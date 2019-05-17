#ifndef		_CO1_H_20100809_
#define		_CO1_H_20100809_


///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ֹ�ü�� ( block )
#pragma pack( push, 1 )

#define NAME_C01     "C01"

// �Է�
typedef struct _C01_InBlock
{
} C01_InBlock, *LPC01_InBlock;
#define NAME_C01_InBlock     "InBlock"

// ���
typedef struct _C01_OutBlock
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
    char    yakseq              [  11];    // [string,   11] ������ȣ                       StartPos 94, Length 11
    char    cheprice            [  11];    // [float , 11.2] ü�ᰡ��                       StartPos 105, Length 11
    char    chevol              [  10];    // [long  ,   10] ü�����                       StartPos 116, Length 10
    char    ordgb               [   2];    // [string,    2] ü������                       StartPos 126, Length 2
    char    chedate             [   8];    // [string,    8] ü������                       StartPos 128, Length 8
    char    chetime             [   9];    // [string,    9] ü��ð�                       StartPos 136, Length 9
    char    spdprc1             [  11];    // [float , 11.2] �ֱٿ�ü�ᰡ��                 StartPos 145, Length 11
    char    spdprc2             [  11];    // [float , 11.2] ���ٿ�ü�ᰡ��                 StartPos 156, Length 11
    char    dosugb              [   1];    // [string,    1] �ŵ�������                     StartPos 167, Length 1
    char    accno1              [  12];    // [string,   12] ���¹�ȣ1                      StartPos 168, Length 12
    char    sihogagb            [  11];    // [string,   11] ��������ȣ������               StartPos 180, Length 11
    char    jakino              [   5];    // [string,    5] ��Ź���ȣ                     StartPos 191, Length 5
    char    daeyong             [  12];    // [string,   12] ����ֱǰ��¹�ȣ               StartPos 196, Length 12
    char    mem_filler          [   6];    // [string,    6] mem_filler                     StartPos 208, Length 6
    char    mem_accno           [  11];    // [string,   11] mem_accno                      StartPos 214, Length 11
    char    mem_filler1         [  23];    // [string,   23] mem_filler1                    StartPos 225, Length 23
    char    filler              [  81];    // [string,   81] Filler                         StartPos 248, Length 81
} C01_OutBlock, *LPC01_OutBlock;
#define NAME_C01_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif