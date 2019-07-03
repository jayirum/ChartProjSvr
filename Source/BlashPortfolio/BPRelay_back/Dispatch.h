/*
	- Get message from Listen thread

	- dispatch data to each CMasterSlave 
*/

#pragma once

#include "MasterSlave.h"
#include <map>
#include <string>
using namespace std;

class CDispatch
{
public:
	CDispatch();
	~CDispatch();

private:
	map<string, CMasterSlave*>	m_mapMaster;

};

