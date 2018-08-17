#ifndef _CIDBT01000_H_
#define _CIDBT01000_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� ����ֹ� ( SERVICE=CIDBT01000,ENCRYPT,SIGNATURE,HEADTYPE=B,CREATOR=�ֿ�ȣ,CREDATE=2012/04/26 14:52:30 )
#pragma pack( push, 1 )

#define NAME_CIDBT01000     "CIDBT01000"

// In(*EMPTY*)                    
typedef struct _CIDBT01000InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 5, Length 8
    char    BrnNo               [   3];    // [string,    3] ������ȣ                        StartPos 13, Length 3
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 16, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 36, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 44, Length 18
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] �ؿܼ������ֹ���ȣ              StartPos 62, Length 10
    char    FutsOrdTpCode       [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 72, Length 1
    char    PrdtTpCode          [   2];    // [string,    2] ��ǰ�����ڵ�                    StartPos 73, Length 2
    char    ExchCode            [  10];    // [string,   10] �ŷ����ڵ�                      StartPos 75, Length 10
} CIDBT01000InBlock1, *LPCIDBT01000InBlock1;
#define NAME_CIDBT01000InBlock1     "CIDBT01000InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBT01000OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 5, Length 8
    char    BrnNo               [   3];    // [string,    3] ������ȣ                        StartPos 13, Length 3
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 16, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 36, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 44, Length 18
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] �ؿܼ������ֹ���ȣ              StartPos 62, Length 10
    char    FutsOrdTpCode       [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 72, Length 1
    char    PrdtTpCode          [   2];    // [string,    2] ��ǰ�����ڵ�                    StartPos 73, Length 2
    char    ExchCode            [  10];    // [string,   10] �ŷ����ڵ�                      StartPos 75, Length 10
} CIDBT01000OutBlock1, *LPCIDBT01000OutBlock1;
#define NAME_CIDBT01000OutBlock1     "CIDBT01000OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CIDBT01000OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    OvrsFutsOrdNo       [  10];    // [string,   10] �ؿܼ����ֹ���ȣ                StartPos 25, Length 10
    char    InnerMsgCnts        [  80];    // [string,   80] ���θ޽�������                  StartPos 35, Length 80
} CIDBT01000OutBlock2, *LPCIDBT01000OutBlock2;
#define NAME_CIDBT01000OutBlock2     "CIDBT01000OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBT01000_H_
