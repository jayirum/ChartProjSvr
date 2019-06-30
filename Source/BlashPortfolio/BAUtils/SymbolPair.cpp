#include "SymbolPair.h"



CSymbolPair::CSymbolPair()
{
}


CSymbolPair::~CSymbolPair()
{
}



void	CSymbolPair::Add(string sMasterSymbol, string sSlaveSymbol)
{
	m_mapPair[sMasterSymbol] = sSlaveSymbol;
}

bool	CSymbolPair::Get(_In_ string sMasterSymbol, char* _Out_ pzSlaveSymbol)
{
	map<string, string>::iterator it = m_mapPair.find(sMasterSymbol);

	if (it == m_mapPair.end())
		return false;

	strcpy(pzSlaveSymbol, (*it).second.c_str());
	return true;
}