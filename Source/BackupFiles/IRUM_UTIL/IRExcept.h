#pragma once
#pragma warning(disable:4996)

#define ERR_BASE		90000
#define ERR_COMM_INIT	ERR_BASE + 1

class CIRExcept
{
public:
	CIRExcept(int nError);
	CIRExcept(char* pzMsg);
	virtual ~CIRExcept();

	char* GetMsg() { return m_zMsg; }
	const char* GetCodeMsg(int nErr);

private:
	int		m_nErrNo;
	char	m_zMsg[1024];
};