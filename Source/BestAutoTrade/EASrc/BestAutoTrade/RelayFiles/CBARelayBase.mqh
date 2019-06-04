

#ifndef _BARELAY_CLASS_H
#define _BARELAY_CLASS_H


#include "BARelayInc.mqh"
#include "../bautils.mqh"
#include "BAProtocol.mqh"


#define  TIMEOUT_MS_SEND   100
#define  TIMEOUT_MS_RECV   10

struct CHANNEL_INFO
{
    int     sock;
    int     endpoint;
    char    chName[];
    bool    bInit;
    int     timeout;
};

//char g_zRecvBuff[BUF_LEN];
    
class CBARelayBase
{
public:
    CBARelayBase();
    ~CBARelayBase();
    
    bool InitChannel(string sMyId, string sMasterId, string sSendChannel, string sRecvChannel);
    void DeInitChannel();
        
    int  RecvData(char& RecvBuff[], int nBuffLen);
    void Ping();
    string GetMsg(){return m_msg;}
    
protected:
    bool    InitSendChannel();
    bool    InitRecvChannel();
        
    string  HeaderTime();
    string  OrderTime();
    string  AccTp();
protected:

    CHANNEL_INFO    m_chSend;
    CHANNEL_INFO    m_chRecv;
    COPY_TP         m_copyTp;
    CProtoSet       m_ProtoSet;

    int             m_nMyAccNo;
    string          m_sMyId;
    int             m_nMasterAccNo;
    string          m_sMasterId;
    string          m_sSendChannel;
    string          m_sRecvChannel;
    
    string          m_msg;
};


CBARelayBase::CBARelayBase()
{
    ZeroMemory(m_chSend);
    ZeroMemory(m_chRecv);
    
    m_copyTp = TP_NONE;
}

CBARelayBase::~CBARelayBase()
{
    DeInitChannel();
}


void CBARelayBase::DeInitChannel()
{
   if(m_chSend.sock>0){
      nn_shutdown(m_chSend.sock, m_chSend.endpoint);
      nn_close(m_chSend.sock);
      m_chSend.sock = -1;
   }
   if(m_chRecv.sock>0){
      nn_shutdown(m_chRecv.sock, m_chRecv.endpoint);
      nn_close(m_chRecv.sock);
      m_chRecv.sock = -1;
   }
}

bool CBARelayBase::InitChannel(string sMyId, string sMasterId, string sSendChannel, string sRecvChannel)
{
    m_nMyAccNo      = AccountNumber();
    m_sMyId         = sMyId;
    m_nMasterAccNo  = AccountNumber();
    m_sMasterId     = sMasterId;
    m_sSendChannel  = StringFormat("tcp://%s",sSendChannel);
    m_sRecvChannel  = StringFormat("tcp://%s",sRecvChannel);
        
    if(!InitSendChannel())
        return false;
    
    if(!InitRecvChannel())
        return false;
    
    printlog(StringFormat("InitChannel OK (%s)(%s)", m_sSendChannel, m_sRecvChannel));
    return true;
}

bool CBARelayBase::InitSendChannel()
{
    m_chSend.sock = nn_socket(AF_SP, NN_PUSH); 
    if (m_chSend.sock == -1) {
        m_msg = StringFormat( "Send Channel create Failed(%s):%s", m_sSendChannel, nn_err_strerror(nn_errno()));
		return false;
	}

	StringToCharArray(m_sSendChannel, m_chSend.chName);
	m_chSend.endpoint = nn_connect(m_chSend.sock, m_chSend.chName);
	if (m_chSend.endpoint < 0){
		m_msg = StringFormat( "Connect Server Channel Failed(%s):%s", m_sSendChannel, nn_err_strerror(nn_errno()));
		return false;
	}

	int nTimeout = TIMEOUT_MS_SEND;
    int ret = nn_setsockopt(m_chSend.sock, NN_SOL_SOCKET, NN_SNDTIMEO, nTimeout, sizeof(nTimeout));
	if (ret == -1) {
		m_msg = StringFormat( "nn_setsockopt for send timeout Failed:%s", nn_err_strerror(nn_errno()));
		return false;
	}

	m_msg = StringFormat("Server Channel Ok(Channel:%s) (SendTimeout:%d ms) (Symbol:%s) (Broker:%s) (MyAcc:%d)[%s] ", 
	                m_sSendChannel, nTimeout, Symbol(), AccountInfoString(ACCOUNT_COMPANY), m_nMyAccNo, AccTp());
	       
	m_chSend.bInit = true;
	         
    printlog(m_msg);
    
	return true;
}

bool CBARelayBase::InitRecvChannel()
{
    m_chRecv.sock = nn_socket(AF_SP, NN_SUB);    
    if (m_chRecv.sock == -1) {
        m_msg = StringFormat( "Receive Channel create Failed(%s):%s", m_sRecvChannel, nn_err_strerror(nn_errno()));
        return false;
    }
    
    int val=0;   
    int ret = nn_setsockopt(m_chRecv.sock, NN_SUB, NN_SUB_SUBSCRIBE, val, 0);
    if (ret == -1) {
        m_msg = StringFormat( "nn_setsockopt for subscribe Failed(%s):%s",m_sRecvChannel, nn_err_strerror(nn_errno()));
        return false;
    }
    
    StringToCharArray(m_sRecvChannel, m_chRecv.chName);
    
    m_chRecv.endpoint = nn_connect(m_chRecv.sock, m_chRecv.chName);
    if (m_chRecv.endpoint < 0){
        m_msg = StringFormat( "Connect Receive Channel Failed:%s", nn_err_strerror(nn_errno()));
        return false;
    }
    
    int nTimeout = TIMEOUT_MS_RECV;
    ret = nn_setsockopt(m_chRecv.sock, NN_SOL_SOCKET, NN_RCVTIMEO, nTimeout, sizeof(nTimeout));
    if (ret == -1) {
        m_msg = StringFormat( "nn_setsockopt for recv timeout Failed:%s", nn_err_strerror(nn_errno()));
        return false;
    }
    
    m_msg = StringFormat("Connect to Receive Channel(Channel:%s) (Symbol:%s) (Broker:%s) (Acc:%d)[%s] ", 
                            m_sRecvChannel, Symbol(), AccountInfoString(ACCOUNT_COMPANY), AccountNumber(), AccTp());
    printlog(m_msg);
    
    m_chRecv.bInit = true;
    
    return true;
}

string CBARelayBase::AccTp()
{
    string accTp;
    
    accTp    = "LIVE";
    if(IsDemo()) accTp = "DEMO";
    
    return accTp;
}


int  CBARelayBase::RecvData(char& RecvBuff[], int nBuffLen)
{
    if(!m_chRecv.bInit)
        return Q_SUCCESS;
  
    int ret = nn_recv (m_chRecv.sock, RecvBuff, nBuffLen, 0 );
    
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
    	
    printlog(StringFormat("DataRecv[%s]", CharArrayToString(RecvBuff)));
    return ret;
}

string CBARelayBase::HeaderTime()
{
    char zTime[32];
    BAUtils_HeaderTime(zTime);
    m_msg = CharArrayToString(zTime);
    return m_msg;
    //return TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES|TIME_SECONDS);
}

string CBARelayBase::OrderTime()
{
    MqlDateTime tm;
    TimeCurrent(tm);
    string s;
    s = StringFormat("%02d%02d%02d", tm.hour, tm.min, tm.sec);
   
    return s;
}



void CBARelayBase::Ping(void)
{
    m_ProtoSet.Begin();
    
    m_ProtoSet.SetVal(FDS_CODE,             CODE_PING);    
    m_ProtoSet.SetVal(FDS_COMMAND,          TP_COMMAND);
    m_ProtoSet.SetVal(FDS_SYS,              "MT4");    
    m_ProtoSet.SetVal(FDS_BROKER,           AccountInfoString(ACCOUNT_COMPANY));
    m_ProtoSet.SetVal(FDS_TM_HEADER,        HeaderTime());
    m_ProtoSet.SetVal(FDS_USERID_MINE,      m_sMyId);
    m_ProtoSet.SetVal(FDS_USERID_MASTER,    m_sMasterId);
    m_ProtoSet.SetVal(FDN_ACCNO_MY,         m_nMyAccNo);    
    m_ProtoSet.SetVal(FDN_ACCNO_MASTER,     m_nMasterAccNo);
	
	string  result;
	char    zBuffer[];
	int     nSend = m_ProtoSet.Complete(result);
	
    ArrayResize(zBuffer, nSend);
    StringToCharArray(result, zBuffer);
    
    printlog(StringFormat("Ping Try to Send(%s)(%d)", result, nSend));
    
	int ret = nn_send(m_chSend.sock, zBuffer, nSend, 0);
	if (ret < 0)
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			return ;
		}
		m_msg = StringFormat( "Failed to send:%s", nn_err_strerror(err));
		return ;
	}
	else
	    printlog(StringFormat("Ping Send OK (%s)", result));
	
	m_copyTp = TP_MASTER;
	m_chSend.bInit = true;
	
	return ;

}


#endif // _BARELAY_CLASS_H