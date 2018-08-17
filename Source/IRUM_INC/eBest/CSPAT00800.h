#ifndef _CSPAT00800_H_
#define _CSPAT00800_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ����ֹ� ( SERVICE=CSPAT00800,ENCRYPT,SIGNATURE,HEADTYPE=B,CREATOR=�����,CREDATE=2011/12/07 09:23:48 )
#pragma pack( push, 1 )

#define NAME_CSPAT00800     "CSPAT00800"

// In(*EMPTY*)                    
typedef struct _CSPAT00800InBlock1
{
    char    OrgOrdNo            [  10];    // [long  ,   10] ���ֹ���ȣ                      StartPos 0, Length 10
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 10, Length 20
    char    InptPwd             [   8];    // [string,    8] �Էº�й�ȣ                    StartPos 30, Length 8
    char    IsuNo               [  12];    // [string,   12] �����ȣ                        StartPos 38, Length 12
    char    OrdQty              [  16];    // [long  ,   16] �ֹ�����                        StartPos 50, Length 16
} CSPAT00800InBlock1, *LPCSPAT00800InBlock1;
#define NAME_CSPAT00800InBlock1     "CSPAT00800InBlock1"

// In(*EMPTY*)                    
typedef struct _CSPAT00800OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OrgOrdNo            [  10];    // [long  ,   10] ���ֹ���ȣ                      StartPos 5, Length 10
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 15, Length 20
    char    InptPwd             [   8];    // [string,    8] �Էº�й�ȣ                    StartPos 35, Length 8
    char    IsuNo               [  12];    // [string,   12] �����ȣ                        StartPos 43, Length 12
    char    OrdQty              [  16];    // [long  ,   16] �ֹ�����                        StartPos 55, Length 16
    char    CommdaCode          [   2];    // [string,    2] ��Ÿ�ü�ڵ�                    StartPos 71, Length 2
    char    GrpId               [  20];    // [string,   20] �׷�ID                          StartPos 73, Length 20
    char    StrtgCode           [   6];    // [string,    6] �����ڵ�                        StartPos 93, Length 6
    char    OrdSeqNo            [  10];    // [long  ,   10] �ֹ�ȸ��                        StartPos 99, Length 10
    char    PtflNo              [  10];    // [long  ,   10] ��Ʈ��������ȣ                  StartPos 109, Length 10
    char    BskNo               [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                      StartPos 119, Length 10
    char    TrchNo              [  10];    // [long  ,   10] Ʈ��ġ��ȣ                      StartPos 129, Length 10
    char    ItemNo              [  10];    // [long  ,   10] �����۹�ȣ                      StartPos 139, Length 10
} CSPAT00800OutBlock1, *LPCSPAT00800OutBlock1;
#define NAME_CSPAT00800OutBlock1     "CSPAT00800OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CSPAT00800OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OrdNo               [  10];    // [long  ,   10] �ֹ���ȣ                        StartPos 5, Length 10
    char    PrntOrdNo           [  10];    // [long  ,   10] ���ֹ���ȣ                      StartPos 15, Length 10
    char    OrdTime             [   9];    // [string,    9] �ֹ��ð�                        StartPos 25, Length 9
    char    OrdMktCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 34, Length 2
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 36, Length 2
    char    ShtnIsuNo           [   9];    // [string,    9] ���������ȣ                    StartPos 38, Length 9
    char    PrgmOrdprcPtnCode   [   2];    // [string,    2] ���α׷�ȣ�������ڵ�            StartPos 47, Length 2
    char    StslOrdprcTpCode    [   1];    // [string,    1] ���ŵ�ȣ������                  StartPos 49, Length 1
    char    StslAbleYn          [   1];    // [string,    1] ���ŵ����ɿ���                  StartPos 50, Length 1
    char    MgntrnCode          [   3];    // [string,    3] �ſ�ŷ��ڵ�                    StartPos 51, Length 3
    char    LoanDt              [   8];    // [string,    8] ������                          StartPos 54, Length 8
    char    CvrgOrdTp           [   1];    // [string,    1] �ݴ�Ÿ��ֹ�����                StartPos 62, Length 1
    char    LpYn                [   1];    // [string,    1] �����������ڿ���                StartPos 63, Length 1
    char    MgempNo             [   9];    // [string,    9] ���������ȣ                    StartPos 64, Length 9
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű���                        StartPos 73, Length 1
    char    SpareOrdNo          [  10];    // [long  ,   10] �����ֹ���ȣ                    StartPos 74, Length 10
    char    CvrgSeqno           [  10];    // [long  ,   10] �ݴ�Ÿ��Ϸù�ȣ                StartPos 84, Length 10
    char    RsvOrdNo            [  10];    // [long  ,   10] �����ֹ���ȣ                    StartPos 94, Length 10
    char    AcntNm              [  40];    // [string,   40] ���¸�                          StartPos 104, Length 40
    char    IsuNm               [  40];    // [string,   40] �����                          StartPos 144, Length 40
} CSPAT00800OutBlock2, *LPCSPAT00800OutBlock2;
#define NAME_CSPAT00800OutBlock2     "CSPAT00800OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CSPAT00800_H_
