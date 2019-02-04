#include "CompletionKey.h"



CCompletionKey::CCompletionKey()
{
	refCnt = 0; 
	sockUser = INVALID_SOCKET; 
	status = _ALGOINC::STATUS_NON_LOGON; 
}


CCompletionKey::~CCompletionKey()
{
}


void CCompletionKey::addRef() 
{ 
	InterlockedIncrement(&refCnt); 
}

void CCompletionKey::releaseRef() 
{ 
	InterlockedDecrement(&refCnt); 
}

bool CCompletionKey::hasRefCnt() 
{ 
	return (refCnt > 0); 
}

void CCompletionKey::SetInitData(SOCKET sock, char* pzUserId, char* psIp)
{
	sockUser	= sock; 
	clientIp	= std::string(psIp);
	userID		= std::string(pzUserId);
}

void CCompletionKey::markCloseRequested() 
{ 
	status = _ALGOINC::STATUS_CLOSING; 
}

bool CCompletionKey::hasMarkedClose() 
{ 
	return (status == _ALGOINC::STATUS_CLOSING); 
}

bool CCompletionKey::isLoggedOn() 
{ 
	return (status == _ALGOINC::STATUS_LOGGEDON); 
}

bool CCompletionKey::isNonLoggedOn() 
{ 
	return (status == _ALGOINC::STATUS_NON_LOGON); 
}