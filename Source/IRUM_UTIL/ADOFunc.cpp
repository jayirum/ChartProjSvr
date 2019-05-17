// ADOFunc.cpp: implementation of the CADOFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "ADOFunc.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int nDebug = 0;

CADOFunc::CADOFunc()
{
	m_pConn = NULL;
	m_pRs = NULL;
	m_pCmd = NULL;
	m_pzCmdBuffer = new char[LEN_CMDBUFF];
	ZeroMemory(m_pzCmdBuffer, LEN_CMDBUFF);
}

CADOFunc::~CADOFunc()
{
	Destroy();
}


BOOL CADOFunc::Open(char *pzServerIP, char *pzUser, char *pzPassword, char* pzDBName)
{
	_ParameterPtr	pParam;
	m_bErrFlag = FALSE;
	sprintf(m_zConnStr, "Provider=SQLOLEDB.1;Data Source=%s;User Id=%s;Password=%s;Initial Catalog=%s", 
					pzServerIP, pzUser, pzPassword, pzDBName);
	_bstr_t bstrConnStr(m_zConnStr);
	_bstr_t bstrUser("");
	_bstr_t bstrPassword("");

	try
	{
		m_pCmd.CreateInstance(__uuidof(Command));
		m_pCmd->CommandType = adCmdStoredProc;
		m_pCmd->CommandTimeout = 60;

		pParam = m_pCmd->CreateParameter("RetVal", adInteger, adParamReturnValue, 0);
		m_pCmd->Parameters->Append(pParam);
		
		m_pConn.CreateInstance(__uuidof(Connection));
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Mode = adModeShareDenyNone;
		m_pConn->IsolationLevel = adXactIsolated;
		m_pConn->Open(bstrConnStr, bstrUser, bstrPassword, -1);


		m_pCmd->ActiveConnection = m_pConn;
		//		m_pCmd->Parameters->Refresh();
	}
	catch (_com_error &e)
	{
		DumpError(e, (char *)bstrConnStr);
		return FALSE;
	}

	return TRUE;
}

BOOL CADOFunc::QrySelect(char* pzQry)
{
	if (m_pRs != NULL && m_pRs->State != adStateClosed) {
		m_pRs->Close();
		m_pRs = NULL;
	}

	try {
		m_pRs.CreateInstance(__uuidof(Recordset));
		m_pRs->Open(_bstr_t(pzQry), m_pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockReadOnly,
			adCmdText);
	}

	catch (_com_error &e)
	{
		DumpError(e, m_pzCmdBuffer);
		return FALSE;
	}

	return TRUE;
}

/*
	일반 쿼리문 실행.(SP 아님)
*/
BOOL CADOFunc::ExecQuery(char* pzQry)
{
	_variant_t vtEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);
	_variant_t vtEmpty2(DISP_E_PARAMNOTFOUND, VT_ERROR);

	if (m_pRs != NULL && m_pRs->State != adStateClosed) {
		m_pRs->Close();
		m_pRs = NULL;
	}

	_bstr_t bstrCommand(pzQry);
	m_pCmd->CommandText = bstrCommand;

	try
	{
		m_pRs = m_pCmd->Execute(&vtEmpty, &vtEmpty2, adCmdText);
	}
	catch (_com_error &e)
	{
		DumpError(e, m_pzCmdBuffer);
		return FALSE;
	}

	m_bErrFlag = FALSE;
	return TRUE;

	return TRUE;
}

BOOL CADOFunc::AddParam(char *pName, enum DataTypeEnum eType, enum ParameterDirectionEnum eDirection, LONG lSize, VARIANT vtValue)
{
	_ParameterPtr	pParam;
	m_bErrFlag = FALSE;

	try 
	{
		pParam = m_pCmd->CreateParameter(pName, eType, eDirection, lSize, vtValue);
		m_pCmd->Parameters->Append(pParam);
	}
	catch(_com_error &e)
	{
		DumpError(e,m_pzCmdBuffer);
		return FALSE;
	}
	return TRUE;
}

BOOL CADOFunc::DelParam(LPCTSTR pName)
{
	_bstr_t bstrName(pName);

	if(m_pCmd->Parameters->GetItem(bstrName) == NULL)
		return FALSE;

	m_pCmd->Parameters->Delete(bstrName);
	return TRUE;
}

BOOL CADOFunc::DelParam(LONG lIndex)
{
	// 0번 파라미터는 제외
	if(lIndex <= 0 || m_pCmd->Parameters->Count <= lIndex)
		return FALSE;

	m_pCmd->Parameters->Delete(lIndex);
	return TRUE;
}

BOOL CADOFunc::DelParamAll()
{
	try
	{
		// 0번 파라미터는 제외
		while(m_pCmd->Parameters->Count > 1)
			m_pCmd->Parameters->Delete(1L);
	}
	catch(_com_error &e)
	{
		DumpError(e,m_pzCmdBuffer);
		return FALSE;
	}
	return TRUE;
}

BOOL CADOFunc::SetParamValue(LONG lIndex, VARIANT vtValue)
{
	// 0번 파라미터는 제외
	if(lIndex <= 0 || m_pCmd->Parameters->Count <= lIndex)
		return FALSE;

	m_pCmd->Parameters->GetItem(lIndex)->PutValue(vtValue);

	return TRUE;
}

BOOL CADOFunc::SetParamValue(LPCTSTR pName, VARIANT vtValue)
{
	_bstr_t bstrName(pName);
	if(m_pCmd->Parameters->GetItem(bstrName) == NULL)
		return FALSE;

	m_pCmd->Parameters->GetItem(bstrName)->PutValue(vtValue);

	return TRUE;
}

long CADOFunc::GetParamCount()
{
	return m_pCmd->Parameters->Count;
}

long CADOFunc::GetRecordCount()
{
	long lCnt = 0;

	if(m_pRs == NULL)
		lCnt = -1;
	else if(m_pRs->State == adStateClosed)
		lCnt = 0;
	else
		m_pRs->get_RecordCount((long*)&lCnt);

	return lCnt;
}

VARIANT CADOFunc::GetValue(LONG lField)
{
	_variant_t vtVal = m_pRs->Fields->GetItem(lField)->Value;
	if(vtVal.vt == VT_NULL)
		VariantClear(&vtVal);

	return vtVal;
}


//VOID CADOFunc::GetValueEx2(LPCTSTR pszField, VARIANT* pRet)
//{
//	VariantCopy(pRet, &m_pRs->Fields->Item[_bstr_t(pszField)]->Value);
//}

char* CADOFunc::GetStrWithLen(LPCTSTR pszField, int nMaxLen, char* pzOut)
{
	_variant_t vVal;

	try {
		VariantCopy(&vVal, &m_pRs->Fields->Item[_bstr_t(pszField)]->Value);
	}
	//__except (ReportException(GetExceptionCode(), "CADOFunc::GetStrWithLen", pzOut))
	catch(...)
	{
		sprintf(m_szMessage, "GetStrWithLen try catch error");
		return NULL;
	}
	//strcpy(pzOut, (LPCSTR)(_bstr_t)GetValue(pszField)); 
	sprintf(pzOut, "%.*s", nMaxLen, (LPCSTR)(_bstr_t)vVal);
	return pzOut;
}


char* CADOFunc::GetStr(LPCTSTR pszField, char* pzOut)
{
	VARIANT vVal;
	if (!GetValueEx(pszField, &vVal))
		return NULL;

	//strcpy(pzOut, (LPCSTR)(_bstr_t)GetValue(pszField)); 
	strcpy(pzOut, (LPCSTR)(_bstr_t)vVal);
	return pzOut;
}



int CADOFunc::GetStrEx(LPCTSTR pszField, char* pzOut, int * pnLen)
{
	*pnLen = -1;
	__try {
		GetStr(pszField, pzOut);
		*pnLen = strlen(pzOut);
	}
	__except (ReportException(GetExceptionCode(), "CADOFunc::GetStr", pzOut))
	{
	}
	return *pnLen;
}


VARIANT CADOFunc::GetValue(LPCTSTR pszField)
{
	return m_pRs->Fields->Item[_bstr_t(pszField)]->Value;
}


BOOL CADOFunc::GetValueEx(LPCTSTR pszField, VARIANT* pRet)
{
	BOOL bRet = TRUE;
	__try {
		*pRet = GetValue(pszField);
	}
	__except (ReportException(GetExceptionCode(), "CADOFunc::GetValueEx", m_szMessage))
	{
		bRet = FALSE;
	}
	return bRet;
}



BOOL CADOFunc::GetRows(VARIANT *pRecordArray)
{
	_variant_t vtRecord;
	try{
		vtRecord = m_pRs->GetRows(adGetRowsRest);
		VariantCopy(pRecordArray, &vtRecord);
	}
	catch(_com_error &e)
	{
		//VariantClear(pRecordArray);
		//->2004.09.19 김형기 수정
		VariantInit(pRecordArray);
		DumpError(e,m_pzCmdBuffer);
		return FALSE;
	}
	return TRUE;
}
BOOL CADOFunc::IsEOF()
{
	if(m_pRs->State == adStateClosed)
		return TRUE;

	return (BOOL)m_pRs->ADOEOF;
}

BOOL CADOFunc::IsNextRow()
{
	return (!IsEOF());
}

void CADOFunc::Next()
{
	Sleep(1);
	m_pRs->MoveNext();
}

long CADOFunc::ExecSP(char *pszFormat, ...)
{
	va_list argptr;

	va_start(argptr, pszFormat);
	vsprintf(m_pzCmdBuffer, pszFormat, argptr);
	va_end(argptr);

	if(!Execute()) return -1;
	long lRet = (long)(m_pCmd->Parameters->GetItem(0L)->Value);

	return lRet;
}

long CADOFunc::ExecSP(char *pszFormat, char *pszRecordArray)
{
	lstrcpy(m_pzCmdBuffer, pszFormat);

	if(!Execute()) return -1;
	long lRet = (long)(m_pCmd->Parameters->GetItem(0L)->Value);

	try
	{
		long lRecCnt = GetRecordCount();
		if(lRecCnt <= 0)
		{
			lRet = -1;
		}
		else
		{
			pszRecordArray[0] = 0x00;
			for(long k=0; k < lRecCnt; k++)
			{
				for(long i=0; i<m_pRs->Fields->Count; i++)
					lstrcat(pszRecordArray, (LPCSTR)(_bstr_t)GetValue(i));

				m_pRs->MoveNext();
			}
		}
	}
	catch(_com_error &e)
	{
		DumpError(e,m_pzCmdBuffer);
		lRet = -1;
	}

	if(m_pRs != NULL && m_pRs->State != adStateClosed)
		m_pRs->Close();

	m_pRs = NULL;

	return lRet;
}
/*
BOOL CADOFunc::Execute(char * szCmdText, VARIANT vtParam)
{
	if(m_pRs != NULL && m_pRs->State != adStateClosed){
		m_pRs->Close();
		m_pRs = NULL;
	}

	m_lRecCnt = 0;

	try 
	{
		m_pCmd.CreateInstance(__uuidof(Command));
		m_pCmd->ActiveConnection = m_pConn;
		m_pCmd->CommandType = adCmdStoredProc;
		m_pCmd->CommandText = szCmdText;
		m_pCmd->CommandTimeout = 60;
		m_pCmd->Parameters->Refresh();
		m_pCmd->Parameters->GetItem(1L)->put_Value(vtParam);

		m_pRs = m_pCmd->Execute(&_variant_t(DISP_E_PARAMNOTFOUND, VT_ERROR), 
			&_variant_t(DISP_E_PARAMNOTFOUND, VT_ERROR), adCmdUnspecified);
		
		if(m_pRs->State == adStateClosed)
			m_lRecCnt = 0;
		else
			m_lRecCnt = m_pRs->RecordCount;

		VariantCopy(&m_dwRtc, &(m_pCmd->Parameters->GetItem(0L)->Value));
		if(m_pCmd){
			m_pCmd.Release();
			m_pCmd = NULL;
		}
	}
	catch(_com_error &e) 
	{
		lstrcpy(m_pzCmdBuffer, szCmdText);
		DumpError(e,m_pzCmdBuffer);
		return FALSE;
	}

	m_bErrFlag = FALSE;
	return TRUE;
}
*/
BOOL CADOFunc::Execute()
{
	_variant_t vtEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);
	_variant_t vtEmpty2(DISP_E_PARAMNOTFOUND, VT_ERROR);

	if(m_pRs != NULL && m_pRs->State != adStateClosed){
		m_pRs->Close();
		m_pRs = NULL;
	}

	_bstr_t bstrCommand(m_pzCmdBuffer);
	m_pCmd->CommandText = bstrCommand;

	try 
	{
//		m_pRs = m_pCmd->Execute(&vtEmpty, &vtEmpty2, adCmdUnspecified);
		m_pRs = m_pCmd->Execute(&vtEmpty, &vtEmpty2, adCmdStoredProc);
	}
	catch(_com_error &e) 
	{
		DumpError(e,m_pzCmdBuffer);
		return FALSE;
	}

	m_bErrFlag = FALSE;
	return TRUE;
}


void CADOFunc::Close()
{
	if (m_pRs != NULL && m_pRs->State != adStateClosed) {
		m_pRs->Close();
		m_pRs.Release();
		m_pRs = NULL;
	}
}


void CADOFunc::Destroy()
{
	try
	{
		if(m_pRs != NULL && m_pRs->State != adStateClosed){
			m_pRs->Close();
			m_pRs.Release();
			m_pRs = NULL;
		}

		if(m_pConn){
			m_pConn->Close();
			m_pConn.Release();
			m_pConn = NULL;
		}

		delete[] m_pzCmdBuffer;
	}
	catch(_com_error &e)
	{
		DumpError(e,m_pzCmdBuffer);
		m_bErrFlag = TRUE;
	}
}

void CADOFunc::DumpError(_com_error &e,char *szBuffer)
{
	m_bErrFlag = TRUE;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

    ErrorPtr  pErr  = NULL;

	char szConnErr[2048];
	szConnErr[0] = 0x00;
    if( (m_pConn->Errors->Count) > 0)
    {
        pErr = m_pConn->Errors->GetItem((long)(m_pConn->Errors->Count - 1));
		sprintf_s(szConnErr, "%s", (LPCSTR)pErr->Description);
    }

	sprintf_s(m_szMessage,"CONNECTION 객체 에러->%s<- \n DESCRIPTION->%s<- \n DSN/SP명->%s<- \n%s 에러코드->%ld<-",
			szConnErr, (LPCSTR)bstrDescription, szBuffer, (LPCSTR)bstrSource, (long)e.Error());
}

char* CADOFunc::GetError()
{
	return m_szMessage;
}

BOOL CADOFunc::GetErrFlag()
{
	return m_bErrFlag;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<DBPool>
CDBPoolAdo::CDBPoolAdo( char *pzServerIP, char *zUserID, char *zPassword, char* zDBName)
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
	strcpy(m_zServer, pzServerIP);
	strcpy(m_zID, zUserID);
	strcpy(m_zPwd, zPassword);
	strcpy(m_zDBName, zDBName);
	CoInitialize(NULL);
}
CDBPoolAdo::~CDBPoolAdo()
{
	Destroy();
	DeleteCriticalSection(&m_cs);
	CoUninitialize();
}



INT CDBPoolAdo::Available()
{
	return (m_setDB.size());
}


BOOL CDBPoolAdo::Init(int nInitCnt)
{
	if (nInitCnt <= 0)
	{
		sprintf_s(m_zMsg, "DB POOL Count Err:%d", nInitCnt);
		return FALSE;
	}
	for (int i = 0; i < nInitCnt; i++)
	{
		CADOFunc* p = new CADOFunc();
		if (!p->Open(m_zServer, m_zID, m_zPwd, m_zDBName))
		{
			sprintf_s(m_zMsg, "DB init err(SERVER:%s)(DB:%s)(ID:%s)(Pwd:%s)(%s)",
				m_zServer, m_zDBName, m_zID, m_zPwd, p->GetError());
			Destroy();
			return FALSE;
		}
		m_setDB.insert(p);
	}
	return TRUE;
}
INT CDBPoolAdo::Release(CADOFunc* p)
{
	LOCK();
	//	__try
	{
		m_setDB.insert(p);
	}
	//	__finally
	{
		UNLOCK();
	}
	return m_setDB.size();
}


BOOL  CDBPoolAdo::AddNew()
{
	CADOFunc* p;
	LOCK();

	p = new CADOFunc();
	if (!p->Open(m_zServer, m_zID, m_zPwd, m_zDBName))
	{
		sprintf_s(m_zMsg, "DB init err(SERVER:%s)(DB:%s)(ID:%s)(Pwd:%s)(%s)",
			m_zServer, m_zDBName, m_zID, m_zPwd, p->GetError());
		delete p;
		UNLOCK();
		return FALSE;
	}
	m_setDB.insert(p);

	UNLOCK();

	return TRUE;
}

INT  CDBPoolAdo::Del(CADOFunc* p)
{
	LOCK();
	__try
	{
		m_setDB.erase(p);
		delete p;
	}
	__finally
	{
		UNLOCK();
	}
	return m_setDB.size();
}

VOID  CDBPoolAdo::Destroy()
{
	LOCK();
	std::set<CADOFunc*>::iterator it;
	//__try
	{
		for (it = m_setDB.begin(); it != m_setDB.end(); ++it)
		{
			delete (*it);
		}
		m_setDB.clear();
	}
	//__finally
	{
		UNLOCK();
	}

}


CADOFunc* CDBPoolAdo::GetAvailableDB()
{
	if (m_setDB.size() == 0)
	{
		if (!AddNew())
		{
			sprintf(m_zMsg, "POOL 에 남아있는 세션이 없음 ");
			return NULL;
		}
	}

	CADOFunc* p;
	LOCK();
	std::set<CADOFunc*>::iterator it = m_setDB.begin();
	p = (*it);
	m_setDB.erase(it);
	UNLOCK();

	return p;//(*it);
}

CADOFunc* CDBPoolAdo::GetAvailableAdo(CADOFunc* p)
{
	if (m_setDB.size() == 0)
	{
		if (!AddNew())
		{
			sprintf(m_zMsg, "POOL 에 남아있는 세션이 없음 ");
			return NULL;
		}
	}

	LOCK();
	std::set<CADOFunc*>::iterator it = m_setDB.begin();
	p = (*it);
	m_setDB.erase(it);
	UNLOCK();

	return p;//(*it);
}

//<DBPool>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<CDBHandlerAdo>
CDBHandlerAdo::CDBHandlerAdo(CDBPoolAdo* p)
{
	m_pPool = NULL;
	m_pAdo = NULL;
	m_pPool = p;
	m_pAdo = m_pPool->GetAvailableDB();
}

CDBHandlerAdo::~CDBHandlerAdo()
{
	m_pAdo->Close();
	m_pPool->Release(m_pAdo);
}


CADOFunc* CDBHandlerAdo::operator ->()
{
	return m_pAdo;
}




//<CDBHandlerAdo>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
