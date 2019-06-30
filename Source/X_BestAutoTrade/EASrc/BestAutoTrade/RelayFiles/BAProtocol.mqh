#ifndef _BA_PROTOCOL_H
#define _BA_PROTOCOL_H


#include "BARelayInc.mqh"
#include "../BAUtils.mqh"

//+--------------------------------------------------------+//
//  FIELD
//  1~99    : STRING
//  100~199 : INT
//  200~299 : DOUBLE
//+--------------------------------------------------------+//

#define DEF_DELI    0x01

#define FDS_CODE			1
#define FDS_COMMAND			2   //  ORDER, COMMAND
#define FDS_SYS				3   //  MT4, 
#define FDS_TM_HEADER		4   //  yyyymmdd_hhmmssmmm
#define FDS_BROKER			5
#define FDS_REGUNREG		6   //  R, U
#define FDS_SYMBOL			7
#define FDS_ORD_ACTION		8   // O:Open, C:Close, P:Partial
#define FDS_OPENED_TM		10
#define FDS_CLOSED_TM		11
#define FDS_COMMENTS		12
#define FDS_LIVEDEMO		13
#define FDS_USERID_MINE		14
#define FDS_USERID_MASTER	15
#define FDS_LOGONOFF_MSG    16

#define FDN_ACCNO_MY        100
#define FDN_ACCNO_MASTER    101

#define FDN_OPENED_TICKET   103
#define FDN_CLOSED_TICKET   104
#define FDN_ORD_TYPE        105 // OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP
#define FDN_PUBSCOPE_TP		106	// enum PUBSCOPE_TP { ALL = 0, SPECIFIC_MASTERID, SPECIFIC_SLAVEID };
#define FDN_OPENED_TM       107
#define FDN_CLOSED_TM       108


#define FDD_OPENED_PRC   200
#define FDD_CLOSED_PRC   201  
#define FDD_OPENED_LOTS  202
#define FDD_CLOSED_LOTS  203
#define FDD_SLPRC        204
#define FDD_PTPRC        205
#define FDD_PROFIT       206
#define FDD_SWAP         207
#define FDD_COMMISSION   208

#define MAX_BUF 1024
#define DOT_CNT 5


const string SYS_MT4	= "MT4";
const string SYS_NINJA	= "NINJA";
const string SYS_ZORRO	= "ZORRO";


const string CODE_REG_MASTER	= "1001";
const string CODE_REG_SLAVE		= "1002";
const string CODE_MASTER_ORDER	= "1003";
const string CODE_PUBLISH_ORDER	= "1004";
const string CODE_PING			= "1005";
const string CODE_LOGOFF		= "1006";
const string CODE_LOGON			= "1007";

const string TP_COMMAND = "C";
const string TP_ORDER   = "O";
const string TP_REG     = "R";
const string TP_UNREG   = "U";


class CProtoUtils
{
public:
    CProtoUtils(){}
    ~CProtoUtils(){}
    
    static string PacketCode(char& RecvPack[])
    {
        string sPacket = CharArrayToString(RecvPack);
        string sCode="";
        
        if( StringFind(sPacket, StringFormat("1=%s",CODE_PUBLISH_ORDER)))
            sCode = CODE_PUBLISH_ORDER;
        else if( StringFind(sPacket, StringFormat("1=%s",CODE_LOGOFF)))
            sCode = CODE_LOGOFF;
            
        return sCode;
    }
};

struct PackHeader
{
    string      sCode;
    string      sMasterId;
    string      sMyId;
    int         nMasterAcc;
    int         nMyAcc;
    datetime    dtHeader;
    string      sBroker;
    string      sSystem;
    PUBSCOPE_TP pubScope;
};

//enum PUBSCOPE_TP {
//		ALLSLAVES_UNDER_ONEMASTER,	//	All slaves under one master id
//		ONESLAVE_WITH_ID,			//	one Specific Slave
//		ALLMASTERS,					//	All Masters
//		ONEMASTER_WITH_ID,			//	one specific master
//		ALL
//	};
    
class CProtoGet
{
public:
    CProtoGet(string sMasterId, string sMyId);
    ~CProtoGet();
    int     SplitPacket(/*in*/char& RecvPack[], int nRecvLen, /*out*/string& result[]);
    virtual bool    ParsingPacket(/*in*/char& RecvPack[], int nRecvLen) = 0;

    bool    IsMyMaster()    { return (m_h.sMasterId==m_sMasterId);}
    bool    IsMyId()        { return (m_h.sMyId==m_sMyId);}
    
    int     MasterAcc()     { return m_h.nMasterAcc;}
    int     MyAcc()         { return m_h.nMyAcc;}
    string  MasterId()      { return m_h.sMasterId;}
    string  MyId()          { return m_h.sMyId;}
    PUBSCOPE_TP PubScope()  { return m_h.pubScope;}
    
    void    GetRecvBuff(string& s)  {s = m_sRecvPack;}
    void    GetMsg(string& s)       {s = m_sMsg;}

protected:
    string      m_sRecvPack;
    string      m_sMsg; 
    
protected:
    PackHeader  m_h;    
    string      m_sMasterId;
    string      m_sMyId;
};

CProtoGet::CProtoGet(string sMasterId, string sMyId)
{
    ZeroMemory(m_h);
    m_sMasterId = sMasterId;
    m_sMyId    = sMyId;
}
CProtoGet::~CProtoGet()
{
}

int CProtoGet::SplitPacket(/*in*/char& RecvPack[], int nRecvLen, /*out*/string& result[])
{
    ushort deli = DEF_DELI;
    
    m_sRecvPack = CharArrayToString(RecvPack);
    printlog(StringFormat("[RECV][%d](%s)", nRecvLen, m_sRecvPack));
        
    int nCnt = StringSplit(m_sRecvPack,deli,result);
    
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