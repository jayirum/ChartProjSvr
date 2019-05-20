#pragma once



#include "../Common/NanoQPipe.h"
#include "../Common/NanoQPubSub.h"
#include <map>
#include <string>

using namespace _BA_RELAY;
using namespace std;

struct PUB_CHANNEL
{
	CNanoQPub	pub;
	UINT		SlaveCnt;
};



class CRelay : public CNanoQReader
{
public:
	CRelay();
	~CRelay();

	BOOL Begin();
	void End();
	static void WINAPI RecvProc(void *pClassInstance, char *pRecvBuf, int nRecvLen);	// class instance pointer, recv data, return value

private:

	// Init, DeInit the publish q
	BOOL	PublishInit(const char* psMasterAcc, CNanoQPub* pPub);
	VOID	PublishDeInit(CNanoQPub* pPub);

	// functions which is done after receive data 
	VOID	RegUnregMaster(char* pData);
	VOID	RegUnregSlave(char* pData); 
	VOID	PublishOrder(char* pData);
	void	PublishUnregMaster(string sMasterAcc, CNanoQPub* pPub);
	VOID	PublishData(CNanoQPub* pPub, char* pData, int nSendLen);

	BOOL	RecvQInit();
	VOID	RecvQDeInit();

	inline void Lock() { EnterCriticalSection(&m_cs); }
	inline void UnLock() { LeaveCriticalSection(&m_cs); }

private:
	CNanoQReader				*m_recvQ;
	char						m_zMsg[1024];
	map<string, PUB_CHANNEL*>	m_mapMaster;
	CRITICAL_SECTION			m_cs;
};

