#include "UserInfo.h"



CUserInfo::CUserInfo()
{
	refCnt = 0; 
	sockUser = INVALID_SOCKET; 
	status = _ALGOINC::STATUS_NON_LOGON; 
}


CUserInfo::~CUserInfo()
{
}


void CUserInfo::addRef() 
{ 
	InterlockedIncrement(&refCnt); 
}

void CUserInfo::releaseRef() 
{ 
	InterlockedDecrement(&refCnt); 
}

bool CUserInfo::hasRefCnt() 
{ 
	return (refCnt > 0); 
}

void CUserInfo::SetInitData(SOCKET sock, char* pzUserId, char* psIp)
{
	sockUser	= sock; 
	clientIp	= std::string(psIp);
	userID		= std::string(pzUserId);
}

void CUserInfo::markCloseRequested() 
{ 
	status = _ALGOINC::STATUS_CLOSING; 
}

bool CUserInfo::hasMarkedClose() 
{ 
	return (status == _ALGOINC::STATUS_CLOSING); 
}

bool CUserInfo::isLoggedOn() 
{ 
	return (status == _ALGOINC::STATUS_LOGGEDON); 
}

bool CUserInfo::isNonLoggedOn() 
{ 
	return (status == _ALGOINC::STATUS_NON_LOGON); 
}