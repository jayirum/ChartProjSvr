#ifndef	__SCREWDB_H_20100614
#define __SCREWDB_H_20100614

#pragma warning(disable:4996)
#pragma comment(lib, "odbc32.lib")

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <set>


//#define DllExport	__declspec( dllexport)

class ScrewDB;

class dbColumn {
protected:
	char szColumnName[50];
	long lLength;
	long lRealLength;
	int iIndex;
	HSTMT m_hStmt;
public:
	friend ScrewDB;
	dbColumn(char *szColumnName, int iLength, int iIndex);
	virtual ~dbColumn();

	virtual int GetInt();
	virtual long GetLong() = 0;
	virtual float GetFloat();
	virtual double GetDouble() = 0;
	virtual char *Get() = 0;

protected:
	virtual void Bind(HSTMT m_hStmt);
};


class ColString : public dbColumn {
protected:
	char *pData;
public:
	ColString(char *szColumnName, int iLength, int iIndex);
	virtual ~ColString();

	virtual long GetLong();
	virtual double GetDouble();
	virtual char *Get();
protected:
	virtual void Bind(HSTMT m_hStmt);
};




class ColLong : public dbColumn {
protected:
	long lData;
	char szData[35];
public:
	ColLong(char *szColumnName, int iLength, int iIndex);
	virtual ~ColLong();

	virtual long GetLong();
	virtual double GetDouble();
	virtual char *Get();
protected:
	virtual void Bind(HSTMT m_hStmt);
};





class ColDouble : public dbColumn {
protected:
	double dfData;
	char szData[35];
public:
	ColDouble(char *szColumnName, int iLength, int iIndex);
	virtual ~ColDouble();

	virtual long GetLong();
	virtual double GetDouble();
	virtual char *Get();
protected:
	virtual void Bind(HSTMT m_hStmt);
};



	

typedef struct _tagColumn {
	dbColumn *pColumn;
	struct _tagColumn *pNext, *pPrev;
} DBCOLUMN;

typedef struct _tagPreValue {
	char szColumn[51];
	char szValue[258];
	struct _tagPreValue *pNext, *pPrev;
} PREVALUE;

class ScrewDB {
//class DllExport ScrewDB {
protected:
	HENV		m_hEnv;
	HDBC		m_hDbc;
	HSTMT		m_hStmt;
	DBCOLUMN	*pHead, *pTail;
	PREVALUE	*pPreValueHead, *pPreValueTail;
	BOOL		bDebug;
	char		szLastError[512];
	static BOOL bSuccessCheck;
	char		szErrorState[255];
	BOOL		m_bUseODBC;
public:
	ScrewDB(BOOL bUseODBC=FALSE);
	virtual ~ScrewDB();

	void SetDebug(BOOL);

	BOOL Init(char *szDataSourceName, char *szUserID, char *szPassword, char* szDBName, BOOL bAutoCommit = TRUE);
	//BOOL init(char *szDataSourceName, char *szUserID, char *szPassword, BOOL bAutoCommit = TRUE) {
	//	return Init(szDataSourceName, szUserID, szPassword, bAutoCommit);
	//}
	void Free();
	//void free() {
	//	Free();
	//}

	BOOL Cmd(char *, ...);
	BOOL cmd(char *, ...);

	BOOL Exec();
	////BOOL exec() {
	//	return Exec();
	//}

	BOOL Trans(BOOL bCommit);
	//BOOL trans(BOOL bCommit) {
	//	return Trans(bCommit);
	//}

	BOOL NextRow();
	//BOOL nextRow() {
	//	return NextRow();
	//}

	BOOL Close();
	//BOOL close() {
	//	return Close();
	//}

	void Pre(char *szColumnName, char *szValue);
	//void pre(char *szColumnName, char *szValue) {
	//	Pre(szColumnName, szValue);
	//}

	void Pre(char *szColumnName, long lValue);
	//void pre(char *szColumnName, long lValue) {
	//	Pre(szColumnName, lValue);
	//}

	void Pre(char *szColumnName, double dfValue);
	//void pre(char *szColumnName, double dfValue) {
	//	Pre(szColumnName, dfValue);
	//}

	void Pren(char *szColumnName, char *szValue);
	//void pren(char *szColumnName, char *szValue) {
	//	Pre(szColumnName, szValue);
	//}

	void Pren(char *szColumnName, long lValue);
	//void pren(char *szColumnName, long lValue) {
	//	Pren(szColumnName, lValue);
	//}

	void Pren(char *szColumnName, double dfValue);
	//void pren(char *szColumnName, double dfValue) {
	//	Pren(szColumnName, dfValue);
	//}

	BOOL InsExec(char *szTableName);
	//BOOL insExec(char *szTableName) {
	//	return InsExec(szTableName);
	//}

	BOOL UpdateExec(char *szTableName, char *szCondition);
	//BOOL updateExec(char *szTableName, char *szCondition) {
	//	return UpdateExec(szTableName, szCondition);
	//}
	

	int GetInt(char *szColumnName);
	//int getInt(char *szColumnName) {
	//	return GetInt(szColumnName);
	//}

	long GetLong(char *szColumnName);
	//long getLong(char *szColumnName) {
	//	return GetLong(szColumnName);
	//}

	float GetFloat(char *szColumnName);
	//float getFloat(char *szColumnName) {
	//	return GetFloat(szColumnName);
	//}

	double GetDouble(char *szColumnName);
	//double getDouble(char *szColumnName) {
	//	return GetDouble(szColumnName);
	//}

	char *Get(char *szColumnName);
	//char *get(char *szColumnName) {
	//	return Get(szColumnName);
	//}
	char* Get( char* szColumName, char* psOut );

	// 1 Based Index ....
	int GetInt(int iIndex);
	//int getInt(int iIndex) {
	//	return GetInt(iIndex);
	//}

	long GetLong(int iIndex);
	//long getLong(int iIndex) {
	//	return GetLong(iIndex);
	//}

	float GetFloat(int iIndex);
	//float getFloat(int iIndex) {
	//	return GetFloat(iIndex);
	//}

	double GetDouble(int iIndex);
	//double getDouble(int iIndex) {
	//	return GetDouble(iIndex);
	//}

	char *Get(int iIndex);
	//char *get(int iIndex) {
	//	return Get(iIndex);
	//}

	char *GetLastError() {
		return szLastError;
	}

	char *GetErrorState() {
		return szErrorState;
	}

	static void SuccessCheck(BOOL bSuccessCheck);

protected:
	dbColumn *SearchColumn(char *szColumnName);
	dbColumn *SearchColumn(int iIndex);
	void AppendColumn(dbColumn *pdbColumn);
	void AppendPreValue(char *szColumn, char *szValue);
	void ErrorDisplay(char *szError);
	void ODBCErrorDisplay();
	void KillAllColumns();
	void KillAllPreValues();
};



class CDBPool
{
public:
	CDBPool(BOOL bODBCUse, char *zDataSourceName, char *zUserID, char *zPassword, char* zDBName);
	virtual ~CDBPool();

	BOOL Init(int nInitCnt );
	INT Release(ScrewDB* p);
	ScrewDB* GetAvailableDB();
	CDBPool* Get() { return this; };
	char* GetMsg() { return m_zMsg; };
	INT Available();
private:
	VOID LOCK() { EnterCriticalSection(&m_cs); };
	VOID UNLOCK(){ LeaveCriticalSection(&m_cs); };
	BOOL AddNew();
	INT Del(ScrewDB* p);
	VOID Destroy();
private:
	BOOL	m_bODBCUse;
	int m_nNowCnt;
	CRITICAL_SECTION m_cs;
	std::set<ScrewDB*>	m_setDB;
	char	m_zMsg[1024];
	char	m_zServer[128], m_zID[128], m_zPwd[128], m_zDBName[128];
	//sprintf(connStr, "DRIVER={SQL SERVER};Server=%s;Database=KRHedge;User ID=%s;Password=%s",ip, id, pwd);
};


class CDBHandler
{
public:
	CDBHandler(CDBPool* p);
	virtual ~CDBHandler();

	ScrewDB* operator ->();

private:
	CDBPool	*m_pPool;
	ScrewDB	*m_pDB;
};


#endif