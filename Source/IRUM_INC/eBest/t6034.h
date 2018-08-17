///////////////////////////////////////////////////////////////////////////////////////////////////
// ���¿�Ź��Ȳ(����/�ɼ�)(t6034) ( base21=SONBQ105,headtype=B )
#pragma pack( push, 1 )

#define NAME_t6034     "t6034"

// �⺻�Է�
typedef struct _t6034InBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] ��й�ȣ                       StartPos 25, Length 8
} t6034InBlock, *LPt6034InBlock;
#define NAME_t6034InBlock     "t6034InBlock"

// �⺻���
typedef struct _t6034OutBlock
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accno               [  20];    // [string,   20] ���¹�ȣ                       StartPos 5, Length 20
    char    passwd              [   8];    // [string,    8] ��й�ȣ                       StartPos 25, Length 8
} t6034OutBlock, *LPt6034OutBlock;
#define NAME_t6034OutBlock     "t6034OutBlock"

// �⺻���1
typedef struct _t6034OutBlock1
{
    char    reccnt              [   5];    // [long  ,    5] ���ڵ尹��                     StartPos 0, Length 5
    char    accname             [  40];    // [string,   40] ���¸�                         StartPos 5, Length 40
    char    yetaktotamt         [  16];    // [long  ,   16] ��Ź���Ѿ�                     StartPos 45, Length 16
    char    yesuamt             [  16];    // [long  ,   16] ������                         StartPos 61, Length 16
    char    dyamt               [  16];    // [long  ,   16] ���ݾ�                       StartPos 77, Length 16
    char    addyetaktotamt      [  16];    // [long  ,   16] ��翹Ź���Ѿ�                 StartPos 93, Length 16
    char    addyesuamt          [  16];    // [long  ,   16] ��翹����                     StartPos 109, Length 16
    char    futprofitamt        [  16];    // [long  ,   16] �������ͱݾ�                   StartPos 125, Length 16
    char    outamt              [  16];    // [long  ,   16] ���Ⱑ�ɱݾ�                   StartPos 141, Length 16
    char    outcashamt          [  16];    // [long  ,   16] ���Ⱑ�����ݾ�                 StartPos 157, Length 16
    char    outdyamt            [  16];    // [long  ,   16] ���Ⱑ�ɴ��ݾ�               StartPos 173, Length 16
    char    dipositamt          [  16];    // [long  ,   16] ���űݾ�                       StartPos 189, Length 16
    char    cashdipositamt      [  16];    // [long  ,   16] �������űݾ�                   StartPos 205, Length 16
    char    ordpoamt            [  16];    // [long  ,   16] �ֹ����ɱݾ�                   StartPos 221, Length 16
    char    cashpoamt           [  16];    // [long  ,   16] �����ֹ����ɱݾ�               StartPos 237, Length 16
    char    adddipositamt       [  16];    // [long  ,   16] �߰����űݾ�                   StartPos 253, Length 16
    char    cashadddipositamt   [  16];    // [long  ,   16] �����߰����űݾ�               StartPos 269, Length 16
    char    supyoinamt          [  16];    // [long  ,   16] �����ϼ�ǥ�Աݾ�               StartPos 285, Length 16
    char    foptpredymedoamt    [  16];    // [long  ,   16] �����ɼ����ϴ��ŵ��ݾ�       StartPos 301, Length 16
    char    fopttodaydymedoamt  [  16];    // [long  ,   16] �����ɼǱ��ϴ��ŵ��ݾ�       StartPos 317, Length 16
    char    foptpreaddamt       [  16];    // [long  ,   16] �����ɼ����ϰ��Աݾ�           StartPos 333, Length 16
    char    fopttodayaddamt     [  16];    // [long  ,   16] �����ɼǱ��ϰ��Աݾ�           StartPos 349, Length 16
    char    fordyamt            [  16];    // [long  ,   16] ��ȭ���ݾ�                   StartPos 365, Length 16
    char    foptaccpositname    [  20];    // [string,   20] �����ɼǰ��»������űݸ�       StartPos 381, Length 20
} t6034OutBlock1, *LPt6034OutBlock1;
#define NAME_t6034OutBlock1     "t6034OutBlock1"

// �⺻���2, occurs
typedef struct _t6034OutBlock2
{
    char    groupcodname        [  20];    // [string,   20] ��ǰ���ڵ��                   StartPos 0, Length 20
    char    dangerpositamt      [  16];    // [long  ,   16] ���������űݾ�                 StartPos 20, Length 16
    char    pricepositamt       [  16];    // [long  ,   16] �������űݾ�                   StartPos 36, Length 16
    char    spreadpositamt      [  16];    // [long  ,   16] �����������űݾ�               StartPos 52, Length 16
    char    priceratepositamt   [  16];    // [long  ,   16] ���ݺ������űݾ�               StartPos 68, Length 16
    char    minpositamt         [  16];    // [long  ,   16] �ּ����űݾ�                   StartPos 84, Length 16
    char    ordpositamt         [  16];    // [long  ,   16] �ֹ����űݾ�                   StartPos 100, Length 16
    char    optsunmesuamt       [  16];    // [long  ,   16] �ɼǼ��ż��ݾ�                 StartPos 116, Length 16
    char    wtpositamt          [  16];    // [long  ,   16] ��Ź���űݾ�                   StartPos 132, Length 16
    char    ujpositamt          [  16];    // [long  ,   16] �������űݾ�                   StartPos 148, Length 16
    char    fmesucheamt         [  16];    // [long  ,   16] �����ż�ü��ݾ�               StartPos 164, Length 16
    char    fmedocheamt         [  16];    // [long  ,   16] �����ŵ�ü��ݾ�               StartPos 180, Length 16
    char    optmesucheamt       [  16];    // [long  ,   16] �ɼǸż�ü��ݾ�               StartPos 196, Length 16
    char    optmedocheamt       [  16];    // [long  ,   16] �ɼǸŵ�ü��ݾ�               StartPos 212, Length 16
    char    flossamt            [  16];    // [long  ,   16] �������ͱݾ�                   StartPos 228, Length 16
    char    totriskwtpositamt   [  16];    // [long  ,   16] ��������Ź���ű�               StartPos 244, Length 16
} t6034OutBlock2, *LPt6034OutBlock2;
#define NAME_t6034OutBlock2     "t6034OutBlock2"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
