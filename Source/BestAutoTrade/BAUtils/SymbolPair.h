#pragma once


#pragma warning(disable:4996)

#include "BAUtils.h"
#include <map>
#include <string>
using namespace std;

class CSymbolPair
{
public:
	CSymbolPair();
	~CSymbolPair();

	void	Add(string sMasterSymbol, string sSlaveSymbol);
	bool	Get(_In_ string sMasterSymbol, char* _Out_ pzSlaveSymbol);

private:
	map<string, string>	m_mapPair;
};

