/////////////////////////////////////////////////////////////////////////////
// (주) 에스엔뱅크
// 2001. 6. 1	Made by Park. Jun-Young
/////////////////////////////////////////////////////////////////////////////
// ODBCTestDlg.cpp : implementation file
//
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "screwdb.h"

//#define DllImport	__declspec( dllimport )
//#define DllExport	__declspec( dllexport )


dbColumn::dbColumn(char *szColumnName, int lLength, int iIndex) {
	lstrcpy(dbColumn::szColumnName, szColumnName);
	dbColumn::lLength = lLength;
	dbColumn::iIndex = iIndex;
}

dbColumn::~dbColumn() {}

int dbColumn::GetInt() {
	return (int)GetLong();
}

float dbColumn::GetFloat() {
	return (float)GetDouble();
}

void dbColumn::Bind(HSTMT m_hStmt) {
	dbColumn::m_hStmt = m_hStmt;
}




ColString::ColString(char *szColumnName, int lLength, int iIndex) 
			: dbColumn(szColumnName, lLength, iIndex) {}

ColString::~ColString() {
	GlobalFree(pData);
}

void ColString::Bind(HSTMT m_hStmt) {
	dbColumn::Bind(m_hStmt);
	pData = (char *)GlobalAlloc(GPTR, lLength + 10);
	lRealLength = lLength;
	SQLBindCol(m_hStmt, iIndex, SQL_C_CHAR, pData, lLength + 10, (long *)&lRealLength);
}

long ColString::GetLong() {
	return atol(pData);
}

double ColString::GetDouble() {
	return atof(pData);
}

char *ColString::Get() {
	return pData;
}






ColLong::ColLong(char *szColumnName, int lLength, int iIndex)
			:dbColumn(szColumnName, lLength, iIndex) {}

ColLong::~ColLong() {}

void ColLong::Bind(HSTMT m_hStmt) {
	dbColumn::Bind(m_hStmt);
	lRealLength = lLength;
	SQLBindCol(m_hStmt, iIndex, SQL_C_SLONG, &lData, 0, (long *)&lRealLength);
}

long ColLong::GetLong() {
	return lData;
}

double ColLong::GetDouble() {	
	return (double)lData;
}
	
char *ColLong::Get() {
	sprintf_s(szData, "%ld", lData);
	return szData;
}








ColDouble::ColDouble(char *szColumnName, int lLength, int iIndex)
	: dbColumn(szColumnName, lLength, iIndex) {}

ColDouble::~ColDouble() {}

void ColDouble::Bind(HSTMT m_hStmt) {
	dbColumn::Bind(m_hStmt);
	lRealLength = lLength;
	SQLBindCol(m_hStmt, iIndex, SQL_C_DOUBLE, &dfData, 0, (long *)&lRealLength);
}

long ColDouble::GetLong() {
	return (long)dfData;
}

double ColDouble::GetDouble() {
	return dfData;
}

char *ColDouble::Get() {
	sprintf_s(szData, "%.3lf", dfData);
	return szData;
}


BOOL ScrewDB::bSuccessCheck = FALSE;

ScrewDB::ScrewDB(BOOL bUseODBC) {
	pHead = pTail = NULL;
	pPreValueHead = pPreValueTail = NULL;
	m_hEnv = NULL;
	m_hDbc = NULL;
	m_hStmt = NULL;
	bDebug = FALSE;
	memset(szLastError, 0x00, sizeof(szLastError) );
	memset(szErrorState, 0x00, sizeof(szErrorState) );
	m_bUseODBC = bUseODBC;
}

ScrewDB::~ScrewDB() {

	KillAllColumns();
	KillAllPreValues();

	Free();
}

void ScrewDB::SetDebug(BOOL bDebug) {
	ScrewDB::bDebug = bDebug;
}

dbColumn *ScrewDB::SearchColumn(char *szColumnName) {
	DBCOLUMN *p = pHead;
	do {
		if(!p) break;
		if(strcmp(p->pColumn->szColumnName, szColumnName) == 0) 
			return p->pColumn;
		p = p->pNext;
	} while(TRUE);
	return NULL;
}

dbColumn *ScrewDB::SearchColumn(int iIndex) {
	DBCOLUMN *p = pHead;
	do {
		if(!p) break;
		if(p->pColumn->iIndex == iIndex) 
			return p->pColumn;
		p = p->pNext;
	} while(TRUE);
	return NULL;
}

int ScrewDB::GetInt(char *szColumnName) {
	dbColumn *p = SearchColumn(szColumnName);

	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%s] no exist!", szColumnName);
		ErrorDisplay(szBuf);
		return 0;
	}
	return p->GetInt();
}

long ScrewDB::GetLong(char *szColumnName) {
	dbColumn *p = SearchColumn(szColumnName);

	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%s] no exist!", szColumnName);
		ErrorDisplay(szBuf);
		return 0L;
	}
	return p->GetLong();
}

float ScrewDB::GetFloat(char *szColumnName) {
	dbColumn *p = SearchColumn(szColumnName);
	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%s] no exist!", szColumnName);
		ErrorDisplay(szBuf);
		return (float)0;
	}
	return p->GetFloat();
}

double ScrewDB::GetDouble(char *szColumnName) {
	dbColumn *p = SearchColumn(szColumnName);
	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%s] no exist!", szColumnName);
		ErrorDisplay(szBuf);
		return (double)0;
	}
	return p->GetDouble();
}

char *ScrewDB::Get(char *szColumnName) {
	static char ret[32]={0,};

	dbColumn *p = SearchColumn(szColumnName);
	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%s] no exist!", szColumnName);
		ErrorDisplay(szBuf);
		return ret;
	}
	return p->Get();
}

char* ScrewDB::Get( char* szColumnName, char* psOut )
{
	dbColumn *p = SearchColumn(szColumnName);
	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%s] no exist!", szColumnName);
		ErrorDisplay(szBuf);
		*psOut = 0x00;
		return psOut;
	}
	lstrcpy( psOut,  p->Get() );
	return psOut;
}








int ScrewDB::GetInt(int iIndex) {
	dbColumn *p = SearchColumn(iIndex);

	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%d] no exist!", iIndex);
		ErrorDisplay(szBuf);
		return 0;
	}
	return p->GetInt();
}

long ScrewDB::GetLong(int iIndex) {
	dbColumn *p = SearchColumn(iIndex);

	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%d] no exist!", iIndex);
		ErrorDisplay(szBuf);
		return 0L;
	}
	return p->GetLong();
}

float ScrewDB::GetFloat(int iIndex) {
	dbColumn *p = SearchColumn(iIndex);
	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%d] no exist!", iIndex);
		ErrorDisplay(szBuf);
		return (float)0;
	}
	return p->GetFloat();
}

double ScrewDB::GetDouble(int iIndex) {
	dbColumn *p = SearchColumn(iIndex);
	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%d] no exist!", iIndex);
		ErrorDisplay(szBuf);
		return (double)0;
	}
	return p->GetDouble();
}

char *ScrewDB::Get(int iIndex) {
	dbColumn *p = SearchColumn(iIndex);
	if(!p) {
		char szBuf[126];
		sprintf_s(szBuf, "dbColumn[%d] no exist!", iIndex);
		ErrorDisplay(szBuf);
		return 0L;
	}
	return p->Get();
}






void ScrewDB::KillAllColumns() {
	DBCOLUMN *p = pHead;
	do {
		if(!p) break;
		pHead = p->pNext;
		delete p->pColumn;
		delete p;
		p = pHead;
	} while(TRUE);
	pHead = pTail = NULL;
}

void ScrewDB::KillAllPreValues() {
	PREVALUE *p = pPreValueHead;
	do {
		if(!p) break;
		pPreValueHead = p->pNext;
		delete p;
		p = pPreValueHead;
	} while(TRUE);
	pPreValueHead = pPreValueTail = NULL;
}


void ScrewDB::AppendColumn(dbColumn *pColumn) {
	DBCOLUMN *p = (DBCOLUMN *)new DBCOLUMN;
	p->pColumn = pColumn;
	pColumn->Bind(m_hStmt);
	if(!pHead) {
		pHead = pTail = p;
		p->pNext = p->pPrev = NULL;
	}
	else {
		p->pPrev = pTail;
		pTail->pNext = p;
		p->pNext = NULL;
		pTail = p;
	}
}	

void ScrewDB::AppendPreValue(char *szColumn, char *szValue) {
	PREVALUE *p = new PREVALUE;
	lstrcpy(p->szColumn, szColumn);
	lstrcpy(p->szValue, szValue);
	if(!pPreValueHead) {
		pPreValueHead = pPreValueTail = p;
		p->pNext = p->pPrev = NULL;
	}
	else {
		p->pPrev = pPreValueTail;
		pPreValueTail->pNext = p;
		p->pNext = NULL;
		pPreValueTail = p;
	}
}

void ScrewDB::ErrorDisplay(char *szError) {
	UCHAR szSqlState[256];
	UCHAR szErrorMsg[256];
	SQLINTEGER fNativeError;
	SQLSMALLINT iError;

	memset	(szSqlState,0x00,sizeof(szSqlState));
	memset	(szErrorState,0x00,sizeof(szErrorState));

	SQLError(m_hEnv, m_hDbc, m_hStmt, szSqlState, &fNativeError, szErrorMsg, 256, &iError);

	lstrcpy(szErrorState, (char *)szSqlState);

	if(bSuccessCheck) {
		MessageBeep(MB_ICONHAND);
		::MessageBox(NULL, szError, "Error - ScrewDB", MB_ICONHAND | MB_OK);
	}
	lstrcpy(szLastError, szError);
}

void ScrewDB::ODBCErrorDisplay() {
	UCHAR szSqlState[256];
	UCHAR szErrorMsg[256];
	SQLINTEGER fNativeError;
	SQLSMALLINT iError;

	memset	(szSqlState,0x00,sizeof(szSqlState));
	memset	(szErrorState,0x00,sizeof(szErrorState));

	SQLError(m_hEnv, m_hDbc, m_hStmt, szSqlState, &fNativeError, szErrorMsg, 256, &iError);
	lstrcpy(szErrorState, (char *)szSqlState);

	if(bSuccessCheck) {
		MessageBeep(MB_ICONHAND);
		::MessageBox(NULL, (char *)szErrorMsg, "Error - ScrewDB", MB_ICONHAND | MB_OK);
	}
	lstrcpy(szLastError, (char *)szErrorMsg);
}

	
/*
	1. ODBC 를 사용하는 경우 (m_bUserODBC==TRUE)
		szDataSourceName - ODBC 명
		szDBName - NULL

	2. ODBC 를 사용하지 않는 경우
		szDataSourceName - SRVER 명 (IP)
		szDBName - DB 명
*/
BOOL ScrewDB::Init(char *szDataSourceName, char *szUserID, char *szPassword, char* szDBName,  BOOL bAutoCommit) 
{
      /*Allocate environment handle */
	int iResult = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	if(iResult == SQL_ERROR) {
		ErrorDisplay("Allocation environment!");
		return FALSE;
	}
   /* Set the ODBC version environment attribute */
   iResult = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 

   int iTimeOut=1;
   SQLSetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, &iTimeOut, 0);
   iResult = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 


   /* Connect to data source */
   if (m_bUseODBC)
   {
	   iResult = SQLConnect(m_hDbc,
		   (SQLCHAR*)szDataSourceName, SQL_NTS,
		   (SQLCHAR*)szUserID, SQL_NTS,
		   (SQLCHAR*)szPassword, SQL_NTS);
	   if (iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) {
		   ErrorDisplay("Connection error!");
		   return FALSE;
	   }
   }
   else
   {
	   char zConnStr[1024];
	   sprintf(zConnStr, "DRIVER={SQL SERVER};Server=%s;Database=%s;User ID=%s;Password=%s",
		   szDataSourceName, szDBName, szUserID, szPassword);
	   int len = strlen(zConnStr);
	   iResult = SQLDriverConnect(m_hDbc,
		   NULL,
		   (SQLCHAR*)zConnStr,
		   SQL_NTS,
		   (SQLCHAR*)NULL,
		   (SQLSMALLINT)0,
		   (SQLSMALLINT*)NULL,
		   (SQLSMALLINT)SQL_DRIVER_NOPROMPT);
	   if (iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) {
		   ErrorDisplay("Connection error!");
		   return FALSE;
	   }
   }
	return TRUE;

}

BOOL ScrewDB::Cmd(char *szFormat, ...) {
	KillAllPreValues();
	KillAllColumns();
	char szBuffer[4096];
	va_list marker;

	va_start( marker, szFormat );     /* Initialize variable arguments. */
	vsprintf_s( szBuffer, szFormat, marker);
	va_end( marker );              /* Reset variable arguments.      */

	if(bDebug) 
		::MessageBox(NULL, szBuffer, "Query Debug", MB_ICONINFORMATION | MB_OK);

    int iResult;
	/* Allocate statement handle */
	iResult = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt); 
	// Specify that hstmt1 is to be executed asynchronously.
	iResult = SQL_ASYNC_ENABLE_ON;
	SQLSetStmtAttr(m_hStmt, SQL_ATTR_ASYNC_ENABLE, &iResult, 0);

	iResult = SQLPrepare(m_hStmt, (unsigned char *)szBuffer, strlen(szBuffer));
	if(iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) {
		ODBCErrorDisplay();
		SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
		m_hStmt = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL ScrewDB::cmd(char *szFormat, ...) {
	KillAllPreValues();
	KillAllColumns();
	char szBuffer[4096];
	va_list marker;

	va_start( marker, szFormat );     /* Initialize variable arguments. */
	vsprintf_s( szBuffer, szFormat, marker);
	va_end( marker );              /* Reset variable arguments.      */

	if(bDebug) 
		::MessageBox(NULL, szBuffer, "Query Debug", MB_ICONINFORMATION | MB_OK);
   
    int iResult;
	/* Allocate statement handle */
	iResult = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt); 
	// Specify that hstmt1 is to be executed asynchronously.
	iResult = SQL_ASYNC_ENABLE_ON;
	SQLSetStmtAttr(m_hStmt, SQL_ATTR_ASYNC_ENABLE, &iResult, 0);

	iResult = SQLPrepare(m_hStmt, (unsigned char *)szBuffer, strlen(szBuffer));
	if(iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) {
		ODBCErrorDisplay();
//		ErrorDisplay("Execution failure!");
		SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
		m_hStmt = NULL;
		return FALSE;
	}
	return TRUE;
}


BOOL ScrewDB::Exec() {
	int iResult = SQLExecute(m_hStmt);
	if(iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) {
		ODBCErrorDisplay();
		SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
		m_hStmt = NULL;
		return FALSE;
	}

	KillAllPreValues();
	KillAllColumns();

	long lColumnCount;
	int rgbDesc;
	short siDescMax = 1, siDesc;
	iResult = SQLColAttribute(m_hStmt, 0, SQL_DESC_COUNT, &rgbDesc, siDescMax, &siDesc, &lColumnCount);
	if(iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) {	
		SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
		m_hStmt = NULL;
		return TRUE;
	}

	char szColumnName[51];
	long lColumnType, lColumnLength;
	short siColumnNameLength;
	for(int iCount = 1; iCount <= (int)lColumnCount; iCount++) {
		iResult = SQLColAttribute(m_hStmt, iCount, SQL_DESC_NAME, szColumnName, sizeof(szColumnName), &siColumnNameLength, NULL);
		iResult = SQLColAttribute(m_hStmt, iCount, SQL_DESC_TYPE, NULL, 0, NULL, &lColumnType);
		iResult = SQLColAttribute(m_hStmt, iCount, SQL_DESC_LENGTH, NULL, 0, NULL, &lColumnLength);

		switch(lColumnType) {
			case SQL_BINARY:
			case SQL_VARBINARY:
			case SQL_LONGVARBINARY:
			case SQL_CHAR:
			case SQL_VARCHAR:
			case SQL_LONGVARCHAR: 
				AppendColumn(new ColString(szColumnName, lColumnLength, iCount));
				break;
			
			case SQL_NUMERIC:
			case SQL_SMALLINT:
			case SQL_INTEGER:
			case SQL_BIT:
			case SQL_TINYINT:
			case SQL_BIGINT:
				AppendColumn(new ColLong(szColumnName, lColumnLength, iCount));
				break;
			
			case SQL_REAL:
			case SQL_DECIMAL:
			case SQL_FLOAT:
			case SQL_DOUBLE:
				AppendColumn(new ColDouble(szColumnName, lColumnLength, iCount));
				break;
							
			case SQL_DATE:
				AppendColumn(new ColString(szColumnName, lColumnLength, iCount));
				break;

			case SQL_TIME:
				AppendColumn(new ColString(szColumnName, lColumnLength, iCount));
				break;

			case SQL_TIMESTAMP:
				AppendColumn(new ColString(szColumnName, lColumnLength, iCount));
				break;
		}
	}

	return TRUE;
}




BOOL ScrewDB::Trans(BOOL bCommit) {
	int iResult;
	switch(bCommit) {
		case TRUE:
			iResult = SQLEndTran(SQL_HANDLE_ENV  ,m_hEnv,SQL_COMMIT);
			iResult = SQLEndTran(SQL_HANDLE_DBC ,m_hDbc,SQL_COMMIT);
			break;
		case FALSE:
			iResult = SQLEndTran(SQL_HANDLE_ENV  ,m_hEnv,SQL_ROLLBACK);
			iResult = SQLEndTran(SQL_HANDLE_DBC ,m_hDbc,SQL_ROLLBACK);
			break;
	}
	if(iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) return FALSE;

	SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
	m_hStmt = NULL;
	return TRUE;
}

BOOL ScrewDB::NextRow() {
	int iResult = SQLFetch(m_hStmt);
	if(iResult == SQL_NO_DATA_FOUND) {
		KillAllPreValues();
		KillAllColumns();
		SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
		m_hStmt = NULL;
		return FALSE;
	}
	if(iResult == SQL_ERROR || iResult == SQL_INVALID_HANDLE) {
		ErrorDisplay("Fatch error(Some issue)!");
		return FALSE;
	}
	return TRUE;
}

BOOL ScrewDB::Close() {
	KillAllPreValues();
	KillAllColumns();
	if( m_hStmt )	SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
	m_hStmt = NULL;
	return TRUE;
}


void ScrewDB::Free() {
	if(m_hDbc){
		SQLDisconnect(m_hDbc);	
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	}
	m_hDbc = NULL;

	if(m_hEnv)	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	m_hEnv = NULL;
}

void ScrewDB::Pre(char *szColumnName, char *szValue) {
	char szBuf[258];
	sprintf_s(szBuf, "'%s'", szValue);
	AppendPreValue(szColumnName, szBuf);
}	

void ScrewDB::Pre(char *szColumnName, long lValue) {
	char szBuf[258];
	sprintf_s(szBuf, "'%ld'", lValue);
	AppendPreValue(szColumnName, szBuf);
}

void ScrewDB::Pre(char *szColumnName, double dfValue) {
	char szBuf[258];
	sprintf_s(szBuf, "'%lf'", dfValue);
	AppendPreValue(szColumnName, szBuf);
}

void ScrewDB::Pren(char *szColumnName, char *szValue) {
	AppendPreValue(szColumnName, szValue);
}	

void ScrewDB::Pren(char *szColumnName, long lValue) {
	char szBuf[258];
	sprintf_s(szBuf, "%ld", lValue);
	AppendPreValue(szColumnName, szBuf);
}

void ScrewDB::Pren(char *szColumnName, double dfValue) {
	char szBuf[258];
	sprintf_s(szBuf, "%lf", dfValue);
	AppendPreValue(szColumnName, szBuf);
}

BOOL ScrewDB::InsExec(char *szTableName) {
	char szBuf[4096];
	sprintf_s(szBuf, "INSERT INTO %s (", szTableName);
	PREVALUE *p = pPreValueHead;
	do {
		if(!p) break;
		strcat_s(szBuf, p->szColumn);
		strcat_s(szBuf, ",");
		p = p->pNext;
	} while(TRUE);

	szBuf[strlen(szBuf) - 1] = '\0';

	strcat_s(szBuf, ") VALUES (");
	p = pPreValueHead;
	do {
		if(!p) break;
		strcat_s(szBuf, p->szValue);
		strcat_s(szBuf, ",");
		p = p->pNext;
	} while(TRUE);
	
	szBuf[strlen(szBuf) - 1] = '\0';
	strcat_s(szBuf, ")");

	Cmd(szBuf);

	return Exec();
}

BOOL ScrewDB::UpdateExec(char *szTableName, char *szCondition) {
	char szBuf[4096];
	sprintf_s(szBuf, "UPDATE %s SET ", szTableName);

	PREVALUE *p = pPreValueHead;
	do {
		if(!p) break;
		strcat_s(szBuf, p->szColumn);
		strcat_s(szBuf, "=");
		strcat_s(szBuf, p->szValue);
		strcat_s(szBuf, ",");
		p = p->pNext;
	} while(TRUE);
	szBuf[strlen(szBuf) - 1] = '\0';

	strcat_s(szBuf, " WHERE ");
	strcat_s(szBuf, szCondition);


	Cmd(szBuf);
	return Exec();
}
	
void ScrewDB::SuccessCheck(BOOL bSuccessCheck) {
	ScrewDB::bSuccessCheck = bSuccessCheck;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<DBPool>
CDBPool::CDBPool(BOOL bODBCUse, char *zDataSourceName, char *zUserID, char *zPassword, char* zDBName)
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
	m_bODBCUse = bODBCUse;
	strcpy(m_zServer, zDataSourceName);
	strcpy(m_zID, zUserID);
	strcpy(m_zPwd, zPassword);
	strcpy(m_zDBName, zDBName);
}
CDBPool::~CDBPool()
{
	Destroy();
	DeleteCriticalSection(&m_cs);
}



INT CDBPool::Available()
{
	return (m_setDB.size());
}


BOOL CDBPool::Init(int nInitCnt )
{
	if (nInitCnt <= 0)
	{
		sprintf_s(m_zMsg, "DB POOL Count 오류:%d", nInitCnt);
		return FALSE;
	}
	for (int i = 0; i < nInitCnt; i++)
	{
		ScrewDB* p = new ScrewDB(m_bODBCUse);
		if (!p->Init(m_zServer, m_zID, m_zPwd, m_zDBName)) 
		{
			sprintf_s(m_zMsg, "DB init err(SERVER:%s)(DB:%s)(ID:%s)(Pwd:%s)(%s)",
						m_zServer, m_zDBName, m_zID, m_zPwd, p->GetLastError());
			Destroy();
			return FALSE;
		}
		m_setDB.insert(p);
	}
	return TRUE;
}
INT CDBPool::Release(ScrewDB* p)
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


BOOL  CDBPool::AddNew()
{
	ScrewDB* p;
	LOCK();
	
	p = new ScrewDB(m_bODBCUse);
	if (!p->Init(m_zServer, m_zID, m_zPwd, m_zDBName)) 
	{
		sprintf_s(m_zMsg, "DB init err(SERVER:%s)(DB:%s)(ID:%s)(Pwd:%s)(%s)",
			m_zServer, m_zDBName, m_zID, m_zPwd, p->GetLastError());
		delete p;
		UNLOCK();
		return FALSE;
	}
	m_setDB.insert(p);
	
	UNLOCK();
	
	return TRUE;
}

INT  CDBPool::Del(ScrewDB* p)
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
	
VOID  CDBPool::Destroy()
{
	LOCK();
	std::set<ScrewDB*>::iterator it;
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


ScrewDB* CDBPool::GetAvailableDB()
{
	
	if (m_setDB.size() == 0)
	{
		if (!AddNew())
		{
			sprintf(m_zMsg, "POOL 에 남아있는 세션이 없음 ");
			return NULL;
		}
	}

	ScrewDB* p;
	LOCK();
	std::set<ScrewDB*>::iterator it = m_setDB.begin();
	p = (*it);
	m_setDB.erase(it);
	UNLOCK();
	
	return p;//(*it);
}


//<DBPool>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<CDBHandler>
CDBHandler::CDBHandler(CDBPool* p)
{
	m_pPool = NULL;
	m_pDB = NULL;
	m_pPool = p;
	m_pDB = m_pPool->GetAvailableDB();
}

CDBHandler::~CDBHandler()
{
	m_pDB->Close();
	m_pPool->Release(m_pDB);
}


ScrewDB* CDBHandler::operator ->()
{
	return m_pDB;
}




//<CDBHandler>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
