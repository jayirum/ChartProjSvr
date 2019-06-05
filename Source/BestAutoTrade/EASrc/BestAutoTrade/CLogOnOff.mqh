#ifndef SLAVE_LOGOFF_H
#define SLAVE_LOGOFF_H





#include "./RelayFiles/BAProtocol.mqh"
#include "BAUtils.mqh"

class CLogOnOff : public CProtoGet
{
public:
    CLogOnOff(string sMasterId, string sMyId);
    ~CLogOnOff();
    
    bool    ParsingPacket(char& RecvPack[], int nRecvLen);
    
    bool    IsLogOn() { return (m_h.sCode==CODE_LOGON);}
    bool    IsLogOff() { return (m_h.sCode==CODE_LOGOFF);}
    
    virtual bool    IsMyLogOff() = 0;
    virtual bool    IsMyLogOn() = 0;
    
    string  GetLogOffMsg();
    
private:
    string  m_sLogOnOffMsg;
};

class CSlaveLogOnOff : public CLogOnOff
{
public:
    CSlaveLogOnOff(string sMasterId, string sMyId):CLogOnOff(sMasterId, sMyId){}
    ~CSlaveLogOnOff(){};
    
    bool    IsMyLogOff();
    bool    IsMyLogOn();
};



class CMasterLogOnOff : public CLogOnOff
{
public:
    CMasterLogOnOff(string sMasterId, string sMyId):CLogOnOff(sMasterId, sMyId){}
    ~CMasterLogOnOff(){};
    
    bool    IsMyLogOff();
    bool    IsMyLogOn();
};



CLogOnOff::CLogOnOff(string sMasterId, string sMyId):CProtoGet(sMasterId, sMyId)
{
}
CLogOnOff::~CLogOnOff()
{
}
    

/*
enum PUBSCOPE_TP {
	ALLSLAVES_UNDER_ONEMASTER,	//	All slaves under one master id
	ONESLAVE_WITH_ID,			//	one Specific Slave
	ALLMASTERS,					//	All Masters
	ONEMASTER_WITH_ID,			//	one specific master
	ALL
};
*/
bool CLogOnOff::ParsingPacket(char& RecvPack[], int nRecvLen)
{
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
        case FDS_CODE:
            m_h.sCode = unit[1];
            printlog(StringFormat("[LOGONOFF]FDS_CODE:%s",m_h.sCode));
            break; 
        case FDS_USERID_MASTER:
            m_h.sMasterId = unit[1];
            printlog(StringFormat("[LOGONOFF]FDS_USERID_MASTER:%s",m_h.sMasterId));
            break; 
        case FDS_USERID_MINE:
            m_h.sMyId = unit[1];
            printlog(StringFormat("[LOGONOFF]FDS_USERID_MINE:%s",m_h.sMyId));
            break;
        case FDN_PUBSCOPE_TP:
            m_h.pubScope = (PUBSCOPE_TP)StringToInteger(unit[1]);
            printlog(StringFormat("[LOGONOFF]FDN_PUBSCOPE_TP:%d",m_h.pubScope));
            break;
        case FDS_LOGONOFF_MSG:
            m_sLogOnOffMsg = unit[1];
            printlog(StringFormat("[LOGONOFF]FDS_LOGOFF_MSG:%s",m_sLogOnOffMsg));
            break; 
        }//switch(nCode)
    }//for(int i=0; i<nCnt; i++)

    
   
    
    return false;
}

bool CSlaveLogOnOff::IsMyLogOff()
{
    if( !IsLogOff() )
        return false;
    
    if( (m_h.pubScope==PUBSCOPE_TP::ALL) || (m_h.pubScope==PUBSCOPE_TP::ALLMASTERS))
    {
        return true;
    }    
    else if( m_h.pubScope==PUBSCOPE_TP::ONESLAVE_WITH_ID )
    {
        return CProtoGet::IsMyId();
    }
    
    else if( m_h.pubScope==PUBSCOPE_TP::ALLSLAVES_UNDER_ONEMASTER )
    {
        return CProtoGet::IsMyMaster();
    }
    return false;
}



bool CSlaveLogOnOff::IsMyLogOn()
{
    if( !IsLogOn() )
        return false;
    
    if( m_h.pubScope==PUBSCOPE_TP::ALL) 
    {
        return true;
    }    
    else if( m_h.pubScope==PUBSCOPE_TP::ONESLAVE_WITH_ID )
    {
        return CProtoGet::IsMyId();
    }
    
    return false;
}


bool CMasterLogOnOff::IsMyLogOff()
{
    if( !IsLogOff() )
        return false;
    
    if( (m_h.pubScope==PUBSCOPE_TP::ALL) || (m_h.pubScope==PUBSCOPE_TP::ALLMASTERS))
    {
        return true;
    }    
    else if( m_h.pubScope==PUBSCOPE_TP::ONEMASTER_WITH_ID )
    {
        return CProtoGet::IsMyId();
    }
    
    return false;
}



bool CMasterLogOnOff::IsMyLogOn()
{
    if( !IsLogOn() )
        return false;
    
    if( m_h.pubScope==PUBSCOPE_TP::ALL) 
    {
        return true;
    }    
    else if( m_h.pubScope==PUBSCOPE_TP::ONEMASTER_WITH_ID )
    {
        return CProtoGet::IsMyId();
    }
    
    return false;
}

string  CLogOnOff::GetLogOffMsg()
{
    return m_sLogOnOffMsg;
}



























#endif //SLAVE_LOGOFF_H
