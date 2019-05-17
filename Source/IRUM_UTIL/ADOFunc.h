// ADOFunc.h: interface for the CADOFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOFUNC_H__2EF3B881_D0C2_40D8_AF30_075F5606803F__INCLUDED_)
#define AFX_ADOFUNC_H__2EF3B881_D0C2_40D8_AF30_075F5606803F__INCLUDED_


#pragma warning (disable:4530)
#pragma warning(disable : 4146)
#pragma warning(disable:4996)
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "ADOEOF") 
#include <set>

#define LEN_CMDBUFF 4096

#define DBPoolPtr(x) ((CDBPoolAdo*)##x##)

class CADOFunc  
{
public:
	CADOFunc();
	virtual ~CADOFunc();

//	BOOL Execute(char * szCmdText, VARIANT vtParam);
	//BOOL Init(BOOL bMSSQL = TRUE);
	BOOL Open(char *pzServerIP, char *pzUser, char *pzPassword, char* pzDBName);
	//BOOL DB_Open();

	void Close();
	void Destroy();
	char* GetError();
	long ExecSP(char *pszFormat, ...);
	long ExecSP(char *pszFormat, char *pszRecordArray);
	BOOL QrySelect(char* pzQry);	//일반 쿼리문 실행해서 레코드셋 반환
	BOOL ExecQuery(char* pzQry);	//일반 쿼리문 실행.
	BOOL AddParam(char *pName, enum DataTypeEnum eType, enum ParameterDirectionEnum eDirection, LONG lSize, VARIANT vtValue);
	BOOL DelParam(LPCTSTR pName);
	BOOL DelParam(LONG lIndex);
	BOOL DelParamAll();
	BOOL SetParamValue(LONG lIndex, VARIANT vtValue);
	BOOL SetParamValue(LPCTSTR pName, VARIANT vtValue);
	long GetParamCount();
	VARIANT GetParamValue(LONG lParam);
	VARIANT GetParamValue(LPCTSTR pszParam);
	long GetRecordCount();
	VARIANT GetValue(LONG lField);
	LPSTR GetStr(int lField, char* pzOut) { strcpy(pzOut, (LPCSTR)(_bstr_t)GetValue((LONG)lField)); return pzOut; }
	long GetLong(int lField){ return (long)(_variant_t)GetValue((LONG)lField); }
	double GetDbl(int lField) { return (double)(_variant_t)GetValue((LONG)lField); }
	double GetDouble(int lField) { return GetDbl(lField); }

	VARIANT GetValue(LPCTSTR pszField);
	BOOL GetValueEx(LPCTSTR pszField, VARIANT* pRet);
	//VOID GetValueEx2(LPCTSTR pszField, VARIANT* pRet);
	char* GetStr(LPCTSTR pszField, char* pzOut);// { strcpy(pzOut, (LPCSTR)(_bstr_t)GetValue(pszField)); return pzOut; }
	char* GetStrWithLen(LPCTSTR pszField, int nMaxLen, char* pzOut);
	int GetStrEx(LPCTSTR pszField, char* pzOut, int * pnLen);// { strcpy(pzOut, (LPCSTR)(_bstr_t)GetValue(pszField)); return pzOut; }
	long GetLong(LPCTSTR pszField) { return (long)(_variant_t)GetValue(pszField); }
	double GetDbl(LPCTSTR pszField) { return (double)(_variant_t)GetValue(pszField); }
	double GetDouble(LPCTSTR pszField) { return GetDbl(pszField); }

	BOOL GetRows(VARIANT *pRecordArray);
	BOOL IsEOF();
	BOOL IsNextRow();
	void Next();
	BOOL GetErrFlag();

protected:
	BOOL Execute();
	void DumpError(_com_error &e, char *szBuffer);

	_ConnectionPtr	m_pConn;
	_CommandPtr		m_pCmd;
	_RecordsetPtr	m_pRs;
	char			*m_pzCmdBuffer;
	char			m_szMessage[1024];
	BOOL			m_bErrFlag;
	char			m_zConnStr[1024];
};


class CDBPoolAdo
{
public:
	CDBPoolAdo(char *pzServerIP, char *zUserID, char *zPassword, char* zDBName);
	virtual ~CDBPoolAdo();

	BOOL Init(int nInitCnt);
	INT Release(CADOFunc* p);
	CADOFunc* GetAvailableDB();
	CADOFunc* GetAvailableAdo(CADOFunc* p);
	CDBPoolAdo* Get() { return this; };
	char* GetMsg() { return m_zMsg; };
	INT Available();
private:
	VOID LOCK() { EnterCriticalSection(&m_cs); };
	VOID UNLOCK() { LeaveCriticalSection(&m_cs); };
	BOOL AddNew();
	INT Del(CADOFunc* p);
	VOID Destroy();
private:
	int m_nNowCnt;
	CRITICAL_SECTION m_cs;
	std::set<CADOFunc*>	m_setDB;
	char	m_zMsg[1024];
	char	m_zServer[128], m_zID[128], m_zPwd[128], m_zDBName[128];
	//sprintf(connStr, "DRIVER={SQL SERVER};Server=%s;Database=KRHedge;User ID=%s;Password=%s",ip, id, pwd);
};


class CDBHandlerAdo
{
public:
	CDBHandlerAdo(CDBPoolAdo* p);
	virtual ~CDBHandlerAdo();

	CADOFunc* operator ->();

private:
	CDBPoolAdo	*m_pPool;
	CADOFunc	*m_pAdo;
};

#endif // !defined(AFX_ADOFUNC_H__2EF3B881_D0C2_40D8_AF30_075F5606803F__INCLUDED_)
