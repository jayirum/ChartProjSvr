#ifndef _t8411_H_
#define _t8411_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ�íƮ(ƽ/nƽ) ( ATTR,BLOCK,HEADTYPE=A )
#pragma pack( push, 1 )

#define NAME_t8411     "t8411"

// �⺻�Է�                       
typedef struct _t8411InBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                        StartPos 0, Length 6
    char    ncnt                [   4];    char    _ncnt                ;    // [long  ,    4] ����(nƽ)                       StartPos 7, Length 4
    char    qrycnt              [   4];    char    _qrycnt              ;    // [long  ,    4] ��û�Ǽ�(�ִ�-����:2000�����:5 StartPos 12, Length 4
    char    nday                [   1];    char    _nday                ;    // [string,    1] ��ȸ�����ϼ�(0:�̻��1>=���)   StartPos 17, Length 1
    char    sdate               [   8];    char    _sdate               ;    // [string,    8] ��������                        StartPos 19, Length 8
    char    stime               [   6];    char    _stime               ;    // [string,    6] ���۽ð�(����̻��)            StartPos 28, Length 6
    char    edate               [   8];    char    _edate               ;    // [string,    8] ��������                        StartPos 35, Length 8
    char    etime               [   6];    char    _etime               ;    // [string,    6] ����ð�(����̻��)            StartPos 44, Length 6
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] ��������                        StartPos 51, Length 8
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] ���ӽð�                        StartPos 60, Length 10
    char    comp_yn             [   1];    char    _comp_yn             ;    // [string,    1] ���࿩��(Y:����N:�����)        StartPos 71, Length 1
} t8411InBlock, *LPt8411InBlock;
#define NAME_t8411InBlock     "t8411InBlock"

// ���                           
typedef struct _t8411OutBlock
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�                        StartPos 0, Length 6
    char    jisiga              [   8];    char    _jisiga              ;    // [long  ,    8] ���Ͻð�                        StartPos 7, Length 8
    char    jihigh              [   8];    char    _jihigh              ;    // [long  ,    8] ���ϰ�                        StartPos 16, Length 8
    char    jilow               [   8];    char    _jilow               ;    // [long  ,    8] ��������                        StartPos 25, Length 8
    char    jiclose             [   8];    char    _jiclose             ;    // [long  ,    8] ��������                        StartPos 34, Length 8
    char    jivolume            [  12];    char    _jivolume            ;    // [long  ,   12] ���ϰŷ���                      StartPos 43, Length 12
    char    disiga              [   8];    char    _disiga              ;    // [long  ,    8] ���Ͻð�                        StartPos 56, Length 8
    char    dihigh              [   8];    char    _dihigh              ;    // [long  ,    8] ���ϰ�                        StartPos 65, Length 8
    char    dilow               [   8];    char    _dilow               ;    // [long  ,    8] ��������                        StartPos 74, Length 8
    char    diclose             [   8];    char    _diclose             ;    // [long  ,    8] ��������                        StartPos 83, Length 8
    char    highend             [   8];    char    _highend             ;    // [long  ,    8] ���Ѱ�                          StartPos 92, Length 8
    char    lowend              [   8];    char    _lowend              ;    // [long  ,    8] ���Ѱ�                          StartPos 101, Length 8
    char    cts_date            [   8];    char    _cts_date            ;    // [string,    8] ��������                        StartPos 110, Length 8
    char    cts_time            [  10];    char    _cts_time            ;    // [string,   10] ���ӽð�                        StartPos 119, Length 10
    char    s_time              [   6];    char    _s_time              ;    // [string,    6] ����۽ð�(HHMMSS)              StartPos 130, Length 6
    char    e_time              [   6];    char    _e_time              ;    // [string,    6] ������ð�(HHMMSS)              StartPos 137, Length 6
    char    dshmin              [   2];    char    _dshmin              ;    // [string,    2] ����ȣ��ó���ð�(MM:��)         StartPos 144, Length 2
    char    rec_count           [   7];    char    _rec_count           ;    // [long  ,    7] ���ڵ�ī��Ʈ                    StartPos 147, Length 7
} t8411OutBlock, *LPt8411OutBlock;
#define NAME_t8411OutBlock     "t8411OutBlock"

// ���1                          , occurs
typedef struct _t8411OutBlock
{
    char    date                [   8];    char    _date                ;    // [string,    8] ��¥                            StartPos 0, Length 8
    char    time                [  10];    char    _time                ;    // [string,   10] �ð�                            StartPos 9, Length 10
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�                            StartPos 20, Length 8
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��                            StartPos 29, Length 8
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����                            StartPos 38, Length 8
    char    close               [   8];    char    _close               ;    // [long  ,    8] ����                            StartPos 47, Length 8
    char    jdiff_vol           [  12];    char    _jdiff_vol           ;    // [long  ,   12] �ŷ���                          StartPos 56, Length 12
    char    jongchk             [  13];    char    _jongchk             ;    // [long  ,   13] ��������                        StartPos 69, Length 13
    char    rate                [   6];    char    _rate                ;    // [double,  6.2] ��������                        StartPos 83, Length 6
    char    pricechk            [  13];    char    _pricechk            ;    // [long  ,   13] �����ְ��ݿ��׸�                StartPos 90, Length 13
} t8411OutBlock, *LPt8411OutBlock;
#define NAME_t8411OutBlock     "t8411OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t8411_H_
