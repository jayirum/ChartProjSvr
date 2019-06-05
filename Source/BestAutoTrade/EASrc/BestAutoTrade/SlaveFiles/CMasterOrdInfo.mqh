#ifndef CMASTER_ORD_H
#define CMASTER_ORD_H

/*
    Parsing the Master Order Information received
*/

/*
OP_BUY - buy order,
OP_SELL - sell order,
OP_BUYLIMIT - buy limit pending order,
OP_BUYSTOP - buy stop pending order,
OP_SELLLIMIT - sell limit pending order,
OP_SELLSTOP - sell stop pending order.

sAction           // O:Open, C:Close, M:Modify
*/
#include "../RelayFiles/BAProtocol.mqh"
#include "../BAUtils.mqh"

class CMasterOrdInfo : public CProtoGet
{
public:
    CMasterOrdInfo(string sMasterId, string sMyId);
    ~CMasterOrdInfo();
    
    bool    ParsingPacket(char& RecvPack[], int nRecvLen);
    
    bool    IsCloseOrder() { return(m_sOrdAction=="C");};
    bool    IsModifyOrder(){ return(m_sOrdAction=="M");};
    bool    IsNewOrder()   { return(m_sOrdAction=="O");};
    
    int     MasterTicket()  { return m_nMasterTicket;}
    string  SymbolCode()    { return m_sSymbol;}
    int     OrdType()       { return m_nOrdType;}
    string  OrdAction()     { return m_sOrdAction;}
    double  OrdLots();       
    double  OrdPrc();
    
private:
    //string  m_sMasterId;
    //int     m_nMasterAcc;
    int     m_nMasterTicket;
    string  m_sOrdAction;
    int		m_nOrdType;
    double	m_dOpenLots;
    int     m_nOpenTm;
    double	m_dOpenPrc;;
    double	m_dSLPrc;
    double	m_dPTPrc;
    double  m_dCloseLots;
    int 	m_nCloseTm;
    double	m_dClosePrc;
    string  m_sComment;
    string  m_sSymbol;

};

CMasterOrdInfo::CMasterOrdInfo(string sMasterId, string sMyId):CProtoGet(sMasterId, sMyId)
{
    m_nOrdType      = -1;
    m_dOpenLots     = 0;
    m_nOpenTm       = 0;
    m_dOpenPrc      = 0;
    m_dSLPrc        = 0;
    m_dPTPrc        = 0;
    m_dCloseLots    = 0;
    m_nCloseTm      = 0;
    m_dClosePrc     = 0;
}

CMasterOrdInfo::~CMasterOrdInfo()
{
}


double  CMasterOrdInfo::OrdLots()
{
    if(m_sOrdAction=="O")
        return m_dOpenLots;
    else
        return m_dCloseLots;
}


double  CMasterOrdInfo::OrdPrc()
{
    if(m_sOrdAction=="O")
        return m_dOpenPrc;
    else
        return m_dClosePrc;
}

bool CMasterOrdInfo::ParsingPacket(char& RecvPack[], int nRecvLen)
{
    /*
        1=1004/2=C/ array
    */
    string result[];
    int nCnt = CProtoGet::SplitPacket(RecvPack, nRecvLen, result);
        
    int nCode;
    string unit[];
    ushort equal = StringGetCharacter("=",0);
    for(int i=0; i<nCnt; i++)
    {
        int kCnt = StringSplit(result[i], equal, unit);
        if(kCnt!=2)
        {
            m_sMsg = StringFormat("[i:%d, k:%d]Packet Unit error(%s)", i, kCnt, result[i]);
            printlog(m_sMsg);
            continue;
        }
        nCode =  (int)StringToInteger(unit[0]);
        switch(nCode)
        {
        case FDS_USERID_MASTER:
            m_h.sMasterId = unit[1];
            printlog(StringFormat("[MASTERORD]FDS_USERID_MASTER:%s",m_h.sMasterId));
            if( !CProtoGet::IsMyMaster() ){
                printlog(StringFormat("[PACK MASTER:%s][MY MASTER:%s]",m_h.sMasterId, m_sMasterId));
                return false;
            }
            break;
        case FDN_ACCNO_MASTER://     101:
            m_h.nMasterAcc = (int)StringToInteger(unit[1]);
            printlog(StringFormat("[MASTERORD]FDN_ACCNO_MASTER:%d",m_h.nMasterAcc));
            break;
        case FDN_OPENED_TICKET:
            m_nMasterTicket = (int)StringToInteger(unit[1]);
            printlog(StringFormat("[MASTERORD]FDN_OPENED_TICKET:%d",m_nMasterTicket));
            break;
        case FDS_SYMBOL: 
            m_sSymbol = unit[1];
            break;
        case FDS_ORD_ACTION:
            m_sOrdAction = unit[1];
            break;
        case FDN_ORD_TYPE:
            m_nOrdType = (int)StringToInteger(unit[1]);
            printlog(StringFormat("[MASTERORD]FDN_ORD_TYPE:%d",m_nOrdType));
            break;
        case FDD_OPENED_PRC:
            m_dOpenPrc = StringToDouble(unit[1]);
            printlog(StringFormat("[MASTERORD]FDD_OPENED_PRC:%f",m_dOpenPrc));
            break;
        case FDD_OPENED_LOTS:
            m_dOpenLots = StringToDouble(unit[1]);
            printlog(StringFormat("[MASTERORD]FDD_OPENED_LOTS:%f",m_dOpenLots));
            break;
        case FDN_OPENED_TM:
            m_nOpenTm = (int)StringToInteger(unit[1]);
            break;
        case FDD_CLOSED_PRC:
            m_dClosePrc = StringToDouble(unit[1]);
            printlog(StringFormat("[MASTERORD]FDD_CLOSED_PRC:%f",m_dClosePrc));
            break;
        case FDD_CLOSED_LOTS:
            m_dCloseLots = StringToDouble(unit[1]);
            printlog(StringFormat("[MASTERORD]FDD_CLOSED_LOTS:%f",m_dCloseLots));
            break;
        case FDN_CLOSED_TM:
            m_nCloseTm = (int)StringToInteger(unit[1]);
            break;
        case FDD_SLPRC:
            m_dSLPrc = StringToDouble(unit[1]);
            break;
        case FDD_PTPRC:
            m_dPTPrc = StringToDouble(unit[1]);
            break;
        case FDS_COMMENTS:
            m_sComment = unit[1];
            break;
        }//switch(nCode)
    }//for(int i=0; i<nCnt; i++)

    //printlog(StringFormat("(MasterAcc:%d)(MasterTicket:%d)(SYMBOL:%s)(ORD_ACTION:%s)(ORD_TYPE:%d)", m_nMasterAcc, m_nMasterTicket, m_sSymbol, m_sOrdAction , m_nOrdType));
    //printlog(StringFormat("(O_PRC:%.5f)(O_LOTS:%.5f)(O_TM:%d)(C_PRC:%.5f)(C_LOTS:%.5f)(C_TM:%d)(SLPRC:%f)(PTPRC:%f)(COMMENT:%s)",m_dOpenPrc, m_dOpenLots, m_nOpenTm, m_dClosePrc, m_dCloseLots, m_nCloseTm, m_nCloseTm, m_dSLPrc, m_dPTPrc, m_sComment));
        
        
    //printlog(m_sMsg);

    return true;
}






#endif // CMASTER_ORD_H