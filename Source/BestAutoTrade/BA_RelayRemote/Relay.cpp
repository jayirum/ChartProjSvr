#include "Relay.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"

extern char		g_zConfig[_MAX_PATH];
extern CLogMsg	g_log;

CRelay::CRelay()
{
	m_recvQ = NULL;
}


CRelay::~CRelay()
{
	End();
}

void CRelay::End()
{
	delete m_recvQ;
	delete m_connManager;
	delete m_pubMaster;
	delete m_pubSlave;
}
BOOL CRelay::Begin()
{
	if(! RecvQInit())
	{
		g_log.log(ERR, "[%s]RecvQ Begin Error(%s)\n", m_recvQ->ChannelNm(), m_recvQ->GetMsg());
		return FALSE;
	}
	else
		g_log.log(INFO, "RecvQ Pipe InitChannel OK[%s]\n", m_recvQ->ChannelNm());

	// connection manager
	m_connManager = new CConnManager;
	m_connManager->RegCallBackForCloseSess(this, (PResultCallBack)CallBackReturnClient);

	m_pubMaster = new CPubToMS;
	if (!m_pubMaster->InitChannel(CLIENT_TP::MASTR))
		return FALSE;

	m_pubSlave = new CPubToMS;
	if (!m_pubSlave->InitChannel(CLIENT_TP::SLAVE))
		return FALSE;

	return TRUE;
}

BOOL CRelay::RecvQInit() 
{
	if (!m_recvQ)
		m_recvQ = new CNanoQReader();

	char szIp[32], szPort[32], szChannel[32];
	CUtil::GetConfig(g_zConfig, "RELAY_CHANNEL_INFO", "RELAY_IP", szIp);
	CUtil::GetConfig(g_zConfig, "RELAY_CHANNEL_INFO", "PORT_RELAY_READQ", szPort);

	sprintf(szChannel, "%s:%s", szIp, szPort);

	if (!m_recvQ->Begin(szChannel, RECV_TIMEOUT_MS, TP_TCP, TRUE)) {
		g_log.log(ERR, "RecvQInit CNanoQReader.Begin error(%s)\n", m_recvQ->GetMsg());
		return FALSE;
	}

	if (!m_recvQ->Connect())
	{
		g_log.log(ERR, "RecvQInit CNanoQReader.Connect error(%s)\n", m_recvQ->GetMsg());
		return FALSE;
	} 
	
	m_recvQ->RegisterCallBack((LPVOID)this, (PRecvCallBack)CallBackRecvProc);
	return TRUE;
}


//enum PUBSCOPE_TP {
//	ALLSLAVES_UNDER_ONEMASTER,	//	All slaves under one master id
//	ONESLAVE_WITH_ID,			//	one Specific Slave
//	ALLMASTERS,					//	All Masters
//	ONEMASTER_WITH_ID,			//	one specific master
//	ALL
//};
void WINAPI CRelay::CallBackReturnClient(CLIENT_TP , void *pClassInstance, const char* pPacket, const int nLen)
{
	CRelay* pThis = (CRelay*)pClassInstance;

	pThis->m_pubSlave->PubData((char*)pPacket, nLen);

	delete[]pPacket;
}

// class instance pointer, recv data, return value
void WINAPI CRelay::CallBackRecvProc(void *pClassInstance, char *pRecvBuf, int nRecvLen)
{
	g_log.log(INFO,"[RECV][LEN:%d](%.*s)\n", nRecvLen, nRecvLen, pRecvBuf);

	CRelay* pThis = (CRelay*)pClassInstance;

	pThis->m_protoGet.Parsing(pRecvBuf, nRecvLen);
	string sCode;
	if (!pThis->m_protoGet.GetCode(sCode))
	{
		g_log.log(INFO, "CallBackRecvProc but there is no Code");
		return;
	}

	string sMyID;
	if (!pThis->m_protoGet.GetVal(FDS_USERID_MINE, &sMyID))
	{
		g_log.log(INFO, "MyID is not in the packet");
		return;
	}

	string sMasterId;
	if( !pThis->m_protoGet.GetVal(FDS_USERID_MASTER, &sMasterId) )
	{
		g_log.log(INFO, "Master ID is not in the packet");
		return;
	}
	
	if (sCode.compare(CODE_REG_MASTER)==0)
	{
		pThis->RegUnregMaster(sMasterId, sMyID);
	}
	else if (sCode.compare(CODE_REG_SLAVE) == 0) 
	{
		pThis->RegUnregSlave(sMasterId, sMyID);
	}
	else if (sCode.compare(CODE_PING) == 0)
	{
		pThis->UpdatePing(sMyID);
	}
	else if (sCode.compare(CODE_MASTER_ORDER) == 0)
	{
		pThis->PublishOrder( sMasterId, sMyID, pRecvBuf);
	}

}

VOID CRelay::PublishOrder(string sMasterId, string sMyID, char* pzRecvBuff)
{
	char		temp[128];
	char		zSendBuf[512];
	int			nSendLen;
	CProtoSet	ProtoSet;
	
	ProtoSet.CopyFromRecvData(m_protoGet.GetRecvData());
	ProtoSet.SetVal(FDS_SYS, "RELAY");
	ProtoSet.SetVal(FDS_TM_HEADER, Now(temp));
	ProtoSet.SetVal(FDN_PUBSCOPE_TP, PUBSCOPE_TP::ALLSLAVES_UNDER_ONEMASTER);
	//ProtoSet.SetVal(FDS_USERID_MASTER, pClosingId);
	//ProtoSet.SetVal(FDS_CODE, CODE_LOGOFF);
	//ProtoSet.SetVal(FDS_COMMAND, TP_COMMAND);
	//ProtoSet.SetVal(FDS_USERID_MINE, m_sMyId);
	//ProtoSet.SetVal(FDN_ACCNO_MY, m_nMyAccNo);
	//ProtoSet.SetVal(FDN_ACCNO_MASTER, m_nMasterAccNo);
	//ProtoSet.SetVal(FDS_BROKER, AccountInfoString(ACCOUNT_COMPANY));
	
	nSendLen = ProtoSet.Complete(zSendBuf);
	m_pubSlave->PubData(zSendBuf, nSendLen);
	
}

VOID CRelay::UpdatePing(string sMyId)
{
	m_connManager->UpdatePing(sMyId);
}

VOID CRelay::RegUnregMaster(string sMasterId, string sMyID)
{
	string sRegUnreg;
	if (!m_protoGet.GetVal(FDS_REGUNREG, &sRegUnreg))
	{
		g_log.log(INFO, "REG/UNREG is not in the packet");
		return;
	}
	m_connManager->RegUnregMaster(sMasterId, sRegUnreg.at(0));
}

VOID CRelay::RegUnregSlave(string sMasterId, string sMyID)
{
	string sRegUnreg;
	if (!m_protoGet.GetVal(FDS_REGUNREG, &sRegUnreg))
	{
		g_log.log(INFO, "REG/UNREG is not in the packet");
		return;
	}

	m_connManager->RegUnregSlave(sMasterId, sMyID, sRegUnreg.at(0));
}



//
//VOID CRelay::RegUnregMaster(char* pData)
//{
//	char zAcc[128];
//	int nAcc;
//	if (!m_protoGet.GetVal(FDN_ACCNO_MASTER, &nAcc))
//	{
//		g_log.log(ERR, "[RegUnregMaster] Master Acc is not in packet");
//		return;
//	}
//	g_log.log(ERR, "[RegUnregMaster] Master Acc:%d", nAcc);
//	sprintf(zAcc, "%d", nAcc);
//	string	sAcc = zAcc;
//
//	string sRegUnreg;
//	if (!m_protoGet.GetVal(FDS_REGUNREG, &sRegUnreg))
//	{
//		g_log.log(ERR, "[RegUnregMaster] Reg / Unreg type is not in packet");
//		return;
//	}
//	g_log.log(ERR, "[RegUnregMaster] Reg / Unreg:%s", sRegUnreg.c_str());
//	UINT	nSlaveCnt = 0;
//
//	Lock();
//	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sAcc);
//
//	// This is the first time to register as Master
//	if (it == m_mapMaster.end())
//	{
//		if (sRegUnreg.at(0) == TP_REG)
//		{
//			// Create a new master channel
//			PUB_CHANNEL* pChannel = new PUB_CHANNEL;
//			if (PublishInit(sAcc.c_str(), &pChannel->pub))
//			{
//				pChannel->SlaveCnt = 0;
//				m_mapMaster[sAcc] = pChannel;
//				g_log.log(INFO, "[MasterAcc:%s]Register Master 1st Time and Pub Init Ok.", sAcc.c_str());
//			}
//			else
//			{
//				g_log.log(ERR, "[MasterAcc:%s]Register Master 1st Time and Pub Init Failed.", sAcc.c_str());
//			}
//		}
//		if (sRegUnreg.at(0) == TP_UNREG)
//		{
//			g_log.log(ERR, "[MasterAcc:%s]UnRegister Master but there isn't the same MasterAcc", sAcc.c_str());
//		}
//	}
//
//	// This master has been registered already.
//	if (it != m_mapMaster.end())
//	{
//		if (sRegUnreg.at(0) == TP_REG)
//		{
//			g_log.log(INFO, "[MasterAcc:%s]Register Master. The same Master already exists. Do thing", sAcc.c_str());
//		}
//		if (sRegUnreg.at(0) == TP_UNREG)
//		{
//			// remove from Master List (map)
//			PUB_CHANNEL* pChannel = (*it).second;
//			PublishUnregMaster(sAcc, &pChannel->pub);
//			PublishDeInit(&pChannel->pub);
//			delete pChannel;
//			m_mapMaster.erase(sAcc);
//			g_log.log(INFO, "[MasterAcc:%s]UnRegister Master.Publish to All Slaves and remove.", sAcc.c_str());
//		}
//	}
//	UnLock();
//}
//
//
//
//VOID CRelay::RegUnregSlave(char* pData)
//{
//	int nMasterAcc, nSlaveAcc;
//
//	if (!m_protoGet.GetVal(FDN_ACCNO_MASTER, &nMasterAcc))
//	{
//		g_log.log(ERR, "[RegUnregSlave] Master Acc is not in packet");
//		return;
//	}
//	if (!m_protoGet.GetVal(FDN_ACCNO_MY, &nSlaveAcc))
//	{
//		g_log.log(ERR, "[RegUnregSlave] nSlaveAcc Acc is not in packet");
//		return;
//	}
//
//	char zMaster[128], zSlave[128];
//	sprintf(zMaster, "%d", nMasterAcc);
//	sprintf(zSlave, "%d", nSlaveAcc);
//
//	string	sMasterAcc = zMaster;
//	string	sSlaveAcc = zSlave;
//	UINT	nSlaveCnt = 0;
//	string sRegUnreg;
//	if (!m_protoGet.GetVal(FDS_REGUNREG, &sRegUnreg))
//	{
//		g_log.log(ERR, "[RegUnregSlave] Reg / Unreg type is not in packet");
//		return;
//	}
//
//
//	Lock();
//	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sMasterAcc);
//
//	// There is no Master
//	if (it == m_mapMaster.end())
//	{
//		g_log.log(ERR, "[MasterAcc:%s]Register Slave. But there isn't the MasterAcc", sMasterAcc.c_str());
//		return;
//	}
//
//	PUB_CHANNEL* pChannel = (*it).second;
//
//	if (sRegUnreg.at(0) == TP_REG)
//	{
//		pChannel->SlaveCnt++;
//		m_mapMaster[sMasterAcc] = pChannel;
//		g_log.log(INFO, "[MasterAcc:%s]Register Slave(SlaveAcc:%s).Increase SlaveCnt(%d)"
//			, sMasterAcc.c_str(), sSlaveAcc.c_str(), pChannel->SlaveCnt);
//	}
//	else if (sRegUnreg.at(0) == TP_UNREG)
//	{
//		pChannel->SlaveCnt--;
//		m_mapMaster[sMasterAcc] = pChannel;
//		g_log.log(INFO, "[MasterAcc:%s]UnRegister Slave(SlaveAcc:%s).Decrease SlaveCnt(%d)"
//			, sMasterAcc.c_str(), sSlaveAcc.c_str(), pChannel->SlaveCnt);
//	}
//	UnLock();
//}
//
//
//
//VOID CRelay::PublishDeInit(CNanoQPub* pPub)
//{
//	pPub->End();
//}
//
//
///*
//Create and Init One Master Channel
//*/
//
//BOOL CRelay::PublishInit(const char* psMasterAcc, CNanoQPub* pPub)
//{
//	if (!pPub->Begin((char*)psMasterAcc, DEF_SEND_TIMEOUT, TP_INTERPROC, FALSE))
//		//if (!pPub->Begin((char*)psMasterAcc, DEF_SEND_TIMEOUT, TP_TCP, FALSE))
//	{
//		g_log.log(ERR, "[%s]PublishInit publisher.Begin error(%s)\n", psMasterAcc, pPub->GetMsg());
//		return FALSE;
//	}
//
//	if (!pPub->Connect())
//	{
//		g_log.log(ERR, "[%s]PublishInit publisher.Connect error(%s)\n", psMasterAcc, pPub->GetMsg());
//		return FALSE;
//	}
//	g_log.log(INFO, "[%s]PublishInit create publish Channel Ok[%s]\n", psMasterAcc, pPub->ChannelNm());
//	return TRUE;
//}
//
//VOID CRelay::PublishData(CNanoQPub* pPub, char* pData, int nSendLen)
//{
//	int nRet, nErrCode;
//	nRet = pPub->SendData(pData, nSendLen, &nErrCode);
//	g_log.log(INFO, "[%s][PublishData](%.*s)", pPub->ChannelNm(), nSendLen, pData);
//
//	if (nRet == Q_ERROR)
//	{
//		if (nErrCode == Q_ERROR)
//		{
//			g_log.log(ERR, "[%s]publisher.SendData error (%s)\n",
//				pPub->ChannelNm(), pPub->GetMsg());
//		}
//		else
//		{
//			g_log.log(INFO, "[%s]publisher.SendData timeout.....", pPub->ChannelNm());
//		}
//	}
//}
//
//
///*
//	Send Unregister message to every slave 
//*/
//void CRelay::PublishUnregMaster(string sMasterAcc, CNanoQPub* pPub)
//{
//	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sMasterAcc);
//	if (it == m_mapMaster.end())
//	{
//		//TODO. LOG
//		return;
//	}
//
//	char temp[128];
//	char szBuffer[1024] = { 0, };
//	int nBufLen = sizeof(szBuffer);
//	CProtoSet set;
//	set.Begin();
//	set.SetVal(FDS_CODE, CODE_REG_SLAVE);
//	set.SetVal(FDS_COMMAND, TP_COMMAND);
//	set.SetVal(FDN_ACCNO_MY, atoi(sMasterAcc.c_str()));
//	set.SetVal(FDS_TM_HEADER, Now(temp));
//	set.SetVal(FDS_REGUNREG, TP_UNREG);
//	int nLen = set.Complete(szBuffer);
//	PUB_CHANNEL* pChannel = (*it).second;
//	PublishData(&pChannel->pub, szBuffer, nLen);
//}
//
//
//VOID CRelay::PublishOrder(char* pData)
//{
//	int nAcc;
//	if (!m_protoGet.GetVal(FDN_ACCNO_MASTER, &nAcc))
//	{
//		g_log.log(ERR, "[PublishOrder] Master Acc is not in packet");
//		return;
//	}
//	char zMasterAcc[32];
//	sprintf(zMasterAcc, "%d", nAcc);
//	string sMasterAcc = zMasterAcc;
//
//	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sMasterAcc);
//	if (it == m_mapMaster.end())
//	{
//		g_log.log(ERR, "Order data comes in but there is not the MasterAcc[%s] ", zMasterAcc);
//		return;
//	}
//
//	char szBuffer[1024] = { 0, };
//	int nBufLen = sizeof(szBuffer);
//	CProtoSet set;
//	set.Begin();
//	set.CopyFromRecvData(m_protoGet.GetRecvData());
//	set.SetVal(FDS_CODE, CODE_PUBLISH_ORDER);
//	set.SetVal(FDN_ACCNO_MY, nAcc);
//
//	int nOpenedTicket;
//	m_protoGet.GetVal(FDN_OPENED_TICKET, &nOpenedTicket);
//	char zComment[30];
//	sprintf(zComment, "BA[%d]", nOpenedTicket);
//	set.SetVal(FDS_COMMENTS, zComment);
//
//	int nLen = set.Complete(szBuffer);
//	PUB_CHANNEL* pChannel = (*it).second;
//	PublishData(&pChannel->pub, szBuffer, nLen);
//	g_log.log(INFO, "[Publish Master Order](%s)", szBuffer);
//}