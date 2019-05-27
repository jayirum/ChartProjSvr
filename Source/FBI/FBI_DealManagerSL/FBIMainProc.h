#pragma once

//#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "../../IRUM_UTIL/SmartMessage.h"
#include <map>
#include <string>
#include <list>
#include "StkOrd.h"

using namespace std;

typedef map<string, CStkOrd*>::iterator	IT_MAP;

// CFBIMainProc dialog
class CFBIMainProc //: public CBaseThread
{
// Construction
public:
	CFBIMainProc();
	~CFBIMainProc();

	// Generated message map functions
	BOOL Initialize();
	VOID Finalize();

private:
	//VOID ThreadFunc();
	static unsigned WINAPI SaveResultThread(LPVOID lp);

	static long WINAPI CallBackSMPrc(int index, char* pCustomPtr, char* Message);
	static long WINAPI CallBackSMOrd(int index, char* pCustomPtr, char* Message);

	BOOL	LoadStkInfo();
	BOOL	CreateStkOrders();
	BOOL	InitializeSM();
	VOID	DeInitializeSM();
	VOID	ClearDealMap();

	//IT_MAP* GetMapIterator() { return new IT_MAP; }
	//VOID	ReleaseMapIterator(IT_MAP* pIt) { delete pIt; }
	//CStkOrd* CStkOrdFactory(const std::string *sStkCd) { return new CStkOrd(*sStkCd); }

private:
	CDBPoolAdo		*m_pDBPool;
	CSmartMessage	*m_pSM;

	HANDLE			m_hSaveData;
	unsigned int	m_unSaveData;

	list<_FBI::ST_STK_INFO>	m_lstStkInfo;
	map<string, CStkOrd*>	m_mapStkOrd;
	CRITICAL_SECTION		m_csStkOrd;

	CSmartMessage	m_smPrc;
	CSmartMessage	m_smOrd;
	
};
