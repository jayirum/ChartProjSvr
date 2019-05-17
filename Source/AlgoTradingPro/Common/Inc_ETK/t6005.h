///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ű������ֹ����ɱݾ���ȸ(t6005) ( base21=SONAQ201,headtype=B )
#pragma pack( push, 1 )

#define NAME_t6005     "t6005"

// �⺻�Է�
typedef struct _t6005InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 5, Length 1
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 6, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 26, Length 8
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 34, Length 12
    char    hopemesu            [  13];    // [double, 13.2] �ֹ���                         StartPos 46, Length 13
    char    mechegb             [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 59, Length 2
} t6005InBlock, *LPt6005InBlock;
#define NAME_t6005InBlock     "t6005InBlock"

// �⺻���
typedef struct _t6005OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    memegb              [   1];    // [string,    1] �Ÿű���                       StartPos 5, Length 1
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 6, Length 20
    char    passwd              [   8];    // [string,    8] �Էº�й�ȣ                   StartPos 26, Length 8
    char    expcode             [  12];    // [string,   12] �����ȣ                       StartPos 34, Length 12
    char    hopemesu            [  13];    // [double, 13.2] �ֹ���                         StartPos 46, Length 13
    char    mechegb             [   2];    // [string,    2] ��Ÿ�ü�ڵ�                   StartPos 59, Length 2
} t6005OutBlock, *LPt6005OutBlock;
#define NAME_t6005OutBlock     "t6005OutBlock"

// �⺻���1
typedef struct _t6005OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accname             [  40];    // [string,   40] ���¸�                         StartPos 5, Length 40
    char    codename            [  40];    // [string,   40] �����                         StartPos 45, Length 40
    char    depoamt             [  16];    // [long  ,   16] ������                         StartPos 85, Length 16
    char    daeamt              [  16];    // [long  ,   16] ���ݾ�                       StartPos 101, Length 16
    char    cdamreuse           [  16];    // [long  ,   16] �ſ�㺸����ݾ�             StartPos 117, Length 16
    char    cashordamt          [  16];    // [long  ,   16] �����ֹ����ɱݾ�               StartPos 133, Length 16
    char    orddae              [  16];    // [long  ,   16] ����ֹ����ɱݾ�               StartPos 149, Length 16
    char    margincash          [  16];    // [long  ,   16] �������űݾ�                   StartPos 165, Length 16
    char    margindae           [  16];    // [long  ,   16] ������űݾ�                   StartPos 181, Length 16
    char    kospiamt            [  16];    // [long  ,   16] �ŷ��ұݾ�                     StartPos 197, Length 16
    char    kosdaqamt           [  16];    // [long  ,   16] �ڽ��ڱݾ�                     StartPos 213, Length 16
    char    d1cash              [  16];    // [long  ,   16] ����������(D+1)                StartPos 229, Length 16
    char    d2cash              [  16];    // [long  ,   16] ����������(D+2)                StartPos 245, Length 16
    char    chulcanamt          [  16];    // [long  ,   16] ��ݰ��ɱݾ�                   StartPos 261, Length 16
    char    misuamt             [  16];    // [long  ,   16] �̼��ݾ�                       StartPos 277, Length 16
    char    susuryorate         [  11];    // [double, 11.8] ��������                       StartPos 293, Length 11
    char    addchamt            [  16];    // [long  ,   16] �߰�¡���ݾ�                   StartPos 304, Length 16
    char    resuseamt           [  16];    // [long  ,   16] ������ݾ�                 StartPos 320, Length 16
    char    cashresuseamt       [  16];    // [long  ,   16] ����������ݾ�             StartPos 336, Length 16
    char    usemarginrate       [   7];    // [double,  7.4] �̿�����űݷ�                 StartPos 352, Length 7
    char    daereuseamt         [  16];    // [long  ,   16] ���������ݾ�             StartPos 359, Length 16
    char    daeresuseamt        [   7];    // [double,  7.4] �������űݷ�                   StartPos 375, Length 7
    char    charate             [   7];    // [double,  7.4] �������űݷ�                   StartPos 382, Length 7
    char    marginrate          [   7];    // [double,  7.4] �ŷ����űݷ�                   StartPos 389, Length 7
    char    susuryo             [  16];    // [long  ,   16] ������                         StartPos 396, Length 16
    char    ordamt50            [  16];    // [long  ,   16] ���űݷ�50�ۼ�Ʈ�ֹ����ɱݾ�   StartPos 412, Length 16
    char    ordcashqty50        [  16];    // [long  ,   16] ���űݷ�50�ۼ�Ʈ�ֹ����ɼ���   StartPos 428, Length 16
    char    ordamt30            [  16];    // [long  ,   16] ���űݷ�30�ۼ�Ʈ�ֹ����ɱݾ�   StartPos 444, Length 16
    char    ordqty30            [  16];    // [long  ,   16] ���űݷ�30�ۼ�Ʈ�ֹ����ɼ���   StartPos 460, Length 16
    char    ordamt40            [  16];    // [long  ,   16] ���űݷ�40�ۼ�Ʈ�ֹ����ɱݾ�   StartPos 476, Length 16
    char    ordqty40            [  16];    // [long  ,   16] ���űݷ�40�ۼ�Ʈ�ֹ����ɼ���   StartPos 492, Length 16
    char    ordamt100           [  16];    // [long  ,   16] ���űݷ�100�ۼ�Ʈ�ֹ����ɱݾ�  StartPos 508, Length 16
    char    ordqty100           [  16];    // [long  ,   16] ���űݷ�100�ۼ�Ʈ�ֹ����ɼ���  StartPos 524, Length 16
    char    ordcashamt100       [  16];    // [long  ,   16] ���űݷ�100�ۼ�Ʈ�����ֹ����ɱ� StartPos 540, Length 16
    char    ordcashqty100       [  16];    // [long  ,   16] ���űݷ�100�ۼ�Ʈ�����ֹ����ɼ� StartPos 556, Length 16
    char    reamt20             [  16];    // [long  ,   16] ���űݷ�20�ۼ�Ʈ���밡�ɱݾ� StartPos 572, Length 16
    char    reamt30             [  16];    // [long  ,   16] ���űݷ�30�ۼ�Ʈ���밡�ɱݾ� StartPos 588, Length 16
    char    reamt40             [  16];    // [long  ,   16] ���űݷ�40�ۼ�Ʈ���밡�ɱݾ� StartPos 604, Length 16
    char    reamt100            [  16];    // [long  ,   16] ���űݷ�100�ۼ�Ʈ���밡�ɱݾ� StartPos 620, Length 16
    char    memeqty             [  16];    // [long  ,   16] �ֹ����ɼ���                   StartPos 636, Length 16
    char    ordamt              [  16];    // [long  ,   16] �ֹ����ɱݾ�                   StartPos 652, Length 16
} t6005OutBlock1, *LPt6005OutBlock1;
#define NAME_t6005OutBlock1     "t6005OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
