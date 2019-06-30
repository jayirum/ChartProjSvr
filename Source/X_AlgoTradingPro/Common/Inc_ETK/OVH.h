#ifndef _OVH_H_
#define _OVH_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ���ȣ��(OVH) ( attr,block )
#pragma pack( push, 1 )

#define NAME_OVH     "OVH"

// �Է�                           
typedef struct _OVH_InBlock
{
    char    symbol              [   8];    char    _symbol              ;    // [string,    8] �����ڵ�                        StartPos 0, Length 8
} OVH_InBlock, *LPOVH_InBlock;
#define NAME_OVH_InBlock     "InBlock"

// ���                           
typedef struct _OVH_OutBlock
{
    char    symbol              [   8];    char    _symbol              ;    // [string,    8] �����ڵ�                        StartPos 0, Length 8
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] ȣ���ð�                        StartPos 9, Length 6
    char    offerho1            [  15];    char    _offerho1            ;    // [double, 15.8] �ŵ�ȣ�� 1                      StartPos 16, Length 15
    char    bidho1              [  15];    char    _bidho1              ;    // [double, 15.8] �ż�ȣ�� 1                      StartPos 32, Length 15
    char    offerrem1           [  10];    char    _offerrem1           ;    // [long  ,   10] �ŵ�ȣ�� �ܷ� 1                 StartPos 48, Length 10
    char    bidrem1             [  10];    char    _bidrem1             ;    // [long  ,   10] �ż�ȣ�� �ܷ� 1                 StartPos 59, Length 10
    char    offerno1            [  10];    char    _offerno1            ;    // [long  ,   10] �ŵ�ȣ�� �Ǽ� 1                 StartPos 70, Length 10
    char    bidno1              [  10];    char    _bidno1              ;    // [long  ,   10] �ż�ȣ�� �Ǽ� 1                 StartPos 81, Length 10
    char    offerho2            [  15];    char    _offerho2            ;    // [double, 15.8] �ŵ�ȣ�� 2                      StartPos 92, Length 15
    char    bidho2              [  15];    char    _bidho2              ;    // [double, 15.8] �ż�ȣ�� 2                      StartPos 108, Length 15
    char    offerrem2           [  10];    char    _offerrem2           ;    // [long  ,   10] �ŵ�ȣ�� �ܷ� 2                 StartPos 124, Length 10
    char    bidrem2             [  10];    char    _bidrem2             ;    // [long  ,   10] �ż�ȣ�� �ܷ� 2                 StartPos 135, Length 10
    char    offerno2            [  10];    char    _offerno2            ;    // [long  ,   10] �ŵ�ȣ�� �Ǽ� 2                 StartPos 146, Length 10
    char    bidno2              [  10];    char    _bidno2              ;    // [long  ,   10] �ż�ȣ�� �Ǽ� 2                 StartPos 157, Length 10
    char    offerho3            [  15];    char    _offerho3            ;    // [double, 15.8] �ŵ�ȣ�� 3                      StartPos 168, Length 15
    char    bidho3              [  15];    char    _bidho3              ;    // [double, 15.8] �ż�ȣ�� 3                      StartPos 184, Length 15
    char    offerrem3           [  10];    char    _offerrem3           ;    // [long  ,   10] �ŵ�ȣ�� �ܷ� 3                 StartPos 200, Length 10
    char    bidrem3             [  10];    char    _bidrem3             ;    // [long  ,   10] �ż�ȣ�� �ܷ� 3                 StartPos 211, Length 10
    char    offerno3            [  10];    char    _offerno3            ;    // [long  ,   10] �ŵ�ȣ�� �Ǽ� 3                 StartPos 222, Length 10
    char    bidno3              [  10];    char    _bidno3              ;    // [long  ,   10] �ż�ȣ�� �Ǽ� 3                 StartPos 233, Length 10
    char    offerho4            [  15];    char    _offerho4            ;    // [double, 15.8] �ŵ�ȣ�� 4                      StartPos 244, Length 15
    char    bidho4              [  15];    char    _bidho4              ;    // [double, 15.8] �ż�ȣ�� 4                      StartPos 260, Length 15
    char    offerrem4           [  10];    char    _offerrem4           ;    // [long  ,   10] �ŵ�ȣ�� �ܷ� 4                 StartPos 276, Length 10
    char    bidrem4             [  10];    char    _bidrem4             ;    // [long  ,   10] �ż�ȣ�� �ܷ� 4                 StartPos 287, Length 10
    char    offerno4            [  10];    char    _offerno4            ;    // [long  ,   10] �ŵ�ȣ�� �Ǽ� 4                 StartPos 298, Length 10
    char    bidno4              [  10];    char    _bidno4              ;    // [long  ,   10] �ż�ȣ�� �Ǽ� 4                 StartPos 309, Length 10
    char    offerho5            [  15];    char    _offerho5            ;    // [double, 15.8] �ŵ�ȣ�� 5                      StartPos 320, Length 15
    char    bidho5              [  15];    char    _bidho5              ;    // [double, 15.8] �ż�ȣ�� 5                      StartPos 336, Length 15
    char    offerrem5           [  10];    char    _offerrem5           ;    // [long  ,   10] �ŵ�ȣ�� �ܷ� 5                 StartPos 352, Length 10
    char    bidrem5             [  10];    char    _bidrem5             ;    // [long  ,   10] �ż�ȣ�� �ܷ� 5                 StartPos 363, Length 10
    char    offerno5            [  10];    char    _offerno5            ;    // [long  ,   10] �ŵ�ȣ�� �Ǽ� 5                 StartPos 374, Length 10
    char    bidno5              [  10];    char    _bidno5              ;    // [long  ,   10] �ż�ȣ�� �Ǽ� 5                 StartPos 385, Length 10
    char    totoffercnt         [  10];    char    _totoffercnt         ;    // [long  ,   10] �ŵ�ȣ���ѰǼ�                  StartPos 396, Length 10
    char    totbidcnt           [  10];    char    _totbidcnt           ;    // [long  ,   10] �ż�ȣ���ѰǼ�                  StartPos 407, Length 10
    char    totofferrem         [  10];    char    _totofferrem         ;    // [long  ,   10] �ŵ�ȣ���Ѽ���                  StartPos 418, Length 10
    char    totbidrem           [  10];    char    _totbidrem           ;    // [long  ,   10] �ż�ȣ���Ѽ���                  StartPos 429, Length 10
} OVH_OutBlock, *LPOVH_OutBlock;
#define NAME_OVH_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _OVH_H_
