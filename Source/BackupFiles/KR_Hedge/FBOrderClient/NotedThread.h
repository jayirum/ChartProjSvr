#pragma once

#include <afx.h>
#include <map>

class CNotedThread
{
public:
	CNotedThread(std::map<CString, void*>* mapAcnt, void* pDBPool);
	~CNotedThread();

	//BOOL	Init();
	//VOID	Close();
	VOID		Polling(void* pRecvBuf);
	CHAR*	GetMsg() { return m_sMsg.GetBuffer(); }
private:
	std::map<CString, void*>*	m_mapAcnt;
	void*						m_pDBPool;
	CString						m_sMsg;

};

