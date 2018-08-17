#ifndef _CSPAQ02300_H_
#define _CSPAQ02300_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �������� �ܰ��� ��ȸ ( SERVICE=CSPAQ02300,HEADTYPE=B,CREATOR=�̻���,CREDATE=2011/11/15 14:05:42 )
#pragma pack( push, 1 )

#define NAME_CSPAQ02300     "CSPAQ02300"

// In(*EMPTY*)                    
typedef struct _CSPAQ02300InBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 25, Length 8
    char    BalCreTp            [   1];    // [string,    1] �ܰ��������                    StartPos 33, Length 1
    char    CmsnAppTpCode       [   1];    // [string,    1] ���������뱸��                  StartPos 34, Length 1
    char    D2balBaseQryTp      [   1];    // [string,    1] D2�ܰ������ȸ����              StartPos 35, Length 1
    char    UprcTpCode          [   1];    // [string,    1] �ܰ�����                        StartPos 36, Length 1
} CSPAQ02300InBlock1, *LPCSPAQ02300InBlock1;
#define NAME_CSPAQ02300InBlock1     "CSPAQ02300InBlock1"

// In(*EMPTY*)                    
typedef struct _CSPAQ02300OutBlock1
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    AcntNo              [  20];    // [string,   20] ���¹�ȣ                        StartPos 5, Length 20
    char    Pwd                 [   8];    // [string,    8] ��й�ȣ                        StartPos 25, Length 8
    char    BalCreTp            [   1];    // [string,    1] �ܰ��������                    StartPos 33, Length 1
    char    CmsnAppTpCode       [   1];    // [string,    1] ���������뱸��                  StartPos 34, Length 1
    char    D2balBaseQryTp      [   1];    // [string,    1] D2�ܰ������ȸ����              StartPos 35, Length 1
    char    UprcTpCode          [   1];    // [string,    1] �ܰ�����                        StartPos 36, Length 1
} CSPAQ02300OutBlock1, *LPCSPAQ02300OutBlock1;
#define NAME_CSPAQ02300OutBlock1     "CSPAQ02300OutBlock1"

// Out(*EMPTY*)                   
typedef struct _CSPAQ02300OutBlock2
{
    char    RecCnt              [   5];    // [long  ,    5] ���ڵ尹��                      StartPos 0, Length 5
    char    BrnNm               [  40];    // [string,   40] ������                          StartPos 5, Length 40
    char    AcntNm              [  40];    // [string,   40] ���¸�                          StartPos 45, Length 40
    char    MnyOrdAbleAmt       [  16];    // [long  ,   16] �����ֹ����ɱݾ�                StartPos 85, Length 16
    char    MnyoutAbleAmt       [  16];    // [long  ,   16] ��ݰ��ɱݾ�                    StartPos 101, Length 16
    char    SeOrdAbleAmt        [  16];    // [long  ,   16] �ŷ��ұݾ�                      StartPos 117, Length 16
    char    KdqOrdAbleAmt       [  16];    // [long  ,   16] �ڽ��ڱݾ�                      StartPos 133, Length 16
    char    HtsOrdAbleAmt       [  16];    // [long  ,   16] HTS�ֹ����ɱݾ�                 StartPos 149, Length 16
    char    MgnRat100pctOrdAbleAmt[  16];    // [long  ,   16] ���űݷ�100�ۼ�Ʈ�ֹ����ɱݾ�   StartPos 165, Length 16
    char    BalEvalAmt          [  16];    // [long  ,   16] �ܰ��򰡱ݾ�                    StartPos 181, Length 16
    char    PchsAmt             [  16];    // [long  ,   16] ���Աݾ�                        StartPos 197, Length 16
    char    RcvblAmt            [  16];    // [long  ,   16] �̼��ݾ�                        StartPos 213, Length 16
    char    PnlRat              [  18];    // [double, 18.6] ������                          StartPos 229, Length 18
    char    InvstOrgAmt         [  20];    // [long  ,   20] ���ڿ���                        StartPos 247, Length 20
    char    InvstPlAmt          [  16];    // [long  ,   16] ���ڼ��ͱݾ�                    StartPos 267, Length 16
    char    CrdtPldgOrdAmt      [  16];    // [long  ,   16] �ſ�㺸�ֹ��ݾ�                StartPos 283, Length 16
    char    Dps                 [  16];    // [long  ,   16] ������                          StartPos 299, Length 16
    char    D1Dps               [  16];    // [long  ,   16] D1������                        StartPos 315, Length 16
    char    D2Dps               [  16];    // [long  ,   16] D2������                        StartPos 331, Length 16
    char    OrdDt               [   8];    // [string,    8] �ֹ���                          StartPos 347, Length 8
    char    MnyMgn              [  16];    // [long  ,   16] �������űݾ�                    StartPos 355, Length 16
    char    SubstMgn            [  16];    // [long  ,   16] ������űݾ�                    StartPos 371, Length 16
    char    SubstAmt            [  16];    // [long  ,   16] ���ݾ�                        StartPos 387, Length 16
    char    PrdayBuyExecAmt     [  16];    // [long  ,   16] ���ϸż�ü��ݾ�                StartPos 403, Length 16
    char    PrdaySellExecAmt    [  16];    // [long  ,   16] ���ϸŵ�ü��ݾ�                StartPos 419, Length 16
    char    CrdayBuyExecAmt     [  16];    // [long  ,   16] ���ϸż�ü��ݾ�                StartPos 435, Length 16
    char    CrdaySellExecAmt    [  16];    // [long  ,   16] ���ϸŵ�ü��ݾ�                StartPos 451, Length 16
    char    EvalPnlSum          [  15];    // [long  ,   15] �򰡼����հ�                    StartPos 467, Length 15
    char    DpsastTotamt        [  16];    // [long  ,   16] ��Ź�ڻ��Ѿ�                    StartPos 482, Length 16
    char    Evrprc              [  19];    // [long  ,   19] �����                          StartPos 498, Length 19
    char    RuseAmt             [  16];    // [long  ,   16] ����ݾ�                      StartPos 517, Length 16
    char    EtclndAmt           [  16];    // [long  ,   16] ��Ÿ�뿩�ݾ�                    StartPos 533, Length 16
    char    PrcAdjstAmt         [  16];    // [long  ,   16] ������ݾ�                      StartPos 549, Length 16
    char    D1CmsnAmt           [  16];    // [long  ,   16] D1������                        StartPos 565, Length 16
    char    D2CmsnAmt           [  16];    // [long  ,   16] D2������                        StartPos 581, Length 16
    char    D1EvrTax            [  16];    // [long  ,   16] D1������                        StartPos 597, Length 16
    char    D2EvrTax            [  16];    // [long  ,   16] D2������                        StartPos 613, Length 16
    char    D1SettPrergAmt      [  16];    // [long  ,   16] D1���������ݾ�                  StartPos 629, Length 16
    char    D2SettPrergAmt      [  16];    // [long  ,   16] D2���������ݾ�                  StartPos 645, Length 16
    char    PrdayKseMnyMgn      [  16];    // [long  ,   16] ����KSE�������ű�               StartPos 661, Length 16
    char    PrdayKseSubstMgn    [  16];    // [long  ,   16] ����KSE������ű�               StartPos 677, Length 16
    char    PrdayKseCrdtMnyMgn  [  16];    // [long  ,   16] ����KSE�ſ��������ű�           StartPos 693, Length 16
    char    PrdayKseCrdtSubstMgn[  16];    // [long  ,   16] ����KSE�ſ������ű�           StartPos 709, Length 16
    char    CrdayKseMnyMgn      [  16];    // [long  ,   16] ����KSE�������ű�               StartPos 725, Length 16
    char    CrdayKseSubstMgn    [  16];    // [long  ,   16] ����KSE������ű�               StartPos 741, Length 16
    char    CrdayKseCrdtMnyMgn  [  16];    // [long  ,   16] ����KSE�ſ��������ű�           StartPos 757, Length 16
    char    CrdayKseCrdtSubstMgn[  16];    // [long  ,   16] ����KSE�ſ������ű�           StartPos 773, Length 16
    char    PrdayKdqMnyMgn      [  16];    // [long  ,   16] �����ڽ����������ű�            StartPos 789, Length 16
    char    PrdayKdqSubstMgn    [  16];    // [long  ,   16] �����ڽ��ڴ�����ű�            StartPos 805, Length 16
    char    PrdayKdqCrdtMnyMgn  [  16];    // [long  ,   16] �����ڽ��ڽſ��������ű�        StartPos 821, Length 16
    char    PrdayKdqCrdtSubstMgn[  16];    // [long  ,   16] �����ڽ��ڽſ������ű�        StartPos 837, Length 16
    char    CrdayKdqMnyMgn      [  16];    // [long  ,   16] �����ڽ����������ű�            StartPos 853, Length 16
    char    CrdayKdqSubstMgn    [  16];    // [long  ,   16] �����ڽ��ڴ�����ű�            StartPos 869, Length 16
    char    CrdayKdqCrdtMnyMgn  [  16];    // [long  ,   16] �����ڽ��ڽſ��������ű�        StartPos 885, Length 16
    char    CrdayKdqCrdtSubstMgn[  16];    // [long  ,   16] �����ڽ��ڽſ������ű�        StartPos 901, Length 16
    char    PrdayFrbrdMnyMgn    [  16];    // [long  ,   16] �������������������ű�          StartPos 917, Length 16
    char    PrdayFrbrdSubstMgn  [  16];    // [long  ,   16] �����������������ű�          StartPos 933, Length 16
    char    CrdayFrbrdMnyMgn    [  16];    // [long  ,   16] �������������������ű�          StartPos 949, Length 16
    char    CrdayFrbrdSubstMgn  [  16];    // [long  ,   16] �����������������ű�          StartPos 965, Length 16
    char    PrdayCrbmkMnyMgn    [  16];    // [long  ,   16] ��������������ű�              StartPos 981, Length 16
    char    PrdayCrbmkSubstMgn  [  16];    // [long  ,   16] ������ܴ�����ű�              StartPos 997, Length 16
    char    CrdayCrbmkMnyMgn    [  16];    // [long  ,   16] ��������������ű�              StartPos 1013, Length 16
    char    CrdayCrbmkSubstMgn  [  16];    // [long  ,   16] ������ܴ�����ű�              StartPos 1029, Length 16
    char    DpspdgQty           [  16];    // [long  ,   16] ��Ź�㺸����                    StartPos 1045, Length 16
    char    BuyAdjstAmtD2       [  16];    // [long  ,   16] �ż������(D+2)                 StartPos 1061, Length 16
    char    SellAdjstAmtD2      [  16];    // [long  ,   16] �ŵ������(D+2)                 StartPos 1077, Length 16
    char    RepayRqrdAmtD1      [  16];    // [long  ,   16] �����ҿ��(D+1)                 StartPos 1093, Length 16
    char    RepayRqrdAmtD2      [  16];    // [long  ,   16] �����ҿ��(D+2)                 StartPos 1109, Length 16
    char    LoanAmt             [  16];    // [long  ,   16] ����ݾ�                        StartPos 1125, Length 16
} CSPAQ02300OutBlock2, *LPCSPAQ02300OutBlock2;
#define NAME_CSPAQ02300OutBlock2     "CSPAQ02300OutBlock2"

// ST_OUT(*EMPTY*)                , occurs
typedef struct _CSPAQ02300OutBlock3
{
    char    IsuNo               [  12];    // [string,   12] �����ȣ                        StartPos 0, Length 12
    char    IsuNm               [  40];    // [string,   40] �����                          StartPos 12, Length 40
    char    SecBalPtnCode       [   2];    // [string,    2] ���������ܰ������ڵ�            StartPos 52, Length 2
    char    SecBalPtnNm         [  40];    // [string,   40] ���������ܰ�������              StartPos 54, Length 40
    char    BalQty              [  16];    // [long  ,   16] �ܰ����                        StartPos 94, Length 16
    char    BnsBaseBalQty       [  16];    // [long  ,   16] �Ÿű����ܰ����                StartPos 110, Length 16
    char    CrdayBuyExecQty     [  16];    // [long  ,   16] ���ϸż�ü�����                StartPos 126, Length 16
    char    CrdaySellExecQty    [  16];    // [long  ,   16] ���ϸŵ�ü�����                StartPos 142, Length 16
    char    SellPrc             [  21];    // [double, 21.4] �ŵ���                          StartPos 158, Length 21
    char    BuyPrc              [  21];    // [double, 21.4] �ż���                          StartPos 179, Length 21
    char    SellPnlAmt          [  16];    // [long  ,   16] �ŵ����ͱݾ�                    StartPos 200, Length 16
    char    PnlRat              [  18];    // [double, 18.6] ������                          StartPos 216, Length 18
    char    NowPrc              [  15];    // [double, 15.2] ���簡                          StartPos 234, Length 15
    char    CrdtAmt             [  16];    // [long  ,   16] �ſ�ݾ�                        StartPos 249, Length 16
    char    DueDt               [   8];    // [string,    8] ������                          StartPos 265, Length 8
    char    PrdaySellExecPrc    [  13];    // [double, 13.2] ���ϸŵ�ü�ᰡ                  StartPos 273, Length 13
    char    PrdaySellQty        [  16];    // [long  ,   16] ���ϸŵ�����                    StartPos 286, Length 16
    char    PrdayBuyExecPrc     [  13];    // [double, 13.2] ���ϸż�ü�ᰡ                  StartPos 302, Length 13
    char    PrdayBuyQty         [  16];    // [long  ,   16] ���ϸż�����                    StartPos 315, Length 16
    char    LoanDt              [   8];    // [string,    8] ������                          StartPos 331, Length 8
    char    AvrUprc             [  13];    // [double, 13.2] ��մܰ�                        StartPos 339, Length 13
    char    SellAbleQty         [  16];    // [long  ,   16] �ŵ����ɼ���                    StartPos 352, Length 16
    char    SellOrdQty          [  16];    // [long  ,   16] �ŵ��ֹ�����                    StartPos 368, Length 16
    char    CrdayBuyExecAmt     [  16];    // [long  ,   16] ���ϸż�ü��ݾ�                StartPos 384, Length 16
    char    CrdaySellExecAmt    [  16];    // [long  ,   16] ���ϸŵ�ü��ݾ�                StartPos 400, Length 16
    char    PrdayBuyExecAmt     [  16];    // [long  ,   16] ���ϸż�ü��ݾ�                StartPos 416, Length 16
    char    PrdaySellExecAmt    [  16];    // [long  ,   16] ���ϸŵ�ü��ݾ�                StartPos 432, Length 16
    char    BalEvalAmt          [  16];    // [long  ,   16] �ܰ��򰡱ݾ�                    StartPos 448, Length 16
    char    EvalPnl             [  16];    // [long  ,   16] �򰡼���                        StartPos 464, Length 16
    char    MnyOrdAbleAmt       [  16];    // [long  ,   16] �����ֹ����ɱݾ�                StartPos 480, Length 16
    char    OrdAbleAmt          [  16];    // [long  ,   16] �ֹ����ɱݾ�                    StartPos 496, Length 16
    char    SellUnercQty        [  16];    // [long  ,   16] �ŵ���ü�����                  StartPos 512, Length 16
    char    SellUnsttQty        [  16];    // [long  ,   16] �ŵ��̰�������                  StartPos 528, Length 16
    char    BuyUnercQty         [  16];    // [long  ,   16] �ż���ü�����                  StartPos 544, Length 16
    char    BuyUnsttQty         [  16];    // [long  ,   16] �ż��̰�������                  StartPos 560, Length 16
    char    UnsttQty            [  16];    // [long  ,   16] �̰�������                      StartPos 576, Length 16
    char    UnercQty            [  16];    // [long  ,   16] ��ü�����                      StartPos 592, Length 16
    char    PrdayCprc           [  15];    // [double, 15.2] ��������                        StartPos 608, Length 15
    char    PchsAmt             [  16];    // [long  ,   16] ���Աݾ�                        StartPos 623, Length 16
    char    RegMktCode          [   2];    // [string,    2] ��Ͻ����ڵ�                    StartPos 639, Length 2
    char    LoanDtlClssCode     [   2];    // [string,    2] ����󼼺з��ڵ�                StartPos 641, Length 2
    char    DpspdgLoanQty       [  16];    // [long  ,   16] ��Ź�㺸�������                StartPos 643, Length 16
} CSPAQ02300OutBlock3, *LPCSPAQ02300OutBlock3;
#define NAME_CSPAQ02300OutBlock3     "CSPAQ02300OutBlock3"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _CSPAQ02300_H_
