#pragma once
#include <winsock2.h>
#include "../Common/AlgoInc.h"
#include <string>
class CUserInfo
{
public:

	SOCKET			sockUser;
	unsigned long	refCnt;
	_ALGOINC::EN_CLIENT_STATUS	status;
	std::string		userID;
	std::string		clientIp;
public:
	CUserInfo();
	~CUserInfo();
	void addRef();
	void releaseRef();
	bool hasRefCnt();
	void SetInitData(SOCKET sock, char* pzUserId, char* psIp);
	void markCloseRequested();
	bool hasMarkedClose();
	bool isLoggedOn();
	bool isNonLoggedOn();

};

