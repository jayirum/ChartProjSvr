#include "Relay.h"
#include "../Common/BARelayInc.h"
#include "Main.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"
extern CLogMsg		g_log;

CRelay::CRelay()
{
	m_recvQ = NULL;
	InitializeCriticalSection(&m_cs);
}


CRelay::~CRelay()
{
	End();
	DeleteCriticalSection(&m_cs);
}

void CRelay::End()
{
	Lock();
	map<string, PUB_CHANNEL*>::iterator it;
	for (it = m_mapMaster.begin(); it != m_mapMaster.end(); it++)
	{
		PUB_CHANNEL* pChannel = (*it).second;

		PublishUnregMaster((*it).first, &pChannel->pub);

		PublishDeInit(&pChannel->pub);

		delete pChannel;
	}
	m_mapMaster.clear();
	UnLock();

	RecvQDeInit();
}

VOID CRelay::RegUnregMaster(char* pData)
{
	char zAcc[128];
	_BA_RELAY::PT_REG_MASTER* p = (_BA_RELAY::PT_REG_MASTER*)pData;
	sprintf(zAcc, "%.*s", sizeof(p->MasterAccNo), p->MasterAccNo);
	CUtil::RTrim(zAcc, strlen(zAcc));
	string	sAcc = zAcc;
	UINT	nSlaveCnt = 0;

	Lock();
	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sAcc);
	
	// This is the first time to register as Master
	if (it == m_mapMaster.end())
	{ 
		if (p->Action[0] == TP_REG)
		{
			// Create a new master channel
			PUB_CHANNEL* pChannel = new PUB_CHANNEL;
			if (PublishInit(sAcc.c_str(), &pChannel->pub))
			{
				pChannel->SlaveCnt = 0;
				m_mapMaster[sAcc] = pChannel;
				g_log.log(INFO, "[MasterAcc:%s]Register Master 1st Time and Pub Init Ok.", sAcc.c_str());
			}
			else
			{
				g_log.log(ERR, "[MasterAcc:%s]Register Master 1st Time and Pub Init Failed.", sAcc.c_str());
			}
		}
		if (p->Action[0] == TP_UNREG)
		{
			g_log.log(ERR, "[MasterAcc:%s]UnRegister Master but there isn't the same MasterAcc", sAcc.c_str());
		}
	}

	// This master has been registered already.
	if (it != m_mapMaster.end())
	{
		if (p->Action[0] == TP_REG)
		{
			g_log.log(INFO, "[MasterAcc:%s]Register Master. The same Master already exists. Do thing", sAcc.c_str());
		}
		if (p->Action[0] == TP_UNREG)
		{
			// remove from Master List (map)
			PUB_CHANNEL* pChannel = (*it).second;
			PublishUnregMaster(sAcc.c_str(), &pChannel->pub);
			PublishDeInit(&pChannel->pub);
			delete pChannel;
			m_mapMaster.erase(sAcc);
			g_log.log(INFO, "[MasterAcc:%s]UnRegister Master.Publish to All Slaves and remove.", sAcc.c_str());
		}
	}
	UnLock();
}



VOID CRelay::RegUnregSlave(char* pData)
{
	_BA_RELAY::PT_REG_SLAVE* p = (_BA_RELAY::PT_REG_SLAVE*)pData;

	char zMaster[128], zSlave[128];
	sprintf(zMaster, "%.*s", sizeof(p->MasterAccNo), p->MasterAccNo);
	sprintf(zSlave, "%.*s", sizeof(p->header.AccNo), p->header.AccNo);

	CUtil::RTrim(zMaster, strlen(zMaster));
	CUtil::RTrim(zSlave, strlen(zSlave));

	string	sMasterAcc = zMaster;
	string	sSlaveAcc = zSlave;
	UINT	nSlaveCnt = 0;

	Lock();
	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sMasterAcc);

	// There is no Master
	if (it == m_mapMaster.end())
	{
		g_log.log(ERR, "[MasterAcc:%s]Register Slave. But there isn't the MasterAcc", sMasterAcc.c_str());
		return;
	}

	PUB_CHANNEL* pChannel = (*it).second;

	if (p->Action[0] == TP_REG)
	{
		pChannel->SlaveCnt++;
		m_mapMaster[sMasterAcc] = pChannel; 
		g_log.log(INFO, "[MasterAcc:%s]Register Slave(SlaveAcc:%s).Increase SlaveCnt(%d)"
			, sMasterAcc.c_str(), sSlaveAcc.c_str(), pChannel->SlaveCnt);
	}
	else if (p->Action[0] == TP_UNREG)
	{
		pChannel->SlaveCnt--;
		m_mapMaster[sMasterAcc] = pChannel;
		g_log.log(INFO, "[MasterAcc:%s]UnRegister Slave(SlaveAcc:%s).Decrease SlaveCnt(%d)"
			, sMasterAcc.c_str(), sSlaveAcc.c_str(), pChannel->SlaveCnt);
	}
	UnLock();
}



VOID CRelay::PublishDeInit(CNanoQPub* pPub)
{
	pPub->End();
}


/*
	Create and Init One Master Channel
*/

BOOL CRelay::PublishInit(const char* psMasterAcc, CNanoQPub* pPub)
{
	if (!pPub->Begin( (char*)psMasterAcc, DEF_SEND_TIMEOUT, TP_INTERPROC, FALSE))
	//if (!pPub->Begin((char*)psMasterAcc, DEF_SEND_TIMEOUT, TP_TCP, FALSE))
	{
		g_log.log(ERR,"[%s]PublishInit publisher.Begin error(%s)\n", psMasterAcc, pPub->GetMsg());
		return FALSE;
	}
	
	if (!pPub->Connect())
	{
		g_log.log(ERR, "[%s]PublishInit publisher.Connect error(%s)\n", psMasterAcc, pPub->GetMsg());
		return FALSE;
	}
	g_log.log(INFO, "[%s]PublishInit create publish Channel Ok[%s]\n", psMasterAcc,pPub->ChannelNm());
	return TRUE;
}

BOOL CRelay::Begin()
{
	BOOL bRet = RecvQInit();
	if (!bRet) {
		g_log.log(ERR, "[%s]RecvQ Begin Error(%s)\n", 
			m_recvQ->ChannelNm(), m_recvQ->GetMsg());
	}
	else{
		g_log.log(INFO, "[%s]RecvQ Begin OK\n", m_recvQ->ChannelNm());
	}
	return bRet;
}

BOOL CRelay::RecvQInit() 
{
	if (!m_recvQ)
		m_recvQ = new CNanoQReader();

	if (!m_recvQ->Begin(NULL, DEF_RECV_TIMEOUT, TP_INTERPROC, TRUE)) {
	//if (!m_recvQ->Begin(NULL, DEF_RECV_TIMEOUT, TP_TCP, TRUE)) {
		g_log.log(ERR, "RecvQInit CNanoQReader.Begin error(%s)\n", m_recvQ->GetMsg());
		return FALSE;
	}

	if (!m_recvQ->Connect())
	{
		g_log.log(ERR, "RecvQInit CNanoQReader.Connect error(%s)\n", m_recvQ->GetMsg());
		return FALSE;
	}
	
	m_recvQ->RegisterCallBack((LPVOID)this, (PRecvCallBack)RecvProc);
	return TRUE;
}

VOID CRelay::RecvQDeInit()
{
	if(m_recvQ) delete m_recvQ;
	m_recvQ = NULL;
}


// class instance pointer, recv data, return value
void WINAPI CRelay::RecvProc(void *pClassInstance, char *pRecvBuf, int nRecvLen)
{
	g_log.log(INFO,"[RECV][LEN:%d](%.*s)\n", nRecvLen, nRecvLen, pRecvBuf);

	CRelay* pThis = (CRelay*)pClassInstance;

	_BA_RELAY::Header* pH = (_BA_RELAY::Header*)pRecvBuf;

	if ( strncmp(pH->Code, CODE_REG_MASTER, sizeof(pH->Code))==0) {
		g_log.log(INFO, "Reg/Unreg Master Packet. RegUnregMaster start");
		pThis->RegUnregMaster(pRecvBuf);
	}
	if (strncmp(pH->Code, CODE_REG_SLAVE, sizeof(pH->Code))==0) {
		g_log.log(INFO, "Reg/Unreg Slave Packet. RegUnregSlave start");
		pThis->RegUnregSlave(pRecvBuf);
	}
	if (strncmp(pH->Code, CODE_MASTER_ORDER, sizeof(pH->Code))==0) {
		g_log.log(INFO, "Master Order Packet. PublishOrder start");
		pThis->PublishOrder(pRecvBuf);
	}

}



VOID CRelay::PublishData(CNanoQPub* pPub, char* pData, int nSendLen)
{
	int nRet, nErrCode;
	nRet = pPub->SendData(pData, nSendLen, &nErrCode);
	g_log.log(INFO, "[%s][PublishData](%.*s)", pPub->ChannelNm(), nSendLen, pData);

	if (nRet == Q_ERROR)
	{
		if (nErrCode == Q_ERROR)
		{
			g_log.log(ERR, "[%s]publisher.SendData error (%s)\n",
				pPub->ChannelNm(), pPub->GetMsg());
		}
		else
		{
			g_log.log(INFO, "[%s]publisher.SendData timeout.....", pPub->ChannelNm());
		}
	}
}


/*
	Send Unregister message to every slave 
*/
void CRelay::PublishUnregMaster(string sMasterAcc, CNanoQPub* pPub)
{
	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sMasterAcc);
	if (it == m_mapMaster.end())
	{
		//TODO. LOG
		return;
	}

	char temp[128];
	char szBuffer[1024] = { 0, };
	int nBufLen = sizeof(szBuffer);

	PT_REG_SLAVE* pPack = (PT_REG_SLAVE*)szBuffer;
	memset(pPack, 0x20, sizeof(PT_REG_SLAVE));

	memcpy(pPack->header.Code, CODE_REG_SLAVE, sizeof(pPack->header.Code));
	pPack->header.Type[0] = TP_COMMAND;
	memcpy(pPack->header.AccNo, sMasterAcc.c_str(), sMasterAcc.size());
	memcpy(pPack->header.Tm, Now(temp), sizeof(pPack->header.Tm));

	pPack->Action[0] = TP_UNREG;

	PUB_CHANNEL* pChannel = (*it).second;
	PublishData(&pChannel->pub, (char*)pPack, sizeof(PT_REG_SLAVE));
}


VOID CRelay::PublishOrder(char* pData)
{
	PT_MASTER_ORD* pOrd = (PT_MASTER_ORD*)pData;
	char zMasterAcc[32];
	sprintf(zMasterAcc, "%.*s", sizeof(pOrd->MasterAccNo), pOrd->MasterAccNo);
	CUtil::RTrim(zMasterAcc, strlen(zMasterAcc));
	string sMasterAcc = zMasterAcc;

	map<string, PUB_CHANNEL*>::iterator it = m_mapMaster.find(sMasterAcc);
	if (it == m_mapMaster.end())
	{
		g_log.log(ERR, "Order data comes in but there is not the MasterAcc[%s] ", zMasterAcc);
		return;
	}

	char temp[128];
	//char szBuffer[1024] = { 0, };
	//int nBufLen = sizeof(szBuffer);

	//PT_REG_SLAVE* pPack = (PT_REG_SLAVE*)szBuffer;
	//memset(pPack, 0x20, sizeof(PT_REG_SLAVE));

	//memcpy(pPack->header.Code, CODE_REG_SLAVE, sizeof(pPack->header.Code));
	//pPack->header.Type[0] = TP_ORDER;
	//memcpy(pPack->header.AccNo, sMasterAcc.c_str(), sMasterAcc.size());
	//memcpy(pPack->header.Tm, Now(temp), sizeof(pPack->header.Tm));

	// store master ticket on comment
	sprintf(temp, "%.*s", sizeof(pOrd->Ticket), pOrd->Ticket);
	char zComment[30];
	sprintf(zComment, "BA[%d]", atoi(temp));
	memcpy(pOrd->Comments, zComment, strlen(zComment));

	PUB_CHANNEL* pChannel = (*it).second;
	PublishData(&pChannel->pub, (char*)pOrd, sizeof(PT_MASTER_ORD));

}