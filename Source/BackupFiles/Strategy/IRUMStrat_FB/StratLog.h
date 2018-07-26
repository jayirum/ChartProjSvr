#pragma once

#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_INC/IRUM_Common.h"

class CStratLog
{
public:
	CStratLog();
	~CStratLog();

	BOOL Begin(char* pzUserID);
	BOOL ReBegin();
	VOID End();
	               
	VOID logProcSig(char* psStratCd, char* pzSymbol, char* pzUserID, char* pzChartNm, BOOL bApplied, char* pMsg, ...);
	VOID logProcClPack(char* psStratCd, char* pzSymbol, char* pzUserID, char* pzChartNm, char* pMsg, ...);
	
	VOID logOrd(char* psStratCd, char* pzSymbol, char* pzUserID, char* pzChartNm, char cOrdTp, char cOrdProcTp, char* pzPrc, char* pzOrgPrc, char* pMsg, ...);

private:
	VOID	Write(char* pzBuf);
	VOID	LOCK() { EnterCriticalSection(&m_cs); };
	VOID	UNLOCK() { LeaveCriticalSection(&m_cs); };
private:
	char	m_zUserID[128];
	int		m_fd;
	char	m_szPath[_MAX_PATH];
	char	m_szFileName[_MAX_PATH];
	char	m_szPureFileName[_MAX_PATH];
	char	m_szDate[8 + 1];
	char	m_szMsg[1024];
	char	m_zCd[128];
	CRITICAL_SECTION m_cs;
};

