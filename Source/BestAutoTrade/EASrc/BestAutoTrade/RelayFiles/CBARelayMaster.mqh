
#ifndef _BARELAY_CLASS_MASTER_H
#define _BARELAY_CLASS_MASTER_H


#include "CBARelayBase.mqh"

    
class CBARelayMaster : public CBARelayBase
{
public:
    CBARelayMaster();
    ~CBARelayMaster();
    
    bool RegisterAsMaster();
    bool UnRegisterAsMaster();
    
    
    bool SendMasterOrder(
        string sMasterAccNo
        ,int    nTicket
        ,string sSymbol
        ,string sAction
        ,int    nType
        ,double dOrdPrc
        ,double dSLPrc
        ,double dPTPrc
        ,double dLots
        //,string sOrdTp
        ,datetime dtOrdTm
        ,int     nOpenedTicket
        ,double  dOpenedPrc
        ,double dOpenedLots
        ,datetime dtOpenedTm
        ,double dProfit
        ,double dSwap
        ,string sComments
    );
    
    //+----------------------------------------
    //BAE CLASS
    //bool Initialize();
    //void DeInitialize();
    //int  RecvData(char& RecvBuff[], int nBuffLen);
    //string GetMsg();
    //+----------------------------------------
    
    
    
private:
    bool   RegUnregMaster(string sMasterAcc, string sRegUnreg);
    
    
    //+----------------------------------------
    //BAE CLASS
    //string MasterChannelNm();
    //string HeaderTime();
    //string OrderTime();
    //string AccTp();
    //+----------------------------------------
private:

    

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBARelayMaster::CBARelayMaster()
{}
CBARelayMaster::~CBARelayMaster()
{
    UnRegisterAsMaster();
}



bool CBARelayMaster::SendMasterOrder(
        string      sMasterAccNo// char MasterAccNo    [20];
        ,int        nTicket     // char Ticket         [10];
        ,string     sSymbol     // char symbol         [10];
        ,string     sAction     // char Action         [1];				// O:Open, C:Close
        ,int        nType       // char Type           [1];				// OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP
        ,double     dOrdPrc     // char OrdPrc         [10];
        ,double     dSLPrc      // char SLPrc          [10];
        ,double     dPTPrc      // char PTPrc          [10];
        ,double     dLots       // char Lots           [7];
        //,string   sOrdTp    // char OrdTp          [2];		// MT, PD, SL, TP
        ,datetime   dtOrdTm   // char OrdTm          [6];
        ,int        nOpenedTicket// char OpendedTicket  [10];
        ,double     dOpenedPrc // char OpendedPrc     [10];
        ,double     dOpenedLots // char OpenedLots     [10];
        ,datetime   dtOpenedTm// char OpenedTm       [6];
        ,double     dProfit     // char Profit         [9];
        ,double     dSwap       // char Swap           [9];
        ,string     sComments
)
{

    m_ProtoSet.Begin();
    
    m_ProtoSet.SetVal(FDS_CODE,             CODE_MASTER_ORDER);
    m_ProtoSet.SetVal(FDS_COMMAND,          TP_ORDER);
    m_ProtoSet.SetVal(FDS_SYS,              "MT4");       
    m_ProtoSet.SetVal(FDS_BROKER,           AccountInfoString(ACCOUNT_COMPANY));
	m_ProtoSet.SetVal(FDS_USERID_MINE,      m_sMyId);
	m_ProtoSet.SetVal(FDS_USERID_MASTER,    m_sMasterId);
    m_ProtoSet.SetVal(FDN_ACCNO_MY,         AccountNumber());     
    m_ProtoSet.SetVal(FDN_ACCNO_MASTER,     m_nMasterAccNo);
    m_ProtoSet.SetVal(FDS_TM_HEADER,        HeaderTime());
    
    
    m_ProtoSet.SetVal(FDN_OPENED_TICKET,    nTicket);    
    m_ProtoSet.SetVal(FDS_SYMBOL,           sSymbol);    
    m_ProtoSet.SetVal(FDS_ORD_ACTION,       sAction);    
    m_ProtoSet.SetVal(FDN_ORD_TYPE,         nType);
    
    if(sAction=="O"){
        m_ProtoSet.SetVal(FDD_OPENED_PRC,   dOrdPrc);
        m_ProtoSet.SetVal(FDD_OPENED_LOTS,  dLots);
        m_ProtoSet.SetVal(FDN_OPENED_TM,    (int)dtOrdTm);
    }
    else{
        m_ProtoSet.SetVal(FDD_CLOSED_PRC,   dOrdPrc);
        m_ProtoSet.SetVal(FDD_CLOSED_LOTS,  dLots);
        m_ProtoSet.SetVal(FDN_CLOSED_TM,    (int)dtOrdTm);
        m_ProtoSet.SetVal(FDD_OPENED_PRC,   dOpenedPrc);
        m_ProtoSet.SetVal(FDD_OPENED_LOTS,  dOpenedLots);
        m_ProtoSet.SetVal(FDN_OPENED_TICKET, nOpenedTicket);
        m_ProtoSet.SetVal(FDN_OPENED_TM,    (int)dtOpenedTm);
    }
    
	m_ProtoSet.SetVal(FDD_SLPRC,    dSLPrc);
	m_ProtoSet.SetVal(FDD_PTPRC,    dPTPrc);
	m_ProtoSet.SetVal(FDD_PROFIT,   dProfit);
	m_ProtoSet.SetVal(FDD_SWAP,     dSwap);
	m_ProtoSet.SetVal(FDS_COMMENTS, sComments);
	
	
	string result;
	int nSend = m_ProtoSet.Complete(result);
	char zBuffer[];
    ArrayResize(zBuffer, nSend);
    StringToCharArray(result, zBuffer);
    
	int ret = nn_send(m_chSend.sock, zBuffer, nSend, 0);
	if (ret < 0)
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			return false;
		}
		m_msg = StringFormat( "Failed to send: %s\n", nn_err_strerror(err));
		return false;
	}
	
	m_msg = StringFormat( "[MASTER ORD PUB OK](%s)", result);
	
	m_copyTp = TP_MASTER;
	return (ret>0);
}



bool CBARelayMaster::RegisterAsMaster(void)
{
 //   string  result = "1234567890abcdefghijklmn";
	//char    zBuffer[128];
	//StringToCharArray(result, zBuffer);
 //   int ret = nn_send(m_chSend.sock, zBuffer, StringLen(result), 0);
	//if (ret < 0)
	//{
	//	int err = nn_errno();
	//	if (err == ETIMEDOUT)
	//	{
	//		return false;
	//	}
	//	m_msg = StringFormat( "Failed to send:%s", nn_err_strerror(err));
	//	return false;
	//}
	//else
	//    printlog(StringFormat("RegUnreg Send OK (%s)", result));
	    
    return RegUnregMaster(m_sMasterId,"R");
}

bool CBARelayMaster::UnRegisterAsMaster(void)
{
    return RegUnregMaster(m_sMasterId,"U");
}


bool CBARelayMaster::RegUnregMaster(string sMasterAcc,string sRegUnreg)
{
    m_ProtoSet.Begin();
    
    m_ProtoSet.SetVal(FDS_CODE,             CODE_REG_MASTER);    
    m_ProtoSet.SetVal(FDS_COMMAND,          TP_COMMAND);
    m_ProtoSet.SetVal(FDS_SYS,              "MT4");    
    m_ProtoSet.SetVal(FDS_BROKER,           AccountInfoString(ACCOUNT_COMPANY));
    m_ProtoSet.SetVal(FDS_TM_HEADER,        HeaderTime());
    m_ProtoSet.SetVal(FDS_USERID_MINE,      m_sMyId);
    m_ProtoSet.SetVal(FDS_USERID_MASTER,    m_sMasterId);
    m_ProtoSet.SetVal(FDN_ACCNO_MY,         m_nMyAccNo);    
    m_ProtoSet.SetVal(FDN_ACCNO_MASTER,     m_nMasterAccNo);
	
	m_ProtoSet.SetVal(FDS_REGUNREG,         sRegUnreg);
    
	string  result;
	char    zBuffer[];
	int     nSend = m_ProtoSet.Complete(result);
	
    ArrayResize(zBuffer, nSend);
    StringToCharArray(result, zBuffer);
    
    printlog(StringFormat("RegUnreg Try to Send(%s)(%d)", result, nSend));
    
	int ret = nn_send(m_chSend.sock, zBuffer, nSend, 0);
	if (ret < 0)
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			return false;
		}
		m_msg = StringFormat( "Failed to send:%s", nn_err_strerror(err));
		return false;
	}
	else
	    printlog(StringFormat("RegUnreg Send OK (%s)", result));
	
	m_copyTp = TP_MASTER;
	m_chSend.bInit = true;
	
	return true;

}




#endif // _BARELAY_CLASS_H