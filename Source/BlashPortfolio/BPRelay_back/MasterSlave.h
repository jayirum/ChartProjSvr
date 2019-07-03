#pragma once


#include <winsock2.h>
#include <map>
#include <string>
using namespace std;



class CMasterSlave
{
public:
	CMasterSlave();
	~CMasterSlave();

private:
	SOCKET		m_sockMaster;
	string		m_sMasterId;
private:
	map<string, SOCKET>	m_mapSlave;		// slave id, slave socket
};

