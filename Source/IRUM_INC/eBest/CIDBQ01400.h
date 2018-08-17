#ifndef _CIDBQ01400_H_
#define _CIDBQ01400_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ��� ü�᳻������ ��ȸ(�ֹ����ɼ���) ( SERVICE=CIDBQ01400,HEADTYPE=B,CREATOR=��ȣ��,CREDATE=2015/07/30 09:08:25 )
#pragma pack( push, 1 )

#define NAME_CIDBQ01400     "CIDBQ01400"

// In(*EMPTY*)                    
typedef struct _CIDBQ01400InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    QryTpCode           [   1];    // [string,    1] ��ȸ�����ڵ�                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 6, Length 20
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 26, Length 18
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 44, Length 1
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] �ؿ��Ļ��ֹ�����                StartPos 45, Length 25
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] �ؿܼ����ֹ������ڵ�            StartPos 70, Length 1
} CIDBQ01400InBlock1, *LPCIDBQ01400InBlock1;
#define NAME_CIDBQ01400InBlock1     "CIDBQ01400InBlock1"

// In(*EMPTY*)                    
typedef struct _CIDBQ01400OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    QryTpCode           [   1];    // [string,    1] ��ȸ�����ڵ�                    StartPos 5, Length 1
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 6, Length 20
    char    IsuCodeVal          [  18];    // [string,   18] �����ڵ尪                      StartPos 26, Length 18
    char    BnsTpCode           [   1];    // [string,    1] �Ÿű����ڵ�                    StartPos 44, Length 1
    char    OvrsDrvtOrdPrc      [  25];    // [double, 25.8] �ؿ��Ļ��ֹ�����                StartPos 45, Length 25
    char    AbrdFutsOrdPtnCode  [   1];    // [string,    1] �ؿܼ����ֹ������ڵ�            StartPos 70, Length 1
} CIDBQ01400OutBlock1, *LPCIDBQ01400OutBlock1;
#define NAME_CIDBQ01400OutBlock1     "CIDBQ01400OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CIDBQ01400OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    OrdAbleQty          [  16];    // [long  ,   16] �ֹ����ɼ���                    StartPos 5, Length 16
} CIDBQ01400OutBlock2, *LPCIDBQ01400OutBlock2;
#define NAME_CIDBQ01400OutBlock2     "CIDBQ01400OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CIDBQ01400_H_
