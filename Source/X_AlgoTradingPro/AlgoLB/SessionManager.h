#pragma once


#include <windows.h>
#include <map>
#include <list>
#include <string>
#include "../../IRUM_UTIL/Irum_Common.h"
#include "../Common/AlgoInc.h"

struct FRONT_INFO
{
	std::string name;
	std::string ip;
	int			port;
};

struct CLIENT_INFO
{
	std::string id;
	std::string loginTm;
	std::string frontName;
};

class CSessionManager
{
private:
	CRITICAL_SECTION	m_cs;
	std::map<std::string, std::list<CLIENT_INFO> >	m_mapFront;		//front name, client info
	std::map<std::string, std::list<FRONT_INFO> >	m_mapClient;	//client id, front info

public:
	CSessionManager();
	~CSessionManager();

	BOOL Initialize();
	void Finalize();

	// Front disconnect
	VOID FrontDisconnect(char* pzFrontName);
	BOOL FrontConnect();

	// front tells these 3 action
	VOID FrontTell_ClientLogon(char* pzFrontName, char* pzClientId);
	VOID FrontTell_ClientLogoff(char* pzFrontName, char* pzClientId);
	
	// once client send a correct credential, send back the front ip, port
	BOOL ClientLogonProc();
	
	// if the client connections excess limit, force to log off the oldest session
	VOID TellToFront_CloseClientSession();


	void lock() { EnterCriticalSection(&m_cs); }
	void unlock() { LeaveCriticalSection(&m_cs); }
};

