#ifndef _TC1_H_
#define _TC1_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� �ֹ����� ( BLOCK, KEY=7, GROUP=1 )
#pragma pack( push, 1 )

#define NAME_TC1     "TC1"

// �Է�                           
typedef struct _TC1_InBlock
{
} TC1_InBlock, *LPTC1_InBlock;
#define NAME_TC1_InBlock     "InBlock"

// ���                           
typedef struct _TC1_OutBlock
{
    char    lineseq             [  10];    // [long  ,   10] �����Ϸù�ȣ                    StartPos 0, Length 10
    char    key                 [  11];    // [string,   11] KEY                             StartPos 10, Length 11
    char    user                [   8];    // [string,    8] ������ID                        StartPos 21, Length 8
    char    svc_id              [   4];    // [string,    4] ����ID                        StartPos 29, Length 4
    char    ordr_dt             [   8];    // [string,    8] �ֹ�����                        StartPos 33, Length 8
    char    brn_cd              [   3];    // [string,    3] ������ȣ                        StartPos 41, Length 3
    char    ordr_no             [  10];    // [long  ,   10] �ֹ���ȣ                        StartPos 44, Length 10
    char    orgn_ordr_no        [  10];    // [long  ,   10] ���ֹ���ȣ                      StartPos 54, Length 10
    char    mthr_ordr_no        [  10];    // [long  ,   10] ���ֹ���ȣ                      StartPos 64, Length 10
    char    ac_no               [  11];    // [string,   11] ���¹�ȣ                        StartPos 74, Length 11
    char    is_cd               [  30];    // [string,   30] �����ڵ�                        StartPos 85, Length 30
    char    s_b_ccd             [   1];    // [string,    1] �ŵ��ż�����                    StartPos 115, Length 1
    char    ordr_ccd            [   1];    // [string,    1] �����������                    StartPos 116, Length 1
    char    ordr_typ_cd         [   1];    // [string,    1] �ֹ������ڵ�                    StartPos 117, Length 1
    char    ordr_typ_prd_ccd    [   2];    // [string,    2] �ֹ��Ⱓ�ڵ�                    StartPos 118, Length 2
    char    ordr_aplc_strt_dt   [   8];    // [string,    8] �ֹ������������                StartPos 120, Length 8
    char    ordr_aplc_end_dt    [   8];    // [string,    8] �ֹ�������������                StartPos 128, Length 8
    char    ordr_prc            [  15];    // [double, 15.8] �ֹ�����                        StartPos 136, Length 15
    char    cndt_ordr_prc       [  15];    // [double, 15.8] �ֹ����ǰ���                    StartPos 151, Length 15
    char    ordr_q              [  12];    // [long  ,   12] �ֹ�����                        StartPos 166, Length 12
    char    ordr_tm             [   9];    // [string,    9] �ֹ��ð�                        StartPos 178, Length 9
    char    userid              [   8];    // [string,    8] �����ID                        StartPos 187, Length 8
} TC1_OutBlock, *LPTC1_OutBlock;
#define NAME_TC1_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _TC1_H_
