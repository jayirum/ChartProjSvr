#pragma once

#include <string>
#include <list>
#include <Windows.h>
#include "../../IRUM_UTIL/BaseThread.h"

using namespace std;
typedef list<string>	LIST_SLAVE;

class COrderRelay : public CBaseThread
{
public:
	COrderRelay();
	~COrderRelay();

	BOOL Begin();
	void End();
	inline unsigned int GetThreadId() { return GetMyThreadID(); }
	void ThreadFunc();

	int(*QW_Begin)(char *, int, int);

	//typedef int(*QWDll_Connect)();
	//QWDll_Connect	QW_Connect;

	//typedef int(*QWDll_SendData)(char*, int);
	//QWDll_SendData	QW_SendData;


	//typedef int(*SWDll_Close)();
	//SWDll_Close	QW_Close;

	//typedef void(*QWDll_GetLastMsg)(char*);
	//QWDll_GetLastMsg	QW_GetLastMsg;


private:
	string			m_sMasterAcc;
	LIST_SLAVE		m_listSlave;
};

