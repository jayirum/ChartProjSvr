
#ifndef _BARELAY_CLASS_SLAVE_H
#define _BARELAY_CLASS_SLAVE_H


#include "CBARelayBase.mqh"

    
class CBARelaySlave : public CBARelayBase
{
public:
    CBARelaySlave();
    ~CBARelaySlave();
    
    bool RegisterAsSlave(string sMasterAcc);
    bool UnRegisterAsSlave(string sMasterAcc);
    
    
    
    //+----------------------------------------
    //BAE CLASS
    //bool Initialize();
    //void DeInitialize();    
    //int  RecvData(char& RecvBuff[], int nBuffLen);
    //string GetMsg();
    //+----------------------------------------
    
    
    
private:
    bool   RegUnregSlave(string sMasterAcc, string sRegUnreg);
    
    //+----------------------------------------
    //BAE CLASS
    //string MasterChannelNm();
    //string HeaderTime();
    //string OrderTime();
    //string AccTp();
    //+----------------------------------------
    private:

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

CBARelaySlave::CBARelaySlave()
{
}

CBARelaySlave::~CBARelaySlave()
{
}

bool CBARelaySlave::RegisterAsSlave(string sMasterAcc)
{
    return RegUnregSlave(sMasterAcc,TP_REG);
}


bool CBARelaySlave::UnRegisterAsSlave(string sMasterAcc)
{
    return RegUnregSlave(sMasterAcc, TP_UNREG);
}

bool CBARelaySlave::RegUnregSlave(string sMasterAcc, string sRegUnreg)
{
    m_nMasterAccNo = (int)StringToInteger(sMasterAcc);
    
    
    m_ProtoSet.Begin();
    m_ProtoSet.SetVal(FDS_CODE,             CODE_REG_SLAVE);    
    m_ProtoSet.SetVal(FDS_COMMAND,          TP_COMMAND);
    m_ProtoSet.SetVal(FDS_SYS,              "MT4");    
    m_ProtoSet.SetVal(FDS_BROKER,           AccountInfoString(ACCOUNT_COMPANY));
    m_ProtoSet.SetVal(FDS_TM_HEADER,        HeaderTime());
    m_ProtoSet.SetVal(FDS_USERID_MINE,      m_sMyId);
    m_ProtoSet.SetVal(FDS_USERID_MASTER,    m_sMasterId);
    m_ProtoSet.SetVal(FDN_ACCNO_MY,         m_nMyAccNo);    
    m_ProtoSet.SetVal(FDN_ACCNO_MASTER,     m_nMasterAccNo);
    
    m_ProtoSet.SetVal(FDS_REGUNREG,         sRegUnreg);
	
	
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
    	    printlog("RegUnregSlave timeout");
    		return false;
    	}
    	m_msg = StringFormat( "Failed to send: %s\n", nn_err_strerror(err));
    	return false;
    }
	
	
	m_msg = StringFormat("Reg Slave(%s)", result);
	printlog(m_msg);
	
	m_chSend.bInit = true;
	m_copyTp = TP_SLAVE;
	return (ret>0);

}




#endif // _BARELAY_CLASS_H