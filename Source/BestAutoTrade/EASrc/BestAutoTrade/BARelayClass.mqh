//+------------------------------------------------------------------+
//|                                                  BANanoRelay.mqh |
//|                              Copyright 2019, BestAutoTrade Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, BestAutoTrade Corp."
#property link      "https://www.mql5.com"
#property strict

#ifndef _BARELAY_CLASS_H
#define _BARELAY_CLASS_H


#include "BARelayInc.mqh"
#include "bautils.mqh"
#include "BAProtocol.mqh"


struct CHANNEL_INFO
{
    int     sock;
    char    chName[];
    bool    bInit;
    int     timeout;
};

//char g_zRecvBuff[BUF_LEN];
    
class CBARelay
{
public:
    CBARelay();
    ~CBARelay();
    
    bool Initialize();
    void DeInitialize();
    
    bool RegisterAsMaster();
    
    bool RegisterAsSlave(string sMasterAcc, int nRecvTimeoutMs);
    bool UnRegisterAsSlave(string sMasterAcc, int nRecvTimeoutMs);
    bool InitSlave();
    
    int  RecvData(char& RecvBuff[], int nBuffLen);
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
    
    string GetMsg();
    
    
    
private:
    bool   RegUnregSlave(string sMasterAcc, int nRecvTimeoutMs, string sRegUnreg);
    string MasterChannelNm();
    string HeaderTime();
    string OrderTime();
    string AccTp();
private:

    CHANNEL_INFO    m_chToSrv;
    CHANNEL_INFO    m_chToMaster;

    int             m_nAccNo;
    int             m_nMasterAccNo;
    
    COPY_TP         m_copyTp;
    
    int             m_nRecvTimeoutMs;
    string          m_msg;    
    
    CProtoSet    m_ProtoSet;
};

string CBARelay::GetMsg()
{
    return m_msg;
}

string CBARelay::AccTp()
{
    string accTp;
    
    accTp    = "LIVE";
    if(IsDemo()) accTp = "DEMO";
    
    return accTp;
}


int  CBARelay::RecvData(char& RecvBuff[], int nBuffLen)
{
    if(!m_chToMaster.bInit)
        return Q_SUCCESS;
  
    int ret = nn_recv (m_chToMaster.sock, RecvBuff, nBuffLen, 0 );
    
    if (ret < 0)
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			return Q_TIMEOUT;
		}
		m_msg = StringFormat("Failed to recv: %s\n", nn_err_strerror(err));
		return Q_ERROR;
	}
    	
    //m_msg = StringFormat("DataRecv[%s]", CharArrayToString(RecvBuff));
    return ret;
}

string CBARelay::HeaderTime()
{
    char zTime[32];
    BAUtils_HeaderTime(zTime);
    m_msg = CharArrayToString(zTime);
    return m_msg;
    //return TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES|TIME_SECONDS);
}

string CBARelay::OrderTime()
{
    MqlDateTime tm;
    TimeCurrent(tm);
    string s;
    s = StringFormat("%02d%02d%02d", tm.hour, tm.min, tm.sec);
   
    return s;
}

string CBARelay::MasterChannelNm()
{
    return CharArrayToString (m_chToMaster.chName);
}

bool CBARelay::InitSlave()
{
    m_chToMaster.sock = nn_socket(AF_SP, NN_SUB);    
    if (m_chToMaster.sock == -1) {
		m_msg = StringFormat( "Slave Channel create Failed:%s", nn_err_strerror(nn_errno()));
		return false;
	}
	
    int val=0;   
    int ret = nn_setsockopt(m_chToMaster.sock, NN_SUB, NN_SUB_SUBSCRIBE, val, 0);
	if (ret == -1) {
		m_msg = StringFormat( "nn_setsockopt for subscribe Failed:%s", nn_err_strerror(nn_errno()));
		return false;
	}
	
	string sChannelNm = StringFormat("ipc://MASTER_%d", m_nMasterAccNo);
	StringToCharArray(sChannelNm, m_chToMaster.chName);
	
	ret = nn_connect(m_chToMaster.sock, m_chToMaster.chName);
	if (ret < 0){
		m_msg = StringFormat( "Connect Master Channel Failed:%s", nn_err_strerror(nn_errno()));
		return false;
	}
	
	m_msg = StringFormat("[InitSlave]Connect to Master Channel(Channel:%s) (Symbol:%s) (Broker:%s) (Acc:%d)[%s] ", 
	                MasterChannelNm(), Symbol(), AccountInfoString(ACCOUNT_COMPANY), AccountNumber(), AccTp());
    printlog(m_msg);

    return true;
}

bool CBARelay::RegisterAsSlave(string sMasterAcc, int nRecvTimeoutMs)
{
    return RegUnregSlave(sMasterAcc, nRecvTimeoutMs, TP_REG);
}


bool CBARelay::UnRegisterAsSlave(string sMasterAcc, int nRecvTimeoutMs)
{
    return RegUnregSlave(sMasterAcc, nRecvTimeoutMs, TP_UNREG);
}

bool CBARelay::RegUnregSlave(string sMasterAcc, int nRecvTimeoutMs, string sRegUnreg)
{
    m_nMasterAccNo = (int)StringToInteger(sMasterAcc);
    m_nRecvTimeoutMs = nRecvTimeoutMs;
    
    if(!InitSlave())
        return false;
    
    m_ProtoSet.Begin();
    m_ProtoSet.SetVal(FDS_CODE, CODE_REG_SLAVE);
    m_ProtoSet.SetVal(FDS_COMMAND, TP_COMMAND);
    m_ProtoSet.SetVal(FDS_SYS, "");    
    m_ProtoSet.SetVal(FDN_ACCNO_MY, AccountNumber());    
    m_ProtoSet.SetVal(FDS_TM_HEADER, HeaderTime());
    m_ProtoSet.SetVal(FDS_BROKER, AccountInfoString(ACCOUNT_COMPANY));
    m_ProtoSet.SetVal(FDS_REGUNREG, sRegUnreg);
    m_ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
	
    string result;
	int nSend = m_ProtoSet.Complete(result);
	char zBuffer[];
    ArrayResize(zBuffer, nSend);
    StringToCharArray(result, zBuffer);
    
	int ret = nn_send(m_chToSrv.sock, zBuffer, nSend, 0);
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
	
	
	m_msg = StringFormat("Reg Slave(%s)", result);
	printlog(m_msg);
	
	m_chToMaster.bInit = true;
	m_copyTp = TP_SLAVE;
	return (ret>0);

}


bool CBARelay::SendMasterOrder(
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
    
    m_ProtoSet.SetVal(FDS_CODE, CODE_MASTER_ORDER);
    m_ProtoSet.SetVal(FDS_COMMAND, TP_ORDER);
    m_ProtoSet.SetVal(FDS_SYS, "");    
    m_ProtoSet.SetVal(FDN_ACCNO_MY, AccountNumber());    
    m_ProtoSet.SetVal(FDS_TM_HEADER, HeaderTime());
    m_ProtoSet.SetVal(FDS_BROKER, AccountInfoString(ACCOUNT_COMPANY));
    
    m_ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
    m_ProtoSet.SetVal(FDN_OPENED_TICKET, nTicket);    
    m_ProtoSet.SetVal(FDS_SYMBOL, sSymbol);    
    m_ProtoSet.SetVal(FDS_ORD_ACTION, sAction);    
    m_ProtoSet.SetVal(FDN_ORD_TYPE, nType);
    
    if(sAction=="O"){
        m_ProtoSet.SetVal(FDD_OPENED_PRC, dOrdPrc);
        m_ProtoSet.SetVal(FDD_OPENED_LOTS, dLots);
        m_ProtoSet.SetVal(FDN_OPENED_TM, (int)dtOrdTm);
    }
    else{
        m_ProtoSet.SetVal(FDD_CLOSED_PRC, dOrdPrc);
        m_ProtoSet.SetVal(FDN_CLOSED_TM, (int)dtOrdTm);
        m_ProtoSet.SetVal(FDD_OPENED_PRC, dOpenedPrc);
        m_ProtoSet.SetVal(FDD_OPENED_LOTS, dOpenedLots);
        m_ProtoSet.SetVal(FDN_OPENED_TICKET, nOpenedTicket);
        m_ProtoSet.SetVal(FDN_OPENED_TM, (int)dtOpenedTm);
    }
    
	m_ProtoSet.SetVal(FDD_SLPRC, dSLPrc);
	m_ProtoSet.SetVal(FDD_PTPRC, dPTPrc);
	m_ProtoSet.SetVal(FDD_PROFIT, dProfit);
	m_ProtoSet.SetVal(FDD_SWAP, dSwap);
	m_ProtoSet.SetVal(FDS_COMMENTS, sComments);
	
	
	string result;
	int nSend = m_ProtoSet.Complete(result);
	char zBuffer[];
    ArrayResize(zBuffer, nSend);
    StringToCharArray(result, zBuffer);
    
	int ret = nn_send(m_chToSrv.sock, zBuffer, nSend, 0);
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
	
	m_msg = StringFormat( "[MASTER ORD](%s)", result);
	
	m_copyTp = TP_MASTER;
	return (ret>0);
}


CBARelay::CBARelay()
{
    ZeroMemory(m_chToSrv);
    ZeroMemory(m_chToMaster);
    
    m_copyTp = TP_NONE;
}

CBARelay::~CBARelay()
{
    DeInitialize();
}

void CBARelay::DeInitialize()
{
    if(m_chToSrv.sock>0){
        nn_close(m_chToSrv.sock);
        m_chToSrv.sock = -1;
    }
    if(m_chToMaster.sock>0){
        nn_close(m_chToMaster.sock);
        m_chToMaster.sock = -1;
    }
}

bool CBARelay::Initialize()
{
    m_nAccNo = AccountNumber();
    m_nMasterAccNo = m_nAccNo;
    
    m_chToSrv.sock = nn_socket(AF_SP, NN_PUSH); 
    if (m_chToSrv.sock == -1) {
        m_msg = StringFormat( "Server Channel create Failed:%s", nn_err_strerror(nn_errno()));
		return false;
	}

	StringToCharArray(RELAY_CHANNEL_NM, m_chToSrv.chName);
	int ret = nn_connect(m_chToSrv.sock, m_chToSrv.chName);
	if (ret < 0){
		m_msg = StringFormat( "Connect Server Channel Failed:%s", nn_err_strerror(nn_errno()));
		return false;
	}

	int nTimeout = SEND_TIMEOUT_MS;
    ret = nn_setsockopt(m_chToSrv.sock, NN_SOL_SOCKET, NN_SNDTIMEO, nTimeout, sizeof(nTimeout));
	if (ret == -1) {
		m_msg = StringFormat( "nn_setsockopt for send timeout Failed:%s", nn_err_strerror(nn_errno()));
		return false;
	}

	m_msg = StringFormat("Initialize Server Channel Ok(Channel:%s) (SendTimeout:%d ms) (Symbol:%s) (Broker:%s) (Acc:%d)[%s] ", 
	                RELAY_CHANNEL_NM, nTimeout, Symbol(), AccountInfoString(ACCOUNT_COMPANY), m_nAccNo, AccTp());
	return true;
}


bool CBARelay::RegisterAsMaster()
{
    m_ProtoSet.Begin();
    
    m_ProtoSet.SetVal(FDS_CODE, CODE_REG_MASTER);
    m_ProtoSet.SetVal(FDS_COMMAND, TP_COMMAND);
    m_ProtoSet.SetVal(FDS_SYS, "");    
    m_ProtoSet.SetVal(FDN_ACCNO_MY, AccountNumber());    
    m_ProtoSet.SetVal(FDS_TM_HEADER, HeaderTime());
    m_ProtoSet.SetVal(FDS_BROKER, AccountInfoString(ACCOUNT_COMPANY));
    m_ProtoSet.SetVal(FDS_REGUNREG, TP_REG);
    m_ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
	
	string result;
	int nSend = m_ProtoSet.Complete(result);
	char zBuffer[];
    ArrayResize(zBuffer, nSend);
    StringToCharArray(result, zBuffer);
    
    m_msg = StringFormat("RegMaster(%s)", result);
    
	int ret = nn_send(m_chToSrv.sock, zBuffer, nSend, 0);
	if (ret < 0)
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			return false;
		}
		//Alert("send error");
		m_msg = StringFormat( "Failed to send:%s", nn_err_strerror(err));
		return false;
	}
	
	//m_msg = StringFormat("RegMaster(%s)", result);
	
	m_copyTp = TP_MASTER;
	m_chToSrv.bInit = true;
	
	return true;

}




#endif // _BARELAY_CLASS_H