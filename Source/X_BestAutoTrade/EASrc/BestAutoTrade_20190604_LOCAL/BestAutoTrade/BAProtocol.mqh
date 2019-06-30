#ifndef _BA_PROTOCOL_H
#define _BA_PROTOCOL_H


//+--------------------------------------------------------+//
//  FIELD
//  1~99    : STRING
//  100~199 : INT
//  200~299 : DOUBLE
//+--------------------------------------------------------+//

#define DEF_DELI    0x01

#define FDS_CODE     1
#define FDS_COMMAND  2   //  ORDER, COMMAND
#define FDS_SYS      3   //  MT4, 
#define FDS_TM_HEADER  4   //  yyyymmdd_hhmmssmmm
#define FDS_BROKER   5
#define FDS_REGUNREG 6   //  R, U
#define FDS_SYMBOL   7
#define FDS_ORD_ACTION   8   // O:Open, C:Close, P:Partial
#define FDS_COMMENTS     9
#define FDS_LIVEDEMO     10

#define FDN_ACCNO_MY         100
#define FDN_ACCNO_MASTER     101
//102
#define FDN_OPENED_TICKET    103
#define FDN_CLOSED_TICKET    104
#define FDN_ORD_TYPE         105 // OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP
#define FDN_OPENED_TM    106
#define FDN_CLOSED_TM    117



#define FDD_OPENED_PRC   200
#define FDD_CLOSED_PRC   201  
#define FDD_OPENED_LOTS  202
#define FDD_CLOSED_LOTS  203
#define FDD_SLPRC        204
#define FDD_PTPRC        205
#define FDD_PROFIT       205
#define FDD_SWAP         206
#define FDD_COMMISSION   207

#define MAX_BUF 1024
#define DOT_CNT 5


const string SYS_MT4	= "MT4";
const string SYS_NINJA	= "NINJA";
const string SYS_ZORRO	= "ZORRO";


const string CODE_REG_MASTER	= "1001";
const string CODE_REG_SLAVE		= "1002";
const string CODE_MASTER_ORDER	= "1003";
const string CODE_PUBLISH_ORDER	= "1004";

const string TP_COMMAND = "C";
const string TP_ORDER = "O";
const string TP_REG = "R";
const string TP_UNREG = "U";

class CProtoGet
{
public:
    CProtoGet();
    ~CProtoGet();
    static int ParsingPack(/*in*/string& sPacket, /*out*/string& result[]);
};

int CProtoGet::ParsingPack(/*in*/string& sPacket, /*out*/string& result[])
{
    ushort deli = DEF_DELI;
    
    int nCnt = StringSplit(sPacket,deli,result);
    
    return nCnt;
};


class CProtoSet
{
public:
    CProtoSet();
    ~CProtoSet();
    
    void Begin();
    int Complete(/*out*/string& result);
    
    void SetVal(int nFd, string val);
    void SetVal(int nFd, int val);
    void SetVal(int nFd, double val);
    
private:
    string m_sBuf;
    string m_sTemp;
};




CProtoSet::CProtoSet()
{
}

CProtoSet::~CProtoSet()
{
}

void CProtoSet::Begin(void)
{
    ZeroMemory(m_sBuf);
}

int CProtoSet::Complete(string &result)
{
    result = m_sBuf;
    return StringLen(result);
}

void CProtoSet::SetVal(int nFd,int val)
{
    m_sTemp = StringFormat("%d=%d", nFd, val);
    StringAdd(m_sBuf, StringSetChar(m_sTemp, StringLen(m_sTemp), 0x01));
}


void CProtoSet::SetVal(int nFd,string val)
{
    if(StringLen(val)==0)
        return;
    m_sTemp = StringFormat("%d=%s", nFd, val);
    StringAdd(m_sBuf, StringSetChar(m_sTemp, StringLen(m_sTemp), 0x01));
}


void CProtoSet::SetVal(int nFd,double val)
{
    m_sTemp = StringFormat("%d=%.5f", nFd, val);
    StringAdd(m_sBuf, StringSetChar(m_sTemp, StringLen(m_sTemp), 0x01));
}




////////////////////////////////////////////////////////////////



#endif //_BA_PROTOCOL_H