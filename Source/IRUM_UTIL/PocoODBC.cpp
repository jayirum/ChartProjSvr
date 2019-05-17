#include "PocoODBC.h"
#include <Poco/Exception.h>




CPocoODBC::CPocoODBC()
{
	m_ses	= NULL;
	m_rs	= NULL;
	m_stat	= NULL;
}


CPocoODBC::~CPocoODBC()
{
	Disconnect();
}


BOOL CPocoODBC::Connect(char* pzDbIp, char* pzDbName, char* pzUserId, char* pzPassword)
{
	char zConnStr[128];
	
	try
	{
		m_sDbIp = pzDbIp;
		m_sDbName = pzDbName;
		m_sUserId = pzUserId;
		m_sPassword = pzPassword;

		Poco::Data::ODBC::Connector::registerConnector();
		sprintf(zConnStr, "DRIVER=SQL Server;SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;",
			pzDbIp, pzDbName, pzUserId, pzPassword);
		std::string str = zConnStr;

		//Disconnect();

		m_ses = new Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::ODBC::Connector::KEY, str));
	}
	catch (const Poco::Exception& e)
	{
		sprintf(m_zMsg, "Exception while connecting to DB(%s)", e.displayText().c_str());
		return FALSE;
	}
	return TRUE;
}

VOID CPocoODBC::Disconnect()
{
	if (m_ses) {
		m_ses->close();
		delete m_ses;
	}
	Poco::Data::ODBC::Connector::unregisterConnector();
}

BOOL CPocoODBC::ExecQueryWithRs(char* pzQ)
{
	try
	{
		if (!m_stat)
			m_stat = new Statement(*m_ses);
		else
			m_stat->reset(*m_ses);


		*m_stat << pzQ;	// "SELECT * FROM AA_STK_MST";
		m_stat->execute();

		if (!m_rs)
			m_rs = new RecordSet(*m_stat);
		else
			m_rs->reset(*m_stat);

		m_bCalledFirstRow = FALSE;
	}
	catch (const Poco::Exception& e)
	{
		std::cout <<  e.displayText().c_str();
		sprintf(m_zMsg, "Exception while ExecQuery(%s)", e.displayText().c_str());
		return FALSE;

	}
	return TRUE;
}

BOOL CPocoODBC::ExecQueryWithRs(std::string sQ)
{
	try
	{
		if (!m_stat)
			m_stat = new Statement(*m_ses);
		else
			m_stat->reset(*m_ses);


		*m_stat << sQ;	// "SELECT * FROM AA_STK_MST";
		m_stat->execute();

		if (!m_rs)
			m_rs = new RecordSet(*m_stat);
		else
			m_rs->reset(*m_stat);

		m_bCalledFirstRow = FALSE;
	}
	catch (const Poco::Exception& e)
	{
		printf("%s\n", e.displayText().c_str());
		sprintf(m_zMsg, "Exception while ExecQuery(%s)", e.displayText().c_str());
		return FALSE;

	}
	return TRUE;
}


BOOL CPocoODBC::MoveNext()
{
	if (!m_bCalledFirstRow ) {
		m_bCalledFirstRow = TRUE;
		return m_rs->moveFirst();
	}
	return m_rs->moveNext();
}

BOOL CPocoODBC::GetValue(char* pzFieldName, char* pResult)
{
	if (!m_rs) {
		//TODO
		return FALSE;
	}
	if(!m_stat) {
		//TODO
		return FALSE;
	}

	try
	{
		Poco::Dynamic::Var val = m_rs->value(pzFieldName);
		sprintf(pResult, "%s", val.convert<std::string>().c_str());
	}
	catch (const Poco::Exception& e)
	{
		sprintf(m_zMsg, "Exception while GetValStr(%s)", e.displayText().c_str());
		return FALSE;
	}
	return TRUE;
}

BOOL	CPocoODBC::GetValue(char* pzFieldName, long* pResult)
{
	if (!m_rs) {
		//TODO
		return FALSE;
	}
	if (!m_stat) {
		//TODO
		return FALSE;
	}

	try
	{
		Poco::Dynamic::Var val = m_rs->value(pzFieldName);
		*pResult = val.convert<long>();
	}
	catch (const Poco::Exception& e)
	{
		sprintf(m_zMsg, "Exception while GetValLong(%s)", e.displayText().c_str());
		return FALSE;
	}
	return TRUE;
}

BOOL	CPocoODBC::GetValue(char* pzFieldName, double* pResult)
{
	if (!m_rs) {
		//TODO
		return FALSE;
	}
	if (!m_stat) {
		//TODO
		return FALSE;
	}

	try
	{
		Poco::Dynamic::Var val = m_rs->value(pzFieldName);
		*pResult = val.convert<double>();
	}
	catch (const Poco::Exception& e)
	{
		sprintf(m_zMsg, "Exception while GetValDbl(%s)", e.displayText().c_str());
		return FALSE;
	}
	return TRUE;
}