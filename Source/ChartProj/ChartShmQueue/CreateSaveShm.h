#pragma once
#include "../../IRUM_UTIL/ADOFunc.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/QueueShm.h"
#include <string>

#define MAX_TP_CNT	20

class CCreateSaveShm : public CBaseThread
{
public:
	CCreateSaveShm(char *pzArtcSymbol);
	~CCreateSaveShm();

	BOOL	Create();
	BOOL	Initialize();

	virtual VOID	ThreadFunc();
	//BOOL	LoadShm(/*CHART_TP*/int tp);
	//BOOL	LoadShmWrapper(/*CHART_TP*/int tp);

	//VOID	DBSave(/*CHART_TP*/int  tp);
	//VOID	DBSaveWrapper(/*CHART_TP*/int  tp);

	char*	GetMsg() { return m_zMsg; }
private:
	char			m_zArtc[32];
	CDBPoolAdo		*m_pDBPool;
	CQueueShm		m_shmQ;
	//CSharedMem		m_shmLast;
	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_zConfig[_MAX_PATH];
	char			m_zMsg[1024];
	BOOL			m_bContinue;
	//std::string		m_sLastSaveNm[MAX_TP_CNT];
};

