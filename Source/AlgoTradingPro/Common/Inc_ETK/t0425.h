#ifndef _t0425_H_
#define _t0425_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ�ü��/��ü��(t0425) ( attr,tuxcode=t0425,headtype=D )
#pragma pack( push, 1 )

#define NAME_t0425     "t0425"

// �⺻�Է�
typedef struct _t0425InBlock
{
    char    accno               [  11];    char    _accno               ;    // [string,   11] ���¹�ȣ                       StartPos 0, Length 11
    char    passwd              [   8];    char    _passwd              ;    // [string,    8] ��й�ȣ                       StartPos 12, Length 8
    char    expcode             [  12];    char    _expcode             ;    // [string,   12] �����ȣ                       StartPos 21, Length 12
    char    chegb               [   1];    char    _chegb               ;    // [string,    1] ü�ᱸ��                       StartPos 34, Length 1
    char    medosu              [   1];    char    _medosu              ;    // [string,    1] �Ÿű���                       StartPos 36, Length 1
    char    sortgb              [   1];    char    _sortgb              ;    // [string,    1] ���ļ���                       StartPos 38, Length 1
    char    cts_ordno           [  10];    char    _cts_ordno           ;    // [string,   10] �ֹ���ȣ                       StartPos 40, Length 10
} t0425InBlock, *LPt0425InBlock;
#define NAME_t0425InBlock     "t0425InBlock"

// ���
typedef struct _t0425OutBlock
{
    char    tqty                [  18];    char    _tqty                ;    // [long  ,   18] ���ֹ�����                     StartPos 0, Length 18
    char    tcheqty             [  18];    char    _tcheqty             ;    // [long  ,   18] ��ü�����                     StartPos 19, Length 18
    char    tordrem             [  18];    char    _tordrem             ;    // [long  ,   18] �ѹ�ü�����                   StartPos 38, Length 18
    char    cmss                [  18];    char    _cmss                ;    // [long  ,   18] ����������                     StartPos 57, Length 18
    char    tamt                [  18];    char    _tamt                ;    // [long  ,   18] ���ֹ��ݾ�                     StartPos 76, Length 18
    char    tmdamt              [  18];    char    _tmdamt              ;    // [long  ,   18] �Ѹŵ�ü��ݾ�                 StartPos 95, Length 18
    char    tmsamt              [  18];    char    _tmsamt              ;    // [long  ,   18] �Ѹż�ü��ݾ�                 StartPos 114, Length 18
    char    tax                 [  18];    char    _tax                 ;    // [long  ,   18] ����������                     StartPos 133, Length 18
    char    cts_ordno           [  10];    char    _cts_ordno           ;    // [string,   10] �ֹ���ȣ                       StartPos 152, Length 10
} t0425OutBlock, *LPt0425OutBlock;
#define NAME_t0425OutBlock     "t0425OutBlock"

// ���1, occurs
typedef struct _t0425OutBlock1
{
    char    ordno               [  10];    char    _ordno               ;    // [long  ,   10] �ֹ���ȣ                       StartPos 0, Length 10
    char    expcode             [  12];    char    _expcode             ;    // [string,   12] �����ȣ                       StartPos 11, Length 12
    char    medosu              [  10];    char    _medosu              ;    // [string,   10] ����                           StartPos 24, Length 10
    char    qty                 [   9];    char    _qty                 ;    // [long  ,    9] �ֹ�����                       StartPos 35, Length 9
    char    price               [   9];    char    _price               ;    // [long  ,    9] �ֹ�����                       StartPos 45, Length 9
    char    cheqty              [   9];    char    _cheqty              ;    // [long  ,    9] ü�����                       StartPos 55, Length 9
    char    cheprice            [   9];    char    _cheprice            ;    // [long  ,    9] ü�ᰡ��                       StartPos 65, Length 9
    char    ordrem              [   9];    char    _ordrem              ;    // [long  ,    9] ��ü���ܷ�                     StartPos 75, Length 9
    char    cfmqty              [   9];    char    _cfmqty              ;    // [long  ,    9] Ȯ�μ���                       StartPos 85, Length 9
    char    status              [  10];    char    _status              ;    // [string,   10] ����                           StartPos 95, Length 10
    char    orgordno            [  10];    char    _orgordno            ;    // [long  ,   10] ���ֹ���ȣ                     StartPos 106, Length 10
    char    ordgb               [  20];    char    _ordgb               ;    // [string,   20] ����                           StartPos 117, Length 20
    char    ordtime             [   8];    char    _ordtime             ;    // [string,    8] �ֹ��ð�                       StartPos 138, Length 8
    char    ordermtd            [  10];    char    _ordermtd            ;    // [string,   10] �ֹ���ü                       StartPos 147, Length 10
    char    sysprocseq          [  10];    char    _sysprocseq          ;    // [long  ,   10] ó������                       StartPos 158, Length 10
    char    hogagb              [   2];    char    _hogagb              ;    // [string,    2] ȣ������                       StartPos 169, Length 2
    char    price1              [   8];    char    _price1              ;    // [long  ,    8] ���簡                         StartPos 172, Length 8
    char    orggb               [   2];    char    _orggb               ;    // [string,    2] �ֹ�����                       StartPos 181, Length 2
    char    singb               [   2];    char    _singb               ;    // [string,    2] �ſ뱸��                       StartPos 184, Length 2
    char    loandt              [   8];    char    _loandt              ;    // [string,    8] ��������                       StartPos 187, Length 8
} t0425OutBlock1, *LPt0425OutBlock1;
#define NAME_t0425OutBlock1     "t0425OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t0425_H_
