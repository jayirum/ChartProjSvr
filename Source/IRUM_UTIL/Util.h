// CommonUtil.h: interface for the CCommonUtil class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable:4996)


#include <time.h>
#include <windows.h>
#include <math.h>
//#include "../INCLUDE/COMMON.H"
#include "Log.h"
//#include "ScrewDB.h"
#include "logdebug.h"
#include <functional>
#include <string.h>
#include <list>
#include <string>
//#include "../ERRCODE.H"
//#include "ERRTRACE.H"


DWORD	ReportException(DWORD dExitCode, const char* psPos, _Out_ char* pzMsgBuff);
VOID	DumpErr(char* pzSrc, int nErr, char* pzMsg);
char*	GetCnfgValue(char* i_psCnfgFileNm, char* i_psSectionNm, char* i_psKeyNm, char* o_psValue);

//#define	CNFG_PATH	"D:\\cnfg"
#define property(DATATYPE, READ, WRITE) __declspec(property(get=READ, put=WRITE)) DATATYPE

enum { FULLMODE=0, NORMALMODE, DATEMODE, TIMEMODE, HOURMIN, MILLISECMODE };
enum { EN_SUNDAY=0, EN_MONDAY, EN_TUESDAY, EN_WEDNESDAY, EN_THURSDAY, EN_FRIDAY, EN_SATURDAY};

#define FMT_GETTIME_DOT_DATEMODE_LEN			10		/*! YYYY.MM.DD */
#define FMT_GETTIME_DOT_TIMEMODE_LEN			8		/*! HH:MM:SS */
#define FMT_GETTIME_DOT_HOURMIN_LEN				5		/*! HH:MM */
#define FMT_GETTIME_DOT_MILLISECMODE_LEN		(FMT_GETTIME_DOT_TIMEMODE_LEN + 4) /*! HH:MM:SS.mmm */
#define FMT_GETTIME_DOT_FULLMODE_LEN			(FMT_GETTIME_DOT_DATEMODE_LEN + FMT_GETTIME_DOT_MILLISECMODE_LEN)		
#define FMT_GETTIME_DOT_NORMALMODE_LEN			(FMT_GETTIME_DOT_DATEMODE_LEN + FMT_GETTIME_DOT_TIMEMODE_LEN)		

#define FMT_GETTIME_NODOT_DATEMODE_LEN			8		/*! YYYYMMDD */
#define FMT_GETTIME_NODOT_TIMEMODE_LEN			6		/*! HHMMSS */
#define FMT_GETTIME_NODOT_HOURMIN_LEN			4		/*! HHMM */
#define FMT_GETTIME_NODOT_MILLISECMODE_LEN		(FMT_GETTIME_NODOT_TIMEMODE_LEN + 3)	/*! HHMMSSmmm */
#define FMT_GETTIME_NODOT_FULLMODE_LEN			(FMT_GETTIME_NODOT_DATEMODE_LEN + FMT_GETTIME_NODOT_MILLISECMODE_LEN)		
#define FMT_GETTIME_NODOT_NORMALMODE_LEN		(FMT_GETTIME_NODOT_DATEMODE_LEN + FMT_GETTIME_NODOT_TIMEMODE_LEN)		
#define SELFSIZE(F, X) F##(X,sizeof(##X##))
#define MEMCPY(dest, src) memcpy(&dest, src, __min(sizeof(##dest##), sizeof(##src##)))
#define STRMEMCPY(dest, src) memcpy(&dest, src, __min(sizeof(##dest##), lstrlen(src)))
#define MEMSET(dest, fil) memset(&dest, fil, sizeof(##dest##))
#define N2S0(out,in,len) {TCHAR c = *(out+len); sprintf_s( out, len+1, "%0*d", len, in); *(out+len) = c; }

#define LOCK_CS(x) EnterCriticalSection(&##x##);
#define UNLOCK_CS(x) LeaveCriticalSection(&##x##);

#ifndef SAFE_ARR_DELETE
#define SAFE_ARR_DELETE(p)					if (p != NULL){ delete []p; p = NULL;}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)						if (p != NULL){ delete p; p = NULL;}
#endif

#ifndef SAFE_CLOSEHANDLE
#define SAFE_CLOSEHANDLE(p)					if (p != NULL){ CloseHandle(p); p = NULL;}
#endif


#ifndef SAFE_CLOSESOCKET
#define SAFE_CLOSESOCKET(p)					if (p != INVALID_SOCKET){ closesocket(p); p = INVALID_SOCKET;}
#endif

#define MAX_MESSAGE_BUFF	512


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

//#ifndef min
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
//#endif

//////////////////////////////////////////////////////////////////////////
//	COMPONENT 에서 반환할 ㅐ
#define _SetComReturn(pvRet,pvOut,lRet,pMsg)	\
{												\
	VariantCopy(pvRet, &_variant_t(lRet));		\
	VariantCopy(pvOut, &_variant_t(pMsg));		\
}

#define _SetComReturn2(pvRet,pvOut,pvSLen, pvHeader, lRet,pMsg, lSLen, pHeader)	\
{												\
	VariantCopy(pvRet, &_variant_t(lRet));		\
	VariantCopy(pvOut, &_variant_t(pMsg));		\
	VariantCopy(pvSLen, &_variant_t(lSLen));		\
	VariantCopy(pvHeader, &_variant_t(pHeader));		\
}

//====================================
//	형 변환
//====================================
#define	S2N(src,len)					CUtil::Str2N(src,len)
#define	S2D(src,len)					CUtil::Str2D(src,len)
#define	S2L(src,len)					CUtil::Str2L(src,len)
#define	S2LL(src,len)					CUtil::Str2LL(src,len)
#define N2S(out,in,len)					CUtil::N2Str(out, in,len)
//#define N2SZ(out,in)					CUtil::N2StrNull(out, in)
#define L2S(out,in,len)					CUtil::L2Str(out, in,len)
#define LL2S(out,in,len)				CUtil::LL2Str(out, in,len)
#define D2S(out,in,len,dotcnt)			CUtil::D2Str(out, in,len,dotcnt)
#define	VARIANT2DBL(pvt)				CUtil::Variant2Dbl(pvt)
#define	VARIANT2LONG(pvt)				CUtil::Variant2Long(pvt)
#define SUBSTR(buf,start,len)			CUtil::SubString(buf,start,len)
#define TODAY()							CUtil::Get_NowTime(1)
#define NOWTIME()						CUtil::Get_NowTime(0)
#define NOWTIME_SHORT(out)				{SYSTEMTIME st; GetLocalTime(&st); sprintf(out,"%02d:%02d:%02d",st.wHour, st.wMinute, st.wSecond); }
#define NOWTIME_NONE_DOT(out)			{SYSTEMTIME st; GetLocalTime(&st); sprintf(out,"%02d%02d%02d%03d",st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); }
#define TODAY_NONE_DOT(out)				{SYSTEMTIME st; GetLocalTime(&st); sprintf(out,"%04d%02d%02d",st.wYear, st.wMonth, st.wDay); }
#define RTRIM(src,len)					CUtil::RTrim(src, len)
#define	TRIMALL(src,len)				CUtil::LTrim(RTrim(src,len))
//#define CopyRAlign(dest,src, dSize, sSize, filler)	CUtil::CopyRAlign(dest,src, dSize, sSize, filler)
#define REPLACE(start, len, before, after)	CUtil::Replace(start,len,before,after)
#define TRUNCDBL(src)					CUtil::TruncDbl(src);
#define	TRUNCDBL2(src, point)			CUtil::TruncDbl2(src,point)
#define	ROUND(src, OffSet)				CUtil::Round(src, OffSet)


char* MakeGUID(char *pzGUID);

class GetSvcNameT
	: public std::unary_function<VOID, LPCTSTR>
{
public:
	LPCTSTR operator()(VOID){
		static TCHAR szSvcName[_MAX_FNAME];

		TCHAR* pTemp;
		TCHAR szTemp[MAX_PATH];

		ZeroMemory(szTemp, sizeof(szTemp));
		GetModuleFileName(NULL, szTemp, sizeof(szTemp));

		//// get right after dir path
		pTemp = strrchr(szTemp, '\\');
		if (pTemp)
			pTemp++;
		else
			pTemp = szTemp;

		lstrcpy(szSvcName,	pTemp);

		//// truncate ext
		pTemp = strrchr(szSvcName, '.');
		if (pTemp)
			*pTemp = '\0';

		sprintf(szTemp, "BLASH_%s", szSvcName);
		lstrcpy(szSvcName, szTemp);
		return szSvcName;
	}
};

class CUtil
{
public:
	CUtil();
	virtual ~CUtil();

	static	TCHAR*	Get_NowTime(BOOL bFull=FALSE);
	static	int		Get_WeekDay();
	static	char*	GetToday(char* pzToday);
	static	char*	GetYesterday(char* pzYesterday);
	static	void	InitBuff(TCHAR* pBuf, int nLen, TCHAR Filler);
	static	void	RTrim( TCHAR* pBuf, int nScope );
	static	void	LTrim( TCHAR* pBuf);
	static	void	TrimAll( TCHAR* pBuf, int nScope );
	static	TCHAR*	TrimAllEx( TCHAR* pBuf, int nScope );
	static	void	RemoveDot(TCHAR* pData, int nLen);
	static	void	Remove_TCHAR(TCHAR* pData, int nLen, TCHAR i_cTarget);
	static	void	Remove_char(char* pData, int nLen, char i_cTarget);
	static	void	Replace(TCHAR* pSrc, int nChgLen, TCHAR cSrc, TCHAR cChg);
	static	bool	Is_PassedDay(TCHAR* pCompared, bool bChange, bool bDot=false);
	//static VOID	CopyRAlign(TCHAR* pDest, const TCHAR *pSrc, long destSize, long srcSize,TCHAR cFiller);

	static	double	TruncDbl(double dSrc);
	static	double	TruncDbl2(double dSrc, double dPos);
	static	double	Round(double src, int nOffSet);
	static	double	roundoff(double src, int offset);
	static	TCHAR*	SubString(TCHAR* p, int start, int len);
	static	long	Variant2Long(VARIANT* pVt);
	
	static	int		Str2N( TCHAR* pszIn, int nLen=0 );
	static	int		Str2L( TCHAR* pszIn, int nLen=0 );
	static	double	Str2D( char* pszIn, int nLen=0 );
	static	LONGLONG Str2LL( TCHAR* pszIn, int nLen=0 );
	static	TCHAR*	N2Str(  TCHAR* result, int nIn, int nLen, BOOL bNullPointing=FALSE);
	static	TCHAR*	L2Str(  TCHAR* result, LONG lIn, int nLen, BOOL bNullPointing=FALSE);
	static	TCHAR*	D2Str(  TCHAR* result, double dIn, int nLen, int nDot, BOOL bNullPointing=FALSE);
	static	TCHAR*	LL2Str( TCHAR* result, LONGLONG llIn, int nLen, BOOL bNullPointing=FALSE);
//	static	TCHAR*	ResizeStr_Int( TCHAR* pOrg, int nOrgLen, TCHAR* pOut, int nOutLen );
//	static	TCHAR*	ResizeStr_Dbl( TCHAR* pOrg, int nOrgLen, TCHAR* pOut, int nOutLen );

	static	int		GetDecimalCnt( TCHAR* psNTTN );	//	소숫점 이하 갯수
	static	BOOL	IsPassedMin( TCHAR* i_psLastTime, int i_nBaseMin );	//	분(minitue)가 지났는지 여부
	static	TCHAR*	CvtInt_SpaceLeading( TCHAR* i_psOrg, int i_nOrgLen, int i_nOutLen, TCHAR* o_psOut );
	static	TCHAR*	CvtDbl_SpaceLeading( TCHAR* i_psOrg, int i_nOrgLen, int i_nOutLen, TCHAR* o_psOut );

	//static	VOID	GetYearMonSymbol_KS200(TCHAR* o_psYearSymbol, TCHAR* o_psMonSymbol);


	static VOID PrintErr(CLog* pLog, BOOL bDebug, int nErrNo);
	static void LogMsg( CLog *log, BOOL bSucc, char* pMsg, ...) ;
	static char* LogMsgEx( CLog *log, BOOL bSucc, char* pOut, char* pMsg, ...) ;
	static void LogEmergency( CLog *log, BOOL bSucc, char* pMsg, ...) ;
	static void LogPrint( CLog *log, BOOL bSucc, char* pMsg, ...) ;
	
	static void		GetMyModuleAndDir(char *o_psDir, char* o_psModule, char* o_psConfig);
	static	char*	GetCnfgFileNm(char *i_psDir, char* i_psFileNm, char* o_psValue );
	static	char*	GetCnfgXMLFileNm(char *i_psDir, char* i_psFileNm, char* o_psValue);
	static	char*	GetConfig(char* i_psCnfgFileNm, char* i_psSectionNm, char* i_psKeyNm, char* o_psValue );
	static  BOOL	GetNextConfigData(char* pzCnfgFileNm, char* pzSectionNm, char* pzPrevKeyNm, char* o_pzNextKeyNm, char* o_pzNextValue);
	static int		GetTickGap(double dFirstPrc, double dSndPrc, int nDotCnt, double dTickSize);
	static double	GetPrcByTick(char* pzOrigPrc, double dTickCnt, double dTickSize, char cPlusMinus);
	static int CompPrc(const char* pPrc1, const int nLen1, const char* pPrc2, const int nLen2, const int nDotCnt, const int nFormatLen);
	static int CompPrc(const double pPrc1, const double pPrc2, const int nDotCnt, const int nFormatLen);
	static BOOL IsSamePrice( char* pPrc1,  int nLen1,  char* pPrc2,  int nLen2,  int nDotCnt,  int nFormatLen);

	//static	BOOL	Send2MngMQ(char* pszLabel, char* pszBody);
	//static	BOOL	Send2FrontMQ(char* pszLabel, char* pszBody);

	static VOID FormatErrMsg(_In_ int nErrNo, _Out_ char* pzMsg);
	static VOID SplitData(_In_ char* psData, _In_ char cDelimeter, _Out_ std::list<std::string>* pListResult);
	static VOID SplitDataEx(_In_ char* psData, _In_ char cDelimeter, _In_ int nSize, _Out_ std::list<std::string>* pListResult);
};





