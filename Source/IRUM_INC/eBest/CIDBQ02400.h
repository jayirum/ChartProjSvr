#ifndef _CIDBQ02400_H_
#define _CIDBQ02400_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� �ֹ�ü�᳻�� �� ��ȸ ( SERVICE=CIDBQ02400,ENCRYPT,HEADTYPE=B,CREATOR=��ȣ��,CREDATE=2015/06/18 18:34:41 )
#pragma pack( push, 1 )

#define NAME_CIDBQ02400     "CIDBQ02400"

// In(*EMPTY*)                    
typedef struct _CIDBQ02400InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 33, Length 18
    char    QrySrtDt            [   8];    // [string,    8] ��ȸ��������                    StartPos 51, Length 8
    char    QryEndDt            [   8];    // [string,    8] ��ȸ��������                    StartPos 59, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] ���ϱ����ڵ�                    StartPos 67, Length 1
    char    OrdStatCode         [   1];    // [string,    1] �ֹ������ڵ�                    StartPos 68, Length 1
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 69, Length 1
    char    QryTpCode           [   1];    // [string,    1] ��ȸ�����ڵ�                    StartPos 70, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 71, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] �ؿ��Ļ������ɼǱ����ڵ�        StartPos 73, Length 1
} CIDBQ02400InBlock1, *LPCIDBQ02400InBlock1;
#define NAME_CIDBQ02400InBlock1     "CIDBQ02400InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ02400OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 25, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 33, Length 18
    char    QrySrtDt            [   8];    // [string,    8] ��ȸ��������                    StartPos 51, Length 8
    char    QryEndDt            [   8];    // [string,    8] ��ȸ��������                    StartPos 59, Length 8
    char    ThdayTpCode         [   1];    // [string,    1] ���ϱ����ڵ�                    StartPos 67, Length 1
    char    OrdStatCode         [   1];    // [string,    1] �ֹ������ڵ�                    StartPos 68, Length 1
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 69, Length 1
    char    QryTpCode           [   1];    // [string,    1] ��ȸ�����ڵ�                    StartPos 70, Length 1
    char    OrdPtnCode          [   2];    // [string,    2] �ֹ������ڵ�                    StartPos 71, Length 2
    char    OvrsDrvtFnoTpCode   [   1];    // [string,    1] �ؿ��Ļ������ɼǱ����ڵ�        StartPos 73, Length 1
} CIDBQ02400OutBlock1, *LPCIDBQ02400OutBlock1;
#define NAME_CIDBQ02400OutBlock1     "CIDBQ02400OutBlock1"

// Out(*EMPTY*)                   , occurs
typedef struct _CIDBQ02400OutBlock2
{
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 0, Length 8
    char    OvrsFutsOrdNo       [  10];    // [string,   10] �ؿܼ����ֹ���ȣ                StartPos 8, Length 10
    char    OvrsFutsOrgOrdNo    [  10];    // [string,   10] �ؿܼ������ֹ���ȣ              StartPos 18, Length 10
    char    FcmOrdNo            [  15];    // [string,   15] FCM�ֹ���ȣ                     StartPos 28, Length 15
    char    OvrsFutsExecNo      [  10];    // [string,   10] �ؿܼ���ü���ȣ                StartPos 43, Length 10
    char    FcmAcntNo           [  20];    // [string,   20] FCM���¹�ȣ                     StartPos 53, Length 20
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 73, Length 18
    char    IsuNm               [  50];    // [string,   50] �����                          StartPos 91, Length 50
    char    AbrdFutsXrcPrc      [  29];    // [double,29.10] �ؿܼ�����簡��                StartPos 141, Length 29
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 170, Length 1
    char    BnsTpNm             [  10];    // [string,   10] �Ÿű��и�                      StartPos 171, Length 10
    char    FutsOrdStatCode     [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 181, Length 1
    char    TpCodeNm            [  50];    // [string,   50] �����ڵ��                      StartPos 182, Length 50
    char    FutsOrdTpCode       [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 232, Length 1
    char    TrdTpNm             [  20];    // [string,   20] �ŷ����и�                      StartPos 233, Length 20
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] �ؿܼ����ֹ������ڵ�            StartPos 253, Length 1
    char    OrdPtnNm            [  40];    // [string,   40] �ֹ�������                      StartPos 254, Length 40
    char    OrdPtnTermTpCode    [   2];    // [string,    2] �ֹ������Ⱓ�����ڵ�            StartPos 294, Length 2
    char    CmnCodeNm           [ 100];    // [string,  100] �����ڵ��                      StartPos 296, Length 100
    char    AppSrtDt            [   8];    // [string,    8] �����������                    StartPos 396, Length 8
    char    AppEndDt            [   8];    // [string,    8] ������������                    StartPos 404, Length 8
    char    OrdQty              [  16];    // [long  ,   16] �ֹ�����                        StartPos 412, Length 16
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] �ؿ��Ļ��ֹ�����                StartPos 428, Length 25
    char    ExecQty             [  16];    // [long  ,   16] ü�����                        StartPos 453, Length 16
    char    AbrdFutsExecPrc     [  29];    // [double,29.10] �ؿܼ���ü�ᰡ��                StartPos 469, Length 29
    char    OrdCndiPrc          [  25];    // [double, 25.8] �ֹ����ǰ���                    StartPos 498, Length 25
    char    NowPrc              [  15];    // [double, 15.2] ���簡                          StartPos 523, Length 15
    char    TrxStatCode         [   2];    // [string,    2] ó�������ڵ�                    StartPos 538, Length 2
    char    TrxStatCodeNm       [  40];    // [string,   40] ó�������ڵ��                  StartPos 540, Length 40
    char    CsgnCmsn            [  19];    // [double, 19.2] ��Ź������                      StartPos 580, Length 19
    char    FcmCmsn             [  21];    // [double, 21.4] FCM������                       StartPos 599, Length 21
    char    ThcoCmsn            [  19];    // [double, 19.2] ��������                      StartPos 620, Length 19
    char    MdaCode             [   2];    // [string,    2] ��ü�ڵ�                        StartPos 639, Length 2
    char    MdaCodeNm           [  40];    // [string,   40] ��ü�ڵ��                      StartPos 641, Length 40
    char    RegTmnlNo           [   3];    // [string,    3] ��ϴܸ���ȣ                    StartPos 681, Length 3
    char    RegUserId           [  16];    // [string,   16] ��ϻ����ID                    StartPos 684, Length 16
    char    OrdDttm             [  30];    // [string,   30] �ֹ��Ͻ�                        StartPos 700, Length 30
    char    OrdTime             [   9];    // [string,    9] �ֹ��ð�                        StartPos 730, Length 9
    char    ExecDt              [   8];    // [string,    8] ü������                        StartPos 739, Length 8
    char    ExecTime            [   9];    // [string,    9] ü��ð�                        StartPos 747, Length 9
    char    EufOneCmsnAmt       [  19];    // [double, 19.2] �ŷ��Һ��1������ݾ�           StartPos 756, Length 19
    char    EufTwoCmsnAmt       [  19];    // [double, 19.2] �ŷ��Һ��2������ݾ�           StartPos 775, Length 19
    char    LchOneCmsnAmt       [  19];    // [double, 19.2] ����û���1������ݾ�           StartPos 794, Length 19
    char    LchTwoCmsnAmt       [  19];    // [double, 19.2] ����û���2������ݾ�           StartPos 813, Length 19
    char    TrdOneCmsnAmt       [  19];    // [double, 19.2] �ŷ�1������ݾ�                 StartPos 832, Length 19
    char    TrdTwoCmsnAmt       [  19];    // [double, 19.2] �ŷ�2������ݾ�                 StartPos 851, Length 19
    char    TrdThreeCmsnAmt     [  19];    // [double, 19.2] �ŷ�3������ݾ�                 StartPos 870, Length 19
    char    StrmOneCmsnAmt      [  19];    // [double, 19.2] �ܱ�1������ݾ�                 StartPos 889, Length 19
    char    StrmTwoCmsnAmt      [  19];    // [double, 19.2] �ܱ�2������ݾ�                 StartPos 908, Length 19
    char    StrmThreeCmsnAmt    [  19];    // [double, 19.2] �ܱ�3������ݾ�                 StartPos 927, Length 19
    char    TransOneCmsnAmt     [  19];    // [double, 19.2] ����1������ݾ�                 StartPos 946, Length 19
    char    TransTwoCmsnAmt     [  19];    // [double, 19.2] ����2������ݾ�                 StartPos 965, Length 19
    char    TransThreeCmsnAmt   [  19];    // [double, 19.2] ����3������ݾ�                 StartPos 984, Length 19
    char    TransFourCmsnAmt    [  19];    // [double, 19.2] ����4������ݾ�                 StartPos 1003, Length 19
    char    OvrsOptXrcRsvTpCode [   1];    // [string,    1] �ؿܿɼ���翹�౸���ڵ�        StartPos 1022, Length 1
} CIDBQ02400OutBlock2, *LPCIDBQ02400OutBlock2;
#define NAME_CIDBQ02400OutBlock2     "CIDBQ02400OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ02400_H_
