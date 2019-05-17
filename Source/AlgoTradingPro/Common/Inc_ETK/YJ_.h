///////////////////////////////////////////////////////////////////////////////////////////////////
// ��������(YJ) ( attr,block )
#pragma pack( push, 1 )

#define NAME_YJ_     "YJ_"

// �Է�
typedef struct _YJ__InBlock
{
    char    upcode              [   3];    // [string,    3] �����ڵ�                       StartPos 0, Length 3
} YJ__InBlock, *LPYJ__InBlock;
#define NAME_YJ__InBlock     "InBlock"

// ���
typedef struct _YJ__OutBlock
{
    char    time                [   6];    char    _time                ;    // [string,    6] �ð�                           StartPos 0, Length 6
    char    jisu                [   8];    char    _jisu                ;    // [float ,  8.2] ��������                       StartPos 7, Length 8
    char    sign                [   1];    char    _sign                ;    // [string,    1] �������ϴ�񱸺�               StartPos 16, Length 1
    char    change              [   8];    char    _change              ;    // [float ,  8.2] �������Ϻ�                     StartPos 18, Length 8
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] ��������                     StartPos 27, Length 6
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ����ü�ᷮ                     StartPos 34, Length 8
    char    volume              [   8];    char    _volume              ;    // [long  ,    8] �����ŷ���                     StartPos 43, Length 8
    char    value               [   8];    char    _value               ;    // [long  ,    8] ����ŷ����                   StartPos 52, Length 8
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�                       StartPos 61, Length 3
} YJ__OutBlock, *LPYJ__OutBlock;
#define NAME_YJ__OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
