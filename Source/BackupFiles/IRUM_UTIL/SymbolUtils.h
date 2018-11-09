#pragma once

#include<string.h>


class CSymbolUtils
{
public:
	CSymbolUtils();
	virtual ~CSymbolUtils();


	double calc_tick(double dPrcGap);
	double calc_prc_byTick(double dTickGap, double dBasePrc, bool bAdd);
	
	void set_artc(char* p) { strcpy(m_zArtcCd,p); }
	void set_symbol(char* p) { strcpy(m_zSymbol,p); }
	void set_name(char* p) { strcpy(m_zSymbolName,p); }
	void set_ticksize(double d) { m_dTickSize=d; }
	void set_tickval(double d) { m_dTickValue=d; }
	void set_dotcnt(int i) { m_nDotCnt=i; }

	char*	artc() { return m_zArtcCd; }
	char*	symbol() { return m_zSymbol; }
	char*	name() { return m_zSymbolName; }
	double	ticksize() { return m_dTickSize; }
	double	tickval() { return m_dTickValue; }
	int		dotcnt() { return m_nDotCnt; }

private:

	char	m_zArtcCd[32];
	char	m_zSymbol[32];
	char	m_zSymbolName[128];
	double	m_dTickSize;
	double	m_dTickValue;
	int		m_nDotCnt;
};
