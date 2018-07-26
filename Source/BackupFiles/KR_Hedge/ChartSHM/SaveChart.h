#pragma once
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/sharedmem.h"
#include "../../IRUM_UTIL/Screwdb.h"


class CSaveChart : public CBaseThread
{
public:
	CSaveChart(int chartTp, char* pzConfig);
	~CSaveChart();

	BOOL	Initialize();

	virtual VOID	ThreadFunc();
	BOOL	LoadShm();
	BOOL	LoadShmWrapper();

	VOID	DBSave();
	VOID	DBSaveWrapper();

private:
	int			m_nChartTp;
	CDBPool			*m_pDBPool;
	CSharedMem		m_shm, m_lastShm;
	char			m_zConfig[_MAX_PATH];
	char			m_zMsg[1024];
	BOOL			m_bContinue;
};

