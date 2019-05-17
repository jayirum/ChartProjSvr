/*
	C/C++ -> 일반 -> 추가 포함 디렉터리 : 
		C:\poco-1.7.8\Data\include;
		C:\poco-1.7.8\Data\ODBC\include;
		C:\poco-1.7.8\Foundation\include;

	VC++ 디렉토리 -> 라이브러리 디렉터리
		C:\poco-1.7.8\lib

	Link : PocoFoundationd.lib
*/
#pragma once

#include <string>
#include <sstream>
#include <Poco/Data/RecordSet.h>		//C:\poco-1.7.8\Data\include
#include <Poco/Data/Session.h>			//C:\poco-1.7.8\Data\include
#include <Poco/Data/ODBC/Connector.h>	//C:\poco-1.7.8\Data\ODBC\include
#include <Poco/Data/SessionFactory.h>	//C:\poco-1.7.8\Data\include\Poco\Data

using namespace Poco::Data;

class CPocoODBC
{
public:
	CPocoODBC();
	~CPocoODBC();
	
	BOOL	Connect(char* pzDbIp, char* pzDbName, char* pzUserId, char* pzPassword);
	VOID	Disconnect();
	BOOL	ExecQueryWithRs(char* pzQ);
	BOOL	ExecQueryWithRs(std::string sQ);

	BOOL	GetValue(char* pzFieldName, char* pResult);
	BOOL	GetValue(char* pzFieldName, long* pResult);
	BOOL	GetValue(char* pzFieldName, double* pResult);

	BOOL	MoveNext();
	//inline BOOL	HasNextRow() { return m_bHasNext; }
	inline char*	getmsg() { return m_zMsg; }

private:
	std::string		m_sConnStr;
	std::string		m_sDbIp, m_sDbName, m_sUserId, m_sPassword;
	Session			*m_ses;
	RecordSet		*m_rs;
	Statement		*m_stat;
	char			m_zMsg[1024];
	BOOL			m_bCalledFirstRow;
};