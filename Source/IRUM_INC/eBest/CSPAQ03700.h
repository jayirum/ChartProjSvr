#ifndef _CSPAQ03700_H_
#define _CSPAQ03700_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �������� �ֹ�ü�᳻�� ��ȸ ( ENCRYPT,SERVICE=CSPAQ03700,HEADTYPE=B,CREATOR=�̻���,CREDATE=2012/03/14 13:45:26 )
#pragma pack( push, 1 )

#define NAME_CSPAQ03700     "CSPAQ03700"

// In(*EMPTY*)                    
typedef struct _CSPAQ03700InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    InptPwd             [   8];    // [string,    8] �Էº�й�ȣ                    StartPos 25, Length 8
    char    OrdMktCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 33, Length 2
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 35, Length 1
    char    IsuNo               [  12];    // [string,   12] �����ȣ                        StartPos 36, Length 12
    char    ExecYn              [   1];    // [string,    1] ü�Ῡ��                        StartPos 48, Length 1
    char    OrdDt               [   8];    // [string,    8] �ֹ���                          StartPos 49, Length 8
    char    SrtOrdNo2           [  10];    // [long  ,   10] �����ֹ���ȣ2                   StartPos 57, Length 10
    char    BkseqTpCode         [   1];    // [string,    1] ��������                        StartPos 67, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 68, Length 2
} CSPAQ03700InBlock1, *LPCSPAQ03700InBlock1;
#define NAME_CSPAQ03700InBlock1     "CSPAQ03700InBlock1"

// In(*EMPTY*)                    
typedef struct _CSPAQ03700OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    InptPwd             [   8];    // [string,    8] �Էº�й�ȣ                    StartPos 25, Length 8
    char    OrdMktCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 33, Length 2
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 35, Length 1
    char    IsuNo               [  12];    // [string,   12] �����ȣ                        StartPos 36, Length 12
    char    ExecYn              [   1];    // [string,    1] ü�Ῡ��                        StartPos 48, Length 1
    char    OrdDt               [   8];    // [string,    8] �ֹ���                          StartPos 49, Length 8
    char    SrtOrdNo2           [  10];    // [long  ,   10] �����ֹ���ȣ2                   StartPos 57, Length 10
    char    BkseqTpCode         [   1];    // [string,    1] ��������                        StartPos 67, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 68, Length 2
} CSPAQ03700OutBlock1, *LPCSPAQ03700OutBlock1;
#define NAME_CSPAQ03700OutBlock1     "CSPAQ03700OutBlock1"

// OUT1(*EMPTY*)                  
typedef struct _CSPAQ03700OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    SellExecAmt         [  16];    // [long  ,   16] �ŵ�ü��ݾ�                    StartPos 5, Length 16
    char    BuyExecAmt          [  16];    // [long  ,   16] �ż�ü��ݾ�                    StartPos 21, Length 16
    char    SellExecQty         [  16];    // [long  ,   16] �ŵ�ü�����                    StartPos 37, Length 16
    char    BuyExecQty          [  16];    // [long  ,   16] �ż�ü�����                    StartPos 53, Length 16
    char    SellOrdQty          [  16];    // [long  ,   16] �ŵ��ֹ�����                    StartPos 69, Length 16
    char    BuyOrdQty           [  16];    // [long  ,   16] �ż��ֹ�����                    StartPos 85, Length 16
} CSPAQ03700OutBlock2, *LPCSPAQ03700OutBlock2;
#define NAME_CSPAQ03700OutBlock2     "CSPAQ03700OutBlock2"

// OUT(*EMPTY*)                   , occurs
typedef struct _CSPAQ03700OutBlock3
{
    char    OrdDt               [   8];    // [string,    8] �ֹ���                          StartPos 0, Length 8
    char    MgmtBrnNo           [   3];    // [string,    3] ����������ȣ                    StartPos 8, Length 3
    char    OrdMktCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 11, Length 2
    char    OrdNo               [  10];    // [long  ,   10] �ֹ���ȣ                        StartPos 13, Length 10
    char    OrgOrdNo            [  10];    // [long  ,   10] ���ֹ���ȣ                      StartPos 23, Length 10
    char    IsuNo               [  12];    // [string,   12] �����ȣ                        StartPos 33, Length 12
    char    IsuNm               [  40];    // [string,   40] �����                          StartPos 45, Length 40
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 85, Length 1
    char    BnsTpNm             [  10];    // [string,   10] �Ÿű��и�                      StartPos 86, Length 10
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 96, Length 2
    char    OrdPtnNm            [  40];    // [string,   40] �ֹ�������                      StartPos 98, Length 40
    char    OrdTrxPtnCode       [   9];    // [long  ,    9] �ֹ�ó�������ڵ�                StartPos 138, Length 9
    char    OrdTrxPtnNm         [  50];    // [string,   50] �ֹ�ó��������                  StartPos 147, Length 50
    char    MrcTpCode           [   1];    // [string,    1] ������ұ���                    StartPos 197, Length 1
    char    MrcTpNm             [  10];    // [string,   10] ������ұ��и�                  StartPos 198, Length 10
    char    MrcQty              [  16];    // [long  ,   16] ������Ҽ���                    StartPos 208, Length 16
    char    MrcAbleQty          [  16];    // [long  ,   16] ������Ұ��ɼ���                StartPos 224, Length 16
    char    OrdQty              [  16];    // [long  ,   16] �ֹ�����                        StartPos 240, Length 16
    char    OrdPrc              [  15];    // [double, 15.2] �ֹ�����                        StartPos 256, Length 15
    char    ExecQty             [  16];    // [long  ,   16] ü�����                        StartPos 271, Length 16
    char    ExecPrc             [  15];    // [double, 15.2] ü�ᰡ                          StartPos 287, Length 15
    char    ExecTrxTime         [   9];    // [string,    9] ü��ó���ð�                    StartPos 302, Length 9
    char    LastExecTime        [   9];    // [string,    9] ����ü��ð�                    StartPos 311, Length 9
    char    OrdprcPtnCode       [   2];    // [string,    2] ȣ�������ڵ�                    StartPos 320, Length 2
    char    OrdprcPtnNm         [  40];    // [string,   40] ȣ��������                      StartPos 322, Length 40
    char    OrdCndiTpCode       [   1];    // [string,    1] �ֹ����Ǳ���                    StartPos 362, Length 1
    char    AllExecQty          [  16];    // [long  ,   16] ��üü�����                    StartPos 363, Length 16
    char    RegCommdaCode       [   2];    // [string,    2] ��Ÿ�ü�ڵ�                    StartPos 379, Length 2
    char    CommdaNm            [  40];    // [string,   40] ��Ÿ�ü��                      StartPos 381, Length 40
    char    MbrNo               [   3];    // [string,    3] ȸ����ȣ                        StartPos 421, Length 3
    char    RsvOrdYn            [   1];    // [string,    1] �����ֹ�����                    StartPos 424, Length 1
    char    LoanDt              [   8];    // [string,    8] ������                          StartPos 425, Length 8
    char    OrdTime             [   9];    // [string,    9] �ֹ��ð�                        StartPos 433, Length 9
    char    OpDrtnNo            [  12];    // [string,   12] ������ù�ȣ                    StartPos 442, Length 12
    char    OdrrId              [  16];    // [string,   16] �ֹ���ID                        StartPos 454, Length 16
} CSPAQ03700OutBlock3, *LPCSPAQ03700OutBlock3;
#define NAME_CSPAQ03700OutBlock3     "CSPAQ03700OutBlock3"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CSPAQ03700_H_
