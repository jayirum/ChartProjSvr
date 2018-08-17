#ifndef _K3__H_
#define _K3__H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// KOSDAQü�� ( ATTR, KEY=6, GROUP=1 )
#pragma pack( push, 1 )

#define NAME_K3_     "K3_"

// �Է�                           
typedef struct _K3__InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                        StartPos 0, Length 6
} K3__InBlock, *LPK3__InBlock;
#define NAME_K3__InBlock     "InBlock"

// ���                           
typedef struct _K3__OutBlock
{
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] ü��ð�                        StartPos 0, Length 6
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�                    StartPos 7, Length 1
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��                        StartPos 9, Length 8
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����                          StartPos 18, Length 6
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡                          StartPos 25, Length 8
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] �ð��ð�                        StartPos 34, Length 6
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�                            StartPos 41, Length 8
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] ���ð�                        StartPos 50, Length 6
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��                            StartPos 57, Length 8
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] �����ð�                        StartPos 66, Length 6
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����                            StartPos 73, Length 8
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] ü�ᱸ��                        StartPos 82, Length 1
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ü�ᷮ                          StartPos 84, Length 8
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���                      StartPos 93, Length 12
    char    value               [  12];    char    _value               ;    // [long  ,   12] �����ŷ����                    StartPos 106, Length 12
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�����ü�ᷮ                  StartPos 119, Length 12
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�����ü��Ǽ�                StartPos 132, Length 8
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�����ü�ᷮ                  StartPos 141, Length 12
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�����ü��Ǽ�                StartPos 154, Length 8
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] ü�ᰭ��                        StartPos 163, Length 9
    char    w_avrg              [   8];    char    _w_avrg              ;    // [long  ,    8] ������հ�                      StartPos 173, Length 8
    char    offerho             [   8];    char    _offerho             ;    // [long  ,    8] �ŵ�ȣ��                        StartPos 182, Length 8
    char    bidho               [   8];    char    _bidho               ;    // [long  ,    8] �ż�ȣ��                        StartPos 191, Length 8
    char    status              [   2];    char    _status              ;    // [string,    2] ������                          StartPos 200, Length 2
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϵ��ð���ŷ���              StartPos 203, Length 12
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                        StartPos 216, Length 6
} K3__OutBlock, *LPK3__OutBlock;
#define NAME_K3__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _K3__H_
