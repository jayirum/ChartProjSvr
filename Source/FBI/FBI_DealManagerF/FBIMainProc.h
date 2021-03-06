#pragma once


#include "../../IRUM_UTIL/adofunc.h"
#include <map>
#include <string>
#include "DealManager.h"
#include "DealManagerTenOp.h"
#include "../../IRUM_UTIL/SmartMessage.h"


// CFBIMainProc dialog
class CFBIMainProc
{
// Construction
public:
	CFBIMainProc();
	~CFBIMainProc();

	// Generated message map functions
	BOOL Initialize();
	VOID Finalize();

private:
	static unsigned WINAPI SendToClientThread(LPVOID lp);
	BOOL InitializeSM();
	void UnInitializeSM() { m_smClient.End(); }

	BOOL	LoadStkCode();
	VOID	ClearDealMap();

private:
	CDBPoolAdo		*m_pDBPool;
	std::map<std::string, CDealManager*>	m_mapDealManager;	//STK_CD
	CRITICAL_SECTION						m_csDM;
	
	std::map<std::string, CDealManagerTenOp*>	m_mapDealManagerTenOp;	//STK_CD
	CRITICAL_SECTION							m_csDMTenOp;

	CSmartMessage	m_smClient;
	HANDLE			m_hToClient;
	unsigned int	m_unToClient;

};
