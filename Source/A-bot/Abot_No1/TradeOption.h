#pragma once

#include "../../IRUM_UTIL/adofunc.h"

class COptBase
{
public:
	COptBase(char* pzConfig);
	~COptBase();

	virtual BOOL	TurnOn(void* p1) = 0;
	void	TurnOff() { m_bOn = FALSE; }

	virtual void	Finalize();
	char*	getmsg() { return m_zMsg; }
	BOOL	ConnectDB();
	BOOL	IsOn() { return m_bOn; }
protected:
	char		m_zConfig[_MAX_PATH];
	BOOL		m_bOn;
	char		m_zMsg[1024];
	CDBPoolAdo	*m_pDBPool;
};



class CCrossOpt : public COptBase
{
public:
	CCrossOpt(char* pzConfig);
	~CCrossOpt();

	virtual BOOL	TurnOn(void* p1);
	BOOL		Is_1MinCandle() { return (m_nCandleMin == 1); }
	BOOL		Is_3MinCandle() { return (m_nCandleMin == 3); }
	BOOL		Is_5MinCandle() { return (m_nCandleMin == 5); }
	int			GetCandleMin() { return m_nCandleMin; }
private:
	int		m_nCandleMin;
};


class CTradeOption
{
public:
	CTradeOption(char* pzConfig);
	~CTradeOption();

	BOOL TurnOn_Cross(char* pzSymbol);
	void TurnOff_Cross();
	
	BOOL IsOn_Cross() { return m_Cross->IsOn(); }

	CCrossOpt* cross() { return m_Cross; }

	char*	getmsg() { return m_zMsg; }
private:
	CCrossOpt	*m_Cross;
	char		m_zMsg[1024];
};

