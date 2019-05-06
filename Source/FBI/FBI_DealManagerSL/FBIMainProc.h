#pragma once


#include "../../IRUM_UTIL/TcpClient.h"
#include "../../IRUM_UTIL/adofunc.h"
#include <map>
#include <string>
#include "DealManager.h"
#include "DealManagerTenOp.h"


// CFBIMainProc dialog
class CFBIMainProc : public CBaseThread
{
// Construction
public:
	CFBIMainProc();
	~CFBIMainProc();

	// Generated message map functions
	BOOL Initialize();
	VOID Finalize();

private:
	VOID ThreadFunc();
	//static unsigned WINAPI Thread_ApiChart(LPVOID lp);
	static unsigned WINAPI Thread_SaveChart(LPVOID lp);


	BOOL	LoadStkCode();
	VOID	ClearDealMap();
	//void	InitApiSocket();
	//void	CloseApiSocket();

	//void	testChart();

private:
	CDBPoolAdo		*m_pDBPool;

	//char			m_zApiIP[32];
	//char			m_zApiPort[32];
		
	HANDLE			m_hSaveData;
	unsigned int	m_unSaveData;

	std::map<std::string, CDealManager*>	m_mapDealManager;	//STK_CD
	CRITICAL_SECTION						m_csDM;
	
	std::map<std::string, CDealManagerTenOp*>	m_mapDealManagerTenOp;	//STK_CD
	CRITICAL_SECTION							m_csDMTenOp;

};
