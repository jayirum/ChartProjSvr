#include "stdafx.h"
#include "NotedThread.h"


#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/recordset.h"

CNotedThread::CNotedThread(std::map<CString, void*>* mapAcnt, void* pDBPool)
{
	m_mapAcnt = mapAcnt;
	m_pDBPool = pDBPool;
}


CNotedThread::~CNotedThread()
{
}

//
//VOID CNotedThread::Close()
//{
//	SAFE_DELETE(m_sock);
//}

VOID	CNotedThread::Polling(void* pRecvBuf)
{

}