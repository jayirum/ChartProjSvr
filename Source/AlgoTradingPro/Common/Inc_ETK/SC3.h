#ifndef _SC3_H_
#define _SC3_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ��ֹ���� ( block )
#pragma pack( push, 1 )

#define NAME_SC3     "SC3"

// �Է�
typedef struct _SC3_InBlock
{
} SC3_InBlock, *LPSC3_InBlock;
#define NAME_SC3_InBlock     "InBlock"

// ���
typedef struct _SC3_OutBlock
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
    char    ordxctptncode       [   2];    // [string,    2] �ֹ�ü�������ڵ�               StartPos 241, Length 2
    char    ordmktcode          [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 243, Length 2
    char    ordptncode          [   2];    // [string,    2] �ֹ������ڵ�                   StartPos 245, Length 2
    char    mgmtbrnno           [   3];    // [string,    3] ����������ȣ                   StartPos 247, Length 3
    char    accno1              [  11];    // [string,   11] ���¹�ȣ                       StartPos 250, Length 11
    char    accno2              [   9];    // [string,    9] ���¹�ȣ                       StartPos 261, Length 9
    char    acntnm              [  40];    // [string,   40] ���¸�                         StartPos 270, Length 40
    char    Isuno               [  12];    // [string,   12] �����ȣ                       StartPos 310, Length 12
    char    Isunm               [  40];    // [string,   40] �����                         StartPos 322, Length 40
    char    ordno               [  10];    // [long  ,   10] �ֹ���ȣ                       StartPos 362, Length 10
    char    orgordno            [  10];    // [long  ,   10] ���ֹ���ȣ                     StartPos 372, Length 10
    char    execno              [  10];    // [long  ,   10] ü���ȣ                       StartPos 382, Length 10
    char    ordqty              [  16];    // [long  ,   16] �ֹ�����                       StartPos 392, Length 16
    char    ordprc              [  13];    // [long  ,   13] �ֹ�����                       StartPos 408, Length 13
    char    execqty             [  16];    // [long  ,   16] ü�����                       StartPos 421, Length 16
    char    execprc             [  13];    // [long  ,   13] ü�ᰡ��                       StartPos 437, Length 13
    char    mdfycnfqty          [  16];    // [long  ,   16] ����Ȯ�μ���                   StartPos 450, Length 16
    char    mdfycnfprc          [  16];    // [long  ,   16] ����Ȯ�ΰ���                   StartPos 466, Length 16
    char    canccnfqty          [  16];    // [long  ,   16] ���Ȯ�μ���                   StartPos 482, Length 16
    char    rjtqty              [  16];    // [long  ,   16] �źμ���                       StartPos 498, Length 16
    char    ordtrxptncode       [   4];    // [long  ,    4] �ֹ�ó�������ڵ�               StartPos 514, Length 4
    char    mtiordseqno         [  10];    // [long  ,   10] �����ֹ��Ϸù�ȣ               StartPos 518, Length 10
    char    ordcndi             [   1];    // [string,    1] �ֹ�����                       StartPos 528, Length 1
    char    ordprcptncode       [   2];    // [string,    2] ȣ�������ڵ�                   StartPos 529, Length 2
    char    nsavtrdqty          [  16];    // [long  ,   16] ������ü�����                 StartPos 531, Length 16
    char    shtnIsuno           [   9];    // [string,    9] ���������ȣ                   StartPos 547, Length 9
    char    opdrtnno            [  12];    // [string,   12] ������ù�ȣ                   StartPos 556, Length 12
    char    cvrgordtp           [   1];    // [string,    1] �ݴ�Ÿ��ֹ�����               StartPos 568, Length 1
    char    unercqty            [  16];    // [long  ,   16] ��ü�����(�ֹ�)               StartPos 569, Length 16
    char    orgordunercqty      [  16];    // [long  ,   16] ���ֹ���ü�����               StartPos 585, Length 16
    char    orgordmdfyqty       [  16];    // [long  ,   16] ���ֹ���������                 StartPos 601, Length 16
    char    orgordcancqty       [  16];    // [long  ,   16] ���ֹ���Ҽ���                 StartPos 617, Length 16
    char    ordavrexecprc       [  13];    // [long  ,   13] �ֹ����ü�ᰡ��               StartPos 633, Length 13
    char    ordamt              [  16];    // [long  ,   16] �ֹ��ݾ�                       StartPos 646, Length 16
    char    stdIsuno            [  12];    // [string,   12] ǥ�������ȣ                   StartPos 662, Length 12
    char    bfstdIsuno          [  12];    // [string,   12] ��ǥ�������ȣ                 StartPos 674, Length 12
    char    bnstp               [   1];    // [string,    1] �Ÿű���                       StartPos 686, Length 1
    char    ordtrdptncode       [   2];    // [string,    2] �ֹ��ŷ������ڵ�               StartPos 687, Length 2
    char    mgntrncode          [   3];    // [string,    3] �ſ�ŷ��ڵ�                   StartPos 689, Length 3
    char    adduptp             [   2];    // [string,    2] �������ջ��ڵ�                 StartPos 692, Length 2
    char    commdacode          [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 694, Length 2
    char    Loandt              [   8];    // [string,    8] ������                         StartPos 696, Length 8
    char    mbrnmbrno           [   3];    // [long  ,    3] ȸ��/��ȸ�����ȣ              StartPos 704, Length 3
    char    ordacntno           [  20];    // [string,   20] �ֹ����¹�ȣ                   StartPos 707, Length 20
    char    agrgbrnno           [   3];    // [string,    3] ����������ȣ                   StartPos 727, Length 3
    char    mgempno             [   9];    // [string,    9] ���������ȣ                   StartPos 730, Length 9
    char    futsLnkbrnno        [   3];    // [string,    3] ��������������ȣ               StartPos 739, Length 3
    char    futsLnkacntno       [  20];    // [string,   20] ����������¹�ȣ               StartPos 742, Length 20
    char    futsmkttp           [   1];    // [string,    1] �������屸��                   StartPos 762, Length 1
    char    regmktcode          [   2];    // [string,    2] ��Ͻ����ڵ�                   StartPos 763, Length 2
    char    mnymgnrat           [   7];    // [long  ,    7] �������űݷ�                   StartPos 765, Length 7
    char    substmgnrat         [   9];    // [long  ,    9] ������űݷ�                   StartPos 772, Length 9
    char    mnyexecamt          [  16];    // [long  ,   16] ����ü��ݾ�                   StartPos 781, Length 16
    char    ubstexecamt         [  16];    // [long  ,   16] ���ü��ݾ�                   StartPos 797, Length 16
    char    cmsnamtexecamt      [  16];    // [long  ,   16] ������ü��ݾ�                 StartPos 813, Length 16
    char    crdtpldgexecamt     [  16];    // [long  ,   16] �ſ�㺸ü��ݾ�               StartPos 829, Length 16
    char    crdtexecamt         [  16];    // [long  ,   16] �ſ�ü��ݾ�                   StartPos 845, Length 16
    char    prdayruseexecval    [  16];    // [long  ,   16] ��������ü��ݾ�             StartPos 861, Length 16
    char    crdayruseexecval    [  16];    // [long  ,   16] ��������ü��ݾ�             StartPos 877, Length 16
    char    spotexecqty         [  16];    // [long  ,   16] �ǹ�ü�����                   StartPos 893, Length 16
    char    stslexecqty         [  16];    // [long  ,   16] ���ŵ�ü�����                 StartPos 909, Length 16
    char    strtgcode           [   6];    // [string,    6] �����ڵ�                       StartPos 925, Length 6
    char    grpId               [  20];    // [string,   20] �׷�Id                         StartPos 931, Length 20
    char    ordseqno            [  10];    // [long  ,   10] �ֹ�ȸ��                       StartPos 951, Length 10
    char    ptflno              [  10];    // [long  ,   10] ��Ʈ��������ȣ                 StartPos 961, Length 10
    char    bskno               [  10];    // [long  ,   10] �ٽ��Ϲ�ȣ                     StartPos 971, Length 10
    char    trchno              [  10];    // [long  ,   10] Ʈ��ġ��ȣ                     StartPos 981, Length 10
    char    itemno              [  10];    // [long  ,   10] �����۹�ȣ                     StartPos 991, Length 10
    char    orduserId           [  16];    // [string,   16] �ֹ���Id                       StartPos 1001, Length 16
    char    brwmgmtYn           [   1];    // [long  ,    1] ���԰�������                   StartPos 1017, Length 1
    char    frgrunqno           [   6];    // [string,    6] �ܱ��ΰ�����ȣ                 StartPos 1018, Length 6
    char    trtzxLevytp         [   1];    // [string,    1] �ŷ���¡������                 StartPos 1024, Length 1
    char    lptp                [   1];    // [string,    1] �����������ڱ���               StartPos 1025, Length 1
    char    exectime            [   9];    // [string,    9] ü��ð�                       StartPos 1026, Length 9
    char    rcptexectime        [   9];    // [string,    9] �ŷ��Ҽ���ü��ð�             StartPos 1035, Length 9
    char    rmndLoanamt         [  16];    // [long  ,   16] �ܿ�����ݾ�                   StartPos 1044, Length 16
    char    secbalqty           [  16];    // [long  ,   16] �ܰ����                       StartPos 1060, Length 16
    char    spotordableqty      [  16];    // [long  ,   16] �ǹ����ɼ���                   StartPos 1076, Length 16
    char    ordableruseqty      [  16];    // [long  ,   16] ���밡�ɼ���(�ŵ�)           StartPos 1092, Length 16
    char    flctqty             [  16];    // [long  ,   16] ��������                       StartPos 1108, Length 16
    char    secbalqtyd2         [  16];    // [long  ,   16] �ܰ����(d2)                   StartPos 1124, Length 16
    char    sellableqty         [  16];    // [long  ,   16] �ŵ��ֹ����ɼ���               StartPos 1140, Length 16
    char    unercsellordqty     [  16];    // [long  ,   16] ��ü��ŵ��ֹ�����             StartPos 1156, Length 16
    char    avrpchsprc          [  13];    // [long  ,   13] ��ո��԰�                     StartPos 1172, Length 13
    char    pchsant             [  16];    // [long  ,   16] ���Աݾ�                       StartPos 1185, Length 16
    char    deposit             [  16];    // [long  ,   16] ������                         StartPos 1201, Length 16
    char    substamt            [  16];    // [long  ,   16] ����                         StartPos 1217, Length 16
    char    csgnmnymgn          [  16];    // [long  ,   16] ��Ź���ű�����                 StartPos 1233, Length 16
    char    csgnsubstmgn        [  16];    // [long  ,   16] ��Ź���űݴ��                 StartPos 1249, Length 16
    char    crdtpldgruseamt     [  16];    // [long  ,   16] �ſ�㺸�����               StartPos 1265, Length 16
    char    ordablemny          [  16];    // [long  ,   16] �ֹ���������                   StartPos 1281, Length 16
    char    ordablesubstamt     [  16];    // [long  ,   16] �ֹ����ɴ��                   StartPos 1297, Length 16
    char    ruseableamt         [  16];    // [long  ,   16] ���밡�ɱݾ�                 StartPos 1313, Length 16
} SC3_OutBlock, *LPSC3_OutBlock;
#define NAME_SC3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _SC3_H_
