#include "StratLog.h"
#include "../../IRUM_UTIL/Util.h"

extern char	g_zConfig[_MAX_PATH];

CStratLog::CStratLog()
{
	m_fd = 0;
}


CStratLog::~CStratLog()
{
	End();
	DeleteCriticalSection(&m_cs);
}


BOOL CStratLog::Begin(char* pzUserID)
{
	InitializeCriticalSection(&m_cs);
	strcpy(m_zUserID, pzUserID);
	
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", m_szPath);

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(m_szDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	sprintf_s(m_szFileName, "%s\\STRATORD_%s_%s.log", m_szPath, m_zUserID, m_szDate);
	
	errno_t err = _sopen_s(&m_fd, m_szFileName, _O_CREAT | _O_APPEND | _O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	if (err < 0) {
		//UNLOCK();
		return FALSE;
	}
	//UNLOCK();
	return TRUE;

}

VOID CStratLog::End()
{
	if (m_fd > 0) {
		_close(m_fd);
		m_fd = 0;
	}
}


BOOL CStratLog::ReBegin()
{
	char szToday[8 + 1];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(szToday, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	if (strcmp(szToday, m_szDate)>0)
	{
		//return OpenLog( m_szPath, m_szFileName );
		return Begin(m_zUserID);

	}
	return	TRUE;
}


VOID CStratLog::Write(char* pzBuf)
{
	LOCK();
	//ReBegin();

	if (m_fd <= 0) {
		UNLOCK();
		return;
	}
	_write(m_fd, pzBuf, strlen(pzBuf));

	UNLOCK();
}

VOID CStratLog::logProcSig(char* psStratCd, char* pzSymbol, char* pzUserID, char* pzChartNm, BOOL bApplied, char* pMsg, ...)
{
	char zCodeName[32], zApplied[32];
	STRATID2NAME(psStratCd, zCodeName);
	if (bApplied)	strcpy(zApplied, "적용");
	else			strcpy(zApplied, "스킵");
	char buff[DEF_LOG_LEN];
	SYSTEMTIME	st;

	GetLocalTime(&st);
	
	sprintf(buff, "[%02d:%02d:%02d.%03d]"
		"[%s]"	//코드 이름
		"[%s]"	//USERID
		"[%s]"	//종목코드
		"[%s]"
		"[%-*.*s]" //차트이름
		"%.*s"
		,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		zCodeName,
		pzUserID,
		pzSymbol, 
		zApplied,
		LEN_CHART_NM, LEN_CHART_NM, pzChartNm,
		
		DEF_LOG_LEN - 100, pMsg
	);
	
	Write(buff);
}


VOID CStratLog::logProcClPack(char* psStratCd, char* pzSymbol, char* pzUserID, char* pzChartNm, char* pMsg, ...)
{
	char zCodeName[32];
	STRATID2NAME(psStratCd, zCodeName);
	
	char buff[DEF_LOG_LEN];
	SYSTEMTIME	st;

	GetLocalTime(&st);

	sprintf(buff, "[%02d:%02d:%02d.%03d]"
		"[%s]"	//코드 이름
		"[%s]"	//USERID
		"[%s]"	//종목코드
		"[%-*.*s]" //차트이름
		//"[%s]"
		"%.*s"
		,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		zCodeName,
		pzUserID,
		pzSymbol,
		//zApplied,
		LEN_CHART_NM, LEN_CHART_NM, pzChartNm,

		DEF_LOG_LEN - 100, pMsg
	);
	Write(buff);
}


VOID CStratLog::logOrd(char* psStratCd, char* pzSymbol, char* pzUserID, char* pzChartNm, char cOrdTp, char cOrdProcTp, char* pzPrc, char* pzOrgPrc, char* pMsg, ...)
{
	char zCodeName[32], zOrdTp[32], zOrdProc[32], zPrc[10], zOrgPrc[10];
	STRATID2NAME(psStratCd, zCodeName);
	ORDTP2NAME(cOrdTp, zOrdTp);
	ORDPROC2NAME(cOrdProcTp, zOrdProc);

	sprintf(zPrc, "%09f", atof(zPrc));
	sprintf(zOrgPrc, "%09f", atof(zOrgPrc));
	
	char buff[DEF_LOG_LEN];
	SYSTEMTIME	st;

	GetLocalTime(&st);

	sprintf(buff, "[%02d:%02d:%02d.%03d]"
		"[%s]"	//코드 이름
		"[%s]"	// USERID
		"[%s]"	//종목코드
		"[%-*.*s]" //차트이름
		"[%s]"		//ord tp
		"[%s]"		// ordproc
		"[%s]"		// prc
		"[%s]"		// org prc
		"%.*s"
		,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		zCodeName,
		pzUserID,
		pzSymbol,
		LEN_CHART_NM, LEN_CHART_NM, pzChartNm,
		zOrdTp,
		zOrdProc,
		zPrc,
		zOrgPrc,
		DEF_LOG_LEN - 100, pMsg
	);
	Write(buff);
}