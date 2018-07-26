
//#include <afx.h>
#include <stdio.h>
#include "Util.h"
#include "Prop.h"
//#include "../INCLUDE/COMMON.H"
//#include "MQBase.h"

#pragma warning(disable : 4530)



DWORD	ReportException(DWORD dExitCode, const char* psPos, _Out_ char* pzMsgBuff) // 20120510
{
	switch (dExitCode)
	{
	case EXCEPTION_ACCESS_VIOLATION: strcpy(pzMsgBuff, "EXCEPTION_ACCESS_VIOLATION");		break;
	case EXCEPTION_BREAKPOINT: strcpy(pzMsgBuff, "EXCEPTION_BREAKPOINT");			break;
	case EXCEPTION_DATATYPE_MISALIGNMENT: strcpy(pzMsgBuff, "EXCEPTION_DATATYPE_MISALIGNMENT"); break;
	case EXCEPTION_SINGLE_STEP: strcpy(pzMsgBuff, "EXCEPTION_SINGLE_STEP");			break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: strcpy(pzMsgBuff, "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"); break;
	case EXCEPTION_FLT_DENORMAL_OPERAND: strcpy(pzMsgBuff, "EXCEPTION_FLT_DENORMAL_OPERAND");	break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: strcpy(pzMsgBuff, "EXCEPTION_FLT_DIVIDE_BY_ZERO");	break;
	case EXCEPTION_FLT_INEXACT_RESULT: strcpy(pzMsgBuff, "EXCEPTION_FLT_INEXACT_RESULT");	break;
	case EXCEPTION_FLT_INVALID_OPERATION: strcpy(pzMsgBuff, "EXCEPTION_FLT_INVALID_OPERATION"); break;
	case EXCEPTION_FLT_OVERFLOW: strcpy(pzMsgBuff, "EXCEPTION_FLT_OVERFLOW");			break;
	case EXCEPTION_FLT_STACK_CHECK: strcpy(pzMsgBuff, "EXCEPTION_FLT_STACK_CHECK");		break;
	case EXCEPTION_FLT_UNDERFLOW: strcpy(pzMsgBuff, "EXCEPTION_FLT_UNDERFLOW");		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO: strcpy(pzMsgBuff, "EXCEPTION_INT_DIVIDE_BY_ZERO");	break;
	case EXCEPTION_INT_OVERFLOW: strcpy(pzMsgBuff, "EXCEPTION_INT_OVERFLOW");			break;
	case EXCEPTION_PRIV_INSTRUCTION: strcpy(pzMsgBuff, "EXCEPTION_PRIV_INSTRUCTION");		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: strcpy(pzMsgBuff, "EXCEPTION_NONCONTINUABLE_EXCEPTION"); break;
	default:sprintf(pzMsgBuff, "[except code:%d]undefined error", dExitCode); break;
	}
	return EXCEPTION_EXECUTE_HANDLER;
}



CUtil::CUtil(){}
CUtil::~CUtil(){}


/**	\brief	The CUtil::Get_NowTime function

	\param	pTime	a parameter of type TCHAR*
	\param	nMode	a parameter of type UINT
	\param	bDot	a parameter of type BOOL

	\return	void

	* 현재 시각을 가져온다.
	- FULL : YYYYMMDDHHMMSSMMM
	- NORMAL : YYYYMMDDHHMMSS
	- DATE : YYYYMMDD
	- TIME : HHMMSS
	- bDot : YYYY.MM.DD	
*/

TCHAR* CUtil::Get_NowTime(BOOL bFull)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	static TCHAR time[32];
	if(bFull){
		sprintf_s(time, "%04d.%02d.%02d %02d:%02d:%02d.%03d",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}
	else{
		sprintf_s(time, "%02d:%02d:%02d.%03d",st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}
	
	return time;
} 


/**	\brief	The CUtil::Get_WeekDay function


	\return	int

	* 오늘의 요일을 가져온다.
	- enum { SUNDAY=0, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY };	
*/

int CUtil::Get_WeekDay()
{
	time_t ltime;
    struct tm *today = NULL;
	time( &ltime );
	localtime_s(today, &ltime );
	return today->tm_wday;
}

// 오늘일자반환
char* CUtil::GetToday(char* pzToday)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(pzToday, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	return pzToday;
}


// 어제일자 반환
char* CUtil::GetYesterday(char* pzYesterday)
{
	time_t timer;
	struct tm *t;


	timer = time(NULL) - (24 * 60 * 60);
	t = localtime(&timer);

	sprintf(pzYesterday, "%04d%02d%02d",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

	return pzYesterday;
}

/**	\brief	The CUtil::InitBuff function

	\param	pBuf	a parameter of type TCHAR*
	\param	nLen	a parameter of type int
	\param	Filler	a parameter of type TCHAR

	\return	void

	버퍼의 초기화(Filler 로 채운다.)
*/

void CUtil::InitBuff(TCHAR* pBuf, int nLen, TCHAR Filler)
{
	if(Filler==0x20){
		FillMemory(pBuf, nLen-1, 0x20);
		*(pBuf+nLen-1) = 0x00;
	}
	if(Filler==0x00)
		ZeroMemory(pBuf, nLen);
}


void CUtil::RTrim( TCHAR* pBuf, int nScope )
{
	for( int i=nScope-1; i>=0; i--)
	{
		if( *( pBuf+i ) == 0x20 ||
			*( pBuf+i ) == '\t'	||
			*( pBuf+i ) == '\r'	||
			*( pBuf+i ) == '\n'	
			)
		{
			*( pBuf+i ) = 0x00;
		}
		else
		{
			break;
		}
	}
}

void CUtil::LTrim( TCHAR* pBuf)
{
	int nLen = lstrlen(pBuf);
	TCHAR *pTmp = new TCHAR[nLen+1];
	lstrcpy(pTmp, pBuf);

	int nPos = 0;
	for(int i=0; i<nLen; i++){
		if( *(pTmp+nPos)==0x20 ||
			*(pTmp+nPos)=='\n' ||
			*(pTmp+nPos)=='\r' ||
			*(pTmp+nPos)=='\t' 
			)
		{
			nPos++;
		}
		else
		{
			break;
		}
	}

	lstrcpy(pBuf, pTmp+nPos);
	delete[] pTmp;

}

TCHAR* CUtil::TrimAllEx( TCHAR* pBuf, int nScope )
{
	static TCHAR b_szRESULT_BUFFER[256] = {0,};
	sprintf_s( b_szRESULT_BUFFER, "%.*s", nScope, pBuf );
	CUtil::RTrim(b_szRESULT_BUFFER, nScope );
	CUtil::LTrim(b_szRESULT_BUFFER);
	return b_szRESULT_BUFFER;
}

void CUtil::TrimAll( TCHAR* pBuf, int nScope )
{
	CUtil::RTrim(pBuf,nScope);
	CUtil::LTrim(pBuf);
}

/**	\brief	The CUtil::RemoveDot function

	\param	pData	a parameter of type TCHAR *
	\param	nLen	a parameter of type int

	\return	void

	패킷에 있는 '.' 을 제거한다.
*/

void CUtil::RemoveDot(TCHAR *pData, int nLen)
{
	int nChangedLen = nLen;
	for(int i=0; i<nLen; i++)
	{
		if(*(pData+i)=='.')
		{
			memmove(pData+i, pData+i+1, nLen-i-2);
			nChangedLen--;
		}
	}
	*(pData+nChangedLen-1) = 0x00;
}

/**	\brief	The CUtil::RemoveDot function

	\param	pData	a parameter of type TCHAR *
	\param	nLen	a parameter of type int

	\return	void

	패킷에 있는 특정 TCHAR ('-','/' 등등) 을 제거한다.
*/

void CUtil::Remove_TCHAR(TCHAR* pData, int nLen, TCHAR i_cTarget)
{
	int nChangedLen = nLen;
	for(int i=0; i<nLen; i++)
	{
		if(*(pData+i)==i_cTarget)
		{
			memmove(pData+i, pData+i+1, nLen-i-2);
			nChangedLen--;
		}
	}
	*(pData+nChangedLen-1) = 0x00;
}



/**	\brief	The CUtil::RemoveDot function

	\param	pData	a parameter of type TCHAR *
	\param	nLen	a parameter of type int

	\return	void

	패킷에 있는 특정 TCHAR ('-','/' 등등) 을 제거한다.
*/

void CUtil::Remove_char(char* pData, int nLen, char i_cTarget)
{
	int nChangedLen = nLen;
	for(int i=0; i<nLen; i++)
	{
		if(*(pData+i)==i_cTarget)
		{
			memmove(pData+i, pData+i+1, nLen-i-2);
			nChangedLen--;
		}
	}
	*(pData+nChangedLen-1) = 0x00;
}


/**	\brief	The CUtil::Replace function

	\param	pSrc	a parameter of type TCHAR*
	\param	nChgLen	a parameter of type int
	\param	cSrc	a parameter of type TCHAR
	\param	cChg	a parameter of type TCHAR

	\return	void

	* pSrc 에 포함된 cSrc 를 cChg 로 변경한다.
*/

void CUtil::Replace(TCHAR* pSrc, int nChgLen, TCHAR cSrc, TCHAR cChg)
{
	for(int i=0; i<nChgLen;i++)
	{
		if(*(pSrc+i) == cSrc)
			*(pSrc+i) = cChg;
	}
}


/**	\brief	The CUtil::Is_PassedDay function

	\param	pCompared	a parameter of type TCHAR*
	\param	bChange	a parameter of type bool
	\param	bDot	a parameter of type bool

	\return	bool

	일자가 지났는지 판단한다.
*/

bool CUtil::Is_PassedDay(TCHAR* pCompared, bool bChange, bool bDot)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR szToday[11];

	int nComp = 0;
	if(bDot){
		sprintf_s(szToday, "%04d.%02d.%02d", st.wYear, st.wMonth, st.wDay);
		nComp = strncmp(szToday, pCompared, 10);
	}
	else{
		sprintf_s(szToday, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
		nComp = strncmp(szToday, pCompared, 8);
	}

	if(nComp>0)
	{
		if(bChange)
			sprintf_s(pCompared, lstrlen(szToday)+1, "%.*s", lstrlen(szToday), szToday);
	}

	return (nComp>0);
}


/**	\brief	double 형 데이터의 소숫점 이하를 떼어내고 정수부분만 리턴한다.

  \param	dSrc	a parameter of type double
  
	\return	static inline double
*/
double CUtil::TruncDbl(double dSrc)
{
	double dInt = 0;
	modf(dSrc, &dInt);
	return dInt;
}


/**	\brief	double 형 데이터의 일정 자릿수 이하의 소숫점 버린다.

  \param	dSrc	변환하고자 하는 수
  \param	dPos	자릿수 위치
  
	\return	static inline double
	
	  ex) 123.123 을 소수점 2자리 이하는 버린다.
	  => TruncDbl2(123.123, 2)
*/
double CUtil::TruncDbl2(double dSrc, double dPos)
{
	double dInt = 0;
	double dMultiple = pow(10, dPos);	//	100
	double dSrcCopy = dSrc;				//	123.123
	
	dSrcCopy = dSrcCopy * dMultiple;	//	dSrcCopy = 12312.3
	modf(dSrcCopy, &dInt);				//	dInt = 12312
	
	double dResult = dInt / dMultiple;	//	dResult = 12312 / 100 = 123.12
	return dResult;
}


/**	\brief	double 형 데이터의 소수점 이상의 일정 자리는 버린다.

  \param	src			변환하고자 하는 수
  \param	nOffSet		버리고자 하는 자릿수
  
	\return	static inline double
	
	  ex) 12345 ==> 10000
	  Round(12345, 4)
	  
*/
double CUtil::Round(double src, int nOffSet)
{
	double dMultiple = pow((double)10, (double)nOffSet);	//	10000
	
	//	floor : 해당 수 이하 최대 정수 ex) 1.2345 => 1
	double dRet = floor( src / dMultiple );	//	dRet = 1
	dRet *= dMultiple;						//	dRet = 1 * 10000 = 10000
	return dRet;
}


/*
	반올림.

	56.349  => 소수세자리에서 반올림 : 56.34

	roundoff(56.349, 2)
*/
double CUtil::roundoff(double src, int offset)
{
	double dMultiple = pow((double)10, (double)offset);	//	100

	int nRet = (int)((src * dMultiple)+0.5);	//	nRet = 56.349*100 + 0.5 = 5634.9 + 0.5 = 5635.4 => 5635
	double dRet = (double)(nRet / dMultiple);	//	dRet = 5635 / 100 = 56.35
	return dRet;
}


// VOID CUtil::CopyRAlign(TCHAR* pDest, const TCHAR *pSrc, long destSize, long srcSize,TCHAR cFiller)
// {
// 	//	Filler 로 초기화
// 	FillMemory(pDest, destSize, cFiller);
// 	
// 	//	할당한 메모리 block 보다 copy할 block 이 더 크면 
// 	//	할당된 메모리 block 크기로 맞춘다.
// 	if(srcSize>destSize)
// 		srcSize = destSize;
// 	long lPos = destSize - srcSize;
// 	CopyMemory( pDest+lPos, pSrc, srcSize );
// }


/**	\brief	TCHAR string 을 원하는 만큼 자른다.

  \param	p		src 데이터
  \param	start	시작점 
  \param	len		자르는 길이
  
	\return	static inline TCHAR*
*/
TCHAR* CUtil::SubString(TCHAR* p, int start, int len)
{
	static TCHAR result[128];
	sprintf_s(result, "%.*s", len, p+start);
	return result;
}


/**	\brief	VARIANT TYPE 을 double 로 변환

  \param	pVt	a parameter of type VARIANT*
  
	\return	static inline double
*/
// double CUtil::Variant2Dbl(VARIANT* pVt)
// {
// 	HRESULT hr;
// 	hr = VariantChangeType(pVt, pVt, 0, VT_R8);
// 	return pVt->dblVal;
// }

/**	\brief	VARIANT TYPE 을 long 로 변환

  \param	pVt	a parameter of type VARIANT*
  
	\return	static inline double
*/
long CUtil::Variant2Long(VARIANT* pVt)
{
	HRESULT hr;
	hr = VariantChangeType(pVt, pVt, 0,  VT_I4);
	return pVt->lVal;
}


	//////////////////////////////////////////////////////////////////////////
	//	string 을 여러 숫자 type 으로 변환
	//////////////////////////////////////////////////////////////////////////

/**	\brief	TCHAR string 을 int 형으로 변환

	\param	pszIn	a parameter of type TCHAR*
	\param	nLen	a parameter of type int

	\return	static inline int
*/
int CUtil::Str2N( TCHAR* pszIn, int nLen )
{
	if(!nLen)	return atoi(pszIn);

	TCHAR result[128];
	sprintf_s(result, "%*.*s", nLen, nLen, pszIn);
	//ReplaceChr(result, result+nLen, 0x20, '0');
	return atoi(result);
}

/**	\brief	TCHAR string 을 long 형으로 변환

	\param	pszIn	a parameter of type TCHAR*
	\param	nLen	a parameter of type int

	\return	static inline int
*/
int CUtil::Str2L( TCHAR* pszIn, int nLen )
{
	if(!nLen)	return atoi(pszIn);

	TCHAR result[128];
	sprintf_s(result, "%*.*s", nLen, nLen, pszIn);
	//ReplaceChr(result, result+nLen, 0x20, '0');
	return atol(result);
}

/**	\brief	TCHAR string 을 double 형으로 변환

	\param	pszIn	a parameter of type TCHAR*
	\param	nLen	a parameter of type int

	\return	static inline int
*/
double CUtil::Str2D( char* pszIn, int nLen )
{
	if(!nLen)	return atof(pszIn);

	TCHAR result[128];
	sprintf_s(result, "%*.*s", nLen, nLen, pszIn);
	//ReplaceChr(result, result+nLen, 0x20, '0');
	return atof(result);
}

/**	\brief	TCHAR string 을 LONGLONG 형으로 변환

	\param	pszIn	a parameter of type TCHAR*
	\param	nLen	a parameter of type int

	\return	static inline int
*/
LONGLONG CUtil::Str2LL( TCHAR* pszIn, int nLen )
{
	if(!nLen)	return _atoi64(pszIn);

	TCHAR result[128];
	sprintf_s(result, "%*.*s", nLen, nLen, pszIn);
	//ReplaceChr(result, result+nLen, 0x20, '0');
	return _atoi64(result);
}

//	TCHAR* sprintf_s_d( TCHAR* pszIn, TCHAR* pOut, int nLen )
//	{
//		int n = StrToN(pIn, nResultBufLen);
//		NToStr(n,nResultBufLen,true,pOut);
//		return pOut;
//	}


	//////////////////////////////////////////////////////////////////////////
	//	여러 숫자 type 을 str 으로 변환
	//////////////////////////////////////////////////////////////////////////

/**	\brief	int 형을 TCHAR string 형으로 변환

	\param	result	a parameter of type TCHAR*
	\param	nIn	a parameter of type int
	\param	nLen	a parameter of type int
	\param	bNullPointing	a parameter of type BOOL

	\return	static inline TCHAR*
	
	
*/
TCHAR* CUtil::N2Str(  TCHAR* result, int nIn, int nLen, BOOL bNullPointing)
{
	bool bMinus = false;
	if(nIn<0){
		nIn *= -1;
		bMinus=true;
	}

	TCHAR tmp[128];
	sprintf_s( tmp, "%*d", nLen, nIn);
	CopyMemory(result, tmp, nLen);
	if(bMinus)
	{
		result[0] = '-';
		Replace(result, nLen, 0x20, '0');
	}
	if(bNullPointing)
		result[nLen] = 0x00;
	return result;
}

// TCHAR* CUtil::N2StrNull(  TCHAR* result, int nIn)
// {
// 	sprintf_s(result, "%d", nIn);
// 	return result;
// }

/**	\brief	long 형을 TCHAR string 형으로 변환

	\param	result	a parameter of type TCHAR*
	\param	lIn	a parameter of type LONG
	\param	nLen	a parameter of type int
	\param	bNullPointing	a parameter of type BOOL

	\return	static inline TCHAR*

	
*/
TCHAR* CUtil::L2Str(  TCHAR* result, LONG lIn, int nLen, BOOL bNullPointing)
{
	bool bMinus = false;
	if(lIn<0){
		lIn *= -1;
		bMinus=true;
	}

	TCHAR tmp[128];
	sprintf_s( tmp, "%*ld", nLen, lIn);
	CopyMemory(result, tmp, nLen);
	if(bMinus)
	{
		result[0] = '-';
		Replace(result, nLen, 0x20, '0');
	}
	if(bNullPointing)
		result[nLen] = 0x00;
	return result;
}

/**	\brief	double 형을 TCHAR string 형으로 변환

	\param	result	a parameter of type TCHAR*
	\param	dIn	a parameter of type double
	\param	nLen	a parameter of type int
	\param	nDot	a parameter of type int
	\param	bNullPointing	a parameter of type BOOL

	\return	static inline TCHAR*

	*	값은 뒤로 정렬된다.
		-1234 를 10BYTE 버퍼에 저장을 하면
		"     -1234" 가 된다.
*/
TCHAR* CUtil::D2Str(  TCHAR* result, double dIn, int nLen, int nDot, BOOL bNullPointing)
{	
//		bool bMinus = false;
//		if(dIn<0){
//			dIn*=-1.;
//			bMinus=true;
//		}
//
//		TCHAR tmp[128];
//		sprintf_s( tmp, "%*.*f", nLen, nDot, dIn);
//		int len = lstrlen(tmp);
//		
//		CopyMemory(result, tmp, nLen);
//		if(bMinus)
//		{
//			result[0] = '-';
//			Replace(result, nLen, 0x20, '0');
//		}
	FillMemory(result, nLen, 0x20);
	TCHAR tmp[128];
	sprintf_s( tmp, "%.*f", nDot, dIn);
	int size = lstrlen(tmp);

	//	필요길이보다 데이터 길이가 더 길면 데이터를 자른다.
	if(size>nLen)
	{
		int nDiff = size-nLen;
		size -= nDiff;
		tmp[size] = 0x00;
	}

	CopyMemory(result+(nLen-size), tmp, size);

	if(bNullPointing)
		result[nLen] = 0x00;
	return result;
}

	//////////////////////////////////////////
	//	LLToStr
	//	- dIn : 변환하고자 하는 수


/**	\brief	LONGLONG 형을 TCHAR string 형으로 변환

	\param	result	a parameter of type TCHAR*
	\param	llIn	a parameter of type LONGLONG
	\param	nLen	a parameter of type int
	\param	bNullPointing	a parameter of type BOOL

	\return	static TCHAR*

	
*/
TCHAR* CUtil::LL2Str( TCHAR* result, LONGLONG llIn, int nLen, BOOL bNullPointing)
{
	bool bMinus = false;
	if(llIn<0){
		llIn *= -1;
		bMinus=true;
	}

	TCHAR tmp[128];
	sprintf_s( tmp, "%*I64d", nLen, llIn);
	CopyMemory(result, tmp, nLen);
	if(bMinus)
	{
		result[0] = '-';
		Replace(result, nLen, 0x20, '0');
	}
	if(bNullPointing)
		result[nLen] = 0x00;
	return result;
}

VOID CUtil::FormatErrMsg(_In_ int nErrNo, _Out_ char* pzMsg)
{
	LPVOID lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErrNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);

	strcpy(pzMsg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

VOID CUtil::PrintErr(CLog* pLog, BOOL bDebug, int nErrNo)
{
	LPVOID lpMsgBuf=NULL;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErrNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	if (bDebug) 
	{
		printf("[Error](%d)(%s)",nErrNo, (LPCTSTR)lpMsgBuf);
	}
	
	if(pLog)
	{
		pLog->Log("[Error](%d)(%s)",nErrNo, (LPCTSTR)lpMsgBuf);
	}
	
	LocalFree( lpMsgBuf );
}



#define	DEF_BUF_LEN	4096

void CUtil::LogMsg( CLog *log, BOOL bSucc, char* pMsg, ...)
{
	char buff1[DEF_BUF_LEN];
	char buff2[DEF_BUF_LEN];
	va_list argptr;
	SYSTEMTIME	st;
	
	if(lstrlen(pMsg)>=DEF_BUF_LEN)
		*(pMsg+DEF_BUF_LEN-1) = 0x00;

	va_start(argptr, pMsg);
	vsprintf_s(buff1, pMsg, argptr);
	va_end(argptr);
	
	
	GetLocalTime(&st);
	if(bSucc)
		sprintf(buff2, "[I][%02d:%02d:%02d.%03d]%s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);
	else
		sprintf(buff2, "[F][%02d:%02d:%02d.%03d]%s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);

	log->LogEx(buff2);
}


void CUtil::LogPrint( CLog *log, BOOL bSucc, char* pMsg, ...)
{
	char buff1[DEF_BUF_LEN];
	char buff2[DEF_BUF_LEN];
	va_list argptr;
	SYSTEMTIME	st;
	
	if(lstrlen(pMsg)>=DEF_BUF_LEN)
		*(pMsg+DEF_BUF_LEN-1) = 0x00;

	va_start(argptr, pMsg);
	vsprintf_s(buff1, pMsg, argptr);
	va_end(argptr);
	
	
	GetLocalTime(&st);
	if(bSucc)
		sprintf(buff2, "[I][%02d:%02d:%02d.%03d]%s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);
	else
		sprintf(buff2, "[F][%02d:%02d:%02d.%03d]%s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);

	log->LogEx(buff2);
	printf(buff2);
}


char* CUtil::LogMsgEx( CLog *log, BOOL bSucc, char* pOut, char* pMsg, ...)
{
	char buff1[DEF_BUF_LEN];
	va_list argptr;
	SYSTEMTIME	st;
	
	if(lstrlen(pMsg)>=DEF_BUF_LEN)
		*(pMsg+DEF_BUF_LEN-1) = 0x00;

	va_start(argptr, pMsg);
	vsprintf_s(buff1, pMsg, argptr);
	va_end(argptr);
	
	
	GetLocalTime(&st);
	if(bSucc)
		sprintf(pOut, "[I][%02d:%02d:%02d.%03d]%s\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);
	else
		sprintf(pOut, "[F][%02d:%02d:%02d.%03d]%s\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);

	log->log(pOut);
	return pOut;
}




void CUtil::LogEmergency( CLog *log, BOOL bSucc, char* pMsg, ...)
{
	char buff1[DEF_BUF_LEN];
	char buff2[DEF_BUF_LEN];
	va_list argptr;
	SYSTEMTIME	st;
	
	if(lstrlen(pMsg)>=DEF_BUF_LEN)
		*(pMsg+DEF_BUF_LEN-1) = 0x00;

	va_start(argptr, pMsg);
	vsprintf_s(buff1, pMsg, argptr);
	va_end(argptr);
	
	
	GetLocalTime(&st);
	sprintf(buff2, "[E][%02d:%02d:%02d.%03d]%s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buff1);

	log->LogEx(buff2);
}



/************************************************************************
	진법코드를 받아서 소숫점 이하 갯수를 반환한다.

	D0, D1, D2
************************************************************************/
int	CUtil::GetDecimalCnt( TCHAR* psNTTN )
{
	int nRet;
	int nLen = strlen(psNTTN);
	TCHAR temp[3];
	sprintf_s(temp, "%.*s", nLen-1, psNTTN+1 );
	nRet = atoi(temp);
	return nRet;
}

/************************************************************************
	현재 시각이 정해진 시간(분)을 지났는지 여부

	-- i_psLastTime : hhmmss 형태
************************************************************************/
BOOL CUtil::IsPassedMin( TCHAR* i_psLastTime, int i_nBaseMin )
{
	TCHAR	b_szNowH[32];
	TCHAR	b_szNowM[32];

	TCHAR	b_szLastH[32];
	TCHAR	b_szLastM[32];

	SYSTEMTIME	st;

	GetLocalTime(&st);

	sprintf_s( b_szLastH, "%.2s", i_psLastTime);
	sprintf_s( b_szNowH, "%02d", st.wHour );

	//	HOUR 가 변했으면 이미 지난 것임
	if( atoi(b_szNowH) > atoi(b_szLastH) )	
		return TRUE;

	sprintf_s( b_szLastM, "%.2s", i_psLastTime+2);
	sprintf_s( b_szNowM, "%02d", st.wMinute );

	int nGapMin = atoi( b_szNowM ) - atoi(b_szLastM);

	return ( nGapMin>i_nBaseMin );

}


///************************************************************************
//	오늘 현재에 해당하는 KS200 선물, 옵션의 Year, Month 기호 반환
//************************************************************************/
//VOID CUtil::GetYearMonSymbol_KS200(TCHAR* o_psYearSymbol, TCHAR* o_psMonSymbol)
//{
//	TCHAR	szYear[5], szMon[3];
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//	sprintf_s( szYear, "%04d", st.wYear );
//	sprintf_s( szMon, "%02d", st.wMonth );
//
//	if( strcmp( szYear, "2010")==0 )		lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_10 );
//	else if( strcmp( szYear, "2011")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_11 );
//	else if( strcmp( szYear, "2012")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_12 );
//	else if( strcmp( szYear, "2013")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_13 );
//	else if( strcmp( szYear, "2014")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_14 );
//	else if( strcmp( szYear, "2015")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_15 );
//	else if( strcmp( szYear, "2016")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_16 );
//	else if( strcmp( szYear, "2017")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_17 );
//	else if( strcmp( szYear, "2018")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_18 );
//	else if( strcmp( szYear, "2019")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_19 );
//	else if( strcmp( szYear, "2020")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_20 );
//	else if( strcmp( szYear, "2021")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_21 );
//	else if( strcmp( szYear, "2022")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_22 );
//	else if( strcmp( szYear, "2023")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_23 );
//	else if( strcmp( szYear, "2024")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_24 );
//	else if( strcmp( szYear, "2025")==0 )	lstrcpy( o_psYearSymbol, DEF_KS200_YEAR_25 );
//	else sprintf_s( o_psYearSymbol, 2,  "%.1s", szYear+3 );
//
//	if( strcmp(szMon, "10") )		lstrcpy( o_psMonSymbol, DEF_KS200_MON_10 );
//	else if( strcmp(szMon, "11") )	lstrcpy( o_psMonSymbol, DEF_KS200_MON_11 );
//	else if( strcmp(szMon, "12") )	lstrcpy( o_psMonSymbol, DEF_KS200_MON_12 );
//	else sprintf_s( o_psMonSymbol, 2, "%.1s", szMon+1 );
//
//	return ;
//
//}



/************************************************************************
	"0000123" => "    123" 과 같이 변경 (클라이언트(델파이)를 위해)
************************************************************************/
TCHAR* CUtil::CvtInt_SpaceLeading( TCHAR* i_psOrg, int i_nOrgLen, int i_nOutLen, TCHAR* o_psOut )
{
	sprintf_s( o_psOut, i_nOrgLen, "%.*s", i_nOrgLen, i_psOrg );

	int nOrg = atoi(o_psOut);

	sprintf_s( o_psOut, i_nOutLen, "%*d", i_nOutLen, nOrg );

	return o_psOut;	
}


/************************************************************************
	"0000123" => "    123" 과 같이 변경 (클라이언트(델파이)를 위해)
************************************************************************/
TCHAR* CUtil::CvtDbl_SpaceLeading( TCHAR* i_psOrg, int i_nOrgLen, int i_nOutLen, TCHAR* o_psOut )
{
sprintf_s(o_psOut, i_nOrgLen, "%.*s", i_nOrgLen, i_psOrg);

double dOrg = atof(o_psOut);

sprintf_s(o_psOut, i_nOutLen, "%*f", i_nOutLen, dOrg);

return o_psOut;
}


void CUtil::GetMyModuleAndDir(char *o_psDir, char* o_psModule, char* o_psConfig)
{

	// config file
	char szFullName[_MAX_PATH];
	GetModuleFileName(NULL, szFullName, _MAX_PATH);

	int nLen = strlen(szFullName);
	for (int i = nLen - 1; i > 0; i--)
	{
		if (szFullName[i] == '\\')
		{
			sprintf(o_psDir, "%.*s", i, szFullName);
			strcpy(o_psModule, &(szFullName[i + 1]));
			strcpy(o_psConfig, GetCnfgFileNm(o_psDir, o_psModule, o_psConfig));
			return;
		}
	}
}


/*
	config file 이름
*/
char* CUtil::GetCnfgFileNm(char *i_psDir, char* i_psFileNm, char* o_psValue)
{
	char szDir[MAX_PATH], szFileNm[MAX_PATH];
	strcpy(szDir, i_psDir);
	if (i_psDir[strlen(i_psDir) - 1] != '\\')
		strcat(szDir, "\\");


	char temp[1024];
	strcpy(temp, i_psFileNm);
	_strupr(temp);
	char* pos = strstr(temp, ".EXE");
	if (pos == 0)
	{
		sprintf(szFileNm, "%s%s.ini", szDir, i_psFileNm);
	}
	else
	{
		int nLen = strlen(i_psFileNm) - strlen(pos);
		sprintf(szFileNm, "%s%.*s.ini", szDir, nLen, i_psFileNm);
	}

	strcpy(o_psValue, szFileNm);
	return o_psValue;
}



char* CUtil::GetConfig(char* i_psCnfgFileNm, char* i_psSectionNm, char* i_psKeyNm, char* o_psValue)
{
	*o_psValue = 0x00;
	DWORD dwRET = GetPrivateProfileString(i_psSectionNm, i_psKeyNm, NULL, o_psValue, 1024, (LPCSTR)i_psCnfgFileNm);
	// 주석은 제거
	char* pComment = strstr(o_psValue, "//");
	if (pComment)
		*(pComment) = 0x00;
	return o_psValue;
}


BOOL CUtil::GetNextConfigData(char* pzCnfgFileNm, char* pzSectionNm, char* pzPrevKeyNm, char* o_pzNextKeyNm, char* o_pzNextValue)
{
	char zTemp[1024] = { 0, };
	DWORD dwRet;

	dwRet = GetPrivateProfileString(pzSectionNm, NULL, NULL, zTemp, sizeof(zTemp), (LPCSTR)pzCnfgFileNm);
	if (dwRet <= 0)
		return FALSE;

	std::list<std::string> listKey;
	SplitDataEx(zTemp, NULL, dwRet, &listKey);
	if (listKey.empty())
		return FALSE;

	// 최초 조회
	if (pzPrevKeyNm[0] == NULL) {
		// 첫번째 key 와 value 반환
		strcpy(o_pzNextKeyNm, (*listKey.begin()).c_str());
		listKey.pop_front();
		GetPrivateProfileString(pzSectionNm, o_pzNextKeyNm, NULL, o_pzNextValue, sizeof(zTemp), (LPCSTR)pzCnfgFileNm);
		return TRUE;
	}
	else
	{
		std::list<std::string>::iterator it;
		for (it = listKey.begin(); it != listKey.end(); it++)
		{
			strcpy(zTemp, (*it).c_str());
			if (strcmp(zTemp, pzPrevKeyNm) == 0)
			{
				it++;
				if (it == listKey.end())
					return FALSE;

				strcpy(o_pzNextKeyNm, (*it).c_str());
				GetPrivateProfileString(pzSectionNm, o_pzNextKeyNm, NULL, o_pzNextValue, sizeof(zTemp), (LPCSTR)pzCnfgFileNm);
				return TRUE;
			}
		}
	}
	return FALSE;
}
	




int CUtil::GetTickGap(double dFirstPrc, double dSndPrc, int nDotCnt, double dTickSize)
{
	double dPow = pow(10., (double)nDotCnt);

	dFirstPrc = dFirstPrc * dPow;
	dSndPrc = dSndPrc * dPow;
	int nGapTick = 0;
	nGapTick = (int)((dFirstPrc - dSndPrc) / dTickSize / dPow);
	return nGapTick;
}

double CUtil::GetPrcByTick(char* pzOrigPrc, double dTickCnt, double dTickSize, char cPlusMinus)
{
	double dRsltPrc = atof(pzOrigPrc);
	if(cPlusMinus=='+') dRsltPrc += dTickCnt* dTickSize;
	else				dRsltPrc -= dTickCnt* dTickSize;

	return dRsltPrc;
}

/*
#define	FORMAT_PRC(prc,dotcnt,out) { sprintf(out, "%0*.*f", LEN_PRC, dotcnt, prc); }
*/
int CUtil::CompPrc(const char* pPrc1, const int nLen1, const char* pPrc2, const int nLen2, const int nDotCnt, const int nFormatLen)
{
	char zPrc1[32], zPrc2[32];
	sprintf(zPrc1, "%.*s", nLen1, pPrc1);
	sprintf(zPrc1, "%0*.*f", nFormatLen, nDotCnt, atof(zPrc1));

	sprintf(zPrc2, "%.*s", nLen2, pPrc2);
	sprintf(zPrc2, "%0*.*f", nFormatLen, nDotCnt, atof(zPrc2));
	
	return strncmp(zPrc1, zPrc2, nFormatLen);

}

int CUtil::CompPrc(const double pPrc1, const double pPrc2, const int nDotCnt, const int nFormatLen)
{
	char zPrc1[32], zPrc2[32];
	sprintf(zPrc1, "%0*.*f", nFormatLen, nDotCnt, pPrc1);

	sprintf(zPrc2, "%0*.*f", nFormatLen, nDotCnt, pPrc2);

	return strncmp(zPrc1, zPrc2, nFormatLen);

}


BOOL CUtil::IsSamePrice( char* pPrc1, int nLen1, char* pPrc2, int nLen2, int nDotCnt, int nFormatLen)
{
	char zPrc1[32], zPrc2[32];
	sprintf(zPrc1, "%.*s", nLen1, pPrc1);
	sprintf(zPrc1, "%0*.*f", nFormatLen, nDotCnt, atof(zPrc1));

	sprintf(zPrc2, "%.*s", nLen2, pPrc2);
	sprintf(zPrc2, "%0*.*f", nFormatLen, nDotCnt, atof(zPrc2));

	return (strncmp(zPrc1, zPrc2, nFormatLen)==0);

}

VOID CUtil::SplitDataEx(_In_ char* psData, _In_ char cDelimeter, _In_ int nSize, _Out_ std::list<std::string>* pListResult)
{
	char pData[1024] = { 0, };
	memcpy(pData, psData, nSize);

	if (cDelimeter == 0x00)
	{
		for (int i = 0; i < nSize; i++)
		{
			if (*(pData + i) == 0x00)
			{
				*(pData + i) = '@';
			}
		}
	}

	SplitData(pData, '@', pListResult);
}

VOID CUtil::SplitData(_In_ char* psData, _In_ char cDelimeter, _Out_ std::list<std::string>* pListResult)
{
	char* pFind;
	char temp[1024];
	char* pData = psData;



	while (TRUE)
	{
		pFind = strchr(pData, cDelimeter);

		// 123/456
		if (!pFind)
		{
			if (strlen(pData) > 0)
			{
				std::string sData = temp;
				pListResult->push_back(sData);
			}
			break;
		}
			

		// list 에 넣는다. ( 123/456/)
		sprintf(temp, "%.*s", strlen(pData) - strlen(pFind), pData);
		std::string sData = temp;
		pListResult->push_back(sData);
		
		// '/' 다음에 데이터가 없으면
		// 123/456/
		if (strlen(pFind) == 1)
			break;

		pData = pFind + 1;
	}
}



VOID	DumpErr(char* pzSrc, int nErr, char* pzMsg)
{
	LPVOID lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	sprintf(pzMsg, "[%s:%d]%s", pzSrc, nErr, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}



char* GetCnfgValue(char* i_psCnfgFileNm, char* i_psSectionNm, char* i_psKeyNm, char* o_psValue)
{
	*o_psValue = 0x00;
	DWORD dwRET = GetPrivateProfileString(i_psSectionNm, i_psKeyNm, NULL, o_psValue, 1024, (LPCSTR)i_psCnfgFileNm);
	// 주석은 제거
	char* pComment = strstr(o_psValue, "//");
	if (pComment)
		*(pComment) = 0x00;
	return o_psValue;
}