#ifndef _TC3_H_
#define _TC3_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� �ֹ�ü�� ( BLOCK, KEY=7, GROUP=1 )
#pragma pack( push, 1 )

#define NAME_TC3     "TC3"

// �Է�                           
typedef struct _TC3_InBlock
{
} TC3_InBlock, *LPTC3_InBlock;
#define NAME_TC3_InBlock     "InBlock"

// ���                           
typedef struct _TC3_OutBlock
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
    char    ccls_q              [  15];    // [long  ,   15] ü�����                        StartPos 117, Length 15
    char    ccls_prc            [  15];    // [double, 15.8] ü�ᰡ��                        StartPos 132, Length 15
    char    ccls_no             [  10];    // [string,   10] ü���ȣ                        StartPos 147, Length 10
    char    ccls_tm             [   9];    // [string,    9] ü��ð�                        StartPos 157, Length 9
    char    avg_byng_uprc       [  12];    // [double, 12.6] ������մܰ�                    StartPos 166, Length 12
    char    byug_amt            [  25];    // [double, 25.8] ���Աݾ�                        StartPos 178, Length 25
    char    clr_pl_amt          [  19];    // [double, 19.2] û�����                        StartPos 203, Length 19
    char    ent_fee             [  19];    // [double, 19.2] ��Ź������                      StartPos 222, Length 19
    char    fcm_fee             [  19];    // [double, 19.2] FCM������                       StartPos 241, Length 19
    char    userid              [   8];    // [string,    8] �����ID                        StartPos 260, Length 8
    char    now_prc             [  15];    // [double, 15.8] ���簡��                        StartPos 268, Length 15
    char    crncy_cd            [   3];    // [string,    3] ��ȭ�ڵ�                        StartPos 283, Length 3
    char    mtrt_dt             [   8];    // [string,    8] ��������                        StartPos 286, Length 8
} TC3_OutBlock, *LPTC3_OutBlock;
#define NAME_TC3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _TC3_H_
