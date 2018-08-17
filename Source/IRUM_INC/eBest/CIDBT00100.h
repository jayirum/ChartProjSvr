#ifndef _CIDBT00100_H_
#define _CIDBT00100_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� �ű��ֹ� ( SERVICE=CIDBT00100,ENCRYPT,SIGNATURE,HEADTYPE=B,CREATOR=�ֿ�ȣ,CREDATE=2012/04/26 14:50:17 )
#pragma pack( push, 1 )

#define NAME_CIDBT00100     "CIDBT00100"

// In(*EMPTY*)                    
typedef struct _CIDBT00100InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 5, Length 8
    char    BrnCode             [   7];    // [string,    7] �����ڵ�                        StartPos 13, Length 7
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 20, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 40, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 48, Length 18
    char    FutsOrdTpCode       [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 66, Length 1
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�(1:�ŵ�, 2:�ż�)    StartPos 67, Length 1
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] �ؿܼ����ֹ������ڵ�            StartPos 68, Length 1
    char    CrcyCode            [   3];    // [string,    3] ��ȭ�ڵ�                        StartPos 69, Length 3
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] �ؿ��Ļ��ֹ�����                StartPos 72, Length 25
    char    CndiOrdPrc          [  25];    // [double, 25.8] �����ֹ�����                    StartPos 97, Length 25
    char    OrdQty              [  16];    // [long  ,   16] �ֹ�����                        StartPos 122, Length 16
    char    PrdtCode            [   6];    // [string,    6] ��ǰ�ڵ�                        StartPos 138, Length 6
    char    DueYymm             [   6];    // [string,    6] ������                        StartPos 144, Length 6
    char    ExchCode            [  10];    // [string,   10] �ŷ����ڵ�                      StartPos 150, Length 10
} CIDBT00100InBlock1, *LPCIDBT00100InBlock1;
#define NAME_CIDBT00100InBlock1     "CIDBT00100InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBT00100OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OrdDt               [   8];    // [string,    8] �ֹ�����                        StartPos 5, Length 8
    char    BrnCode             [   7];    // [string,    7] �����ڵ�                        StartPos 13, Length 7
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 20, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 40, Length 8
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 48, Length 18
    char    FutsOrdTpCode       [   1];    // [string,    1] �����ֹ������ڵ�                StartPos 66, Length 1
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 67, Length 1
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] �ؿܼ����ֹ������ڵ�            StartPos 68, Length 1
    char    CrcyCode            [   3];    // [string,    3] ��ȭ�ڵ�                        StartPos 69, Length 3
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] �ؿ��Ļ��ֹ�����                StartPos 72, Length 25
    char    CndiOrdPrc          [  25];    // [double, 25.8] �����ֹ�����                    StartPos 97, Length 25
    char    OrdQty              [  16];    // [long  ,   16] �ֹ�����                        StartPos 122, Length 16
    char    PrdtCode            [   6];    // [string,    6] ��ǰ�ڵ�                        StartPos 138, Length 6
    char    DueYymm             [   6];    // [string,    6] ������                        StartPos 144, Length 6
    char    ExchCode            [  10];    // [string,   10] �ŷ����ڵ�                      StartPos 150, Length 10
} CIDBT00100OutBlock1, *LPCIDBT00100OutBlock1;
#define NAME_CIDBT00100OutBlock1     "CIDBT00100OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CIDBT00100OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    OvrsFutsOrdNo       [  10];    // [string,   10] �ؿܼ����ֹ���ȣ                StartPos 25, Length 10
} CIDBT00100OutBlock2, *LPCIDBT00100OutBlock2;
#define NAME_CIDBT00100OutBlock2     "CIDBT00100OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBT00100_H_
