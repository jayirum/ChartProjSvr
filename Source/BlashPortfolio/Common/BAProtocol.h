#pragma once
#pragma warning(disable:4996)
#include "BARelayInc.h"



#include <string>
#include <map>
using namespace std;


class CProtoSet
{
public:
	CProtoSet();
	~CProtoSet();

	void Begin();
	int Complete(/*out*/string& result);
	int Complete(/*out*/char* pzResult);

	void CopyFromRecvData(char* pzData);
	void SetVal(int nFd, string val);
	void SetVal(int nFd, char* val);
	void SetVal(int nFd, int val);
	void SetVal(int nFd, double val);
private:
	void DelSameField(int nFd);
private:
	string	m_sBuf;
	char	m_zTemp[MAX_BUF];
};

struct ST_VAL
{
	string	sVal;
	int		nVal;
	double	dVal;
};

class CProtoGet
{
public:
	CProtoGet();
	~CProtoGet();

	int		Parsing(_In_  const char* pData, int nDataLen);
	char*	GetRecvData() { return m_zBuf; }
	bool	GetCode(_Out_ string& sCode);
	bool	GetVal(int nFd, _Out_ char* pzVal);
	bool	GetVal(int nFd, _Out_ string* psVal);
	bool	GetVal(int nFd, _Out_ int* pnVal);
	bool	GetVal(int nFd, _Out_ double* pdVal);

private:
	void SetValByField(char* pzFd, char* pzVal, ST_VAL* pVal);
	bool IsParsed();
private:
	char				m_zBuf[MAX_BUF];
	map<int, ST_VAL>	m_mapResult;
};

