#pragma once

class CSymbolPosition
{
public:
	CSymbolPosition();
	~CSymbolPosition();

public:
	char m_zArtcCd[32];
	char m_zSymbol[32];
	char m_zName[128];
	double m_dTickSize;
	double m_dTickValue[32];
	int m_nDotCnt;
	int m_nMaxCntSL;
	int m_nMaxCntPT;
	char m_cBsTp;
	double m_dQty;
	char m_zEndTM[32];

	

private:
	char m_OpenPrc[32];
	char m_zEntrPrc[32];
	char m_zClrPrc[32];
	char m_zPL[32];

};

