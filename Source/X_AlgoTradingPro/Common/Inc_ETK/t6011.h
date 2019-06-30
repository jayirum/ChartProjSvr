///////////////////////////////////////////////////////////////////////////////////////////////////
// ���º���������ȸ(t6011) ( base21=SONAQ109,headtype=B )
#pragma pack( push, 1 )

#define NAME_t6011     "t6011"

// �⺻�Է�
typedef struct _t6011InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    kwbpno              [   3];    // [string,    3] ����������ȣ                   StartPos 5, Length 3
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 8, Length 20
    char    passwd              [   8];    // [string,    8] ��й�ȣ                       StartPos 28, Length 8
    char    jangb               [   1];    // [string,    1] �ܰ��������                   StartPos 36, Length 1
} t6011InBlock, *LPt6011InBlock;
#define NAME_t6011InBlock     "t6011InBlock"

// �⺻���
typedef struct _t6011OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    kwbpno              [   3];    // [string,    3] ����������ȣ                   StartPos 5, Length 3
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 8, Length 20
    char    passwd              [   8];    // [string,    8] ��й�ȣ                       StartPos 28, Length 8
    char    jangb               [   1];    // [string,    1] �ܰ��������                   StartPos 36, Length 1
} t6011OutBlock, *LPt6011OutBlock;
#define NAME_t6011OutBlock     "t6011OutBlock"

// �⺻���1
typedef struct _t6011OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    bpname              [  40];    // [string,   40] ������                         StartPos 5, Length 40
    char    name                [  40];    // [string,   40] ���¸�                         StartPos 45, Length 40
    char    ordcamt             [  16];    // [long  ,   16] �����ֹ����ɱݾ�               StartPos 85, Length 16
    char    drawamt             [  16];    // [long  ,   16] ��ݰ��ɱݾ�                   StartPos 101, Length 16
    char    kospiamt            [  16];    // [long  ,   16] �ŷ��ұݾ�                     StartPos 117, Length 16
    char    kosdaqamt           [  16];    // [long  ,   16] �ڽ��ڱݾ�                     StartPos 133, Length 16
    char    evlamt              [  16];    // [long  ,   16] �򰡱ݾ�                       StartPos 149, Length 16
    char    sunapsum            [  16];    // [long  ,   16] �̼��ݾ�                       StartPos 165, Length 16
    char    yetaktot            [  16];    // [long  ,   16] ��Ź�ڻ��Ѿ�                   StartPos 181, Length 16
    char    sonik               [  12];    // [double, 12.6] ���ͷ�                         StartPos 197, Length 12
    char    tujawon             [  20];    // [long  ,   20] ���ڿ���                       StartPos 209, Length 20
    char    tujason             [  16];    // [long  ,   16] ���ڼ��ͱݾ�                   StartPos 229, Length 16
    char    sindambo            [  16];    // [long  ,   16] �ſ�㺸�ֹ��ݾ�               StartPos 245, Length 16
    char    depoamt             [  16];    // [long  ,   16] ������                         StartPos 261, Length 16
    char    daeamt              [  16];    // [long  ,   16] ���ݾ�                       StartPos 277, Length 16
    char    d1depoamt           [  16];    // [long  ,   16] D1������                       StartPos 293, Length 16
    char    d2depoamt           [  16];    // [long  ,   16] D2������                       StartPos 309, Length 16
    char    cashamt             [  16];    // [long  ,   16] ���ݹ̼��ݾ�                   StartPos 325, Length 16
    char    margincash          [  16];    // [long  ,   16] ���ű�����                     StartPos 341, Length 16
    char    margindae           [  16];    // [long  ,   16] ���űݴ��                     StartPos 357, Length 16
    char    supyoamt            [  16];    // [long  ,   16] ��ǥ�ݾ�                       StartPos 373, Length 16
    char    ordcamtdae          [  16];    // [long  ,   16] ����ֹ����ɱݾ�               StartPos 389, Length 16
    char    ord100              [  16];    // [long  ,   16] ���űݷ�100�ۼ�Ʈ�ֹ����ɱݾ�  StartPos 405, Length 16
    char    inordamt            [  16];    // [long  ,   16] ���űݷ�35%�ֹ����ɱݾ�        StartPos 421, Length 16
    char    ord50               [  16];    // [long  ,   16] ���űݷ�50%�ֹ����ɱݾ�        StartPos 437, Length 16
    char    dmdosoamt           [  16];    // [long  ,   16] ���ϸŵ�����ݾ�               StartPos 453, Length 16
    char    dmsusoamt           [  16];    // [long  ,   16] ���ϸż�����ݾ�               StartPos 469, Length 16
    char    todosoamt           [  16];    // [long  ,   16] ���ϸŵ�����ݾ�               StartPos 485, Length 16
    char    tosusoamt           [  16];    // [long  ,   16] ���ϸż�����ݾ�               StartPos 501, Length 16
    char    dmpayneed           [  16];    // [long  ,   16] D1��ü�����ҿ�ݾ�             StartPos 517, Length 16
    char    topayneed           [  16];    // [long  ,   16] D2��ü�����ҿ�ݾ�             StartPos 533, Length 16
    char    d1drawamt           [  16];    // [long  ,   16] D1�������Ⱑ�ɱݾ�             StartPos 549, Length 16
    char    d2drawamt           [  16];    // [long  ,   16] D2�������Ⱑ�ɱݾ�             StartPos 565, Length 16
    char    dambore             [  16];    // [long  ,   16] ��Ź�㺸����ݾ�               StartPos 581, Length 16
    char    limreq              [  16];    // [long  ,   16] �ſ뼳��������                 StartPos 597, Length 16
    char    loanamt             [  16];    // [long  ,   16] ���ڱݾ�                       StartPos 613, Length 16
    char    damborate           [   9];    // [double,  9.3] �㺸����                       StartPos 629, Length 9
    char    cashjan             [  16];    // [long  ,   16] ���㺸�ݾ�                     StartPos 638, Length 16
    char    bucashjan           [  16];    // [long  ,   16] �δ㺸�ݾ�                     StartPos 654, Length 16
    char    needdambo           [  16];    // [long  ,   16] �ҿ�㺸�ݾ�                   StartPos 670, Length 16
    char    orgdambo            [  16];    // [long  ,   16] ���㺸�����ݾ�                 StartPos 686, Length 16
    char    dambolack           [  16];    // [long  ,   16] �㺸�����ݾ�                   StartPos 702, Length 16
    char    chugadambo          [  16];    // [long  ,   16] �߰��㺸����                   StartPos 718, Length 16
    char    d1jumun             [  16];    // [long  ,   16] D1�ֹ����ɱݾ�                 StartPos 734, Length 16
    char    sinimnap            [  16];    // [long  ,   16] �ſ����ڹ̳��ݾ�               StartPos 750, Length 16
    char    etcloan             [  16];    // [long  ,   16] ��Ÿ�뿩�ݾ�                   StartPos 766, Length 16
    char    nextbandae          [  16];    // [long  ,   16] ���������ݴ�Ÿűݾ�           StartPos 782, Length 16
    char    cashjantot          [  16];    // [long  ,   16] ���㺸�հ�ݾ�                 StartPos 798, Length 16
    char    sinordamt           [  16];    // [long  ,   16] �ſ��ֹ����ɱݾ�               StartPos 814, Length 16
    char    bucashjantot        [  16];    // [long  ,   16] �δ㺸�հ�ݾ�                 StartPos 830, Length 16
    char    sindamcash          [  16];    // [long  ,   16] �ſ�㺸������                 StartPos 846, Length 16
    char    sindamdae           [  16];    // [long  ,   16] �ſ�㺸���ݾ�               StartPos 862, Length 16
    char    addsindamcash       [  16];    // [long  ,   16] �߰��ſ�㺸����               StartPos 878, Length 16
    char    sindamreuseamt      [  16];    // [long  ,   16] �ſ�㺸����ݾ�             StartPos 894, Length 16
    char    addsindamdae        [  16];    // [long  ,   16] �߰��ſ�㺸���               StartPos 910, Length 16
    char    mddaedamloanamt     [  16];    // [long  ,   16] �ŵ���ݴ㺸����ݾ�           StartPos 926, Length 16
    char    chlimitamt          [  16];    // [long  ,   16] ó�����ѱݾ�                   StartPos 942, Length 16
} t6011OutBlock1, *LPt6011OutBlock1;
#define NAME_t6011OutBlock1     "t6011OutBlock1"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
