#pragma once

#include <string>
#include <map>

typedef std::map<std::string, unsigned int>	MAP_RELAY;	// account, threadID

class CMainProc
{
public:
	CMainProc();
	~CMainProc();


private:
	MAP_RELAY	m_mapRelay;
};

