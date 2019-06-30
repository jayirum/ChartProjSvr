#ifndef _SC0_H_
#define _SC0_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ��ֹ����� ( block )
#pragma pack( push, 1 )

#define NAME_SC0     "SC0"

// �Է�
typedef struct _SC0_InBlock
{
} SC0_InBlock, *LPSC0_InBlock;
#define NAME_SC0_InBlock     "InBlock"

// ���
typedef struct _SC0_OutBlock
{
    char    lineseq             [  10];    // [long  ,   10] �����Ϸù�ȣ                   StartPos 0, Length 10
    char    accno               [  11];    // [string,   11] ���¹�ȣ                       StartPos 10, Length 11
    char    user                [   8];    // [string,    8] ������ID                       StartPos 21, Length 8
    char    len                 [   6];    // [long  ,    6] �������                       StartPos 29, Length 6
    char    gubun               [   1];    // [string,    1] �������                       StartPos 35, Length 1
    char    compress            [   1];    // [string,    1] ���౸��                       StartPos 36, Length 1
    char    encrypt             [   1];    // [string,    1] ��ȣ����                       StartPos 37, Length 1
    char    offset              [   3];    // [long  ,    3] �����������                   StartPos 38, Length 3
    char    trcode              [   8];    // [string,    8] TRCODE                         StartPos 41, Length 8
    char    compid              [   3];    // [string,    3] �̿���ȣ                     StartPos 49, Length 3
    char    userid              [  16];    // [string,   16] �����ID                       StartPos 52, Length 16
    char    media               [   2];    // [string,    2] ���Ӹ�ü                       StartPos 68, Length 2
    char    ifid                [   3];    // [string,    3] I/F�Ϸù�ȣ                    StartPos 70, Length 3
    char    seq                 [   9];    // [string,    9] �����Ϸù�ȣ                   StartPos 73, Length 9
    char    trid                [  16];    // [string,   16] TR����ID                       StartPos 82, Length 16
    char    pubip               [  12];    // [string,   12] ����IP                         StartPos 98, Length 12
    char    prvip               [  12];    // [string,   12] �缳IP                         StartPos 110, Length 12
    char    pcbpno              [   3];    // [string,    3] ó��������ȣ                   StartPos 122, Length 3
    char    bpno                [   3];    // [string,    3] ������ȣ                       StartPos 125, Length 3
    char    termno              [   8];    // [string,    8] �ܸ���ȣ                       StartPos 128, Length 8
    char    lang                [   1];    // [string,    1] ����                       StartPos 136, Length 1
    char    proctm              [   9];    // [long  ,    9] APó���ð�                     StartPos 137, Length 9
    char    msgcode             [   4];    // [string,    4] �޼����ڵ�                     StartPos 146, Length 4
    char    outgu               [   1];    // [string,    1] �޼�����±���                 StartPos 150, Length 1
    char    compreq             [   1];    // [string,    1] �����û����                   StartPos 151, Length 1
    char    funckey             [   4];    // [string,    4] ���Ű                         StartPos 152, Length 4
    char    reqcnt              [   4];    // [long  ,    4] ��û���ڵ尳��                 StartPos 156, Length 4
    char    filler              [   6];    // [string,    6] ���񿵿�                       StartPos 160, Length 6
    char    cont                [   1];    // [string,    1] ���ӱ���                       StartPos 166, Length 1
    char    contkey             [  18];    // [string,   18] ����Ű��                       StartPos 167, Length 18
    char    varlen              [   2];    // [long  ,    2] �����ý��۱���                 StartPos 185, Length 2
    char    varhdlen            [   2];    // [long  ,    2] �����ش�����                   StartPos 187, Length 2
    char    varmsglen           [   2];    // [long  ,    2] �����޽�������                 StartPos 189, Length 2
    char    trsrc               [   1];    // [string,    1] ��ȸ�߿���                     StartPos 191, Length 1
    char    eventid             [   4];    // [string,    4] I/F�̺�ƮID                    StartPos 192, Length 4
    char    ifinfo              [   4];    // [string,    4] I/F����                        StartPos 196, Length 4
    char    filler1             [  41];    // [string,   41] ���񿵿�                       StartPos 200, Length 41
    char    ordchegb            [   2];    // [string,    2] �ֹ�ü�ᱸ��                   StartPos 241, Length 2
    char    marketgb            [   2];    // [string,    2] ���屸��                       StartPos 243, Length 2
    char    ordgb               [   2];    // [string,    2] �ֹ�����                       StartPos 245, Length 2
    char    orgordno            [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 247, Length 10
    char    accno1              [  11];    // [string,   11] ���¹�ȣ                       StartPos 257, Length 11
    char    accno2              [   9];    // [string,    9] ���¹�ȣ                       StartPos 268, Length 9
    char    passwd              [   8];    // [string,    8] ��й�ȣ                       StartPos 277, Length 8
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 285, Length 12
    char    shtcode             [   9];    // [string,    9] ���������ȣ                   StartPos 297, Length 9
    char    hname               [  40];    // [string,   40] �����                         StartPos 306, Length 40
    char    ordqty              [  16];    // [long  ,   16] �ֹ�����                       StartPos 346, Length 16
    char    ordprice            [  13];    // [long  ,   13] �ֹ�����                       StartPos 362, Length 13
    char    hogagb              [   1];    // [string,    1] �ֹ�����                       StartPos 375, Length 1
    char    etfhogagb           [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 376, Length 2
    char    pgmtype             [   2];    // [long  ,    2] ���α׷�ȣ������               StartPos 378, Length 2
    char    gmhogagb            [   1];    // [long  ,    1] ���ŵ�ȣ������                 StartPos 380, Length 1
    char    gmhogayn            [   1];    // [long  ,    1] ���ŵ����ɿ���                 StartPos 381, Length 1
    char    singb               [   3];    // [string,    3] �ſ뱸��                       StartPos 382, Length 3
    char    loandt              [   8];    // [string,    8] ������                         StartPos 385, Length 8
    char    cvrgordtp           [   1];    // [string,    1] �ݴ�Ÿ��ֹ�����               StartPos 393, Length 1
    char    strtgcode           [   6];    // [string,    6] �����ڵ�                       StartPos 394, Length 6
    char    groupid             [  20];    // [string,   20] �׷�ID                         StartPos 400, Length 20
    char    ordseqno            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 420, Length 10
    char    prtno               [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 430, Length 10
    char    basketno            [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 440, Length 10
    char    trchno              [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 450, Length 10
    char    itemno              [  10];    // [long  ,   10] �ƾ��۹�ȣ                     StartPos 460, Length 10
    char    brwmgmyn            [   1];    // [long  ,    1] ���Ա���                       StartPos 470, Length 1
    char    mbrno               [   3];    // [long  ,    3] ȸ�����ȣ                     StartPos 471, Length 3
    char    procgb              [   1];    // [string,    1] ó������                       StartPos 474, Length 1
    char    admbrchno           [   3];    // [string,    3] ����������ȣ                   StartPos 475, Length 3
    char    futaccno            [  20];    // [string,   20] �������¹�ȣ                   StartPos 478, Length 20
    char    futmarketgb         [   1];    // [string,    1] ������ǰ����                   StartPos 498, Length 1
    char    tongsingb           [   2];    // [string,    2] ��Ÿ�ü����                   StartPos 499, Length 2
    char    lpgb                [   1];    // [string,    1] �����������ڱ���               StartPos 501, Length 1
    char    dummy               [  20];    // [string,   20] DUMMY                          StartPos 502, Length 20
    char    ordno               [  10];    // [long  ,   10] �ֹ���ȣ                       StartPos 522, Length 10
    char    ordtm               [   9];    // [string,    9] �ֹ��ð�                       StartPos 532, Length 9
    char    prntordno           [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 541, Length 10
    char    mgempno             [   9];    // [string,    9] ���������ȣ                   StartPos 551, Length 9
    char    orgordundrqty       [  16];    // [long  ,   16] ���ֹ���ü�����               StartPos 560, Length 16
    char    orgordmdfyqty       [  16];    // [long  ,   16] ���ֹ���������                 StartPos 576, Length 16
    char    ordordcancelqty     [  16];    // [long  ,   16] ���ֹ���Ҽ���                 StartPos 592, Length 16
    char    nmcpysndno          [  10];    // [long  ,   10] ��ȸ����۽Ź�ȣ               StartPos 608, Length 10
    char    ordamt              [  16];    // [long  ,   16] �ֹ��ݾ�                       StartPos 618, Length 16
    char    bnstp               [   1];    // [string,    1] �Ÿű���                       StartPos 634, Length 1
    char    spareordno          [  10];    // [long  ,   10] �����ֹ���ȣ                   StartPos 635, Length 10
    char    cvrgseqno           [  10];    // [long  ,   10] �ݴ�Ÿ��Ϸù�ȣ               StartPos 645, Length 10
    char    rsvordno            [  10];    // [long  ,   10] �����ֹ���ȣ                   StartPos 655, Length 10
    char    mtordseqno          [  10];    // [long  ,   10] �����ֹ��Ϸù�ȣ               StartPos 665, Length 10
    char    spareordqty         [  16];    // [long  ,   16] �����ֹ�����                   StartPos 675, Length 16
    char    orduserid           [  16];    // [string,   16] �ֹ������ȣ                   StartPos 691, Length 16
    char    spotordqty          [  16];    // [long  ,   16] �ǹ��ֹ�����                   StartPos 707, Length 16
    char    ordruseqty          [  16];    // [long  ,   16] �����ֹ�����                 StartPos 723, Length 16
    char    mnyordamt           [  16];    // [long  ,   16] �����ֹ��ݾ�                   StartPos 739, Length 16
    char    ordsubstamt         [  16];    // [long  ,   16] �ֹ����ݾ�                   StartPos 755, Length 16
    char    ruseordamt          [  16];    // [long  ,   16] �����ֹ��ݾ�                 StartPos 771, Length 16
    char    ordcmsnamt          [  16];    // [long  ,   16] �������ֹ��ݾ�                 StartPos 787, Length 16
    char    crdtuseamt          [  16];    // [long  ,   16] ���ſ�㺸�����           StartPos 803, Length 16
    char    secbalqty           [  16];    // [long  ,   16] �ܰ����                       StartPos 819, Length 16
    char    spotordableqty      [  16];    // [long  ,   16] �ǹ����ɼ���                   StartPos 835, Length 16
    char    ordableruseqty      [  16];    // [long  ,   16] ���밡�ɼ���(�ŵ�)           StartPos 851, Length 16
    char    flctqty             [  16];    // [long  ,   16] ��������                       StartPos 867, Length 16
    char    secbalqtyd2         [  16];    // [long  ,   16] �ܰ����(D2)                   StartPos 883, Length 16
    char    sellableqty         [  16];    // [long  ,   16] �ŵ��ֹ����ɼ���               StartPos 899, Length 16
    char    unercsellordqty     [  16];    // [long  ,   16] ��ü��ŵ��ֹ�����             StartPos 915, Length 16
    char    avrpchsprc          [  13];    // [long  ,   13] ��ո��԰�                     StartPos 931, Length 13
    char    pchsamt             [  16];    // [long  ,   16] ���Աݾ�                       StartPos 944, Length 16
    char    deposit             [  16];    // [long  ,   16] ������                         StartPos 960, Length 16
    char    substamt            [  16];    // [long  ,   16] ����                         StartPos 976, Length 16
    char    csgnmnymgn          [  16];    // [long  ,   16] ��Ź���ű�����                 StartPos 992, Length 16
    char    csgnsubstmgn        [  16];    // [long  ,   16] ��Ź���űݴ��                 StartPos 1008, Length 16
    char    crdtpldgruseamt     [  16];    // [long  ,   16] �ſ�㺸�����               StartPos 1024, Length 16
    char    ordablemny          [  16];    // [long  ,   16] �ֹ���������                   StartPos 1040, Length 16
    char    ordablesubstamt     [  16];    // [long  ,   16] �ֹ����ɴ��                   StartPos 1056, Length 16
    char    ruseableamt         [  16];    // [long  ,   16] ���밡�ɱݾ�                 StartPos 1072, Length 16
} SC0_OutBlock, *LPSC0_OutBlock;
#define NAME_SC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _SC0_H_
