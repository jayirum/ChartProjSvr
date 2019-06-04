#pragma once



#include "../Common/NanoQPipe.h"
#include "../Common/BAProtocol.h"
#include "ConnManager.h"
#include "PubToMS.h"

#include <map>
#include <string>

using namespace _BA_RELAY;
using namespace std;


#define RECV_TIMEOUT_MS	10

class CRelay //: public CNanoQReader
{
public:
	CRelay();
	~CRelay();

	BOOL Begin();
	void End();
	static void WINAPI RecvProc(void *pClassInstance, char *pRecvBuf, int nRecvLen);	// class instance pointer, recv data, return value
	static void WINAPI ClosingProc(void *pClassInstance, int nPubScopeTp, char *pClosingId);	

private:
	BOOL	RecvQInit();

	VOID	RegUnregMaster(string sMasterId, string sMyID);
	VOID	RegUnregSlave(string sMasterId, string sMyID);
	VOID	UpdatePing(string sMyId);
	VOID	PublishOrder(string sMasterId, string sMyID, char* pzRecvBuff);

	// Init, DeInit the publish q
	//BOOL	PublishInit(const char* psMasterAcc, CNanoQPub* pPub);
	//VOID	PublishDeInit(CNanoQPub* pPub);

	//// functions which is done after receive data 
	//VOID	RegUnregMaster(char* pData);
	//VOID	RegUnregSlave(char* pData); 
	//VOID	PublishOrder(char* pData);
	//void	PublishUnregMaster(string sMasterAcc, CNanoQPub* pPub);
	//VOID	PublishData(CNanoQPub* pPub, char* pData, int nSendLen);

	

	//inline void Lock() { EnterCriticalSection(&m_cs); }
	//inline void UnLock() { LeaveCriticalSection(&m_cs); }

private:
	CNanoQReader	*m_recvQ;
	CProtoGet		m_protoGet;
	CConnManager	*m_connManager;
	CPubToMS		*m_pubMaster;
	CPubToMS		*m_pubSlave;
	char			m_zMsg[1024];

};

