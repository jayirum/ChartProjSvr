#ifndef _CIDBQ01800_H_
#define _CIDBQ01800_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� �ֹ�ü�᳻�� ��ȸ ( SERVICE=CIDBQ01800,HEADTYPE=B,CREATOR=��ȣ��,CREDATE=2015/06/22 19:06:11 )
#pragma pack( push, 1 )

#define NAME_CIDBQ01800     "CIDBQ01800"

// In(*EMPTY*)                    
typedef struct _CIDBQ01800InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 33, Length 18
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 51, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] ���ϱ����ڵ�                    StartPos 59, Length 1
    char    OrdStatCode         [   1];    // [string,    1] �ֹ������ڵ�                    StartPos 60, Length 1
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 61, Length 1
    char    QryTpCode           [   1];    // [string,    1] ��ȸ�����ڵ�                    StartPos 62, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 63, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] �ؿ��Ļ������ɼǱ����ڵ�        StartPos 65, Length 1
} CIDBQ01800InBlock1, *LPCIDBQ01800InBlock1;
#define NAME_CIDBQ01800InBlock1     "CIDBQ01800InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ01800OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 33, Length 18
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 51, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] ���ϱ����ڵ�                    StartPos 59, Length 1
    char    OrdStatCode         [   1];    // [string,    1] �ֹ������ڵ�                    StartPos 60, Length 1
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 61, Length 1
    char    QryTpCode           [   1];    // [string,    1] ��ȸ�����ڵ�                    StartPos 62, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 63, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] �ؿ��Ļ������ɼǱ����ڵ�        StartPos 65, Length 1
} CIDBQ01800OutBlock1, *LPCIDBQ01800OutBlock1;
#define NAME_CIDBQ01800OutBlock1     "CIDBQ01800OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ01800OutBlock2
{
    char    OvrsFutsOrdNo       [  10];    // [string,   10] �ؿܼ����ֹ���ȣ                StartPos 0, Length 10
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] �ؿܼ������ֹ���ȣ              StartPos 10, Length 10
    char    FcmOrdNo            [  15];    // [string,   15] FCM�ֹ���ȣ                     StartPos 20, Length 15
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 35, Length 18
    char    IsuNm               [  50];    // [string,   50] �����                          StartPos 53, Length 50
    char    AbrdFutsXrcPrc      [  29];    // [double,29.10] �ؿܼ�����簡��                StartPos 103, Length 29
    char    FcmAcntNo           [  20];    // [string,   20] FCM���¹�ȣ                     StartPos 132, Length 20
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 152, Length 1
    char    BnsTpNm             [  10];    // [string,   10] �Ÿű��и�                      StartPos 153, Length 10
    char    FutsOrdStatCode     [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 163, Length 1
    char    TpCodeNm            [  50];    // [string,   50] �����ڵ��                      StartPos 164, Length 50
    char    FutsOrdTpCode       [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 214, Length 1
    char    TrdTpNm             [  20];    // [string,   20] �ŷ����и�                      StartPos 215, Length 20
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] �ؿܼ����ֹ������ڵ�            StartPos 235, Length 1
    char    OrdPtnNm            [  40];    // [string,   40] �ֹ�������                      StartPos 236, Length 40
    char    OrdPtnTermTpCode    [   2];    // [string,    2] �ֹ������Ⱓ�����ڵ�            StartPos 276, Length 2
    char    CmnCodeNm           [ 100];    // [string,  100] �����ڵ��                      StartPos 278, Length 100
    char    AppSrtDt            [   8];    // [string,    8] �����������                    StartPos 378, Length 8
    char    AppEndDt            [   8];    // [string,    8] ������������                    StartPos 386, Length 8
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] �ؿ��Ļ��ֹ�����                StartPos 394, Length 25
    char    OrdQty              [  16];    // [long  ,   16] �ֹ�����                        StartPos 419, Length 16
    char    AbrdFutsExecPrc     [  29];    // [double,29.10] �ؿܼ���ü�ᰡ��                StartPos 435, Length 29
    char    ExecQty             [  16];    // [long  ,   16] ü�����                        StartPos 464, Length 16
    char    OrdCndiPrc          [  25];    // [double, 25.8] �ֹ����ǰ���                    StartPos 480, Length 25
    char    OvrsDrvtNowPrc      [  25];    // [double, 25.8] �ؿ��Ļ����簡                  StartPos 505, Length 25
    char    MdfyQty             [  16];    // [long  ,   16] ��������                        StartPos 530, Length 16
    char    CancQty             [  16];    // [long  ,   16] ��Ҽ���                        StartPos 546, Length 16
    char    RjtQty              [  13];    // [long  ,   13] �źμ���                        StartPos 562, Length 13
    char    CnfQty              [  16];    // [long  ,   16] Ȯ�μ���                        StartPos 575, Length 16
    char    CvrgYn              [   1];    // [string,    1] �ݴ�Ÿſ���                    StartPos 591, Length 1
    char    RegTmnlNo           [   3];    // [string,    3] ��ϴܸ���ȣ                    StartPos 592, Length 3
    char    RegBrnNo            [   3];    // [string,    3] ���������ȣ                    StartPos 595, Length 3
    char    RegUserId           [  16];    // [string,   16] ��ϻ����ID                    StartPos 598, Length 16
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 614, Length 8
    char    OrdTime             [   9];    // [string,    9] �ֹ��ð�                        StartPos 622, Length 9
    char    OvrsOptXrcRsvTpCode [   1];    // [string,    1] �ؿܿɼ���翹�౸���ڵ�        StartPos 631, Length 1
} CIDBQ01800OutBlock2, *LPCIDBQ01800OutBlock2;
#define NAME_CIDBQ01800OutBlock2     "CIDBQ01800OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ01800_H_
