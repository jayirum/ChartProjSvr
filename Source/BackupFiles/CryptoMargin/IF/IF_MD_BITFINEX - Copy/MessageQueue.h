#pragma once

#include <string>	
#include <list>
#include <ctime>
#include <Windows.h>

using namespace std;

typedef struct _MESSAGE
{

	time_t				timeTimeStamp;
	string			sMessage;
	time_t				timeLastUpdateTimeStamp;	
	int					nStatus;
	//dataWebSocketData	*m_pPreviousData, *m_pNextData;
}MESSAGE;

class CMessageQueue
{
public:
	CMessageQueue();
	~CMessageQueue();

	BOOL	InsertData(string sData);
	string	GetData();
	VOID	DeleteAllData();
	void ShowAllMessages(int x, int y);
	time_t GetLastUpdateTime()
	{
		return m_timeLastUpdateTimeStamp;
	}

private:
	VOID	Lock() { EnterCriticalSection(&m_cs); }
	VOID	Unlock() { LeaveCriticalSection(&m_cs); }
private:

	list<MESSAGE*>		m_list;
	time_t				m_timeLastUpdateTimeStamp;
	CRITICAL_SECTION	m_cs;
};

