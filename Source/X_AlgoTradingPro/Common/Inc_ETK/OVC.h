#ifndef _OVC_H_
#define _OVC_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿܼ���ü��(OVC) ( attr,block )
#pragma pack( push, 1 )

#define NAME_OVC     "OVC"

// �Է�                           
typedef struct _OVC_InBlock
{
    char    symbol              [   8];    char    _symbol              ;    // [string,    8] �����ڵ�                        StartPos 0, Length 8
} OVC_InBlock, *LPOVC_InBlock;
#define NAME_OVC_InBlock     "InBlock"

// ���                           
typedef struct _OVC_OutBlock
{
    char    symbol              [   8];    char    _symbol              ;    // [string,    8] �����ڵ�                        StartPos 0, Length 8
    char    ovsdate             [   8];    char    _ovsdate             ;    // [string,    8] ü������(����)                  StartPos 9, Length 8
    char    kordate             [   8];    char    _kordate             ;    // [string,    8] ü������(�ѱ�)                  StartPos 18, Length 8
    char    trdtm               [   6];    char    _trdtm               ;    // [string,    6] ü��ð�(����)                  StartPos 27, Length 6
    char    kortm               [   6];    char    _kortm               ;    // [string,    6] ü��ð�(�ѱ�)                  StartPos 34, Length 6
    char    curpr               [  15];    char    _curpr               ;    // [double, 15.8] ü�ᰡ��                        StartPos 41, Length 15
    char    ydiffpr             [  15];    char    _ydiffpr             ;    // [double, 15.8] ���ϴ��                        StartPos 57, Length 15
    char    ydiffSign           [   1];    char    _ydiffSign           ;    // [string,    1] ���ϴ���ȣ                    StartPos 73, Length 1
    char    open                [  15];    char    _open                ;    // [double, 15.8] �ð�                            StartPos 75, Length 15
    char    high                [  15];    char    _high                ;    // [double, 15.8] ��                            StartPos 91, Length 15
    char    low                 [  15];    char    _low                 ;    // [double, 15.8] ����                            StartPos 107, Length 15
    char    chgrate             [   6];    char    _chgrate             ;    // [float ,  6.2] �����	                         StartPos 123, Length 6
    char    trdq                [  10];    char    _trdq                ;    // [long  ,   10] �Ǻ�ü�����                    StartPos 130, Length 10
    char    totq                [  15];    char    _totq                ;    // [string,   15] ����ü�����                    StartPos 141, Length 15
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] ü�ᱸ��                        StartPos 157, Length 1
    char    mdvolume            [  15];    char    _mdvolume            ;    // [string,   15] �ŵ�����ü�����                StartPos 159, Length 15
    char    msvolume            [  15];    char    _msvolume            ;    // [string,   15] �ż�����ü�����                StartPos 175, Length 15
    char    ovsmkend            [   8];    char    _ovsmkend            ;    // [string,    8] �帶����                        StartPos 191, Length 8
} OVC_OutBlock, *LPOVC_OutBlock;
#define NAME_OVC_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _OVC_H_
