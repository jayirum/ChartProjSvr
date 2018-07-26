#include "MessageQueue.h"
#include "Precompiled.h"

#include <curses.h>			/* curses.h includes stdio.h */  


CMessageQueue::CMessageQueue()
{
	std::time(&m_timeLastUpdateTimeStamp);

	InitializeCriticalSection(&m_cs);
}


CMessageQueue::~CMessageQueue()
{
	DeleteAllData();
	DeleteCriticalSection(&m_cs);
}


void CMessageQueue::DeleteAllData()
{
	Lock();
	list<MESSAGE*>::iterator it;
	for (it = m_list.begin(); it != m_list.end(); it++)
	{
		delete (*it);
	}
	m_list.clear();
	Unlock();
}

BOOL CMessageQueue::InsertData(string sData)
{
	MESSAGE *pMsg = new MESSAGE;

	std::time(&pMsg->timeTimeStamp);
	std::time(&pMsg->timeLastUpdateTimeStamp);
	pMsg->sMessage = sData;

	Lock();
	m_list.push_back(pMsg);
	Unlock();

	return TRUE;
}

string CMessageQueue::GetData()
{
	Lock();

	if (m_list.empty()) {
		Unlock();
		return "";
	}

	MESSAGE* p = m_list.front();
	string ret = p->sMessage;
	m_list.pop_front();
	delete p;
	Unlock();

	return ret;
}

void CMessageQueue::ShowAllMessages(int x, int y)
{
	int nRow = 0;
	char sTimeStamp[15] = "\0";
	char sLastUpdateTimeStamp[15] = "\0";

	Lock();
	list<MESSAGE*>::iterator it;
	for (it = m_list.begin(); it != m_list.end(); it++)
	{
		MESSAGE* p = *it;
		Time_tToString(p->timeTimeStamp, sTimeStamp);
		Time_tToString(p->timeLastUpdateTimeStamp, sLastUpdateTimeStamp);

		if (p->sMessage.length()>256)
			mvprintw(y + nRow, x, "Time : %s,\tMessage : %s\tTimeLastUpdate : %s", sTimeStamp, p->sMessage.substr(0, 255), sLastUpdateTimeStamp);
		else
			mvprintw(y + nRow, x, "Time : %s,\tMessage : %s\tTimeLastUpdate : %s", sTimeStamp, p->sMessage.c_str(), sLastUpdateTimeStamp);
	}

	Unlock();
}
