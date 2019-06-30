#include "BAProtocol.h"
#include <string.h>
#include <Windows.h>

CProtoSet::CProtoSet()
{}
CProtoSet::~CProtoSet()
{}

void CProtoSet::Begin()
{
	m_sBuf.erase(m_sBuf.begin(), m_sBuf.end());
}
int CProtoSet::Complete(/*out*/string& result)
{
	result = m_sBuf;
	return result.size();
}
int CProtoSet::Complete(/*out*/char* pzResult)
{
	strcpy(pzResult, m_sBuf.c_str());
	return strlen(pzResult);
}


void CProtoSet::CopyFromRecvData(char* pzData)
{
	m_sBuf = pzData;
}

void CProtoSet::DelSameField(int nFd)
{
	sprintf(m_zTemp, "%d=", nFd);
	basic_string <char>::size_type deli;
	basic_string <char>::size_type field = m_sBuf.find(m_zTemp);
	if (field == string::npos)
		return;

	// 제일 첫 데이터
	if (field == 0)
	{
		deli = m_sBuf.find(DEF_DELI, field);
		if (deli != string::npos)
		{
			m_sBuf.erase(field, deli - field + 1);
		}
	}
	else {
		sprintf(m_zTemp, "%c%d=", DEF_DELI, nFd);
		field = m_sBuf.find(m_zTemp);
		if (field == string::npos)
			return;
		deli = m_sBuf.find(0x01, field+1);
		if (deli != string::npos)
			m_sBuf.erase(field, deli - field);
	}
}

void CProtoSet::SetVal(int nFd, string val)
{
	DelSameField(nFd);
	sprintf(m_zTemp, "%d=%s%c", nFd, val.c_str(), DEF_DELI);
	m_sBuf += m_zTemp;
}

void CProtoSet::SetVal(int nFd, char* val)
{
	if (strlen(val) == 0)
		return;
	DelSameField(nFd);
	sprintf(m_zTemp, "%d=%s%c", nFd, val, DEF_DELI);
	m_sBuf += m_zTemp;
}
void CProtoSet::SetVal(int nFd, int val)
{
	DelSameField(nFd);
	sprintf(m_zTemp, "%d=%d%c", nFd, val, DEF_DELI);
	m_sBuf += m_zTemp;
}
void CProtoSet::SetVal(int nFd, double val)
{
	DelSameField(nFd);
	sprintf(m_zTemp, "%d=%.*f%c", nFd,DOT_CNT, val, DEF_DELI);
	m_sBuf += m_zTemp;
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

CProtoGet::CProtoGet()
{}

CProtoGet::~CProtoGet()
{}

void CProtoGet::SetValByField(char* pzFd, char* pzVal, ST_VAL* pVal)
{
	int nFd = atoi(pzFd);
	if (nFd > 0 && nFd < 100)
	{
		pVal->sVal = pzVal;
	}
	else if (nFd >= 100 && nFd < 200)
	{
		pVal->nVal = atoi(pzVal);
	}
	else if (nFd>=200 && nFd<300){
		pVal->dVal = atof(pzVal);
	}
}

int CProtoGet::Parsing(_In_  const char* pData, int nDataLen)
{
	sprintf(m_zBuf, "%.*s",nDataLen, pData);
	m_mapResult.erase(m_mapResult.begin(), m_mapResult.end());
	char temp[1024];
	char zFd[32], zVal[1024];
	char* pEqual;
	char* pFirst = m_zBuf;
	char* pDeli = strchr(pFirst, DEF_DELI);

	// 1=1004/2=2049/
	while (pDeli)
	{
		sprintf(temp, "%.*s", (int)(pDeli - pFirst), pFirst);
		pEqual = strchr(pFirst, '=');

		//1004/2=2004
		if(pEqual>pDeli)
		{
			pFirst = pDeli + 1;
			pDeli = strchr(pFirst, DEF_DELI);
			continue;
		}
		// 1-1004/2=2004
		if (pEqual)
		{
			// 1=1004/2=2004/
			sprintf(zFd, "%.*s", (int)(pEqual - pFirst), pFirst);
			sprintf(zVal, "%.*s", (int)(pDeli - pEqual-1), pEqual + 1);
			ST_VAL stVal; ZeroMemory(&stVal, sizeof(ST_VAL));
			SetValByField(zFd, zVal, &stVal);
			m_mapResult[atoi(zFd)] = stVal;
			//printf("[MAP][%s][%s]\n", zFd, zVal);

			// 다음에 데이터가 없으면 나간다.
			if (strlen(pDeli) == 1)
				break;
			pFirst = pDeli + 1;
			pDeli = strchr(pFirst, DEF_DELI);
		}
		else
		{
			// 2=2049/39876
			break;
		}
	}

	// 2=2004
	if (pDeli==NULL || strlen(pDeli) > 1)
	{
		pEqual = strchr(pFirst, '=');
		if (pEqual)
		{
			sprintf(zFd, "%.*s", (int)(pEqual - pFirst), pFirst);
			sprintf(zVal, "%.*s", strlen(pEqual) - 1, pEqual + 1);
			ST_VAL stVal; ZeroMemory(&stVal, sizeof(ST_VAL));
			SetValByField(zFd, zVal, &stVal);
			m_mapResult[atoi(zFd)] = stVal;
			printf("[MAP][%s][%s]\n", zFd, zVal);
		}
	}
	return m_mapResult.size();
}

bool CProtoGet::IsParsed()
{
	return (m_mapResult.size() > 0);
}

bool CProtoGet::GetCode(_Out_ string& sCode)
{
	if (!IsParsed())
		return false;
	map<int, ST_VAL>::iterator it = m_mapResult.find(FDS_CODE);
	if (it == m_mapResult.end())
		return false;

	sCode = (*it).second.sVal;
	return true;
}


bool CProtoGet::GetVal(int nFd, _Out_ char* pzVal)
{
	if (!IsParsed())
		return false;
	map<int, ST_VAL>::iterator it = m_mapResult.find(nFd);
	if (it == m_mapResult.end())
		return false;

	strcpy(pzVal, (*it).second.sVal.c_str());
	return true;
}

bool CProtoGet::GetVal(int nFd, _Out_ string* psVal)
{
	if (!IsParsed())
		return false;
	map<int, ST_VAL>::iterator it = m_mapResult.find(nFd);
	if (it == m_mapResult.end())
		return false;

	*psVal = (*it).second.sVal;
	return true;
}
bool CProtoGet::GetVal(int nFd, _Out_ int* pnVal)
{
	if (!IsParsed())
		return false;
	map<int, ST_VAL>::iterator it = m_mapResult.find(nFd);
	if (it == m_mapResult.end())
		return false;

	*pnVal = (*it).second.nVal;
	return true;
}
bool CProtoGet::GetVal(int nFd, _Out_ double* pdVal)
{
	if (!IsParsed())
		return false;
	map<int, ST_VAL>::iterator it = m_mapResult.find(nFd);
	if (it == m_mapResult.end())
		return false;

	*pdVal = (*it).second.dVal;
	return true;
}